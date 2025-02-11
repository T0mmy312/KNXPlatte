#include <knx_io.h>

// ----------------------------------------------------------------------------------------------------
// functions
// ----------------------------------------------------------------------------------------------------

bool percentChance(float percent) {
    percent = constrain(percent, 0.0, 100.0);
    float randomValue = random(0, 10000) / 100.0;
    return randomValue < percent;
}

// ----------------------------------------------------------------------------------------------------
// operator overloads
// ----------------------------------------------------------------------------------------------------

bool operator==(const MFRC522::Uid& lhs, const MFRC522::Uid& rhs) {
    if (lhs.size != rhs.size) {
        return false;
    }
    for (uint8_t i = 0; i < lhs.size; i++) {
        if (lhs.uidByte[i] != rhs.uidByte[i]) {
            return false;
        }
    }
    return true;
}


bool operator!=(const MFRC522::Uid& lhs, const MFRC522::Uid& rhs) {
    return !(lhs == rhs);
}

namespace knx {

// ----------------------------------------------------------------------------------------------------
// Blind class
// ----------------------------------------------------------------------------------------------------

// --------------------------------------------------
// public methods
// --------------------------------------------------

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
    int16_t deltaTime = (_down_input() - _up_input()) * (millis() - _lastUpdateTime);

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

void Door::begin() {
    pinMode(_is_closed_pin, OUTPUT);
}

void Door::update() {
    uint32_t delta_time = millis() - _last_update_time;

    if (!_opening)
        _opening = _open_input() || _open_knx_input();

    _curr_angle += (delta_time/(float)_anim_time) * PI/2 * (_opening ? 1 : -1);
    _curr_angle = clamp<float>(_curr_angle, 0, PI/2.0);

    clear();

    circle(0, 7, 1, _radius, 0, _curr_angle);

    uint8_t ex = round(_radius*cos(_curr_angle));
    uint8_t ey = 7 - round(_radius*sin(_curr_angle));

    drawLine(0, 7, ex, ey);

    digitalWrite(_is_closed_pin, _curr_angle == 0);

    if (_curr_angle >= PI/2.0 && !_open_input() && !_open_knx_input())
        _opening = false;

    _last_update_time = millis();
}

// ----------------------------------------------------------------------------------------------------
// GarageDoor class
// ----------------------------------------------------------------------------------------------------

// --------------------------------------------------
// public methods
// --------------------------------------------------

void GarageDoor::begin() {
    pinMode(_closed_out_pin, OUTPUT);
}

void GarageDoor::update() {
    uint32_t delta_time = millis() - _last_update_time; // in ms
    bool up = _up_input();
    bool down = _down_input();
    _pos -= (up - down) * (8 * (delta_time / (float)_close_time));
    _pos = clamp<float>(_pos, 0, 8);
    digitalWrite(_closed_out_pin, _pos >= 8);
    clear();
    fillRect(0, 0, 8, _pos);
    _last_update_time = millis();
}

// ----------------------------------------------------------------------------------------------------
// Window class
// ----------------------------------------------------------------------------------------------------

// --------------------------------------------------
// public methods
// --------------------------------------------------

void Window::begin() {
    pinMode(_closed_pin, OUTPUT);
}

void Window::update() {
    uint32_t delta_time = millis() - _last_update_time; // in ms
    bool open = _open_input();
    _angle += (!open - open) * (PI/2.0 * (delta_time / (float)_close_time));
    _angle = clamp<float>(_angle, 0, PI/2);

    float x = sin(_angle) * 7; // 7 = height
    float dist = cos(_angle) * 7; // 7 = height
    float y = 3.5 - (3.5 * _focal_lenght) / (_focal_lenght + dist); // 3.5 = height / 2

    clear();
    drawLine(0, 0, x, y);
    drawLine(0, 7, x, 7 - y);
    drawLine(0, 0, 0, 7);
    drawLine(x, y, x, 7 - y);

    float vert_x = sin(_angle) * 3.5;
    float vert_y = 3.5 - (3.5 * _focal_lenght) / (_focal_lenght + cos(_angle) * 3.5);
    drawLine(floor(vert_x), vert_y, floor(vert_x), 7 - vert_y);
    drawLine(round(vert_x), vert_y, round(vert_x), 7 - vert_y);

    drawLine(0, 3, x, 3);
    //drawLine(0, 4, x, 4);

    digitalWrite(_closed_pin, _angle >= PI/2);

    _last_update_time = millis();
}

// ----------------------------------------------------------------------------------------------------
// Weather class
// ----------------------------------------------------------------------------------------------------

// --------------------------------------------------
// Constructer
// --------------------------------------------------

Weather::Weather(uint16_t start_index, uint16_t lenght, bool (*on_input)(void), bool (*auto_input)(void), uint8_t out_pin, uint32_t on_color, uint32_t off_color, uint16_t time_per_period, float weather_chance, uint16_t time_per_on_frame, Adafruit_NeoPixel* parent)
    : LEDSegment(start_index, lenght, parent), _lenght(lenght), _on_input(on_input), _auto_input(auto_input), _out_pin(out_pin), _on_color(on_color), _off_color(off_color), _time_per_period(time_per_period), _weather_chance(weather_chance) , _time_per_on_frame(time_per_on_frame)
{
    if (_lenght == 0)
        Serial.println("Weather can not be initialized with a lenght of 0!");
    _state_list = (bool*)malloc(_lenght * sizeof(bool));
    randomSeed(millis());
    for (uint16_t i = 0; i < _lenght; i++)
        _state_list[i] = percentChance(_weather_chance);
}

// --------------------------------------------------
// public methods
// --------------------------------------------------

void Weather::begin() {
    pinMode(_out_pin, OUTPUT);
}

uint16_t Weather::getNextStates(bool* buffer) const {
    strcpy((char*)buffer, (const char*)_state_list);
    return _lenght;
}

void Weather::update() {
    bool onState = _on_input();
    bool autoState = _auto_input();
    if (!onState && !autoState)
        _current_state = WeatherState::OFF;
    else if (onState)
        _current_state = WeatherState::ON;
    else
        _current_state = WeatherState::AUTO;

    if (_current_state == WeatherState::OFF) {
        fill(_off_color);
        digitalWrite(_out_pin, LOW);
        show();
        return;
    }
    else if (_current_state == WeatherState::ON) {
        digitalWrite(_out_pin, HIGH);
        for (uint16_t i = 0; i < _lenght; i++) {
            if ((i + _on_anim_frame) % 3 == 0)
                setPixelColor(i, _on_color);
            else
                setPixelColor(i, _off_color);
        }
        if (millis() - _last_frame_update >= _time_per_on_frame) {
            _on_anim_frame = (_on_anim_frame + 1) % 3;
            _last_frame_update = millis();
        }
        show();
        return;
    }

    if (millis() - _last_period_change >= _time_per_period) {
        for (int i = 0; i < _lenght - 1; i++) // move up each period
            _state_list[i] = _state_list[i + 1];
        _state_list[_lenght - 1] = percentChance(_weather_chance); // add a new period to the back
        _last_period_change = millis();
    }

    for (int i = 0; i < _lenght; i++) {
        if (_state_list[i])
            setPixelColor(i, _on_color);
        else
            setPixelColor(i, _off_color);
    }

    digitalWrite(_out_pin, _state_list[0]);

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

// ----------------------------------------------------------------------------------------------------
// Heater class
// ----------------------------------------------------------------------------------------------------

// --------------------------------------------------
// public methods
// --------------------------------------------------

void Heater::update() {
    uint32_t delta_time = millis() - _last_update_time;
    _last_update_time = millis();

    int8_t direc = _heating_input() - _cooling_input();

    if (direc == 0) {
        if (_normalized_temp < 0) {
            _normalized_temp += (delta_time / (float)_temp_reach_time);
            _normalized_temp = clamp<float>(_normalized_temp, -1, 0);
        }
        else if (_normalized_temp > 0) {
            _normalized_temp -= (delta_time / (float)_temp_reach_time);
            _normalized_temp = clamp<float>(_normalized_temp, 0, 1);
        }
    }
    else {
        _normalized_temp += direc * (delta_time / (float)_temp_reach_time);
        _normalized_temp = clamp<float>(_normalized_temp, -1, 1);
    }

    uint16_t hue = lerpCircular(_heating_hue, _cooling_hue, (_normalized_temp + 1) / 2.0);
    uint8_t val = abs(_normalized_temp) * 0xFF;

    LEDSegment::fill(colorHSV(hue, 0xFF, val));
    LEDSegment::show();
}

// ----------------------------------------------------------------------------------------------------
// RFID class
// ----------------------------------------------------------------------------------------------------

// --------------------------------------------------
// public methods
// --------------------------------------------------

void RFID::begin() {
    SPI.begin(18, 19, 23, _ssPin); // VSPI

    pinMode(_output_pin, OUTPUT);

    _mfrc522 = MFRC522(_ssPin, _rstPin);
    _mfrc522.PCD_Init();

    _ledSegment = LEDSegment(0, _ledCount);

}

void RFID::update() {
    if (millis() - _last_read > RFID_READ_TIME) {
        bool is_allowed = false;
        if (_mfrc522.PICC_IsNewCardPresent() && _mfrc522.PICC_ReadCardSerial()) {        
            for (uint8_t j = 0; j < _allowed_size; j++) {
                if (_mfrc522.uid ==  _allowed[j]) {
                    is_allowed = true;
                    startAnimation(true);
                    Serial.println("allowed");
                    break;
                }
                else {
                    is_allowed = false;
                    startAnimation(false);
                    Serial.println("not allowed");
                    break;
                }
            }
            _mfrc522.PICC_HaltA();
        }

        if (is_allowed) {
            digitalWrite(_output_pin, HIGH);
        }
        else {
            digitalWrite(_output_pin, LOW);
        }

        _last_read = millis();
    }

    doAnimation();

}

void RFID::startAnimation(bool allowed) {
    _allowed_animation = allowed;
    _animation = true;
    _on_led = 0;
    _on_way_back = false;
    _last_led_change = millis();
}

void RFID::doAnimation() {
    if (!_animation) return;

    if (millis() - _last_led_change > RFID_LED_TIME) {
        _on_led++;
        _last_led_change = millis();
    }
    else return;


    if (_on_led >= _ledCount) {
        if (_on_way_back) {
            _animation = false;
        }
        else {
            _on_way_back = true;
            _on_led = 0;
        }
    }

    for (int i = 0; i < _ledCount; i++) {
        if ((i <= _on_led && !_on_way_back) || (i >= _on_led && _on_way_back)) {
            if (_allowed_animation) _ledSegment.setPixelColor(i, _ledSegment.Color(0, 100, 0));
            else _ledSegment.setPixelColor(i, _ledSegment.Color(100, 0, 0));
        }
        else _ledSegment.setPixelColor(i, _ledSegment.Color(0, 0, 2));
    }

    _ledSegment.show();
}

}