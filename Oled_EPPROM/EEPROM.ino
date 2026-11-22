void Save_ASK_Stay(bool ASK_Stay)
{
  EEPROM.put(0, ASK_Stay);
}
bool Get_ASK_Stay()
{
  bool stay;
  EEPROM.get(0, stay);
  return stay;
}

void Save_VCO_Freq(float VCO_freq)
{
  EEPROM.put(1, VCO_freq);
}
float Get_VCO_Freq()
{
  float freq;
  EEPROM.get(1, freq);
  return freq;
}

void Save_ASK_Freq(uint8_t ASK_freq)
{
  EEPROM.put(5, ASK_freq);
}
uint8_t Get_ASK_Freq()
{
  uint8_t freq; 
  EEPROM.get(5, freq);
  return freq;
}

void Save_ATT_Stay(float mask)
{
  EEPROM.put(6, mask);
}
float Get_ATT_Stay()
{ 
  float mask;
  EEPROM.get(6, mask);
  return mask;
}