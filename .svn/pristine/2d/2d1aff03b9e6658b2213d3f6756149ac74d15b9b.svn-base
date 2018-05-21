#ifndef screenPedCountdownPropertyH
#define screenPedCountdownPropertyH
//---------------------------------------------------------------------------
#include "SCREEN.h"
//---------------------------------------------------------------------------
class ScreenPedCountdownProperty:public SCREEN
{
    public:
        ScreenPedCountdownProperty(void);
        ~ScreenPedCountdownProperty(void);

        void DoKeyWork(BYTE);
        void DisplayPedCountdownProperty(void);

   private:
        BYTE PedCountdownPropertyBitmap[3840];
        DISP_WORD current;
        unsigned short int tmpPedCountdownProperty;

        void LoadBitmapFromFile(void);
        void InitDispWord(void);

        void DoKey1Work(void);
        void DoKey2Work(void);
        void DoKeyEnterWork(void);
        void DoKeyF1Work(void);
        void DoKeyF4Work(void);
};
//---------------------------------------------------------------------------
extern ScreenPedCountdownProperty screenPedCountdownProperty;
#endif


