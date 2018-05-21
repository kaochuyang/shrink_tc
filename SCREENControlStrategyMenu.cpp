#include "SCREENControlStrategyMenu.h"
#include "LCD240x128.h"
//#include "screenLogo.h"
#include "SCREENMain.h"
#include "screenRunTableList.h"

#include "screenSegtypeMenu.h"
//#include "screenVDPhaseMapTable.h"
#include "SCREENActuateStrategyMenu.h"
#include "screenPlan.h"
#include "CSTC.h"
#include <stdio.h>
#include "SMEM.h"
#include <time.h>

//---------------------------------------------------
SCREENControlStrategyMenu screenControlStrategyMenu;
//---------------------------------------------------------------------------
SCREENControlStrategyMenu::SCREENControlStrategyMenu(void)
{
try {
    loadBitmapFromFile();
    initDispWord();
    cSelect=0;
  } catch (...) {}
}
//---------------------------------------------------------------------------
SCREENControlStrategyMenu::~SCREENControlStrategyMenu(void)
{

}
//---------------------------------------------------------------------------
void SCREENControlStrategyMenu::doKeyWork(BYTE key)                                        //¤w¹LÂo,¶i¨Óªº¬O0x80~0x98¤§¶¡
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
        case 0x8B:
        case 0x8C:
        case 0x8D:
        case 0x8E:
        case 0x8F:
        break;
        case 0x90://F1 ESC
          doKeyF1Work();
        break;
        case 0x91:
          doKeyF2Work();
        break;
        case 0x92:
        //OT Pass
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
        case 0x98://ENTER
          doKeyEnterWork();
        break;
        default:
        break;
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENControlStrategyMenu::loadBitmapFromFile(void)
{
try {
    FILE *bitmap;
    bitmap=fopen("//cct//bitmap//backGround//actmenu//controlStrategyMenu.bit","rb");
    if (bitmap) {
        fread(controlStrategyBitmap,3840,1,bitmap);
        fclose(bitmap);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENControlStrategyMenu::DisplayControlStrategyMenu(void)
{
try {
    smem.SetcFace(cCTRLSTRATEGY);
    smem.vWriteMsgToDOM("Display Controller Common Setup Screen");
    lcd240x128.DISPLAY_GRAPHIC(0,controlStrategyBitmap,128,30);
    DisplayDateTime();
    setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENControlStrategyMenu::initDispWord(void)
{
try {
    for (int i=0;i<8;i++) {
         if (i>=0 && i<=3) dateWord[i].X=128+i*8;                               //xxxx¦~
         else if (i>=4 && i<=5) dateWord[i].X=176+(i-4)*8;                      //xx¤ë
         else if (i>=6 && i<=7) dateWord[i].X=208+(i-6)*8;                      //xx¤é
         dateWord[i].Y=0;
         dateWord[i].width=8;
         dateWord[i].height=16;
    }
    for (int i=0;i<6;i++) {
         if (i>=0 && i<=1) timeWord[i].X=136+i*8;
         else if (i>=2 && i<=3) timeWord[i].X=176+(i-2)*8;
         else if (i>=4 && i<=5) timeWord[i].X=208+(i-4)*8;
         timeWord[i].Y=15;
         timeWord[i].width=8;
         timeWord[i].height=16;
    }
    for (int i=0;i<10;i++) {
         if (i>=0 && i<=4) {
             mark[i].X=16;
             mark[i].Y=34+i*16;
         } else if (i>=5 && i<=9) {
             mark[i].X=128;
             mark[i].Y=34+(i-5)*16;
         }
         mark[i].width=16;
         mark[i].height=16;
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENControlStrategyMenu::DisplayDateTime(void)
{
try {

    if (smem.GetcFace()==cCTRLSTRATEGY) {

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
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENControlStrategyMenu::doKey0Work(void)
{

}
//---------------------------------------------------------------------------
void SCREENControlStrategyMenu::doKey1Work(void)
{
	screenSegtypeMenu.DisplaySegtypeMenu();//定時控制
}
//---------------------------------------------------------------------------
void SCREENControlStrategyMenu::doKey2Work(void)
{
	//screenVDPhaseMapTable.DisplayVDPhaseMapTable();
	screenActuateStrategyMenu.DisplayActuateStrategyMenu();
}
//---------------------------------------------------------------------------
void SCREENControlStrategyMenu::doKey3Work(void)
{

}
//---------------------------------------------------------------------------
void SCREENControlStrategyMenu::doKey4Work(void)
{

}
//---------------------------------------------------------------------------
void SCREENControlStrategyMenu::doKey5Work(void)
{

}
//---------------------------------------------------------------------------
void SCREENControlStrategyMenu::doKey6Work(void)
{

}
//---------------------------------------------------------------------------
void SCREENControlStrategyMenu::doKey7Work(void)
{

}
//---------------------------------------------------------------------------
void SCREENControlStrategyMenu::doKey8Work(void)
{
}
//---------------------------------------------------------------------------
void SCREENControlStrategyMenu::doKey9Work(void)
{
}
//---------------------------------------------------------------------------
void SCREENControlStrategyMenu::doKeyF1Work(void)
{
    screenMain.DisplayMain();
}
//---------------------------------------------------------------------------
void SCREENControlStrategyMenu::doKeyF2Work(void)
{

}
//OT Pass
//---------------------------------------------------------------------------
void SCREENControlStrategyMenu::doKeyF3Work(void)
{

}
//---------------------------------------------------------------------------
void SCREENControlStrategyMenu::doKeyF4Work(void)
{
    screenMain.DisplayMain();
}
//---------------------------------------------------------------------------
void SCREENControlStrategyMenu::doKeyEnterWork(void)
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
//---------------------------------------------------------------------------
void SCREENControlStrategyMenu::doKeyUPWork(void)
{
try {
    clearSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    if (cSelect==0) cSelect=1;
    cSelect--;
    setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENControlStrategyMenu::doKeyDOWNWork(void)
{
try {
    clearSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    cSelect++;
    if (cSelect>=1) cSelect=1;
//    if (cSelect>=8) cSelect=8;
    setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENControlStrategyMenu::doKeyLEFTWork(void)
{
try {
    clearSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    //if (cSelect>=5 && cSelect<=9)
        //cSelect-=5;
    setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENControlStrategyMenu::doKeyRIGHTWork(void)
{
try {
    clearSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    //if (cSelect>=0 && cSelect<=4)
        //cSelect+=5;
    setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
  } catch (...) {}
}
//---------------------------------------------------------------------------
