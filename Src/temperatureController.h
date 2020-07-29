#include <Arduino.h>
uint32_t DELAY_SPLIT;
bool DELAY_START=false;
#define valvePin 15
class temperatureController
{

public:
    void doControl(configController configController_,sensorController sensorController_,GPIOController GPIOController_);
    
};

void temperatureController::doControl(configController configController_,sensorController sensorController_,GPIOController GPIOController_){
    const float BAND_GAP = configController_.config_.BAND_GAP;
    const float SET_TEMP = configController_.config_.TEMP_SET;
    const float BAND_OFFSET = configController_.config_.BAND_OFFSET;
    
    const int DELAY_TIME = configController_.config_.DELAY_TIME*1000;
    const float BAND_LOWER = SET_TEMP + BAND_OFFSET - (BAND_GAP/2);
    const float BAND_UPPER = SET_TEMP + BAND_OFFSET + (BAND_GAP/2);
    const int valveState_ = GPIOController_.getValveState();
    const int offset = configController_.config_.TEMP_CALIBRATE;

    const float PROD_TEMP = sensorController_.readTemp(sensorController_.prodTempSensor());
    // lower band cut
    if((PROD_TEMP+offset)<= BAND_LOWER){

        if(valveState_ == 0){

            if(DELAY_TIME != 0){
                if(DELAY_START == false){
                    DELAY_SPLIT = millis();
                    DELAY_START = true;
                }else if(millis() - DELAY_SPLIT > DELAY_TIME){
                    GPIOController_.setValveState(1);
                   
                    DELAY_START = false;
                }
            }else 
            {
                GPIOController_.setValveState(1);
                
            }
        }
    }
    if((PROD_TEMP+offset) >= BAND_UPPER){

        if(valveState_== 1){
            
            if(DELAY_TIME != 0){
                if(DELAY_START == false){
                    DELAY_SPLIT = millis();
                    DELAY_START = true;
                }else if(millis() - DELAY_SPLIT > DELAY_TIME){
                    GPIOController_.setValveState(0);
                    
                    DELAY_START = false;
                }
            }else 
            {
                GPIOController_.setValveState(0);
                
            }

        }
    }
}
