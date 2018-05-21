#include "SCREENLast92TCPlanSegmentUpdate.h"
#include "SCREENMain.h"
#include "SCREENCtlSetup.h"
#include "LCD240x128.h"
#include "SMEM.h"

SCREENLast92TCPlanSegmentUpdate screenLast92TCPlanSegmentUpdate;
//---------------------------------------------------------------------------
SCREENLast92TCPlanSegmentUpdate::SCREENLast92TCPlanSegmentUpdate(void)
{
try {
    LoadBitmapFromFile();
    InitDispWord();
  } catch (...) {}
}
//---------------------------------------------------------------------------
SCREENLast92TCPlanSegmentUpdate::~SCREENLast92TCPlanSegmentUpdate(void)
{
}
//---------------------------------------------------------------------------
void SCREENLast92TCPlanSegmentUpdate::DoKeyWork(BYTE key)                       //已過濾,進來的是0x80~0x98之間
{
try {
    switch (key) {
        case 0x80:
        case 0x81:
        case 0x82:
        case 0x83:
        case 0x84:
        case 0x85:
        case 0x86:
        case 0x87:
        case 0x88:
        case 0x89:
        case 0x8A:
        case 0x8B:
        case 0x8C:
        case 0x8D:
        case 0x8E:
        case 0x8F:
        case 0x91://F2
        case 0x92://F3
        break;

        case 0x90://F1,ESC
          DoKeyF1Work();
        break;
        case 0x93://F4
          DoKeyF4Work();
        break;

        case 0x94://UP
        case 0x95://DOWN
        case 0x96://LEFT
        case 0x97://RIGHT
        case 0x98://Enter
        break;
        default:
        break;
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENLast92TCPlanSegmentUpdate::LoadBitmapFromFile(void)
{
try {
    FILE *bitmap;
    bitmap=fopen("//cct//bitmap//backGround//2_Last92TCPlanSegmentUpdate.bit","rb");
    if (bitmap) {
        fread(lastDownBitmap,3840,1,bitmap);
        fclose(bitmap);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENLast92TCPlanSegmentUpdate::DisplayLastUpdate(void)
{
try {
    smem.SetcFace(cLAST92TCPLANSEGMENTUPDATE);
    smem.vWriteMsgToDOM("Enter Last Get 5F15 Screen");
    lcd240x128.DISPLAY_GRAPHIC(0,lastDownBitmap,128,30);

    usiSegmentType = 0;
    usiPlanID = 0;

    YMDHMS temp=smem.vGetLast92TC_5F15Time();

    lcd240x128.DISPLAY_GRAPHIC_XY(DLastTime[0].X,DLastTime[0].Y,word8x16[temp.Year/10],DLastTime[0].height,DLastTime[0].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(DLastTime[1].X,DLastTime[1].Y,word8x16[temp.Year%10],DLastTime[1].height,DLastTime[1].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(DLastTime[2].X,DLastTime[2].Y,word8x16[temp.Month/10],DLastTime[2].height,DLastTime[2].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(DLastTime[3].X,DLastTime[3].Y,word8x16[temp.Month%10],DLastTime[3].height,DLastTime[3].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(DLastTime[4].X,DLastTime[4].Y,word8x16[temp.Day/10],DLastTime[4].height,DLastTime[4].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(DLastTime[5].X,DLastTime[5].Y,word8x16[temp.Day%10],DLastTime[5].height,DLastTime[5].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(DLastTime[6].X,DLastTime[6].Y,word8x16[temp.Hour/10],DLastTime[6].height,DLastTime[6].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(DLastTime[7].X,DLastTime[7].Y,word8x16[temp.Hour%10],DLastTime[7].height,DLastTime[7].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(DLastTime[8].X,DLastTime[8].Y,word8x16[temp.Min/10],DLastTime[8].height,DLastTime[8].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(DLastTime[9].X,DLastTime[9].Y,word8x16[temp.Min%10],DLastTime[9].height,DLastTime[9].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(DLastTime[10].X,DLastTime[10].Y,word8x16[temp.Sec/10],DLastTime[10].height,DLastTime[10].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(DLastTime[11].X,DLastTime[11].Y,word8x16[temp.Sec%10],DLastTime[11].height,DLastTime[11].width/8);

    lcd240x128.DISPLAY_GRAPHIC_XY(DPlanID[0].X,DPlanID[0].Y,word8x16[usiPlanID/10],DPlanID[0].height,DPlanID[0].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(DPlanID[1].X,DPlanID[1].Y,word8x16[usiPlanID%10],DPlanID[1].height,DPlanID[1].width/8);

    lcd240x128.DISPLAY_GRAPHIC_XY(DSegmentType[0].X,DSegmentType[0].Y,word8x16[usiSegmentType/10],DSegmentType[0].height,DSegmentType[0].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(DSegmentType[1].X,DSegmentType[1].Y,word8x16[usiSegmentType%10],DSegmentType[1].height,DSegmentType[1].width/8);

  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENLast92TCPlanSegmentUpdate::InitDispWord(void)
{
try {
    for (int i=0;i<12;i++) {
         DLastTime[i].width=8;
         DLastTime[i].height=16;
         if (i==0 || i==1) {
             DLastTime[i].X=16+i*8;
             DLastTime[i].Y=40;
         } else if (i==2 || i==3) {
             DLastTime[i].X=48+(i-2)*8;
             DLastTime[i].Y=40;
         } else if (i==4 || i==5) {
             DLastTime[i].X=80+(i-4)*8;
             DLastTime[i].Y=40;
         } else if (i==6 || i==7) {
             DLastTime[i].X=112+(i-6)*8;
             DLastTime[i].Y=40;
         } else if (i==8 || i==9) {
             DLastTime[i].X=144+(i-8)*8;
             DLastTime[i].Y=40;
         } else if (i==10 || i==11) {
             DLastTime[i].X=176+(i-10)*8;
             DLastTime[i].Y=40;
         }
    }
    for (int i = 0; i <2; i++) {
      DPlanID[i].width=8;
      DPlanID[i].height=16;
      DPlanID[i].X=56+i*8;
      DPlanID[i].Y=64;
    }

    for (int i = 0; i <2; i++) {
      DSegmentType[i].width=8;
      DSegmentType[i].height=16;
      DSegmentType[i].X=56+i*8;
      DSegmentType[i].Y=84;
    }


  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENLast92TCPlanSegmentUpdate::DoKeyF1Work(void)
{
    screenMain.DisplayMain();
}
//---------------------------------------------------------------------------
void SCREENLast92TCPlanSegmentUpdate::DoKeyF4Work(void)
{
    screenMain.DisplayMain();
}

//---------------------------------------------------------------------------
void SCREENLast92TCPlanSegmentUpdate::DisplayPlanUpdate(void)
{
try {

    if(usiPlanID == 0) { smem.vSaveLast92TC_5F15Time(); }

    if(smem.GetcFace() == cLAST92TCPLANSEGMENTUPDATE) {

      if(usiPlanID == 0 && usiSegmentType == 0) {  DisplayLastUpdate(); }
      usiPlanID++;
      lcd240x128.DISPLAY_GRAPHIC_XY(DPlanID[0].X,DPlanID[0].Y,word8x16[usiPlanID/10],DPlanID[0].height,DPlanID[0].width/8);
      lcd240x128.DISPLAY_GRAPHIC_XY(DPlanID[1].X,DPlanID[1].Y,word8x16[usiPlanID%10],DPlanID[1].height,DPlanID[1].width/8);
    }

  } catch (...) {}
}

//---------------------------------------------------------------------------
void SCREENLast92TCPlanSegmentUpdate::DisplaySegmentUpdate(void)
{
try {

    if(usiSegmentType == 0) { smem.vSaveLast92TC_5F15Time(); }

    if(smem.GetcFace() == cLAST92TCPLANSEGMENTUPDATE) {

      if(usiSegmentType == 0 && usiPlanID == 0) { DisplayLastUpdate(); }
      usiSegmentType++;
      lcd240x128.DISPLAY_GRAPHIC_XY(DSegmentType[0].X,DSegmentType[0].Y,word8x16[usiSegmentType/10],DSegmentType[0].height,DSegmentType[0].width/8);
      lcd240x128.DISPLAY_GRAPHIC_XY(DSegmentType[1].X,DSegmentType[1].Y,word8x16[usiSegmentType%10],DSegmentType[1].height,DSegmentType[1].width/8);
    }

  } catch (...) {}
}
