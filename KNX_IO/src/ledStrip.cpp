#include <ledStrip.h>

uint32_t LEDSegment::Color(uint8_t r, uint8_t g, uint8_t b) {
    return _parent->Color(r, g, b);
}

void LEDSegment::fill(uint32_t c, uint16_t first, uint16_t count) {
    if (first >= _lenght)
        return;
    if (first + count >= _lenght)
        count = _lenght - first;
    if (count == 0)
        _parent->fill(c, first + _startIndex, _lenght - first);
    else
        _parent->fill(c, first + _startIndex, count);
}

uint32_t LEDSegment::getPixelColor(uint16_t n) const {
    if (n >= _lenght)
        return 0U;
    return _parent->getPixelColor(n + _startIndex);
}

void LEDSegment::setPixelColor(uint16_t n, uint32_t c) {
    if (n >= _lenght)
        return;
    _parent->setPixelColor(n + _startIndex, c);
}

void LEDSegment::show() {
    _parent->show();
}