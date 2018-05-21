#include "screenChildChainSet.h"
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

ScreenChildChainSet screenChildChainSet;
//---------------------------------------------------------------------------
ScreenChildChainSet::ScreenChildChainSet(void)
{
    LoadBitmapFromFile();
    InitDispWord();
    cPosition=0;
    cSelect = 0;
}
//---------------------------------------------------------------------------
ScreenChildChainSet::~ScreenChildChainSet(void)
{
}
//---------------------------------------------------------------------------
void ScreenChildChainSet::LoadBitmapFromFile(void)
{
try {
    FILE *bitmap;
    bitmap=fopen("//cct//bitmap//backGround//TC5F//ChildChainSet.bit","rb");
    if (bitmap) {
        fread(chainChildSetBitmap,3840,1,bitmap);
        fclose(bitmap);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenChildChainSet::DisplayChildChainSet(void)
{
try {
    smem.SetcFace(cCHILDCHAINSET);
    lcd240x128.DISPLAY_GRAPHIC(0,chainChildSetBitmap,128,30);
    InitDispWord();
    DisplayData();

    cPosition=0;
    cSelect=0;
    setCursor8x16(cChainLockPhaseSetWord[cPosition].X,cChainLockPhaseSetWord[cPosition].Y+16);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenChildChainSet::InitDispWord(void)
{
    cChainLockPhaseSetWord[0].X = 120;
    cChainLockPhaseSetWord[0].Y = 32;

    cChainLockPhaseSetWord[1].X = 120;
    cChainLockPhaseSetWord[1].Y = 48;

    for(int i = 0; i < 2; i++) {
      cChainLockPhaseSetWord[i].width = 8;
      cChainLockPhaseSetWord[i].height = 16;
    }

    cChainStartOffsetSignedWord.X =72;
    cChainStartOffsetSignedWord.Y =64;
    cChainStartOffsetSignedWord.width = 8;
    cChainStartOffsetSignedWord.height = 16;

    cChainEndOffsetSignedWord.X =72;
    cChainEndOffsetSignedWord.Y =80;
    cChainEndOffsetSignedWord.width = 8;
    cChainEndOffsetSignedWord.height = 16;

    for(int i = 0; i < 3; i++) {
      cChainStartOffsetWord[i].X = 208 + i*8;
      cChainStartOffsetWord[i].Y = 64;
      cChainStartOffsetWord[i].width = 8;
      cChainStartOffsetWord[i].height = 16;

      cChainEndOffsetWord[i].X = 208 + i*8;
      cChainEndOffsetWord[i].Y = 80;
      cChainEndOffsetWord[i].width = 8;
      cChainEndOffsetWord[i].height = 16;
    }
}
//---------------------------------------------------------------------------
void ScreenChildChainSet::DisplayData(void)
{
try {
    int iTmp;
    ucChainLockPhaseSet[0] = smem.vGetUCData(TC92_5F32StartSubPhaseId);
    ucChainLockPhaseSet[1] = smem.vGetUCData(TC92_5F32EndSubPhaseId);

    for(int i = 0; i < 2; i++) {
      lcd240x128.DISPLAY_GRAPHIC_XY(cChainLockPhaseSetWord[i].X,cChainLockPhaseSetWord[i].Y,word8x16[ucChainLockPhaseSet[i]],cChainLockPhaseSetWord[i].height,cChainLockPhaseSetWord[i].width/8);
    }

    iTmp = smem.vGetChainOffset(1, 0);
    if(iTmp < 0) { ucChainStartOffsetSigned = 0; }
    else { ucChainStartOffsetSigned = 1; }
    lcd240x128.DISPLAY_GRAPHIC_XY(cChainStartOffsetSignedWord.X,cChainStartOffsetSignedWord.Y,word8x16[ucChainStartOffsetSigned],cChainStartOffsetSignedWord.height,cChainStartOffsetSignedWord.width/8);

    iTmp = abs(iTmp);
    ucChainStartOffset[0] = iTmp/100;
    ucChainStartOffset[1] = (iTmp/10)%10;
    ucChainStartOffset[2] = iTmp%10;;
    for(int i = 0; i < 3; i++) {
      lcd240x128.DISPLAY_GRAPHIC_XY(cChainStartOffsetWord[i].X,cChainStartOffsetWord[i].Y,word8x16[ucChainStartOffset[i]],cChainStartOffsetWord[i].height,cChainStartOffsetWord[i].width/8);
    }

    iTmp = smem.vGetChainOffset(2, 0);
    if(iTmp < 0) { ucChainEndOffsetSigned = 0; }
    else { ucChainEndOffsetSigned = 1; }
    lcd240x128.DISPLAY_GRAPHIC_XY(cChainEndOffsetSignedWord.X,cChainEndOffsetSignedWord.Y,word8x16[ucChainEndOffsetSigned],cChainEndOffsetSignedWord.height,cChainEndOffsetSignedWord.width/8);

    iTmp = abs(iTmp);
    ucChainEndOffset[0] = iTmp/100;
    ucChainEndOffset[1] = (iTmp/10)%10;
    ucChainEndOffset[2] = iTmp%10;;
    for(int i = 0; i < 3; i++) {
      lcd240x128.DISPLAY_GRAPHIC_XY(cChainEndOffsetWord[i].X,cChainEndOffsetWord[i].Y,word8x16[ucChainEndOffset[i]],cChainEndOffsetWord[i].height,cChainEndOffsetWord[i].width/8);
    }

  } catch (...) {}
}

//---------------------------------------------------------------------------
void ScreenChildChainSet::DoKeyWork(BYTE key)
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
void ScreenChildChainSet::DoKey0Work(void)
{
  if(cSelect == 0) {
    lcd240x128.DISPLAY_GRAPHIC_XY(cChainLockPhaseSetWord[cPosition].X,cChainLockPhaseSetWord[cPosition].Y,word8x16[0],cChainLockPhaseSetWord[cPosition].height,cChainLockPhaseSetWord[cPosition].width/8);
    ucChainLockPhaseSet[cPosition]=0;
    DoKeyRIGHTWork();
  } else if (cSelect == 1) {
    lcd240x128.DISPLAY_GRAPHIC_XY(cChainStartOffsetSignedWord.X,cChainStartOffsetSignedWord.Y,word8x16[0],cChainStartOffsetSignedWord.height,cChainStartOffsetSignedWord.width/8);
    ucChainStartOffsetSigned = 0;
    DoKeyRIGHTWork();
  } else if (cSelect == 2) {
    lcd240x128.DISPLAY_GRAPHIC_XY(cChainStartOffsetWord[cPosition].X,cChainStartOffsetWord[cPosition].Y,word8x16[0],cChainStartOffsetWord[cPosition].height,cChainStartOffsetWord[cPosition].width/8);
    ucChainStartOffset[cPosition]=0;
    DoKeyRIGHTWork();
  } else if (cSelect == 3) {
    lcd240x128.DISPLAY_GRAPHIC_XY(cChainEndOffsetSignedWord.X,cChainEndOffsetSignedWord.Y,word8x16[0],cChainEndOffsetSignedWord.height,cChainEndOffsetSignedWord.width/8);
    ucChainEndOffsetSigned = 0;
    DoKeyRIGHTWork();
  } else if (cSelect == 4) {
    lcd240x128.DISPLAY_GRAPHIC_XY(cChainEndOffsetWord[cPosition].X,cChainEndOffsetWord[cPosition].Y,word8x16[0],cChainEndOffsetWord[cPosition].height,cChainEndOffsetWord[cPosition].width/8);
    ucChainEndOffset[cPosition] = 0;
    DoKeyRIGHTWork();
  }
}
//---------------------------------------------------------------------------
void ScreenChildChainSet::DoKey1Work(void)
{
  if(cSelect == 0) {
    lcd240x128.DISPLAY_GRAPHIC_XY(cChainLockPhaseSetWord[cPosition].X,cChainLockPhaseSetWord[cPosition].Y,word8x16[1],cChainLockPhaseSetWord[cPosition].height,cChainLockPhaseSetWord[cPosition].width/8);
    ucChainLockPhaseSet[cPosition]=1;
    DoKeyRIGHTWork();
  } else if (cSelect == 1) {
    lcd240x128.DISPLAY_GRAPHIC_XY(cChainStartOffsetSignedWord.X,cChainStartOffsetSignedWord.Y,word8x16[1],cChainStartOffsetSignedWord.height,cChainStartOffsetSignedWord.width/8);
    ucChainStartOffsetSigned = 1;
    DoKeyRIGHTWork();
  } else if (cSelect == 2) {
    lcd240x128.DISPLAY_GRAPHIC_XY(cChainStartOffsetWord[cPosition].X,cChainStartOffsetWord[cPosition].Y,word8x16[1],cChainStartOffsetWord[cPosition].height,cChainStartOffsetWord[cPosition].width/8);
    ucChainStartOffset[cPosition]=1;
    DoKeyRIGHTWork();
  } else if (cSelect == 3) {
    lcd240x128.DISPLAY_GRAPHIC_XY(cChainEndOffsetSignedWord.X,cChainEndOffsetSignedWord.Y,word8x16[1],cChainEndOffsetSignedWord.height,cChainEndOffsetSignedWord.width/8);
    ucChainEndOffsetSigned = 1;
    DoKeyRIGHTWork();
  } else if (cSelect == 4) {
    lcd240x128.DISPLAY_GRAPHIC_XY(cChainEndOffsetWord[cPosition].X,cChainEndOffsetWord[cPosition].Y,word8x16[1],cChainEndOffsetWord[cPosition].height,cChainEndOffsetWord[cPosition].width/8);
    ucChainEndOffset[cPosition] = 1;
    DoKeyRIGHTWork();
  }

}
//---------------------------------------------------------------------------
void ScreenChildChainSet::DoKey2Work(void)
{
  if(cSelect == 0) {
    lcd240x128.DISPLAY_GRAPHIC_XY(cChainLockPhaseSetWord[cPosition].X,cChainLockPhaseSetWord[cPosition].Y,word8x16[2],cChainLockPhaseSetWord[cPosition].height,cChainLockPhaseSetWord[cPosition].width/8);
    ucChainLockPhaseSet[cPosition]=2;
    DoKeyRIGHTWork();
  } else if (cSelect == 2) {
    lcd240x128.DISPLAY_GRAPHIC_XY(cChainStartOffsetWord[cPosition].X,cChainStartOffsetWord[cPosition].Y,word8x16[2],cChainStartOffsetWord[cPosition].height,cChainStartOffsetWord[cPosition].width/8);
    ucChainStartOffset[cPosition]=2;
    DoKeyRIGHTWork();
  } else if (cSelect == 4) {
    lcd240x128.DISPLAY_GRAPHIC_XY(cChainEndOffsetWord[cPosition].X,cChainEndOffsetWord[cPosition].Y,word8x16[2],cChainEndOffsetWord[cPosition].height,cChainEndOffsetWord[cPosition].width/8);
    ucChainEndOffset[cPosition] = 2;
    DoKeyRIGHTWork();
  }
}
//---------------------------------------------------------------------------
void ScreenChildChainSet::DoKey3Work(void)
{
  if(cSelect == 0) {
    lcd240x128.DISPLAY_GRAPHIC_XY(cChainLockPhaseSetWord[cPosition].X,cChainLockPhaseSetWord[cPosition].Y,word8x16[3],cChainLockPhaseSetWord[cPosition].height,cChainLockPhaseSetWord[cPosition].width/8);
    ucChainLockPhaseSet[cPosition]=3;
    DoKeyRIGHTWork();
  } else if (cSelect == 2) {
    lcd240x128.DISPLAY_GRAPHIC_XY(cChainStartOffsetWord[cPosition].X,cChainStartOffsetWord[cPosition].Y,word8x16[3],cChainStartOffsetWord[cPosition].height,cChainStartOffsetWord[cPosition].width/8);
    ucChainStartOffset[cPosition]=3;
    DoKeyRIGHTWork();
  } else if (cSelect == 4) {
    lcd240x128.DISPLAY_GRAPHIC_XY(cChainEndOffsetWord[cPosition].X,cChainEndOffsetWord[cPosition].Y,word8x16[3],cChainEndOffsetWord[cPosition].height,cChainEndOffsetWord[cPosition].width/8);
    ucChainEndOffset[cPosition] = 3;
    DoKeyRIGHTWork();
  }
}
//---------------------------------------------------------------------------
void ScreenChildChainSet::DoKey4Work(void)
{
  if(cSelect == 0) {
    lcd240x128.DISPLAY_GRAPHIC_XY(cChainLockPhaseSetWord[cPosition].X,cChainLockPhaseSetWord[cPosition].Y,word8x16[4],cChainLockPhaseSetWord[cPosition].height,cChainLockPhaseSetWord[cPosition].width/8);
    ucChainLockPhaseSet[cPosition]=4;
    DoKeyRIGHTWork();
  } else if (cSelect == 2) {
    lcd240x128.DISPLAY_GRAPHIC_XY(cChainStartOffsetWord[cPosition].X,cChainStartOffsetWord[cPosition].Y,word8x16[4],cChainStartOffsetWord[cPosition].height,cChainStartOffsetWord[cPosition].width/8);
    ucChainStartOffset[cPosition]=4;
    DoKeyRIGHTWork();
  } else if (cSelect == 4) {
    lcd240x128.DISPLAY_GRAPHIC_XY(cChainEndOffsetWord[cPosition].X,cChainEndOffsetWord[cPosition].Y,word8x16[4],cChainEndOffsetWord[cPosition].height,cChainEndOffsetWord[cPosition].width/8);
    ucChainEndOffset[cPosition] = 4;
    DoKeyRIGHTWork();
  }
}
//---------------------------------------------------------------------------
void ScreenChildChainSet::DoKey5Work(void)
{
  if(cSelect == 0) {
    lcd240x128.DISPLAY_GRAPHIC_XY(cChainLockPhaseSetWord[cPosition].X,cChainLockPhaseSetWord[cPosition].Y,word8x16[5],cChainLockPhaseSetWord[cPosition].height,cChainLockPhaseSetWord[cPosition].width/8);
    ucChainLockPhaseSet[cPosition]=5;
    DoKeyRIGHTWork();
  } else if (cSelect == 2) {
    lcd240x128.DISPLAY_GRAPHIC_XY(cChainStartOffsetWord[cPosition].X,cChainStartOffsetWord[cPosition].Y,word8x16[5],cChainStartOffsetWord[cPosition].height,cChainStartOffsetWord[cPosition].width/8);
    ucChainStartOffset[cPosition]=5;
    DoKeyRIGHTWork();
  } else if (cSelect == 4) {
    lcd240x128.DISPLAY_GRAPHIC_XY(cChainEndOffsetWord[cPosition].X,cChainEndOffsetWord[cPosition].Y,word8x16[5],cChainEndOffsetWord[cPosition].height,cChainEndOffsetWord[cPosition].width/8);
    ucChainEndOffset[cPosition] = 5;
    DoKeyRIGHTWork();
  }
}
//---------------------------------------------------------------------------
void ScreenChildChainSet::DoKey6Work(void)
{
  if(cSelect == 0) {
    lcd240x128.DISPLAY_GRAPHIC_XY(cChainLockPhaseSetWord[cPosition].X,cChainLockPhaseSetWord[cPosition].Y,word8x16[6],cChainLockPhaseSetWord[cPosition].height,cChainLockPhaseSetWord[cPosition].width/8);
    ucChainLockPhaseSet[cPosition]=6;
    DoKeyRIGHTWork();
  } else if (cSelect == 2) {
    lcd240x128.DISPLAY_GRAPHIC_XY(cChainStartOffsetWord[cPosition].X,cChainStartOffsetWord[cPosition].Y,word8x16[6],cChainStartOffsetWord[cPosition].height,cChainStartOffsetWord[cPosition].width/8);
    ucChainStartOffset[cPosition]=6;
    DoKeyRIGHTWork();
  } else if (cSelect == 4) {
    lcd240x128.DISPLAY_GRAPHIC_XY(cChainEndOffsetWord[cPosition].X,cChainEndOffsetWord[cPosition].Y,word8x16[6],cChainEndOffsetWord[cPosition].height,cChainEndOffsetWord[cPosition].width/8);
    ucChainEndOffset[cPosition] = 6;
    DoKeyRIGHTWork();
  }
}
//---------------------------------------------------------------------------
void ScreenChildChainSet::DoKey7Work(void)
{
  if(cSelect == 0) {
    lcd240x128.DISPLAY_GRAPHIC_XY(cChainLockPhaseSetWord[cPosition].X,cChainLockPhaseSetWord[cPosition].Y,word8x16[7],cChainLockPhaseSetWord[cPosition].height,cChainLockPhaseSetWord[cPosition].width/8);
    ucChainLockPhaseSet[cPosition]=7;
    DoKeyRIGHTWork();
  } else if (cSelect == 2) {
    lcd240x128.DISPLAY_GRAPHIC_XY(cChainStartOffsetWord[cPosition].X,cChainStartOffsetWord[cPosition].Y,word8x16[7],cChainStartOffsetWord[cPosition].height,cChainStartOffsetWord[cPosition].width/8);
    ucChainStartOffset[cPosition]=7;
    DoKeyRIGHTWork();
  } else if (cSelect == 4) {
    lcd240x128.DISPLAY_GRAPHIC_XY(cChainEndOffsetWord[cPosition].X,cChainEndOffsetWord[cPosition].Y,word8x16[7],cChainEndOffsetWord[cPosition].height,cChainEndOffsetWord[cPosition].width/8);
    ucChainEndOffset[cPosition] = 7;
    DoKeyRIGHTWork();
  }
}
//---------------------------------------------------------------------------
void ScreenChildChainSet::DoKey8Work(void)
{
  if(cSelect == 0) {
    lcd240x128.DISPLAY_GRAPHIC_XY(cChainLockPhaseSetWord[cPosition].X,cChainLockPhaseSetWord[cPosition].Y,word8x16[8],cChainLockPhaseSetWord[cPosition].height,cChainLockPhaseSetWord[cPosition].width/8);
    ucChainLockPhaseSet[cPosition]=8;
    DoKeyRIGHTWork();
  } else if (cSelect == 2) {
    lcd240x128.DISPLAY_GRAPHIC_XY(cChainStartOffsetWord[cPosition].X,cChainStartOffsetWord[cPosition].Y,word8x16[8],cChainStartOffsetWord[cPosition].height,cChainStartOffsetWord[cPosition].width/8);
    ucChainStartOffset[cPosition]=8;
    DoKeyRIGHTWork();
  } else if (cSelect == 4) {
    lcd240x128.DISPLAY_GRAPHIC_XY(cChainEndOffsetWord[cPosition].X,cChainEndOffsetWord[cPosition].Y,word8x16[8],cChainEndOffsetWord[cPosition].height,cChainEndOffsetWord[cPosition].width/8);
    ucChainEndOffset[cPosition] = 8;
    DoKeyRIGHTWork();
  }
}
//---------------------------------------------------------------------------
void ScreenChildChainSet::DoKey9Work(void)
{
  if (cSelect == 2) {
    lcd240x128.DISPLAY_GRAPHIC_XY(cChainStartOffsetWord[cPosition].X,cChainStartOffsetWord[cPosition].Y,word8x16[9],cChainStartOffsetWord[cPosition].height,cChainStartOffsetWord[cPosition].width/8);
    ucChainStartOffset[cPosition]=9;
    DoKeyRIGHTWork();
  } else if (cSelect == 4) {
    lcd240x128.DISPLAY_GRAPHIC_XY(cChainEndOffsetWord[cPosition].X,cChainEndOffsetWord[cPosition].Y,word8x16[9],cChainEndOffsetWord[cPosition].height,cChainEndOffsetWord[cPosition].width/8);
    ucChainEndOffset[cPosition] = 9;
    DoKeyRIGHTWork();
  }
}
//---------------------------------------------------------------------------
void ScreenChildChainSet::DoKeyF1Work(void)
{
    screenMain.DisplayMain();
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void ScreenChildChainSet::DoKeyF4Work(void)
{
    screenChainMenu.DisplayChainMenu();
}
//---------------------------------------------------------------------------
void ScreenChildChainSet::doKeyUPWork(void)
{
try {
  if(cSelect == 0) {
    clearCursor8x16(cChainLockPhaseSetWord[cPosition].X,cChainLockPhaseSetWord[cPosition].Y+16);
    if(cPosition > 0) cPosition--;
    setCursor8x16(cChainLockPhaseSetWord[cPosition].X,cChainLockPhaseSetWord[cPosition].Y+16);
  } else if (cSelect == 1) {
    clearCursor8x16(cChainStartOffsetSignedWord.X,cChainStartOffsetSignedWord.Y+16);
    cPosition = 1;
    cSelect = 0;
    setCursor8x16(cChainLockPhaseSetWord[cPosition].X,cChainLockPhaseSetWord[cPosition].Y+16);
  } else if (cSelect == 2) {
    clearCursor8x16(cChainStartOffsetWord[cPosition].X,cChainStartOffsetWord[cPosition].Y+16);
    if(cPosition > 0) { cPosition--; setCursor8x16(cChainStartOffsetWord[cPosition].X,cChainStartOffsetWord[cPosition].Y+16); }
    else {
      cPosition = 0;
      cSelect = 1;
      setCursor8x16(cChainStartOffsetSignedWord.X,cChainStartOffsetSignedWord.Y+16);
    }
  } else if (cSelect == 3) {
    clearCursor8x16(cChainEndOffsetSignedWord.X,cChainEndOffsetSignedWord.Y+16);
    cPosition = 0;
    cSelect = 1;
    setCursor8x16(cChainStartOffsetSignedWord.X,cChainStartOffsetSignedWord.Y+16);
  } else if (cSelect == 4) {
    clearCursor8x16(cChainEndOffsetWord[cPosition].X,cChainEndOffsetWord[cPosition].Y+16);
    if(cPosition > 0) { cPosition--; setCursor8x16(cChainEndOffsetWord[cPosition].X,cChainEndOffsetWord[cPosition].Y+16); }
    else {
      cSelect = 3;
      setCursor8x16(cChainStartOffsetWord[cPosition].X,cChainStartOffsetWord[cPosition].Y+16);
    }
  }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenChildChainSet::doKeyDOWNWork(void)
{
try {
  if(cSelect == 0) {
    clearCursor8x16(cChainLockPhaseSetWord[cPosition].X,cChainLockPhaseSetWord[cPosition].Y+16);
    if(cPosition < 1) { cPosition++; setCursor8x16(cChainLockPhaseSetWord[cPosition].X,cChainLockPhaseSetWord[cPosition].Y+16); }
    else { cPosition = 0; cSelect = 1; setCursor8x16(cChainStartOffsetSignedWord.X,cChainStartOffsetSignedWord.Y+16); }
  } else if (cSelect == 1) {
    clearCursor8x16(cChainStartOffsetSignedWord.X,cChainStartOffsetSignedWord.Y+16);
    cSelect = 3;
    setCursor8x16(cChainEndOffsetSignedWord.X,cChainEndOffsetSignedWord.Y+16);
  } else if (cSelect == 2) {
    clearCursor8x16(cChainStartOffsetWord[cPosition].X,cChainStartOffsetWord[cPosition].Y+16);
    cSelect = 4;
    setCursor8x16(cChainEndOffsetWord[cPosition].X,cChainEndOffsetWord[cPosition].Y+16);
    }
 }
 catch (...) {}
}

//---------------------------------------------------------------------------
void ScreenChildChainSet::DoKeyLEFTWork(void)
{
  if(cSelect == 0) {
    clearCursor8x16(cChainLockPhaseSetWord[cPosition].X,cChainLockPhaseSetWord[cPosition].Y+16);
    if(cPosition > 0) cPosition--;
    setCursor8x16(cChainLockPhaseSetWord[cPosition].X,cChainLockPhaseSetWord[cPosition].Y+16);
  } else if (cSelect == 1) {
    clearCursor8x16(cChainStartOffsetSignedWord.X,cChainStartOffsetSignedWord.Y+16);
    cPosition = 1;
    cSelect = 0;
    setCursor8x16(cChainLockPhaseSetWord[cPosition].X,cChainLockPhaseSetWord[cPosition].Y+16);
  } else if (cSelect == 2) {
    clearCursor8x16(cChainStartOffsetWord[cPosition].X,cChainStartOffsetWord[cPosition].Y+16);
    if(cPosition > 0) { cPosition--; setCursor8x16(cChainStartOffsetWord[cPosition].X,cChainStartOffsetWord[cPosition].Y+16); }
    else {
      cPosition = 0;
      cSelect = 1;
      setCursor8x16(cChainStartOffsetSignedWord.X,cChainStartOffsetSignedWord.Y+16);
    }
  } else if (cSelect == 3) {
    clearCursor8x16(cChainEndOffsetSignedWord.X,cChainEndOffsetSignedWord.Y+16);
    cPosition = 2;
    cSelect = 2;
    setCursor8x16(cChainStartOffsetWord[cPosition].X,cChainStartOffsetWord[cPosition].Y+16);
  } else if (cSelect == 4) {
    clearCursor8x16(cChainEndOffsetWord[cPosition].X,cChainEndOffsetWord[cPosition].Y+16);
    if(cPosition > 0) { cPosition--; setCursor8x16(cChainEndOffsetWord[cPosition].X,cChainEndOffsetWord[cPosition].Y+16); }
    else {
      cSelect = 3;
      cPosition = 0;
      setCursor8x16(cChainEndOffsetSignedWord.X,cChainEndOffsetSignedWord.Y+16);
    }
  }
}
//---------------------------------------------------------------------------
void ScreenChildChainSet::DoKeyRIGHTWork(void)
{
  if(cSelect == 0) {
    clearCursor8x16(cChainLockPhaseSetWord[cPosition].X,cChainLockPhaseSetWord[cPosition].Y+16);
    if(cPosition < 1) { cPosition++; setCursor8x16(cChainLockPhaseSetWord[cPosition].X,cChainLockPhaseSetWord[cPosition].Y+16); }
    else { cPosition = 0; cSelect = 1; setCursor8x16(cChainStartOffsetSignedWord.X,cChainStartOffsetSignedWord.Y+16); }
  } else if (cSelect == 1) {
    clearCursor8x16(cChainStartOffsetSignedWord.X,cChainStartOffsetSignedWord.Y+16);
    cSelect = 2;
    cPosition = 0;
    setCursor8x16(cChainStartOffsetWord[cPosition].X,cChainStartOffsetWord[cPosition].Y+16);
  } else if (cSelect == 2) {
    clearCursor8x16(cChainStartOffsetWord[cPosition].X,cChainStartOffsetWord[cPosition].Y+16);
    if(cPosition < 2) { cPosition++; setCursor8x16(cChainStartOffsetWord[cPosition].X,cChainStartOffsetWord[cPosition].Y+16); }
    else { cSelect = 3; cPosition = 0; setCursor8x16(cChainEndOffsetSignedWord.X,cChainEndOffsetSignedWord.Y+16); }
  } else if (cSelect == 3) {
    clearCursor8x16(cChainEndOffsetSignedWord.X,cChainEndOffsetSignedWord.Y+16);
    cSelect = 4;
    cPosition = 0;
    setCursor8x16(cChainEndOffsetWord[cPosition].X,cChainEndOffsetWord[cPosition].Y+16);
  } else if (cSelect == 4) {
    clearCursor8x16(cChainEndOffsetWord[cPosition].X,cChainEndOffsetWord[cPosition].Y+16);
    if (cPosition < 2) { cPosition++; }
    setCursor8x16(cChainEndOffsetWord[cPosition].X,cChainEndOffsetWord[cPosition].Y+16);
  }
}
//---------------------------------------------------------------------------
void ScreenChildChainSet::DoKeyEnterWork(void)
{
try {
  int iTmp;

  smem.vSetUCData(TC92_5F32StartSubPhaseId, ucChainLockPhaseSet[0]);
  smem.vSetUCData(TC92_5F32EndSubPhaseId, ucChainLockPhaseSet[1]);

  iTmp = 0;
  iTmp =  ucChainStartOffset[0] * 100;
  iTmp += ucChainStartOffset[1] * 10;
  iTmp += ucChainStartOffset[2];
  if(ucChainStartOffsetSigned == 0) {
    iTmp = 0 - iTmp;
  }
  smem.vSetChainOffset(1, 0, iTmp);

  iTmp = 0;
  iTmp =  ucChainEndOffset[0] * 100;
  iTmp += ucChainEndOffset[1] * 10;
  iTmp += ucChainEndOffset[2];
  if(ucChainEndOffsetSigned == 0) {
    iTmp = 0 - iTmp;
  }
  smem.vSetChainOffset(2, 0, iTmp);

  screenChainMenu.DisplayChainMenu();

  } catch (...) {}
}
//---------------------------------------------------------------------------
