//---------------------------------------------------------------------------
#include "SCREENOperMode.h"
#include "SCREENMain.h"
#include "SCREENCtlSetup.h"
#include "LCD240x128.h"
#include "SMEM.h"

SCREENOperMode screenOperMode;
//---------------------------------------------------------------------------
SCREENOperMode::SCREENOperMode(void)
{
    LoadBitmapFromFile();
}
//---------------------------------------------------------------------------
SCREENOperMode::~SCREENOperMode(void)
{
}
//---------------------------------------------------------------------------
void SCREENOperMode::DoKeyWork(BYTE key)              //已過濾,進來的是0x80~0x98之間,5X5按鍵
{
try {
    switch (key) {
        case 0x90:
          DoKeyF1Work();
        break;
        case 0x93:
          DoKeyF4Work();
        break;
        default:
        break;
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENOperMode::LoadBitmapFromFile(void)
{
try {
    FILE *bitmap;
    bitmap=fopen("//cct//bitmap//backGround//4_1_9.bit","rb");
    if (bitmap) {
        fread(operModeBitmap,3840,1,bitmap);
        fclose(bitmap);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENOperMode::DisplayOperMode(void)
{
try {
    smem.SetcFace(cOPERMODE);
    smem.vWriteMsgToDOM("Enter Operate Mode Screen");
    lcd240x128.DISPLAY_GRAPHIC(0,operModeBitmap,128,30);
    UpdateOperMode();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENOperMode::UpdateOperMode(void)
{
    if (smem.GetcFace()==cOPERMODE) {
        if (smem.GetOperMode()==1)              //遠端操作
            lcd240x128.DISPLAY_GRAPHIC_XY(112,7,word24x24[1],24,24/8);
        else if (smem.GetOperMode()==0)        //現場操作
            lcd240x128.DISPLAY_GRAPHIC_XY(112,7,word24x24[2],24,24/8);
    }
}
//---------------------------------------------------------------------------
void SCREENOperMode::DoKeyF1Work(void)
{
    screenMain.DisplayMain();
}
//---------------------------------------------------------------------------
void SCREENOperMode::DoKeyF4Work(void)
{
    screenCtlSetup.DisplayCtlSetup();
}
//---------------------------------------------------------------------------
