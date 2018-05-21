#include "screenSpecialDay.h"
#include "screenSpecialDayView.h"
#include "SCREENMain.h"
#include "screenSegtype.h"
#include "screenSegtypeMenu.h"
//#include "faceDefine.h"
#include "LCD240x128.h"
#include "CSTC.h"
#include <stdio.h>

#include "SMEM.h"

//---------------------------------------------------------------------------
ScreenSpecialDay screenSpecialDay;
//---------------------------------------------------------------------------
ScreenSpecialDay::ScreenSpecialDay(void)
{
    loadBitmapFromFile();
    initDispWord();
    for (int i=0;i<14;i++) {
         weekDaySegtype[i]._segment_type=0;
         weekDaySegtype[i]._weekday=0;
    }
    cDatePosition=0;
//    cSegPosition=0;

    cSegPosition = 0;
}
//---------------------------------------------------------------------------
ScreenSpecialDay::~ScreenSpecialDay(void)
{
}
//---------------------------------------------------------------------------
void ScreenSpecialDay::loadBitmapFromFile(void)
{
try {
    FILE *bitmap;
    bitmap=fopen("//cct//bitmap//backGround//TC5F//specialDay.bit","rb");
    if (bitmap) {
        fread(speicalDayBitmap,3840,1,bitmap);
        fclose(bitmap);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenSpecialDay::DisplaySpecialDay(void)
{
try {
    smem.SetcFace(cSPECIALDAY);
    lcd240x128.DISPLAY_GRAPHIC(0,speicalDayBitmap,128,30);
    DisplayDate();
    DisplayWeekDaySegtype();

    time_t currentTime=time(NULL);
    struct tm *now=localtime(&currentTime);

    setCursor16x16(segtypeWord[cSegPosition].X,segtypeWord[cSegPosition].Y+16);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenSpecialDay::initDispWord(void)
{
try {
    for (int i=0;i<8;i++) {
         if (i>=0 && i<=3) dateWord[i].X=128+i*8;                               //xxxx�~
         else if (i>=4 && i<=5) dateWord[i].X=176+(i-4)*8;                      //xx��
         else if (i>=6 && i<=7) dateWord[i].X=208+(i-6)*8;                      //xx��
         dateWord[i].Y=2;
         dateWord[i].width=8;
         dateWord[i].height=16;
    }
    for (int i=0;i<52;i++) {
         if (i>=0 && i<=1) segtypeWord[i].X=64+i*8;
         else if (i>=2 && i<=3) segtypeWord[i].X=88+(i-2)*8;
         else if (i>=4 && i<=5) segtypeWord[i].X=112+(i-4)*8;
         else if (i>=6 && i<=7) segtypeWord[i].X=136+(i-6)*8;
         else if (i>=8 && i<=9) segtypeWord[i].X=160+(i-8)*8;
         else if (i>=10 && i<=11) segtypeWord[i].X=184+(i-10)*8;
         else if (i>=12 && i<=13) segtypeWord[i].X=208+(i-12)*8;

         else if (i>=14 && i<=15) segtypeWord[i].X=64+(i-14)*8;
         else if (i>=16 && i<=17) segtypeWord[i].X=88+(i-16)*8;
         else if (i>=18 && i<=19) segtypeWord[i].X=112+(i-18)*8;
         else if (i>=20 && i<=21) segtypeWord[i].X=136+(i-20)*8;
         else if (i>=22 && i<=23) segtypeWord[i].X=160+(i-22)*8;
         else if (i>=24 && i<=25) segtypeWord[i].X=184+(i-24)*8;
         else if (i>=26 && i<=27) segtypeWord[i].X=208+(i-26)*8;

         else if (i>=28 && i<=29) segtypeWord[i].X=64+(i-28)*8;
         else if (i>=30 && i<=31) segtypeWord[i].X=88+(i-30)*8;
         else if (i>=32 && i<=33) segtypeWord[i].X=112+(i-32)*8;
         else if (i>=34 && i<=35) segtypeWord[i].X=136+(i-34)*8;
         else if (i>=36 && i<=37) segtypeWord[i].X=160+(i-36)*8;
         else if (i>=38 && i<=39) segtypeWord[i].X=184+(i-38)*8;

         else if (i>=40 && i<=41) segtypeWord[i].X=64+(i-40)*8;
         else if (i>=42 && i<=43) segtypeWord[i].X=88+(i-42)*8;
         else if (i>=44 && i<=45) segtypeWord[i].X=112+(i-44)*8;
         else if (i>=46 && i<=47) segtypeWord[i].X=136+(i-46)*8;
         else if (i>=48 && i<=49) segtypeWord[i].X=160+(i-48)*8;
         else if (i>=50 && i<=51) segtypeWord[i].X=184+(i-50)*8;

         if (i>=0 && i<=13)  segtypeWord[i].Y=24;
         else if (i>=14 && i<=27)  segtypeWord[i].Y=40;
         else if (i>=28 && i<=39)  segtypeWord[i].Y=64;
         else if (i>=40 && i<=51)  segtypeWord[i].Y=80;
         segtypeWord[i].width=8;
         segtypeWord[i].height=16;
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenSpecialDay::DisplayDate(void)
{
try {
    unsigned short cYear=0,cMonth=0,cDay=0;
    time_t currentTime=time(NULL);
    struct tm *now=localtime(&currentTime);
    cYear=now->tm_year+1900;
    cMonth=now->tm_mon+1;
    cDay=now->tm_mday;


    lcd240x128.DISPLAY_GRAPHIC_XY(dateWord[0].X,dateWord[0].Y,word8x16[cYear/1000],dateWord[0].height,dateWord[0].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(dateWord[1].X,dateWord[1].Y,word8x16[(cYear/100)%10],dateWord[1].height,dateWord[1].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(dateWord[2].X,dateWord[2].Y,word8x16[(cYear/10)%10],dateWord[2].height,dateWord[2].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(dateWord[3].X,dateWord[3].Y,word8x16[cYear%10],dateWord[3].height,dateWord[3].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(dateWord[4].X,dateWord[4].Y,word8x16[cMonth/10],dateWord[4].height,dateWord[4].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(dateWord[5].X,dateWord[5].Y,word8x16[cMonth%10],dateWord[5].height,dateWord[5].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(dateWord[6].X,dateWord[6].Y,word8x16[cDay/10],dateWord[6].height,dateWord[6].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(dateWord[7].X,dateWord[7].Y,word8x16[cDay%10],dateWord[7].height,dateWord[7].width/8);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenSpecialDay::DisplayWeekDaySegtype(void)
{
try {
    for (int i=0;i<26;i+=2) {
         if(i < 14) {
           lcd240x128.DISPLAY_GRAPHIC_XY(segtypeWord[i+14].X,segtypeWord[i+14].Y,word8x16[((i/2)+8)/10],segtypeWord[i+14].height,segtypeWord[i+14].width/8);
           lcd240x128.DISPLAY_GRAPHIC_XY(segtypeWord[i+15].X,segtypeWord[i+15].Y,word8x16[((i/2)+8)%10],segtypeWord[i+15].height,segtypeWord[i+15].width/8);
         } else {
           lcd240x128.DISPLAY_GRAPHIC_XY(segtypeWord[i+26].X,segtypeWord[i+26].Y,word8x16[((i/2)+8)/10],segtypeWord[i+26].height,segtypeWord[i+26].width/8);
           lcd240x128.DISPLAY_GRAPHIC_XY(segtypeWord[i+27].X,segtypeWord[i+27].Y,word8x16[((i/2)+8)%10],segtypeWord[i+27].height,segtypeWord[i+27].width/8);
         }
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenSpecialDay::doKeyWork(BYTE key)
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
void ScreenSpecialDay::doKey0Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenSpecialDay::doKey1Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenSpecialDay::doKey2Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenSpecialDay::doKey3Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenSpecialDay::doKey4Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenSpecialDay::doKey5Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenSpecialDay::doKey6Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenSpecialDay::doKey7Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenSpecialDay::doKey8Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenSpecialDay::doKey9Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenSpecialDay::doKeyAWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenSpecialDay::doKeyBWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenSpecialDay::doKeyCWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenSpecialDay::doKeyDWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenSpecialDay::doKeyEWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenSpecialDay::doKeyFWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenSpecialDay::doKeyF1Work(void)
{
    screenMain.DisplayMain();
}
//---------------------------------------------------------------------------
void ScreenSpecialDay::doKeyF2Work(void)
{
//    screenSegTypeEditStep1.DisplaySegTypeEditStep1_keySegTypeID();
//    screenSpecialDayEditF2.DisplayWeekDay();
}
//---------------------------------------------------------------------------
void ScreenSpecialDay::doKeyF3Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenSpecialDay::doKeyF4Work(void)
{
    screenSegtypeMenu.DisplaySegtypeMenu();
}
//---------------------------------------------------------------------------
void ScreenSpecialDay::doKeyUPWork(void)
{
    clearCursor16x16(segtypeWord[cSegPosition].X,segtypeWord[cSegPosition].Y+16);

    if (cSegPosition>=14 && cSegPosition<=39) cSegPosition-=14;
    else if (cSegPosition>=40 && cSegPosition<=51) cSegPosition-=12;
    setCursor16x16(segtypeWord[cSegPosition].X,segtypeWord[cSegPosition].Y+16);
}
//---------------------------------------------------------------------------
void ScreenSpecialDay::doKeyDOWNWork(void)
{
    clearCursor16x16(segtypeWord[cSegPosition].X,segtypeWord[cSegPosition].Y+16);
    if (cSegPosition>=0 && cSegPosition<=25) cSegPosition+=14;
    else if (cSegPosition>=28 && cSegPosition<=39) cSegPosition+=12;
    setCursor16x16(segtypeWord[cSegPosition].X,segtypeWord[cSegPosition].Y+16);
}
//---------------------------------------------------------------------------
void ScreenSpecialDay::doKeyLEFTWork(void)
{
try {
    clearCursor16x16(segtypeWord[cSegPosition].X,segtypeWord[cSegPosition].Y+16);

    if (cSegPosition > 1)
      cSegPosition-=2;

    setCursor16x16(segtypeWord[cSegPosition].X,segtypeWord[cSegPosition].Y+16);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenSpecialDay::doKeyRIGHTWork(void)
{
try {
    clearCursor16x16(segtypeWord[cSegPosition].X,segtypeWord[cSegPosition].Y+16);
    if (cSegPosition < 50)
      cSegPosition+=2;
    setCursor16x16(segtypeWord[cSegPosition].X,segtypeWord[cSegPosition].Y+16);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenSpecialDay::doKeyEnterWork(void)
{
  int x;
  if(cSegPosition < 14) {
    x= (cSegPosition/2)+8;
    if (x>=0 && x<=20) screenSpecialDayView.DisplaySpecialDayView(x);
  } else if(cSegPosition >= 28 && cSegPosition < 40) {
    x= ((cSegPosition-14)/2)+8;
    if (x>=0 && x<=20) screenSpecialDayView.DisplaySpecialDayView(x);
  } else if(cSegPosition >= 14 && cSegPosition < 28) {
    x= ((cSegPosition-14)/2)+8;
    screenSegtype.DisplaySegtype(x, cSPECIALDAY);
  } else if(cSegPosition >= 40) {
    x= ((cSegPosition-26)/2)+8;
    screenSegtype.DisplaySegtype(x, cSPECIALDAY);
  }
}
//---------------------------------------------------------------------------
