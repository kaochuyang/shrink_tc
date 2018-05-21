#ifndef SCREENDBSetupH
#define SCREENDBSetupH

#include "SCREEN.h"
//---------------------------------------------------------------------------
class SCREENDBSetup:public SCREEN
{
    public:

      SCREENDBSetup(void);
      ~SCREENDBSetup(void);

      void DisplayDBSetup(void);                                                //顯示
      void DoKeyWork(BYTE);                                                     //根據KEY的值作事

    private:

      DISP_WORD db;
      DISP_WORD mark[3];                                                        //顯示游標的位置
      int cSelect;                                                              //現在選擇的選項

      BYTE dbSetupBitmap[3840];                                                 //底圖
      void LoadBitmapFromFile(void);                                            //將底圖Load進記憶體
      void InitDispWord(void);                                                  //初始化空白位置

      void DoKey0Work(void);
      void DoKey1Work(void);
      void DoKey2Work(void);

      void DoKeyF1Work(void);
      void DoKeyF4Work(void);
      void DoKeyUPWork(void);
      void DoKeyDOWNWork(void);
      void DoKeyLEFTWork(void);
      void DoKeyRIGHTWork(void);
      void DoKeyEnterWork(void);
};
//---------------------------------------------------------------------------
extern SCREENDBSetup screenDBSetup;
#endif



