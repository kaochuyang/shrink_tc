#include "screenPassModeSet.h"

#include "LCD240x128.h"
#include "SCREENMain.h"

#include <stdio.h>
#include "SMEM.h"
#include <string.h>

//---------------------------------------------------------------------------
ScreenPassModeSet screenPassModeSet;
//---------------------------------------------------------------------------
ScreenPassModeSet::ScreenPassModeSet(void)
{
    loadBitmapFromFile();
    initDispWord();
    cPosition=0;
}
//---------------------------------------------------------------------------
ScreenPassModeSet::~ScreenPassModeSet(void)
{
}
//---------------------------------------------------------------------------
void ScreenPassModeSet::loadBitmapFromFile(void)
{
try {
    FILE *bitmap;
    bitmap=fopen("//cct//bitmap//backGround//passModeSet.bit","rb");
    if (bitmap) {
        fread(passModeSetBitmap,3840,1,bitmap);
        fclose(bitmap);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenPassModeSet::DisplayPassModeSet(void)
{
try {
    smem.SetcFace(cPassModeSet);
    lcd240x128.DISPLAY_GRAPHIC(0,passModeSetBitmap,128,30);

    cPosition=0;
    cSelect = 0;

    vInitUCTMPData();

    vDataToTmpArray();

    vDisplayPassModeSwitch();
    vDisplayPassModeServer();

    setCursor8x16(cPassModeSwitch.X,cPassModeSwitch.Y+16);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenPassModeSet::initDispWord(void)
{
try {
    cPassModeSwitch.X = 88;
    cPassModeSwitch.Y = 32;
    cPassModeSwitch.width = 8;
    cPassModeSwitch.height = 16;

  for (int i = 0; i < 4; i++) {
    cPassServerLCN[i].X = 136 + i*8;
    cPassServerLCN[i].Y = 56;
    cPassServerLCN[i].width = 8;
    cPassServerLCN[i].height = 16;
  }

} catch (...) {}
}


//---------------------------------------------------------------------------
void ScreenPassModeSet::doKeyWork(BYTE key)
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
void ScreenPassModeSet::doKey0Work(void)
{
try {

    if (cSelect == 0) {
      lcd240x128.DISPLAY_GRAPHIC_XY(cPassModeSwitch.X,cPassModeSwitch.Y,word8x16[0],cPassModeSwitch.height,cPassModeSwitch.width/8);
      ucTMP_PassModeSwitch = 0x0;
    }
    else if (cSelect == 1) {
      lcd240x128.DISPLAY_GRAPHIC_XY(cPassServerLCN[cPosition].X,cPassServerLCN[cPosition].Y,word8x16[0],cPassServerLCN[cPosition].height,cPassServerLCN[cPosition].width/8);
      ucTMP_PassServerLCN[cPosition] = 0x0;
    }

    doKeyRIGHTWork();

  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenPassModeSet::doKey1Work(void)
{
try {

  if (cSelect == 0) {
    lcd240x128.DISPLAY_GRAPHIC_XY(cPassModeSwitch.X,cPassModeSwitch.Y,word8x16[1],cPassModeSwitch.height,cPassModeSwitch.width/8);
    ucTMP_PassModeSwitch = 0x1;
  }
  else if (cSelect == 1) {
    lcd240x128.DISPLAY_GRAPHIC_XY(cPassServerLCN[cPosition].X,cPassServerLCN[cPosition].Y,word8x16[1],cPassServerLCN[cPosition].height,cPassServerLCN[cPosition].width/8);
    ucTMP_PassServerLCN[cPosition] = 0x1;
  }

  doKeyRIGHTWork();

  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenPassModeSet::doKey2Work(void)
{
try {
  if (cSelect == 0) {
    lcd240x128.DISPLAY_GRAPHIC_XY(cPassModeSwitch.X,cPassModeSwitch.Y,word8x16[2],cPassModeSwitch.height,cPassModeSwitch.width/8);
    ucTMP_PassModeSwitch = 0x2;
  }
  else if (cSelect == 1) {
    lcd240x128.DISPLAY_GRAPHIC_XY(cPassServerLCN[cPosition].X,cPassServerLCN[cPosition].Y,word8x16[2],cPassServerLCN[cPosition].height,cPassServerLCN[cPosition].width/8);
    ucTMP_PassServerLCN[cPosition] = 0x2;
    doKeyRIGHTWork();
  }

} catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenPassModeSet::doKey3Work(void)
{
try {
  if (cSelect == 0) {
  }
  else if (cSelect == 1) {
    lcd240x128.DISPLAY_GRAPHIC_XY(cPassServerLCN[cPosition].X,cPassServerLCN[cPosition].Y,word8x16[3],cPassServerLCN[cPosition].height,cPassServerLCN[cPosition].width/8);
    ucTMP_PassServerLCN[cPosition] = 0x3;
    doKeyRIGHTWork();
  }

} catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenPassModeSet::doKey4Work(void)
{
try {
  if (cSelect == 0) {
  }
  else if (cSelect == 1) {
    lcd240x128.DISPLAY_GRAPHIC_XY(cPassServerLCN[cPosition].X,cPassServerLCN[cPosition].Y,word8x16[4],cPassServerLCN[cPosition].height,cPassServerLCN[cPosition].width/8);
    ucTMP_PassServerLCN[cPosition] = 0x4;
    doKeyRIGHTWork();
  }
} catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenPassModeSet::doKey5Work(void)
{
try {
  if (cSelect == 0) {
  }
  else if (cSelect == 1) {
    lcd240x128.DISPLAY_GRAPHIC_XY(cPassServerLCN[cPosition].X,cPassServerLCN[cPosition].Y,word8x16[5],cPassServerLCN[cPosition].height,cPassServerLCN[cPosition].width/8);
    ucTMP_PassServerLCN[cPosition] = 0x5;
    doKeyRIGHTWork();
  }
} catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenPassModeSet::doKey6Work(void)
{
try {
  if (cSelect == 0) {
  }
  else if (cSelect == 1) {
    lcd240x128.DISPLAY_GRAPHIC_XY(cPassServerLCN[cPosition].X,cPassServerLCN[cPosition].Y,word8x16[6],cPassServerLCN[cPosition].height,cPassServerLCN[cPosition].width/8);
    ucTMP_PassServerLCN[cPosition] = 0x6;
    doKeyRIGHTWork();
  }

} catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenPassModeSet::doKey7Work(void)
{
try {
  if (cSelect == 0) {
  }
  else if (cSelect == 1) {
    lcd240x128.DISPLAY_GRAPHIC_XY(cPassServerLCN[cPosition].X,cPassServerLCN[cPosition].Y,word8x16[7],cPassServerLCN[cPosition].height,cPassServerLCN[cPosition].width/8);
    ucTMP_PassServerLCN[cPosition] = 0x7;
    doKeyRIGHTWork();
  }
} catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenPassModeSet::doKey8Work(void)
{
try {
  if (cSelect == 0) {
  }
  else if (cSelect == 1) {
    lcd240x128.DISPLAY_GRAPHIC_XY(cPassServerLCN[cPosition].X,cPassServerLCN[cPosition].Y,word8x16[8],cPassServerLCN[cPosition].height,cPassServerLCN[cPosition].width/8);
    ucTMP_PassServerLCN[cPosition] = 0x8;
    doKeyRIGHTWork();
  }

} catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenPassModeSet::doKey9Work(void)
{
try {
  if (cSelect == 0) {
  }
  else if (cSelect == 1) {
    lcd240x128.DISPLAY_GRAPHIC_XY(cPassServerLCN[cPosition].X,cPassServerLCN[cPosition].Y,word8x16[9],cPassServerLCN[cPosition].height,cPassServerLCN[cPosition].width/8);
    ucTMP_PassServerLCN[cPosition] = 0x9;
    doKeyRIGHTWork();
  }
} catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenPassModeSet::doKeyAWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenPassModeSet::doKeyBWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenPassModeSet::doKeyCWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenPassModeSet::doKeyDWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenPassModeSet::doKeyEWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenPassModeSet::doKeyFWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenPassModeSet::doKeyF1Work(void)
{
    screenMain.DisplayMain();
}
//---------------------------------------------------------------------------
void ScreenPassModeSet::doKeyF2Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenPassModeSet::doKeyF3Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenPassModeSet::doKeyF4Work(void)
{
    screenMain.DisplayMain();
}
//---------------------------------------------------------------------------
void ScreenPassModeSet::doKeyUPWork(void)
{
  if (cSelect == 0) {
  }
  else if (cSelect == 1) {
    clearCursor8x16(cPassServerLCN[cPosition].X,cPassServerLCN[cPosition].Y+16);
    cSelect = 0;
    cPosition = 0;
    setCursor8x16(cPassModeSwitch.X,cPassModeSwitch.Y+16);
  }
}
//---------------------------------------------------------------------------
void ScreenPassModeSet::doKeyDOWNWork(void)
{

  if (cSelect == 0) {
    clearCursor8x16(cPassModeSwitch.X,cPassModeSwitch.Y+16);
    cSelect = 1;  // = cSelect++;
    cPosition = 0;
    setCursor8x16(cPassServerLCN[cPosition].X,cPassServerLCN[cPosition].Y+16);
  }
}
//---------------------------------------------------------------------------
void ScreenPassModeSet::doKeyLEFTWork(void)
{
try {
  if (cSelect == 0) {
    clearCursor8x16(cPassModeSwitch.X,cPassModeSwitch.Y+16);
    setCursor8x16(cPassModeSwitch.X,cPassModeSwitch.Y+16);
  }

  else if (cSelect == 1) {
    clearCursor8x16(cPassServerLCN[cPosition].X,cPassServerLCN[cPosition].Y+16);
    if(cPosition > 0 ) {
      cPosition--;
      setCursor8x16(cPassServerLCN[cPosition].X,cPassServerLCN[cPosition].Y+16);
    }
    else {
      cSelect = 0;
      cPosition = 0;
      setCursor8x16(cPassModeSwitch.X,cPassModeSwitch.Y+16);
    }
  }

  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenPassModeSet::doKeyRIGHTWork(void)
{
try {

  if (cSelect == 0) {
    clearCursor8x16(cPassModeSwitch.X,cPassModeSwitch.Y+16);
    cPosition = 0;
    cSelect = 1;
    setCursor8x16(cPassServerLCN[cPosition].X,cPassServerLCN[cPosition].Y+16);
  }

  else if (cSelect == 1) {
    clearCursor8x16(cPassServerLCN[cPosition].X,cPassServerLCN[cPosition].Y+16);
    if(cPosition < 3 ) {
      cPosition++;
      setCursor8x16(cPassServerLCN[cPosition].X,cPassServerLCN[cPosition].Y+16);
    }
    else {
      setCursor8x16(cPassServerLCN[cPosition].X,cPassServerLCN[cPosition].Y+16);
    }
  }

  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenPassModeSet::doKeyEnterWork(void)
{

  vTmpArrayToData();

  if( vCheckRationality() ) { //Check
    smem.vSetPassMode(ucTMP_PassModeSwitch);
    smem.vSetPassServerLCN(usiPassServerLCN);

    screenMain.DisplayMain();
//
  }

}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void ScreenPassModeSet::vDisplayPassModeSwitch(void)
{
try{
  int iTMP;
  if(ucTMP_PassModeSwitch == 0x0)
    iTMP = 0;
  else if(ucTMP_PassModeSwitch == 0x1)
    iTMP = 1;
  else if(ucTMP_PassModeSwitch == 0x2)
    iTMP = 2;

  lcd240x128.DISPLAY_GRAPHIC_XY(cPassModeSwitch.X,cPassModeSwitch.Y,word8x16[iTMP],cPassModeSwitch.height,cPassModeSwitch.width/8);
}catch(...){}
}

//---------------------------------------------------------------------------
void ScreenPassModeSet::vDisplayPassModeServer(void)
{
try{
  //HEX
  lcd240x128.DISPLAY_GRAPHIC_XY(cPassServerLCN[0].X,cPassServerLCN[0].Y,word8x16[ucTMP_PassServerLCN[0]],cPassServerLCN[0].height,cPassServerLCN[0].width/8);
  lcd240x128.DISPLAY_GRAPHIC_XY(cPassServerLCN[1].X,cPassServerLCN[1].Y,word8x16[ucTMP_PassServerLCN[1]],cPassServerLCN[1].height,cPassServerLCN[1].width/8);
  lcd240x128.DISPLAY_GRAPHIC_XY(cPassServerLCN[2].X,cPassServerLCN[2].Y,word8x16[ucTMP_PassServerLCN[2]],cPassServerLCN[2].height,cPassServerLCN[2].width/8);
  lcd240x128.DISPLAY_GRAPHIC_XY(cPassServerLCN[3].X,cPassServerLCN[3].Y,word8x16[ucTMP_PassServerLCN[3]],cPassServerLCN[3].height,cPassServerLCN[3].width/8);

}catch(...){}
}

//---------------------------------------------------------------------------
void ScreenPassModeSet::vDataToTmpArray(void)
{
try{
  usiPassServerLCN = smem.vGetPassServerLCN();

  ucTMP_PassServerLCN[0] = (usiPassServerLCN / 1000) & 0xFF;
  ucTMP_PassServerLCN[1] = (usiPassServerLCN - (ucTMP_PassServerLCN[0]*1000)) / 100 & 0xFF;
  ucTMP_PassServerLCN[2] = (usiPassServerLCN - (ucTMP_PassServerLCN[0]*1000) - (ucTMP_PassServerLCN[1]*100)) / 10 & 0xFF;
  ucTMP_PassServerLCN[3] = (usiPassServerLCN % 10) & 0xFF;

  ucTMP_PassModeSwitch = smem.vGetPassMode();

}catch(...){}
}

//---------------------------------------------------------------------------
void ScreenPassModeSet::vTmpArrayToData(void)
{
try{

  usiPassServerLCN = 0;
  usiPassServerLCN += ucTMP_PassServerLCN[0]*1000;
  usiPassServerLCN += ucTMP_PassServerLCN[1]*100;
  usiPassServerLCN += ucTMP_PassServerLCN[2]*10;
  usiPassServerLCN += ucTMP_PassServerLCN[3];

}catch(...){}
}

//---------------------------------------------------------------------------
bool ScreenPassModeSet::vCheckRationality(void)
{
try{
  bool bRET;
  bRET = true;

  if(usiPassServerLCN > 9999)
    bRET = false;

  if(ucTMP_PassModeSwitch > 0x2)
    bRET = false;

  return bRET;

}catch(...){}
}

//---------------------------------------------------------------------------
bool ScreenPassModeSet::vInitUCTMPData(void)
{
try{
  bzero(ucTMP_PassServerLCN, 4);

}catch(...){}
}

