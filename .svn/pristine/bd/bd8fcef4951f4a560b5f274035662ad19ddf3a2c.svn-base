#include "SCREENPedSignalHWSet.h"
#include "LCD240x128.h"
#include <stdio.h>
#include "SMEM.h"
#include "SCREENMain.h"
#include "SCREENPedSignalMenu.h"
#include "CSTORINI.h"

SCREENPedSignalHWSet screenPedSignalHWSet;

SCREENPedSignalHWSet::SCREENPedSignalHWSet()
{
    loadBitmapFromFile();
    initDispWord();
    cSelect=0;
}
SCREENPedSignalHWSet::~SCREENPedSignalHWSet()
{
}
//---------------------------------------------------------------------------
void SCREENPedSignalHWSet::loadBitmapFromFile(void)
{
try {
    FILE *bitmap;
    bitmap=fopen("//cct//bitmap//backGround//actmenu//pedSignalHWSet.bit","rb");
    if (bitmap) {
        fread(PedSignalHWSetBitmap,3840,1,bitmap);
        fclose(bitmap);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENPedSignalHWSet::DisplayPedSignalHWSet(void)
{
try {
    smem.SetcFace(cPedSignalHW);
    lcd240x128.DISPLAY_GRAPHIC(0,PedSignalHWSetBitmap,128,30);

    cSelect=0;
    if(smem.vGetUCData(PedestrianModeOpen)>0){
        cSelect=0;//open
    }else{
        cSelect=1;//close
    }
    setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENPedSignalHWSet::initDispWord(void)
{
try {
    for (int i=0;i<3;i++) {
         if (i>=0 && i<=4) {
             mark[i].X=22;
             mark[i].Y=32+i*16;
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
void SCREENPedSignalHWSet::doKeyWork(BYTE key)
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
void SCREENPedSignalHWSet::doKey0Work(void)
{
}
//---------------------------------------------------------------------------
void SCREENPedSignalHWSet::doKey1Work(void)
{
    smem.vSetUCData(PedestrianModeOpen, 1);
    oStorINI.vSaveIntToINI("CCTSwitch","PedestrianModeOpen",1);
    screenPedSignalMenu.DisplayPedSignalMenu();
}
//---------------------------------------------------------------------------
void SCREENPedSignalHWSet::doKey2Work(void)
{
    smem.vSetUCData(PedestrianModeOpen, 0);
    oStorINI.vSaveIntToINI("CCTSwitch","PedestrianModeOpen",0);
    screenPedSignalMenu.DisplayPedSignalMenu();
}
//---------------------------------------------------------------------------
void SCREENPedSignalHWSet::doKey3Work(void)
{

}
//---------------------------------------------------------------------------
void SCREENPedSignalHWSet::doKey4Work(void)
{

}
//---------------------------------------------------------------------------
void SCREENPedSignalHWSet::doKey5Work(void)
{

}
//---------------------------------------------------------------------------
void SCREENPedSignalHWSet::doKey6Work(void)
{

}
//---------------------------------------------------------------------------
void SCREENPedSignalHWSet::doKey7Work(void)
{

}
//---------------------------------------------------------------------------
void SCREENPedSignalHWSet::doKey8Work(void)
{
}
//---------------------------------------------------------------------------
void SCREENPedSignalHWSet::doKey9Work(void)
{
}
//---------------------------------------------------------------------------
void SCREENPedSignalHWSet::doKeyAWork(void)
{
}
//---------------------------------------------------------------------------
void SCREENPedSignalHWSet::doKeyBWork(void)
{
}
//---------------------------------------------------------------------------
void SCREENPedSignalHWSet::doKeyCWork(void)
{
}
//---------------------------------------------------------------------------
void SCREENPedSignalHWSet::doKeyDWork(void)
{
}
//---------------------------------------------------------------------------
void SCREENPedSignalHWSet::doKeyEWork(void)
{
}
//---------------------------------------------------------------------------
void SCREENPedSignalHWSet::doKeyFWork(void)
{
}
//---------------------------------------------------------------------------
void SCREENPedSignalHWSet::doKeyF1Work(void)
{
//    screenLogo.DisplayLogo();
    screenMain.DisplayMain();
}
//---------------------------------------------------------------------------
void SCREENPedSignalHWSet::doKeyF2Work(void)
{
}
//---------------------------------------------------------------------------
void SCREENPedSignalHWSet::doKeyF3Work(void)
{
}
//---------------------------------------------------------------------------
void SCREENPedSignalHWSet::doKeyF4Work(void)
{
    screenPedSignalMenu.DisplayPedSignalMenu();
}
//---------------------------------------------------------------------------
void SCREENPedSignalHWSet::doKeyUPWork(void)
{
try {
    clearSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    if (cSelect==0) cSelect=1;
    cSelect--;
    setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENPedSignalHWSet::doKeyDOWNWork(void)
{
try {
    clearSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    cSelect++;
    if (cSelect>=2) cSelect=2;
    setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENPedSignalHWSet::doKeyLEFTWork(void)
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
void SCREENPedSignalHWSet::doKeyRIGHTWork(void)
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
void SCREENPedSignalHWSet::doKeyEnterWork(void)
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
