#include "screenRedCountHWCheckCycleNum.h"
#include "LCD240x128.h"
#include "SCREENMain.h"

#include "screenRedCountMenu.h"

#include "WRITEJOB.h"
#include "CDataToMessageOK.h"

#include <stdio.h>
#include "SMEM.h"
//---------------------------------------------------------------------------
ScreenRedCountHWCheckCycleNum screenRedCountHWCheckCycleNum;
//---------------------------------------------------------------------------
ScreenRedCountHWCheckCycleNum::ScreenRedCountHWCheckCycleNum(void)
{
    loadBitmapFromFile();
    initDispWord();
    cPosition=0;
}
//---------------------------------------------------------------------------
ScreenRedCountHWCheckCycleNum::~ScreenRedCountHWCheckCycleNum(void)
{
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheckCycleNum::loadBitmapFromFile(void)
{
try {
    FILE *bitmap;
    bitmap=fopen("//cct//bitmap//backGround//TC5F//RedCountHWCheckSelect.bit","rb");
    if (bitmap) {
        fread(RedCountHWCheckSelBitmap,3840,1,bitmap);
        fclose(bitmap);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheckCycleNum::DisplayRedCountHWCheckCycleNum(void)
{
try {
    smem.SetcFace(cREDCOUNTHWCHECKCYCLENUM);
    lcd240x128.DISPLAY_GRAPHIC(0,RedCountHWCheckSelBitmap,128,30);
    DisplaySetSegtype(0);
    cPosition=0;
    setCursor24x24(redCountHWCheckCycleNum.X,redCountHWCheckCycleNum.Y+24);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheckCycleNum::initDispWord(void)
{
         redCountHWCheckCycleNum.X=96;
         redCountHWCheckCycleNum.Y=68;
         redCountHWCheckCycleNum.width=24;
         redCountHWCheckCycleNum.height=24;
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheckCycleNum::DisplaySetSegtype(int a)
{
try {
    lcd240x128.DISPLAY_GRAPHIC_XY(redCountHWCheckCycleNum.X,redCountHWCheckCycleNum.Y,word24x24[a],redCountHWCheckCycleNum.height,redCountHWCheckCycleNum.width/8);
    redCountHWCheckTmp = a;
  } catch (...){}
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheckCycleNum::doKeyWork(BYTE key)
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
          doKeyAWork();
        break;
        case 0x8B:
          doKeyBWork();
        break;
        case 0x8C:
          doKeyCWork();
        break;
        case 0x8D:
          doKeyDWork();
        break;
        case 0x8E:
          doKeyEWork();
        break;
        case 0x8F:
          doKeyFWork();
        break;
        case 0x90:
          doKeyF1Work();
        break;
        case 0x91:
          doKeyF2Work();
        break;
        case 0x92:
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
        case 0x98:
          doKeyEnterWork();
        break;
        default:
        break;
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheckCycleNum::doKey0Work(void)
{
try {
    lcd240x128.DISPLAY_GRAPHIC_XY(redCountHWCheckCycleNum.X,redCountHWCheckCycleNum.Y,word24x24[0],redCountHWCheckCycleNum.height,redCountHWCheckCycleNum.width/8);
    redCountHWCheckTmp = 0;
    doKeyRIGHTWork();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheckCycleNum::doKey1Work(void)
{
try {
    lcd240x128.DISPLAY_GRAPHIC_XY(redCountHWCheckCycleNum.X,redCountHWCheckCycleNum.Y,word24x24[1],redCountHWCheckCycleNum.height,redCountHWCheckCycleNum.width/8);
    redCountHWCheckTmp = 0x01;
    doKeyRIGHTWork();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheckCycleNum::doKey2Work(void)
{
try {
    lcd240x128.DISPLAY_GRAPHIC_XY(redCountHWCheckCycleNum.X,redCountHWCheckCycleNum.Y,word24x24[2],redCountHWCheckCycleNum.height,redCountHWCheckCycleNum.width/8);
    redCountHWCheckTmp=0x02;
    doKeyRIGHTWork();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheckCycleNum::doKey3Work(void)
{
try {
    lcd240x128.DISPLAY_GRAPHIC_XY(redCountHWCheckCycleNum.X,redCountHWCheckCycleNum.Y,word24x24[3],redCountHWCheckCycleNum.height,redCountHWCheckCycleNum.width/8);
    redCountHWCheckTmp=0x03;
    doKeyRIGHTWork();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheckCycleNum::doKey4Work(void)
{
try {
    lcd240x128.DISPLAY_GRAPHIC_XY(redCountHWCheckCycleNum.X,redCountHWCheckCycleNum.Y,word24x24[4],redCountHWCheckCycleNum.height,redCountHWCheckCycleNum.width/8);
    redCountHWCheckTmp=0x04;
    doKeyRIGHTWork();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheckCycleNum::doKey5Work(void)
{
try {
    lcd240x128.DISPLAY_GRAPHIC_XY(redCountHWCheckCycleNum.X,redCountHWCheckCycleNum.Y,word24x24[5],redCountHWCheckCycleNum.height,redCountHWCheckCycleNum.width/8);
    redCountHWCheckTmp=0x05;
    doKeyRIGHTWork();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheckCycleNum::doKey6Work(void)
{
try {
    lcd240x128.DISPLAY_GRAPHIC_XY(redCountHWCheckCycleNum.X,redCountHWCheckCycleNum.Y,word24x24[6],redCountHWCheckCycleNum.height,redCountHWCheckCycleNum.width/8);
    redCountHWCheckTmp = 0x06;
    doKeyRIGHTWork();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheckCycleNum::doKey7Work(void)
{
try {
    lcd240x128.DISPLAY_GRAPHIC_XY(redCountHWCheckCycleNum.X,redCountHWCheckCycleNum.Y,word24x24[7],redCountHWCheckCycleNum.height,redCountHWCheckCycleNum.width/8);
    redCountHWCheckTmp=0x07;
    doKeyRIGHTWork();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheckCycleNum::doKey8Work(void)
{
try {
    lcd240x128.DISPLAY_GRAPHIC_XY(redCountHWCheckCycleNum.X,redCountHWCheckCycleNum.Y,word24x24[8],redCountHWCheckCycleNum.height,redCountHWCheckCycleNum.width/8);
    redCountHWCheckTmp=0x08;
    doKeyRIGHTWork();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheckCycleNum::doKey9Work(void)
{
try {
    lcd240x128.DISPLAY_GRAPHIC_XY(redCountHWCheckCycleNum.X,redCountHWCheckCycleNum.Y,word24x24[9],redCountHWCheckCycleNum.height,redCountHWCheckCycleNum.width/8);
    redCountHWCheckTmp=0x09;
    doKeyRIGHTWork();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheckCycleNum::doKeyAWork(void)
{
  lcd240x128.DISPLAY_GRAPHIC_XY(redCountHWCheckCycleNum.X,redCountHWCheckCycleNum.Y,word24x24[0x0A],redCountHWCheckCycleNum.height,redCountHWCheckCycleNum.width/8);
  redCountHWCheckTmp=0x0A;
  doKeyRIGHTWork();

}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheckCycleNum::doKeyBWork(void)
{
  lcd240x128.DISPLAY_GRAPHIC_XY(redCountHWCheckCycleNum.X,redCountHWCheckCycleNum.Y,word24x24[0x0B],redCountHWCheckCycleNum.height,redCountHWCheckCycleNum.width/8);
  redCountHWCheckTmp=0x0B;
  doKeyRIGHTWork();
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheckCycleNum::doKeyCWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheckCycleNum::doKeyDWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheckCycleNum::doKeyEWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheckCycleNum::doKeyFWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheckCycleNum::doKeyF1Work(void)
{
    screenMain.DisplayMain();
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheckCycleNum::doKeyF2Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheckCycleNum::doKeyF3Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheckCycleNum::doKeyF4Work(void)
{
    screenRedCountMenu.DisplayRedCountMenu();
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheckCycleNum::doKeyUPWork(void)
{
    DisplaySetSegtype(redCountHWCheckTmp);
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheckCycleNum::doKeyDOWNWork(void)
{
    DisplaySetSegtype(redCountHWCheckTmp);
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheckCycleNum::doKeyLEFTWork(void)
{
try {
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheckCycleNum::doKeyRIGHTWork(void)
{
try {
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheckCycleNum::doKeyEnterWork(void)
{
    int x=redCountHWCheckTmp;
    unsigned char ucData[6];
    MESSAGEOK _MsgOK;

    if (x == 0x0B) { x = 65535; }

    ucData[0] = 0xEA;
    ucData[1] = 0x11;
    ucData[2] = 0x64;      //force define 100sec cycle

    _MsgOK = oDataToMessageOK.vPackageINFOTo92ProtocolSetADDR(ucData, 3, 0x36, x);
    _MsgOK.InnerOrOutWard = cOutWard;
    writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICEREDCOUNTVERCCT97);

    screenRedCountMenu.DisplayRedCountMenu();

}
//---------------------------------------------------------------------------
