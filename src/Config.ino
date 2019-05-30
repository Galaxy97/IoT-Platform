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

uint8_t read_one_EEPROM(int startAdr)
{
  EEPROM.begin(EEPROM_SIZE);
  int ch = (int)(EEPROM.read(startAdr));
  EEPROM.end();
  return ch;
}

void write_one_EEPROM(int startAdr, uint8_t data)
{
  EEPROM.begin(EEPROM_SIZE);
  Serial.println(data);
  EEPROM.write(startAdr, data);
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

unsigned long EEPROM_long_read(uint16_t addr) // чтение из ЕЕПРОМ
{
  byte x[4];
  for (byte i = 0; i < 4; i++)
    x[i] = EEPROM.read(i + addr);
  unsigned long *y = (unsigned long *)&x;
  return y[0];
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
void write_long_data_to_eeprom(unsigned long data, uint16_t address)
{
  EEPROM.begin(EEPROM_SIZE);
  yield();
  //EEPROM.write(address, (char)36); // 36 - $
  EEPROM_float_write(address, data);
  EEPROM.commit();
  EEPROM.end();
}


void read_all_eeprom()
{
  Serial.println("Read all data from eeprom");
  EEPROM.begin(EEPROM_SIZE);
  delay(10);
  for (uint16_t i = 0; i < EEPROM_SIZE; i++)
  {
    Serial.print((char)(EEPROM.read(i)));
  }
  EEPROM.end();
}
String s_read_all_eeprom()
{
  String data;
  Serial.println("Read all data from eeprom");
  EEPROM.begin(EEPROM_SIZE);
  delay(10);
  for (uint16_t i = 0; i < EEPROM_SIZE; i++)
  {
    data += (char)(EEPROM.read(i));
    Serial.print((char)(EEPROM.read(i)));
  }
  EEPROM.end();
  return data;
}

void read_data_from_eeprom()
{
  uint8_t addr = read_one_EEPROM(100);
  Serial.print("addr ");
  Serial.println(addr);
  EEPROM.begin(EEPROM_SIZE);
  delay(10);
  while(addr - 20 >= 128){
    addr -= 4;
    unsigned long unix_time = EEPROM_long_read(addr);
    Serial.print("unix_time ");
    Serial.println(unix_time);
    addr -= 4;
    float dust = EEPROM_float_read(addr);
    Serial.print("dust ");
    Serial.println(dust);
    addr -= 4;
    float pres = EEPROM_float_read(addr);
    Serial.print("pres ");
    Serial.println(pres);
    addr -= 4;
    float hum = EEPROM_float_read(addr);
    Serial.print("hum ");
    Serial.println(hum);
    addr -= 4;
    float temp = EEPROM_float_read(addr);
    Serial.print("temp ");
    Serial.println(temp);
  }
  EEPROM.end();
}