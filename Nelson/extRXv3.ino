#define photoPin 0
#define timeout 1000
#define bitlength 9

int currentValue;
int previousValue = -1;
bool bits[bitlength] = {0,0,0,0,0,0,0,0,0};
int iterator = 0;
int startTime = 0;
int currentTime = 0;
bool trackTime = false;
int parityBit = 0;

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
  for (int i = 0; i<bitlength; i++){
    Serial.print(bits[i]);
  }
  Serial.println();
  parityBit = 0;
  for (int i = 0; i<bitlength-1; i++) {
    parityBit+=bits[i];
  }
  if (bits[bitlength-1]!=parityBit%2) {
    Serial.println("Parity Bit Fail - Need to Retransmit Data");
  }
}

void keepBit(int value) {
  bits[iterator] = value;
  if (iterator==bitlength-1) {
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
