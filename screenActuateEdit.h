#ifndef screenActuateEditH
#define screenActuateEditH

//---------------------------------------------------------------------------
#include "SCREEN.h"
//---------------------------------------------------------------------------
class ScreenActuateEdit:public SCREEN
{
    public:
      ScreenActuateEdit(void);
      ~ScreenActuateEdit(void);

      void DisplayActuateEdit(void);
      void doKeyWork(BYTE);                                                     //根據KEY的值作事

    private:
//      DISP_WORD planEdit1[2];                                                     //此頁的空白處
      unsigned int cPosition;                                                   //現在的位置
      int cSelect;                                                              //上下切換   0:日期 1:時間
//      BYTE planTemp[2];

      DISP_WORD cActuateSwitch;
      bool bTMP_ActuateSwitch;
      DISP_WORD cActuatePlan[2];
      DISP_WORD cActuateVDID[5];
      unsigned char ucTMP_ActuatePlan[2];
      unsigned char ucTMP_ActuateVDID[5];
      unsigned short int usiActuatePlan;
      unsigned short int usiActuateVDID;

      DISP_WORD cActuatePhaseID;
      unsigned char ucActuatePhaseID;

      unsigned char ucActuateType_By_TOD;
      DISP_WORD cActuateType_By_TOD;

      void initDispWord(void);                                                  //初始化此畫面的座標等參數
      void DisplaySetSegtype(int,int);

      BYTE actuateEditBitmap[3840];
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

      void vDisplayActuateSwitch(void);
      void vDisplayActuatePlanID(void);
      void vDisplayActuateExtendSecPhaseID(void);
      void vDisplayActuateVDID(void);
      void vDisplayActuateByTOD(void);

      void vDataToTmpArray();
      void vTmpArrayToData();

      bool vCheckRationality();
      bool vInitUCTMPData(void);

};
//---------------------------------------------------------------------------
extern ScreenActuateEdit screenActuateEdit;
#endif
