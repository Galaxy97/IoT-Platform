#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include "FS.h"
#include "ArduinoJson.h"

class data_json
{
public:
    String ssid;
    String ip;
    String passwd;
    int freqPollSensors;
    int freqSendToServer;

    void begin()
    {
        load_data_json();
    };

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
            passwd = doc["passwd"] | "";
            ip = doc["ip"] | "";
            freqPollSensors = doc["freqPoll"] | 1;
            freqPollSensors = doc["freqSend"] | 1;
            Serial.println(ssid);
        }
        conf_data.close();
    };

    void write_data_json()
    {
        File conf_data_read = SPIFFS.open("/setting.json", "r");
        if (!conf_data_read)
            Serial.println("Failed to open setting file");
        else
        {
            StaticJsonDocument<200> doc;
            DeserializationError error = deserializeJson(doc, conf_data_read);
            if (error)
                Serial.println(F("Failed to read file"));
            File conf_data_write = SPIFFS.open("/setting.json", "w");
            if(ssid != "defolt") doc["ssid"] = ssid;
            doc["passwd"] = passwd;
            Serial.println(ssid);
            if (serializeJson(doc, conf_data_write) == 0)
            {
                Serial.println(F("Failed to write to file"));
            }
            conf_data_write.close();
        }
        conf_data_read.close();
    };

    void write_scan_ssid(String ssids)
    {
        File data = SPIFFS.open("/scan_ssid.json", "w");
        if (!data)
            Serial.println("Failed to open setting file");
        else
        {
            DynamicJsonDocument doc(1024);
            doc["wifi"] = serialized(ssids);
            if (serializeJsonPretty(doc, data) == 0)
            {
                Serial.println(F("Failed to write to file"));
            }
            data.close();
        }
        data.close();
    };
};
