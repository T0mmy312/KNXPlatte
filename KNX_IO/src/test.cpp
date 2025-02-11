#include <Arduino.h>
#include <knx_io.h>
#include <ledStrip.h>
#include <speaker.h>

#define CS_PIN 17

#define ON_PIN 18
#define AUTO_PIN 19
#define OUT_PIN 27

#define UP_PIN 32
#define DOWN_PIN 33

#define HEATING_PIN 16
#define COOLING_PIN 17

Dotmatrix globalDotmatrix(CS_PIN, 5);

//LocalDotmatrix ld(0, 2, 8, 8, &globalDotmatrix);

//knx::Blind blind(0, 8, 22, 21);

//knx::Door door(CS_PIN, UP_PIN, LED_BUILTIN, DOWN_PIN);

//knx::Weather weather(0, 10, ON_PIN, AUTO_PIN, OUT_PIN, globalLedStrip.Color(0, 0, 255), globalLedStrip.Color(0, 0, 0), 5000, 60, 200);

//knx::GarageDoor garageDoor(CS_PIN, OUT_PIN, UP_PIN, DOWN_PIN, 2000);

//knx::Window window(CS_PIN, OUT_PIN, UP_PIN, 1000);

//DfMp3 dfmp3(Serial2);

//knx::Heater heater(0, 10, HEATING_PIN, COOLING_PIN, 0, 43691, 1000);

void setup() {
  Serial.begin(115200);
  randomSeed(analogRead(0));
  delay(1000);

  //globalLedStrip.begin();
  globalDotmatrix.begin();

  //dfmp3.begin();
  //dfmp3.reset();
  //dfmp3.setVolume(12);
  //dfmp3.playFolderTrack(1, 1); // sd:/01/001.mp3
  //garageDoor.begin();
  //window.begin();
  //door.begin();
  //weather.begin();
  //blind.begin();
  //heater.begin();
}

void loop() {
  //dfmp3.loop();
  //garageDoor.update();
  //window.update();
  //door.update();
  //blind.update();
  //weather.update();
  //heater.update();
  delay(10);
}