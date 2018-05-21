//---------------------------------------------------------------------------
#include "SCREENWarning.h"
#include "SCREENMain.h"
#include "SCREENMBSelect.h"
#include "SCREENUart2.h"
#include "LCD240x128.h"
#include "SMEM.h"

#include "SCREENHWReset.h"
#include "WRITEJOB.h"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <error.h>

#include "CSTC.h"

SCREENWarning screenWarning;
//---------------------------------------------------------------------------
SCREENWarning::SCREENWarning(void)
{
    LoadBitmapFromFile();
}
//---------------------------------------------------------------------------
SCREENWarning::~SCREENWarning(void)
{
}
//---------------------------------------------------------------------------
void SCREENWarning::DoKeyWork(BYTE key)              //已過濾,進來的是0x80~0x98之間,5X5按鍵
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
void SCREENWarning::LoadBitmapFromFile(void)
{
try {
    FILE *bitmap;
    bitmap=fopen("//cct//bitmap//backGround//00-OTWarning.bit","rb");
    if (bitmap) {
        fread(ucBitmap,3840,1,bitmap);
        fclose(bitmap);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENWarning::DisplayWarning(void)
{
try {
    smem.SetcFace(cWARNING);
    smem.vWriteMsgToDOM("Enter Warning Screen");
    lcd240x128.DISPLAY_GRAPHIC(0,ucBitmap,128,30);

  } catch (...) {}
}

//---------------------------------------------------------------------------
/*
void SCREENWarning::UpdateSSComm(void)
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
void SCREENWarning::DoKeyF1Work(void)
{
    screenMain.DisplayMain();
}
//---------------------------------------------------------------------------
void SCREENWarning::DoKeyF2Work(void)  //硬體重置
{
try {

  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENWarning::DoKeyF3Work(void)
{
try {

  system("sync");
  system("sync");
  stc.CalculateAndSendRedCount(10);
  writeJob.WriteLetW77E58AutoControl();                                      //let W77e58 autoControl.
  system("sync");
  system("reboot");
  screenHWReset.DisplayHWReset();
  system("sync");
  system("sync");

  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENWarning::DoKeyF4Work(void)
{
  int iTmp;
  iTmp = smem.GetLastFace();

  if(iTmp == cMBSELECT) {
    screenMBSelect.DisplayMBSelect();
  } else if (iTmp == cUART2SELECT) {
    screenUart2.DisplayUart2Select();
  }
}
//---------------------------------------------------------------------------
