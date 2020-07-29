#include "Nextion.h"
uint32_t pageUpdateTimer_ = millis();

NexPage passwordPage = NexPage(0, 0, "Password");
NexPage maintenencePage = NexPage(0, 0, "Maintenence");
NexPage programPage = NexPage(0, 0, "Program");
NexPage menuPage = NexPage(0, 0, "Menu");
NexPage statusPage = NexPage(0, 0, "Status");
NexPage loadingPage = NexPage(0, 0, "Loading");
NexPage productionPage = NexPage(0, 0, "Production");
NexPage OutputTestPage = NexPage(0, 0, "OutputTest");
NexPage BatchCompletePage = NexPage(0, 0, "CycleComplete");
NexPage StatusPage = NexPage(0, 0, "Status");
NexPage StartUpPage = NexPage(0,0,"Setup");
//buttons
enum Page
{
    MENU,
    STATUS,
    PRODUCTION,
    LOADING,
    MAINTENENCE,
    TESTING,
    PROGRAM
};

NexButton menuOKButton = NexButton(0, 8, "b0");
NexButton passwordOKButton = NexButton(3, 6, "b0");
NexButton passwordCancelButton = NexButton(3, 7, "b1");
NexButton zeroScaleButton = NexButton(6, 4, "b0");
NexButton cancelLoadingButton = NexButton(6, 5, "b2");
NexButton enterButtonProgramPage = NexButton(2, 4, "b0");
NexButton enterMaintenencePageButton = NexButton(4, 24, "b11");
NexButton StatusButton = NexButton(0, 7, "StatusSelect");
NexButton productionPageMenuButton = NexButton(5, 19, "b1");
NexButton productionPageStatusButton = NexButton(5, 20, "b2");
NexButton outputTestValveButton = NexButton(7, 1, "b0");
NexButton outputTestBackButton = NexButton(7, 5, "b1");
NexButton cancelMaintenenceButton = NexButton(4, 23, "b10");
NexButton cancelProgramButton = NexButton(2, 11, "b15");
//Text Objects
NexText mode1Select = NexText(0, 1, "Mode1Select");
NexText mode2Select = NexText(0, 2, "Mode2Select");
NexText mode3Select = NexText(0, 3, "Mode3Select");
NexText mode4Select = NexText(0, 5, "Mode4Select");
NexText mode5Select = NexText(0, 4, "Mode5Select");
NexText mode1Marker = NexText(0, 9, "t0");
NexText mode2Marker = NexText(0, 10, "t1");
NexText mode3Marker = NexText(0, 11, "t2");
NexText mode5Marker = NexText(0, 12, "t3");
NexText mode4Marker = NexText(0, 13, "t4");
NexText maintenenceOutputTestButton = NexText(4, 22, "t6");
NexText CurrentModeText = NexText(0, 14, "t5");
NexText statusValveState = NexText(1, 6, "ValveState");
NexText statusPageMenuButton = NexText(1, 7, "t1");
NexText outputTestSteamValveState = NexText(7, 4, "t2");

