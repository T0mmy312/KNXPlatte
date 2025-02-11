#ifndef _KNX_INPUT_H_
#define _KNX_INPUT_H_

#include <Arduino.h>
#include <dotmatrix.h>
#include <ledStrip.h>
#include <SPI.h>
#include <MFRC522.h>
#include <string.h>

#define BLIND_HEIGHT 32
#define BLIND_WIDTH 8

#define RFID_LED_COUNT 16
#define RFID_LED_TIME 15
#define RFID_READ_TIME 100

template <typename T>
T clamp(T x, T min, T max) {
    if (x < min)
        return min;
    if (x > max)
        return max;
    return x;
}

inline float lerp(float a, float b, float t) {
    return a + (b - a) * t;
}

inline uint16_t lerpCircular(uint16_t a, uint16_t b, float t) {
    int16_t diff = b - a;
    if (diff > 32768) diff -= 65536;  // Wraparound handling
    if (diff < -32768) diff += 65536;
    return a + diff * t;
}

// caution, make sure you set randomSeed() like randomSeed(millis());
bool percentChance(float percent);

bool operator==(const MFRC522::Uid& lhs, const MFRC522::Uid& rhs);
bool operator!=(const MFRC522::Uid& lhs, const MFRC522::Uid& rhs);

namespace knx {

    // each Blind consits of n 8x8 dotmatrixes (editable through the BLIND_NUM_DEVICES define (default 2))
    // BLIND_HEIGHT and BLIND_WIDTH should be set according to BLIND_NUM_DEVICES
    // CLK and data pins are SPI CLK and MOSI pins
    class Blind : private LocalDotmatrix {
    private:
        uint8_t _up_pin; // input for the up signal
        uint8_t _down_pin; // input for the down signal

        uint16_t _closeTime; // time needed for the blinds to close completely
        uint16_t _lamellaTurnoverTime;

        uint32_t _lastUpdateTime = 0;
        int16_t _timeGoingDown = 0;
        float _blindPos = 0; // in pix (but needs to be float because of low update times)
        float _blindAngle = 0; // in rads
        uint8_t _blindSize;

        uint16_t _timeContinuslyGoingDown = 0;
        uint16_t _timeContinuslyGoingUp = 0;

    public:
        Blind(uint8_t topLeftX, uint8_t topLeftY, uint8_t up_pin, uint8_t down_pin, uint16_t closeTime = 5000, uint16_t lamellaTurnoverTime = 3000, uint8_t blindSize = 3, Dotmatrix* parent = &globalDotmatrix) 
            : LocalDotmatrix(topLeftX, topLeftY, BLIND_WIDTH, BLIND_HEIGHT, parent), _up_pin(up_pin), _down_pin(down_pin), _closeTime(closeTime), _lamellaTurnoverTime(lamellaTurnoverTime), _blindSize(blindSize) {}

        void begin();

        void setUpPin(uint8_t up_pin) {pinMode(up_pin, INPUT_PULLUP); _up_pin = up_pin;}
        uint8_t getUpPin() const {return _up_pin;}
        void setDownPin(uint8_t down_pin) {pinMode(down_pin, INPUT_PULLUP); _down_pin = down_pin;}
        uint8_t getDownPin() const {return _down_pin;}

        void setCloseTime(uint16_t closeTime); // in ms
        uint16_t getCloseTime() const {return _closeTime;} // in ms
        void setLamellaTurnoverTime(uint16_t lamellaTurnoverTime); // lamellen wende zeit (in ms)
        uint16_t getLamellaTurnoverTime() const {return _lamellaTurnoverTime;} // lamellen wende zeit (in ms)
        void setBlindSize(uint8_t blindSize) {_blindSize = blindSize;}
        uint8_t getBlindSize() const {return _blindSize;};

        void update();
    };

    class Door : private Dotmatrix {
    private:
        uint8_t _open_pin, _is_closed_pin, _open_pin_knx;
        uint16_t _anim_time;
        uint8_t _radius;

        float _curr_angle = 0;

        bool _opening = false;
        uint32_t _last_update_time = 0;

    public:
        Door(uint8_t cs_pin, uint8_t open_pin, uint8_t is_closed_pin, uint8_t open_pin_knx, uint16_t anim_time = 1000, uint8_t radius = 7)
            : Dotmatrix(cs_pin, 1), _open_pin(open_pin), _is_closed_pin(is_closed_pin), _open_pin_knx(open_pin_knx), _anim_time(anim_time), _radius(radius) {}

        bool begin();

