#include "screenChainMenu.h"

#include "screenMotherChainSet.h"
#include "screenChildChainSet.h"
// #include "screenSpecialDay.h"
// #include "screenSegtypeSel.h"

#include "screenRailwayChainSet.h"

#include <stdio.h>

#include "LCD240x128.h"
#include "SCREENMain.h"
#include "SMEM.h"

//---------------------------------------------------------------------------
ScreenChainMenu screenChainMenu;
//---------------------------------------------------------------------------
ScreenChainMenu::ScreenChainMenu(void)
{
    loadBitmapFromFile();
    initDispWord();
    cSelect=0;
}
//---------------------------------------------------------------------------
ScreenChainMenu::~ScreenChainMenu(void)
{
}

//---------------------------------------------------------------------------
void ScreenChainMenu::loadBitmapFromFile(void)
{
try {
    FILE *bitmap;
    bitmap=fopen("//cct//bitmap//backGround//TC5F//ChainMenu.bit","rb");
    if (bitmap) {
        fread(chainMenuBitmap,3840,1,bitmap);
        fclose(bitmap);
    }
  } catch (...) {}
}

//---------------------------------------------------------------------------
void ScreenChainMenu::DisplayChainMenu(void)
{
try {
    smem.SetcFace(cCHAINMENU);
    lcd240x128.DISPLAY_GRAPHIC(0,chainMenuBitmap,128,30);
    cSelect=0;
    setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenChainMenu::initDispWord(void)
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
void ScreenChainMenu::doKeyWork(BYTE key)
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
void ScreenChainMenu::doKey0Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenChainMenu::doKey1Work(void)
{
    screenMotherChainSet.DisplayMotherChainSet();
}
//---------------------------------------------------------------------------
void ScreenChainMenu::doKey2Work(void)
{
    screenChildChainSet.DisplayChildChainSet();
}
//---------------------------------------------------------------------------
void ScreenChainMenu::doKey3Work(void)
{
    screenRailwayChainSet.DisplayRailwayChainSet();
//    screenSegtypeSel.DisplaySegtypeSel();
}
//---------------------------------------------------------------------------
void ScreenChainMenu::doKey4Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenChainMenu::doKey5Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenChainMenu::doKey6Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenChainMenu::doKey7Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenChainMenu::doKey8Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenChainMenu::doKey9Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenChainMenu::doKeyAWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenChainMenu::doKeyBWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenChainMenu::doKeyCWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenChainMenu::doKeyDWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenChainMenu::doKeyEWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenChainMenu::doKeyFWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenChainMenu::doKeyF1Work(void)
{
    screenMain.DisplayMain();
}
//---------------------------------------------------------------------------
void ScreenChainMenu::doKeyF2Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenChainMenu::doKeyF3Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenChainMenu::doKeyF4Work(void)
{
    screenMain.DisplayMain();
}
//---------------------------------------------------------------------------
void ScreenChainMenu::doKeyUPWork(void)
{
try {
    clearSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    if (cSelect==0) cSelect=1;
    cSelect--;
    setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenChainMenu::doKeyDOWNWork(void)
{
try {
    clearSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    cSelect++;
    if (cSelect>=9) cSelect=9;
    setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenChainMenu::doKeyLEFTWork(void)
{
try {
    clearSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    if (cSelect>=5 && cSelect<=9)
        cSelect-=5;
    setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenChainMenu::doKeyRIGHTWork(void)
{
try {
    clearSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    if (cSelect>=0 && cSelect<=5)
        cSelect+=5;
    setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenChainMenu::doKeyEnterWork(void)
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
