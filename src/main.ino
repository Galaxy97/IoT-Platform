#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include "ArduinoJson.h"
#include "FS.h"
#include "Ticker.h"
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <Adafruit_ADS1015.h>

#define DEFAULT_SSID "**default_ssid**"
#define ledPin 14
String SERIAL_NUMBER = "123";
String TOKEN = "1927";

ESP8266WebServer HTTP(80);
File fsUploadFile;
Adafruit_BME280 bme;
Adafruit_ADS1115 ads;

void send_monitoring_data_to_server();
Ticker timer4(send_monitoring_data_to_server, 10000);

char LAN_SSID[16];
char LAN_PSWD[16];
char IP_DEVICE[16];
String scan_ssid;

void setup()
{
  delay(1000);
  Serial.begin(115200);
  Serial.println("Read parametres from EEPROM");
  readEEPROM(0, 16, LAN_SSID); // зчитання даних з енергонезалежної пам'яті
  readEEPROM(16, 16, LAN_PSWD);
  FS_init();
  if (check_of_default_ssid())
  {
    begin_ap_connection();
    HTTP.begin();
  }
  else if (begin_sta_connection())
  {
    HTTP.begin();
    while (!registr_device())
      delay(5000);
    timer4.start();
    bme.begin(0x76);
    pinMode(ledPin, OUTPUT);
    ads.setGain(GAIN_ONE);
    ads.begin();
  }
  else
  {
    begin_ap_connection();
  }
}

void loop()
{
  // put your main code here, to run repeatedly:
  HTTP.handleClient();
  timer4.update();
}
