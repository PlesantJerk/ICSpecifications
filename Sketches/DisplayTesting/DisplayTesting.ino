#include <LiquidCrystal.h>
#include "LCD1602.h"

// LCD1602
// https://www.waveshare.com/datasheet/LCD_en_PDF/LCD1602.pdf

LCD1602 lcd;

void setup()
{
  Serial.begin(57600);
  LCD1602Setup setup;
  setup.AssignDataPins(9,10,2,3,4,5,6,7);
  setup.PIN_RS = 13;
  setup.PIN_SIG = 12;
  lcd.Initialize(setup);
  lcd.SendText("Welcome!", true);
}

void loop()
{
  if (Serial.available())
  {
    String text = Serial.readStringUntil("\n");
    text.trim();   
    lcd.SendText(text, true);    
    delay(500);
  }
}
