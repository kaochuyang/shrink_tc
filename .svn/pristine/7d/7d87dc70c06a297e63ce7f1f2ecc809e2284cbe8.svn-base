#include "screenRTMSInfo.h"
#include "LCD240x128.h"
#include "SCREENMain.h"
#include <stdio.h>

#include "SMEM.h"
//---------------------------------------------------------------------------
ScreenRTMSInfo screenRTMSInfo;
//---------------------------------------------------------------------------
ScreenRTMSInfo::ScreenRTMSInfo(void)
{
    loadBitmapFromFile();
}
//---------------------------------------------------------------------------
ScreenRTMSInfo::~ScreenRTMSInfo(void)
{
}
//---------------------------------------------------------------------------
void ScreenRTMSInfo::loadBitmapFromFile(void)
{
try {
    FILE *bitmap;
    bitmap=fopen("//cct//bitmap//backGround//TC5F//RTMSInfo.bit","rb");
    if (bitmap) {
        fread(rtmsInfoBitmap,3840,1,bitmap);
        fclose(bitmap);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenRTMSInfo::DisplayRTMSInfo(void)
{
try {
    smem.SetcFace(cRTMSInfo);
    BYTE clearTemp[72]={0};
    lcd240x128.DISPLAY_GRAPHIC(0,rtmsInfoBitmap,128,30);                        //畫上底圖
    lcd240x128.DISPLAY_GRAPHIC_XY(120,3,clearTemp,24,3);                        //畫上空白
  } catch (...) {}
}
//--------------------------------------------------------------------------
void ScreenRTMSInfo::DisplayCRTMSInfo(int state)
{
try {
    if (state==1)  lcd240x128.DISPLAY_GRAPHIC_XY(120,3,word24x24[10],24,3);     //A
    else if (state==2) lcd240x128.DISPLAY_GRAPHIC_XY(120,3,word24x24[11],24,3); //B
    else if (state==3) lcd240x128.DISPLAY_GRAPHIC_XY(120,3,word24x24[12],24,3); //C
    else if (state==4) lcd240x128.DISPLAY_GRAPHIC_XY(120,3,word24x24[13],24,3); //D
    else if (state==5) lcd240x128.DISPLAY_GRAPHIC_XY(120,3,word24x24[14],24,3); //E
    else if (state==6) lcd240x128.DISPLAY_GRAPHIC_XY(120,3,word24x24[15],24,3); //F
  } catch (...) {}
}
//--------------------------------------------------------------------------
void ScreenRTMSInfo::doKeyWork(BYTE key)
{
try {
    switch (key) {
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
void ScreenRTMSInfo::doKeyF1Work(void)
{
    screenMain.DisplayMain();
}
//---------------------------------------------------------------------------
void ScreenRTMSInfo::doKeyF2Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenRTMSInfo::doKeyF3Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenRTMSInfo::doKeyF4Work(void)
{
//    BYTE clearTemp[72]={0};
//    lcd240x128.DISPLAY_GRAPHIC_XY(120,3,clearTemp,24,3);
    screenMain.DisplayMain();
}
//---------------------------------------------------------------------------
void ScreenRTMSInfo::doKeyUPWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenRTMSInfo::doKeyDOWNWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenRTMSInfo::doKeyLEFTWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenRTMSInfo::doKeyRIGHTWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenRTMSInfo::doKeyEnterWork(void)
{
    screenMain.DisplayMain();
}
//---------------------------------------------------------------------------
