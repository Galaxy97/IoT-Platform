#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <FS.h>

#include "ap_mode.cpp"

const char *ssid = "server";
const char *passwd = "yeswecan";
String token = "123456"; 
String temperature;
String humidity;
String pressure;
String dust;

ESP8266WebServer server(80);



void postRequest(){
  
}


void setup() {

}

void loop() {
  // put your main code here, to run repeatedly:
  server.handleClient();
}
