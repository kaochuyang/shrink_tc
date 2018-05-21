#ifndef SCREENOperStatH
#define SCREENOperStatH

#include "SCREEN.h"
//---------------------------------------------------------------------------
class SCREENOperStat:public SCREEN
{
    public:
        SCREENOperStat(void);
        ~SCREENOperStat(void);

        void DisplayOperStat(void);
        void DoKeyWork(BYTE);                                                   //根據KEY的值作事

        void vShowGPSTime(time_t);
        void vShowMachineLocation(void);

        void vShowGreenConflict(void);

    private:
        BYTE operStatBitmap[3840];                                              //底圖

        void LoadBitmapFromFile(void);                                          //將底圖Load進記憶體

        void DoKeyF1Work();
        void DoKeyF2Work();
        void DoKeyF3Work();
        void DoKeyF4Work();
        void DoKeyEnterWork();

        DISP_WORD dateWord[8];                                                    //顯示日期的位置
        DISP_WORD timeWord[6];                                                    //顯示時間的位置
        DISP_WORD GreenConflictWord[6];
        DISP_WORD HWStatusWord[4];
        DISP_WORD MachineLocationWord;
        void InitDispWord(void);                                                  //初始化空白位置
};
//---------------------------------------------------------------------------
extern SCREENOperStat screenOperStat;
#endif

