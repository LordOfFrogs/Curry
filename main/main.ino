#include <PS4BT.h>;
#include <usbhub.h>;
#include <Servo.h>;

#define EYES_SERVO_PIN 3
#define HEAD_SERVO_PIN 2
#define SWIVEL_SERVO_PIN 1

#define EYES_HAPPY 5
#define EYES_NEUTRAL 15
#define EYES_CLOSED 35
#define HEAD_MAX 95
#define HEAD_NEUTRAL 75
#define HEAD_MIN 50
#define SWIVEL_MAX 110
#define SWIVEL_NEUTRAL 60
#define SWIVEL_MIN 20

USB usb;
BTD Btd(&usb);
PS4BT PS4(&Btd, PAIR);
Servo eyesServo;
Servo headServo;
Servo swivelServo;

int error = 0;
byte type = 0;
byte vibrate = 0;

int headPos = HEAD_NEUTRAL;
int swivelPos = SWIVEL_NEUTRAL;
int eyesPos = EYES_NEUTRAL;

void setup() {
  Serial.begin(9600);

  // bluetooth init
  if (usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
  }
  Serial.print(F("\r\nPS4 Bluetooth Library Started"));

  // servos
  eyesServo.attach(EYES_SERVO_PIN);
  headServo.attach(HEAD_SERVO_PIN);
  swivelServo.attach(SWIVEL_SERVO_PIN);

  eyesServo.write(EYES_NEUTRAL);
  headServo.write(HEAD_NEUTRAL);
  swivelServo.write(SWIVEL_NEUTRAL);
}

void loop() {
  usb.Task();
  
  if(PS4.connected()) {
    headPos += PS4.getAnalogHat(LeftHatY);
    headPos = constrain(headPos, HEAD_MIN, HEAD_MAX);

    swivelPos += PS4.getAnalogHat(LeftHatX);
    swivelPos = constrain(swivelPos, SWIVEL_MIN, SWIVEL_MAX);

    if (PS4.getButtonClick(CROSS)) {
      eyesPos = EYES_HAPPY;
    }
    if(PS4.getButtonClick(CIRCLE)) {
      eyesPos = EYES_CLOSED;
    }
    if(PS4.getButtonClick(SQUARE)) {
      eyesPos = EYES_NEUTRAL;
    }

    if(PS4.getButtonPress(L1) && PS4.getButtonPress(R1)) {
      headPos = HEAD_NEUTRAL;
      swivelPos = SWIVEL_NEUTRAL;
      eyesPos = EYES_NEUTRAL;
    }
  }

  headServo.write(headPos);
  swivelServo.write(swivelPos);
  eyesServo.write(eyesPos);
}
