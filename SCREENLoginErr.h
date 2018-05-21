#ifndef SCREENLoginErrH
#define SCREENLoginErrH

#include "SCREEN.h"
//---------------------------------------------------------------------------
class SCREENLoginErr:public SCREEN
{
    public:
      SCREENLoginErr(void);
      ~SCREENLoginErr(void);

      void DisplayLoginErr(void);                                               //顯示登入錯誤畫面
      void DoKeyWork(BYTE);                                                     //根據KEY的值作事

    private:
      BYTE loginErrBitmap[3840];                                                //底圖
      void LoadBitmapFromFile(void);                                            //將底圖Load進記憶體
      void DoKeyEscWork();                                                      //按ESC鍵時作的事
      void DoKeyEnterWork();                                                    //按Enter鍵時作的事
};
//---------------------------------------------------------------------------
extern SCREENLoginErr screenLoginErr;
#endif
