#ifndef _LED_STRIP_H_
#define _LED_STRIP_H_

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#define GLOBAL_LED_STRIP_PIN 15
#define GLOBAL_LED_STRIP_NUM_LED 10

//! Caution if global led strip is used globalLedStrip.begin(); must be called before use

// the global led strip is usefull for if you are using a single led strip for all your leds
inline Adafruit_NeoPixel globalLedStrip(GLOBAL_LED_STRIP_NUM_LED, GLOBAL_LED_STRIP_PIN, NEO_GRB + NEO_KHZ800);

inline uint32_t colorRGB(uint8_t r, uint8_t g, uint8_t b) {
    return Adafruit_NeoPixel::Color(r, g, b);
}

inline uint32_t colorHSV(uint16_t h, uint8_t s, uint8_t v) {
    return Adafruit_NeoPixel::ColorHSV(h, s, v);
}

// should be able to set a segment of a total strip, because all led will be in serial
class LEDSegment {
private:
    Adafruit_NeoPixel* _parent;
    uint16_t _startIndex, _lenght; // relative indexing (indexing in overloaded functions is: index + _startIndex)

public:
    LEDSegment(uint16_t startIndex, uint16_t lenght, Adafruit_NeoPixel* parent = &globalLedStrip) 
        : _startIndex(startIndex), _lenght(lenght), _parent(parent) {}

    void setStartIndex(uint16_t startIndex) {_startIndex = startIndex;}
    uint16_t getStartIndex() const {return _startIndex;}
    void setLenght(uint16_t lenght) {_lenght = lenght;}
    uint16_t getLenght() const {return _lenght;}
    void setParent(Adafruit_NeoPixel* parent) {_parent = parent;}

    uint32_t Color(uint8_t r, uint8_t g, uint8_t b);
    void fill(uint32_t c = 0U, uint16_t first = (uint16_t)0U, uint16_t count = (uint16_t)0U);
    void clear() {_parent->fill(0U, _startIndex, _lenght);}
    uint32_t getPixelColor(uint16_t n) const;
    void setPixelColor(uint16_t n, uint32_t c);
    void show();
};

#endif