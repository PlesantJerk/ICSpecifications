#include <stdarg.h>
#ifndef LCD1602_H
#define LCD1602_H

struct LCD1602Setup
{
  int PIN_DATA[8] = { 0, 0, 0, 0, 0, 0, 0, 0};
  int PIN_RS = 0;
  int PIN_SIG = 0;

  void AssignDataPins(...)
  {
    va_list ptr;
    va_start(ptr, 8);
    for(int i=0;i<8;i++)
      PIN_DATA[i] = va_arg(ptr, int);
    va_end(ptr);
  }
};
/*
const int DATA_PINS[8] = { 9,10,2,3,4,5,6,7};
const int RS_PIN = 13;
const int SIG_PIN = 12;
*/
class LCD1602
{
  private: 
    int mPinData[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    int mPinRS = 0;
    int mPinSig = 0;
    void ClearExistingPins();
    void SetupExistingPins();
    void ResetPin(int pin);
  protected:
    void SendData(int value, bool isInstruction);
    virtual void InitializeDisplay();
  public:
    void Initialize(LCD1602Setup setup);
    void SendText(String text, bool clearDisplay);
};
#endif