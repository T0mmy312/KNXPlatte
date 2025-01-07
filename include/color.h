#ifndef _COLOR_H_
#define _COLOR_H_

#include <Esp.h>

class Color {
public:
    Color(uint8_t r = 0, uint8_t g = 0, uint8_t b = 0) 
        : r(r), g(g), b(b) {}

    uint8_t r, g, b;

    operator uint32_t() {
        uint32_t out = 0xFF;
        ((uint8_t*)&out)[1] = r;
        ((uint8_t*)&out)[2] = g;
        ((uint8_t*)&out)[3] = b;
        return out; 
    }
};

#endif