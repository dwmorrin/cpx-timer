#include <Arduino.h>

const uint8_t LED_PIN = 13;

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LED_PIN, !digitalRead(LED_PIN));
  delay(500);
}