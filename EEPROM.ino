void Save_PWR_ON_Count(uint8_t PWR_ON_Count)
{
  EEPROM.put(0, PWR_ON_Count);
}
uint8_t Get_PWR_ON_Count()
{
  uint8_t count;
  EEPROM.get(0, count);
  return count;
}

void Save_Stay_ASK(bool Stay_ASK)
{
  EEPROM.put(1, Stay_ASK);
}
bool Get_Stay_ASK()
{
  bool stay;
  EEPROM.get(1, stay);
  return stay;
}

void Save_VCO_Freq(uint16_t VCO_freq)
{
  EEPROM.put(2, VCO_freq);
}
uint16_t Get_VCO_Freq()
{
  uint16_t freq;
  EEPROM.get(2, freq);
  return freq;
}

void Save_ASK_Freq(uint8_t ASK_freq)
{
  EEPROM.put(4, ASK_freq);
}
uint8_t Get_ASK_Freq()
{
  uint8_t freq; 
  EEPROM.get(4, freq);
  return freq;
}

void Save_Stay_ATT_2(bool mask_2[6])
{
  
  uint8_t mask_10 = 0;
  uint8_t grade;
  for(uint8_t i = 0; i < 6; i++)
  {
    grade = 1;
    for(uint8_t ii = 0; ii < i; ii++)
      grade = grade * 2;

    if(mask_2[i])
      mask_10 += grade;
  }
  EEPROM.put(5, mask_10);
/*  Serial.print("SAVE: ");
  Serial.print(mask_10);
  Serial.print(" ");
  for(uint8_t i = 0; i < 6; i++)
  {
    Serial.print(mask_2[i]);
    Serial.print(" ");
  }
  Serial.println(" ");
*/
}

void Save_Stay_ATT_10(uint8_t mask_10)
{
  EEPROM.put(5, mask_10);
}



void Get_Stay_ATT_2()
{ 
  bool mask_2[6];
  uint8_t mask_10 = 0;
  uint8_t grade; 
  EEPROM.get(5, mask_10);
//  Serial.print("READ: ");
//  Serial.print(mask_10);
  for(int8_t i = 0; i < 6; i++)
  {
    grade = 1;
    for(uint8_t ii = 0; ii < (5 - i); ii++)
      grade = grade * 2;
    if(mask_10 >= grade)
    {
      mask_10 -= grade;
      Stay_ATT_2[5 - i] = 1;
    }
    else
    {
      Stay_ATT_2[5 - i] = 0;
    }   
  }
/*  Serial.print(" ");
  for(uint8_t i = 0; i < 6; i++)
  {
    Serial.print(Stay_ATT[i]);
    Serial.print(" ");
  }
  Serial.println();
*/
}



uint8_t Get_Stay_ATT_10()
{ 
  uint8_t mask_10;
  EEPROM.get(5, mask_10);
  return mask_10;
}
