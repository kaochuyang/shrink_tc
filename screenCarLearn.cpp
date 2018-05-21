#include "screenCarLearn.h"
#include "screenCountdownProperty.h"
#include "LCD240x128.h"
#include "SCREENMain.h"
#include "CSTC.h"
#include "SMEM.h"

#include <stdio.h>
//---------------------------------------------------------------------------
ScreenCarLearn screenCarLearn;
//---------------------------------------------------------------------------
ScreenCarLearn::ScreenCarLearn(void)
{
try {
    LoadBitmapFromFile();
    InitDispWord();
  } catch (...) {}
}
//---------------------------------------------------------------------------
ScreenCarLearn::~ScreenCarLearn(void)
{
}
//---------------------------------------------------------------------------
void ScreenCarLearn::DoKeyWork(BYTE key)
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
void ScreenCarLearn::LoadBitmapFromFile(void)
{
try {
    FILE *bitmap;
    bitmap=fopen("//cct//bitmap//backGround//actmenu//CarLearn.bit","rb");
    if (bitmap) {
        fread(CarLearnBitmap,3840,1,bitmap);
    fclose(bitmap);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenCarLearn::InitDispWord(void)
{
    current.X=104;
    current.Y=5;
    current.width=24;
    current.height=24;
}
//----------------------------------------------------------
void ScreenCarLearn::DisplayCarLearn(void)
{
try{
    smem.SetcFace(cCarLearn);
    lcd240x128.DISPLAY_GRAPHIC(0,CarLearnBitmap,128,30);

    unsigned short int Countdownunm = smem.GetCarLearnSwitch();
    printf("read smem Countdownunm: %d\n", Countdownunm);
    lcd240x128.DISPLAY_GRAPHIC_XY(current.X,current.Y,word24x24[Countdownunm],current.height,current.width/8);

 } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenCarLearn::DoKey0Work(void)
{
try {
    tmpCarLearn = 0;
    lcd240x128.DISPLAY_GRAPHIC_XY(current.X,current.Y,word24x24[tmpCarLearn],current.height,current.width/8);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenCarLearn::DoKey1Work(void)
{
try {
    tmpCarLearn = 1;
    lcd240x128.DISPLAY_GRAPHIC_XY(current.X,current.Y,word24x24[tmpCarLearn],current.height,current.width/8);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenCarLearn::DoKeyF1Work(void)
{
    screenMain.DisplayMain();
}
//---------------------------------------------------------------------------
void ScreenCarLearn::DoKeyF4Work(void)
{
    screenCountdownProperty.DisplayCountdownProperty(); //倒數秒數屬性
}
//---------------------------------------------------------------------------
void ScreenCarLearn::DoKeyEnterWork(void)
{
    FILE *tmp;

    tmp = fopen("/cct/Data/SETTING/CarLearn.txt","w");
    if(tmp == NULL){
        printf("open CarLearn.txt error\n");
    }
    else fprintf(tmp,"enable=1\nswitch=%d",tmpCarLearn);

    fclose(tmp);

    smem.SetCarLearnSwitch(tmpCarLearn);
    screenCountdownProperty.DisplayCountdownProperty(); //倒數秒數屬性
}

