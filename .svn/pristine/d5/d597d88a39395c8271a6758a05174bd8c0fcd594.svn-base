#ifndef SCREENACTUATESTRATEGYMENU_H_INCLUDED
#define SCREENACTUATESTRATEGYMENU_H_INCLUDED

#include "SCREEN.h"
#include "var.h"

class SCREENActuateStrategyMenu:public SCREEN
{

    public:
    SCREENActuateStrategyMenu();
    ~SCREENActuateStrategyMenu();


    void DisplayActuateStrategyMenu(void);
    void doKeyWork(BYTE);
    private:

      void initDispWord(void);
      void DisplayActuateCloseMode(void);       //no use chou 2013/01/30

      DISP_WORD mark[10];
      DISP_WORD markActType[3];
      int cSelect;
      int usiActType;

      BYTE ActuateStrategyBitmap[3840];
      BYTE ActuateCloseModeBitmap[3840];
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
extern SCREENActuateStrategyMenu screenActuateStrategyMenu;
#endif // SCREENACTUATESTRATEGYMENU_H_INCLUDED
