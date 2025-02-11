#include <Arduino.h>
#include <knx_io.h>
#include <defaultConfig.h>

void setup() {
    Serial.begin(115200);
    delay(500);

    Serial.println("Starting init!");
    if (!initKNX()) {
        Serial.println("Errors occurred in initialization!");
        Serial.println("Stopping Script!");
        while (true) {}
    }
    Serial.println("Initialization successful!");
}

void loop() {
    // loop code here:

    // update all parts:
    updateAll();
}