//Sends String 

#define PR_Pin    0
#define LED_Pin 10


#define TX_ON_DELAY  300  //how long to wait after turning on the LED
#define TX_OFF_DELAY 80  //how long to wait after turning off the LED

#define RX_DELAY    40  //if RX input changes, wait before getting actual reading

//these are specific to each LED, lamp, etc
#define RX0_LO  200
#define RX0_HI  350
#define RX1_LO  650
#define RX1_HI  850


#define RX_READS 7
#define RX_OFF  255

#define bitlength 9

byte tx, rx, rxP = RX_OFF;
int reading;
char character;

int iterator = 0;
int iteratorLen = 0;
String message;
byte parityBit = 0;
byte bits[bitlength] = {0,0,0,0,0,0,0,0,0};
byte rxVal[bitlength] = {0,0,0,0,0,0,0,0,0};

String messageRX = "";
byte txBits[bitlength]={0,0,0,0,0,0,0,0,0};

int numRX;

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
char toChar() {
  return rxVal[7]*1 + rxVal[6]*2 + rxVal[5]*4 + rxVal[4]*8 + rxVal[3]*16 + rxVal[2]*32 + rxVal[1]*64 + rxVal[0]*128;
}
int toDec() {
  return rxVal[7]*1 + rxVal[6]*2 + rxVal[5]*4 + rxVal[4]*8 + rxVal[3]*16 + rxVal[2]*32 + rxVal[1]*64 + rxVal[0]*128;
}
void keepBit(){
   rxVal[iterator] = rx;
   iterator = iterator + 1;
//   Serial.println("Iterator:");
//   Serial.println(iterator);
    if(iterator == 9 & iteratorLen == 0){
    showRXLenBits();
    iterator = 0 ;
    iteratorLen = iteratorLen + 1;
   }
   if(iterator == 9 & iteratorLen != 0){
    showRXBits();
    iterator = 0 ;
    iteratorLen = iteratorLen + 1;
    character = toChar();
    Serial.println();
    Serial.print("Character:");
    Serial.print(character);
    Serial.println();

    messageRX = messageRX + character;
   }
   if(iteratorLen == (numRX +1) ){
      iteratorLen = 0;
      Serial.print("Received a message of: ");
      Serial.print(messageRX);
      Serial.println();
      messageRX = "";
   }
   
}
void sendTXByte() { //message length transmit
  Serial.print("Send Length of:");
  for (int i = 0; i<bitlength; i++) {
    if (txBits[i]==0) {
      tx=0;
      sendBit();
      Serial.print(tx);
//      delay(50);

    } else if (txBits[i]==1) { 
      tx=1;
      sendBit();
      Serial.print(tx);
//      delay(50);

    } 
    delay(50);
  }
  Serial.println();
}

void messageLength(){
  //Print Message length in terms of character
  int num = message.length();
  Serial.println();
  Serial.print("Sent a message length of: ");
  Serial.println(num);
  encodeTX(num);
  sendTXByte();
}
void showRXLenBits() {
  parityBit = 0;
  Serial.print("Received String Length Data:");
  for (int i = 0; i<bitlength; i++){
    Serial.print(rxVal[i]);
  }
  Serial.println();
  Serial.print("Received Length of:");
  for (int i = 0; i<bitlength-1; i++){
    Serial.print(rxVal[i]);
    parityBit = parityBit + rxVal[i];
  }
  Serial.println();
  Serial.print("Length of:");
  numRX = toDec();
  Serial.print(numRX);
  Serial.println();
  numRX = toDec();
//  Serial.print(numRX);

//  Serial.println();
//  Serial.println(parityBit%2);
//  Serial.println(rxVal[8]);

  if (rxVal[bitlength-1]!=parityBit%2) {
    Serial.println("Parity Bit Fail - Need to Retransmit Data");
  }
}
void showRXBits() {
  parityBit = 0;
  Serial.print("Received Data:");
  for (int i = 0; i<bitlength; i++){
    Serial.print(rxVal[i]);
  }
  Serial.println();
  Serial.print("Received Character:");
  for (int i = 0; i<bitlength-1; i++){
    Serial.print(rxVal[i]);
    parityBit = parityBit + rxVal[i];
  }
//  Serial.println();
//  Serial.println(parityBit%2);
//  Serial.println(rxVal[8]);

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
void encodeTX(int c) { //store string length
  parityBit = 0;
  for (int i = bitlength-2; i >= 0; i--) {
    txBits[i] = c%2;
    parityBit = parityBit + txBits[i];
    c=c/2;
  }
//  Serial.println(parityBit);
  txBits[8] = parityBit%2;
}
void encode(int c) { //store character in terms of binary
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
//      delay(400);

    } else if (bits[i]==1) { 
      tx=1;
      sendBit();
      Serial.print(tx);
//      delay(400);

    } 
//    delay(50);
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
    Serial.print("Message:");
    Serial.print(message);
    messageLength();
//    delay(50);
    sendString(message);
  }
}

void loop() {
 
  processRX();
  processTX();
}
