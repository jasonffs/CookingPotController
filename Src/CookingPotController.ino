#include "GPIOController.h"
#include "operationModeController.h"
#include "sensorController.h"
#include "timerController.h"
#include "configController.h"
#include "displayController.h"
#include "temperatureController.h"
#include "productionController.h"
#include <SD.h>


//defualt scale factor--------------------
#define scaleCalibrationFactor_ -25500
#define DOUT 6
#define CLK 7


enum logType
{
    RESET,
    ZERO,
    PRODUCTION_START,
    PRODUCTION_RUNNING,
    PRODUCTION_STOP
};


//defualt password sets-------------------


#define programPassword "1111"
#define maintenencePassword "1111"


//defualt GPIO definitions----------------


#define valveOutput 35
#define gpio_input 36
const int chipSelectSD = 4;



//Class instances-------------------------
productionController productionController_;
operationModeController operationModeController_;
sensorController sensorController_;
timerController timerController_;
configController configController_;
displayController displayController_;
GPIOController GPIOController_(gpio_input, valveOutput);
temperatureController temperatureController_;

//Global variables-------------------------
operationModes selectedMode;
Page page_ = MENU;
uint32_t productionLogTimer = 0;
#define SerialAT Serial1
Config defaultConfig_{

};


void setup()
{
    displayController_.displayInit_();
    displayController_.showPage(StartUpPage);
    Serial.begin(115200);
    SerialAT.begin(115200);
    initSD();
    
    sensorController_.initThermo(sensorController_.prodTempSensor());
    sensorController_.initScale(DOUT, CLK);
    sensorController_.setScaleCalibrate_(scaleCalibrationFactor_);
    
    sensorController_.calibrateScale();

    GPIOController_.setPinModes(gpio_input, valveOutput);
    GPIOController_.setValveState(0);

    
    configController_.loadConfigEEPROM();
    delay(1000);


    
    

    // initialize all Nextion object callback functions--------------
    initDisplayCallbacks();

    //SerialAT.write("AT+RST=1");
    //delay(10000);
    //setupGSMConnection();
    displayController_.showPage(menuPage);
    displayController_.updateMenu(operationModeController_.currentMode_);
}

void loop()
{ // Nextion callback listener for touch interface ------------------------------
    // List defined in DisplayController.h
    nexLoop(nex_listen_list);

    switch (operationModeController_.currentMode_)
    {
    case MODE_PRODUCTION:

        temperatureController_.doControl(configController_, sensorController_, GPIOController_);

        if (timerController_.timerUpdate())
        {
            if (millis - productionLogTimer > 60000)
            {
                logDataSD(PRODUCTION_RUNNING);
                productionLogTimer = millis();
            }
            timerController_.timerStop();
            productionController_.stopProduction(timerController_, GPIOController_);
            operationModeController_.changeMode(MODE_IDLE);
            displayController_.showPage(BatchCompletePage);
            completeWeightNumber.setValue(sensorController_.getWeight());
            delay(10000);
            displayController_.showPage(menuPage);
            displayController_.updateMenu(operationModeController_.currentMode_);
        }
    }
    //page details update processing -----------------------------------------------------
    switch (page_)
    {
    case STATUS:
        displayController_.updateStatusPage(GPIOController_, configController_, sensorController_);
        break;
    case PRODUCTION:

        displayController_.updateProductionPage(configController_.config_.TIME_SET, timerController_, sensorController_, configController_.config_.TEMP_SET);
        break;
    case LOADING:
        displayController_.updateLoadingPage(sensorController_.getWeight());
        break;
    case TESTING:
        displayController_.updateTestPage(GPIOController_);
        break;
    default:
        break;
    }
}

// Nextion Display interface functions -----------------------------------------------------

