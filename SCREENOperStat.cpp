//---------------------------------------------------------------------------
#include "SCREENOperStat.h"
#include "SCREENMain.h"
#include "SCREENCtlSetup.h"
#include "LCD240x128.h"
#include "SMEM.h"

#include "SCREENMBSelect.h"
#include "SCREENUart2.h"

#include <string.h>

SCREENOperStat screenOperStat;
//---------------------------------------------------------------------------
SCREENOperStat::SCREENOperStat(void)
{
  InitDispWord();
  LoadBitmapFromFile();
}
//---------------------------------------------------------------------------
SCREENOperStat::~SCREENOperStat(void)
{
}
//---------------------------------------------------------------------------
void SCREENOperStat::DoKeyWork(BYTE key)              //已過濾,進來的是0x80~0x98之間,5X5按鍵
{
try {
    switch (key) {
        case 0x90:
          DoKeyF1Work();
        break;
        case 0x91:
          DoKeyF2Work();
        break;
        case 0x92:
          DoKeyF3Work();
        break;
        case 0x93:
          DoKeyF4Work();
        break;

        case 0x98:
          DoKeyEnterWork();
        break;

        default:
        break;
    }
  } catch (...) {}
}

//---------------------------------------------------------------------------
void SCREENOperStat::InitDispWord(void)
{
try {
    for (int i=0;i<8;i++) {
         if (i>=0 && i<=3) dateWord[i].X=80+i*8;                               //xxxx年
         else if (i>=4 && i<=5) dateWord[i].X=128+(i-4)*8;                      //xx月
         else if (i>=6 && i<=7) dateWord[i].X=160+(i-6)*8;                      //xx日
         dateWord[i].Y=32;
         dateWord[i].width=8;
         dateWord[i].height=16;
    }
    for (int i=0;i<6;i++) {
         if (i>=0 && i<=1) timeWord[i].X=96+i*8;
         else if (i>=2 && i<=3) timeWord[i].X=128+(i-2)*8;
         else if (i>=4 && i<=5) timeWord[i].X=160+(i-4)*8;
         timeWord[i].Y=48;
         timeWord[i].width=8;
         timeWord[i].height=16;
    }

    for (int i=0;i<6;i++) {
         GreenConflictWord[i].X=16+i*8;
         GreenConflictWord[i].Y=112;
         GreenConflictWord[i].width=8;
         GreenConflictWord[i].height=16;
    }

    for (int i=0;i<4;i++) {
         HWStatusWord[i].X=72+i*8;
         HWStatusWord[i].Y=112;
         HWStatusWord[i].width=8;
         HWStatusWord[i].height=16;
    }

    MachineLocationWord.X = 112;
    MachineLocationWord.Y = 112;
    MachineLocationWord.width=8;
    MachineLocationWord.height=16;


  } catch (...) {}
}