//numbers
NexNumber passwordNum1 = NexNumber(3, 1, "n0");
NexNumber passwordNum2 = NexNumber(3, 2, "n1");
NexNumber passwordNum3 = NexNumber(3, 3, "n2");
NexNumber passwordNum4 = NexNumber(3, 4, "n3");
NexNumber TEMP_SET_number = NexNumber(2, 5, "n0");
NexNumber TIME_SET_number = NexNumber(2, 6, "n1");
NexNumber BAND_GAP_number = NexNumber(4, 7, "n0");
NexNumber WIDTH_number = NexNumber(4, 8, "n1");
NexNumber BAND_OFFSET_number = NexNumber(4, 9, "n2");
NexNumber DELAY_TIME_number = NexNumber(4, 10, "n3");
NexNumber TEMP_CALIBRATE_number = NexNumber(4, 11, "n4");
NexNumber weightLoading_number = NexNumber(6, 3, "n2");
NexNumber productionSetTemp = NexNumber(5, 7, "n0");
NexNumber productionTemp = NexNumber(5, 8, "n1");
NexNumber productionWeight = NexNumber(5, 9, "n2");
NexNumber SET_TIME_HOUR = NexNumber(5, 10, "n3");
NexNumber SET_TIME_MINUTE = NexNumber(5, 11, "n4");
NexNumber TIME_LEFT_HOUR = NexNumber(5, 12, "n5");
NexNumber TIME_LEFT_MINUTE = NexNumber(5, 13, "n6");
NexNumber statusProdWeight = NexNumber(1, 8, "n0");
NexNumber statusTempSet = NexNumber(1, 9, "n1");
NexNumber statusProdTemp = NexNumber(1, 10, "n2");
NexNumber completeWeightNumber = NexNumber(8, 3, "n0");
NexTouch *nex_listen_list[] = {
    &mode1Select,
    &mode2Select,
    &mode3Select,
    &mode4Select,
    &mode5Select,
    &menuOKButton,
    &passwordOKButton,
    &passwordCancelButton,
    &zeroScaleButton,
    &cancelLoadingButton,
    &enterButtonProgramPage,
    &enterMaintenencePageButton,
    &StatusButton,
    &statusPageMenuButton,
    &productionPageMenuButton,
    &maintenenceOutputTestButton,
    &outputTestValveButton,
    &outputTestBackButton,
    &cancelMaintenenceButton,
    &cancelProgramButton,
    NULL};
