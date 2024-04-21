#include "Arduino.h"
#include "RAMAccess.h"

RAMAccess::RAMAccess()
{
}

void RAMAccess::SetPins(RAMConfig& config)
{
  if (mPinCS != -1) pinMode(mPinCS, INPUT);
  if (mPinClock != -1) pinMode(mPinClock, INPUT);    
  if (mPinSI != -1) pinMode(mPinSI, INPUT);
  if (mPinSO != -1) pinMode(mPinSO, INPUT);
  mPinClock = config.CLOCK;
  mPinCS = config.CS;
  mPinSI = config.SI;
  mPinSO = config.SO;
  pinMode(mPinCS, OUTPUT);  
  pinMode(mPinClock, OUTPUT);
  pinMode(mPinSI, OUTPUT);
  pinMode(mPinSO, INPUT);

  digitalWrite(mPinClock, LOW);  
  EndCommunication();  
  SetModeSingleByteRW();  
}

void RAMAccess::DelayIt(unsigned int time)
{
  delayMicroseconds(time);
}

void RAMAccess::BeginCommunication()
{
  mHighCount = 0;
  digitalWrite(mPinCS, LOW);
  DelayIt(1);
}

void RAMAccess::EndCommunication()
{
  digitalWrite(mPinCS, HIGH);
  DelayIt(1);
}

void RAMAccess::SetModeSingleByteRW()
{
  BeginCommunication();
  WriteByte(0b00000001);
  WriteByte(0b01000000);
  EndCommunication();
}

void RAMAccess::PushAddress(unsigned long address)
{
  int values[3];
  unsigned long test = address;
  for(int i=0;i<2;i++)
  {
    byte val = test & 0xFF;
    values[i] = val;
    test = test >> 8;
  }
  for(int i=2;i>=0;i--)
    WriteByte(values[i]);  
}

void RAMAccess::SetClock(int state)
{
  if (mClockHigh != (state == 1))
  {
    digitalWrite(mPinClock, state);
    if (mClockHigh) mHighCount++;
    mClockHigh = !mClockHigh;
  }
}

int RAMAccess::ReadByte()
{  
  int ret = 0;
  int test = 0b10000000;
  for(int i=0;i<8;i++)
  {
    SetClock(LOW);    
    DelayIt(1);
    SetClock(HIGH);    
    DelayIt(1);
    if (digitalRead(mPinSO) == HIGH)
      ret += test;
    test = test >> 1;
    SetClock(LOW); 
  }  
  return ret;
}
void RAMAccess::WriteByte(int value)
{
  int test = 0b10000000;  
  SetClock(LOW); 
  for(int i=0;i<8;i++)
  {    
    digitalWrite(mPinSI, (value & test) > 0 ? HIGH : LOW);
    test = test >> 1;    
    DelayIt(1);
    SetClock(HIGH);    
    DelayIt(1);
    SetClock(LOW); 
    DelayIt(1);
  }  
}

int RAMAccess::ReadAddress(unsigned long address)
{
  BeginCommunication();
  WriteByte(0b00000011);
  PushAddress(address);
  int ret = ReadByte();
  EndCommunication();
  return ret;
}

void RAMAccess::WriteAddress(unsigned long address, int value)
{
  BeginCommunication(); 
  WriteByte(0b00000010);
  PushAddress(address);
  WriteByte(value);
  EndCommunication();
}

void RAMAccess::WriteAddressArray(unsigned long address, int size, int *buffer)
{
  BeginCommunication(); 
  WriteByte(0b00000010);
  PushAddress(address);
  for(int i=0;i<size;i++)  
  {
    WriteByte(buffer[i]);
  }
  EndCommunication();
}

void RAMAccess::FillMemory(unsigned long address, unsigned long length, int value)
{
  BeginCommunication(); 
  WriteByte(0b00000010);
  PushAddress(address);
  for(unsigned long i=0;i<length;i++)  
  {
    WriteByte(value);
    //delay(1);
  }
  EndCommunication();
}

int RAMAccess::GetHighCount()
{
  return mHighCount;
}