#include <Arduino.h>

class GPIOController
{

public:

    int GPIO_in;
    int GPIO_out;
    GPIOController(int GPIO_IN,int GPIO_OUT){
        this->GPIO_in=GPIO_IN;
        this->GPIO_out=GPIO_OUT;
    }

    void setPinModes(int GPIO_IN,int GPIO_OUT);
    int getValveState();
    void setValveState(int state_);
    
};
int GPIOController::getValveState(){
    return digitalRead(this->GPIO_out);
}
void GPIOController::setPinModes(int gpio_in, int gpio_out){
    pinMode(gpio_in,INPUT);


    pinMode(gpio_out,OUTPUT);

    
}
void GPIOController::setValveState(int state_){
    if(state_==1){
        digitalWrite(this->GPIO_out,HIGH);
        
    }else if(state_==0){
         digitalWrite(this->GPIO_out,LOW);
         
    }
    
}
