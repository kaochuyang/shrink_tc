#ifndef screenPriorityH
#define screenPriorityH
//---------------------------------------------------------------------------
#include "SCREEN.h"
//---------------------------------------------------------------------------
class ScreenPriority:public SCREEN
{
    public:
        ScreenPriority(void);
        ~ScreenPriority(void);

        void doKeyWork(BYTE);
        void DisplayPriority(void);
        void vRefreshCurrentStep(void);
        void vRefreshStepSec(void);
        void vPrioritytState(void);
        void vPrioritystateclear(void);

   private:
        BYTE BRTpriorityBitmap[3840];
        DISP_WORD markStep[4];
        DISP_WORD markStepSecCount[4];
        DISP_WORD carposition[8];
        DISP_WORD Rcarposition[8];
        DISP_WORD startpriority[3];
        DISP_WORD delay[10];
        void loadBitmapFromFile(void);
        void initDispWord(void);
        void doKeyF1Work(void);
        void doKeyF4Work(void);
};
//---------------------------------------------------------------------------
extern ScreenPriority screenPriority;
#endif
