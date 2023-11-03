#include "CODBOTS_UltraSonic.h"

//ultrasonic
#define ULTRA_TRIG 23
#define ULTRA_ECHO 22

CODBOTS_UltraSonic usensor(ULTRA_TRIG,ULTRA_ECHO);
void setup() {
  Serial.begin(115200);
  usensor.begin(10);
}

void loop() {
  usensor.read();
  Serial.println(usensor.getDistance());
  delay(10);
}
