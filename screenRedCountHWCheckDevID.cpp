#include "screenRedCountHWCheckDevID.h"
#include "LCD240x128.h"
#include "SCREENMain.h"

#include "screenRedCountMenu.h"

#include "WRITEJOB.h"
#include "CDataToMessageOK.h"

#include <stdio.h>
#include "SMEM.h"
//---------------------------------------------------------------------------
ScreenRedCountHWCheckDevID screenRedCountHWCheckDevID;
//---------------------------------------------------------------------------
ScreenRedCountHWCheckDevID::ScreenRedCountHWCheckDevID(void)
{
    loadBitmapFromFile();
    initDispWord();
    cPosition=0;
}
//---------------------------------------------------------------------------
ScreenRedCountHWCheckDevID::~ScreenRedCountHWCheckDevID(void)
{
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheckDevID::loadBitmapFromFile(void)
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
void ScreenRedCountHWCheckDevID::DisplayRedCountHWCheckDevID(void)
{
try {
    smem.SetcFace(cREDCOUNTHWCHECKDEVID);
    lcd240x128.DISPLAY_GRAPHIC(0,RedCountHWCheckSelBitmap,128,30);
    DisplaySetSegtype(0);
    cPosition=0;
    setCursor24x24(redCountHWCheckDevID.X,redCountHWCheckDevID.Y+24);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheckDevID::initDispWord(void)
{
         redCountHWCheckDevID.X=96;
         redCountHWCheckDevID.Y=68;
         redCountHWCheckDevID.width=24;
         redCountHWCheckDevID.height=24;
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheckDevID::DisplaySetSegtype(int a)
{
try {
    lcd240x128.DISPLAY_GRAPHIC_XY(redCountHWCheckDevID.X,redCountHWCheckDevID.Y,word24x24[a],redCountHWCheckDevID.height,redCountHWCheckDevID.width/8);
    redCountHWCheckTmp = a;
  } catch (...){}
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheckDevID::doKeyWork(BYTE key)
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
void ScreenRedCountHWCheckDevID::doKey0Work(void)
{
try {
    lcd240x128.DISPLAY_GRAPHIC_XY(redCountHWCheckDevID.X,redCountHWCheckDevID.Y,word24x24[0],redCountHWCheckDevID.height,redCountHWCheckDevID.width/8);
    redCountHWCheckTmp = 0;
    doKeyRIGHTWork();
    RedCountCheckID(0);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheckDevID::doKey1Work(void)
{
try {
    lcd240x128.DISPLAY_GRAPHIC_XY(redCountHWCheckDevID.X,redCountHWCheckDevID.Y,word24x24[1],redCountHWCheckDevID.height,redCountHWCheckDevID.width/8);
    redCountHWCheckTmp = 0x01;
    doKeyRIGHTWork();
    RedCountCheckID(1);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheckDevID::doKey2Work(void)
{
try {
    lcd240x128.DISPLAY_GRAPHIC_XY(redCountHWCheckDevID.X,redCountHWCheckDevID.Y,word24x24[2],redCountHWCheckDevID.height,redCountHWCheckDevID.width/8);
    redCountHWCheckTmp=0x02;
    doKeyRIGHTWork();
    RedCountCheckID(2);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheckDevID::doKey3Work(void)
{
try {
    lcd240x128.DISPLAY_GRAPHIC_XY(redCountHWCheckDevID.X,redCountHWCheckDevID.Y,word24x24[3],redCountHWCheckDevID.height,redCountHWCheckDevID.width/8);
    redCountHWCheckTmp=0x03;
    doKeyRIGHTWork();
    RedCountCheckID(3);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheckDevID::doKey4Work(void)
{
try {
    lcd240x128.DISPLAY_GRAPHIC_XY(redCountHWCheckDevID.X,redCountHWCheckDevID.Y,word24x24[4],redCountHWCheckDevID.height,redCountHWCheckDevID.width/8);
    redCountHWCheckTmp=0x04;
    doKeyRIGHTWork();
    RedCountCheckID(4);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheckDevID::doKey5Work(void)
{
try {
    lcd240x128.DISPLAY_GRAPHIC_XY(redCountHWCheckDevID.X,redCountHWCheckDevID.Y,word24x24[5],redCountHWCheckDevID.height,redCountHWCheckDevID.width/8);
    redCountHWCheckTmp=0x05;
    doKeyRIGHTWork();
    RedCountCheckID(5);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheckDevID::doKey6Work(void)
{
try {
    lcd240x128.DISPLAY_GRAPHIC_XY(redCountHWCheckDevID.X,redCountHWCheckDevID.Y,word24x24[6],redCountHWCheckDevID.height,redCountHWCheckDevID.width/8);
    redCountHWCheckTmp = 0x06;
    doKeyRIGHTWork();
    RedCountCheckID(6);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheckDevID::doKey7Work(void)
{
try {
    lcd240x128.DISPLAY_GRAPHIC_XY(redCountHWCheckDevID.X,redCountHWCheckDevID.Y,word24x24[7],redCountHWCheckDevID.height,redCountHWCheckDevID.width/8);
    redCountHWCheckTmp=0x07;
    doKeyRIGHTWork();
    RedCountCheckID(7);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheckDevID::doKey8Work(void)
{
try {
    lcd240x128.DISPLAY_GRAPHIC_XY(redCountHWCheckDevID.X,redCountHWCheckDevID.Y,word24x24[8],redCountHWCheckDevID.height,redCountHWCheckDevID.width/8);
    redCountHWCheckTmp=0x08;
    doKeyRIGHTWork();
    RedCountCheckID(8);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheckDevID::doKey9Work(void)
{
try {
    lcd240x128.DISPLAY_GRAPHIC_XY(redCountHWCheckDevID.X,redCountHWCheckDevID.Y,word24x24[9],redCountHWCheckDevID.height,redCountHWCheckDevID.width/8);
    redCountHWCheckTmp=0x09;
    doKeyRIGHTWork();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheckDevID::doKeyAWork(void)
{
  lcd240x128.DISPLAY_GRAPHIC_XY(redCountHWCheckDevID.X,redCountHWCheckDevID.Y,word24x24[0x0A],redCountHWCheckDevID.height,redCountHWCheckDevID.width/8);
  redCountHWCheckTmp=0x0A;
  doKeyRIGHTWork();

}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheckDevID::doKeyBWork(void)
{
  lcd240x128.DISPLAY_GRAPHIC_XY(redCountHWCheckDevID.X,redCountHWCheckDevID.Y,word24x24[0x0B],redCountHWCheckDevID.height,redCountHWCheckDevID.width/8);
  redCountHWCheckTmp=0x0B;
  doKeyRIGHTWork();
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheckDevID::doKeyCWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheckDevID::doKeyDWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheckDevID::doKeyEWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheckDevID::doKeyFWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheckDevID::doKeyF1Work(void)
{
    screenMain.DisplayMain();
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheckDevID::doKeyF2Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheckDevID::doKeyF3Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheckDevID::doKeyF4Work(void)
{
    screenRedCountMenu.DisplayRedCountMenu();
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheckDevID::doKeyUPWork(void)
{
    DisplaySetSegtype(redCountHWCheckTmp);
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheckDevID::doKeyDOWNWork(void)
{
    DisplaySetSegtype(redCountHWCheckTmp);
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheckDevID::doKeyLEFTWork(void)
{
try {
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheckDevID::doKeyRIGHTWork(void)
{
try {
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheckDevID::doKeyEnterWork(void)
{
    int x=redCountHWCheckTmp;
    unsigned char ucData[6];
    MESSAGEOK _MsgOK;

    if (x == 0x0B) { x = 65535; }

    ucData[0] = 0xEA;
    ucData[1] = 0x12;

    _MsgOK = oDataToMessageOK.vPackageINFOTo92ProtocolSetADDR(ucData, 2, 0x36, x);
    _MsgOK.InnerOrOutWard = cOutWard;
    writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICEREDCOUNTVERCCT97);

    screenRedCountMenu.DisplayRedCountMenu();

}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheckDevID::RedCountCheckID(unsigned short int ID)
{
try{
       MESSAGEOK _MsgOK;

       switch (ID){
        case 0:
            _MsgOK = oDataToMessageOK.vPackageINFOToredcountNSP(0 ,20, 0x99, 0x99, 0x99, 0x99, 0x99, 0x99, 0);
            _MsgOK.InnerOrOutWard = cOutWard;
        break;
        case 1:
            _MsgOK = oDataToMessageOK.vPackageINFOToredcountNSP(0x99 ,0x99, 0, 21, 0x99, 0x99, 0x99, 0x99, 0);
            _MsgOK.InnerOrOutWard = cOutWard;
        break;
        case 2:
            _MsgOK = oDataToMessageOK.vPackageINFOToredcountNSP(0x99 ,0x99, 0x99 ,0x99, 0, 22, 0x99, 0x99, 0);
            _MsgOK.InnerOrOutWard = cOutWard;
        break;
        case 3:
            _MsgOK = oDataToMessageOK.vPackageINFOToredcountNSP(0x99 ,0x99, 0x99 ,0x99, 0x99 ,0x99, 0, 23, 0);
            _MsgOK.InnerOrOutWard = cOutWard;
        break;
        case 4:
            _MsgOK = oDataToMessageOK.vPackageINFOToredcountNSP(0 ,24, 0x99 ,0x99, 0x99 ,0x99, 0x99, 0x99, 1);
            _MsgOK.InnerOrOutWard = cOutWard;
        break;
        case 5:
            _MsgOK = oDataToMessageOK.vPackageINFOToredcountNSP(0x99 ,0x99, 0 ,25, 0x99 ,0x99, 0x99, 0x99, 1);
            _MsgOK.InnerOrOutWard = cOutWard;
        break;
        case 6:
            _MsgOK = oDataToMessageOK.vPackageINFOToredcountNSP(0x99 ,0x99, 0x99 ,0x99, 0 ,26, 0x99, 0x99, 1);
            _MsgOK.InnerOrOutWard = cOutWard;
        break;
        case 7:
            _MsgOK = oDataToMessageOK.vPackageINFOToredcountNSP(0x99 ,0x99, 0x99 ,0x99, 0x99 ,0x99, 0, 27, 1);
            _MsgOK.InnerOrOutWard = cOutWard;
        break;
        default:
        break;
    }

    writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICENSPREDCOUNT);

 } catch (...) {}
}
