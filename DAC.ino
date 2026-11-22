void Set_VCO_Freq(uint16_t volt)
{
  byte buffer[3];
  buffer[0] = 0b01000000;             //записываем в buffer0 контрольный байт (010-Sets in Write mode)
  buffer[1] = volt >> 4;              //записываем наиболее значимые биты
  buffer[2] = volt << 4;              //записываем наименее значимые биты

  Wire.beginTransmission(MCP4725);    //присоединяемся к шине I2C с MCP4725 с адресом 0x61

  Wire.write(buffer[0]);              //передаем контрольный байт с помощью протокола I2C 
  Wire.write(buffer[1]);              //передаем наиболее значимые биты с помощью протокола I2C 
  Wire.write(buffer[2]);              // передаем наименее значимые биты с помощью протокола I2C
  Wire.endTransmission();             //окончание передачи
}