#ifndef _DEFAULT_CONFIG_H_
#define _DEFAULT_CONFIG_H_

#include <knx_io.h>
#include <ledStrip.h>
#include <dotmatrix.h>

#define GLOBAL_LED_STRIP_PIN 15 //! placeholder
#define GLOBAL_LED_STRIP_NUM_LED 10 //! placeholder
extern Adafruit_NeoPixel globalLedStrip;

#define GLOBAL_DOTMATRIX_CS_PIN 0 //! placeholder
#define GLOBAL_DOTMATRIX_NUM_DEVICES 0 //! placeholder
extern Dotmatrix globalDotmatrix;

#define BLIND0_TOP_LEFT_X 0 //! placeholder
#define BLIND0_TOP_LEFT_Y 0 //! placeholder
#define BLIND0_UP_PIN 0 //! placeholder
#define BLIND0_DOWN_PIN 0 //! placeholder
extern knx::Blind blind0;

#define BLIND1_TOP_LEFT_X 0 //! placeholder
#define BLIND1_TOP_LEFT_Y 0 //! placeholder
#define BLIND1_UP_PIN 0 //! placeholder
#define BLIND1_DOWN_PIN 0 //! placeholder
extern knx::Blind blind1;

#define DOOR_CS_PIN 0 //! placeholder
#define DOOR_OPEN_PIN 0 //! placeholder
#define DOOR_OPEN_PIN_KNX 0 //! placeholder
#define DOOR_IS_CLOSED_PIN 0 //! placeholder
extern knx::Door door;

#define GARAGE_DOOR_CS_PIN 0 //! placeholder
#define GARAGE_DOOR_IS_CLOSED_PIN 0 //! placeholder
#define GARAGE_DOOR_UP_PIN 0 //! placeholder
#define GARAGE_DOOR_DOWN_PIN 0 //! placeholder
#define GARAGE_DOOR_CLOSE_TIME 5000 //! placeholder
extern knx::GarageDoor garageDoor;

#define WINDOW_CS_PIN 0 //! placeholder
#define WINDOW_IS_CLOSED_PIN 0 //! placeholder
#define WINDOW_OPEN_PIN 0 //! placeholder
extern knx::Window window;

#define RAIN_START_INDEX 0 //! placeholder
#define RAIN_LED_LENGHT 0 //! placeholder
#define RAIN_ON_PIN 0 //! placeholder
#define RAIN_AUTO_PIN 0 //! placeholder
#define RAIN_OUT_PIN 0 //! placeholder
#define RAIN_ON_COLOR colorRGB(0, 0, 255)
#define RAIN_OFF_COLOR colorRGB(0, 0, 0)
#define RAIN_TIME_PER_PERIOD 5000 // ms for each rain / no rain random period (on auto setting)
#define RAIN_CHANCE 50 // the percent chance that each period is a rain period (on auto setting) 
extern knx::Weather rain;

#define WIND_START_INDEX 0 //! placeholder
#define WIND_LED_LENGHT 0 //! placeholder
#define WIND_ON_PIN 0 //! placeholder
#define WIND_AUTO_PIN 0 //! placeholder
#define WIND_OUT_PIN 0 //! placeholder
#define WIND_ON_COLOR colorRGB(255, 255, 255)
#define WIND_OFF_COLOR colorRGB(0, 0, 0)
#define WIND_TIME_PER_PERIOD 5000 // ms for each wind / no wind random period (on auto setting)
#define WIND_CHANCE 50 // the percent chance that each period is a wind period (on auto setting)
extern knx::Weather wind; 

#define HEATER_START_INDEX 0 //! placeholder
#define HEATER_LED_LENGHT 0 //! placeholder
#define HEATER_HEATING_PIN 0 //! placeholder
#define HEATER_COOLING_PIN 0 //! placeholder
#define HEATER_HEATING_HUE 0 // the hsv hue for when it is heating (0 is red)
#define HEATER_COOLING_HUE 43691 // the hsv hue for when it is cooling (43691 is blue)
#define HEATER_TEMP_REACH_TIME 3000 // the time needed for the hue to reach the end hue 
extern knx::Heater heater;

#define RFID_CS_PIN 0 //! placeholder
#define RFID_RST_PIN 0 //! placeholder
#define RFID_OUTPUT_PIN 0 //! placeholder
static MFRC522::Uid allowed_uids[] = {
    {4, {0x44, 0x77, 0x6E, 0x71}, 0x08}
  };
extern knx::RFID rfid;

#define PRINT_LOC if (1) {Serial.print(__FILE__); Serial.print(": "); Serial.print(__LINE__);}
#define INIT(var) if (!var.begin()) {worked = false; Serial.print(#var); Serial.print(" failed to init! in "); PRINT_LOC; Serial.println();}

//returns false if an error occoured
bool initKNX();
void updateAll();

#endif