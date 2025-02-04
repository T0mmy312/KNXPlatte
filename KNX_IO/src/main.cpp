#include <Arduino.h>
#include <knx_io.h>
#include <ledStrip.h>
#include <speaker.h>

#define CS_PIN 25

#define ON_PIN 18
#define AUTO_PIN 19
#define OUT_PIN 27

#define UP_PIN 32
#define DOWN_PIN 33

//knx::Blind blind(CS_PIN, 22, 21);

knx::Door door(CS_PIN, 27, LED_BUILTIN, 35);

//knx::Weather weather(0, 10, ON_PIN, AUTO_PIN, OUT_PIN, globalLedStrip.Color(0, 0, 255), globalLedStrip.Color(0, 0, 0), 5000, 60, 200);

//knx::GarageDoor garageDoor(CS_PIN, OUT_PIN, UP_PIN, DOWN_PIN, 2000);

//knx::Window window(CS_PIN, OUT_PIN, UP_PIN, DOWN_PIN, 1000);

DfMp3 dfmp3(Serial2);

void setup() {
  Serial.begin(115200);
  randomSeed(analogRead(0));
  delay(1000);

  //dfmp3.begin();
  //dfmp3.reset();
  //dfmp3.setVolume(12);
  //dfmp3.playFolderTrack(1, 1); // sd:/01/001.mp3
  //garageDoor.begin();
  //window.begin();
  door.begin();
  //globalLedStrip.begin();
  //weather.begin();
  //blind.begin();
}

void loop() {
  //dfmp3.loop();
  //garageDoor.update();
  //window.update();
  door.update();
  //blind.update();
  //weather.update();
  delay(100);
}