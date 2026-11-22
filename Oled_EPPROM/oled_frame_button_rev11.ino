#include <EEPROM.h>
#include <SoftwareSerial.h>
SoftwareSerial SoftSerial(5, 6);      // RX, TX

#include <GyverOLED.h>
GyverOLED<SSH1106_128x64> oled;

#include <GyverButton.h>              // Библиотека для кнопок
GButton up(3, LOW_PULL, NORM_OPEN);   // Кнопки
GButton down(3, LOW_PULL, NORM_OPEN);
GButton left(3, LOW_PULL, NORM_OPEN);
GButton right(3, LOW_PULL, NORM_OPEN);
GButton ok(3, LOW_PULL, NORM_OPEN);
GButton back(3, LOW_PULL, NORM_OPEN);

#define ITEMS 5
uint8_t pointer = 1;
bool use_flag = false;
bool increment = false;
uint8_t battery_lvl = 84;

bool ASK_Stay = false;
uint8_t ASK_Freq = 10;
bool OUT_Stay = false;
float VCO_Freq = 9.025;
float ATT_Stay = 3.5;

bool last_ASK_Stay = ASK_Stay;
uint8_t last_ASK_Freq = ASK_Freq;
bool last_OUT_Stay = OUT_Stay;
float last_VCO_Freq = VCO_Freq;
float last_ATT_Stay = ATT_Stay;




void setup() 
{
  pinMode(10, OUTPUT);
  digitalWrite(10, HIGH);
  Serial.begin(9600);
  delay(1500);
  SoftSerial.begin(9600);
  SoftSerial.print("17,35,14,76");

  // Save_ASK_Stay(ASK_Stay);
  // Save_VCO_Freq(VCO_Freq);
  // Save_ASK_Freq(ASK_Freq);
  // Save_ATT_Stay(ATT_Stay);

  ASK_Stay = Get_ASK_Stay();
  VCO_Freq = Get_VCO_Freq();
  ASK_Freq = Get_ASK_Freq();
  ATT_Stay = Get_ATT_Stay();
  OUT_Stay = false;

  last_ASK_Stay = ASK_Stay;
  last_ASK_Freq = ASK_Freq;
  last_OUT_Stay = OUT_Stay;
  last_VCO_Freq = VCO_Freq;
  last_ATT_Stay = ATT_Stay;
  


  // Serial.println(Get_ASK_Stay());
  // Serial.println(Get_VCO_Freq(), 3);
  // Serial.println(Get_ASK_Freq());
  // Serial.println(Get_ATT_Stay(), 1);
  
  oled.init();              // init
  oled.setScale(1);         // font 1
  oled.textMode(BUF_ADD);
  upd_OLED(1);
}

