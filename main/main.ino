#include <PS4Controller.h>
#include <ESP32Servo.h>

const int EYES_SERVO_PIN = 18;
const int HEAD_SERVO_PIN = 19;
const int SWIVEL_SERVO_PIN = 21;

const int MOTOR1_1 = 27;
const int MOTOR1_2 = 26;
const int MOTOR1_ENABLE = 14;
const int MOTOR2_1 = 25;
const int MOTOR2_2 = 33;
const int MOTOR2_ENABLE = 32;

const int PWM_FREQ = 30000;
const int PWM1_CHANNEL = 0;
const int PWM2_CHANNEL = 0;
const int PWM_RESOLUTION = 8;

const int EYES_HAPPY = 17;
const int EYES_NEUTRAL = 25;
const int EYES_CLOSED = 50;
const int HEAD_MAX = 95;
const int HEAD_NEUTRAL = 75;
const int HEAD_MIN = 50;
const int SWIVEL_MAX = 110;
const int SWIVEL_NEUTRAL = 60;
const int SWIVEL_MIN = 20;

const char* ESP32_MAC = "E0:5A:1B:AC:6D:0C";

const int DEADZONE = 30;


Servo eyesServo;
Servo headServo;
Servo swivelServo;

int error = 0;
byte type = 0;
byte vibrate = 0;

int headPos = HEAD_NEUTRAL;
int swivelPos = SWIVEL_NEUTRAL;
int eyesPos = EYES_NEUTRAL;

double t1 = 0;

void setup() {
  Serial.begin(9600);
  PS4.begin(ESP32_MAC);

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

  //motors
  pinMode(MOTOR1_1, OUTPUT);
  pinMode(MOTOR1_2, OUTPUT);
  pinMode(MOTOR1_ENABLE, OUTPUT);
  pinMode(MOTOR2_1, OUTPUT);
  pinMode(MOTOR2_2, OUTPUT);
  pinMode(MOTOR2_ENABLE, OUTPUT);

  ledcSetup(PWM1_CHANNEL, PWM_FREQ, PWM_RESOLUTION);
  ledcSetup(PWM2_CHANNEL, PWM_FREQ, PWM_RESOLUTION);

  ledcAttachPin(MOTOR1_ENABLE, PWM1_CHANNEL);
  ledcAttachPin(MOTOR2_ENABLE, PWM2_CHANNEL);
}

void loop() {
  if(PS4.isConnected()) {
    updateServoPositions();
    drive();
  }
  writeToServos();
  delay(50);
}

void updateServoPositions() {
  double dt = double(millis()) - t1;
  t1 = millis();
  int ly = PS4.LStickY();
  ly = (abs(ly) < DEADZONE) ? 0 : ly;

  int lx = PS4.LStickX();
  lx = (abs(lx) < DEADZONE) ? 0 : lx;
  headPos += ly * (dt / 1000.0);
  headPos = constrain(headPos, HEAD_MIN, HEAD_MAX);

  swivelPos += lx * (dt / 1000.0);
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
}

void drive() {
  double x = PS4.RStickX();
  x = (abs(x) < DEADZONE) ? 0 : x;
  x *= 2; // maps x from [-128,128) to [-255,255]
  double y = PS4.RStickY();
  y = (abs(y) < DEADZONE) ? 0 : y;
  y *= 2; // maps y from [-128,128) to [-255,255]

  // tank drive calculations (https://home.kendra.com/mauser/joystick.html)
  double v = (255-abs(x))*(y/255)+y;
  double w = (255-abs(y))*(x/255)+x;
  double rDrive = (v+w)/2;
  double lDrive = (v-w)/2;
  //write to motors
  ledcWrite(PWM1_CHANNEL, lDrive);
  ledcWrite(PWM2_CHANNEL, rDrive);
  //set directions
  digitalWrite(MOTOR1_1, (lDrive >= 0) ? HIGH : LOW);
  digitalWrite(MOTOR1_2, (lDrive >= 0) ? LOW : HIGH);

  digitalWrite(MOTOR2_1, (rDrive >= 0) ? HIGH : LOW);
  digitalWrite(MOTOR2_2, (rDrive >= 0) ? LOW : HIGH);
}