        void setOpenPin(uint8_t open_pin) {pinMode(open_pin, INPUT_PULLUP); _open_pin = open_pin;}
        uint8_t getOpenPin() const {return _open_pin;}
        void setIsClosePin(uint8_t is_closed_pin) {pinMode(is_closed_pin, OUTPUT); _is_closed_pin = is_closed_pin;}
        uint8_t getIsClosePin() const {return _is_closed_pin;}
        void setIsClosePinKNX(uint8_t open_pin_knx) {pinMode(open_pin_knx, INPUT_PULLUP); _open_pin_knx = open_pin_knx;}
        uint8_t getClosePinKNX() const {return _open_pin_knx;}

        void setAnimationTime(uint16_t anim_time) {_anim_time = anim_time;}
        uint16_t getAnimationTime() const {return _anim_time;}
        void setRadius(uint8_t radius) {_radius = radius;}
        uint8_t getRadius() const {return _radius;}

        bool isClosed() const {return _curr_angle == 0;}

        void update();
    };

    class GarageDoor : private Dotmatrix {
    private:
        uint8_t _closed_out_pin;

        uint8_t _up_in_pin;
        uint8_t _down_in_pin;

        uint32_t _close_time; // in ms
        float _pos = 8; // in pix

        uint32_t _last_update_time = 0;

    public:
        GarageDoor(uint8_t cs_pin, uint8_t closed_out_pin, uint8_t up_in_pin, uint8_t down_in_pin, uint32_t close_time = 5000)
            : Dotmatrix(cs_pin, 1), _closed_out_pin(closed_out_pin), _up_in_pin(up_in_pin), _down_in_pin(down_in_pin), _close_time(close_time) {}

        bool begin();

        void setClosedOutPin(uint8_t closed_out_pin) {_closed_out_pin = closed_out_pin; pinMode(_closed_out_pin, OUTPUT);}
        void setUpInPin(uint8_t up_in_pin) {_up_in_pin = up_in_pin; pinMode(_up_in_pin, INPUT_PULLUP);}
        void setDownInPin(uint8_t down_in_pin) {_down_in_pin = down_in_pin; pinMode(_down_in_pin, INPUT_PULLUP);}
        uint8_t getClosedOutPin() const {return _closed_out_pin;}
        uint8_t getUpInPin() const {return _up_in_pin;}
        uint8_t getDownInPin() const {return _down_in_pin;}

        void setCloseTime(uint32_t close_time) {_close_time = close_time;}
        uint32_t getCloseTime() const {return _close_time;}

        bool isClosed() const {return _pos >= 8;}

        void update();
    };

    class Window : private Dotmatrix {
    private:
        uint8_t _closed_pin;
        uint8_t _open_pin;

        uint16_t _close_time;

        float _focal_lenght = 8; //! check if I should let this be set

        uint32_t _last_update_time = 0;

        float _angle = PI / 2.0; // in rads

    public:
        Window(uint8_t cs_pin, uint8_t closed_pin, uint8_t open_pin, uint16_t close_time = 2000)
            : Dotmatrix(cs_pin, 1), _closed_pin(closed_pin), _open_pin(open_pin), _close_time(close_time) {}

        bool begin();

        void setClosedPin(uint8_t closed_pin) {_closed_pin = closed_pin; pinMode(_closed_pin, OUTPUT);}
        void setOpenPin(uint8_t open_pin) {_open_pin = open_pin; pinMode(_open_pin, INPUT_PULLUP);}
        uint8_t getClosedPin() const {return _closed_pin;}
        uint8_t getOpenPin() const {return _open_pin;}

        void setCloseTime(uint16_t close_time) {_close_time = close_time;}
        uint16_t getcloseTime() const {return _close_time;}

        void update();
    };

    // wind will have three states off / automatic / on (three pin switch)
    // on automatic it should generate random wind periods that are displayed on a led strip so you know if the netxt time segment is on or not

    // heating is a led strip with either red or blue if its heating or not

    // rain will act the same as the wind strip

    // changebale weather color plus random on time generation
    // 2 switch inputs (automatik, on), gpio relay output for knx controller (to say when its on)

    enum WeatherState {
        OFF,
        ON,
        AUTO
    };

    class Weather : private LEDSegment {
    private:
        uint16_t _lenght;
        bool* _state_list;

        uint8_t _on_pin, _auto_pin, _out_pin;
        uint32_t _on_color, _off_color;
        float _weather_chance; // in %t
        uint16_t _time_per_period;

        uint16_t _time_per_on_frame;
        uint32_t _last_frame_update = 0;
        uint8_t _on_anim_frame = 0;

        uint32_t _last_period_change = 0;

        WeatherState _current_state = OFF;

