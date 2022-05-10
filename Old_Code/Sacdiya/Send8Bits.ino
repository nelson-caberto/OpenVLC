//define pins
#define lampPin 8
#define lightPin 0

//define variables
int num;
int input;
int bitValue;
const int reset = 0;
int dimmingValue;
int reading;
int recievedBit;

void setup() {
  // put your setup code here, to run once:
  pinMode(lampPin, OUTPUT);
  pinMode(lightPin, INPUT);
  Serial.begin(9600);


}

void loop() {
  Serial.println("Enter A Value?"); //Prompt User for Input
  while (Serial.available() == 0) {
    // Wait for User to Input Data
  }

  //store user input
  input = Serial.parseInt();
  num = input;
  int bits[8] = {};
  int recievedBitsArray[8] = {};

  //Convert from Integer to Binary
  for (int i = 7; i >= 0; i--) {
    bits[i] = num%2;
    num=num/2;
  }
  
  Serial.println("Sent A bit Value of:" );

  //Print Converted Binary number
  for(int i = 0; i<8; i++){
    Serial.print( bits[i]);
  }
  Serial.println();
    
  for(int i = 0; i<8; i++){
    //Read converted binary number from user
    bitValue = bits[i];
        
    //Send Bit Value using LED brightness
    if(bitValue == 0){
      dimmingValue = 5;
      analogWrite(lampPin, dimmingValue);
         
    }  
    else if(bitValue == 1) {
      dimmingValue = 255;
      analogWrite(lampPin, dimmingValue);
     }
    
    else {
      Serial.println("Number not recognized");
    }  
    delay(200);
    
    //Read Value from Photo Resistor
    reading = analogRead(lightPin);
    
  //Decipher Value into bit value of 0 or 1
  if(90 <= reading && reading <= 328){
    recievedBit = 0;
  }
  else if(615 <= reading && reading <= 794){
    recievedBit = 1;
  }
  else {
  }

 recievedBitsArray[i]=recievedBit;
 delay(1000);
 analogWrite(lampPin, reset);
}
Serial.println("Received A bit Value of:" );

  for(int i = 0; i < 8 ; i++){
    Serial.print(recievedBitsArray[i]);
  }
  Serial.println();

  if (input>255) {input = 0;}
  delay (200);
}
