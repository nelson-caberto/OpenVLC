#define photoPin 0
#define timeout 1000
#define bitlength 9

int currentValue;
int previousValue = -1;
bool bits[bitlength] = {0,0,0,0,0,0,0,0,0};
int iterator = 0;
int startTimeout = 0;
int currentTime = 0;
bool trackTimeout = false;
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
  Serial.print("Bits:");
  for (int i = 0; i<bitlength; i++){
    Serial.print(bits[i]);
  }
  Serial.println();
  parityBit = 0;
  Serial.print("Data:");
  for (int i = 0; i<bitlength-1; i++) {
    Serial.print(bits[i]);
    parityBit+=bits[i];
  }
  Serial.println();
  if (bits[bitlength-1]!=parityBit%2) {
    Serial.println("Parity Bit Fail - Need to Retransmit Data");
  }
}

void keepBit(int value) {
  bits[iterator] = value;
  if (iterator==bitlength-1) {
    showBits();
    iterator = -1;
    trackTimeout = false;
  }
  iterator++;
} 

void startTimeoutTracking() {
  startTimeout = millis();
  trackTimeout = true;
}

void checkTimeout() {
  if (trackTimeout == false)
    return;
  currentTime = millis();
  if (currentTime - startTimeout > timeout) {
    Serial.println("Timeout");
    trackTimeout = false;
    iterator = 0;
  }
}

void loop() 
{
  currentValue = readBit();
  if (currentValue != previousValue) {
    delay(25);
    currentValue = readBit();
  }
  if (currentValue != previousValue) {
    if (currentValue != -1) {
      Serial.println(currentValue);
      startTimeoutTracking();
      keepBit(currentValue);
    }
    previousValue = currentValue;
  }
  checkTimeout();
}
