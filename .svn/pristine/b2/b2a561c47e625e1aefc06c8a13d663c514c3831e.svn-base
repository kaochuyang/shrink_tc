#include "screenCarCountdownProperty.h"
#include "screenCountdownProperty.h"
#include "LCD240x128.h"
#include "SCREENMain.h"
#include "CSTC.h"
#include "SMEM.h"

#include <stdio.h>
//---------------------------------------------------------------------------
ScreenCarCountdownProperty screenCarCountdownProperty;
//---------------------------------------------------------------------------
ScreenCarCountdownProperty::ScreenCarCountdownProperty(void)
{
try {
    LoadBitmapFromFile();
    InitDispWord();
  } catch (...) {}
}
//---------------------------------------------------------------------------
ScreenCarCountdownProperty::~ScreenCarCountdownProperty(void)
{
}
//---------------------------------------------------------------------------
void ScreenCarCountdownProperty::DoKeyWork(BYTE key)
{
try {
    switch (key) {
        case 0x80:
        case 0x81:
          DoKey1Work();
        break;
        case 0x82:
          DoKey2Work();
        break;
        case 0x83:
        case 0x84:
        case 0x85:
        case 0x86:
        case 0x87:
        case 0x88:
        case 0x89:
        case 0x8A:
        case 0x8B:
        case 0x8C:
        case 0x8D:
        case 0x8E:
        case 0x8F:
        case 0x90://F1 ESC
          DoKeyF1Work();
        break;
        case 0x91:
        break;
        case 0x92:
        break;
        case 0x93:
          DoKeyF4Work();
        break;
        case 0x94:
        case 0x95:
        case 0x96:
        case 0x97:
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
void ScreenCarCountdownProperty::LoadBitmapFromFile(void)
{
try {
    FILE *bitmap;
    bitmap=fopen("//cct//bitmap//backGround//actmenu//CarCountdownProperty.bit","rb");
    if (bitmap) {
        fread(CarCountdownPropertyBitmap,3840,1,bitmap);
    fclose(bitmap);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenCarCountdownProperty::InitDispWord(void)
{
    current.X=104;
    current.Y=5;
    current.width=24;
    current.height=24;
}
//----------------------------------------------------------
void ScreenCarCountdownProperty::DisplayCarCountdownProperty(void)
{
try{
    smem.SetcFace(cCarCountdownProperty);
    lcd240x128.DISPLAY_GRAPHIC(0,CarCountdownPropertyBitmap,128,30);

    unsigned short int Countdownunm = smem.GetCarCountdownProperty();
    printf("read smem Countdownunm: %d\n", Countdownunm);
    lcd240x128.DISPLAY_GRAPHIC_XY(current.X,current.Y,word24x24[Countdownunm],current.height,current.width/8);

 } catch (...) {}
}
////---------------------------------------------------------------------------
//void ScreenCarCountdownProperty::DoKey0Work(void)
//{
//try {
//    tmpCarCountdownProperty = 0;
//    lcd240x128.DISPLAY_GRAPHIC_XY(current.X,current.Y,word24x24[tmpCarCountdownProperty],current.height,current.width/8);
//  } catch (...) {}
//}
//---------------------------------------------------------------------------
void ScreenCarCountdownProperty::DoKey1Work(void)
{
try {
    tmpCarCountdownProperty = 1;
    lcd240x128.DISPLAY_GRAPHIC_XY(current.X,current.Y,word24x24[tmpCarCountdownProperty],current.height,current.width/8);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenCarCountdownProperty::DoKey2Work(void)
{
try {
    tmpCarCountdownProperty = 2;
    lcd240x128.DISPLAY_GRAPHIC_XY(current.X,current.Y,word24x24[tmpCarCountdownProperty],current.height,current.width/8);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenCarCountdownProperty::DoKeyF1Work(void)
{
    screenMain.DisplayMain();
}
//---------------------------------------------------------------------------
void ScreenCarCountdownProperty::DoKeyF4Work(void)
{
    screenCountdownProperty.DisplayCountdownProperty(); //倒數秒數屬性
}
//---------------------------------------------------------------------------
void ScreenCarCountdownProperty::DoKeyEnterWork(void)
{
    FILE *tmp;

    tmp = fopen("/cct/Data/SETTING/CarCountdownProperty.txt","w");
    if(tmp == NULL){
        printf("open CarCountdownProperty.txt error\n");
    }
    else fprintf(tmp,"enable=1\nProperty=%d",tmpCarCountdownProperty);

    fclose(tmp);

    smem.SetCarCountdownProperty(tmpCarCountdownProperty);
    screenCountdownProperty.DisplayCountdownProperty(); //倒數秒數屬性
}


