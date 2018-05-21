#ifndef SCREENLast92TCPlanSegmentUpdateH
#define SCREENLast92TCPlanSegmentUpdateH

#include "SCREEN.h"
//---------------------------------------------------------------------------
class SCREENLast92TCPlanSegmentUpdate:public SCREEN
{
    public:

      SCREENLast92TCPlanSegmentUpdate(void);
      ~SCREENLast92TCPlanSegmentUpdate(void);

      void DisplayLastUpdate(void);                                               //顯示斷電時間頁
      void DoKeyWork(BYTE);                                                     //根據KEY的值作事
      void DisplayPlanUpdate(void);
      void DisplaySegmentUpdate(void);


    private:

      DISP_WORD DLastTime[12];                                                    //此頁的空白處,定義座標
      DISP_WORD DPlanID[2];
      DISP_WORD DSegmentType[2];
      unsigned short int usiSegmentType;
      unsigned short int usiPlanID;

      BYTE lastDownBitmap[3840];                                                //底圖

      void LoadBitmapFromFile(void);                                            //將底圖Load進記憶體
      void InitDispWord(void);                                                  //初始化此畫面的座標等參數

      void DoKeyF1Work(void);
      void DoKeyF4Work(void);
};
//---------------------------------------------------------------------------
extern SCREENLast92TCPlanSegmentUpdate screenLast92TCPlanSegmentUpdate;
#endif
