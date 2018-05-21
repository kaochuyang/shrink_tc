#ifndef SCREENACTUATEPLAYSWITCH_H_INCLUDED
#define SCREENACTUATEPLAYSWITCH_H_INCLUDED

#include "SCREEN.h"
class ScreenActuatePlaySwitch:public SCREEN
{
    public:
      ScreenActuatePlaySwitch(void);
      ~ScreenActuatePlaySwitch(void);

      void DisplayActType(int);
      void doKeyWork(BYTE);                                                     //®Ú¾ÚKEYªº­È§@¨Æ

    private:
      BYTE actuatePlayBitmap[3840];
      void loadBitmapFromFile(void);
      void initDispWord(void);
      void DisplayExtendValueAndSwitch(void);
      void getsegmentActData(void);
      int cPosition;
      int cSelect;
      int cFace;
      int usActplayNo;
      int usiActType;
      int usiSwitch[8];
      int usiExtendSec[8];
      int usiActionSec;
      int usiCompensateMode;
      int valueActionSec[2];
      int valueSwitch[8];
      int valueExtendSec[8];
      int valueCompensateMode;

      unsigned char ucTMP_ActuatePlan[2];
      unsigned short int usiActuatePlan;

      DISP_WORD positionSwitch[8];
      DISP_WORD positionExtendSec[8];
      DISP_WORD cPlanNo;
      DISP_WORD cActuateMode;
      DISP_WORD cActuateFlashPlan[2];
      DISP_WORD cStartSec[2];
      DISP_WORD cCompensateMode;


      //
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
extern ScreenActuatePlaySwitch screenActuatePlaySwitch;
#endif // SCREENACTUATEPLAYSWITCH_H_INCLUDED
