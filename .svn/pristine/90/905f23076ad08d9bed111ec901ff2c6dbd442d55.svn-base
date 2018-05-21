#include "screenCurrentLightStatus.h"
#include "LCD240x128.h"
//#include "screenLogo.h"
#include "SCREENMain.h"
#include "screenRunTableList.h"
#include "CSTC.h"

#include <stdio.h>

#include "SMEM.h"
//---------------------------------------------------------------------------
ScreenCurrentLightStatus screenCurrentLightStatus;
//---------------------------------------------------------------------------
ScreenCurrentLightStatus::ScreenCurrentLightStatus(void)
{
    loadBitmapFromFile();
    initDispWord();
    cSelect=0;
}
//---------------------------------------------------------------------------
ScreenCurrentLightStatus::~ScreenCurrentLightStatus(void)
{
}
//---------------------------------------------------------------------------
void ScreenCurrentLightStatus::loadBitmapFromFile(void)
{
try {
    FILE *bitmap;
    bitmap=fopen("//cct//bitmap//backGround//TC5F//CurrentLightStatusBitmap.bit","rb");
    if (bitmap) {
        fread(ucCurrentLightStatusBitmap,3840,1,bitmap);
        fclose(bitmap);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenCurrentLightStatus::DisplayCurrentLightStatus(void)
{
try {
    smem.SetcFace(cCURRENTLIGHTSTATUS);
    lcd240x128.DISPLAY_GRAPHIC(0,ucCurrentLightStatusBitmap,128,30);

    vRefreshCurrentLightStatusData();
    vRefreshStepSec();

  } catch (...) {}
}

//---------------------------------------------------------------------------
void ScreenCurrentLightStatus::vRefreshCurrentLightStatusData(void)
{
try {
//OTMARKPRINTF  printf("Enter OK.\n");
//  unsigned short int iCurrentSegNo    = stc.vGetUSIData(CSTC_exec_segment_current_seg_no);
////OTMARKPRINTF  printf("iCurrentSegNo=%d.\n",iCurrentSegNo);
////OTMARKPRINTF  printf("stc.vGetUSIData(CSTC_exec_segment_current_seg_no)=%d.\n",stc.vGetUSIData(CSTC_exec_segment_current_seg_no));
  unsigned short int iCurrentPhaseID  = stc.vGetUSIData(CSTC_exec_plan_phase_order);
  unsigned short int iCurrentPlanID   = stc.vGetUSIData(CSTC_exec_plan_plan_ID);

//  stc.Lock_to_Load_Segment_for_Panel(iCurrentSegNo);
  stc.Lock_to_Load_Plan_for_Panel(iCurrentPlanID);
  stc.Lock_to_Load_Phase_for_Panel(iCurrentPhaseID);

  int iCurrentSubphase = stc.vGetUSIData(CSTC_exec_phase_current_subphase);
  int iCurrentSubphaseTotal = stc._panel_phase._subphase_count;

  int iCurrentStep     = stc.vGetUSIData(CSTC_exec_phase_current_subphase_step);
  int iCurrentStepTotal = stc._panel_phase._total_step_count;

  //Fuck Chen Ming Zu
  iCurrentStep = iCurrentStep + ((iCurrentSubphase)*5);
  iCurrentStepTotal = iCurrentStepTotal*iCurrentSubphaseTotal;

//  int iCurrentSegNoTotal = stc._panel_segment._segment_count;
//  int iCurrentSegTypeID = stc._panel_segment._segment_type;

  int iHiTMP, iLowTMP;
  unsigned short int usi1, usi2, usi3;

  iCurrentSubphase++;                                                           //From 0 Start, For Human, plus 1
  if(iCurrentSubphase < 100) {
    iHiTMP  = iCurrentSubphase / 10;
    iLowTMP = iCurrentSubphase % 10;
    lcd240x128.DISPLAY_GRAPHIC_XY(markSubPhase[0].X,markSubPhase[0].Y,word8x16[iHiTMP],markSubPhase[0].height,markSubPhase[0].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(markSubPhase[1].X,markSubPhase[1].Y,word8x16[iLowTMP],markSubPhase[1].height,markSubPhase[1].width/8);
  }

  if(iCurrentSubphaseTotal < 100) {
    iHiTMP  = iCurrentSubphaseTotal / 10;
    iLowTMP = iCurrentSubphaseTotal % 10;
    lcd240x128.DISPLAY_GRAPHIC_XY(markSubPhase[2].X,markSubPhase[2].Y,word8x16[iHiTMP],markSubPhase[2].height,markSubPhase[2].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(markSubPhase[3].X,markSubPhase[3].Y,word8x16[iLowTMP],markSubPhase[3].height,markSubPhase[3].width/8);
  }

//--
  iCurrentStep++;                                                               //From 0 Start, For Human, plus 1
  if(iCurrentStep < 100) {
    iHiTMP  = iCurrentStep / 10;
    iLowTMP = iCurrentStep % 10;
    lcd240x128.DISPLAY_GRAPHIC_XY(markStep[0].X,markStep[0].Y,word8x16[iHiTMP],markStep[0].height,markStep[0].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(markStep[1].X,markStep[1].Y,word8x16[iLowTMP],markStep[1].height,markStep[1].width/8);
  }

  iCurrentStepTotal = iCurrentStepTotal / iCurrentSubphaseTotal;
  if(iCurrentStepTotal < 100) {
    iHiTMP  = iCurrentStepTotal / 10;
    iLowTMP = iCurrentStepTotal % 10;
    lcd240x128.DISPLAY_GRAPHIC_XY(markStep[2].X,markStep[2].Y,word8x16[iHiTMP],markStep[2].height,markStep[2].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(markStep[3].X,markStep[3].Y,word8x16[iLowTMP],markStep[3].height,markStep[3].width/8);
  }

//--
  if(1) {
//    iHiTMP  = iCurrentPhaseID / 10;
//    iLowTMP = iCurrentPhaseID % 10;
//    lcd240x128.DISPLAY_GRAPHIC_XY(markPhaseID[0].X,markPhaseID[0].Y,word8x16[iHiTMP],markPhaseID[0].height,markPhaseID[0].width/8);
//    lcd240x128.DISPLAY_GRAPHIC_XY(markPhaseID[1].X,markPhaseID[1].Y,word8x16[iLowTMP],markPhaseID[1].height,markPhaseID[1].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(markPhaseID[0].X,markPhaseID[0].Y,word8x16[(iCurrentPhaseID>>4)&0xFF],markPhaseID[0].height,markPhaseID[0].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(markPhaseID[1].X,markPhaseID[1].Y,word8x16[iCurrentPhaseID-((iCurrentPhaseID>>4)&0xFF)*16],markPhaseID[1].height,markPhaseID[1].width/8);
  }

//--
  if(1) {

    usi1  = iCurrentPlanID / 100;
    usi2 = (iCurrentPlanID - usi1*100 ) / 10;
    usi3 = (iCurrentPlanID - usi1*100 - usi2*10) % 10;
    lcd240x128.DISPLAY_GRAPHIC_XY(markPlanID[0].X,markPlanID[0].Y,word8x16[usi1],markPlanID[0].height,markPlanID[0].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(markPlanID[1].X,markPlanID[1].Y,word8x16[usi2],markPlanID[1].height,markPlanID[1].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(markPlanID[2].X,markPlanID[2].Y,word8x16[usi3],markPlanID[2].height,markPlanID[2].width/8);

  }

  /*
//--
  if(1) {
    iHiTMP  = iCurrentSegTypeID / 10;
    iLowTMP = iCurrentSegTypeID % 10;
    lcd240x128.DISPLAY_GRAPHIC_XY(markSegTypeID[0].X,markSegTypeID[0].Y,word8x16[iHiTMP],markSegTypeID[0].height,markSegTypeID[0].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(markSegTypeID[1].X,markSegTypeID[1].Y,word8x16[iLowTMP],markSegTypeID[1].height,markSegTypeID[1].width/8);

  }

//--
  if(iCurrentSegNo < 100) {
    iHiTMP  = iCurrentSegNo / 10;
    iLowTMP = iCurrentSegNo % 10;
    lcd240x128.DISPLAY_GRAPHIC_XY(markStepSecCount[0].X,markStepSecCount[0].Y,word8x16[iHiTMP],markStepSecCount[0].height,markStepSecCount[0].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(markStepSecCount[1].X,markStepSecCount[1].Y,word8x16[iLowTMP],markStepSecCount[1].height,markStepSecCount[1].width/8);
  //OTMARKPRINTF  printf("iCurrentSegNo: %d.\n", iCurrentSegNo);
  }
  if(iCurrentSegNoTotal < 100) {
    iHiTMP  = iCurrentSegNoTotal / 10;
    iLowTMP = iCurrentSegNoTotal % 10;
    lcd240x128.DISPLAY_GRAPHIC_XY(markStepSecCount[2].X,markStepSecCount[2].Y,word8x16[iHiTMP],markStepSecCount[2].height,markStepSecCount[2].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(markStepSecCount[3].X,markStepSecCount[3].Y,word8x16[iLowTMP],markStepSecCount[3].height,markStepSecCount[3].width/8);
  //OTMARKPRINTF  printf("iCurrentSegNoTotal: %d.\n", iCurrentSegNoTotal);
  }

  if(iCurrentPhaseID==0xFF || iCurrentPhaseID==0x80) {                          //Error Handle
    lcd240x128.DISPLAY_GRAPHIC_XY(markSegTypeID[0].X,markSegTypeID[0].Y,word8x16[0x0F],markSegTypeID[0].height,markSegTypeID[0].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(markSegTypeID[1].X,markSegTypeID[1].Y,word8x16[0x0F],markSegTypeID[1].height,markSegTypeID[1].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(markStepSecCount[0].X,markStepSecCount[0].Y,word8x16[0x0F],markStepSecCount[0].height,markStepSecCount[0].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(markStepSecCount[1].X,markStepSecCount[1].Y,word8x16[0x0F],markStepSecCount[1].height,markStepSecCount[1].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(markStepSecCount[2].X,markStepSecCount[2].Y,word8x16[0x0F],markStepSecCount[2].height,markStepSecCount[2].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(markStepSecCount[3].X,markStepSecCount[3].Y,word8x16[0x0F],markStepSecCount[3].height,markStepSecCount[3].width/8);
  }

*/


} catch(...){}
}

//---------------------------------------------------------------------------
void ScreenCurrentLightStatus::vRefreshStepSec(void)
{
try {
  int i1, i2, i3, i4;
  unsigned short int usiStepSec = stc.vGetStepTime();

  if(usiStepSec < 10000) {
    i1  = usiStepSec / 1000;
    i2 = (usiStepSec - (i1*1000)) / 100;
    lcd240x128.DISPLAY_GRAPHIC_XY(markStepSecCount[0].X,markStepSecCount[0].Y,word8x16[i1],markStepSecCount[0].height,markStepSecCount[0].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(markStepSecCount[1].X,markStepSecCount[1].Y,word8x16[i2],markStepSecCount[1].height,markStepSecCount[1].width/8);

  }
  if(usiStepSec < 10000) {
    i3  = (usiStepSec - (i2*100+i1*1000)) / 10;
    i4 = (usiStepSec - (i3*10+i2*100+i1*1000));
    lcd240x128.DISPLAY_GRAPHIC_XY(markStepSecCount[2].X,markStepSecCount[2].Y,word8x16[i3],markStepSecCount[2].height,markStepSecCount[2].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(markStepSecCount[3].X,markStepSecCount[3].Y,word8x16[i4],markStepSecCount[3].height,markStepSecCount[3].width/8);
  }
}catch(...){}
}

//---------------------------------------------------------------------------
void ScreenCurrentLightStatus::initDispWord(void)
{
try {

    for(int i = 0; i < 4; i++) {
          markSubPhase[i].width = 8;   markSubPhase[i].height = 16;
          markStep[i].width = 8;       markStep[i].height = 16;
if(i<2) { markPhaseID[i].width = 8;    markPhaseID[i].height = 16;    }
if(i<3) { markPlanID[i].width = 8;     markPlanID[i].height = 16;     }
//if(i<2) { markSegTypeID[i].width = 8;  markSegTypeID[i].height = 16;  }
          markStepSecCount[i].width = 8; markStepSecCount[i].height = 16;
    }

    for(int i = 0; i < 4; i++) { markSubPhase[i].Y = 32; markStep[i].Y = 32;}
    markSubPhase[0].X = 72; markSubPhase[1].X = 80;
    markSubPhase[2].X = 96; markSubPhase[3].X = 108;

    markStep[0].X = 160; markStep[1].X = 168;
    markStep[2].X = 184; markStep[3].X = 192;
//====================
    for(int i = 0; i < 2; i++) { markPhaseID[i].Y = 58; }
    for(int i = 0; i < 3; i++) { markPlanID[i].Y = 58; }
    markPhaseID[0].X = 96; markPhaseID[1].X = 104;
    markPlanID[0].X = 200; markPlanID[1].X = 208, markPlanID[2].X = 216;

//====================
//    for(int i = 0; i < 2; i++) { markSegTypeID[i].Y = 84;}
//    markSegTypeID[0].X = 88; markSegTypeID[1].X = 96;

    for(int i = 0; i < 4; i++) { markStepSecCount[i].Y = 84;}
    markStepSecCount[0].X = 72; markStepSecCount[1].X = 80;
    markStepSecCount[2].X = 88; markStepSecCount[3].X = 96;

  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenCurrentLightStatus::doKeyWork(BYTE key)
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
void ScreenCurrentLightStatus::doKey0Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenCurrentLightStatus::doKey1Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenCurrentLightStatus::doKey2Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenCurrentLightStatus::doKey3Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenCurrentLightStatus::doKey4Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenCurrentLightStatus::doKey5Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenCurrentLightStatus::doKey6Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenCurrentLightStatus::doKey7Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenCurrentLightStatus::doKey8Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenCurrentLightStatus::doKey9Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenCurrentLightStatus::doKeyAWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenCurrentLightStatus::doKeyBWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenCurrentLightStatus::doKeyCWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenCurrentLightStatus::doKeyDWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenCurrentLightStatus::doKeyEWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenCurrentLightStatus::doKeyFWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenCurrentLightStatus::doKeyF1Work(void)
{
//    screenLogo.DisplayLogo();
    screenMain.DisplayMain();
}
//---------------------------------------------------------------------------
void ScreenCurrentLightStatus::doKeyF2Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenCurrentLightStatus::doKeyF3Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenCurrentLightStatus::doKeyF4Work(void)
{
//    screenMain.DisplayMain();
    screenRunTableList.DisplayRunTableList();
}
//---------------------------------------------------------------------------
void ScreenCurrentLightStatus::doKeyUPWork(void)
{
try {
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenCurrentLightStatus::doKeyDOWNWork(void)
{
try {
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenCurrentLightStatus::doKeyLEFTWork(void)
{
try {
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenCurrentLightStatus::doKeyRIGHTWork(void)
{
try {
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenCurrentLightStatus::doKeyEnterWork(void)
{
    switch (cSelect+1) {
        case 1:
          doKey1Work();
        break;
        case 2:
          doKey2Work();
        break;
        case 3:
          doKey3Work();
        break;
        case 4:
          doKey4Work();
        break;
        case 5:
          doKey5Work();
        break;
        case 6:
          doKey6Work();
        break;
        case 7:
          doKey7Work();
        break;
        case 8:
          doKey8Work();
        break;
        case 9:
          doKey9Work();
        break;
        case 10:
          doKey0Work();
        break;
        default:
        break;
    }
}
//---------------------------------------------------------------------------

