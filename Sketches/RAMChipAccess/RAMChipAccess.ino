#include "RAMAccess.h"
#include "IC6502.h"

IC6502 ic6502;

bool clock_state = false;

void setup()
{
  Serial.begin(57600);
  Serial.println();    
  IC6502Setup setup;
  setup.SetAddressPins(23, 25, 27, 29, 31, 33, 35, 37, 39, 41, 43, 45, 47, 49, 51, 53);
  setup.SetDataPins(22, 24, 26, 28, 30, 32, 34, 36);
  setup.PIN_RWB = 4;
  setup.PIN_CLOCK = 3;
  setup.PIN_RESET = 5;
  setup.ram = setup_ram();
  delay(500);  
}

void StartCPU(IC6502Setup setup)
{
  ic6502.Initialize(setup);  
  ic6502.Reset();
  for(int i=0;i<60;i++)
  {
    ic6502.Tick();
    Serial.print(i);
    Serial.print(". ");
    Serial.println(ic6502.GetStatus().ToString());
  };    
}
RAMAccess setup_ram()
{
  char line[256];
  RAMAccess ret;
  RAMConfig cfg;
  cfg.CS = 13;
  cfg.CLOCK = 10;
  cfg.SI = 11;
  cfg.SO = 12;
  ret.SetPins(cfg);
  ret.WriteAddress(0x00000002, 0x3b);
  ret.WriteAddress(0x000000A2, 0xFF);
  ret.WriteAddress(0x0000A002, 0x01);
  int d[3] = { 0x06, 0x0B, 0xA9 };
  ret.WriteAddressArray(0xB000, 3, &d[0]);  
  erase_memory(ret);  
  sprintf(line, "You have values of %02X, %02X, and %02X", ret.ReadAddress(0x0002), ret.ReadAddress(0x00A2), ret.ReadAddress(0xA002));
  Serial.println(line);
  sprintf(line, "You have values of %02X, %02X, and %02X", ret.ReadAddress(0xB000), ret.ReadAddress(0xB001), ret.ReadAddress(0xB002));
  Serial.println(line);
  int a=ret.ReadAddress(0xFFFC), b=ret.ReadAddress(0xFFFD), c = ret.ReadAddress(0xD000);
  sprintf(line, "BBB$FFFC:%02X $FFFD: %02X -> %02X", a,b,c);
  Serial.println(line);
  return ret;
}

void erase_memory(RAMAccess mem)
{
  unsigned long s = millis();  
  unsigned long length = 65534;
  mem.FillMemory(0x00000000, length, 0xEA);
  unsigned long e = millis();
  Serial.print("Total time to clear ");
  Serial.print(length);
  Serial.print(" bytes was ");
  Serial.print(e-s);
  Serial.println("ms");
}

void test_it(unsigned long addr)
{
  byte val;
  char line[50];
  Serial.println(sizeof(addr));
  sprintf(line, "test it: $%08X -> ", (unsigned long)addr);
  //sprintf(line, "test it: $%08X -> ", 0x01020304);
  Serial.print(line);
  for(int i=0;i<4;i++)
  {
    val = addr & 0xFF;
    addr = addr >> 8;
    sprintf(line, "%02X", val);
    Serial.print(line);
  }
  Serial.println();
}

void loop() {
  // put your main code here, to run repeatedly:
}
