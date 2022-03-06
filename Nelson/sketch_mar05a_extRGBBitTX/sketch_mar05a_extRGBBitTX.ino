#define LEDr_Pin 10
#define LEDg_Pin 9
#define LEDb_Pin 8

byte tx;

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
    case 0: setLED(  0,  0,  1); break;
    case 1: setLED(  1,  0,  0); break;
    case 2: setLED(  1,  2,  1); break;
    case 3: setLED(  6,  0,  0); break;
    case 4: setLED(  0,  0,255); break;
    case 5: setLED(  0,255,  0); break;
    case 6: setLED(255,  0,  1); break;
    case 7: setLED(255,255,255); break;
    default: break;
  }
  delay(35);
  setLED(0,0,0);
}

byte getInput(String s) {
  Serial.println(s);
  while (Serial.available() == 0) {}
  return Serial.parseInt();
}

void loop() {
  tx = getInput("Waiting for byte value...");
  Serial.print("Data:");
  Serial.println(tx);
  sendBit(tx);
}
