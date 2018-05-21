#include "screenPriority.h"
#include "LCD240x128.h"
#include "SCREENMain.h"
#include "screenPrioritymenu.h"
#include "CSTC.h"
#include "SMEM.h"

#include <stdio.h>
//---------------------------------------------------------------------------
ScreenPriority screenPriority;
//---------------------------------------------------------------------------
ScreenPriority::ScreenPriority(void)
{
try{
    loadBitmapFromFile();
    initDispWord();
 } catch (...) {}
}
//---------------------------------------------------------------------------
ScreenPriority::~ScreenPriority(void)
{
}
//---------------------------------------------------------------------------
void ScreenPriority::loadBitmapFromFile(void)
{
try {
    FILE *bitmap;
    bitmap=fopen("//cct//bitmap//backGround//TC5F//BRTpriority.bit","rb");
    if (bitmap) {
        fread(BRTpriorityBitmap,3840,1,bitmap);
        fclose(bitmap);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenPriority::DisplayPriority(void)
{
try {
    smem.SetcFace(cBRTPRIORITY);
    lcd240x128.DISPLAY_GRAPHIC(0,BRTpriorityBitmap,128,30);
    vRefreshCurrentStep();
    vRefreshStepSec();
    vPrioritytState();

  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenPriority::doKeyWork(BYTE key)
{
try {
    switch (key) {
        case 0x80:
        case 0x81:
        case 0x82:
        case 0x83:
        case 0x84:
        case 0x85:
        case 0x86:
        case 0x87:
        case 0x88:
        case 0x89:
        case 0x8A:
        case 0x8B:
        case 0x8C:
        case 0x8D:
        case 0x8E:
        case 0x8F:
        break;
        case 0x90:
          doKeyF1Work();
        break;
        case 0x91:
        case 0x92:
        break;
        case 0x93:
          doKeyF4Work();
        break;
        case 0x94:
        case 0x95:
        case 0x96:
        case 0x97:
        case 0x98:
        break;
        default:
        break;
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenPriority::initDispWord(void)
{
try {

    for(int i = 0; i < 4; i++) {
        markStep[i].width = 8;       markStep[i].height = 16;
        markStepSecCount[i].width = 8; markStepSecCount[i].height = 16;
    }

    for(int i=0; i < 3; i++){ startpriority[i].width = 8; startpriority[i].height = 16;}

    for(int i=0; i<8; i++){
        carposition[i].width = 8;   carposition[i].height = 16;
        Rcarposition[i].width = 8;  Rcarposition[i].height = 16;
    }

    for(int i=0; i<10; i++){ delay[i].width = 8; delay[i].height = 16; }

    for(int i = 0; i < 4; i++) { markStep[i].Y = 62;}
    markStep[0].X = 168; markStep[1].X = 176;
    markStep[2].X = 192; markStep[3].X = 200;

    for(int i = 0; i < 4; i++) { markStepSecCount[i].Y = 80;}
    markStepSecCount[0].X = 192; markStepSecCount[1].X = 200;
    markStepSecCount[2].X = 208; markStepSecCount[3].X = 216;

    for(int i = 0; i < 8; i++) { carposition[i].Y = 44; Rcarposition[i].Y = 44; }
    carposition[0].X = 16; carposition[1].X = 32; carposition[2].X = 48; carposition[3].X = 56;
    carposition[4].X = 64; carposition[5].X = 72; carposition[6].X = 80; carposition[7].X = 104;
    Rcarposition[6].X = 160; Rcarposition[5].X = 168; Rcarposition[4].X = 176; Rcarposition[3].X = 184;
    Rcarposition[2].X = 192; Rcarposition[1].X = 208; Rcarposition[0].X = 224; Rcarposition[7].X = 136;

    startpriority[0].X = 16; startpriority[0].Y = 64;
    startpriority[1].X = 16; startpriority[1].Y = 80;
    startpriority[2].X = 72; startpriority[2].Y = 80;

    for(int i=0; i<5; i++){ delay[i].Y = 96; }
    delay[0].X = 80; delay[1].X = 88; delay[2].X = 96; delay[3].X = 104; delay[4].X = 120;
    delay[5].X = 16; delay[5].Y = 112;
    for(int i=6; i<10; i++){ delay[i].Y = 112; }
    delay[6].X = 104; delay[7].X = 112; delay[8].X = 120; delay[9].X = 128;

  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenPriority::vRefreshCurrentStep(void)
{
try {

  int iCurrentSubphase = stc.vGetUSIData(CSTC_exec_phase_current_subphase);
  int iCurrentSubphaseTotal = stc._panel_phase._subphase_count;

  int iCurrentStep     = stc.vGetUSIData(CSTC_exec_phase_current_subphase_step);
  int iCurrentStepTotal = stc._panel_phase._total_step_count;

  //Fuck Chen Ming Zu
  iCurrentStep = iCurrentStep + ((iCurrentSubphase)*5);
  iCurrentStepTotal = iCurrentStepTotal*iCurrentSubphaseTotal;

  int iHiTMP, iLowTMP;

//--
  iCurrentStep++;                                        //From 0 Start, For Human, plus 1
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

} catch(...){}
}
//---------------------------------------------------------------------------
void ScreenPriority::vRefreshStepSec(void)
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
void ScreenPriority::vPrioritytState(void)
{
try{
    unsigned char displayPosition;
    unsigned int  ScheduleTH = smem.GetScheduleTHBF0D();
    unsigned int  DelayTime = smem.GetDelayTimeBF0D();
    unsigned int  d1,d2,d3,d4,s1,s2,s3,s4;

    vPrioritystateclear();

    if(smem.GetBF08Condition()==2 || smem.GetBF08Condition()==3){
    //if(smem.GetBF08Condition()==1){
        lcd240x128.DISPLAY_GRAPHIC_XY(startpriority[0].X,startpriority[0].Y,word8x16Light,startpriority[0].height,startpriority[0].width/8);
        if(smem.GetBF08Strategy()==1){
            lcd240x128.DISPLAY_GRAPHIC_XY(startpriority[1].X,startpriority[1].Y,word8x16Light,startpriority[1].height,startpriority[1].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(startpriority[2].X,startpriority[2].Y,word8x16Flash,startpriority[2].height,startpriority[2].width/8);
        } else if (smem.GetBF08Strategy()==2){
            lcd240x128.DISPLAY_GRAPHIC_XY(startpriority[1].X,startpriority[1].Y,word8x16Flash,startpriority[1].height,startpriority[1].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(startpriority[2].X,startpriority[2].Y,word8x16Light,startpriority[2].height,startpriority[2].width/8);
        }
    } else {
        lcd240x128.DISPLAY_GRAPHIC_XY(startpriority[0].X,startpriority[0].Y,word8x16Flash,startpriority[0].height,startpriority[0].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(startpriority[1].X,startpriority[1].Y,word8x16Flash,startpriority[1].height,startpriority[1].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(startpriority[2].X,startpriority[2].Y,word8x16Flash,startpriority[2].height,startpriority[2].width/8);
    }

    if(smem.GetBF0BCarposition() >= 1 && smem.GetBF0BCarposition() <= 8) {
        displayPosition = smem.GetBF0BCarposition() - 1;
        lcd240x128.DISPLAY_GRAPHIC_XY(carposition[displayPosition].X,carposition[displayPosition].Y,word8x16Light,carposition[displayPosition].height,carposition[displayPosition].width/8);
        //printf("1: %d, %d, %d\n", displayPosition, carposition[displayPosition].X, carposition[displayPosition].Y);
    }
    if(smem.GetBF0BRCarposition() >= 1 && smem.GetBF0BRCarposition() <= 8) {
        displayPosition = smem.GetBF0BRCarposition() - 1;
        lcd240x128.DISPLAY_GRAPHIC_XY(Rcarposition[displayPosition].X,Rcarposition[displayPosition].Y,word8x16Light,Rcarposition[displayPosition].height,Rcarposition[displayPosition].width/8);
        //printf("2: %d, %d, %d\n", displayPosition, Rcarposition[displayPosition].X, Rcarposition[displayPosition].Y);
    }

    d1 = DelayTime / 1000; d2 = (DelayTime - (d1*1000)) / 100;
    d3  = (DelayTime - (d2*100+d1*1000)) / 10; d4 = (DelayTime - (d3*10+d2*100+d1*1000));
    //printf("\n\n!! %d %d %d %d !!\n\n",d1,d2,d3,d4);
    lcd240x128.DISPLAY_GRAPHIC_XY(delay[0].X,delay[0].Y,word8x16[d1],delay[0].height,delay[0].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(delay[1].X,delay[1].Y,word8x16[d2],delay[1].height,delay[1].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(delay[2].X,delay[2].Y,word8x16[d3],delay[2].height,delay[2].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(delay[3].X,delay[3].Y,word8x16[d4],delay[3].height,delay[3].width/8);

    if(smem.GetStatusBF0D() == 1)
        lcd240x128.DISPLAY_GRAPHIC_XY(delay[4].X,delay[4].Y,word8x16[14],delay[4].height,delay[4].width/8);
    else if(smem.GetStatusBF0D() ==2)
        lcd240x128.DISPLAY_GRAPHIC_XY(delay[4].X,delay[4].Y,word8x16[13],delay[4].height,delay[4].width/8);

    if (smem.GetOperateConditionBF0D() == 1)
        lcd240x128.DISPLAY_GRAPHIC_XY(delay[5].X,delay[5].Y,word8x16Light,delay[5].height,delay[5].width/8);
    else
        lcd240x128.DISPLAY_GRAPHIC_XY(delay[5].X,delay[5].Y,word8x16Flash,delay[5].height,delay[5].width/8);

    s1 = ScheduleTH / 1000; s2 = (ScheduleTH - (s1*1000)) / 100;
    s3  = (ScheduleTH - (s2*100+s1*1000)) / 10; s4 = (ScheduleTH - (s3*10+s2*100+s1*1000));
    lcd240x128.DISPLAY_GRAPHIC_XY(delay[6].X,delay[6].Y,word8x16[s1],delay[6].height,delay[6].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(delay[7].X,delay[7].Y,word8x16[s2],delay[7].height,delay[7].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(delay[8].X,delay[8].Y,word8x16[s3],delay[8].height,delay[8].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(delay[9].X,delay[9].Y,word8x16[s4],delay[9].height,delay[9].width/8);

 } catch(...){}
}
//--------------------------------------------------------------
void ScreenPriority::vPrioritystateclear(void)
{
    for(unsigned int i = 0; i < 8; i++){
        lcd240x128.DISPLAY_GRAPHIC_XY(carposition[i].X,carposition[i].Y,word8x16Flash,carposition[i].height,carposition[i].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(Rcarposition[i].X,Rcarposition[i].Y,word8x16Flash,Rcarposition[i].height,Rcarposition[i].width/8);
    }
}
//---------------------------------------------------------------------------
void ScreenPriority::doKeyF1Work(void)
{
    screenMain.DisplayMain();
}
//---------------------------------------------------------------------------
void ScreenPriority::doKeyF4Work(void)
{
    //screenRunTableList.DisplayRunTableList();
    screenPrioritymenu.DisplayPrioritymenu();
}
