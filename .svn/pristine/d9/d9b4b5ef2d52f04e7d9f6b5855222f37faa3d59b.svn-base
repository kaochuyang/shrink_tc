#include "screenSegtypePreview.h"
#include "screenSegtype.h"
#include "screenWeekDayEditF2.h"

#include "screenWeekDay.h"
#include "screenSpecialDay.h"

#include "LCD240x128.h"
#include "SCREENMain.h"
#include "SMEM.h"

#include "screenPlan.h"  //OT Debug 0523

#include "CSTC.h"
#include <stdio.h>
//---------------------------------------------------------------------------
ScreenSegtypePreview screenSegtypePreview;
//---------------------------------------------------------------------------
ScreenSegtypePreview::ScreenSegtypePreview(void)
{
    loadBitmapFromFile();
    initDispWord();
    cPosition=0;
    cSelect=2;
    cFace=1;
}
//---------------------------------------------------------------------------
ScreenSegtypePreview::~ScreenSegtypePreview(void)
{
}
//---------------------------------------------------------------------------
void ScreenSegtypePreview::loadBitmapFromFile(void)
{
try {
    FILE *bitmap;
    bitmap=fopen("//cct//bitmap//backGround//TC5F//segtypePreview.bit","rb");
    if (bitmap) {
        fread(segtypeBitmap,3840,1,bitmap);
        fclose(bitmap);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenSegtypePreview::DisplaySegtype(int segtype, int iPageIn)
{
try {
    //OTADD
    smem.SetcFace(cSEGTYPEPREVIEW);
    lcd240x128.DISPLAY_GRAPHIC(0,segtypeBitmap,128,30);                         //畫底圖
    DisplayDate();                                              //畫日期
    DisplayTime();                                              //畫時間

    iSegtypePreviewReturn = segtype;

    LoadSegtype(segtype);                                                       //得到現行時間表
    int count=GetNowPlanOfSegtypeCount();                                       //得到現在第幾段

    printf("count= %d\n", count);

    cFace=((count-1)/6) +1;     //OT Debug 0523             from 1~6           //該點第幾頁

    printf("cface= %d\n", cFace);

    DisplaySegtypeTable(cFace);                                                 //點該頁

    int temp=(count-1)%6;                                                           //判斷此段是此頁第幾筆

    if (temp==0)  cPosition=4;   //OT Debug 0523
    else if (temp==3) cPosition=11;
    else if (temp==1) cPosition=18;
    else if (temp==4) cPosition=25;
    else if (temp==2) cPosition=32;
    else if (temp==5) cPosition=39;

    cSelect=2;                                                                  //現在在時間表欄位
    setCursor8x16(segmentWord[cPosition].X,segmentWord[cPosition].Y+16);        //畫Cursor

    DisplaySegmnetNo();                                    //畫時間表編號
    DisplaySegmnetCount();                              //畫時段數

    if(iPageIn != INVAILDVALUE)
      iLastPage = iPageIn;

  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenSegtypePreview::LoadSegtype(int segtype)
{
try {
    if (stc.Lock_to_Load_Segment_for_Panel(segtype)) {
        segment._segment_type=stc._panel_segment._segment_type;
        segment._segment_count=stc._panel_segment._segment_count;
        for (int i=0;i<32;i++) {                        //先清空
             segment._ptr_seg_exec_time[i]._hour=0;
             segment._ptr_seg_exec_time[i]._minute=0;
             segment._ptr_seg_exec_time[i]._planid=0;
        }
        for (int i=0;i<segment._segment_count;i++) {    //再重新寫
             segment._ptr_seg_exec_time[i]._hour=stc._panel_segment._ptr_seg_exec_time[i]._hour;
             segment._ptr_seg_exec_time[i]._minute=stc._panel_segment._ptr_seg_exec_time[i]._minute;
             segment._ptr_seg_exec_time[i]._planid=stc._panel_segment._ptr_seg_exec_time[i]._planid;
        }
    } else {
        segment._segment_type=segtype;
        segment._segment_count=32;
        for (int i=0;i<segment._segment_count;i++) {
             segment._ptr_seg_exec_time[i]._hour=0;
             segment._ptr_seg_exec_time[i]._minute=0;
             segment._ptr_seg_exec_time[i]._planid=0;
        }
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
int ScreenSegtypePreview::GetNowPlanOfSegtypeCount(void)
{
try {
    int count=1;
    unsigned short cHour=0,cMin=0;
    time_t currentTime=time(NULL);
    struct tm *now=localtime(&currentTime);
    cHour=now->tm_hour;
    cMin=now->tm_min;
    //以上得到現在時間

    for (int i=0;i<segment._segment_count;i++) {                    //以現有時段一一比較
         if (cHour>=segment._ptr_seg_exec_time[i]._hour) {           //時大於
             count=i+1;                                             //指定時段
         } else {                                                   //如果沒大於
              if (cHour==segment._ptr_seg_exec_time[i]._hour)       //判斷是否等於
              //OT Debug 0523
              if (cMin>=segment._ptr_seg_exec_time[i]._minute)       //判斷分鐘數大於
                  count=i+1;                                        //指定時段
         }
    }

    if (count>32) count=32;

    return count;
  } catch (...){}
}
//---------------------------------------------------------------------------
void ScreenSegtypePreview::DisplaySegtypeTable(int page)
{
try {

    unsigned short int usi1, usi2, usi3;

    BYTE space[16]={0};
    cFace=page;
    DisplayItem(page);

    for (int loop=0;loop<2;loop++) {
        lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[0].X,segmentWord[0].Y,word8x16[segment._ptr_seg_exec_time[page*6-6]._hour/10],segmentWord[0].height,segmentWord[0].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[1].X,segmentWord[1].Y,word8x16[segment._ptr_seg_exec_time[page*6-6]._hour%10],segmentWord[1].height,segmentWord[1].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[2].X,segmentWord[2].Y,word8x16[segment._ptr_seg_exec_time[page*6-6]._minute/10],segmentWord[2].height,segmentWord[2].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[3].X,segmentWord[3].Y,word8x16[segment._ptr_seg_exec_time[page*6-6]._minute%10],segmentWord[3].height,segmentWord[3].width/8);

        usi1  = segment._ptr_seg_exec_time[page*6-6]._planid / 100;
        usi2 = (segment._ptr_seg_exec_time[page*6-6]._planid - usi1*100 ) / 10;
        usi3 = (segment._ptr_seg_exec_time[page*6-6]._planid - usi1*100 - usi2*10) % 10;
        lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[4].X,segmentWord[4].Y,word8x16[usi1],segmentWord[4].height,segmentWord[4].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[5].X,segmentWord[5].Y,word8x16[usi2],segmentWord[5].height,segmentWord[5].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[6].X,segmentWord[6].Y,word8x16[usi3],segmentWord[6].height,segmentWord[6].width/8);

        if (page<6) {
            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[7].X,segmentWord[7].Y,word8x16[segment._ptr_seg_exec_time[page*6-3]._hour/10],segmentWord[7].height,segmentWord[7].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[8].X,segmentWord[8].Y,word8x16[segment._ptr_seg_exec_time[page*6-3]._hour%10],segmentWord[8].height,segmentWord[8].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[9].X,segmentWord[9].Y,word8x16[segment._ptr_seg_exec_time[page*6-3]._minute/10],segmentWord[9].height,segmentWord[9].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[10].X,segmentWord[10].Y,word8x16[segment._ptr_seg_exec_time[page*6-3]._minute%10],segmentWord[10].height,segmentWord[10].width/8);

            usi1  = segment._ptr_seg_exec_time[page*6-3]._planid / 100;
            usi2 = (segment._ptr_seg_exec_time[page*6-3]._planid - usi1*100 ) / 10;
            usi3 = (segment._ptr_seg_exec_time[page*6-3]._planid - usi1*100 - usi2*10) % 10;
            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[11].X,segmentWord[11].Y,word8x16[usi1],segmentWord[11].height,segmentWord[11].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[12].X,segmentWord[12].Y,word8x16[usi2],segmentWord[12].height,segmentWord[12].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[13].X,segmentWord[13].Y,word8x16[usi3],segmentWord[13].height,segmentWord[13].width/8);

        } else {
            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[7].X,segmentWord[7].Y,space,segmentWord[7].height,segmentWord[7].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[8].X,segmentWord[8].Y,space,segmentWord[8].height,segmentWord[8].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[9].X,segmentWord[9].Y,space,segmentWord[9].height,segmentWord[9].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[10].X,segmentWord[10].Y,space,segmentWord[10].height,segmentWord[10].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[11].X,segmentWord[11].Y,space,segmentWord[11].height,segmentWord[11].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[12].X,segmentWord[12].Y,space,segmentWord[12].height,segmentWord[12].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[13].X,segmentWord[13].Y,space,segmentWord[13].height,segmentWord[13].width/8);
        }
        lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[14].X,segmentWord[14].Y,word8x16[segment._ptr_seg_exec_time[page*6-5]._hour/10],segmentWord[14].height,segmentWord[14].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[15].X,segmentWord[15].Y,word8x16[segment._ptr_seg_exec_time[page*6-5]._hour%10],segmentWord[15].height,segmentWord[15].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[16].X,segmentWord[16].Y,word8x16[segment._ptr_seg_exec_time[page*6-5]._minute/10],segmentWord[16].height,segmentWord[16].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[17].X,segmentWord[17].Y,word8x16[segment._ptr_seg_exec_time[page*6-5]._minute%10],segmentWord[17].height,segmentWord[17].width/8);

        usi1  = segment._ptr_seg_exec_time[page*6-5]._planid / 100;
        usi2 = (segment._ptr_seg_exec_time[page*6-5]._planid - usi1*100 ) / 10;
        usi3 = (segment._ptr_seg_exec_time[page*6-5]._planid - usi1*100 - usi2*10) % 10;
        lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[18].X,segmentWord[18].Y,word8x16[usi1],segmentWord[18].height,segmentWord[18].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[19].X,segmentWord[19].Y,word8x16[usi2],segmentWord[19].height,segmentWord[19].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[20].X,segmentWord[20].Y,word8x16[usi3],segmentWord[20].height,segmentWord[20].width/8);
        if (page<6) {
            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[21].X,segmentWord[21].Y,word8x16[segment._ptr_seg_exec_time[page*6-2]._hour/10],segmentWord[21].height,segmentWord[21].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[22].X,segmentWord[22].Y,word8x16[segment._ptr_seg_exec_time[page*6-2]._hour%10],segmentWord[22].height,segmentWord[22].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[23].X,segmentWord[23].Y,word8x16[segment._ptr_seg_exec_time[page*6-2]._minute/10],segmentWord[23].height,segmentWord[23].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[24].X,segmentWord[24].Y,word8x16[segment._ptr_seg_exec_time[page*6-2]._minute%10],segmentWord[24].height,segmentWord[24].width/8);

            usi1  = segment._ptr_seg_exec_time[page*6-2]._planid / 100;
            usi2 = (segment._ptr_seg_exec_time[page*6-2]._planid - usi1*100 ) / 10;
            usi3 = (segment._ptr_seg_exec_time[page*6-2]._planid - usi1*100 - usi2*10) % 10;
            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[25].X,segmentWord[25].Y,word8x16[usi1],segmentWord[25].height,segmentWord[25].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[26].X,segmentWord[26].Y,word8x16[usi2],segmentWord[26].height,segmentWord[26].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[27].X,segmentWord[27].Y,word8x16[usi3],segmentWord[27].height,segmentWord[27].width/8);

            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[28].X,segmentWord[28].Y,word8x16[segment._ptr_seg_exec_time[page*6-4]._hour/10],segmentWord[28].height,segmentWord[28].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[29].X,segmentWord[29].Y,word8x16[segment._ptr_seg_exec_time[page*6-4]._hour%10],segmentWord[29].height,segmentWord[29].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[30].X,segmentWord[30].Y,word8x16[segment._ptr_seg_exec_time[page*6-4]._minute/10],segmentWord[30].height,segmentWord[30].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[31].X,segmentWord[31].Y,word8x16[segment._ptr_seg_exec_time[page*6-4]._minute%10],segmentWord[31].height,segmentWord[31].width/8);

            usi1  = segment._ptr_seg_exec_time[page*6-4]._planid / 100;
            usi2 = (segment._ptr_seg_exec_time[page*6-4]._planid - usi1*100 ) / 10;
            usi3 = (segment._ptr_seg_exec_time[page*6-4]._planid - usi1*100 - usi2*10) % 10;
            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[32].X,segmentWord[32].Y,word8x16[usi1],segmentWord[32].height,segmentWord[32].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[33].X,segmentWord[33].Y,word8x16[usi2],segmentWord[33].height,segmentWord[33].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[34].X,segmentWord[34].Y,word8x16[usi3],segmentWord[34].height,segmentWord[34].width/8);

            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[35].X,segmentWord[35].Y,word8x16[segment._ptr_seg_exec_time[page*6-1]._hour/10],segmentWord[35].height,segmentWord[35].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[36].X,segmentWord[36].Y,word8x16[segment._ptr_seg_exec_time[page*6-1]._hour%10],segmentWord[36].height,segmentWord[36].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[37].X,segmentWord[37].Y,word8x16[segment._ptr_seg_exec_time[page*6-1]._minute/10],segmentWord[37].height,segmentWord[37].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[38].X,segmentWord[38].Y,word8x16[segment._ptr_seg_exec_time[page*6-1]._minute%10],segmentWord[38].height,segmentWord[38].width/8);

            usi1  = segment._ptr_seg_exec_time[page*6-1]._planid / 100;
            usi2 = (segment._ptr_seg_exec_time[page*6-1]._planid - usi1*100 ) / 10;
            usi3 = (segment._ptr_seg_exec_time[page*6-1]._planid - usi1*100 - usi2*10) % 10;
            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[39].X,segmentWord[39].Y,word8x16[usi1],segmentWord[39].height,segmentWord[39].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[40].X,segmentWord[40].Y,word8x16[usi2],segmentWord[40].height,segmentWord[40].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[41].X,segmentWord[41].Y,word8x16[usi3],segmentWord[41].height,segmentWord[41].width/8);
        } else {
            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[18].X,segmentWord[18].Y,space,segmentWord[18].height,segmentWord[18].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[19].X,segmentWord[19].Y,space,segmentWord[19].height,segmentWord[19].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[20].X,segmentWord[20].Y,space,segmentWord[20].height,segmentWord[20].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[21].X,segmentWord[21].Y,space,segmentWord[21].height,segmentWord[21].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[22].X,segmentWord[22].Y,space,segmentWord[22].height,segmentWord[22].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[23].X,segmentWord[23].Y,space,segmentWord[23].height,segmentWord[23].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[24].X,segmentWord[24].Y,space,segmentWord[24].height,segmentWord[24].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[25].X,segmentWord[25].Y,space,segmentWord[25].height,segmentWord[25].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[26].X,segmentWord[26].Y,space,segmentWord[26].height,segmentWord[26].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[27].X,segmentWord[27].Y,space,segmentWord[27].height,segmentWord[27].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[28].X,segmentWord[28].Y,space,segmentWord[28].height,segmentWord[28].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[29].X,segmentWord[29].Y,space,segmentWord[29].height,segmentWord[29].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[30].X,segmentWord[30].Y,space,segmentWord[30].height,segmentWord[30].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[31].X,segmentWord[31].Y,space,segmentWord[31].height,segmentWord[31].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[32].X,segmentWord[32].Y,space,segmentWord[32].height,segmentWord[32].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[33].X,segmentWord[33].Y,space,segmentWord[33].height,segmentWord[33].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[34].X,segmentWord[34].Y,space,segmentWord[34].height,segmentWord[34].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[35].X,segmentWord[35].Y,space,segmentWord[35].height,segmentWord[35].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[36].X,segmentWord[36].Y,space,segmentWord[36].height,segmentWord[36].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[37].X,segmentWord[37].Y,space,segmentWord[37].height,segmentWord[37].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[38].X,segmentWord[38].Y,space,segmentWord[38].height,segmentWord[38].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[39].X,segmentWord[39].Y,space,segmentWord[39].height,segmentWord[39].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[40].X,segmentWord[40].Y,space,segmentWord[40].height,segmentWord[40].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(segmentWord[41].X,segmentWord[41].Y,space,segmentWord[41].height,segmentWord[41].width/8);
        }
    }

    DisplayUpAndDownPage(page);  DisplayUpAndDownPage(page);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenSegtypePreview::SaveSegtype(void)
{
}
//---------------------------------------------------------------------------
void ScreenSegtypePreview::initDispWord(void)
{
try {
    for (int i=0;i<4;i++) {
         if (i>=0 && i<=1) timeWord[i].X=80+i*8;
         else if (i>=2 && i<=3) timeWord[i].X=104+(i-2)*8;
         timeWord[i].Y=2;
         timeWord[i].width=8;
         timeWord[i].height=16;
    }

    for (int i=0;i<8;i++) {
         if (i>=0 && i<=3) dateWord[i].X=128+i*8;                               //xxxx年
         else if (i>=4 && i<=5) dateWord[i].X=176+(i-4)*8;                      //xx月
         else if (i>=6 && i<=7) dateWord[i].X=208+(i-6)*8;                      //xx日
         dateWord[i].Y=2;
         dateWord[i].width=8;
         dateWord[i].height=16;
    }

    for (int i=0;i<12;i+=2) {
         if (i>=0 && i<=5) {
             itemWord[i].X=8+(i%2)*8;
             itemWord[i+1].X=8+((i%2)+1)*8;

             itemWord[i].Y=40+(i/2)*17;
             itemWord[i+1].Y=40+(i/2)*17;
         } else if (i>=6 && i<=11) {
             itemWord[i].X=120+(i%2)*8;
             itemWord[i+1].X=120+((i%2)+1)*8;

             itemWord[i].Y=40+((i-6)/2)*17;
             itemWord[i+1].Y=40+((i-6)/2)*17;
         }

         itemWord[i].width=8;
         itemWord[i].height=16;
         itemWord[i+1].width=8;
         itemWord[i+1].height=16;
    }

    for (int i=0;i<42;i++) {
         if (i>=0 && i<=1 )  segmentWord[i].X=32+i*8;
         else if (i>=2 && i<=3)  segmentWord[i].X=56+(i-2)*8;
         else if (i>=4 && i<=6)  segmentWord[i].X=80+(i-4)*8;
         else if (i>=7 && i<=8)  segmentWord[i].X=144+(i-7)*8;
         else if (i>=9 && i<=10)  segmentWord[i].X=168+(i-9)*8;
         else if (i>=11 && i<=13)  segmentWord[i].X=192+(i-11)*8;

         else if (i>=14 && i<=15)  segmentWord[i].X=32+(i-14)*8;
         else if (i>=16 && i<=17)  segmentWord[i].X=56+(i-16)*8;
         else if (i>=18 && i<=20)  segmentWord[i].X=80+(i-18)*8;
         else if (i>=21 && i<=22)  segmentWord[i].X=144+(i-21)*8;
         else if (i>=23 && i<=24)  segmentWord[i].X=168+(i-23)*8;
         else if (i>=25 && i<=27)  segmentWord[i].X=192+(i-25)*8;

         else if (i>=28 && i<=29)  segmentWord[i].X=32+(i-28)*8;
         else if (i>=30 && i<=31)  segmentWord[i].X=56+(i-30)*8;
         else if (i>=32 && i<=34)  segmentWord[i].X=80+(i-32)*8;
         else if (i>=35 && i<=36)  segmentWord[i].X=144+(i-35)*8;
         else if (i>=37 && i<=38)  segmentWord[i].X=168+(i-37)*8;
         else if (i>=39 && i<=41)  segmentWord[i].X=192+(i-39)*8;

         if (i>=0 && i<=13)  segmentWord[i].Y=40;
         else if (i>=14 && i<=27)  segmentWord[i].Y=57;
         else if (i>=28 && i<=41)  segmentWord[i].Y=74;
         segmentWord[i].width=8;
         segmentWord[i].height=16;
    }

    for (int i=0;i<2;i++) {
         segmnetNoWord[i].X=24+i*8;
         segmnetCountWord[i].X=40+i*8;
         segmnetNoWord[i].Y=97;
         segmnetCountWord[i].Y=112;
         segmnetNoWord[i].width=8;
         segmnetCountWord[i].width=8;
         segmnetNoWord[i].height=16;
         segmnetCountWord[i].height=16;
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenSegtypePreview::DisplayDate(void)
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
void ScreenSegtypePreview::DisplayTime(void)
{
try {
    unsigned short cHour=0,cMin=0;
    time_t currentTime=time(NULL);
    struct tm *now=localtime(&currentTime);
    cHour=now->tm_hour;
    cMin=now->tm_min;

    lcd240x128.DISPLAY_GRAPHIC_XY(timeWord[0].X,timeWord[0].Y,word8x16[cHour/10],timeWord[0].height,timeWord[0].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(timeWord[1].X,timeWord[1].Y,word8x16[cHour%10],timeWord[1].height,timeWord[1].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(timeWord[2].X,timeWord[2].Y,word8x16[cMin/10],timeWord[2].height,timeWord[2].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(timeWord[3].X,timeWord[3].Y,word8x16[cMin%10],timeWord[3].height,timeWord[3].width/8);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenSegtypePreview::DisplayItem(int page)
{
try {
    BYTE space[16]={0};
    for (int i=0;i<12;i+=2) {
         if ((page*6-(5-i/2))<=32) {
             lcd240x128.DISPLAY_GRAPHIC_XY(itemWord[i].X,itemWord[i].Y,word8x16[(page*6-(5-i/2))/10],itemWord[i].height,itemWord[i].width/8);
             lcd240x128.DISPLAY_GRAPHIC_XY(itemWord[i+1].X,itemWord[i+1].Y,word8x16[(page*6-(5-i/2))%10],itemWord[i+1].height,itemWord[i+1].width/8);
         } else {
             lcd240x128.DISPLAY_GRAPHIC_XY(itemWord[i].X,itemWord[i].Y,space,itemWord[i].height,itemWord[i].width/8);
             lcd240x128.DISPLAY_GRAPHIC_XY(itemWord[i+1].X,itemWord[i+1].Y,space,itemWord[i+1].height,itemWord[i+1].width/8);
         }
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenSegtypePreview::DisplayUpAndDownPage(int page)
{
try {
    unsigned char temp[16]={0};
    if (page==1) {                          //第一頁
        lcd240x128.DISPLAY_GRAPHIC_XY(224,40,temp,16,1);
        lcd240x128.DISPLAY_GRAPHIC_XY(224,74,arrow8x16[1],16,1);
    } else if (page==6) {                  //最後一頁
        lcd240x128.DISPLAY_GRAPHIC_XY(224,40,arrow8x16[0],16,1);
        lcd240x128.DISPLAY_GRAPHIC_XY(224,74,temp,16,1);
    } else {                                //中間頁
        lcd240x128.DISPLAY_GRAPHIC_XY(224,40,arrow8x16[0],16,1);
        lcd240x128.DISPLAY_GRAPHIC_XY(224,74,arrow8x16[1],16,1);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenSegtypePreview::DisplaySegmnetNo(void)
{
try {
    lcd240x128.DISPLAY_GRAPHIC_XY(segmnetNoWord[0].X,segmnetNoWord[0].Y,word8x16[segment._segment_type/10],segmnetNoWord[0].height,segmnetNoWord[0].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(segmnetNoWord[1].X,segmnetNoWord[1].Y,word8x16[segment._segment_type%10],segmnetNoWord[1].height,segmnetNoWord[1].width/8);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenSegtypePreview::DisplaySegmnetCount(void)
{
try {
    lcd240x128.DISPLAY_GRAPHIC_XY(segmnetCountWord[0].X,segmnetCountWord[0].Y,word8x16[segment._segment_count/10],segmnetCountWord[0].height,segmnetCountWord[0].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(segmnetCountWord[1].X,segmnetCountWord[1].Y,word8x16[segment._segment_count%10],segmnetCountWord[1].height,segmnetCountWord[1].width/8);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenSegtypePreview::doKeyWork(BYTE key)
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
void ScreenSegtypePreview::doKey0Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenSegtypePreview::doKey1Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenSegtypePreview::doKey2Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenSegtypePreview::doKey3Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenSegtypePreview::doKey4Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenSegtypePreview::doKey5Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenSegtypePreview::doKey6Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenSegtypePreview::doKey7Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenSegtypePreview::doKey8Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenSegtypePreview::doKey9Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenSegtypePreview::doKeyAWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenSegtypePreview::doKeyBWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenSegtypePreview::doKeyCWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenSegtypePreview::doKeyDWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenSegtypePreview::doKeyEWork(void)
{
  if (cFace>1) {
      DisplayDate();
      DisplayTime();
      DisplaySegtypeTable(cFace-1);
      cPosition+=35;
      cSelect=2;
  }
  else {
    if(segment._segment_type > 1) {
      cSelect = 2;
      cFace = 1;
      cPosition = 0;
      DisplaySegtype(segment._segment_type - 1, INVAILDVALUE);
    }
  }
}
//---------------------------------------------------------------------------
void ScreenSegtypePreview::doKeyFWork(void)
{
  if (cFace<6 && (cFace*6) <= segment._segment_count) {
      DisplayDate();
      DisplayTime();
      DisplaySegtypeTable(cFace+1);
      cPosition -= 35;
      cSelect=2;
  } else {
    if(segment._segment_type < 20) {
      cSelect = 2;
      cFace = 1;
      cPosition = 0;
      DisplaySegtype(segment._segment_type + 1, INVAILDVALUE);
    }
  }
/*
  if(segment._segment_type < 20) {
    cSelect = 2;
    cFace = 1;
    cPosition = 0;
    cSelect = 2;
    cFace = 1;
    cPosition = 0;
    DisplaySegtype(segment._segment_type + 1, INVAILDVALUE);
  }
*/
}
//---------------------------------------------------------------------------
void ScreenSegtypePreview::doKeyF1Work(void)
{
    screenMain.DisplayMain();
}
//---------------------------------------------------------------------------
void ScreenSegtypePreview::doKeyF2Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenSegtypePreview::doKeyF3Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenSegtypePreview::doKeyF4Work(void)
{
    screenWeekDayEditF2.vDisplayWeekDayFromPreview(INVAILDVALUE);
}
//---------------------------------------------------------------------------
void ScreenSegtypePreview::doKeyUPWork(void)
{
try {
/*
    if (cSelect==0) {
    } else if (cSelect==1) {
    } else if (cSelect==2) {
               clearCursor8x16(segmentWord[cPosition].X,segmentWord[cPosition].Y+16);
               if (cPosition>=7 && cPosition<=13 ) cPosition += 21;
               else if (cPosition>=14 && cPosition<=41) cPosition-=14;
               else {
                   if (cFace>1) {
                       DisplayDate();
                       DisplayTime();
                       DisplaySegtypeTable(cFace-1);
                       cPosition+=35;
                       cSelect=2;
                   }
               }
               setCursor8x16(segmentWord[cPosition].X,segmentWord[cPosition].Y+16);
    }
*/
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenSegtypePreview::doKeyDOWNWork(void)
{
try {
/*
    if (cSelect==0) {
    } else if (cSelect==1) {
    } else if (cSelect==2) {
               clearCursor8x16(segmentWord[cPosition].X,segmentWord[cPosition].Y+16);
               if (cPosition>=0 && cPosition<=27) cPosition+=14;  //OT Debug 0523
               else if(cPosition>=28 && cPosition<=34) {
                 cPosition -= 21;
               } else {
                   if (cFace<6) {
                       DisplayDate();
                       DisplayTime();
                       DisplaySegtypeTable(cFace+1);
                       cPosition -= 35;
                       cSelect=2;
                   }
               }
               setCursor8x16(segmentWord[cPosition].X,segmentWord[cPosition].Y+16);
    }
*/
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenSegtypePreview::doKeyLEFTWork(void)
{
try {
    if (cSelect==0) {
    } else if (cSelect==1) {
    } else if (cSelect==2) {
               clearCursor8x16(segmentWord[cPosition].X,segmentWord[cPosition].Y+16);

               if( cPosition >0 && cPosition <= 6 ||
                   cPosition >7 && cPosition <= 13||
                   cPosition >14 && cPosition <= 20 ||
                   cPosition >21 && cPosition <= 27 ||
                   cPosition >28 && cPosition <= 34 ||
                   cPosition >35 && cPosition <= 41    )
               {
                 cPosition--;
               } else if ( cPosition == 14 || cPosition == 21 || cPosition == 28 || cPosition == 35 )
               {
                 cPosition-=8;
               } else if ( cPosition == 7)
               {
                 cPosition = 34;
               } else if ( cPosition == 0 )
               {
                 if (cFace>1) {
                     DisplayDate();
                     DisplayTime();
                     DisplaySegtypeTable(cFace-1);
                     cPosition=41;
                     cSelect=2;
                 }
               }


//               if (cPosition==0) cPosition=1;
//               cPosition--;
               setCursor8x16(segmentWord[cPosition].X,segmentWord[cPosition].Y+16);
    }
  } catch(...) {}
}
//---------------------------------------------------------------------------
void ScreenSegtypePreview::doKeyRIGHTWork(void)
{
try {
    if (cSelect==0) {
    } else if (cSelect==1) {
    } else if (cSelect==2) {
               clearCursor8x16(segmentWord[cPosition].X,segmentWord[cPosition].Y+16);

               if( cPosition >=0 && cPosition < 6 ||
                   cPosition >=7 && cPosition < 13||
                   cPosition >=14 && cPosition < 20 ||
                   cPosition >=21 && cPosition < 27 ||
                   cPosition >=28 && cPosition < 34 ||
                   cPosition >=35 && cPosition < 41    )
               {
                 cPosition++;
               } else if ( cPosition == 6 || cPosition == 13 || cPosition == 20 || cPosition == 27 )
               {
                 cPosition+=8;
               } else if ( cPosition == 34)
               {
                 cPosition = 7;
               } else if ( cPosition == 41 )
               {
                 if (cFace<6) {
                     DisplayDate();
                     DisplayTime();
                     DisplaySegtypeTable(cFace+1);
                     cPosition=0;
                     cSelect=2;
                 }
               }

               setCursor8x16(segmentWord[cPosition].X,segmentWord[cPosition].Y+16);
    }
  } catch(...) {}
}
//---------------------------------------------------------------------------
void ScreenSegtypePreview::doKeyEnterWork(void)
{
  screenWeekDayEditF2.vDisplayWeekDayFromPreview(iSegtypePreviewReturn);
}
//---------------------------------------------------------------------------
