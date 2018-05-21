#ifndef screenCarCountdownPropertyH
#define screenCarCountdownPropertyH
//---------------------------------------------------------------------------
#include "SCREEN.h"
//---------------------------------------------------------------------------
class ScreenCarCountdownProperty:public SCREEN
{
    public:
        ScreenCarCountdownProperty(void);
        ~ScreenCarCountdownProperty(void);

        void DoKeyWork(BYTE);
        void DisplayCarCountdownProperty(void);

   private:
        BYTE CarCountdownPropertyBitmap[3840];
        DISP_WORD current;
        unsigned short int tmpCarCountdownProperty;

        void LoadBitmapFromFile(void);
        void InitDispWord(void);

        void DoKey1Work(void);
        void DoKey2Work(void);
        void DoKeyEnterWork(void);
        void DoKeyF1Work(void);
        void DoKeyF4Work(void);
};
//---------------------------------------------------------------------------
extern ScreenCarCountdownProperty screenCarCountdownProperty;
#endif

