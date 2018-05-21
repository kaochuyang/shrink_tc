//---------------------------------------------------------------------------
#include "SCREENLockdb.h"
#include "LCD240x128.h"
#include "SMEM.h"

#include "SCREENModifyDT.h"
#include "SCREENHWCycle.h"
#include "SCREENModifyPWD.h"
#include "SCREENDBSetup.h"
#include "SCREENSendCycle.h"
#include "SCREENCmdSet.h"
#include "SCREENLcnEncode.h"
#include "SCREENMain.h"

#include <stdio.h>

SCREENLockdb screenLockdb;
//---------------------------------------------------------------------------
SCREENLockdb::SCREENLockdb(void)
{
    LoadBitmapFromFile();
}
//---------------------------------------------------------------------------
SCREENLockdb::~SCREENLockdb(void)
{
}
//---------------------------------------------------------------------------
void SCREENLockdb::DoKeyWork(BYTE key)              //已過濾,進來的是0x80~0x98之間,5X5按鍵
{
try {
    switch (key) {
        case 0x98:
          DoKeyEnterWork();
        break;
        default:
        break;
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENLockdb::LoadBitmapFromFile(void)
{
try {
    FILE *bitmap;
    bitmap=fopen("//cct//bitmap//backGround//0_lockdb.bit","rb");
    if (bitmap) {
        fread(lockdbBitmap,3840,1,bitmap);
        fclose(bitmap);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENLockdb::DisplayLockdb(void)
{
    smem.SetcFace(cLOCKDB);
    smem.vWriteMsgToDOM("Enter Lock Database Warring Screen");
    lcd240x128.DISPLAY_GRAPHIC(0,lockdbBitmap,128,30);
}
//---------------------------------------------------------------------------
void SCREENLockdb::DoKeyEnterWork(void)
{
try {
    switch (smem.GetLastFace()) {

            case cMODIFYDT:
                 screenModifyDT.DisplayModifyDT();
            break;
            case cHWCYCLE:
                 screenHWCycle.DisplayHWCycle();
            break;
            case cMODIFYPWD:
                 screenModifyPWD.DisplayModifyPWD();
            break;
            case cDBSETUP:
                 screenDBSetup.DisplayDBSetup();
            break;
            case cLCNENCODE:
                 screenLcnEncode.DisplayLcnEncode();
            break;

            case cCMDSET:
                 screenCmdSet.DisplayCmdSet();
            break;

            case cSENDCYCLE:
                 screenSendCycle.DisplaySendCycle();
            break;

            default:
                 screenMain.DisplayMain();
            break;

    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
