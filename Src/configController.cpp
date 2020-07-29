#include "configController.h"
#include "EEPROM.h"

void configController::saveConfigEEPROM(Config config_){
    EEPROM.write(0, config_.TEMP_SET);
    EEPROM.write(1, config_.BAND_GAP);
    EEPROM.write(2, config_.WIDTH);
    EEPROM.write(3, config_.BAND_OFFSET);
    EEPROM.write(4, config_.DELAY_TIME);
    EEPROM.write(5, config_.TEMP_CALIBRATE);
    EEPROM.write(6, config_.TIME_SET);
    Serial.println("Saving Production Config Data");
}
void configController::loadConfigEEPROM(){
    this->config_.TEMP_SET= EEPROM.read(0);
    this->config_.BAND_GAP= EEPROM.read(1);
    this->config_.WIDTH= EEPROM.read(2);
    this->config_.BAND_OFFSET= EEPROM.read(3);
    this->config_.DELAY_TIME= EEPROM.read(4);
    this->config_.TEMP_CALIBRATE= EEPROM.read(5);
    this->config_.TIME_SET= EEPROM.read(6);
    Serial.println("Loading Production Config Data");
    
}
void configController::saveDefaultEEPROM(Config config){
    EEPROM.write(0, config.TEMP_SET);
    EEPROM.write(1, config.BAND_GAP);
    EEPROM.write(2, config.WIDTH);
    EEPROM.write(3, config.BAND_OFFSET);
    EEPROM.write(4, config.DELAY_TIME);
    EEPROM.write(5, config.TEMP_CALIBRATE);
    EEPROM.write(6, config.TIME_SET);
}
