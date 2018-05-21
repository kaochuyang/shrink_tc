#include "SCREENMain.h"
#include "SCREENLogo.h"
#include "SCREENCtlSetup.h"
//#include "SCREENSendCycle.h"
#include "SCREENUpdateDB.h"
#include "LCD240x128.h"
#include "SMEM.h"

//OTADD
#include "screenSegtypeMenu.h"
#include "screenSegtype.h"
//Removed #include "screenPlanMenu.h"
#include "screenPlan.h"
#include "screenRunTableList.h"
#include "screenRTMSInfo.h"
//#include "screenRedCountMenu.h"

#include "screenReverseMenu.h"

//OT Debug 0523
#include "screenActuateEdit.h"

#include "SCREENDirMap.h"
#include "screenDirLightBoardMappingMenu.h"

#include "SCREENLast92TCPlanSegmentUpdate.h"

#include "screenChainMenu.h"

#include "SCREENControlStrategyMenu.h"

#include "SCREENNetworkSet.h"
#include "SCREENV3Related.h"

#include <time.h>

SCREENMain screenMain;
//---------------------------------------------------------------------------
SCREENMain::SCREENMain(void)
{
try {
    LoadBitmapFromFile();
    InitDispWord();
    cSelect=0;
  } catch (...) {}
}
//---------------------------------------------------------------------------
SCREENMain::~SCREENMain(void)
{
}
//---------------------------------------------------------------------------
void SCREENMain::DoKeyWork(BYTE key)                                            //¤w¹LÂo,¶i¨Óªº¬O0x80~0x98¤§¶¡
{
try {
    switch (key) {
        case 0x80:
          DoKey0Work();
        break;
        case 0x81:
          DoKey1Work();
        break;
        case 0x82:
          DoKey2Work();
        break;
        case 0x83:
          DoKey3Work();
        break;
        case 0x84:
          DoKey4Work();
        break;
        case 0x85:
          DoKey5Work();
        break;
        case 0x86:
          DoKey6Work();
        break;
        case 0x87:
          DoKey7Work();
        break;
        case 0x88:
          DoKey8Work();
        break;
        case 0x89:
        //OT Debug 0523
          DoKey9Work();
        break;
        case 0x90:
          DoKeyF1Work();
        break;
        case 0x91:
          DoKeyF2Work();
        break;
        case 0x92:
          DoKeyF3Work();
        break;
        case 0x93:
        break;
        case 0x94:
          DoKeyUPWork();
        break;
        case 0x95:
          DoKeyDOWNWork();
        break;
        case 0x96:
          DoKeyLEFTWork();
        break;
        case 0x97:
          DoKeyRIGHTWork();
        break;
        case 0x98://ENTER
          DoKeyEnterWork();
        break;
        default:
        break;
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENMain::LoadBitmapFromFile(void)
{
try {
    FILE *bitmap;
//    bitmap=fopen("//cct//bitmap//backGround//3_mainmenu.bit","rb");

//OT Debug 0523
//arwen modify    bitmap=fopen("//cct//bitmap//backGround//mainMenuV3.bit","rb");
    bitmap=fopen("//cct//bitmap//backGround//actmenu//mainMenuV3.bit","rb");
    if (bitmap) {
        fread(mainBitmap,3840,1,bitmap);
        fclose(bitmap);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENMain::DisplayMain(void)
{
try {
    smem.SetcFace(cMAIN);
    //cSelect = 2;
    cSelect =0;
    smem.vWriteMsgToDOM("Display Main Screen");
    lcd240x128.DISPLAY_GRAPHIC(0,mainBitmap,128,30);
    DisplayDateTime();
    setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENMain::InitDispWord(void)
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
             mark[i].X=0;
             mark[i].Y=34+i*16;
         } else if (i>=5 && i<=9) {
             mark[i].X=112;
             mark[i].Y=34+(i-5)*16;
         }
         mark[i].width=16;
         mark[i].height=16;
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENMain::DisplayDateTime(void)
{
try {
    if (smem.GetcFace()==cMAIN) {

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
void SCREENMain::DoKey0Work(void)
{
//arwen modify    screenDirLightBoardMappingMenu.DisplayDirLightBoardMappingMenu();
}
//---------------------------------------------------------------------------
void SCREENMain::DoKey1Work(void)
{
//arwen modify    screenCtlSetup.DisplayCtlSetup();
    //screenControlStrategyMenu.DisplayControlStrategyMenu();//arwen ++控制策略設定
    screenSegtypeMenu.DisplaySegtypeMenu();//定時控制
}
//---------------------------------------------------------------------------
void SCREENMain::DoKey2Work(void)
{
//    screenCarLength.DisplayCarLength();
//    screenSegtypeMenu.DisplaySegtypeMenu();
//arwen modify    screenLast92TCPlanSegmentUpdate.DisplayLastUpdate();
    screenCtlSetup.DisplayCtlSetup();//arwen++ 控制器設定
}
//---------------------------------------------------------------------------
void SCREENMain::DoKey3Work(void)
{
//    screenPlanMenu.DisplayPlanMenu();
//arwen modify    screenSegtypeMenu.DisplaySegtypeMenu();
    screenRunTableList.DisplayRunTableList();//arwen ++控制器資訊ok
}
//---------------------------------------------------------------------------
void SCREENMain::DoKey4Work(void)
{
//    screenRecordCycle.DisplayRecordCycle();
    //screenRunTableList.DisplayRunTableList();//arwen modify
    screenUpdateDB.DisplayUpdateDB();//arwen++資訊庫更新
}
//---------------------------------------------------------------------------
void SCREENMain::DoKey5Work(void)
{
//arwen modify    screenRTMSInfo.DisplayRTMSInfo();
	screenLast92TCPlanSegmentUpdate.DisplayLastUpdate();//arwen++最後收到協定
}
//---------------------------------------------------------------------------
void SCREENMain::DoKey6Work(void)
{
//arwen modify    screenUpdateDB.DisplayUpdateDB();
//    screenLaneSet.DisplayLaneSet();
	screenNetworkSet.DisplayMainNetworkSet();//arwen++IP設定
}
//---------------------------------------------------------------------------
void SCREENMain::DoKey7Work(void)
{
//arwen modify      screenSendCycle.DisplaySendCycle();
//    screenRealStat.DisplayRealStat();
    screenV3Related.DisplayV3Related();//V3相關設定
}
//---------------------------------------------------------------------------
void SCREENMain::DoKey8Work(void)
{
//    screenRealEvent.DisplayRealEvent();
//arwen modify      screenRedCountMenu.DisplayRedCountMenu();
}

//OT Debug 0523
//---------------------------------------------------------------------------
void SCREENMain::DoKey9Work(void)
{
//arwen modify      screenActuateEdit.DisplayActuateEdit();
}

//---------------------------------------------------------------------------
void SCREENMain::DoKeyF1Work(void)
{
    screenLogo.DisplayLogo();
}

//---------------------------------------------------------------------------
void SCREENMain::DoKeyF2Work(void)
{
    screenReverseMenu.DisplayReverseMenu();
}

//---------------------------------------------------------------------------
void SCREENMain::DoKeyF3Work(void)
{
    screenChainMenu.DisplayChainMenu();
}

//---------------------------------------------------------------------------
void SCREENMain::DoKeyEnterWork(void)
{
    switch (cSelect+1) {
        case 1:
          DoKey1Work();
        break;
        case 2:
          DoKey2Work();
        break;
        case 3:
          DoKey3Work();
        break;
        case 4:
          DoKey4Work();
        break;
        case 5:
          DoKey5Work();
        break;
        case 6:
          DoKey6Work();
        break;
        case 7:
          DoKey7Work();
        break;
        case 8:
          DoKey8Work();
        break;
        case 9:
          DoKey9Work();
        break;
        default:
        break;
    }
}
//---------------------------------------------------------------------------
void SCREENMain::DoKeyUPWork(void)
{
try {
    clearSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    if (cSelect==0) cSelect=1;
    cSelect--;
    setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENMain::DoKeyDOWNWork(void)
{
try {
    clearSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    cSelect++;
    if (cSelect>=9) cSelect=9;
//    if (cSelect>=8) cSelect=8;
    setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENMain::DoKeyLEFTWork(void)
{
try {
    clearSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    if (cSelect>=5 && cSelect<=9)
        cSelect-=5;
    setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENMain::DoKeyRIGHTWork(void)
{
try {
    clearSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    if (cSelect>=0 && cSelect<=4)
        cSelect+=5;
    setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
  } catch (...) {}
}
//---------------------------------------------------------------------------
