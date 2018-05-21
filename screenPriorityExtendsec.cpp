#include "screenPriorityExtendsec.h"
#include "LCD240x128.h"
#include "SCREENMain.h"
#include "screenPrioritymenu.h"
#include "CSTC.h"
#include "SMEM.h"

#include <stdio.h>
#include <string.h>
//---------------------------------------------------------------------------
ScreenPriorityExtendsec screenPriorityExtendsec;
//---------------------------------------------------------------------------
ScreenPriorityExtendsec::ScreenPriorityExtendsec(void)
{
try{
    loadBitmapFromFile();
    initDispWord();
 } catch (...) {}
}
//---------------------------------------------------------------------------
ScreenPriorityExtendsec::~ScreenPriorityExtendsec(void)
{
}
//---------------------------------------------------------------------------
void ScreenPriorityExtendsec::loadBitmapFromFile(void)
{
try {
    FILE *bitmap;
    bitmap=fopen("//cct//bitmap//backGround//TC5F//Extendsec.bit","rb");
    if (bitmap) {
        fread(BRTpriorityBitmap,3840,1,bitmap);
        fclose(bitmap);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenPriorityExtendsec::DisplayPriorityExtendsec(void)
{
try {
    smem.SetcFace(cBRTExtendsec);
    lcd240x128.DISPLAY_GRAPHIC(0,BRTpriorityBitmap,128,30);
    PlanPhase();
    information();

  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenPriorityExtendsec::doKeyWork(BYTE key)
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
void ScreenPriorityExtendsec::initDispWord(void)
{
try {
    for(int i=0; i<3; i++){
        planid[i].width=8; planid[i].height=16; planid[i].Y=24;
    }
    planid[0].X=48; planid[1].X=56; planid[2].X=64;

    for(int i=0; i<4; i++){
        subphase[i].width=8; subphase[i].height=16; subphase[i].Y=88;
    }
    subphase[0].X=152; subphase[1].X=160; subphase[2].X=176; subphase[3].X=184;

    for(int i=0; i<16; i++){
        eastPercentage[i].width=8; eastPercentage[i].height=16; eastPercentage[i].Y=56;
        westPercentage[i].width=8; westPercentage[i].height=16; westPercentage[i].Y=72;
    }
    eastPercentage[0].X=48; eastPercentage[1].X=56; eastPercentage[2].X=72; eastPercentage[3].X=80;
    eastPercentage[4].X=96; eastPercentage[5].X=104; eastPercentage[6].X=120; eastPercentage[7].X=128;
    eastPercentage[8].X=144; eastPercentage[9].X=152; eastPercentage[10].X=168; eastPercentage[11].X=176;
    eastPercentage[12].X=192; eastPercentage[13].X=200; eastPercentage[14].X=216; eastPercentage[15].X=224;
    westPercentage[0].X=48; westPercentage[1].X=56; westPercentage[2].X=72; westPercentage[3].X=80;
    westPercentage[4].X=96; westPercentage[5].X=104; westPercentage[6].X=120; westPercentage[7].X=128;
    westPercentage[8].X=144; westPercentage[9].X=152; westPercentage[10].X=168; westPercentage[11].X=176;
    westPercentage[12].X=192; westPercentage[13].X=200; westPercentage[14].X=216; westPercentage[15].X=224;

    for(int i=0; i<8; i++){
        eastsubphase[i].width=8; eastsubphase[i].height=16; eastsubphase[i].Y=96;
        westsubphase[i].width=8; westsubphase[i].height=16; westsubphase[i].Y=112;
    }
    eastsubphase[0].X=48; eastsubphase[1].X=56; eastsubphase[2].X=64; eastsubphase[3].X=72;
    eastsubphase[4].X=80; eastsubphase[5].X=88; eastsubphase[6].X=96; eastsubphase[7].X=104;
    westsubphase[0].X=48; westsubphase[1].X=56; westsubphase[2].X=64; westsubphase[3].X=72;
    westsubphase[4].X=80; westsubphase[5].X=88; westsubphase[6].X=96; westsubphase[7].X=104;

    onoff.width=8; onoff.height=16; onoff.X=152; onoff.Y=24;

  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenPriorityExtendsec::PlanPhase(void)
{
try{
    unsigned short int iCurrentPlanID   = stc.vGetUSIData(CSTC_exec_plan_plan_ID);
    unsigned short int iCurrentPhaseID  = stc.vGetUSIData(CSTC_exec_plan_phase_order);
    unsigned short int usi1, usi2, usi3;

    if(1) {
        usi1  = iCurrentPlanID / 100;
        usi2 = (iCurrentPlanID - usi1*100 ) / 10;
        usi3 = (iCurrentPlanID - usi1*100 - usi2*10) % 10;
        lcd240x128.DISPLAY_GRAPHIC_XY(planid[0].X,planid[0].Y,word8x16[usi1],planid[0].height,planid[0].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(planid[1].X,planid[1].Y,word8x16[usi2],planid[1].height,planid[1].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(planid[2].X,planid[2].Y,word8x16[usi3],planid[2].height,planid[2].width/8);
    }

    //stc.Lock_to_Load_Plan_for_Panel(iCurrentPlanID);
    stc.Lock_to_Load_Phase_for_Panel(iCurrentPhaseID);
    int iCurrentSubphase = stc.vGetUSIData(CSTC_exec_phase_current_subphase);
    int iCurrentSubphaseTotal = stc._panel_phase._subphase_count;
    int iHiTMP, iLowTMP;

    iCurrentSubphase++;                                                           //From 0 Start, For Human, plus 1
    if(iCurrentSubphase < 100) {
        iHiTMP  = iCurrentSubphase / 10;
        iLowTMP = iCurrentSubphase % 10;
        lcd240x128.DISPLAY_GRAPHIC_XY(subphase[0].X,subphase[0].Y,word8x16[iHiTMP],subphase[0].height,subphase[0].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(subphase[1].X,subphase[1].Y,word8x16[iLowTMP],subphase[1].height,subphase[1].width/8);
    }
    if(iCurrentSubphaseTotal < 100) {
        iHiTMP  = iCurrentSubphaseTotal / 10;
        iLowTMP = iCurrentSubphaseTotal % 10;
        lcd240x128.DISPLAY_GRAPHIC_XY(subphase[2].X,subphase[2].Y,word8x16[iHiTMP],subphase[2].height,subphase[2].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(subphase[3].X,subphase[3].Y,word8x16[iLowTMP],subphase[3].height,subphase[3].width/8);
    }

 } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenPriorityExtendsec::information(void)
{
try{
    CPlanInfo tmpplan = smem.Get_exeBF13();
    unsigned int tmp = tmpplan._BF13phasecount;
    unsigned int tmpPercentage[tmp*2];

    memset(tmpPercentage,'\0',sizeof(tmpPercentage));
    unsigned int k=0;
    for(int j=0; j<2; j++){
        for(int i=0; i<tmp; i++){
            tmpPercentage[k] = tmpplan._ptr_subplaninfo[i]._percentage[j];
            k++;
        }
    }

    unsigned int inum1, inum2,j=0;
    for(int i=0; i<tmp; i++){
        inum1 = tmpPercentage[i]/10; inum2 = tmpPercentage[i]%10;
        lcd240x128.DISPLAY_GRAPHIC_XY(eastPercentage[j].X,eastPercentage[j].Y,word8x16[inum1],eastPercentage[j].height,eastPercentage[j].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(eastPercentage[j+1].X,eastPercentage[j+1].Y,word8x16[inum2],eastPercentage[j+1].height,eastPercentage[j+1].width/8);
        j+=2;
    }
    j=0;
    for(int i=tmp; i<tmp*2; i++){
        inum1 = tmpPercentage[i]/10; inum2 = tmpPercentage[i]%10;
        lcd240x128.DISPLAY_GRAPHIC_XY(westPercentage[j].X,westPercentage[j].Y,word8x16[inum1],westPercentage[j].height,westPercentage[j].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(westPercentage[j+1].X,westPercentage[j+1].Y,word8x16[inum2],westPercentage[j+1].height,westPercentage[j+1].width/8);
        j+=2;
    }

    unsigned char east = tmpplan._east;
    unsigned char west = tmpplan._west;
    unsigned char tmpshift = 0x0;

    for(int i=0; i<tmp; i++){
        tmpshift = (east << (7-i)) & 0x80;
        if(tmpshift == 0x80)
            lcd240x128.DISPLAY_GRAPHIC_XY(eastsubphase[i].X,eastsubphase[i].Y,word8x16[1],eastsubphase[i].height,eastsubphase[i].width/8);
        else
            lcd240x128.DISPLAY_GRAPHIC_XY(eastsubphase[i].X,eastsubphase[i].Y,word8x16[0],eastsubphase[i].height,eastsubphase[i].width/8);
    }
    for(int i=0; i<tmp; i++){
        tmpshift = (west << (7-i)) & 0x80;
        if(tmpshift == 0x80)
            lcd240x128.DISPLAY_GRAPHIC_XY(westsubphase[i].X,westsubphase[i].Y,word8x16[1],westsubphase[i].height,westsubphase[i].width/8);
        else
            lcd240x128.DISPLAY_GRAPHIC_XY(westsubphase[i].X,westsubphase[i].Y,word8x16[0],westsubphase[i].height,westsubphase[i].width/8);
    }

    if(smem.GetBF11onoff() == 0){
        lcd240x128.DISPLAY_GRAPHIC_XY(onoff.X,onoff.Y,word8x16[0],onoff.height,onoff.width/8);
    } else if(smem.GetBF11onoff() == 1){
        lcd240x128.DISPLAY_GRAPHIC_XY(onoff.X,onoff.Y,word8x16[2],onoff.height,onoff.width/8);
    } else if(smem.GetDefiniteTimeonoff() == 1){
        lcd240x128.DISPLAY_GRAPHIC_XY(onoff.X,onoff.Y,word8x16[3],onoff.height,onoff.width/8);
    } else{
        lcd240x128.DISPLAY_GRAPHIC_XY(onoff.X,onoff.Y,word8x16[0],onoff.height,onoff.width/8);
    }

 } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenPriorityExtendsec::doKeyF1Work(void)
{
    screenMain.DisplayMain();
}
//---------------------------------------------------------------------------
void ScreenPriorityExtendsec::doKeyF4Work(void)
{
    //screenRunTableList.DisplayRunTableList();
    screenPrioritymenu.DisplayPrioritymenu();
}
