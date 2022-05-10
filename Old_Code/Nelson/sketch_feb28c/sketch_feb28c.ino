/* This is manual testing the Super Bright RGB with PhotoResistor */

#define LEDr_Pin 10
#define LEDg_Pin 9
#define LEDb_Pin 8
#define PR_Pin 0    //PhotoResistor
#define LED_INCREMENT 50
#define PR_ITER 10000

byte r = 0,
     g = 0,
     b = LED_INCREMENT;
int prMIN = 1000, prMAX = 0, prAVG = 0; //PhotoResistor

void setup() {
  pinMode(LEDr_Pin, OUTPUT);
  pinMode(LEDg_Pin, OUTPUT);
  pinMode(LEDb_Pin, OUTPUT);
  pinMode(PR_Pin, INPUT);
  Serial.begin(9600);
}

void getPR() {
  long current = 0, pr = 0;
  prMIN = 1000;
  prMAX = 0;
  for (int i = 0; i<PR_ITER; i++) {
    current = analogRead(PR_Pin);
    if (current>prMAX) prMAX = current;
    if (current<prMIN) prMIN = current;
    pr += current;
  }
  prAVG = pr/PR_ITER;
}

void setLED() {
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
  r = g = b = 0;
  setLED();
  r = getInput("Waiting for RED value...");
  g = getInput("Waiting for GREEN value...");
  b = getInput("Waiting for BLUE value...");
  Serial.println("prMIN\tprAVG\tprMAX\tr\tg\tb");
  for (int i = 0; i<3; i++) {
    setLED();
    delay(300);
    getPR();
    Serial.print(prMIN);
    Serial.print("\t");
    Serial.print(prAVG);
    Serial.print("\t");
    Serial.print(prMAX);
    Serial.print("\t");
    Serial.print(r);
    Serial.print("\t");
    Serial.print(g);
    Serial.print("\t");
    Serial.println(b);
  }
}
