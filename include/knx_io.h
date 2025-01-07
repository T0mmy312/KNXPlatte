#ifndef _KNX_INPUT_H_
#define _KNX_INPUT_H_

#include <Arduino.h>
#include <dotmatrix.h>

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

        uint64_t _lastUpdateTime = 0;
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

        void setCloseTime(uint16_t closeTime); // in ms
        uint16_t getCloseTime() const {return _closeTime;} // in ms
        void setLamellaTurnoverTime(uint16_t lamellaTurnoverTime); // lamellen wende zeit (in ms)
        uint16_t getLamellaTurnoverTime() const {return _lamellaTurnoverTime;} // lamellen wende zeit (in ms)
        void setBlindSize(uint8_t blindSize) {_blindSize = blindSize;}
        uint8_t getBlindSize() const {return _blindSize;};

        void update();
    };

    class MainDoor {
        // display
    };

    class GarageDoor {
        // led dot matrix that should go up or down
    };

    // wind will have three states off / automatic / on (three pin switch)
    // on automatic it should generate random wind periods that are displayed on a led strip so you know if the netxt time segment is on or not

    // heating is a led strip with either red or blue if its heating or not

    // rain will act the same as the wind strip

    class LEDStrip {
        
    };

    // changebale weather color plus random on time generation
    class Weather : LEDStrip {

    };

}

#endif