#ifndef SCREENFLASHMODE_H_INCLUDED
#define SCREENFLASHMODE_H_INCLUDED
#include "SCREEN.h"
#include <stdio.h>
#include <string.h>
#include "ConfigFile.h"
class ScreenFlashMode:public SCREEN
{
	public:
	ScreenFlashMode();
	~ScreenFlashMode();
	void DisplayFlashMode( );

    void DoKeyWork(BYTE);

    private:

      DISP_WORD cActuateType_By_TOD;
      DISP_WORD positionSwitch[8];
      DISP_WORD positionActionSec[2];
      DISP_WORD cActuatePlan[2];
      unsigned char ucTMP_ActuatePlan[2];
      unsigned short int usiActuatePlan;
      unsigned int cPosition;
      unsigned int cPosition2;
      int cSelect;
      int usiSwitch[8];
      int usiExtendSec;
      int _tmpVD[8];
      int usiActType;
      int usiActuateVDID;
      int usiActionSec[64];
      int usiNumVD;
      int ucActuateType_By_TOD;
      int valueSwitch[8];
      int valueActionSec[2];
      BYTE FlashModeBitmap[3840];
      BYTE SettingFlashTypeBitmap[3840];
      string sTmp[64];

      void LoadBitmapFromFile(void);
      void InitDispWord(void);

      void DisplayFlashSwitch(void);
      void DisplayFlashVDPhaseSwitchEdit(void);
      void DisplayFlashActiontSec(void);
      void DisplaySettingFlashType(void);
      void DisplayActuatePlanID(void);

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
      void DoKeyAWork(void);
      void DoKeyBWork(void);
      void DoKeyCWork(void);
      void DoKeyDWork(void);
      void DoKeyEWork(void);
      void DoKeyFWork(void);
      void DoKeyF1Work(void);
      void DoKeyF2Work(void);
      void DoKeyF3Work(void);
      void DoKeyF4Work(void);
      void DoKeyUPWork(void);
      void DoKeyDOWNWork(void);
      void DoKeyLEFTWork(void);
      void DoKeyRIGHTWork(void);
      void DoKeyEnterWork(void);

      void ReadVDdevFile(void);
      void ReadVDNumFile(void);
      void ReadActPlan(void);
      void SaveVDdevFile(void);

};
extern  ScreenFlashMode screenFlashMode;
#endif // SCREENFLASHMODE_H_INCLUDED
