#include "screenSpecialDayEditF2.h"
#include  "screenSpecialDayView.h"
#include  "screenSpecialDay.h"
#include "screenSegtype.h"
#include "screenSegtypeMenu.h"
#include "SCREENMain.h"
#include "LCD240x128.h"

#include "CSTC.h"
#include "var.h"
#include <stdio.h>

#include "SMEM.h"
//---------------------------------------------------------------------------
ScreenSpecialDayEditF2 screenSpecialDayEditF2;
//---------------------------------------------------------------------------
ScreenSpecialDayEditF2::ScreenSpecialDayEditF2(void)
{
    loadBitmapFromFile();
    initDispWord();
    cPosition=0;
    cSelect=1;
    cFace=1;
}
//---------------------------------------------------------------------------
ScreenSpecialDayEditF2::~ScreenSpecialDayEditF2(void)
{
}
//---------------------------------------------------------------------------
void ScreenSpecialDayEditF2::loadBitmapFromFile(void)
{
try {
    FILE *bitmap;
    bitmap=fopen("//cct//bitmap//backGround//TC5F//specialDayEditF2.bit","rb");
    if (bitmap) {
        fread(specialDayBitmap,3840,1,bitmap);
        fclose(bitmap);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenSpecialDayEditF2::DisplaySpecialDayView(int iSegIn)
{
try {
    smem.SetcFace(cSPECIALDAYEDITF2);
    lcd240x128.DISPLAY_GRAPHIC(0,specialDayBitmap,128,30);
    cPosition=0;
    cSelect=1;
    iSeg = iSegIn;
    iSpecialDayID = iSeg - 7;
    DisplayDate();
    LoadSpecialDaySegtype();
    DisplaySpecialDaySegtype(iSegIn);
    vDiskplaySpecialDayID(iSpecialDayID);
    vDiskplaySegID(iSegIn);

    setCursor8x16(specialDayWord[cPosition].X,specialDayWord[cPosition].Y+16);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenSpecialDayEditF2::initDispWord(void)
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

    for (int i=0;i<16;i++) {
         if ((i>=0 && i<=3) || (i>=8 && i<=11) )  specialDayWord[i].X=48+(i%8)*8;
         else  if ((i>=4 && i<=5) || (i>=12 && i<=13) )  specialDayWord[i].X=96+(i%8-4)*8;
         else  if ((i>=6 && i<=7) || (i>=14 && i<=15) )  specialDayWord[i].X=128+(i%8-6)*8;

         if (i>=0 && i<=7) specialDayWord[i].Y=40;
         else if (i>=8 && i<=15) specialDayWord[i].Y=72;

         specialDayWord[i].width=8;
         specialDayWord[i].height=16;
    }

    SpecialDayIDWord[0].X = 24; SpecialDayIDWord[0].Y = 98; SpecialDayIDWord[0].width=8; SpecialDayIDWord[0].height = 16;
    SpecialDayIDWord[1].X = 32; SpecialDayIDWord[1].Y = 98; SpecialDayIDWord[1].width=8; SpecialDayIDWord[1].height = 16;

    SegIDWord[0].X = 88; SegIDWord[0].Y = 113; SegIDWord[0].width=8; SegIDWord[0].height = 16;
    SegIDWord[1].X = 96; SegIDWord[1].Y = 113; SegIDWord[1].width=8; SegIDWord[1].height = 16;


  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenSpecialDayEditF2::DisplayDate(void)
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
void ScreenSpecialDayEditF2::LoadSpecialDaySegtype(void)
{
try {
    for (int i=8;i<=20;i++) {
         stc.Lock_to_Load_HoliDaySegment_for_Panel(i);
         specialDaySegtype[i-8]._segment_type=stc._panel_holidayseg._segment_type;
         specialDaySegtype[i-8]._start_year=stc._panel_holidayseg._start_year;
         specialDaySegtype[i-8]._start_month=stc._panel_holidayseg._start_month;
         specialDaySegtype[i-8]._start_day=stc._panel_holidayseg._start_day;
         specialDaySegtype[i-8]._end_year=stc._panel_holidayseg._end_year;
         specialDaySegtype[i-8]._end_month=stc._panel_holidayseg._end_month;
         specialDaySegtype[i-8]._end_day=stc._panel_holidayseg._end_day;

         printf("stc._panel_holidayseg._seg %d\n", stc._panel_holidayseg._segment_type);
         printf("stc._end_year._reverse_time_type %d\n", stc._panel_holidayseg._end_year);

    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenSpecialDayEditF2::DisplaySpecialDaySegtype(int iSeg)
{
try {
    BYTE space[16]={0};

    lcd240x128.DISPLAY_GRAPHIC_XY(specialDayWord[0].X,specialDayWord[0].Y,word8x16[specialDaySegtype[iSeg-8]._start_year/1000],specialDayWord[0].height,specialDayWord[0].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(specialDayWord[1].X,specialDayWord[1].Y,word8x16[(specialDaySegtype[iSeg-8]._start_year/100)%10],specialDayWord[1].height,specialDayWord[1].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(specialDayWord[2].X,specialDayWord[2].Y,word8x16[(specialDaySegtype[iSeg-8]._start_year/10)%10],specialDayWord[2].height,specialDayWord[2].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(specialDayWord[3].X,specialDayWord[3].Y,word8x16[specialDaySegtype[iSeg-8]._start_year%10],specialDayWord[3].height,specialDayWord[3].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(specialDayWord[4].X,specialDayWord[4].Y,word8x16[specialDaySegtype[iSeg-8]._start_month/10],specialDayWord[4].height,specialDayWord[4].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(specialDayWord[5].X,specialDayWord[5].Y,word8x16[specialDaySegtype[iSeg-8]._start_month%10],specialDayWord[5].height,specialDayWord[5].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(specialDayWord[6].X,specialDayWord[6].Y,word8x16[specialDaySegtype[iSeg-8]._start_day/10],specialDayWord[6].height,specialDayWord[6].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(specialDayWord[7].X,specialDayWord[7].Y,word8x16[specialDaySegtype[iSeg-8]._start_day%10],specialDayWord[7].height,specialDayWord[7].width/8);

    lcd240x128.DISPLAY_GRAPHIC_XY(specialDayWord[8].X,specialDayWord[8].Y,word8x16[specialDaySegtype[iSeg-8]._end_year/1000],specialDayWord[8].height,specialDayWord[8].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(specialDayWord[9].X,specialDayWord[9].Y,word8x16[(specialDaySegtype[iSeg-8]._end_year/100)%10],specialDayWord[9].height,specialDayWord[9].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(specialDayWord[10].X,specialDayWord[10].Y,word8x16[(specialDaySegtype[iSeg-8]._end_year/10)%10],specialDayWord[10].height,specialDayWord[10].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(specialDayWord[11].X,specialDayWord[11].Y,word8x16[specialDaySegtype[iSeg-8]._end_year%10],specialDayWord[11].height,specialDayWord[11].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(specialDayWord[12].X,specialDayWord[12].Y,word8x16[specialDaySegtype[iSeg-8]._end_month/10],specialDayWord[12].height,specialDayWord[12].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(specialDayWord[13].X,specialDayWord[13].Y,word8x16[specialDaySegtype[iSeg-8]._end_month%10],specialDayWord[13].height,specialDayWord[13].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(specialDayWord[14].X,specialDayWord[14].Y,word8x16[specialDaySegtype[iSeg-8]._end_day/10],specialDayWord[14].height,specialDayWord[14].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(specialDayWord[15].X,specialDayWord[15].Y,word8x16[specialDaySegtype[iSeg-8]._end_day%10],specialDayWord[15].height,specialDayWord[15].width/8);

    ucSpecialDay[0] =  specialDaySegtype[iSeg-8]._start_year/1000;
    ucSpecialDay[1] = (specialDaySegtype[iSeg-8]._start_year/100)%10;
    ucSpecialDay[2] = (specialDaySegtype[iSeg-8]._start_year/10)%10;
    ucSpecialDay[3] =  specialDaySegtype[iSeg-8]._start_year%10;
    ucSpecialDay[4] =  specialDaySegtype[iSeg-8]._start_month/10;
    ucSpecialDay[5] =  specialDaySegtype[iSeg-8]._start_month%10;
    ucSpecialDay[6] =  specialDaySegtype[iSeg-8]._start_day/10;
    ucSpecialDay[7] =  specialDaySegtype[iSeg-8]._start_day%10;

    ucSpecialDay[8] =  specialDaySegtype[iSeg-8]._end_year/1000;
    ucSpecialDay[9] = (specialDaySegtype[iSeg-8]._end_year/100)%10;
    ucSpecialDay[10] = (specialDaySegtype[iSeg-8]._end_year/10)%10;
    ucSpecialDay[11] =  specialDaySegtype[iSeg-8]._end_year%10;
    ucSpecialDay[12] =  specialDaySegtype[iSeg-8]._end_month/10;
    ucSpecialDay[13] =  specialDaySegtype[iSeg-8]._end_month%10;
    ucSpecialDay[14] =  specialDaySegtype[iSeg-8]._end_day/10;
    ucSpecialDay[15] =  specialDaySegtype[iSeg-8]._end_day%10;


  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenSpecialDayEditF2::doKeyWork(BYTE key)
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
void ScreenSpecialDayEditF2::doKey0Work(void)
{
  lcd240x128.DISPLAY_GRAPHIC_XY(specialDayWord[cPosition].X,specialDayWord[cPosition].Y,word8x16[0],specialDayWord[cPosition].height,specialDayWord[cPosition].width/8);
  ucSpecialDay[cPosition] = 0;
  doKeyRIGHTWork();
}
//---------------------------------------------------------------------------
void ScreenSpecialDayEditF2::doKey1Work(void)
{
  lcd240x128.DISPLAY_GRAPHIC_XY(specialDayWord[cPosition].X,specialDayWord[cPosition].Y,word8x16[1],specialDayWord[cPosition].height,specialDayWord[cPosition].width/8);
  ucSpecialDay[cPosition] = 1;
  doKeyRIGHTWork();
}
//---------------------------------------------------------------------------
void ScreenSpecialDayEditF2::doKey2Work(void)
{
  lcd240x128.DISPLAY_GRAPHIC_XY(specialDayWord[cPosition].X,specialDayWord[cPosition].Y,word8x16[2],specialDayWord[cPosition].height,specialDayWord[cPosition].width/8);
  ucSpecialDay[cPosition] = 2;
  doKeyRIGHTWork();
}
//---------------------------------------------------------------------------
void ScreenSpecialDayEditF2::doKey3Work(void)
{
  lcd240x128.DISPLAY_GRAPHIC_XY(specialDayWord[cPosition].X,specialDayWord[cPosition].Y,word8x16[3],specialDayWord[cPosition].height,specialDayWord[cPosition].width/8);
  ucSpecialDay[cPosition] = 3;
  doKeyRIGHTWork();
}
//---------------------------------------------------------------------------
void ScreenSpecialDayEditF2::doKey4Work(void)
{
  lcd240x128.DISPLAY_GRAPHIC_XY(specialDayWord[cPosition].X,specialDayWord[cPosition].Y,word8x16[4],specialDayWord[cPosition].height,specialDayWord[cPosition].width/8);
  ucSpecialDay[cPosition] = 4;
  doKeyRIGHTWork();
}
//---------------------------------------------------------------------------
void ScreenSpecialDayEditF2::doKey5Work(void)
{
  lcd240x128.DISPLAY_GRAPHIC_XY(specialDayWord[cPosition].X,specialDayWord[cPosition].Y,word8x16[5],specialDayWord[cPosition].height,specialDayWord[cPosition].width/8);
  ucSpecialDay[cPosition] = 5;
  doKeyRIGHTWork();
}
//---------------------------------------------------------------------------
void ScreenSpecialDayEditF2::doKey6Work(void)
{
  lcd240x128.DISPLAY_GRAPHIC_XY(specialDayWord[cPosition].X,specialDayWord[cPosition].Y,word8x16[6],specialDayWord[cPosition].height,specialDayWord[cPosition].width/8);
  ucSpecialDay[cPosition] = 6;
  doKeyRIGHTWork();
}
//---------------------------------------------------------------------------
void ScreenSpecialDayEditF2::doKey7Work(void)
{
  lcd240x128.DISPLAY_GRAPHIC_XY(specialDayWord[cPosition].X,specialDayWord[cPosition].Y,word8x16[7],specialDayWord[cPosition].height,specialDayWord[cPosition].width/8);
  ucSpecialDay[cPosition] = 7;
  doKeyRIGHTWork();
}
//---------------------------------------------------------------------------
void ScreenSpecialDayEditF2::doKey8Work(void)
{
  lcd240x128.DISPLAY_GRAPHIC_XY(specialDayWord[cPosition].X,specialDayWord[cPosition].Y,word8x16[8],specialDayWord[cPosition].height,specialDayWord[cPosition].width/8);
  ucSpecialDay[cPosition] = 8;
  doKeyRIGHTWork();
}
//---------------------------------------------------------------------------
void ScreenSpecialDayEditF2::doKey9Work(void)
{
  lcd240x128.DISPLAY_GRAPHIC_XY(specialDayWord[cPosition].X,specialDayWord[cPosition].Y,word8x16[9],specialDayWord[cPosition].height,specialDayWord[cPosition].width/8);
  ucSpecialDay[cPosition] = 9;
  doKeyRIGHTWork();
}
//---------------------------------------------------------------------------
void ScreenSpecialDayEditF2::doKeyAWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenSpecialDayEditF2::doKeyBWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenSpecialDayEditF2::doKeyCWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenSpecialDayEditF2::doKeyDWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenSpecialDayEditF2::doKeyEWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenSpecialDayEditF2::doKeyFWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenSpecialDayEditF2::doKeyF1Work(void)
{
    screenMain.DisplayMain();
}
//---------------------------------------------------------------------------
void ScreenSpecialDayEditF2::doKeyF2Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenSpecialDayEditF2::doKeyF3Work(void)
{
  vTmpArrayToData();
  if(vCheck()) {
    vSaveSpecialDay();
    screenSpecialDayView.DisplaySpecialDayView(iSeg);
  }
}
//---------------------------------------------------------------------------
void ScreenSpecialDayEditF2::doKeyF4Work(void)
{
    screenSpecialDayView.DisplaySpecialDayView(iSeg);
}
//---------------------------------------------------------------------------
void ScreenSpecialDayEditF2::doKeyUPWork(void)
{
try {
  if(cPosition >= 8) {
    clearCursor8x16(specialDayWord[cPosition].X,specialDayWord[cPosition].Y+16);
    cPosition -= 8;
    setCursor8x16(specialDayWord[cPosition].X,specialDayWord[cPosition].Y+16);
  }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenSpecialDayEditF2::doKeyDOWNWork(void)
{
try {
  if(cPosition < 8) {
    clearCursor8x16(specialDayWord[cPosition].X,specialDayWord[cPosition].Y+16);
    cPosition += 8;
    setCursor8x16(specialDayWord[cPosition].X,specialDayWord[cPosition].Y+16);
  }


}catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenSpecialDayEditF2::doKeyLEFTWork(void)
{
try {
    clearCursor8x16(specialDayWord[cPosition].X,specialDayWord[cPosition].Y+16);
  if(cPosition > 0)
    cPosition -= 1;
    setCursor8x16(specialDayWord[cPosition].X,specialDayWord[cPosition].Y+16);
  } catch(...) {}
}
//---------------------------------------------------------------------------
void ScreenSpecialDayEditF2::doKeyRIGHTWork(void)
{
try {
  clearCursor8x16(specialDayWord[cPosition].X,specialDayWord[cPosition].Y+16);
  if(cPosition < 15)
    cPosition += 1;
  setCursor8x16(specialDayWord[cPosition].X,specialDayWord[cPosition].Y+16);

  } catch(...) {}
}
//---------------------------------------------------------------------------
void ScreenSpecialDayEditF2::doKeyEnterWork(void)
{
try {
/*
    vTmpArrayToData();
    if(vCheck()) {
      vSaveSpecialDay();
      screenSpecialDayView.DisplaySpecialDayView(iSeg);
    }
*/
  } catch (...){}
}

//---------------------------------------------------------------------------
void ScreenSpecialDayEditF2::vDiskplaySegID(int iSegIn)
{
try{
    lcd240x128.DISPLAY_GRAPHIC_XY(SegIDWord[0].X,SegIDWord[0].Y,word8x16[specialDaySegtype[iSegIn-8]._segment_type / 10],SegIDWord[0].height,SegIDWord[0].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(SegIDWord[1].X,SegIDWord[1].Y,word8x16[specialDaySegtype[iSegIn-8]._segment_type % 10],SegIDWord[1].height,SegIDWord[1].width/8);
}catch(...){}
}

//---------------------------------------------------------------------------
void ScreenSpecialDayEditF2::vTmpArrayToData(void)
{
try{
  specialDaySegtype[iSeg-8]._start_year =  ucSpecialDay[0] * 1000;
  specialDaySegtype[iSeg-8]._start_year += ucSpecialDay[1] * 100;
  specialDaySegtype[iSeg-8]._start_year += ucSpecialDay[2] * 10;
  specialDaySegtype[iSeg-8]._start_year += ucSpecialDay[3];

  specialDaySegtype[iSeg-8]._start_month = ucSpecialDay[4] * 10;
  specialDaySegtype[iSeg-8]._start_month += ucSpecialDay[5];

  specialDaySegtype[iSeg-8]._start_day = ucSpecialDay[6] * 10;
  specialDaySegtype[iSeg-8]._start_day += ucSpecialDay[7];

  specialDaySegtype[iSeg-8]._end_year =  ucSpecialDay[8] * 1000;
  specialDaySegtype[iSeg-8]._end_year += ucSpecialDay[9] * 100;
  specialDaySegtype[iSeg-8]._end_year += ucSpecialDay[10] * 10;
  specialDaySegtype[iSeg-8]._end_year += ucSpecialDay[11];

  specialDaySegtype[iSeg-8]._end_month = ucSpecialDay[12] * 10;
  specialDaySegtype[iSeg-8]._end_month += ucSpecialDay[13];

  specialDaySegtype[iSeg-8]._end_day = ucSpecialDay[14] * 10;
  specialDaySegtype[iSeg-8]._end_day += ucSpecialDay[15];

}catch(...){}
}

//---------------------------------------------------------------------------
bool ScreenSpecialDayEditF2::vCheck(void)
{
try{
  bool bRet = true;
  if(specialDaySegtype[iSeg-8]._start_year < 2000) { printf("printfMsg 1\n"); bRet = false; }
  if(specialDaySegtype[iSeg-8]._end_year > 2037)  { printf("printfMsg 2\n"); bRet = false; }

  if(specialDaySegtype[iSeg-8]._start_year >= specialDaySegtype[iSeg-8]._end_year) {
    if(specialDaySegtype[iSeg-8]._start_year > specialDaySegtype[iSeg-8]._end_year) { printf("printfMsg 3\n"); bRet = false; }
    if(specialDaySegtype[iSeg-8]._start_month >= specialDaySegtype[iSeg-8]._end_month) {
      if(specialDaySegtype[iSeg-8]._start_month > specialDaySegtype[iSeg-8]._end_month) { printf("printfMsg 4\n"); bRet = false; }
      if(specialDaySegtype[iSeg-8]._start_day > specialDaySegtype[iSeg-8]._end_day) {
        printf("printfMsg 5\n"); bRet = false;
      }
    }
  }

  if(specialDaySegtype[iSeg-8]._start_year < 2000) { printf("printfMsg 6\n"); bRet = false; }
  if(specialDaySegtype[iSeg-8]._end_year > 2037) { printf("printfMsg 7\n"); bRet = false; }
  if(specialDaySegtype[iSeg-8]._start_month < 1) { printf("printfMsg 8\n"); bRet = false; }
  if(specialDaySegtype[iSeg-8]._end_month > 12) { printf("printfMsg 9\n"); bRet = false; }
  if(specialDaySegtype[iSeg-8]._start_day < 1) { printf("printfMsg 10\n"); bRet = false; }
  if(specialDaySegtype[iSeg-8]._end_day > 31) { printf("printfMsg 11\n"); bRet = false; }

  return bRet;

}catch(...){}
}

//---------------------------------------------------------------------------
void ScreenSpecialDayEditF2::vSaveSpecialDay(void)
{
try{
  stc._panel_holidayseg._segment_type = iSeg;
  stc._panel_holidayseg._start_year = specialDaySegtype[iSeg-8]._start_year;
  stc._panel_holidayseg._end_year = specialDaySegtype[iSeg-8]._end_year;
  stc._panel_holidayseg._start_month = specialDaySegtype[iSeg-8]._start_month;
  stc._panel_holidayseg._end_month = specialDaySegtype[iSeg-8]._end_month;
  stc._panel_holidayseg._start_day = specialDaySegtype[iSeg-8]._start_day;
  stc._panel_holidayseg._end_day = specialDaySegtype[iSeg-8]._end_day;

  stc.Lock_to_Save_HoliDaySegment_from_Panel();

}catch(...){}
}

//---------------------------------------------------------------------------
void ScreenSpecialDayEditF2::vDiskplaySpecialDayID(int iSpecialDayIDIn)
{
try{
    lcd240x128.DISPLAY_GRAPHIC_XY(SpecialDayIDWord[0].X,SpecialDayIDWord[0].Y,word8x16[iSpecialDayIDIn / 10],SpecialDayIDWord[0].height,SpecialDayIDWord[0].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(SpecialDayIDWord[1].X,SpecialDayIDWord[1].Y,word8x16[iSpecialDayIDIn % 10],SpecialDayIDWord[1].height,SpecialDayIDWord[1].width/8);
}catch(...){}
}

