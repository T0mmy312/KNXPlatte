#ifndef _KNX_INPUT_H_
#define _KNX_INPUT_H_

#include <Arduino.h>
#include <dotmatrix.h>
#include <ledStrip.h>

#define BLIND_HEIGHT 32
#define BLIND_WIDTH 8

#define BLIND_NUM_DEVICES 4

template <typename T>
T clamp(T x, T min, T max) {
    if (x < min)
        return min;
    if (x > max)
        return max;
    return x;
}

// caution, make sure you set randomSeed() like randomSeed(millis());
bool percentChance(float percent) {
    percent = constrain(percent, 0.0, 100.0);
    float randomValue = random(0, 10000) / 100.0;
    return randomValue < percent;
}


namespace knx {

    // each Blind consits of n 8x8 dotmatrixes (editable through the BLIND_NUM_DEVICES define (default 2))
    // BLIND_HEIGHT and BLIND_WIDTH should be set according to BLIND_NUM_DEVICES
    // CLK and data pins are SPI CLK and MOSI pins
    class Blind : private Dotmatrix {
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
        Blind(uint8_t cs_pin, uint8_t up_pin, uint8_t down_pin, uint16_t closeTime = 5000, uint16_t lamellaTurnoverTime = 3000, uint8_t blindSize = 3) 
            : Dotmatrix(cs_pin, BLIND_NUM_DEVICES), _up_pin(up_pin), _down_pin(down_pin), _closeTime(closeTime), _lamellaTurnoverTime(lamellaTurnoverTime), _blindSize(blindSize) {}

        bool begin();

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
        // led dot matrix that should go up or down
    };

    class Window : private Dotmatrix {
        
    };

    enum WeatherState {
        OFF,
        ON,
        AUTO
    };

    // wind will have three states off / automatic / on (three pin switch)
    // on automatic it should generate random wind periods that are displayed on a led strip so you know if the netxt time segment is on or not

    // heating is a led strip with either red or blue if its heating or not

    // rain will act the same as the wind strip

    // changebale weather color plus random on time generation
    // 2 switch inputs (automatik, on), gpio relay output for knx controller (to say when its on)
    // caution, does not work if on color and off color are the same value
    //! this is not tested!
    // TODO: test this:
    class Weather : private LEDSegment {
    private:
        uint16_t _lenght;
        bool* _state_list;

        uint8_t _on_pin, _auto_pin, _out_pin;
        uint32_t _on_color, _off_color;
        float _weather_chance; // in %
        uint16_t _time_per_period;

        uint16_t _last_period_change = 0;

        WeatherState _current_state = OFF;

    public:
        Weather(uint16_t start_index, uint16_t lenght, uint8_t on_pin, uint8_t auto_pin, uint8_t out_pin, uint32_t on_color, uint32_t off_color, uint16_t time_per_period, float weather_chance = 50, Adafruit_NeoPixel* parent = &globalLedStrip)
            : LEDSegment(start_index, lenght, parent), _lenght(lenght), _on_pin(on_pin), _auto_pin(auto_pin), _out_pin(out_pin), _on_color(on_color), _off_color(off_color), _time_per_period(time_per_period), _weather_chance(weather_chance) 
        {
            _state_list = (bool*)malloc(_lenght * sizeof(bool));
            randomSeed(millis());
        }
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

    class Speaker {

    };
}

#endif