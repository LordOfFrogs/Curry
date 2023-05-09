#include <PS4Controller.h>
#include <Servo.h>

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
#define ESP32_MAC "E0:5A:1B:AC:6D:0C"


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
  PS4.begin(ESP_MAC);

  // controller init
  Serial.print("Waiting for bluetooth connection...");
  while (!PS4.isConnected());
  Serial.println("PS4 controller Connected!");

  // servos
  eyesServo.attach(EYES_SERVO_PIN);
  headServo.attach(HEAD_SERVO_PIN);
  swivelServo.attach(SWIVEL_SERVO_PIN);

  eyesServo.write(EYES_NEUTRAL);
  headServo.write(HEAD_NEUTRAL);
  swivelServo.write(SWIVEL_NEUTRAL);
}

void loop() {
  if(PS4.connected()) {
    updateServoPositions();
  }

  writeToServos();
}

void updateServoPositions() {
  headPos += PS4.data.analog.stick.ly;
    headPos = constrain(headPos, HEAD_MIN, HEAD_MAX);

    swivelPos += PS4.data.analog.stick.lx;
    swivelPos = constrain(swivelPos, SWIVEL_MIN, SWIVEL_MAX);

    if (PS4.data.button.cross) {
      eyesPos = EYES_HAPPY;
    }
    if(PS4.data.button.circle) {
      eyesPos = EYES_CLOSED;
    }
    if(PS4.data.button.square) {
      eyesPos = EYES_NEUTRAL;
    }

    if(PS4.data.button.l1 && PS4.data.button.r1) {
      headPos = HEAD_NEUTRAL;
      swivelPos = SWIVEL_NEUTRAL;
      eyesPos = EYES_NEUTRAL;
    }
}

void writeToServos() {
  headServo.write(headPos);
  swivelServo.write(swivelPos);
  eyesServo.write(eyesPos);
}
