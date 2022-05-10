#define LEDr_Pin 10
#define LEDg_Pin 9
#define LEDb_Pin 8
#define PR_Pin 0    //PhotoResistor
#define PR_reads 7

byte input, output;
long prAVG;

void setup() {
  pinMode(LEDr_Pin, OUTPUT);
  pinMode(LEDg_Pin, OUTPUT);
  pinMode(LEDb_Pin, OUTPUT);
  pinMode(PR_Pin, INPUT);
  Serial.begin(9600);
}

byte readBit() {
  prAVG = 0;
  for (int i = 0; i<PR_reads; i++ ) prAVG += analogRead(PR_Pin);
  prAVG /= PR_reads;
  switch(prAVG) {
    case   0 ...   2: return -1;  //represented as 255
    case  40 ...  95: return 0;
    case 125 ... 222: return 1;
    case 263 ... 370: return 2;
    case 390 ... 555: return 3;
    case 762 ... 771: return 4;
    case 817 ... 832: return 5;
    case 907 ... 913: return 6;
    case 938 ... 944: return 7;
    default: return readBit();
  }
}

void setLED(byte r, byte g, byte b) {
  analogWrite(LEDr_Pin, r);
  analogWrite(LEDg_Pin, g);
  analogWrite(LEDb_Pin, b);
}

void sendBit(byte oct) {
  switch(oct) {
    case 0: setLED(  0,  0,  1); break;
    case 1: setLED(  1,  0,  0); break;
    case 2: setLED(  1,  2,  1); break;
    case 3: setLED(  6,  0,  0); break;
    case 4: setLED(  0,  0,255); break;
    case 5: setLED(  0,255,  0); break;
    case 6: setLED(255,  0,  1); break;
    case 7: setLED(255,255,255); break;
    default: break;
  }
  delay(30);
}

byte getInput(String s) {
  Serial.println(s);
  while (Serial.available() == 0) {}
  return Serial.parseInt();
}

void loop() {
  input = getInput("Waiting for byte value...");
  Serial.print("Data:");
  Serial.println(input);
  sendBit(input);
  output = readBit();
  Serial.print("Read:");
  Serial.println(output);
  setLED(0,0,0);
}
