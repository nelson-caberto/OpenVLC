#define lampPin 8
#define bitlength 9

bool bits[bitlength] = {0,0,0,0,0,0,0,0,0};
int value;
int parityBit = 0;

void setup() 
{
  pinMode(lampPin, OUTPUT);
  Serial.begin(9600);
}

void encode(int num) {
  parityBit = 0;
  for (int i = bitlength-2; i >= 0; i--) {
    bits[i] = num%2;
    parityBit += bits[i];
    num=num/2;
  }
  bits[8] = parityBit%2;
}

void showBits() {
  Serial.print("Data:");
  for (int i = 0; i<bitlength-1; i++){
    Serial.print(bits[i]);
  }
  Serial.println();
}

void sendBits() {
  Serial.print("Send:");
  for (int i = 0; i<bitlength; i++) {
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
    delay(320);
    analogWrite(lampPin,0);
    delay(310);
  }
  Serial.println();
}

void loop() 
{
  Serial.println("Waiting for input...");
  while (Serial.available() == 0) {}
  value = Serial.parseInt();
  Serial.print("Input:");
  Serial.println(value);
  encode(value);
  showBits();
  sendBits();
}  
