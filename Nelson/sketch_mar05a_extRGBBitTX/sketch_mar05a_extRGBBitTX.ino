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
    case 0: setLED(  1,  1,  1); break;
    case 1: setLED(  0,  0,255); break;
    case 2: setLED(  0,255,  0); break;
    case 3: setLED(255,  0,  0); break;
    default: break;
  }
  delay(50);
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
