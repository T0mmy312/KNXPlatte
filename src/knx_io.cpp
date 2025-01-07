#include <knx_io.h>

namespace knx {

// ----------------------------------------------------------------------------------------------------
// Blind class
// ----------------------------------------------------------------------------------------------------

// --------------------------------------------------
// public methods
// --------------------------------------------------

bool Blind::begin() {
    pinMode(_up_pin, INPUT_PULLDOWN);
    pinMode(_down_pin, INPUT_PULLDOWN);
    return Dotmatrix::begin();
}

void Blind::setCloseTime(uint16_t closeTime) {
    if (closeTime == 0) {
        Serial.println("closeTime can't be set to 0!");
        return;
    }
    _closeTime = closeTime;
}

void Blind::setLamellaTurnoverTime(uint16_t lamellaTurnoverTime) {
    if (lamellaTurnoverTime == 0) {
        Serial.println("lamellaTurnoverTime can't be set to 0!");
        return;
    }
    _lamellaTurnoverTime = lamellaTurnoverTime;
}

void Blind::update() {
    clear();
    float maxAngle = atan(_blindSize);
    float minAngle = atan(1.0/_blindSize);
    int16_t deltaTime = (digitalRead(_down_pin) - digitalRead(_up_pin)) * (millis() - _lastUpdateTime);

    if (deltaTime > 0) {
        _timeContinuslyGoingUp = 0;
        _timeContinuslyGoingDown += deltaTime;
        _blindAngle += (_timeContinuslyGoingDown / (float)_lamellaTurnoverTime) * (maxAngle - minAngle);
    }
    else if (deltaTime < 0) {
        _timeContinuslyGoingDown = 0;
        _timeContinuslyGoingUp -= deltaTime;
        _blindAngle -= (_timeContinuslyGoingUp / (float)_lamellaTurnoverTime) * (maxAngle - minAngle);
    }

    _blindAngle = clamp<float>(_blindAngle, minAngle, maxAngle);

    _blindPos += (deltaTime / (float)_closeTime) * (BLIND_HEIGHT - _blindSize);
    _blindPos = clamp<float>(_blindPos, 0, BLIND_HEIGHT - 1 - _blindSize);

    uint8_t cx = BLIND_WIDTH / 2;
    float dx = cos(_blindAngle) * _blindSize, dy = sin(_blindAngle) * _blindSize;
    for (int y = _blindPos; y >= -_blindSize*2; y-=_blindSize*2) {
        drawLine(round(cx - dx), round(y + dy), round(cx + dx), round(y - dy));
        drawLine(round(cx - dx) - 1, round(y + dy), round(cx + dx) - 1, round(y - dy));
    }

    _lastUpdateTime = millis();
}

}