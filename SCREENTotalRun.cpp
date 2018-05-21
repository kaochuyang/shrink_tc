#include "SCREENTotalRun.h"
#include "SCREENMain.h"
#include "SCREENCtlSetup.h"
#include "LCD240x128.h"
#include "SMEM.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

SCREENTotalRun screenTotalRun;
//---------------------------------------------------------------------------
SCREENTotalRun::SCREENTotalRun(void)
{
try {
    LoadBitmapFromFile();
    InitDispWord();
  } catch (...) {}
}
//---------------------------------------------------------------------------
SCREENTotalRun::~SCREENTotalRun(void)
{
}
//---------------------------------------------------------------------------
void SCREENTotalRun::DoKeyWork(BYTE key)                                           //已過濾,進來的是0x80~0x98之間
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
        case 0x92://F3
        break;

        case 0x90://F1,ESC
          DoKeyF1Work();
        break;
        case 0x91:
          DoKeyF2Work();
        break;
        case 0x93://F4
          DoKeyF4Work();
        break;

        case 0x94://UP
        case 0x95://DOWN
        case 0x96://LEFT
        case 0x97://RIGHT
        case 0x98://Enter
          DoKeyEnterWork();
        break;
        default:
        break;
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENTotalRun::LoadBitmapFromFile(void)
{
try {
    FILE *bitmap;
    bitmap=fopen("//cct//bitmap//backGround//4_1_11.bit","rb");
    if (bitmap) {
        fread(totalRunBitmap,3840,1,bitmap);
        fclose(bitmap);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENTotalRun::DisplayTotalRun(void)
{
try {
    smem.SetcFace(cTOTALRUN);
    smem.vWriteMsgToDOM("Enter Total Run Screen");
    lcd240x128.DISPLAY_GRAPHIC(0,totalRunBitmap,128,30);

    DisplayStartDateTime();
    DisplayTotalTime();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENTotalRun::DisplayStartDateTime(void)
{
try {
    unsigned short cYear=0,cMonth=0,cDay=0;
    unsigned short cHour=0,cMin=0,cSec=0;

    time_t startTime=smem.GetStartTime();
    struct tm *st=localtime(&startTime);
    cYear=st->tm_year+1900;
    cMonth=st->tm_mon+1;
    cDay=st->tm_mday;
    cHour=st->tm_hour;
    cMin=st->tm_min;
    cSec=st->tm_sec;

    lcd240x128.DISPLAY_GRAPHIC_XY(date[0].X,date[0].Y,word8x16[cYear/1000],date[0].height,date[0].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(date[1].X,date[1].Y,word8x16[(cYear/100)%10],date[1].height,date[1].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(date[2].X,date[2].Y,word8x16[(cYear/10)%10],date[2].height,date[2].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(date[3].X,date[3].Y,word8x16[cYear%10],date[3].height,date[3].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(date[4].X,date[4].Y,word8x16[cMonth/10],date[4].height,date[4].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(date[5].X,date[5].Y,word8x16[cMonth%10],date[5].height,date[5].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(date[6].X,date[6].Y,word8x16[cDay/10],date[6].height,date[6].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(date[7].X,date[7].Y,word8x16[cDay%10],date[7].height,date[7].width/8);

    lcd240x128.DISPLAY_GRAPHIC_XY(time[0].X,time[0].Y,word8x16[cHour/10],time[0].height,time[0].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(time[1].X,time[1].Y,word8x16[cHour%10],time[1].height,time[1].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(time[2].X,time[2].Y,word8x16[cMin/10],time[2].height,time[2].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(time[3].X,time[3].Y,word8x16[cMin%10],time[3].height,time[3].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(time[4].X,time[4].Y,word8x16[cSec/10],time[4].height,time[4].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(time[5].X,time[5].Y,word8x16[cSec%10],time[5].height,time[5].width/8);

  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENTotalRun::DisplayTotalTime(void)
{
try {
    if (smem.GetcFace()==cTOTALRUN) {
        if (smem.GetCurrentTime()>smem.GetStartTime()) {
            long ttotal=smem.GetCurrentTime()-smem.GetStartTime();    //得到總秒數
            unsigned short int tDay=0,tHour=0,tMin=0,tSec=0;

            tDay=ttotal/86400;
            tHour=(ttotal%86400)/3600;
            tMin=((ttotal%86400)%3600)/60;
            tSec=((ttotal%86400)%3600)%60;

            lcd240x128.DISPLAY_GRAPHIC_XY(total[0].X,total[0].Y,word8x16[tDay/1000],total[0].height,total[0].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(total[1].X,total[1].Y,word8x16[(tDay/100)%10],total[1].height,total[1].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(total[2].X,total[2].Y,word8x16[(tDay/10)%10],total[2].height,total[2].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(total[3].X,total[3].Y,word8x16[tDay%10],total[3].height,total[3].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(total[4].X,total[4].Y,word8x16[tHour/10],total[4].height,total[4].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(total[5].X,total[5].Y,word8x16[tHour%10],total[5].height,total[5].width/8);

            lcd240x128.DISPLAY_GRAPHIC_XY(total[6].X,total[6].Y,word8x16[tMin/10],total[6].height,total[6].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(total[7].X,total[7].Y,word8x16[tMin%10],total[7].height,total[7].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(total[8].X,total[8].Y,word8x16[tSec/10],total[8].height,total[8].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(total[9].X,total[9].Y,word8x16[tSec%10],total[9].height,total[9].width/8);
        }
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENTotalRun::InitDispWord(void)
{
    for (int i=0;i<8;i++) {

         if (i>=0 && i<=3) date[i].X=56+i*8;
         else if (i>=4 && i<=5) date[i].X=104+(i-4)*8;
         else if (i>=6 && i<=7) date[i].X=136+(i-6)*8;

         date[i].Y=32;
         date[i].width=8;
         date[i].height=16;
    }//開機日期

    for (int i=0;i<6;i++) {
         if (i>=0 && i<=1) time[i].X=72+i*8;
         else if (i>=2 && i<=3) time[i].X=104+(i-2)*8;
         else if (i>=4 && i<=5) time[i].X=136+(i-4)*8;

         time[i].Y=50;
         time[i].width=8;
         time[i].height=16;
    }//開機時間

    for (int i=0;i<10;i++) {
         if (i>=0 && i<=3) total[i].X=64+i*8;
         else if (i>=4 && i<=5) total[i].X=112+(i-4)*8;
         else if (i>=6 && i<=7) total[i].X=144+(i-6)*8;
         else if (i>=8 && i<=9) total[i].X=176+(i-8)*8;

         total[i].Y=74;
         total[i].width=8;
         total[i].height=16;
    }//總開機時間
}
//---------------------------------------------------------------------------
void SCREENTotalRun::DoKeyF1Work(void)
{
    screenMain.DisplayMain();
}
//---------------------------------------------------------------------------
void SCREENTotalRun::DoKeyF2Work(void)
{
}
//---------------------------------------------------------------------------
void SCREENTotalRun::DoKeyF4Work(void)
{
    screenCtlSetup.DisplayCtlSetup();
}
//---------------------------------------------------------------------------
void SCREENTotalRun::DoKeyEnterWork(void)
{
    screenCtlSetup.DisplayCtlSetup();
}
//---------------------------------------------------------------------------

