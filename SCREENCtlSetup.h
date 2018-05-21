#ifndef SCREENCtlSetupH
#define SCREENCtlSetupH

#include "SCREEN.h"
//---------------------------------------------------------------------------
class SCREENCtlSetup:public SCREEN
{
    public:

      SCREENCtlSetup(void);
      ~SCREENCtlSetup(void);

      void DisplayCtlSetup(void);                                               //顯示ControlSetup頁
      void DoKeyWork(BYTE);                                                     //根據KEY的值作事
      void DisplayDateTime(void);

    private:

      DISP_WORD dateWord[8];                                                    //顯示日期的位置
      DISP_WORD timeWord[6];                                                    //顯示時間的位置
      DISP_WORD mark[10];                                                       //顯示游標的位置
      int cSelect;                                                              //現在選擇的選項

      BYTE ctlSetupBitmap[3840];                                                //底圖
      void LoadBitmapFromFile(void);                                            //將底圖Load進記憶體
      void InitDispWord(void);                                                  //初始化空白位置

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
      void DoKeyF1Work(void);
      void DoKeyF2Work(void);
      void DoKeyF3Work(void);
      void DoKeyF4Work(void);
      void DoKeyUPWork(void);
      void DoKeyDOWNWork(void);
      void DoKeyLEFTWork(void);
      void DoKeyRIGHTWork(void);
      void DoKeyEnterWork(void);
};
//---------------------------------------------------------------------------
extern SCREENCtlSetup screenCtlSetup;
#endif
