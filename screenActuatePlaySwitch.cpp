#include "screenActuatePlaySwitch.h"

#include "LCD240x128.h"
#include "SCREENMain.h"
#include "screenSegtype.h"
#include "SMEM.h"
#include "CSTC.h"
#include <stdio.h>
#include "ConfigFile.h"//arwen ++ 1000929
#include "screenSegmentVDPhaseMapTableEdit.h"//arwen ++ 1000929
ScreenActuatePlaySwitch screenActuatePlaySwitch;
//------------------------------------------------
ScreenActuatePlaySwitch::ScreenActuatePlaySwitch(void)
{
try {
    loadBitmapFromFile();
    initDispWord();
    cPosition=0;
    cSelect=0;
    cFace=1;
    usActplayNo=0;

  } catch (...) {}
}
ScreenActuatePlaySwitch::~ScreenActuatePlaySwitch(void)
{
try {

  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenActuatePlaySwitch::loadBitmapFromFile(void)
{
try {
    FILE *bitmap;
    bitmap=fopen("//cct//bitmap//backGround//actmenu//ActuatePlaySwitch.bit","rb");
    if (bitmap) {
        fread(actuatePlayBitmap,3840,1,bitmap);
        fclose(bitmap);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenActuatePlaySwitch::initDispWord()
{
try {
    cPlanNo.X = 216;
    cPlanNo.Y = 0;
    cPlanNo.width=8;
    cPlanNo.height=16;

    cActuateMode.X = 32;
    cActuateMode.Y = 16;
    cActuateMode.width=8;
    cActuateMode.height=16;



    for(int i= 0 ;i<2;i++){
        valueActionSec[i]=0;
    cActuateFlashPlan[i].X = 64 +(i*8);
    cActuateFlashPlan[i].Y = 80;
    cActuateFlashPlan[i].width=8;
    cActuateFlashPlan[i].height=16;

    cStartSec[i].X = 64+(i*8);
    cStartSec[i].Y = 96;
    cStartSec[i].width=8;
    cStartSec[i].height=16;
    }

    for (int i=0;i<8;i++) {
        valueSwitch[i]=0;
        positionSwitch[i].X=64+(i*24);
        positionSwitch[i].Y=48;
        positionSwitch[i].width=8;
        positionSwitch[i].height=16;

        valueExtendSec[i]=0;
        positionExtendSec[i].X=64+(i*24);
        positionExtendSec[i].Y=64;
        positionExtendSec[i].width=8;
        positionExtendSec[i].height=16;
    }

    valueCompensateMode = 0;
    cCompensateMode.X=64;
    cCompensateMode.Y=112;
    cCompensateMode.width=8;
    cCompensateMode.height=16;

  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenActuatePlaySwitch::DisplayActType(int PlanNo)
{
try {
    smem.SetcFace(cACTUATEPLAYSWITCH);
    lcd240x128.DISPLAY_GRAPHIC(0,actuatePlayBitmap,128,30);
    setCursor8x16(cActuateMode.X,cActuateMode.Y+16);
    cPosition=0;
    cSelect=0;
    usActplayNo=PlanNo;
    getsegmentActData();
    DisplayExtendValueAndSwitch();

  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenActuatePlaySwitch::DisplayExtendValueAndSwitch()
{
try{
	lcd240x128.DISPLAY_GRAPHIC_XY(cPlanNo.X,cPlanNo.Y,word8x16[usActplayNo],cPlanNo.height,cPlanNo.width/8);
	lcd240x128.DISPLAY_GRAPHIC_XY(cActuateMode.X,cActuateMode.Y,word8x16[usiActType],cActuateMode.height,cActuateMode.width/8);
	for(int i=0;i<8;i++){
	    lcd240x128.DISPLAY_GRAPHIC_XY(positionSwitch[i].X,positionSwitch[i].Y,word8x16[valueSwitch[i]],positionSwitch[i].height,positionSwitch[i].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(positionExtendSec[i].X,positionExtendSec[i].Y,word8x16[valueExtendSec[i]],positionExtendSec[i].height,positionExtendSec[i].width/8);
	}
	for(int i=0 ;i<2;i++){
	    lcd240x128.DISPLAY_GRAPHIC_XY(cActuateFlashPlan[i].X,cActuateFlashPlan[i].Y,word8x16[ucTMP_ActuatePlan[i]],cActuateFlashPlan[i].height,cActuateFlashPlan[i].width/8);
	    lcd240x128.DISPLAY_GRAPHIC_XY(cStartSec[i].X,cStartSec[i].Y,word8x16[valueActionSec[i]],cStartSec[i].height,cStartSec[i].width/8);
	}
    lcd240x128.DISPLAY_GRAPHIC_XY(cCompensateMode.X,cCompensateMode.Y,word8x16[usiCompensateMode],cCompensateMode.height,cCompensateMode.width/8);

} catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenActuatePlaySwitch::getsegmentActData()
{
	//---------------------------------------------------
		for(int i=0;i<8;i++){
            usiSwitch[i]=smem.getSegmentActVDPhaseTriggerSwitch(usActplayNo,i);
            valueSwitch[i]=usiSwitch[i];
            printf("Segment VDPhaseSwitch[%d][%d]=%d \n",usActplayNo,i,valueSwitch[i]);

            usiExtendSec[i]=smem.getSegmentActExtendSec(usActplayNo,i);
            valueExtendSec[i] = usiExtendSec[i];
            printf("Segment ExtendSec[%d][%d]=%d \n",usActplayNo,i,valueExtendSec[i]);
	    }
	    usiActType=smem.getSegmentActType(usActplayNo);

	    //usiActuatePlan = smem.vGetUSIData(TC_CCT_In_LongTanu_ActuateType_Switch);
	    usiActuatePlan = smem.getSegmentActPlan(usActplayNo);
        ucTMP_ActuatePlan[0] = (usiActuatePlan / 10) & 0xFF;
        ucTMP_ActuatePlan[1] = (usiActuatePlan % 10) & 0xFF;

        usiActionSec = smem.getSegmentZoneTimes(usActplayNo);
        valueActionSec[0] = (usiActionSec / 10) & 0xFF;
        valueActionSec[1] = (usiActionSec % 10) & 0xFF;

        usiCompensateMode = smem.getSegmentCompensateMode(usActplayNo);
        valueCompensateMode = usiCompensateMode;
	//---------------------------------------------------
}
//---------------------------------------------------------------------------
void ScreenActuatePlaySwitch::doKeyWork(BYTE key)                                        //¤w¹LÂo,¶i¨Óªº¬O0x80~0x98¤§¶¡
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
        case 0x8B:
        case 0x8C:
        case 0x8D:
        case 0x8E:
        case 0x8F:
        break;
        case 0x90://F1 ESC
          doKeyF1Work();
        break;
        case 0x91:
          doKeyF2Work();
        break;
        case 0x92:
        //OT Pass
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
        case 0x98://ENTER
          doKeyEnterWork();
        break;
        default:
        break;
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenActuatePlaySwitch::doKey0Work(void)
{
    if(cSelect==0){
        clearCursor8x16(cActuateMode.X,cActuateMode.Y+16);
        usiActType=0;
        cSelect=1;
        cPosition=0;
        lcd240x128.DISPLAY_GRAPHIC_XY(cActuateMode.X,cActuateMode.Y,word8x16[usiActType],cActuateMode.height,cActuateMode.width/8);
        setCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);
    }else if(cSelect==1){
        clearCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);
        valueSwitch[cPosition]=0;
        lcd240x128.DISPLAY_GRAPHIC_XY(positionSwitch[cPosition].X,positionSwitch[cPosition].Y,word8x16[valueSwitch[cPosition]],positionSwitch[cPosition].height,positionSwitch[cPosition].width/8);
		if(cPosition<7){
		    cPosition++;
		    setCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);
        }else{
            cSelect=2 ;
            cPosition=0;
            setCursor8x16(positionExtendSec[cPosition].X,positionExtendSec[cPosition].Y+16);
        }
    }else if(cSelect==2){
        clearCursor8x16(positionExtendSec[cPosition].X,positionExtendSec[cPosition].Y+16);
        valueExtendSec[cPosition]=0;
        lcd240x128.DISPLAY_GRAPHIC_XY(positionExtendSec[cPosition].X,positionExtendSec[cPosition].Y,word8x16[valueExtendSec[cPosition]],positionExtendSec[cPosition].height,positionExtendSec[cPosition].width/8);
		if(cPosition<7){
		    cPosition++;
		    setCursor8x16(positionExtendSec[cPosition].X,positionExtendSec[cPosition].Y+16);
        }else{
            cSelect=3 ;
            cPosition=0;
            setCursor8x16(cActuateFlashPlan[cPosition].X,cActuateFlashPlan[cPosition].Y+16);
        }
        //setCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);
    }else if(cSelect==3){
        clearCursor8x16(cActuateFlashPlan[cPosition].X,cActuateFlashPlan[cPosition].Y+16);
        ucTMP_ActuatePlan[cPosition]=0;
        //lcd240x128.DISPLAY_GRAPHIC_XY(cActuatePlan[0].X,cActuatePlan[0].Y,word8x16[ucTMP_ActuatePlan[0]],cActuatePlan[0].height,cActuatePlan[0].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(cActuateFlashPlan[cPosition].X,cActuateFlashPlan[cPosition].Y,word8x16[ucTMP_ActuatePlan[cPosition]],cActuateFlashPlan[cPosition].height,cActuateFlashPlan[cPosition].width/8);
        if(cPosition<1){
            cPosition++;
            setCursor8x16(cActuateFlashPlan[cPosition].X,cActuateFlashPlan[cPosition].Y+16);
        }else{
            cSelect=4 ;
            cPosition=0;
            setCursor8x16(cStartSec[cPosition].X,cStartSec[cPosition].Y+16);
        }
    }else if(cSelect==4){
        clearCursor8x16(cStartSec[cPosition].X,cStartSec[cPosition].Y+16);
        valueActionSec[cPosition]=0;
        //lcd240x128.DISPLAY_GRAPHIC_XY(cActuatePlan[0].X,cActuatePlan[0].Y,word8x16[ucTMP_ActuatePlan[0]],cActuatePlan[0].height,cActuatePlan[0].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(cStartSec[cPosition].X,cStartSec[cPosition].Y,word8x16[valueActionSec[cPosition]],cStartSec[cPosition].height,cStartSec[cPosition].width/8);
        if(cPosition<1){
            cPosition++;
            setCursor8x16(cStartSec[cPosition].X,cStartSec[cPosition].Y+16);
        }else{
            cSelect=5 ;
            cPosition=0;
            setCursor8x16(cCompensateMode.X,cCompensateMode.Y+16);
        }
    }else if(cSelect==5){
        clearCursor8x16(cCompensateMode.X,cCompensateMode.Y+16);
        valueCompensateMode = 0;
        //lcd240x128.DISPLAY_GRAPHIC_XY(cActuatePlan[0].X,cActuatePlan[0].Y,word8x16[ucTMP_ActuatePlan[0]],cActuatePlan[0].height,cActuatePlan[0].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(cCompensateMode.X,cCompensateMode.Y,word8x16[valueCompensateMode],cCompensateMode.height,cCompensateMode.width/8);
        cPosition = 0;
        setCursor8x16(cCompensateMode.X,cCompensateMode.Y+16);
    }
}
//---------------------------------------------------------------------------
void ScreenActuatePlaySwitch::doKey1Work(void)
{
    if(cSelect==0){
        clearCursor8x16(cActuateMode.X,cActuateMode.Y+16);
        usiActType=1;
        cSelect=1;
        cPosition=0;
        lcd240x128.DISPLAY_GRAPHIC_XY(cActuateMode.X,cActuateMode.Y,word8x16[usiActType],cActuateMode.height,cActuateMode.width/8);
        setCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);
    }else if(cSelect==1){
        clearCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);
        valueSwitch[cPosition]=1;
        lcd240x128.DISPLAY_GRAPHIC_XY(positionSwitch[cPosition].X,positionSwitch[cPosition].Y,word8x16[valueSwitch[cPosition]],positionSwitch[cPosition].height,positionSwitch[cPosition].width/8);
		if(cPosition<7){
		    cPosition++;
		    setCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);
        }else{
            cSelect=2 ;
            cPosition=0;
            setCursor8x16(positionExtendSec[cPosition].X,positionExtendSec[cPosition].Y+16);
        }
    }else if(cSelect==2){
        clearCursor8x16(positionExtendSec[cPosition].X,positionExtendSec[cPosition].Y+16);
        valueExtendSec[cPosition]=1;
        lcd240x128.DISPLAY_GRAPHIC_XY(positionExtendSec[cPosition].X,positionExtendSec[cPosition].Y,word8x16[valueExtendSec[cPosition]],positionExtendSec[cPosition].height,positionExtendSec[cPosition].width/8);
		if(cPosition<7){
		    cPosition++;
		    setCursor8x16(positionExtendSec[cPosition].X,positionExtendSec[cPosition].Y+16);
        }else{
            cSelect=3 ;
            cPosition=0;
            setCursor8x16(cActuateFlashPlan[cPosition].X,cActuateFlashPlan[cPosition].Y+16);
        }
        //setCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);
    }else if(cSelect==3){
        clearCursor8x16(cActuateFlashPlan[cPosition].X,cActuateFlashPlan[cPosition].Y+16);
        ucTMP_ActuatePlan[cPosition]=1;
        //lcd240x128.DISPLAY_GRAPHIC_XY(cActuatePlan[0].X,cActuatePlan[0].Y,word8x16[ucTMP_ActuatePlan[0]],cActuatePlan[0].height,cActuatePlan[0].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(cActuateFlashPlan[cPosition].X,cActuateFlashPlan[cPosition].Y,word8x16[ucTMP_ActuatePlan[cPosition]],cActuateFlashPlan[cPosition].height,cActuateFlashPlan[cPosition].width/8);
        if(cPosition<1){
            cPosition++;
            setCursor8x16(cActuateFlashPlan[cPosition].X,cActuateFlashPlan[cPosition].Y+16);
        }else{
            cSelect=4 ;
            cPosition=0;
            setCursor8x16(cStartSec[cPosition].X,cStartSec[cPosition].Y+16);
        }
    }else if(cSelect==4){
        clearCursor8x16(cStartSec[cPosition].X,cStartSec[cPosition].Y+16);
        valueActionSec[cPosition]=1;
        //lcd240x128.DISPLAY_GRAPHIC_XY(cActuatePlan[0].X,cActuatePlan[0].Y,word8x16[ucTMP_ActuatePlan[0]],cActuatePlan[0].height,cActuatePlan[0].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(cStartSec[cPosition].X,cStartSec[cPosition].Y,word8x16[valueActionSec[cPosition]],cStartSec[cPosition].height,cStartSec[cPosition].width/8);
        if(cPosition<1){
            cPosition++;
            setCursor8x16(cStartSec[cPosition].X,cStartSec[cPosition].Y+16);
        }else{
            cSelect=5 ;
            cPosition=0;
            setCursor8x16(cCompensateMode.X,cCompensateMode.Y+16);
        }
    }else if(cSelect==5){
        clearCursor8x16(cCompensateMode.X,cCompensateMode.Y+16);
        valueCompensateMode = 1;
        //lcd240x128.DISPLAY_GRAPHIC_XY(cActuatePlan[0].X,cActuatePlan[0].Y,word8x16[ucTMP_ActuatePlan[0]],cActuatePlan[0].height,cActuatePlan[0].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(cCompensateMode.X,cCompensateMode.Y,word8x16[valueCompensateMode],cCompensateMode.height,cCompensateMode.width/8);
        cPosition = 0;
        setCursor8x16(cCompensateMode.X,cCompensateMode.Y+16);
    }
}
//---------------------------------------------------------------------------
void ScreenActuatePlaySwitch::doKey2Work(void)
{
    if(cSelect==0){
        clearCursor8x16(cActuateMode.X,cActuateMode.Y+16);
        usiActType=2;
        cSelect=1;
        cPosition=0;
        lcd240x128.DISPLAY_GRAPHIC_XY(cActuateMode.X,cActuateMode.Y,word8x16[usiActType],cActuateMode.height,cActuateMode.width/8);
        setCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);
    }else if(cSelect==1){
        clearCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);
        valueSwitch[cPosition]=2;
        lcd240x128.DISPLAY_GRAPHIC_XY(positionSwitch[cPosition].X,positionSwitch[cPosition].Y,word8x16[valueSwitch[cPosition]],positionSwitch[cPosition].height,positionSwitch[cPosition].width/8);
		if(cPosition<7){
		    cPosition++;
		    setCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);
        }else{
            cSelect=2 ;
            cPosition=0;
            setCursor8x16(positionExtendSec[cPosition].X,positionExtendSec[cPosition].Y+16);
        }
    }else if(cSelect==2){
        clearCursor8x16(positionExtendSec[cPosition].X,positionExtendSec[cPosition].Y+16);
        valueExtendSec[cPosition]=2;
        lcd240x128.DISPLAY_GRAPHIC_XY(positionExtendSec[cPosition].X,positionExtendSec[cPosition].Y,word8x16[valueExtendSec[cPosition]],positionExtendSec[cPosition].height,positionExtendSec[cPosition].width/8);
		if(cPosition<7){
		    cPosition++;
		    setCursor8x16(positionExtendSec[cPosition].X,positionExtendSec[cPosition].Y+16);
        }else{
            cSelect=3 ;
            cPosition=0;
            setCursor8x16(cActuateFlashPlan[cPosition].X,cActuateFlashPlan[cPosition].Y+16);
        }
        //setCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);
    }else if(cSelect==3){
        clearCursor8x16(cActuateFlashPlan[cPosition].X,cActuateFlashPlan[cPosition].Y+16);
        ucTMP_ActuatePlan[cPosition]=2;
        //lcd240x128.DISPLAY_GRAPHIC_XY(cActuatePlan[0].X,cActuatePlan[0].Y,word8x16[ucTMP_ActuatePlan[0]],cActuatePlan[0].height,cActuatePlan[0].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(cActuateFlashPlan[cPosition].X,cActuateFlashPlan[cPosition].Y,word8x16[ucTMP_ActuatePlan[cPosition]],cActuateFlashPlan[cPosition].height,cActuateFlashPlan[cPosition].width/8);
        if(cPosition<1){
            cPosition++;
            setCursor8x16(cActuateFlashPlan[cPosition].X,cActuateFlashPlan[cPosition].Y+16);
        }else{
            cSelect=4 ;
            cPosition=0;
            setCursor8x16(cStartSec[cPosition].X,cStartSec[cPosition].Y+16);
        }
    }else if(cSelect==4){
        clearCursor8x16(cStartSec[cPosition].X,cStartSec[cPosition].Y+16);
        valueActionSec[cPosition]=2;
        //lcd240x128.DISPLAY_GRAPHIC_XY(cActuatePlan[0].X,cActuatePlan[0].Y,word8x16[ucTMP_ActuatePlan[0]],cActuatePlan[0].height,cActuatePlan[0].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(cStartSec[cPosition].X,cStartSec[cPosition].Y,word8x16[valueActionSec[cPosition]],cStartSec[cPosition].height,cStartSec[cPosition].width/8);
        if(cPosition<1){
            cPosition++;
            setCursor8x16(cStartSec[cPosition].X,cStartSec[cPosition].Y+16);
        }else{
            cSelect=5 ;
            cPosition=0;
            setCursor8x16(cCompensateMode.X,cCompensateMode.Y+16);
        }
    }else if(cSelect==5){
        clearCursor8x16(cCompensateMode.X,cCompensateMode.Y+16);
        valueCompensateMode = 2;
        //lcd240x128.DISPLAY_GRAPHIC_XY(cActuatePlan[0].X,cActuatePlan[0].Y,word8x16[ucTMP_ActuatePlan[0]],cActuatePlan[0].height,cActuatePlan[0].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(cCompensateMode.X,cCompensateMode.Y,word8x16[valueCompensateMode],cCompensateMode.height,cCompensateMode.width/8);
        cPosition = 0;
        setCursor8x16(cCompensateMode.X,cCompensateMode.Y+16);
    }
}
//---------------------------------------------------------------------------
void ScreenActuatePlaySwitch::doKey3Work(void)
{
    if(cSelect==0){
        clearCursor8x16(cActuateMode.X,cActuateMode.Y+16);
        usiActType=3;
        cSelect=1;
        cPosition=0;
        lcd240x128.DISPLAY_GRAPHIC_XY(cActuateMode.X,cActuateMode.Y,word8x16[usiActType],cActuateMode.height,cActuateMode.width/8);
        setCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);
    }else if(cSelect==1){
        clearCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);
        valueSwitch[cPosition]=3;
        lcd240x128.DISPLAY_GRAPHIC_XY(positionSwitch[cPosition].X,positionSwitch[cPosition].Y,word8x16[valueSwitch[cPosition]],positionSwitch[cPosition].height,positionSwitch[cPosition].width/8);
		if(cPosition<7){
		    cPosition++;
		    setCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);
        }else{
            cSelect=2 ;
            cPosition=0;
            setCursor8x16(positionExtendSec[cPosition].X,positionExtendSec[cPosition].Y+16);
        }
    }else if(cSelect==2){
        clearCursor8x16(positionExtendSec[cPosition].X,positionExtendSec[cPosition].Y+16);
        valueExtendSec[cPosition]=3;
        lcd240x128.DISPLAY_GRAPHIC_XY(positionExtendSec[cPosition].X,positionExtendSec[cPosition].Y,word8x16[valueExtendSec[cPosition]],positionExtendSec[cPosition].height,positionExtendSec[cPosition].width/8);
		if(cPosition<7){
		    cPosition++;
		    setCursor8x16(positionExtendSec[cPosition].X,positionExtendSec[cPosition].Y+16);
        }else{
            cSelect=3 ;
            cPosition=0;
            setCursor8x16(cActuateFlashPlan[cPosition].X,cActuateFlashPlan[cPosition].Y+16);
        }
        //setCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);
    }else if(cSelect==3){
        clearCursor8x16(cActuateFlashPlan[cPosition].X,cActuateFlashPlan[cPosition].Y+16);
        ucTMP_ActuatePlan[cPosition]=3;
        //lcd240x128.DISPLAY_GRAPHIC_XY(cActuatePlan[0].X,cActuatePlan[0].Y,word8x16[ucTMP_ActuatePlan[0]],cActuatePlan[0].height,cActuatePlan[0].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(cActuateFlashPlan[cPosition].X,cActuateFlashPlan[cPosition].Y,word8x16[ucTMP_ActuatePlan[cPosition]],cActuateFlashPlan[cPosition].height,cActuateFlashPlan[cPosition].width/8);
        if(cPosition<1){
            cPosition++;
            setCursor8x16(cActuateFlashPlan[cPosition].X,cActuateFlashPlan[cPosition].Y+16);
        }else{
            cSelect=4 ;
            cPosition=0;
            setCursor8x16(cStartSec[cPosition].X,cStartSec[cPosition].Y+16);
        }
    }else if(cSelect==4){
        clearCursor8x16(cStartSec[cPosition].X,cStartSec[cPosition].Y+16);
        valueActionSec[cPosition]=3;
        //lcd240x128.DISPLAY_GRAPHIC_XY(cActuatePlan[0].X,cActuatePlan[0].Y,word8x16[ucTMP_ActuatePlan[0]],cActuatePlan[0].height,cActuatePlan[0].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(cStartSec[cPosition].X,cStartSec[cPosition].Y,word8x16[valueActionSec[cPosition]],cStartSec[cPosition].height,cStartSec[cPosition].width/8);
        if(cPosition<1){
            cPosition++;
            setCursor8x16(cStartSec[cPosition].X,cStartSec[cPosition].Y+16);
        }else{
            cSelect=5 ;
            cPosition=0;
            setCursor8x16(cCompensateMode.X,cCompensateMode.Y+16);
        }
    }else if(cSelect==5){
        clearCursor8x16(cCompensateMode.X,cCompensateMode.Y+16);
        valueCompensateMode = 3;
        //lcd240x128.DISPLAY_GRAPHIC_XY(cActuatePlan[0].X,cActuatePlan[0].Y,word8x16[ucTMP_ActuatePlan[0]],cActuatePlan[0].height,cActuatePlan[0].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(cCompensateMode.X,cCompensateMode.Y,word8x16[valueCompensateMode],cCompensateMode.height,cCompensateMode.width/8);
        cPosition = 0;
        setCursor8x16(cCompensateMode.X,cCompensateMode.Y+16);
    }
}
//---------------------------------------------------------------------------
void ScreenActuatePlaySwitch::doKey4Work(void)
{
    if(cSelect==0){
        clearCursor8x16(cActuateMode.X,cActuateMode.Y+16);
        usiActType=4;
        cSelect=1;
        cPosition=0;
        lcd240x128.DISPLAY_GRAPHIC_XY(cActuateMode.X,cActuateMode.Y,word8x16[usiActType],cActuateMode.height,cActuateMode.width/8);
        setCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);
    }else if(cSelect==1){
        clearCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);
        valueSwitch[cPosition]=4;
        lcd240x128.DISPLAY_GRAPHIC_XY(positionSwitch[cPosition].X,positionSwitch[cPosition].Y,word8x16[valueSwitch[cPosition]],positionSwitch[cPosition].height,positionSwitch[cPosition].width/8);
		if(cPosition<7){
		    cPosition++;
		    setCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);
        }else{
            cSelect=2 ;
            cPosition=0;
            setCursor8x16(positionExtendSec[cPosition].X,positionExtendSec[cPosition].Y+16);
        }
    }else if(cSelect==2){
        clearCursor8x16(positionExtendSec[cPosition].X,positionExtendSec[cPosition].Y+16);
        valueExtendSec[cPosition]=4;
        lcd240x128.DISPLAY_GRAPHIC_XY(positionExtendSec[cPosition].X,positionExtendSec[cPosition].Y,word8x16[valueExtendSec[cPosition]],positionExtendSec[cPosition].height,positionExtendSec[cPosition].width/8);
		if(cPosition<7){
		    cPosition++;
		    setCursor8x16(positionExtendSec[cPosition].X,positionExtendSec[cPosition].Y+16);
        }else{
            cSelect=3 ;
            cPosition=0;
            setCursor8x16(cActuateFlashPlan[cPosition].X,cActuateFlashPlan[cPosition].Y+16);
        }
        //setCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);
    }else if(cSelect==3){
        clearCursor8x16(cActuateFlashPlan[cPosition].X,cActuateFlashPlan[cPosition].Y+16);
        ucTMP_ActuatePlan[cPosition]=4;
        //lcd240x128.DISPLAY_GRAPHIC_XY(cActuatePlan[0].X,cActuatePlan[0].Y,word8x16[ucTMP_ActuatePlan[0]],cActuatePlan[0].height,cActuatePlan[0].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(cActuateFlashPlan[cPosition].X,cActuateFlashPlan[cPosition].Y,word8x16[ucTMP_ActuatePlan[cPosition]],cActuateFlashPlan[cPosition].height,cActuateFlashPlan[cPosition].width/8);
        if(cPosition<1){
            cPosition++;
            setCursor8x16(cActuateFlashPlan[cPosition].X,cActuateFlashPlan[cPosition].Y+16);
        }else{
            cSelect=4 ;
            cPosition=0;
            setCursor8x16(cStartSec[cPosition].X,cStartSec[cPosition].Y+16);
        }
    }else if(cSelect==4){
        clearCursor8x16(cStartSec[cPosition].X,cStartSec[cPosition].Y+16);
        valueActionSec[cPosition]=4;
        //lcd240x128.DISPLAY_GRAPHIC_XY(cActuatePlan[0].X,cActuatePlan[0].Y,word8x16[ucTMP_ActuatePlan[0]],cActuatePlan[0].height,cActuatePlan[0].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(cStartSec[cPosition].X,cStartSec[cPosition].Y,word8x16[valueActionSec[cPosition]],cStartSec[cPosition].height,cStartSec[cPosition].width/8);
        if(cPosition<1){
            cPosition++;
            setCursor8x16(cStartSec[cPosition].X,cStartSec[cPosition].Y+16);
        }else{
            cSelect=5 ;
            cPosition=0;
            setCursor8x16(cCompensateMode.X,cCompensateMode.Y+16);
        }
    }else if(cSelect==5){
        clearCursor8x16(cCompensateMode.X,cCompensateMode.Y+16);
        valueCompensateMode = 4;
        //lcd240x128.DISPLAY_GRAPHIC_XY(cActuatePlan[0].X,cActuatePlan[0].Y,word8x16[ucTMP_ActuatePlan[0]],cActuatePlan[0].height,cActuatePlan[0].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(cCompensateMode.X,cCompensateMode.Y,word8x16[valueCompensateMode],cCompensateMode.height,cCompensateMode.width/8);
        cPosition = 0;
        setCursor8x16(cCompensateMode.X,cCompensateMode.Y+16);
    }
}
//---------------------------------------------------------------------------
void ScreenActuatePlaySwitch::doKey5Work(void)
{
    if(cSelect==0){
        clearCursor8x16(cActuateMode.X,cActuateMode.Y+16);
        usiActType=5;
        cSelect=1;
        cPosition=0;
        lcd240x128.DISPLAY_GRAPHIC_XY(cActuateMode.X,cActuateMode.Y,word8x16[usiActType],cActuateMode.height,cActuateMode.width/8);
        setCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);
    }else if(cSelect==1){
        clearCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);
        valueSwitch[cPosition]=5;
        lcd240x128.DISPLAY_GRAPHIC_XY(positionSwitch[cPosition].X,positionSwitch[cPosition].Y,word8x16[valueSwitch[cPosition]],positionSwitch[cPosition].height,positionSwitch[cPosition].width/8);
		if(cPosition<7){
		    cPosition++;
		    setCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);
        }else{
            cSelect=2 ;
            cPosition=0;
            setCursor8x16(positionExtendSec[cPosition].X,positionExtendSec[cPosition].Y+16);
        }
    }else if(cSelect==2){
        clearCursor8x16(positionExtendSec[cPosition].X,positionExtendSec[cPosition].Y+16);
        valueExtendSec[cPosition]=5;
        lcd240x128.DISPLAY_GRAPHIC_XY(positionExtendSec[cPosition].X,positionExtendSec[cPosition].Y,word8x16[valueExtendSec[cPosition]],positionExtendSec[cPosition].height,positionExtendSec[cPosition].width/8);
		if(cPosition<7){
		    cPosition++;
		    setCursor8x16(positionExtendSec[cPosition].X,positionExtendSec[cPosition].Y+16);
        }else{
            cSelect=3 ;
            cPosition=0;
            setCursor8x16(cActuateFlashPlan[cPosition].X,cActuateFlashPlan[cPosition].Y+16);
        }
        //setCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);
    }else if(cSelect==3){
        clearCursor8x16(cActuateFlashPlan[cPosition].X,cActuateFlashPlan[cPosition].Y+16);
        ucTMP_ActuatePlan[cPosition]=5;
        //lcd240x128.DISPLAY_GRAPHIC_XY(cActuatePlan[0].X,cActuatePlan[0].Y,word8x16[ucTMP_ActuatePlan[0]],cActuatePlan[0].height,cActuatePlan[0].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(cActuateFlashPlan[cPosition].X,cActuateFlashPlan[cPosition].Y,word8x16[ucTMP_ActuatePlan[cPosition]],cActuateFlashPlan[cPosition].height,cActuateFlashPlan[cPosition].width/8);
        if(cPosition<1){
            cPosition++;
            setCursor8x16(cActuateFlashPlan[cPosition].X,cActuateFlashPlan[cPosition].Y+16);
        }else{
            cSelect=4 ;
            cPosition=0;
            setCursor8x16(cStartSec[cPosition].X,cStartSec[cPosition].Y+16);
        }
    }else if(cSelect==4){
        clearCursor8x16(cStartSec[cPosition].X,cStartSec[cPosition].Y+16);
        valueActionSec[cPosition]=5;
        //lcd240x128.DISPLAY_GRAPHIC_XY(cActuatePlan[0].X,cActuatePlan[0].Y,word8x16[ucTMP_ActuatePlan[0]],cActuatePlan[0].height,cActuatePlan[0].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(cStartSec[cPosition].X,cStartSec[cPosition].Y,word8x16[valueActionSec[cPosition]],cStartSec[cPosition].height,cStartSec[cPosition].width/8);
        if(cPosition<1){
            cPosition++;
            setCursor8x16(cStartSec[cPosition].X,cStartSec[cPosition].Y+16);
        }else{
            cSelect=5 ;
            cPosition=0;
            setCursor8x16(cCompensateMode.X,cCompensateMode.Y+16);
        }
    }else if(cSelect==5){
        clearCursor8x16(cCompensateMode.X,cCompensateMode.Y+16);
        valueCompensateMode = 5;
        //lcd240x128.DISPLAY_GRAPHIC_XY(cActuatePlan[0].X,cActuatePlan[0].Y,word8x16[ucTMP_ActuatePlan[0]],cActuatePlan[0].height,cActuatePlan[0].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(cCompensateMode.X,cCompensateMode.Y,word8x16[valueCompensateMode],cCompensateMode.height,cCompensateMode.width/8);
        cPosition = 0;
        setCursor8x16(cCompensateMode.X,cCompensateMode.Y+16);
    }
}
//---------------------------------------------------------------------------
void ScreenActuatePlaySwitch::doKey6Work(void)
{
    if(cSelect==0){
        clearCursor8x16(cActuateMode.X,cActuateMode.Y+16);
        usiActType=6;
        cSelect=1;
        cPosition=0;
        lcd240x128.DISPLAY_GRAPHIC_XY(cActuateMode.X,cActuateMode.Y,word8x16[usiActType],cActuateMode.height,cActuateMode.width/8);
        setCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);
    }else if(cSelect==1){
        clearCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);
        valueSwitch[cPosition]=6;
        lcd240x128.DISPLAY_GRAPHIC_XY(positionSwitch[cPosition].X,positionSwitch[cPosition].Y,word8x16[valueSwitch[cPosition]],positionSwitch[cPosition].height,positionSwitch[cPosition].width/8);
		if(cPosition<7){
		    cPosition++;
		    setCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);
        }else{
            cSelect=2 ;
            cPosition=0;
            setCursor8x16(positionExtendSec[cPosition].X,positionExtendSec[cPosition].Y+16);
        }
    }else if(cSelect==2){
        clearCursor8x16(positionExtendSec[cPosition].X,positionExtendSec[cPosition].Y+16);
        valueExtendSec[cPosition]=6;
        lcd240x128.DISPLAY_GRAPHIC_XY(positionExtendSec[cPosition].X,positionExtendSec[cPosition].Y,word8x16[valueExtendSec[cPosition]],positionExtendSec[cPosition].height,positionExtendSec[cPosition].width/8);
		if(cPosition<7){
		    cPosition++;
		    setCursor8x16(positionExtendSec[cPosition].X,positionExtendSec[cPosition].Y+16);
        }else{
            cSelect=3 ;
            cPosition=0;
            setCursor8x16(cActuateFlashPlan[cPosition].X,cActuateFlashPlan[cPosition].Y+16);
        }
        //setCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);
    }else if(cSelect==3){
        clearCursor8x16(cActuateFlashPlan[cPosition].X,cActuateFlashPlan[cPosition].Y+16);
        ucTMP_ActuatePlan[cPosition]=6;
        //lcd240x128.DISPLAY_GRAPHIC_XY(cActuatePlan[0].X,cActuatePlan[0].Y,word8x16[ucTMP_ActuatePlan[0]],cActuatePlan[0].height,cActuatePlan[0].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(cActuateFlashPlan[cPosition].X,cActuateFlashPlan[cPosition].Y,word8x16[ucTMP_ActuatePlan[cPosition]],cActuateFlashPlan[cPosition].height,cActuateFlashPlan[cPosition].width/8);
        if(cPosition<1){
            cPosition++;
            setCursor8x16(cActuateFlashPlan[cPosition].X,cActuateFlashPlan[cPosition].Y+16);
        }else{
            cSelect=4 ;
            cPosition=0;
            setCursor8x16(cStartSec[cPosition].X,cStartSec[cPosition].Y+16);
        }
    }else if(cSelect==4){
        clearCursor8x16(cStartSec[cPosition].X,cStartSec[cPosition].Y+16);
        valueActionSec[cPosition]=6;
        //lcd240x128.DISPLAY_GRAPHIC_XY(cActuatePlan[0].X,cActuatePlan[0].Y,word8x16[ucTMP_ActuatePlan[0]],cActuatePlan[0].height,cActuatePlan[0].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(cStartSec[cPosition].X,cStartSec[cPosition].Y,word8x16[valueActionSec[cPosition]],cStartSec[cPosition].height,cStartSec[cPosition].width/8);
        if(cPosition<1){
            cPosition++;
            setCursor8x16(cStartSec[cPosition].X,cStartSec[cPosition].Y+16);
        }else{
            cSelect=5 ;
            cPosition=0;
            setCursor8x16(cCompensateMode.X,cCompensateMode.Y+16);
        }
    }else if(cSelect==5){
        clearCursor8x16(cCompensateMode.X,cCompensateMode.Y+16);
        valueCompensateMode = 6;
        //lcd240x128.DISPLAY_GRAPHIC_XY(cActuatePlan[0].X,cActuatePlan[0].Y,word8x16[ucTMP_ActuatePlan[0]],cActuatePlan[0].height,cActuatePlan[0].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(cCompensateMode.X,cCompensateMode.Y,word8x16[valueCompensateMode],cCompensateMode.height,cCompensateMode.width/8);
        cPosition = 0;
        setCursor8x16(cCompensateMode.X,cCompensateMode.Y+16);
    }
}
//---------------------------------------------------------------------------
void ScreenActuatePlaySwitch::doKey7Work(void)
{
    if(cSelect==0){
        clearCursor8x16(cActuateMode.X,cActuateMode.Y+16);
        usiActType=7;
        cSelect=1;
        cPosition=0;
        lcd240x128.DISPLAY_GRAPHIC_XY(cActuateMode.X,cActuateMode.Y,word8x16[usiActType],cActuateMode.height,cActuateMode.width/8);
        setCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);
    }else if(cSelect==1){
        clearCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);
        valueSwitch[cPosition]=7;
        lcd240x128.DISPLAY_GRAPHIC_XY(positionSwitch[cPosition].X,positionSwitch[cPosition].Y,word8x16[valueSwitch[cPosition]],positionSwitch[cPosition].height,positionSwitch[cPosition].width/8);
		if(cPosition<7){
		    cPosition++;
		    setCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);
        }else{
            cSelect=2 ;
            cPosition=0;
            setCursor8x16(positionExtendSec[cPosition].X,positionExtendSec[cPosition].Y+16);
        }
    }else if(cSelect==2){
        clearCursor8x16(positionExtendSec[cPosition].X,positionExtendSec[cPosition].Y+16);
        valueExtendSec[cPosition]=7;
        lcd240x128.DISPLAY_GRAPHIC_XY(positionExtendSec[cPosition].X,positionExtendSec[cPosition].Y,word8x16[valueExtendSec[cPosition]],positionExtendSec[cPosition].height,positionExtendSec[cPosition].width/8);
		if(cPosition<7){
		    cPosition++;
		    setCursor8x16(positionExtendSec[cPosition].X,positionExtendSec[cPosition].Y+16);
        }else{
            cSelect=3 ;
            cPosition=0;
            setCursor8x16(cActuateFlashPlan[cPosition].X,cActuateFlashPlan[cPosition].Y+16);
        }
        //setCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);
    }else if(cSelect==3){
        clearCursor8x16(cActuateFlashPlan[cPosition].X,cActuateFlashPlan[cPosition].Y+16);
        ucTMP_ActuatePlan[cPosition]=7;
        //lcd240x128.DISPLAY_GRAPHIC_XY(cActuatePlan[0].X,cActuatePlan[0].Y,word8x16[ucTMP_ActuatePlan[0]],cActuatePlan[0].height,cActuatePlan[0].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(cActuateFlashPlan[cPosition].X,cActuateFlashPlan[cPosition].Y,word8x16[ucTMP_ActuatePlan[cPosition]],cActuateFlashPlan[cPosition].height,cActuateFlashPlan[cPosition].width/8);
        if(cPosition<1){
            cPosition++;
            setCursor8x16(cActuateFlashPlan[cPosition].X,cActuateFlashPlan[cPosition].Y+16);
        }else{
            cSelect=4 ;
            cPosition=0;
            setCursor8x16(cStartSec[cPosition].X,cStartSec[cPosition].Y+16);
        }
    }else if(cSelect==4){
        clearCursor8x16(cStartSec[cPosition].X,cStartSec[cPosition].Y+16);
        valueActionSec[cPosition]=7;
        //lcd240x128.DISPLAY_GRAPHIC_XY(cActuatePlan[0].X,cActuatePlan[0].Y,word8x16[ucTMP_ActuatePlan[0]],cActuatePlan[0].height,cActuatePlan[0].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(cStartSec[cPosition].X,cStartSec[cPosition].Y,word8x16[valueActionSec[cPosition]],cStartSec[cPosition].height,cStartSec[cPosition].width/8);
        if(cPosition<1){
            cPosition++;
            setCursor8x16(cStartSec[cPosition].X,cStartSec[cPosition].Y+16);
        }else{
            cSelect=5 ;
            cPosition=0;
            setCursor8x16(cCompensateMode.X,cCompensateMode.Y+16);
        }
    }else if(cSelect==5){
        clearCursor8x16(cCompensateMode.X,cCompensateMode.Y+16);
        valueCompensateMode = 7;
        //lcd240x128.DISPLAY_GRAPHIC_XY(cActuatePlan[0].X,cActuatePlan[0].Y,word8x16[ucTMP_ActuatePlan[0]],cActuatePlan[0].height,cActuatePlan[0].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(cCompensateMode.X,cCompensateMode.Y,word8x16[valueCompensateMode],cCompensateMode.height,cCompensateMode.width/8);
        cPosition = 0;
        setCursor8x16(cCompensateMode.X,cCompensateMode.Y+16);
    }
}
//---------------------------------------------------------------------------
void ScreenActuatePlaySwitch::doKey8Work(void)
{
    if(cSelect==0){
        clearCursor8x16(cActuateMode.X,cActuateMode.Y+16);
        usiActType=8;
        cSelect=1;
        cPosition=0;
        lcd240x128.DISPLAY_GRAPHIC_XY(cActuateMode.X,cActuateMode.Y,word8x16[usiActType],cActuateMode.height,cActuateMode.width/8);
        setCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);
    }else if(cSelect==1){
        clearCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);
        valueSwitch[cPosition]=8;
        lcd240x128.DISPLAY_GRAPHIC_XY(positionSwitch[cPosition].X,positionSwitch[cPosition].Y,word8x16[valueSwitch[cPosition]],positionSwitch[cPosition].height,positionSwitch[cPosition].width/8);
		if(cPosition<7){
		    cPosition++;
		    setCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);
        }else{
            cSelect=2 ;
            cPosition=0;
            setCursor8x16(positionExtendSec[cPosition].X,positionExtendSec[cPosition].Y+16);
        }
    }else if(cSelect==2){
        clearCursor8x16(positionExtendSec[cPosition].X,positionExtendSec[cPosition].Y+16);
        valueExtendSec[cPosition]=8;
        lcd240x128.DISPLAY_GRAPHIC_XY(positionExtendSec[cPosition].X,positionExtendSec[cPosition].Y,word8x16[valueExtendSec[cPosition]],positionExtendSec[cPosition].height,positionExtendSec[cPosition].width/8);
		if(cPosition<7){
		    cPosition++;
		    setCursor8x16(positionExtendSec[cPosition].X,positionExtendSec[cPosition].Y+16);
        }else{
            cSelect=3 ;
            cPosition=0;
            setCursor8x16(cActuateFlashPlan[cPosition].X,cActuateFlashPlan[cPosition].Y+16);
        }
        //setCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);
    }else if(cSelect==3){
        clearCursor8x16(cActuateFlashPlan[cPosition].X,cActuateFlashPlan[cPosition].Y+16);
        ucTMP_ActuatePlan[cPosition]=8;
        //lcd240x128.DISPLAY_GRAPHIC_XY(cActuatePlan[0].X,cActuatePlan[0].Y,word8x16[ucTMP_ActuatePlan[0]],cActuatePlan[0].height,cActuatePlan[0].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(cActuateFlashPlan[cPosition].X,cActuateFlashPlan[cPosition].Y,word8x16[ucTMP_ActuatePlan[cPosition]],cActuateFlashPlan[cPosition].height,cActuateFlashPlan[cPosition].width/8);
        if(cPosition<1){
            cPosition++;
            setCursor8x16(cActuateFlashPlan[cPosition].X,cActuateFlashPlan[cPosition].Y+16);
        }else{
            cSelect=4 ;
            cPosition=0;
            setCursor8x16(cStartSec[cPosition].X,cStartSec[cPosition].Y+16);
        }
    }else if(cSelect==4){
        clearCursor8x16(cStartSec[cPosition].X,cStartSec[cPosition].Y+16);
        valueActionSec[cPosition]=8;
        //lcd240x128.DISPLAY_GRAPHIC_XY(cActuatePlan[0].X,cActuatePlan[0].Y,word8x16[ucTMP_ActuatePlan[0]],cActuatePlan[0].height,cActuatePlan[0].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(cStartSec[cPosition].X,cStartSec[cPosition].Y,word8x16[valueActionSec[cPosition]],cStartSec[cPosition].height,cStartSec[cPosition].width/8);
        if(cPosition<1){
            cPosition++;
            setCursor8x16(cStartSec[cPosition].X,cStartSec[cPosition].Y+16);
        }else{
            cSelect=5 ;
            cPosition=0;
            setCursor8x16(cCompensateMode.X,cCompensateMode.Y+16);
        }
    }else if(cSelect==5){
        clearCursor8x16(cCompensateMode.X,cCompensateMode.Y+16);
        valueCompensateMode = 8;
        //lcd240x128.DISPLAY_GRAPHIC_XY(cActuatePlan[0].X,cActuatePlan[0].Y,word8x16[ucTMP_ActuatePlan[0]],cActuatePlan[0].height,cActuatePlan[0].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(cCompensateMode.X,cCompensateMode.Y,word8x16[valueCompensateMode],cCompensateMode.height,cCompensateMode.width/8);
        cPosition = 0;
        setCursor8x16(cCompensateMode.X,cCompensateMode.Y+16);
    }
}
//---------------------------------------------------------------------------
void ScreenActuatePlaySwitch::doKey9Work(void)
{
    if(cSelect==0){
        clearCursor8x16(cActuateMode.X,cActuateMode.Y+16);
        usiActType=9;
        cSelect=1;
        cPosition=0;
        lcd240x128.DISPLAY_GRAPHIC_XY(cActuateMode.X,cActuateMode.Y,word8x16[usiActType],cActuateMode.height,cActuateMode.width/8);
        setCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);
    }else if(cSelect==1){
        clearCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);
        valueSwitch[cPosition]=9;
        lcd240x128.DISPLAY_GRAPHIC_XY(positionSwitch[cPosition].X,positionSwitch[cPosition].Y,word8x16[valueSwitch[cPosition]],positionSwitch[cPosition].height,positionSwitch[cPosition].width/8);
		if(cPosition<7){
		    cPosition++;
		    setCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);
        }else{
            cSelect=2 ;
            cPosition=0;
            setCursor8x16(positionExtendSec[cPosition].X,positionExtendSec[cPosition].Y+16);
        }
    }else if(cSelect==2){
        clearCursor8x16(positionExtendSec[cPosition].X,positionExtendSec[cPosition].Y+16);
        valueExtendSec[cPosition]=9;
        lcd240x128.DISPLAY_GRAPHIC_XY(positionExtendSec[cPosition].X,positionExtendSec[cPosition].Y,word8x16[valueExtendSec[cPosition]],positionExtendSec[cPosition].height,positionExtendSec[cPosition].width/8);
		if(cPosition<7){
		    cPosition++;
		    setCursor8x16(positionExtendSec[cPosition].X,positionExtendSec[cPosition].Y+16);
        }else{
            cSelect=3 ;
            cPosition=0;
            setCursor8x16(cActuateFlashPlan[cPosition].X,cActuateFlashPlan[cPosition].Y+16);
        }
        //setCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);
    }else if(cSelect==3){
        clearCursor8x16(cActuateFlashPlan[cPosition].X,cActuateFlashPlan[cPosition].Y+16);
        ucTMP_ActuatePlan[cPosition]=9;
        //lcd240x128.DISPLAY_GRAPHIC_XY(cActuatePlan[0].X,cActuatePlan[0].Y,word8x16[ucTMP_ActuatePlan[0]],cActuatePlan[0].height,cActuatePlan[0].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(cActuateFlashPlan[cPosition].X,cActuateFlashPlan[cPosition].Y,word8x16[ucTMP_ActuatePlan[cPosition]],cActuateFlashPlan[cPosition].height,cActuateFlashPlan[cPosition].width/8);
        if(cPosition<1){
            cPosition++;
            setCursor8x16(cActuateFlashPlan[cPosition].X,cActuateFlashPlan[cPosition].Y+16);
        }else{
            cSelect=4 ;
            cPosition=0;
            setCursor8x16(cStartSec[cPosition].X,cStartSec[cPosition].Y+16);
        }
    }else if(cSelect==4){
        clearCursor8x16(cStartSec[cPosition].X,cStartSec[cPosition].Y+16);
        valueActionSec[cPosition]=9;
        //lcd240x128.DISPLAY_GRAPHIC_XY(cActuatePlan[0].X,cActuatePlan[0].Y,word8x16[ucTMP_ActuatePlan[0]],cActuatePlan[0].height,cActuatePlan[0].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(cStartSec[cPosition].X,cStartSec[cPosition].Y,word8x16[valueActionSec[cPosition]],cStartSec[cPosition].height,cStartSec[cPosition].width/8);
        if(cPosition<1){
            cPosition++;
            setCursor8x16(cStartSec[cPosition].X,cStartSec[cPosition].Y+16);
        }else{
            cSelect=5 ;
            cPosition=0;
            setCursor8x16(cCompensateMode.X,cCompensateMode.Y+16);
        }
    }else if(cSelect==5){
        clearCursor8x16(cCompensateMode.X,cCompensateMode.Y+16);
        valueCompensateMode = 9;
        //lcd240x128.DISPLAY_GRAPHIC_XY(cActuatePlan[0].X,cActuatePlan[0].Y,word8x16[ucTMP_ActuatePlan[0]],cActuatePlan[0].height,cActuatePlan[0].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(cCompensateMode.X,cCompensateMode.Y,word8x16[valueCompensateMode],cCompensateMode.height,cCompensateMode.width/8);
        cPosition = 0;
        setCursor8x16(cCompensateMode.X,cCompensateMode.Y+16);
    }
}
//---------------------------------------------------------------------------
void ScreenActuatePlaySwitch::doKeyAWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenActuatePlaySwitch::doKeyBWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenActuatePlaySwitch::doKeyCWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenActuatePlaySwitch::doKeyDWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenActuatePlaySwitch::doKeyEWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenActuatePlaySwitch::doKeyFWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenActuatePlaySwitch::doKeyF1Work(void)
{
//    screenLogo.DisplayLogo();
    screenMain.DisplayMain();
}
//---------------------------------------------------------------------------
void ScreenActuatePlaySwitch::doKeyF2Work(void)
{
    screenSegmentVDPhaseMapTableEdit.DisplayVDPhaseMapTableEdit(usActplayNo);
}
//---------------------------------------------------------------------------
void ScreenActuatePlaySwitch::doKeyF3Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenActuatePlaySwitch::doKeyF4Work(void)
{
//    screenMain.DisplayMain();
    screenSegtype.DisplaySegtype(stc._panel_segment._segment_type, INVAILDVALUE);
}
//---------------------------------------------------------------------------
void ScreenActuatePlaySwitch::doKeyUPWork(void)
{
try {
    if(cSelect==0){
		clearCursor8x16(cActuateMode.X,cActuateMode.Y+16);
		//cPosition=0;
		setCursor8x16(cActuateMode.X,cActuateMode.Y+16);
	}else if(cSelect==1){
		clearCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);
		//cPosition=0;
		cSelect=0;
		setCursor8x16(cActuateMode.X,cActuateMode.Y+16);
	}else if(cSelect==2){
		clearCursor8x16(positionExtendSec[cPosition].X,positionExtendSec[cPosition].Y+16);
		cPosition=0;
		cSelect=1;
		setCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);
	}else if(cSelect==3){
		clearCursor8x16(cActuateFlashPlan[cPosition].X,cActuateFlashPlan[cPosition].Y+16);
		cPosition=0;
		cSelect=2;
		setCursor8x16(positionExtendSec[cPosition].X,positionExtendSec[cPosition].Y+16);
	}else if(cSelect==4){
		clearCursor8x16(cStartSec[cPosition].X,cStartSec[cPosition].Y+16);
		cPosition=0;
		cSelect=3;
		setCursor8x16(cActuateFlashPlan[cPosition].X,cActuateFlashPlan[cPosition].Y+16);
	}else if(cSelect==5){
		clearCursor8x16(cCompensateMode.X,cCompensateMode.Y+16);
		cPosition=0;
		cSelect=4;
		setCursor8x16(cStartSec[cPosition].X,cStartSec[cPosition].Y+16);
	}
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenActuatePlaySwitch::doKeyDOWNWork(void)
{
try {
    if(cSelect==0){
		clearCursor8x16(cActuateMode.X,cActuateMode.Y+16);
		cSelect=1;
		cPosition=0;
		setCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);
	}else if(cSelect==1){
		clearCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);
		cSelect=2;
		cPosition=0;
		setCursor8x16(positionExtendSec[cPosition].X,positionExtendSec[cPosition].Y+16);
	}else if(cSelect==2){
		clearCursor8x16(positionExtendSec[cPosition].X,positionExtendSec[cPosition].Y+16);
		cSelect=3;
		cPosition=0;
		setCursor8x16(cActuateFlashPlan[cPosition].X,cActuateFlashPlan[cPosition].Y+16);
	}
	else if(cSelect==3){
		clearCursor8x16(cActuateFlashPlan[cPosition].X,cActuateFlashPlan[cPosition].Y+16);
		cSelect=4;
		cPosition=0;
		setCursor8x16(cStartSec[cPosition].X,cStartSec[cPosition].Y+16);
	}else if(cSelect==4){
		clearCursor8x16(cStartSec[cPosition].X,cStartSec[cPosition].Y+16);
		cSelect=5;
		cPosition=0;
		setCursor8x16(cCompensateMode.X,cCompensateMode.Y+16);
	}else if(cSelect==5){
		clearCursor8x16(cCompensateMode.X,cCompensateMode.Y+16);
		setCursor8x16(cCompensateMode.X,cCompensateMode.Y+16);
	}
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenActuatePlaySwitch::doKeyLEFTWork(void)
{
try {
    if(cSelect==0){
		clearCursor8x16(cActuateMode.X,cActuateMode.Y+16);
		cPosition=0;
		setCursor8x16(cActuateMode.X,cActuateMode.Y+16);
	}else if(cSelect==1){
		clearCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);
		if(cPosition<1){
			cSelect=0;
			cPosition=0;
			setCursor8x16(cActuateMode.X,cActuateMode.Y+16);
		}else{
			cPosition--;
			setCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);
		}
	}else if(cSelect==2){
		clearCursor8x16(positionExtendSec[cPosition].X,positionExtendSec[cPosition].Y+16);
		if(cPosition<1){
			cSelect=1;
			cPosition=7;
			setCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);
		}else{
			cPosition--;
			setCursor8x16(positionExtendSec[cPosition].X,positionExtendSec[cPosition].Y+16);
		}
	}else if(cSelect==3){
        clearCursor8x16(cActuateFlashPlan[cPosition].X,cActuateFlashPlan[cPosition].Y+16);
		if(cPosition<1){
			cSelect=2;
			cPosition=7;
			setCursor8x16(positionExtendSec[cPosition].X,positionExtendSec[cPosition].Y+16);
		}else{
			cPosition--;
			setCursor8x16(cActuateFlashPlan[cPosition].X,cActuateFlashPlan[cPosition].Y+16);
		}
    }else if(cSelect==4){
        clearCursor8x16(cStartSec[cPosition].X,cStartSec[cPosition].Y+16);
		if(cPosition<1){
			cSelect=3;
			cPosition=1;
			setCursor8x16(cActuateFlashPlan[cPosition].X,cActuateFlashPlan[cPosition].Y+16);
		}else{
			cPosition--;
			setCursor8x16(cStartSec[cPosition].X,cStartSec[cPosition].Y+16);
		}
    }else if(cSelect==5){
        clearCursor8x16(cCompensateMode.X,cCompensateMode.Y+16);
		cSelect=4;
		cPosition=1;
		setCursor8x16(cStartSec[cPosition].X,cStartSec[cPosition].Y+16);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenActuatePlaySwitch::doKeyRIGHTWork(void)
{
try {
    if(cSelect==0){
		clearCursor8x16(cActuateMode.X,cActuateMode.Y+16);
		cSelect=1;
		cPosition=0;
		setCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);
	}else if(cSelect==1){
		clearCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);
		if(cPosition<7){
			cPosition++;
			setCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);
		}else{
			cSelect=2;
			cPosition=0;
			setCursor8x16(positionExtendSec[cPosition].X,positionExtendSec[cPosition].Y+16);
		}

	}else if(cSelect==2){
		clearCursor8x16(positionExtendSec[cPosition].X,positionExtendSec[cPosition].Y+16);
		if(cPosition<7){
			cPosition++;
			setCursor8x16(positionExtendSec[cPosition].X,positionExtendSec[cPosition].Y+16);
		}else{
			//cPosition=7;
			cSelect=3;
			cPosition=0;
			setCursor8x16(cActuateFlashPlan[cPosition].X,cActuateFlashPlan[cPosition].Y+16);
		}
	}else if(cSelect==3){
		clearCursor8x16(cActuateFlashPlan[cPosition].X,cActuateFlashPlan[cPosition].Y+16);
		if(cPosition<1){
			cPosition++;
			setCursor8x16(cActuateFlashPlan[cPosition].X,cActuateFlashPlan[cPosition].Y+16);
		}else{
            //cPosition=1;
            cSelect=4;
            cPosition=0;
            setCursor8x16(cStartSec[cPosition].X,cStartSec[cPosition].Y+16);
		}

	}else if(cSelect==4){
	    clearCursor8x16(cStartSec[cPosition].X,cStartSec[cPosition].Y+16);
		if(cPosition<1){
			cPosition++;
			setCursor8x16(cStartSec[cPosition].X,cStartSec[cPosition].Y+16);
		}else{
		    cSelect=4;
            cPosition=0;
            setCursor8x16(cCompensateMode.X,cCompensateMode.Y+16);

		}
	}else if(cSelect==5){
	    clearCursor8x16(cCompensateMode.X,cCompensateMode.Y+16);
		cPosition = 0;
		setCursor8x16(cCompensateMode.X,cCompensateMode.Y+16);

	}
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenActuatePlaySwitch::doKeyEnterWork(void)
{
try {
    bool res=false;
    if(usiActType>6){
        res=true;
    }
    for(int i = 0 ; i < 8 ; i++){
        if(valueSwitch[i]>1){
            res=true;
            break;
        }
        if(valueExtendSec[i]>9){
            res=true;
            break;
        }
    }
    unsigned int tmpActPlan=ucTMP_ActuatePlan[0]*10+ucTMP_ActuatePlan[1];
    if(tmpActPlan>48){
        res=true;
    }
    unsigned int tmpActionSec=valueActionSec[0]*10+valueActionSec[1];
    if( tmpActionSec>20 && tmpActionSec != 0){
        res=true;
    }
    unsigned int tmpCompensateMode = valueCompensateMode;
    if(tmpCompensateMode > 1){
        res=true;
    }
    if(res){
        return ;
    }else{
        smem.vSetUCData(actuateSwitchChange_for_Arwen,1);//arwen actChange 1000707
        screenSegtype.DisplaySegtype(stc._panel_segment._segment_type, INVAILDVALUE);
        smem.saveSegmentActData(usActplayNo,usiActType,valueSwitch,valueExtendSec,tmpActPlan,tmpActionSec,tmpCompensateMode);
    }



  } catch (...) {}
}
