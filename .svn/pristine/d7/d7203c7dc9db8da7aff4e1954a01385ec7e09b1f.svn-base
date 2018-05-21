#ifndef SCREENOperModeH
#define SCREENOperModeH

#include "SCREEN.h"
//---------------------------------------------------------------------------
class SCREENOperMode:public SCREEN
{
    public:
        SCREENOperMode(void);
        ~SCREENOperMode(void);

        void DisplayOperMode(void);
        void DoKeyWork(BYTE);                                                   //根據KEY的值作事

        void UpdateOperMode(void);

    private:
        BYTE operModeBitmap[3840];                                              //底圖

        void LoadBitmapFromFile(void);                                          //將底圖Load進記憶體

        void DoKeyF1Work();
        void DoKeyF4Work();
};
//---------------------------------------------------------------------------
extern SCREENOperMode screenOperMode;
#endif

