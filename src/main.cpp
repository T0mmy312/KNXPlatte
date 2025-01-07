#include <Arduino.h>
#include <knx_io.h>

#define CS_PIN 5

knx::Blind blind(CS_PIN, 22, 21);

void setup() {
  Serial.begin(115200);
  delay(1000);
  blind.begin();
}

void loop() {
  blind.update();
  delay(100);
}