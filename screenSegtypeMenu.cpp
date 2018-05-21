#include "screenSegtypeMenu.h"
#include "screenWeekDay.h"
#include "screenSpecialDay.h"
#include "screenSegtypeSel.h"
#include <stdio.h>

#include "LCD240x128.h"
#include "SCREENMain.h"
#include "SCREENControlStrategyMenu.h"
#include "SMEM.h"

//---------------------------------------------------------------------------
ScreenSegtypeMenu screenSegtypeMenu;
//---------------------------------------------------------------------------
ScreenSegtypeMenu::ScreenSegtypeMenu(void)
{
    loadBitmapFromFile();
    initDispWord();
    cSelect=0;
}
//---------------------------------------------------------------------------
ScreenSegtypeMenu::~ScreenSegtypeMenu(void)
{
}
//---------------------------------------------------------------------------
void ScreenSegtypeMenu::loadBitmapFromFile(void)
{
try {
    FILE *bitmap;
    bitmap=fopen("//cct//bitmap//backGround//TC5F//segtypeMenu.bit","rb");
    if (bitmap) {
        fread(segtypeMenuBitmap,3840,1,bitmap);
        fclose(bitmap);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenSegtypeMenu::DisplaySegtypeMenu(void)
{
try {
    smem.SetcFace(cSEGTYPEMENU);
    lcd240x128.DISPLAY_GRAPHIC(0,segtypeMenuBitmap,128,30);
    cSelect=0;
    setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenSegtypeMenu::initDispWord(void)
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
void ScreenSegtypeMenu::doKeyWork(BYTE key)
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
void ScreenSegtypeMenu::doKey0Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenSegtypeMenu::doKey1Work(void)
{
    screenWeekDay.DisplayWeekDay();
}
//---------------------------------------------------------------------------
void ScreenSegtypeMenu::doKey2Work(void)
{
    screenSpecialDay.DisplaySpecialDay();
}
//---------------------------------------------------------------------------
void ScreenSegtypeMenu::doKey3Work(void)
{
    screenSegtypeSel.DisplaySegtypeSel();
}
//---------------------------------------------------------------------------
void ScreenSegtypeMenu::doKey4Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenSegtypeMenu::doKey5Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenSegtypeMenu::doKey6Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenSegtypeMenu::doKey7Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenSegtypeMenu::doKey8Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenSegtypeMenu::doKey9Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenSegtypeMenu::doKeyAWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenSegtypeMenu::doKeyBWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenSegtypeMenu::doKeyCWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenSegtypeMenu::doKeyDWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenSegtypeMenu::doKeyEWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenSegtypeMenu::doKeyFWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenSegtypeMenu::doKeyF1Work(void)
{
    screenMain.DisplayMain();
}
//---------------------------------------------------------------------------
void ScreenSegtypeMenu::doKeyF2Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenSegtypeMenu::doKeyF3Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenSegtypeMenu::doKeyF4Work(void)
{
    screenMain.DisplayMain();
//arwen modify 1001003 screenControlStrategyMenu.DisplayControlStrategyMenu();

}
//---------------------------------------------------------------------------
void ScreenSegtypeMenu::doKeyUPWork(void)
{
try {
    clearSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    if (cSelect==0) cSelect=1;
    cSelect--;
    setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenSegtypeMenu::doKeyDOWNWork(void)
{
try {
    clearSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    cSelect++;
    if (cSelect>=9) cSelect=9;
    setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenSegtypeMenu::doKeyLEFTWork(void)
{
try {
    clearSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    if (cSelect>=5 && cSelect<=9)
        cSelect-=5;
    setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenSegtypeMenu::doKeyRIGHTWork(void)
{
try {
    clearSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    if (cSelect>=0 && cSelect<=5)
        cSelect+=5;
    setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenSegtypeMenu::doKeyEnterWork(void)
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
