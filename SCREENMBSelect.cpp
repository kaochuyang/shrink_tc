#include "SCREENMBSelect.h"
#include "SCREENMain.h"
#include "SCREENOperStat.h"
#include "SCREENLockdb.h"
#include "SCREENWarning.h"

#include "LCD240x128.h"
#include "SMEM.h"

#include "CDataToMessageOK.h"
#include "WRITEJOB.h"

SCREENMBSelect screenMBSelect;
//---------------------------------------------------------------------------
SCREENMBSelect::SCREENMBSelect(void)
{
try {
    LoadBitmapFromFile();
    InitDispWord();
    cSelect=0;
  } catch (...) {}
}
//---------------------------------------------------------------------------
SCREENMBSelect::~SCREENMBSelect(void)
{
}
//---------------------------------------------------------------------------
void SCREENMBSelect::DoKeyWork(BYTE key)                                         //已過濾,進來的是0x80~0x98之間
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
        break;
        case 0x87:
        case 0x88:
        case 0x89:
        case 0x8A:
        case 0x8B:
        case 0x8C:
        case 0x8D:
        case 0x8E:
        case 0x8F:
        break;
        case 0x90://F1 ESC
          DoKeyF1Work();
        break;
        case 0x91:
        case 0x92:
          DoKeyF3Work();
        break;
        case 0x93://F4
          DoKeyF4Work();
        break;
        case 0x94:
          DoKeyUPWork();
        break;
        case 0x95:
          DoKeyDOWNWork();
        break;
        case 0x96:
          DoKeyLEFTWork();
        break;
        case 0x97:
          DoKeyRIGHTWork();
        break;
        case 0x98://ENTER
          DoKeyEnterWork();
        break;
        default:
        break;
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENMBSelect::LoadBitmapFromFile(void)
{
try {
    FILE *bitmap;
    bitmap=fopen("//cct//bitmap//backGround//01-08-F2-OTMBSelect.bit","rb");
    if (bitmap) {
        fread(ucBitmap,3840,1,bitmap);
        fclose(bitmap);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENMBSelect::DisplayMBSelect(void)
{
try {
    smem.SetcFace(cMBSELECT);
    smem.vWriteMsgToDOM("Enter MotherBoard Setup Screen");
    lcd240x128.DISPLAY_GRAPHIC(0,ucBitmap,128,30);

    cSelect = smem.vGetMotherBoardType();
    if(cSelect >= 3) {
      cSelect = 0;
    }
    else {
        setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(cycle.X,cycle.Y,word24x24[cSelect],cycle.height,cycle.width/8);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENMBSelect::InitDispWord(void)
{
try {
    cycle.X=144;
    cycle.Y=5;
    cycle.width=24;
    cycle.height=24;

    for (int i=0;i<6;i++) {
         if (i>=0 && i<=2) {
             mark[i].X=24;
             mark[i].Y=44+i*19;
         } else if (i>=3 && i<=5) {
             mark[i].X=120;
             mark[i].Y=44+(i-3)*19;
         }
         mark[i].width=16;
         mark[i].height=16;
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENMBSelect::DoKey0Work(void)
{
    if (cSelect<6) clearSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    cSelect=0;
    setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(cycle.X,cycle.Y,word24x24[cSelect],cycle.height,cycle.width/8);
}
//---------------------------------------------------------------------------
void SCREENMBSelect::DoKey1Work(void)
{
    if (cSelect<6) clearSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    cSelect=1;
    setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(cycle.X,cycle.Y,word24x24[cSelect],cycle.height,cycle.width/8);
}
//---------------------------------------------------------------------------
void SCREENMBSelect::DoKey2Work(void)
{
    if (cSelect<6) clearSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    cSelect=2;
    setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(cycle.X,cycle.Y,word24x24[cSelect],cycle.height,cycle.width/8);
}
//---------------------------------------------------------------------------
void SCREENMBSelect::DoKey3Work(void)
{
/*
    if (cSelect<6) clearSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    cSelect=3;
    setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(cycle.X,cycle.Y,word24x24[cSelect],cycle.height,cycle.width/8);
*/
}
//---------------------------------------------------------------------------
void SCREENMBSelect::DoKey4Work(void)
{
/*
    if (cSelect<6) clearSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    cSelect=4;
    setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(cycle.X,cycle.Y,word24x24[cSelect],cycle.height,cycle.width/8);
*/
}
//---------------------------------------------------------------------------
void SCREENMBSelect::DoKey5Work(void)
{
  /*
    if (cSelect<6) clearSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    cSelect=5;
    setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(cycle.X,cycle.Y,word24x24[cSelect],cycle.height,cycle.width/8);
*/
}
//---------------------------------------------------------------------------
void SCREENMBSelect::DoKeyF1Work(void)
{
    screenMain.DisplayMain();
}
//---------------------------------------------------------------------------
void SCREENMBSelect::DoKeyF4Work(void)
{
    screenOperStat.DisplayOperStat();
}
//---------------------------------------------------------------------------
void SCREENMBSelect::DoKeyF3Work(void)
{
try {

  } catch (...) {}
}

//---------------------------------------------------------------------------
void SCREENMBSelect::DoKeyEnterWork(void)
{
try {
  smem.SetLastFace(cMBSELECT);
  if (cSelect==0) {
    smem.vWriteMsgToDOM("Set MotherBoard To Arbor");
  }
  else if (cSelect==1) {
    smem.vWriteMsgToDOM("Set MotherBoard To ICOP-6047");
  }
  else if (cSelect==2) {
    smem.vWriteMsgToDOM("Set MotherBoard To ICOP-6115");
  }

  smem.vSetMotherBoardType(cSelect);

  screenWarning.DisplayWarning();


  } catch (...) {}
}

//---------------------------------------------------------------------------
void SCREENMBSelect::DoKeyDOWNWork(void)
{
try {
    if (cSelect<3) clearSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    cSelect++;
    if (cSelect>2) cSelect=2;
    setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(cycle.X,cycle.Y,word24x24[cSelect],cycle.height,cycle.width/8);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENMBSelect::DoKeyUPWork(void)
{
try {
    if (cSelect<3) clearSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    if (cSelect==0 || cSelect>=3) cSelect=1;
    cSelect--;
    setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(cycle.X,cycle.Y,word24x24[cSelect],cycle.height,cycle.width/8);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENMBSelect::DoKeyLEFTWork(void)
{
try {
/*
    if (cSelect<3) clearSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    if (cSelect>=3 && cSelect<=5) cSelect-=3;
    if (cSelect<3) setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    if (cSelect<3) lcd240x128.DISPLAY_GRAPHIC_XY(cycle.X,cycle.Y,word24x24[cSelect],cycle.height,cycle.width/8);
*/
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENMBSelect::DoKeyRIGHTWork(void)
{
try {
/*
    if (cSelect<3) clearSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    if (cSelect>=0 && cSelect<=2) cSelect+=3;
    if (cSelect<3) setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    if (cSelect<3) lcd240x128.DISPLAY_GRAPHIC_XY(cycle.X,cycle.Y,word24x24[cSelect],cycle.height,cycle.width/8);
*/
  } catch (...) {}
}
//---------------------------------------------------------------------------
