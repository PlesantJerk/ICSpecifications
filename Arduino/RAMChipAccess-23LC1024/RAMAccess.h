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
    int mPinCLock = -1;
    int mPinSI = -1;
    int mPinSO = -1;
    void DelayIt(unsigned int time);
  protected:
    void BeginCommunication();
    void EndCommunication();
    int ReadByte();
    void WriteByte(int value);
    void PushAddress(int address);
    int ReadRegister();
    void SetModeSingleByteRW();
  public:
    RAMAccess();    
    void SetPins(RAMConfig& config);
    int ReadAddress(int address);
    void WriteAddress(int address, int value);
};


#endif