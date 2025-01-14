#include <Arduino.h>
#include <knx_io.h>

#define CS_PIN 5

//knx::Blind blind(CS_PIN, 22, 21);

knx::Door door(CS_PIN, 4, LED_BUILTIN, 0);

void setup() {
  Serial.begin(115200);
  delay(1000);
  door.begin();
  //blind.begin();
}

void loop() {
  door.update();
  //blind.update();
  delay(100);
}