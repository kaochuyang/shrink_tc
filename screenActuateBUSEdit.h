#ifndef screenActuateBUSEditH
#define screenActuateBUSEditH

//---------------------------------------------------------------------------
#include "SCREEN.h"
//---------------------------------------------------------------------------
class ScreenActuateBUSEdit:public SCREEN
{
    public:
      ScreenActuateBUSEdit(void);
      ~ScreenActuateBUSEdit(void);

      void DisplayActuateBUSEdit(void);
      void doKeyWork(BYTE);                                                     //®Ú¾ÚKEYªº­È§@¨Æ

    private:
//      DISP_WORD planEdit1[2];                                                     //¦¹­¶ªºªÅ¥Õ³B
      unsigned int cPosition;                                                   //²{¦bªº¦ì¸m
      int cSelect;                                                              //¤W¤U¤Á´«   0:¤é´Á 1:®É¶¡
//      BYTE planTemp[2];

      DISP_WORD cActuateSwitch;
      bool bTMP_ActuateSwitch;
      DISP_WORD cActuatePlan[2];
      unsigned char ucTMP_ActuatePlan[2];
      DISP_WORD cActuateautoswitch;
      bool bTMP_Actuateautoswitch;
      unsigned short int usiActuatePlan;
      //bool bActuateautoswitch;

      DISP_WORD cActuatePhaseID;
      unsigned char ucActuatePhaseID;

      unsigned char ucActuateType_By_TOD;
      DISP_WORD cActuateType_By_TOD;

      void initDispWord(void);                                                  //ªì©l¤Æ¦¹µe­±ªº®y¼Ðµ¥°Ñ¼Æ
      void DisplaySetSegtype(int,int);

      BYTE actuateBUSEditBitmap[3840];
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

      void vDisplayActuateSwitch(void);
      void vDisplayActuatesubphase(void);
      void vDisplayActuateExtendPhaseID(void);
      void vDisplayActuateautoswitch(void);
      void vDisplayActuateByTOD(void);

      void vDataToTmpArray();
      void vTmpArrayToData();

      bool vCheckRationality();
      bool vInitUCTMPData(void);

};
//---------------------------------------------------------------------------
extern ScreenActuateBUSEdit screenActuateBUSEdit;
#endif

