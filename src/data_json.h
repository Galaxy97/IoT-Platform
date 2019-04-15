#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include "FS.h"
#include "ArduinoJson.h"

class data_json
{
private:
    String ssid;
    String ip;
    int freqPollSensors;
    int freqSendToServer;

public:
    void load_data_json()
    {
        File conf_data = SPIFFS.open("/setting.json", "r");
        if (!conf_data)
            Serial.println("Failed to open setting file");
        else
        {
            StaticJsonDocument<200> doc;
            DeserializationError error = deserializeJson(doc, conf_data);
            if (error)
                Serial.println(F("Failed to read file"));
            ssid = doc["ssid"] | "defolt"; // if doc["ssid"] is none alt "defolt"
            Serial.println(ssid);
        }
        conf_data.close();
    };

    void write_data_json()
    {
        File conf_data = SPIFFS.open("/setting.json", "r+");
        if (!conf_data)
            Serial.println("Failed to open setting file");
        else
        {
            StaticJsonDocument<200> doc;
            DeserializationError error = deserializeJson(doc, conf_data);
            if (error)
                Serial.println(F("Failed to read file"));
            doc["ssid"] = "Dynamo";
            Serial.println(ssid);
            if (serializeJson(doc, conf_data) == 0)
            {
                Serial.println(F("Failed to write to file"));
            }
        }
        conf_data.close();
    };
};
