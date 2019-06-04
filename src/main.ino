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
#include <WiFiUdp.h>
#include <TimeLib.h>

#define DEFAULT_SSID "**default_ssid**"
#define ledPin 14
#define EEPROM_SIZE 512
String SERIAL_NUMBER = "123";
String TOKEN = "1927";
static const char ntpServerName[] = "ua.pool.ntp.org";

const int timeZone = 3; // Central European Time
WiFiUDP Udp;
unsigned int localPort = 8888; // local port to listen for UDP packets
ESP8266WebServer HTTP(80);
File fsUploadFile;
Adafruit_BME280 bme;
Adafruit_ADS1115 ads;

void send_monitoring_data_to_server();
void polling_sensors();
Ticker timer4(send_monitoring_data_to_server, 60000);
Ticker timer3(polling_sensors,10000);

char LAN_SSID[16];
char LAN_PSWD[16];
char IP_DEVICE[16];
String scan_ssid;
uint16_t eeprom_data_addres = 128;
bool flag = true;

time_t getNtpTime();
String digitalClockDisplay();
void printDigits(int digits, String &time_now);
void sendNTPpacket(IPAddress &address);

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
    timer3.start();
    bme.begin(0x76);
    pinMode(ledPin, OUTPUT);
    ads.setGain(GAIN_ONE);
    ads.begin();
    Udp.begin(localPort);
    Serial.print("Local port: ");
    Serial.println(Udp.localPort());
    Serial.println("waiting for sync");
    setSyncProvider(getNtpTime);
    setSyncInterval(300);
  }
  else
  {
    begin_ap_connection();
    HTTP.begin();
  }
}

time_t prevDisplay = 0; // when the digital clock was displayed

void loop()
{
  // put your main code here, to run repeatedly:
  HTTP.handleClient();
  timer4.update();
  timer3.update();
}

const int NTP_PACKET_SIZE = 48; // NTP time is in the first 48 bytes of message
byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming & outgoing packets

time_t getNtpTime()
{
  IPAddress ntpServerIP; // NTP server's ip address

  while (Udp.parsePacket() > 0) ; // discard any previously received packets
  Serial.println("Transmit NTP Request");
  // get a random server from the pool
  WiFi.hostByName(ntpServerName, ntpServerIP);
  Serial.print(ntpServerName);
  Serial.print(": ");
  Serial.println(ntpServerIP);
  sendNTPpacket(ntpServerIP);
  uint32_t beginWait = millis();
  while (millis() - beginWait < 1500) {
    int size = Udp.parsePacket();
    if (size >= NTP_PACKET_SIZE) {
      Serial.println("Receive NTP Response");
      Udp.read(packetBuffer, NTP_PACKET_SIZE);  // read packet into the buffer
      unsigned long secsSince1900;
      // convert four bytes starting at location 40 to a long integer
      secsSince1900 =  (unsigned long)packetBuffer[40] << 24;
      secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
      secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
      secsSince1900 |= (unsigned long)packetBuffer[43];
      return secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR;
    }
  }
  Serial.println("No NTP Response :-(");
  return 0; // return 0 if unable to get the time
}

// send an NTP request to the time server at the given address
void sendNTPpacket(IPAddress &address)
{
  // set all bytes in the buffer to 0
  memset(packetBuffer, 0, NTP_PACKET_SIZE);
  // Initialize values needed to form NTP request
  // (see URL above for details on the packets)
  packetBuffer[0] = 0b11100011;   // LI, Version, Mode
  packetBuffer[1] = 0;     // Stratum, or type of clock
  packetBuffer[2] = 6;     // Polling Interval
  packetBuffer[3] = 0xEC;  // Peer Clock Precision
  // 8 bytes of zero for Root Delay & Root Dispersion
  packetBuffer[12] = 49;
  packetBuffer[13] = 0x4E;
  packetBuffer[14] = 49;
  packetBuffer[15] = 52;
  // all NTP fields have been given values, now
  // you can send a packet requesting a timestamp:
  Udp.beginPacket(address, 123); //NTP requests are to port 123
  Udp.write(packetBuffer, NTP_PACKET_SIZE);
  Udp.endPacket();
}

String time_now() {
  prevDisplay = now();
  return String( prevDisplay);
}