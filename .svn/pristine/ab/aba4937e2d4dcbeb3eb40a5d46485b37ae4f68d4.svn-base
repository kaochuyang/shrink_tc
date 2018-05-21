#include "screenRedCountHWCheckNum88.h"
#include "LCD240x128.h"
#include "SCREENMain.h"

#include "screenRedCountMenu.h"

#include "WRITEJOB.h"
#include "CDataToMessageOK.h"

#include <stdio.h>
#include "SMEM.h"
//---------------------------------------------------------------------------
ScreenRedCountHWCheckNum88 screenRedCountHWCheckNum88;
//---------------------------------------------------------------------------
ScreenRedCountHWCheckNum88::ScreenRedCountHWCheckNum88(void)
{
    loadBitmapFromFile();
    initDispWord();
    cPosition=0;
}
//---------------------------------------------------------------------------
ScreenRedCountHWCheckNum88::~ScreenRedCountHWCheckNum88(void)
{
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheckNum88::loadBitmapFromFile(void)
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
void ScreenRedCountHWCheckNum88::DisplayRedCountHWCheckNum88(void)
{
try {
    smem.SetcFace(cREDCOUNTHWCHECKNUM88);
    lcd240x128.DISPLAY_GRAPHIC(0,RedCountHWCheckSelBitmap,128,30);
    DisplaySetSegtype(0);
    cPosition=0;
    setCursor24x24(redCountHWCheckNum88.X,redCountHWCheckNum88.Y+24);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheckNum88::initDispWord(void)
{
         redCountHWCheckNum88.X=96;
         redCountHWCheckNum88.Y=68;
         redCountHWCheckNum88.width=24;
         redCountHWCheckNum88.height=24;
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheckNum88::DisplaySetSegtype(int a)
{
try {
    lcd240x128.DISPLAY_GRAPHIC_XY(redCountHWCheckNum88.X,redCountHWCheckNum88.Y,word24x24[a],redCountHWCheckNum88.height,redCountHWCheckNum88.width/8);
    redCountHWCheckTmp = a;
  } catch (...){}
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheckNum88::doKeyWork(BYTE key)
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
void ScreenRedCountHWCheckNum88::doKey0Work(void)
{
try {
    lcd240x128.DISPLAY_GRAPHIC_XY(redCountHWCheckNum88.X,redCountHWCheckNum88.Y,word24x24[0],redCountHWCheckNum88.height,redCountHWCheckNum88.width/8);
    redCountHWCheckTmp = 0;
    doKeyRIGHTWork();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheckNum88::doKey1Work(void)
{
try {
    lcd240x128.DISPLAY_GRAPHIC_XY(redCountHWCheckNum88.X,redCountHWCheckNum88.Y,word24x24[1],redCountHWCheckNum88.height,redCountHWCheckNum88.width/8);
    redCountHWCheckTmp = 0x01;
    doKeyRIGHTWork();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheckNum88::doKey2Work(void)
{
try {
    lcd240x128.DISPLAY_GRAPHIC_XY(redCountHWCheckNum88.X,redCountHWCheckNum88.Y,word24x24[2],redCountHWCheckNum88.height,redCountHWCheckNum88.width/8);
    redCountHWCheckTmp=0x02;
    doKeyRIGHTWork();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheckNum88::doKey3Work(void)
{
try {
    lcd240x128.DISPLAY_GRAPHIC_XY(redCountHWCheckNum88.X,redCountHWCheckNum88.Y,word24x24[3],redCountHWCheckNum88.height,redCountHWCheckNum88.width/8);
    redCountHWCheckTmp=0x03;
    doKeyRIGHTWork();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheckNum88::doKey4Work(void)
{
try {
    lcd240x128.DISPLAY_GRAPHIC_XY(redCountHWCheckNum88.X,redCountHWCheckNum88.Y,word24x24[4],redCountHWCheckNum88.height,redCountHWCheckNum88.width/8);
    redCountHWCheckTmp=0x04;
    doKeyRIGHTWork();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheckNum88::doKey5Work(void)
{
try {
    lcd240x128.DISPLAY_GRAPHIC_XY(redCountHWCheckNum88.X,redCountHWCheckNum88.Y,word24x24[5],redCountHWCheckNum88.height,redCountHWCheckNum88.width/8);
    redCountHWCheckTmp=0x05;
    doKeyRIGHTWork();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheckNum88::doKey6Work(void)
{
try {
    lcd240x128.DISPLAY_GRAPHIC_XY(redCountHWCheckNum88.X,redCountHWCheckNum88.Y,word24x24[6],redCountHWCheckNum88.height,redCountHWCheckNum88.width/8);
    redCountHWCheckTmp = 0x06;
    doKeyRIGHTWork();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheckNum88::doKey7Work(void)
{
try {
    lcd240x128.DISPLAY_GRAPHIC_XY(redCountHWCheckNum88.X,redCountHWCheckNum88.Y,word24x24[7],redCountHWCheckNum88.height,redCountHWCheckNum88.width/8);
    redCountHWCheckTmp=0x07;
    doKeyRIGHTWork();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheckNum88::doKey8Work(void)
{
try {
    lcd240x128.DISPLAY_GRAPHIC_XY(redCountHWCheckNum88.X,redCountHWCheckNum88.Y,word24x24[8],redCountHWCheckNum88.height,redCountHWCheckNum88.width/8);
    redCountHWCheckTmp=0x08;
    doKeyRIGHTWork();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheckNum88::doKey9Work(void)
{
try {
    lcd240x128.DISPLAY_GRAPHIC_XY(redCountHWCheckNum88.X,redCountHWCheckNum88.Y,word24x24[9],redCountHWCheckNum88.height,redCountHWCheckNum88.width/8);
    redCountHWCheckTmp=0x09;
    doKeyRIGHTWork();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheckNum88::doKeyAWork(void)
{
  lcd240x128.DISPLAY_GRAPHIC_XY(redCountHWCheckNum88.X,redCountHWCheckNum88.Y,word24x24[0x0A],redCountHWCheckNum88.height,redCountHWCheckNum88.width/8);
  redCountHWCheckTmp=0x0A;
  doKeyRIGHTWork();

}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheckNum88::doKeyBWork(void)
{
  lcd240x128.DISPLAY_GRAPHIC_XY(redCountHWCheckNum88.X,redCountHWCheckNum88.Y,word24x24[0x0B],redCountHWCheckNum88.height,redCountHWCheckNum88.width/8);
  redCountHWCheckTmp=0x0B;
  doKeyRIGHTWork();
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheckNum88::doKeyCWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheckNum88::doKeyDWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheckNum88::doKeyEWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheckNum88::doKeyFWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheckNum88::doKeyF1Work(void)
{
    screenMain.DisplayMain();
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheckNum88::doKeyF2Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheckNum88::doKeyF3Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheckNum88::doKeyF4Work(void)
{
    screenRedCountMenu.DisplayRedCountMenu();
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheckNum88::doKeyUPWork(void)
{
    DisplaySetSegtype(redCountHWCheckTmp);
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheckNum88::doKeyDOWNWork(void)
{
    DisplaySetSegtype(redCountHWCheckTmp);
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheckNum88::doKeyLEFTWork(void)
{
try {
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheckNum88::doKeyRIGHTWork(void)
{
try {
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheckNum88::doKeyEnterWork(void)
{
    int x=redCountHWCheckTmp;
    unsigned char ucData[6];
    MESSAGEOK _MsgOK;

    if (x == 0x0B) { x = 65535; }

    ucData[0] = 0xEA;
    ucData[1] = 0x13;

    _MsgOK = oDataToMessageOK.vPackageINFOTo92ProtocolSetADDR(ucData, 2, 0x36, x);
    _MsgOK.InnerOrOutWard = cOutWard;
    writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICEREDCOUNTVERCCT97);

    screenRedCountMenu.DisplayRedCountMenu();

}
//---------------------------------------------------------------------------
