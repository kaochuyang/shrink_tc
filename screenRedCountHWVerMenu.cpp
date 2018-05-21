#include "screenRedCountHWVerMenu.h"
#include "LCD240x128.h"
#include "SCREENMain.h"
#include <stdio.h>
#include <stdlib.h>

#include "CSTC.h"
#include "WRITEJOB.h"
#include "SMEM.h"

#include "screenRedCountMenu.h"

#include "SCREENHWReset.h"

//---------------------------------------------------------------------------
ScreenRedCountHWVerMenu screenRedCountHWVerMenu;
//---------------------------------------------------------------------------
ScreenRedCountHWVerMenu::ScreenRedCountHWVerMenu(void)
{
    loadBitmapFromFile();
    initDispWord();
    cSelect=0;
}
//---------------------------------------------------------------------------
ScreenRedCountHWVerMenu::~ScreenRedCountHWVerMenu(void)
{
}
//---------------------------------------------------------------------------
void ScreenRedCountHWVerMenu::loadBitmapFromFile(void)
{
try {
    FILE *bitmap;
    bitmap=fopen("//cct//bitmap//backGround//TC5F//redCountHWVerMenu.bit","rb");
    if (bitmap) {
        fread(redCountHWVerMenuBitmap,3840,1,bitmap);
        fclose(bitmap);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenRedCountHWVerMenu::DisplayRedCountHWVerMenu(void)
{
try {
    smem.SetcFace(cREDCOUNTHWVERMENU);
    lcd240x128.DISPLAY_GRAPHIC(0,redCountHWVerMenuBitmap,128,30);

    if( smem.vGetINTData(TC92_RedCountVer) == TC_RedCountVerHK)
      cSelect = 0;
    else if( smem.vGetINTData(TC92_RedCountVer) == TC_RedCountVer94)
      cSelect = 1;
    else if( smem.vGetINTData(TC92_RedCountVer) == TC_RedCountVer94v2)
      cSelect = 2;
    else if( smem.vGetINTData(TC92_RedCountVer) == TC_RedCountVerCCT97)
      cSelect = 3;

    setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenRedCountHWVerMenu::initDispWord(void)
{
try {
    for (int i=0;i<10;i++) {
         if (i>=0 && i<=4) {
             mark[i].X=24;
             mark[i].Y=24+i*16;
         } else if (i>=5 && i<=9) {
             mark[i].X=128;
             mark[i].Y=24+(i-5)*16;
         }
         mark[i].width=16;
         mark[i].height=16;
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenRedCountHWVerMenu::doKeyWork(BYTE key)
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
void ScreenRedCountHWVerMenu::doKey0Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenRedCountHWVerMenu::doKey1Work(void)
{

  smem.vSetINTData(TC92_RedCountVer, TC_RedCountVerHK);

  system("sync");
  stc.CalculateAndSendRedCount(10);
  writeJob.WriteLetW77E58AutoControl();                                         //let W77e58 autoControl.
  system("sync");
  system("sync");
  system("reboot");

  screenHWReset.DisplayHWReset();

}
//---------------------------------------------------------------------------
void ScreenRedCountHWVerMenu::doKey2Work(void)
{

  smem.vSetINTData(TC92_RedCountVer, TC_RedCountVer94);

  system("sync");
  stc.CalculateAndSendRedCount(10);
  writeJob.WriteLetW77E58AutoControl();
  system("sync");
  system("sync");
  system("reboot");

  screenHWReset.DisplayHWReset();

}
//---------------------------------------------------------------------------
void ScreenRedCountHWVerMenu::doKey3Work(void)
{
  smem.vSetINTData(TC92_RedCountVer, TC_RedCountVer94v2);

  system("sync");
  stc.CalculateAndSendRedCount(10);
  writeJob.WriteLetW77E58AutoControl();
  system("sync");
  system("sync");
  system("reboot");

  screenHWReset.DisplayHWReset();

}
//---------------------------------------------------------------------------
void ScreenRedCountHWVerMenu::doKey4Work(void)
{
  smem.vSetINTData(TC92_RedCountVer, TC_RedCountVerCCT97);

  system("sync");
  stc.CalculateAndSendRedCount(10);
  writeJob.WriteLetW77E58AutoControl();
  system("sync");
  system("sync");
  system("reboot");

  screenHWReset.DisplayHWReset();

}
//---------------------------------------------------------------------------
void ScreenRedCountHWVerMenu::doKey5Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenRedCountHWVerMenu::doKey6Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenRedCountHWVerMenu::doKey7Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenRedCountHWVerMenu::doKey8Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenRedCountHWVerMenu::doKey9Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenRedCountHWVerMenu::doKeyAWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenRedCountHWVerMenu::doKeyBWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenRedCountHWVerMenu::doKeyCWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenRedCountHWVerMenu::doKeyDWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenRedCountHWVerMenu::doKeyEWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenRedCountHWVerMenu::doKeyFWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenRedCountHWVerMenu::doKeyF1Work(void)
{
    screenMain.DisplayMain();
}
//---------------------------------------------------------------------------
void ScreenRedCountHWVerMenu::doKeyF2Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenRedCountHWVerMenu::doKeyF3Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenRedCountHWVerMenu::doKeyF4Work(void)
{
//    screenMain.DisplayMain();
    screenRedCountMenu.DisplayRedCountMenu();
}
//---------------------------------------------------------------------------
void ScreenRedCountHWVerMenu::doKeyUPWork(void)
{
try {
    clearSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    if (cSelect==0) cSelect=1;
    cSelect--;
    setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenRedCountHWVerMenu::doKeyDOWNWork(void)
{
try {
    clearSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    cSelect++;
    if (cSelect>=9) cSelect=9;
    setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenRedCountHWVerMenu::doKeyLEFTWork(void)
{
try {
    clearSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    if (cSelect>=5 && cSelect<=9)
        cSelect-=5;
    setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenRedCountHWVerMenu::doKeyRIGHTWork(void)
{
try {
    clearSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    if (cSelect>=0 && cSelect<=5)
        cSelect+=5;
    setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenRedCountHWVerMenu::doKeyEnterWork(void)
{

    smem.vSetUCData(TC_Redcount_Booting_Display_Enable, 1);

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
//---------------------------------------------------------------------------




