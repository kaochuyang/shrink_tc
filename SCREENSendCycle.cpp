#include "SCREENSendCycle.h"
#include "SCREENMain.h"
#include "SCREENLockdb.h"
#include "LCD240x128.h"
#include "SMEM.h"
#include "SCREENABOErr.h"
#include "SCREENV3Related.h"

#include "SCREENCtlSetup.h"

#include "CDataToMessageOK.h"
#include "WRITEJOB.h"


SCREENSendCycle screenSendCycle;
//---------------------------------------------------------------------------
SCREENSendCycle::SCREENSendCycle(void)
{
try {
    LoadBitmapFromFile();
    InitDispWord();
    for (int i=0;i<2;i++) cycleData[i] = 0;
    cSelect=0;
  } catch (...) {}
}
//---------------------------------------------------------------------------
SCREENSendCycle::~SCREENSendCycle(void)
{
}
//---------------------------------------------------------------------------
void SCREENSendCycle::DoKeyWork(BYTE key)
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
        case 0x86:
        case 0x87:
        case 0x88:
        case 0x89:
        break;
        case 0x8A:
          DoKeyAWork();
        break;
        case 0x8B:
          DoKeyBWork();
        break;
        case 0x8C:
          DoKeyCWork();
        break;
        case 0x8D:
          DoKeyDWork();
        break;
        case 0x8E:
          DoKeyEWork();
        break;
        case 0x8F:
          DoKeyFWork();
        break;
        case 0x90://F1 ESC
          DoKeyF1Work();
        break;
        case 0x91:
        break;
        case 0x92:
        break;
        case 0x93:
          DoKeyF4Work();
        break;
        case 0x94:
          DoKeyUPWork();
        break;
        case 0x95:
          DoKeyDOWNWork();
        break;
        case 0x96:
        case 0x97:
        break;
        case 0x98://ENTER
          DoKeyEnterWork();
        break;
        default:
        break;
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENSendCycle::LoadBitmapFromFile(void)
{
try {
    FILE *bitmap;
    bitmap=fopen("//cct//bitmap//backGround//4_3_5FTC_TransmitCycle.bit","rb");
    if (bitmap) {
        fread(sendCycleBitmap,3840,1,bitmap);
        fclose(bitmap);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENSendCycle::DisplaySendCycle(void)
{ //�ǿ�g��:[0]:��q����g��[1]:�����g��[2]:�ήɰ���[3]:�ƥ�g��
  //(A(10):���� B(11):�@�� C(12):��� D(13):���� E(14):�@�� F(15):���� 0:�G�Q��)
try {
    smem.SetcFace(cSENDCYCLE);
    smem.vWriteMsgToDOM("Enter Send Cycle Screen");
    lcd240x128.DISPLAY_GRAPHIC(0,sendCycleBitmap,128,30);

    int iTMP;
    iTMP = smem.vGetINTData(TC92SignalLightStatus_5F0F_IntervalTime);
    if (iTMP==0) cycleData[0] = 0;  //send change
    else if (iTMP==1) cycleData[0]=11;
    else if (iTMP==2) cycleData[0]=12;
    else if (iTMP==5) cycleData[0]=13;
    else if (iTMP==60) cycleData[0]=14;
    else if (iTMP==300) cycleData[0]=15;
    else if (iTMP==0xFF) cycleData[0]=10;  //send stop
    else cycleData[0] = 13;

    iTMP = smem.vGetINTData(TC92SignalStepStatus_5F03_IntervalTime);
    if (iTMP==0) cycleData[1] = 0;
    else if (iTMP==1) cycleData[1]=11;
    else if (iTMP==2) cycleData[1]=12;
    else if (iTMP==5) cycleData[1]=13;
    else if (iTMP==60) cycleData[1]=14;
    else if (iTMP==300) cycleData[1]=15;
    else if (iTMP==0xFF) cycleData[1]=10;
    else cycleData[1] = 13;

    cSelect=0;
    DisplayCycle(0);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENSendCycle::DisplayCycle(int idx)
{
try {
    cSelect=idx;
    setSelectPoint16x16(data[cSelect].X,data[cSelect].Y,data[cSelect].height,data[cSelect].width/8);
    if (cycleData[cSelect]==0 || cycleData[cSelect]==10 || cycleData[cSelect]==11 || cycleData[cSelect]==12 || cycleData[cSelect]==13 || cycleData[cSelect]==14 || cycleData[cSelect]==15)
    lcd240x128.DISPLAY_GRAPHIC_XY(current.X,current.Y,word24x24[cycleData[cSelect]],current.height,current.width/8);

    if (idx==0 || idx==1) {  //��q����g�����,�����g�����,�Y�ɰ�����
        for (int i=0;i<6;i++) lcd240x128.DISPLAY_GRAPHIC_XY(select[i].X,select[i].Y,word16x16[10+i],select[i].height,select[i].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(select[6].X,select[6].Y,word16x16[0],select[6].height,select[6].width/8);
    } else {  //��~
        for (int i=0;i<7;i++) clearSelectPoint16x16(select[i].X,select[i].Y,select[i].height,select[i].width/8);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENSendCycle::InitDispWord(void)
{
    current.X=104;
    current.Y=5;
    current.width=24;
    current.height=24;

    for (int i=0;i<4;i++) {
         data[i].X=8;
         data[i].Y=30+i*17;
         data[i].width=16;
         data[i].height=16;
    }

    for (int i=0;i<7;i++) {
         select[i].X=160;
         select[i].Y=8+i*16;
         select[i].width=16;
         select[i].height=16;
    }
}
//---------------------------------------------------------------------------
void SCREENSendCycle::DoKey0Work(void)
{
try {
    if (cSelect==0 || cSelect==1) {
        cycleData[cSelect]=0;
        lcd240x128.DISPLAY_GRAPHIC_XY(current.X,current.Y,word24x24[cycleData[cSelect]],current.height,current.width/8);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENSendCycle::DoKey1Work(void)
{
try {
    clearSelectPoint16x16(data[cSelect].X,data[cSelect].Y,data[cSelect].height,data[cSelect].width/8);
    DisplayCycle(0);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENSendCycle::DoKey2Work(void)
{
try {
    clearSelectPoint16x16(data[cSelect].X,data[cSelect].Y,data[cSelect].height,data[cSelect].width/8);
    DisplayCycle(1);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENSendCycle::DoKey3Work(void)
{
try {
//    clearSelectPoint16x16(data[cSelect].X,data[cSelect].Y,data[cSelect].height,data[cSelect].width/8);
//    DisplayCycle(2);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENSendCycle::DoKey4Work(void)
{
try {
//    clearSelectPoint16x16(data[cSelect].X,data[cSelect].Y,data[cSelect].height,data[cSelect].width/8);
//    DisplayCycle(3);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENSendCycle::DoKeyAWork(void)
{
    cycleData[cSelect]=10;
    lcd240x128.DISPLAY_GRAPHIC_XY(current.X,current.Y,word24x24[cycleData[cSelect]],current.height,current.width/8);
}
//---------------------------------------------------------------------------
void SCREENSendCycle::DoKeyBWork(void)
{
    cycleData[cSelect]=11;
    lcd240x128.DISPLAY_GRAPHIC_XY(current.X,current.Y,word24x24[cycleData[cSelect]],current.height,current.width/8);
}
//---------------------------------------------------------------------------
void SCREENSendCycle::DoKeyCWork(void)
{
try {
    if (cSelect==0 || cSelect==1) {
        cycleData[cSelect]=12;
        lcd240x128.DISPLAY_GRAPHIC_XY(current.X,current.Y,word24x24[cycleData[cSelect]],current.height,current.width/8);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENSendCycle::DoKeyDWork(void)
{
try {
    if (cSelect==0 || cSelect==1) {
        cycleData[cSelect]=13;
        lcd240x128.DISPLAY_GRAPHIC_XY(current.X,current.Y,word24x24[cycleData[cSelect]],current.height,current.width/8);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENSendCycle::DoKeyEWork(void)
{
try {
    if (cSelect==0 || cSelect==1) {
        cycleData[cSelect]=14;
        lcd240x128.DISPLAY_GRAPHIC_XY(current.X,current.Y,word24x24[cycleData[cSelect]],current.height,current.width/8);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENSendCycle::DoKeyFWork(void)
{
try {
    if (cSelect==0 || cSelect==1) {
        cycleData[cSelect]=15;
        lcd240x128.DISPLAY_GRAPHIC_XY(current.X,current.Y,word24x24[cycleData[cSelect]],current.height,current.width/8);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENSendCycle::DoKeyF1Work(void)
{
    screenMain.DisplayMain();
}
//---------------------------------------------------------------------------
void SCREENSendCycle::DoKeyF4Work(void)
{
	//screenCtlSetup.DisplayCtlSetup();
	screenV3Related.DisplayV3Related();//V3相關設定
}
//---------------------------------------------------------------------------
void SCREENSendCycle::DoKeyEnterWork(void)
{
//�ǿ�g��:[0]:
//(A(10):���� B(11):�@�� C(12):��� D(13):���� E(14):�@�� F(15):���� 0:�ܤƶǰe)
try {
    if (smem.GetDbOperStat()==0 || smem.vLoadCenterConnectStatus()==false) {
        if (smem.vGet0FCommandSet()>0) {

          //OTMARKPRINTF  printf("%d,%d\n",cycleData[0],cycleData[1]);


            if ((cycleData[0]==0 || (cycleData[0]>=10 && cycleData[0]<=15)) &&
                (cycleData[1]==0 || (cycleData[1]>=10 && cycleData[1]<=15))) {
                 int tempData=0;

                 for (int i=0;i<2;i++) {
                      if(cycleData[i]==10) tempData = 0xFF;                        //stop send
                      else if(cycleData[i]==11) tempData=1;
                      else if(cycleData[i]==12) tempData=2;
                      else if(cycleData[i]==13) tempData=3;
                      else if(cycleData[i]==14) tempData=4;
                      else if(cycleData[i]==15) tempData=5;
                      else if(cycleData[i]==0) tempData = 0;                 //send change

                      unsigned char data[6]={0};
                      data[0] = 0x5F;
                      data[1] = 0x3F;
                      data[2] = i+1;
                      data[3] = (BYTE)tempData;

                      MESSAGEOK _MSG;
                      _MSG = oDataToMessageOK.vPackageINFOTo92Protocol(data, 4,false);
                      _MSG.InnerOrOutWard=cComingFromScreen;
                      writeJob.WriteWorkByMESSAGEOUT(_MSG);

                 }

                 char message[250]={0};
                 sprintf(message,"Set Traffic Transmit Signal Light Cycle:[%#d] ,Step change Cycle:[%#d] ,Real Detect Cycle:[%#d], Event Detect Cycle:[%#d]",cycleData[0],cycleData[1]);
                 smem.vWriteMsgToDOM(message);

                 //screenMain.DisplayMain();
                 screenCtlSetup.DisplayCtlSetup();
             }
        } else {
            smem.SetLastFace(cSENDCYCLE);
            screenABOErr.DisplayABOErr();
        }
    } else {
        smem.SetLastFace(cSENDCYCLE);
        screenLockdb.DisplayLockdb();
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENSendCycle::DoKeyUPWork(void)
{
try {
    clearSelectPoint16x16(data[cSelect].X,data[cSelect].Y,data[cSelect].height,data[cSelect].width/8);
    if (cSelect==0) cSelect=1;
    cSelect--;
    setSelectPoint16x16(data[cSelect].X,data[cSelect].Y,data[cSelect].height,data[cSelect].width/8);
    DisplayCycle(cSelect);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENSendCycle::DoKeyDOWNWork(void)
{
try {
    clearSelectPoint16x16(data[cSelect].X,data[cSelect].Y,data[cSelect].height,data[cSelect].width/8);
    cSelect++;
    if (cSelect>=1) cSelect=1;
    setSelectPoint16x16(data[cSelect].X,data[cSelect].Y,data[cSelect].height,data[cSelect].width/8);
    DisplayCycle(cSelect);
  } catch (...) {}
}
//---------------------------------------------------------------------------
