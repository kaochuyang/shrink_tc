#ifndef screenPedLearnH
#define screenPedLearnH
//---------------------------------------------------------------------------
#include "SCREEN.h"
//---------------------------------------------------------------------------
class ScreenPedLearn:public SCREEN
{
    public:
        ScreenPedLearn(void);
        ~ScreenPedLearn(void);

        void DoKeyWork(BYTE);
        void DisplayPedLearn(void);

   private:
        BYTE PedLearnBitmap[3840];
        DISP_WORD current;
        unsigned short int tmpPedLearn;

        void LoadBitmapFromFile(void);
        void InitDispWord(void);

        void DoKey0Work(void);
        void DoKey1Work(void);
        void DoKeyEnterWork(void);
        void DoKeyF1Work(void);
        void DoKeyF4Work(void);
};
//---------------------------------------------------------------------------
extern ScreenPedLearn screenPedLearn;
#endif

