int num;
int input;
void setup() {
  // put your setup code here, to run once:
Serial.begin(9600);
}

void loop() {
  Serial.println("Enter A Value?"); //Prompt User for Input
  while (Serial.available() == 0) {
    // Wait for User to Input Data
  }
  input = Serial.parseInt();
  num = input;
  int bits[8] = {};
  for (int i = 8; i > 0; i--) {
    bits[i] = num%2;
    num=num/2;
  }
  Serial.print("Num in Dec = "); 
  Serial.print(input); 
  Serial.print(" is "); 
  
  for(int i = 1; i<9; i++){
    Serial.print( bits[i]);
  }
  Serial.println(" in Binary");
  input++;
  
  if (input>255) {input = 0;}
  delay (200);
}
