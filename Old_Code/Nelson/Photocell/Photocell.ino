#define photoPin 0
#define lampPin 8
#define samples 2000

int reading;
int lampMIN;
int lampMAX;
long lampAVG;
int brightness = 0;

void setup() 
{
  pinMode(lampPin, OUTPUT);
  Serial.begin(9600);
  Serial.println("lampMIN\tlampMAX\tlampAVG\tLED");
}
void getLampMinMax()
{
  analogWrite(lampPin,brightness);
  lampMIN = 1000;
  lampMAX = 0;
  lampAVG = 0;
  for (int i = 1; i < samples; i++)
  {
    reading = analogRead(photoPin);
    lampAVG += reading;
    if (reading < lampMIN) lampMIN = reading;
    if (reading > lampMAX) lampMAX = reading;
  }
  lampAVG /= samples - 1;
  analogWrite(lampPin,0);
}
void serialout()
{
  Serial.print(lampMIN);
  Serial.print("\t");
  Serial.print(lampMAX);
  Serial.print("\t");
  Serial.print(lampAVG);
  Serial.print("\t");
  Serial.println(brightness);  
}
void loop() 
{
  getLampMinMax();
  serialout();
  brightness++;
  if (brightness > 256) while(true);
}
