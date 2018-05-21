#ifndef screenChainMenuH
#define screenChainMenuH
//---------------------------------------------------------------------------
#include "SCREEN.h"
//---------------------------------------------------------------------------
class ScreenChainMenu:public SCREEN
{
    public:
      ScreenChainMenu(void);
      ~ScreenChainMenu(void);

      void DisplayChainMenu(void);
      void doKeyWork(BYTE);

    private:
      DISP_WORD mark[10];
      void initDispWord(void);
      int cSelect;

      BYTE chainMenuBitmap[3840];
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
extern ScreenChainMenu screenChainMenu;
#endif
