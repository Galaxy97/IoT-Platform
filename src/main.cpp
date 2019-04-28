#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
<<<<<<< HEAD
#include "Ticker.h"
#include "FS.h"
#include "data_json.h"


String serial_number = "147852";
String token = "963258";

void sendMonitoringDataToServer();
Ticker timer4(sendMonitoringDataToServer, 10000);
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
=======
#include "FS.h"
#include "data_json.h"
ESP8266WebServer HTTP(80);
data_json data;
#include "FileSystem.h"

void setup()
{
  FS_init();
  Serial.begin(115200);
  data.begin();
  if (data.ssid == "defolt")
  {
    WiFi.mode(WIFI_AP_STA);
    Serial.println("Scan wifi");
    int n = WiFi.scanNetworks();
    String SSIDS[n];
    if (n != 0)
    {
      String str = "[";
      for (int i = 0; i < n; i++)
      {
        SSIDS[i] = WiFi.SSID(i);
        Serial.print(SSIDS[i]);
        str += "{ \"ssid\" : " + WiFi.SSID(i) + "}";
        if(i != n-1) str +=",";
      }
      str += "]";
      data.write_scan_ssid(str);
    }
    WiFi.mode(WIFI_AP);
    WiFi.softAP(data.ssid);

    HTTP.on("/wifi_configs",HTTP_POST,[](){
      data.ssid =  HTTP.arg("ssid");
      data.passwd = HTTP.arg("passwd");
      HTTP.send(200, "text/plain", "OK");
      delay(15);
      ESP.reset();
    });
  }
  else
  {
    WiFi.begin(data.ssid, data.passwd);
    byte tries = 30; // час на підключення до мережі
    while (--tries && WiFi.status() != WL_CONNECTED)
    {
      delay(500);
      Serial.print(".");
    }
    Serial.print("WL connected ");
    Serial.println("SSID is" + data.ssid);
    Serial.print("IP is ");
    Serial.println(WiFi.localIP());
  }

  HTTP.begin();
>>>>>>> 8842e34d02b238984f94af704f33f9b065f4fb4d
}

void loop()
{
  // put your main code here, to run repeatedly:
  HTTP.handleClient();
<<<<<<< HEAD
  timer4.update();
}
=======
}
>>>>>>> 8842e34d02b238984f94af704f33f9b065f4fb4d
