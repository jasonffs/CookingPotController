#include "operationModeController.h"


void operationModeController::changeMode(operationModes nextMode)
{
    if (this->currentMode_ == nextMode)
    {
        return;
    }
    else
    {
        this->previousMode_ = this->currentMode_;
        this->currentMode_ = nextMode;
    }
}
