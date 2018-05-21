#ifndef SCREENLoginH
#define SCREENLoginH

#include "SCREEN.h"
//---------------------------------------------------------------------------
class SCREENLogin:public SCREEN
{
    public:

      SCREENLogin(void);
      ~SCREENLogin(void);

      void DisplayLogin(void);                                                  //顯示LOGIN頁
      void DoKeyWork(BYTE);                                                     //根據KEY的值作事


    private:

      DISP_WORD pwd[6];                                                         //此頁的空白處,定義座標
      unsigned int cPosition;                                                   //現在的位置
      char passWord[6];                                                         //記錄密碼

      BYTE loginBitmap[3840];                                                   //底圖

      void LoadBitmapFromFile(void);                                            //將底圖Load進記憶體
      void InitDispWord(void);                                                  //初始化此畫面的座標等參數

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
      void DoKeyAWork(void);
      void DoKeyBWork(void);
      void DoKeyCWork(void);
      void DoKeyDWork(void);
      void DoKeyEWork(void);
      void DoKeyFWork(void);
      void DoKeyF1Work(void);
      void DoKeyF2Work(void);
      void DoKeyUPWork(void);
      void DoKeyDOWNWork(void);
      void DoKeyLEFTWork(void);
      void DoKeyRIGHTWork(void);
      void DoKeyEnterWork(void);
};
//---------------------------------------------------------------------------
extern SCREENLogin screenLogin;
#endif
