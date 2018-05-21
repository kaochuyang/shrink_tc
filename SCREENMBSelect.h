#ifndef SCREENMBSelectH
#define SCREENMBSelectH

#include "SCREEN.h"
//---------------------------------------------------------------------------
class SCREENMBSelect:public SCREEN
{
    public:

      SCREENMBSelect(void);
      ~SCREENMBSelect(void);

      void DisplayMBSelect(void);                                                //顯示
      void DoKeyWork(BYTE);                                                     //根據KEY的值作事

    private:

      DISP_WORD cycle;
      DISP_WORD mark[6];                                                        //顯示游標的位置
      int cSelect;                                                              //現在選擇的選項

      BYTE ucBitmap[3840];                                                 //底圖
      void LoadBitmapFromFile(void);                                            //將底圖Load進記憶體
      void InitDispWord(void);                                                  //初始化空白位置

      void DoKey0Work(void);
      void DoKey1Work(void);
      void DoKey2Work(void);
      void DoKey3Work(void);
      void DoKey4Work(void);
      void DoKey5Work(void);

      void DoKeyF1Work(void);
      void DoKeyF3Work(void);
      void DoKeyF4Work(void);

      void DoKeyUPWork(void);
      void DoKeyDOWNWork(void);
      void DoKeyLEFTWork(void);
      void DoKeyRIGHTWork(void);
      void DoKeyEnterWork(void);
};
//---------------------------------------------------------------------------
extern SCREENMBSelect screenMBSelect;
#endif
