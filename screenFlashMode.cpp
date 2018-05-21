#include "screenFlashMode.h"
#include "SCREENControlStrategyMenu.h"
#include "screenVDPhaseMapTableEdit.h"
#include "var.h"
#include "LCD240x128.h"
#include "SCREENMain.h"
#include "SCREENCtlSetup.h"
#include <stdio.h>
#include "SMEM.h"
#include <string.h>
#include "CSTORINI.h"
#include "ConfigFile.h"
#include "SCREENActuateStrategyMenu.h"

#include "CPlanInfo.h"
#include "CPhaseInfo.h"
#include "CSTC.h"

#include <stdlib.h>
//--------------------------------------
ScreenFlashMode screenFlashMode;
//---------------------------------------------------------------------------
ScreenFlashMode::ScreenFlashMode(void)
{
    LoadBitmapFromFile();
    InitDispWord();
    cSelect=0;
    cPosition=0;
    cPosition2=0;
}
//---------------------------------------------------------------------------
ScreenFlashMode::~ScreenFlashMode(void)
{
}
//---------------------------------------------------------------------------
void ScreenFlashMode::DoKeyWork(BYTE key)                                           //已�\uFFFD�\uFFFD\uFFFD��\uFFFD\uFFFD\uFFFD\uFFFD0x80~0x98�\uFFFD\uFFFD
{
try {
    switch (key) {
        case 0x80:
          DoKey0Work();
        break;
        case 0x81:
          DoKey1Work();
        break;
        case 0x82:
          DoKey2Work();
        break;
        case 0x83:
          DoKey3Work();
        break;
        case 0x84:
          DoKey4Work();
        break;
        case 0x85:
          DoKey5Work();
        break;
        case 0x86:
          DoKey6Work();
        break;
        case 0x87:
          DoKey7Work();
        break;
        case 0x88:
          DoKey8Work();
        break;
        case 0x89:
          DoKey9Work();
        break;
        case 0x8A:
          DoKeyAWork();
        break;
        case 0x8B:
          DoKeyBWork();
        break;
        case 0x8C:
          DoKeyCWork();
        break;
        case 0x8D:
          DoKeyDWork();
        break;
        case 0x8E:
          DoKeyEWork();
        break;
        case 0x8F:
          DoKeyFWork();
        break;
        case 0x90://F1,ESC
          DoKeyF1Work();
        break;
        case 0x91://F2
          DoKeyF2Work();
        break;
        case 0x92://F3
          DoKeyF3Work();
        break;
        case 0x93://F4
          DoKeyF4Work();
        break;
        case 0x94://UP
          DoKeyUPWork();
        break;
        case 0x95://DOWN
          DoKeyDOWNWork();
        break;
        case 0x96://LEFT
          DoKeyLEFTWork();
        break;
        case 0x97://RIGHT
          DoKeyRIGHTWork();
        break;
        case 0x98://Enter
          DoKeyEnterWork();
        break;
        default:
        break;
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenFlashMode::LoadBitmapFromFile(void)
{
try {
    FILE *bitmap;
    bitmap=fopen("//cct//bitmap//backGround//actmenu//ActuateFlashMode.bit","rb");
    if (bitmap) {
        fread(FlashModeBitmap,3840,1,bitmap);
        fclose(bitmap);
    }

    bitmap=fopen("//cct//bitmap//backGround//actmenu//SettingFlashType.bit","rb");
    if (bitmap) {
        fread(SettingFlashTypeBitmap,3840,1,bitmap);
        fclose(bitmap);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenFlashMode::DisplaySettingFlashType()
{
	smem.SetcFace(cFLASHMODE_SETTING);
	lcd240x128.DISPLAY_GRAPHIC(0,SettingFlashTypeBitmap,128,30);
}
//---------------------------------------------------------------------------
void ScreenFlashMode::DisplayFlashMode()
{
try {
    smem.SetcFace(cFLASHMODE);
    smem.vWriteMsgToDOM("Enter VDPhaseMapTableEdit Setup Screen");
    lcd240x128.DISPLAY_GRAPHIC(0,FlashModeBitmap,128,30);
    setCursor8x16(cActuateType_By_TOD.X,cActuateType_By_TOD.Y+16);
    cPosition=0;
    cSelect=0;
    ReadVDdevFile();
    ReadVDNumFile();
    ReadActPlan();
    DisplayFlashSwitch();
    DisplayFlashVDPhaseSwitchEdit();
    DisplayFlashActiontSec();
    DisplayActuatePlanID();

  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenFlashMode::InitDispWord(void)
{
try {


	cActuateType_By_TOD.X = 94;
	cActuateType_By_TOD.Y = 24;
	cActuateType_By_TOD.width = 8;
	cActuateType_By_TOD.height = 16;

	for (int i=0;i<8;i++) {
		valueSwitch[i]=0;
		positionSwitch[i].width=8;
		positionSwitch[i].height=16;
		positionSwitch[i].X=96+i*16;
		positionSwitch[i].Y=78;
	}
	for(int i=0;i<2;i++){
		valueActionSec[i]=0;
		valueActionSec[i]=0;
		positionActionSec[i].width=8;
		positionActionSec[i].height=16;
		positionActionSec[i].X=146+i*8;
		positionActionSec[i].Y=44;
	}

	for (int i = 0; i < 2; i++) {
    cActuatePlan[i].X = 64 + i*8;
    cActuatePlan[i].Y = 84;
    cActuatePlan[i].width = 8;
    cActuatePlan[i].height = 16;
    }


  } catch (...) {}
}

void ScreenFlashMode::DisplayFlashSwitch(void)
{
try {
	//smem.vSetUSIData(TC_ActuateVDID, usiActuateVDID);
	//smem.vSetUCData(TC_CCT_ActuateType_By_TOD, ucActuateType_By_TOD);
	usiActuateVDID = smem.vGetUSIData(TC_ActuateVDID);
	ucActuateType_By_TOD = smem.vGetUCData(TC_CCT_ActuateType_By_TOD);

	printf("ActuateVDID:%d ActuateType_By_TOD:%d\n",usiActuateVDID,ucActuateType_By_TOD);
	lcd240x128.DISPLAY_GRAPHIC_XY(cActuateType_By_TOD.X,cActuateType_By_TOD.Y,word8x16[ucActuateType_By_TOD],cActuateType_By_TOD.height,cActuateType_By_TOD.width/8);


  } catch (...) {}
}
void ScreenFlashMode::DisplayFlashVDPhaseSwitchEdit(void)
{
try {
	for(int i=0;i<8;i++){
            usiSwitch[i]=smem.vGetVDPhaseTriggerSwitch(i);
            valueSwitch[i]=usiSwitch[i];
            printf("VDPhaseSwitch[%d]=%d \n",i,valueSwitch[i]);
	}


	for(int i=0;i<8;i++){
		lcd240x128.DISPLAY_GRAPHIC_XY(positionSwitch[i].X,positionSwitch[i].Y,word8x16[valueSwitch[i]],positionSwitch[i].height,positionSwitch[i].width/8);
	}

  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenFlashMode::DisplayFlashActiontSec(void)
{
	int tmpHiSec=0;
	int tmpLoSec=0;
	tmpHiSec=(usiActionSec[0]/10)%10;
	tmpLoSec=usiActionSec[0]%10;
	lcd240x128.DISPLAY_GRAPHIC_XY(positionActionSec[0].X,positionActionSec[0].Y,word8x16[tmpHiSec],positionActionSec[0].height,positionActionSec[0].width/8);
	lcd240x128.DISPLAY_GRAPHIC_XY(positionActionSec[1].X,positionActionSec[1].Y,word8x16[tmpLoSec],positionActionSec[1].height,positionActionSec[1].width/8);
}
//---------------------------------------------------------------------------
void ScreenFlashMode::DisplayActuatePlanID(void)
{
try{
  //HEX
  lcd240x128.DISPLAY_GRAPHIC_XY(cActuatePlan[0].X,cActuatePlan[0].Y,word8x16[ucTMP_ActuatePlan[0]],cActuatePlan[0].height,cActuatePlan[0].width/8);
  lcd240x128.DISPLAY_GRAPHIC_XY(cActuatePlan[1].X,cActuatePlan[1].Y,word8x16[ucTMP_ActuatePlan[1]],cActuatePlan[1].height,cActuatePlan[1].width/8);

}catch(...){}
}
//---------------------------------------------------------------------------

void ScreenFlashMode::DoKey0Work(void)
{
try {
	if(smem.GetcFace()==cFLASHMODE){
		if(cSelect==0){
			clearCursor8x16(cActuateType_By_TOD.X,cActuateType_By_TOD.Y+16);
			ucActuateType_By_TOD=0;
			cSelect=1;
			cPosition=0;
			lcd240x128.DISPLAY_GRAPHIC_XY(cActuateType_By_TOD.X,cActuateType_By_TOD.Y,word8x16[ucActuateType_By_TOD],cActuateType_By_TOD.height,cActuateType_By_TOD.width/8);
			setCursor8x16(positionActionSec[cPosition].X,positionActionSec[cPosition].Y+16);
		}else if(cSelect==1){
			clearCursor8x16(positionActionSec[cPosition].X,positionActionSec[cPosition].Y+16);
			valueActionSec[cPosition]=0;
			lcd240x128.DISPLAY_GRAPHIC_XY(positionActionSec[cPosition].X,positionActionSec[cPosition].Y,word8x16[valueActionSec[cPosition]],positionActionSec[cPosition].height,positionActionSec[cPosition].width/8);
			if(cPosition<1){
				cPosition++;
				setCursor8x16(positionActionSec[cPosition].X,positionActionSec[cPosition].Y+16);
			}else{
				cSelect=2 ;
				cPosition=0;
				setCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);
			}

		}else if(cSelect==2){
			clearCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);
			valueSwitch[cPosition]=0;
			lcd240x128.DISPLAY_GRAPHIC_XY(positionSwitch[cPosition].X,positionSwitch[cPosition].Y,word8x16[valueSwitch[cPosition]],positionSwitch[cPosition].height,positionSwitch[cPosition].width/8);
			if(cPosition<7){
				cPosition++;
				setCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);
			}else{
			    cSelect=3 ;
				cPosition=0;
				setCursor8x16(cActuatePlan[cPosition].X,cActuatePlan[cPosition].Y+16);
			}
			//setCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);

		}else if(cSelect==3){
			clearCursor8x16(cActuatePlan[cPosition].X,cActuatePlan[cPosition].Y+16);
			ucTMP_ActuatePlan[cPosition]=0;
			//lcd240x128.DISPLAY_GRAPHIC_XY(cActuatePlan[0].X,cActuatePlan[0].Y,word8x16[ucTMP_ActuatePlan[0]],cActuatePlan[0].height,cActuatePlan[0].width/8);

			lcd240x128.DISPLAY_GRAPHIC_XY(cActuatePlan[cPosition].X,cActuatePlan[cPosition].Y,word8x16[ucTMP_ActuatePlan[cPosition]],cActuatePlan[cPosition].height,cActuatePlan[cPosition].width/8);
			if(cPosition<1){
				cPosition++;
			}
			setCursor8x16(cActuatePlan[cPosition].X,cActuatePlan[cPosition].Y+16);
		}
	}

  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenFlashMode::DoKey1Work(void)
{
try {
	if(smem.GetcFace()==cFLASHMODE){
		if(cSelect==0){
			clearCursor8x16(cActuateType_By_TOD.X,cActuateType_By_TOD.Y+16);
			ucActuateType_By_TOD=1;
			cSelect=1;
			cPosition=0;
			lcd240x128.DISPLAY_GRAPHIC_XY(cActuateType_By_TOD.X,cActuateType_By_TOD.Y,word8x16[ucActuateType_By_TOD],cActuateType_By_TOD.height,cActuateType_By_TOD.width/8);
			setCursor8x16(positionActionSec[cPosition].X,positionActionSec[cPosition].Y+16);
		}else if(cSelect==1){
			clearCursor8x16(positionActionSec[cPosition].X,positionActionSec[cPosition].Y+16);
			valueActionSec[cPosition]=1;
			lcd240x128.DISPLAY_GRAPHIC_XY(positionActionSec[cPosition].X,positionActionSec[cPosition].Y,word8x16[valueActionSec[cPosition]],positionActionSec[cPosition].height,positionActionSec[cPosition].width/8);
			if(cPosition<1){
				cPosition++;
				setCursor8x16(positionActionSec[cPosition].X,positionActionSec[cPosition].Y+16);
			}else{
				cSelect=2 ;
				cPosition=0;
				setCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);
			}

		}else if(cSelect==2){
			clearCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);
			valueSwitch[cPosition]=1;
			lcd240x128.DISPLAY_GRAPHIC_XY(positionSwitch[cPosition].X,positionSwitch[cPosition].Y,word8x16[valueSwitch[cPosition]],positionSwitch[cPosition].height,positionSwitch[cPosition].width/8);
			if(cPosition<7){
				cPosition++;
			}else{
			    cSelect=3 ;
				cPosition=0;
				setCursor8x16(cActuatePlan[cPosition].X,cActuatePlan[cPosition].Y+16);
			}
			//setCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);

		}else if(cSelect==3){
			clearCursor8x16(cActuatePlan[cPosition].X,cActuatePlan[cPosition].Y+16);
			ucTMP_ActuatePlan[cPosition]=1;
			//lcd240x128.DISPLAY_GRAPHIC_XY(cActuatePlan[0].X,cActuatePlan[0].Y,word8x16[ucTMP_ActuatePlan[0]],cActuatePlan[0].height,cActuatePlan[0].width/8);

			lcd240x128.DISPLAY_GRAPHIC_XY(cActuatePlan[cPosition].X,cActuatePlan[cPosition].Y,word8x16[ucTMP_ActuatePlan[cPosition]],cActuatePlan[cPosition].height,cActuatePlan[cPosition].width/8);
			if(cPosition<1){
				cPosition++;
			}
			setCursor8x16(cActuatePlan[cPosition].X,cActuatePlan[cPosition].Y+16);
		}
	}

  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenFlashMode::DoKey2Work(void)
{
try {
	if(smem.GetcFace()==cFLASHMODE){
		if(cSelect==0){
			clearCursor8x16(cActuateType_By_TOD.X,cActuateType_By_TOD.Y+16);
			ucActuateType_By_TOD=2;
			cSelect=1;
			cPosition=0;
			lcd240x128.DISPLAY_GRAPHIC_XY(cActuateType_By_TOD.X,cActuateType_By_TOD.Y,word8x16[ucActuateType_By_TOD],cActuateType_By_TOD.height,cActuateType_By_TOD.width/8);
			setCursor8x16(positionActionSec[cPosition].X,positionActionSec[cPosition].Y+16);
		}else if(cSelect==1){
			clearCursor8x16(positionActionSec[cPosition].X,positionActionSec[cPosition].Y+16);
			valueActionSec[cPosition]=2;
			lcd240x128.DISPLAY_GRAPHIC_XY(positionActionSec[cPosition].X,positionActionSec[cPosition].Y,word8x16[valueActionSec[cPosition]],positionActionSec[cPosition].height,positionActionSec[cPosition].width/8);
			if(cPosition<1){
				cPosition++;
				setCursor8x16(positionActionSec[cPosition].X,positionActionSec[cPosition].Y+16);
			}else{
				cSelect=2 ;
				cPosition=0;
				setCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);
			}

		}else if(cSelect==2){
			clearCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);
			valueSwitch[cPosition]=2;
			lcd240x128.DISPLAY_GRAPHIC_XY(positionSwitch[cPosition].X,positionSwitch[cPosition].Y,word8x16[valueSwitch[cPosition]],positionSwitch[cPosition].height,positionSwitch[cPosition].width/8);
			if(cPosition<7){
				cPosition++;
			}else{
			    cSelect=3 ;
				cPosition=0;
				setCursor8x16(cActuatePlan[cPosition].X,cActuatePlan[cPosition].Y+16);
			}
			//setCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);

		}else if(cSelect==3){
			clearCursor8x16(cActuatePlan[cPosition].X,cActuatePlan[cPosition].Y+16);
			ucTMP_ActuatePlan[cPosition]=2;
			//lcd240x128.DISPLAY_GRAPHIC_XY(cActuatePlan[0].X,cActuatePlan[0].Y,word8x16[ucTMP_ActuatePlan[0]],cActuatePlan[0].height,cActuatePlan[0].width/8);

			lcd240x128.DISPLAY_GRAPHIC_XY(cActuatePlan[cPosition].X,cActuatePlan[cPosition].Y,word8x16[ucTMP_ActuatePlan[cPosition]],cActuatePlan[cPosition].height,cActuatePlan[cPosition].width/8);
			if(cPosition<1){
				cPosition++;
			}
			setCursor8x16(cActuatePlan[cPosition].X,cActuatePlan[cPosition].Y+16);
		}
	}

  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenFlashMode::DoKey3Work(void)
{
try {
	if(smem.GetcFace()==cFLASHMODE){
		if(cSelect==0){
			clearCursor8x16(cActuateType_By_TOD.X,cActuateType_By_TOD.Y+16);
			ucActuateType_By_TOD=3;
			cSelect=1;
			cPosition=0;
			lcd240x128.DISPLAY_GRAPHIC_XY(cActuateType_By_TOD.X,cActuateType_By_TOD.Y,word8x16[ucActuateType_By_TOD],cActuateType_By_TOD.height,cActuateType_By_TOD.width/8);
			setCursor8x16(positionActionSec[cPosition].X,positionActionSec[cPosition].Y+16);
		}else if(cSelect==1){
			clearCursor8x16(positionActionSec[cPosition].X,positionActionSec[cPosition].Y+16);
			valueActionSec[cPosition]=3;
			lcd240x128.DISPLAY_GRAPHIC_XY(positionActionSec[cPosition].X,positionActionSec[cPosition].Y,word8x16[valueActionSec[cPosition]],positionActionSec[cPosition].height,positionActionSec[cPosition].width/8);
			if(cPosition<1){
				cPosition++;
				setCursor8x16(positionActionSec[cPosition].X,positionActionSec[cPosition].Y+16);
			}else{
				cSelect=2 ;
				cPosition=0;
				setCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);
			}

		}else if(cSelect==2){
			clearCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);
			valueSwitch[cPosition]=3;
			lcd240x128.DISPLAY_GRAPHIC_XY(positionSwitch[cPosition].X,positionSwitch[cPosition].Y,word8x16[valueSwitch[cPosition]],positionSwitch[cPosition].height,positionSwitch[cPosition].width/8);
			if(cPosition<7){
				cPosition++;
			}else{
			    cSelect=3 ;
				cPosition=0;
				setCursor8x16(cActuatePlan[cPosition].X,cActuatePlan[cPosition].Y+16);
			}
			//setCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);

		}else if(cSelect==3){
			clearCursor8x16(cActuatePlan[cPosition].X,cActuatePlan[cPosition].Y+16);
			ucTMP_ActuatePlan[cPosition]=3;
			//lcd240x128.DISPLAY_GRAPHIC_XY(cActuatePlan[0].X,cActuatePlan[0].Y,word8x16[ucTMP_ActuatePlan[0]],cActuatePlan[0].height,cActuatePlan[0].width/8);

			lcd240x128.DISPLAY_GRAPHIC_XY(cActuatePlan[cPosition].X,cActuatePlan[cPosition].Y,word8x16[ucTMP_ActuatePlan[cPosition]],cActuatePlan[cPosition].height,cActuatePlan[cPosition].width/8);
			if(cPosition<1){
				cPosition++;
			}
			setCursor8x16(cActuatePlan[cPosition].X,cActuatePlan[cPosition].Y+16);
		}
	}

  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenFlashMode::DoKey4Work(void)
{
try {
	if(smem.GetcFace()==cFLASHMODE){
		if(cSelect==0){
			clearCursor8x16(cActuateType_By_TOD.X,cActuateType_By_TOD.Y+16);
			ucActuateType_By_TOD=4;
			cSelect=1;
			cPosition=0;
			lcd240x128.DISPLAY_GRAPHIC_XY(cActuateType_By_TOD.X,cActuateType_By_TOD.Y,word8x16[ucActuateType_By_TOD],cActuateType_By_TOD.height,cActuateType_By_TOD.width/8);
			setCursor8x16(positionActionSec[cPosition].X,positionActionSec[cPosition].Y+16);
		}else if(cSelect==1){
			clearCursor8x16(positionActionSec[cPosition].X,positionActionSec[cPosition].Y+16);
			valueActionSec[cPosition]=4;
			lcd240x128.DISPLAY_GRAPHIC_XY(positionActionSec[cPosition].X,positionActionSec[cPosition].Y,word8x16[valueActionSec[cPosition]],positionActionSec[cPosition].height,positionActionSec[cPosition].width/8);
			if(cPosition<1){
				cPosition++;
				setCursor8x16(positionActionSec[cPosition].X,positionActionSec[cPosition].Y+16);
			}else{
				cSelect=2 ;
				cPosition=0;
				setCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);
			}

		}else if(cSelect==2){
			clearCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);
			valueSwitch[cPosition]=4;
			lcd240x128.DISPLAY_GRAPHIC_XY(positionSwitch[cPosition].X,positionSwitch[cPosition].Y,word8x16[valueSwitch[cPosition]],positionSwitch[cPosition].height,positionSwitch[cPosition].width/8);
			if(cPosition<7){
				cPosition++;
			}else{
			    cSelect=3 ;
				cPosition=0;
				setCursor8x16(cActuatePlan[cPosition].X,cActuatePlan[cPosition].Y+16);
			}
			//setCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);

		}else if(cSelect==3){
			clearCursor8x16(cActuatePlan[cPosition].X,cActuatePlan[cPosition].Y+16);
			ucTMP_ActuatePlan[cPosition]=4;
			//lcd240x128.DISPLAY_GRAPHIC_XY(cActuatePlan[0].X,cActuatePlan[0].Y,word8x16[ucTMP_ActuatePlan[0]],cActuatePlan[0].height,cActuatePlan[0].width/8);

			lcd240x128.DISPLAY_GRAPHIC_XY(cActuatePlan[cPosition].X,cActuatePlan[cPosition].Y,word8x16[ucTMP_ActuatePlan[cPosition]],cActuatePlan[cPosition].height,cActuatePlan[cPosition].width/8);
			if(cPosition<1){
				cPosition++;
			}
			setCursor8x16(cActuatePlan[cPosition].X,cActuatePlan[cPosition].Y+16);
		}
	}

  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenFlashMode::DoKey5Work(void)
{
try {
	if(smem.GetcFace()==cFLASHMODE){
		if(cSelect==0){
			clearCursor8x16(cActuateType_By_TOD.X,cActuateType_By_TOD.Y+16);
			ucActuateType_By_TOD=5;
			cSelect=1;
			cPosition=0;
			lcd240x128.DISPLAY_GRAPHIC_XY(cActuateType_By_TOD.X,cActuateType_By_TOD.Y,word8x16[ucActuateType_By_TOD],cActuateType_By_TOD.height,cActuateType_By_TOD.width/8);
			setCursor8x16(positionActionSec[cPosition].X,positionActionSec[cPosition].Y+16);
		}else if(cSelect==1){
			clearCursor8x16(positionActionSec[cPosition].X,positionActionSec[cPosition].Y+16);
			valueActionSec[cPosition]=5;
			lcd240x128.DISPLAY_GRAPHIC_XY(positionActionSec[cPosition].X,positionActionSec[cPosition].Y,word8x16[valueActionSec[cPosition]],positionActionSec[cPosition].height,positionActionSec[cPosition].width/8);
			if(cPosition<1){
				cPosition++;
				setCursor8x16(positionActionSec[cPosition].X,positionActionSec[cPosition].Y+16);
			}else{
				cSelect=2 ;
				cPosition=0;
				setCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);
			}

		}else if(cSelect==2){
			clearCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);
			valueSwitch[cPosition]=5;
			lcd240x128.DISPLAY_GRAPHIC_XY(positionSwitch[cPosition].X,positionSwitch[cPosition].Y,word8x16[valueSwitch[cPosition]],positionSwitch[cPosition].height,positionSwitch[cPosition].width/8);
			if(cPosition<7){
				cPosition++;
			}else{
			    cSelect=3 ;
				cPosition=0;
				setCursor8x16(cActuatePlan[cPosition].X,cActuatePlan[cPosition].Y+16);
			}
			//setCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);

		}else if(cSelect==3){
			clearCursor8x16(cActuatePlan[cPosition].X,cActuatePlan[cPosition].Y+16);
			ucTMP_ActuatePlan[cPosition]=5;
			//lcd240x128.DISPLAY_GRAPHIC_XY(cActuatePlan[0].X,cActuatePlan[0].Y,word8x16[ucTMP_ActuatePlan[0]],cActuatePlan[0].height,cActuatePlan[0].width/8);

			lcd240x128.DISPLAY_GRAPHIC_XY(cActuatePlan[cPosition].X,cActuatePlan[cPosition].Y,word8x16[ucTMP_ActuatePlan[cPosition]],cActuatePlan[cPosition].height,cActuatePlan[cPosition].width/8);
			if(cPosition<1){
				cPosition++;
			}
			setCursor8x16(cActuatePlan[cPosition].X,cActuatePlan[cPosition].Y+16);
		}
	}

  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenFlashMode::DoKey6Work(void)
{
try {
	if(smem.GetcFace()==cFLASHMODE){
		if(cSelect==0){
			clearCursor8x16(cActuateType_By_TOD.X,cActuateType_By_TOD.Y+16);
			ucActuateType_By_TOD=6;
			cSelect=1;
			cPosition=0;
			lcd240x128.DISPLAY_GRAPHIC_XY(cActuateType_By_TOD.X,cActuateType_By_TOD.Y,word8x16[ucActuateType_By_TOD],cActuateType_By_TOD.height,cActuateType_By_TOD.width/8);
			setCursor8x16(positionActionSec[cPosition].X,positionActionSec[cPosition].Y+16);
		}else if(cSelect==1){
			clearCursor8x16(positionActionSec[cPosition].X,positionActionSec[cPosition].Y+16);
			valueActionSec[cPosition]=6;
			lcd240x128.DISPLAY_GRAPHIC_XY(positionActionSec[cPosition].X,positionActionSec[cPosition].Y,word8x16[valueActionSec[cPosition]],positionActionSec[cPosition].height,positionActionSec[cPosition].width/8);
			if(cPosition<1){
				cPosition++;
				setCursor8x16(positionActionSec[cPosition].X,positionActionSec[cPosition].Y+16);
			}else{
				cSelect=2 ;
				cPosition=0;
				setCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);
			}

		}else if(cSelect==2){
			clearCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);
			valueSwitch[cPosition]=6;
			lcd240x128.DISPLAY_GRAPHIC_XY(positionSwitch[cPosition].X,positionSwitch[cPosition].Y,word8x16[valueSwitch[cPosition]],positionSwitch[cPosition].height,positionSwitch[cPosition].width/8);
			if(cPosition<7){
				cPosition++;
			}else{
			    cSelect=3 ;
				cPosition=0;
				setCursor8x16(cActuatePlan[cPosition].X,cActuatePlan[cPosition].Y+16);
			}
			//setCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);

		}else if(cSelect==3){
			clearCursor8x16(cActuatePlan[cPosition].X,cActuatePlan[cPosition].Y+16);
			ucTMP_ActuatePlan[cPosition]=6;
			//lcd240x128.DISPLAY_GRAPHIC_XY(cActuatePlan[0].X,cActuatePlan[0].Y,word8x16[ucTMP_ActuatePlan[0]],cActuatePlan[0].height,cActuatePlan[0].width/8);

			lcd240x128.DISPLAY_GRAPHIC_XY(cActuatePlan[cPosition].X,cActuatePlan[cPosition].Y,word8x16[ucTMP_ActuatePlan[cPosition]],cActuatePlan[cPosition].height,cActuatePlan[cPosition].width/8);
			if(cPosition<1){
				cPosition++;
			}
			setCursor8x16(cActuatePlan[cPosition].X,cActuatePlan[cPosition].Y+16);
		}
	}

  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenFlashMode::DoKey7Work(void)
{
try {
	if(smem.GetcFace()==cFLASHMODE){
		if(cSelect==0){
			clearCursor8x16(cActuateType_By_TOD.X,cActuateType_By_TOD.Y+16);
			ucActuateType_By_TOD=7;
			cSelect=1;
			cPosition=0;
			lcd240x128.DISPLAY_GRAPHIC_XY(cActuateType_By_TOD.X,cActuateType_By_TOD.Y,word8x16[ucActuateType_By_TOD],cActuateType_By_TOD.height,cActuateType_By_TOD.width/8);
			setCursor8x16(positionActionSec[cPosition].X,positionActionSec[cPosition].Y+16);
		}else if(cSelect==1){
			clearCursor8x16(positionActionSec[cPosition].X,positionActionSec[cPosition].Y+16);
			valueActionSec[cPosition]=7;
			lcd240x128.DISPLAY_GRAPHIC_XY(positionActionSec[cPosition].X,positionActionSec[cPosition].Y,word8x16[valueActionSec[cPosition]],positionActionSec[cPosition].height,positionActionSec[cPosition].width/8);
			if(cPosition<1){
				cPosition++;
				setCursor8x16(positionActionSec[cPosition].X,positionActionSec[cPosition].Y+16);
			}else{
				cSelect=2 ;
				cPosition=0;
				setCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);
			}

		}else if(cSelect==2){
			clearCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);
			valueSwitch[cPosition]=7;
			lcd240x128.DISPLAY_GRAPHIC_XY(positionSwitch[cPosition].X,positionSwitch[cPosition].Y,word8x16[valueSwitch[cPosition]],positionSwitch[cPosition].height,positionSwitch[cPosition].width/8);
			if(cPosition<7){
				cPosition++;
			}else{
			    cSelect=3 ;
				cPosition=0;
				setCursor8x16(cActuatePlan[cPosition].X,cActuatePlan[cPosition].Y+16);
			}
			//setCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);

		}else if(cSelect==3){
			clearCursor8x16(cActuatePlan[cPosition].X,cActuatePlan[cPosition].Y+16);
			ucTMP_ActuatePlan[cPosition]=7;
			//lcd240x128.DISPLAY_GRAPHIC_XY(cActuatePlan[0].X,cActuatePlan[0].Y,word8x16[ucTMP_ActuatePlan[0]],cActuatePlan[0].height,cActuatePlan[0].width/8);

			lcd240x128.DISPLAY_GRAPHIC_XY(cActuatePlan[cPosition].X,cActuatePlan[cPosition].Y,word8x16[ucTMP_ActuatePlan[cPosition]],cActuatePlan[cPosition].height,cActuatePlan[cPosition].width/8);
			if(cPosition<1){
				cPosition++;
			}
			setCursor8x16(cActuatePlan[cPosition].X,cActuatePlan[cPosition].Y+16);
		}
	}

  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenFlashMode::DoKey8Work(void)
{
try {
	if(smem.GetcFace()==cFLASHMODE){
		if(cSelect==0){
			clearCursor8x16(cActuateType_By_TOD.X,cActuateType_By_TOD.Y+16);
			ucActuateType_By_TOD=8;
			cSelect=1;
			cPosition=0;
			lcd240x128.DISPLAY_GRAPHIC_XY(cActuateType_By_TOD.X,cActuateType_By_TOD.Y,word8x16[ucActuateType_By_TOD],cActuateType_By_TOD.height,cActuateType_By_TOD.width/8);
			setCursor8x16(positionActionSec[cPosition].X,positionActionSec[cPosition].Y+16);
		}else if(cSelect==1){
			clearCursor8x16(positionActionSec[cPosition].X,positionActionSec[cPosition].Y+16);
			valueActionSec[cPosition]=8;
			lcd240x128.DISPLAY_GRAPHIC_XY(positionActionSec[cPosition].X,positionActionSec[cPosition].Y,word8x16[valueActionSec[cPosition]],positionActionSec[cPosition].height,positionActionSec[cPosition].width/8);
			if(cPosition<1){
				cPosition++;
				setCursor8x16(positionActionSec[cPosition].X,positionActionSec[cPosition].Y+16);
			}else{
				cSelect=2 ;
				cPosition=0;
				setCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);
			}

		}else if(cSelect==2){
			clearCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);
			valueSwitch[cPosition]=8;
			lcd240x128.DISPLAY_GRAPHIC_XY(positionSwitch[cPosition].X,positionSwitch[cPosition].Y,word8x16[valueSwitch[cPosition]],positionSwitch[cPosition].height,positionSwitch[cPosition].width/8);
			if(cPosition<7){
				cPosition++;
			}else{
			    cSelect=3 ;
				cPosition=0;
				setCursor8x16(cActuatePlan[cPosition].X,cActuatePlan[cPosition].Y+16);
			}
			//setCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);

		}else if(cSelect==3){
			clearCursor8x16(cActuatePlan[cPosition].X,cActuatePlan[cPosition].Y+16);
			ucTMP_ActuatePlan[cPosition]=8;
			//lcd240x128.DISPLAY_GRAPHIC_XY(cActuatePlan[0].X,cActuatePlan[0].Y,word8x16[ucTMP_ActuatePlan[0]],cActuatePlan[0].height,cActuatePlan[0].width/8);

			lcd240x128.DISPLAY_GRAPHIC_XY(cActuatePlan[cPosition].X,cActuatePlan[cPosition].Y,word8x16[ucTMP_ActuatePlan[cPosition]],cActuatePlan[cPosition].height,cActuatePlan[cPosition].width/8);
			if(cPosition<1){
				cPosition++;
			}
			setCursor8x16(cActuatePlan[cPosition].X,cActuatePlan[cPosition].Y+16);
		}
	}

  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenFlashMode::DoKey9Work(void)
{
try {
	if(smem.GetcFace()==cFLASHMODE){
		if(cSelect==0){
			clearCursor8x16(cActuateType_By_TOD.X,cActuateType_By_TOD.Y+16);
			ucActuateType_By_TOD=9;
			cSelect=1;
			cPosition=0;
			lcd240x128.DISPLAY_GRAPHIC_XY(cActuateType_By_TOD.X,cActuateType_By_TOD.Y,word8x16[ucActuateType_By_TOD],cActuateType_By_TOD.height,cActuateType_By_TOD.width/8);
			setCursor8x16(positionActionSec[cPosition].X,positionActionSec[cPosition].Y+16);
		}else if(cSelect==1){
			clearCursor8x16(positionActionSec[cPosition].X,positionActionSec[cPosition].Y+16);
			valueActionSec[cPosition]=9;
			lcd240x128.DISPLAY_GRAPHIC_XY(positionActionSec[cPosition].X,positionActionSec[cPosition].Y,word8x16[valueActionSec[cPosition]],positionActionSec[cPosition].height,positionActionSec[cPosition].width/8);
			if(cPosition<1){
				cPosition++;
				setCursor8x16(positionActionSec[cPosition].X,positionActionSec[cPosition].Y+16);
			}else{
				cSelect=2 ;
				cPosition=0;
				setCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);
			}

		}else if(cSelect==2){
			clearCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);
			valueSwitch[cPosition]=9;
			lcd240x128.DISPLAY_GRAPHIC_XY(positionSwitch[cPosition].X,positionSwitch[cPosition].Y,word8x16[valueSwitch[cPosition]],positionSwitch[cPosition].height,positionSwitch[cPosition].width/8);
			if(cPosition<7){
				cPosition++;
			}else{
			    cSelect=3 ;
				cPosition=0;
				setCursor8x16(cActuatePlan[cPosition].X,cActuatePlan[cPosition].Y+16);
			}
			//setCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);

		}else if(cSelect==3){
			clearCursor8x16(cActuatePlan[cPosition].X,cActuatePlan[cPosition].Y+16);
			ucTMP_ActuatePlan[cPosition]=9;
			//lcd240x128.DISPLAY_GRAPHIC_XY(cActuatePlan[0].X,cActuatePlan[0].Y,word8x16[ucTMP_ActuatePlan[0]],cActuatePlan[0].height,cActuatePlan[0].width/8);

			lcd240x128.DISPLAY_GRAPHIC_XY(cActuatePlan[cPosition].X,cActuatePlan[cPosition].Y,word8x16[ucTMP_ActuatePlan[cPosition]],cActuatePlan[cPosition].height,cActuatePlan[cPosition].width/8);
			if(cPosition<1){
				cPosition++;
			}
			setCursor8x16(cActuatePlan[cPosition].X,cActuatePlan[cPosition].Y+16);
		}
	}

  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenFlashMode::DoKeyAWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenFlashMode::DoKeyBWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenFlashMode::DoKeyCWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenFlashMode::DoKeyDWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenFlashMode::DoKeyEWork(void)
{

}
//---------------------------------------------------------------------------
void ScreenFlashMode::DoKeyFWork(void)
{


}
//---------------------------------------------------------------------------
void ScreenFlashMode::DoKeyF1Work(void)
{
try {
	if(smem.GetcFace()==cFLASHMODE){
    screenMain.DisplayMain();
	}
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenFlashMode::DoKeyF2Work(void)
{
	ucActuateType_By_TOD=0;
	lcd240x128.DISPLAY_GRAPHIC_XY(cActuateType_By_TOD.X,cActuateType_By_TOD.Y,word8x16[ucActuateType_By_TOD],cActuateType_By_TOD.height,cActuateType_By_TOD.width/8);
	for(int i=0;i<8;i++){
		valueSwitch[i]=0;
		lcd240x128.DISPLAY_GRAPHIC_XY(positionSwitch[i].X,positionSwitch[i].Y,word8x16[valueSwitch[i]],positionSwitch[i].height,positionSwitch[i].width/8);
	}
	for(int i=0;i<2;i++){
		valueActionSec[i]=0;
		lcd240x128.DISPLAY_GRAPHIC_XY(positionActionSec[i].X,positionActionSec[i].Y,word8x16[valueActionSec[i]],positionActionSec[i].height,positionActionSec[i].width/8);
	}
}
//---------------------------------------------------------------------------
void ScreenFlashMode::DoKeyF3Work(void)
{
	usiActuatePlan = 0;
    usiActuatePlan += ucTMP_ActuatePlan[0]*10;
    usiActuatePlan += ucTMP_ActuatePlan[1];
	if(smem.GetcFace()==cFLASHMODE_SETTING){
			if(ucActuateType_By_TOD<=1){
				smem.vSetBOOLData(TC_CCT_In_LongTanu_ActuateType_FunctionEnable, true/*bTMP_ActuateSwitch*/);
				//smem.vSetUSIData(TC_CCT_In_LongTanu_ActuateType_Switch,smem.vGetUSIData(TC_CCT_In_LongTanu_ActuateType_Switch) /*usiActuatePlan*/);
				smem.vSetUSIData(TC_CCT_In_LongTanu_ActuateType_Switch,usiActuatePlan);
				smem.vSetActuatePhaseExtend(/*ucActuatePhaseID*/1);
				smem.vSetUSIData(TC_ActuateVDID, /*usiActuateVDID*/11111);
				smem.vSetUCData(TC_CCT_ActuateType_By_TOD, /*ucActuateType_By_TOD*/true);
				smem.vSetBOOLData(TCSegmentTypeUpdate, true);
				SaveVDdevFile();
				char tmp[100];
				for(int i=0;i<8;i++){
					sprintf(tmp,"PhaseTriggerSwitch%d",i);
					smem.vSetVDPhaseTriggerSwitch(i,valueSwitch[i]);
					oStorINI.vSaveIntToINI("VDTrigger", tmp, valueSwitch[i]);
					printf("p[%d]=%d ",i,smem.vGetVDPhaseTriggerSwitch(i));
				}
			}

			printf("TC_ActuateVDID:%d  TC_CCT_ActuateType_By_TOD:%d\n",usiActuateVDID,ucActuateType_By_TOD);
			system("sync");
			system("sync");
			system("killall -9 act_mode");
			//if(smem.vGetINTData(Com2_TYPE) != Com2IsPedestrianPushButton){
                system("/cct/act_mode >/dev/tty5 &");
			//}
			//screenVDPhaseMapTableEdit.DisplayVDPhaseMapTableEdit(0);
			screenActuateStrategyMenu.DisplayActuateStrategyMenu();
		}
}
//---------------------------------------------------------------------------
void ScreenFlashMode::DoKeyF4Work(void)
{
try {
	//screenControlStrategyMenu.DisplayControlStrategyMenu();
	screenActuateStrategyMenu.DisplayActuateStrategyMenu();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenFlashMode::DoKeyEnterWork(void)
{
try {
	if(smem.GetcFace()==cFLASHMODE){
		DisplaySettingFlashType();
	}

  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenFlashMode::DoKeyUPWork(void)
{
try {
	if(cSelect==0){
		clearCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);
		//cPosition=0;
		setCursor8x16(cActuateType_By_TOD.X,cActuateType_By_TOD.Y+16);
	}else if(cSelect==1){
		clearCursor8x16(positionActionSec[cPosition].X,positionActionSec[cPosition].Y+16);
		//cPosition=0;
		cSelect=0;
		setCursor8x16(cActuateType_By_TOD.X,cActuateType_By_TOD.Y+16);
	}else if(cSelect==2){
		clearCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);
		//cPosition=0;
		cSelect=1;
		setCursor8x16(positionActionSec[cPosition].X,positionActionSec[cPosition].Y+16);
	}else if(cSelect==3){
		clearCursor8x16(cActuatePlan[cPosition].X,cActuatePlan[cPosition].Y+16);
		cPosition=0;
		cSelect=2;
		setCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);
	}
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenFlashMode::DoKeyDOWNWork(void)
{
try {
	if(cSelect==0){
		clearCursor8x16(cActuateType_By_TOD.X,cActuateType_By_TOD.Y+16);
		cSelect=1;
		cPosition=0;
		setCursor8x16(positionActionSec[cPosition].X,positionActionSec[cPosition].Y+16);
	}else if(cSelect==1){
		clearCursor8x16(positionActionSec[cPosition].X,positionActionSec[cPosition].Y+16);
		cSelect=2;
		cPosition=0;
		setCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);
	}else if(cSelect==2){
		clearCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);
		cSelect=3;
		cPosition=0;
		setCursor8x16(cActuatePlan[cPosition].X,cActuatePlan[cPosition].Y+16);
	}
	else if(cSelect==3){
		clearCursor8x16(cActuatePlan[cPosition].X,cActuatePlan[cPosition].Y+16);
		setCursor8x16(cActuatePlan[cPosition].X,cActuatePlan[cPosition].Y+16);
	}
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenFlashMode::DoKeyLEFTWork(void)
{
try {
	if(cSelect==0){
		clearCursor8x16(cActuateType_By_TOD.X,cActuateType_By_TOD.Y+16);
		cPosition=0;
		setCursor8x16(cActuateType_By_TOD.X,cActuateType_By_TOD.Y+16);
	}else if(cSelect==1){
		clearCursor8x16(positionActionSec[cPosition].X,positionActionSec[cPosition].Y+16);
		if(cPosition<1){
			cSelect=0;
			cPosition=0;
			setCursor8x16(cActuateType_By_TOD.X,cActuateType_By_TOD.Y+16);
		}else{
			cPosition--;
			setCursor8x16(positionActionSec[cPosition].X,positionActionSec[cPosition].Y+16);
		}
	}else if(cSelect==2){
		clearCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);
		if(cPosition<1){
			cSelect=1;
			cPosition=0;
			setCursor8x16(positionActionSec[cPosition].X,positionActionSec[cPosition].Y+16);
		}else{
			cPosition--;
			setCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);
		}
	}else if(cSelect==3){
        clearCursor8x16(cActuatePlan[cPosition].X,cActuatePlan[cPosition].Y+16);
		if(cPosition<1){
			cSelect=2;
			cPosition=7;
			setCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);
		}else{
			cPosition--;
			setCursor8x16(cActuatePlan[cPosition].X,cActuatePlan[cPosition].Y+16);
		}
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenFlashMode::DoKeyRIGHTWork(void)
{
try {
	if(cSelect==0){
		clearCursor8x16(cActuateType_By_TOD.X,cActuateType_By_TOD.Y+16);
		cSelect=1;
		cPosition=0;
		setCursor8x16(positionActionSec[cPosition].X,positionActionSec[cPosition].Y+16);
	}else if(cSelect==1){
		clearCursor8x16(positionActionSec[cPosition].X,positionActionSec[cPosition].Y+16);
		if(cPosition<1){
			cPosition++;
			setCursor8x16(positionActionSec[cPosition].X,positionActionSec[cPosition].Y+16);
		}else{
			cSelect=2;
			cPosition=0;
			setCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);
		}

	}else if(cSelect==2){
		clearCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);
		if(cPosition<7){
			cPosition++;
			setCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);
		}else{
			//cPosition=7;
			cSelect=3;
			cPosition=0;
			setCursor8x16(cActuatePlan[cPosition].X,cActuatePlan[cPosition].Y+16);
		}
	}else if(cSelect==3){
		clearCursor8x16(cActuatePlan[cPosition].X,cActuatePlan[cPosition].Y+16);
		if(cPosition<1){
			cPosition++;
		}else{
            cPosition=1;
		}
		setCursor8x16(cActuatePlan[cPosition].X,cActuatePlan[cPosition].Y+16);
	}

  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenFlashMode::ReadVDdevFile()
{
	//---------------------------------------------------
		ConfigFile config( "/cct/vddata/dev.cfg" );
		// Values can be read from the file by name
		int iTmp = 0;
		usiNumVD=0;

		if(config.readInto( iTmp, "ACTTYPE" )){
			printf("Read The ACTTYPE of is:%d\n",iTmp);
			usiActType=iTmp;
		}
		if(config.readInto( iTmp, "NUMVD" )){
			printf("Read The NUMVD of is:%d\n",iTmp);
			usiNumVD=iTmp;
		}
	//---------------------------------------------------
}
//-----------------------------------------------------------
void ScreenFlashMode::ReadVDNumFile()
{
	char tmp[512]={0};

	int iTmp = 0;

	for(int i=0;i<usiNumVD;i++){
		sprintf(tmp,"/cct/vddata/vd%d.cfg",i+1);
		ConfigFile config( tmp );
		if(config.readInto( sTmp[i], "type" )){
			//printf("type of is:%d\n",sTmp[i]);
		}
		if(config.readInto( iTmp, "zonetimes" )){
			printf("zonetimes of is:%d \n",iTmp);
			usiActionSec[i]=iTmp;
		}
		if(sTmp[i]=="tfd"){
			usiActionSec[i]=usiActionSec[i]/8;
			printf("usiActionSec[%d]:%d",i,usiActionSec[i]);
		}
		printf("====ReadVDNumFile[%d]======\n",i);
	}
	valueActionSec[0]=usiActionSec[0]/10;
	valueActionSec[1]=usiActionSec[0]%10;

}
void ScreenFlashMode::ReadActPlan()
{
    usiActuatePlan = smem.vGetUSIData(TC_CCT_In_LongTanu_ActuateType_Switch);
    ucTMP_ActuatePlan[0] = (usiActuatePlan / 10) & 0xFF;
    ucTMP_ActuatePlan[1] = (usiActuatePlan % 10) & 0xFF;
}
void ScreenFlashMode::SaveVDdevFile()
{
	//---------------------------------------------------
		ConfigFile config( "/cct/vddata/dev.cfg" );
		// Values can be read from the file by name
		config.add( "ACTTYPE", 1 );//閃光觸動
		printf("Set ACTTYPE is:%d",1);
		config.writeCf();

		char tmp[512];
		for(int i=0;i<8;i++){
			sprintf(tmp,"AFPHASE%d",i+1);
			config.add(tmp,valueSwitch[i]);
			config.writeCf();
		}
		int tmpvalue[64];
		char tmp_config[512];
		for(int i=0;i<usiNumVD;i++){
			tmpvalue[i]=(valueActionSec[0]*10)+valueActionSec[1];
			sprintf(tmp_config,"/cct/vddata/vd%d.cfg",i+1);
			ConfigFile config(tmp_config);

			if(sTmp[i]=="tfd"){
				tmpvalue[i]=tmpvalue[i]*8;
			}
			printf("=====vd[%d] zonetimes=%d =====\n",i,tmpvalue[i]);
			config.add( "zonetimes", tmpvalue[i]);
			config.writeCf();
		}

	//---------------------------------------------------

}

