#ifndef ScreenRailwayChainSetH
#define ScreenRailwayChainSetH

#include "SCREEN.h"
//---------------------------------------------------------------------------
class ScreenRailwayChainSet:public SCREEN
{
    public:
      ScreenRailwayChainSet(void);
      ~ScreenRailwayChainSet(void);

      void DisplayRailwayChainSet(void);
      void DoKeyWork(BYTE);

    private:
      DISP_WORD cChainTrainEnableWord;
      DISP_WORD cChainTrainComingJumpSubphaseWord;
      DISP_WORD cChainTrainComingBanSubphaseWord;

      BYTE ucChainTrainEnable;
      BYTE ucChainTrainComingJumpSubphase;
      BYTE ucChainTrainComingBanSubphase;

      int cPosition;
      int cSelect;

      void InitDispWord(void);
      void DisplayData(void);

      BYTE chainRailwaySetBitmap[3840];
      void LoadBitmapFromFile(void);

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
extern ScreenRailwayChainSet screenRailwayChainSet;
#endif
