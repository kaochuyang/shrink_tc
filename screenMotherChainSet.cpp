#include "screenMotherChainSet.h"
#include "SCREENMain.h"

#include "screenChainMenu.h"

#include "SMEM.h"
#include "LCD240x128.h"
#include "CTIMER.h"

#include "CSTC.h"

#include <stdio.h>
#include <stdlib.h>

#include "CDataToMessageOK.h"
#include "WRITEJOB.h"

ScreenMotherChainSet screenMotherChainSet;
//---------------------------------------------------------------------------
ScreenMotherChainSet::ScreenMotherChainSet(void)
{
    LoadBitmapFromFile();
    InitDispWord();
    cPosition=0;
}
//---------------------------------------------------------------------------
ScreenMotherChainSet::~ScreenMotherChainSet(void)
{
}
//---------------------------------------------------------------------------
void ScreenMotherChainSet::LoadBitmapFromFile(void)
{
try {
    FILE *bitmap;
    bitmap=fopen("//cct//bitmap//backGround//TC5F//MotherChainSet.bit","rb");
    if (bitmap) {
        fread(chainMotherSetBitmap,3840,1,bitmap);
        fclose(bitmap);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenMotherChainSet::DisplayMotherChainSet(void)
{
try {
    smem.SetcFace(cMOTHERCHAINSET);
    lcd240x128.DISPLAY_GRAPHIC(0,chainMotherSetBitmap,128,30);
    DisplayData();

    cPosition=0;
    setCursor8x16(cMotherChainSetWord[cPosition].X,cMotherChainSetWord[cPosition].Y+16);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenMotherChainSet::InitDispWord(void)
{
    cMotherChainSetWord[0].X = 120;
    cMotherChainSetWord[0].Y = 32;

    cMotherChainSetWord[1].X = 168;
    cMotherChainSetWord[1].Y = 32;

    cMotherChainSetWord[2].X = 120;
    cMotherChainSetWord[2].Y = 48;;

    cMotherChainSetWord[3].X = 168;
    cMotherChainSetWord[3].Y = 48;

    cMotherChainSetWord[4].X = 120;
    cMotherChainSetWord[4].Y = 64;;

    cMotherChainSetWord[5].X = 120;
    cMotherChainSetWord[5].Y = 80;

    for(int i = 0; i < 6; i++) {
      cMotherChainSetWord[i].width = 8;
      cMotherChainSetWord[i].height = 16;
    }

}
//---------------------------------------------------------------------------
void ScreenMotherChainSet::DisplayData(void)
{
try {
    ucMotherChainSet[0] = smem.vGetUCData(TC92_5F31StartSubPhaseId);
    ucMotherChainSet[1] = smem.vGetUCData(TC_MotherChainStartStepId);
    ucMotherChainSet[2] = smem.vGetUCData(TC92_5F31EndSubPhaseId);
    ucMotherChainSet[3] = smem.vGetUCData(TC_MotherChainEndStepId);
    ucMotherChainSet[4] = smem.vGetUCData(TC92_5F31Manual);
    ucMotherChainSet[5] = smem.vGetUCData(TC92_5F31TOD);

    for(int i = 0; i < 6; i++) {
      lcd240x128.DISPLAY_GRAPHIC_XY(cMotherChainSetWord[i].X,cMotherChainSetWord[i].Y,word8x16[ucMotherChainSet[i]],cMotherChainSetWord[i].height,cMotherChainSetWord[i].width/8);
    }

  } catch (...) {}
}

//---------------------------------------------------------------------------
void ScreenMotherChainSet::DoKeyWork(BYTE key)
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
          DoKey6Work();
        break;
        case 0x87:
          DoKey7Work();
        break;
        case 0x88:
          DoKey8Work();
        break;
        case 0x89:
          DoKey9Work();
        break;
        case 0x8A:
        case 0x8B:
        case 0x8C:
        case 0x8D:
        case 0x8E:
        case 0x8F:
        break;
        case 0x90:
          DoKeyF1Work();
        break;
        case 0x91:
        case 0x92:
        break;
        case 0x93:
          DoKeyF4Work();
        break;
        case 0x94:
          doKeyUPWork();
        break;
        case 0x95:
          doKeyDOWNWork();
        break;
        case 0x96:
          DoKeyLEFTWork();
        break;
        case 0x97:
          DoKeyRIGHTWork();
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
void ScreenMotherChainSet::DoKey0Work(void)
{
  if(cPosition == 0 || cPosition == 2 || cPosition == 4 || cPosition == 5) {
    lcd240x128.DISPLAY_GRAPHIC_XY(cMotherChainSetWord[cPosition].X,cMotherChainSetWord[cPosition].Y,word8x16[0],cMotherChainSetWord[cPosition].height,cMotherChainSetWord[cPosition].width/8);
    ucMotherChainSet[cPosition]=0;
    DoKeyRIGHTWork();
  }
}
//---------------------------------------------------------------------------
void ScreenMotherChainSet::DoKey1Work(void)
{
  if(cPosition == 0 || cPosition == 1 || cPosition == 2 || cPosition == 3 || cPosition == 4 || cPosition == 5) {
    lcd240x128.DISPLAY_GRAPHIC_XY(cMotherChainSetWord[cPosition].X,cMotherChainSetWord[cPosition].Y,word8x16[1],cMotherChainSetWord[cPosition].height,cMotherChainSetWord[cPosition].width/8);
    ucMotherChainSet[cPosition]=1;
    DoKeyRIGHTWork();
  }
}
//---------------------------------------------------------------------------
void ScreenMotherChainSet::DoKey2Work(void)
{
  if(cPosition == 0 || cPosition == 1 || cPosition == 2 || cPosition == 3 ) {
    lcd240x128.DISPLAY_GRAPHIC_XY(cMotherChainSetWord[cPosition].X,cMotherChainSetWord[cPosition].Y,word8x16[2],cMotherChainSetWord[cPosition].height,cMotherChainSetWord[cPosition].width/8);
    ucMotherChainSet[cPosition]=2;
    DoKeyRIGHTWork();
  }
}
//---------------------------------------------------------------------------
void ScreenMotherChainSet::DoKey3Work(void)
{
  if(cPosition == 0 || cPosition == 1 || cPosition == 2 || cPosition == 3 ) {
    lcd240x128.DISPLAY_GRAPHIC_XY(cMotherChainSetWord[cPosition].X,cMotherChainSetWord[cPosition].Y,word8x16[3],cMotherChainSetWord[cPosition].height,cMotherChainSetWord[cPosition].width/8);
    ucMotherChainSet[cPosition]=3;
    DoKeyRIGHTWork();
  }
}
//---------------------------------------------------------------------------
void ScreenMotherChainSet::DoKey4Work(void)
{
  if(cPosition == 0 || cPosition == 1 || cPosition == 2 || cPosition == 3 ) {
    lcd240x128.DISPLAY_GRAPHIC_XY(cMotherChainSetWord[cPosition].X,cMotherChainSetWord[cPosition].Y,word8x16[4],cMotherChainSetWord[cPosition].height,cMotherChainSetWord[cPosition].width/8);
    ucMotherChainSet[cPosition]=4;
    DoKeyRIGHTWork();
  }
}
//---------------------------------------------------------------------------
void ScreenMotherChainSet::DoKey5Work(void)
{
  if(cPosition == 0 || cPosition == 1 || cPosition == 2 || cPosition == 3 ) {
    lcd240x128.DISPLAY_GRAPHIC_XY(cMotherChainSetWord[cPosition].X,cMotherChainSetWord[cPosition].Y,word8x16[5],cMotherChainSetWord[cPosition].height,cMotherChainSetWord[cPosition].width/8);
    ucMotherChainSet[cPosition]=5;
    DoKeyRIGHTWork();
  }
}
//---------------------------------------------------------------------------
void ScreenMotherChainSet::DoKey6Work(void)
{
  if(cPosition == 0 || cPosition == 2) {
    lcd240x128.DISPLAY_GRAPHIC_XY(cMotherChainSetWord[cPosition].X,cMotherChainSetWord[cPosition].Y,word8x16[6],cMotherChainSetWord[cPosition].height,cMotherChainSetWord[cPosition].width/8);
    ucMotherChainSet[cPosition]=6;
    DoKeyRIGHTWork();
  }
}
//---------------------------------------------------------------------------
void ScreenMotherChainSet::DoKey7Work(void)
{
  if(cPosition == 0 || cPosition == 2) {
    lcd240x128.DISPLAY_GRAPHIC_XY(cMotherChainSetWord[cPosition].X,cMotherChainSetWord[cPosition].Y,word8x16[7],cMotherChainSetWord[cPosition].height,cMotherChainSetWord[cPosition].width/8);
    ucMotherChainSet[cPosition]=7;
    DoKeyRIGHTWork();
  }
}
//---------------------------------------------------------------------------
void ScreenMotherChainSet::DoKey8Work(void)
{
  if(cPosition == 0 || cPosition == 2) {
    lcd240x128.DISPLAY_GRAPHIC_XY(cMotherChainSetWord[cPosition].X,cMotherChainSetWord[cPosition].Y,word8x16[8],cMotherChainSetWord[cPosition].height,cMotherChainSetWord[cPosition].width/8);
    ucMotherChainSet[cPosition]=8;
    DoKeyRIGHTWork();
  }
}
//---------------------------------------------------------------------------
void ScreenMotherChainSet::DoKey9Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenMotherChainSet::DoKeyF1Work(void)
{
    screenMain.DisplayMain();
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void ScreenMotherChainSet::DoKeyF4Work(void)
{
    screenChainMenu.DisplayChainMenu();
}
//---------------------------------------------------------------------------
void ScreenMotherChainSet::doKeyUPWork(void)
{
try {
  clearCursor8x16(cMotherChainSetWord[cPosition].X,cMotherChainSetWord[cPosition].Y+16);
  if (cPosition==0) { }
  else { cPosition--; }
  setCursor8x16(cMotherChainSetWord[cPosition].X,cMotherChainSetWord[cPosition].Y+16);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenMotherChainSet::doKeyDOWNWork(void)
{
try {
  clearCursor8x16(cMotherChainSetWord[cPosition].X,cMotherChainSetWord[cPosition].Y+16);
  if (cPosition>=5) { }
  else { cPosition++; }
  setCursor8x16(cMotherChainSetWord[cPosition].X,cMotherChainSetWord[cPosition].Y+16);
  } catch (...) {}
}

//---------------------------------------------------------------------------
void ScreenMotherChainSet::DoKeyLEFTWork(void)
{
        clearCursor8x16(cMotherChainSetWord[cPosition].X,cMotherChainSetWord[cPosition].Y+16);
        if (cPosition==0) { }
        else { cPosition--; }
        setCursor8x16(cMotherChainSetWord[cPosition].X,cMotherChainSetWord[cPosition].Y+16);
}
//---------------------------------------------------------------------------
void ScreenMotherChainSet::DoKeyRIGHTWork(void)
{
        clearCursor8x16(cMotherChainSetWord[cPosition].X,cMotherChainSetWord[cPosition].Y+16);
        if (cPosition>=5) { }
        else { cPosition++; }
        setCursor8x16(cMotherChainSetWord[cPosition].X,cMotherChainSetWord[cPosition].Y+16);
}
//---------------------------------------------------------------------------
void ScreenMotherChainSet::DoKeyEnterWork(void)
{
try {
  smem.vSetUCData(TC92_5F31StartSubPhaseId, ucMotherChainSet[0]);
  smem.vSetUCData(TC_MotherChainStartStepId, ucMotherChainSet[1]);
  smem.vSetUCData(TC92_5F31EndSubPhaseId, ucMotherChainSet[2]);
  smem.vSetUCData(TC_MotherChainEndStepId, ucMotherChainSet[3]);
  smem.vSetUCData(TC92_5F31Manual, ucMotherChainSet[4]);
  smem.vSetUCData(TC92_5F31TOD, ucMotherChainSet[5]);

  screenChainMenu.DisplayChainMenu();

  } catch (...) {}
}
//---------------------------------------------------------------------------
