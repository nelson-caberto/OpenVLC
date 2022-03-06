//define pins
#define lightPin 0
#define LEDr_Pin 10
#define LEDg_Pin 9
#define LEDb_Pin 8
//define variables
int bitValue;
int num;
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
  Serial.println(prompt); //prompt for message
  while (Serial.available() == 0) {
    // Wait for User to Input Data
  }

  //store user input
  message = Serial.readString(); //write input to variable
  Serial.println(confirm); //print confirm to Serial
  Serial.println(message); //print message to Serial

  //define local varables
  int bits[8] = {};
  int recievedBitsArray[8] = {};
  int txBits[8] = {};
  
  //Print Message length in terms of character
  num = message.length();
  Serial.println("Sent a character length of: ");
  Serial.println(num);
  
  //Convert message length from Integer to Binary 
  for (int i = 7; i >= 0; i--) {
    txBits[i] = num%2;
    num=num/2;
  }
 for(int i = 0; i<8; i++){
    //Read converted binary number from user
    bitValue = txBits[i];
        
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

  delay (50);
  
  for(int j=0;j<message.length(); j++){ //loop reads message from left to right
    char character = message.charAt(j); //each character is assigned to the character variable
    Serial.println("Sent A bit Value of:" );

    for(int i=7; i>=0; i--){ //looping from i=7 to i=0 to read octet from left to right
      bits[i] = bitRead(character,i); //returns a 1 or a 0 for each bit of the octet
      Serial.print(bits[i]); //prints the ith bit to the Serial Monitor
      //for(int i = 0; i< 8; i++){
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
      //Serial.println(reading);
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
        //Serial.println(recievedBitsArray[i]);

      delay(200);
      setLED(0, 0, 0);  
    }
    Serial.println();
    Serial.println("Received A bit Value of:" );

  for(int i = 7; i >=0; i--){
    Serial.print(recievedBitsArray[i]);
  }
  Serial.println();

  }
}
