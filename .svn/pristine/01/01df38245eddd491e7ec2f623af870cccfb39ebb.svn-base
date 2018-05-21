#ifndef SCREENSYSTEMINFOMENU_H_INCLUDED
#define SCREENSYSTEMINFOMENU_H_INCLUDED

#include "SCREEN.h"
#include "var.h"

class SCREENSystemInfoMenu:public SCREEN
{

    public:
    SCREENSystemInfoMenu();
    ~SCREENSystemInfoMenu();


    void DisplaySystemInfoMenu(void);
      void doKeyWork(BYTE);

    private:
      DISP_WORD mark[10];
      void initDispWord(void);
      int cSelect;

      BYTE systemInfoBitmap[3840];
      void loadBitmapFromFile(void);

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
extern SCREENSystemInfoMenu screenSystemInfoMenu;
#endif // SCREENSYSTEMINFOMENU_H_INCLUDED