void loop() 
{
  int analog0 = analogRead(A0);
  int analog1 = analogRead(A1);
  up.tick(analog0 < 850 && analog0 > 750);
  left.tick(analog0 < 300 && analog0 > 200);
  down.tick(analog0 < 100);
  ok.tick(analog1 < 850 && analog1 > 750);
  right.tick(analog1 < 300 && analog1 > 200);
  back.tick(analog1 < 100);

  if(up.isClick() && !use_flag)
  {
    if(pointer > 1 && pointer != 4)
      pointer--;
    else
    {
      if(pointer == 1)
        pointer = 3;
      if(pointer == 4)
        pointer = 5;
    }
    upd_OLED(pointer);
  }

  if(left.isClick())
  {
    if(use_flag)
    {
      math(pointer, false, false);
    }
    else
    {
      if(pointer == 4)
        pointer = 1;
      else if(pointer == 5)
        pointer = 3;
    }
    upd_OLED(pointer);
  }
  if(left.isHold())
  {
    if(use_flag)
    {
      math(pointer, false, true);
    }
    else
    {
      if(pointer == 4)
        pointer = 1;
      else if(pointer == 5)
        pointer = 3;
    }
    upd_OLED(pointer);
  }

  if(down.isClick() && !use_flag)
  {
    if(pointer < 5 && pointer != 3)
      pointer++;
    else
    {
      if(pointer == 3)
        pointer = 1;
      if(pointer == 5)
        pointer = 4;
    }

    upd_OLED(pointer);
  }

  if(ok.isClick())
  {
    use_flag = !use_flag;
    upd_OLED(pointer);
    if(!use_flag)
    {
      last_ASK_Stay = ASK_Stay;
      last_ASK_Freq = ASK_Freq;
      last_VCO_Freq = VCO_Freq;
      last_ATT_Stay = ATT_Stay;
      last_OUT_Stay = OUT_Stay;
      
      Save_ASK_Stay(ASK_Stay);
      Save_VCO_Freq(VCO_Freq);
      Save_ASK_Freq(ASK_Freq);
      Save_ATT_Stay(ATT_Stay);

      Send_Data();
    }
  }

  if(right.isClick())
  {
    if(use_flag)
    {
      math(pointer, true, false);
    }
    else
    {
      if(pointer == 1 || pointer == 2)
        pointer = 4;
      else if(pointer == 3)
        pointer = 5;
    }
    upd_OLED(pointer);
  }
  if(right.isHold())
  {
    if(use_flag)
    {
      math(pointer, true, true);
    }
    else
    {
      if(pointer == 1 || pointer == 2)
        pointer = 4;
      else if(pointer == 3)
        pointer = 5;
    }
    upd_OLED(pointer);
  }

  if(back.isClick() && use_flag)
  {
    use_flag = false;
    ASK_Stay = last_ASK_Stay;
    ASK_Freq = last_ASK_Freq;
    VCO_Freq = last_VCO_Freq;
    ATT_Stay = last_ATT_Stay;
    OUT_Stay = last_OUT_Stay;
    upd_OLED(pointer);
  }
}

void math(uint8_t pointer, bool increment, bool step_up)
{
  switch(pointer)
  {    
    case 0:
      break;
    case 1:
      if(increment)
      {
        if(ASK_Freq < 10)
          ASK_Freq += 1;
      }
      else
      {
        if(ASK_Freq > 1)
          ASK_Freq -= 1;
      }
      if(ASK_Freq > 10)
        ASK_Freq = 10;
      if(ASK_Freq < 1)
        ASK_Freq = 1;
      break;
    case 2:
      if(increment)
      {
        if(ATT_Stay < 5)
          if(step_up)
            ATT_Stay += 1;
          else
            ATT_Stay += 0.5;
      }
      else
      {
        if(ATT_Stay > -26.5)
          if(step_up)
            ATT_Stay -= 1;
          else
            ATT_Stay -= 0.5;
      }
      if(ATT_Stay > 5)
        ATT_Stay = 5;
      if(ATT_Stay < -26.5)
        ATT_Stay = -26.5;
      break;
    case 3:
      if(increment)
      {
        if(VCO_Freq < 11.975)
          if(step_up)
            VCO_Freq += 0.5;
          else
            VCO_Freq += 0.025;
      }
      else
      {
        if(VCO_Freq > 8)
          if(step_up)
            VCO_Freq -= 0.25;
          else
            VCO_Freq -= 0.025;
      }
      if(VCO_Freq > 12)
        VCO_Freq = 12;
      if(VCO_Freq < 8)
        VCO_Freq = 8;
      break;
    case 4:
      ASK_Stay = !ASK_Stay;  
      break;
    case 5:
      OUT_Stay = !OUT_Stay;
      break;    
  }
}
void Send_Data_Serial(uint8_t adr, uint16_t value)
{
  SoftSerial.print(adr);
  SoftSerial.print(',');
  SoftSerial.print(value);
  SoftSerial.print(';');
}
void Send_Data()
{
  Send_Data_Serial(0, ASK_Stay);
  Send_Data_Serial(1, ASK_Freq);
  delay(50);
  Send_Data_Serial(2, (63 - int(2 * (ATT_Stay + 26.5))));
  Send_Data_Serial(3, Approximation(VCO_Freq));
  delay(50);
  Send_Data_Serial(4, OUT_Stay);

}