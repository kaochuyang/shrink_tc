//---------------------------------------------------------------------------
#include "SCREENCommStat.h"
#include "SCREENMain.h"
//#include "SCREENCtlSetup.h"
#include "SCREENSystemInfoMenu.h"

#include "SCREENHWReset.h"
#include "LCD240x128.h"
#include "SMEM.h"
#include "DIGITALIO.h"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <error.h>

#include "CSTC.h"

#include "WRITEJOB.h"

SCREENCommStat screenCommStat;
//---------------------------------------------------------------------------
SCREENCommStat::SCREENCommStat(void)
{
    LoadBitmapFromFile();
}
//---------------------------------------------------------------------------
SCREENCommStat::~SCREENCommStat(void)
{
}
//---------------------------------------------------------------------------
void SCREENCommStat::DoKeyWork(BYTE key)              //已過濾,進來的是0x80~0x98之間,5X5按鍵
{
try {
    switch (key) {
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
          DoKeyF4Work();
        break;
        default:
        break;
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENCommStat::LoadBitmapFromFile(void)
{
try {
    FILE *bitmap;
    bitmap=fopen("//cct//bitmap//backGround//4_1_10CommStatus.bit","rb");
    if (bitmap) {
        fread(commStatBitmap,3840,1,bitmap);
        fclose(bitmap);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENCommStat::DisplayCommStat(void)
{
try {
    smem.SetcFace(cCOMMSTAT);
    smem.vWriteMsgToDOM("Enter Communication Status Screen");
    lcd240x128.DISPLAY_GRAPHIC(0,commStatBitmap,128,30);
    UpdateCenterComm();
//    UpdateSSComm();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENCommStat::UpdateComm(void)
{
    UpdateCenterComm();
//    UpdateSSComm();
}
//---------------------------------------------------------------------------
void SCREENCommStat::UpdateCenterComm(void)
{
try {
    if (smem.GetcFace()==cCOMMSTAT) {
        if (smem.vLoadCenterConnectStatus())
            lcd240x128.DISPLAY_GRAPHIC_XY(96,40,word16x16[1],16,16/8);
        else lcd240x128.DISPLAY_GRAPHIC_XY(96,40,word16x16[2],16,16/8);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
/*
void SCREENCommStat::UpdateSSComm(void)
{
try {
    if (smem.GetcFace()==cCOMMSTAT) {
        if (smem.GetSSComm())
            lcd240x128.DISPLAY_GRAPHIC_XY(96,60,word16x16[1],16,16/8);
        else lcd240x128.DISPLAY_GRAPHIC_XY(96,60,word16x16[2],16,16/8);
    }
  } catch (...) {}
}
*/
//---------------------------------------------------------------------------
void SCREENCommStat::DoKeyF1Work(void)
{
    screenMain.DisplayMain();
}
//---------------------------------------------------------------------------
void SCREENCommStat::DoKeyF2Work(void)  //硬體重置
{
try {

    digitalIO.WriteDigitalIO(0xFF);
    usleep(1500000);
//autorun    stc.Lock_to_Set_Control_Strategy(STRATEGY_FLASH);
    digitalIO.WriteDigitalIO(0x00);

    stc.CalculateAndSendRedCount(10);
    writeJob.WriteLetW77E58AutoControl();                                       //let W77e58 autoControl.

    system("sync");
    system("sync");
    system("reboot");

    screenHWReset.DisplayHWReset();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENCommStat::DoKeyF3Work(void)
{
try {
    smem.vWriteMsgToDOM("Restart The GPRS Modem By Control Power");
    smem.vSet0F11CommuncationResetChangeStatus(true);
//arwen modify    screenCtlSetup.DisplayCtlSetup();
    screenSystemInfoMenu.DisplaySystemInfoMenu();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENCommStat::DoKeyF4Work(void)
{
//arwen modify    screenCtlSetup.DisplayCtlSetup();
    screenSystemInfoMenu.DisplaySystemInfoMenu();
}
//---------------------------------------------------------------------------
