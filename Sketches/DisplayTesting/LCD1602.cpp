#include <Arduino.h>
#include "LCD1602.h"


void LCD1602::Initialize(LCD1602Setup setup)
{
  ClearExistingPins();
  for(int i=0;i<8;i++)
    mPinData[i] = setup.PIN_DATA[i];
  mPinRS = setup.PIN_RS;
  mPinSig = setup.PIN_SIG;
  SetupExistingPins();
  InitializeDisplay();

}

void LCD1602::SendText(String text, bool clearDisplay)
{
  if (clearDisplay)
    SendData(1, true);
  for(int c=0;c<text.length();c++)
  {
    int ch = (int)text.charAt(c);     
    SendData(ch, false); 
  }    
}

void LCD1602::InitializeDisplay()
{
  SendData(0b00001111, true);
  SendData(0b00000110, true);
  SendData(0b00110000, true);
}

void LCD1602::SetupExistingPins()
{
  for(int i=0;i<8;i++)
    pinMode(mPinData[i], OUTPUT);
  pinMode(mPinRS, OUTPUT);
  pinMode(mPinSig, OUTPUT);
}

void LCD1602::ClearExistingPins()
{
  for(int i=0;i<8;i++)
  {
    ResetPin(mPinData[i]);
    mPinData[i] = 0;
  }
  ResetPin(mPinRS);
  ResetPin(mPinSig);
  mPinRS = 0;
  mPinSig = 0;
}

void LCD1602::ResetPin(int pin)
{
  if (pin > 0)
    pinMode(pin, INPUT);
}

void LCD1602::SendData(int value, bool isInstruction)
{    
  if (isInstruction)
    digitalWrite(mPinRS, LOW);
  else
    digitalWrite(mPinRS, HIGH);  
  digitalWrite(mPinSig,HIGH);  
  int testValue = 1;
  for(int i=0;i<8;i++)
  {
    digitalWrite(mPinData[i], (value & testValue) > 0 ? HIGH : LOW);    
    testValue = testValue << 1;
  }
  delayMicroseconds(40);
  digitalWrite(mPinSig, LOW);
  delay(2);
}
