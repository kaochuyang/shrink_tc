//---------------------------------------------------------------------------
#include "SCREENCmdSet.h"
#include "LCD240x128.h"
#include "SMEM.h"

#include "SCREENMain.h"
#include "SCREENCtlSetup.h"
#include "SCREENFWVerDT.h"
#include "SCREENLockdb.h"

#include <stdio.h>

SCREENCmdSet screenCmdSet;
//---------------------------------------------------------------------------
SCREENCmdSet::SCREENCmdSet(void)
{
    LoadBitmapFromFile();
    InitDispWord();
}
//---------------------------------------------------------------------------
SCREENCmdSet::~SCREENCmdSet(void)
{
}
//---------------------------------------------------------------------------
void SCREENCmdSet::DoKeyWork(BYTE key)              //已過濾,進來的是0x80~0x98之間,5X5按鍵
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
        case 0x85:
        case 0x86:
        case 0x87:
        case 0x88:
        case 0x89:
        break;
        case 0x90:
          DoKeyF1Work();
        break;
        case 0x91:
          DoKeyF2Work();
        break;
        case 0x92:
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
        case 0x97:
        break;
        case 0x98:
          DoKeyEnterWork();
        break;
        default:
        break;
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENCmdSet::LoadBitmapFromFile(void)
{
try {
    FILE *bitmap;
    bitmap=fopen("//cct//bitmap//backGround//4_1_4_1.bit","rb");
    if (bitmap) {
        fread(cmdSetBitmap,3840,1,bitmap);
        fclose(bitmap);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENCmdSet::DisplayCmdSet(void)
{
try {
    smem.SetcFace(cCMDSET);
    smem.vWriteMsgToDOM("Enter Command Setup Screen");
    lcd240x128.DISPLAY_GRAPHIC(0,cmdSetBitmap,128,30);
    cSelect=smem.vGet0FCommandSet();
    if (cSelect<0 || cSelect>3)  cSelect=3;      //default
    lcd240x128.DISPLAY_GRAPHIC_XY(command.X,command.Y,word24x24[cSelect],command.height,command.width/8);
    setSelectPoint16x16(position[cSelect].X,position[cSelect].Y,position[cSelect].height,position[cSelect].width/8);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENCmdSet::InitDispWord(void)
{
    command.X=120;
    command.Y=4;
    command.width=24;
    command.height=24;

    for (int i=0;i<4;i++) {
         position[i].X=32;
         position[i].Y=32+i*16;
         position[i].width=16;
         position[i].height=16;
    }
}
//---------------------------------------------------------------------------
void SCREENCmdSet::DoKey0Work(void)
{
try {
    if (cSelect>=0 && cSelect<=3) clearSelectPoint16x16(position[cSelect].X,position[cSelect].Y,position[cSelect].height,position[cSelect].width/8);
    cSelect=0;
    setSelectPoint16x16(position[cSelect].X,position[cSelect].Y,position[cSelect].height,position[cSelect].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(command.X,command.Y,word24x24[cSelect],command.height,command.width/8);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENCmdSet::DoKey1Work(void)
{
try {
    if (cSelect>=0 && cSelect<=3) clearSelectPoint16x16(position[cSelect].X,position[cSelect].Y,position[cSelect].height,position[cSelect].width/8);
    cSelect=1;
    setSelectPoint16x16(position[cSelect].X,position[cSelect].Y,position[cSelect].height,position[cSelect].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(command.X,command.Y,word24x24[cSelect],command.height,command.width/8);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENCmdSet::DoKey2Work(void)
{
try {
    if (cSelect>=0 && cSelect<=3) clearSelectPoint16x16(position[cSelect].X,position[cSelect].Y,position[cSelect].height,position[cSelect].width/8);
    cSelect=2;
    setSelectPoint16x16(position[cSelect].X,position[cSelect].Y,position[cSelect].height,position[cSelect].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(command.X,command.Y,word24x24[cSelect],command.height,command.width/8);
  } catch(...) {}
}
//---------------------------------------------------------------------------
void SCREENCmdSet::DoKey3Work(void)
{
    if (cSelect>=0 && cSelect<=3) clearSelectPoint16x16(position[cSelect].X,position[cSelect].Y,position[cSelect].height,position[cSelect].width/8);
    cSelect=3;
    setSelectPoint16x16(position[cSelect].X,position[cSelect].Y,position[cSelect].height,position[cSelect].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(command.X,command.Y,word24x24[cSelect],command.height,command.width/8);
}
//---------------------------------------------------------------------------
void SCREENCmdSet::DoKeyF1Work(void)
{
    screenMain.DisplayMain();
}
//---------------------------------------------------------------------------
void SCREENCmdSet::DoKeyF2Work(void)
{
    screenCtlSetup.DisplayCtlSetup();
}
//---------------------------------------------------------------------------
void SCREENCmdSet::DoKeyF4Work(void)
{
    screenFWVerDT.DisplayFWVerDT();
}
//---------------------------------------------------------------------------
void SCREENCmdSet::DoKeyUPWork(void)
{
try {
    clearSelectPoint16x16(position[cSelect].X,position[cSelect].Y,position[cSelect].height,position[cSelect].width/8);
    if (cSelect==0 || cSelect>3) cSelect=1;
    cSelect--;
    setSelectPoint16x16(position[cSelect].X,position[cSelect].Y,position[cSelect].height,position[cSelect].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(command.X,command.Y,word24x24[cSelect],command.height,command.width/8);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENCmdSet::DoKeyDOWNWork(void)
{
try {
    clearSelectPoint16x16(position[cSelect].X,position[cSelect].Y,position[cSelect].height,position[cSelect].width/8);
    cSelect++;
    if (cSelect>3) cSelect=3;
    setSelectPoint16x16(position[cSelect].X,position[cSelect].Y,position[cSelect].height,position[cSelect].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(command.X,command.Y,word24x24[cSelect],command.height,command.width/8);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENCmdSet::DoKeyEnterWork(void)
{
try {
    if (smem.GetDbOperStat()==0 || smem.vLoadCenterConnectStatus()==false) {

        if (cSelect==0)  smem.vWriteMsgToDOM("Accept Command B , In Wireless Area");
        else if (cSelect==1)  smem.vWriteMsgToDOM("Accept Command B");
        else if (cSelect==2)  smem.vWriteMsgToDOM("Accept Command B,A");
        else if (cSelect==3)  smem.vWriteMsgToDOM("Accept Command B,A,O");

        smem.vSet0FCommandSet(cSelect);
        screenFWVerDT.DisplayFWVerDT();
    } else {
        smem.SetLastFace(cCMDSET);
        screenLockdb.DisplayLockdb();
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
