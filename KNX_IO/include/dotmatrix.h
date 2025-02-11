#ifndef _DOTMATRIX_H_
#define _DOTMATRIX_H_

#include <Arduino.h>
#include <MD_MAX72xx.h>

// D_in = SPI MOSI, CLK = SPI CLK
// esp32 defaults: SPI MOSI = G23, SPI CLK = G18

class Dotmatrix : public MD_MAX72XX {
public:
    Dotmatrix(uint8_t cs_pin, uint8_t numDevices = 1) : MD_MAX72XX(MD_MAX72XX::FC16_HW, cs_pin, numDevices) {}

    bool setPoint(uint8_t x, uint16_t y, bool state);
    void drawLine(float x1, float y1, float x2, float y2, bool color = true);
    void circle(uint8_t mx, uint8_t my, bool color, float radius, float angleStart = 0, float angleStop = 2 * PI); // in rads
    void fillRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, bool color = true);
};

extern Dotmatrix globalDotmatrix;

class LocalDotmatrix {
private:
    Dotmatrix* _parent = &globalDotmatrix;

    uint8_t _topLeftx, _topLefty;
    uint8_t _width, _height;

public:
    LocalDotmatrix(uint8_t topLeftx, uint8_t topLefty, uint8_t width = 8, uint8_t height = 8, Dotmatrix* parent = &globalDotmatrix) : 
        _topLeftx(topLeftx), _topLefty(topLefty), _width(width), _height(height), _parent(parent) {}
    
    uint8_t getTopLeftX() const {return _topLeftx;}
    uint8_t getTopLeftY() const {return _topLefty;}
    uint8_t getWidth() const {return _width;}
    uint8_t getHeight() const {return _height;}
    Dotmatrix* getParent() {return _parent;}

    void setTopLeftX(uint8_t topLeftx) {_topLeftx = topLeftx;}
    void setTopLeftY(uint8_t topLefty) {_topLefty = topLefty;}
    void setWidth(uint8_t width) {_width = width;}
    void setHeight(uint8_t height) {_height = height;}

    bool setPoint(uint8_t x, uint16_t y, bool state);
    bool getPoint(uint8_t x, uint8_t y);
    void drawLine(float x1, float y1, float x2, float y2, bool color = true);
    void circle(uint8_t mx, uint8_t my, bool color, float radius, float angleStart = 0, float angleStop = 2 * PI); // in rads
    void fillRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, bool color = true);
    void clear();
};

#endif