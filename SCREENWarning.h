#ifndef SCREENWarningH
#define SCREENWarningH

#include "SCREEN.h"
//---------------------------------------------------------------------------
class SCREENWarning:public SCREEN
{
    public:
        SCREENWarning(void);
        ~SCREENWarning(void);

        void DisplayWarning(void);
        void DoKeyWork(BYTE);                                                   //根據KEY的值作事



    private:
        BYTE ucBitmap[3840];                                              //底圖

        void LoadBitmapFromFile(void);                                          //將底圖Load進記憶體

        void DoKeyF1Work();
        void DoKeyF2Work();
        void DoKeyF3Work();
        void DoKeyF4Work();
};
//---------------------------------------------------------------------------
extern SCREENWarning screenWarning;
#endif
