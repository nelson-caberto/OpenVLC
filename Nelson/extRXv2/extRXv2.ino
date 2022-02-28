#define photoPin 0
#define timeout 1000

int currentValue;
int previousValue = -1;
bool bits[8] = {0,0,0,0,0,0,0,0};
int iterator = 0;
int startTime = 0;
int currentTime = 0;
bool trackTime = false;

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

void showBits() {
  Serial.print("Data:");
  for (int i = 0; i<8; i++){
    Serial.print(bits[i]);
  }
  Serial.println();
}

void keepBit(int value) {
  bits[iterator] = value;
  if (iterator==7) {
    showBits();
    iterator = -1;
    trackTime = false;
  }
  iterator++;
}

void startTimer() {
  startTime = millis();
  trackTime = true;
}

void checkTimer() {
  if (trackTime == false)
    return;
  currentTime = millis();
  if (currentTime - startTime > timeout) {
    Serial.println("Timeout");
    trackTime = false;
    iterator = 0;
  }
}

void loop() 
{
  currentValue = readBit();
  if (currentValue != previousValue) {
    delay(310);
  }
  if (currentValue != previousValue) {
    if (currentValue != -1) {
      Serial.println(currentValue);
      startTimer();
      keepBit(currentValue);
    }
    previousValue = currentValue;
  }
  checkTimer();
}
