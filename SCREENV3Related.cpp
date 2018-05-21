#include "SCREENV3Related.h"
#include "LCD240x128.h"
//#include "screenLogo.h"
#include "SCREENMain.h"
#include "SCREENCtlSetup.h"
#include "screenPlan.h"
#include "SCREENHWCycle.h"
#include "SCREENModifyPWD.h"
#include "SCREENDBSetup.h"
#include "SCREENCtlNo.h"
#include "SCREENSendCycle.h"

#include "CSTC.h"
#include <stdio.h>
#include "SMEM.h"

//---------------------------------------------------
SCREENV3Related screenV3Related;
//---------------------------------------------------
SCREENV3Related::SCREENV3Related()
{
    loadBitmapFromFile();
    initDispWord();
    cSelect=0;
}
SCREENV3Related::~SCREENV3Related()
{

}
//---------------------------------------------------------------------------
void SCREENV3Related::loadBitmapFromFile(void)
{
try {
    FILE *bitmap;
    bitmap=fopen("//cct//bitmap//backGround//actmenu//v3RelatedMenu.bit","rb");
    if (bitmap) {
        fread(v3RelatedBitmap,3840,1,bitmap);
        fclose(bitmap);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENV3Related::DisplayV3Related(void)
{
try {
    smem.SetcFace(cV3RELATED);
    lcd240x128.DISPLAY_GRAPHIC(0,v3RelatedBitmap,128,30);
    cSelect=0;
    setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENV3Related::initDispWord(void)
{
try {
    for (int i=0;i<5;i++) {
         if (i>=0 && i<=4) {
             mark[i].X=0;
             mark[i].Y=34+i*16;
         } /*else if (i>=5 && i<=9) {
             mark[i].X=128;
             mark[i].Y=32+(i-5)*16;
         }*/
         mark[i].width=16;
         mark[i].height=16;
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENV3Related::doKeyWork(BYTE key)
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
void SCREENV3Related::doKey0Work(void)
{
}
//---------------------------------------------------------------------------
void SCREENV3Related::doKey1Work(void)
{
    screenCtlNo.DisplayCtlNo();//設備編號
}
//---------------------------------------------------------------------------
void SCREENV3Related::doKey2Work(void)
{
    screenHWCycle.DisplayHWCycle();//回報週期設定
}
//---------------------------------------------------------------------------
void SCREENV3Related::doKey3Work(void)
{
    screenSendCycle.DisplaySendCycle();//傳輸週期設定
}
//---------------------------------------------------------------------------
void SCREENV3Related::doKey4Work(void)
{
    screenModifyPWD.DisplayModifyPWD();//更新系統密碼
}
//---------------------------------------------------------------------------
void SCREENV3Related::doKey5Work(void)
{
    screenDBSetup.DisplayDBSetup();//資料庫操作設定
}
//---------------------------------------------------------------------------
void SCREENV3Related::doKey6Work(void)
{

}
//---------------------------------------------------------------------------
void SCREENV3Related::doKey7Work(void)
{

}
//---------------------------------------------------------------------------
void SCREENV3Related::doKey8Work(void)
{
}
//---------------------------------------------------------------------------
void SCREENV3Related::doKey9Work(void)
{
}
//---------------------------------------------------------------------------
void SCREENV3Related::doKeyAWork(void)
{
}
//---------------------------------------------------------------------------
void SCREENV3Related::doKeyBWork(void)
{
}
//---------------------------------------------------------------------------
void SCREENV3Related::doKeyCWork(void)
{
}
//---------------------------------------------------------------------------
void SCREENV3Related::doKeyDWork(void)
{
}
//---------------------------------------------------------------------------
void SCREENV3Related::doKeyEWork(void)
{
}
//---------------------------------------------------------------------------
void SCREENV3Related::doKeyFWork(void)
{
}
//---------------------------------------------------------------------------
void SCREENV3Related::doKeyF1Work(void)
{
//    screenLogo.DisplayLogo();
    screenMain.DisplayMain();
}
//---------------------------------------------------------------------------
void SCREENV3Related::doKeyF2Work(void)
{
}
//---------------------------------------------------------------------------
void SCREENV3Related::doKeyF3Work(void)
{
}
//---------------------------------------------------------------------------
void SCREENV3Related::doKeyF4Work(void)
{
    screenMain.DisplayMain();
    //screenCtlSetup.DisplayCtlSetup();
}
//---------------------------------------------------------------------------
void SCREENV3Related::doKeyUPWork(void)
{
try {
    clearSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    if (cSelect==0) cSelect=1;
    cSelect--;
    setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENV3Related::doKeyDOWNWork(void)
{
try {
    clearSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    cSelect++;
    if (cSelect>=4) cSelect=4;
    setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENV3Related::doKeyLEFTWork(void)
{
try {
    clearSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    /*
    if (cSelect>=5 && cSelect<=9)
        cSelect-=5;
    */
    setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENV3Related::doKeyRIGHTWork(void)
{
try {
    clearSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    /*
    if (cSelect>=0 && cSelect<=5)
        cSelect+=5;
    */
    setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENV3Related::doKeyEnterWork(void)
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
