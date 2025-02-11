#ifndef _INPUT_CONFIG_H_
#define _INPUT_CONFIG_H_

#include <Arduino.h>

// multiplexer configuration
#define NUM_MUL_SELECT_PINS 0
static uint8_t multiplexerSelectPins[NUM_MUL_SELECT_PINS] = {}; //! placeholder
#define MUL_DATA_PIN 0 //! placeholder

// ----------------------------------------------------------------------------------------------------
// fast input function definiton macros:

//! not yet tested (TEST THIS)
// defines a multiplexer read function where all of the extra arguments are an array of 0 and 1 for each multiplexerSelectPin in multiplexerSelectPins
#define MUL_READ_FUNC_DEF(name, ...) inline bool name() {\
        uint8_t pinStates[] = {__VA_ARGS__};\
        for (int pinIndex = 0; pinIndex < NUM_MUL_SELECT_PINS; pinIndex++)\
            digitalWrite(multiplexerSelectPins[pinIndex], pinStates[pinIndex]);\
        return digitalRead(MUL_DATA_PIN);\
    }

// use this to create a function to read pins defined as INPUT_PULLUP
#define PIN_READ_PULLUP_FUNC_DEF(name, pin) inline bool name() {return !digitalRead(pin);}

// use this to create a function to read pins defined as INPUT_PULLDOWN
#define PIN_READ_PULLDOWN_FUNC_DEF(name, pin) inline bool name() {return digitalRead(pin);}
// ----------------------------------------------------------------------------------------------------

// blinds:
MUL_READ_FUNC_DEF(blind0UpInput, 1, 0, 0, 1, 1); //! placeholder example
PIN_READ_PULLDOWN_FUNC_DEF(blind0DownInput, 0); //! placeholder example

PIN_READ_PULLDOWN_FUNC_DEF(blind1UpInput, 0); //! placeholder
PIN_READ_PULLDOWN_FUNC_DEF(blind1DownInput, 0); //! placeholder

// door:
PIN_READ_PULLDOWN_FUNC_DEF(doorOpenInput, 0); //! placeholder
PIN_READ_PULLDOWN_FUNC_DEF(doorOpenKNXInput, 0); //! placeholder

// garage door:
PIN_READ_PULLDOWN_FUNC_DEF(garageDoorUpInput, 0); //! placeholder
PIN_READ_PULLDOWN_FUNC_DEF(garageDoorDownInput, 0); //! placeholder

// window input:
PIN_READ_PULLDOWN_FUNC_DEF(windowOpenInput, 0); //! placeholder

// rain input:
PIN_READ_PULLDOWN_FUNC_DEF(rainOnInput, 0); //! placeholder
PIN_READ_PULLDOWN_FUNC_DEF(rainAutoInput, 0); //! placeholder

// wind input:
PIN_READ_PULLDOWN_FUNC_DEF(windOnInput, 0); //! placeholder
PIN_READ_PULLDOWN_FUNC_DEF(windAutoInput, 0); //! placeholder

// heater input:
PIN_READ_PULLDOWN_FUNC_DEF(heaterHeatingInput, 0); //! placeholder
PIN_READ_PULLDOWN_FUNC_DEF(heaterCoolingInput, 0); //! placeholder

inline void initPins() {
    // init multiplexer input
    pinMode(MUL_DATA_PIN, INPUT_PULLDOWN);
    for (int pinIndex = 0; pinIndex < NUM_MUL_SELECT_PINS; pinIndex++)
        pinMode(multiplexerSelectPins[pinIndex], OUTPUT);
    
    // set all of your pinModes here:
}

#endif