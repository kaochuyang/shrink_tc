#include "screenWeekDayEditF2.h"
#include "screenWeekDay.h"
#include "SCREENMain.h"
#include "screenSegtype.h"
#include "screenSegtypeMenu.h"
//#include "faceDefine.h"
#include "LCD240x128.h"
#include "CSTC.h"
#include <stdio.h>

//Removed #include "screenSegTypeEditStep1.h"

#include "screenSegtypePreview.h"

#include "SMEM.h"

//---------------------------------------------------------------------------
ScreenWeekDayEditF2 screenWeekDayEditF2;
//---------------------------------------------------------------------------
ScreenWeekDayEditF2::ScreenWeekDayEditF2(void)
{
    loadBitmapFromFile();
    initDispWord();
    for (int i=0;i<14;i++) {
         weekDaySegtype[i]._segment_type=0;
         weekDaySegtype[i]._weekday=0;
         //CCJ++
         WeekTemp[i*2] = 0;
         WeekTemp[i*2+1] = 0;
         //CCJ--
    }
    cDatePosition=0;
    cSegPosition=0;
}
//---------------------------------------------------------------------------
ScreenWeekDayEditF2::~ScreenWeekDayEditF2(void)
{
}
//---------------------------------------------------------------------------
void ScreenWeekDayEditF2::loadBitmapFromFile(void)
{
try {
    FILE *bitmap;
    bitmap=fopen("//cct//bitmap//backGround//TC5F//weekDayEditF2.bit","rb");
    if (bitmap) {
        fread(weekDayBitmap,3840,1,bitmap);
        fclose(bitmap);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenWeekDayEditF2::DisplayWeekDay(void)
{
try {
    smem.SetcFace(cWEEKDAYEDITF2);
    lcd240x128.DISPLAY_GRAPHIC(0,weekDayBitmap,128,30);
    DisplayDate();

    LoadWeekDaySegtype(true);

    DisplayWeekDaySegtype();

    time_t currentTime=time(NULL);
    struct tm *now=localtime(&currentTime);

    if(   ((((((now->tm_mday-1)/7)+1)%2)==0 ) && (now->tm_wday>=((now->tm_mday-1)%7)))  //Even week
       || ((((((now->tm_mday-1)/7)+1)%2)> 0 ) && (now->tm_wday< ((now->tm_mday-1)%7))) ) {
       printf("printfMsg Even Week.\n");
       if (now->tm_wday==0)  cSegPosition=26;
       else cSegPosition=((now->tm_wday-1)*2)+14;
    }
    else {
      printf("printfMsg odd Week.\n");
      if (now->tm_wday==0)  cSegPosition=12;
      else cSegPosition=(now->tm_wday-1)*2;
    }


    setCursor8x16(segtypeWord[cSegPosition].X,segtypeWord[cSegPosition].Y+16);
  } catch (...) {}
}

//---------------------------------------------------------------------------
void ScreenWeekDayEditF2::vDisplayWeekDayFromPreview(int iSegtypePreviewSelectd)
{
try {
    smem.SetcFace(cWEEKDAYEDITF2);
    lcd240x128.DISPLAY_GRAPHIC(0,weekDayBitmap,128,30);
    DisplayDate();

//replace origral data
    if(iSegtypePreviewSelectd != INVAILDVALUE && iSegtypePreviewSelectd >= 1 && iSegtypePreviewSelectd <= 20)
      stc._panel_weekdayseg[iSegPositionPreview/2]._segment_type = iSegtypePreviewSelectd;

    LoadWeekDaySegtype(false);

    DisplayWeekDaySegtype();

    cSegPosition = iSegPositionPreview;


    setCursor8x16(segtypeWord[cSegPosition].X,segtypeWord[cSegPosition].Y+16);
  } catch (...) {}
}

//---------------------------------------------------------------------------
void ScreenWeekDayEditF2::initDispWord(void)
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
    for (int i=0;i<28;i++) {
         if (i>=0 && i<=1) segtypeWord[i].X=56+i*8;
         else if (i>=2 && i<=3) segtypeWord[i].X=80+(i-2)*8;
         else if (i>=4 && i<=5) segtypeWord[i].X=104+(i-4)*8;
         else if (i>=6 && i<=7) segtypeWord[i].X=128+(i-6)*8;
         else if (i>=8 && i<=9) segtypeWord[i].X=152+(i-8)*8;
         else if (i>=10 && i<=11) segtypeWord[i].X=176+(i-10)*8;
         else if (i>=12 && i<=13) segtypeWord[i].X=200+(i-12)*8;
         else if (i>=14 && i<=15) segtypeWord[i].X=56+(i-14)*8;
         else if (i>=16 && i<=17) segtypeWord[i].X=80+(i-16)*8;
         else if (i>=18 && i<=19) segtypeWord[i].X=104+(i-18)*8;
         else if (i>=20 && i<=21) segtypeWord[i].X=128+(i-20)*8;
         else if (i>=22 && i<=23) segtypeWord[i].X=152+(i-22)*8;
         else if (i>=24 && i<=25) segtypeWord[i].X=176+(i-24)*8;
         else if (i>=26 && i<=27) segtypeWord[i].X=200+(i-26)*8;


         if (i>=0 && i<=13)  segtypeWord[i].Y=52;
         else if (i>=14 && i<=27)  segtypeWord[i].Y=72;
         segtypeWord[i].width=8;
         segtypeWord[i].height=16;
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenWeekDayEditF2::DisplayDate(void)
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
void ScreenWeekDayEditF2::LoadWeekDaySegtype(bool bLoadFromCSTC)
{
try {
    if(bLoadFromCSTC) {
      stc.Lock_to_Load_WeekDaySegment_for_Panel();
    }
    for (int i=0;i<14;i++) {
         weekDaySegtype[i]._segment_type=stc._panel_weekdayseg[i]._segment_type;
         weekDaySegtype[i]._weekday=stc._panel_weekdayseg[i]._weekday;

         WeekTemp[i*2] = weekDaySegtype[i]._segment_type / 10;
         WeekTemp[i*2+1] = weekDaySegtype[i]._segment_type % 10;
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenWeekDayEditF2::DisplayWeekDaySegtype(void)
{
try {
    for (int i=0;i<28;i+=2) {
         lcd240x128.DISPLAY_GRAPHIC_XY(segtypeWord[i].X,segtypeWord[i].Y,word8x16[weekDaySegtype[i/2]._segment_type/10],segtypeWord[i].height,segtypeWord[i].width/8);
         lcd240x128.DISPLAY_GRAPHIC_XY(segtypeWord[i+1].X,segtypeWord[i+1].Y,word8x16[weekDaySegtype[i/2]._segment_type%10],segtypeWord[i+1].height,segtypeWord[i+1].width/8);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
bool ScreenWeekDayEditF2::SaveWeekDaySegtype(bool bSaveToCSTC)
{
try {
    bool bCheckOK;
    int tmp_segtype = 0;
    bCheckOK = true;

    for(int i = 0; i < 14; i++) {
      tmp_segtype = WeekTemp[i*2] *10;
      tmp_segtype += WeekTemp[i*2+1];
      if(tmp_segtype == 0 || tmp_segtype > 7) {
        bCheckOK = false;
        printf("printfMsg tmp_segtype error: %d\n", tmp_segtype);
      }
    }

    if(bCheckOK) {
      for (int i=0;i<14;i++) {
        tmp_segtype = WeekTemp[i*2] *10;
        tmp_segtype += WeekTemp[i*2+1];
        if(tmp_segtype != 0)
          weekDaySegtype[i]._segment_type = tmp_segtype;
        stc._panel_weekdayseg[i]._segment_type=weekDaySegtype[i]._segment_type;
        stc._panel_weekdayseg[i]._weekday=weekDaySegtype[i]._weekday;
      }

      if(bSaveToCSTC) {
        stc.Lock_to_Save_WeekDaySegment_from_Panel();
        screenWeekDay.DisplayWeekDay();
      }
    }
    return bCheckOK;
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenWeekDayEditF2::doKeyWork(BYTE key)
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
void ScreenWeekDayEditF2::doKey0Work(void)
{
  lcd240x128.DISPLAY_GRAPHIC_XY(segtypeWord[cSegPosition].X,segtypeWord[cSegPosition].Y,word8x16[0],segtypeWord[cSegPosition].height,segtypeWord[cSegPosition].width/8);
  WeekTemp[cSegPosition] = 0;
  doKeyRIGHTWork();
}
//---------------------------------------------------------------------------
void ScreenWeekDayEditF2::doKey1Work(void)
{
  lcd240x128.DISPLAY_GRAPHIC_XY(segtypeWord[cSegPosition].X,segtypeWord[cSegPosition].Y,word8x16[1],segtypeWord[cSegPosition].height,segtypeWord[cSegPosition].width/8);
  WeekTemp[cSegPosition] = 0x1;
  doKeyRIGHTWork();
}
//---------------------------------------------------------------------------
void ScreenWeekDayEditF2::doKey2Work(void)
{
  lcd240x128.DISPLAY_GRAPHIC_XY(segtypeWord[cSegPosition].X,segtypeWord[cSegPosition].Y,word8x16[2],segtypeWord[cSegPosition].height,segtypeWord[cSegPosition].width/8);
  WeekTemp[cSegPosition] = 0x2;
  doKeyRIGHTWork();

}
//---------------------------------------------------------------------------
void ScreenWeekDayEditF2::doKey3Work(void)
{
  lcd240x128.DISPLAY_GRAPHIC_XY(segtypeWord[cSegPosition].X,segtypeWord[cSegPosition].Y,word8x16[3],segtypeWord[cSegPosition].height,segtypeWord[cSegPosition].width/8);
  WeekTemp[cSegPosition] = 0x3;
  doKeyRIGHTWork();
}
//---------------------------------------------------------------------------
void ScreenWeekDayEditF2::doKey4Work(void)
{
  lcd240x128.DISPLAY_GRAPHIC_XY(segtypeWord[cSegPosition].X,segtypeWord[cSegPosition].Y,word8x16[4],segtypeWord[cSegPosition].height,segtypeWord[cSegPosition].width/8);
  WeekTemp[cSegPosition] = 0x4;
  doKeyRIGHTWork();
}
//---------------------------------------------------------------------------
void ScreenWeekDayEditF2::doKey5Work(void)
{
  lcd240x128.DISPLAY_GRAPHIC_XY(segtypeWord[cSegPosition].X,segtypeWord[cSegPosition].Y,word8x16[5],segtypeWord[cSegPosition].height,segtypeWord[cSegPosition].width/8);
  WeekTemp[cSegPosition] = 0x5;
  doKeyRIGHTWork();
}
//---------------------------------------------------------------------------
void ScreenWeekDayEditF2::doKey6Work(void)
{
  lcd240x128.DISPLAY_GRAPHIC_XY(segtypeWord[cSegPosition].X,segtypeWord[cSegPosition].Y,word8x16[6],segtypeWord[cSegPosition].height,segtypeWord[cSegPosition].width/8);
  WeekTemp[cSegPosition] = 0x6;
  doKeyRIGHTWork();
}
//---------------------------------------------------------------------------
void ScreenWeekDayEditF2::doKey7Work(void)
{
  lcd240x128.DISPLAY_GRAPHIC_XY(segtypeWord[cSegPosition].X,segtypeWord[cSegPosition].Y,word8x16[7],segtypeWord[cSegPosition].height,segtypeWord[cSegPosition].width/8);
  WeekTemp[cSegPosition] = 0x7;
  doKeyRIGHTWork();
}
//---------------------------------------------------------------------------
void ScreenWeekDayEditF2::doKey8Work(void)
{
  lcd240x128.DISPLAY_GRAPHIC_XY(segtypeWord[cSegPosition].X,segtypeWord[cSegPosition].Y,word8x16[8],segtypeWord[cSegPosition].height,segtypeWord[cSegPosition].width/8);
  WeekTemp[cSegPosition] = 0x8;
  doKeyRIGHTWork();
}
//---------------------------------------------------------------------------
void ScreenWeekDayEditF2::doKey9Work(void)
{
  lcd240x128.DISPLAY_GRAPHIC_XY(segtypeWord[cSegPosition].X,segtypeWord[cSegPosition].Y,word8x16[9],segtypeWord[cSegPosition].height,segtypeWord[cSegPosition].width/8);
  WeekTemp[cSegPosition] = 0x9;
  doKeyRIGHTWork();
}
//---------------------------------------------------------------------------
void ScreenWeekDayEditF2::doKeyAWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenWeekDayEditF2::doKeyBWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenWeekDayEditF2::doKeyCWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenWeekDayEditF2::doKeyDWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenWeekDayEditF2::doKeyEWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenWeekDayEditF2::doKeyFWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenWeekDayEditF2::doKeyF1Work(void)
{
    screenMain.DisplayMain();
}
//---------------------------------------------------------------------------
void ScreenWeekDayEditF2::doKeyF2Work(void)
{
//Removed     screenSegTypeEditStep1.DisplaySegTypeEditStep1_keySegTypeID();
}
//---------------------------------------------------------------------------
void ScreenWeekDayEditF2::doKeyF3Work(void)
{
    SaveWeekDaySegtype(true);
    //OT Debug 951128
    smem.vSetVDUpdateDB(1,true);
}
//---------------------------------------------------------------------------
void ScreenWeekDayEditF2::doKeyF4Work(void)
{
//    screenSegtypeMenu.DisplaySegtypeMenu();
    screenWeekDay.DisplayWeekDay();
}
//---------------------------------------------------------------------------
void ScreenWeekDayEditF2::doKeyUPWork(void)
{
    clearCursor8x16(segtypeWord[cSegPosition].X,segtypeWord[cSegPosition].Y+16);
    if (cSegPosition>=14 && cSegPosition<=27) cSegPosition-=14;
    setCursor8x16(segtypeWord[cSegPosition].X,segtypeWord[cSegPosition].Y+16);
}
//---------------------------------------------------------------------------
void ScreenWeekDayEditF2::doKeyDOWNWork(void)
{
    clearCursor8x16(segtypeWord[cSegPosition].X,segtypeWord[cSegPosition].Y+16);
    if (cSegPosition>=0 && cSegPosition<=13) cSegPosition+=14;
    setCursor8x16(segtypeWord[cSegPosition].X,segtypeWord[cSegPosition].Y+16);
}
//---------------------------------------------------------------------------
void ScreenWeekDayEditF2::doKeyLEFTWork(void)
{
try {
    clearCursor8x16(segtypeWord[cSegPosition].X,segtypeWord[cSegPosition].Y+16);

    if (cSegPosition>=0 && cSegPosition<=13) {
        if (cSegPosition==0) cSegPosition=1;
    } else {
        if (cSegPosition==14) cSegPosition=15;
    }

    cSegPosition--;
    setCursor8x16(segtypeWord[cSegPosition].X,segtypeWord[cSegPosition].Y+16);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenWeekDayEditF2::doKeyRIGHTWork(void)
{
try {
    clearCursor8x16(segtypeWord[cSegPosition].X,segtypeWord[cSegPosition].Y+16);
    cSegPosition++;
    if (cSegPosition>=0 && cSegPosition<=14) {
        if (cSegPosition>=13) cSegPosition=13;
    } else {
        if (cSegPosition>=27) cSegPosition=27;
    }
    setCursor8x16(segtypeWord[cSegPosition].X,segtypeWord[cSegPosition].Y+16);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenWeekDayEditF2::doKeyEnterWork(void)
{
    bool bRet;
    bRet = SaveWeekDaySegtype(false);
    int x=weekDaySegtype[cSegPosition/2]._segment_type;

    iSegPositionPreview = cSegPosition;

    if (x>=0 && x<=20 && bRet) {
      screenSegtypePreview.DisplaySegtype(x, cWEEKDAY);
    }
}
//---------------------------------------------------------------------------
