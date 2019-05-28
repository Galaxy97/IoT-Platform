bool registr_device()
{
    yield();
    WiFiClient client;
    HTTPClient http;                                                     //Declare object of class HTTPClient
    http.begin(client, "http://iotplatform.ikto.net/api/device/reg");    //Specify request destination
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
    http.begin(client, "http://iotplatform.ikto.net/api/device/save");   //Specify request destination
    http.addHeader("Content-Type", "application/x-www-form-urlencoded"); //Specify content-type header
    String data = "serial=" + SERIAL_NUMBER;
    data += "&temp=" + String(bme.readTemperature());
    data += "&hum=" + String(bme.readHumidity());
    data += "&pres=" + String(bme.readPressure() / 100.0F);
    data += "&dust=" + dust_sensor();
    Serial.println(data);
    int httpCode = http.POST(data);
    if (httpCode == 200)
    {
        String payload = http.getString(); //Get the response payload
        Serial.println(httpCode);          //Print HTTP return code
        Serial.println(payload);           //Print request response payload
    }

    http.end(); //Close connection
}

String dust_sensor()
{
    digitalWrite(ledPin, LOW); // power on the LED
    delayMicroseconds(280);
    float voltsMeasured = ads.readADC_SingleEnded(0); // read the dust value
    delayMicroseconds(40);
    digitalWrite(ledPin, HIGH); // turn the LED off
    delayMicroseconds(9680);
    //measure your 5v and change below
    float calcVoltage = (voltsMeasured * 0.1875) / 1000;
    return String(0.17 * calcVoltage);
}