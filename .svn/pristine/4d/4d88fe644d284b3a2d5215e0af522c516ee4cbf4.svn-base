#ifndef SCREENCtlNoH
#define SCREENCtlNoH

#include "SCREEN.h"
//---------------------------------------------------------------------------
class SCREENCtlNo:public SCREEN
{
    public:

      SCREENCtlNo(void);
      ~SCREENCtlNo(void);

      void DisplayCtlNo(void);                                                  //顯示設備編號頁
      void DoKeyWork(BYTE);                                                     //根據KEY的值作事

      void DisplayNum(void);

    private:

      DISP_WORD lcn[5];
      int address;                                                        //此頁的空白處,定義座標

      BYTE ctlNoBitmap[3840];                                                   //底圖
      //jacky20140311 BRT
      char LCNaddress[5];
      int   tmpaddress;
      unsigned int cPosition;

      void LoadBitmapFromFile(void);                                            //將底圖Load進記憶體
      void InitDispWord(void);                                                  //初始化此畫面的座標等參數

      //20140226 BRT
      void DoKey0Work(void);
      void DoKey1Work(void);
      void DoKey2Work(void);
      void DoKey3Work(void);
      void DoKey4Work(void);
      void DoKey5Work(void);
      void DoKey6Work(void);
      void DoKey7Work(void);
      void DoKey8Work(void);
      void DoKey9Work(void);
      void DoKeyUPWork(void);
      void DoKeyDOWNWork(void);
      void DoKeyLEFTWork(void);
      void DoKeyRIGHTWork(void);
      void DoKeyEnterWork(void);

      void DoKeyF1Work(void);
      void DoKeyF2Work(void);
      void DoKeyF4Work(void);
};
//---------------------------------------------------------------------------
extern SCREENCtlNo screenCtlNo;
#endif

