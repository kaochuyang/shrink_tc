#ifndef screenRedCountHWCheckNum88H
#define screenRedCountHWCheckNum88H
//---------------------------------------------------------------------------
#include "SCREEN.h"
//---------------------------------------------------------------------------
class ScreenRedCountHWCheckNum88:public SCREEN
{
    public:
      ScreenRedCountHWCheckNum88(void);
      ~ScreenRedCountHWCheckNum88(void);

      void DisplayRedCountHWCheckNum88(void);
      void doKeyWork(BYTE);                                                     //根據KEY的值作事

    private:
//      DISP_WORD planSel[2];                                                     //此頁的空白處
      DISP_WORD redCountHWCheckNum88;
      unsigned int cPosition;                                                   //現在的位置
//      BYTE planTemp[2];
      BYTE redCountHWCheckTmp;

      void initDispWord(void);                                                  //初始化此畫面的座標等參數
      void DisplaySetSegtype(int);

      BYTE RedCountHWCheckSelBitmap[3840];
      void loadBitmapFromFile(void);                                            //將底圖Load進記憶體

      void doKey0Work(void);
      void doKey1Work(void);
      void doKey2Work(void);
      void doKey3Work(void);
      void doKey4Work(void);
      void doKey5Work(void);
      void doKey6Work(void);
      void doKey7Work(void);
      void doKey8Work(void);
      void doKey9Work(void);
      void doKeyAWork(void);
      void doKeyBWork(void);
      void doKeyCWork(void);
      void doKeyDWork(void);
      void doKeyEWork(void);
      void doKeyFWork(void);
      void doKeyF1Work(void);
      void doKeyF2Work(void);
      void doKeyF3Work(void);
      void doKeyF4Work(void);
      void doKeyUPWork(void);
      void doKeyDOWNWork(void);
      void doKeyLEFTWork(void);
      void doKeyRIGHTWork(void);
      void doKeyEnterWork(void);
};
//---------------------------------------------------------------------------
extern ScreenRedCountHWCheckNum88 screenRedCountHWCheckNum88;
#endif
