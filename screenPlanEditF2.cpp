#include "screenPlanEditF2.h"
#include "screenPlan.h"

#include "SCREENMain.h"

#include "LCD240x128.h"
#include "CSTC.h"
//Removed #include "screenPlanEditStep1.h"

#include "screenPhasePreview.h"

#include <stdio.h>

#include "SMEM.h"
//---------------------------------------------------------------------------
ScreenPlanEditF2 screenPlanEditF2;
//---------------------------------------------------------------------------
ScreenPlanEditF2::ScreenPlanEditF2(void)
{
    loadBitmapFromFile();
    initDispWord();
    face=1;
}
//---------------------------------------------------------------------------
ScreenPlanEditF2::~ScreenPlanEditF2(void)
{
}
//---------------------------------------------------------------------------
void ScreenPlanEditF2::loadBitmapFromFile(void)
{
try {
    FILE *bitmap;
    bitmap=fopen("//cct//bitmap//backGround//TC5F//plan_LUeditF2.bit","rb");
    if (bitmap) {
        fread(planBitmap[0],3840,1,bitmap);
        fclose(bitmap);
    }

    bitmap=fopen("//cct//bitmap//backGround//TC5F//plan_LMeditF2.bit","rb");
    if (bitmap) {
        fread(planBitmap[1],3840,1,bitmap);
        fclose(bitmap);
    }

    bitmap=fopen("//cct//bitmap//backGround//TC5F//plan_LDeditF2.bit","rb");
    if (bitmap) {
        fread(planBitmap[2],3840,1,bitmap);
        fclose(bitmap);
    }

    bitmap=fopen("//cct//bitmap//backGround//TC5F//plan_RUeditF2.bit","rb");
    if (bitmap) {
        fread(planBitmap[3],3840,1,bitmap);
        fclose(bitmap);
    }

    bitmap=fopen("//cct//bitmap//backGround//TC5F//plan_RMeditF2.bit","rb");
    if (bitmap) {
        fread(planBitmap[4],3840,1,bitmap);
        fclose(bitmap);
    }

    bitmap=fopen("//cct//bitmap//backGround//TC5F//plan_RDeditF2.bit","rb");
    if (bitmap) {
        fread(planBitmap[5],3840,1,bitmap);
        fclose(bitmap);
    }

  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenPlanEditF2::DisplayPlan(int plan)
{
try {
    smem.SetcFace(cPLANF2);
    face=1;   //左上
//    cSelPosition = 0;
    cSelPosition = 1;
//    cPosition = 2;                                                              //no plan
    cPosition = 0;
    lcd240x128.DISPLAY_GRAPHIC(0,planBitmap[face-1],128,30);
    LoadPlan(plan);
    DisplayPlanTable(face);
    DisplayPlanIdAndPhase();  //DisplayPlanIdAndPhase();
    DisplayCycleAndOffset();  //DisplayCycleAndOffset();

//    setCursor8x16(planIdAndPhase[cPosition].X,planIdAndPhase[cPosition].Y+16);
    setCursor8x16(planTable[cPosition].X,planTable[cPosition].Y+16);

  } catch (...) {}
}

//---------------------------------------------------------------------------
void ScreenPlanEditF2::vDisplayPlanNoReload(int plan)
{
try {
    smem.SetcFace(cPLANF2);
    lcd240x128.DISPLAY_GRAPHIC(0,planBitmap[face-1],128,30);

    vLoadPlanOnlyFromSTCPanel(plan);

    DisplayPlanTable(face);
    DisplayPlanIdAndPhase();  //DisplayPlanIdAndPhase();
    DisplayCycleAndOffset();  //DisplayCycleAndOffset();

    setCursor8x16(planIdAndPhase[cPosition].X,planIdAndPhase[cPosition].Y+16);

  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenPlanEditF2::initDispWord(void)
{
    for (int i=0;i<4;i++) {
         if (i>=0 && i<=1) planIdAndPhase[i].X=144+i*8;
         else if (i>=2 && i<=3) planIdAndPhase[i].X=208+(i-2)*8;
         planIdAndPhase[i].Y=2;
         planIdAndPhase[i].width=8;
         planIdAndPhase[i].height=16;
    }

    for (int i=0;i<48;i++) {
         if (i>=0 && i<=3) planTable[i].X=64+i*8;
         else if (i>=4 && i<=7) planTable[i].X=104+(i-4)*8;
         else if (i>=8 && i<=11) planTable[i].X=144+(i-8)*8;
         else if (i>=12 && i<=15) planTable[i].X=184+(i-12)*8;
         else if (i>=16 && i<=19) planTable[i].X=64+(i-16)*8;
         else if (i>=20 && i<=23) planTable[i].X=104+(i-20)*8;
         else if (i>=24 && i<=27) planTable[i].X=144+(i-24)*8;
         else if (i>=28 && i<=31) planTable[i].X=184+(i-28)*8;
         else if (i>=32 && i<=35) planTable[i].X=64+(i-32)*8;
         else if (i>=36 && i<=39) planTable[i].X=104+(i-36)*8;
         else if (i>=40 && i<=43) planTable[i].X=144+(i-40)*8;
         else if (i>=44 && i<=47) planTable[i].X=184+(i-44)*8;

         if (i>=0 && i<=15) planTable[i].Y=34;
         else if (i>=16 && i<=31) planTable[i].Y=50;
         else if (i>=32 && i<=47) planTable[i].Y=66;
         planTable[i].width=8;
         planTable[i].height=16;
    }

    for (int i=0;i<8;i++) {
         if (i>=0 && i<=3) cycleAndOffset[i].X=32+i*8;
         else if (i>=4 && i<=7) cycleAndOffset[i].X=32+(i-4)*8;
         if (i>=0 && i<=3) cycleAndOffset[i].Y=94;
         else if (i>=4 && i<=7) cycleAndOffset[i].Y=111;
         cycleAndOffset[i].width=8;
         cycleAndOffset[i].height=16;
    }

}
//---------------------------------------------------------------------------
void ScreenPlanEditF2::LoadPlan(int plan)
{
try {
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

  } catch (...) {}
}

//---------------------------------------------------------------------------
void ScreenPlanEditF2::vLoadPlanOnlyFromSTCPanel(int plan)
{
try {

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

  } catch (...) {}
}

//---------------------------------------------------------------------------
void ScreenPlanEditF2::DisplayPlanTable(int page)
{
try {
    DisplayBackGround(page);

    for (int i=0;i<2;i++) {
         if (page==1 || page==4) DisplayGreenAllRedYellow(page);
         else if (page==2 || page==5) DisplayPGFPR(page);
         else if (page==3 || page==6) DisplayMinmaxGreen(page);
    }

    DisplayUpAndDownPage(page); //DisplayUpAndDownPage(page);
    DisplayLeftAndRightPage(page); //DisplayLeftAndRightPage(page);

    setCursor8x16(planTable[cPosition].X,planTable[cPosition].Y+16);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenPlanEditF2::SavePlan(bool bSaveToSTC)
{
  //Read phase firest, i want subphasecount of phase

  printf("printfMsg planInfo._phase_orderX:%x\n", planInfo._phase_order);
  printf("printfMsg planInfo._planid:%d\n", planInfo._planid);
  printf("printfMsg planInfo._subphase_count:%d\n", planInfo._subphase_count);

  if(stc.Lock_to_Load_Phase_for_Panel(planInfo._phase_order)) {
      if(stc._panel_phase._subphase_count == planInfo._subphase_count &&
         stc._panel_phase._phase_order == planInfo._phase_order) {

        printf("Data Right, writing\n");


        stc.Lock_to_Reset_Plan_for_Panel(planInfo._planid, planInfo._subphase_count);
        stc._panel_plan._planid = planInfo._planid;
        stc._panel_plan._dir = planInfo._dir;
        stc._panel_plan._phase_order = planInfo._phase_order;

        stc._panel_plan._subphase_count = planInfo._subphase_count;
  //printfMsg      printf("savePlan subphase_count: %d\n", planInfo._subphase_count);

        stc._panel_plan._cycle_time = planInfo._cycle_time;
        stc._panel_plan._offset = planInfo._offset;
        printf("savePlan _offset: %d\n", planInfo._offset);

        for (int i=0;i<planInfo._subphase_count;i++) {
             stc._panel_plan._ptr_subplaninfo[i]._green = planInfo._ptr_subplaninfo[i]._green;
             stc._panel_plan._ptr_subplaninfo[i]._min_green = planInfo._ptr_subplaninfo[i]._min_green;
             stc._panel_plan._ptr_subplaninfo[i]._max_green = planInfo._ptr_subplaninfo[i]._max_green;
             stc._panel_plan._ptr_subplaninfo[i]._yellow = planInfo._ptr_subplaninfo[i]._yellow;
             stc._panel_plan._ptr_subplaninfo[i]._allred = planInfo._ptr_subplaninfo[i]._allred;
             stc._panel_plan._ptr_subplaninfo[i]._pedgreen_flash = planInfo._ptr_subplaninfo[i]._pedgreen_flash;
             stc._panel_plan._ptr_subplaninfo[i]._pedred = planInfo._ptr_subplaninfo[i]._pedred;
        }

        if(bSaveToSTC) {
          stc.Lock_to_Save_Plan_from_Panel();
          screenPlan.DisplayPlan(planInfo._planid, INVAILDVALUE);
        }
      } else if ( stc._panel_phase._phase_order == planInfo._phase_order &&
                  planInfo._phase_order == 0xB0 &&
//OT20111128
//                  MACHINELOCATE == MACHINELOCATEATHSINCHU )                     //Version HsinChu
                  smem.vGetUCData(TC_CCT_MachineLocation) == MACHINELOCATEATHSINCHU)
      {
        printf("Phase 0xB0, writing\n");

        stc.Lock_to_Reset_Plan_for_Panel(planInfo._planid, 1);
        stc._panel_plan._planid = planInfo._planid;
        stc._panel_plan._dir = planInfo._dir;
        stc._panel_plan._phase_order = planInfo._phase_order;

        stc._panel_plan._subphase_count = 1;

        if(planInfo._ptr_subplaninfo[0]._green > 10 || planInfo._ptr_subplaninfo[0]._green == 0) {
          planInfo._ptr_subplaninfo[0]._green = 10;
        }
        planInfo._cycle_time = planInfo._ptr_subplaninfo[0]._green;

        stc._panel_plan._cycle_time = planInfo._cycle_time;

        stc._panel_plan._offset = planInfo._offset;

        stc._panel_plan._ptr_subplaninfo[0]._green = planInfo._ptr_subplaninfo[0]._green;
        stc._panel_plan._ptr_subplaninfo[0]._min_green = 0;
        stc._panel_plan._ptr_subplaninfo[0]._max_green = 255;
        stc._panel_plan._ptr_subplaninfo[0]._yellow = 0;
        stc._panel_plan._ptr_subplaninfo[0]._allred = 0;
        stc._panel_plan._ptr_subplaninfo[0]._pedgreen_flash = 0;
        stc._panel_plan._ptr_subplaninfo[0]._pedred = 0;

        if(bSaveToSTC) {
          stc.Lock_to_Save_Plan_from_Panel();
          screenPlan.DisplayPlan(planInfo._planid, INVAILDVALUE);
        }

      }
  }
}
//---------------------------------------------------------------------------
void ScreenPlanEditF2::DisplayPlanIdAndPhase(void)
{
try {
    //PlanId
    lcd240x128.DISPLAY_GRAPHIC_XY(planIdAndPhase[0].X,planIdAndPhase[0].Y,word8x16[planInfo._planid/10],planIdAndPhase[0].height,planIdAndPhase[0].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planIdAndPhase[1].X,planIdAndPhase[1].Y,word8x16[planInfo._planid%10],planIdAndPhase[1].height,planIdAndPhase[1].width/8);
    //Phase
    lcd240x128.DISPLAY_GRAPHIC_XY(planIdAndPhase[2].X,planIdAndPhase[2].Y,word8x16[(planInfo._phase_order>>4)&0xFF],planIdAndPhase[2].height,planIdAndPhase[2].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planIdAndPhase[3].X,planIdAndPhase[3].Y,word8x16[planInfo._phase_order-((planInfo._phase_order>>4)&0xFF)*16],planIdAndPhase[3].height,planIdAndPhase[3].width/8);

    ucPhaseTemp[0] =  planInfo._phase_order/16;
    ucPhaseTemp[1] =  planInfo._phase_order%16;

  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenPlanEditF2::DisplayCycleAndOffset(void)
{
try {
    //cycle
    lcd240x128.DISPLAY_GRAPHIC_XY(cycleAndOffset[0].X,cycleAndOffset[0].Y,word8x16[planInfo._cycle_time/1000],cycleAndOffset[0].height,cycleAndOffset[0].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(cycleAndOffset[1].X,cycleAndOffset[1].Y,word8x16[(planInfo._cycle_time/100)%10],cycleAndOffset[1].height,cycleAndOffset[1].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(cycleAndOffset[2].X,cycleAndOffset[2].Y,word8x16[(planInfo._cycle_time/10)%10],cycleAndOffset[2].height,cycleAndOffset[2].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(cycleAndOffset[3].X,cycleAndOffset[3].Y,word8x16[planInfo._cycle_time%10],cycleAndOffset[3].height,cycleAndOffset[3].width/8);
    //offset
    lcd240x128.DISPLAY_GRAPHIC_XY(cycleAndOffset[4].X,cycleAndOffset[4].Y,word8x16[planInfo._offset/1000],cycleAndOffset[4].height,cycleAndOffset[4].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(cycleAndOffset[5].X,cycleAndOffset[5].Y,word8x16[(planInfo._offset/100)%10],cycleAndOffset[5].height,cycleAndOffset[5].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(cycleAndOffset[6].X,cycleAndOffset[6].Y,word8x16[(planInfo._offset/10)%10],cycleAndOffset[6].height,cycleAndOffset[6].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(cycleAndOffset[7].X,cycleAndOffset[7].Y,word8x16[planInfo._offset%10],cycleAndOffset[7].height,cycleAndOffset[7].width/8);

    OffsetTemp[0] =  planInfo._offset/1000;
    OffsetTemp[1] =  (planInfo._offset/100)%10;
    OffsetTemp[2] =  (planInfo._offset/10)%10;
    OffsetTemp[3] =  planInfo._offset%10;


  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenPlanEditF2::DisplayUpAndDownPage(int page)
{
try {
    unsigned char temp[16]={0};
    switch (page) {
        case 1:   //左上
        case 4:   //右上
          lcd240x128.DISPLAY_GRAPHIC_XY(224,22,temp,16,1);
          lcd240x128.DISPLAY_GRAPHIC_XY(224,70,arrow8x16[1],16,1);
        break;
        case 2:   //左中
        case 5:   //右中
          lcd240x128.DISPLAY_GRAPHIC_XY(224,22,arrow8x16[0],16,1);
          lcd240x128.DISPLAY_GRAPHIC_XY(224,70,arrow8x16[1],16,1);
        break;
        case 3:   //左下
        case 6:   //右下
          lcd240x128.DISPLAY_GRAPHIC_XY(224,22,arrow8x16[0],16,1);
          lcd240x128.DISPLAY_GRAPHIC_XY(224,70,temp,16,1);
        break;
        default:
        break;
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenPlanEditF2::DisplayLeftAndRightPage(int page)
{
try {
    unsigned char temp[16]={0};
    switch (page) {
        case 1:   //左上
        case 2:   //左中
        case 3:   //左下
          lcd240x128.DISPLAY_GRAPHIC_XY(224,38,arrow8x16[3],16,1);
          lcd240x128.DISPLAY_GRAPHIC_XY(224,54,temp,16,1);
        break;
        case 4:   //右上
        case 5:   //右中
        case 6:   //右下
          lcd240x128.DISPLAY_GRAPHIC_XY(224,38,temp,16,1);
          lcd240x128.DISPLAY_GRAPHIC_XY(224,54,arrow8x16[2],16,1);
        break;
        default:
        break;
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenPlanEditF2::DisplayBackGround(int page)
{
try {
    face=page;
    lcd240x128.DISPLAY_GRAPHIC(0,planBitmap[face-1],128,30);
    DisplayPlanIdAndPhase();
    DisplayCycleAndOffset();
  } catch (...){}
}
//---------------------------------------------------------------------------
void ScreenPlanEditF2::doKeyWork(BYTE key)
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
void ScreenPlanEditF2::doKey0Work(void)
{
  if(cSelPosition == 0) { ucPhaseTemp[cPosition-2] = 0; lcd240x128.DISPLAY_GRAPHIC_XY(planIdAndPhase[cPosition].X,planIdAndPhase[cPosition].Y,word8x16[0],planIdAndPhase[cPosition].height,planIdAndPhase[cPosition].width/8); doKeyRIGHTWork(); }
  else if(cSelPosition == 1) { planTableTemp[cPosition] =   0; lcd240x128.DISPLAY_GRAPHIC_XY(planTable[cPosition].X,planTable[cPosition].Y,word8x16[0],planTable[cPosition].height,planTable[cPosition].width/8); doKeyRIGHTWork(); }
  else if(cSelPosition == 2) { OffsetTemp[cPosition-4] = 0; lcd240x128.DISPLAY_GRAPHIC_XY(cycleAndOffset[cPosition].X,cycleAndOffset[cPosition].Y,word8x16[0],cycleAndOffset[cPosition].height,cycleAndOffset[cPosition].width/8); doKeyRIGHTWork(); }
}
//---------------------------------------------------------------------------
void ScreenPlanEditF2::doKey1Work(void)
{
  if(cSelPosition == 0) { ucPhaseTemp[cPosition-2] = 1; lcd240x128.DISPLAY_GRAPHIC_XY(planIdAndPhase[cPosition].X,planIdAndPhase[cPosition].Y,word8x16[1],planIdAndPhase[cPosition].height,planIdAndPhase[cPosition].width/8); doKeyRIGHTWork(); }
  else if(cSelPosition == 1) { planTableTemp[cPosition] =   1; lcd240x128.DISPLAY_GRAPHIC_XY(planTable[cPosition].X,planTable[cPosition].Y,word8x16[1],planTable[cPosition].height,planTable[cPosition].width/8); doKeyRIGHTWork(); }
  else if(cSelPosition == 2) { OffsetTemp[cPosition-4] = 1; lcd240x128.DISPLAY_GRAPHIC_XY(cycleAndOffset[cPosition].X,cycleAndOffset[cPosition].Y,word8x16[1],cycleAndOffset[cPosition].height,cycleAndOffset[cPosition].width/8); doKeyRIGHTWork(); }
}
//---------------------------------------------------------------------------
void ScreenPlanEditF2::doKey2Work(void)
{
  if(cSelPosition == 0) { ucPhaseTemp[cPosition-2] = 2; lcd240x128.DISPLAY_GRAPHIC_XY(planIdAndPhase[cPosition].X,planIdAndPhase[cPosition].Y,word8x16[2],planIdAndPhase[cPosition].height,planIdAndPhase[cPosition].width/8); doKeyRIGHTWork(); }
  else if(cSelPosition == 1) { planTableTemp[cPosition] =   2; lcd240x128.DISPLAY_GRAPHIC_XY(planTable[cPosition].X,planTable[cPosition].Y,word8x16[2],planTable[cPosition].height,planTable[cPosition].width/8); doKeyRIGHTWork(); }
  else if(cSelPosition == 2) { OffsetTemp[cPosition-4] = 2; lcd240x128.DISPLAY_GRAPHIC_XY(cycleAndOffset[cPosition].X,cycleAndOffset[cPosition].Y,word8x16[2],cycleAndOffset[cPosition].height,cycleAndOffset[cPosition].width/8); doKeyRIGHTWork(); }
}
//---------------------------------------------------------------------------
void ScreenPlanEditF2::doKey3Work(void)
{
  if(cSelPosition == 0) { ucPhaseTemp[cPosition-2] = 3; lcd240x128.DISPLAY_GRAPHIC_XY(planIdAndPhase[cPosition].X,planIdAndPhase[cPosition].Y,word8x16[3],planIdAndPhase[cPosition].height,planIdAndPhase[cPosition].width/8); doKeyRIGHTWork(); }
  else if(cSelPosition == 1) { planTableTemp[cPosition] =   3; lcd240x128.DISPLAY_GRAPHIC_XY(planTable[cPosition].X,planTable[cPosition].Y,word8x16[3],planTable[cPosition].height,planTable[cPosition].width/8); doKeyRIGHTWork(); }
  else if(cSelPosition == 2) { OffsetTemp[cPosition-4] = 3; lcd240x128.DISPLAY_GRAPHIC_XY(cycleAndOffset[cPosition].X,cycleAndOffset[cPosition].Y,word8x16[3],cycleAndOffset[cPosition].height,cycleAndOffset[cPosition].width/8); doKeyRIGHTWork(); }
}
//---------------------------------------------------------------------------
void ScreenPlanEditF2::doKey4Work(void)
{
  if(cSelPosition == 0) { ucPhaseTemp[cPosition-2] = 4; lcd240x128.DISPLAY_GRAPHIC_XY(planIdAndPhase[cPosition].X,planIdAndPhase[cPosition].Y,word8x16[4],planIdAndPhase[cPosition].height,planIdAndPhase[cPosition].width/8); doKeyRIGHTWork(); }
  else if(cSelPosition == 1) { planTableTemp[cPosition] =   4; lcd240x128.DISPLAY_GRAPHIC_XY(planTable[cPosition].X,planTable[cPosition].Y,word8x16[4],planTable[cPosition].height,planTable[cPosition].width/8); doKeyRIGHTWork(); }
  else if(cSelPosition == 2) { OffsetTemp[cPosition-4] = 4; lcd240x128.DISPLAY_GRAPHIC_XY(cycleAndOffset[cPosition].X,cycleAndOffset[cPosition].Y,word8x16[4],cycleAndOffset[cPosition].height,cycleAndOffset[cPosition].width/8); doKeyRIGHTWork(); }
}
//---------------------------------------------------------------------------
void ScreenPlanEditF2::doKey5Work(void)
{
  if(cSelPosition == 0) { ucPhaseTemp[cPosition-2] = 5; lcd240x128.DISPLAY_GRAPHIC_XY(planIdAndPhase[cPosition].X,planIdAndPhase[cPosition].Y,word8x16[5],planIdAndPhase[cPosition].height,planIdAndPhase[cPosition].width/8); doKeyRIGHTWork(); }
  else if(cSelPosition == 1) { planTableTemp[cPosition] =   5; lcd240x128.DISPLAY_GRAPHIC_XY(planTable[cPosition].X,planTable[cPosition].Y,word8x16[5],planTable[cPosition].height,planTable[cPosition].width/8); doKeyRIGHTWork(); }
  else if(cSelPosition == 2) { OffsetTemp[cPosition-4] = 5; lcd240x128.DISPLAY_GRAPHIC_XY(cycleAndOffset[cPosition].X,cycleAndOffset[cPosition].Y,word8x16[5],cycleAndOffset[cPosition].height,cycleAndOffset[cPosition].width/8); doKeyRIGHTWork(); }
}
//---------------------------------------------------------------------------
void ScreenPlanEditF2::doKey6Work(void)
{
  if(cSelPosition == 0) { ucPhaseTemp[cPosition-2] = 6; lcd240x128.DISPLAY_GRAPHIC_XY(planIdAndPhase[cPosition].X,planIdAndPhase[cPosition].Y,word8x16[6],planIdAndPhase[cPosition].height,planIdAndPhase[cPosition].width/8); doKeyRIGHTWork(); }
  else if(cSelPosition == 1) { planTableTemp[cPosition] =   6; lcd240x128.DISPLAY_GRAPHIC_XY(planTable[cPosition].X,planTable[cPosition].Y,word8x16[6],planTable[cPosition].height,planTable[cPosition].width/8); doKeyRIGHTWork(); }
  else if(cSelPosition == 2) { OffsetTemp[cPosition-4] = 6; lcd240x128.DISPLAY_GRAPHIC_XY(cycleAndOffset[cPosition].X,cycleAndOffset[cPosition].Y,word8x16[6],cycleAndOffset[cPosition].height,cycleAndOffset[cPosition].width/8); doKeyRIGHTWork(); }
}
//---------------------------------------------------------------------------
void ScreenPlanEditF2::doKey7Work(void)
{
  if(cSelPosition == 0) { ucPhaseTemp[cPosition-2] = 7; lcd240x128.DISPLAY_GRAPHIC_XY(planIdAndPhase[cPosition].X,planIdAndPhase[cPosition].Y,word8x16[7],planIdAndPhase[cPosition].height,planIdAndPhase[cPosition].width/8); doKeyRIGHTWork(); }
  else if(cSelPosition == 1) { planTableTemp[cPosition] =   7; lcd240x128.DISPLAY_GRAPHIC_XY(planTable[cPosition].X,planTable[cPosition].Y,word8x16[7],planTable[cPosition].height,planTable[cPosition].width/8); doKeyRIGHTWork(); }
  else if(cSelPosition == 2) { OffsetTemp[cPosition-4] = 7; lcd240x128.DISPLAY_GRAPHIC_XY(cycleAndOffset[cPosition].X,cycleAndOffset[cPosition].Y,word8x16[7],cycleAndOffset[cPosition].height,cycleAndOffset[cPosition].width/8); doKeyRIGHTWork(); }
}
//---------------------------------------------------------------------------
void ScreenPlanEditF2::doKey8Work(void)
{
  if(cSelPosition == 0) { ucPhaseTemp[cPosition-2] = 8; lcd240x128.DISPLAY_GRAPHIC_XY(planIdAndPhase[cPosition].X,planIdAndPhase[cPosition].Y,word8x16[8],planIdAndPhase[cPosition].height,planIdAndPhase[cPosition].width/8); doKeyRIGHTWork(); }
  else if(cSelPosition == 1) { planTableTemp[cPosition] =   8; lcd240x128.DISPLAY_GRAPHIC_XY(planTable[cPosition].X,planTable[cPosition].Y,word8x16[8],planTable[cPosition].height,planTable[cPosition].width/8); doKeyRIGHTWork(); }
  else if(cSelPosition == 2) { OffsetTemp[cPosition-4] = 8; lcd240x128.DISPLAY_GRAPHIC_XY(cycleAndOffset[cPosition].X,cycleAndOffset[cPosition].Y,word8x16[8],cycleAndOffset[cPosition].height,cycleAndOffset[cPosition].width/8); doKeyRIGHTWork(); }
}
//---------------------------------------------------------------------------
void ScreenPlanEditF2::doKey9Work(void)
{
  if(cSelPosition == 0) { ucPhaseTemp[cPosition-2] = 9; lcd240x128.DISPLAY_GRAPHIC_XY(planIdAndPhase[cPosition].X,planIdAndPhase[cPosition].Y,word8x16[9],planIdAndPhase[cPosition].height,planIdAndPhase[cPosition].width/8); doKeyRIGHTWork(); }
  else if(cSelPosition == 1) { planTableTemp[cPosition] =   9; lcd240x128.DISPLAY_GRAPHIC_XY(planTable[cPosition].X,planTable[cPosition].Y,word8x16[9],planTable[cPosition].height,planTable[cPosition].width/8); doKeyRIGHTWork(); }
  else if(cSelPosition == 2) { OffsetTemp[cPosition-4] = 9; lcd240x128.DISPLAY_GRAPHIC_XY(cycleAndOffset[cPosition].X,cycleAndOffset[cPosition].Y,word8x16[9],cycleAndOffset[cPosition].height,cycleAndOffset[cPosition].width/8); doKeyRIGHTWork(); }
}
//---------------------------------------------------------------------------
void ScreenPlanEditF2::doKeyAWork(void)
{

  unsigned short int usiCopyPlan = smem.vGetScreenCopyPlanID();

  if(cSelPosition == 0) { ucPhaseTemp[cPosition-2] = 0xA; lcd240x128.DISPLAY_GRAPHIC_XY(planIdAndPhase[cPosition].X,planIdAndPhase[cPosition].Y,word8x16[0xA],planIdAndPhase[cPosition].height,planIdAndPhase[cPosition].width/8); doKeyRIGHTWork(); }
  else {
      if(usiCopyPlan != INVAILDVALUE) {

      vAppendPlan();
      DisplayPlanTable(face);
      DisplayPlanIdAndPhase();  //DisplayPlanIdAndPhase();
      DisplayCycleAndOffset();  //DisplayCycleAndOffset();

      setCursor8x16(planIdAndPhase[cPosition].X,planIdAndPhase[cPosition].Y+16);
    }
  }



/*  if(cSelPosition == 0) { ucPhaseTemp[cPosition-2] = 0xA; lcd240x128.DISPLAY_GRAPHIC_XY(planIdAndPhase[cPosition].X,planIdAndPhase[cPosition].Y,word8x16[0xA],planIdAndPhase[cPosition].height,planIdAndPhase[cPosition].width/8); doKeyRIGHTWork(); } */
/*
  int iPlanidLess = planInfo._planid - 1;                                   //上一個Plan
  int iPlanNextDisplay = 999;

  for(int i = iPlanidLess; i >= 0; i--) {
    if( iPlanNextDisplay == 999  && smem.vGetTCPhasePlanSegTypeData(TC_Plan, i) == true)
      iPlanNextDisplay = i;
  }
  for(int i = 48; i >= iPlanidLess; i--) {
    if(  iPlanNextDisplay == 999 && smem.vGetTCPhasePlanSegTypeData(TC_Plan, i) == true )
      iPlanNextDisplay = i;
  }

  screenPlanEditF2.DisplayPlan(iPlanNextDisplay);                                 // >0，直接display
*/
}
//---------------------------------------------------------------------------
void ScreenPlanEditF2::doKeyBWork(void)
{
  if(cSelPosition == 0) { ucPhaseTemp[cPosition-2] = 0xB; lcd240x128.DISPLAY_GRAPHIC_XY(planIdAndPhase[cPosition].X,planIdAndPhase[cPosition].Y,word8x16[0xB],planIdAndPhase[cPosition].height,planIdAndPhase[cPosition].width/8); doKeyRIGHTWork(); }
}
//---------------------------------------------------------------------------
void ScreenPlanEditF2::doKeyCWork(void)
{
  if(cSelPosition == 0) { ucPhaseTemp[cPosition-2] = 0xC; lcd240x128.DISPLAY_GRAPHIC_XY(planIdAndPhase[cPosition].X,planIdAndPhase[cPosition].Y,word8x16[0xC],planIdAndPhase[cPosition].height,planIdAndPhase[cPosition].width/8); doKeyRIGHTWork(); }
  else {
    if (face!=1 && face!=2 && face!=3) {
      vTmpArrayToData();
      vSetSubphaseCount();
      DisplayPlanTable(face-3);
    }
  }
}
//---------------------------------------------------------------------------
void ScreenPlanEditF2::doKeyDWork(void)
{
  if(cSelPosition == 0) { ucPhaseTemp[cPosition-2] = 0xD; lcd240x128.DISPLAY_GRAPHIC_XY(planIdAndPhase[cPosition].X,planIdAndPhase[cPosition].Y,word8x16[0xD],planIdAndPhase[cPosition].height,planIdAndPhase[cPosition].width/8); doKeyRIGHTWork(); }
  else {
    if (face!=4 && face!=5 && face!=6) {
      vTmpArrayToData();
      vSetSubphaseCount();
      DisplayPlanTable(face+3);
    }
  }
}
//---------------------------------------------------------------------------
void ScreenPlanEditF2::doKeyEWork(void)
{
  if(cSelPosition == 0) { ucPhaseTemp[cPosition-2] = 0xE; lcd240x128.DISPLAY_GRAPHIC_XY(planIdAndPhase[cPosition].X,planIdAndPhase[cPosition].Y,word8x16[0xE],planIdAndPhase[cPosition].height,planIdAndPhase[cPosition].width/8); doKeyRIGHTWork(); }
  else {
    if (face!=1 && face!=4) {
      vTmpArrayToData();
      vSetSubphaseCount();
      DisplayPlanTable(face-1);
    }
  }

/*
  int iPlanidPlus = planInfo._planid + 1;                                   //下一個Plan
  int iPlanNextDisplay = 999;

  for(int i = iPlanidPlus; i <= 48; i++) {
  //OTMARKPRINTF  printf("i=%d, iPlanNextDisplay=%d, smem.vGetTCPhasePlanSegTypeData()=%d.\n", i, iPlanNextDisplay, smem.vGetTCPhasePlanSegTypeData(TC_Plan, i));
    if( iPlanNextDisplay == 999  && smem.vGetTCPhasePlanSegTypeData(TC_Plan, i) == true)
      iPlanNextDisplay = i;
  }
  for(int i = 0; i <= iPlanidPlus; i++) {
  //OTMARKPRINTF  printf("i=%d, iPlanNextDisplay=%d, smem.vGetTCPhasePlanSegTypeData()=%d.\n", i, iPlanNextDisplay, smem.vGetTCPhasePlanSegTypeData(TC_Plan, i));
    if(  iPlanNextDisplay == 999 && smem.vGetTCPhasePlanSegTypeData(TC_Plan, i) == true )
      iPlanNextDisplay = i;
  }
  screenPlanEditF2.DisplayPlan(iPlanNextDisplay);                                 // >0，直接display
*/

}
//---------------------------------------------------------------------------
void ScreenPlanEditF2::doKeyFWork(void)
{
  if(cSelPosition == 0) { ucPhaseTemp[cPosition-2] = 0xF; lcd240x128.DISPLAY_GRAPHIC_XY(planIdAndPhase[cPosition].X,planIdAndPhase[cPosition].Y,word8x16[0xF],planIdAndPhase[cPosition].height,planIdAndPhase[cPosition].width/8); doKeyRIGHTWork(); }
  else {
    if (face!=3 && face!=6) {

      if(cSelPosition == 1) {
        if(face == 1 || face == 4) {
          if(cPosition > 31) {
            cPosition -= 16;
          }
        }
      }

      vTmpArrayToData();
      vSetSubphaseCount();
      DisplayPlanTable(face+1);
    }
  }
}
//---------------------------------------------------------------------------
void ScreenPlanEditF2::doKeyF1Work(void)
{
    screenMain.DisplayMain();
}
//---------------------------------------------------------------------------
void ScreenPlanEditF2::doKeyF2Work(void)
{
//Removed     screenPlanEditStep1.DisplayplanEditStep1_keyPlanID();
}
//---------------------------------------------------------------------------
void ScreenPlanEditF2::doKeyF3Work(void)
{
  //    screenWeekDay.DisplayWeekDay();
      vTmpArrayToData();
      vSetSubphaseCount();
      SavePlan(true); //save then quit
      //OT Debug 951128
      smem.vSetVDUpdateDB(2,true);
      smem.vSetVDUpdateDB(3,true);

}
//---------------------------------------------------------------------------
void ScreenPlanEditF2::doKeyF4Work(void)
{
    screenPlan.DisplayPlan(planInfo._planid, INVAILDVALUE);
}
//---------------------------------------------------------------------------
void ScreenPlanEditF2::doKeyUPWork(void)
{
try {

  int iPlanTableHeight = 0;
  if(face == 1 || face == 4) iPlanTableHeight = 32;
  if(face == 2 || face == 3 || face == 5 || face == 6) iPlanTableHeight = 16;

  if(cSelPosition == 2) {
    clearCursor8x16(cycleAndOffset[cPosition].X,cycleAndOffset[cPosition].Y+16);
    cSelPosition = 1;
    cPosition = iPlanTableHeight;
    setCursor8x16(planTable[cPosition].X,planTable[cPosition].Y+16);
  } else if(cSelPosition == 1) {
    clearCursor8x16(planTable[cPosition].X,planTable[cPosition].Y+16);
    if(cPosition > 15) {
      cPosition -= 16;
      setCursor8x16(planTable[cPosition].X,planTable[cPosition].Y+16);
    }
    else {
      cSelPosition = 0;
      cPosition = 2;
      setCursor8x16(planIdAndPhase[cPosition].X,planIdAndPhase[cPosition].Y+16);
    }
  }
  else if(cSelPosition == 0) {
//    vGoToPhaseSel();
  }
} catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenPlanEditF2::doKeyDOWNWork(void)
{
try {

  int iPlanTableHeight = 0;
  if(face == 1 || face == 4) iPlanTableHeight = 32;
  if(face == 2 || face == 3 || face == 5 || face == 6) iPlanTableHeight = 16;

  if(cSelPosition == 0) {
    clearCursor8x16(planIdAndPhase[cPosition].X,planIdAndPhase[cPosition].Y+16);
    cSelPosition = 1;
    cPosition = 0;
    setCursor8x16(planTable[cPosition].X,planTable[cPosition].Y+16);

  } else if(cSelPosition == 1) {
    clearCursor8x16(planTable[cPosition].X,planTable[cPosition].Y+16);
    if(cPosition < iPlanTableHeight) {
      cPosition += 16;
      setCursor8x16(planTable[cPosition].X,planTable[cPosition].Y+16);
    }
    else {
      cSelPosition = 2;
      cPosition = 4;
      setCursor8x16(cycleAndOffset[cPosition].X,cycleAndOffset[cPosition].Y+16);
    }
  }

} catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenPlanEditF2::doKeyLEFTWork(void)
{
try {
//    if (face>=4) DisplayPlanTable(face-3);
    if(cSelPosition == 0) {
      if(cPosition > 2) { clearCursor8x16(planIdAndPhase[cPosition].X,planIdAndPhase[cPosition].Y+16);
                           cPosition--;
                           setCursor8x16(planIdAndPhase[cPosition].X,planIdAndPhase[cPosition].Y+16); }

      else               { clearCursor8x16(planIdAndPhase[cPosition].X,planIdAndPhase[cPosition].Y+16);
                           setCursor8x16(planIdAndPhase[cPosition].X,planIdAndPhase[cPosition].Y+16); }
    }
    else if(cSelPosition == 1) {

        if(cPosition > 0) {
                              clearCursor8x16(planTable[cPosition].X,planTable[cPosition].Y+16);
                              cPosition--;
                              setCursor8x16(planTable[cPosition].X,planTable[cPosition].Y+16);
                           }
        else               {
                              clearCursor8x16(planTable[cPosition].X,planTable[cPosition].Y+16);
                              cSelPosition = 0;
                              cPosition = 3;
                              setCursor8x16(planIdAndPhase[cPosition].X,planIdAndPhase[cPosition].Y+16);
                           }
      }
      else if(cSelPosition == 2) {
        if(cPosition > 4) {
                              clearCursor8x16(cycleAndOffset[cPosition].X,cycleAndOffset[cPosition].Y+16);
                              cPosition--;
                              setCursor8x16(cycleAndOffset[cPosition].X,cycleAndOffset[cPosition].Y+16);
                          }
        else {
                              clearCursor8x16(cycleAndOffset[cPosition].X,cycleAndOffset[cPosition].Y+16);
                              cSelPosition = 1;
                              if(face == 1 || face == 4) { cPosition = 47; }
                              else { cPosition = 31; }

                              setCursor8x16(planTable[cPosition].X,planTable[cPosition].Y+16);
        }

      }



  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenPlanEditF2::doKeyRIGHTWork(void)
{
try {
//    if (face<=3) DisplayPlanTable(face+3);
    if(cSelPosition == 0) {
      if(cPosition < 3) { clearCursor8x16(planIdAndPhase[cPosition].X,planIdAndPhase[cPosition].Y+16);
                           cPosition++;
                           setCursor8x16(planIdAndPhase[cPosition].X,planIdAndPhase[cPosition].Y+16); }

      else               { clearCursor8x16(planIdAndPhase[cPosition].X,planIdAndPhase[cPosition].Y+16);
                           cSelPosition = 1;
                           cPosition = 0;
                           setCursor8x16(planTable[cPosition].X,planTable[cPosition].Y+16);            }
    }
    else if(cSelPosition == 1) {
      if(face == 1 || face == 4) {
        if(cPosition < 47) {
                              clearCursor8x16(planTable[cPosition].X,planTable[cPosition].Y+16);
                              cPosition++;
                              setCursor8x16(planTable[cPosition].X,planTable[cPosition].Y+16);
                           }
        else               {
                              clearCursor8x16(planTable[cPosition].X,planTable[cPosition].Y+16);
                              cSelPosition = 2;
                              cPosition = 4;                                    //From 4 Start, don't edit Cycle
                              setCursor8x16(cycleAndOffset[cPosition].X,cycleAndOffset[cPosition].Y+16);
                           }

        } else { // face = 2 3 5 6
          if(cPosition < 31) {
                              clearCursor8x16(planTable[cPosition].X,planTable[cPosition].Y+16);
                              cPosition++;
                              setCursor8x16(planTable[cPosition].X,planTable[cPosition].Y+16);
                             }
          else               {
                              clearCursor8x16(planTable[cPosition].X,planTable[cPosition].Y+16);
                              cSelPosition = 2;
                              cPosition = 4;                                    //From 4 Start, don't edit Cycle
                              setCursor8x16(cycleAndOffset[cPosition].X,cycleAndOffset[cPosition].Y+16);
                             }

        }

      }
      else if(cSelPosition == 2) {
        if(cPosition < 7) {
                              clearCursor8x16(cycleAndOffset[cPosition].X,cycleAndOffset[cPosition].Y+16);
                              cPosition++;
                              setCursor8x16(cycleAndOffset[cPosition].X,cycleAndOffset[cPosition].Y+16);
                           }

      }


  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenPlanEditF2::doKeyEnterWork(void)
{
  if(cSelPosition == 0) {
    screenPhasePreview.vSetPreviewOrView(PreviewScreen);
    vGoToPhasePreview();
  }

}
//---------------------------------------------------------------------------
void ScreenPlanEditF2::DisplayGreenAllRedYellow(int page)
{
try {
    int temp=0;
    if (page==1) temp=0;
    else temp=4;
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[0].X,planTable[0].Y,word8x16[planInfo._ptr_subplaninfo[temp]._green/1000],planTable[0].height,planTable[0].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[1].X,planTable[1].Y,word8x16[(planInfo._ptr_subplaninfo[temp]._green/100)%10],planTable[1].height,planTable[1].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[2].X,planTable[2].Y,word8x16[(planInfo._ptr_subplaninfo[temp]._green/10)%10],planTable[2].height,planTable[2].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[3].X,planTable[3].Y,word8x16[planInfo._ptr_subplaninfo[temp]._green%10],planTable[3].height,planTable[3].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[4].X,planTable[4].Y,word8x16[planInfo._ptr_subplaninfo[temp+1]._green/1000],planTable[4].height,planTable[4].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[5].X,planTable[5].Y,word8x16[(planInfo._ptr_subplaninfo[temp+1]._green/100)%10],planTable[5].height,planTable[5].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[6].X,planTable[6].Y,word8x16[(planInfo._ptr_subplaninfo[temp+1]._green/10)%10],planTable[6].height,planTable[6].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[7].X,planTable[7].Y,word8x16[planInfo._ptr_subplaninfo[temp+1]._green%10],planTable[7].height,planTable[7].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[8].X,planTable[8].Y,word8x16[planInfo._ptr_subplaninfo[temp+2]._green/1000],planTable[8].height,planTable[8].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[9].X,planTable[9].Y,word8x16[(planInfo._ptr_subplaninfo[temp+2]._green/100)%10],planTable[9].height,planTable[9].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[10].X,planTable[10].Y,word8x16[(planInfo._ptr_subplaninfo[temp+2]._green/10)%10],planTable[10].height,planTable[10].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[11].X,planTable[11].Y,word8x16[planInfo._ptr_subplaninfo[temp+2]._green%10],planTable[11].height,planTable[11].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[12].X,planTable[12].Y,word8x16[planInfo._ptr_subplaninfo[temp+3]._green/1000],planTable[12].height,planTable[12].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[13].X,planTable[13].Y,word8x16[(planInfo._ptr_subplaninfo[temp+3]._green/100)%10],planTable[13].height,planTable[13].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[14].X,planTable[14].Y,word8x16[(planInfo._ptr_subplaninfo[temp+3]._green/10)%10],planTable[14].height,planTable[14].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[15].X,planTable[15].Y,word8x16[planInfo._ptr_subplaninfo[temp+3]._green%10],planTable[15].height,planTable[15].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[16].X,planTable[16].Y,word8x16[planInfo._ptr_subplaninfo[temp]._allred/1000],planTable[16].height,planTable[16].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[17].X,planTable[17].Y,word8x16[(planInfo._ptr_subplaninfo[temp]._allred/100)%10],planTable[17].height,planTable[17].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[18].X,planTable[18].Y,word8x16[(planInfo._ptr_subplaninfo[temp]._allred/10)%10],planTable[18].height,planTable[18].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[19].X,planTable[19].Y,word8x16[planInfo._ptr_subplaninfo[temp]._allred%10],planTable[19].height,planTable[19].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[20].X,planTable[20].Y,word8x16[planInfo._ptr_subplaninfo[temp+1]._allred/1000],planTable[20].height,planTable[20].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[21].X,planTable[21].Y,word8x16[(planInfo._ptr_subplaninfo[temp+1]._allred/100)%10],planTable[21].height,planTable[21].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[22].X,planTable[22].Y,word8x16[(planInfo._ptr_subplaninfo[temp+1]._allred/10)%10],planTable[22].height,planTable[22].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[23].X,planTable[23].Y,word8x16[planInfo._ptr_subplaninfo[temp+1]._allred%10],planTable[23].height,planTable[23].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[24].X,planTable[24].Y,word8x16[planInfo._ptr_subplaninfo[temp+2]._allred/1000],planTable[24].height,planTable[24].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[25].X,planTable[25].Y,word8x16[(planInfo._ptr_subplaninfo[temp+2]._allred/100)%10],planTable[25].height,planTable[25].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[26].X,planTable[26].Y,word8x16[(planInfo._ptr_subplaninfo[temp+2]._allred/10)%10],planTable[26].height,planTable[26].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[27].X,planTable[27].Y,word8x16[planInfo._ptr_subplaninfo[temp+2]._allred%10],planTable[27].height,planTable[27].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[28].X,planTable[28].Y,word8x16[planInfo._ptr_subplaninfo[temp+3]._allred/1000],planTable[28].height,planTable[28].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[29].X,planTable[29].Y,word8x16[(planInfo._ptr_subplaninfo[temp+3]._allred/100)%10],planTable[29].height,planTable[29].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[30].X,planTable[30].Y,word8x16[(planInfo._ptr_subplaninfo[temp+3]._allred/10)%10],planTable[30].height,planTable[30].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[31].X,planTable[31].Y,word8x16[planInfo._ptr_subplaninfo[temp+3]._allred%10],planTable[31].height,planTable[31].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[32].X,planTable[32].Y,word8x16[planInfo._ptr_subplaninfo[temp]._yellow/1000],planTable[32].height,planTable[32].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[33].X,planTable[33].Y,word8x16[(planInfo._ptr_subplaninfo[temp]._yellow/100)%10],planTable[33].height,planTable[33].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[34].X,planTable[34].Y,word8x16[(planInfo._ptr_subplaninfo[temp]._yellow/10)%10],planTable[34].height,planTable[34].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[35].X,planTable[35].Y,word8x16[planInfo._ptr_subplaninfo[temp]._yellow%10],planTable[35].height,planTable[35].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[36].X,planTable[36].Y,word8x16[planInfo._ptr_subplaninfo[temp+1]._yellow/1000],planTable[36].height,planTable[36].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[37].X,planTable[37].Y,word8x16[(planInfo._ptr_subplaninfo[temp+1]._yellow/100)%10],planTable[37].height,planTable[37].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[38].X,planTable[38].Y,word8x16[(planInfo._ptr_subplaninfo[temp+1]._yellow/10)%10],planTable[38].height,planTable[38].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[39].X,planTable[39].Y,word8x16[planInfo._ptr_subplaninfo[temp+1]._yellow%10],planTable[39].height,planTable[39].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[40].X,planTable[40].Y,word8x16[planInfo._ptr_subplaninfo[temp+2]._yellow/1000],planTable[40].height,planTable[40].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[41].X,planTable[41].Y,word8x16[(planInfo._ptr_subplaninfo[temp+2]._yellow/100)%10],planTable[41].height,planTable[41].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[42].X,planTable[42].Y,word8x16[(planInfo._ptr_subplaninfo[temp+2]._yellow/10)%10],planTable[42].height,planTable[42].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[43].X,planTable[43].Y,word8x16[planInfo._ptr_subplaninfo[temp+2]._yellow%10],planTable[43].height,planTable[43].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[44].X,planTable[44].Y,word8x16[planInfo._ptr_subplaninfo[temp+3]._yellow/1000],planTable[44].height,planTable[44].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[45].X,planTable[45].Y,word8x16[(planInfo._ptr_subplaninfo[temp+3]._yellow/100)%10],planTable[45].height,planTable[45].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[46].X,planTable[46].Y,word8x16[(planInfo._ptr_subplaninfo[temp+3]._yellow/10)%10],planTable[46].height,planTable[46].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[47].X,planTable[47].Y,word8x16[planInfo._ptr_subplaninfo[temp+3]._yellow%10],planTable[47].height,planTable[47].width/8);

    for(int i = 0; i < 4; i++) {
      planTableTemp[0+i*4] =  planInfo._ptr_subplaninfo[temp+i]._green/1000;
      planTableTemp[1+i*4] = (planInfo._ptr_subplaninfo[temp+i]._green/100)%10;
      planTableTemp[2+i*4] = (planInfo._ptr_subplaninfo[temp+i]._green/10)%10;
      planTableTemp[3+i*4] =  planInfo._ptr_subplaninfo[temp+i]._green%10;

      planTableTemp[16+i*4] =  planInfo._ptr_subplaninfo[temp+i]._allred/1000;
      planTableTemp[17+i*4] = (planInfo._ptr_subplaninfo[temp+i]._allred/100)%10;
      planTableTemp[18+i*4] = (planInfo._ptr_subplaninfo[temp+i]._allred/10)%10;
      planTableTemp[19+i*4] =  planInfo._ptr_subplaninfo[temp+i]._allred%10;

      planTableTemp[32+i*4] =  planInfo._ptr_subplaninfo[temp+i]._yellow/1000;
      planTableTemp[33+i*4] = (planInfo._ptr_subplaninfo[temp+i]._yellow/100)%10;
      planTableTemp[34+i*4] = (planInfo._ptr_subplaninfo[temp+i]._yellow/10)%10;
      planTableTemp[35+i*4] =  planInfo._ptr_subplaninfo[temp+i]._yellow%10;
    }

  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenPlanEditF2::DisplayPGFPR(int page)
{
try {
    int temp=0;
    if (page==2) temp=0;
    else temp=4;
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[0].X,planTable[0].Y,word8x16[planInfo._ptr_subplaninfo[temp]._pedgreen_flash/1000],planTable[0].height,planTable[0].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[1].X,planTable[1].Y,word8x16[(planInfo._ptr_subplaninfo[temp]._pedgreen_flash/100)%10],planTable[1].height,planTable[1].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[2].X,planTable[2].Y,word8x16[(planInfo._ptr_subplaninfo[temp]._pedgreen_flash/10)%10],planTable[2].height,planTable[2].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[3].X,planTable[3].Y,word8x16[planInfo._ptr_subplaninfo[temp]._pedgreen_flash%10],planTable[3].height,planTable[3].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[4].X,planTable[4].Y,word8x16[planInfo._ptr_subplaninfo[temp+1]._pedgreen_flash/1000],planTable[4].height,planTable[4].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[5].X,planTable[5].Y,word8x16[(planInfo._ptr_subplaninfo[temp+1]._pedgreen_flash/100)%10],planTable[5].height,planTable[5].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[6].X,planTable[6].Y,word8x16[(planInfo._ptr_subplaninfo[temp+1]._pedgreen_flash/10)%10],planTable[6].height,planTable[6].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[7].X,planTable[7].Y,word8x16[planInfo._ptr_subplaninfo[temp+1]._pedgreen_flash%10],planTable[7].height,planTable[7].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[8].X,planTable[8].Y,word8x16[planInfo._ptr_subplaninfo[temp+2]._pedgreen_flash/1000],planTable[8].height,planTable[8].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[9].X,planTable[9].Y,word8x16[(planInfo._ptr_subplaninfo[temp+2]._pedgreen_flash/100)%10],planTable[9].height,planTable[9].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[10].X,planTable[10].Y,word8x16[(planInfo._ptr_subplaninfo[temp+2]._pedgreen_flash/10)%10],planTable[10].height,planTable[10].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[11].X,planTable[11].Y,word8x16[planInfo._ptr_subplaninfo[temp+2]._pedgreen_flash%10],planTable[11].height,planTable[11].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[12].X,planTable[12].Y,word8x16[planInfo._ptr_subplaninfo[temp+3]._pedgreen_flash/1000],planTable[12].height,planTable[12].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[13].X,planTable[13].Y,word8x16[(planInfo._ptr_subplaninfo[temp+3]._pedgreen_flash/100)%10],planTable[13].height,planTable[13].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[14].X,planTable[14].Y,word8x16[(planInfo._ptr_subplaninfo[temp+3]._pedgreen_flash/10)%10],planTable[14].height,planTable[14].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[15].X,planTable[15].Y,word8x16[planInfo._ptr_subplaninfo[temp+3]._pedgreen_flash%10],planTable[15].height,planTable[15].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[16].X,planTable[16].Y,word8x16[planInfo._ptr_subplaninfo[temp]._pedred/1000],planTable[16].height,planTable[16].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[17].X,planTable[17].Y,word8x16[(planInfo._ptr_subplaninfo[temp]._pedred/100)%10],planTable[17].height,planTable[17].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[18].X,planTable[18].Y,word8x16[(planInfo._ptr_subplaninfo[temp]._pedred/10)%10],planTable[18].height,planTable[18].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[19].X,planTable[19].Y,word8x16[planInfo._ptr_subplaninfo[temp]._pedred%10],planTable[19].height,planTable[19].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[20].X,planTable[20].Y,word8x16[planInfo._ptr_subplaninfo[temp+1]._pedred/1000],planTable[20].height,planTable[20].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[21].X,planTable[21].Y,word8x16[(planInfo._ptr_subplaninfo[temp+1]._pedred/100)%10],planTable[21].height,planTable[21].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[22].X,planTable[22].Y,word8x16[(planInfo._ptr_subplaninfo[temp+1]._pedred/10)%10],planTable[22].height,planTable[22].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[23].X,planTable[23].Y,word8x16[planInfo._ptr_subplaninfo[temp+1]._pedred%10],planTable[23].height,planTable[23].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[24].X,planTable[24].Y,word8x16[planInfo._ptr_subplaninfo[temp+2]._pedred/1000],planTable[24].height,planTable[24].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[25].X,planTable[25].Y,word8x16[(planInfo._ptr_subplaninfo[temp+2]._pedred/100)%10],planTable[25].height,planTable[25].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[26].X,planTable[26].Y,word8x16[(planInfo._ptr_subplaninfo[temp+2]._pedred/10)%10],planTable[26].height,planTable[26].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[27].X,planTable[27].Y,word8x16[planInfo._ptr_subplaninfo[temp+2]._pedred%10],planTable[27].height,planTable[27].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[28].X,planTable[28].Y,word8x16[planInfo._ptr_subplaninfo[temp+3]._pedred/1000],planTable[28].height,planTable[28].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[29].X,planTable[29].Y,word8x16[(planInfo._ptr_subplaninfo[temp+3]._pedred/100)%10],planTable[29].height,planTable[29].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[30].X,planTable[30].Y,word8x16[(planInfo._ptr_subplaninfo[temp+3]._pedred/10)%10],planTable[30].height,planTable[30].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[31].X,planTable[31].Y,word8x16[planInfo._ptr_subplaninfo[temp+3]._pedred%10],planTable[31].height,planTable[31].width/8);

    for(int i = 0; i < 4; i++) {
      planTableTemp[0+i*4] =  planInfo._ptr_subplaninfo[temp+i]._pedgreen_flash/1000;
      planTableTemp[1+i*4] = (planInfo._ptr_subplaninfo[temp+i]._pedgreen_flash/100)%10;
      planTableTemp[2+i*4] = (planInfo._ptr_subplaninfo[temp+i]._pedgreen_flash/10)%10;
      planTableTemp[3+i*4] =  planInfo._ptr_subplaninfo[temp+i]._pedgreen_flash%10;

      planTableTemp[16+i*4] =  planInfo._ptr_subplaninfo[temp+i]._pedred/1000;
      planTableTemp[17+i*4] = (planInfo._ptr_subplaninfo[temp+i]._pedred/100)%10;
      planTableTemp[18+i*4] = (planInfo._ptr_subplaninfo[temp+i]._pedred/10)%10;
      planTableTemp[19+i*4] =  planInfo._ptr_subplaninfo[temp+i]._pedred%10;
    }

  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenPlanEditF2::DisplayMinmaxGreen(int page)
{
try {
    int temp=0;
    if (page==3) temp=0;
    else temp=4;
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[0].X,planTable[0].Y,word8x16[planInfo._ptr_subplaninfo[temp]._min_green/1000],planTable[0].height,planTable[0].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[1].X,planTable[1].Y,word8x16[(planInfo._ptr_subplaninfo[temp]._min_green/100)%10],planTable[1].height,planTable[1].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[2].X,planTable[2].Y,word8x16[(planInfo._ptr_subplaninfo[temp]._min_green/10)%10],planTable[2].height,planTable[2].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[3].X,planTable[3].Y,word8x16[planInfo._ptr_subplaninfo[temp]._min_green%10],planTable[3].height,planTable[3].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[4].X,planTable[4].Y,word8x16[planInfo._ptr_subplaninfo[temp+1]._min_green/1000],planTable[4].height,planTable[4].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[5].X,planTable[5].Y,word8x16[(planInfo._ptr_subplaninfo[temp+1]._min_green/100)%10],planTable[5].height,planTable[5].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[6].X,planTable[6].Y,word8x16[(planInfo._ptr_subplaninfo[temp+1]._min_green/10)%10],planTable[6].height,planTable[6].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[7].X,planTable[7].Y,word8x16[planInfo._ptr_subplaninfo[temp+1]._min_green%10],planTable[7].height,planTable[7].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[8].X,planTable[8].Y,word8x16[planInfo._ptr_subplaninfo[temp+2]._min_green/1000],planTable[8].height,planTable[8].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[9].X,planTable[9].Y,word8x16[(planInfo._ptr_subplaninfo[temp+2]._min_green/100)%10],planTable[9].height,planTable[9].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[10].X,planTable[10].Y,word8x16[(planInfo._ptr_subplaninfo[temp+2]._min_green/10)%10],planTable[10].height,planTable[10].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[11].X,planTable[11].Y,word8x16[planInfo._ptr_subplaninfo[temp+2]._min_green%10],planTable[11].height,planTable[11].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[12].X,planTable[12].Y,word8x16[planInfo._ptr_subplaninfo[temp+3]._min_green/1000],planTable[12].height,planTable[12].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[13].X,planTable[13].Y,word8x16[(planInfo._ptr_subplaninfo[temp+3]._min_green/100)%10],planTable[13].height,planTable[13].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[14].X,planTable[14].Y,word8x16[(planInfo._ptr_subplaninfo[temp+3]._min_green/10)%10],planTable[14].height,planTable[14].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[15].X,planTable[15].Y,word8x16[planInfo._ptr_subplaninfo[temp+3]._min_green%10],planTable[15].height,planTable[15].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[16].X,planTable[16].Y,word8x16[planInfo._ptr_subplaninfo[temp]._max_green/1000],planTable[16].height,planTable[16].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[17].X,planTable[17].Y,word8x16[(planInfo._ptr_subplaninfo[temp]._max_green/100)%10],planTable[17].height,planTable[17].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[18].X,planTable[18].Y,word8x16[(planInfo._ptr_subplaninfo[temp]._max_green/10)%10],planTable[18].height,planTable[18].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[19].X,planTable[19].Y,word8x16[planInfo._ptr_subplaninfo[temp]._max_green%10],planTable[19].height,planTable[19].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[20].X,planTable[20].Y,word8x16[planInfo._ptr_subplaninfo[temp+1]._max_green/1000],planTable[20].height,planTable[20].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[21].X,planTable[21].Y,word8x16[(planInfo._ptr_subplaninfo[temp+1]._max_green/100)%10],planTable[21].height,planTable[21].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[22].X,planTable[22].Y,word8x16[(planInfo._ptr_subplaninfo[temp+1]._max_green/10)%10],planTable[22].height,planTable[22].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[23].X,planTable[23].Y,word8x16[planInfo._ptr_subplaninfo[temp+1]._max_green%10],planTable[23].height,planTable[23].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[24].X,planTable[24].Y,word8x16[planInfo._ptr_subplaninfo[temp+2]._max_green/1000],planTable[24].height,planTable[24].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[25].X,planTable[25].Y,word8x16[(planInfo._ptr_subplaninfo[temp+2]._max_green/100)%10],planTable[25].height,planTable[25].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[26].X,planTable[26].Y,word8x16[(planInfo._ptr_subplaninfo[temp+2]._max_green/10)%10],planTable[26].height,planTable[26].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[27].X,planTable[27].Y,word8x16[planInfo._ptr_subplaninfo[temp+2]._max_green%10],planTable[27].height,planTable[27].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[28].X,planTable[28].Y,word8x16[planInfo._ptr_subplaninfo[temp+3]._max_green/1000],planTable[28].height,planTable[28].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[29].X,planTable[29].Y,word8x16[(planInfo._ptr_subplaninfo[temp+3]._max_green/100)%10],planTable[29].height,planTable[29].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[30].X,planTable[30].Y,word8x16[(planInfo._ptr_subplaninfo[temp+3]._max_green/10)%10],planTable[30].height,planTable[30].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(planTable[31].X,planTable[31].Y,word8x16[planInfo._ptr_subplaninfo[temp+3]._max_green%10],planTable[31].height,planTable[31].width/8);

    for(int i = 0; i < 4; i++) {
      planTableTemp[0+i*4] =  planInfo._ptr_subplaninfo[temp+i]._min_green/1000;
      planTableTemp[1+i*4] = (planInfo._ptr_subplaninfo[temp+i]._min_green/100)%10;
      planTableTemp[2+i*4] = (planInfo._ptr_subplaninfo[temp+i]._min_green/10)%10;
      planTableTemp[3+i*4] =  planInfo._ptr_subplaninfo[temp+i]._min_green%10;

      planTableTemp[16+i*4] =  planInfo._ptr_subplaninfo[temp+i]._max_green/1000;
      planTableTemp[17+i*4] = (planInfo._ptr_subplaninfo[temp+i]._max_green/100)%10;
      planTableTemp[18+i*4] = (planInfo._ptr_subplaninfo[temp+i]._max_green/10)%10;
      planTableTemp[19+i*4] =  planInfo._ptr_subplaninfo[temp+i]._max_green%10;
    }

  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenPlanEditF2::vTmpArrayToData(void)
{
try{
  printf("Go to vTmpArrayToData face:%d\n", face);

  if(face == 1) {
    for(int i = 0; i < 4; i++) {
      if(i < 4) {
        planInfo._ptr_subplaninfo[i]._green = 0;
        planInfo._ptr_subplaninfo[i]._green = planTableTemp[0+i*4] * 1000;
        planInfo._ptr_subplaninfo[i]._green += (planTableTemp[1+i*4] * 100);
        planInfo._ptr_subplaninfo[i]._green += (planTableTemp[2+i*4] * 10);
        planInfo._ptr_subplaninfo[i]._green += planTableTemp[3+i*4];

        printf("planInfo._ptr_subplaninfo[%d]._green: %d\n", i, planInfo._ptr_subplaninfo[i]._green);

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
      if(i+4 < 8) {
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
    printf("Go to face = 2\n planInfo._subphase_count = %d\n", planInfo._subphase_count);
    for(int i = 0; i < 4; i++) {
      if(i < 4) {
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

        printf("QQQ planInfo._ptr_subplaninfo[%d]._pedgreen_flash = %d\n", i, planInfo._ptr_subplaninfo[i]._pedgreen_flash);
      }
    }
  }
  else if(face == 5) {
    for(int i = 0; i < 4; i++) {
      if(i+4 < 8) {
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
      if(i < 4) {
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
      if(i+4 < 8) {
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

        if(planInfo._ptr_subplaninfo[4+i]._max_green == 0)
          planInfo._ptr_subplaninfo[4+i]._max_green = 255;                      //cheat

      }
    }
  }

  planInfo._offset = 0;
  planInfo._offset = OffsetTemp[0] * 1000;
  planInfo._offset += (OffsetTemp[1] * 100);
  planInfo._offset += (OffsetTemp[2] * 10);
  planInfo._offset += OffsetTemp[3];

  // cal. cycle

  planInfo._cycle_time = 0;
  for(int i = 0; i < 8; i++) {
    planInfo._cycle_time += planInfo._ptr_subplaninfo[i]._green;
    planInfo._cycle_time += planInfo._ptr_subplaninfo[i]._allred;
    planInfo._cycle_time += planInfo._ptr_subplaninfo[i]._yellow;
    planInfo._cycle_time += planInfo._ptr_subplaninfo[i]._pedgreen_flash;
    planInfo._cycle_time += planInfo._ptr_subplaninfo[i]._pedred;
  }

  planInfo._phase_order = ucPhaseTemp[0]*16 + ucPhaseTemp[1];


}catch(...){}
}
//---------------------------------------------------------------------------
bool ScreenPlanEditF2::vSetSubphaseCount(void)
{
try{
  int iTmp;
  int iSubPhaseCount;
  iSubPhaseCount = 0;
  for(int i = 0; i < 8; i++)
  {
    iTmp = 0;
    iTmp += planInfo._ptr_subplaninfo[i]._green;
//    iTmp +=  planInfo._ptr_subplaninfo[i]._yellow;
//    iTmp +=  planInfo._ptr_subplaninfo[i]._allred;
//    iTmp +=  planInfo._ptr_subplaninfo[i]._pedgreen_flash;
//    iTmp +=  planInfo._ptr_subplaninfo[i]._pedred;

    if(iTmp > 0) iSubPhaseCount++;

//printfMsg    printf("planInfo._ptr_subplaninfo[%d]._green: %d\n", i, planInfo._ptr_subplaninfo[i]._green);

  }
  planInfo._subphase_count = iSubPhaseCount;
//printfMsg  printf("iSubPhaseCount: %d\n", iSubPhaseCount);

}catch(...){}
}

//---------------------------------------------------------------------------
bool ScreenPlanEditF2::vGoToPhasePreview(void)
{
try{
  vTmpArrayToData();
  vSetSubphaseCount();
  SavePlan(false);
  screenPhasePreview.DisplayPhasePreview(planInfo._planid, planInfo._phase_order, true);
}catch(...){}
}

//---------------------------------------------------------------------------
bool ScreenPlanEditF2::vAppendPlan(void)
{
try{
  unsigned short int usiOrgPlanID = planInfo._planid;
  unsigned short int usiCopyPlanID = smem.vGetScreenCopyPlanID();

  if (stc.Lock_to_Load_Plan_for_Panel(usiCopyPlanID)) {
      planInfo._planid = usiOrgPlanID;
      planInfo._dir = stc._panel_plan._dir;
      planInfo._phase_order = stc._panel_plan._phase_order;
      planInfo._subphase_count = stc._panel_plan._subphase_count;
      planInfo._cycle_time = stc._panel_plan._cycle_time;
      planInfo._offset = stc._panel_plan._offset;
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
      planInfo._planid = usiOrgPlanID;
      planInfo._dir=0x01;
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
  return true;
}catch(...){}
}

