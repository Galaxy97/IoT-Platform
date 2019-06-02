void polling_sensors()
{
    float data_arr[4] = {bme.readTemperature(), bme.readHumidity(), bme.readPressure() / 100.0F, dust_sensor()};
    unsigned long time_now = now();
    Serial.println(time_now);
    uint8_t addr = read_one_EEPROM(100);
    if (addr >= 234 )
    {
         write_one_EEPROM(100, 128);
    }
    
    for (size_t i = 0; i < 4; i++)
    {
        write_data_to_eeprom(data_arr[i], addr);
        addr += 4;
    }
    delay(5);
    EEPROM_long_write(addr,time_now);
    addr += 4;
    write_one_EEPROM(100, addr);
    Serial.print("write to eeprom 100: ");
    Serial.println(addr);
}

float dust_sensor()
{
    digitalWrite(ledPin, LOW); // power on the LED
    delayMicroseconds(280);
    float voltsMeasured = ads.readADC_SingleEnded(0); // read the dust value
    delayMicroseconds(40);
    digitalWrite(ledPin, HIGH); // turn the LED off
    delayMicroseconds(9680);
    //measure your 5v and change below
    float calcVoltage = (voltsMeasured * 0.1875) / 1000;
    return (0.17 * calcVoltage);
}