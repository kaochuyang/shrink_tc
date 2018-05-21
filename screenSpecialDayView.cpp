#include "screenSpecialDayView.h"
#include "screenSpecialDayEditF2.h"
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
ScreenSpecialDayView screenSpecialDayView;
//---------------------------------------------------------------------------
ScreenSpecialDayView::ScreenSpecialDayView(void)
{
    loadBitmapFromFile();
    initDispWord();
    cPosition=0;
    cSelect=1;
    cFace=1;
}
//---------------------------------------------------------------------------
ScreenSpecialDayView::~ScreenSpecialDayView(void)
{
}
//---------------------------------------------------------------------------
void ScreenSpecialDayView::loadBitmapFromFile(void)
{
try {
    FILE *bitmap;
    bitmap=fopen("//cct//bitmap//backGround//TC5F//specialDayView.bit","rb");
    if (bitmap) {
        fread(specialDayBitmap,3840,1,bitmap);
        fclose(bitmap);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenSpecialDayView::DisplaySpecialDayView(int iSegIn)
{
try {
    smem.SetcFace(cSPECIALDAYVIEW);
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
//    setCursor8x16(specialDayWord[cPosition].X,specialDayWord[cPosition].Y+16);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenSpecialDayView::initDispWord(void)
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
void ScreenSpecialDayView::DisplayDate(void)
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
void ScreenSpecialDayView::LoadSpecialDaySegtype(void)
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
         printf("stc._panel_holidayseg._start_year %d\n", stc._panel_holidayseg._start_year);
         printf("stc._panel_holidayseg._start_month %d\n", stc._panel_holidayseg._start_month);
         printf("stc._panel_holidayseg._start_day %d\n", stc._panel_holidayseg._start_day);
         printf("stc._panel_holidayseg._end_year %d\n", stc._panel_holidayseg._end_year);
         printf("stc._panel_holidayseg._end_month %d\n", stc._panel_holidayseg._end_month);
         printf("stc._panel_holidayseg._end_day %d\n", stc._panel_holidayseg._end_day);

    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenSpecialDayView::DisplaySpecialDaySegtype(int iSeg)
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

  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenSpecialDayView::doKeyWork(BYTE key)
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
void ScreenSpecialDayView::doKey0Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenSpecialDayView::doKey1Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenSpecialDayView::doKey2Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenSpecialDayView::doKey3Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenSpecialDayView::doKey4Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenSpecialDayView::doKey5Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenSpecialDayView::doKey6Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenSpecialDayView::doKey7Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenSpecialDayView::doKey8Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenSpecialDayView::doKey9Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenSpecialDayView::doKeyAWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenSpecialDayView::doKeyBWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenSpecialDayView::doKeyCWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenSpecialDayView::doKeyDWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenSpecialDayView::doKeyEWork(void)
{
  if(iSeg > 8)
    DisplaySpecialDayView(iSeg-1);
}
//---------------------------------------------------------------------------
void ScreenSpecialDayView::doKeyFWork(void)
{
  if(iSeg < 20)
    DisplaySpecialDayView(iSeg+1);
}
//---------------------------------------------------------------------------
void ScreenSpecialDayView::doKeyF1Work(void)
{
    screenMain.DisplayMain();
}
//---------------------------------------------------------------------------
void ScreenSpecialDayView::doKeyF2Work(void)
{
     screenSpecialDayEditF2.DisplaySpecialDayView(iSeg);
}
//---------------------------------------------------------------------------
void ScreenSpecialDayView::doKeyF3Work(void)
{

}
//---------------------------------------------------------------------------
void ScreenSpecialDayView::doKeyF4Work(void)
{
    screenSpecialDay.DisplaySpecialDay();
}
//---------------------------------------------------------------------------
void ScreenSpecialDayView::doKeyUPWork(void)
{
try {
  /*
  if(iSeg > 8)
    DisplaySpecialDayView(iSeg-1);
  */
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenSpecialDayView::doKeyDOWNWork(void)
{
try {
  /*
  if(iSeg < 20)
    DisplaySpecialDayView(iSeg+1);
  */

}catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenSpecialDayView::doKeyLEFTWork(void)
{
try {
  } catch(...) {}
}
//---------------------------------------------------------------------------
void ScreenSpecialDayView::doKeyRIGHTWork(void)
{
try {
  } catch(...) {}
}
//---------------------------------------------------------------------------
void ScreenSpecialDayView::doKeyEnterWork(void)
{
try {
//        if (x>=0 && x<=20) screenSegtype.DisplaySegtype(x);
  } catch (...){}
}

//---------------------------------------------------------------------------
void ScreenSpecialDayView::vDiskplaySpecialDayID(int iSpecialDayIDIn)
{
try{
    lcd240x128.DISPLAY_GRAPHIC_XY(SpecialDayIDWord[0].X,SpecialDayIDWord[0].Y,word8x16[iSpecialDayIDIn / 10],SpecialDayIDWord[0].height,SpecialDayIDWord[0].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(SpecialDayIDWord[1].X,SpecialDayIDWord[1].Y,word8x16[iSpecialDayIDIn % 10],SpecialDayIDWord[1].height,SpecialDayIDWord[1].width/8);
}catch(...){}
}
//---------------------------------------------------------------------------
void ScreenSpecialDayView::vDiskplaySegID(int iSegIn)
{
try{
    lcd240x128.DISPLAY_GRAPHIC_XY(SegIDWord[0].X,SegIDWord[0].Y,word8x16[specialDaySegtype[iSegIn-8]._segment_type / 10],SegIDWord[0].height,SegIDWord[0].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(SegIDWord[1].X,SegIDWord[1].Y,word8x16[specialDaySegtype[iSegIn-8]._segment_type % 10],SegIDWord[1].height,SegIDWord[1].width/8);
}catch(...){}
}

