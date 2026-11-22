#include <EEPROM.h>

#include "GyverTimers.h"
//GTimer_us ASKTimer(US);

#include<Wire.h>                   // Библиотека протокола I2C
#define MCP4725 0x60               // I2C адрес DAC

#include "Parser.h"
#include "AsyncStream.h"  // асинхронное чтение сериал
AsyncStream<50> serial(&Serial, ';');   // указываем обработчик и стоп символ

#define PWR_VCO A0                 // Питание ГУН
#define PWR_AMP A1                 // Питание усилителя
#define LED_PC 3                   // Доп индикация
#define LED_ASK 4

#define ATT05 5                    // Пины Аттенюатора
#define ATT1 6                    
#define ATT2 7    
#define ATT4 8    
#define ATT8 9    
#define ATT16 10     
uint8_t ATT[6] = {ATT05, ATT1, ATT2, ATT4, ATT8, ATT16}; // Массив адресов пинов Аттенюатора
bool Stay_ATT_2[6] = {1, 1, 1, 1, 1, 1};
bool mask_0[6] = {0, 0, 0, 0, 0, 0};
bool mask_1[6] = {1, 1, 1, 1, 1, 1};

uint8_t pass[4] = {17, 35, 14, 76};

bool ASK_stay_pins = true;                               // Состояние пинов ASK
bool ASK_start = true;                                   // ВКЛ/ВЫКЛ ASK
uint8_t ASK_freq = 1;                                    // Частота ASK kHz
uint32_t VCO_freq = 1;                                   // Частота ASK kHz
uint8_t PWR_ON_COUNT;                                    // Счётчик кол-ва включений МК
bool FAST_BOOT = false;
bool BOOT = false;
bool CONNECTION = false;



void setup() 
{
  // Save_PWR_ON_Count(0);
  // Save_Stay_ATT_2(Stay_ATT_2);
  // Save_Stay_ASK(1);
  // Save_ASK_Freq(10);
  // Save_VCO_Freq(1024);

  PWR_ON_COUNT = Get_PWR_ON_Count();             // Подсчёт кол-ва включений МК
  if(PWR_ON_COUNT < 5)
  {
    PWR_ON_COUNT++;
    Save_PWR_ON_Count(PWR_ON_COUNT);
  }
  else
  {
    PWR_ON_COUNT = 0;
    Save_PWR_ON_Count(PWR_ON_COUNT);
    FAST_BOOT = true;
  }
  //Serial.print("PWR_ON_COUNT: ");
  //Serial.println(PWR_ON_COUNT);

  for(uint8_t i = 0; i < 6; i++)
    pinMode(ATT[i], OUTPUT);
  pinMode(PWR_VCO, OUTPUT);
  pinMode(PWR_AMP, OUTPUT);
  pinMode(LED_ASK, OUTPUT);
  pinMode(LED_PC, OUTPUT);

  for(uint8_t i = 0; i < 6; i++)
    digitalWrite(ATT[i], LOW);
  digitalWrite(PWR_VCO, LOW);
  digitalWrite(PWR_AMP, LOW);
  digitalWrite(LED_ASK, LOW);
  digitalWrite(LED_PC, LOW);
  
  Serial.begin(9600);

  Set_ATT(mask_1);                    // Установка MIN выхода ATT
  Wire.begin();                      // Cтарт связи по протоколу I2C
  Set_VCO_Freq(0);                     // Установка MIN выхода DAC
}

