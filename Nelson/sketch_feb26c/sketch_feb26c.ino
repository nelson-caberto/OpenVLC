#define photoPin 0

int currentValue;
int previousValue = -1;

void setup() 
{
  pinMode(photoPin, INPUT);
  Serial.begin(9600);
}

int readBit() {
  int reading;
  reading = analogRead(photoPin);
  if(106 <= reading && reading <= 328){
    return 0;
  }
  if(615 <= reading && reading <= 794){
    return 1;
  }
  return -1;
}

void loop() 
{
  currentValue = readBit();
  if (currentValue != previousValue) {
    delay(310);
  }
  if (currentValue != previousValue) {
    Serial.println(currentValue);
    previousValue = currentValue;
  }
}
