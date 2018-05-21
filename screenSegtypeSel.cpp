#include "screenSegtypeSel.h"
#include "LCD240x128.h"
#include "SCREENMain.h"
#include "screenSegtypeMenu.h"
#include "screenSegtype.h"
#include <stdio.h>

#include "SMEM.h"
//---------------------------------------------------------------------------
ScreenSegtypeSel screenSegtypeSel;
//---------------------------------------------------------------------------
ScreenSegtypeSel::ScreenSegtypeSel(void)
{
    loadBitmapFromFile();
    initDispWord();
    cPosition=0;
}
//---------------------------------------------------------------------------
ScreenSegtypeSel::~ScreenSegtypeSel(void)
{
}
//---------------------------------------------------------------------------
void ScreenSegtypeSel::loadBitmapFromFile(void)
{
try {
    FILE *bitmap;
    bitmap=fopen("//cct//bitmap//backGround//TC5F//segtypeSelect.bit","rb");
    if (bitmap) {
        fread(segtypeSelBitmap,3840,1,bitmap);
        fclose(bitmap);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenSegtypeSel::DisplaySegtypeSel(void)
{
try {
    smem.SetcFace(cSEGTYPESEL);
    lcd240x128.DISPLAY_GRAPHIC(0,segtypeSelBitmap,128,30);
    DisplaySetSegtype(0,0);  DisplaySetSegtype(0,0);
    cPosition=0;
    setCursor24x24(segSel[cPosition].X,segSel[cPosition].Y+24);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenSegtypeSel::initDispWord(void)
{
    for (int i=0;i<2;i++) {
         segSel[i].X=96+i*24;
         segSel[i].Y=68;
         segSel[i].width=24;
         segSel[i].height=24;
    }
}
//---------------------------------------------------------------------------
void ScreenSegtypeSel::DisplaySetSegtype(int a,int b)
{
try {
    lcd240x128.DISPLAY_GRAPHIC_XY(segSel[0].X,segSel[0].Y,word24x24[a],segSel[0].height,segSel[0].width/8);
    segTemp[0]=a;
    lcd240x128.DISPLAY_GRAPHIC_XY(segSel[1].X,segSel[1].Y,word24x24[b],segSel[1].height,segSel[1].width/8);
    segTemp[1]=b;
  } catch (...){}
}
//---------------------------------------------------------------------------
void ScreenSegtypeSel::doKeyWork(BYTE key)
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
void ScreenSegtypeSel::doKey0Work(void)
{
try {
    lcd240x128.DISPLAY_GRAPHIC_XY(segSel[cPosition].X,segSel[cPosition].Y,word24x24[0],segSel[cPosition].height,segSel[cPosition].width/8);
    segTemp[cPosition]=0;
    doKeyRIGHTWork();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenSegtypeSel::doKey1Work(void)
{
    lcd240x128.DISPLAY_GRAPHIC_XY(segSel[cPosition].X,segSel[cPosition].Y,word24x24[1],segSel[cPosition].height,segSel[cPosition].width/8);
    segTemp[cPosition]=0x01;
    doKeyRIGHTWork();
}
//---------------------------------------------------------------------------
void ScreenSegtypeSel::doKey2Work(void)
{
    lcd240x128.DISPLAY_GRAPHIC_XY(segSel[cPosition].X,segSel[cPosition].Y,word24x24[2],segSel[cPosition].height,segSel[cPosition].width/8);
    segTemp[cPosition]=0x02;
    doKeyRIGHTWork();
}
//---------------------------------------------------------------------------
void ScreenSegtypeSel::doKey3Work(void)
{
    lcd240x128.DISPLAY_GRAPHIC_XY(segSel[cPosition].X,segSel[cPosition].Y,word24x24[3],segSel[cPosition].height,segSel[cPosition].width/8);
    segTemp[cPosition]=0x03;
    doKeyRIGHTWork();
}
//---------------------------------------------------------------------------
void ScreenSegtypeSel::doKey4Work(void)
{
    lcd240x128.DISPLAY_GRAPHIC_XY(segSel[cPosition].X,segSel[cPosition].Y,word24x24[4],segSel[cPosition].height,segSel[cPosition].width/8);
    segTemp[cPosition]=0x04;
    doKeyRIGHTWork();
}
//---------------------------------------------------------------------------
void ScreenSegtypeSel::doKey5Work(void)
{
    lcd240x128.DISPLAY_GRAPHIC_XY(segSel[cPosition].X,segSel[cPosition].Y,word24x24[5],segSel[cPosition].height,segSel[cPosition].width/8);
    segTemp[cPosition]=0x05;
    doKeyRIGHTWork();
}
//---------------------------------------------------------------------------
void ScreenSegtypeSel::doKey6Work(void)
{
    lcd240x128.DISPLAY_GRAPHIC_XY(segSel[cPosition].X,segSel[cPosition].Y,word24x24[6],segSel[cPosition].height,segSel[cPosition].width/8);
    segTemp[cPosition]=0x06;
    doKeyRIGHTWork();
}
//---------------------------------------------------------------------------
void ScreenSegtypeSel::doKey7Work(void)
{
    lcd240x128.DISPLAY_GRAPHIC_XY(segSel[cPosition].X,segSel[cPosition].Y,word24x24[7],segSel[cPosition].height,segSel[cPosition].width/8);
    segTemp[cPosition]=0x07;
    doKeyRIGHTWork();
}
//---------------------------------------------------------------------------
void ScreenSegtypeSel::doKey8Work(void)
{
    lcd240x128.DISPLAY_GRAPHIC_XY(segSel[cPosition].X,segSel[cPosition].Y,word24x24[8],segSel[cPosition].height,segSel[cPosition].width/8);
    segTemp[cPosition]=0x08;
    doKeyRIGHTWork();
}
//---------------------------------------------------------------------------
void ScreenSegtypeSel::doKey9Work(void)
{
    lcd240x128.DISPLAY_GRAPHIC_XY(segSel[cPosition].X,segSel[cPosition].Y,word24x24[9],segSel[cPosition].height,segSel[cPosition].width/8);
    segTemp[cPosition]=0x09;
    doKeyRIGHTWork();
}
//---------------------------------------------------------------------------
void ScreenSegtypeSel::doKeyAWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenSegtypeSel::doKeyBWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenSegtypeSel::doKeyCWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenSegtypeSel::doKeyDWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenSegtypeSel::doKeyEWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenSegtypeSel::doKeyFWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenSegtypeSel::doKeyF1Work(void)
{
    screenMain.DisplayMain();
}
//---------------------------------------------------------------------------
void ScreenSegtypeSel::doKeyF2Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenSegtypeSel::doKeyF3Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenSegtypeSel::doKeyF4Work(void)
{
    screenSegtypeMenu.DisplaySegtypeMenu();
}
//---------------------------------------------------------------------------
void ScreenSegtypeSel::doKeyUPWork(void)
{
    int x=segTemp[0]*10+segTemp[1]+1;
    if (x==100) x=99;
    DisplaySetSegtype(x/10,x%10);
}
//---------------------------------------------------------------------------
void ScreenSegtypeSel::doKeyDOWNWork(void)
{
    if (segTemp[0]*10+segTemp[1]==0) segTemp[1]=1;
    int x=segTemp[0]*10+segTemp[1]-1;
    DisplaySetSegtype(x/10,x%10);
}
//---------------------------------------------------------------------------
void ScreenSegtypeSel::doKeyLEFTWork(void)
{
try {
    clearCursor24x24(segSel[cPosition].X,segSel[cPosition].Y+24);
    if (cPosition==0) cPosition=1;
    cPosition--;
    setCursor24x24(segSel[cPosition].X,segSel[cPosition].Y+24);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenSegtypeSel::doKeyRIGHTWork(void)
{
try {
    clearCursor24x24(segSel[cPosition].X,segSel[cPosition].Y+24);
    cPosition++;
    if (cPosition>=2) cPosition=1;
    setCursor24x24(segSel[cPosition].X,segSel[cPosition].Y+24);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenSegtypeSel::doKeyEnterWork(void)
{
    int x=segTemp[0]*10+segTemp[1];
    if (x>=0 && x<=20) screenSegtype.DisplaySegtype(x, cSEGTYPESEL);
}
//---------------------------------------------------------------------------


