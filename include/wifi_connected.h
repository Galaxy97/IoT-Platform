#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>

void scanWiFi()
{
  int n = WiFi.scanNetworks();
  String SSIDS[n];
  if (n != 0)
  {
    String str = "[";
    for (int i = 0; i < n; i++)
    {
      Serial.print(SSIDS[i]);
      str += "{";
      str += " \"ssid\" : ";
      str += "\"" + WiFi.SSID(i) + "\",";
      str += " \"power\" : ";
      str += "\"" + String(WiFi.RSSI(i)) + "\",";
      str += " \"open\" : ";
      str += "\"";
      WiFi.encryptionType(i) == ENC_TYPE_NONE ? str += "true" : str += "false";
      str += "\"";
      str += "}";
      if (i != n - 1)
        str += ",";
    }
    str += "]";
    data.write_scan_ssid(str);
    Serial.println(str);
  }
}

void ap_server()
{
  WiFi.mode(WIFI_AP_STA);
  Serial.println("Scan wifi");
  scanWiFi();
  WiFi.mode(WIFI_AP);
  WiFi.softAP("default");

  HTTP.on("/wifi_configs", HTTP_POST, []() {
    data.ssid = HTTP.arg("ssid");
    data.passwd = HTTP.arg("passwd");
    HTTP.send(200, "text/plain", "OK");
    delay(15);
    ESP.reset();
  });
}

bool sta_server()
{
  delay(500);
  Serial.println(data.ssid);
  Serial.println(data.passwd);
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
  IPAddress broadCast = WiFi.localIP();
  data.ip = "";
  for (int i = 0; i < 4; i++)
  {
    data.ip += String(broadCast[i]);
    if (i < 3)
      data.ip += ".";
  }
  
  if (WiFi.status() == WL_CONNECTED){
    Serial.println("Connected succes");
    return true;
  }
  else{
    Serial.println("Connected is NOT succes");
    return false;
  }
}