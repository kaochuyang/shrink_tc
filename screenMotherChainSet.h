#ifndef ScreenMotherChainSetH
#define ScreenMotherChainSetH

#include "SCREEN.h"
//---------------------------------------------------------------------------
class ScreenMotherChainSet:public SCREEN
{
    public:
      ScreenMotherChainSet(void);
      ~ScreenMotherChainSet(void);

      void DisplayMotherChainSet(void);
      void DoKeyWork(BYTE);                                                     //根據KEY的值作事

    private:
      DISP_WORD cMotherChainSetWord[6];
      /*
      0:  ucChainStartPhaseWord;
      1:  ucChainStartStepWord;
      2:  ucChainEndPhaseWord;
      3:  ucChainEndStepWord;
      4:  ucChainManuleEnable;
      5: ChainTODEnable;
      */

      BYTE ucMotherChainSet[6];

/*
      BYTE ucChainStartPhaseWord;
      BYTE ucChainStartStepWord;
      BYTE ucChainEndPhaseWord;
      BYTE ucChainEndStepWord;

      BYTE ucChainManuleEnable;
      BYTE ucChainTODEnable;
*/

      int cPosition;                                                            //左右移動的位置
//      int cSelect;                                                              //上下切換   0:日期 1:時間

      void InitDispWord(void);                                                  //初始化此畫面的座標等參數
      void DisplayData(void);

      BYTE chainMotherSetBitmap[3840];
      void LoadBitmapFromFile(void);                                            //將底圖Load進記憶體

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
      void DoKeyF4Work(void);
      void doKeyUPWork(void);
      void doKeyDOWNWork(void);
      void DoKeyLEFTWork(void);
      void DoKeyRIGHTWork(void);
      void DoKeyEnterWork(void);
};
//---------------------------------------------------------------------------
extern ScreenMotherChainSet screenMotherChainSet;
#endif
