//define pins
#define lightPin 0
#define LEDr_Pin 10
#define LEDg_Pin 9
#define LEDb_Pin 8
//define variables
int num;
int bitValue;
const int reset = 0;
int input;
int reading;
int recievedBit;
String prompt = "What is your message?"; //initiate prompt
String message; //initiate variable
String confirm = "Your message is "; //repeats message
void setup() {
  // put your setup code here, to run once:
  pinMode(LEDr_Pin, OUTPUT);
  pinMode(LEDg_Pin, OUTPUT);
  pinMode(LEDb_Pin, OUTPUT);
  pinMode(lightPin, INPUT);
  Serial.begin(9600);


}
void setLED(byte r, byte g, byte b) {
  analogWrite(LEDr_Pin, r);
  analogWrite(LEDg_Pin, g);
  analogWrite(LEDb_Pin, b);
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
        setLED(0, 255, 0);  
    }  
    else if(bitValue == 1) {
         setLED(255, 0, 0);  

     }
    
    else {
      Serial.println("Number not recognized");
    }  
    delay(50);
    
    //Read Value from Photo Resistor
    reading = analogRead(lightPin);
      //Decipher Value into bit value of 0 or 1
      if(200 <= reading && reading <= 400){
       recievedBit = 0;
      }
      else if(800 <= reading && reading <= 900){
       recievedBit = 1;
      }
      else {
      }

 recievedBitsArray[i]=recievedBit;
 delay(200);
 setLED(0, 0, 0);  

}
Serial.println("Received A bit Value of:" );

  for(int i = 0; i < 8 ; i++){
    Serial.print(recievedBitsArray[i]);
  }
  Serial.println();

  if (input>255) {input = 0;}
  delay (50);
}
