#include "screenRedCountHWCheck.h"
#include "LCD240x128.h"
//#include "screenLogo.h"
#include "SCREENMain.h"
#include "screenRunTableList.h"
#include "CSTC.h"

#include "WRITEJOB.h"
#include "CDataToMessageOK.h"

#include "screenRedCountMenu.h"
#include "screenRedCountHWCheckSel.h"

#include <stdio.h>

#include "SMEM.h"
//---------------------------------------------------------------------------
ScreenRedCountHWCheck screenRedCountHWCheck;
//---------------------------------------------------------------------------
ScreenRedCountHWCheck::ScreenRedCountHWCheck(void)
{
    loadBitmapFromFile();
    initDispWord();
    cSelect=0;
}
//---------------------------------------------------------------------------
ScreenRedCountHWCheck::~ScreenRedCountHWCheck(void)
{
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheck::loadBitmapFromFile(void)
{
try {
    FILE *bitmap;
    bitmap=fopen("//cct//bitmap//backGround//TC5F//RedCountHWCheckBitmap.bit","rb");
    if (bitmap) {
        fread(ucRedCountHWCheckBitmap,3840,1,bitmap);
        fclose(bitmap);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheck::DisplayRedCountHWCheck(unsigned short int usiIDOfCheck)
{
try {
    int iTmp;
    unsigned char ucData[6];
    MESSAGEOK _MsgOK;
    smem.SetcFace(cREDCOUNTHWCHECK);
    lcd240x128.DISPLAY_GRAPHIC(0,ucRedCountHWCheckBitmap,128,30);
    ucID = usiIDOfCheck & 0xFF;

    if(usiIDOfCheck == 0x0F) { usiIDOfCheck = 65535; }

    ucData[0] = 0xEA;
    ucData[1] = 0x43;

    iTmp = smem.vGetINTData(TC92_RedCountVer);
    if( iTmp == TC_RedCountVer94 || iTmp == TC_RedCountVer94v2)
    {
      _MsgOK = oDataToMessageOK.vPackageINFOToVer94RedCount(0x83, ucID, 0x64, 0x64, 0x64, 0x64);
      _MsgOK.InnerOrOutWard = cOutWard;
      writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICEREDCOUNTVER94);
    } else if( iTmp == TC_RedCountVerCCT97) {
        _MsgOK = oDataToMessageOK.vPackageINFOTo92ProtocolSetADDR(ucData, 2, 0x36, usiIDOfCheck);      //brocasting
        _MsgOK.InnerOrOutWard = cOutWard;
        writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICEREDCOUNTVERCCT97);
    }

  } catch (...) {}
}

//---------------------------------------------------------------------------
void ScreenRedCountHWCheck::vRefreshRedCountHWCheck(unsigned char ucIDInput, unsigned char ucA, unsigned char ucB)
{
try {

  if( smem.GetcFace() == cREDCOUNTHWCHECK)
  {

    if(ucIDInput == ucID)
    {

      DATA_Bit _A, _B;
      _A.DBit = ucA;
      _B.DBit = ucB;

      if( _A.switchBit.b1 == 1 ) {
        lcd240x128.DISPLAY_GRAPHIC_XY(markLeftWord[0].X,markLeftWord[0].Y,word8x16[1],markLeftWord[0].height,markLeftWord[0].width/8);
      } else {
        lcd240x128.DISPLAY_GRAPHIC_XY(markLeftWord[0].X,markLeftWord[0].Y,word8x16[0],markLeftWord[0].height,markLeftWord[0].width/8);
      }
      if( _A.switchBit.b2 == 1 ) {
        lcd240x128.DISPLAY_GRAPHIC_XY(markLeftWord[1].X,markLeftWord[1].Y,word8x16[1],markLeftWord[1].height,markLeftWord[1].width/8);
      } else {
        lcd240x128.DISPLAY_GRAPHIC_XY(markLeftWord[1].X,markLeftWord[1].Y,word8x16[0],markLeftWord[1].height,markLeftWord[1].width/8);
      }
      if( _A.switchBit.b3 == 1 ) {
        lcd240x128.DISPLAY_GRAPHIC_XY(markLeftWord[2].X,markLeftWord[2].Y,word8x16[1],markLeftWord[2].height,markLeftWord[2].width/8);
      } else {
        lcd240x128.DISPLAY_GRAPHIC_XY(markLeftWord[2].X,markLeftWord[2].Y,word8x16[0],markLeftWord[2].height,markLeftWord[2].width/8);
      }
      if( _A.switchBit.b4 == 1 ) {
        lcd240x128.DISPLAY_GRAPHIC_XY(markLeftWord[3].X,markLeftWord[3].Y,word8x16[1],markLeftWord[3].height,markLeftWord[3].width/8);
      } else {
        lcd240x128.DISPLAY_GRAPHIC_XY(markLeftWord[3].X,markLeftWord[3].Y,word8x16[0],markLeftWord[3].height,markLeftWord[3].width/8);
      }
      if( _A.switchBit.b5 == 1 ) {
        lcd240x128.DISPLAY_GRAPHIC_XY(markLeftWord[4].X,markLeftWord[4].Y,word8x16[1],markLeftWord[4].height,markLeftWord[4].width/8);
      } else {
        lcd240x128.DISPLAY_GRAPHIC_XY(markLeftWord[4].X,markLeftWord[4].Y,word8x16[0],markLeftWord[4].height,markLeftWord[4].width/8);
      }
      if( _A.switchBit.b6 == 1 ) {
        lcd240x128.DISPLAY_GRAPHIC_XY(markLeftWord[5].X,markLeftWord[5].Y,word8x16[1],markLeftWord[5].height,markLeftWord[5].width/8);
      } else {
        lcd240x128.DISPLAY_GRAPHIC_XY(markLeftWord[5].X,markLeftWord[5].Y,word8x16[0],markLeftWord[5].height,markLeftWord[5].width/8);
      }
      if( _A.switchBit.b7 == 1 ) {
        lcd240x128.DISPLAY_GRAPHIC_XY(markLeftWord[6].X,markLeftWord[6].Y,word8x16[1],markLeftWord[6].height,markLeftWord[6].width/8);
      } else {
        lcd240x128.DISPLAY_GRAPHIC_XY(markLeftWord[6].X,markLeftWord[6].Y,word8x16[0],markLeftWord[6].height,markLeftWord[6].width/8);
      }

      if( _B.switchBit.b1 == 1 ) {
        lcd240x128.DISPLAY_GRAPHIC_XY(markRightWord[0].X,markRightWord[0].Y,word8x16[1],markRightWord[0].height,markRightWord[0].width/8);
      } else {
        lcd240x128.DISPLAY_GRAPHIC_XY(markRightWord[0].X,markRightWord[0].Y,word8x16[0],markRightWord[0].height,markRightWord[0].width/8);
      }
      if( _B.switchBit.b2 == 1 ) {
        lcd240x128.DISPLAY_GRAPHIC_XY(markRightWord[1].X,markRightWord[1].Y,word8x16[1],markRightWord[1].height,markRightWord[1].width/8);
      } else {
        lcd240x128.DISPLAY_GRAPHIC_XY(markRightWord[1].X,markRightWord[1].Y,word8x16[0],markRightWord[1].height,markRightWord[1].width/8);
      }
      if( _B.switchBit.b3 == 1 ) {
        lcd240x128.DISPLAY_GRAPHIC_XY(markRightWord[2].X,markRightWord[2].Y,word8x16[1],markRightWord[2].height,markRightWord[2].width/8);
      } else {
        lcd240x128.DISPLAY_GRAPHIC_XY(markRightWord[2].X,markRightWord[2].Y,word8x16[0],markRightWord[2].height,markRightWord[2].width/8);
      }
      if( _B.switchBit.b4 == 1 ) {
        lcd240x128.DISPLAY_GRAPHIC_XY(markRightWord[3].X,markRightWord[3].Y,word8x16[1],markRightWord[3].height,markRightWord[3].width/8);
      } else {
        lcd240x128.DISPLAY_GRAPHIC_XY(markRightWord[3].X,markRightWord[3].Y,word8x16[0],markRightWord[3].height,markRightWord[3].width/8);
      }
      if( _B.switchBit.b5 == 1 ) {
        lcd240x128.DISPLAY_GRAPHIC_XY(markRightWord[4].X,markRightWord[4].Y,word8x16[1],markRightWord[4].height,markRightWord[4].width/8);
      } else {
        lcd240x128.DISPLAY_GRAPHIC_XY(markRightWord[4].X,markRightWord[4].Y,word8x16[0],markRightWord[4].height,markRightWord[4].width/8);
      }
      if( _B.switchBit.b6 == 1 ) {
        lcd240x128.DISPLAY_GRAPHIC_XY(markRightWord[5].X,markRightWord[5].Y,word8x16[1],markRightWord[5].height,markRightWord[5].width/8);
      } else {
        lcd240x128.DISPLAY_GRAPHIC_XY(markRightWord[5].X,markRightWord[5].Y,word8x16[0],markRightWord[5].height,markRightWord[5].width/8);
      }
      if( _B.switchBit.b7 == 1 ) {
        lcd240x128.DISPLAY_GRAPHIC_XY(markRightWord[6].X,markRightWord[6].Y,word8x16[1],markRightWord[6].height,markRightWord[6].width/8);
      } else {
        lcd240x128.DISPLAY_GRAPHIC_XY(markRightWord[6].X,markRightWord[6].Y,word8x16[0],markRightWord[6].height,markRightWord[6].width/8);
      }
    }
  }

} catch(...){}
}

//---------------------------------------------------------------------------
void ScreenRedCountHWCheck::initDispWord(void)
{
try {

  for(int i = 0; i < 7; i++) {
    markLeftWord[i].width = 8;
    markRightWord[i].width = 8;
    markLeftWord[i].height = 16;
    markRightWord[i].height = 16;
  }

  markLeftWord[0].X = 72;   markLeftWord[0].Y = 2;
  markLeftWord[1].X = 72;   markLeftWord[1].Y = 32;
  markLeftWord[2].X = 72;   markLeftWord[2].Y = 72;
  markLeftWord[3].X = 48;   markLeftWord[3].Y = 80;
  markLeftWord[4].X = 8;    markLeftWord[4].Y = 72;
  markLeftWord[5].X = 8;    markLeftWord[5].Y = 32;
  markLeftWord[6].X = 48;   markLeftWord[6].Y = 40;

  markRightWord[0].X = 8+80;   markRightWord[0].Y = 2;
  markRightWord[1].X = 72+80;   markRightWord[1].Y = 32;
  markRightWord[2].X = 72+80;   markRightWord[2].Y = 72;
  markRightWord[3].X = 48+80;   markRightWord[3].Y = 80;
  markRightWord[4].X = 8+80;    markRightWord[4].Y = 72;
  markRightWord[5].X = 8+80;    markRightWord[5].Y = 32;
  markRightWord[6].X = 48+80;   markRightWord[6].Y = 40;

  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheck::doKeyWork(BYTE key)
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
void ScreenRedCountHWCheck::doKey0Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheck::doKey1Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheck::doKey2Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheck::doKey3Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheck::doKey4Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheck::doKey5Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheck::doKey6Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheck::doKey7Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheck::doKey8Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheck::doKey9Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheck::doKeyAWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheck::doKeyBWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheck::doKeyCWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheck::doKeyDWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheck::doKeyEWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheck::doKeyFWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheck::doKeyF1Work(void)
{
    screenMain.DisplayMain();
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheck::doKeyF2Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheck::doKeyF3Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheck::doKeyF4Work(void)
{
    screenRedCountHWCheckSel.DisplayRedCountHWCheckSel();
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheck::doKeyUPWork(void)
{
try {
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheck::doKeyDOWNWork(void)
{
try {
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheck::doKeyLEFTWork(void)
{
try {
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheck::doKeyRIGHTWork(void)
{
try {
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheck::doKeyEnterWork(void)
{
    switch (cSelect+1) {
        case 1:
          doKey1Work();
        break;
        case 2:
          doKey2Work();
        break;
        case 3:
          doKey3Work();
        break;
        case 4:
          doKey4Work();
        break;
        case 5:
          doKey5Work();
        break;
        case 6:
          doKey6Work();
        break;
        case 7:
          doKey7Work();
        break;
        case 8:
          doKey8Work();
        break;
        case 9:
          doKey9Work();
        break;
        case 10:
          doKey0Work();
        break;
        default:
        break;
    }
}
//---------------------------------------------------------------------------

