#include <dotmatrix.h>

// ----------------------------------------------------------------------------------------------------
// class dotmatrix
// ----------------------------------------------------------------------------------------------------

// --------------------------------------------------
// public methods
// --------------------------------------------------

bool Dotmatrix::setPoint(uint8_t r, uint16_t c, bool state) {
    // this is just to show that the functions can be intercepted and alterd like this (in this case it is pointless) 
    return MD_MAX72XX::setPoint(r, c, state);
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