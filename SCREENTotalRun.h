#ifndef SCREENTotalRunH
#define SCREENTotalRunH

#include "SCREEN.h"
//---------------------------------------------------------------------------
class SCREENTotalRun:public SCREEN
{
    public:

      SCREENTotalRun(void);
      ~SCREENTotalRun(void);

      void DisplayTotalRun(void);
      void DoKeyWork(BYTE);                                                     //根據KEY的值作事

      void DisplayTotalTime(void);                                              //顯示總開機累積時間

    private:

      DISP_WORD date[8];
      DISP_WORD time[6];
      DISP_WORD total[10];


      BYTE totalRunBitmap[3840];                                                //底圖

      void LoadBitmapFromFile(void);                                            //將底圖Load進記憶體
      void InitDispWord(void);                                                  //初始化此畫面的座標等參數

      void DisplayStartDateTime(void);                                          //顯示開機時間

      void DoKeyF1Work(void);
      void DoKeyF2Work(void);
      void DoKeyF4Work(void);
      void DoKeyEnterWork(void);
};
//---------------------------------------------------------------------------
extern SCREENTotalRun screenTotalRun;
#endif



