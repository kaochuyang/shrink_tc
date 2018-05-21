#include "screenPhasePreview.h"

#include "SCREENMain.h"
#include "LCD240x128.h"
#include "screenPlanEditF2.h"
#include "screenPlan.h"

#include "CSTC.h"
#include "var.h"
#include <stdio.h>

#include "SMEM.h"
//---------------------------------------------------------------------------
ScreenPhasePreview screenPhasePreview;
//---------------------------------------------------------------------------
ScreenPhasePreview::ScreenPhasePreview(void)
{
  /* Hsinchu:0 Tainan:1 Yulin:2 */
    //OT20111128
    // usiHTY = MACHINELOCATE;
    usiHTY = smem.vGetUCData(TC_CCT_MachineLocation);

    loadBitmapFromFile();
    initDispWord();
    cPosition=0;
    cSelect=1;
    cFace=0;
    ucPhaseSel = 0;
    ucPhaseOri = 0;

    for(int i = 0; i < 256; i++)
      usiPhaseOrderArray[i] = 999;

//PhaseAdd
    if(usiHTY ==0) {                                                            //Hsinchu
      usiPhaseOrderArray[0] = 0;
      usiPhaseOrderArray[1] = 0x4C;
      usiPhaseOrderArray[2] = 0x5C;
    }
    else if(usiHTY == 1) {                                                      //Tainan
      usiPhaseOrderArray[0] = 0;
      usiPhaseOrderArray[1] = 0x10;
      usiPhaseOrderArray[2] = 0x20;
    }
    else if(usiHTY == 2) {                                                      //YuLin
      usiPhaseOrderArray[0] = 0;
      usiPhaseOrderArray[1] = 0x10;
      usiPhaseOrderArray[2] = 0x11;
      usiPhaseOrderArray[3] = 0x58;
      usiPhaseOrderArray[4] = 0x82;
    }


}
//---------------------------------------------------------------------------
ScreenPhasePreview::~ScreenPhasePreview(void)
{
}
//---------------------------------------------------------------------------
void ScreenPhasePreview::loadBitmapFromFile(void)
{
try {
    FILE *bitmap;

    printf("printfMsg loadBitmapFromFile ucPhaseSel:%d\n", ucPhaseSel);

//PhaseAdd
    if(usiHTY ==0) {
      switch(ucPhaseSel) {
        case (0):
          bitmap=fopen("//cct//bitmap//backGround//PhaseHC//PhaseHC_00.bit","rb");
          break;
        case (0x4C):
          bitmap=fopen("//cct//bitmap//backGround//PhaseHC//PhaseHC_4C.bit","rb");
          break;
        case (0x5C):
          bitmap=fopen("//cct//bitmap//backGround//PhaseHC//PhaseHC_5C.bit","rb");
          break;

        default:
          bitmap=fopen("//cct//bitmap//backGround//PhaseHC//PhaseHC_00.bit","rb");
          ucPhaseSel = 0;
          break;
      }
    }
    else if(usiHTY == 1) {
      switch(ucPhaseSel) {
        case (0):
          bitmap=fopen("//cct//bitmap//backGround//PhaseTN//PhaseTN_00.bit","rb");
          break;
        case (0x10):
          bitmap=fopen("//cct//bitmap//backGround//PhaseTN//PhaseTN_10.bit","rb");
          break;
        case (0x20):
          bitmap=fopen("//cct//bitmap//backGround//PhaseTN//PhaseTN_20.bit","rb");
          break;


        default:
          bitmap=fopen("//cct//bitmap//backGround//PhaseTN//PhaseTN_00.bit","rb");
          ucPhaseSel = 0;
          break;
      }
    }
    else if(usiHTY == 2) {
      switch(ucPhaseSel) {
        case (0):
          bitmap=fopen("//cct//bitmap//backGround//PhaseYL//PhaseYL_00.bit","rb");
          break;
        case (0x10):
          bitmap=fopen("//cct//bitmap//backGround//PhaseYL//PhaseYL_10.bit","rb");
          break;
        case (0x11):
          bitmap=fopen("//cct//bitmap//backGround//PhaseYL//PhaseYL_11.bit","rb");
          break;
        case (0x58):
          bitmap=fopen("//cct//bitmap//backGround//PhaseYL//PhaseYL_58.bit","rb");
          break;
        case (0x82):
          bitmap=fopen("//cct//bitmap//backGround//PhaseYL//PhaseYL_82.bit","rb");
          break;

        default:
          bitmap=fopen("//cct//bitmap//backGround//PhaseYL//PhaseYL_00.bit","rb");
          ucPhaseSel = 0;
          break;
      }
    }

    if (bitmap) {
        fread(phaseSelBitmap,3840,1,bitmap);
        fclose(bitmap);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenPhasePreview::DisplayPhasePreview(int iPlanIn, unsigned char ucPhaseIn, bool bFreshOri)
{
try {
    smem.SetcFace(cPHASESEL);

    if(bFreshOri)
      ucPhaseOri = ucPhaseIn;

    ucPhaseSel = ucPhaseIn;
    iPlan = iPlanIn;

    vFindPhase();

    loadBitmapFromFile();
    lcd240x128.DISPLAY_GRAPHIC(0,phaseSelBitmap,128,30);

    vClearSpace();

    printf("printfMsg ucPhaseSel:%d\n", ucPhaseSel);

  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenPhasePreview::DisplayPhasePreviewRefresh(void)
{
try {
    loadBitmapFromFile();
    lcd240x128.DISPLAY_GRAPHIC(0,phaseSelBitmap,128,30);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenPhasePreview::initDispWord(void)
{
try {

  } catch (...) {}
}

//---------------------------------------------------------------------------
void ScreenPhasePreview::doKeyWork(BYTE key)
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
void ScreenPhasePreview::doKey0Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenPhasePreview::doKey1Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenPhasePreview::doKey2Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenPhasePreview::doKey3Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenPhasePreview::doKey4Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenPhasePreview::doKey5Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenPhasePreview::doKey6Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenPhasePreview::doKey7Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenPhasePreview::doKey8Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenPhasePreview::doKey9Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenPhasePreview::doKeyAWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenPhasePreview::doKeyBWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenPhasePreview::doKeyCWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenPhasePreview::doKeyDWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenPhasePreview::doKeyEWork(void)
{
  if(cFace > 0) {
    cFace--;
    if( usiPhaseOrderArray[cFace] != 999 )
      ucPhaseSel = usiPhaseOrderArray[cFace];
    else
      cFace++;

    DisplayPhasePreviewRefresh();
    vClearSpace();
  }
  printf("printfMsg ucPhaseSel:%d\n", ucPhaseSel);
}
//---------------------------------------------------------------------------
void ScreenPhasePreview::doKeyFWork(void)
{
  if(cFace < 255) {
    cFace++;
    if( usiPhaseOrderArray[cFace] != 999 ) {
      printf("printfMsg usiPhaseOrderArray[cFace]:%d\n", usiPhaseOrderArray[cFace]);
      ucPhaseSel = usiPhaseOrderArray[cFace];
      printf("printfMsg ucPhaseSel:%d\n", ucPhaseSel);
      DisplayPhasePreviewRefresh();
      vClearSpace();
    }
    else
      cFace--;
  }
  printf("printfMsg end ucPhaseSel:%d\n", ucPhaseSel);
}
//---------------------------------------------------------------------------
void ScreenPhasePreview::doKeyF1Work(void)
{
    screenMain.DisplayMain();
}
//---------------------------------------------------------------------------
void ScreenPhasePreview::doKeyF2Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenPhasePreview::doKeyF3Work(void)
{

}
//---------------------------------------------------------------------------
void ScreenPhasePreview::doKeyF4Work(void)
{
  if(usiPreviewOrView == PreviewScreen) {
    screenPlanEditF2.vDisplayPlanNoReload(iPlan);
  }
  else if(usiPreviewOrView == ViewScreen) {
    screenPlan.DisplayPlan(iPlan, INVAILDVALUE);
  }
}
//---------------------------------------------------------------------------
void ScreenPhasePreview::doKeyUPWork(void)
{
try {
    if(cFace > 0) {
      cFace--;
      if( usiPhaseOrderArray[cFace] != 999 )
        ucPhaseSel = usiPhaseOrderArray[cFace];
      else
        cFace++;

      DisplayPhasePreviewRefresh();
      vClearSpace();
    }
    printf("printfMsg ucPhaseSel:%d\n", ucPhaseSel);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenPhasePreview::doKeyDOWNWork(void)
{
try {
    if(cFace < 255) {
      cFace++;
      if( usiPhaseOrderArray[cFace] != 999 ) {
        printf("printfMsg usiPhaseOrderArray[cFace]:%d\n", usiPhaseOrderArray[cFace]);
        ucPhaseSel = usiPhaseOrderArray[cFace];
        printf("printfMsg ucPhaseSel:%d\n", ucPhaseSel);
        DisplayPhasePreviewRefresh();
        vClearSpace();
      }
      else
        cFace--;
    }
    printf("printfMsg end ucPhaseSel:%d\n", ucPhaseSel);
}catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenPhasePreview::doKeyLEFTWork(void)
{
try {
  } catch(...) {}
}
//---------------------------------------------------------------------------
void ScreenPhasePreview::doKeyRIGHTWork(void)
{
try {
  } catch(...) {}
}
//---------------------------------------------------------------------------
void ScreenPhasePreview::doKeyEnterWork(void)
{
try {

  if(usiPreviewOrView == PreviewScreen) {
    if(vSetPlanTable()) {
      screenPlanEditF2.vDisplayPlanNoReload(iPlan);
    }
  }

  } catch (...){}
}

//---------------------------------------------------------------------------
void ScreenPhasePreview::vTmpArrayToData(void)
{
try{

  /*
  specialDaySegtype[iSeg-8]._start_year =  ucSpecialDay[0] * 1000;
  specialDaySegtype[iSeg-8]._start_year += ucSpecialDay[1] * 100;
  specialDaySegtype[iSeg-8]._start_year += ucSpecialDay[2] * 10;
  specialDaySegtype[iSeg-8]._start_year += ucSpecialDay[3];

  specialDaySegtype[iSeg-8]._start_month = ucSpecialDay[4];
  specialDaySegtype[iSeg-8]._start_month += ucSpecialDay[5];

  specialDaySegtype[iSeg-8]._start_day = ucSpecialDay[6];
  specialDaySegtype[iSeg-8]._start_day += ucSpecialDay[7];

  specialDaySegtype[iSeg-8]._end_year =  ucSpecialDay[8] * 1000;
  specialDaySegtype[iSeg-8]._end_year += ucSpecialDay[9] * 100;
  specialDaySegtype[iSeg-8]._end_year += ucSpecialDay[10] * 10;
  specialDaySegtype[iSeg-8]._end_year += ucSpecialDay[11];

  specialDaySegtype[iSeg-8]._end_month = ucSpecialDay[12];
  specialDaySegtype[iSeg-8]._end_month += ucSpecialDay[13];

  specialDaySegtype[iSeg-8]._end_day = ucSpecialDay[14];
  specialDaySegtype[iSeg-8]._end_day += ucSpecialDay[15];
  */

}catch(...){}
}

//---------------------------------------------------------------------------
bool ScreenPhasePreview::vCheck(void)
{
try{
  bool bRet = true;

/*
  if(specialDaySegtype[iSeg-8]._start_year < 2000) { printf("printfMsg 1\n"); bRet = false; }
  if(specialDaySegtype[iSeg-8]._end_year > 2037)  { printf("printfMsg 2\n"); bRet = false; }

  if(specialDaySegtype[iSeg-8]._start_year >= specialDaySegtype[iSeg-8]._end_year) {
    if(specialDaySegtype[iSeg-8]._start_year > specialDaySegtype[iSeg-8]._end_year) { printf("printfMsg 3\n"); bRet = false; }
    if(specialDaySegtype[iSeg-8]._start_month >= specialDaySegtype[iSeg-8]._end_month) {
      if(specialDaySegtype[iSeg-8]._start_month > specialDaySegtype[iSeg-8]._end_month) { printf("printfMsg 4\n"); bRet = false; }
      if(specialDaySegtype[iSeg-8]._start_day > specialDaySegtype[iSeg-8]._end_day) {
        printf("printfMsg 5\n"); bRet = false;
      }
    }
  }

  if(specialDaySegtype[iSeg-8]._start_year < 2000) { printf("printfMsg 6\n"); bRet = false; }
  if(specialDaySegtype[iSeg-8]._end_year > 2037) { printf("printfMsg 7\n"); bRet = false; }
  if(specialDaySegtype[iSeg-8]._start_month < 1) { printf("printfMsg 8\n"); bRet = false; }
  if(specialDaySegtype[iSeg-8]._end_month > 12) { printf("printfMsg 9\n"); bRet = false; }
  if(specialDaySegtype[iSeg-8]._start_day < 1) { printf("printfMsg 10\n"); bRet = false; }
  if(specialDaySegtype[iSeg-8]._end_day > 31) { printf("printfMsg 11\n"); bRet = false; }
*/
  return bRet;

}catch(...){}
}

//---------------------------------------------------------------------------
void ScreenPhasePreview::vSaveSpecialDay(void)
{
try{

/*
  stc._panel_holidayseg._segment_type = iSeg;
  stc._panel_holidayseg._start_year = specialDaySegtype[iSeg-8]._start_year;
  stc._panel_holidayseg._end_year = specialDaySegtype[iSeg-8]._end_year;
  stc._panel_holidayseg._start_month = specialDaySegtype[iSeg-8]._start_month;
  stc._panel_holidayseg._end_month = specialDaySegtype[iSeg-8]._end_month;
  stc._panel_holidayseg._start_day = specialDaySegtype[iSeg-8]._start_day;
  stc._panel_holidayseg._end_day = specialDaySegtype[iSeg-8]._end_day;

  stc.Lock_to_Save_HoliDaySegment_from_Panel();
*/

}catch(...){}
}

//---------------------------------------------------------------------------
bool ScreenPhasePreview::vSetPlanTable(void)
{
try{

  int iSubphaseCount;

//PhaseAdd
  if(ucPhaseSel != ucPhaseOri)
  {
    if(usiHTY ==0) {                                                            //Hsinchu
    switch(ucPhaseSel) {
      case (0):
        iSubphaseCount = 2;
      break;
      case (0x4C):
        iSubphaseCount = 4;
      break;
      case (0x5C):
        iSubphaseCount = 5;
      break;

      default:
        return false;
      break;
    }

    }
    else if(usiHTY == 1) {                                                       //Tainan
      switch(ucPhaseSel) {
        case (0):
          iSubphaseCount = 2;
        break;
        case (0x10):
          iSubphaseCount = 3;
        break;
        case (0x20):
          iSubphaseCount = 3;
        break;
        default:
          return false;
        break;
      }
    }
    else if(usiHTY == 2) {                                                      //YuLin
      switch(ucPhaseSel) {
        case (0):
          iSubphaseCount = 2;
        break;
        case (0x10):
          iSubphaseCount = 3;
        break;
        case (0x11):
          iSubphaseCount = 3;
        break;
        case (0x58):
          iSubphaseCount = 4;
        break;
        case (0x82):
          iSubphaseCount = 1;
        break;

        default:
          return false;
        break;
      }
    }

    if(iSubphaseCount != stc._panel_plan._subphase_count)
      stc.Lock_to_Reset_Plan_for_Panel(iPlan, iSubphaseCount);                  //reset

    stc._panel_plan._dir = 0;
    stc._panel_plan._phase_order = ucPhaseSel;
    stc._panel_plan._subphase_count = iSubphaseCount;

    stc._panel_plan._cycle_time = 0;
    stc._panel_plan._offset = 0;

    for (int i=0;i<iSubphaseCount;i++) {       //¦A­«¼g
         if(i < 3)
           stc._panel_plan._ptr_subplaninfo[i]._green = 20 - i*5;
         else
           stc._panel_plan._ptr_subplaninfo[i]._green = 5;
         stc._panel_plan._ptr_subplaninfo[i]._min_green = 0;
         stc._panel_plan._ptr_subplaninfo[i]._max_green = 255;
         stc._panel_plan._ptr_subplaninfo[i]._yellow = 3;
         stc._panel_plan._ptr_subplaninfo[i]._allred = 2;
         stc._panel_plan._ptr_subplaninfo[i]._pedgreen_flash = 3;
         stc._panel_plan._ptr_subplaninfo[i]._pedred = 2;

         stc._panel_plan._cycle_time += stc._panel_plan._ptr_subplaninfo[i]._green;
         stc._panel_plan._cycle_time += stc._panel_plan._ptr_subplaninfo[i]._allred;
         stc._panel_plan._cycle_time += stc._panel_plan._ptr_subplaninfo[i]._yellow;
         stc._panel_plan._cycle_time += stc._panel_plan._ptr_subplaninfo[i]._pedgreen_flash;
         stc._panel_plan._cycle_time += stc._panel_plan._ptr_subplaninfo[i]._pedred;
    }

  }
  return true;

}catch(...){}
}

//---------------------------------------------------------------------------
bool ScreenPhasePreview::vFindPhase(void)
{
try{

  for(int i = 0; i < 256; i++)
  {
    if(ucPhaseSel == (unsigned char)usiPhaseOrderArray[i]) {
      cFace = i;
      return true;
      break;
    }
  }

  return false;
}catch(...){}
}

//---------------------------------------------------------------------------
bool ScreenPhasePreview::vSetPreviewOrView(unsigned short int usiIn)
{
try{
  usiPreviewOrView = usiIn;
  return true;

}catch(...){}
}

//---------------------------------------------------------------------------
bool ScreenPhasePreview::vClearSpace(void)
{
try{

  BYTE cursor[1]={0};
//  lcd240x128.DISPLAY_GRAPHIC_XY(1,1,cursor,1,1);
//  lcd240x128.DISPLAY_GRAPHIC_XY(x,y,cursor,1,1);

  if(usiPreviewOrView == PreviewScreen) {
    for(int i = 4; i < 14; i++) {
      lcd240x128.DISPLAY_GRAPHIC_XY(32, i,cursor,1,1);
      lcd240x128.DISPLAY_GRAPHIC_XY(40, i,cursor,1,1);
      lcd240x128.DISPLAY_GRAPHIC_XY(48, i,cursor,1,1);
    }
  }
  else if(usiPreviewOrView == ViewScreen) {

    for(int i = 97; i <= 126; i++) {                                           //OK button y
      lcd240x128.DISPLAY_GRAPHIC_XY(208, i,cursor,1,1);
      lcd240x128.DISPLAY_GRAPHIC_XY(216, i,cursor,1,1);
      lcd240x128.DISPLAY_GRAPHIC_XY(224, i,cursor,1,1);
      lcd240x128.DISPLAY_GRAPHIC_XY(232, i,cursor,1,1);
    }
    for(int i = 3; i < 15; i++) {
      lcd240x128.DISPLAY_GRAPHIC_XY(8, i,cursor,1,1);
      lcd240x128.DISPLAY_GRAPHIC_XY(16, i,cursor,1,1);
      lcd240x128.DISPLAY_GRAPHIC_XY(24, i,cursor,1,1);
    }
  }
  return true;

}catch(...){}
}