void initDisplayCallbacks()
{
    //menu screen object callbacks ------------------------------------------------------
    menuOKButton.attachPop(menuOKCallback, &menuOKButton);
    mode1Select.attachPop(mode1SelectPopCallback, &mode1Select);
    mode2Select.attachPop(mode2SelectPopCallback, &mode2Select);
    mode3Select.attachPop(mode3SelectPopCallback, &mode3Select);
    mode4Select.attachPop(mode4SelectPopCallback, &mode4Select);
    mode5Select.attachPop(mode5SelectPopCallback, &mode5Select);
    StatusButton.attachPop(StatusButtonCallback, &StatusButton);
    statusPageMenuButton.attachPop(statusPageMenuButtonCallback, &statusPageMenuButton);
    productionPageMenuButton.attachPop(productionPageMenuButtonCallback, &productionPageMenuButton);
    outputTestValveButton.attachPop(outputTestValveButtonCallback, &outputTestValveButton);
    maintenenceOutputTestButton.attachPop(maintenenceOutputTestButtonCallback, &maintenenceOutputTestButton);
    outputTestBackButton.attachPop(outputTestBackButtonCallback, &outputTestBackButton);
    cancelMaintenenceButton.attachPop(cancelMaintenenceButtonCallback,&cancelMaintenenceButton);
    cancelProgramButton.attachPop(cancelProgramButtonCallback,&cancelProgramButton);
    //password screen callbacks
    passwordOKButton.attachPop(passwordOKCheck, &passwordOKButton);
    passwordCancelButton.attachPop(passwordCancelCallback, &passwordCancelButton);
    //loading screen callbacks
    zeroScaleButton.attachPop(zeroScalecallback, &zeroScaleButton);
    cancelLoadingButton.attachPop(cancelLoadingButtoncallback, &cancelLoadingButton);
    enterButtonProgramPage.attachPop(enterButtonProgramPageCallback, &enterButtonProgramPage);
    enterMaintenencePageButton.attachPop(enterMaintenencePageButtoncallback, &enterMaintenencePageButton);
}
void cancelMaintenenceButtonCallback(void *ptr){
    displayController_.showPage(menuPage);
    operationModeController_.changeMode(MODE_IDLE);
    displayController_.updateMenu(operationModeController_.currentMode_);
    selectedMode=operationModeController_.currentMode_;
}
void cancelProgramButtonCallback(void *ptr){
    displayController_.showPage(menuPage);
    operationModeController_.changeMode(MODE_IDLE);
    displayController_.updateMenu(operationModeController_.currentMode_);selectedMode=operationModeController_.currentMode_;
}
void outputTestBackButtonCallback(void *ptr)
{
    operationModeController_.changeMode(MODE_MAINTENENCE);
    displayController_.showPage(maintenencePage);
    page_ = MAINTENENCE;
    displayController_.updateMaintenencePage(configController_);
}
void outputTestValveButtonCallback(void *ptr)
{
    if (GPIOController_.getValveState() == 1)
    {
        GPIOController_.setValveState(0);
        Serial.println(digitalRead(valveOutput));
    }
    else
    {
        GPIOController_.setValveState(1);
        Serial.println(digitalRead(valveOutput));
    }
}
void maintenenceOutputTestButtonCallback(void *ptr)
{
    page_ = TESTING;
    operationModeController_.changeMode(MODE_TESTING);
    displayController_.showPage(OutputTestPage);
}
void productionPageMenuButtonCallback(void *ptr)
{
    displayController_.showPage(menuPage);
    page_ = MENU;
    displayController_.updateMenu(operationModeController_.currentMode_);
}
void statusPageMenuButtonCallback(void *ptr)
{
    displayController_.showPage(menuPage);
    page_ = MENU;
    displayController_.updateMenu(operationModeController_.currentMode_);
}
void StatusButtonCallback(void *ptr)
{
    page_ = STATUS;
    displayController_.showPage(StatusPage);
    displayController_.updateStatusPage(GPIOController_, configController_, sensorController_);
}
void zeroScalecallback(void *ptr)
{
    sensorController_.calibrateScale();
    logDataSD(ZERO);
}
void passwordCancelCallback(void *ptr)
{
    operationModeController_.changeMode(MODE_IDLE);
    displayController_.showPage(menuPage);
    page_ = MENU;
    displayController_.updateMenu(operationModeController_.currentMode_);selectedMode=operationModeController_.currentMode_;
}
void passwordOKCheck(void *ptr)
{

    uint32_t number1 = 0;
    uint32_t number2 = 0;
    uint32_t number3 = 0;
    uint32_t number4 = 0;
    passwordNum1.getValue(&number1);
    delay(100);
    passwordNum2.getValue(&number2);
    delay(100);
    passwordNum3.getValue(&number3);
    delay(100);
    passwordNum4.getValue(&number4);
    String pass = String(number1);
    String pass1 = pass + number2 + number3 + number4;
    Serial.println(pass1);
    switch (operationModeController_.currentMode_)
    {
    case MODE_PROGRAM:
        if (pass1 == programPassword)
        {
            page_ = PROGRAM;
            displayController_.showPage(programPage);
            displayController_.updateProgramPage(configController_);
        }
        break;
    case MODE_MAINTENENCE:
        if (pass1 == maintenencePassword)
        {
            page_ = MAINTENENCE;
            displayController_.showPage(maintenencePage);
            displayController_.updateMaintenencePage(configController_);
        }
        break;
    default:
        break;
    }
}
void enterMaintenencePageButtoncallback(void *ptr)
{
    BAND_GAP_number.getValue(&configController_.config_.BAND_GAP);
    WIDTH_number.getValue(&configController_.config_.WIDTH);
    BAND_OFFSET_number.getValue(&configController_.config_.BAND_OFFSET);
    DELAY_TIME_number.getValue(&configController_.config_.DELAY_TIME);
    TEMP_CALIBRATE_number.getValue(&configController_.config_.TEMP_CALIBRATE);

    configController_.saveConfigEEPROM(configController_.config_);
    operationModeController_.changeMode(MODE_IDLE);
    displayController_.showPage(menuPage);
    page_ = MENU;
    displayController_.updateMenu(operationModeController_.currentMode_);
}

