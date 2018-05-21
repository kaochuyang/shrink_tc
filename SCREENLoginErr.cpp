#include "SCREENLoginErr.h"
#include "SCREENLogo.h"
#include "SCREENLogin.h"
#include "LCD240x128.h"
#include "SMEM.h"

SCREENLoginErr screenLoginErr;
//---------------------------------------------------------------------------
SCREENLoginErr::SCREENLoginErr(void)
{
    LoadBitmapFromFile();
}
//---------------------------------------------------------------------------
SCREENLoginErr::~SCREENLoginErr(void)
{
}
//---------------------------------------------------------------------------
void SCREENLoginErr::DisplayLoginErr(void)
{
try {
    smem.SetcFace(cLOGINERR);
    smem.vWriteMsgToDOM("Enter Login Error Screen");
    lcd240x128.DISPLAY_GRAPHIC(0,loginErrBitmap,128,30);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENLoginErr::DoKeyWork(BYTE key)   //已過濾,進來的是0x80~0x98之間
{
try {
    switch (key) {
        case 0x90:     //ESC,F1
          DoKeyEscWork();
        break;
        case 0x98:     //ENTER
          DoKeyEnterWork();
        break;
        default:
        break;
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENLoginErr::LoadBitmapFromFile(void)
{
try {
    FILE *bitmap;
    bitmap=fopen("//cct//bitmap//backGround//2_passworderr.bit","rb");
    if (bitmap) {
        fread(loginErrBitmap,3840,1,bitmap);
        fclose(bitmap);
    }
  } catch (...) {}
}

//---------------------------------------------------------------------------
void SCREENLoginErr::DoKeyEscWork(void)
{
try {
    screenLogo.DisplayLogo();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENLoginErr::DoKeyEnterWork(void)
{
try {
    screenLogin.DisplayLogin();
  } catch (...) {}
}
//---------------------------------------------------------------------------
