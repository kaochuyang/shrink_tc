#ifndef SCREENFWVerDTH
#define SCREENFWVerDTH

#include "SCREEN.h"
#include "var.h"
//---------------------------------------------------------------------------
class SCREENFWVerDT:public SCREEN
{
    public:
        SCREENFWVerDT(void);
        ~SCREENFWVerDT(void);

        void DisplayFWVerDT(void);                                              //顯示
        void DoKeyWork(BYTE);                                                   //根據KEY的值作事

    private:
        BYTE fwVerDTBitmap[3840];                                               //底圖

        DISP_WORD version[2];
        DISP_WORD fwDate[8];
        DISP_WORD fwW77E58[6];

        void InitDispWord(void);

        void LoadBitmapFromFile(void);                                          //將底圖Load進記憶體
        void DoKeyF1Work(void);
        void DoKeyF2Work(void);
        void DoKeyF3Work(void);
        void DoKeyF4Work(void);
};
//---------------------------------------------------------------------------
extern SCREENFWVerDT screenFWVerDT;
#endif

