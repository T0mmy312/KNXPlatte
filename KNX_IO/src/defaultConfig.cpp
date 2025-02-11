#include <defaultConfig.h>
#include <inputConfig.h>

Adafruit_NeoPixel globalLedStrip(GLOBAL_LED_STRIP_NUM_LED, GLOBAL_LED_STRIP_PIN, NEO_GRB + NEO_KHZ800);

Dotmatrix globalDotmatrix(GLOBAL_DOTMATRIX_CS_PIN, GLOBAL_DOTMATRIX_NUM_DEVICES);

knx::Blind blind0(BLIND0_TOP_LEFT_X, BLIND0_TOP_LEFT_Y, blind0UpInput, blind0DownInput);

knx::Blind blind1(BLIND1_TOP_LEFT_X, BLIND1_TOP_LEFT_Y, blind1UpInput, blind1DownInput);

knx::Door door(DOOR_TOP_LEFT_X, DOOR_TOP_LEFT_Y, doorOpenInput, doorOpenKNXInput, DOOR_IS_CLOSED_PIN);

knx::GarageDoor garageDoor(GARAGE_DOOR_TOP_LEFT_X, GARAGE_DOOR_TOP_LEFT_Y, GARAGE_DOOR_IS_CLOSED_PIN, garageDoorUpInput, garageDoorDownInput, GARAGE_DOOR_CLOSE_TIME);

knx::Window window(WINDOW_TOP_LEFT_X, WINDOW_TOP_LEFT_Y, WINDOW_IS_CLOSED_PIN, windowOpenInput);

knx::Weather rain(RAIN_START_INDEX, RAIN_LED_LENGHT, rainOnInput, rainAutoInput, RAIN_OUT_PIN, RAIN_ON_COLOR, RAIN_OFF_COLOR, RAIN_TIME_PER_PERIOD, RAIN_CHANCE);

knx::Weather wind(WIND_START_INDEX, WIND_LED_LENGHT, windOnInput, windAutoInput, WIND_OUT_PIN, WIND_ON_COLOR, WIND_OFF_COLOR, WIND_TIME_PER_PERIOD, WIND_CHANCE); 

knx::Heater heater(HEATER_START_INDEX, HEATER_LED_LENGHT, heaterHeatingInput, heaterCoolingInput, HEATER_HEATING_HUE, HEATER_COOLING_HUE, HEATER_TEMP_REACH_TIME);

knx::RFID rfid(RFID_START_INDEX, RFID_LED_LENGHT, RFID_CS_PIN, RFID_RST_PIN, allowed_uids, sizeof(allowed_uids)/sizeof(MFRC522::Uid), RFID_OUTPUT_PIN);

bool initKNX() { // returns false if an error occoured
    bool worked = true;

    initPins();

    globalLedStrip.begin();
    INIT(globalDotmatrix);
    
    door.begin();
    garageDoor.begin();
    window.begin();
    rain.begin();
    wind.begin();
    rfid.begin();

    return worked;
}

void updateAll() {
    blind0.update();
    blind1.update();
    door.update();
    garageDoor.update();
    window.update();
    rain.update();
    wind.update();
    heater.update();
    rfid.update();
}