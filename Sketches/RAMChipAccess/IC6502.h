#include <stdarg.h>
#include <wstring.h>
#include <stdio.h>
#include "RAMAccess.h"
#ifndef IC6502
#define IC6502 IC6502

struct IC6502Setup
{
  int PIN_ADDRESS[16];
  int PIN_DATA[8];
  int PIN_RWB;
  int PIN_CLOCK;
  int PIN_RESET;
  RAMAccess ram;
  void SetAddressPins( ...)
  {
    va_list ptr; 
    va_start(ptr, 16);
    for(int i=0;i<16;i++)
      PIN_ADDRESS[i] = va_arg(ptr, int);
    va_end(ptr);
  }


  void SetDataPins(...)
  {
    va_list ptr; 
    va_start(ptr, 8);
    for(int i=0;i<8;i++)
      PIN_DATA[i] = va_arg(ptr, int);
    va_end(ptr);
  }

};

struct IC6502Status
{
  int address;
  int data;
  bool IsWriteOp;

  String ToString()
  {
    char *buffer = new char[256];
    sprintf(buffer, "address %04X - R/W: %s - Data: %02X\0x00", address, IsWriteOp ? "W" : "R", data);
    String s= (String)buffer;
    delete[] buffer;
    return s;
  }
};

class IC6502
{
  private:
    IC6502Setup mSetup;
    int mP_Addr[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int mP_Data[8] = { 0, 0, 0, 0, 0, 0, 0, 0};    
    int mP_RWB = 0;
    int mP_CLOCK = 0;
    int mP_RESET = 0;
    bool mIsWritingData = false;
    bool mClockState = false;

    void CloseCurrentPins();
    void ClosePinIfNeeded(int pin);
    void SetupCurrentPins();
    void UpdateMode();
  protected:    
    bool IsWriteMode();
  public:
    void Initialize(IC6502Setup setup);
    void Reset();
    int ReadAddress();
    int ReadData();
    void WriteData(int value);
    void PrintStatus();
    void Tick();
    IC6502Status GetStatus();    
};
#endif