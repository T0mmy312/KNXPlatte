#ifndef _DOTMATRIX_H_
#define _DOTMATRIX_H_

#include <Arduino.h>
#include <MD_MAX72xx.h>

class Dotmatrix : public MD_MAX72XX {
public:
    Dotmatrix(uint8_t cs_pin, uint8_t numDevices = 1) : MD_MAX72XX(MD_MAX72XX::FC16_HW, cs_pin, numDevices) {}

    bool setPoint(uint8_t r, uint16_t c, bool state);
    void drawLine(float x1, float y1, float x2, float y2, bool color = true);
};

#endif