#include "SCREENModifyDT.h"
#include "SCREENMain.h"
#include "SCREENCtlSetup.h"
#include "SCREENLockdb.h"
#include "SMEM.h"
#include "LCD240x128.h"
#include "CTIMER.h"

#include "CSTC.h"

#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>

#include "CDataToMessageOK.h"
#include "WRITEJOB.h"
#include "var.h"

SCREENModifyDT screenModifyDT;
//---------------------------------------------------------------------------
SCREENModifyDT::SCREENModifyDT(void)
{
    LoadBitmapFromFile();
    InitDispWord();
    cPosition=0;
    cSelect=0;
}
//---------------------------------------------------------------------------
SCREENModifyDT::~SCREENModifyDT(void)
{
}
//---------------------------------------------------------------------------
void SCREENModifyDT::LoadBitmapFromFile(void)
{
try {
    FILE *bitmap;
    bitmap=fopen("//cct//bitmap//backGround//4_1_3.bit","rb");
    if (bitmap) {
        fread(modifyDTBitmap,3840,1,bitmap);
        fclose(bitmap);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENModifyDT::DisplayModifyDT(void)
{
try {
    smem.SetcFace(cMODIFYDT);
    smem.vWriteMsgToDOM("Enter Modify DateTime Screen");
    lcd240x128.DISPLAY_GRAPHIC(0,modifyDTBitmap,128,30);
    DisplayDate();  DisplayTime();
    DisplayDate();  DisplayTime();
    cPosition=0;    cSelect=0;
    setCursor8x16(dateWord[cPosition].X,dateWord[cPosition].Y+16);
    DisplayCurrentDateTime();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENModifyDT::InitDispWord(void)                                         //��l�Ʀ��e�����y�е��Ѽ�
{
    //�ثe�ɶ�
    for (int i=0;i<8;i++) {
         if (i>=0 && i<=3) cDateWord[i].X=152+i*8;
         else if (i>=4 && i<=5) cDateWord[i].X=192+(i-4)*8;
         else if (i>=6 && i<=7) cDateWord[i].X=216+(i-6)*8;
         cDateWord[i].Y=0;
         cDateWord[i].width=8;
         cDateWord[i].height=16;
    }
    for (int i=0;i<6;i++) {
         if (i>=0 && i<=1) cTimeWord[i].X=168+i*8;
         else if (i>=2 && i<=3) cTimeWord[i].X=192+(i-2)*8;
         else if (i>=4 && i<=5) cTimeWord[i].X=216+(i-4)*8;
         cTimeWord[i].Y=15;
         cTimeWord[i].width=8;
         cTimeWord[i].height=16;
    }

    //��ק諸�ɶ�
    for (int i=0;i<8;i++) {
         if (i>=0 && i<=3) dateWord[i].X=64+i*8;
         else if (i>=4 && i<=5) dateWord[i].X=112+(i-4)*8;
         else if (i>=6 && i<=7) dateWord[i].X=144+(i-6)*8;
         dateWord[i].Y=47;
         dateWord[i].width=8;
         dateWord[i].height=16;
    }
    for (int i=0;i<6;i++) {
         if (i>=0 && i<=1)  timeWord[i].X=80+i*8;
         else if (i>=2 && i<=3)  timeWord[i].X=112+(i-2)*8;
         else if (i>=4 && i<=5)  timeWord[i].X=144+(i-4)*8;
         timeWord[i].Y=70;
         timeWord[i].width=8;
         timeWord[i].height=16;
    }
}
//---------------------------------------------------------------------------
void SCREENModifyDT::DisplayCurrentDateTime(void)
{
try {
    if (smem.GetcFace()==cMODIFYDT) {

        unsigned short cYear=0,cMonth=0,cDay=0,cHour=0,cMin=0,cSec=0;
        time_t currentTime=time(NULL);
        struct tm *now=localtime(&currentTime);

        cYear=now->tm_year+1900;
        cMonth=now->tm_mon+1;
        cDay=now->tm_mday;
        cHour=now->tm_hour;
        cMin=now->tm_min;
        cSec=now->tm_sec;

        lcd240x128.DISPLAY_GRAPHIC_XY(cDateWord[0].X,cDateWord[0].Y,word8x16[cYear/1000],cDateWord[0].height,cDateWord[0].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(cDateWord[1].X,cDateWord[1].Y,word8x16[(cYear/100)%10],cDateWord[1].height,cDateWord[1].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(cDateWord[2].X,cDateWord[2].Y,word8x16[(cYear/10)%10],cDateWord[2].height,cDateWord[2].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(cDateWord[3].X,cDateWord[3].Y,word8x16[cYear%10],cDateWord[3].height,cDateWord[3].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(cDateWord[4].X,cDateWord[4].Y,word8x16[cMonth/10],cDateWord[4].height,cDateWord[4].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(cDateWord[5].X,cDateWord[5].Y,word8x16[cMonth%10],cDateWord[5].height,cDateWord[5].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(cDateWord[6].X,cDateWord[6].Y,word8x16[cDay/10],cDateWord[6].height,cDateWord[6].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(cDateWord[7].X,cDateWord[7].Y,word8x16[cDay%10],cDateWord[7].height,cDateWord[7].width/8);

        lcd240x128.DISPLAY_GRAPHIC_XY(cTimeWord[0].X,cTimeWord[0].Y,word8x16[cHour/10],cTimeWord[0].height,cTimeWord[0].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(cTimeWord[1].X,cTimeWord[1].Y,word8x16[cHour%10],cTimeWord[1].height,cTimeWord[1].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(cTimeWord[2].X,cTimeWord[2].Y,word8x16[cMin/10],cTimeWord[2].height,cTimeWord[2].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(cTimeWord[3].X,cTimeWord[3].Y,word8x16[cMin%10],cTimeWord[3].height,cTimeWord[3].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(cTimeWord[4].X,cTimeWord[4].Y,word8x16[cSec/10],cTimeWord[4].height,cTimeWord[4].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(cTimeWord[5].X,cTimeWord[5].Y,word8x16[cSec%10],cTimeWord[5].height,cTimeWord[5].width/8);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENModifyDT::DisplayDate(void)                                          //��ܤ��
{
try {

    unsigned short cYear=0,cMonth=0,cDay=0;
    time_t currentTime=time(NULL);
    struct tm *now=localtime(&currentTime);
    cYear=now->tm_year+1900;
    cMonth=now->tm_mon+1;
    cDay=now->tm_mday;

    lcd240x128.DISPLAY_GRAPHIC_XY(dateWord[0].X,dateWord[0].Y,word8x16[cYear/1000],dateWord[0].height,dateWord[0].width/8);
    dateTemp[0]=cYear/1000;
    lcd240x128.DISPLAY_GRAPHIC_XY(dateWord[1].X,dateWord[1].Y,word8x16[(cYear/100)%10],dateWord[1].height,dateWord[1].width/8);
    dateTemp[1]=(cYear/100)%10;
    lcd240x128.DISPLAY_GRAPHIC_XY(dateWord[2].X,dateWord[2].Y,word8x16[(cYear/10)%10],dateWord[2].height,dateWord[2].width/8);
    dateTemp[2]=(cYear/10)%10;
    lcd240x128.DISPLAY_GRAPHIC_XY(dateWord[3].X,dateWord[3].Y,word8x16[cYear%10],dateWord[3].height,dateWord[3].width/8);
    dateTemp[3]=cYear%10;
    lcd240x128.DISPLAY_GRAPHIC_XY(dateWord[4].X,dateWord[4].Y,word8x16[cMonth/10],dateWord[4].height,dateWord[4].width/8);
    dateTemp[4]=cMonth/10;
    lcd240x128.DISPLAY_GRAPHIC_XY(dateWord[5].X,dateWord[5].Y,word8x16[cMonth%10],dateWord[5].height,dateWord[5].width/8);
    dateTemp[5]=cMonth%10;
    lcd240x128.DISPLAY_GRAPHIC_XY(dateWord[6].X,dateWord[6].Y,word8x16[cDay/10],dateWord[6].height,dateWord[6].width/8);
    dateTemp[6]=cDay/10;
    lcd240x128.DISPLAY_GRAPHIC_XY(dateWord[7].X,dateWord[7].Y,word8x16[cDay%10],dateWord[7].height,dateWord[7].width/8);
    dateTemp[7]=cDay%10;

  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENModifyDT::DisplayTime(void)                                          //��ܮɶ�
{
try {
    unsigned short cHour=0,cMin=0,cSec=0;
    time_t currentTime=time(NULL);
    struct tm *now=localtime(&currentTime);
    cHour=now->tm_hour;
    cMin=now->tm_min;
    cSec=now->tm_sec;

    lcd240x128.DISPLAY_GRAPHIC_XY(timeWord[0].X,timeWord[0].Y,word8x16[cHour/10],timeWord[0].height,timeWord[0].width/8);
    timeTemp[0]=cHour/10;
    lcd240x128.DISPLAY_GRAPHIC_XY(timeWord[1].X,timeWord[1].Y,word8x16[cHour%10],timeWord[1].height,timeWord[1].width/8);
    timeTemp[1]=cHour%10;
    lcd240x128.DISPLAY_GRAPHIC_XY(timeWord[2].X,timeWord[2].Y,word8x16[cMin/10],timeWord[2].height,timeWord[2].width/8);
    timeTemp[2]=cMin/10;
    lcd240x128.DISPLAY_GRAPHIC_XY(timeWord[3].X,timeWord[3].Y,word8x16[cMin%10],timeWord[3].height,timeWord[3].width/8);
    timeTemp[3]=cMin%10;
    lcd240x128.DISPLAY_GRAPHIC_XY(timeWord[4].X,timeWord[4].Y,word8x16[cSec/10],timeWord[4].height,timeWord[4].width/8);
    timeTemp[4]=cSec/10;
    lcd240x128.DISPLAY_GRAPHIC_XY(timeWord[5].X,timeWord[5].Y,word8x16[cSec%10],timeWord[5].height,timeWord[5].width/8);
    timeTemp[5]=cSec%10;
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENModifyDT::DoKeyWork(BYTE key)
{
try {
    switch (key) {
        case 0x80:
          DoKey0Work();
        break;
        case 0x81:
          DoKey1Work();
        break;
        case 0x82:
          DoKey2Work();
        break;
        case 0x83:
          DoKey3Work();
        break;
        case 0x84:
          DoKey4Work();
        break;
        case 0x85:
          DoKey5Work();
        break;
        case 0x86:
          DoKey6Work();
        break;
        case 0x87:
          DoKey7Work();
        break;
        case 0x88:
          DoKey8Work();
        break;
        case 0x89:
          DoKey9Work();
        break;
        case 0x8A:
        case 0x8B:
        case 0x8C:
        case 0x8D:
        case 0x8E:
        case 0x8F:
        break;
        case 0x90:
          DoKeyF1Work();
        break;
        case 0x91:
        case 0x92:
        break;
        case 0x93:
          DoKeyF4Work();
        break;
        case 0x94:
        case 0x95:
        break;
        case 0x96:
          DoKeyLEFTWork();
        break;
        case 0x97:
          DoKeyRIGHTWork();
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
void SCREENModifyDT::DoKey0Work(void)
{
    if (cSelect==0) {
        lcd240x128.DISPLAY_GRAPHIC_XY(dateWord[cPosition].X,dateWord[cPosition].Y,word8x16[0],dateWord[cPosition].height,dateWord[cPosition].width/8);
        dateTemp[cPosition]=0;
    } else if (cSelect==1) {
        lcd240x128.DISPLAY_GRAPHIC_XY(timeWord[cPosition].X,timeWord[cPosition].Y,word8x16[0],timeWord[cPosition].height,timeWord[cPosition].width/8);
        timeTemp[cPosition]=0;
    }
    DoKeyRIGHTWork();
}
//---------------------------------------------------------------------------
void SCREENModifyDT::DoKey1Work(void)
{
    if (cSelect==0) {
        lcd240x128.DISPLAY_GRAPHIC_XY(dateWord[cPosition].X,dateWord[cPosition].Y,word8x16[1],dateWord[cPosition].height,dateWord[cPosition].width/8);
        dateTemp[cPosition]=0x01;
    } else if (cSelect==1) {
        lcd240x128.DISPLAY_GRAPHIC_XY(timeWord[cPosition].X,timeWord[cPosition].Y,word8x16[1],timeWord[cPosition].height,timeWord[cPosition].width/8);
        timeTemp[cPosition]=0x01;
    }
    DoKeyRIGHTWork();
}
//---------------------------------------------------------------------------
void SCREENModifyDT::DoKey2Work(void)
{
    if (cSelect==0) {
        lcd240x128.DISPLAY_GRAPHIC_XY(dateWord[cPosition].X,dateWord[cPosition].Y,word8x16[2],dateWord[cPosition].height,dateWord[cPosition].width/8);
        dateTemp[cPosition]=0x02;
    } else if (cSelect==1) {
        lcd240x128.DISPLAY_GRAPHIC_XY(timeWord[cPosition].X,timeWord[cPosition].Y,word8x16[2],timeWord[cPosition].height,timeWord[cPosition].width/8);
        timeTemp[cPosition]=0x02;
    }
    DoKeyRIGHTWork();
}
//---------------------------------------------------------------------------
void SCREENModifyDT::DoKey3Work(void)
{
    if (cSelect==0) {
        lcd240x128.DISPLAY_GRAPHIC_XY(dateWord[cPosition].X,dateWord[cPosition].Y,word8x16[3],dateWord[cPosition].height,dateWord[cPosition].width/8);
        dateTemp[cPosition]=0x03;
    } else if (cSelect==1) {
        lcd240x128.DISPLAY_GRAPHIC_XY(timeWord[cPosition].X,timeWord[cPosition].Y,word8x16[3],timeWord[cPosition].height,timeWord[cPosition].width/8);
        timeTemp[cPosition]=0x03;
    }
    DoKeyRIGHTWork();
}
//---------------------------------------------------------------------------
void SCREENModifyDT::DoKey4Work(void)
{
    if (cSelect==0) {
        lcd240x128.DISPLAY_GRAPHIC_XY(dateWord[cPosition].X,dateWord[cPosition].Y,word8x16[4],dateWord[cPosition].height,dateWord[cPosition].width/8);
        dateTemp[cPosition]=0x04;
    } else if (cSelect==1) {
        lcd240x128.DISPLAY_GRAPHIC_XY(timeWord[cPosition].X,timeWord[cPosition].Y,word8x16[4],timeWord[cPosition].height,timeWord[cPosition].width/8);
        timeTemp[cPosition]=0x04;
    }
    DoKeyRIGHTWork();
}
//---------------------------------------------------------------------------
void SCREENModifyDT::DoKey5Work(void)
{
    if (cSelect==0) {
        lcd240x128.DISPLAY_GRAPHIC_XY(dateWord[cPosition].X,dateWord[cPosition].Y,word8x16[5],dateWord[cPosition].height,dateWord[cPosition].width/8);
        dateTemp[cPosition]=0x05;
    } else if (cSelect==1) {
        lcd240x128.DISPLAY_GRAPHIC_XY(timeWord[cPosition].X,timeWord[cPosition].Y,word8x16[5],timeWord[cPosition].height,timeWord[cPosition].width/8);
        timeTemp[cPosition]=0x05;
    }
    DoKeyRIGHTWork();
}
//---------------------------------------------------------------------------
void SCREENModifyDT::DoKey6Work(void)
{
    if (cSelect==0) {
        lcd240x128.DISPLAY_GRAPHIC_XY(dateWord[cPosition].X,dateWord[cPosition].Y,word8x16[6],dateWord[cPosition].height,dateWord[cPosition].width/8);
        dateTemp[cPosition]=0x06;
    } else if (cSelect==1) {
        lcd240x128.DISPLAY_GRAPHIC_XY(timeWord[cPosition].X,timeWord[cPosition].Y,word8x16[6],timeWord[cPosition].height,timeWord[cPosition].width/8);
        timeTemp[cPosition]=0x06;
    }
    DoKeyRIGHTWork();
}
//---------------------------------------------------------------------------
void SCREENModifyDT::DoKey7Work(void)
{
    if (cSelect==0) {
        lcd240x128.DISPLAY_GRAPHIC_XY(dateWord[cPosition].X,dateWord[cPosition].Y,word8x16[7],dateWord[cPosition].height,dateWord[cPosition].width/8);
        dateTemp[cPosition]=0x07;
    } else if (cSelect==1) {
        lcd240x128.DISPLAY_GRAPHIC_XY(timeWord[cPosition].X,timeWord[cPosition].Y,word8x16[7],timeWord[cPosition].height,timeWord[cPosition].width/8);
        timeTemp[cPosition]=0x07;
    }
    DoKeyRIGHTWork();
}
//---------------------------------------------------------------------------
void SCREENModifyDT::DoKey8Work(void)
{
    if (cSelect==0) {
        lcd240x128.DISPLAY_GRAPHIC_XY(dateWord[cPosition].X,dateWord[cPosition].Y,word8x16[8],dateWord[cPosition].height,dateWord[cPosition].width/8);
        dateTemp[cPosition]=0x08;
    } else if (cSelect==1) {
        lcd240x128.DISPLAY_GRAPHIC_XY(timeWord[cPosition].X,timeWord[cPosition].Y,word8x16[8],timeWord[cPosition].height,timeWord[cPosition].width/8);
        timeTemp[cPosition]=0x08;
    }
    DoKeyRIGHTWork();
}
//---------------------------------------------------------------------------
void SCREENModifyDT::DoKey9Work(void)
{
    if (cSelect==0) {
        lcd240x128.DISPLAY_GRAPHIC_XY(dateWord[cPosition].X,dateWord[cPosition].Y,word8x16[9],dateWord[cPosition].height,dateWord[cPosition].width/8);
        dateTemp[cPosition]=0x09;
    } else if (cSelect==1) {
        lcd240x128.DISPLAY_GRAPHIC_XY(timeWord[cPosition].X,timeWord[cPosition].Y,word8x16[9],timeWord[cPosition].height,timeWord[cPosition].width/8);
        timeTemp[cPosition]=0x09;
    }
    DoKeyRIGHTWork();
}
//---------------------------------------------------------------------------
void SCREENModifyDT::DoKeyF1Work(void)
{
    screenMain.DisplayMain();
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void SCREENModifyDT::DoKeyF4Work(void)
{
    screenCtlSetup.DisplayCtlSetup();
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void SCREENModifyDT::DoKeyLEFTWork(void)
{
    if (cSelect==0) {
        clearCursor8x16(dateWord[cPosition].X,dateWord[cPosition].Y+16);
        if (cPosition==0) cPosition=1;
        cPosition--;
        setCursor8x16(dateWord[cPosition].X,dateWord[cPosition].Y+16);
    } else if (cSelect==1) {
        clearCursor8x16(timeWord[cPosition].X,timeWord[cPosition].Y+16);
        if (cPosition==0) {
            cSelect=0;
            cPosition=7;
            setCursor8x16(dateWord[cPosition].X,dateWord[cPosition].Y+16);
        } else {
           cPosition--;
           setCursor8x16(timeWord[cPosition].X,timeWord[cPosition].Y+16);
        }
    }
}
//---------------------------------------------------------------------------
void SCREENModifyDT::DoKeyRIGHTWork(void)
{
    if (cSelect==0) {
        clearCursor8x16(dateWord[cPosition].X,dateWord[cPosition].Y+16);
        cPosition++;
        if (cPosition>=8) {
            cSelect=1;
            cPosition=0;
            setCursor8x16(timeWord[cPosition].X,timeWord[cPosition].Y+16);
        } else  setCursor8x16(dateWord[cPosition].X,dateWord[cPosition].Y+16);
    } else if (cSelect==1) {
        clearCursor8x16(timeWord[cPosition].X,timeWord[cPosition].Y+16);
        cPosition++;
        if (cPosition>=6) cPosition=5;
        setCursor8x16(timeWord[cPosition].X,timeWord[cPosition].Y+16);
    }
}
//---------------------------------------------------------------------------
void SCREENModifyDT::DoKeyEnterWork(void)
{
try {
    if (smem.GetDbOperStat()==0 || smem.vLoadCenterConnectStatus()==false) {
        unsigned int year=dateTemp[0]*1000+dateTemp[1]*100+dateTemp[2]*10+dateTemp[3],month=dateTemp[4]*10+dateTemp[5],day=dateTemp[6]*10+dateTemp[7];
        unsigned int hour=timeTemp[0]*10+timeTemp[1],min=timeTemp[2]*10+timeTemp[3],sec=timeTemp[4]*10+timeTemp[5];

      //OTMARKPRINTF  printf("%d-%d-%d   %d:%d:%d\n",year,month,day,hour,min,sec);

        //�D�X�~���ɤ���

        char date[22]={0},time[17]={0};

        if (year>=1900 && year<=2200 && month>=1 && month <=12 && day>=1 && day<=31 && hour>=0 && hour<=24 && min>=0 && min<=60 && sec>=0 && sec<=60) {


            stc.TimersRead_BeforeResetCMOSTime();  //OTBUG =1
            _intervalTimer.TimersRead_BeforeResetCMOSTime();
            smem.vSetTimerMutexRESET(1);
            SendTimeisChangetoArwenActuate();//to arwen
            while(smem.vGetTimerMutexCTIMER() == 0 || smem.vGetTimerMutexCSTC() == 0) {

            	usleep(100);
//              printf("screen data while\n");
            }
//            smem.vSetTimerMutexCTIMER(true);
//            smem.vSetTimerMutexCSTC(true);

//            _intervalTimer.TimersRead_BeforeResetCMOSTime();
//            stc.TimersRead_BeforeResetCMOSTime();  //OTBUG =1

            smem.vSetSystemClockTime(year, month, day, hour, min, sec);
/*move to smem

            sprintf(date,"date -s %#02d%#02d%#02d%#02d%#04d",month,day,hour,min,year);
          //OTMARKPRINTF  printf("\n%s\n",date);
            system(date);
            sprintf(time,"date -s %#02d:%#02d:%#02d",hour,min,sec);
          //OTMARKPRINTF  printf("\n%s\n",time);
            system(time);
*/

            char message[200]={0};
            sprintf(message,"Change DateTime for %#04d-%#02d-%#02d %#02d:%#02d:%#02d",year,month,day,hour,min,sec);
            smem.vWriteMsgToDOM(message);

            _intervalTimer.TimersReset_AfterResetCMOSTime();
            stc.TimersReset_AfterResetCMOSTime();  //OTBUG =1

//OT Debug 951128 here for VD
//            smem.vSetVDUpdateDB(0,true);

//move to smem            system("hwclock -w");

            screenCtlSetup.DisplayCtlSetup();

//            smem.vSetTimerMutexCSTC(false);
//            smem.vSetTimerMutexCTIMER(false);
            smem.vSetTimerMutexRESET(0);

            unsigned char ucSendData[6]={0};
            ucSendData[0] = 0x0F;
            ucSendData[1] = 0x02;

            MESSAGEOK _MSG;

            _MSG = oDataToMessageOK.vPackageINFOTo92Protocol(ucSendData, 2, true);
            _MSG.InnerOrOutWard = cOutWard;
            writeJob.WriteWorkByMESSAGEOUT(_MSG);
            writeJob.WritePhysicalOut(_MSG.packet, _MSG.packetLength, DEVICECENTER92);

            smem.vSendTimerUpdateToCCJ_5F9E();

            system("hwclock -w");


        }
    } else {
        smem.SetLastFace(cMODIFYDT);
        screenLockdb.DisplayLockdb();
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENModifyDT::SendTimeisChangetoArwenActuate()
{
	unsigned char data[20]={0};
	data[0]=0x0F;
	data[1]=0x12;
	MESSAGEOK MsgOK;
	MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, 2, true);
    MsgOK.InnerOrOutWard = cOutWard;
    writeJob.WritePhysicalOut(MsgOK.packet, MsgOK.packetLength, DEVICECCJDYNCTL);

}
