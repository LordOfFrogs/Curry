#include <Servo.h>
#include <PS4Controller.h>


static const int eyesServoPin = 18;
static const int headServoPin = 19;
static const int swivelServoPin = 21;

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


int xSpeedVal;
int ySpeedVal;
int yVal;
int xVal;
int posDegrees;
int yMove;
int xMove;

int motor1Pin1 = 27; 
int motor1Pin2 = 26; 
int enable1Pin = 14;
int motor2Pin1 = 25; 
int motor2Pin2 = 33; 
int enable2Pin = 32;

const int freq = 30000;
const int pwm1Channel = 0;
const int pwm2Channel = 0;
const int resolution = 8;


void setup() {
 eyesServo.attach(eyesServoPin);
  headServo.attach(headServoPin);
  swivelServo.attach(swivelServoPin);

  eyesServo.write(EYES_NEUTRAL);
  headServo.write(HEAD_NEUTRAL);
  swivelServo.write(SWIVEL_NEUTRAL);

  Serial.begin(9600);
  PS4.begin(ESP32_MAC);

  // controller init
  Serial.print("Waiting for bluetooth connection...");
  while (!PS4.isConnected());
  Serial.println("PS4 controller Connected!");

  // servos
 

  


  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(enable1Pin, OUTPUT);
  pinMode(motor2Pin1, OUTPUT);
  pinMode(motor2Pin2, OUTPUT);
  pinMode(enable2Pin, OUTPUT);

  ledcSetup(pwm1Channel, freq, resolution);
  ledcSetup(pwm2Channel, freq, resolution);
  
  ledcAttachPin(enable1Pin, pwm1Channel);
  ledcAttachPin(enable2Pin, pwm2Channel);

 
}




void loop() {
  //if(PS4.isConnected()) {
    //updateServoPositions();
    //driveSpeedValues();
   
  //}
  //driveControl();
 // writeToServos();
if(PS4.Circle()){
  eyesServo.write(35);
}
if(PS4.Square()){
  eyesServo.write(5);
}
 
}

/*int updateServoPositions() {
 int headPos = map(PS4.LStickY(), -1, 1, HEAD_MIN, HEAD_MAX);
  //headPos += yMove;
    //headPos = constrain(headPos, HEAD_MIN, HEAD_MAX);

  int xMove = map(PS4.LStickX(), -1, 1, -10, 10);
    swivelPos += xMove;
    swivelPos = constrain(swivelPos, SWIVEL_MIN, SWIVEL_MAX);

    if (PS4.Cross()) {
      eyesPos = EYES_HAPPY;
    }
    if(PS4.Circle()) {
      eyesPos = EYES_CLOSED;
    }
    if(PS4.Square()) {
      eyesPos = EYES_NEUTRAL;
    }

    if(PS4.L1() && PS4.R1()) {
      headPos = HEAD_NEUTRAL;
      swivelPos = SWIVEL_NEUTRAL;
      eyesPos = EYES_NEUTRAL;
    }
}

void writeToServos() {
  headServo.write(headPos);
  swivelServo.write(swivelPos);
  eyesServo.write(eyesPos);
}*/
int driveSpeedValues(){
  int xVal =  PS4.RStickX();
   xSpeedVal = map(xVal, -1, 1, -255, 255);
  int yVal =  PS4.RStickY();
   ySpeedVal = map(yVal, -1, 1, -255, 255);
}

int driveControl() {
  if (yVal > 0 && xVal > 0) {
    ledcWrite(pwm1Channel, xSpeedVal);
    digitalWrite (motor1Pin1, LOW);
    digitalWrite (motor1Pin2, HIGH);
    ledcWrite(pwm2Channel, xSpeedVal - yVal);
    digitalWrite (motor2Pin1, LOW);
    digitalWrite (motor2Pin2, HIGH);
  }
  if (yVal < 0 && xVal > 0) {
    ledcWrite(pwm1Channel, xSpeedVal);
    digitalWrite (motor1Pin1, LOW);
    digitalWrite (motor1Pin2, HIGH);
    ledcWrite(pwm2Channel, xSpeedVal + yVal);
    digitalWrite (motor2Pin1, LOW);
    digitalWrite (motor2Pin2, HIGH);
  }

  if (yVal > 0 && xVal < 0) {
    ledcWrite(pwm1Channel, -xSpeedVal);
    digitalWrite (motor1Pin1, HIGH);
    digitalWrite (motor1Pin2, LOW);
    ledcWrite(pwm2Channel, -xSpeedVal - yVal);
    digitalWrite (motor2Pin1, HIGH);
    digitalWrite (motor2Pin2, LOW);
  }

  if (yVal < 0 && xVal < 0) {
    ledcWrite(pwm1Channel, -xSpeedVal);
    digitalWrite (motor1Pin1, HIGH);
    digitalWrite (motor1Pin2, LOW);
    ledcWrite(pwm2Channel, -xSpeedVal + yVal);
    digitalWrite (motor2Pin1, HIGH);
    digitalWrite (motor2Pin2, LOW);
  }

  if (PS4.R3() && yVal < 0) {
    ledcWrite(pwm1Channel, -ySpeedVal);
    digitalWrite (motor1Pin1, LOW);
    digitalWrite (motor1Pin2, HIGH);
    ledcWrite(pwm2Channel, -ySpeedVal);
    digitalWrite (motor2Pin1, HIGH);
    digitalWrite (motor2Pin2, LOW);
  }

  if (PS4.R3() && yVal > 0) {
  ledcWrite(pwm1Channel, ySpeedVal);
    digitalWrite (motor1Pin1, HIGH);
    digitalWrite (motor1Pin2, LOW);
    ledcWrite(pwm2Channel, ySpeedVal);
    digitalWrite (motor2Pin1, LOW);
    digitalWrite (motor2Pin2, HIGH);
  }
}




