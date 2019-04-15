#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include "FS.h"

ESP8266WebServer HTTP(80);
#include "FileSystem.h"
#include "data_json.h"

void setup()
{
  Serial.begin(115200);
  data_json data;
  WiFi.begin("Galaxy", "Xiaominote5");  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  FS_init();
  HTTP.begin();
  Serial.println("Server is launch");
  data.load_data_json();
}

void loop()
{
  // put your main code here, to run repeatedly:
  HTTP.handleClient();
}