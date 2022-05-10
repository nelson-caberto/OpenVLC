//Sends String with acknowledgments (green tape LED) and determines if it needs to resend or not

#include <Wire.h>
#include <Adafruit_ADS1X15.h>

Adafruit_ADS1015 ads1015;

#define MOSFETsignal 2
#define LED_Pin 8

#define TX_ON_DELAY  10  //how long to wait after turning on the LED
#define TX_OFF_DELAY 20  //how long to wait after turning off the LED

#define RX_DELAY    5  //if RX input changes, wait before getting actual reading

//these are specific to each LED, lamp, etc
#define RX0_LO  85
#define RX0_HI  95
#define RX1_LO  125
#define RX1_HI  135



#define RX_READS 7
#define RX_OFF  255

#define bitlength 9

byte tx, rx, rxP = RX_OFF;
int reading;
char character;
int paritychecker = 0;
int iteratorAck = 0;
int ackSignal = 0;

int iterator = 0;
int iteratorLen = 0;
String message;
byte parityBit = 0;
byte bits[bitlength] = {0,0,0,0,0,0,0,0,0};
byte rxVal[bitlength] = {0,0,0,0,0,0,0,0,0};
byte rxAck[2] = {0,0};

String messageRX = "";
byte txBits[bitlength]={0,0,0,0,0,0,0,0,0};

int numRX;
int rxIterator = 0;
int adc0;

void setup() {
  pinMode(MOSFETsignal, OUTPUT);
  pinMode(LED_Pin, OUTPUT);
  ads1015.begin();
  Serial.begin(9600);

}

byte readBit() {
  long prAVG = 0;
  adc0 = ads1015.readADC_SingleEnded(0);
  for (int i = 0; i<RX_READS; i++) prAVG += adc0;
  prAVG /= RX_READS;
//  if (prAVG>65){
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
    case 0: digitalWrite(LED_Pin,0); break;
    case 1: digitalWrite(LED_Pin,0); break;
    default: break;
  }
}

void ledOn() {
  switch(tx) {
    case 0:  digitalWrite(LED_Pin,1);digitalWrite(MOSFETsignal,0);break;
    case 1:   digitalWrite(LED_Pin,1);digitalWrite(MOSFETsignal,1); break;
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
  //  delay(50);
  if(ackSignal == 0){
     rxVal[iterator] = rx;
     iterator = iterator + 1;
  }
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
      paritychecker = 0;
//      delay(100);
      sendAck();
   }
   if (ackSignal ==1 & iteratorAck != 2){
    rxAck[iteratorAck] = rx;
//    Serial.print("Ack: ");
//    Serial.println(rxAck[iteratorAck]);
//    Serial.println();
//    Serial.print("iterator:");
//    Serial.print(iteratorAck);
    iteratorAck = iteratorAck + 1;
   }
   if(iteratorAck == 2){
//    Serial.println();
//    Serial.print("iterator:");
//    Serial.print(iteratorAck);
//    Serial.println();
//    Serial.print("signal:");
//    Serial.println(ackSignal);

    showRXAck();
    ackSignal = 0;
    iteratorAck = 0;
   }
}
void sendAck(){
  if(paritychecker == 0){
    Serial.println("Sending Acknowledgment of:");
    delay(350);
    for(int i = 0; i<2; i++){
      tx = 1;
      Serial.print(tx);
      sendBit();
//      delay(50);
    }
    Serial.println();
  }
}
void sendTXByte() { //message length transmit
  Serial.print("Send Length of:");
  for (int i = 0; i<bitlength; i++) {
    if (txBits[i]==0) {
      tx=0;
      sendBit();
      Serial.print(tx);
//      delay(400);

    } else if (txBits[i]==1) { 
      tx=1;
      sendBit();
      Serial.print(tx);
//      delay(400);

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
void showRXAck() {
//  parityBit = 0;
  Serial.print("Received An Acknowledgement Data of: ");
  for (int i = 0; i<2; i++){
    Serial.print(rxAck[i]);
    rxIterator = rxIterator + 1;
  }
  Serial.println();
  Serial.print("Received An Acknowledgement of: ");
  Serial.print(rxAck[0]);
  Serial.println();

//  Serial.println();
//  Serial.println(parityBit%2);
//  Serial.println(rxVal[8]);

//  if (rxVal[0] =! rxVal[1]) {
//    Serial.println("Parity Bit Fail - Need to Retransmit Acknowledgment");
//  }
}
void checkRXAck(){
  if(rxVal[0] !=1 & rxVal[1]!=1 & rxIterator == 2){
    Serial.println("Message was recieved incorrectly, Resending...");
    sendMessage();
    rxIterator = 0;

  }
  else {
    Serial.println("Message was recieved correctly");
    rxIterator = 0;
  }
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
    paritychecker = paritychecker + 1;

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
//
//  if (rxVal[bitlength-1]!=parityBit%2) {
//    Serial.println("Parity Bit Fail - Need to Retransmit Data");
//    paritychecker = paritychecker + 1;
//  }
}

void processRX() {
  rx = readBit();
  if (rx != rxP) {
    delay(RX_DELAY);
    rx = readBit();
  }
  if (rx != rxP) {
    if (rx != RX_OFF) {
      if(ackSignal == 0){
         Serial.println(rx);
      }
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
  ackSignal = 1;
  for (int i = 0; i<message.length();i++) {
    encode(message[i]);
    showBits();
    sendByte();
  }
  rxIterator = 0;

}
void sendMessage(){
  messageLength();
  sendString(message);
  checkRXAck();
}
void processTX() {
  if (Serial.available() > 0) {
    Serial.println("Waiting for a message:");
    message = Serial.readString();
    Serial.print("Message:");
    Serial.print(message);
    sendMessage();
  }
}

void loop() {
 
  processRX();
  processTX();
}
