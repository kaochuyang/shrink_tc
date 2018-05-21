#include "screenRunTableList.h"
#include "LCD240x128.h"
//#include "screenLogo.h"
#include "SCREENMain.h"
#include "screenStrategy.h"

#include "screenSegtype.h"

#include "screenCurrentLightStatus.h"
#include "screenPlan.h"
#include "CSTC.h"

#include "SCREENTFDStatus.h"

#include "screenChainStatus.h"

//#include "screenActuateArwenStatus.h"
//#include "screenActuateArwenManyStatus.h"
#include "ScreenActuateArwenMenu.h"
#include "SCREENSystemInfoMenu.h"
#include "screenPrioritymenu.h"

#include <stdio.h>

#include "SMEM.h"
//---------------------------------------------------------------------------
ScreenRunTableList screenRunTableList;
//---------------------------------------------------------------------------
ScreenRunTableList::ScreenRunTableList(void)
{
    loadBitmapFromFile();
    initDispWord();
    cSelect=0;
}
//---------------------------------------------------------------------------
ScreenRunTableList::~ScreenRunTableList(void)
{
}
//---------------------------------------------------------------------------
void ScreenRunTableList::loadBitmapFromFile(void)
{
try {
    FILE *bitmap;
    //bitmap=fopen("//cct//bitmap//backGround//TC5F//currentRunTable.bit","rb");//arwen modify
    bitmap=fopen("//cct//bitmap//backGround//actmenu//currentRunTable.bit","rb");
    if (bitmap) {
        fread(runTabkeListBitmap,3840,1,bitmap);
        fclose(bitmap);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenRunTableList::DisplayRunTableList(void)
{
try {
    smem.SetcFace(cRUNTABLE);
    lcd240x128.DISPLAY_GRAPHIC(0,runTabkeListBitmap,128,30);
    cSelect=0;
    setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenRunTableList::initDispWord(void)
{
try {
    for (int i=0;i<10;i++) {
         if (i>=0 && i<=4) {
             mark[i].X=0;
             mark[i].Y=30+i*16;
         } else if (i>=5 && i<=9) {
             mark[i].X=116;
             mark[i].Y=30+(i-5)*16;
         }
         mark[i].width=16;
         mark[i].height=16;
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenRunTableList::doKeyWork(BYTE key)
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
void ScreenRunTableList::doKey0Work(void)
{

}
//---------------------------------------------------------------------------
void ScreenRunTableList::doKey1Work(void)
{
    screenStrategy.DisplayStrategy();
}
//---------------------------------------------------------------------------
void ScreenRunTableList::doKey2Work(void)
{
      screenCurrentLightStatus.DisplayCurrentLightStatus();
}
//---------------------------------------------------------------------------
void ScreenRunTableList::doKey3Work(void)
{
    unsigned short int x = stc.vGetUSIData(CSTC_exec_segment_type);//Modify Arwen++
    if (x<=20) screenSegtype.DisplaySegtype(x, cRUNTABLE);
}
//---------------------------------------------------------------------------
void ScreenRunTableList::doKey4Work(void)
{
    unsigned short int x = stc.vGetUSIData(CSTC_exec_plan_plan_ID);
    screenPlan.DisplayPlan(x, cRUNTABLE);
}
//---------------------------------------------------------------------------
void ScreenRunTableList::doKey5Work(void)
{
    screenChainStatus.DisplayChainStatus();
}
//---------------------------------------------------------------------------
void ScreenRunTableList::doKey6Work(void)
{
    screenTFDStatus.DisplayTFDStatus();
}
//---------------------------------------------------------------------------
void ScreenRunTableList::doKey7Work(void)
{
    //screenActuateArwenStatus.DisplayActuateArwenStatus();
	screenActuateArwenMenu.DisplayActuateArwenMenu();

}
//---------------------------------------------------------------------------
void ScreenRunTableList::doKey8Work(void)
{
	screenSystemInfoMenu.DisplaySystemInfoMenu();
}
//---------------------------------------------------------------------------
void ScreenRunTableList::doKey9Work(void)
{
    screenPrioritymenu.DisplayPrioritymenu();
}
//---------------------------------------------------------------------------
void ScreenRunTableList::doKeyAWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenRunTableList::doKeyBWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenRunTableList::doKeyCWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenRunTableList::doKeyDWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenRunTableList::doKeyEWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenRunTableList::doKeyFWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenRunTableList::doKeyF1Work(void)
{
//    screenLogo.DisplayLogo();
    screenMain.DisplayMain();
}
//---------------------------------------------------------------------------
void ScreenRunTableList::doKeyF2Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenRunTableList::doKeyF3Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenRunTableList::doKeyF4Work(void)
{
//    screenMain.DisplayMain();
    screenMain.DisplayMain();
}
//---------------------------------------------------------------------------
void ScreenRunTableList::doKeyUPWork(void)
{
try {
    clearSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    if (cSelect==0) cSelect=1;
    cSelect--;
    setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenRunTableList::doKeyDOWNWork(void)
{
try {
    clearSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    cSelect++;
    if (cSelect>=9) cSelect=9;
    setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenRunTableList::doKeyLEFTWork(void)
{
try {
    clearSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    if (cSelect>=5 && cSelect<=9)
        cSelect-=5;
    setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenRunTableList::doKeyRIGHTWork(void)
{
try {
    clearSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    if (cSelect>=0 && cSelect<=5)
        cSelect+=5;
    setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenRunTableList::doKeyEnterWork(void)
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

