#include "SCREENHWCycle.h"
#include "SCREENMain.h"
//#include "SCREENCtlSetup.h"
#include "SCREENV3Related.h"
#include "SCREENLockdb.h"
#include "SCREENABOErr.h"

#include "LCD240x128.h"
#include "SMEM.h"

#include "CDataToMessageOK.h"
#include "WRITEJOB.h"

SCREENHWCycle screenHWCycle;
//---------------------------------------------------------------------------
SCREENHWCycle::SCREENHWCycle(void)
{
try {
    LoadBitmapFromFile();
    InitDispWord();
    cSelect=0;
  } catch (...) {}
}
//---------------------------------------------------------------------------
SCREENHWCycle::~SCREENHWCycle(void)
{
}
//---------------------------------------------------------------------------
void SCREENHWCycle::DoKeyWork(BYTE key)                                         //已過濾,進來的是0x80~0x98之間
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
        break;
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
        case 0x90://F1 ESC
          DoKeyF1Work();
        break;
        case 0x91:
        case 0x92:
        break;
        case 0x93://F4
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
void SCREENHWCycle::LoadBitmapFromFile(void)
{
try {
    FILE *bitmap;
    bitmap=fopen("//cct//bitmap//backGround//4_1_5.bit","rb");
    if (bitmap) {
        fread(hwCycleBitmap,3840,1,bitmap);
        fclose(bitmap);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENHWCycle::DisplayHWCycle(void)
{
try {
    smem.SetcFace(cHWCYCLE);
    smem.vWriteMsgToDOM("Enter Hardware Cycle Setup Screen");
    lcd240x128.DISPLAY_GRAPHIC(0,hwCycleBitmap,128,30);
    cSelect=smem.vGetHWCycleCodeFor_0F14_0FC4();
    if (cSelect<6) {
        setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(cycle.X,cycle.Y,word24x24[cSelect],cycle.height,cycle.width/8);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENHWCycle::InitDispWord(void)
{
try {
    cycle.X=144;
    cycle.Y=5;
    cycle.width=24;
    cycle.height=24;

    for (int i=0;i<6;i++) {
         if (i>=0 && i<=2) {
             mark[i].X=24;
             mark[i].Y=44+i*19;
         } else if (i>=3 && i<=5) {
             mark[i].X=120;
             mark[i].Y=44+(i-3)*19;
         }
         mark[i].width=16;
         mark[i].height=16;
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENHWCycle::DoKey0Work(void)
{
    if (cSelect<6) clearSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    cSelect=0;
    setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(cycle.X,cycle.Y,word24x24[cSelect],cycle.height,cycle.width/8);
}
//---------------------------------------------------------------------------
void SCREENHWCycle::DoKey1Work(void)
{
    if (cSelect<6) clearSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    cSelect=1;
    setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(cycle.X,cycle.Y,word24x24[cSelect],cycle.height,cycle.width/8);
}
//---------------------------------------------------------------------------
void SCREENHWCycle::DoKey2Work(void)
{
    if (cSelect<6) clearSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    cSelect=2;
    setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(cycle.X,cycle.Y,word24x24[cSelect],cycle.height,cycle.width/8);
}
//---------------------------------------------------------------------------
void SCREENHWCycle::DoKey3Work(void)
{
    if (cSelect<6) clearSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    cSelect=3;
    setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(cycle.X,cycle.Y,word24x24[cSelect],cycle.height,cycle.width/8);
}
//---------------------------------------------------------------------------
void SCREENHWCycle::DoKey4Work(void)
{
    if (cSelect<6) clearSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    cSelect=4;
    setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(cycle.X,cycle.Y,word24x24[cSelect],cycle.height,cycle.width/8);
}
//---------------------------------------------------------------------------
void SCREENHWCycle::DoKey5Work(void)
{
    if (cSelect<6) clearSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    cSelect=5;
    setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(cycle.X,cycle.Y,word24x24[cSelect],cycle.height,cycle.width/8);
}
//---------------------------------------------------------------------------
void SCREENHWCycle::DoKeyF1Work(void)
{
    screenMain.DisplayMain();
}
//---------------------------------------------------------------------------
void SCREENHWCycle::DoKeyF4Work(void)
{
//arwen modify    screenCtlSetup.DisplayCtlSetup();
    screenV3Related.DisplayV3Related();
}
//---------------------------------------------------------------------------
void SCREENHWCycle::DoKeyEnterWork(void)
{
try {
    if (smem.GetDbOperStat()==0 || smem.vLoadCenterConnectStatus()==false) {
        if (smem.vGet0FCommandSet()==0 && (cSelect==1 || cSelect==2 || cSelect==3 || cSelect==4)) {
            smem.SetLastFace(cHWCYCLE);
            screenABOErr.DisplayABOErr();
        } else {

            if (cSelect<6) {

                BYTE data[3];

                data[0]  = 0x0F;
                data[1]  = 0x14;
                data[2]  = cSelect;

                MESSAGEOK _MsgOK;

                _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, 3,false);
                _MsgOK.InnerOrOutWard = cComingFromScreen;
                writeJob.WriteWorkByMESSAGEOUT(_MsgOK);

                if (cSelect==0)  smem.vWriteMsgToDOM("Set Hardware Cycle To Stop Hardware Cycle");
                else if (cSelect==1)  smem.vWriteMsgToDOM("Set Hardware Cycle To 1 Second");
                else if (cSelect==2)  smem.vWriteMsgToDOM("Set Hardware Cycle To 2 Seconds");
                else if (cSelect==3)  smem.vWriteMsgToDOM("Set Hardware Cycle To 5 Seconds");
                else if (cSelect==4)  smem.vWriteMsgToDOM("Set Hardware Cycle To 1 Minute");
                else if (cSelect==5)  smem.vWriteMsgToDOM("Set Hardware Cycle To 5 Minutes");

//arwen modify                screenCtlSetup.DisplayCtlSetup();
                screenV3Related.DisplayV3Related();
            }
        }
    } else {
        smem.SetLastFace(cHWCYCLE);
        screenLockdb.DisplayLockdb();
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENHWCycle::DoKeyDOWNWork(void)
{
try {
    if (cSelect<6) clearSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    cSelect++;
    if (cSelect>5) cSelect=5;
    setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(cycle.X,cycle.Y,word24x24[cSelect],cycle.height,cycle.width/8);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENHWCycle::DoKeyUPWork(void)
{
try {
    if (cSelect<6) clearSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    if (cSelect==0 || cSelect>=6) cSelect=1;
    cSelect--;
    setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(cycle.X,cycle.Y,word24x24[cSelect],cycle.height,cycle.width/8);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENHWCycle::DoKeyLEFTWork(void)
{
try {
    if (cSelect<6) clearSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    if (cSelect>=3 && cSelect<=5) cSelect-=3;
    if (cSelect<6) setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    if (cSelect<6) lcd240x128.DISPLAY_GRAPHIC_XY(cycle.X,cycle.Y,word24x24[cSelect],cycle.height,cycle.width/8);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENHWCycle::DoKeyRIGHTWork(void)
{
try {
    if (cSelect<6) clearSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    if (cSelect>=0 && cSelect<=2) cSelect+=3;
    if (cSelect<6) setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    if (cSelect<6) lcd240x128.DISPLAY_GRAPHIC_XY(cycle.X,cycle.Y,word24x24[cSelect],cycle.height,cycle.width/8);
  } catch (...) {}
}
//---------------------------------------------------------------------------