//---------------------------------------------------------------------------
void SCREENOperStat::LoadBitmapFromFile(void)
{
try {
    FILE *bitmap;
//disable    bitmap=fopen("//cct//bitmap//backGround//4_1_8.bit","rb");
    bitmap=fopen("//cct//bitmap//backGround//01-08-OTHWInfo.bit","rb");
    if (bitmap) {
        fread(operStatBitmap,3840,1,bitmap);
        fclose(bitmap);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENOperStat::DisplayOperStat(void)
{
try {
    smem.SetcFace(cOPERSTAT);
    smem.vWriteMsgToDOM("Enter Operate Status Screen");
    lcd240x128.DISPLAY_GRAPHIC(0,operStatBitmap,128,30);

    vShowGPSTime(0);
    vShowMachineLocation();

  } catch (...) {}
}

//---------------------------------------------------------------------------
//OT20111128
void SCREENOperStat::vShowMachineLocation()
{
  unsigned char ucTmp;
  ucTmp = smem.vGetUCData(TC_CCT_MachineLocation);

  lcd240x128.DISPLAY_GRAPHIC_XY(MachineLocationWord.X,MachineLocationWord.Y,word8x16[ucTmp],MachineLocationWord.height,MachineLocationWord.width/8);

}


//---------------------------------------------------------------------------
void SCREENOperStat::vShowGPSTime(time_t screenRunSec)
{
  unsigned short cYear=0,cMonth=0,cDay=0,cHour=0,cMin=0,cSec=0;

  struct tm time_str;
  struct tm *TSP = &time_str;

  TSP = localtime(&screenRunSec);

/*
TSP->tm_mon+1,TSP->tm_mday,TSP->tm_hour,TSP->tm_min,TSP->tm_year+1900
TSP->tm_hour,TSP->tm_min,TSP->tm_sec
*/

  cYear=TSP->tm_year+1900;
  cMonth=TSP->tm_mon+1;
  cDay=TSP->tm_mday;
  cHour=TSP->tm_hour;
  cMin=TSP->tm_min;
  cSec=TSP->tm_sec;

  lcd240x128.DISPLAY_GRAPHIC_XY(dateWord[0].X,dateWord[0].Y,word8x16[cYear/1000],dateWord[0].height,dateWord[0].width/8);
  lcd240x128.DISPLAY_GRAPHIC_XY(dateWord[1].X,dateWord[1].Y,word8x16[(cYear/100)%10],dateWord[1].height,dateWord[1].width/8);
  lcd240x128.DISPLAY_GRAPHIC_XY(dateWord[2].X,dateWord[2].Y,word8x16[(cYear/10)%10],dateWord[2].height,dateWord[2].width/8);
  lcd240x128.DISPLAY_GRAPHIC_XY(dateWord[3].X,dateWord[3].Y,word8x16[cYear%10],dateWord[3].height,dateWord[3].width/8);
  lcd240x128.DISPLAY_GRAPHIC_XY(dateWord[4].X,dateWord[4].Y,word8x16[cMonth/10],dateWord[4].height,dateWord[4].width/8);
  lcd240x128.DISPLAY_GRAPHIC_XY(dateWord[5].X,dateWord[5].Y,word8x16[cMonth%10],dateWord[5].height,dateWord[5].width/8);
  lcd240x128.DISPLAY_GRAPHIC_XY(dateWord[6].X,dateWord[6].Y,word8x16[cDay/10],dateWord[6].height,dateWord[6].width/8);
  lcd240x128.DISPLAY_GRAPHIC_XY(dateWord[7].X,dateWord[7].Y,word8x16[cDay%10],dateWord[7].height,dateWord[7].width/8);

  lcd240x128.DISPLAY_GRAPHIC_XY(timeWord[0].X,timeWord[0].Y,word8x16[cHour/10],timeWord[0].height,timeWord[0].width/8);
  lcd240x128.DISPLAY_GRAPHIC_XY(timeWord[1].X,timeWord[1].Y,word8x16[cHour%10],timeWord[1].height,timeWord[1].width/8);
  lcd240x128.DISPLAY_GRAPHIC_XY(timeWord[2].X,timeWord[2].Y,word8x16[cMin/10],timeWord[2].height,timeWord[2].width/8);
  lcd240x128.DISPLAY_GRAPHIC_XY(timeWord[3].X,timeWord[3].Y,word8x16[cMin%10],timeWord[3].height,timeWord[3].width/8);
  lcd240x128.DISPLAY_GRAPHIC_XY(timeWord[4].X,timeWord[4].Y,word8x16[cSec/10],timeWord[4].height,timeWord[4].width/8);
  lcd240x128.DISPLAY_GRAPHIC_XY(timeWord[5].X,timeWord[5].Y,word8x16[cSec%10],timeWord[5].height,timeWord[5].width/8);

}

//---------------------------------------------------------------------------
void SCREENOperStat::vShowGreenConflict(void)
{
  unsigned char ucGC[6];

  smem.vGetSignalConflictErrorVar(ucGC);

  BYTE test[128]={0};
  char tempBuff[16],buff[64]="";

  for (int i = 0; i < 3; i++) {
    printf("------------%d:%02X---------------------\n", i, ucGC[i]);
       sprintf(tempBuff, "%02X", ucGC[i]);
       strcat(buff, tempBuff);
  }

  int iScreenLength = strlen(buff);

  for(int i = 0; i < iScreenLength; i++)
  {
          if(buff[i] == '0') buff[i] = 0x00;
    else if (buff[i] == '1') buff[i] = 0x01;
    else if (buff[i] == '2') buff[i] = 0x02;
    else if (buff[i] == '3') buff[i] = 0x03;
    else if (buff[i] == '4') buff[i] = 0x04;
    else if (buff[i] == '5') buff[i] = 0x05;
    else if (buff[i] == '6') buff[i] = 0x06;
    else if (buff[i] == '7') buff[i] = 0x07;
    else if (buff[i] == '8') buff[i] = 0x08;
    else if (buff[i] == '9') buff[i] = 0x09;
    else if (buff[i] == 'A') buff[i] = 0x0A;
    else if (buff[i] == 'B') buff[i] = 0x0B;
    else if (buff[i] == 'C') buff[i] = 0x0C;
    else if (buff[i] == 'D') buff[i] = 0x0D;
    else if (buff[i] == 'E') buff[i] = 0x0E;
    else if (buff[i] == 'F') buff[i] = 0x0F;
    else if (buff[i] == 'a') buff[i] = 0x10;
    else if (buff[i] == 'b') buff[i] = 0x11;
    else if (buff[i] == 'c') buff[i] = 0x12;
    else if (buff[i] == 'd') buff[i] = 0x13;
    else if (buff[i] == 'e') buff[i] = 0x14;
    else if (buff[i] == 'f') buff[i] = 0x15;
    else if (buff[i] == ' ') buff[i] = 0x99;
  }
//  printf("GoTo vShowGreenConflict, Msg= %s, len:%d\n", buff, iScreenLength);

  for(int i = 0; i < 6; i++) {
      if( (buff[i] != (char)0x99) ) {
        lcd240x128.DISPLAY_GRAPHIC_XY(GreenConflictWord[i].X,GreenConflictWord[i].Y,word8x16[buff[i]],GreenConflictWord[i].height,GreenConflictWord[i].width/8);
      }
      else {
        lcd240x128.DISPLAY_GRAPHIC_XY(GreenConflictWord[i].X,GreenConflictWord[i].Y,test,GreenConflictWord[i].height,GreenConflictWord[i].width/8);
      }
  }

  //OT20110825
  unsigned char ucTMP;
  unsigned char uc0F04HWStatus[2];
  bzero(tempBuff, 16);
  bzero(buff, 64);
  ucTMP = smem.vGetHardwareStatus(3);
  uc0F04HWStatus[0] = ucTMP;
  ucTMP = smem.vGetHardwareStatus(4);
  uc0F04HWStatus[1] = ucTMP;
  for (int i = 0; i < 2; i++) {
       printf("%d:%02X \n", i, uc0F04HWStatus[i]);
       sprintf(tempBuff, "%02X", uc0F04HWStatus[i]);
       strcat(buff, tempBuff);
  }
  iScreenLength = strlen(buff);
  for(int i = 0; i < iScreenLength; i++)
  {
          if(buff[i] == '0') buff[i] = 0x00;
    else if (buff[i] == '1') buff[i] = 0x01;
    else if (buff[i] == '2') buff[i] = 0x02;
    else if (buff[i] == '3') buff[i] = 0x03;
    else if (buff[i] == '4') buff[i] = 0x04;
    else if (buff[i] == '5') buff[i] = 0x05;
    else if (buff[i] == '6') buff[i] = 0x06;
    else if (buff[i] == '7') buff[i] = 0x07;
    else if (buff[i] == '8') buff[i] = 0x08;
    else if (buff[i] == '9') buff[i] = 0x09;
    else if (buff[i] == 'A') buff[i] = 0x0A;
    else if (buff[i] == 'B') buff[i] = 0x0B;
    else if (buff[i] == 'C') buff[i] = 0x0C;
    else if (buff[i] == 'D') buff[i] = 0x0D;
    else if (buff[i] == 'E') buff[i] = 0x0E;
    else if (buff[i] == 'F') buff[i] = 0x0F;
    else if (buff[i] == 'a') buff[i] = 0x10;
    else if (buff[i] == 'b') buff[i] = 0x11;
    else if (buff[i] == 'c') buff[i] = 0x12;
    else if (buff[i] == 'd') buff[i] = 0x13;
    else if (buff[i] == 'e') buff[i] = 0x14;
    else if (buff[i] == 'f') buff[i] = 0x15;
    else if (buff[i] == ' ') buff[i] = 0x99;
  }
//  printf("GoTo showHWStatus, Msg= %s, len:%d\n", buff, iScreenLength);

  for(int i = 0; i < 4; i++) {
      if( (buff[i] != (char)0x99) ) {
        lcd240x128.DISPLAY_GRAPHIC_XY(HWStatusWord[i].X,HWStatusWord[i].Y,word8x16[buff[i]],HWStatusWord[i].height,HWStatusWord[i].width/8);
      }
      else {
        lcd240x128.DISPLAY_GRAPHIC_XY(HWStatusWord[i].X,HWStatusWord[i].Y,test,HWStatusWord[i].height,HWStatusWord[i].width/8);
      }
  }


}

//---------------------------------------------------------------------------
void SCREENOperStat::DoKeyF1Work(void)
{
    screenMain.DisplayMain();
}

//---------------------------------------------------------------------------
void SCREENOperStat::DoKeyF2Work(void)
{
    screenMBSelect.DisplayMBSelect();
}

//---------------------------------------------------------------------------
void SCREENOperStat::DoKeyF3Work(void)
{
    screenUart2.DisplayUart2Select();
}


//---------------------------------------------------------------------------
void SCREENOperStat::DoKeyF4Work(void)
{
    screenCtlSetup.DisplayCtlSetup();
}

//---------------------------------------------------------------------------
void SCREENOperStat::DoKeyEnterWork(void)
{
    smem.vSetBOOLData(GPS_SYNC, true);
    DisplayOperStat();
}
//---------------------------------------------------------------------------