uint32_t Timer_Connection = millis();
void loop() 
{
  if(!CONNECTION && !FAST_BOOT && !CONNECTION)             // Проверка подключения к ПК
  {
    if(serial.available())                  // Чтение ответа с ПК
    {
      Save_PWR_ON_Count(0);
      Parser data(serial.buf, ',');
      uint8_t check_pass = 0;
      int16_t get_pass[4];                  // Массив для численных данных
      data.parseInts(get_pass);
      for(uint8_t i = 0; i < 4; i++)        // Проверка пароля
        if(get_pass[i] == pass[i])
          check_pass++;
      if(check_pass == 4)                   // Подключено успешно
      {
        CONNECTION = true;
        //Serial.print("GARANTED");
        BOOT = true;
      } 
      else                                  // Подключено не верно
      {
        check_pass = 0;
        while(1)
        {
          digitalWrite(LED_PC, !digitalRead(LED_PC));
          Serial.print("ERROR");
          delay(125);
        }
      }  
    }

    if((millis() - Timer_Connection > 500) && !CONNECTION) // Запрос к ПК
    {
      Timer_Connection = millis();
      digitalWrite(LED_PC, !digitalRead(LED_PC));
      if(digitalRead(LED_PC))
        Serial.print("UHF_GEN_REV1.1");
    }
  }

  if(FAST_BOOT && !BOOT)                     // Включение без ПК
  {
    BOOT = true;
    FastBootInit();
    while(1){}
  }

  if(!FAST_BOOT && BOOT)                     // Цикл работы от ПК
  {
    delay(100);
    DefaultInit();
    while(1)
    {
      parsing();
      static uint32_t tmr = 0;
      if (millis() - tmr > 100) 
      {
        tmr = millis();
        // Serial.print(7);
        // Serial.print(',');
        // Serial.print(analogRead(POT_1));
      }
    }
  }
}

void parsing() {
  if (serial.available()) {
    Parser data(serial.buf, ',');  // отдаём парсеру
    int ints[10];           // массив для численных данных
    data.parseInts(ints);   // парсим в него

    switch (ints[0]) {
      case 0: 
        if(ints[1] == 1)
        {
          Save_Stay_ASK(true);
          Start_ASK(true);
        }
        else
        {
          Save_Stay_ASK(false);
          Start_ASK(false);
        }
        break;
      case 1:
        Save_ASK_Freq(ints[1]);
        Timer1.setPeriod(500/ints[1]);
        break;
      case 2:
        Save_Stay_ATT_10(ints[1]);
        Get_Stay_ATT_2();
        Set_ATT(Stay_ATT_2);
        break;
      case 3:
        Save_VCO_Freq(ints[1]);
        Set_VCO_Freq(ints[1]);
        break;
      case 4:
        if(ints[1] == 1)
        {
          Enable_Out(true);
        }
        else
        {
          Enable_Out(false);
        }
      break;
    break;
    }
  }
}

void DefaultInit()
{
  digitalWrite(LED_PC, HIGH);
  ASK_start = Get_Stay_ASK();
  Start_ASK(ASK_start);
  ASK_freq = Get_ASK_Freq();         // Чтение из памяти прошлой частоты ASK
  Get_Stay_ATT_2();                  // Чтение из памяти прошлого состояния Аттенюатора
  VCO_freq = Get_VCO_Freq();         // Чтение из памяти прошлой частоты VCO
  Timer1.setPeriod(500/ASK_freq);
  Set_VCO_Freq(VCO_freq);
  
  Serial.print(ASK_start);
  Serial.print(',');
  Serial.print(ASK_freq);
  Serial.print(',');
  Serial.print(Get_Stay_ATT_10());
  Serial.print(',');
  Serial.print(VCO_freq);
}

void FastBootInit()
{
  Serial.print("FAST_BOOT");
  digitalWrite(LED_PC, LOW);
  ASK_start = Get_Stay_ASK();
  ASK_freq = Get_ASK_Freq();         // Чтение из памяти прошлой частоты ASK
  Get_Stay_ATT_2();                  // Чтение из памяти прошлого состояния Аттенюатора
  VCO_freq = Get_VCO_Freq();         // Чтение из памяти прошлой частоты VCO

  Set_ATT(Stay_ATT_2);
  Timer1.setPeriod(500/ASK_freq);
  Set_VCO_Freq(VCO_freq);
  Start_ASK(ASK_start);
  Enable_Out(1);
}

void Enable_Out(bool flag)
{
  if(flag)
  {
    digitalWrite(PWR_VCO, HIGH);
    digitalWrite(PWR_AMP, HIGH);
  }
  else
  {
    digitalWrite(PWR_VCO, LOW);
    digitalWrite(PWR_AMP, LOW);
  }

}

ISR(TIMER1_A)
{
  ASK();
}




