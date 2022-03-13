//Sends a bits and recieves a bit
#define PR_Pin    0
#define LED_Pin 10


#define TX_ON_DELAY  400  //how long to wait after turning on the LED
#define TX_OFF_DELAY 100  //how long to wait after turning off the LED

#define RX_DELAY    300  //if RX input changes, wait before getting actual reading

//these are specific to each LED, lamp, etc
#define RX0_LO  15
#define RX0_HI  300
#define RX1_LO  680
#define RX1_HI  750


#define RX_READS 7
#define RX_OFF  255

byte tx, rx, rxP = RX_OFF;
int reading;
void setup() {
  pinMode(PR_Pin, INPUT);
  pinMode(LED_Pin, OUTPUT);
  Serial.begin(9600);
}

byte readBit() {
  long prAVG = 0;
  for (int i = 0; i<RX_READS; i++) prAVG += analogRead(PR_Pin);
  prAVG /= RX_READS;
//  if (prAVG>10){
//      Serial.println(prAVG);
//
//  }
  switch(prAVG) {
    case RX0_LO ... RX0_HI: return 0; //red min
    case RX1_LO ... RX1_HI: return 1; //blue max
    default: return RX_OFF;
  }
}

void ledOff() {
  switch(tx) {
    case 0: analogWrite(LED_Pin,0); break;
    case 1: analogWrite(LED_Pin,0); break;
    default: break;
  }
}

void ledOn() {
  switch(tx) {
    case 0:  analogWrite(LED_Pin,5); break;
    case 1:  analogWrite(LED_Pin,255); break;
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
    Serial.println("Enter a Value:");
    tx = Serial.parseInt();
    Serial.println("Value:");
    Serial.println(tx);
    sendBit();
  }
}

void loop() {
  processRX();
  processTX();
}
