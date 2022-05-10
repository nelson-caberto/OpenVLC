#define LEDr_Pin 10
#define LEDg_Pin 9
#define LEDb_Pin 8
#define bitlength 10

byte tx;
String message;
byte parityBit = 0;
byte bits[bitlength] = {0,0,0,0,0,0,0,0,0,0};

void setup() {
  pinMode(LEDr_Pin, OUTPUT);
  pinMode(LEDg_Pin, OUTPUT);
  pinMode(LEDb_Pin, OUTPUT);
  Serial.begin(9600);
}

void setLED(byte r, byte g, byte b) {
  analogWrite(LEDr_Pin, r);
  analogWrite(LEDg_Pin, g);
  analogWrite(LEDb_Pin, b);
}

void sendBit(byte oct) {
  switch(oct) {
    case 0: setLED(  1,  1,  1); break;
    case 1: setLED(  0,  0,255); break;
    case 2: setLED(  0,255,  0); break;
    case 3: setLED(255,  0,  0); break;
    default: break;
  }
  delay(50);
  setLED(0,0,0);
}

String getInput(String s) {
  Serial.println(s);
  while (Serial.available() == 0) {}
  return Serial.readString();
}

void encode(int c) {
  parityBit = 0;
  for (int i = bitlength-3; i >= 0; i--) {
    bits[i] = c%2;
    parityBit += bits[i];
    c=c/2;
  }
  bits[8] = parityBit%2;
}

void sendByte() {
  Serial.print("Send:");
  for (int i = 0; i<bitlength; i+=2) {
    if        (bits[i]==0 && bits[i+1]==0) {
      sendBit(0);
      Serial.print(0);
    } else if (bits[i]==0 && bits[i+1]==1) { 
      sendBit(1);
      Serial.print(1);
    } else if (bits[i]==1 && bits[i+1]==0) { 
      sendBit(2);
      Serial.print(2);
    } else if (bits[i]==1 && bits[i+1]==1) { 
      sendBit(3);
      Serial.print(3);
    }
    delay(10);
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

void loop() {
  message = getInput("Waiting for Message...");
  Serial.print("Message:");
  Serial.println(message);
  sendString(message);
}
