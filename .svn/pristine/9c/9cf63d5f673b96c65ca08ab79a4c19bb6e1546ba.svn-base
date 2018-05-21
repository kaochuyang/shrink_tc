#ifndef screenReverseH
#define screenReverseH
//---------------------------------------------------------------------------
#include "SCREEN.h"
#include "CReverseTime.h"
//---------------------------------------------------------------------------
class ScreenReverse:public SCREEN
{
    public:
      ScreenReverse(void);
      ~ScreenReverse(void);

      void DisplayReverse(unsigned short int);
      void doKeyWork(BYTE);

    private:
      DISP_WORD timeWord1[4];
      DISP_WORD timeWord2[4];
      DISP_WORD timeWord3[4];
      DISP_WORD timeWord4[4];
      DISP_WORD noWord;
      DISP_WORD secWord1[3];
      DISP_WORD secWord2[3];
      DISP_WORD secWord3[3];

      int cPosition;
      int cSelect;
      int cFace;

      unsigned short int usiLoadRev;

      void initDispWord(void);
//      void DisplayDate(void);

      CReverseTimeInfo reverseInfo;
      //CCJ++
      CReverseTimeInfo tmpReverseTimeInfo;
      void ModifyRev(int tmp_value);
      //CCJ++
      void LoadRev(void);
      void DisplayRev(void);
      void SaveRev(void);
//      void DisplayItem(int);
//      void DisplayUpAndPage(int);

      BYTE reverseBitmap[3840];
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
      void doKeyDefaultWork(void);
};
//---------------------------------------------------------------------------
extern ScreenReverse screenReverse;
#endif
