void upd_OLED(uint8_t pointer)
{
  oled.clear();
  //drawBattery(battery_lvl);
  drawASK(ASK_Stay);
  drawASK_Freq(ASK_Freq);
  drawVCO_Freq(VCO_Freq);
  drawATT_Stay(ATT_Stay);
  drawOUT(OUT_Stay);
  drawFrame();
  switch(pointer)
  {    
    case 0:
      
      break;
    case 1:
      drawSelect(use_flag,  0, 25, 61, 35); //drawASK_Freq
      break;
    case 2:
      drawSelect(use_flag,  0, 39, 61, 49); //drawATT_Stay
      break;
    case 3:
      drawSelect(use_flag,  0, 53, 61, 63); //drawVCO_Freq
      break;
    case 4:
      drawSelect(use_flag, 65, 25, 126, 42); //drawASK
      break;
    case 5:
      drawSelect(use_flag, 65, 46, 126, 63); //drawOUT
      break;    
  }
  oled.update();
}

void drawBattery(byte percent) 
{
  oled.setCursorXY(113, 0);
  oled.drawByte(0b11111110);  
  for (byte i = 0; i < 10; i++) 
  {
    if (i >= percent / 10) 
      oled.drawByte(0b10000010);
    else 
      oled.drawByte(0b11111110);
  }
  oled.drawByte(0b11111110);  
  oled.drawByte(0b00111000);

  if(percent > 99)
    oled.setCursorXY(90, 1);
  else if(percent < 100 && percent > 9)
    oled.setCursorXY(96, 1);
  else if(percent < 10)
    oled.setCursorXY(102, 1);
  oled.print(percent);
  oled.print("%");
}

void drawFrame()
{
  // oled.setCursorXY(1, 1);
  // oled.print(F("NO CONNECTION"));
  oled.setCursorXY(22, 13);
  oled.print(F("UHF_GEN_REV1.2"));

  oled.line(0,   9, 127,  9, OLED_FILL);     //horizontal 1
  oled.line(0,  23, 127, 23, OLED_FILL);   //horizontal 2
  oled.line(0,  37,  62, 37, OLED_FILL);   //horizontal 3
  oled.line(63, 23,  63, 63, OLED_FILL);   //vertical
  oled.line(64, 44, 128, 44, OLED_FILL);   //horizontal 4
  oled.line(0,  51,  62, 51, OLED_FILL);   //horizontal 5
}

void drawASK_Freq(uint8_t freq)
{
  oled.setCursorXY(2, 27);
  oled.print(freq);
  oled.setCursorXY(43, 27);
  oled.print(F("kHz"));
}

void drawATT_Stay(float dbm)
{
  oled.setCursorXY(2, 41);
  oled.print(dbm, 1);
  oled.setCursorXY(43, 41);
  oled.print(F("dBm"));
}

void drawVCO_Freq(float freq)
{
  oled.setCursorXY(2, 55);
  oled.print(freq, 3);
  oled.setCursorXY(43, 55);
  oled.print(F("GHz"));
}

void drawASK(bool ASK_Stay)
{
  oled.setCursorXY(67, 34);
  oled.print(F("ASK"));
  oled.setScale(2);
  if(ASK_Stay)
  {
    oled.setCursorXY(94, 27);
    oled.print(F("ON"));
  }
  else
  {
    oled.setCursorXY(88, 27);
    oled.print(F("OFF"));
  }
  oled.setScale(1);
}

void drawOUT(bool OUT_Stay)
{
  oled.setCursorXY(67, 55);
  oled.print(F("OUT"));
  oled.setScale(2);
  if(OUT_Stay)
  {
    oled.setCursorXY(94, 48);
    oled.print(F("ON"));
  }
  else
  {
    oled.setCursorXY(88, 48);
    oled.print(F("OFF"));
  }
  oled.setScale(1);
}

void drawSelect(bool flag, uint8_t X0, uint8_t Y0, uint8_t X1, uint8_t Y1)
{
  if(flag)
  {
    oled.rect(X0, Y0,     X1, Y1, OLED_STROKE);
  }
  else
  {
    oled.line(X0, Y0, X0 + 5, Y0, OLED_STROKE);
    oled.line(X0, Y0,     X0, Y1, OLED_STROKE);
    oled.line(X0, Y1, X0 + 5, Y1, OLED_STROKE);

    oled.line(X1 - 5, Y1, X1, Y1, OLED_STROKE);
    oled.line(X1,     Y0, X1, Y1, OLED_STROKE);
    oled.line(X1 - 5, Y0, X1, Y0, OLED_STROKE);
  }
}