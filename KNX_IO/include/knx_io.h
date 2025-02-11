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
        bool (*_up_input)(void); // input for the up signal
        bool (*_down_input)(void); // input for the down signal

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
        Blind(uint8_t topLeftX, uint8_t topLeftY, bool (*up_input)(void), bool (*down_input)(void), uint16_t closeTime = 5000, uint16_t lamellaTurnoverTime = 3000, uint8_t blindSize = 3, Dotmatrix* parent = &globalDotmatrix) 
            : LocalDotmatrix(topLeftX, topLeftY, BLIND_WIDTH, BLIND_HEIGHT, parent), _up_input(up_input), _down_input(down_input), _closeTime(closeTime), _lamellaTurnoverTime(lamellaTurnoverTime), _blindSize(blindSize) {}

        void setUpInput(bool (*up_input)(void)) {_up_input = up_input;}
        void setDownInput(bool (*down_input)(void)) {_down_input = down_input;}

        void setCloseTime(uint16_t closeTime); // in ms
        uint16_t getCloseTime() const {return _closeTime;} // in ms
        void setLamellaTurnoverTime(uint16_t lamellaTurnoverTime); // lamellen wende zeit (in ms)
        uint16_t getLamellaTurnoverTime() const {return _lamellaTurnoverTime;} // lamellen wende zeit (in ms)
        void setBlindSize(uint8_t blindSize) {_blindSize = blindSize;}
        uint8_t getBlindSize() const {return _blindSize;};

        void update();
    };

    class Door : private LocalDotmatrix {
    private:
        bool (*_open_input)(void); // input for the open signal
        bool (*_open_knx_input)(void); // input for the open signal from the KNX controller
        uint8_t _is_closed_pin;
        uint16_t _anim_time;
        uint8_t _radius;

        float _curr_angle = 0;

        bool _opening = false;
        uint32_t _last_update_time = 0;

    public:
        Door(uint8_t topLeftX, uint8_t topLeftY, bool (*open_input)(void), bool (*open_knx_input)(void), uint8_t is_closed_pin, uint16_t anim_time = 1000, uint8_t radius = 7, Dotmatrix* parent = &globalDotmatrix)
            : LocalDotmatrix(topLeftX, topLeftY, 8, 8, parent), _open_input(open_input), _open_knx_input(open_knx_input), _is_closed_pin(is_closed_pin), _anim_time(anim_time), _radius(radius) {}

        void begin();

        void setOpenInput(bool (*open_input)(void)) {_open_input = open_input;}
        void setOpenKNXInput(bool (*open_knx_input)(void)) {_open_knx_input = open_knx_input;}
        void setIsClosePin(uint8_t is_closed_pin) {pinMode(is_closed_pin, OUTPUT); _is_closed_pin = is_closed_pin;}
        uint8_t getIsClosePin() const {return _is_closed_pin;}

        void setAnimationTime(uint16_t anim_time) {_anim_time = anim_time;}
        uint16_t getAnimationTime() const {return _anim_time;}
        void setRadius(uint8_t radius) {_radius = radius;}
        uint8_t getRadius() const {return _radius;}

        bool isClosed() const {return _curr_angle == 0;}

        void update();
    };

    class GarageDoor : private LocalDotmatrix {
    private:
        uint8_t _closed_out_pin;

        bool (*_up_input)(void); // input for the up signal
        bool (*_down_input)(void); // input for the down signal

        uint32_t _close_time; // in ms
        float _pos = 8; // in pix

        uint32_t _last_update_time = 0;

    public:
        GarageDoor(uint8_t topLeftX, uint8_t topLeftY, uint8_t closed_out_pin, bool (*up_input)(void), bool (*down_input)(void), uint32_t close_time = 5000, Dotmatrix* parent = &globalDotmatrix)
            : LocalDotmatrix(topLeftX, topLeftY, 8, 8, parent), _closed_out_pin(closed_out_pin), _up_input(up_input), _down_input(down_input), _close_time(close_time) {}

        void begin();

        void setClosedOutPin(uint8_t closed_out_pin) {_closed_out_pin = closed_out_pin; pinMode(_closed_out_pin, OUTPUT);}
        void setUpInput(bool (*up_input)(void)) {_up_input = up_input;}
        void setDownInput(bool (*down_input)(void)) {_down_input = down_input;}
        uint8_t getClosedOutPin() const {return _closed_out_pin;}

        void setCloseTime(uint32_t close_time) {_close_time = close_time;}
        uint32_t getCloseTime() const {return _close_time;}

        bool isClosed() const {return _pos >= 8;}

        void update();
    };

    class Window : private LocalDotmatrix {
    private:
        uint8_t _closed_pin;
        bool (*_open_input)(void); // input for the open signal

        uint16_t _close_time;

        float _focal_lenght = 8; //! check if I should let this be set

        uint32_t _last_update_time = 0;

        float _angle = PI / 2.0; // in rads

    public:
        Window(uint8_t topLeftX, uint8_t topLeftY, uint8_t closed_pin, bool (*open_input)(void), uint16_t close_time = 2000, Dotmatrix* parent = &globalDotmatrix)
            : LocalDotmatrix(topLeftX, topLeftY, 8, 8, parent), _closed_pin(closed_pin), _open_input(open_input), _close_time(close_time) {}

        void begin();

        void setClosedPin(uint8_t closed_pin) {_closed_pin = closed_pin; pinMode(_closed_pin, OUTPUT);}
        void setOpenInput(bool (*open_input)(void)) {_open_input = open_input;}
        uint8_t getClosedPin() const {return _closed_pin;}

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

        bool (*_on_input)(void);
        bool (*_auto_input)(void);
        uint8_t _out_pin;
        uint32_t _on_color, _off_color;
        float _weather_chance; // in %t
        uint16_t _time_per_period;

        uint16_t _time_per_on_frame;
        uint32_t _last_frame_update = 0;
        uint8_t _on_anim_frame = 0;

        uint32_t _last_period_change = 0;

        WeatherState _current_state = OFF;

    public:
        Weather(uint16_t start_index, uint16_t lenght, bool (*on_input)(void), bool (*auto_input)(void), uint8_t out_pin, uint32_t on_color, uint32_t off_color, uint16_t time_per_period, float weather_chance = 50, uint16_t time_per_on_frame = 300, Adafruit_NeoPixel* parent = &globalLedStrip);
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

        void setOnInput(bool (*on_input)(void)) {_on_input = on_input;}
        void setAutoInput(bool (*auto_input)(void)) {_auto_input = auto_input;}
        void setOutPin(uint8_t out_pin) {pinMode(out_pin, OUTPUT); _out_pin = out_pin;}
        uint8_t getOutPin() const {return _out_pin;}

        uint16_t getNextStates(bool* buffer) const; // buffer needs to be at least getLenght() bytes long

        void update();
    };

    class Heater : private LEDSegment {
    private:
        bool (*_heating_input)(void);
        bool (*_cooling_input)(void);

        uint16_t _heating_hue;
        uint16_t _cooling_hue;

        uint32_t _temp_reach_time; // in ms

        float _normalized_temp = 0; // 0 ... 1 is heating 0 ... -1 is cooling 0 is none 
        uint32_t _last_update_time = 0;

    public:
        Heater(uint16_t start_index, uint16_t lenght, bool (*heating_input)(void), bool (*cooling_input)(void), uint16_t heating_hue = 0, uint16_t cooling_hue = 43691, uint32_t temp_reach_time = 3000, Adafruit_NeoPixel* parent = &globalLedStrip)
            : LEDSegment(start_index, lenght, parent), _heating_input(heating_input), _cooling_input(cooling_input), _heating_hue(heating_hue), _cooling_hue(cooling_hue), _temp_reach_time(temp_reach_time) {}
        
        void setHeatingInput(bool (*heating_input)(void)) {_heating_input = heating_input;}
        void setCoolingInput(bool (*cooling_input)(void)) {_cooling_input = cooling_input;}
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
        RFID(uint8_t ledStartIndex, uint8_t ledCount, uint8_t ssPin, uint8_t rstPin, MFRC522::Uid* allowed, uint8_t allowed_size, uint8_t output_pin)
            : _ssPin(ssPin), _rstPin(rstPin), _allowed(allowed), _allowed_size(allowed_size), _output_pin(output_pin), _ledSegment(ledStartIndex, ledCount), _ledCount(ledCount) {}

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
        uint8_t _ledCount;
        bool _animation = false;
        bool _allowed_animation;
        int8_t _on_led;
        bool _on_way_back;
        uint32_t _last_led_change = 0;
    };
}

#endif