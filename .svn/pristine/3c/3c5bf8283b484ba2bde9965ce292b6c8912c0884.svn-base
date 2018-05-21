#ifndef screenPhasePreviewH
#define screenPhasePreviewH
//---------------------------------------------------------------------------
#include "SCREEN.h"
#include "CSegmentInfo.h"
//---------------------------------------------------------------------------
class ScreenPhasePreview:public SCREEN
{
    public:
      ScreenPhasePreview(void);
      ~ScreenPhasePreview(void);

      bool vSetPreviewOrView(unsigned short int);
      void DisplayPhasePreview(int, unsigned char, bool);
      void doKeyWork(BYTE);                                                     //�ھ�KEY���ȧ@��

    private:

      unsigned short int usiPreviewOrView;
      bool  vClearSpace(void);

      unsigned short int usiHTY;                                                //Hsinchu Tainan Yulin Select
      int iPlan;
      unsigned char ucPhaseSel;
      unsigned char ucPhaseOri;
      int cPosition;                                                            //
      int cSelect;                                                              //0:���\uFFFD 1:�S�O�餺�e
      int cFace;

      unsigned short int usiPhaseOrderArray[256];

      void DisplayPhasePreviewRefresh(void);
      bool vFindPhase(void);

      void vTmpArrayToData(void);
      bool vCheck(void);
      void vSaveSpecialDay(void);
      bool vSetPlanTable(void);

      void initDispWord(void);                                                  //��l�Ʀ��e�����y�е��Ѽ�

      BYTE phaseSelBitmap[3840];                                              //����
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
      void doKeyDefaultWork(void);
};
//---------------------------------------------------------------------------
extern ScreenPhasePreview screenPhasePreview;
#endif
