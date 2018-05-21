#ifndef screenCompensationsetH
#define screenCompensationsetH
//---------------------------------------------------------------------------
#include "SCREEN.h"
//---------------------------------------------------------------------------
class ScreenCompensationset:public SCREEN
{
    public:
        ScreenCompensationset(void);
        ~ScreenCompensationset(void);

        void DoKeyWork(BYTE);
        void Displaycompensation(void);

   private:
        BYTE CompensationBitmap[3840];
        DISP_WORD current;
        unsigned short int tmpcycle;

        void LoadBitmapFromFile(void);
        void InitDispWord(void);

        void DoKey1Work(void);

        void DoKeyEnterWork(void);
        void DoKey2Work(void);
        void DoKeyF1Work(void);
      void DoKeyF4Work(void);
};
//---------------------------------------------------------------------------
extern ScreenCompensationset screenCompensationset;
#endif
