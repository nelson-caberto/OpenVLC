/* This is simple testing the Super Bright RGB */

#define LEDr_Pin 10
#define LEDg_Pin 9
#define LEDb_Pin 8
#define PR_Pin 0    //PhotoResistor

byte inputR, inputG, inputB;

void setup() {
  pinMode(LEDr_Pin, OUTPUT);
  pinMode(LEDg_Pin, OUTPUT);
  pinMode(LEDb_Pin, OUTPUT);
  pinMode(PR_Pin, INPUT);
  Serial.begin(9600);
}

void setLED(byte r, byte g, byte b) {
  analogWrite(LEDr_Pin, r);
  analogWrite(LEDg_Pin, g);
  analogWrite(LEDb_Pin, b);
}

byte getInput(String s) {
  Serial.println(s);
  while (Serial.available() == 0) {}
  return Serial.parseInt();
}

void loop() {
  inputR = getInput("Waiting for RED value...");
  inputG = getInput("Waiting for GREEN value...");
  inputB = getInput("Waiting for BLUE value...");
  setLED(inputR, inputG, inputB);
}
