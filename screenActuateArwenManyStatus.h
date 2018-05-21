#ifndef screenActuateArwenManyStatusH
#define screenActuateArwenManyStatusH
//---------------------------------------------------------------------------
#include "SCREEN.h"
//---------------------------------------------------------------------------
class ScreenActuateArwenManyStatus:public SCREEN
{
    public:
      ScreenActuateArwenManyStatus(void);
      ~ScreenActuateArwenManyStatus(void);

      void DisplayActuateArwenManyStatus(void);
      void doKeyWork(BYTE);                                                     //�ھ�KEY���ȧ@��

      void vRefreshActuateArwenManyStatusData(void);
      void vShowToPanel(void);
      void vRefreshStepSec(void);

      void vRefreshActSwitch(void);
      void vCalActTuningStatus(void);
      void vShowActTuningStatus(void);
      void vRefreshEverySec(void);


    private:
      DISP_WORD markSubPhase[4];
      DISP_WORD markStep[4];
      DISP_WORD markActSwitch;
      DISP_WORD markStepSecCount[3];
      DISP_WORD markActTuning[4];
      DISP_WORD markOrgGreenTime[4][3];
      DISP_WORD markAdjGreenTime[4][3];
/*      DISP_WORD markArriveMinGreenTime;
      DISP_WORD markExtenTime[2];
      DISP_WORD markStopGreen[2]; */
      DISP_WORD markMinGreenTime[4][3];
      DISP_WORD markMaxGreenTime[4][3];

      DISP_WORD OnlySubPhase[4];
      DISP_WORD OnlyStep[4];


      unsigned short int ilastPhaseID[4];
      unsigned short int ilastPlanID[4];

      int ilastSubphase[4];
      int ilastStep[4];
      short int ilastPhaseGreen[4];
      int ilastGreenSec[4];
      bool ilastbActTuningStatus[4];
      short int ilastAdjMinGree[4];
      short int ilastAdjMaxGree[4];


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

      void initDispWord(void);                                                  //��l�Ʀ��e�����y�е��Ѽ�
      int cSelect;                                                              //�{�b��ܪ��ﶵ

      BYTE ucActuateArwenBitmap_open[3840];
      BYTE ucActuateArwenBitmap_close[3840];
      void loadBitmapFromFile(void);                                            //�N����Load�i�O����

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
      void vRefreshActuateTimes(void);
      void vDisplayActionChange(void);

      int switch_times;
      short int RecordPhaseGreen;
      int RecordGreenSec;
      int RecordSubphase;
      bool RecordActTuningStatus;
      short int RecordAdjMinGreen;
      short int RecordAdjMaxGreen;
      DISP_WORD mark2SubPhase;
      DISP_WORD mark2Step;
      bool showopen;
      bool showclose;
      bool last_show_open;




};
//---------------------------------------------------------------------------
extern ScreenActuateArwenManyStatus screenActuateArwenManyStatus;
#endif
