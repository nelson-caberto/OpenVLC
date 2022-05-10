#define PR_Pin 0    //PhotoResistor
#define PR_reads 7
#define bitlength 10
#define timeout 1000

byte bits[bitlength] = {0,0,0,0,0,0,0,0,0,0};
byte currentRX, previousRX = -1;
long prAVG;
byte parityBit = 0;
int iterator = 0;
int startTimeout = 0;
int currentTime = 0;
bool trackTimeout = false;

void setup() {
  pinMode(PR_Pin, INPUT);
  Serial.begin(9600);
}

byte readBit() {
  prAVG = 0;
  for (int i = 0; i<PR_reads; i++ ) prAVG += analogRead(PR_Pin);
  prAVG /= PR_reads;
  switch(prAVG) {
    case 150 ... 350: return 0;
    case 760 ... 780: return 1;
    case 820 ... 840: return 2;
    case 895 ... 915: return 3;
    default: return -1;
  }
}

char toChar() {
  return bits[7]*1 + bits[6]*2 + bits[5]*4 + bits[4]*8 + bits[3]*16 + bits[2]*32 + bits[1]*64 + bits[0]*128;
}

void showBits() {
  Serial.print("Bits:");
  for (int i = 0; i<bitlength; i++){
    Serial.print(bits[i]);
  }
  Serial.println();
  parityBit = 0;
  Serial.print("Data:");
  for (int i = 0; i<bitlength-2; i++) {
    Serial.print(bits[i]);
    parityBit+=bits[i];
  }
  Serial.println();
  Serial.print("Message:");
  Serial.print(toChar());
  if (bits[bitlength-2]!=parityBit%2) {
    Serial.println("Parity Bit Fail - Need to Retransmit Data");
  }
}

void keepBit(int value) {
  switch (value) {
    case 0: bits[iterator] = 0; bits[iterator+1] = 0; break;
    case 1: bits[iterator] = 0; bits[iterator+1] = 1; break;
    case 2: bits[iterator] = 1; bits[iterator+1] = 0; break;
    case 3: bits[iterator] = 1; bits[iterator+1] = 1; break;
    default: break;
  }
  if (iterator==bitlength-2) {
    showBits();
    trackTimeout = false;
  }
  iterator+=2;
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

void loop() {
  currentRX = readBit();
  if (currentRX != previousRX) {
    delay(25);
    currentRX = readBit();
  }
  if (currentRX != previousRX) {
    if (currentRX != 255) {
      Serial.println(currentRX);
      startTimeoutTracking();
      keepBit(currentRX);
      delay(30);
    }
    previousRX = currentRX;
  }
  checkTimeout();
}
