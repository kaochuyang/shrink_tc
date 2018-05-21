#include "screenCountdownProperty.h"
#include "LCD240x128.h"
#include "SCREENMain.h"
#include "SCREENCtlSetup.h"
#include "screenCarCountdownProperty.h"
#include "screenPedCountdownProperty.h"
#include "screenCarLearn.h"
#include "screenPedLearn.h"
#include "CSTC.h"
#include "SMEM.h"

#include <stdio.h>
//---------------------------------------------------------------------------
ScreenCountdownProperty screenCountdownProperty;
//---------------------------------------------------------------------------
ScreenCountdownProperty::ScreenCountdownProperty(void)
{
    loadBitmapFromFile();
    initDispWord();
    cSelect=0;
}
//---------------------------------------------------------------------------
ScreenCountdownProperty::~ScreenCountdownProperty(void)
{
}
//---------------------------------------------------------------------------
void ScreenCountdownProperty::loadBitmapFromFile(void)
{
try {
    FILE *bitmap;
    //bitmap=fopen("//cct//bitmap//backGround//TC5F//currentRunTable.bit","rb");//arwen modify
    bitmap=fopen("//cct//bitmap//backGround//actmenu//CountdownProperty.bit","rb");
    if (bitmap) {
        fread(CountdownPropertyBitmap,3840,1,bitmap);
        fclose(bitmap);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenCountdownProperty::DisplayCountdownProperty(void)
{
try {
    smem.SetcFace(cCountdownProperty);
    lcd240x128.DISPLAY_GRAPHIC(0,CountdownPropertyBitmap,128,30);
    cSelect=0;
    setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenCountdownProperty::initDispWord(void)
{
try {
    for (int i=0;i<10;i++) {
         if (i>=0 && i<=4) {
             mark[i].X=0;
             mark[i].Y=30+i*16;
         } else if (i>=5 && i<=9) {
             mark[i].X=116;
             mark[i].Y=30+(i-5)*16;
         }
         mark[i].width=16;
         mark[i].height=16;
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenCountdownProperty::doKeyWork(BYTE key)
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
void ScreenCountdownProperty::doKey0Work(void)
{

}
//---------------------------------------------------------------------------
void ScreenCountdownProperty::doKey1Work(void)
{
    screenCarCountdownProperty.DisplayCarCountdownProperty();
}
//---------------------------------------------------------------------------
void ScreenCountdownProperty::doKey2Work(void)
{
    screenPedCountdownProperty.DisplayPedCountdownProperty();
}
//---------------------------------------------------------------------------
void ScreenCountdownProperty::doKey3Work(void)
{
    screenCarLearn.DisplayCarLearn();
}
//---------------------------------------------------------------------------
void ScreenCountdownProperty::doKey4Work(void)
{
    screenPedLearn.DisplayPedLearn();
}
//---------------------------------------------------------------------------
void ScreenCountdownProperty::doKey5Work(void)
{

}
//---------------------------------------------------------------------------
void ScreenCountdownProperty::doKey6Work(void)
{

}
//---------------------------------------------------------------------------
void ScreenCountdownProperty::doKey7Work(void)
{


}
//---------------------------------------------------------------------------
void ScreenCountdownProperty::doKey8Work(void)
{

}
//---------------------------------------------------------------------------
void ScreenCountdownProperty::doKey9Work(void)
{

}
//---------------------------------------------------------------------------
void ScreenCountdownProperty::doKeyAWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenCountdownProperty::doKeyBWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenCountdownProperty::doKeyCWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenCountdownProperty::doKeyDWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenCountdownProperty::doKeyEWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenCountdownProperty::doKeyFWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenCountdownProperty::doKeyF1Work(void)
{
    screenMain.DisplayMain();
}
//---------------------------------------------------------------------------
void ScreenCountdownProperty::doKeyF2Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenCountdownProperty::doKeyF3Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenCountdownProperty::doKeyF4Work(void)
{
    screenCtlSetup.DisplayCtlSetup();
}
//---------------------------------------------------------------------------
void ScreenCountdownProperty::doKeyUPWork(void)
{
try {
    clearSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    if (cSelect==0) cSelect=1;
    cSelect--;
    setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenCountdownProperty::doKeyDOWNWork(void)
{
try {
    clearSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    cSelect++;
    if (cSelect>=3) cSelect=3;
    setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenCountdownProperty::doKeyLEFTWork(void)
{

}
//---------------------------------------------------------------------------
void ScreenCountdownProperty::doKeyRIGHTWork(void)
{

}
//---------------------------------------------------------------------------
void ScreenCountdownProperty::doKeyEnterWork(void)
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
