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
  switch(prAVG) {
    case  40 ...  95: return 0;
    case 125 ... 222: return 1;
    case 263 ... 370: return 2;
    case 390 ... 555: return 3;
    case 762 ... 771: return 4;
    case 817 ... 832: return 5;
    case 907 ... 913: return 6;
    case 938 ... 944: return 7;
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
    if (currentRX != -1) {
      Serial.println(currentRX);
      delay(150);
    }
    previousRX = currentRX;
  }
}
