#ifndef _DOTMATRIX_H_
#define _DOTMATRIX_H_

#include <Arduino.h>
#include <MD_MAX72xx.h>

// D_in = SPI MOSI, CLK = SPI CLK
// esp32 defaults: SPI MOSI = G23, SPI CLK = G18

class Dotmatrix : public MD_MAX72XX {
public:
    Dotmatrix(uint8_t cs_pin, uint8_t numDevices = 1) : MD_MAX72XX(MD_MAX72XX::FC16_HW, cs_pin, numDevices) {}

    bool setPoint(uint8_t r, uint16_t c, bool state);
    void drawLine(float x1, float y1, float x2, float y2, bool color = true);
    void circle(uint8_t mx, uint8_t my, bool color, float radius, float angleStart = 0, float angleStop = 2 * PI); // in rads
};

#endif