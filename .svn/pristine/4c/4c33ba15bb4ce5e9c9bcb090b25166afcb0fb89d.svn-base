//---------------------------------------------------------------------------
#include "SCREENLcnEncode.h"
#include "SCREENMain.h"
#include "SCREENCtlSetup.h"
#include "SCREENCtlNo.h"
#include "LCD240x128.h"
#include "SMEM.h"
#include "SCREENLockdb.h"

SCREENLcnEncode screenLcnEncode;
//---------------------------------------------------------------------------
SCREENLcnEncode::SCREENLcnEncode(void)
{
try {
    LoadBitmapFromFile();
    InitDispWord();
  } catch (...) {}
}
//---------------------------------------------------------------------------
SCREENLcnEncode::~SCREENLcnEncode(void)
{
}
//---------------------------------------------------------------------------
void SCREENLcnEncode::DoKeyWork(BYTE key)              //已過濾,進來的是0x80~0x98之間,5X5按鍵
{
try {
    switch (key) {
        case 0x80:
        break;
        case 0x81:
             DoKey1Work();
        break;
        case 0x82:
             DoKey2Work();
        break;
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
        break;
        case 0x90://F1,ESC
          DoKeyF1Work();
        break;
        case 0x91://F2
          DoKeyF2Work();
        break;
        case 0x92://F3
        break;
        case 0x93://F4
          DoKeyF4Work();
        break;
        case 0x94://UP
          DoKeyUpWork();
        break;
        case 0x95://DOWN
          DoKeyDownWork();
        break;
        case 0x96://LEFT
        case 0x97://RIGHT
        break;
        case 0x98://Enter
          DoKeyEnterWork();
        break;
        default:
        break;
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENLcnEncode::LoadBitmapFromFile(void)
{
try {
    FILE *bitmap;
    bitmap=fopen("//cct//bitmap//backGround//4_1_1_1.bit","rb");
    if (bitmap) {
        fread(lcnEncodeBitmap,3840,1,bitmap);
        fclose(bitmap);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENLcnEncode::DisplayLcnEncode(void)
{
try {
    smem.SetcFace(cLCNENCODE);
    smem.vWriteMsgToDOM("Enter LcnEncode Screen");
    lcd240x128.DISPLAY_GRAPHIC(0,lcnEncodeBitmap,128,30);

    if (smem.vGetAdjudicateReturnAddressBCDorHEX()==cBCD) cSelect=1;
    else cSelect=2;

    lcd240x128.DISPLAY_GRAPHIC_XY(lcnCode.X,lcnCode.Y,word24x24[cSelect],lcnCode.height,lcnCode.width/8);
    setSelectPoint16x16(select[cSelect-1].X,select[cSelect-1].Y,select[cSelect-1].height,select[cSelect-1].width/8);

  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENLcnEncode::InitDispWord(void)
{
    lcnCode.X=112;
    lcnCode.Y=7;
    lcnCode.width=24;
    lcnCode.height=24;

    for (int i=0;i<2;i++) {
         select[i].X=32;
         select[i].Y=42+i*18;
         select[i].width=16;
         select[i].height=16;
    }
}
//---------------------------------------------------------------------------
void SCREENLcnEncode::DoKey1Work(void)
{
    if (cSelect==1 || cSelect==2) {
        clearSelectPoint16x16(select[cSelect-1].X,select[cSelect-1].Y,select[cSelect-1].height,select[cSelect-1].width/8);
        cSelect=1;
        lcd240x128.DISPLAY_GRAPHIC_XY(lcnCode.X,lcnCode.Y,word24x24[cSelect],lcnCode.height,lcnCode.width/8);
        setSelectPoint16x16(select[cSelect-1].X,select[cSelect-1].Y,select[cSelect-1].height,select[cSelect-1].width/8);
    }
}
//---------------------------------------------------------------------------
void SCREENLcnEncode::DoKey2Work(void)
{
    if (cSelect==1 || cSelect==2) {
        clearSelectPoint16x16(select[cSelect-1].X,select[cSelect-1].Y,select[cSelect-1].height,select[cSelect-1].width/8);
        cSelect=2;
        lcd240x128.DISPLAY_GRAPHIC_XY(lcnCode.X,lcnCode.Y,word24x24[cSelect],lcnCode.height,lcnCode.width/8);
        setSelectPoint16x16(select[cSelect-1].X,select[cSelect-1].Y,select[cSelect-1].height,select[cSelect-1].width/8);
    }
}
//---------------------------------------------------------------------------
void SCREENLcnEncode::DoKeyF1Work(void)
{
    screenMain.DisplayMain();
}
//---------------------------------------------------------------------------
void SCREENLcnEncode::DoKeyF2Work(void)
{
    screenCtlSetup.DisplayCtlSetup();
}
//---------------------------------------------------------------------------
void SCREENLcnEncode::DoKeyF4Work(void)
{
    screenCtlNo.DisplayCtlNo();
}
//---------------------------------------------------------------------------
void SCREENLcnEncode::DoKeyUpWork(void)
{
    if (cSelect==1 || cSelect==2) {
        clearSelectPoint16x16(select[cSelect-1].X,select[cSelect-1].Y,select[cSelect-1].height,select[cSelect-1].width/8);
        cSelect=1;
        lcd240x128.DISPLAY_GRAPHIC_XY(lcnCode.X,lcnCode.Y,word24x24[cSelect],lcnCode.height,lcnCode.width/8);
        setSelectPoint16x16(select[cSelect-1].X,select[cSelect-1].Y,select[cSelect-1].height,select[cSelect-1].width/8);
    }
}
//---------------------------------------------------------------------------
void SCREENLcnEncode::DoKeyDownWork(void)
{
    if (cSelect==1 || cSelect==2) {
        clearSelectPoint16x16(select[cSelect-1].X,select[cSelect-1].Y,select[cSelect-1].height,select[cSelect-1].width/8);
        cSelect=2;
        lcd240x128.DISPLAY_GRAPHIC_XY(lcnCode.X,lcnCode.Y,word24x24[cSelect],lcnCode.height,lcnCode.width/8);
        setSelectPoint16x16(select[cSelect-1].X,select[cSelect-1].Y,select[cSelect-1].height,select[cSelect-1].width/8);
    }
}
//---------------------------------------------------------------------------
void SCREENLcnEncode::DoKeyEnterWork(void)
{
try {
    if (smem.GetDbOperStat()==0 || smem.vLoadCenterConnectStatus()==false) {
        if (cSelect==1 || cSelect==2) {
            if (cSelect==1) {
                smem.vSetAdjudicateReturnAddressBCDorHEX(cBCD);
                smem.vWriteMsgToDOM("Set Lcd Encode To cBCD!");
            } else {
                smem.vSetAdjudicateReturnAddressBCDorHEX(cHEX);
                smem.vWriteMsgToDOM("Set Lcd Encode To cHEX!");
            }
            screenCtlNo.DisplayCtlNo();
        }
    } else {
        smem.SetLastFace(cLCNENCODE);
        screenLockdb.DisplayLockdb();
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
