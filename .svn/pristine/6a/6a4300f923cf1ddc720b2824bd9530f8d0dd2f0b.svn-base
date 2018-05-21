#include "screenLightPreview.h"
#include "screenSegtype.h"
#include "screenSegtypeEditF2.h"
#include "screenRunTableList.h"
#include "SCREENMain.h"
#include "screenWeekDay.h"
#include "LCD240x128.h"
#include "CSTC.h"
//#include "screenLightPreviewEditStep1.h"

#include "screenPlan.h"
#include "screenLightEdit.h"

#include <stdio.h>

#include "SMEM.h"
//---------------------------------------------------------------------------
ScreenLightPreview screenLightPreview;
//---------------------------------------------------------------------------
ScreenLightPreview::ScreenLightPreview(void)
{
    loadBitmapFromFile();
    initDispWord();
    face=1;
    usiCurrentSubphaseNo = 0;
    usiCurrentStepNo = 0;
}
//---------------------------------------------------------------------------
ScreenLightPreview::~ScreenLightPreview(void)
{
}
//---------------------------------------------------------------------------
void ScreenLightPreview::loadBitmapFromFile(void)
{
try {
    FILE *bitmap;
    bitmap=fopen("//cct//bitmap//backGround//LightView.bit","rb");
    if (bitmap) {
        fread(lightPreviewBitmap,3840,1,bitmap);
        fclose(bitmap);
    }

  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenLightPreview::DisplayLightPreview(int phaseID, unsigned short int usiPlanID)
{
try {
    smem.SetcFace(cLIGHTPREVIEW);

    if(usiPlanID != INVAILDVALUE)
      usiFromWhereInLight = usiPlanID;

    face=1;   //左上
    cSelPosition = 0;
    cPosition = 0;

    usiCurrentSubphaseNo = 0;
    usiCurrentStepNo = 0;
    lcd240x128.DISPLAY_GRAPHIC(0,lightPreviewBitmap,128,30);
    LoadLight(phaseID);
    DisplayLight();

    setCursor16x16(PhaseID[cPosition].X,PhaseID[cPosition].Y+16);

  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenLightPreview::initDispWord(void)
{
    for(int i = 0; i < 6; i++) {
      for(int j = 0; j < 8; j++) {
        Light[i][j].X = 8 + 16*i;
        Light[i][j].Y = j*16;
        Light[i][j].width = 8;
        Light[i][j].height = 16;
      }
    }

    for(int i = 0; i < 2; i++) {
      PhaseID[i].X = 184 + 8*i;
      PhaseID[i].Y = 16;
      PhaseID[i].width = 8;
      PhaseID[i].height = 16;

      SubPhaseID[i].X = 148 + 16*i;
      SubPhaseID[i].Y = 32;
      SubPhaseID[i].width = 8;
      SubPhaseID[i].height = 16;

      StepID[i].X = 148 + 16*i;
      StepID[i].Y = 48;
      StepID[i].width = 8;
      StepID[i].height = 16;

    }

    SignalCount.X = 160;
    SignalCount.Y = 64;
    SignalCount.width = 8;
    SignalCount.height = 16;


}
//---------------------------------------------------------------------------
void ScreenLightPreview::LoadLight(int PhaseID)
{
try {
  stc.Lock_to_Load_Phase_for_Panel(PhaseID);
  for(int i = 0; i < 8; i++) {                                                  //init
    for(int j = 0; j < 5; j++)
      for(int k = 0; k < 8; k++)
        usiLight[i][j][k] = 0;
  }
  for(int i = 0; i < 8; i++) {
      ucStepCount[i] = 0;
  }

  ucPhaseID = stc._panel_phase._phase_order;
  ucSignalMap = stc._panel_phase._signal_map;
  ucSignalCount = stc._panel_phase._signal_count;
  ucSubphaseCount = stc._panel_phase._subphase_count;
  for(int i = 0; i < ucSubphaseCount; i++) {
    ucStepCount[i] = stc._panel_phase._ptr_subphase_step_count[i];
  }


  for(int i = 0; i < ucSubphaseCount; i++) {
    for(int j = 0; j < ucStepCount[i]; j++) {
      for(int k = 0; k < ucSignalCount; k++) {
        usiLight[i][j][k] = stc._panel_phase._ptr_subphase_step_signal_status[i][j][k];
      }
    }
  }


  /*
    if (stc.Lock_to_Load_Plan_for_Panel(plan)) {
        planInfo._planid=stc._panel_plan._planid;
        planInfo._dir=stc._panel_plan._dir;
        planInfo._phase_order=stc._panel_plan._phase_order;
        planInfo._subphase_count=stc._panel_plan._subphase_count;
        planInfo._cycle_time=stc._panel_plan._cycle_time;
        planInfo._offset=stc._panel_plan._offset;
        for (int i=0;i<8;i++) {                              //先清空
             planInfo._ptr_subplaninfo[i]._green=0;
             planInfo._ptr_subplaninfo[i]._min_green=0;
             planInfo._ptr_subplaninfo[i]._max_green=0;
             planInfo._ptr_subplaninfo[i]._yellow=0;
             planInfo._ptr_subplaninfo[i]._allred=0;
             planInfo._ptr_subplaninfo[i]._pedgreen_flash=0;
             planInfo._ptr_subplaninfo[i]._pedred=0;
        }
        for (int i=0;i<planInfo._subphase_count;i++) {       //再重寫
             planInfo._ptr_subplaninfo[i]._green=stc._panel_plan._ptr_subplaninfo[i]._green;
             planInfo._ptr_subplaninfo[i]._min_green=stc._panel_plan._ptr_subplaninfo[i]._min_green;
             planInfo._ptr_subplaninfo[i]._max_green=stc._panel_plan._ptr_subplaninfo[i]._max_green;
             planInfo._ptr_subplaninfo[i]._yellow=stc._panel_plan._ptr_subplaninfo[i]._yellow;
             planInfo._ptr_subplaninfo[i]._allred=stc._panel_plan._ptr_subplaninfo[i]._allred;
             planInfo._ptr_subplaninfo[i]._pedgreen_flash=stc._panel_plan._ptr_subplaninfo[i]._pedgreen_flash;
             planInfo._ptr_subplaninfo[i]._pedred=stc._panel_plan._ptr_subplaninfo[i]._pedred;
        }
    } else {
        planInfo._planid=plan;
        planInfo._dir=0;
        planInfo._phase_order=0;
        planInfo._subphase_count=8;
        planInfo._cycle_time=0;
        planInfo._offset=0;
        for (int i=0;i<8;i++) {
             planInfo._ptr_subplaninfo[i]._green=0;
             planInfo._ptr_subplaninfo[i]._min_green=0;
             planInfo._ptr_subplaninfo[i]._max_green=0;
             planInfo._ptr_subplaninfo[i]._yellow=0;
             planInfo._ptr_subplaninfo[i]._allred=0;
             planInfo._ptr_subplaninfo[i]._pedgreen_flash=0;
             planInfo._ptr_subplaninfo[i]._pedred=0;
        }
    }
*/
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenLightPreview::DisplayLight(void)
{
try {

  unsigned short int usiTMP;
  int iSignalStatus[16];

  for(int i = 0; i < ucSignalCount; i++) {
    usiTMP = usiLight[usiCurrentSubphaseNo][usiCurrentStepNo][i];

    for(int j = 0; j < 16; j++) {
      iSignalStatus[j] = ((usiTMP&0x0001)==0) ? 0 : 1;                          // 0x80 => 10000000 (1byte)
      usiTMP = usiTMP >> 1;

      if(j % 2 == 1) {
        if(iSignalStatus[j-1] == 1 && iSignalStatus[j] == 1) {                  //shining
          lcd240x128.DISPLAY_GRAPHIC_XY(Light[i][j/2].X,Light[i][j/2].Y,word8x16Light,Light[i][j/2].height,Light[i][j/2].width/8);
        } else if(iSignalStatus[j-1] == 0 && iSignalStatus[j] == 0) {          //black
          lcd240x128.DISPLAY_GRAPHIC_XY(Light[i][j/2].X,Light[i][j/2].Y,word8x16Die,Light[i][j/2].height,Light[i][j/2].width/8);
        } else {
          lcd240x128.DISPLAY_GRAPHIC_XY(Light[i][j/2].X,Light[i][j/2].Y,word8x16Flash,Light[i][j/2].height,Light[i][j/2].width/8);
        }
      }
    }

  }

  //Phase
  lcd240x128.DISPLAY_GRAPHIC_XY(PhaseID[0].X,PhaseID[0].Y,word8x16[(ucPhaseID>>4)&0xFF],PhaseID[0].height,PhaseID[0].width/8);
  lcd240x128.DISPLAY_GRAPHIC_XY(PhaseID[1].X,PhaseID[1].Y,word8x16[(ucPhaseID)&0x0F],PhaseID[1].height,PhaseID[1].width/8);

  //SubPhase
  lcd240x128.DISPLAY_GRAPHIC_XY(SubPhaseID[0].X,SubPhaseID[0].Y,word8x16[usiCurrentSubphaseNo+1],SubPhaseID[0].height,SubPhaseID[0].width/8);
  lcd240x128.DISPLAY_GRAPHIC_XY(SubPhaseID[1].X,SubPhaseID[1].Y,word8x16[ucSubphaseCount],SubPhaseID[1].height,SubPhaseID[1].width/8);

  //Step
  lcd240x128.DISPLAY_GRAPHIC_XY(StepID[0].X,StepID[0].Y,word8x16[usiCurrentStepNo+1],StepID[0].height,StepID[0].width/8);
  lcd240x128.DISPLAY_GRAPHIC_XY(StepID[1].X,StepID[1].Y,word8x16[ucStepCount[usiCurrentSubphaseNo]],StepID[1].height,StepID[1].width/8);

  //SignalCount
  lcd240x128.DISPLAY_GRAPHIC_XY(SignalCount.X,SignalCount.Y,word8x16[ucSignalCount],SignalCount.height,SignalCount.width/8);


  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenLightPreview::SavePlan(void)
{
}

//---------------------------------------------------------------------------
void ScreenLightPreview::doKeyWork(BYTE key)
{
try {
    switch (key) {
        case 0x80:
          doKey0Work();
        break;
        case 0x81:
          doKey1Work();
        break;
        case 0x82:
          doKey2Work();
        break;
        case 0x83:
          doKey3Work();
        break;
        case 0x84:
          doKey4Work();
        break;
        case 0x85:
          doKey5Work();
        break;
        case 0x86:
          doKey6Work();
        break;
        case 0x87:
          doKey7Work();
        break;
        case 0x88:
          doKey8Work();
        break;
        case 0x89:
          doKey9Work();
        break;
        case 0x8A:
          doKeyAWork();
        break;
        case 0x8B:
          doKeyBWork();
        break;
        case 0x8C:
          doKeyCWork();
        break;
        case 0x8D:
          doKeyDWork();
        break;
        case 0x8E:
          doKeyEWork();
        break;
        case 0x8F:
          doKeyFWork();
        break;
        case 0x90:
          doKeyF1Work();
        break;
        case 0x91:
          doKeyF2Work();
        break;
        case 0x92:
          doKeyF3Work();
        break;
        case 0x93:
          doKeyF4Work();
        break;
        case 0x94:
          doKeyUPWork();
        break;
        case 0x95:
          doKeyDOWNWork();
        break;
        case 0x96:
          doKeyLEFTWork();
        break;
        case 0x97:
          doKeyRIGHTWork();
        break;
        case 0x98:
          doKeyEnterWork();
        break;
        default:
        break;
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenLightPreview::doKey0Work(void)
{
//  if(cSelPosition == 1) { planTableTemp[cPosition] =   0; lcd240x128.DISPLAY_GRAPHIC_XY(planTable[cPosition].X,planTable[cPosition].Y,word8x16[0],planTable[cPosition].height,planTable[cPosition].width/8); doKeyRIGHTWork(); }
//  else if(cSelPosition == 2) { OffsetTemp[cPosition] = 0; lcd240x128.DISPLAY_GRAPHIC_XY(cycleAndOffset[cPosition].X,cycleAndOffset[cPosition].Y,word8x16[0],cycleAndOffset[cPosition].height,cycleAndOffset[cPosition].width/8); doKeyRIGHTWork(); }
}
//---------------------------------------------------------------------------
void ScreenLightPreview::doKey1Work(void)
{
//  if(cSelPosition == 1) { planTableTemp[cPosition] =   1; lcd240x128.DISPLAY_GRAPHIC_XY(planTable[cPosition].X,planTable[cPosition].Y,word8x16[1],planTable[cPosition].height,planTable[cPosition].width/8); doKeyRIGHTWork(); }
//  else if(cSelPosition == 2) { OffsetTemp[cPosition] = 1; lcd240x128.DISPLAY_GRAPHIC_XY(cycleAndOffset[cPosition].X,cycleAndOffset[cPosition].Y,word8x16[1],cycleAndOffset[cPosition].height,cycleAndOffset[cPosition].width/8); doKeyRIGHTWork(); }
}
//---------------------------------------------------------------------------
void ScreenLightPreview::doKey2Work(void)
{
//  if(cSelPosition == 1) { planTableTemp[cPosition] =   2; lcd240x128.DISPLAY_GRAPHIC_XY(planTable[cPosition].X,planTable[cPosition].Y,word8x16[2],planTable[cPosition].height,planTable[cPosition].width/8); doKeyRIGHTWork(); }
//  else if(cSelPosition == 2) { OffsetTemp[cPosition] = 2; lcd240x128.DISPLAY_GRAPHIC_XY(cycleAndOffset[cPosition].X,cycleAndOffset[cPosition].Y,word8x16[2],cycleAndOffset[cPosition].height,cycleAndOffset[cPosition].width/8); doKeyRIGHTWork(); }
}
//---------------------------------------------------------------------------
void ScreenLightPreview::doKey3Work(void)
{
//  if(cSelPosition == 1) { planTableTemp[cPosition] =   3; lcd240x128.DISPLAY_GRAPHIC_XY(planTable[cPosition].X,planTable[cPosition].Y,word8x16[3],planTable[cPosition].height,planTable[cPosition].width/8); doKeyRIGHTWork(); }
//  else if(cSelPosition == 2) { OffsetTemp[cPosition] = 3; lcd240x128.DISPLAY_GRAPHIC_XY(cycleAndOffset[cPosition].X,cycleAndOffset[cPosition].Y,word8x16[3],cycleAndOffset[cPosition].height,cycleAndOffset[cPosition].width/8); doKeyRIGHTWork(); }
}
//---------------------------------------------------------------------------
void ScreenLightPreview::doKey4Work(void)
{
//  if(cSelPosition == 1) { planTableTemp[cPosition] =   4; lcd240x128.DISPLAY_GRAPHIC_XY(planTable[cPosition].X,planTable[cPosition].Y,word8x16[4],planTable[cPosition].height,planTable[cPosition].width/8); doKeyRIGHTWork(); }
//  else if(cSelPosition == 2) { OffsetTemp[cPosition] = 4; lcd240x128.DISPLAY_GRAPHIC_XY(cycleAndOffset[cPosition].X,cycleAndOffset[cPosition].Y,word8x16[4],cycleAndOffset[cPosition].height,cycleAndOffset[cPosition].width/8); doKeyRIGHTWork(); }
}
//---------------------------------------------------------------------------
void ScreenLightPreview::doKey5Work(void)
{
//  if(cSelPosition == 1) { planTableTemp[cPosition] =   5; lcd240x128.DISPLAY_GRAPHIC_XY(planTable[cPosition].X,planTable[cPosition].Y,word8x16[5],planTable[cPosition].height,planTable[cPosition].width/8); doKeyRIGHTWork(); }
//  else if(cSelPosition == 2) { OffsetTemp[cPosition] = 5; lcd240x128.DISPLAY_GRAPHIC_XY(cycleAndOffset[cPosition].X,cycleAndOffset[cPosition].Y,word8x16[5],cycleAndOffset[cPosition].height,cycleAndOffset[cPosition].width/8); doKeyRIGHTWork(); }
}
//---------------------------------------------------------------------------
void ScreenLightPreview::doKey6Work(void)
{
//  if(cSelPosition == 1) { planTableTemp[cPosition] =   6; lcd240x128.DISPLAY_GRAPHIC_XY(planTable[cPosition].X,planTable[cPosition].Y,word8x16[6],planTable[cPosition].height,planTable[cPosition].width/8); doKeyRIGHTWork(); }
//  else if(cSelPosition == 2) { OffsetTemp[cPosition] = 6; lcd240x128.DISPLAY_GRAPHIC_XY(cycleAndOffset[cPosition].X,cycleAndOffset[cPosition].Y,word8x16[6],cycleAndOffset[cPosition].height,cycleAndOffset[cPosition].width/8); doKeyRIGHTWork(); }
}
//---------------------------------------------------------------------------
void ScreenLightPreview::doKey7Work(void)
{
//  if(cSelPosition == 1) { planTableTemp[cPosition] =   7; lcd240x128.DISPLAY_GRAPHIC_XY(planTable[cPosition].X,planTable[cPosition].Y,word8x16[7],planTable[cPosition].height,planTable[cPosition].width/8); doKeyRIGHTWork(); }
//  else if(cSelPosition == 2) { OffsetTemp[cPosition] = 7; lcd240x128.DISPLAY_GRAPHIC_XY(cycleAndOffset[cPosition].X,cycleAndOffset[cPosition].Y,word8x16[7],cycleAndOffset[cPosition].height,cycleAndOffset[cPosition].width/8); doKeyRIGHTWork(); }
}
//---------------------------------------------------------------------------
void ScreenLightPreview::doKey8Work(void)
{
//  if(cSelPosition == 1) { planTableTemp[cPosition] =   8; lcd240x128.DISPLAY_GRAPHIC_XY(planTable[cPosition].X,planTable[cPosition].Y,word8x16[8],planTable[cPosition].height,planTable[cPosition].width/8); doKeyRIGHTWork(); }
//  else if(cSelPosition == 2) { OffsetTemp[cPosition] = 8; lcd240x128.DISPLAY_GRAPHIC_XY(cycleAndOffset[cPosition].X,cycleAndOffset[cPosition].Y,word8x16[8],cycleAndOffset[cPosition].height,cycleAndOffset[cPosition].width/8); doKeyRIGHTWork(); }
}
//---------------------------------------------------------------------------
void ScreenLightPreview::doKey9Work(void)
{
//  if(cSelPosition == 1) { planTableTemp[cPosition] =   9; lcd240x128.DISPLAY_GRAPHIC_XY(planTable[cPosition].X,planTable[cPosition].Y,word8x16[9],planTable[cPosition].height,planTable[cPosition].width/8); doKeyRIGHTWork(); }
//  else if(cSelPosition == 2) { OffsetTemp[cPosition] = 9; lcd240x128.DISPLAY_GRAPHIC_XY(cycleAndOffset[cPosition].X,cycleAndOffset[cPosition].Y,word8x16[9],cycleAndOffset[cPosition].height,cycleAndOffset[cPosition].width/8); doKeyRIGHTWork(); }
}
//---------------------------------------------------------------------------
void ScreenLightPreview::doKeyAWork(void)
{

}
//---------------------------------------------------------------------------
void ScreenLightPreview::doKeyBWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenLightPreview::doKeyCWork(void)
{
  bool bPhaseStatus;
  int iQueryPhaseOrder;
  iQueryPhaseOrder = ucPhaseID;

  while(1) {
    if(iQueryPhaseOrder != 0) iQueryPhaseOrder -= 1;
    else iQueryPhaseOrder = 0xFF;

    bPhaseStatus = smem.vGetTCPhasePlanSegTypeData(TC_Phase, iQueryPhaseOrder);
    if(bPhaseStatus) { break; }
  }

  usiCurrentSubphaseNo = 0;
  usiCurrentStepNo = 0;
  lcd240x128.DISPLAY_GRAPHIC(0,lightPreviewBitmap,128,30);
  LoadLight(iQueryPhaseOrder);
  DisplayLight();
}
//---------------------------------------------------------------------------
void ScreenLightPreview::doKeyDWork(void)
{
  bool bPhaseStatus;
  int iQueryPhaseOrder;
  iQueryPhaseOrder = ucPhaseID;

  while(1) {
    if(iQueryPhaseOrder != 0xFF) iQueryPhaseOrder += 1;
    else iQueryPhaseOrder = 0;

    bPhaseStatus = smem.vGetTCPhasePlanSegTypeData(TC_Phase, iQueryPhaseOrder);
    if(bPhaseStatus) { break; }
  }

  usiCurrentSubphaseNo = 0;
  usiCurrentStepNo = 0;
  lcd240x128.DISPLAY_GRAPHIC(0,lightPreviewBitmap,128,30);
  LoadLight(iQueryPhaseOrder);
  DisplayLight();

}
//---------------------------------------------------------------------------
void ScreenLightPreview::doKeyEWork(void)
{
  if(usiCurrentStepNo > 0) {
    usiCurrentStepNo--;
  } else if(usiCurrentSubphaseNo > 0 ) {
    usiCurrentSubphaseNo--;
    usiCurrentStepNo = ucStepCount[usiCurrentSubphaseNo]-1;
  }
  DisplayLight();
}
//---------------------------------------------------------------------------
void ScreenLightPreview::doKeyFWork(void)
{
//  unsigned int uiTmp;

  if(usiCurrentStepNo < ucStepCount[usiCurrentSubphaseNo]-1) {
    usiCurrentStepNo++;
  } else if(usiCurrentSubphaseNo < ucSubphaseCount - 1 ) {
    usiCurrentSubphaseNo++;
    usiCurrentStepNo = 0;
  }

  DisplayLight();
}

//---------------------------------------------------------------------------
void ScreenLightPreview::doKeyF1Work(void)
{
    screenMain.DisplayMain();
}
//---------------------------------------------------------------------------
void ScreenLightPreview::doKeyF2Work(void)
{
  screenLightEdit.DisplayLightEdit(ucPhaseID, ucPhaseID);
}
//---------------------------------------------------------------------------
void ScreenLightPreview::doKeyF3Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenLightPreview::doKeyF4Work(void)
{
//    screenSegtypeEditF2.vDisplaySegtypeFromPreview(INVAILDVALUE);
    screenPlan.DisplayPlan(usiFromWhereInLight, INVAILDVALUE);
}
//---------------------------------------------------------------------------
void ScreenLightPreview::doKeyUPWork(void)
{
try {

  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenLightPreview::doKeyDOWNWork(void)
{
try {
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenLightPreview::doKeyLEFTWork(void)
{
try {

  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenLightPreview::doKeyRIGHTWork(void)
{
try {
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenLightPreview::doKeyEnterWork(void)
{
//    screenSegtypeEditF2.vDisplaySegtypeFromPreview(iPlanReturn);
}
//---------------------------------------------------------------------------
void ScreenLightPreview::vTmpArrayToData(void)
{
try{
/*
  if(face == 1) {
    for(int i = 0; i < 4; i++) {
      if(i < planInfo._subphase_count) {
        planInfo._ptr_subplaninfo[i]._green = 0;
        planInfo._ptr_subplaninfo[i]._green = planTableTemp[0+i*4] * 1000;
        planInfo._ptr_subplaninfo[i]._green += (planTableTemp[1+i*4] * 100);
        planInfo._ptr_subplaninfo[i]._green += (planTableTemp[2+i*4] * 10);
        planInfo._ptr_subplaninfo[i]._green += planTableTemp[3+i*4];

        planInfo._ptr_subplaninfo[i]._allred = 0;
        planInfo._ptr_subplaninfo[i]._allred = planTableTemp[16+i*4] * 1000;
        planInfo._ptr_subplaninfo[i]._allred += (planTableTemp[17+i*4] * 100);
        planInfo._ptr_subplaninfo[i]._allred += (planTableTemp[18+i*4] * 10);
        planInfo._ptr_subplaninfo[i]._allred += planTableTemp[19+i*4];

        planInfo._ptr_subplaninfo[i]._yellow = 0;
        planInfo._ptr_subplaninfo[i]._yellow = planTableTemp[32+i*4] * 1000;
        planInfo._ptr_subplaninfo[i]._yellow += (planTableTemp[33+i*4] * 100);
        planInfo._ptr_subplaninfo[i]._yellow += (planTableTemp[34+i*4] * 10);
        planInfo._ptr_subplaninfo[i]._yellow += planTableTemp[35+i*4];
      }
    }
  }
  else if(face == 4) {
    for(int i = 0; i < 4; i++) {
      if(i+4 < planInfo._subphase_count) {
        planInfo._ptr_subplaninfo[4+i]._green = 0;
        planInfo._ptr_subplaninfo[4+i]._green = planTableTemp[0+i*4] * 1000;
        planInfo._ptr_subplaninfo[4+i]._green += (planTableTemp[1+i*4] * 100);
        planInfo._ptr_subplaninfo[4+i]._green += (planTableTemp[2+i*4] * 10);
        planInfo._ptr_subplaninfo[4+i]._green += planTableTemp[3+i*4];

        planInfo._ptr_subplaninfo[4+i]._allred = 0;
        planInfo._ptr_subplaninfo[4+i]._allred = planTableTemp[16+i*4] * 1000;
        planInfo._ptr_subplaninfo[4+i]._allred += (planTableTemp[17+i*4] * 100);
        planInfo._ptr_subplaninfo[4+i]._allred += (planTableTemp[18+i*4] * 10);
        planInfo._ptr_subplaninfo[4+i]._allred += planTableTemp[19+i*4];

        planInfo._ptr_subplaninfo[4+i]._yellow = 0;
        planInfo._ptr_subplaninfo[4+i]._yellow = planTableTemp[32+i*4] * 1000;
        planInfo._ptr_subplaninfo[4+i]._yellow += (planTableTemp[33+i*4] * 100);
        planInfo._ptr_subplaninfo[4+i]._yellow += (planTableTemp[34+i*4] * 10);
        planInfo._ptr_subplaninfo[4+i]._yellow += planTableTemp[35+i*4];
      }
    }
  }
  else if(face == 2) {
    for(int i = 0; i < 4; i++) {
      if(i < planInfo._subphase_count) {
        planInfo._ptr_subplaninfo[i]._pedgreen_flash = 0;
        planInfo._ptr_subplaninfo[i]._pedgreen_flash = planTableTemp[0+i*4] * 1000;
        planInfo._ptr_subplaninfo[i]._pedgreen_flash += (planTableTemp[1+i*4] * 100);
        planInfo._ptr_subplaninfo[i]._pedgreen_flash += (planTableTemp[2+i*4] * 10);
        planInfo._ptr_subplaninfo[i]._pedgreen_flash += planTableTemp[3+i*4];

        planInfo._ptr_subplaninfo[i]._pedred = 0;
        planInfo._ptr_subplaninfo[i]._pedred = planTableTemp[16+i*4] * 1000;
        planInfo._ptr_subplaninfo[i]._pedred += (planTableTemp[17+i*4] * 100);
        planInfo._ptr_subplaninfo[i]._pedred += (planTableTemp[18+i*4] * 10);
        planInfo._ptr_subplaninfo[i]._pedred += planTableTemp[19+i*4];
      }
    }
  }
  else if(face == 5) {
    for(int i = 0; i < 4; i++) {
      if(i+4 < planInfo._subphase_count) {
        planInfo._ptr_subplaninfo[4+i]._pedgreen_flash = 0;
        planInfo._ptr_subplaninfo[4+i]._pedgreen_flash = planTableTemp[0+i*4] * 1000;
        planInfo._ptr_subplaninfo[4+i]._pedgreen_flash += (planTableTemp[1+i*4] * 100);
        planInfo._ptr_subplaninfo[4+i]._pedgreen_flash += (planTableTemp[2+i*4] * 10);
        planInfo._ptr_subplaninfo[4+i]._pedgreen_flash += planTableTemp[3+i*4];

        planInfo._ptr_subplaninfo[4+i]._pedred = 0;
        planInfo._ptr_subplaninfo[4+i]._pedred = planTableTemp[16+i*4] * 1000;
        planInfo._ptr_subplaninfo[4+i]._pedred += (planTableTemp[17+i*4] * 100);
        planInfo._ptr_subplaninfo[4+i]._pedred += (planTableTemp[18+i*4] * 10);
        planInfo._ptr_subplaninfo[4+i]._pedred += planTableTemp[19+i*4];
      }
    }
  }
  else if(face == 3) {
    for(int i = 0; i < 4; i++) {
      if(i < planInfo._subphase_count) {
        planInfo._ptr_subplaninfo[i]._min_green = 0;
        planInfo._ptr_subplaninfo[i]._min_green = planTableTemp[0+i*4] * 1000;
        planInfo._ptr_subplaninfo[i]._min_green += (planTableTemp[1+i*4] * 100);
        planInfo._ptr_subplaninfo[i]._min_green += (planTableTemp[2+i*4] * 10);
        planInfo._ptr_subplaninfo[i]._min_green += planTableTemp[3+i*4];

        planInfo._ptr_subplaninfo[i]._max_green = 0;
        planInfo._ptr_subplaninfo[i]._max_green = planTableTemp[16+i*4] * 1000;
        planInfo._ptr_subplaninfo[i]._max_green += (planTableTemp[17+i*4] * 100);
        planInfo._ptr_subplaninfo[i]._max_green += (planTableTemp[18+i*4] * 10);
        planInfo._ptr_subplaninfo[i]._max_green += planTableTemp[19+i*4];
      }
    }
  }
  else if(face == 6) {
    for(int i = 0; i < 4; i++) {
      if(i+4 < planInfo._subphase_count) {
        planInfo._ptr_subplaninfo[4+i]._min_green = 0;
        planInfo._ptr_subplaninfo[4+i]._min_green = planTableTemp[0+i*4] * 1000;
        planInfo._ptr_subplaninfo[4+i]._min_green += (planTableTemp[1+i*4] * 100);
        planInfo._ptr_subplaninfo[4+i]._min_green += (planTableTemp[2+i*4] * 10);
        planInfo._ptr_subplaninfo[4+i]._min_green += planTableTemp[3+i*4];

        planInfo._ptr_subplaninfo[4+i]._max_green = 0;
        planInfo._ptr_subplaninfo[4+i]._max_green = planTableTemp[16+i*4] * 1000;
        planInfo._ptr_subplaninfo[4+i]._max_green += (planTableTemp[17+i*4] * 100);
        planInfo._ptr_subplaninfo[4+i]._max_green += (planTableTemp[18+i*4] * 10);
        planInfo._ptr_subplaninfo[4+i]._max_green += planTableTemp[19+i*4];
      }
    }
  }

  // cal. cycle

  planInfo._cycle_time = 0;
  for(int i = 0; i < planInfo._subphase_count; i++) {
    planInfo._cycle_time += planInfo._ptr_subplaninfo[i]._green;
    planInfo._cycle_time += planInfo._ptr_subplaninfo[i]._allred;
    planInfo._cycle_time += planInfo._ptr_subplaninfo[i]._yellow;
    planInfo._cycle_time += planInfo._ptr_subplaninfo[i]._pedgreen_flash;
    planInfo._cycle_time += planInfo._ptr_subplaninfo[i]._pedred;
  }
*/

}catch(...){}
}
//---------------------------------------------------------------------------
