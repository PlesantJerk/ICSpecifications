#include "Arduino.h"
#include "RAMAccess.h"

RAMAccess::RAMAccess()
{
}

void RAMAccess::SetPins(RAMConfig& config)
{
  if (mPinCS != -1) pinMode(mPinCS, INPUT);
  if (mPinCLock != -1) pinMode(mPinCLock, INPUT);
  if (mPinSI != -1) pinMode(mPinSI, INPUT);
  if (mPinSO != -1) pinMode(mPinSO, INPUT);
  mPinCLock = config.CLOCK;
  mPinCS = config.CS;
  mPinSI = config.SI;
  mPinSO = config.SO;
  pinMode(mPinCS, OUTPUT);
  pinMode(mPinCLock, OUTPUT);
  pinMode(mPinSI, OUTPUT);
  pinMode(mPinSO, INPUT);

  digitalWrite(mPinCLock, LOW);  
  EndCommunication();  
  SetModeSingleByteRW();  
}

void RAMAccess::DelayIt(unsigned int time)
{
  delayMicroseconds(time);
}

void RAMAccess::BeginCommunication()
{
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
  WriteByte(0);
  EndCommunication();
}

void RAMAccess::PushAddress(int address)
{
  int test = 1;  
  int sendbytes[3];
  char line[80];
  for(int i=0;i<3;i++)
  {
    int data = 0;
    int current = 1;
    
    for(int j=0;j<8;j++)
    {
      if ((address & test) > 0)
        data += current;
      test = test << 1;
      current = current << 1;          
    }
    sendbytes[i] = data;
  }
  for(int i=2;i>=0;i--)
    WriteByte(sendbytes[i]);
}

int RAMAccess::ReadByte()
{  
  int ret = 0;
  int test = 0b10000000;
  for(int i=0;i<8;i++)
  {
    digitalWrite(mPinCLock, LOW);  
    DelayIt(1);
    digitalWrite(mPinCLock, HIGH);      
    DelayIt(1);
    if (digitalRead(mPinSO) == HIGH)
      ret += test;
    test = test >> 1;
    digitalWrite(mPinCLock, LOW);      
  }  
  return ret;
}
void RAMAccess::WriteByte(int value)
{
  int test = 0b10000000;  
  digitalWrite(mPinCLock, LOW);  
  for(int i=0;i<8;i++)
  {    
    digitalWrite(mPinSI, (value & test) > 0 ? HIGH : LOW);
    test = test >> 1;    
    DelayIt(1);
    digitalWrite(mPinCLock, HIGH);  
    DelayIt(1);
    digitalWrite(mPinCLock, LOW);  
    DelayIt(1);
  }  
}

int RAMAccess::ReadAddress(int address)
{
  BeginCommunication();
  WriteByte(0b00000011);
  PushAddress(address);
  int ret = ReadByte();
  EndCommunication();
  return ret;
}

void RAMAccess::WriteAddress(int address, int value)
{
  BeginCommunication(); 
  WriteByte(0b00000010);
  PushAddress(address);
  WriteByte(value);
  EndCommunication();
}