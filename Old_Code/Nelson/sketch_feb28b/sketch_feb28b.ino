/* This is automated testing the Super Bright RGB with PhotoResistor */

#define LEDr_Pin 10
#define LEDg_Pin 9
#define LEDb_Pin 8
#define PR_Pin 0    //PhotoResistor
#define LED_INCREMENT 5
#define LED_INCREMENT_MAX 100   //range is 0-255
#define PR_ITER 5000

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
  prMIN = 1100;
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

void incrementRGB() {
  if (b<LED_INCREMENT_MAX-(LED_INCREMENT_MAX%LED_INCREMENT)) {
    b+=LED_INCREMENT;
    return;
  }
  b = 0;
  if (g<LED_INCREMENT_MAX-(LED_INCREMENT_MAX%LED_INCREMENT)) {
    g+=LED_INCREMENT;
    return;
  }
  g = 0;
  if (r<LED_INCREMENT_MAX-(LED_INCREMENT_MAX%LED_INCREMENT)) {
    r+=LED_INCREMENT;
    return;
  }
  r = 0;
  setLED();
  while(true);
}

void loop() {
  setLED();
  delay(50);
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
  incrementRGB();
}
