#include <CODBOTS_SmoothServo.h>
#include <CODBOTS_UltraSonic.h>
//ultrasonic
#define PIN_ULTRA_TRIG 23
#define PIN_ULTRA_ECHO 22
//servo
#define PIN_SERVO 18


CODBOTS_SmoothServo servo(PIN_SERVO);
CODBOTS_UltraSonic usensor(PIN_ULTRA_TRIG, PIN_ULTRA_ECHO);

void setup() {
  Serial.begin(115200);
  servo.begin();

  usensor.begin(0, 180, 12, 10);
  usensor.reset();

  servo.write(90); 
}

void loop() {
  if (servo.isReached()) {
    servo.write(usensor.getAngle());
  }
  usensor.scan(servo.isReached());

}
