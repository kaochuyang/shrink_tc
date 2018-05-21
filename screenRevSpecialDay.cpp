#include "screenRevSpecialDay.h"
#include "screenReverse.h"
#include "screenReverseMenu.h"
#include "SCREENMain.h"
#include "LCD240x128.h"
//#include "screenSegTypeEditStep1.h"

#include "CSTC.h"
#include "var.h"
#include <stdio.h>

#include "SMEM.h"
//---------------------------------------------------------------------------
ScreenRevSpecialDay screenRevSpecialDay;
//---------------------------------------------------------------------------
ScreenRevSpecialDay::ScreenRevSpecialDay(void)
{
    loadBitmapFromFile();
    initDispWord();
    cPosition=0;
    cSelect=1;
    cFace=1;
}
//---------------------------------------------------------------------------
ScreenRevSpecialDay::~ScreenRevSpecialDay(void)
{
}
//---------------------------------------------------------------------------
void ScreenRevSpecialDay::loadBitmapFromFile(void)
{
try {
    FILE *bitmap;
    bitmap=fopen("//cct//bitmap//backGround//TC5F//specialRevDay.bit","rb");
    if (bitmap) {
        fread(specialDayBitmap,3840,1,bitmap);
        fclose(bitmap);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenRevSpecialDay::DisplaySpecialDay(void)
{
try {
    smem.SetcFace(cREVSPECIALDAY);
    lcd240x128.DISPLAY_GRAPHIC(0,specialDayBitmap,128,30);
    cPosition=0;
    cSelect=1;
    DisplayDate();  DisplayDate();
    LoadSpecialDayRev();
    DisplaySpecialDayRev(1);
    setCursor8x16(specialDayWord[cPosition].X,specialDayWord[cPosition].Y+16);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenRevSpecialDay::initDispWord(void)
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
    for (int i=0;i<6;i+=2) {
         itemWord[i].X=24+(i%2)*8;
         itemWord[i+1].X=24+((i%2)+1)*8;

         itemWord[i].Y=40+(i/2)*17;
         itemWord[i+1].Y=40+(i/2)*17;

         itemWord[i].width=8;
         itemWord[i].height=16;
         itemWord[i+1].width=8;
         itemWord[i+1].height=16;
    }
    for (int i=0;i<30;i++) {
         if ((i>=0 && i<=3) || (i>=10 && i<=13) || (i>=20 && i<=23))  specialDayWord[i].X=48+(i%10)*8;
         else  if ((i>=4 && i<=5) || (i>=14 && i<=15) || (i>=24 && i<=25))  specialDayWord[i].X=96+(i%10-4)*8;
         else  if ((i>=6 && i<=7) || (i>=16 && i<=17) || (i>=26 && i<=27))  specialDayWord[i].X=128+(i%10-6)*8;
         else  if ((i>=8 && i<=9) || (i>=18 && i<=19) || (i>=28 && i<=29))  specialDayWord[i].X=176+(i%10-8)*8;

         if (i>=0 && i<=9) specialDayWord[i].Y=40;
         else if (i>=10 && i<=19) specialDayWord[i].Y=57;
         else if (i>=20 && i<=29) specialDayWord[i].Y=74;

         specialDayWord[i].width=8;
         specialDayWord[i].height=16;
    }

  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenRevSpecialDay::DisplayDate(void)
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
void ScreenRevSpecialDay::LoadSpecialDayRev(void)
{
try {
    for (int i=4;i<=16;i++) {
         stc.Lock_to_Load_HoliDayReverseTime_for_Panel(i);
         specialDayRev[i-4]._reverse_time_type=stc._panel_holidayrev._reverse_time_type;
         specialDayRev[i-4]._start_year=stc._panel_holidayrev._start_year;
         specialDayRev[i-4]._start_month=stc._panel_holidayrev._start_month;
         specialDayRev[i-4]._start_day=stc._panel_holidayrev._start_day;
         specialDayRev[i-4]._end_year=stc._panel_holidayrev._end_year;
         specialDayRev[i-4]._end_month=stc._panel_holidayrev._end_month;
         specialDayRev[i-4]._end_day=stc._panel_holidayrev._end_day;
         //CCJ++
         Tmp_specialDayRev[i-4]._reverse_time_type=stc._panel_holidayrev._reverse_time_type;
         Tmp_specialDayRev[i-4]._start_year=stc._panel_holidayrev._start_year;
         Tmp_specialDayRev[i-4]._start_month=stc._panel_holidayrev._start_month;
         Tmp_specialDayRev[i-4]._start_day=stc._panel_holidayrev._start_day;
         Tmp_specialDayRev[i-4]._end_year=stc._panel_holidayrev._end_year;
         Tmp_specialDayRev[i-4]._end_month=stc._panel_holidayrev._end_month;
         Tmp_specialDayRev[i-4]._end_day=stc._panel_holidayrev._end_day;
         //CCJ++

         printf("stc._panel_holidayrev._reverse_time_type %d\n", stc._panel_holidayrev._reverse_time_type);
         printf("stc._end_year._reverse_time_type %d\n", stc._panel_holidayrev._end_year);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenRevSpecialDay::DisplaySpecialDayRev(int page)
{
try {
    BYTE space[16]={0};

    cFace=page;
    DisplayItem(page);  DisplayItem(page);

    lcd240x128.DISPLAY_GRAPHIC_XY(specialDayWord[0].X,specialDayWord[0].Y,word8x16[Tmp_specialDayRev[page*3-2-1]._start_year/1000],specialDayWord[0].height,specialDayWord[0].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(specialDayWord[1].X,specialDayWord[1].Y,word8x16[(Tmp_specialDayRev[page*3-2-1]._start_year/100)%10],specialDayWord[1].height,specialDayWord[1].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(specialDayWord[2].X,specialDayWord[2].Y,word8x16[(Tmp_specialDayRev[page*3-2-1]._start_year/10)%10],specialDayWord[2].height,specialDayWord[2].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(specialDayWord[3].X,specialDayWord[3].Y,word8x16[Tmp_specialDayRev[page*3-2-1]._start_year%10],specialDayWord[3].height,specialDayWord[3].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(specialDayWord[4].X,specialDayWord[4].Y,word8x16[Tmp_specialDayRev[page*3-2-1]._start_month/10],specialDayWord[4].height,specialDayWord[4].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(specialDayWord[5].X,specialDayWord[5].Y,word8x16[Tmp_specialDayRev[page*3-2-1]._start_month%10],specialDayWord[5].height,specialDayWord[5].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(specialDayWord[6].X,specialDayWord[6].Y,word8x16[Tmp_specialDayRev[page*3-2-1]._start_day/10],specialDayWord[6].height,specialDayWord[6].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(specialDayWord[7].X,specialDayWord[7].Y,word8x16[Tmp_specialDayRev[page*3-2-1]._start_day%10],specialDayWord[7].height,specialDayWord[7].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(specialDayWord[8].X,specialDayWord[8].Y,word8x16[Tmp_specialDayRev[page*3-2-1]._reverse_time_type/10],specialDayWord[8].height,specialDayWord[8].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(specialDayWord[9].X,specialDayWord[9].Y,word8x16[Tmp_specialDayRev[page*3-2-1]._reverse_time_type%10],specialDayWord[9].height,specialDayWord[9].width/8);
    if (page<5) {
        lcd240x128.DISPLAY_GRAPHIC_XY(specialDayWord[10].X,specialDayWord[10].Y,word8x16[Tmp_specialDayRev[page*3-1-1]._start_year/1000],specialDayWord[10].height,specialDayWord[10].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(specialDayWord[11].X,specialDayWord[11].Y,word8x16[(Tmp_specialDayRev[page*3-1-1]._start_year/100)%10],specialDayWord[11].height,specialDayWord[11].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(specialDayWord[12].X,specialDayWord[12].Y,word8x16[(Tmp_specialDayRev[page*3-1-1]._start_year/10)%10],specialDayWord[12].height,specialDayWord[12].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(specialDayWord[13].X,specialDayWord[13].Y,word8x16[Tmp_specialDayRev[page*3-1-1]._start_year%10],specialDayWord[13].height,specialDayWord[13].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(specialDayWord[14].X,specialDayWord[14].Y,word8x16[Tmp_specialDayRev[page*3-1-1]._start_month/10],specialDayWord[14].height,specialDayWord[14].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(specialDayWord[15].X,specialDayWord[15].Y,word8x16[Tmp_specialDayRev[page*3-1-1]._start_month%10],specialDayWord[15].height,specialDayWord[15].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(specialDayWord[16].X,specialDayWord[16].Y,word8x16[Tmp_specialDayRev[page*3-1-1]._start_day/10],specialDayWord[16].height,specialDayWord[16].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(specialDayWord[17].X,specialDayWord[17].Y,word8x16[Tmp_specialDayRev[page*3-1-1]._start_day%10],specialDayWord[17].height,specialDayWord[17].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(specialDayWord[18].X,specialDayWord[18].Y,word8x16[Tmp_specialDayRev[page*3-1-1]._reverse_time_type/10],specialDayWord[18].height,specialDayWord[18].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(specialDayWord[19].X,specialDayWord[19].Y,word8x16[Tmp_specialDayRev[page*3-1-1]._reverse_time_type%10],specialDayWord[19].height,specialDayWord[19].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(specialDayWord[20].X,specialDayWord[20].Y,word8x16[Tmp_specialDayRev[page*3-1]._start_year/1000],specialDayWord[20].height,specialDayWord[20].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(specialDayWord[21].X,specialDayWord[21].Y,word8x16[(Tmp_specialDayRev[page*3-1]._start_year/100)%10],specialDayWord[21].height,specialDayWord[21].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(specialDayWord[22].X,specialDayWord[22].Y,word8x16[(Tmp_specialDayRev[page*3-1]._start_year/10)%10],specialDayWord[22].height,specialDayWord[22].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(specialDayWord[23].X,specialDayWord[23].Y,word8x16[Tmp_specialDayRev[page*3-1]._start_year%10],specialDayWord[23].height,specialDayWord[23].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(specialDayWord[24].X,specialDayWord[24].Y,word8x16[Tmp_specialDayRev[page*3-1]._start_month/10],specialDayWord[24].height,specialDayWord[24].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(specialDayWord[25].X,specialDayWord[25].Y,word8x16[Tmp_specialDayRev[page*3-1]._start_month%10],specialDayWord[25].height,specialDayWord[25].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(specialDayWord[26].X,specialDayWord[26].Y,word8x16[Tmp_specialDayRev[page*3-1]._start_day/10],specialDayWord[26].height,specialDayWord[26].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(specialDayWord[27].X,specialDayWord[27].Y,word8x16[Tmp_specialDayRev[page*3-1]._start_day%10],specialDayWord[27].height,specialDayWord[27].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(specialDayWord[28].X,specialDayWord[28].Y,word8x16[Tmp_specialDayRev[page*3-1]._reverse_time_type/10],specialDayWord[28].height,specialDayWord[28].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(specialDayWord[29].X,specialDayWord[29].Y,word8x16[Tmp_specialDayRev[page*3-1]._reverse_time_type%10],specialDayWord[29].height,specialDayWord[29].width/8);
    } else {
        lcd240x128.DISPLAY_GRAPHIC_XY(specialDayWord[10].X,specialDayWord[10].Y,space,specialDayWord[10].height,specialDayWord[10].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(specialDayWord[11].X,specialDayWord[11].Y,space,specialDayWord[11].height,specialDayWord[11].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(specialDayWord[12].X,specialDayWord[12].Y,space,specialDayWord[12].height,specialDayWord[12].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(specialDayWord[13].X,specialDayWord[13].Y,space,specialDayWord[13].height,specialDayWord[13].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(specialDayWord[14].X,specialDayWord[14].Y,space,specialDayWord[14].height,specialDayWord[14].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(specialDayWord[15].X,specialDayWord[15].Y,space,specialDayWord[15].height,specialDayWord[15].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(specialDayWord[16].X,specialDayWord[16].Y,space,specialDayWord[16].height,specialDayWord[16].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(specialDayWord[17].X,specialDayWord[17].Y,space,specialDayWord[17].height,specialDayWord[17].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(specialDayWord[18].X,specialDayWord[18].Y,space,specialDayWord[18].height,specialDayWord[18].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(specialDayWord[19].X,specialDayWord[19].Y,space,specialDayWord[19].height,specialDayWord[19].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(specialDayWord[20].X,specialDayWord[20].Y,space,specialDayWord[20].height,specialDayWord[20].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(specialDayWord[21].X,specialDayWord[21].Y,space,specialDayWord[21].height,specialDayWord[21].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(specialDayWord[22].X,specialDayWord[22].Y,space,specialDayWord[22].height,specialDayWord[22].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(specialDayWord[23].X,specialDayWord[23].Y,space,specialDayWord[23].height,specialDayWord[23].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(specialDayWord[24].X,specialDayWord[24].Y,space,specialDayWord[24].height,specialDayWord[24].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(specialDayWord[25].X,specialDayWord[25].Y,space,specialDayWord[25].height,specialDayWord[25].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(specialDayWord[26].X,specialDayWord[26].Y,space,specialDayWord[26].height,specialDayWord[26].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(specialDayWord[27].X,specialDayWord[27].Y,space,specialDayWord[27].height,specialDayWord[27].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(specialDayWord[28].X,specialDayWord[28].Y,space,specialDayWord[28].height,specialDayWord[28].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(specialDayWord[29].X,specialDayWord[29].Y,space,specialDayWord[29].height,specialDayWord[29].width/8);
    }

    DisplayUpAndPage(page);  DisplayUpAndPage(page);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenRevSpecialDay::SaveSpecialDayRev(void)
{
try {
  unsigned short cYear=0,cMonth=0,cDay=0;
  time_t currentTime=time(NULL);
  struct tm *now=localtime(&currentTime);
  cYear=now->tm_year+1900;
  cMonth=now->tm_mon+1;
  cDay=now->tm_mday;
  printf("Save Special Day!\n");
  for (int i=4;i<=16;i++) {
    printf("Tmp_specialDayRev[%d-4]._reverse_time_type: %d", i, Tmp_specialDayRev[i-4]._reverse_time_type);
    //--------------------------------------------------------------------------
    if( (Tmp_specialDayRev[i-4]._reverse_time_type != specialDayRev[i-4]._reverse_time_type ) )
    {
      stc._panel_holidayrev._reverse_time_type=Tmp_specialDayRev[i-4]._reverse_time_type;
    }
    else
    {
      stc._panel_holidayrev._reverse_time_type=specialDayRev[i-4]._reverse_time_type;
    }
    printf("stc._panel_holidayrev._reverse_time_type: %d", stc._panel_holidayrev._reverse_time_type);
    //--------------------------------------------------------------------------
    if( (Tmp_specialDayRev[i-4]._start_year != specialDayRev[i-4]._start_year) &&
    Tmp_specialDayRev[i-4]._start_year >= cYear )
    {
      stc._panel_holidayrev._start_year = Tmp_specialDayRev[i-4]._start_year;
      stc._panel_holidayrev._end_year = Tmp_specialDayRev[i-4]._start_year;
    }
    else
    {
      stc._panel_holidayrev._start_year = specialDayRev[i-4]._start_year;
      stc._panel_holidayrev._end_year = specialDayRev[i-4]._start_year;
    }
    //---------------------------------------------------------------------
    if( (Tmp_specialDayRev[i-4]._start_month != specialDayRev[i-4]._start_month))
    {
      stc._panel_holidayrev._start_month = Tmp_specialDayRev[i-4]._start_month;
      stc._panel_holidayrev._end_month = Tmp_specialDayRev[i-4]._start_month;
    }
    else
    {
      stc._panel_holidayrev._start_month = specialDayRev[i-4]._start_month;
      stc._panel_holidayrev._end_month = specialDayRev[i-4]._start_month;
    }
    //---------------------------------------------------------------------
    if( (Tmp_specialDayRev[i-4]._start_day != specialDayRev[i-4]._start_day))
    {
      stc._panel_holidayrev._start_day = Tmp_specialDayRev[i-4]._start_day;
      stc._panel_holidayrev._end_day = Tmp_specialDayRev[i-4]._start_day;
    }
    else
    {
      stc._panel_holidayrev._start_day = specialDayRev[i-4]._start_day;
      stc._panel_holidayrev._end_day = specialDayRev[i-4]._start_day;
    }
    //--------------------------------------------------------------------
//    stc._panel_holidayrev._end_year=specialDayRev[i-4]._end_year;
//    stc._panel_holidayrev._end_month=specialDayRev[i-4]._end_month;
//    stc._panel_holidayrev._end_day=specialDayRev[i-4]._end_day;

    stc.Lock_to_Save_HoliDayReverseTime_Step1(1);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenRevSpecialDay::DisplayItem(int page)
{
try {
    BYTE space[16]={0};
    for (int i=0;i<6;i+=2) {
         if ((page*3-(2-i/2))<=13) {
             lcd240x128.DISPLAY_GRAPHIC_XY(itemWord[i].X,itemWord[i].Y,word8x16[(page*3-(2-i/2))/10],itemWord[i].height,itemWord[i].width/8);
             lcd240x128.DISPLAY_GRAPHIC_XY(itemWord[i+1].X,itemWord[i+1].Y,word8x16[(page*3-(2-i/2))%10],itemWord[i+1].height,itemWord[i+1].width/8);
         } else {
             lcd240x128.DISPLAY_GRAPHIC_XY(itemWord[i].X,itemWord[i].Y,space,itemWord[i].height,itemWord[i].width/8);
             lcd240x128.DISPLAY_GRAPHIC_XY(itemWord[i+1].X,itemWord[i+1].Y,space,itemWord[i+1].height,itemWord[i+1].width/8);
         }
    }
  } catch(...) {}
}
//---------------------------------------------------------------------------
void ScreenRevSpecialDay::DisplayUpAndPage(int page)
{
try {
    unsigned char temp[16]={0};
    if (page==1) {                          //�Ĥ@��
        lcd240x128.DISPLAY_GRAPHIC_XY(216,40,temp,16,1);
        lcd240x128.DISPLAY_GRAPHIC_XY(216,74,arrow8x16[1],16,1);
    } else if (page==5) {                   //�̫�@��
        lcd240x128.DISPLAY_GRAPHIC_XY(216,40,arrow8x16[0],16,1);
        lcd240x128.DISPLAY_GRAPHIC_XY(216,74,temp,16,1);
    } else {                                //������
        lcd240x128.DISPLAY_GRAPHIC_XY(216,40,arrow8x16[0],16,1);
        lcd240x128.DISPLAY_GRAPHIC_XY(216,74,arrow8x16[1],16,1);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenRevSpecialDay::doKeyWork(BYTE key)
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
void ScreenRevSpecialDay::doKey0Work(void)
{
  lcd240x128.DISPLAY_GRAPHIC_XY(specialDayWord[cPosition].X,specialDayWord[cPosition].Y,word8x16[0],specialDayWord[cPosition].height,specialDayWord[cPosition].width/8);
  ModifyRev(0);
  doKeyRIGHTWork();
}
//---------------------------------------------------------------------------
void ScreenRevSpecialDay::doKey1Work(void)
{
  lcd240x128.DISPLAY_GRAPHIC_XY(specialDayWord[cPosition].X,specialDayWord[cPosition].Y,word8x16[1],specialDayWord[cPosition].height,specialDayWord[cPosition].width/8);
  ModifyRev(1);
  doKeyRIGHTWork();
}
//---------------------------------------------------------------------------
void ScreenRevSpecialDay::doKey2Work(void)
{
  lcd240x128.DISPLAY_GRAPHIC_XY(specialDayWord[cPosition].X,specialDayWord[cPosition].Y,word8x16[2],specialDayWord[cPosition].height,specialDayWord[cPosition].width/8);
  ModifyRev(2);
  doKeyRIGHTWork();
}
//---------------------------------------------------------------------------
void ScreenRevSpecialDay::doKey3Work(void)
{
  lcd240x128.DISPLAY_GRAPHIC_XY(specialDayWord[cPosition].X,specialDayWord[cPosition].Y,word8x16[3],specialDayWord[cPosition].height,specialDayWord[cPosition].width/8);
  ModifyRev(3);
  doKeyRIGHTWork();
}
//---------------------------------------------------------------------------
void ScreenRevSpecialDay::doKey4Work(void)
{
  lcd240x128.DISPLAY_GRAPHIC_XY(specialDayWord[cPosition].X,specialDayWord[cPosition].Y,word8x16[4],specialDayWord[cPosition].height,specialDayWord[cPosition].width/8);
  ModifyRev(4);
  doKeyRIGHTWork();
}
//---------------------------------------------------------------------------
void ScreenRevSpecialDay::doKey5Work(void)
{
  lcd240x128.DISPLAY_GRAPHIC_XY(specialDayWord[cPosition].X,specialDayWord[cPosition].Y,word8x16[5],specialDayWord[cPosition].height,specialDayWord[cPosition].width/8);
  ModifyRev(5);
  doKeyRIGHTWork();
}
//---------------------------------------------------------------------------
void ScreenRevSpecialDay::doKey6Work(void)
{
  lcd240x128.DISPLAY_GRAPHIC_XY(specialDayWord[cPosition].X,specialDayWord[cPosition].Y,word8x16[6],specialDayWord[cPosition].height,specialDayWord[cPosition].width/8);
  ModifyRev(6);
  doKeyRIGHTWork();
}
//---------------------------------------------------------------------------
void ScreenRevSpecialDay::doKey7Work(void)
{
  lcd240x128.DISPLAY_GRAPHIC_XY(specialDayWord[cPosition].X,specialDayWord[cPosition].Y,word8x16[7],specialDayWord[cPosition].height,specialDayWord[cPosition].width/8);
  ModifyRev(7);
  doKeyRIGHTWork();
}
//---------------------------------------------------------------------------
void ScreenRevSpecialDay::doKey8Work(void)
{
  lcd240x128.DISPLAY_GRAPHIC_XY(specialDayWord[cPosition].X,specialDayWord[cPosition].Y,word8x16[8],specialDayWord[cPosition].height,specialDayWord[cPosition].width/8);
  ModifyRev(8);
  doKeyRIGHTWork();
}
//---------------------------------------------------------------------------
void ScreenRevSpecialDay::doKey9Work(void)
{
  lcd240x128.DISPLAY_GRAPHIC_XY(specialDayWord[cPosition].X,specialDayWord[cPosition].Y,word8x16[9],specialDayWord[cPosition].height,specialDayWord[cPosition].width/8);
  ModifyRev(9);
  doKeyRIGHTWork();
}
//---------------------------------------------------------------------------
void ScreenRevSpecialDay::doKeyAWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenRevSpecialDay::doKeyBWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenRevSpecialDay::doKeyCWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenRevSpecialDay::doKeyDWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenRevSpecialDay::doKeyEWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenRevSpecialDay::doKeyFWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenRevSpecialDay::doKeyF1Work(void)
{
    screenMain.DisplayMain();
}
//---------------------------------------------------------------------------
void ScreenRevSpecialDay::doKeyF2Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenRevSpecialDay::doKeyF3Work(void)
{
    SaveSpecialDayRev();
}
//---------------------------------------------------------------------------
void ScreenRevSpecialDay::doKeyF4Work(void)
{
    screenReverseMenu.DisplayReverseMenu();
}
//---------------------------------------------------------------------------
void ScreenRevSpecialDay::doKeyUPWork(void)
{
try {
    if (cSelect==0) {
    } else if (cSelect>=1) {
               clearCursor8x16(specialDayWord[cPosition].X,specialDayWord[cPosition].Y+16);
               if (cPosition>=10 && cPosition<=29) cPosition-=10;
               else {
                   if (cFace>1) {
                       DisplayDate();
                       DisplaySpecialDayRev(cFace-1);
                       cPosition=0;
                       cSelect=1;
                   }
               }
               setCursor8x16(specialDayWord[cPosition].X,specialDayWord[cPosition].Y+16);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenRevSpecialDay::doKeyDOWNWork(void)
{
try {
    if (cSelect==0) {
    } else if (cSelect>=1) {
               clearCursor8x16(specialDayWord[cPosition].X,specialDayWord[cPosition].Y+16);
               if (cPosition>=0 && cPosition<=19) cPosition+=10;
               else {
                   if (cFace<5) {
                       DisplayDate();
                       DisplaySpecialDayRev(cFace+1);
                       cPosition=0;
                       cSelect=1;
                   }
               }
               setCursor8x16(specialDayWord[cPosition].X,specialDayWord[cPosition].Y+16);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenRevSpecialDay::doKeyLEFTWork(void)
{
try {
    if (cSelect==0) {
    } else if (cSelect==1) {
              clearCursor8x16(specialDayWord[cPosition].X,specialDayWord[cPosition].Y+16);
              if (cPosition==0) cPosition=1;
              cPosition--;
              if (cPosition >= 8  && cPosition <=9 ) cSelect =4;
              if( cPosition >= 18 && cPosition <=19) cSelect =4;
              setCursor8x16(specialDayWord[cPosition].X,specialDayWord[cPosition].Y+16);
    } else if (cSelect==2) {
              clearCursor8x16(specialDayWord[cPosition].X,specialDayWord[cPosition].Y+16);
              //if (cPosition==0) cPosition=1;
              cPosition--;
              if (cPosition >= 0  && cPosition <= 3 )   cSelect =1;
              if (cPosition >= 10 && cPosition <= 13 ) cSelect =1;
              if( cPosition >= 20 && cPosition <= 23)  cSelect =1;
              setCursor8x16(specialDayWord[cPosition].X,specialDayWord[cPosition].Y+16);
    } else if (cSelect ==3){
              clearCursor8x16(specialDayWord[cPosition].X,specialDayWord[cPosition].Y+16);
              //if (cPosition==0) cPosition=1;
              cPosition--;
              if (cPosition >= 4  && cPosition <= 5 )   cSelect =2;
              if (cPosition >= 14 && cPosition <= 15 ) cSelect =2;
              if( cPosition >= 24 && cPosition <= 25)  cSelect =2;
              setCursor8x16(specialDayWord[cPosition].X,specialDayWord[cPosition].Y+16);
    } else if (cSelect ==4){
              clearCursor8x16(specialDayWord[cPosition].X,specialDayWord[cPosition].Y+16);
              //if (cPosition==0) cPosition=1;
              cPosition--;
              if (cPosition >= 6  && cPosition <=7 ) cSelect =3;
              if( cPosition >= 16 && cPosition <=17) cSelect =3;
              if (cPosition >= 26 && cPosition <=27) cSelect =3;
              setCursor8x16(specialDayWord[cPosition].X,specialDayWord[cPosition].Y+16);
    }
  } catch(...) {}
}
//---------------------------------------------------------------------------
void ScreenRevSpecialDay::doKeyRIGHTWork(void)
{
try {
    if (cSelect==0) {
    } else if (cSelect==1) {
                clearCursor8x16(specialDayWord[cPosition].X,specialDayWord[cPosition].Y+16);
                cPosition++;
                if (cPosition>=4  && cPosition <= 5 )   cSelect = 2;
                if (cPosition>=14 && cPosition <= 15) cSelect = 2;
                if (cPosition>=24 && cPosition <= 25) cSelect = 2;
                //if (cPosition>=30) cPosition=29;
                setCursor8x16(specialDayWord[cPosition].X,specialDayWord[cPosition].Y+16);
    } else if (cSelect==2) {
                clearCursor8x16(specialDayWord[cPosition].X,specialDayWord[cPosition].Y+16);
                cPosition++;
                if (cPosition >= 6  && cPosition <=7 ) cSelect =3;
                if( cPosition >= 16 && cPosition <=17) cSelect =3;
                if (cPosition >= 26 && cPosition <=27) cSelect =3;
                setCursor8x16(specialDayWord[cPosition].X,specialDayWord[cPosition].Y+16);
    } else if (cSelect==3) {
                clearCursor8x16(specialDayWord[cPosition].X,specialDayWord[cPosition].Y+16);
                cPosition++;
                if (cPosition >= 8  && cPosition <=9 ) cSelect =4;
                if( cPosition >= 18 && cPosition <=19) cSelect =4;
                if (cPosition >= 28 && cPosition <=29) cSelect =4;
                setCursor8x16(specialDayWord[cPosition].X,specialDayWord[cPosition].Y+16);
    } else if (cSelect==4) {
                clearCursor8x16(specialDayWord[cPosition].X,specialDayWord[cPosition].Y+16);
                cPosition++;
                if (cPosition >= 10 && cPosition <= 13 ) cSelect =1;
                if( cPosition >= 20 && cPosition <= 23) cSelect =1;
                if (cPosition >= 30) cPosition = 29;
                setCursor8x16(specialDayWord[cPosition].X,specialDayWord[cPosition].Y+16);
    }
  } catch(...) {}
}
//---------------------------------------------------------------------------
void ScreenRevSpecialDay::doKeyEnterWork(void)
{
try {
    int x=-1;
//    if (cSelect==1) {
        if (cPosition>=0 && cPosition<=9) x=specialDayRev[0+(cFace-1)*3]._reverse_time_type;
        else if (cPosition>=10 && cPosition<=19 && cFace<5) x=specialDayRev[1+(cFace-1)*3]._reverse_time_type;
        else if (cPosition>=20 && cPosition<=29 && cFace<5) x=specialDayRev[2+(cFace-1)*3]._reverse_time_type;

        if (x>=0 && x<=16) screenReverse.DisplayReverse(x);
//    }
  } catch (...){}
}
//---------------------------------------------------------------------------
void ScreenRevSpecialDay::ModifyRev(int tmp_value)
{
try{
  printf("tmp_value:%d cPosition:%d\n", tmp_value, cPosition);
  int tmp = 0;
  int tmp_seg = 0;
  if( cPosition >=0  && cPosition <=9 ) tmp_seg = 3;
  if( cPosition >=10 && cPosition <=19) tmp_seg = 2;
  if( cPosition >=20 && cPosition <=29) tmp_seg = 1;
  switch(cPosition)
  {
    case(0): case(10): case(20):
      tmp = tmp_value * 1000;
      tmp += ((Tmp_specialDayRev[cFace*3 - tmp_seg]._start_year/100)%10)*100;
      tmp += ((Tmp_specialDayRev[cFace*3 - tmp_seg]._start_year/10)%10)*10;
      tmp += Tmp_specialDayRev[cFace*3 - tmp_seg]._start_year%10;
      Tmp_specialDayRev[cFace*3 - tmp_seg]._start_year = tmp;
      break;
    case(1): case(11): case(21):
      tmp = (Tmp_specialDayRev[cFace*3 - tmp_seg]._start_year/1000)*1000;
      tmp += tmp_value * 100;
      tmp += ((Tmp_specialDayRev[cFace*3 - tmp_seg]._start_year/10)%10)*10;
      tmp += Tmp_specialDayRev[cFace*3 - tmp_seg]._start_year%10;
      Tmp_specialDayRev[cFace*3 - tmp_seg]._start_year = tmp;
      break;
    case(2): case(12): case(22):
      tmp = (Tmp_specialDayRev[cFace*3 - tmp_seg]._start_year/1000)*1000;
      tmp += ((Tmp_specialDayRev[cFace*3 - tmp_seg]._start_year/100)%10)*100;
      tmp += tmp_value * 10;
      tmp += Tmp_specialDayRev[cFace*3 - tmp_seg]._start_year%10;
      Tmp_specialDayRev[cFace*3 - tmp_seg]._start_year = tmp;
      break;
    case(3): case(13): case(23):
      tmp = (Tmp_specialDayRev[cFace*3 - tmp_seg]._start_year/1000)*1000;
      tmp += ((Tmp_specialDayRev[cFace*3 - tmp_seg]._start_year/100)%10)*100;
      tmp += ((Tmp_specialDayRev[cFace*3 - tmp_seg]._start_year/10)%10)*10;
      tmp += tmp_value;
      Tmp_specialDayRev[cFace*3 - tmp_seg]._start_year = tmp;
      break;
    case(4): case(14): case(24):
      tmp = tmp_value*10;
      tmp += Tmp_specialDayRev[cFace*3 - tmp_seg]._start_month%10;
      if( tmp_value <= 1)
        Tmp_specialDayRev[cFace*3 - tmp_seg]._start_month = tmp;
      else
      {
        Tmp_specialDayRev[cFace*3 - tmp_seg]._start_month = specialDayRev[cFace*3 - tmp_seg]._start_month;
        clearCursor8x16(specialDayWord[cPosition].X,specialDayWord[cPosition].Y+16);
        lcd240x128.DISPLAY_GRAPHIC_XY(specialDayWord[cPosition].X,specialDayWord[cPosition].Y,word8x16[Tmp_specialDayRev[cFace*3 - tmp_seg]._start_month/10],specialDayWord[cPosition].height,specialDayWord[cPosition].width/8);
        cPosition--;
      }
      break;
    case(5): case(15): case(25):
      tmp = (Tmp_specialDayRev[cFace*3 - tmp_seg]._start_month/10)*10;
      tmp += tmp_value;
      if( tmp <= 12)
        Tmp_specialDayRev[cFace*3 - tmp_seg]._start_month = tmp;
      else
      {
        Tmp_specialDayRev[cFace*3 - tmp_seg]._start_month = specialDayRev[cFace*3 - tmp_seg]._start_month;
        clearCursor8x16(specialDayWord[cPosition].X,specialDayWord[cPosition].Y+16);
        lcd240x128.DISPLAY_GRAPHIC_XY(specialDayWord[cPosition-1].X,specialDayWord[cPosition-1].Y,word8x16[Tmp_specialDayRev[cFace*3 - tmp_seg]._start_month/10],specialDayWord[cPosition-1].height,specialDayWord[cPosition-1].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(specialDayWord[cPosition].X,specialDayWord[cPosition].Y,word8x16[Tmp_specialDayRev[cFace*3 - tmp_seg]._start_month%10],specialDayWord[cPosition].height,specialDayWord[cPosition].width/8);
        cPosition--;
        cPosition--;
      }
      break;
    case(6): case(16): case(26):
      tmp = tmp_value*10;
      tmp += Tmp_specialDayRev[cFace*3 - tmp_seg]._start_day%10;
      if (tmp_value <=3)
        Tmp_specialDayRev[cFace*3 - tmp_seg]._start_day = tmp;
      else
      {
        Tmp_specialDayRev[cFace*3 - tmp_seg]._start_day = specialDayRev[cFace*3 - tmp_seg]._start_day;
        clearCursor8x16(specialDayWord[cPosition].X,specialDayWord[cPosition].Y+16);
        lcd240x128.DISPLAY_GRAPHIC_XY(specialDayWord[cPosition].X,specialDayWord[cPosition].Y,word8x16[Tmp_specialDayRev[cFace*3 - tmp_seg]._start_day/10],specialDayWord[cPosition].height,specialDayWord[cPosition].width/8);
        cPosition--;
      }
      break;
    case(7): case(17): case(27):
      tmp = (Tmp_specialDayRev[cFace*3 - tmp_seg]._start_day/10)*10;
      tmp += tmp_value;
      if( tmp <=31 && (Tmp_specialDayRev[cFace*3 - tmp_seg]._start_month != 2))
        Tmp_specialDayRev[cFace*3 - tmp_seg]._start_day = tmp;
      else if(Tmp_specialDayRev[cFace*3 - tmp_seg]._start_month == 2 &&
      (tmp <=29) &&
      ((Tmp_specialDayRev[cFace*3 - tmp_seg]._start_year%4) == 0))
      {
        Tmp_specialDayRev[cFace*3 - tmp_seg]._start_day = tmp;
      }
      else if(Tmp_specialDayRev[cFace*3 - tmp_seg]._start_month == 2 &&
      (tmp <=28) &&
      ((Tmp_specialDayRev[cFace*3 - tmp_seg]._start_year%4) != 0))
      {
        Tmp_specialDayRev[cFace*3 - tmp_seg]._start_day = tmp;
      }
      else
      {
        Tmp_specialDayRev[cFace*3 - tmp_seg]._start_day = specialDayRev[cFace*3 - tmp_seg]._start_day;
        clearCursor8x16(specialDayWord[cPosition].X,specialDayWord[cPosition].Y+16);
        lcd240x128.DISPLAY_GRAPHIC_XY(specialDayWord[cPosition-1].X,specialDayWord[cPosition-1].Y,word8x16[Tmp_specialDayRev[cFace*3 - tmp_seg]._start_day/10],specialDayWord[cPosition-1].height,specialDayWord[cPosition-1].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(specialDayWord[cPosition].X,specialDayWord[cPosition].Y,word8x16[Tmp_specialDayRev[cFace*3 - tmp_seg]._start_day%10],specialDayWord[cPosition].height,specialDayWord[cPosition].width/8);
        cPosition--;
        cPosition--;
      }
      break;
    case(8): case(18): case(28):
      tmp = tmp_value*10;
      tmp += Tmp_specialDayRev[cFace*3 - tmp_seg]._reverse_time_type%10;
      Tmp_specialDayRev[cFace*3 - tmp_seg]._reverse_time_type = tmp;
      printf("Tmp_specialDayRev._reverse_time_type:%d\n", tmp);
      break;
    case(9): case(19): case(29):
      tmp = (Tmp_specialDayRev[cFace*3 - tmp_seg]._reverse_time_type/10)*10;
      tmp += tmp_value;
      Tmp_specialDayRev[cFace*3 - tmp_seg]._reverse_time_type = tmp;
      printf("Tmp_specialDayRev._reverse_time_type:%d\n", Tmp_specialDayRev[cFace*3 - tmp_seg]._reverse_time_type);
      break;
  }
 }catch(...){}
}
