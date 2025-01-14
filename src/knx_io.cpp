#include <knx_io.h>

namespace knx {

// ----------------------------------------------------------------------------------------------------
// Blind class
// ----------------------------------------------------------------------------------------------------

// --------------------------------------------------
// public methods
// --------------------------------------------------

bool Blind::begin() {
    pinMode(_up_pin, INPUT_PULLUP);
    pinMode(_down_pin, INPUT_PULLUP);
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
    int16_t deltaTime = (!digitalRead(_down_pin) - !digitalRead(_up_pin)) * (millis() - _lastUpdateTime);

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

// ----------------------------------------------------------------------------------------------------
// Door class
// ----------------------------------------------------------------------------------------------------

// --------------------------------------------------
// public methods
// --------------------------------------------------

bool Door::begin() {
    pinMode(_open_pin, INPUT_PULLUP);
    pinMode(_is_closed_pin, OUTPUT);
    pinMode(_open_pin_knx, INPUT_PULLUP);
    return Dotmatrix::begin();
}

void Door::update() {
    uint32_t delta_time = millis() - _last_update_time;

    if (!_opening)
        _opening = !digitalRead(_open_pin) || !digitalRead(_open_pin_knx);

    _curr_angle += (delta_time/(float)_anim_time) * PI/2 * (_opening ? 1 : -1);
    _curr_angle = clamp<float>(_curr_angle, 0, PI/2.0);

    clear();

    circle(0, 7, 1, _radius, 0, _curr_angle);

    uint8_t ex = round(_radius*cos(_curr_angle));
    uint8_t ey = 7 - round(_radius*sin(_curr_angle));

    drawLine(0, 7, ex, ey);

    digitalWrite(_is_closed_pin, _curr_angle == 0);

    if (_curr_angle >= PI/2.0 && digitalRead(_open_pin) && digitalRead(_open_pin_knx))
        _opening = false;

    _last_update_time = millis();
}

// ----------------------------------------------------------------------------------------------------
// Weather class
// ----------------------------------------------------------------------------------------------------

// --------------------------------------------------
// public methods
// --------------------------------------------------

void Weather::begin() {
    pinMode(_on_pin, INPUT_PULLUP);
    pinMode(_auto_pin, INPUT_PULLUP);
    pinMode(_out_pin, OUTPUT);
}

uint16_t Weather::getNextStates(bool* buffer) const {
    strcpy((char*)buffer, (const char*)_state_list);
    return _lenght;
}

void Weather::update() {
    bool onState = !digitalRead(_on_pin);
    bool autoState = !digitalRead(_auto_pin);
    if (!onState && !autoState)
        _current_state = WeatherState::OFF;
    else if (onState)
        _current_state = WeatherState::ON;
    else
        _current_state = WeatherState::AUTO;

    if (_current_state == WeatherState::OFF) {
        fill(_off_color);
        digitalWrite(_out_pin, LOW);
        return;
    }
    else if (_current_state == WeatherState::ON) {
        fill(_on_color);
        digitalWrite(_out_pin, HIGH);
        return;
    }

    if (millis() - _last_period_change >= _time_per_period) {
        for (int i = 0; i < _lenght - 1; i++) // move up each period
            _state_list[i] = _state_list[i + 1];
        _state_list[_lenght - 1] = percentChance(_weather_chance); // add a new period to the back
    }

    for (int i = 0; i < _lenght; i++) {
        if (_state_list[i])
            setPixelColor(i, _on_color);
        else
            setPixelColor(i, _off_color);
    }

    show();
}

bool Weather::getCurrentState() const {
    switch (_current_state) {
    case WeatherState::ON:
        return true;
    case WeatherState::OFF:
        return false;
    case WeatherState::AUTO:
        return _state_list[0];
    }
    return false;
}

}