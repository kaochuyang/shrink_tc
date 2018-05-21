#ifndef screenLightPreviewH
#define screenLightPreviewH
//---------------------------------------------------------------------------
#include "SCREEN.h"
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
class ScreenLightPreview:public SCREEN
{
    public:
      ScreenLightPreview(void);
      ~ScreenLightPreview(void);

      void DisplayLightPreview(int, unsigned short int);
      void doKeyWork(BYTE);                                                     //根據KEY的值作事

    private:
      DISP_WORD Light[6][8];
      DISP_WORD PhaseID[2];
      DISP_WORD SubPhaseID[2];
      DISP_WORD StepID[2];
      DISP_WORD SignalCount;

      BYTE BLight[6][8];
      BYTE BPhaseID[2];
      BYTE BSubPhaseID;
      BYTE BStepID;

      unsigned short int usiLight[8][5][8];
      unsigned char ucPhaseID;
      unsigned char ucSignalMap;
      unsigned char ucSignalCount;
      unsigned char ucSubphaseCount;
      unsigned char ucStepCount[8];

      unsigned short int usiCurrentSubphaseNo;
      unsigned short int usiCurrentStepNo;

      unsigned short int usiFromWhereInLight;

      void vTmpArrayToData();
      int iPlanReturn;


      int face;                                                                 //頁數 1:左上 2:左中 3:左下 4:右上 5:右中 6:右下
      int cSelPosition;                                                         //OT Debug 0523
      int cPosition;                                                            //OT Debug 0523
      void LoadLight(int);                                                       //從學勇那裡得到
      void SavePlan(void);                                                      //寫回學勇那裡

      void initDispWord(void);                                                  //初始化此畫面的座標等參數

      BYTE lightPreviewBitmap[3840];                                                 //底圖
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
      void doKeyDefaultWork(void);

      void DisplayLight(void);
};
//---------------------------------------------------------------------------
extern ScreenLightPreview screenLightPreview;
#endif


