//---------------------------------------------------------------------------
#include "SCREENHWReset.h"
#include "LCD240x128.h"
#include "SMEM.h"

SCREENHWReset screenHWReset;
//---------------------------------------------------------------------------
SCREENHWReset::SCREENHWReset(void)
{
    LoadBitmapFromFile();
    InitDispWord();
}
//---------------------------------------------------------------------------
SCREENHWReset::~SCREENHWReset(void)
{
}
//---------------------------------------------------------------------------
void SCREENHWReset::DoKeyWork(BYTE key)
{
}
//---------------------------------------------------------------------------
void SCREENHWReset::LoadBitmapFromFile(void)
{
try {
    FILE *bitmap;
    bitmap=fopen("//cct//bitmap//backGround//0_hwreset.bit","rb");
    if (bitmap) {
        fread(hwResetBitmap,3840,1,bitmap);
        fclose(bitmap);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENHWReset::DisplayHWReset(void)
{
try {
    smem.SetcFace(cHWRESET);
    smem.vWriteMsgToDOM("Hardware Reset!!");
    lcd240x128.DISPLAY_GRAPHIC(0,hwResetBitmap,128,30);
    DisplayDateTime();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENHWReset::InitDispWord(void)                                         //初始化此畫面的座標等參數
{
    for (int i=0;i<8;i++) {
         if (i>=0 && i<=3) dateWord[i].X=64+i*8;
         else if (i>=4 && i<=5) dateWord[i].X=112+(i-4)*8;
         else if (i>=6 && i<=7) dateWord[i].X=144+(i-6)*8;
         dateWord[i].Y=25;
         dateWord[i].width=8;
         dateWord[i].height=16;
    }
    for (int i=0;i<6;i++) {
         if (i>=0 && i<=1)  timeWord[i].X=80+i*8;
         else if (i>=2 && i<=3)  timeWord[i].X=112+(i-2)*8;
         else if (i>=4 && i<=5)  timeWord[i].X=144+(i-4)*8;
         timeWord[i].Y=45;
         timeWord[i].width=8;
         timeWord[i].height=16;
    }
}
//---------------------------------------------------------------------------
void SCREENHWReset::DisplayDateTime(void)
{
try {
        unsigned short cYear=0,cMonth=0,cDay=0,cHour=0,cMin=0,cSec=0;
        time_t currentTime=time(NULL);
        struct tm *now=localtime(&currentTime);

        cYear=now->tm_year+1900;
        cMonth=now->tm_mon+1;
        cDay=now->tm_mday;
        cHour=now->tm_hour;
        cMin=now->tm_min;
        cSec=now->tm_sec;

        lcd240x128.DISPLAY_GRAPHIC_XY(dateWord[0].X,dateWord[0].Y,word8x16[cYear/1000],dateWord[0].height,dateWord[0].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(dateWord[1].X,dateWord[1].Y,word8x16[(cYear/100)%10],dateWord[1].height,dateWord[1].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(dateWord[2].X,dateWord[2].Y,word8x16[(cYear/10)%10],dateWord[2].height,dateWord[2].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(dateWord[3].X,dateWord[3].Y,word8x16[cYear%10],dateWord[3].height,dateWord[3].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(dateWord[4].X,dateWord[4].Y,word8x16[cMonth/10],dateWord[4].height,dateWord[4].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(dateWord[5].X,dateWord[5].Y,word8x16[cMonth%10],dateWord[5].height,dateWord[5].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(dateWord[6].X,dateWord[6].Y,word8x16[cDay/10],dateWord[6].height,dateWord[6].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(dateWord[7].X,dateWord[7].Y,word8x16[cDay%10],dateWord[7].height,dateWord[7].width/8);

        lcd240x128.DISPLAY_GRAPHIC_XY(timeWord[0].X,timeWord[0].Y,word8x16[cHour/10],timeWord[0].height,timeWord[0].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(timeWord[1].X,timeWord[1].Y,word8x16[cHour%10],timeWord[1].height,timeWord[1].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(timeWord[2].X,timeWord[2].Y,word8x16[cMin/10],timeWord[2].height,timeWord[2].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(timeWord[3].X,timeWord[3].Y,word8x16[cMin%10],timeWord[3].height,timeWord[3].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(timeWord[4].X,timeWord[4].Y,word8x16[cSec/10],timeWord[4].height,timeWord[4].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(timeWord[5].X,timeWord[5].Y,word8x16[cSec%10],timeWord[5].height,timeWord[5].width/8);

  } catch (...) {}
}
//---------------------------------------------------------------------------
