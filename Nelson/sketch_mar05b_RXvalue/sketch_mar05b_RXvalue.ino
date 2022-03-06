#define PR_Pin 0    //PhotoResistor
#define PR_reads 7

byte currentRX, previousRX = -1;
long prAVG;

void setup() {
  pinMode(PR_Pin, INPUT);
  Serial.begin(9600);
}

byte readBit() {
  prAVG = 0;
  for (int i = 0; i<PR_reads; i++ ) prAVG += analogRead(PR_Pin);
  prAVG /= PR_reads;
  Serial.println(prAVG);
  switch(prAVG) {
    case 150 ... 350: return 0;
    case 760 ... 780: return 1;
    case 820 ... 840: return 2;
    case 895 ... 915: return 3;
    default: return -1;
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
      delay(150);
    }
    previousRX = currentRX;
  }
}