class displayController
{

public:
    void displayInit_();
    void showPage(NexPage nextPage);
    void updateMenu(int currentMode_);
    void updateMaintenencePage(configController configController_);
    void updateProgramPage(configController configController_);
    void updateLoadingPage(float weight_);
    void updateProductionPage(float SET_TIME, timerController timerController_, sensorController sensorController_, int SET_TEMP);
    void updateStatusPage(GPIOController GPIOController_, configController configController_, sensorController sensorController_);
    void updateTestPage(GPIOController GPIOController_);
};
void displayController::updateTestPage(GPIOController GPIOController_)
{
    if (millis() - pageUpdateTimer_ > 1000)
    {
        if (GPIOController_.getValveState() == 1)
        {
            outputTestSteamValveState.setText("OPEN");
        }
        else
        {
            outputTestSteamValveState.setText("CLOSED");
        }
        pageUpdateTimer_ = millis();
    }
}
void displayController::updateStatusPage(GPIOController GPIOController_, configController configController_, sensorController sensorController_)
{

    if (millis() - pageUpdateTimer_ > 2000)
    {
        int valvestate = GPIOController_.getValveState();
        if (valvestate == 1)
        {
            statusValveState.setText("OPEN");
        }
        else if (valvestate == 0)
        {
            statusValveState.setText("CLOSED");
        }
        statusProdWeight.setValue(sensorController_.getWeight());
        statusTempSet.setValue(configController_.config_.TEMP_SET);
        statusProdTemp.setValue(sensorController_.readTemp(sensorController_.prodTempSensor()));
        pageUpdateTimer_ = millis();
    }
}
void displayController::updateProductionPage(float SET_TIME, timerController timerController_, sensorController sensorController_, int SET_TEMP)
{
    if (millis() - pageUpdateTimer_ > 1500)
    {
        SET_TIME_HOUR.setValue(SET_TIME / 60);

        SET_TIME_MINUTE.setValue((int)(SET_TIME) % 60);
        TIME_LEFT_HOUR.setValue((timerController_.timeLeft) / 3600);
        TIME_LEFT_MINUTE.setValue((int)(timerController_.timeLeft / 60) % 60);

        productionWeight.setValue(sensorController_.getWeight());
        productionTemp.setValue(sensorController_.readTemp(sensorController_.prodTempSensor()));
        productionSetTemp.setValue(SET_TEMP);
        pageUpdateTimer_ = millis();
    }
}
void displayController::updateProgramPage(configController configController_)
{
    TEMP_SET_number.setValue(configController_.config_.TEMP_SET);
    TIME_SET_number.setValue(configController_.config_.TIME_SET);
}
void displayController::updateMaintenencePage(configController configController_)
{
    BAND_GAP_number.setValue(configController_.config_.BAND_GAP);
    WIDTH_number.setValue(configController_.config_.WIDTH);
    BAND_OFFSET_number.setValue(configController_.config_.BAND_OFFSET);
    DELAY_TIME_number.setValue(configController_.config_.DELAY_TIME);
    TEMP_CALIBRATE_number.setValue(configController_.config_.TEMP_CALIBRATE);
}
void displayController::showPage(NexPage nextPage)
{
    nextPage.show();
}
void displayController::displayInit_()
{

    nexInit();

    //
}
void displayController::updateLoadingPage(float weight_)
{
    if (millis() - pageUpdateTimer_ > 500)
    {
        weightLoading_number.setValue(weight_);
        pageUpdateTimer_ = millis();
    }
}
void displayController::updateMenu(int currentMode_)
{

    switch (currentMode_)
    {
    case 0:
        CurrentModeText.setText("MODE_IDLE");
        mode1Select.Set_background_color_bco(3744);
        mode2Select.Set_background_color_bco(65535);
        mode3Select.Set_background_color_bco(65535);
        mode4Select.Set_background_color_bco(65535);
        mode5Select.Set_background_color_bco(65535);
        CurrentModeText.Set_background_color_bco(21502);

        mode1Marker.Set_background_color_bco(3744);
        mode2Marker.Set_background_color_bco(65535);
        mode3Marker.Set_background_color_bco(65535);
        mode4Marker.Set_background_color_bco(65535);
        mode5Marker.Set_background_color_bco(65535);
        break;
    case 1:
        CurrentModeText.setText("MODE_PRODUCTION");
        mode1Select.Set_background_color_bco(65535);
        mode2Select.Set_background_color_bco(3744);
        mode3Select.Set_background_color_bco(65535);
        mode4Select.Set_background_color_bco(65535);
        mode5Select.Set_background_color_bco(65535);
        CurrentModeText.Set_background_color_bco(21502);

        mode1Marker.Set_background_color_bco(65535);
        mode2Marker.Set_background_color_bco(3744);
        mode3Marker.Set_background_color_bco(65535);
        mode4Marker.Set_background_color_bco(65535);
        mode5Marker.Set_background_color_bco(65535);
        break;
    case 2:
        CurrentModeText.setText("MODE_PROGRAM");
        mode1Select.Set_background_color_bco(65535);
        mode2Select.Set_background_color_bco(65535);
        mode3Select.Set_background_color_bco(3744);
        mode4Select.Set_background_color_bco(65535);
        mode5Select.Set_background_color_bco(65535);
        CurrentModeText.Set_background_color_bco(21502);

        mode1Marker.Set_background_color_bco(65535);
        mode2Marker.Set_background_color_bco(65535);
        mode3Marker.Set_background_color_bco(3744);
        mode4Marker.Set_background_color_bco(65535);
        mode5Marker.Set_background_color_bco(65535);
        break;
    case 3:
        CurrentModeText.setText("MODE_MAINTENENCE");
        mode1Select.Set_background_color_bco(65535);
        mode2Select.Set_background_color_bco(65535);
        mode3Select.Set_background_color_bco(65535);
        mode4Select.Set_background_color_bco(3744);
        mode5Select.Set_background_color_bco(65535);
        CurrentModeText.Set_background_color_bco(21502);

        mode1Marker.Set_background_color_bco(65535);
        mode2Marker.Set_background_color_bco(65535);
        mode3Marker.Set_background_color_bco(65535);
        mode4Marker.Set_background_color_bco(3744);
        mode5Marker.Set_background_color_bco(65535);
        break;
    case 4:
        CurrentModeText.setText("MODE_LOADING");
        mode1Select.Set_background_color_bco(65535);
        mode2Select.Set_background_color_bco(65535);
        mode3Select.Set_background_color_bco(65535);
        mode4Select.Set_background_color_bco(65535);
        mode5Select.Set_background_color_bco(3744);
        CurrentModeText.Set_background_color_bco(21502);

        mode1Marker.Set_background_color_bco(65535);
        mode2Marker.Set_background_color_bco(65535);
        mode3Marker.Set_background_color_bco(65535);
        mode4Marker.Set_background_color_bco(65535);
        mode5Marker.Set_background_color_bco(3744);
        break;
    }
}
