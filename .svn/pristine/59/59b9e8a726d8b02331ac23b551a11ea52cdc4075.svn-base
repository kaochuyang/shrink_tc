#include "SCREENLastDown.h"
#include "SCREENMain.h"
//#include "SCREENCtlSetup.h"
#include "SCREENSystemInfoMenu.h"
#include "LCD240x128.h"
#include "SMEM.h"

SCREENLastDown screenLastDown;
//---------------------------------------------------------------------------
SCREENLastDown::SCREENLastDown(void)
{
try {
    LoadBitmapFromFile();
    InitDispWord();
  } catch (...) {}
}
//---------------------------------------------------------------------------
SCREENLastDown::~SCREENLastDown(void)
{
}
//---------------------------------------------------------------------------
void SCREENLastDown::DoKeyWork(BYTE key)                                        //已過濾,進來的是0x80~0x98之間
{
try {
    switch (key) {
        case 0x80:
        case 0x81:
        case 0x82:
        case 0x83:
        case 0x84:
        case 0x85:
        case 0x86:
        case 0x87:
        case 0x88:
        case 0x89:
        case 0x8A:
        case 0x8B:
        case 0x8C:
        case 0x8D:
        case 0x8E:
        case 0x8F:
        case 0x91://F2
        case 0x92://F3
        break;

        case 0x90://F1,ESC
          DoKeyF1Work();
        break;
        case 0x93://F4
          DoKeyF4Work();
        break;

        case 0x94://UP
        case 0x95://DOWN
        case 0x96://LEFT
        case 0x97://RIGHT
        case 0x98://Enter
        break;
        default:
        break;
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENLastDown::LoadBitmapFromFile(void)
{
try {
    FILE *bitmap;
    bitmap=fopen("//cct//bitmap//backGround//4_1_2.bit","rb");
    if (bitmap) {
        fread(lastDownBitmap,3840,1,bitmap);
        fclose(bitmap);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENLastDown::DisplayLastDown(void)
{
try {
    smem.SetcFace(cLASTDOWN);
    smem.vWriteMsgToDOM("Enter Last Power Down Screen");
    lcd240x128.DISPLAY_GRAPHIC(0,lastDownBitmap,128,30);

    YMDHMS temp=smem.vGetLastResetTime();

    lcd240x128.DISPLAY_GRAPHIC_XY(poweroff[0].X,poweroff[0].Y,word16x16[temp.Month/10],poweroff[0].height,poweroff[0].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(poweroff[1].X,poweroff[1].Y,word16x16[temp.Month%10],poweroff[1].height,poweroff[1].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(poweroff[2].X,poweroff[2].Y,word16x16[temp.Day/10],poweroff[2].height,poweroff[2].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(poweroff[3].X,poweroff[3].Y,word16x16[temp.Day%10],poweroff[3].height,poweroff[3].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(poweroff[4].X,poweroff[4].Y,word16x16[temp.Hour/10],poweroff[4].height,poweroff[4].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(poweroff[5].X,poweroff[5].Y,word16x16[temp.Hour%10],poweroff[5].height,poweroff[5].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(poweroff[6].X,poweroff[6].Y,word16x16[temp.Min/10],poweroff[6].height,poweroff[6].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(poweroff[7].X,poweroff[7].Y,word16x16[temp.Min%10],poweroff[7].height,poweroff[7].width/8);

    lcd240x128.DISPLAY_GRAPHIC_XY(poweroff[0].X,poweroff[0].Y,word16x16[temp.Month/10],poweroff[0].height,poweroff[0].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(poweroff[1].X,poweroff[1].Y,word16x16[temp.Month%10],poweroff[1].height,poweroff[1].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(poweroff[2].X,poweroff[2].Y,word16x16[temp.Day/10],poweroff[2].height,poweroff[2].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(poweroff[3].X,poweroff[3].Y,word16x16[temp.Day%10],poweroff[3].height,poweroff[3].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(poweroff[4].X,poweroff[4].Y,word16x16[temp.Hour/10],poweroff[4].height,poweroff[4].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(poweroff[5].X,poweroff[5].Y,word16x16[temp.Hour%10],poweroff[5].height,poweroff[5].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(poweroff[6].X,poweroff[6].Y,word16x16[temp.Min/10],poweroff[6].height,poweroff[6].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(poweroff[7].X,poweroff[7].Y,word16x16[temp.Min%10],poweroff[7].height,poweroff[7].width/8);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENLastDown::InitDispWord(void)
{
try {
    for (int i=0;i<8;i++) {
         if (i==0 || i==1) {
             poweroff[i].X=24+i*16;
             poweroff[i].Y=54;
             poweroff[i].width=16;
             poweroff[i].height=16;
         } else if (i==2 || i==3) {
             poweroff[i].X=80+(i-2)*16;
             poweroff[i].Y=54;
             poweroff[i].width=16;
             poweroff[i].height=16;
         } else if (i==4 || i==5) {
             poweroff[i].X=130+(i-4)*16;
             poweroff[i].Y=54;
             poweroff[i].width=16;
             poweroff[i].height=16;
         } else if (i==6 || i==7) {
             poweroff[i].X=184+(i-6)*16;
             poweroff[i].Y=54;
             poweroff[i].width=16;
             poweroff[i].height=16;
         }
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENLastDown::DoKeyF1Work(void)
{
    screenMain.DisplayMain();
}
//---------------------------------------------------------------------------
void SCREENLastDown::DoKeyF4Work(void)
{
//arwen modify    screenCtlSetup.DisplayCtlSetup();
    screenSystemInfoMenu.DisplaySystemInfoMenu();
}
//---------------------------------------------------------------------------
