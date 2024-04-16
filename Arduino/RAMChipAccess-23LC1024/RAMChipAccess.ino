#include "RAMAccess.h"

void setup()
{
  Serial.begin(9600);
  Serial.println();
  RAMAccess ca;
  RAMConfig cfg;
  cfg.CS = 52;
  cfg.CLOCK = 50;
  cfg.SI = 51;
  cfg.SO = 53;
  ca.SetPins(cfg);
  ca.WriteAddress(0x0002, 0x3b);
  ca.WriteAddress(0x00A2, 0xFF);
  ca.WriteAddress(0xA002, 0x01);
  char line[256];
  sprintf(line, "You have values of %02X, %02X, and %02X", ca.ReadAddress(0x0002), ca.ReadAddress(0x00A2), ca.ReadAddress(0xA002));
  Serial.println(line);
}

void loop() {
  // put your main code here, to run repeatedly:

}
