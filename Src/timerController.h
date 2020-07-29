#include <Arduino.h>
class timerController
{

public:
    uint32_t startTime;
    uint32_t SET_TIME;
    bool timerRunning;
    float timeLeft;
    void timerStart(int SET_TIME);
    bool timerUpdate();
    void timerStop();
   uint32_t GetSetTime();
};
void timerController::timerStop()
{
    this->timerRunning = false;
}
void timerController::timerStart(int SET_TIME)
{
    this->SET_TIME = SET_TIME;
    
    this->startTime = millis();
    this->timerRunning = true;
}
bool timerController::timerUpdate()
{
    this->timeLeft = (this->SET_TIME * 60000 - ((millis() - this->startTime))) / 1000;
     
    if (timeLeft <= 0)
    {

        return true;
    }
    else
    {
        return false;
    }
}

uint32_t timerController::GetSetTime(){
  return this->SET_TIME;
}
