#include "screenVDActPhaseEdit.h"
#include "SCREENControlStrategyMenu.h"
#include "screenVDPhaseMapTableEdit.h"
#include "var.h"
#include "LCD240x128.h"
#include "SCREENMain.h"
#include "SCREENCtlSetup.h"
#include <stdio.h>
#include "SMEM.h"
#include <string.h>
#include <stdlib.h>
#include "CSTORINI.h"
#include "ConfigFile.h"
#include "SCREENActuateStrategyMenu.h"
//--------------------------------------
ScreenVDActPhaseEdit screenVDActPhaseEdit;
//---------------------------------------------------------------------------
ScreenVDActPhaseEdit::ScreenVDActPhaseEdit(void)
{
    LoadBitmapFromFile();
    InitDispWord();
    cSelect=0;
    cPosition=0;
}
//---------------------------------------------------------------------------
ScreenVDActPhaseEdit::~ScreenVDActPhaseEdit(void)
{
}
//---------------------------------------------------------------------------
void ScreenVDActPhaseEdit::DoKeyWork(BYTE key)                                           //已�\uFFFD�\uFFFD\uFFFD��\uFFFD\uFFFD\uFFFD\uFFFD0x80~0x98�\uFFFD\uFFFD
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
void ScreenVDActPhaseEdit::LoadBitmapFromFile(void)
{
try {
    FILE *bitmap;
    bitmap=fopen("//cct//bitmap//backGround//actmenu//ActuateVDPhaseEdit.bit","rb");
    if (bitmap) {
        fread(VDPhaseEditBitmap,3840,1,bitmap);
        fclose(bitmap);
    }
    bitmap=fopen("//cct//bitmap//backGround//actmenu//ActuatePhaseGreenMode.bit","rb");
    if (bitmap) {
        fread(PhaseGreenModeBitmap,3840,1,bitmap);
        fclose(bitmap);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenVDActPhaseEdit::DisplayActuatePhaseGreenMode()
{
	 smem.SetcFace(cVDActPhaseGreen);
	 lcd240x128.DISPLAY_GRAPHIC(0,PhaseGreenModeBitmap,128,30);
}
//---------------------------------------------------------------------------
void ScreenVDActPhaseEdit::DisplayVDActPhaseEdit()
{
try {
    smem.SetcFace(cVDActPhaseEdit);
    smem.vWriteMsgToDOM("Enter VDPhaseMapTableEdit Setup Screen");
    lcd240x128.DISPLAY_GRAPHIC(0,VDPhaseEditBitmap,128,30);
    cPosition=0;
    cSelect=0;
    //setCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);
    setCursor8x16(fixedGreenMode.X,fixedGreenMode.Y+16);
    ReadVDdevFile();
    DisplayVDPhaseSwitchEdit();
    DisplayExtendValue();
    DisplayfixedGree();

  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenVDActPhaseEdit::InitDispWord(void)
{
try {
	for(int i=0;i<2;i++){
		makrphaseId[i].width=8;
		makrphaseId[i].height=16;
		makrphaseId[i].X=160+(i*8);
		makrphaseId[i].Y=1;
	}

	for(int i=0;i<8;i++){
        valExtendSec[i]=0;
        positionExtendSec[i].width=8;
        positionExtendSec[i].height=16;
        positionExtendSec[i].X=86+i*16;
        positionExtendSec[i].Y=90;
	}
	for (int i=0;i<8;i++) {
		 valueSwitch[i]=0;
         positionSwitch[i].width=8;
         positionSwitch[i].height=16;
         positionSwitch[i].X=54+i*24;
         positionSwitch[i].Y=55;
    }

    fixedGreenMode.width = 8;
    fixedGreenMode.height = 16;
    fixedGreenMode.X=112;
    fixedGreenMode.Y=20;



  } catch (...) {}
}

void ScreenVDActPhaseEdit::DisplayVDPhaseSwitchEdit(void)
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
void ScreenVDActPhaseEdit::DisplayExtendValue()
{
try{
	printf("TIMEXTEND:%d\n",valExtendSec);
	for(int i=0;i<8;i++){
	    valExtendSec[i]=usiExtendSec[i];
        lcd240x128.DISPLAY_GRAPHIC_XY(positionExtendSec[i].X,positionExtendSec[i].Y,word8x16[valExtendSec[i]],positionExtendSec[i].height,positionExtendSec[i].width/8);
	}
} catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenVDActPhaseEdit::DisplayfixedGree()
{
try{
	printf("fixedGreeMode:%d\n",valuefixedGreenMode);
	valuefixedGreenMode=usifixedGreenMode;
	lcd240x128.DISPLAY_GRAPHIC_XY(fixedGreenMode.X,fixedGreenMode.Y,word8x16[valuefixedGreenMode],fixedGreenMode.height,fixedGreenMode.width/8);

} catch (...) {}
}
//---------------------------------------------------------------------------

void ScreenVDActPhaseEdit::DoKey0Work(void)
{
try {
	if(smem.GetcFace()==cVDActPhaseEdit){
    if(cSelect==0){
        clearCursor8x16(fixedGreenMode.X,fixedGreenMode.Y+16);
        cSelect=1;
        valuefixedGreenMode = 0;
        lcd240x128.DISPLAY_GRAPHIC_XY(fixedGreenMode.X,fixedGreenMode.Y,word8x16[valuefixedGreenMode],fixedGreenMode.height,fixedGreenMode.width/8);
        setCursor8x16(positionSwitch[0].X,positionSwitch[0].Y+16);
    }else if(cSelect==1){
    clearCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);
    valueSwitch[cPosition]=0;
    lcd240x128.DISPLAY_GRAPHIC_XY(positionSwitch[cPosition].X,positionSwitch[cPosition].Y,word8x16[valueSwitch[cPosition]],positionSwitch[cPosition].height,positionSwitch[cPosition].width/8);
    if (cPosition<8) cPosition++;
    if(cPosition>=8){cSelect=2;cPosition=7;}
    setCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);
    if(cSelect==2){cPosition=8;}
    }else if(cSelect ==2){
        if(cPosition==8){
            clearCursor8x16(positionSwitch[cPosition-1].X,positionSwitch[cPosition-1].Y+16);
            cPosition=0;
        }else{
            clearCursor8x16(positionExtendSec[cPosition].X,positionExtendSec[cPosition].Y+16);
        }
        valExtendSec[cPosition]=0;
        lcd240x128.DISPLAY_GRAPHIC_XY(positionExtendSec[cPosition].X,positionExtendSec[cPosition].Y,word8x16[valExtendSec[cPosition]],positionExtendSec[cPosition].height,positionExtendSec[cPosition].width/8);
        if(cPosition<8)cPosition++;
        if(cPosition>=8){ cPosition=7;}

        setCursor8x16(positionExtendSec[cPosition].X,positionExtendSec[cPosition].Y+16);
    }
	}
  } catch (...) {}
}

//---------------------------------------------------------------------------
void ScreenVDActPhaseEdit::DoKey1Work(void)
{
try {
	if(smem.GetcFace()==cVDActPhaseEdit){
    if(cSelect==0){
        clearCursor8x16(fixedGreenMode.X,fixedGreenMode.Y+16);
        cSelect=1;
        valuefixedGreenMode = 1;
        lcd240x128.DISPLAY_GRAPHIC_XY(fixedGreenMode.X,fixedGreenMode.Y,word8x16[valuefixedGreenMode],fixedGreenMode.height,fixedGreenMode.width/8);
        setCursor8x16(positionSwitch[0].X,positionSwitch[0].Y+16);
    }else if(cSelect==1){
    clearCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);
    valueSwitch[cPosition]=1;
    if(cPosition>=8){cSelect=2;cPosition=7;}
    lcd240x128.DISPLAY_GRAPHIC_XY(positionSwitch[cPosition].X,positionSwitch[cPosition].Y,word8x16[valueSwitch[cPosition]],positionSwitch[cPosition].height,positionSwitch[cPosition].width/8);
    if (cPosition<8) cPosition++;
    setCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);
    if(cSelect==2){cPosition=8;}
    }else if(cSelect ==2){
        if(cPosition==8){
            clearCursor8x16(positionSwitch[cPosition-1].X,positionSwitch[cPosition-1].Y+16);
            cPosition=0;
        }else{
            clearCursor8x16(positionExtendSec[cPosition].X,positionExtendSec[cPosition].Y+16);
        }
        valExtendSec[cPosition]=1;
        lcd240x128.DISPLAY_GRAPHIC_XY(positionExtendSec[cPosition].X,positionExtendSec[cPosition].Y,word8x16[valExtendSec[cPosition]],positionExtendSec[cPosition].height,positionExtendSec[cPosition].width/8);
        if(cPosition<8)cPosition++;
        if(cPosition>=8){ cPosition=7;}

        setCursor8x16(positionExtendSec[cPosition].X,positionExtendSec[cPosition].Y+16);
    }
	}
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenVDActPhaseEdit::DoKey2Work(void)
{
try {
	if(smem.GetcFace()==cVDActPhaseEdit){
    if(cSelect==0){
        clearCursor8x16(fixedGreenMode.X,fixedGreenMode.Y+16);
        cSelect=1;
        setCursor8x16(positionSwitch[0].X,positionSwitch[0].Y+16);
    }else if(cSelect==1){
    clearCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);
    valueSwitch[cPosition]=2;
    lcd240x128.DISPLAY_GRAPHIC_XY(positionSwitch[cPosition].X,positionSwitch[cPosition].Y,word8x16[valueSwitch[cPosition]],positionSwitch[cPosition].height,positionSwitch[cPosition].width/8);
    if (cPosition<8) cPosition++;
    if(cPosition>=8){cSelect=2;cPosition=7;}
    setCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);
    if(cSelect==2){cPosition=8;}
    }else if(cSelect ==2){
        if(cPosition==8){
            clearCursor8x16(positionSwitch[cPosition-1].X,positionSwitch[cPosition-1].Y+16);
            cPosition=0;
        }else{
            clearCursor8x16(positionExtendSec[cPosition].X,positionExtendSec[cPosition].Y+16);
        }
        valExtendSec[cPosition]=2;
        lcd240x128.DISPLAY_GRAPHIC_XY(positionExtendSec[cPosition].X,positionExtendSec[cPosition].Y,word8x16[valExtendSec[cPosition]],positionExtendSec[cPosition].height,positionExtendSec[cPosition].width/8);
        if(cPosition<8)cPosition++;
        if(cPosition>=8){ cPosition=7;}

        setCursor8x16(positionExtendSec[cPosition].X,positionExtendSec[cPosition].Y+16);
    }
	}
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenVDActPhaseEdit::DoKey3Work(void)
{
try {
	if(smem.GetcFace()==cVDActPhaseEdit){
    if(cSelect==0){
        clearCursor8x16(fixedGreenMode.X,fixedGreenMode.Y+16);
        cSelect=1;
        setCursor8x16(positionSwitch[0].X,positionSwitch[0].Y+16);
    }else if(cSelect==1){
    clearCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);
    valueSwitch[cPosition]=3;
    lcd240x128.DISPLAY_GRAPHIC_XY(positionSwitch[cPosition].X,positionSwitch[cPosition].Y,word8x16[valueSwitch[cPosition]],positionSwitch[cPosition].height,positionSwitch[cPosition].width/8);
    if (cPosition<8) cPosition++;
    if(cPosition>=8){cSelect=2;cPosition=7;}
    setCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);
    if(cSelect==2){cPosition=8;}
    }else if(cSelect ==2){
        if(cPosition==8){
            clearCursor8x16(positionSwitch[cPosition-1].X,positionSwitch[cPosition-1].Y+16);
            cPosition=0;
        }else{
            clearCursor8x16(positionExtendSec[cPosition].X,positionExtendSec[cPosition].Y+16);
        }
        valExtendSec[cPosition]=3;
        lcd240x128.DISPLAY_GRAPHIC_XY(positionExtendSec[cPosition].X,positionExtendSec[cPosition].Y,word8x16[valExtendSec[cPosition]],positionExtendSec[cPosition].height,positionExtendSec[cPosition].width/8);
        if(cPosition<8)cPosition++;
        if(cPosition>=8){ cPosition=7;}

        setCursor8x16(positionExtendSec[cPosition].X,positionExtendSec[cPosition].Y+16);
    }
	}
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenVDActPhaseEdit::DoKey4Work(void)
{
try {
	if(smem.GetcFace()==cVDActPhaseEdit){
    if(cSelect==0){
        clearCursor8x16(fixedGreenMode.X,fixedGreenMode.Y+16);
        cSelect=1;
        setCursor8x16(positionSwitch[0].X,positionSwitch[0].Y+16);
    }else if(cSelect==1){
    clearCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);
    valueSwitch[cPosition]=4;
    lcd240x128.DISPLAY_GRAPHIC_XY(positionSwitch[cPosition].X,positionSwitch[cPosition].Y,word8x16[valueSwitch[cPosition]],positionSwitch[cPosition].height,positionSwitch[cPosition].width/8);
    if (cPosition<8) cPosition++;
    if(cPosition>=8){cSelect=2;cPosition=7;}
    setCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);
    if(cSelect==2){cPosition=8;}
    }else if(cSelect ==2){
        if(cPosition==8){
            clearCursor8x16(positionSwitch[cPosition-1].X,positionSwitch[cPosition-1].Y+16);
            cPosition=0;
        }else{
            clearCursor8x16(positionExtendSec[cPosition].X,positionExtendSec[cPosition].Y+16);
        }
        valExtendSec[cPosition]=4;
        lcd240x128.DISPLAY_GRAPHIC_XY(positionExtendSec[cPosition].X,positionExtendSec[cPosition].Y,word8x16[valExtendSec[cPosition]],positionExtendSec[cPosition].height,positionExtendSec[cPosition].width/8);
        if(cPosition<8)cPosition++;
        if(cPosition>=8){ cPosition=7;}

        setCursor8x16(positionExtendSec[cPosition].X,positionExtendSec[cPosition].Y+16);
    }
	}
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenVDActPhaseEdit::DoKey5Work(void)
{
try {
	if(smem.GetcFace()==cVDActPhaseEdit){
    if(cSelect==0){
        clearCursor8x16(fixedGreenMode.X,fixedGreenMode.Y+16);
        cSelect=1;
        setCursor8x16(positionSwitch[0].X,positionSwitch[0].Y+16);
    }else if(cSelect==1){
    clearCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);
    valueSwitch[cPosition]=5;
    lcd240x128.DISPLAY_GRAPHIC_XY(positionSwitch[cPosition].X,positionSwitch[cPosition].Y,word8x16[valueSwitch[cPosition]],positionSwitch[cPosition].height,positionSwitch[cPosition].width/8);
    if (cPosition<8) cPosition++;
    if(cPosition>=8){cSelect=2;cPosition=7;}
    setCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);
    if(cSelect==2){cPosition=8;}
    }else if(cSelect ==2){
        if(cPosition==8){
            clearCursor8x16(positionSwitch[cPosition-1].X,positionSwitch[cPosition-1].Y+16);
            cPosition=0;
        }else{
            clearCursor8x16(positionExtendSec[cPosition].X,positionExtendSec[cPosition].Y+16);
        }
        valExtendSec[cPosition]=5;
        lcd240x128.DISPLAY_GRAPHIC_XY(positionExtendSec[cPosition].X,positionExtendSec[cPosition].Y,word8x16[valExtendSec[cPosition]],positionExtendSec[cPosition].height,positionExtendSec[cPosition].width/8);
        if(cPosition<8)cPosition++;
        if(cPosition>=8){ cPosition=7;}

        setCursor8x16(positionExtendSec[cPosition].X,positionExtendSec[cPosition].Y+16);
    }
	}
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenVDActPhaseEdit::DoKey6Work(void)
{
try {
	if(smem.GetcFace()==cVDActPhaseEdit){
    if(cSelect==0){
        clearCursor8x16(fixedGreenMode.X,fixedGreenMode.Y+16);
        cSelect=1;
        setCursor8x16(positionSwitch[0].X,positionSwitch[0].Y+16);
    }else if(cSelect==1){
    clearCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);
    valueSwitch[cPosition]=6;
    lcd240x128.DISPLAY_GRAPHIC_XY(positionSwitch[cPosition].X,positionSwitch[cPosition].Y,word8x16[valueSwitch[cPosition]],positionSwitch[cPosition].height,positionSwitch[cPosition].width/8);
    if (cPosition<8) cPosition++;
    if(cPosition>=8){cSelect=2;cPosition=7;}
    setCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);
    if(cSelect==2){cPosition=8;}
    }else if(cSelect ==2){
        if(cPosition==8){
            clearCursor8x16(positionSwitch[cPosition-1].X,positionSwitch[cPosition-1].Y+16);
            cPosition=0;
        }else{
            clearCursor8x16(positionExtendSec[cPosition].X,positionExtendSec[cPosition].Y+16);
        }
        valExtendSec[cPosition]=6;
        lcd240x128.DISPLAY_GRAPHIC_XY(positionExtendSec[cPosition].X,positionExtendSec[cPosition].Y,word8x16[valExtendSec[cPosition]],positionExtendSec[cPosition].height,positionExtendSec[cPosition].width/8);
        if(cPosition<8)cPosition++;
        if(cPosition>=8){ cPosition=7;}

        setCursor8x16(positionExtendSec[cPosition].X,positionExtendSec[cPosition].Y+16);
    }
	}
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenVDActPhaseEdit::DoKey7Work(void)
{
try {
	if(smem.GetcFace()==cVDActPhaseEdit){
    if(cSelect==0){
        clearCursor8x16(fixedGreenMode.X,fixedGreenMode.Y+16);
        cSelect=1;
        setCursor8x16(positionSwitch[0].X,positionSwitch[0].Y+16);
    }else if(cSelect==1){
    clearCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);
    valueSwitch[cPosition]=7;
    lcd240x128.DISPLAY_GRAPHIC_XY(positionSwitch[cPosition].X,positionSwitch[cPosition].Y,word8x16[valueSwitch[cPosition]],positionSwitch[cPosition].height,positionSwitch[cPosition].width/8);
    if (cPosition<8) cPosition++;
    if(cPosition>=8){cSelect=2;cPosition=7;}
    setCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);
    if(cSelect==2){cPosition=8;}
    }else if(cSelect ==2){
        if(cPosition==8){
            clearCursor8x16(positionSwitch[cPosition-1].X,positionSwitch[cPosition-1].Y+16);
            cPosition=0;
        }else{
            clearCursor8x16(positionExtendSec[cPosition].X,positionExtendSec[cPosition].Y+16);
        }
        valExtendSec[cPosition]=7;
        lcd240x128.DISPLAY_GRAPHIC_XY(positionExtendSec[cPosition].X,positionExtendSec[cPosition].Y,word8x16[valExtendSec[cPosition]],positionExtendSec[cPosition].height,positionExtendSec[cPosition].width/8);
        if(cPosition<8)cPosition++;
        if(cPosition>=8){ cPosition=7;}

        setCursor8x16(positionExtendSec[cPosition].X,positionExtendSec[cPosition].Y+16);
    }
	}
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenVDActPhaseEdit::DoKey8Work(void)
{
try {
	if(smem.GetcFace()==cVDActPhaseEdit){
    if(cSelect==0){
        clearCursor8x16(fixedGreenMode.X,fixedGreenMode.Y+16);
        cSelect=1;
        setCursor8x16(positionSwitch[0].X,positionSwitch[0].Y+16);
    }else if(cSelect==1){
    clearCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);
    valueSwitch[cPosition]=8;
    lcd240x128.DISPLAY_GRAPHIC_XY(positionSwitch[cPosition].X,positionSwitch[cPosition].Y,word8x16[valueSwitch[cPosition]],positionSwitch[cPosition].height,positionSwitch[cPosition].width/8);
    if (cPosition<8) cPosition++;
    if(cPosition>=8){cSelect=2;cPosition=7;}
    setCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);
    if(cSelect==2){cPosition=8;}
    }else if(cSelect ==2){
        if(cPosition==8){
            clearCursor8x16(positionSwitch[cPosition-1].X,positionSwitch[cPosition-1].Y+16);
            cPosition=0;
        }else{
            clearCursor8x16(positionExtendSec[cPosition].X,positionExtendSec[cPosition].Y+16);
        }
        valExtendSec[cPosition]=8;
        lcd240x128.DISPLAY_GRAPHIC_XY(positionExtendSec[cPosition].X,positionExtendSec[cPosition].Y,word8x16[valExtendSec[cPosition]],positionExtendSec[cPosition].height,positionExtendSec[cPosition].width/8);
        if(cPosition<8)cPosition++;
        if(cPosition>=8){ cPosition=7;}

        setCursor8x16(positionExtendSec[cPosition].X,positionExtendSec[cPosition].Y+16);
    }
	}
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenVDActPhaseEdit::DoKey9Work(void)
{
try {
	if(smem.GetcFace()==cVDActPhaseEdit){
    if(cSelect==0){
        clearCursor8x16(fixedGreenMode.X,fixedGreenMode.Y+16);
        cSelect=1;
        setCursor8x16(positionSwitch[0].X,positionSwitch[0].Y+16);
    }else if(cSelect==1){
    clearCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);
    valueSwitch[cPosition]=9;
    lcd240x128.DISPLAY_GRAPHIC_XY(positionSwitch[cPosition].X,positionSwitch[cPosition].Y,word8x16[valueSwitch[cPosition]],positionSwitch[cPosition].height,positionSwitch[cPosition].width/8);
    if (cPosition<8) cPosition++;
    if(cPosition>=8){cSelect=2;cPosition=7;}
    setCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);
    if(cSelect==2){cPosition=8;}
    }else if(cSelect ==2){
        if(cPosition==8){
            clearCursor8x16(positionSwitch[cPosition-1].X,positionSwitch[cPosition-1].Y+16);
            cPosition=0;
        }else{
            clearCursor8x16(positionExtendSec[cPosition].X,positionExtendSec[cPosition].Y+16);
        }
        valExtendSec[cPosition]=9;
        lcd240x128.DISPLAY_GRAPHIC_XY(positionExtendSec[cPosition].X,positionExtendSec[cPosition].Y,word8x16[valExtendSec[cPosition]],positionExtendSec[cPosition].height,positionExtendSec[cPosition].width/8);
        if(cPosition<8)cPosition++;
        if(cPosition>=8){ cPosition=7;}

        setCursor8x16(positionExtendSec[cPosition].X,positionExtendSec[cPosition].Y+16);
    }
	}
  } catch (...) {}
}

//---------------------------------------------------------------------------
void ScreenVDActPhaseEdit::DoKeyAWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenVDActPhaseEdit::DoKeyBWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenVDActPhaseEdit::DoKeyCWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenVDActPhaseEdit::DoKeyDWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenVDActPhaseEdit::DoKeyEWork(void)
{

}
//---------------------------------------------------------------------------
void ScreenVDActPhaseEdit::DoKeyFWork(void)
{


}
//---------------------------------------------------------------------------
void ScreenVDActPhaseEdit::DoKeyF1Work(void)
{
try {
	if(smem.GetcFace()==cVDActPhaseEdit){
    screenMain.DisplayMain();
	}
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenVDActPhaseEdit::DoKeyF2Work(void)
{
	for(int i=0;i<8;i++){
		valueSwitch[i]=0;
		lcd240x128.DISPLAY_GRAPHIC_XY(positionSwitch[i].X,positionSwitch[i].Y,word8x16[valueSwitch[i]],positionSwitch[i].height,positionSwitch[i].width/8);
	}
	for(int i=0;i<8;i++){
         valExtendSec[i]=0;
         lcd240x128.DISPLAY_GRAPHIC_XY(positionExtendSec[i].X,positionExtendSec[i].Y,word8x16[valExtendSec[i]],positionExtendSec[i].height,positionExtendSec[i].width/8);
	}
}
//---------------------------------------------------------------------------
void ScreenVDActPhaseEdit::DoKeyF3Work(void)
{
	if(smem.GetcFace()==cVDActPhaseGreen){
		char tmp[100];
		for(int i=0;i<8;i++){
	        sprintf(tmp,"PhaseTriggerSwitch%d",i);
			smem.vSetVDPhaseTriggerSwitch(i,valueSwitch[i]);
	        oStorINI.vSaveIntToINI("VDTrigger", tmp, valueSwitch[i]);
	        printf("p[%d]=%d ",i,smem.vGetVDPhaseTriggerSwitch(i));
		}

		smem.vSetBOOLData(TC_CCT_In_LongTanu_ActuateType_FunctionEnable, true/*bTMP_ActuateSwitch*/);
		smem.vSetUSIData(TC_CCT_In_LongTanu_ActuateType_Switch,smem.vGetUSIData(TC_CCT_In_LongTanu_ActuateType_Switch) /*usiActuatePlan*/);
		smem.vSetActuatePhaseExtend(/*ucActuatePhaseID*/0);
		smem.vSetUSIData(TC_ActuateVDID, /*usiActuateVDID*/11111);
		smem.vSetUCData(TC_CCT_ActuateType_By_TOD, /*ucActuateType_By_TOD*/false);
	    smem.vSetBOOLData(TCSegmentTypeUpdate, true);

		printf("\n");
		SaveVDdevFile();

		system("sync");
		system("sync");
		system("killall -9 act_mode");
		system("/cct/act_mode >/dev/tty5 &");

		//screenVDPhaseMapTableEdit.DisplayVDPhaseMapTableEdit(0);
		screenActuateStrategyMenu.DisplayActuateStrategyMenu();
		}
}
//---------------------------------------------------------------------------
void ScreenVDActPhaseEdit::DoKeyF4Work(void)
{
try {
	screenActuateStrategyMenu.DisplayActuateStrategyMenu();
	//screenControlStrategyMenu.DisplayControlStrategyMenu();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenVDActPhaseEdit::DoKeyEnterWork(void)
{
try {

	if(smem.GetcFace()==cVDActPhaseEdit){
		DisplayActuatePhaseGreenMode();
	}


  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenVDActPhaseEdit::DoKeyUPWork(void)
{
try {
    if(cSelect==0){
        clearCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);
        setCursor8x16(fixedGreenMode.X,fixedGreenMode.Y+16);
    }else if(cSelect==1){
    clearCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);
    //if (cPosition>=7) {cSelect=1;cPosition=7;}
    //setCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);
    setCursor8x16(fixedGreenMode.X,fixedGreenMode.Y+16);
    cSelect=0;
    cPosition=0;
    }else if(cSelect ==2){
        clearCursor8x16(positionExtendSec[cPosition].X,positionExtendSec[cPosition].Y+16);
        cSelect=1;
        cPosition=0;
        setCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenVDActPhaseEdit::DoKeyDOWNWork(void)
{
try {
    if(cSelect==0){
        clearCursor8x16(fixedGreenMode.X,fixedGreenMode.Y+16);
        setCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);
        cSelect=1;
    }else if(cSelect==1){
    clearCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);
    cSelect=2;
    setCursor8x16(positionExtendSec[cPosition].X,positionExtendSec[cPosition].Y+16);
    }
    else if(cSelect ==2){
        clearCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);
        cPosition=0;
        setCursor8x16(positionExtendSec[cPosition].X,positionExtendSec[cPosition].Y+16);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenVDActPhaseEdit::DoKeyLEFTWork(void)
{
try {
    if(cSelect==0){
        clearCursor8x16(fixedGreenMode.X,fixedGreenMode.Y+16);
        setCursor8x16(fixedGreenMode.X,fixedGreenMode.Y+16);
        cPosition=0;
    }else if(cSelect==1){
        clearCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);
        //if (cPosition==0) cPosition=1;

        if(cPosition<1){
            cSelect=0;
            clearCursor8x16(positionSwitch[0].X,positionSwitch[0].Y+16);
            setCursor8x16(fixedGreenMode.X,fixedGreenMode.Y+16);
            cPosition=0;
        }else{
        cPosition--;
        setCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);
        }
    }else if(cSelect ==2){
        clearCursor8x16(positionExtendSec[cPosition].X,positionExtendSec[cPosition].Y+16);
        cPosition--;
        if(cPosition<0){
            clearCursor8x16(positionExtendSec[0].X,positionExtendSec[0].Y+16);
            cSelect=1;
            cPosition=7;
            setCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);

        }else{
            setCursor8x16(positionExtendSec[cPosition].X,positionExtendSec[cPosition].Y+16);
        }

    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenVDActPhaseEdit::DoKeyRIGHTWork(void)
{
try {
    if(cSelect==0){
        clearCursor8x16(fixedGreenMode.X,fixedGreenMode.Y+16);
        cPosition =0 ;
        setCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);
        cSelect=1;
    }else if(cSelect==1){
    clearCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);
    cPosition++;
    if (cPosition>=7) {cSelect=2; cPosition=7;}
        setCursor8x16(positionSwitch[cPosition].X,positionSwitch[cPosition].Y+16);
        if(cSelect==2){cPosition=-1;}
    }else if(cSelect==2){
    	if(cPosition==0){clearCursor8x16(positionSwitch[7].X,positionSwitch[7].Y+16);}
    	clearCursor8x16(positionExtendSec[cPosition].X,positionExtendSec[cPosition].Y+16);
    	cPosition++;
    	if(cPosition>=7) {cPosition=7;}
    	setCursor8x16(positionExtendSec[cPosition].X,positionExtendSec[cPosition].Y+16);
    }

  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenVDActPhaseEdit::ReadVDdevFile()
{
	//---------------------------------------------------
		ConfigFile config( "/cct/vddata/dev.cfg" );
		// Values can be read from the file by name
		int iTmp = 0;
		if(config.readInto( iTmp, "ACTTYPE" )){
			printf("Read The ACTTYPE of is:%d\n",iTmp);
			usiActType=iTmp;
		}
        char tmpfile[50];
        for(int i=0;i<8;i++){
            sprintf(tmpfile,"TIMEXTEND%d",i+1);
            if(config.readInto(iTmp,tmpfile)){
                printf("Read The TIMEXTEND%d of is :%d\n",i+1,iTmp);
                usiExtendSec[i] = iTmp;
            }
        }

        if(smem.vGetUCData(fixedGreenModeOpen)>0){
            usifixedGreenMode = 1;
        }else{
            usifixedGreenMode = 0;
        }


	//---------------------------------------------------

}
void ScreenVDActPhaseEdit::SaveVDdevFile()
{
	//---------------------------------------------------

		ConfigFile config( "/cct/vddata/dev.cfg" );
		// Values can be read from the file by name
		if(valuefixedGreenMode==0){
            config.add( "ACTTYPE", 0 );//半觸動
            printf("Set ACTTYPE is:%d",0);
            config.writeCf();
            smem.vSetUCData(fixedGreenModeOpen,0);//固定綠模式關閉
            oStorINI.vSaveIntToINI("CCTSwitch","fixedGreenModeOpen",0);
            oStorINI.vSaveIntToINI("CCTSwitch", "OpenCompensationAct", 1);
            smem.cPedPushButton.SetCompensationAct(1);//compensationAct
		}else{
		    config.add( "ACTTYPE", 3 );//固定綠
            printf("Set ACTTYPE is:%d",3);
            config.writeCf();
            smem.vSetUCData(fixedGreenModeOpen,1);//固定綠模式開啟
            oStorINI.vSaveIntToINI("CCTSwitch","fixedGreenModeOpen",1);
            oStorINI.vSaveIntToINI("CCTSwitch", "OpenCompensationAct", 0);
            smem.cPedPushButton.SetCompensationAct(0);//compensationAct
		}

		char tmp_file[50];
		for(int i=0;i<8;i++){
            sprintf(tmp_file,"TIMEXTEND%d",i+1);
            config.add( tmp_file, valExtendSec[i] );//延長秒數
            printf("Set TIMEXTEND%d is:%d",i,valExtendSec[i]);
            config.writeCf();
		}

	//---------------------------------------------------

}
//---------------------------------------------------------------------------


