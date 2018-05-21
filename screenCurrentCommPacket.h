#ifndef screenCurrentCommPacketH
#define screenCurrentCommPacketH

//---------------------------------------------------------------------------
#include "SCREEN.h"
//---------------------------------------------------------------------------
class ScreenCurrentCommPacket:public SCREEN
{
    public:
      ScreenCurrentCommPacket(void);
      ~ScreenCurrentCommPacket(void);

      void DisplayCurrentCommPacket(void);
      void doKeyWork(BYTE);                                                     //根據KEY的值作事

      void vRefreshCurrentScreenPacket(BYTE *, int, char *);
//      void vRefreshStepSec(void);

    private:
      DISP_WORD markPacket[6][30];
      void initDispWord(void);                                                  //初始化此畫面的座標等參數
      int cSelect;                                                              //現在選擇的選項
      static int iLinePtr;
      static int iLineShiftPtr;

      BYTE ucCurrentCommPacketBitmap[3840];
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
extern ScreenCurrentCommPacket screenCurrentCommPacket;
#endif
