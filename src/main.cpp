#include <Arduino.h>
#include <knx_io.h>
#include <ledStrip.h>

#define CS_PIN 5

//knx::Blind blind(CS_PIN, 22, 21);

//knx::Door door(CS_PIN, 4, LED_BUILTIN, 0);

LEDSegment strip(1, 9);

void setup() {
  Serial.begin(115200);
  randomSeed(analogRead(0));
  delay(1000);
  //door.begin();
  globalLedStrip.begin();
  strip.clear();
  strip.show();
  //blind.begin();
}

void loop() {
  //door.update();
  //blind.update();
  delay(500);
}