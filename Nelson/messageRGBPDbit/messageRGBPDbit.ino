#define PR_Pin 0
#define LEDr_Pin 10
#define LEDg_Pin 9
#define LEDb_Pin  8
#define PR_reads 40

byte tx, rx;

void setup() {
  pinMode(PR_Pin, INPUT);
  pinMode(LEDr_Pin, OUTPUT);
  pinMode(LEDg_Pin, OUTPUT);
  pinMode(LEDb_Pin, OUTPUT);
  Serial.begin(9600);
}

byte readBit() {
  long prAVG = 0;
  for (int i = 0; i<PR_reads; i++) prAVG += analogRead(PR_Pin);
  prAVG /= PR_reads;
  switch(prAVG) {
    case 175 ... 275: return 0; //red min
    case 755 ... 775: return 1; //blue max
    case 820 ... 840: return 2; //green max
    case 885 ... 915: return 3; //red max
    default: return 255;
  }
}

void ledOff() {
  switch(tx) {
    case 0: analogWrite(LEDr_Pin,0); break;
    case 1: digitalWrite(LEDb_Pin,LOW); break;
    case 2: digitalWrite(LEDg_Pin,LOW); break;
    case 3: digitalWrite(LEDr_Pin,LOW); break;
    default: break;
  }
}

void ledOn() {
  switch(tx) {
    case 0: analogWrite(LEDr_Pin,1); break;
    case 1: digitalWrite(LEDb_Pin,HIGH); break;
    case 2: digitalWrite(LEDg_Pin,HIGH); break;
    case 3: digitalWrite(LEDr_Pin,HIGH); break;
    default: break;
  }
}

void sendBit() {
  ledOn();
  delay(75);
  ledOff();
  delay(75);
}

void loop() {
  rx = readBit();
  if (rx != 255) Serial.println(rx);
  if (Serial.available() > 0) {
    tx = Serial.parseInt();
    sendBit();
  }
}
