//---------------------------------------------------------------------------
#include "SCREENLogo.h"
#include "LCD240x128.h"
#include "SMEM.h"
#include "SCREENLogin.h"

#include <stdio.h>
#include <stdlib.h>

SCREENLogo screenLogo;
//---------------------------------------------------------------------------
SCREENLogo::SCREENLogo(void)
{
    LoadBitmapFromFile();
}
//---------------------------------------------------------------------------
SCREENLogo::~SCREENLogo(void)
{
}
//---------------------------------------------------------------------------
void SCREENLogo::DoKeyWork(BYTE key)              //已過濾,進來的是0x80~0x98之間,5X5按鍵
{
try {
    switch (key) {
        case 0x90:
          DoKeyEscWork();
        break;
        default:
          DoKeyDefaultWork();
        break;
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENLogo::LoadBitmapFromFile(void)
{
try {
    FILE *bitmap;
//    bitmap=fopen("//cct//bitmap//backGround//1_logo.bit","rb");
/*OTCombo */
    bitmap=fopen("//cct//bitmap//backGround//1_CCTlogo.bit","rb");
    if (bitmap) {
        fread(logoBitmap,3840,1,bitmap);
        fclose(bitmap);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENLogo::DisplayLogo(void)
{
try {
    smem.SetcFace(cLOGO);
    smem.vWriteMsgToDOM("Display Logo Screen");

    lcd240x128.DISPLAY_GRAPHIC(0, logoBitmap, 128, 30);

    /* disable
    int x=rand()%3;
    if (x==0)  lcd240x128.DISPLAY_GRAPHICFULL_SPECIAL1(logoBitmap);
    else if (x==1)  lcd240x128.DISPLAY_GRAPHICFULL_SPECIAL2(logoBitmap);
    else if (x==2)  lcd240x128.DISPLAY_GRAPHICFULL_SPECIAL3(logoBitmap);
    */

  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENLogo::DoKeyEscWork(void)
{
    lcd240x128.GRAPHIC_MODE();
    DisplayLogo();
}
//---------------------------------------------------------------------------
void SCREENLogo::DoKeyDefaultWork(void)
{
    screenLogin.DisplayLogin();
}
//---------------------------------------------------------------------------
