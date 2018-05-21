#include "SCREENSystemInfoMenu.h"
#include "LCD240x128.h"
//#include "screenLogo.h"
#include "SCREENMain.h"

#include "screenPlan.h"

#include "SCREENFWVerDT.h"
#include "SCREENCommStat.h"
#include "screenCurrentCommPacket.h"
#include "SCREENLastDown.h"

#include "CSTC.h"
#include <stdio.h>
#include "SMEM.h"

//---------------------------------------------------
SCREENSystemInfoMenu screenSystemInfoMenu;
//---------------------------------------------------
SCREENSystemInfoMenu::SCREENSystemInfoMenu()
{
    loadBitmapFromFile();
    initDispWord();
    cSelect=0;
}
SCREENSystemInfoMenu::~SCREENSystemInfoMenu()
{

}
//---------------------------------------------------------------------------
void SCREENSystemInfoMenu::loadBitmapFromFile(void)
{
try {
    FILE *bitmap;
    bitmap=fopen("//cct//bitmap//backGround//actmenu//systemInfo.bit","rb");
    if (bitmap) {
        fread(systemInfoBitmap,3840,1,bitmap);
        fclose(bitmap);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENSystemInfoMenu::DisplaySystemInfoMenu(void)
{
try {
    smem.SetcFace(cSYSTEMINFO);
    lcd240x128.DISPLAY_GRAPHIC(0,systemInfoBitmap,128,30);
    cSelect=0;
    setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENSystemInfoMenu::initDispWord(void)
{
try {
    for (int i=0;i<10;i++) {
         if (i>=0 && i<=4) {
             mark[i].X=24;
             mark[i].Y=32+i*16;
         } else if (i>=5 && i<=9) {
             mark[i].X=128;
             mark[i].Y=32+(i-5)*16;
         }
         mark[i].width=16;
         mark[i].height=16;
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENSystemInfoMenu::doKeyWork(BYTE key)
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
void SCREENSystemInfoMenu::doKey0Work(void)
{
}
//---------------------------------------------------------------------------
void SCREENSystemInfoMenu::doKey1Work(void)
{
    screenFWVerDT.DisplayFWVerDT();//韌體版本日期 ok
}
//---------------------------------------------------------------------------
void SCREENSystemInfoMenu::doKey2Work(void)
{
    screenCommStat.DisplayCommStat();//通訊狀況 ok
}
//---------------------------------------------------------------------------
void SCREENSystemInfoMenu::doKey3Work(void)
{
    screenCurrentCommPacket.DisplayCurrentCommPacket();//通訊封包 ok
}
//---------------------------------------------------------------------------
void SCREENSystemInfoMenu::doKey4Work(void)
{
    screenLastDown.DisplayLastDown();//上次斷電時間 ok
}
//---------------------------------------------------------------------------
void SCREENSystemInfoMenu::doKey5Work(void)
{

}
//---------------------------------------------------------------------------
void SCREENSystemInfoMenu::doKey6Work(void)
{

}
//---------------------------------------------------------------------------
void SCREENSystemInfoMenu::doKey7Work(void)
{

}
//---------------------------------------------------------------------------
void SCREENSystemInfoMenu::doKey8Work(void)
{
}
//---------------------------------------------------------------------------
void SCREENSystemInfoMenu::doKey9Work(void)
{
}
//---------------------------------------------------------------------------
void SCREENSystemInfoMenu::doKeyAWork(void)
{
}
//---------------------------------------------------------------------------
void SCREENSystemInfoMenu::doKeyBWork(void)
{
}
//---------------------------------------------------------------------------
void SCREENSystemInfoMenu::doKeyCWork(void)
{
}
//---------------------------------------------------------------------------
void SCREENSystemInfoMenu::doKeyDWork(void)
{
}
//---------------------------------------------------------------------------
void SCREENSystemInfoMenu::doKeyEWork(void)
{
}
//---------------------------------------------------------------------------
void SCREENSystemInfoMenu::doKeyFWork(void)
{
}
//---------------------------------------------------------------------------
void SCREENSystemInfoMenu::doKeyF1Work(void)
{
//    screenLogo.DisplayLogo();
    screenMain.DisplayMain();
}
//---------------------------------------------------------------------------
void SCREENSystemInfoMenu::doKeyF2Work(void)
{
}
//---------------------------------------------------------------------------
void SCREENSystemInfoMenu::doKeyF3Work(void)
{
}
//---------------------------------------------------------------------------
void SCREENSystemInfoMenu::doKeyF4Work(void)
{
//    screenMain.DisplayMain();
    screenMain.DisplayMain();
}
//---------------------------------------------------------------------------
void SCREENSystemInfoMenu::doKeyUPWork(void)
{
try {
    clearSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    if (cSelect==0) cSelect=1;
    cSelect--;
    setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENSystemInfoMenu::doKeyDOWNWork(void)
{
try {
    clearSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    cSelect++;
    if (cSelect>=9) cSelect=9;
    setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENSystemInfoMenu::doKeyLEFTWork(void)
{
try {
    clearSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    if (cSelect>=5 && cSelect<=9)
        cSelect-=5;
    setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENSystemInfoMenu::doKeyRIGHTWork(void)
{
try {
    clearSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    if (cSelect>=0 && cSelect<=5)
        cSelect+=5;
    setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENSystemInfoMenu::doKeyEnterWork(void)
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
