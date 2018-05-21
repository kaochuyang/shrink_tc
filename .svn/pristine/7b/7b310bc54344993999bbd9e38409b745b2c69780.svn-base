#include "screenStrategy.h"
#include "LCD240x128.h"
#include "SCREENMain.h"
#include "screenRunTableList.h"
#include "CSTC.h"
#include <stdio.h>

#include "SMEM.h"
//---------------------------------------------------------------------------
ScreenStrategy screenStrategy;
//---------------------------------------------------------------------------
ScreenStrategy::ScreenStrategy(void)
{
    loadBitmapFromFile();
}
//---------------------------------------------------------------------------
ScreenStrategy::~ScreenStrategy(void)
{
}
//---------------------------------------------------------------------------
void ScreenStrategy::loadBitmapFromFile(void)
{
try {
    FILE *bitmap;
    bitmap=fopen("//cct//bitmap//backGround//TC5F//currentStrategy.bit","rb");
    if (bitmap) {
        fread(strategyBitmap,3840,1,bitmap);
        fclose(bitmap);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenStrategy::DisplayStrategy(void)
{
try {
    smem.SetcFace(cSTRATEGY);
    lcd240x128.DISPLAY_GRAPHIC(0,strategyBitmap,128,30);
    DisplayCStrategy();
  } catch (...) {}
}
//--------------------------------------------------------------------------
void ScreenStrategy::DisplayCStrategy(void)
{
try {
    if(smem.GetcFace()==cSTRATEGY) {
      int strategy=0;
      switch  (stc.Lock_to_LoadControlStrategy()) {
        case STRATEGY_TOD:
             strategy=10;
        break;
  //      case STRATEGY_CADC:
  //           strategy=11;
  //      break;
        case STRATEGY_ALLDYNAMIC:
             strategy=11;
        break;
        case STRATEGY_MANUAL:
             strategy=12;
        break;
        case STRATEGY_FLASH:
             strategy=13;
        break;
        case STRATEGY_ALLRED:
             strategy=14;
        break;
        case STRATEGY_CADC:
             strategy=15;
        break;
        case STRATEGY_AUTO_CADC:
             strategy=15;
        break;
        default:
             strategy=10;
        break;
      }

    lcd240x128.DISPLAY_GRAPHIC_XY(120,3,word24x24[strategy],24,3);
    lcd240x128.DISPLAY_GRAPHIC_XY(120,3,word24x24[strategy],24,3);
    }
  } catch (...) {}
}
//--------------------------------------------------------------------------
void ScreenStrategy::doKeyWork(BYTE key)
{
try {
    switch (key) {
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
void ScreenStrategy::doKeyF1Work(void)
{
    screenMain.DisplayMain();
}
//---------------------------------------------------------------------------
void ScreenStrategy::doKeyF2Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenStrategy::doKeyF3Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenStrategy::doKeyF4Work(void)
{
    screenRunTableList.DisplayRunTableList();
}
//---------------------------------------------------------------------------
void ScreenStrategy::doKeyUPWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenStrategy::doKeyDOWNWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenStrategy::doKeyLEFTWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenStrategy::doKeyRIGHTWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenStrategy::doKeyEnterWork(void)
{
    screenRunTableList.DisplayRunTableList();
}
//---------------------------------------------------------------------------
