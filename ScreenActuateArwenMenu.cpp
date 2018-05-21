/*
 * ScreenActuateArwenMenu.cpp
 *
 *  Created on: 2010/10/5
 *      Author: arwen
 */

#include "ScreenActuateArwenMenu.h"
#include "screenRunTableList.h"
#include "SCREENMain.h"
#include "screenActuateArwenManyStatus.h"
#include "screenActuateArwenManyStatus2.h"
#include "ScreenActuateArwenFlashManyStatus.h"
#include "SCREENPedSwitchStatus.h"
#include "screenActuateArwenVDInfo.h"
#include "screenActuateArwenStatus.h"
#include "LCD240x128.h"
#include "SMEM.h"
#include <stdio.h>
#include <stdlib.h>

ScreenActuateArwenMenu screenActuateArwenMenu;
//---------------------------------------------------------------------------
ScreenActuateArwenMenu::ScreenActuateArwenMenu() {
	// TODO Auto-generated constructor stub
	loadBitmapFromFile();
	initDispWord();
	cSelect=0;

}
//---------------------------------------------------------------------------
ScreenActuateArwenMenu::~ScreenActuateArwenMenu() {
	// TODO Auto-generated destructor stub
}
//---------------------------------------------------------------------------
void ScreenActuateArwenMenu::loadBitmapFromFile(void)
{
try {
    FILE *bitmap;
    bitmap=fopen("//cct//bitmap//backGround//actmenu//ActuateArwenMenu.bit","rb");
    if (bitmap) {
        fread(actuateArwenMenuBitmap,3840,1,bitmap);
        fclose(bitmap);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenActuateArwenMenu::DisplayActuateArwenMenu(void)
{
try {
	smem.SetcFace(cACTARWENMENU);
	smem.vWriteMsgToDOM("Display Actuate Arwen Menu Screen");
	lcd240x128.DISPLAY_GRAPHIC(0,actuateArwenMenuBitmap,128,30);
	setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);

  } catch (...) {}
}
void ScreenActuateArwenMenu::initDispWord(void)
{
try {
	for (int i=0;i<10;i++) {
		if (i>=0 && i<=4) {
			mark[i].X=24;
			mark[i].Y=28+i*16;
			//mark[i].Y=34+i*16;
		} else if (i>=5 && i<=9) {
			mark[i].X=112;
			mark[i].Y=28+(i-5)*16;
			//mark[i].Y=34+(i-5)*16;
		}
	        mark[i].width=16;
	        mark[i].height=16;
	}
}catch(...){}
}
//----------------------------------------------------------------------------
void ScreenActuateArwenMenu::DoKeyWork(BYTE key)                                        //¤w¹LÂo,¶i¨Óªº¬O0x80~0x98¤§¶¡
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
        case 0x8B:
        case 0x8C:
        case 0x8D:
        case 0x8E:
        case 0x8F:
        break;
        case 0x90://F1 ESC
          DoKeyF1Work();
        break;
        case 0x91:
          DoKeyF2Work();
        break;
        case 0x92:
        //OT Pass
          DoKeyF3Work();
        break;
        case 0x93:
          DoKeyF4Work();
        break;
        case 0x94:
          DoKeyUPWork();
        break;
        case 0x95:
          DoKeyDOWNWork();
        break;
        case 0x96:
          DoKeyLEFTWork();
        break;
        case 0x97:
          DoKeyRIGHTWork();
        break;
        case 0x98://ENTER
          DoKeyEnterWork();
        break;
        default:
        break;
    }
  } catch (...) {}
}
//----------------------------------------------------------------
//---------------------------------------------------------------------------
void ScreenActuateArwenMenu::DoKey0Work(void)
{

}
//---------------------------------------------------------------------------
void ScreenActuateArwenMenu::DoKey1Work(void)
{
    //screenActuateArwenStatus.DisplayActuateArwenManyStatus();
	screenActuateArwenManyStatus.DisplayActuateArwenManyStatus();

}
//---------------------------------------------------------------------------
void ScreenActuateArwenMenu::DoKey2Work(void)
{
	screenActuateArwenFlashManyStatus.DisplayActuateArwenFlashManyStatus();
}
//---------------------------------------------------------------------------
void ScreenActuateArwenMenu::DoKey3Work(void)
{
    screenPedSwitchStatus.DisplayPedSwitchStatus();
}
//---------------------------------------------------------------------------
void ScreenActuateArwenMenu::DoKey4Work(void)
{
    screenActuateArwenVDINFO.DisplayArwenDetectorInfo();
}
//---------------------------------------------------------------------------
void ScreenActuateArwenMenu::DoKey5Work(void)
{
    screenActuateArwenManyStatus.DisplayActuateArwenManyStatus();
    //screenActuateArwenStatus.DisplayActuateArwenStatus();
}
//---------------------------------------------------------------------------
void ScreenActuateArwenMenu::DoKey6Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenActuateArwenMenu::DoKey7Work(void)
{

}
//---------------------------------------------------------------------------
void ScreenActuateArwenMenu::DoKey8Work(void)
{

}

//OT Debug 0523
//---------------------------------------------------------------------------
void ScreenActuateArwenMenu::DoKey9Work(void)
{

}

//---------------------------------------------------------------------------
void ScreenActuateArwenMenu::DoKeyF1Work(void)
{
	screenMain.DisplayMain();
}

//---------------------------------------------------------------------------
void ScreenActuateArwenMenu::DoKeyF2Work(void)
{

}

//---------------------------------------------------------------------------
void ScreenActuateArwenMenu::DoKeyF3Work(void)
{

}
//---------------------------------------------------------------------------
void ScreenActuateArwenMenu::DoKeyF4Work(void)
{
	screenRunTableList.DisplayRunTableList();
}
//---------------------------------------------------------------------------
void ScreenActuateArwenMenu::DoKeyEnterWork(void)
{
    switch (cSelect+1) {
        case 1:
          DoKey1Work();
        break;
        case 2:
          DoKey2Work();
        break;
        case 3:
          DoKey3Work();
        break;
        case 4:
          DoKey4Work();
        break;
        case 5:
          DoKey5Work();
        break;
        case 6:
          DoKey6Work();
        break;
        case 7:
          DoKey7Work();
        break;
        case 8:
          DoKey8Work();
        break;
        case 9:
          DoKey9Work();
        break;
        default:
        break;
    }
}
//---------------------------------------------------------------------------
void ScreenActuateArwenMenu::DoKeyUPWork(void)
{
try {
    clearSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    if (cSelect==0) cSelect=1;
    cSelect--;
    setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenActuateArwenMenu::DoKeyDOWNWork(void)
{
try {
    clearSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    cSelect++;
    if (cSelect>=4) cSelect=4;
//    if (cSelect>=8) cSelect=8;
    setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenActuateArwenMenu::DoKeyLEFTWork(void)
{
try {
    clearSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    //if (cSelect>=5 && cSelect<=9)
        //cSelect-=5;
    setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenActuateArwenMenu::DoKeyRIGHTWork(void)
{
try {
    clearSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    //if (cSelect>=0 && cSelect<=4)
        //cSelect+=5;
    setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
  } catch (...) {}
}
//---------------------------------------------------------------------------
