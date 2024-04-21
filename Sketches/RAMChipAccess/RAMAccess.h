#ifndef RAMACCESS_H 
#define RAMACCESS_H

struct RAMConfig
{
  int CS = -1;
  int CLOCK = -1;
  int SI = -1;
  int SO = -1;
};

class RAMAccess{
  private:
    int mPinCS = -1;
    int mPinClock = -1;
    int mPinSI = -1;
    int mPinSO = -1;
    bool mClockHigh = false;
    int mHighCount = 0;    
    void DelayIt(unsigned int time);
  protected:
    void BeginCommunication();
    void EndCommunication();
    int ReadByte();
    void WriteByte(int value);
    void PushAddress(unsigned long address);
    int ReadRegister();
    void SetModeSingleByteRW();
    void SetClock(int state);
  public:
    RAMAccess();    
    void SetPins(RAMConfig& config);
    int ReadAddress(unsigned long address);
    void WriteAddress(unsigned long address, int value);
    void WriteAddressArray(unsigned long address, int size, int *buffer);
    void FillMemory(unsigned long address, unsigned long length, int value);
    int GetHighCount();
};


#endif