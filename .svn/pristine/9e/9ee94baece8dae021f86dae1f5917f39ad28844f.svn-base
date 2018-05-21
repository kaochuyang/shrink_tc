#ifndef SCREENLastDownH
#define SCREENLastDownH

#include "SCREEN.h"
//---------------------------------------------------------------------------
class SCREENLastDown:public SCREEN
{
    public:

      SCREENLastDown(void);
      ~SCREENLastDown(void);

      void DisplayLastDown(void);                                               //顯示斷電時間頁
      void DoKeyWork(BYTE);                                                     //根據KEY的值作事

    private:

      DISP_WORD poweroff[8];                                                    //此頁的空白處,定義座標

      BYTE lastDownBitmap[3840];                                                //底圖

      void LoadBitmapFromFile(void);                                            //將底圖Load進記憶體
      void InitDispWord(void);                                                  //初始化此畫面的座標等參數

      void DoKeyF1Work(void);
      void DoKeyF4Work(void);
};
//---------------------------------------------------------------------------
extern SCREENLastDown screenLastDown;
#endif

