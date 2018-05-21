#ifndef ScreenChildChainSetH
#define ScreenChildChainSetH

#include "SCREEN.h"
//---------------------------------------------------------------------------
class ScreenChildChainSet:public SCREEN
{
    public:
      ScreenChildChainSet(void);
      ~ScreenChildChainSet(void);

      void DisplayChildChainSet(void);
      void DoKeyWork(BYTE);

    private:
      DISP_WORD cChainLockPhaseSetWord[2];
      DISP_WORD cChainStartOffsetSignedWord;
      DISP_WORD cChainStartOffsetWord[3];
      DISP_WORD cChainEndOffsetSignedWord;
      DISP_WORD cChainEndOffsetWord[3];

      BYTE ucChainLockPhaseSet[2];
      BYTE ucChainStartOffsetSigned;
      BYTE ucChainStartOffset[3];
      BYTE ucChainEndOffsetSigned;
      BYTE ucChainEndOffset[3];

      int cPosition;
      int cSelect;

      void InitDispWord(void);                                                  //初始化此畫面的座標等參數
      void DisplayData(void);

      BYTE chainChildSetBitmap[3840];
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
extern ScreenChildChainSet screenChildChainSet;
#endif
