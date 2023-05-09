#include <WiFi.h>

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println(WiFi.macAddress());
}

void loop() {
  // put your main code here, to run repeatedly:

}
