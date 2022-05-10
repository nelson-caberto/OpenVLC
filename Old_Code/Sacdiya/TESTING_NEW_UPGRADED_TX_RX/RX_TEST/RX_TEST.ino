#include <Wire.h>
#include <Adafruit_ADS1X15.h>

Adafruit_ADS1015 ads1015;

void setup(void)
{
  Serial.begin(9600);
  Serial.println("Hello!");
  
  Serial.println("Reading a Photodiode reading");
  ads1015.begin();
}

void loop(void)
{
  int adc0;
  adc0 = ads1015.readADC_SingleEnded(0);
  Serial.print("Reading: "); Serial.println(adc0);
  delay(1000);
}
