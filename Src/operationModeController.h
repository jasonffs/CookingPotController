

enum operationModes{
   MODE_IDLE,
   MODE_PRODUCTION,
   MODE_PROGRAM,
   MODE_MAINTENENCE,
   MODE_LOADING,
   MODE_TESTING
};

class operationModeController
{

   
public:
   operationModes currentMode_;
   operationModes previousMode_;
   operationModes selectedMode_;

   operationModeController()
   {
       this->currentMode_ = MODE_IDLE;
       this->previousMode_ = MODE_IDLE;
   }
   void changeMode(operationModes nextMode);
   
   
};
