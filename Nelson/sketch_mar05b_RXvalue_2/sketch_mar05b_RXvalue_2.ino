#define PR_Pin 0    //PhotoResistor
#define PR_reads 3000

int prMINcurrent,
    prMINprevious,
    prMINavg,
    prMAXcurrent,
    prMAXprevious,
    prMAXavg,
    currentValue,
    prREADs = 1;
long prAVGprevious = 0, prAVGcurrent;
unsigned long startTime, endTime, elapsedTime;

void setup() {
  pinMode(PR_Pin, INPUT);
  Serial.begin(9600);
}

void loop() {
  for (int i = 0; i < 200; i++) {
    prAVGcurrent = 0;
    prMINcurrent = prMINprevious = 1100;
    prMAXcurrent = prMAXprevious = 0;
    for (int i = 0; i<prREADs; i++ ) {
      currentValue = analogRead(PR_Pin);
      if (currentValue>prMAXcurrent) {
        prMAXprevious = prMAXcurrent;
        prMAXcurrent = currentValue;
      }
      if (currentValue<prMINcurrent) {
        prMINprevious = prMINcurrent;
        prMINcurrent = currentValue;
      }
      prAVGcurrent += currentValue;
    }
    prAVGcurrent /= prREADs;
    prMAXavg /= prREADs;
    prMINavg /= prREADs;
    switch(abs(prAVGprevious-prAVGcurrent)) {
      case  5 ...  10: prREADs+= 1; break;
      case 11 ...  20: prREADs+=10; break;
      case 21 ...  30: prREADs+=20; break;
      default: break;
    }
    if (prMINprevious == prMINcurrent && prMAXprevious == prMAXcurrent && prAVGprevious == prAVGcurrent)
      prREADs = 1;
    prAVGprevious = prAVGcurrent;
    startTime = micros();
    for (int i = 0; i<prREADs; i++) {
      currentValue = analogRead(PR_Pin);
    }
    endTime = micros();
    elapsedTime = endTime-startTime;
    Serial.print("prMIN:\t");
    Serial.print(prMINcurrent);
    Serial.print("\tprAVG:\t");
    Serial.print(prAVGcurrent);
    Serial.print("\tprMAX:\t");
    Serial.print(prMAXcurrent);
    Serial.print("\tuSec:\t");
    Serial.print(elapsedTime);
    Serial.print("\tprREADs:\t");
    Serial.println(prREADs);   
  }
  while(true);
}
