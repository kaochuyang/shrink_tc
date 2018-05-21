#ifndef SCREENVDACTPHASEEDIT_H_INCLUDED
#define SCREENVDACTPHASEEDIT_H_INCLUDED
#include "SCREEN.h"
class ScreenVDActPhaseEdit:public SCREEN
{
	public:
	ScreenVDActPhaseEdit();
	~ScreenVDActPhaseEdit();
	void DisplayVDActPhaseEdit( );
      void DoKeyWork(BYTE);

    private:

      DISP_WORD positionSwitch[8];
      DISP_WORD positionExtendSec[8];
      DISP_WORD fixedGreenMode;

      unsigned short int valueSwitch[8];
      unsigned short int valExtendSec[8];
      unsigned short int valuefixedGreenMode;
      DISP_WORD makrphaseId[2];
      unsigned int cPosition;
      int cSelect;
      int usiSwitch[8];
      int usiExtendSec[8];
      int usifixedGreenMode;
      int _tmpVD[8];
      int usiActType;

      BYTE VDPhaseEditBitmap[3840];
      BYTE PhaseGreenModeBitmap[3840];

      void LoadBitmapFromFile(void);
      void InitDispWord(void);

      void DisplayVDPhaseSwitchEdit(void);
      void DisplayExtendValue(void);
      void DisplayfixedGree(void);
      void DisplayActuatePhaseGreenMode(void);

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
      void SaveVDdevFile(void);

};
extern  ScreenVDActPhaseEdit screenVDActPhaseEdit;
#endif // SCREENVDACTPHASEEEDIT_H_INCLUDED
