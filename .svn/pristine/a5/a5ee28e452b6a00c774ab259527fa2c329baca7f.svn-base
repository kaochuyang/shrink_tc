#include "screenRedCountHWCheckSel.h"
#include "LCD240x128.h"
#include "SCREENMain.h"

#include "screenRedCountHWCheck.h"
#include "screenRedCountMenu.h"

#include <stdio.h>
#include "SMEM.h"
//---------------------------------------------------------------------------
ScreenRedCountHWCheckSel screenRedCountHWCheckSel;
//---------------------------------------------------------------------------
ScreenRedCountHWCheckSel::ScreenRedCountHWCheckSel(void)
{
    loadBitmapFromFile();
    initDispWord();
    cPosition=0;
}
//---------------------------------------------------------------------------
ScreenRedCountHWCheckSel::~ScreenRedCountHWCheckSel(void)
{
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheckSel::loadBitmapFromFile(void)
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
void ScreenRedCountHWCheckSel::DisplayRedCountHWCheckSel(void)
{
try {
    smem.SetcFace(cREDCOUNTHWCHECKSEL);
    lcd240x128.DISPLAY_GRAPHIC(0,RedCountHWCheckSelBitmap,128,30);
    DisplaySetSegtype(0);
    cPosition=0;
    setCursor24x24(redCountHWCheckSel.X,redCountHWCheckSel.Y+24);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheckSel::initDispWord(void)
{
         redCountHWCheckSel.X=96;
         redCountHWCheckSel.Y=68;
         redCountHWCheckSel.width=24;
         redCountHWCheckSel.height=24;

         int iXptr = 128;
         int iYptr = 0;
         for (int i=0;i<8;i++) {
              RedCountSec[i].X = iXptr; iXptr+= 8;
              RedCountSec[i].Y = iYptr;
              RedCountSec[i].width = 8;
              RedCountSec[i].height = 16;
              if(i%2 == 1) { iXptr+= 8; }
         }

         iXptr = 128;
         iYptr = 16;
         for (int i = 8;i < 16; i++) {
              RedCountSec[i].X = iXptr; iXptr+= 8;
              RedCountSec[i].Y = iYptr;
              RedCountSec[i].width = 8;
              RedCountSec[i].height = 16;
              if(i%2 == 1) { iXptr+= 8; }
         }

}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheckSel::DisplaySetSegtype(int a)
{
try {
    lcd240x128.DISPLAY_GRAPHIC_XY(redCountHWCheckSel.X,redCountHWCheckSel.Y,word24x24[a],redCountHWCheckSel.height,redCountHWCheckSel.width/8);
    redCountHWCheckTmp = a;
  } catch (...){}
}

//---------------------------------------------------------------------------
void ScreenRedCountHWCheckSel::DisplayRedCountSec(unsigned char ucA, unsigned char ucB, unsigned char ucC, unsigned char ucD, unsigned char ucE, unsigned char ucF, unsigned char ucG, unsigned char ucH)
{
try {
  lcd240x128.DISPLAY_GRAPHIC_XY(RedCountSec[0].X,RedCountSec[0].Y,word8x16[ucA/10],RedCountSec[0].height,RedCountSec[0].width/8);
  lcd240x128.DISPLAY_GRAPHIC_XY(RedCountSec[1].X,RedCountSec[1].Y,word8x16[ucA%10],RedCountSec[1].height,RedCountSec[1].width/8);
  lcd240x128.DISPLAY_GRAPHIC_XY(RedCountSec[2].X,RedCountSec[2].Y,word8x16[ucB/10],RedCountSec[2].height,RedCountSec[2].width/8);
  lcd240x128.DISPLAY_GRAPHIC_XY(RedCountSec[3].X,RedCountSec[3].Y,word8x16[ucB%10],RedCountSec[3].height,RedCountSec[3].width/8);
  lcd240x128.DISPLAY_GRAPHIC_XY(RedCountSec[4].X,RedCountSec[4].Y,word8x16[ucC/10],RedCountSec[4].height,RedCountSec[4].width/8);
  lcd240x128.DISPLAY_GRAPHIC_XY(RedCountSec[5].X,RedCountSec[5].Y,word8x16[ucC%10],RedCountSec[5].height,RedCountSec[5].width/8);
  lcd240x128.DISPLAY_GRAPHIC_XY(RedCountSec[6].X,RedCountSec[6].Y,word8x16[ucD/10],RedCountSec[6].height,RedCountSec[6].width/8);
  lcd240x128.DISPLAY_GRAPHIC_XY(RedCountSec[7].X,RedCountSec[7].Y,word8x16[ucD%10],RedCountSec[7].height,RedCountSec[7].width/8);
  lcd240x128.DISPLAY_GRAPHIC_XY(RedCountSec[8].X,RedCountSec[8].Y,word8x16[ucE/10],RedCountSec[8].height,RedCountSec[8].width/8);
  lcd240x128.DISPLAY_GRAPHIC_XY(RedCountSec[9].X,RedCountSec[9].Y,word8x16[ucE%10],RedCountSec[9].height,RedCountSec[9].width/8);
  lcd240x128.DISPLAY_GRAPHIC_XY(RedCountSec[10].X,RedCountSec[10].Y,word8x16[ucF/10],RedCountSec[10].height,RedCountSec[10].width/8);
  lcd240x128.DISPLAY_GRAPHIC_XY(RedCountSec[11].X,RedCountSec[11].Y,word8x16[ucF%10],RedCountSec[11].height,RedCountSec[11].width/8);
  lcd240x128.DISPLAY_GRAPHIC_XY(RedCountSec[12].X,RedCountSec[12].Y,word8x16[ucG/10],RedCountSec[12].height,RedCountSec[12].width/8);
  lcd240x128.DISPLAY_GRAPHIC_XY(RedCountSec[13].X,RedCountSec[13].Y,word8x16[ucG%10],RedCountSec[13].height,RedCountSec[13].width/8);
  lcd240x128.DISPLAY_GRAPHIC_XY(RedCountSec[14].X,RedCountSec[14].Y,word8x16[ucH/10],RedCountSec[14].height,RedCountSec[14].width/8);
  lcd240x128.DISPLAY_GRAPHIC_XY(RedCountSec[15].X,RedCountSec[15].Y,word8x16[ucH%10],RedCountSec[15].height,RedCountSec[15].width/8);

  } catch (...){}
}


//---------------------------------------------------------------------------
void ScreenRedCountHWCheckSel::doKeyWork(BYTE key)
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
void ScreenRedCountHWCheckSel::doKey0Work(void)
{
try {
    lcd240x128.DISPLAY_GRAPHIC_XY(redCountHWCheckSel.X,redCountHWCheckSel.Y,word24x24[0],redCountHWCheckSel.height,redCountHWCheckSel.width/8);
    redCountHWCheckTmp = 0;
    doKeyRIGHTWork();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheckSel::doKey1Work(void)
{
try {
    lcd240x128.DISPLAY_GRAPHIC_XY(redCountHWCheckSel.X,redCountHWCheckSel.Y,word24x24[1],redCountHWCheckSel.height,redCountHWCheckSel.width/8);
    redCountHWCheckTmp = 0x01;
    doKeyRIGHTWork();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheckSel::doKey2Work(void)
{
try {
    lcd240x128.DISPLAY_GRAPHIC_XY(redCountHWCheckSel.X,redCountHWCheckSel.Y,word24x24[2],redCountHWCheckSel.height,redCountHWCheckSel.width/8);
    redCountHWCheckTmp=0x02;
    doKeyRIGHTWork();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheckSel::doKey3Work(void)
{
try {
    lcd240x128.DISPLAY_GRAPHIC_XY(redCountHWCheckSel.X,redCountHWCheckSel.Y,word24x24[3],redCountHWCheckSel.height,redCountHWCheckSel.width/8);
    redCountHWCheckTmp=0x03;
    doKeyRIGHTWork();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheckSel::doKey4Work(void)
{
try {
    lcd240x128.DISPLAY_GRAPHIC_XY(redCountHWCheckSel.X,redCountHWCheckSel.Y,word24x24[4],redCountHWCheckSel.height,redCountHWCheckSel.width/8);
    redCountHWCheckTmp=0x04;
    doKeyRIGHTWork();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheckSel::doKey5Work(void)
{
try {
    lcd240x128.DISPLAY_GRAPHIC_XY(redCountHWCheckSel.X,redCountHWCheckSel.Y,word24x24[5],redCountHWCheckSel.height,redCountHWCheckSel.width/8);
    redCountHWCheckTmp=0x05;
    doKeyRIGHTWork();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheckSel::doKey6Work(void)
{
try {
    lcd240x128.DISPLAY_GRAPHIC_XY(redCountHWCheckSel.X,redCountHWCheckSel.Y,word24x24[6],redCountHWCheckSel.height,redCountHWCheckSel.width/8);
    redCountHWCheckTmp = 0x06;
    doKeyRIGHTWork();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheckSel::doKey7Work(void)
{
try {
    lcd240x128.DISPLAY_GRAPHIC_XY(redCountHWCheckSel.X,redCountHWCheckSel.Y,word24x24[7],redCountHWCheckSel.height,redCountHWCheckSel.width/8);
    redCountHWCheckTmp=0x07;
    doKeyRIGHTWork();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheckSel::doKey8Work(void)
{
try {
    lcd240x128.DISPLAY_GRAPHIC_XY(redCountHWCheckSel.X,redCountHWCheckSel.Y,word24x24[8],redCountHWCheckSel.height,redCountHWCheckSel.width/8);
    redCountHWCheckTmp=0x08;
    doKeyRIGHTWork();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheckSel::doKey9Work(void)
{
try {
    lcd240x128.DISPLAY_GRAPHIC_XY(redCountHWCheckSel.X,redCountHWCheckSel.Y,word24x24[9],redCountHWCheckSel.height,redCountHWCheckSel.width/8);
    redCountHWCheckTmp=0x09;
    doKeyRIGHTWork();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheckSel::doKeyAWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheckSel::doKeyBWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheckSel::doKeyCWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheckSel::doKeyDWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheckSel::doKeyEWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheckSel::doKeyFWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheckSel::doKeyF1Work(void)
{
    screenMain.DisplayMain();
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheckSel::doKeyF2Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheckSel::doKeyF3Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheckSel::doKeyF4Work(void)
{
    screenRedCountMenu.DisplayRedCountMenu();
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheckSel::doKeyUPWork(void)
{
    DisplaySetSegtype(redCountHWCheckTmp);
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheckSel::doKeyDOWNWork(void)
{
    DisplaySetSegtype(redCountHWCheckTmp);
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheckSel::doKeyLEFTWork(void)
{
try {
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheckSel::doKeyRIGHTWork(void)
{
try {
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenRedCountHWCheckSel::doKeyEnterWork(void)
{
    int x=redCountHWCheckTmp;
//    if (x>=0 && x<=4) {
     screenRedCountHWCheck.DisplayRedCountHWCheck(x);
//    }
}
//---------------------------------------------------------------------------
