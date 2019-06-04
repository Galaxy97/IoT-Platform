void ap_server_routes()
{
  HTTP.on("/wifi_configs", HTTP_POST, []() {
    Serial.println(HTTP.arg(0));
    StaticJsonDocument<200> doc;
    DeserializationError error = deserializeJson(doc, HTTP.arg(0));
    if (error)
      Serial.println(F("Failed to read file"));

    // data.ssid = HTTP.arg("ssid");
    // data.passwd = HTTP.arg("password");
    String ssid = doc["ssid"] | DEFAULT_SSID;
    String passwd = doc["password"] | "";
    // Serial.println(ssid);
    // Serial.println(passwd);
    writeEEPROM(0,16,strdup((ssid.c_str())));
    writeEEPROM(16,32,strdup((passwd.c_str())));
    HTTP.send(200, "text/plain", "OK");
    delay(250);
    readEEPROM(0,16,LAN_SSID); // зчитання даних з енергонезалежної пам'яті
	  readEEPROM(16,16,LAN_PSWD);
    delay(250);
    ESP.reset();
  });
   HTTP.on("/scan_ssid", HTTP_GET, []() {
     HTTP.send(200, "text/json","{\"wifi\":"+scan_ssid+"}" );
  });
}

void sta_server_routes(){
  HTTP.on("/reset_par", HTTP_GET, []() {
    writeEEPROM(0,16,strdup(("**default_ssid**")));
    writeEEPROM(16,32,strdup(("")));
    Serial.println("clean parametres");
    ESP.reset();
  });

  HTTP.on("/poll_sensors", HTTP_GET, []() {
    polling_sensors();
    HTTP.send(200, "text/html","ok");
  });

  HTTP.on("/start_addres_eeprom", HTTP_GET, []() {
    write_key_EEPROM(100,128);
    HTTP.send(200, "text/html", "ok");
  });
}