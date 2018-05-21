#ifndef screenRTMSInfoH
#define screenRTMSInfoH
//---------------------------------------------------------------------------
#include "SCREEN.h"
//---------------------------------------------------------------------------
class ScreenRTMSInfo:public SCREEN
{
    public:
      ScreenRTMSInfo(void);
      ~ScreenRTMSInfo(void);

      void DisplayRTMSInfo(void);
      void DisplayCRTMSInfo(int);
      void doKeyWork(BYTE);                                                     //根據KEY的值作事

    private:

      BYTE rtmsInfoBitmap[3840];
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
extern ScreenRTMSInfo screenRTMSInfo;
#endif
