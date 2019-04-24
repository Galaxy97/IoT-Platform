#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include "Ticker.h"
#include "FS.h"
#include "data_json.h"

String serial_number = "147852";
String token = "963258";

void sendMonitoringDataToServer();
Ticker timer4(sendMonitoringDataToServer, 5000);
ESP8266WebServer HTTP(80);
data_json data;
#include "FileSystem.h"
#include "wifi_connected.h"
#include "requests.h"

void setup(){
  FS_init();
  Serial.begin(115200);
  data.begin();
  if (data.ssid == "default")
  {
    ap_server();
    HTTP.begin();
  }
  else
  {
    if (sta_server())
    {
      HTTP.begin();
      while(!sendParamToServer()) delay(5000);
      timer4.start();
    }
    else
    {
      ap_server();
      HTTP.begin();
    }
  }
}



void loop()
{
  // put your main code here, to run repeatedly:
  HTTP.handleClient();
  timer4.update();
}