void cancelLoadingButtoncallback(void *ptr)
{
    operationModeController_.changeMode(MODE_IDLE);
    displayController_.showPage(menuPage);
    page_ = MENU;
    displayController_.updateMenu(operationModeController_.currentMode_);selectedMode=operationModeController_.currentMode_;
}
void enterButtonProgramPageCallback(void *ptr)
{
    TEMP_SET_number.getValue(&configController_.config_.TEMP_SET);
    TIME_SET_number.getValue(&configController_.config_.TIME_SET);
    configController_.saveConfigEEPROM(configController_.config_);
    operationModeController_.changeMode(MODE_IDLE);
    displayController_.showPage(menuPage);
    page_ = MENU;
    displayController_.updateMenu(operationModeController_.currentMode_);
}
void menuOKCallback(void *ptr)
{
    switch (selectedMode)
    {
    case MODE_IDLE:
        displayController_.showPage(menuPage);
        operationModeController_.changeMode(MODE_IDLE);
        displayController_.updateMenu(operationModeController_.currentMode_);
        productionController_.stopProduction(timerController_, GPIOController_);

        break;
    case MODE_PRODUCTION:
        if (operationModeController_.currentMode_ == MODE_PRODUCTION)
        {
            displayController_.showPage(productionPage);
        }
        else
        {
            GPIOController_.setValveState(1);
            Serial.println("TURNING VALVE ON ");
            operationModeController_.changeMode(MODE_PRODUCTION);
            timerController_.timerStart(configController_.config_.TIME_SET);
            displayController_.showPage(productionPage);
            logDataSD(PRODUCTION_START);
            productionLogTimer = millis();
        }
        page_ = PRODUCTION;

        break;
    case MODE_PROGRAM:
        if (operationModeController_.currentMode_ == MODE_PRODUCTION)
        {
        }
        else
        {
            displayController_.showPage(passwordPage);
            operationModeController_.changeMode(MODE_PROGRAM);
        }
        page_ = PROGRAM;

        break;
    case MODE_MAINTENENCE:
        if (operationModeController_.currentMode_ == MODE_PRODUCTION)
        {
        }
        else
        {
            displayController_.showPage(passwordPage);
            operationModeController_.changeMode(MODE_MAINTENENCE);
        }

        break;
    case MODE_LOADING:
    
        
        if (operationModeController_.currentMode_ == MODE_PRODUCTION)
        {
        }
        else
        {
            page_ = LOADING;
            operationModeController_.changeMode(MODE_LOADING);
            displayController_.showPage(loadingPage);
        }

        break;

    default:
        break;
    }
}

