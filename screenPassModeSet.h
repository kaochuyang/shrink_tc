#ifndef screenPassModeSetH
#define screenPassModeSetH

//---------------------------------------------------------------------------
#include "SCREEN.h"
//---------------------------------------------------------------------------
class ScreenPassModeSet:public SCREEN
{
    public:
      ScreenPassModeSet(void);
      ~ScreenPassModeSet(void);

      void DisplayPassModeSet(void);
      void doKeyWork(BYTE);                                                     //根據KEY的值作事

    private:

      unsigned int cPosition;                                                   //現在的位置
      int cSelect;                                                              //上下切換   0:日期 1:時間

      DISP_WORD cPassModeSwitch;
      unsigned char ucTMP_PassModeSwitch;
      DISP_WORD cPassServerLCN[4];
      unsigned char ucTMP_PassServerLCN[4];
      unsigned short int usiPassServerLCN;

      void initDispWord(void);                                                  //初始化此畫面的座標等參數
//      void DisplaySetPassMode(int,int);

      BYTE passModeSetBitmap[3840];
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

      void vDisplayPassModeSwitch(void);
      void vDisplayPassModeServer(void);

      void vDataToTmpArray();
      void vTmpArrayToData();

      bool vCheckRationality();
      bool vInitUCTMPData(void);

};
//---------------------------------------------------------------------------
extern ScreenPassModeSet screenPassModeSet;
#endif
