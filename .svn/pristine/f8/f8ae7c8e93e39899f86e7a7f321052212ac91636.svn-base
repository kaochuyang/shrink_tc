//---------------------------------------------------------------------------
#include "SCREENOffline.h"
#include "LCD240x128.h"
#include "SCREENMain.h"
#include "SMEM.h"
#include "SCREENLogo.h"

SCREENOffline screenOffline;
//---------------------------------------------------------------------------
SCREENOffline::SCREENOffline(void)
{
    LoadBitmapFromFile();
}
//---------------------------------------------------------------------------
SCREENOffline::~SCREENOffline(void)
{
}
//---------------------------------------------------------------------------
void SCREENOffline::DoKeyWork(BYTE key)                                            //已過濾,進來的是0x80~0x98之間,5X5按鍵
{
try {
    switch (key) {
        case 0x90:
             DoKeyF1Work();
        break;
        case 0x98:
             DoKeyEnterWork();
        break;
        default:
        break;
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENOffline::LoadBitmapFromFile(void)
{
try {
    FILE *bitmap;
    bitmap=fopen("//cct//bitmap//backGround//2_password_offline.bit","rb");
    if (bitmap) {
        fread(offlineBitmap,3840,1,bitmap);
        fclose(bitmap);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENOffline::DisplayOffline(void)
{
try {
    smem.SetcFace(cOFFLINE);
    smem.vWriteMsgToDOM("Enter Offline Screen");
    lcd240x128.DISPLAY_GRAPHIC(0,offlineBitmap,128,30);
    setSelectPoint16x16(32,36,16,16/8);
    setSelectPoint16x16(32,56,16,16/8);
    DisplayOperMode();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENOffline::DisplayOperMode(void)
{
try {
    if (smem.GetcFace()==cOFFLINE) {
        //遠端操作為1,現場操作為0
        if (smem.GetOperMode()==0) setSelectPoint16x16(32,76,16,16/8);
        else clearSelectPoint16x16(32,76,16,16/8);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENOffline::DoKeyF1Work(void)
{
    screenLogo.DisplayLogo();
}
//---------------------------------------------------------------------------
void SCREENOffline::DoKeyEnterWork(void)
{
    if (smem.GetOperMode()==0) screenMain.DisplayMain();
}
//---------------------------------------------------------------------------
