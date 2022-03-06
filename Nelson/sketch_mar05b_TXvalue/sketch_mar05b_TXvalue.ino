#define LEDr_Pin 10
#define LEDg_Pin 9
#define LEDb_Pin 8

byte r, g, b;

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

byte getInput(String s) {
  Serial.println(s);
  while (Serial.available() == 0) {}
  return Serial.parseInt();
}

void loop() {
  r = getInput("Waiting for RED value...");
  g = getInput("Waiting for GREEN value...");
  b = getInput("Waiting for BLUE value...");
  Serial.print(r);
  Serial.print(" ");
  Serial.print(g);
  Serial.print(" ");
  Serial.println(b);
  setLED(r, g, b);
}
