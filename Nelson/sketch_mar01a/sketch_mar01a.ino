/*
 * PhotoDiode
 * Super Bright LED
 * 
 * Exploration
 * 
 */

#define PD_Pin 0
#define LED_Pin 8

int pd = 0;

void setup() {
  pinMode(PD_Pin, INPUT);
  pinMode(LED_Pin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  while(true);
//  for (int i = 0; i<256; i++) {
    analogWrite(LED_Pin,128);
    delay(100);
    for (int j = 0; j<10; j++) {
      pd = analogRead(PD_Pin);
//      Serial.print(i);
      Serial.print(" ");
      Serial.println(pd);
    }
//  }
}

/*
 * Results:
 * 
 * PhotoDiode range is about 30-335.
 * 
 * PhotoDiode is fast enough to detect
 * zero volts from LED because of PWM.
 * 
 * Need to eliminate PWM by using MOSFETs.
 * 
 */
