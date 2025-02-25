#define photoPin 0
#define lampPin 8

byte byteValue;
bool tx[8] = {0,0,0,0,0,0,0,0};

void setup() {
  // put your setup code here, to run once:
//  pinMode(lampPin, OUTPUT);
//  pinMode(photoPin, INPUT);
  Serial.begin(9600);
}

void convert() {
  for (int i = 0; i < 8; i++) {
    tx[i] &= byteValue;
    Serial.print(tx[i]);
  }
  Serial.println();
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Enter A Value:");
  while (Serial.available() == 0) {}
  byteValue = Serial.parseInt();
  convert();
  Serial.println(byteValue);
}
