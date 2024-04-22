#include <Arduino.h>
const int pin_addr[15] = { 53, 51, 49, 47, 45, 43, 41, 39, 37, 35, 33, 31, 29, 27, 25}; // A14-A0
const int pin_data[8] = { 52, 50, 48, 46, 44, 42, 40, 38 }; // I/O7-I/O0
const int pin_cs = 13;
const int pin_we = 12;
const int pin_oe = 11;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(57600);
  Serial.println();
  Serial.println("Setting up pins");
  setup_pins();
  Serial.println("Writing data...");
  write_address(0x4500, 0xBB);
  write_address(0xF201, 0x05);
  write_address(0x0400, 0x5A);
  char line[200];
  Serial.println("Reading data...");
  sprintf(line, "%02X %02X %02X", read_address(0x4500), read_address(0xF201), read_address(0x0400));
  Serial.println(line); 
  unsigned long s = millis();
  for(unsigned int i=0;i<32768;i++)
    write_address(i, 0x66);
  unsigned long e = millis();
  sprintf(line, "it took %ums to write 32K", e-s);
  Serial.println(line);
  s = millis();
  unsigned int bad_count = 0;
  for(unsigned int i=0;i<32768;i++)
    if (read_address(i) != 0x66) bad_count++;
  e = millis();
  sprintf(line, "it took %ums to read 32K (%u errors)", e-s, bad_count);
  Serial.println(line);
}

unsigned int read_address(int address)
{
  reset_pins();
  set_address(address);
  return read_data();
}

void write_address(int address, int value)
{
  reset_pins();
  set_address(address);
  write_data(value);
}
void setup_pins()
{  
  for(int i=0;i<15;i++)
    pinMode(pin_addr[i], OUTPUT);
  set_data_pin_mode(INPUT_PULLUP);
  pinMode(pin_cs, OUTPUT);
  pinMode(pin_we, OUTPUT);
  pinMode(pin_oe, OUTPUT);
  digitalWrite(pin_we, HIGH);  
  digitalWrite(pin_cs, HIGH);  
}

void soft_delay(int time)
{

}
void reset_pins()
{
  digitalWrite(pin_cs, HIGH);
  digitalWrite(pin_we, HIGH);
  digitalWrite(pin_oe, HIGH);
  soft_delay(1);
}
void set_data_pin_mode(int mode)
{
  for(int i=0;i<8;i++)
    pinMode(pin_data[i], mode);
}

void set_address(int address)
{   
  //print_pin_status("set_address start");
  int test = 0b0100000000000000;
  for(int i=0;i<15;i++)
  {
    digitalWrite(pin_addr[i], ((address & test) > 0) ? HIGH : LOW);
    test = test >> 1;
  }
  digitalWrite(pin_cs, LOW);
  //print_pin_status("set_address end");
  soft_delay(1);
}

void print_pin_status(String text)
{
  char line[300];
  char textbuf[200];
  text.toCharArray((char*)&textbuf, 200);
  sprintf(line, "STATUS: %s cs=%i we=%i oe=%i", textbuf, digitalRead(pin_cs), digitalRead(pin_we), digitalRead(pin_oe));
  Serial.println(line);
}
bool mIsWriteMode = false;
void set_is_write_mode(bool isWriteMode)
{
  mIsWriteMode = isWriteMode;
  set_data_pin_mode(isWriteMode ? OUTPUT : INPUT_PULLUP);  
}

void commit()
{
  if (mIsWriteMode)
  {
    //digitalWrite(pin_oe, HIGH);
    digitalWrite(pin_we, LOW);      
  }
  else
  {
    //digitalWrite(pin_we, HIGH);      
    digitalWrite(pin_oe, LOW);  
  }    
  soft_delay(1);  
}

void write_data(int value)
{
  set_is_write_mode(true);
  //print_pin_status("write before setting data pins");
  int test = 0b10000000;
  for(int i=0;i<8;i++)
  {
    digitalWrite(pin_data[i], (value & test) > 0 ? HIGH : LOW);
    test = test >> 1;
  }
  soft_delay(1);
  commit();
  //print_pin_status("write commit data pins");
  reset_pins();
}

unsigned int read_data()
{
  set_is_write_mode(false);
  //print_pin_status("read 1");
  soft_delay(1);  
  unsigned int ret = 0;
  commit();
  //print_pin_status("read after commit");
  for(int i=0;i<8;i++)
  {
    int bit =digitalRead(pin_data[i]) == HIGH ? 1 : 0;
    ret = (ret << 1) + bit;  
  }  
  reset_pins();
  return ret;
}

void loop() {
  // put your main code here, to run repeatedly:

}
