void ASK()
{
  if(ASK_stay_pins)         //Проверка прошлого состояния пинов
  {
    if(!Stay_ATT_2[0])       //Применяем маску выходной мощности
      PORTB |= (1 << 2);    //Инвертируем состояние с заданной частотой
    if(!Stay_ATT_2[1])
      PORTB |= (1 << 1);       
    if(!Stay_ATT_2[2])
      PORTB |= (1 << 0);        
    if(!Stay_ATT_2[3])       
      PORTD |= (1 << 7);
    if(!Stay_ATT_2[4])       
      PORTD |= (1 << 6);
    if(!Stay_ATT_2[5])       
      PORTD |= (1 << 5);  
  }
  else
  {
    if(!Stay_ATT_2[0])        //Применяем маску выходной мощности
      PORTB &= ~(1 << 2);    //Инвертируем состояние с заданной частотой
    if(!Stay_ATT_2[1])
      PORTB &= ~(1 << 1);          
    if(!Stay_ATT_2[2])        
      PORTB &= ~(1 << 0);
    if(!Stay_ATT_2[3])        
      PORTD &= ~(1 << 7);  
    if(!Stay_ATT_2[4])        
      PORTD &= ~(1 << 6);
    if(!Stay_ATT_2[5])        
      PORTD &= ~(1 << 5);  
  }
  ASK_stay_pins = !ASK_stay_pins;  // Флаг прошлого сост пинов
}

void Set_ATT(bool mask_2[6])
{
  for(uint8_t i = 0; i < 6; i++)
  {
    digitalWrite(ATT[5 - i], mask_2[i]);
  }
}

void Start_ASK(bool flag)
{
  if(flag)
  {
    Timer1.enableISR();
    digitalWrite(LED_ASK, HIGH);
  }
  else
  {
    Timer1.disableISR();
    digitalWrite(LED_ASK, LOW);
  }
}