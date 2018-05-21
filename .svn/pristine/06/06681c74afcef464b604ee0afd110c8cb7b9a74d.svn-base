#include "screenDirLightBoardMappingMenu.h"
#include "LCD240x128.h"
#include "SCREENMain.h"
#include <stdio.h>

#include "SMEM.h"

#include "SCREENDirMap.h"
#include "SCREENLightBoardMap.h"
#include "SCREENCtlSetup.h"
//---------------------------------------------------------------------------
ScreenDirLightBoardMappingMenu screenDirLightBoardMappingMenu;
//---------------------------------------------------------------------------
ScreenDirLightBoardMappingMenu::ScreenDirLightBoardMappingMenu(void)
{
    loadBitmapFromFile();
    initDispWord();
    cSelect=0;
}
//---------------------------------------------------------------------------
ScreenDirLightBoardMappingMenu::~ScreenDirLightBoardMappingMenu(void)
{
}
//---------------------------------------------------------------------------
void ScreenDirLightBoardMappingMenu::loadBitmapFromFile(void)
{
try {
    FILE *bitmap;
    bitmap=fopen("//cct//bitmap//backGround//dirLightBoardMappingMenu.bit","rb");
    if (bitmap) {
        fread(dirLightBoardMappingMenuBitmap,3840,1,bitmap);
        fclose(bitmap);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenDirLightBoardMappingMenu::DisplayDirLightBoardMappingMenu(void)
{
try {
    smem.SetcFace(cDIRLIGHTBOARDMAPPINGMENU);
    lcd240x128.DISPLAY_GRAPHIC(0,dirLightBoardMappingMenuBitmap,128,30);
    cSelect=0;
    setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenDirLightBoardMappingMenu::initDispWord(void)
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
void ScreenDirLightBoardMappingMenu::doKeyWork(BYTE key)
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
void ScreenDirLightBoardMappingMenu::doKey0Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenDirLightBoardMappingMenu::doKey1Work(void)
{
//    screenRedCountHWCheckSel.DisplayRedCountHWCheckSel();
    screenDirMap.DisplayDirMapping();
}
//---------------------------------------------------------------------------
void ScreenDirLightBoardMappingMenu::doKey2Work(void)
{
//    screenRedCountHWVerMenu.DisplayRedCountHWVerMenu();
    screenLightBoardMap.DisplayDirMapping();
}
//---------------------------------------------------------------------------
void ScreenDirLightBoardMappingMenu::doKey3Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenDirLightBoardMappingMenu::doKey4Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenDirLightBoardMappingMenu::doKey5Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenDirLightBoardMappingMenu::doKey6Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenDirLightBoardMappingMenu::doKey7Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenDirLightBoardMappingMenu::doKey8Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenDirLightBoardMappingMenu::doKey9Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenDirLightBoardMappingMenu::doKeyAWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenDirLightBoardMappingMenu::doKeyBWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenDirLightBoardMappingMenu::doKeyCWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenDirLightBoardMappingMenu::doKeyDWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenDirLightBoardMappingMenu::doKeyEWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenDirLightBoardMappingMenu::doKeyFWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenDirLightBoardMappingMenu::doKeyF1Work(void)
{
    screenMain.DisplayMain();
}
//---------------------------------------------------------------------------
void ScreenDirLightBoardMappingMenu::doKeyF2Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenDirLightBoardMappingMenu::doKeyF3Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenDirLightBoardMappingMenu::doKeyF4Work(void)
{
//arwen modify    screenMain.DisplayMain();
    screenCtlSetup.DisplayCtlSetup();
}
//---------------------------------------------------------------------------
void ScreenDirLightBoardMappingMenu::doKeyUPWork(void)
{
try {
    clearSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    if (cSelect==0) cSelect=1;
    cSelect--;
    setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenDirLightBoardMappingMenu::doKeyDOWNWork(void)
{
try {
    clearSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    cSelect++;
    if (cSelect>=9) cSelect=9;
    setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenDirLightBoardMappingMenu::doKeyLEFTWork(void)
{
try {
    clearSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    if (cSelect>=5 && cSelect<=9)
        cSelect-=5;
    setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenDirLightBoardMappingMenu::doKeyRIGHTWork(void)
{
try {
    clearSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    if (cSelect>=0 && cSelect<=5)
        cSelect+=5;
    setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenDirLightBoardMappingMenu::doKeyEnterWork(void)
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




