#define MOSFETsignal 2
#define LED_Pin 8

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(MOSFETsignal, OUTPUT);
  pinMode(LED_Pin, OUTPUT);


}

void loop() {
  // put your main code here, to run repeatedly:
 digitalWrite(LED_Pin, 1); //turn on voltage to the LED
 digitalWrite(MOSFETsignal, 1); //Turn on full brightness
 delay(10000);
 digitalWrite(MOSFETsignal, 0); //Turn on dimmed brightness
 delay(10000);
 digitalWrite(LED_Pin, 0); //Turn off LED
 delay(5000);

}
