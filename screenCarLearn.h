#ifndef screenCarLearnH
#define screenCarLearnH
//---------------------------------------------------------------------------
#include "SCREEN.h"
//---------------------------------------------------------------------------
class ScreenCarLearn:public SCREEN
{
    public:
        ScreenCarLearn(void);
        ~ScreenCarLearn(void);

        void DoKeyWork(BYTE);
        void DisplayCarLearn(void);

   private:
        BYTE CarLearnBitmap[3840];
        DISP_WORD current;
        unsigned short int tmpCarLearn;

        void LoadBitmapFromFile(void);
        void InitDispWord(void);

        void DoKey0Work(void);
        void DoKey1Work(void);
        void DoKeyEnterWork(void);
        void DoKeyF1Work(void);
        void DoKeyF4Work(void);
};
//---------------------------------------------------------------------------
extern ScreenCarLearn screenCarLearn;
#endif
