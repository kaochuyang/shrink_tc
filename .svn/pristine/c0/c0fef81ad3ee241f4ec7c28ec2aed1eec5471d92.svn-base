#include "screenReverseMenu.h"

#include <stdio.h>

#include "screenRevWeekDay.h"
#include "screenRevSpecialDay.h"
#include "CSTC.h"

#include "LCD240x128.h"
#include "SCREENMain.h"
#include "SMEM.h"

//---------------------------------------------------------------------------
ScreenReverseMenu screenReverseMenu;
//---------------------------------------------------------------------------
ScreenReverseMenu::ScreenReverseMenu(void)
{
    loadBitmapFromFile();
    initDispWord();
    cSelect=0;
}
//---------------------------------------------------------------------------
ScreenReverseMenu::~ScreenReverseMenu(void)
{
}
//---------------------------------------------------------------------------
void ScreenReverseMenu::loadBitmapFromFile(void)
{
try {
    FILE *bitmap;
    bitmap=fopen("//cct//bitmap//backGround//TC5F//reverseMenu.bit","rb");
    if (bitmap) {
        fread(reverseMenuBitmap,3840,1,bitmap);
        fclose(bitmap);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenReverseMenu::DisplayReverseMenu(void)
{
try {
    smem.SetcFace(cREVERSETIMTMENU);
    lcd240x128.DISPLAY_GRAPHIC(0,reverseMenuBitmap,128,30);
    cSelect=0;
    setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenReverseMenu::initDispWord(void)
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

    for(int i = 0; i < 5; i++) {
      markStepSecCount[i].width = 8;
      markStepSecCount[i].height = 16;
      markStepSecCount[i].X = 48+i*8;
      markStepSecCount[i].Y = 104;
    }
    for(int i = 0; i < 2; i++) {
      markStepID[i].width = 8;
      markStepID[i].height = 16;
      markStepID[i].X = 24 + i*8;
      markStepID[i].Y = 104;
    }

  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenReverseMenu::doKeyWork(BYTE key)
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
void ScreenReverseMenu::doKey0Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenReverseMenu::doKey1Work(void)
{
    screenRevWeekDay.DisplayRevWeekDay();
}
//---------------------------------------------------------------------------
void ScreenReverseMenu::doKey2Work(void)
{
    screenRevSpecialDay.DisplaySpecialDay();
}
//---------------------------------------------------------------------------
void ScreenReverseMenu::doKey3Work(void)
{
    stc.vResetReverseTime();
}
//---------------------------------------------------------------------------
void ScreenReverseMenu::doKey4Work(void)
{
  smem.vSetUCData(TC_ReverseLane_Manual_Enable_Status, 0);                      //Close
}
//---------------------------------------------------------------------------
void ScreenReverseMenu::doKey5Work(void)
{
  smem.vSetUCData(TC_ReverseLane_Manual_Enable_Status, 1);                      //Open
}
//---------------------------------------------------------------------------
void ScreenReverseMenu::doKey6Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenReverseMenu::doKey7Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenReverseMenu::doKey8Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenReverseMenu::doKey9Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenReverseMenu::doKeyAWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenReverseMenu::doKeyBWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenReverseMenu::doKeyCWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenReverseMenu::doKeyDWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenReverseMenu::doKeyEWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenReverseMenu::doKeyFWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenReverseMenu::doKeyF1Work(void)
{
    screenMain.DisplayMain();
}
//---------------------------------------------------------------------------
void ScreenReverseMenu::doKeyF2Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenReverseMenu::doKeyF3Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenReverseMenu::doKeyF4Work(void)
{
    screenMain.DisplayMain();
}
//---------------------------------------------------------------------------
void ScreenReverseMenu::doKeyUPWork(void)
{
try {
    clearSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    if (cSelect==0) cSelect=1;
    cSelect--;
    setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenReverseMenu::doKeyDOWNWork(void)
{
try {
    clearSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    cSelect++;
    if (cSelect>=9) cSelect=9;
    setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenReverseMenu::doKeyLEFTWork(void)
{
try {
    clearSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    if (cSelect>=5 && cSelect<=9)
        cSelect-=5;
    setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenReverseMenu::doKeyRIGHTWork(void)
{
try {
    clearSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    if (cSelect>=0 && cSelect<=5)
        cSelect+=5;
    setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenReverseMenu::doKeyEnterWork(void)
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
void ScreenReverseMenu::vRefreshStepSec(void)
{
try {
  int i1, i2, i3, i4, i5;
  unsigned int uiStepSec = stc.vGetRevTimerSec();

  if(uiStepSec < 100000) {
    i1  = uiStepSec / 10000;
    i2  = (uiStepSec % 10000) / 1000;
    i3  = (uiStepSec % 1000) / 100;
    i4  = (uiStepSec % 100) / 10;
    i5  = (uiStepSec % 10);

    lcd240x128.DISPLAY_GRAPHIC_XY(markStepSecCount[0].X,markStepSecCount[0].Y,word8x16[i1],markStepSecCount[0].height,markStepSecCount[0].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(markStepSecCount[1].X,markStepSecCount[1].Y,word8x16[i2],markStepSecCount[1].height,markStepSecCount[1].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(markStepSecCount[2].X,markStepSecCount[2].Y,word8x16[i3],markStepSecCount[2].height,markStepSecCount[2].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(markStepSecCount[3].X,markStepSecCount[3].Y,word8x16[i4],markStepSecCount[3].height,markStepSecCount[3].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(markStepSecCount[4].X,markStepSecCount[4].Y,word8x16[i5],markStepSecCount[4].height,markStepSecCount[4].width/8);
  }

  stc.vGetRevInfo(&usiLocalRevStep, &_LocalRev);
  i1 = usiLocalRevStep / 10;
  i2 = usiLocalRevStep % 10;
  lcd240x128.DISPLAY_GRAPHIC_XY(markStepID[0].X,markStepID[0].Y,word8x16[i1],markStepID[0].height,markStepID[0].width/8);
  lcd240x128.DISPLAY_GRAPHIC_XY(markStepID[1].X,markStepID[1].Y,word8x16[i2],markStepID[1].height,markStepID[1].width/8);


}catch(...){}
}

