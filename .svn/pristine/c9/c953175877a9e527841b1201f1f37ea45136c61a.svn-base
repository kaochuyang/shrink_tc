#ifndef screenPrioritymenuH
#define screenPrioritymenuH
//---------------------------------------------------------------------------
#include "SCREEN.h"
//---------------------------------------------------------------------------
class ScreenPrioritymenu:public SCREEN
{
    public:
      ScreenPrioritymenu(void);
      ~ScreenPrioritymenu(void);

      void DisplayPrioritymenu(void);
      void doKeyWork(BYTE);                                                     //®Ú¾ÚKEYªº­È§@¨Æ

    private:
      DISP_WORD mark[3];
      void initDispWord(void);                                                  //ªì©l¤Æ¦¹µe­±ªº®y¼Ðµ¥°Ñ¼Æ
      int cSelect;
      BYTE PrioritymenuBitmap[3840];
      void loadBitmapFromFile(void);                                            //±N©³¹ÏLoad¶i°O¾ÐÅé

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
extern ScreenPrioritymenu screenPrioritymenu;
#endif
