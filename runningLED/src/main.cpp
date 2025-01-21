#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#define NEOPIXLE_PIN 25
#define NUM_LED 30
#define DELAYTIME 1000

Adafruit_NeoPixel OUT(NUM_LED,NEOPIXLE_PIN,NEO_GRB + NEO_KHZ800);

uint32_t red = OUT.Color(255,0,0);
uint32_t green = OUT.Color(0, 255, 0);
uint32_t blue = OUT.Color(0,0,255);
uint32_t white = OUT.Color(255,255,255);

uint8_t getRandomNumber(uint8_t min, uint8_t max) {
  //return max;
  return random(min, max + 1);
}

void raining()
{
    for (int i = 0; i < NUM_LED; i++) 
      OUT.setPixelColor(i, 0);

    for (int i = 0; i < getRandomNumber(5,9); i++)
      OUT.setPixelColor(i,blue);
    
    for (int i = getRandomNumber(10,15); i < 20; i++)
      OUT.setPixelColor(i,blue);

    for (int i = 20; i < getRandomNumber(25,29); i++)
      OUT.setPixelColor(i,blue);    

    OUT.show();
  delay(DELAYTIME);
}

void setup() {
  Serial.begin(112500);
  pinMode(14,OUTPUT);
  digitalWrite(14,0);

  OUT.begin();
  randomSeed(analogRead(0));

}

void loop() {  
  raining();
}