    public:
        Weather(uint16_t start_index, uint16_t lenght, uint8_t on_pin, uint8_t auto_pin, uint8_t out_pin, uint32_t on_color, uint32_t off_color, uint16_t time_per_period, float weather_chance = 50, uint16_t time_per_on_frame = 300, Adafruit_NeoPixel* parent = &globalLedStrip);
        ~Weather() {
            free(_state_list);
        }
        
        void begin();

        void setOnColor(uint32_t color) {_on_color = color;}
        uint32_t getOnColor() const {return _on_color;}
        void setOffColor(uint32_t color) {_off_color = color;}
        uint32_t getOffColor() const {return _off_color;}
        void setWeatherChance(float chance) {_weather_chance = chance;} // in %
        float getWeatherChance() const {return _weather_chance;} // in %
        void setTimePerPeriod(uint16_t time_per_period) {_time_per_period = time_per_period;}
        uint16_t getTimePerPeriod() const {return _time_per_period;}
        uint16_t getLenght() const {return _lenght;}
        bool getCurrentState() const;

        void setOnPin(uint8_t on_pin) {pinMode(on_pin, INPUT_PULLUP); _on_pin = on_pin;}
        uint8_t getOnPin() const {return _on_pin;}
        void setAutoPin(uint8_t auto_pin) {pinMode(auto_pin, INPUT_PULLUP); _auto_pin = auto_pin;}
        uint8_t getAutoPin() const {return _auto_pin;}
        void setOutPin(uint8_t out_pin) {pinMode(out_pin, OUTPUT); _out_pin = out_pin;}
        uint8_t getOutPin() const {return _out_pin;}

        uint16_t getNextStates(bool* buffer) const; // buffer needs to be at least getLenght() bytes long

        void update();
    };

    class Heater : private LEDSegment {
    private:
        uint8_t _heating_pin;
        uint8_t _cooling_pin;

        uint16_t _heating_hue;
        uint16_t _cooling_hue;

        uint32_t _temp_reach_time; // in ms

        float _normalized_temp = 0; // 0 ... 1 is heating 0 ... -1 is cooling 0 is none 
        uint32_t _last_update_time = 0;

    public:
        Heater(uint16_t start_index, uint16_t lenght, uint8_t heating_pin, uint8_t cooling_pin, uint16_t heating_hue = 0, uint16_t cooling_hue = 43691, uint32_t temp_reach_time = 3000, Adafruit_NeoPixel* parent = &globalLedStrip)
            : LEDSegment(start_index, lenght, parent), _heating_pin(heating_pin), _cooling_pin(cooling_pin), _heating_hue(heating_hue), _cooling_hue(cooling_hue), _temp_reach_time(temp_reach_time) {}
        
        void setHeatingPin(uint8_t heating_pin) {_heating_pin = heating_pin; pinMode(_heating_pin, INPUT_PULLUP);}
        uint8_t getHeatingPin() const {return _heating_pin;}
        void setCoolingPin(uint8_t cooling_pin) {_cooling_pin = cooling_pin; pinMode(_cooling_pin, INPUT_PULLUP);}
        uint8_t getCoolingPin() const {return _cooling_pin;}
        void setHeatingHue(uint16_t heating_hue) {_heating_hue = heating_hue;}
        uint16_t getHeatingHue() const {return _heating_hue;}
        void setCoolingHue(uint16_t cooling_hue) {_cooling_hue = cooling_hue;}
        uint16_t getCoolingHue() const {return _cooling_hue;}
        void setTempReachTime(uint32_t temp_reach_time) {_temp_reach_time = temp_reach_time;}
        uint32_t getTempReachTime() const {return _temp_reach_time;}

        void update();

        void begin();
    };

    class RFID {
    public:
        RFID(uint8_t ssPin, uint8_t rstPin, MFRC522::Uid* allowed, uint8_t allowed_size, uint8_t output_pin)
            : _ssPin(ssPin), _rstPin(rstPin), _allowed(allowed), _allowed_size(allowed_size), _output_pin(output_pin), _ledSegment(0, RFID_LED_COUNT) {}

        void begin();
        void update();

        void startAnimation(bool allowed);

        void doAnimation();

    private:
        uint8_t _ssPin;
        uint8_t _rstPin;
        uint8_t _output_pin;
        MFRC522::Uid* _allowed;
        uint8_t _allowed_size;
        MFRC522 _mfrc522;
        uint32_t _last_read = 0;

        LEDSegment _ledSegment;
        bool _animation = false;
        bool _allowed_animation;
        int8_t _on_led;
        bool _on_way_back;
        uint32_t _last_led_change = 0;
    };
}

#endif