void mode1SelectPopCallback(void *ptr)
{
    selectedMode = MODE_IDLE;
    mode1Marker.Set_background_color_bco(3744);
    mode2Marker.Set_background_color_bco(65535);
    mode3Marker.Set_background_color_bco(65535);
    mode4Marker.Set_background_color_bco(65535);
    mode5Marker.Set_background_color_bco(65535);
}
void mode2SelectPopCallback(void *ptr)
{
    selectedMode = MODE_PRODUCTION;
    mode1Marker.Set_background_color_bco(65535);
    mode2Marker.Set_background_color_bco(3744);
    mode3Marker.Set_background_color_bco(65535);
    mode4Marker.Set_background_color_bco(65535);
    mode5Marker.Set_background_color_bco(65535);
}
void mode3SelectPopCallback(void *ptr)
{
    selectedMode = MODE_PROGRAM;
    mode1Marker.Set_background_color_bco(65535);
    mode2Marker.Set_background_color_bco(65535);
    mode3Marker.Set_background_color_bco(3744);
    mode4Marker.Set_background_color_bco(65535);
    mode5Marker.Set_background_color_bco(65535);
}
void mode4SelectPopCallback(void *ptr)
{
    selectedMode = MODE_MAINTENENCE;
    mode1Marker.Set_background_color_bco(65535);
    mode2Marker.Set_background_color_bco(65535);
    mode3Marker.Set_background_color_bco(65535);
    mode4Marker.Set_background_color_bco(65535);
    mode5Marker.Set_background_color_bco(3744);
}
void mode5SelectPopCallback(void *ptr)
{
    selectedMode = MODE_LOADING;
    mode1Marker.Set_background_color_bco(65535);
    mode2Marker.Set_background_color_bco(65535);
    mode3Marker.Set_background_color_bco(65535);
    mode4Marker.Set_background_color_bco(3744);
    mode5Marker.Set_background_color_bco(65535);
}

void initSD()
{
  bool success =false;
    for (size_t i = 0; (i < 5) && !success; ++i)
    {
        success = SD.begin(chipSelectSD);
        if (!success)
        {
            Serial.println("Could not initialize SD. retrying...");
        }
        delay(1000);
    }
    if(success){
        Serial.println("SD Init successful");
        
    }
    delay(1000);
}

void logDataSD(logType logType_)
{
    File dataFile = SD.open("datalog.txt", FILE_WRITE);
    if (dataFile)
    {
        switch (logType_)
        {
        case RESET:
            dataFile.println("SYSTEM RESET:  ");
            dataFile.println();
            break;
        case ZERO:
            dataFile.println("SCALE ZERO:  ");
            dataFile.println();
            
            break;
        case PRODUCTION_START:
            dataFile.println("PRODUCTION START:  ");
            dataFile.println();
            dataFile.println("TEMP_SET:  ");
            dataFile.println(configController_.config_.TEMP_SET);
            dataFile.println("  TIME_SET:  ");
            dataFile.println(configController_.config_.TIME_SET);

            break;
        case PRODUCTION_RUNNING:
            dataFile.println("PRODUCTION UPDATE:");
            dataFile.println("PROD TEMP:  ");
            dataFile.println(sensorController_.readTemp(sensorController_.prodTempSensor()));
            dataFile.println("VALVE STATE:  ");
            dataFile.println(GPIOController_.getValveState());
            break;
        case PRODUCTION_STOP:
            dataFile.println("PRODUCTION STOP:  ");
            dataFile.println();
            break;
        default:
            break;
        }

        dataFile.close();
    }
    else
    {
        Serial.println("Unable to open Datalogg file");
    }
}

void setupGSMConnection(){
    SerialAT.write("AT+CGATT=1"); // attach to network
    delay(5000);
    SerialAT.write("AT+CGDCONT=1,'IP','CMNET'");
    delay(3000);
    while(SerialAT.available()){
      Serial.write(SerialAT.read());
    }
    
    SerialAT.write("AT+CGACT=1,1");
    delay(3000);
    SerialAT.write("AT+HTTPGET='http://wiki.ai-thinker.com/gprs_download'");
    delay(10000);
    while(SerialAT.available()){
      Serial.write(SerialAT.read());
    }
}
