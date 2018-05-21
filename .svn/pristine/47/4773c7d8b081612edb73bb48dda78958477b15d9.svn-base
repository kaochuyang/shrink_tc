#include "screenRedCountMenu.h"
#include "LCD240x128.h"
#include "SCREENMain.h"
#include <stdio.h>

#include "CSTC.h"

#include "SMEM.h"

#include "screenRedCountHWCheckSel.h"
#include "screenRedCountHWVerMenu.h"
#include "SCREENRedCountMap.h"

#include "screenRedCountHWCheckDevID.h"
#include "screenRedCountHWCheckNum88.h"
#include "screenRedCountHWCheckCycleNum.h"
#include "SCREENCtlSetup.h"

//---------------------------------------------------------------------------
ScreenRedCountMenu screenRedCountMenu;
//---------------------------------------------------------------------------
ScreenRedCountMenu::ScreenRedCountMenu(void)
{
    loadBitmapFromFile();
    initDispWord();
    cSelect=0;
}
//---------------------------------------------------------------------------
ScreenRedCountMenu::~ScreenRedCountMenu(void)
{
}
//---------------------------------------------------------------------------
void ScreenRedCountMenu::loadBitmapFromFile(void)
{
try {
    FILE *bitmap;
    bitmap=fopen("//cct//bitmap//backGround//TC5F//redCountMenu.bit","rb");
    if (bitmap) {
        fread(redCountMenuBitmap,3840,1,bitmap);
        fclose(bitmap);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenRedCountMenu::DisplayRedCountMenu(void)
{
try {
    smem.SetcFace(cREDCOUNTMENU);
    lcd240x128.DISPLAY_GRAPHIC(0,redCountMenuBitmap,128,30);
    cSelect=0;
    setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenRedCountMenu::initDispWord(void)
{
try {
    for (int i=0;i<10;i++) {
         if (i>=0 && i<=4) {
             mark[i].X=24;
             mark[i].Y=24+i*16;
         } else if (i>=5 && i<=9) {
             mark[i].X=128;
             mark[i].Y=24+(i-5)*16;
         }
         mark[i].width=16;
         mark[i].height=16;
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenRedCountMenu::doKeyWork(BYTE key)
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
void ScreenRedCountMenu::doKey0Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenRedCountMenu::doKey1Work(void)
{
//    unsigned short int x = stc.vGetUSIData(CSTC_exec_plan_plan_ID);
//    screenPlan.DisplayPlan(x);
//    screenPlanSel.DisplayPlanSel();
    screenRedCountHWCheckSel.DisplayRedCountHWCheckSel();

}
//---------------------------------------------------------------------------
void ScreenRedCountMenu::doKey2Work(void)
{
    screenRedCountHWVerMenu.DisplayRedCountHWVerMenu();
}
//---------------------------------------------------------------------------
void ScreenRedCountMenu::doKey3Work(void)
{
    screenRedCountMap.DisplayRedCountMapping();
}
//---------------------------------------------------------------------------
void ScreenRedCountMenu::doKey4Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenRedCountMenu::doKey5Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenRedCountMenu::doKey6Work(void)
{
  screenRedCountHWCheckDevID.DisplayRedCountHWCheckDevID();
}
//---------------------------------------------------------------------------
void ScreenRedCountMenu::doKey7Work(void)
{
  screenRedCountHWCheckNum88.DisplayRedCountHWCheckNum88();
}
//---------------------------------------------------------------------------
void ScreenRedCountMenu::doKey8Work(void)
{
  screenRedCountHWCheckCycleNum.DisplayRedCountHWCheckCycleNum();
}
//---------------------------------------------------------------------------
void ScreenRedCountMenu::doKey9Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenRedCountMenu::doKeyAWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenRedCountMenu::doKeyBWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenRedCountMenu::doKeyCWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenRedCountMenu::doKeyDWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenRedCountMenu::doKeyEWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenRedCountMenu::doKeyFWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenRedCountMenu::doKeyF1Work(void)
{
    screenMain.DisplayMain();
}
//---------------------------------------------------------------------------
void ScreenRedCountMenu::doKeyF2Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenRedCountMenu::doKeyF3Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenRedCountMenu::doKeyF4Work(void)
{
    //screenMain.DisplayMain();
	screenCtlSetup.DisplayCtlSetup();
}
//---------------------------------------------------------------------------
void ScreenRedCountMenu::doKeyUPWork(void)
{
try {
    clearSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    if (cSelect==0) cSelect=1;
    cSelect--;
    setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenRedCountMenu::doKeyDOWNWork(void)
{
try {
    clearSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    cSelect++;
    if (cSelect>=9) cSelect=9;
    setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenRedCountMenu::doKeyLEFTWork(void)
{
try {
    clearSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    if (cSelect>=5 && cSelect<=9)
        cSelect-=5;
    setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenRedCountMenu::doKeyRIGHTWork(void)
{
try {
    clearSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    if (cSelect>=0 && cSelect<=5)
        cSelect+=5;
    setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenRedCountMenu::doKeyEnterWork(void)
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




