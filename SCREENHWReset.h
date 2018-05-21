#ifndef SCREENHWResetH
#define SCREENHWResetH

#include "SCREEN.h"
#include "var.h"
//---------------------------------------------------------------------------
class SCREENHWReset:public SCREEN
{
    public:
        SCREENHWReset(void);
        ~SCREENHWReset(void);

        void DisplayHWReset(void);
        void DoKeyWork(BYTE);                                                   //根據KEY的值作事

    private:

        BYTE hwResetBitmap[3840];
        DISP_WORD dateWord[8];                                                  //顯示日期的位置
        DISP_WORD timeWord[6];                                                  //顯示時間的位置

        void LoadBitmapFromFile(void);                                          //將底圖Load進記憶體
        void InitDispWord(void);                                                //初始化此畫面的座標等參數
        void DisplayDateTime(void);
};
//---------------------------------------------------------------------------
extern SCREENHWReset screenHWReset;
#endif
