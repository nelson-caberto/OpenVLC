#define lightPin 0
#define lampPin 8
int reading;
int dimmingValue;
int recievedBit;
int bitValue;
const int reset=0;


void setup() {
  // put your setup code here, to run once:
  // initialize serial communications at 9600 bps:
  pinMode(lampPin, OUTPUT);
  pinMode(lightPin, INPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  
   Serial.println("Enter A Value?"); //Prompt User for Input
   while (Serial.available() == 0) {
    // Wait for User to Input Data
  }
  bitValue = Serial.parseInt(); //recieve bit value from user
  
  Serial.println("Sent A bit Value of:" );
  Serial.println(bitValue);
  
  //Send Bit Value using LED brightness
  if(bitValue == 0){
    dimmingValue = 5;
    analogWrite(lampPin, dimmingValue);
  }
  else if(bitValue == 1) {
    dimmingValue = 200;
    analogWrite(lampPin, dimmingValue);
    //Serial.println(dimmingValue);

  }
  else {
    Serial.println("Number not recognized");
  }
  
  //Turn LED off
  delay(1000);
  analogWrite(lampPin, reset);

  //Read Value from Photo Resistor
  reading = analogRead(lightPin);

  //Decipher Value into bit value of 0 or 1
  if(106 <= reading && reading <= 328){
    recievedBit = 0;
  }
  else if(615 <= reading && reading <= 794){
    recievedBit = 1;
  }
  else {
    Serial.println("No bit was received");
  }
  
  Serial.println("Recieved a bit value of:");
  Serial.println(recievedBit);

}
