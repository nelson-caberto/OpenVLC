#define photoPin 0
#define lampPin 8

int byteValue;

void setup() {
  // put your setup code here, to run once:
//  pinMode(lampPin, OUTPUT);
//  pinMode(photoPin, INPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Enter A Value:");
  while (Serial.available() == 0) {}
  int byteValue = Serial.parseInt();
  Serial.println(byteValue);
 
}
