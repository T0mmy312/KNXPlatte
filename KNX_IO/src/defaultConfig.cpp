#include <defaultConfig.h>

Adafruit_NeoPixel globalLedStrip(GLOBAL_LED_STRIP_NUM_LED, GLOBAL_LED_STRIP_PIN, NEO_GRB + NEO_KHZ800);

Dotmatrix globalDotmatrix(GLOBAL_DOTMATRIX_CS_PIN, GLOBAL_DOTMATRIX_NUM_DEVICES);

knx::Blind blind0(BLIND0_TOP_LEFT_X, BLIND0_TOP_LEFT_Y, BLIND0_UP_PIN, BLIND0_DOWN_PIN);

knx::Blind blind1(BLIND1_TOP_LEFT_X, BLIND1_TOP_LEFT_Y, BLIND1_UP_PIN, BLIND1_DOWN_PIN);

knx::Door door(DOOR_TOP_LEFT_X, DOOR_TOP_LEFT_Y, DOOR_OPEN_PIN, DOOR_IS_CLOSED_PIN, DOOR_OPEN_PIN_KNX);

knx::GarageDoor garageDoor(GARAGE_DOOR_TOP_LEFT_X, GARAGE_DOOR_TOP_LEFT_Y, GARAGE_DOOR_IS_CLOSED_PIN, GARAGE_DOOR_UP_PIN, GARAGE_DOOR_DOWN_PIN, GARAGE_DOOR_CLOSE_TIME);

knx::Window window(WINDOW_TOP_LEFT_X, WINDOW_TOP_LEFT_Y, WINDOW_IS_CLOSED_PIN, WINDOW_OPEN_PIN);

knx::Weather rain(RAIN_START_INDEX, RAIN_LED_LENGHT, RAIN_ON_PIN, RAIN_AUTO_PIN, RAIN_OUT_PIN, RAIN_ON_COLOR, RAIN_OFF_COLOR, RAIN_TIME_PER_PERIOD, RAIN_CHANCE);

knx::Weather wind(WIND_START_INDEX, WIND_LED_LENGHT, WIND_ON_PIN, WIND_AUTO_PIN, WIND_OUT_PIN, WIND_ON_COLOR, WIND_OFF_COLOR, WIND_TIME_PER_PERIOD, WIND_CHANCE); 

knx::Heater heater(HEATER_START_INDEX, HEATER_LED_LENGHT, HEATER_HEATING_PIN, HEATER_COOLING_PIN, HEATER_HEATING_HUE, HEATER_COOLING_HUE, HEATER_TEMP_REACH_TIME);

knx::RFID rfid(RFID_START_INDEX, RFID_LED_LENGHT, RFID_CS_PIN, RFID_RST_PIN, allowed_uids, sizeof(allowed_uids)/sizeof(MFRC522::Uid), RFID_OUTPUT_PIN);

bool initKNX() { // returns false if an error occoured
    bool worked = true;

    globalLedStrip.begin();
    INIT(globalDotmatrix);

    blind0.begin();
    blind1.begin();
    door.begin();
    garageDoor.begin();
    window.begin();
    rain.begin();
    wind.begin();
    heater.begin();
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