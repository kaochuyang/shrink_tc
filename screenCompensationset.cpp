#include "screenCompensationset.h"
#include "SCREENCtlSetup.h"
#include "LCD240x128.h"
#include "SCREENMain.h"
#include "screenRunTableList.h"
#include "CSTC.h"
#include "SMEM.h"

#include <stdio.h>
//---------------------------------------------------------------------------
ScreenCompensationset screenCompensationset;
//---------------------------------------------------------------------------
ScreenCompensationset::ScreenCompensationset(void)
{
try {
    LoadBitmapFromFile();
    InitDispWord();
  } catch (...) {}
}
//---------------------------------------------------------------------------
ScreenCompensationset::~ScreenCompensationset(void)
{
}
//---------------------------------------------------------------------------
void ScreenCompensationset::DoKeyWork(BYTE key)
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
void ScreenCompensationset::LoadBitmapFromFile(void)
{
try {
    FILE *bitmap;
    bitmap=fopen("//cct//bitmap//backGround//actmenu//compensationset.bit","rb");
    if (bitmap) {
        fread(CompensationBitmap,3840,1,bitmap);
    fclose(bitmap);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenCompensationset::InitDispWord(void)
{
    current.X=104;
    current.Y=5;
    current.width=24;
    current.height=24;
}
//----------------------------------------------------------
void ScreenCompensationset::Displaycompensation(void)
{
try{
    smem.SetcFace(cCompensation);
    lcd240x128.DISPLAY_GRAPHIC(0,CompensationBitmap,128,30);
//    printf("\n----- %s -----\n", "Displaycompensation");
//    printf("\n----- %s -----\n", "Displaycompensation");

    unsigned short int cycleunm = smem.GetCompensationcycle();
    printf("read smem cycleunm: %d\n", cycleunm);
    lcd240x128.DISPLAY_GRAPHIC_XY(current.X,current.Y,word24x24[cycleunm],current.height,current.width/8);

 } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenCompensationset::DoKey1Work(void)
{
try {
    tmpcycle = 1;
    lcd240x128.DISPLAY_GRAPHIC_XY(current.X,current.Y,word24x24[tmpcycle],current.height,current.width/8);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenCompensationset::DoKey2Work(void)
{
try {
    tmpcycle = 2;
    lcd240x128.DISPLAY_GRAPHIC_XY(current.X,current.Y,word24x24[tmpcycle],current.height,current.width/8);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenCompensationset::DoKeyF1Work(void)
{
    screenMain.DisplayMain();
}
//---------------------------------------------------------------------------
void ScreenCompensationset::DoKeyF4Work(void)
{
    screenCtlSetup.DisplayCtlSetup();
}
//---------------------------------------------------------------------------
void ScreenCompensationset::DoKeyEnterWork(void)
{
    FILE *tmp;

    tmp = fopen("/cct/Data/SETTING/compensationset.txt","w");
    if(tmp == NULL){
        printf("open compensationset.txt error\n");
    }
    else fprintf(tmp,"enable=1\ncycle=%d",tmpcycle);

    fclose(tmp);

    smem.SetCompensationcycle(tmpcycle);
    screenCtlSetup.DisplayCtlSetup();//arwen++ 控制器設定
}
