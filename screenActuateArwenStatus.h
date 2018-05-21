#ifndef screenActuateArwenStatusH
#define screenActuateArwenStatusH
//---------------------------------------------------------------------------
#include "SCREEN.h"
//---------------------------------------------------------------------------
class ScreenActuateArwenStatus:public SCREEN
{
    public:
      ScreenActuateArwenStatus(void);
      ~ScreenActuateArwenStatus(void);

      void DisplayActuateArwenStatus(void);
      void doKeyWork(BYTE);                                                     //根據KEY的值作事

      void vRefreshActuateArwenStatusData(void);
      void vShowToPanel(void);
      void vRefreshStepSec(void);

      void vRefreshActSwitch(void);
      void vCalActTuningStatus(void);
      void vShowActTuningStatus(void);
      void vRefreshEverySec(void);

      void vSetFixedGreen(int,int,int);
      void vGetFixedGreen(int&,int&,int&);


    private:
      DISP_WORD markSubPhase[4];
      DISP_WORD markStep[4];
      DISP_WORD markActSwitch;
      DISP_WORD markStepSecCount[2];
      DISP_WORD markActTuning;
      DISP_WORD markOrgGreenTime[2];
      DISP_WORD markAdjGreenTime[2];
/*      DISP_WORD markArriveMinGreenTime;
      DISP_WORD markExtenTime[2];
      DISP_WORD markStopGreen[2]; */
      DISP_WORD markMinGreenTime[2];
      DISP_WORD markMaxGreenTime[3];

      unsigned short int usiStepSec;
      unsigned short int iCurrentPhaseID;
      unsigned short int iCurrentPlanID;
      int iCurrentSubphase;
      int iCurrentSubphaseTotal;
      int iCurrentStep;
      int iLastStep;
      int iCurrentStepTotal;
      int iAdjGreenSec;
      bool bEnableModAdjGreenSec;


      unsigned char ucTCControlStrategy;
      bool bActSwitch;
      bool bActTuningStatusNow;
      bool bActTuningStatusLast;
      short int siAdjMaxGreen;
      short int siAdjMinGreen;
      short int siCurrentPhaseGreen;

      void initDispWord(void);                                                  //初始化此畫面的座標等參數
      int cSelect;                                                              //現在選擇的選項

      BYTE ucActuateArwenBitmap[3840];
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

      void vSendCCTProtocol5F81ActArwen(void);

      int fixedCurrentGreen;
      int fixedMinGreen;
      int fixedMaxGreen;
      bool fixedsta;
      int checkiLastSubphase;

};
//---------------------------------------------------------------------------
extern ScreenActuateArwenStatus screenActuateArwenStatus;
#endif
