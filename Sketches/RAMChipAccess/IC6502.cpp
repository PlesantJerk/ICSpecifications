#include "IC6502.h"
#include "Arduino.h"

void IC6502::Initialize(IC6502Setup setup)
{
  mSetup = setup;
  CloseCurrentPins();
  for(int i=0;i<16;i++)
    mP_Addr[i] = mSetup.PIN_ADDRESS[i];
  for(int i=0;i<8;i++)
    mP_Data[i] = mSetup.PIN_DATA[i];
  mP_RWB = mSetup.PIN_RWB;
  mP_CLOCK = mSetup.PIN_CLOCK;
  mP_RESET = mSetup.PIN_RESET;  
  SetupCurrentPins();
}

void IC6502::Reset()
{
  digitalWrite(mP_RESET, LOW);
  for(int i=0;i<4;i++)
    Tick();
  digitalWrite(mP_RESET, HIGH);
}

void IC6502::Tick()
{
  while(true)
  {
    mClockState = !mClockState; 
    if (mClockState)
      digitalWrite(mP_CLOCK, HIGH);
    else
      digitalWrite(mP_CLOCK, LOW);  
    delay(1);
    if (mClockState) break;
  }
  UpdateMode();
  if (mIsWritingData)
  {    
    mSetup.ram.WriteAddress(ReadAddress(), ReadData());
  }
  else
  {    
    WriteData(mSetup.ram.ReadAddress(ReadAddress()));
  }

}

void IC6502::UpdateMode()
{
  bool isWriteMode = IsWriteMode();
  if (isWriteMode != mIsWritingData)
  {
    mIsWritingData = isWriteMode;
    int pMode = mIsWritingData ? INPUT : OUTPUT;
      for(int i=0;i<8;i++)
        pinMode(mP_Data[i], pMode);
  }
}

int IC6502::ReadAddress()
{
  int ret = 0;  
  for(int i=15;i>=0;i--)
  {
    ret = (ret << 1) + digitalRead(mP_Addr[i]);
  }
  return ret;
}

int IC6502::ReadData()
{
  int ret = 0;  
  for(int i=7;i>=0;i--)
  {
    ret = (ret << 1) + digitalRead(mP_Data[i]);
  }
  return ret;
}

void IC6502::WriteData(int value)
{
  int test = 1;
  for(int i=0;i<8;i++)
  {
    digitalWrite(mP_Data[i], (value & test) > 0 ? HIGH : LOW);
    test = test << 1;      
  }
}

bool IC6502::IsWriteMode()
{
  return digitalRead(mP_RWB) == LOW;
}

void IC6502::PrintStatus()
{
  char line[256];  
  sprintf(line, "address %04X - R/W: %s - Data: %02X", ReadAddress(), IsWriteMode() ? "W" : "R", ReadData());
  Serial.println(line);
}

IC6502Status IC6502::GetStatus()
{
  IC6502Status ret;
  ret.address = ReadAddress();
  ret.data = ReadData();
  ret.IsWriteOp = IsWriteMode();
  return ret;
}

void IC6502::SetupCurrentPins()
{
  for(int i=0;i<16;i++)
    pinMode(mP_Addr[i], INPUT);
  for(int i=0;i<8;i++)
    pinMode(mP_Data[i], OUTPUT);
  pinMode(mP_CLOCK, OUTPUT);
  pinMode(mP_RESET, OUTPUT);
  pinMode(mP_RWB, INPUT);
  digitalWrite(mP_RESET, HIGH);
  digitalWrite(mP_CLOCK, LOW);
  digitalWrite(mP_Data[0], LOW);
  digitalWrite(mP_Data[1], HIGH);
  digitalWrite(mP_Data[2], LOW);
  digitalWrite(mP_Data[3], HIGH);
  digitalWrite(mP_Data[4], LOW);
  digitalWrite(mP_Data[5], HIGH);
  digitalWrite(mP_Data[6], HIGH);
  digitalWrite(mP_Data[7], HIGH);
}
void IC6502::CloseCurrentPins()
{
  for(int i=0;i<16;i++)
  {
    ClosePinIfNeeded(mP_Addr[i]);
    mP_Addr[i] = 0;
  }
  for(int i=0;i<8;i++)
  {
    ClosePinIfNeeded(mP_Data[i]);
    mP_Data[i] = 0;
  }
  ClosePinIfNeeded(mP_RWB);
  ClosePinIfNeeded(mP_CLOCK);
  ClosePinIfNeeded(mP_RESET);
  mP_RWB = 0;
  mP_CLOCK = 0;
  mP_RESET = 0;
}

void IC6502::ClosePinIfNeeded(int pin)
{
  if (pin > 0)
    pinMode(pin, INPUT);
}