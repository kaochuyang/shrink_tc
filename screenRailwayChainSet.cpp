#include "screenRailwayChainSet.h"
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

ScreenRailwayChainSet screenRailwayChainSet;
//---------------------------------------------------------------------------
ScreenRailwayChainSet::ScreenRailwayChainSet(void)
{
    LoadBitmapFromFile();
    InitDispWord();
    cPosition=0;
    cSelect = 0;
}
//---------------------------------------------------------------------------
ScreenRailwayChainSet::~ScreenRailwayChainSet(void)
{
}
//---------------------------------------------------------------------------
void ScreenRailwayChainSet::LoadBitmapFromFile(void)
{
try {
    FILE *bitmap;
    bitmap=fopen("//cct//bitmap//backGround//TC5F//RailwayChainSet.bit","rb");
    if (bitmap) {
        fread(chainRailwaySetBitmap,3840,1,bitmap);
        fclose(bitmap);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenRailwayChainSet::DisplayRailwayChainSet(void)
{
try {
    smem.SetcFace(cRAILWAYCHAINSET);
    lcd240x128.DISPLAY_GRAPHIC(0,chainRailwaySetBitmap,128,30);
    InitDispWord();
    DisplayData();

    cPosition=0;
    cSelect=0;
    setCursor8x16(cChainTrainEnableWord.X, cChainTrainEnableWord.Y+16);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenRailwayChainSet::InitDispWord(void)
{

    cChainTrainEnableWord.X = 80;
    cChainTrainEnableWord.Y = 32;
    cChainTrainEnableWord.width = 8;
    cChainTrainEnableWord.height = 16;

    cChainTrainComingJumpSubphaseWord.X = 120;
    cChainTrainComingJumpSubphaseWord.Y = 64;
    cChainTrainComingJumpSubphaseWord.width = 8;
    cChainTrainComingJumpSubphaseWord.height = 16;

    cChainTrainComingBanSubphaseWord.X = 120;
    cChainTrainComingBanSubphaseWord.Y = 80;
    cChainTrainComingBanSubphaseWord.width = 8;
    cChainTrainComingBanSubphaseWord.height = 16;

}
//---------------------------------------------------------------------------
void ScreenRailwayChainSet::DisplayData(void)
{
try {
    int iTmp;

    ucChainTrainEnable = smem.vGetUCData(TC_TrainChainEnable);
    ucChainTrainComingJumpSubphase = smem.vGetUCData(TC_TrainComingForceJumpSubphase);
    ucChainTrainComingBanSubphase = smem.vGetUCData(TC_TrainComingBanSubphase);

    ucChainTrainComingJumpSubphase += 1;
    ucChainTrainComingBanSubphase += 1;

    lcd240x128.DISPLAY_GRAPHIC_XY(cChainTrainEnableWord.X,cChainTrainEnableWord.Y,word8x16[ucChainTrainEnable],cChainTrainEnableWord.height,cChainTrainEnableWord.width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(cChainTrainComingJumpSubphaseWord.X,cChainTrainComingJumpSubphaseWord.Y,word8x16[ucChainTrainComingJumpSubphase],cChainTrainComingJumpSubphaseWord.height,cChainTrainComingJumpSubphaseWord.width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(cChainTrainComingBanSubphaseWord.X,cChainTrainComingBanSubphaseWord.Y,word8x16[ucChainTrainComingBanSubphase],cChainTrainComingBanSubphaseWord.height,cChainTrainComingBanSubphaseWord.width/8);

  } catch (...) {}
}

//---------------------------------------------------------------------------
void ScreenRailwayChainSet::DoKeyWork(BYTE key)
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
void ScreenRailwayChainSet::DoKey0Work(void)
{
  if(cSelect == 0) {
    lcd240x128.DISPLAY_GRAPHIC_XY(cChainTrainEnableWord.X,cChainTrainEnableWord.Y,word8x16[0],cChainTrainEnableWord.height,cChainTrainEnableWord.width/8);
    ucChainTrainEnable = 0;
    DoKeyRIGHTWork();
  } else if(cSelect == 1) {
    DoKeyRIGHTWork();
  } else if (cSelect == 2) {
    DoKeyRIGHTWork();
  }
}
//---------------------------------------------------------------------------
void ScreenRailwayChainSet::DoKey1Work(void)
{
  if(cSelect == 0) {
    lcd240x128.DISPLAY_GRAPHIC_XY(cChainTrainEnableWord.X,cChainTrainEnableWord.Y,word8x16[1],cChainTrainEnableWord.height,cChainTrainEnableWord.width/8);
    ucChainTrainEnable = 1;
    DoKeyRIGHTWork();
  } else if(cSelect == 1) {
    lcd240x128.DISPLAY_GRAPHIC_XY(cChainTrainComingJumpSubphaseWord.X,cChainTrainComingJumpSubphaseWord.Y,word8x16[1],cChainTrainComingJumpSubphaseWord.height,cChainTrainComingJumpSubphaseWord.width/8);
    ucChainTrainComingJumpSubphase = 1;
    DoKeyRIGHTWork();
  } else if (cSelect == 2) {
    lcd240x128.DISPLAY_GRAPHIC_XY(cChainTrainComingBanSubphaseWord.X,cChainTrainComingBanSubphaseWord.Y,word8x16[1],cChainTrainComingBanSubphaseWord.height,cChainTrainComingBanSubphaseWord.width/8);
    ucChainTrainComingBanSubphase = 1;
    DoKeyRIGHTWork();
  }
}
//---------------------------------------------------------------------------
void ScreenRailwayChainSet::DoKey2Work(void)
{
  if(cSelect == 0) {
    DoKeyRIGHTWork();
  } else if(cSelect == 1) {
    lcd240x128.DISPLAY_GRAPHIC_XY(cChainTrainComingJumpSubphaseWord.X,cChainTrainComingJumpSubphaseWord.Y,word8x16[2],cChainTrainComingJumpSubphaseWord.height,cChainTrainComingJumpSubphaseWord.width/8);
    ucChainTrainComingJumpSubphase = 2;
    DoKeyRIGHTWork();
  } else if (cSelect == 2) {
    lcd240x128.DISPLAY_GRAPHIC_XY(cChainTrainComingBanSubphaseWord.X,cChainTrainComingBanSubphaseWord.Y,word8x16[2],cChainTrainComingBanSubphaseWord.height,cChainTrainComingBanSubphaseWord.width/8);
    ucChainTrainComingBanSubphase = 2;
    DoKeyRIGHTWork();
  }

}
//---------------------------------------------------------------------------
void ScreenRailwayChainSet::DoKey3Work(void)
{
  if(cSelect == 0) {
    DoKeyRIGHTWork();
  } else if(cSelect == 1) {
    lcd240x128.DISPLAY_GRAPHIC_XY(cChainTrainComingJumpSubphaseWord.X,cChainTrainComingJumpSubphaseWord.Y,word8x16[3],cChainTrainComingJumpSubphaseWord.height,cChainTrainComingJumpSubphaseWord.width/8);
    ucChainTrainComingJumpSubphase = 3;
    DoKeyRIGHTWork();
  } else if (cSelect == 2) {
    lcd240x128.DISPLAY_GRAPHIC_XY(cChainTrainComingBanSubphaseWord.X,cChainTrainComingBanSubphaseWord.Y,word8x16[3],cChainTrainComingBanSubphaseWord.height,cChainTrainComingBanSubphaseWord.width/8);
    ucChainTrainComingBanSubphase = 3;
    DoKeyRIGHTWork();
  }

}
//---------------------------------------------------------------------------
void ScreenRailwayChainSet::DoKey4Work(void)
{
  if(cSelect == 0) {
    DoKeyRIGHTWork();
  } else if(cSelect == 1) {
    lcd240x128.DISPLAY_GRAPHIC_XY(cChainTrainComingJumpSubphaseWord.X,cChainTrainComingJumpSubphaseWord.Y,word8x16[4],cChainTrainComingJumpSubphaseWord.height,cChainTrainComingJumpSubphaseWord.width/8);
    ucChainTrainComingJumpSubphase = 4;
    DoKeyRIGHTWork();
  } else if (cSelect == 2) {
    lcd240x128.DISPLAY_GRAPHIC_XY(cChainTrainComingBanSubphaseWord.X,cChainTrainComingBanSubphaseWord.Y,word8x16[4],cChainTrainComingBanSubphaseWord.height,cChainTrainComingBanSubphaseWord.width/8);
    ucChainTrainComingBanSubphase = 4;
    DoKeyRIGHTWork();
  }

}
//---------------------------------------------------------------------------
void ScreenRailwayChainSet::DoKey5Work(void)
{
  if(cSelect == 0) {
    DoKeyRIGHTWork();
  } else if(cSelect == 1) {
    lcd240x128.DISPLAY_GRAPHIC_XY(cChainTrainComingJumpSubphaseWord.X,cChainTrainComingJumpSubphaseWord.Y,word8x16[5],cChainTrainComingJumpSubphaseWord.height,cChainTrainComingJumpSubphaseWord.width/8);
    ucChainTrainComingJumpSubphase = 5;
    DoKeyRIGHTWork();
  } else if (cSelect == 2) {
    lcd240x128.DISPLAY_GRAPHIC_XY(cChainTrainComingBanSubphaseWord.X,cChainTrainComingBanSubphaseWord.Y,word8x16[5],cChainTrainComingBanSubphaseWord.height,cChainTrainComingBanSubphaseWord.width/8);
    ucChainTrainComingBanSubphase = 5;
    DoKeyRIGHTWork();
  }

}
//---------------------------------------------------------------------------
void ScreenRailwayChainSet::DoKey6Work(void)
{
  if(cSelect == 0) {
    DoKeyRIGHTWork();
  } else if(cSelect == 1) {
    lcd240x128.DISPLAY_GRAPHIC_XY(cChainTrainComingJumpSubphaseWord.X,cChainTrainComingJumpSubphaseWord.Y,word8x16[6],cChainTrainComingJumpSubphaseWord.height,cChainTrainComingJumpSubphaseWord.width/8);
    ucChainTrainComingJumpSubphase = 6;
    DoKeyRIGHTWork();
  } else if (cSelect == 2) {
    lcd240x128.DISPLAY_GRAPHIC_XY(cChainTrainComingBanSubphaseWord.X,cChainTrainComingBanSubphaseWord.Y,word8x16[6],cChainTrainComingBanSubphaseWord.height,cChainTrainComingBanSubphaseWord.width/8);
    ucChainTrainComingBanSubphase = 6;
    DoKeyRIGHTWork();
  }

}
//---------------------------------------------------------------------------
void ScreenRailwayChainSet::DoKey7Work(void)
{
  if(cSelect == 0) {
    DoKeyRIGHTWork();
  } else if(cSelect == 1) {
    lcd240x128.DISPLAY_GRAPHIC_XY(cChainTrainComingJumpSubphaseWord.X,cChainTrainComingJumpSubphaseWord.Y,word8x16[7],cChainTrainComingJumpSubphaseWord.height,cChainTrainComingJumpSubphaseWord.width/8);
    ucChainTrainComingJumpSubphase = 7;
    DoKeyRIGHTWork();
  } else if (cSelect == 2) {
    lcd240x128.DISPLAY_GRAPHIC_XY(cChainTrainComingBanSubphaseWord.X,cChainTrainComingBanSubphaseWord.Y,word8x16[7],cChainTrainComingBanSubphaseWord.height,cChainTrainComingBanSubphaseWord.width/8);
    ucChainTrainComingBanSubphase = 7;
    DoKeyRIGHTWork();
  }

}
//---------------------------------------------------------------------------
void ScreenRailwayChainSet::DoKey8Work(void)
{
  if(cSelect == 0) {
    DoKeyRIGHTWork();
  } else if(cSelect == 1) {
    lcd240x128.DISPLAY_GRAPHIC_XY(cChainTrainComingJumpSubphaseWord.X,cChainTrainComingJumpSubphaseWord.Y,word8x16[8],cChainTrainComingJumpSubphaseWord.height,cChainTrainComingJumpSubphaseWord.width/8);
    ucChainTrainComingJumpSubphase = 8;
    DoKeyRIGHTWork();
  } else if (cSelect == 2) {
    lcd240x128.DISPLAY_GRAPHIC_XY(cChainTrainComingBanSubphaseWord.X,cChainTrainComingBanSubphaseWord.Y,word8x16[8],cChainTrainComingBanSubphaseWord.height,cChainTrainComingBanSubphaseWord.width/8);
    ucChainTrainComingBanSubphase = 8;
    DoKeyRIGHTWork();
  }
}
//---------------------------------------------------------------------------
void ScreenRailwayChainSet::DoKey9Work(void)
{
  if(cSelect == 0) {
    DoKeyRIGHTWork();
  } else if(cSelect == 1) {
    DoKeyRIGHTWork();
  } else if (cSelect == 2) {
    DoKeyRIGHTWork();
  }

}
//---------------------------------------------------------------------------
void ScreenRailwayChainSet::DoKeyF1Work(void)
{
    screenMain.DisplayMain();
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void ScreenRailwayChainSet::DoKeyF4Work(void)
{
    screenChainMenu.DisplayChainMenu();
}
//---------------------------------------------------------------------------
void ScreenRailwayChainSet::doKeyUPWork(void)
{
try {
  if(cSelect == 0) {
    clearCursor8x16(cChainTrainEnableWord.X,cChainTrainEnableWord.Y+16);
    setCursor8x16(cChainTrainEnableWord.X,cChainTrainEnableWord.Y+16);
  } else if (cSelect == 1) {
    clearCursor8x16(cChainTrainComingJumpSubphaseWord.X,cChainTrainComingJumpSubphaseWord.Y+16);
    setCursor8x16(cChainTrainEnableWord.X,cChainTrainEnableWord.Y+16);
    cSelect = 0;
    setCursor8x16(cChainTrainEnableWord.X,cChainTrainEnableWord.Y+16);
  } else if (cSelect == 2) {
    clearCursor8x16(cChainTrainComingBanSubphaseWord.X,cChainTrainComingBanSubphaseWord.Y+16);
    setCursor8x16(cChainTrainComingJumpSubphaseWord.X,cChainTrainComingJumpSubphaseWord.Y+16);
    cSelect = 1;
    setCursor8x16(cChainTrainComingJumpSubphaseWord.X,cChainTrainComingJumpSubphaseWord.Y+16);
  }

  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenRailwayChainSet::doKeyDOWNWork(void)
{
try {
  if(cSelect == 0) {
    clearCursor8x16(cChainTrainEnableWord.X,cChainTrainEnableWord.Y+16);
    clearCursor8x16(cChainTrainComingJumpSubphaseWord.X,cChainTrainComingJumpSubphaseWord.Y+16);
    cSelect = 1;
    setCursor8x16(cChainTrainComingJumpSubphaseWord.X,cChainTrainComingJumpSubphaseWord.Y+16);
  } else if (cSelect == 1) {
    clearCursor8x16(cChainTrainComingJumpSubphaseWord.X,cChainTrainComingJumpSubphaseWord.Y+16);
    setCursor8x16(cChainTrainComingBanSubphaseWord.X,cChainTrainComingBanSubphaseWord.Y+16);
    cSelect = 2;
    setCursor8x16(cChainTrainComingBanSubphaseWord.X,cChainTrainComingBanSubphaseWord.Y+16);
  } else if (cSelect == 2) {
    clearCursor8x16(cChainTrainComingBanSubphaseWord.X,cChainTrainComingBanSubphaseWord.Y+16);
    setCursor8x16(cChainTrainComingBanSubphaseWord.X,cChainTrainComingBanSubphaseWord.Y+16);
    cSelect = 2;
    setCursor8x16(cChainTrainComingBanSubphaseWord.X,cChainTrainComingBanSubphaseWord.Y+16);
  }
}
 catch (...) {}
}

//---------------------------------------------------------------------------
void ScreenRailwayChainSet::DoKeyLEFTWork(void)
{
  if(cSelect == 0) {
    clearCursor8x16(cChainTrainEnableWord.X,cChainTrainEnableWord.Y+16);
    setCursor8x16(cChainTrainEnableWord.X,cChainTrainEnableWord.Y+16);
  } else if (cSelect == 1) {
    clearCursor8x16(cChainTrainComingJumpSubphaseWord.X,cChainTrainComingJumpSubphaseWord.Y+16);
    setCursor8x16(cChainTrainEnableWord.X,cChainTrainEnableWord.Y+16);
    cSelect = 0;
    setCursor8x16(cChainTrainEnableWord.X,cChainTrainEnableWord.Y+16);
  } else if (cSelect == 2) {
    clearCursor8x16(cChainTrainComingBanSubphaseWord.X,cChainTrainComingBanSubphaseWord.Y+16);
    setCursor8x16(cChainTrainComingJumpSubphaseWord.X,cChainTrainComingJumpSubphaseWord.Y+16);
    cSelect = 1;
    setCursor8x16(cChainTrainComingJumpSubphaseWord.X,cChainTrainComingJumpSubphaseWord.Y+16);
  }

}
//---------------------------------------------------------------------------
void ScreenRailwayChainSet::DoKeyRIGHTWork(void)
{
  if(cSelect == 0) {
    clearCursor8x16(cChainTrainEnableWord.X,cChainTrainEnableWord.Y+16);
    clearCursor8x16(cChainTrainComingJumpSubphaseWord.X,cChainTrainComingJumpSubphaseWord.Y+16);
    cSelect = 1;
    setCursor8x16(cChainTrainComingJumpSubphaseWord.X,cChainTrainComingJumpSubphaseWord.Y+16);
  } else if (cSelect == 1) {
    clearCursor8x16(cChainTrainComingJumpSubphaseWord.X,cChainTrainComingJumpSubphaseWord.Y+16);
    setCursor8x16(cChainTrainComingBanSubphaseWord.X,cChainTrainComingBanSubphaseWord.Y+16);
    cSelect = 2;
    setCursor8x16(cChainTrainComingBanSubphaseWord.X,cChainTrainComingBanSubphaseWord.Y+16);
  } else if (cSelect == 2) {
    clearCursor8x16(cChainTrainComingBanSubphaseWord.X,cChainTrainComingBanSubphaseWord.Y+16);
    setCursor8x16(cChainTrainComingBanSubphaseWord.X,cChainTrainComingBanSubphaseWord.Y+16);
    cSelect = 2;
    setCursor8x16(cChainTrainComingBanSubphaseWord.X,cChainTrainComingBanSubphaseWord.Y+16);
  }
}
//---------------------------------------------------------------------------
void ScreenRailwayChainSet::DoKeyEnterWork(void)
{
try {
  int iTmp;

  smem.vSetUCData(TC_TrainChainEnable, ucChainTrainEnable);
  smem.vSetUCData(TC_TrainComingForceJumpSubphase, ucChainTrainComingJumpSubphase-1);
  smem.vSetUCData(TC_TrainComingBanSubphase, ucChainTrainComingBanSubphase-1);

  screenChainMenu.DisplayChainMenu();

  } catch (...) {}
}
//---------------------------------------------------------------------------
