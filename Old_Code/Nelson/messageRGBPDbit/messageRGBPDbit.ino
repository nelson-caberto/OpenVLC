#define PR_Pin    0
#define LEDr_Pin 10
#define LEDg_Pin  9
#define LEDb_Pin  8

#define TX_ON_DELAY  75  //how long to wait after turning on the LED
#define TX_OFF_DELAY 75  //how long to wait after turning off the LED

#define RX_DELAY    25  //if RX input changes, wait before getting actual reading

//these are specific to each LED, lamp, etc
#define RX0_LO  175
#define RX0_HI  275
#define RX1_LO  755
#define RX1_HI  775
#define RX2_LO  820
#define RX2_HI  840
#define RX3_LO  885
#define RX3_HI  915

#define RX_READS 20
#define RX_OFF  255

byte tx, rx, rxP = RX_OFF;

void setup() {
  pinMode(PR_Pin, INPUT);
  pinMode(LEDr_Pin, OUTPUT);
  pinMode(LEDg_Pin, OUTPUT);
  pinMode(LEDb_Pin, OUTPUT);
  Serial.begin(9600);
}

byte readBit() {
  long prAVG = 0;
  for (int i = 0; i<RX_READS; i++) prAVG += analogRead(PR_Pin);
  prAVG /= RX_READS;
  switch(prAVG) {
    case RX0_LO ... RX0_HI: return 0; //red min
    case RX1_LO ... RX1_HI: return 1; //blue max
    case RX2_LO ... RX2_HI: return 2; //green max
    case RX3_LO ... RX3_HI: return 3; //red max
    default: return RX_OFF;
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
  delay(TX_ON_DELAY);
  ledOff();
  delay(TX_OFF_DELAY);
}

void processRX() {
  rx = readBit();
  if (rx != rxP) {
    delay(RX_DELAY);
    rx = readBit();
  }
  if (rx != rxP) {
    if (rx != RX_OFF) {
      Serial.println(rx);
    }
  rxP = rx;
  }
}

void processTX() {
  if (Serial.available() > 0) {
    tx = Serial.parseInt();
    sendBit();
  }
}

void loop() {
  processRX();
  processTX();
}
