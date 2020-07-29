#include <SPI.h>
#include <Adafruit_MAX31865.h>
#include <Arduino.h>
#include "HX711.h"


class sensorController
{

public:

    HX711 scaleInterface;
    int scaleCalibrationfactor;

    

    void setScaleCalibrate_(int calibrationFactor);

    Adafruit_MAX31865 &prodTempSensor();
    void initThermo(Adafruit_MAX31865 thermo);
    float readTemp(Adafruit_MAX31865 thermo);

    void initScale(int DOUT,int CLK);
    
    void calibrateScale();
    
    float getWeight();
};

