#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

void sendMonitoringDataToServer()
{
    HTTPClient http;                                                  //Declare object of class HTTPClient
    WiFiClient client;                                                //Declare object of class HTTPClien
    http.begin(client,"http://iotplatform.ikto.net/api/save_data");             //Specify request destination
    http.addHeader("Content-Type", "application/x-www-form-urlencoded"); //Specify content-type header
    String data = "serial=1";
    data += "&temp=" + String(random(15, 25));
    data += "&hum=" + String(random(50, 95));
    data += "&pres=" + String(random(950, 1050));
    data += "&dust=0." + String(random(0, 10));
    Serial.println(data);
    int httpCode = http.POST(data);
    String payload = http.getString(); //Get the response payload

    Serial.println(httpCode); //Print HTTP return code
    Serial.println(payload);  //Print request response payload

    http.end(); //Close connection
}

bool sendParamToServer()
{
    HTTPClient http;                                                     //Declare object of class HTTPClient
    WiFiClient client;    
    http.begin(client ,"http://iotplatform.ikto.net/api/registration");             //Specify request destination
    http.addHeader("Content-Type", "application/x-www-form-urlencoded"); //Specify content-type header
    String data_request = "serial=" + serial_number;
    data_request += "&token=" + token;
    data_request += "&wifi=" + data.ssid;
    data_request += "&ip=" + data.ip;
    Serial.println(data_request);
    int httpCode = http.POST(data_request);
    String payload = http.getString(); //Get the response payload

    Serial.println(httpCode); //Print HTTP return code
    Serial.println(payload);  //Print request response payload
    if (payload == "Ok") {
        http.end(); //Close connection
        return true;
    } else return false;    
}