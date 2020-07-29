#include <Arduino.h>
class productionController
{

public:
    
    void stopProduction(timerController timerController_,GPIOController GPIOController_);
    
};


void productionController::stopProduction(timerController timerController_,GPIOController GPIOController_){
  GPIOController_.setValveState(0);
  timerController_.timerStop();
}
