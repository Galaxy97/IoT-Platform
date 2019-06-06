void begin_ap_connection()
{
  WiFi.mode(WIFI_AP_STA);
  Serial.println("Scan wifi");
  scan_ssid = scanWiFi();
  WiFi.mode(WIFI_AP);
  WiFi.softAP(DEFAULT_SSID);
  ap_server_routes();
}

String scanWiFi()
{
  uint8_t n = WiFi.scanNetworks();
  String str = "[";
  if (n != 0)
  {
    for (int i = 0; i < n; i++)
    {
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
  }
  str += "]";
  Serial.println(str);
  return str;
}

bool begin_sta_connection()
{
  Serial.print(LAN_SSID);
  Serial.print(" ");
  Serial.println(LAN_PSWD);
  WiFi.begin(LAN_SSID,LAN_PSWD);
  byte tries = 30; // час на підключення до мережі
  while (--tries && WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  if (WiFi.status() != WL_CONNECTED)
  {
    Serial.print("WL connected SSID is failed ");
    return false;
  }
  Serial.print("WL connected SSID is ");
  Serial.println(LAN_SSID);
  Serial.print("IP is ");
  Serial.println(WiFi.localIP());
  readEEPROM(32,16,IP_DEVICE); // зчитання попередньої адреси
      IPAddress broadCast = WiFi.localIP();
      String ip;
      for(int i = 0; i < 4; i++) {
        ip += String(broadCast[i]);
        if(i < 3)ip += ".";
      }

      for(int i = 0; i<16;i++) {
        if(ip[i]!= IP_DEVICE[i]) {
          Serial.println("ip is changed");
          writeEEPROM(32,16,strdup(ip.c_str())); // якщо адреса відрізняється перезаписати її
          for(uint8_t i = 0;i<14;i++) IP_DEVICE[i] = ip[i];
          break;
          }
        }    
  sta_server_routes();
  return true;
}