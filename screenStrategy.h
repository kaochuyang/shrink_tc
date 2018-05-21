#ifndef screenStrategyH
#define screenStrategyH
//---------------------------------------------------------------------------
#include "SCREEN.h"
//---------------------------------------------------------------------------
class ScreenStrategy:public SCREEN
{
    public:
      ScreenStrategy(void);
      ~ScreenStrategy(void);

      void DisplayStrategy(void);
      void DisplayCStrategy(void);
      void doKeyWork(BYTE);                                                     //根據KEY的值作事

    private:

      BYTE strategyBitmap[3840];
      void loadBitmapFromFile(void);                                            //將底圖Load進記憶體

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
extern ScreenStrategy screenStrategy;
#endif
