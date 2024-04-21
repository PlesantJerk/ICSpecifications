int LED = 52;
int sensor = A3;
void setup() {
  // put your setup code here, to run once:
  pinMode(LED, OUTPUT);
  Serial.begin(9600);
  
}

void blinkIt()
{
  for(int i=0;i<10;i++)
  {
    digitalWrite(LED, HIGH);
    delay(500);
    digitalWrite(LED, LOW);
    delay(500);
  }
  Serial.print("\rDone\r");
}
void loop() {
  Serial.print(analogRead(sensor)*5.0/1023.0);
  Serial.print("\n");
  delay(500);
  if (Serial.available())
  {
    String text = Serial.readStringUntil("\n");    
    text.trim();
    Serial.print(text);
    Serial.print("\n");    
    if (text.equals(String("blink")))
    {
      Serial.print("found blink\n");
      blinkIt();
    }
  }
}
