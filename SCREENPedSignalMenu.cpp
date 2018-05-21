#include "SCREENPedSignalMenu.h"
#include "LCD240x128.h"
#include <stdio.h>
#include "SMEM.h"
#include "screenActuateEdit.h"
#include "SCREENMain.h"
#include "SCREENCtlSetup.h"
#include "SCREENPedSignalHWSet.h"
SCREENPedSignalMenu screenPedSignalMenu;
SCREENPedSignalMenu::SCREENPedSignalMenu()
{
    loadBitmapFromFile();
    initDispWord();
    cSelect=0;
}
SCREENPedSignalMenu::~SCREENPedSignalMenu()
{
}
//---------------------------------------------------------------------------
void SCREENPedSignalMenu::loadBitmapFromFile(void)
{
try {
    FILE *bitmap;
    bitmap=fopen("//cct//bitmap//backGround//actmenu//pedSignalMenu.bit","rb");
    if (bitmap) {
        fread(PedSignalMenuBitmap,3840,1,bitmap);
        fclose(bitmap);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENPedSignalMenu::DisplayPedSignalMenu(void)
{
try {
    smem.SetcFace(cPedSignalMenu);
    lcd240x128.DISPLAY_GRAPHIC(0,PedSignalMenuBitmap,128,30);
    cSelect=0;
    setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENPedSignalMenu::initDispWord(void)
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
void SCREENPedSignalMenu::doKeyWork(BYTE key)
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
void SCREENPedSignalMenu::doKey0Work(void)
{
}
//---------------------------------------------------------------------------
void SCREENPedSignalMenu::doKey1Work(void)
{
    screenPedSignalHWSet.DisplayPedSignalHWSet();
}
//---------------------------------------------------------------------------
void SCREENPedSignalMenu::doKey2Work(void)
{
    screenActuateEdit.DisplayActuateEdit();//行人觸動設定
}
//---------------------------------------------------------------------------
void SCREENPedSignalMenu::doKey3Work(void)
{

}
//---------------------------------------------------------------------------
void SCREENPedSignalMenu::doKey4Work(void)
{

}
//---------------------------------------------------------------------------
void SCREENPedSignalMenu::doKey5Work(void)
{

}
//---------------------------------------------------------------------------
void SCREENPedSignalMenu::doKey6Work(void)
{

}
//---------------------------------------------------------------------------
void SCREENPedSignalMenu::doKey7Work(void)
{

}
//---------------------------------------------------------------------------
void SCREENPedSignalMenu::doKey8Work(void)
{
}
//---------------------------------------------------------------------------
void SCREENPedSignalMenu::doKey9Work(void)
{
}
//---------------------------------------------------------------------------
void SCREENPedSignalMenu::doKeyAWork(void)
{
}
//---------------------------------------------------------------------------
void SCREENPedSignalMenu::doKeyBWork(void)
{
}
//---------------------------------------------------------------------------
void SCREENPedSignalMenu::doKeyCWork(void)
{
}
//---------------------------------------------------------------------------
void SCREENPedSignalMenu::doKeyDWork(void)
{
}
//---------------------------------------------------------------------------
void SCREENPedSignalMenu::doKeyEWork(void)
{
}
//---------------------------------------------------------------------------
void SCREENPedSignalMenu::doKeyFWork(void)
{
}
//---------------------------------------------------------------------------
void SCREENPedSignalMenu::doKeyF1Work(void)
{
//    screenLogo.DisplayLogo();
    screenMain.DisplayMain();
}
//---------------------------------------------------------------------------
void SCREENPedSignalMenu::doKeyF2Work(void)
{
}
//---------------------------------------------------------------------------
void SCREENPedSignalMenu::doKeyF3Work(void)
{
}
//---------------------------------------------------------------------------
void SCREENPedSignalMenu::doKeyF4Work(void)
{
//    screenMain.DisplayMain();
    screenCtlSetup.DisplayCtlSetup();
}
//---------------------------------------------------------------------------
void SCREENPedSignalMenu::doKeyUPWork(void)
{
try {
    clearSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    if (cSelect==0) cSelect=1;
    cSelect--;
    setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENPedSignalMenu::doKeyDOWNWork(void)
{
try {
    clearSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    cSelect++;
    if (cSelect>=2) cSelect=2;
    setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENPedSignalMenu::doKeyLEFTWork(void)
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
void SCREENPedSignalMenu::doKeyRIGHTWork(void)
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
void SCREENPedSignalMenu::doKeyEnterWork(void)
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
