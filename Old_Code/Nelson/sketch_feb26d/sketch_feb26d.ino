#define lampPin 8

void setup() 
{
  pinMode(lampPin, OUTPUT);
  Serial.begin(9600);
}

void loop() 
{
  Serial.println("Value:");
  while (Serial.available() == 0) {}
  int value = Serial.parseInt();
  Serial.println(value);
  analogWrite(lampPin,value);
}
