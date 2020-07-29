#include "Arduino.h"

struct Config{
    uint32_t TEMP_SET; // degrees
    uint32_t TIME_SET; // minutes
    uint32_t BAND_GAP; // Width *2
    uint32_t WIDTH; // width in 0.1 increment, but stored as ints - byte values for EEPROM
    uint32_t BAND_OFFSET;
    uint32_t DELAY_TIME;
    uint32_t TEMP_CALIBRATE;
};

class configController
{
private:
   
public:
    
    Config config_;
    void saveConfigEEPROM(Config config_);
    void loadConfigEEPROM();
    void saveDefaultEEPROM(Config config);
    
};
