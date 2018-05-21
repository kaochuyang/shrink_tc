#include "SCREENCtlSetup.h"
#include "SCREENMain.h"
//#include "SCREENCtlNo.h"
#include "SCREENLastDown.h"
#include "SCREENModifyDT.h"
#include "SCREENFWVerDT.h"
#include "SCREENHWCycle.h"
#include "SCREENModifyPWD.h"
#include "SCREENDBSetup.h"
#include "SCREENOperStat.h"
#include "SCREENOperMode.h"
#include "SCREENCommStat.h"
#include "SCREENTotalRun.h"
#include "screenCompensationset.h"

//#include "SCREENSendCycle.h"
#include "screenRedCountMenu.h"
#include "screenActuateEdit.h"
#include "screenActuateBUSEdit.h"
#include "screenDirLightBoardMappingMenu.h"
#include "SCREENUpdateDB.h"
#include "screenCurrentCommPacket.h"
//#include "SCREENV3Related.h"
//#include "SCREENPedSignalMenu.h"
//OT Pass
#include "screenPassModeSet.h"

#include "screenCountdownProperty.h"

#include "LCD240x128.h"
#include "SMEM.h"

#include <time.h>

SCREENCtlSetup screenCtlSetup;
//---------------------------------------------------------------------------
SCREENCtlSetup::SCREENCtlSetup(void)
{
try {
    LoadBitmapFromFile();
    InitDispWord();
    cSelect=0;
  } catch (...) {}
}
//---------------------------------------------------------------------------
SCREENCtlSetup::~SCREENCtlSetup(void)
{
}
//---------------------------------------------------------------------------
void SCREENCtlSetup::DoKeyWork(BYTE key)                                        //¤w¹LÂo,¶i¨Óªº¬O0x80~0x98¤§¶¡
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
          DoKey9Work();
        break;
        case 0x8A:
        case 0x8B:
        case 0x8C:
        case 0x8D:
        case 0x8E:
        case 0x8F:
        break;
        case 0x90://F1 ESC
          DoKeyF1Work();
        break;
        case 0x91:
          DoKeyF2Work();
        break;
        case 0x92:
        //OT Pass
          DoKeyF3Work();
        break;
        case 0x93:
          DoKeyF4Work();
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
void SCREENCtlSetup::LoadBitmapFromFile(void)
{
try {
    FILE *bitmap;
//arwen mobify    bitmap=fopen("//cct//bitmap//backGround//4_1_0ControlSetting.bit","rb");
    bitmap=fopen("//cct//bitmap//backGround//actmenu//controlSetting.bit","rb");
    if (bitmap) {
        fread(ctlSetupBitmap,3840,1,bitmap);
        fclose(bitmap);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENCtlSetup::DisplayCtlSetup(void)
{
try {
    smem.SetcFace(cCTLSETUP);
    smem.vWriteMsgToDOM("Display Controller Common Setup Screen");
    lcd240x128.DISPLAY_GRAPHIC(0,ctlSetupBitmap,128,30);
    DisplayDateTime();
    setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENCtlSetup::InitDispWord(void)
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
void SCREENCtlSetup::DisplayDateTime(void)
{
try {

    if (smem.GetcFace()==cCTLSETUP) {

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
void SCREENCtlSetup::DoKey0Work(void)
{
//OTCombo move to 9
//    screenCommStat.DisplayCommStat();
    screenCurrentCommPacket.DisplayCurrentCommPacket();
}
//---------------------------------------------------------------------------
void SCREENCtlSetup::DoKey1Work(void)
{
    //screenCtlNo.DisplayCtlNo();//設備編號
    screenModifyDT.DisplayModifyDT();//日期時間管理
}
//---------------------------------------------------------------------------
void SCREENCtlSetup::DoKey2Work(void)
{
//arwen modify    screenLastDown.DisplayLastDown();
    //screenModifyDT.DisplayModifyDT();//日期時間管理
    screenOperStat.DisplayOperStat();//硬體設定
}
//---------------------------------------------------------------------------
void SCREENCtlSetup::DoKey3Work(void)
{
//arwen modify    screenModifyDT.DisplayModifyDT();
    //screenOperStat.DisplayOperStat();//硬體設定
    screenRedCountMenu.DisplayRedCountMenu();//紅燈倒數設定

}
//---------------------------------------------------------------------------
void SCREENCtlSetup::DoKey4Work(void)
{
//arwen modify        screenFWVerDT.DisplayFWVerDT();
    //screenSendCycle.DisplaySendCycle();//傳輸週期設定
    screenActuateEdit.DisplayActuateEdit();//行人觸動設定
}
//---------------------------------------------------------------------------
void SCREENCtlSetup::DoKey5Work(void)
{
//arwen modify        screenHWCycle.DisplayHWCycle();
    //screenRedCountMenu.DisplayRedCountMenu();//紅燈倒數設定
    screenDirLightBoardMappingMenu.DisplayDirLightBoardMappingMenu();//方向燈設定
}
//---------------------------------------------------------------------------
void SCREENCtlSetup::DoKey6Work(void)
{
//arwen modify        screenModifyPWD.DisplayModifyPWD();
    //screenActuateEdit.DisplayActuateEdit();//行人觸動設定
    screenActuateBUSEdit.DisplayActuateBUSEdit();//BUS觸動設定
    //screenPedSignalMenu.DisplayPedSignalMenu();
}
//---------------------------------------------------------------------------
void SCREENCtlSetup::DoKey7Work(void)
{
//arwen modify        screenDBSetup.DisplayDBSetup();
    //screenDirLightBoardMappingMenu.DisplayDirLightBoardMappingMenu();//方向燈設定
    screenCompensationset.Displaycompensation(); //補償週期設定
    // smem.vReadCompensationFromDisk();
}
//---------------------------------------------------------------------------
void SCREENCtlSetup::DoKey8Work(void)
{
//arwen modify        screenOperStat.DisplayOperStat();
    //screenV3Related.DisplayV3Related(); //V3相關設定
    screenCountdownProperty.DisplayCountdownProperty(); //倒數秒數屬性
}
//---------------------------------------------------------------------------
void SCREENCtlSetup::DoKey9Work(void)
{
    //screenActuateBUSEdit.DisplayActuateBUSEdit();//BUS觸動設定
    //------------------------------------2012/09/28-f---chou
//Disable
//    screenOperMode.DisplayOperMode();
//arwen modify        screenCommStat.DisplayCommStat();
}
//---------------------------------------------------------------------------
void SCREENCtlSetup::DoKeyF1Work(void)
{
    screenMain.DisplayMain();
}
//---------------------------------------------------------------------------
void SCREENCtlSetup::DoKeyF2Work(void)
{
    screenTotalRun.DisplayTotalRun();
}
//OT Pass
//---------------------------------------------------------------------------
void SCREENCtlSetup::DoKeyF3Work(void)
{
    screenPassModeSet.DisplayPassModeSet();
}
//---------------------------------------------------------------------------
void SCREENCtlSetup::DoKeyF4Work(void)
{
    screenMain.DisplayMain();
}
//---------------------------------------------------------------------------
void SCREENCtlSetup::DoKeyEnterWork(void)
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
        case 10:
          DoKey0Work();
        break;
        default:
        break;
    }
}
//---------------------------------------------------------------------------
void SCREENCtlSetup::DoKeyUPWork(void)
{
try {
    clearSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    if (cSelect==0) cSelect=1;
    cSelect--;
    setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENCtlSetup::DoKeyDOWNWork(void)
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
void SCREENCtlSetup::DoKeyLEFTWork(void)
{
try {
    clearSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    if (cSelect>=5 && cSelect<=9)
        cSelect-=5;
    setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENCtlSetup::DoKeyRIGHTWork(void)
{
try {
    clearSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    if (cSelect>=0 && cSelect<=4)
        cSelect+=5;
    setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
  } catch (...) {}
}
//---------------------------------------------------------------------------
