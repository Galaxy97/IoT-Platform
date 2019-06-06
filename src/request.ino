bool registr_device()
{
    yield();
    WiFiClient client;
    HTTPClient http;                                                     //Declare object of class HTTPClient
    http.begin(client, "http://192.168.43.56/api/device/reg");    //Specify request destination
    http.addHeader("Content-Type", "application/x-www-form-urlencoded"); //Specify content-type header
    String data_request = "serial=" + SERIAL_NUMBER;
    data_request += "&token=" + TOKEN;
    data_request += "&wifi=" + String(LAN_SSID);
    data_request += "&ip=" + String(IP_DEVICE);
    Serial.println(data_request);
    int httpCode = http.POST(data_request);
    String payload = http.getString(); //Get the response payload

    Serial.println(httpCode); //Print HTTP return code
    if (httpCode == 200)
    {
        Serial.println(payload); //Print request response payload
        if (payload == "Ok" || payload == "This device alredy registered")
        {
            http.end(); //Close connection
            return true;
        }
        else
            return false;
    }
    else
    {
        return false;
    }
}

void send_monitoring_data_to_server()
{
    yield();
    WiFiClient client;
    HTTPClient http;                                                     //Declare object of class HTTPClient
    http.begin(client, "http://192.168.43.56/api/device/save");   //Specify request destination
    http.addHeader("Content-Type", "application/x-www-form-urlencoded"); //Specify content-type header
    String data = "{\"serial\":" + SERIAL_NUMBER;
    data += ", \"data\":";
    data += read_data_from_eeprom();
    data += "}";

    Serial.println(data);
    int httpCode = http.POST(data);
    if (httpCode == 200)
    {
        Serial.println("CLEAN EEPROM --------------------------------");
        clean_eeprom();
        String payload = http.getString(); //Get the response payload
        Serial.println(httpCode);          //Print HTTP return code
        Serial.println(payload);           //Print request response payload
    }

    http.end(); //Close connection
}

