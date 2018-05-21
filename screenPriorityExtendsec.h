#ifndef screenPriorityExtendsecH
#define screenPriorityExtendsecH
//---------------------------------------------------------------------------
#include "SCREEN.h"
//---------------------------------------------------------------------------
class ScreenPriorityExtendsec:public SCREEN
{
    public:
        ScreenPriorityExtendsec(void);
        ~ScreenPriorityExtendsec(void);

        void doKeyWork(BYTE);
        void DisplayPriorityExtendsec(void);
        void PlanPhase(void);
        void information(void);

   private:
        BYTE BRTpriorityBitmap[3840];
        DISP_WORD planid[3];
        DISP_WORD subphase[4];
        DISP_WORD eastPercentage[16];
        DISP_WORD westPercentage[16];
        DISP_WORD eastsubphase[8];
        DISP_WORD westsubphase[8];
        DISP_WORD onoff;
        void loadBitmapFromFile(void);
        void initDispWord(void);

        void doKeyF1Work(void);
        void doKeyF4Work(void);
};
//---------------------------------------------------------------------------
extern ScreenPriorityExtendsec screenPriorityExtendsec;
#endif
