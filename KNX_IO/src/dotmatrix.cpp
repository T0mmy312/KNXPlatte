#include <dotmatrix.h>

// ----------------------------------------------------------------------------------------------------
// class dotmatrix
// ----------------------------------------------------------------------------------------------------

// --------------------------------------------------
// public methods
// --------------------------------------------------

bool Dotmatrix::setPoint(uint8_t x, uint16_t y, bool state) {
    // this is just to show that the functions can be intercepted and alterd like this (in this case it is pointless) 
    return MD_MAX72XX::setPoint(x, y, state);
}

void Dotmatrix::drawLine(float x1, float y1, float x2, float y2, bool color) {
    float x;
    float y;
    float dx, dy, step;
    int i = 0;
    
    dx = (x2 - x1);
    dy = (y2 - y1);

    if (abs(dx) >= abs(dy))
        step = abs(dx);
    else
        step = abs(dy);

    dx = dx / step;
    dy = dy / step;
    x = x1;
    y = y1;
    i = 0;

    while (i <= step) {
        setPoint(round(x), round(y), color);
        x = x + dx;
        y = y + dy;
        i = i + 1;
    }
}

void Dotmatrix::circle(uint8_t mx, uint8_t my, bool color, float radius, float angleStart, float angleStop) {
    if (radius == 0) {
        setPoint(mx, my, color);
        return;
    }
    float angleStep = asinf(1.0/radius);
    for (float angle = angleStart; angle <= angleStop; angle+=angleStep) {
        uint8_t x = round(radius*cos(angle));
        uint8_t y = round(radius*sin(angle));
        setPoint(x + mx, my - y, color);
    }
}

void Dotmatrix::fillRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, bool color) {
    for (uint8_t cy = 0; cy < h; cy++)
        for (uint8_t cx = 0; cx < w; cx++)
            setPoint(x + cx, y + cy, color);
}


// ----------------------------------------------------------------------------------------------------
// class LocalDotmatrix
// ----------------------------------------------------------------------------------------------------

// --------------------------------------------------
// public methods
// --------------------------------------------------

bool LocalDotmatrix::setPoint(uint8_t x, uint16_t y, bool state) {
    if (!_parent || x >= _width || y >= _height)
        return false;

    return _parent->setPoint(x + _topLeftx, y + _topLefty, state);
}

bool LocalDotmatrix::getPoint(uint8_t x, uint8_t y) {
    if (!_parent || x >= _width || y >= _height)
        return false;
    
    return _parent->getPoint(x + _topLeftx, y + _topLefty);
}

void LocalDotmatrix::drawLine(float x1, float y1, float x2, float y2, bool color) {
    if (!_parent)
        return;
    
    float x;
    float y;
    float dx, dy, step;
    int i = 0;
    
    dx = (x2 - x1);
    dy = (y2 - y1);

    if (abs(dx) >= abs(dy))
        step = abs(dx);
    else
        step = abs(dy);

    dx = dx / step;
    dy = dy / step;
    x = x1;
    y = y1;
    i = 0;

    while (i <= step) {
        setPoint(round(x), round(y), color);
        x = x + dx;
        y = y + dy;
        i = i + 1;
    }
}

void LocalDotmatrix::circle(uint8_t mx, uint8_t my, bool color, float radius, float angleStart, float angleStop) {
    if (!_parent)
        return;

    if (radius == 0) {
        setPoint(mx, my, color);
        return;
    }
    float angleStep = asinf(1.0/radius);
    for (float angle = angleStart; angle <= angleStop; angle+=angleStep) {
        uint8_t x = round(radius*cos(angle));
        uint8_t y = round(radius*sin(angle));
        setPoint(x + mx, my - y, color);
    }
}

void LocalDotmatrix::fillRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, bool color) {
    if (!_parent)
        return;
    
    for (uint8_t cy = 0; cy < h; cy++)
        for (uint8_t cx = 0; cx < w; cx++)
            setPoint(x + cx, y + cy, color);
}

void LocalDotmatrix::clear() {
    fillRect(0, 0, _width, _height, false);
}