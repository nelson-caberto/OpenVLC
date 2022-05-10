//Sends string, displays each character as a byte array, reads string and puts it back into array form

#define PR_Pin    0
#define LED_Pin 10


#define TX_ON_DELAY  400  //how long to wait after turning on the LED
#define TX_OFF_DELAY 100  //how long to wait after turning off the LED

#define RX_DELAY    300  //if RX input changes, wait before getting actual reading

//these are specific to each LED, lamp, etc
#define RX0_LO  11
#define RX0_HI  300
#define RX1_LO  500
#define RX1_HI  650


#define RX_READS 7
#define RX_OFF  255

#define bitlength 9

byte tx, rx, rxP = RX_OFF;
int reading;

int iterator = 0;
String message;
byte parityBit = 0;
byte bits[bitlength] = {0,0,0,0,0,0,0,0,0};
byte rxVal[bitlength] = {0,0,0,0,0,0,0,0,0};

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
    case RX0_LO ... RX0_HI: return 0; //LOW
    case RX1_LO ... RX1_HI: return 1; //HIGH
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


void keepBit(){
   rxVal[iterator] = rx;
   iterator = iterator + 1;
//   Serial.println("Iterator:");
//   Serial.println(iterator);
   if(iterator == 9){
    showRXBits();
    iterator = 0 ;
   }
}
void showRXBits() {
  parityBit =0;
  Serial.print("Recived Data:");
  for (int i = 0; i<bitlength; i++){
    Serial.print(rxVal[i]);
  }
  Serial.println();
  Serial.print("Recived Character:");
  for (int i = 0; i<bitlength-1; i++){
    Serial.print(rxVal[i]);
    parityBit+=rxVal[i];
  }
  Serial.println();
  if (rxVal[bitlength-1]!=parityBit%2) {
    Serial.println("Parity Bit Fail - Need to Retransmit Data");
  }
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
      keepBit();
    }
  rxP = rx;
  }
}

void encode(int c) {
  parityBit = 0;
  for (int i = bitlength-2; i >= 0; i--) {
    bits[i] = c%2;
    parityBit += bits[i];
    c=c/2;
  }
  bits[8] = parityBit%2;
}

void sendByte() {
  Serial.print("Send:");
  for (int i = 0; i<bitlength; i++) {
    if (bits[i]==0) {
      tx=0;
      sendBit();
      Serial.print(tx);
      delay(400);

    } else if (bits[i]==1) { 
      tx=1;
      sendBit();
      Serial.print(tx);
      delay(400);

    } 
    delay(50);
  }
  Serial.println();
}

void showBits() {
  Serial.print("Character:");
  for (int i = 0; i<bitlength-1; i++){
    Serial.print(bits[i]);
  }
  Serial.println();
}
void sendString(String s) {
  for (int i = 0; i<message.length();i++) {
    encode(message[i]);
    showBits();
    sendByte();
  }
}
void processTX() {
  if (Serial.available() > 0) {
    Serial.println("Waiting for a message:");
    message = Serial.readString();
    Serial.println("Message:");
    Serial.println(message);
    sendString(message);
  }
}

void loop() {
  processRX();
  processTX();
}
