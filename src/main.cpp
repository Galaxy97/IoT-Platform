#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>

const char *ssid = "server";
const char *passwd = "yeswecan";
String token = "123456"; 
String temperature;
String humidity;
String pressure;
String dust;

ESP8266WebServer server(80);
HTTPClient http;

String index_page(){
  String data;
  data += "<!DOCTYPE html>";
  data += "<html>";
  data += "<head>";
  data += " <title>IoT-Platfom</title>";
  data += "</head>";
  data += "<body align = 'center'>";
  data += "   <h1 align='center'>Welcome</h1><hr>";
  data += "   <p> Temperature is " + temperature + " </p>";
  data += "   <p> Humidity is " + humidity + " </p>";
  data += "   <p> Pressure is " + pressure + " </p>";
  data += "   <p> Dust is " + dust + " </p>";
  data += "</body>";
  data += "</html>";
  return data;
}

void postRequest(){
  HTTPClient http;
  http.begin("http://192.168.43.56/api/data");
  http.addHeader("Content-Type", "application/x-www-form-urlencoded");
  http.POST("temperature="+ temperature +"&humidity=" + humidity + "&pressure=" + pressure + "&co=400&dust=" + dust);
  http.writeToStream(&Serial);
  http.end();
}

void getData(){
  temperature = String(random(10,25));
  humidity = String(random(50,90));
  pressure = String(random(950,1050));
  dust = "0." + String(random(100,800)/100);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Connection to WIFI");
  // ------------------------------------------- connection to wifi
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid,passwd);  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }  
  Serial.print("WiFi connected to ");
  Serial.println(ssid);
  Serial.println(WiFi.localIP());            //ip adress
// ------------------------------------------- end connection to wifi
  server.on("/", []() {
    getData();
    postRequest();
    server.send(200, "text/html", index_page());
    });

  server.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  server.handleClient();
}

