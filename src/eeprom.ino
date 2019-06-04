bool check_of_default_ssid()
{
  if (LAN_SSID[1] == '*' && LAN_SSID[15] == '*')
    return true;
  else
    return false;
}

void writeEEPROM(int startAdr, int laenge, char *writeString)
{
  EEPROM.begin(EEPROM_SIZE); //Max bytes of eeprom to use
  yield();
  Serial.println();
  Serial.print("writing EEPROM: ");
  //write to eeprom
  for (int i = 0; i < laenge; i++)
  {
    EEPROM.write(startAdr + i, writeString[i]);
    Serial.print(writeString[i]);
  }
  EEPROM.commit();
  EEPROM.end();
}

void readEEPROM(int startAdr, int maxLength, char *dest)
{
  EEPROM.begin(EEPROM_SIZE);
  delay(10);
  for (int i = 0; i < maxLength; i++)
  {
    dest[i] = char(EEPROM.read(startAdr + i));
  }
  EEPROM.end();
  Serial.print("ready reading EEPROM:");
  Serial.println(dest);
}

uint16_t read_key_EEPROM(uint16_t addr)
{
  EEPROM.begin(EEPROM_SIZE);
  byte x[2];
  for (byte i = 0; i < 2; i++)
    x[i] = EEPROM.read(i + addr);
  int *y = (int *)&x;
  EEPROM.end();
  return y[0];
}

void write_key_EEPROM(uint16_t addr,uint16_t data)
{
  EEPROM.begin(EEPROM_SIZE);
  byte *x = (byte *)&data;
  for (byte i = 0; i < 2; i++)
    EEPROM.write(i + addr, x[i]);
  EEPROM.commit();
  EEPROM.end();
}

void EEPROM_float_write(uint16_t addr, float val) // запись в ЕЕПРОМ
{
  byte *x = (byte *)&val;
  for (byte i = 0; i < 4; i++)
    EEPROM.write(i + addr, x[i]);
}

float EEPROM_float_read(uint16_t addr) // чтение из ЕЕПРОМ
{
  byte x[4];
  for (byte i = 0; i < 4; i++)
    x[i] = EEPROM.read(i + addr);
  float *y = (float *)&x;
  return y[0];
}
void EEPROM_long_write(uint16_t addr, unsigned long val) // запись в ЕЕПРОМ
{
  EEPROM.begin(EEPROM_SIZE);
  byte *x = (byte *)&val;
  for (byte i = 0; i < 4; i++)
    EEPROM.write(i + addr, x[i]);
  EEPROM.commit();
  EEPROM.end();
}

unsigned long EEPROM_long_read(uint16_t addr) // чтение из ЕЕПРОМ
{
  byte x[4];
  for (byte i = 0; i < 4; i++)
    x[i] = EEPROM.read(i + addr);
  unsigned long *y = (unsigned long *)&x;
  return y[0];
}

String read_data_from_eeprom()
{
  uint16_t addr = read_key_EEPROM(100);
  uint16_t addr_0 = read_key_EEPROM(102);
  Serial.print("addr ");
  Serial.println(addr);
  EEPROM.begin(EEPROM_SIZE);
  delay(10);
  String data_str = "[";

  if(addr_0 != 0){
    while (addr - 20 >= addr_0)
    {
      addr -= 4;
      unsigned long unix_time = EEPROM_long_read(addr);
      Serial.print("unix_time ");
      Serial.println(unix_time);
      data_str += "{\"unix_time\":";
      data_str += String(unix_time);
      data_str += ",";
      addr -= 4;
      float dust = EEPROM_float_read(addr);
      Serial.print("dust ");
      Serial.println(dust);
      data_str += "\"dust\":";
      data_str += String(dust);
      data_str += ",";
      addr -= 4;
      float pres = EEPROM_float_read(addr);
      Serial.print("pres ");
      Serial.println(pres);
      data_str += "\"pres\":";
      data_str += String(pres);
      data_str += ",";
      addr -= 4;
      float hum = EEPROM_float_read(addr);
      Serial.print("hum ");
      Serial.println(hum);
      data_str += "\"hum\":";
      data_str += String(hum);
      data_str += ",";
      addr -= 4;
      float temp = EEPROM_float_read(addr);
      Serial.print("temp ");
      Serial.println(temp);
      data_str += "\"temp\":";
      data_str += String(temp);
      data_str += "}";
      if (addr >= 148)
        data_str += ",";
    }
  write_key_EEPROM(102,0);
  } else
  {
      while (addr - 20 >= 128)
      {
        addr -= 4;
        unsigned long unix_time = EEPROM_long_read(addr);
        Serial.print("unix_time ");
        Serial.println(unix_time);
        data_str += "{\"unix_time\":";
        data_str += String(unix_time);
        data_str += ",";
        addr -= 4;
        float dust = EEPROM_float_read(addr);
        Serial.print("dust ");
        Serial.println(dust);
        data_str += "\"dust\":";
        data_str += String(dust);
        data_str += ",";
        addr -= 4;
        float pres = EEPROM_float_read(addr);
        Serial.print("pres ");
        Serial.println(pres);
        data_str += "\"pres\":";
        data_str += String(pres);
        data_str += ",";
        addr -= 4;
        float hum = EEPROM_float_read(addr);
        Serial.print("hum ");
        Serial.println(hum);
        data_str += "\"hum\":";
        data_str += String(hum);
        data_str += ",";
        addr -= 4;
        float temp = EEPROM_float_read(addr);
        Serial.print("temp ");
        Serial.println(temp);
        data_str += "\"temp\":";
        data_str += String(temp);
        data_str += "}";
        if (addr >= 148)
          data_str += ",";
      }
    }
  data_str += "]";
  EEPROM.end();
  return data_str;
}

void clean_eeprom()
{
  uint16_t addr = read_key_EEPROM(100);
  EEPROM.begin(EEPROM_SIZE);
  for (; addr >= 128; addr--)
  {
    EEPROM.write(addr, (char)30);
    Serial.print(".");
  }  
  EEPROM.commit();
  EEPROM.end();
  delay(10);
  write_key_EEPROM(100,128);
}

void write_data_to_eeprom(float data, uint16_t address)
{
  EEPROM.begin(EEPROM_SIZE);
  yield();
  //EEPROM.write(address, (char)36); // 36 - $
  EEPROM_float_write(address, data);
  EEPROM.commit();
  EEPROM.end();
}