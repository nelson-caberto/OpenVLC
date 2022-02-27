#define lampPin 8

bool bits[8] = {0,0,0,0,0,0,0,0};
int value;

void setup() 
{
  pinMode(lampPin, OUTPUT);
  Serial.begin(9600);
}

void encode(int num) {
  for (int i = 8; i > 0; i--) {
    bits[i] = num%2;
    num=num/2;
  }
}

void showBits() {
  Serial.print("Data:");
  for (int i = 0; i<8; i++){
    Serial.print(bits[i]);
  }
  Serial.println();
}

void sendBits() {
  Serial.print("Send:");
  for (int i = 0; i<8; i++) {
    switch((int)bits[i]) {
      case 0:
        analogWrite(lampPin,5);
      break;
      case 1:
        analogWrite(lampPin,255);
      break;
      default:
      break;
    }
    Serial.print(bits[i]);
    delay(310);
    analogWrite(lampPin,0);
    delay(310);
  }
  Serial.println();
}

void loop() 
{
  Serial.println("Value:");
  while (Serial.available() == 0) {}
  value = Serial.parseInt();
  encode(value);
  showBits();
  sendBits();
}  
