#include "screenActuateArwenVDInfo.h"
#include "LCD240x128.h"
//#include "screenLogo.h"
#include "SCREENMain.h"
#include "screenRunTableList.h"
#include "CSTC.h"
#include "ScreenActuateArwenMenu.h"
#include <stdio.h>

#include "SMEM.h"
//---------------------------------------------------------------------------
ScreenActuateArwenVDINFO screenActuateArwenVDINFO;
//---------------------------------------------------------------------------
ScreenActuateArwenVDINFO::ScreenActuateArwenVDINFO(void)
{
    loadBitmapFromFile();
    initDispWord();
    cSelect=0;
    m_vdNowCount = 0;
}
//---------------------------------------------------------------------------
ScreenActuateArwenVDINFO::~ScreenActuateArwenVDINFO(void)
{
}
//---------------------------------------------------------------------------
void ScreenActuateArwenVDINFO::loadBitmapFromFile(void)
{
try {
    FILE *bitmap;
    bitmap=fopen("//cct//bitmap//backGround//actmenu//ActuateDetectorInfo.bit","rb");
    if (bitmap) {
        fread(ucChainStatusBitmap,3840,1,bitmap);
        fclose(bitmap);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenActuateArwenVDINFO::DisplayArwenDetectorInfo(void)
{
try {
    smem.SetcFace(cACTDetector);
    lcd240x128.DISPLAY_GRAPHIC(0,ucChainStatusBitmap,128,30);
    cSelect=0;
    //vRefreshChainStatusData();
    printf ("ScreenActuateArwenVDINFO::DisplayArwenDetectorInfo cPedPushButton.GetVirtualVDMode():%d\n",smem.cPedPushButton.GetVirtualVDMode());
    //if(smem.cPedPushButton.GetVirtualVDMode()==0){//虛擬VD
    vRefreshshowDetectorData();
    //}else{
    //vRefreshshowPEDData();
    //}

  } catch (...) {}
}

//---------------------------------------------------------------------------
void ScreenActuateArwenVDINFO::SetDetectorZoneStatus(int id ,int Zone,int status)
{
try {
        //printf("SetDetectorZoneStatus status[%d][%d]:%d\n",id,Zone,status);
        m_VDZoneStatus[id][Zone]=status;

  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenActuateArwenVDINFO::SetDetectorZoneStopped(int id ,int Zone,int Stopped)
{
try {
        //printf("SetDetectorZoneStopped status[%d][%d]:%d\n",id,Zone,Stopped);
        m_VDZoneStopped[id][Zone]=Stopped;
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenActuateArwenVDINFO::detectorWatchDog()
{
try {
    for(int i=0;i<10;i++){
        if(m_VDseconds[i]<255){
            m_VDseconds[i]++;
        }
    }

  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenActuateArwenVDINFO::detectorClear(int id)
{
try {
         m_VDseconds[id]=0;
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenActuateArwenVDINFO::vRefreshshowPEDData()
{
    unsigned short int displanNo[5]={0},tmp_count=0,displanStatus[5][4]={0},displanStopped[5][4]={0},displayWatchDogSec[5];
    unsigned short int _tmpData1=0;
    unsigned int vdSta,vdStopped;
    int _tmpCount=0;
    /*
    for(int i=0;i<8;i++){
        for(int j=0;j<8;j++){
            if(smem.vGetVDPhaseTriggerSwitch(i)){
                    if(smem.vGetVDPhaseMapTable(i,j) != 0){
                        displanNo[tmp_count]=smem.vGetVDPhaseMapTable(i,j);
                        for(int k=0;k<4;k++){
                        displanStatus[tmp_count][k]=m_VDZoneStatus[i][j][k];
                        displanStopped[tmp_count][k]=m_VDZoneStopped[i][j][k];
                        }
                        displayWatchDogSec[tmp_count]=m_VDseconds[i][j];
                        tmp_count++;
                        if(tmp_count >=4){break;}
                    }
            }
        }
    }
    */
    for(int i = 0;i<m_vdNowCount;i++){
        displayWatchDogSec[i]=m_VDseconds[i];
        displayWatchDogSec[i]=m_VDseconds[i];
        for(int j=0;j<4;j++){
            displanStatus[i][j]=m_VDZoneStatus[i][j];
            displanStopped[i][j]=m_VDZoneStopped[i][j];
        }
    }
    for(int i=0;i<m_vdNowCount;i++){
        //Lcn
        if(displanNo[i]>=1 && displanNo[i]<1000){
            _tmpData1 = displanNo[i]/1000;
            lcd240x128.DISPLAY_GRAPHIC_XY(_vdZoneNo[i][0].X,_vdZoneNo[i][0].Y,word8x16[_tmpData1],_vdZoneNo[i][0].height,_vdZoneNo[i][0].width/8);
            _tmpData1 = (displanNo[i]/100);
            lcd240x128.DISPLAY_GRAPHIC_XY(_vdZoneNo[i][1].X,_vdZoneNo[i][1].Y,word8x16[_tmpData1],_vdZoneNo[i][1].height,_vdZoneNo[i][1].width/8);
            _tmpData1 = (displanNo[i]/10)%10;
            lcd240x128.DISPLAY_GRAPHIC_XY(_vdZoneNo[i][2].X,_vdZoneNo[i][2].Y,word8x16[_tmpData1],_vdZoneNo[i][2].height,_vdZoneNo[i][2].width/8);
            _tmpData1 = displanNo[i]%10;
            lcd240x128.DISPLAY_GRAPHIC_XY(_vdZoneNo[i][3].X,_vdZoneNo[i][3].Y,word8x16[_tmpData1],_vdZoneNo[i][3].height,_vdZoneNo[i][3].width/8);
        }else if(displanNo[i]>=1000 && displanNo[i]<10000){
            _tmpData1 = displanNo[i]/1000;
            lcd240x128.DISPLAY_GRAPHIC_XY(_vdZoneNo[i][0].X,_vdZoneNo[i][0].Y,word8x16[_tmpData1],_vdZoneNo[i][0].height,_vdZoneNo[i][0].width/8);
            _tmpData1 = (displanNo[i]/100)%10;
            lcd240x128.DISPLAY_GRAPHIC_XY(_vdZoneNo[i][1].X,_vdZoneNo[i][1].Y,word8x16[_tmpData1],_vdZoneNo[i][1].height,_vdZoneNo[i][1].width/8);
            _tmpData1 = (displanNo[i]/10)%10;
            lcd240x128.DISPLAY_GRAPHIC_XY(_vdZoneNo[i][2].X,_vdZoneNo[i][2].Y,word8x16[_tmpData1],_vdZoneNo[i][2].height,_vdZoneNo[i][2].width/8);
            _tmpData1 = displanNo[i]%10;
            lcd240x128.DISPLAY_GRAPHIC_XY(_vdZoneNo[i][3].X,_vdZoneNo[i][3].Y,word8x16[_tmpData1],_vdZoneNo[i][3].height,_vdZoneNo[i][3].width/8);
        }else if(displanNo[i]>=10000){
            _tmpData1 = (displanNo[i]/1000)%10;
            lcd240x128.DISPLAY_GRAPHIC_XY(_vdZoneNo[i][0].X,_vdZoneNo[i][0].Y,word8x16[_tmpData1],_vdZoneNo[i][0].height,_vdZoneNo[i][0].width/8);
            _tmpData1 = (displanNo[i]/100)%10;
            lcd240x128.DISPLAY_GRAPHIC_XY(_vdZoneNo[i][1].X,_vdZoneNo[i][1].Y,word8x16[_tmpData1],_vdZoneNo[i][1].height,_vdZoneNo[i][1].width/8);
            _tmpData1 = (displanNo[i]/10)%10;
            lcd240x128.DISPLAY_GRAPHIC_XY(_vdZoneNo[i][2].X,_vdZoneNo[i][2].Y,word8x16[_tmpData1],_vdZoneNo[i][2].height,_vdZoneNo[i][2].width/8);
            _tmpData1 = displanNo[i]%10;
            lcd240x128.DISPLAY_GRAPHIC_XY(_vdZoneNo[i][3].X,_vdZoneNo[i][3].Y,word8x16[_tmpData1],_vdZoneNo[i][3].height,_vdZoneNo[i][3].width/8);
        }



        for(int j =0 ;j<smem.cPedPushButton.GetDevCount();j++){
        //printf("displanNo[%d]:%d == cPedPushButton.GetDevID[%d]:%d && cPedPushButton.GetWatchDogStatus[%d]:%d\n",i,displanNo[i],j,smem.cPedPushButton.GetDevID(j),j,smem.cPedPushButton.GetWatchDogStatus(j));
        if(displanNo[i] == smem.cPedPushButton.GetDevID(j) && smem.cPedPushButton.GetWatchDogStatus(j)==true){
        //if(displayWatchDogSec[i]<=3){
        //Status
        smem.cPedPushButton.GetVirtualVDStaSttopped(j,vdSta,vdStopped);
        //printf("smem.cPedPushButton.GetVirtualVDStaSttopped[%d] - sta:%d stopped:%d\n",j,vdSta,vdStopped);
        lcd240x128.DISPLAY_GRAPHIC_XY(_vdZoneStatus[i][0].X,_vdZoneStatus[i][0].Y,word8x16[vdSta],_vdZoneStatus[i][0].height,_vdZoneStatus[i][0].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(_vdZoneStatus[i][1].X,_vdZoneStatus[i][1].Y,word8x16[vdSta],_vdZoneStatus[i][1].height,_vdZoneStatus[i][1].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(_vdZoneStatus[i][2].X,_vdZoneStatus[i][2].Y,word8x16[vdSta],_vdZoneStatus[i][2].height,_vdZoneStatus[i][2].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(_vdZoneStatus[i][3].X,_vdZoneStatus[i][3].Y,word8x16[vdSta],_vdZoneStatus[i][3].height,_vdZoneStatus[i][3].width/8);

        //Stopped

        for(int k=0;k<4;k++){
            if(k==0){_tmpCount=0;}else if(k==1){_tmpCount=3;}else if(k==2){_tmpCount=6;}else if(k==3){_tmpCount=9;}
        if(vdStopped<=99 && vdStopped>=0){
            _tmpData1 = vdStopped/100;
            lcd240x128.DISPLAY_GRAPHIC_XY(_vdZoneStopped[i][k][_tmpCount].X,_vdZoneStopped[i][k][_tmpCount].Y,word8x16[_tmpData1],_vdZoneStopped[i][k][_tmpCount].height,_vdZoneStopped[i][k][_tmpCount].width/8);
            _tmpData1 = vdStopped/10;
            lcd240x128.DISPLAY_GRAPHIC_XY(_vdZoneStopped[i][k][_tmpCount+1].X,_vdZoneStopped[i][k][_tmpCount+1].Y,word8x16[_tmpData1],_vdZoneStopped[i][k][_tmpCount+1].height,_vdZoneStopped[i][k][_tmpCount+1].width/8);
            _tmpData1 = vdStopped%10;
            lcd240x128.DISPLAY_GRAPHIC_XY(_vdZoneStopped[i][k][_tmpCount+2].X,_vdZoneStopped[i][k][_tmpCount+2].Y,word8x16[_tmpData1],_vdZoneStopped[i][k][_tmpCount+2].height,_vdZoneStopped[i][k][_tmpCount+2].width/8);

        }else if(vdStopped >=100 && vdStopped<=999){
            _tmpData1 = vdStopped/100;
            lcd240x128.DISPLAY_GRAPHIC_XY(_vdZoneStopped[i][k][_tmpCount].X,_vdZoneStopped[i][k][_tmpCount].Y,word8x16[_tmpData1],_vdZoneStopped[i][k][_tmpCount].height,_vdZoneStopped[i][k][_tmpCount].width/8);
            _tmpData1 = (vdStopped/10)%10;
            lcd240x128.DISPLAY_GRAPHIC_XY(_vdZoneStopped[i][k][_tmpCount+1].X,_vdZoneStopped[i][k][_tmpCount+1].Y,word8x16[_tmpData1],_vdZoneStopped[i][k][_tmpCount+1].height,_vdZoneStopped[i][k][_tmpCount+1].width/8);
            _tmpData1 = vdStopped%10;
            lcd240x128.DISPLAY_GRAPHIC_XY(_vdZoneStopped[i][k][_tmpCount+2].X,_vdZoneStopped[i][k][_tmpCount+2].Y,word8x16[_tmpData1],_vdZoneStopped[i][k][_tmpCount+2].height,_vdZoneStopped[i][k][_tmpCount+2].width/8);
        }else if(vdStopped>999){
            lcd240x128.DISPLAY_GRAPHIC_XY(_vdZoneStopped[i][k][_tmpCount].X,_vdZoneStopped[i][k][_tmpCount].Y,word8x16[0x9],_vdZoneStopped[i][k][_tmpCount].height,_vdZoneStopped[i][k][_tmpCount].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(_vdZoneStopped[i][k][_tmpCount+1].X,_vdZoneStopped[i][k][_tmpCount+1].Y,word8x16[0x9],_vdZoneStopped[i][k][_tmpCount+1].height,_vdZoneStopped[i][k][_tmpCount+1].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(_vdZoneStopped[i][k][_tmpCount+2].X,_vdZoneStopped[i][k][_tmpCount+2].Y,word8x16[0x9],_vdZoneStopped[i][k][_tmpCount+2].height,_vdZoneStopped[i][k][_tmpCount+2].width/8);
        }
        }

        //Line
        lcd240x128.DISPLAY_GRAPHIC_XY(_vdZoneWatchDog[i].X,_vdZoneWatchDog[i].Y,word8x16[1],_vdZoneWatchDog[i].height,_vdZoneWatchDog[i].width/8);
        }else if(displanNo[i] == smem.cPedPushButton.GetDevID(j) && smem.cPedPushButton.GetWatchDogStatus(j)==false){
            //Status
        lcd240x128.DISPLAY_GRAPHIC_XY(_vdZoneStatus[i][0].X,_vdZoneStatus[i][0].Y,word8x16[0xF],_vdZoneStatus[i][0].height,_vdZoneStatus[i][0].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(_vdZoneStatus[i][1].X,_vdZoneStatus[i][1].Y,word8x16[0xF],_vdZoneStatus[i][1].height,_vdZoneStatus[i][1].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(_vdZoneStatus[i][2].X,_vdZoneStatus[i][2].Y,word8x16[0xF],_vdZoneStatus[i][2].height,_vdZoneStatus[i][2].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(_vdZoneStatus[i][3].X,_vdZoneStatus[i][3].Y,word8x16[0xF],_vdZoneStatus[i][3].height,_vdZoneStatus[i][3].width/8);
        for(int k=0;k<4;k++){
            if(k==0){_tmpCount=0;}else if(k==1){_tmpCount=3;}else if(k==2){_tmpCount=6;}else if(k==3){_tmpCount=9;}
            lcd240x128.DISPLAY_GRAPHIC_XY(_vdZoneStopped[i][k][_tmpCount].X,_vdZoneStopped[i][k][_tmpCount].Y,word8x16[0xF],_vdZoneStopped[i][k][_tmpCount].height,_vdZoneStopped[i][k][_tmpCount].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(_vdZoneStopped[i][k][_tmpCount+1].X,_vdZoneStopped[i][k][_tmpCount+1].Y,word8x16[0xF],_vdZoneStopped[i][k][_tmpCount+1].height,_vdZoneStopped[i][k][_tmpCount+1].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(_vdZoneStopped[i][k][_tmpCount+2].X,_vdZoneStopped[i][k][_tmpCount+2].Y,word8x16[0xF],_vdZoneStopped[i][k][_tmpCount+2].height,_vdZoneStopped[i][k][_tmpCount+2].width/8);
        }
        //Line
        lcd240x128.DISPLAY_GRAPHIC_XY(_vdZoneWatchDog[i].X,_vdZoneWatchDog[i].Y,word8x16[0],_vdZoneWatchDog[i].height,_vdZoneWatchDog[i].width/8);
        }
        }
    }
}
//---------------------------------------------------------------------------
void ScreenActuateArwenVDINFO::vRefreshshowDetectorData()
{
    unsigned short int displanNo[10]={0},tmp_count=0,displanStatus[10][4]={0},displanStopped[10][4]={0},displayWatchDogSec[10];
    unsigned short int _tmpData1=0;
    int _tmpCount=0;
    /*
    for(int i=0;i<8;i++){
        for(int j=0;j<8;j++){
            if(smem.vGetVDPhaseTriggerSwitch(i)){
                    if(smem.vGetVDPhaseMapTable(i,j) != 0){
                        displanNo[tmp_count]=smem.vGetVDPhaseMapTable(i,j);
                        for(int k=0;k<4;k++){
                        displanStatus[tmp_count][k]=m_VDZoneStatus[i][j][k];
                        displanStopped[tmp_count][k]=m_VDZoneStopped[i][j][k];
                        }
                        displayWatchDogSec[tmp_count]=m_VDseconds[i][j];
                        tmp_count++;
                        if(tmp_count >=4){break;}
                    }
            }
        }
    }
    */
    //printf("vRefreshshowDetectorData nowCount:%d\n",m_vdNowCount);
    //for(int i = 0;i<m_vdNowCount;i++){
    for(int i = 0;i<m_vdNowCount;i++){
        displanNo[i]=m_VDlcn[i];
        displayWatchDogSec[i]=m_VDseconds[i];
        //printf("VDLCN:%d WathcDogSec:%d\n",displanNo[i],displayWatchDogSec[i]);
        for(int j=0;j<4;j++){
            displanStatus[i][j]=m_VDZoneStatus[i][j];
            displanStopped[i][j]=m_VDZoneStopped[i][j];
            //printf("displanStatus[%d][%d]:%d stopped[%d][%d]:%d\n",i,j,displanStatus[i][j],i,j,displanStopped[i][j]);
        }
    }

    //for(int i=0;i<m_vdNowCount;i++){
    for(int i=0;i<4;i++){
        if(i+(cSelect*4)<m_vdNowCount){

        //Lcn
        if(displanNo[i+(cSelect*4)]>=1 && displanNo[i+(cSelect*4)]<1000){
            _tmpData1 = displanNo[i+(cSelect*4)]/1000;
            lcd240x128.DISPLAY_GRAPHIC_XY(_vdZoneNo[i][0].X,_vdZoneNo[i][0].Y,word8x16[_tmpData1],_vdZoneNo[i][0].height,_vdZoneNo[i][0].width/8);
            _tmpData1 = (displanNo[i+(cSelect*4)]/100);
            lcd240x128.DISPLAY_GRAPHIC_XY(_vdZoneNo[i][1].X,_vdZoneNo[i][1].Y,word8x16[_tmpData1],_vdZoneNo[i][1].height,_vdZoneNo[i][1].width/8);
            _tmpData1 = (displanNo[i+(cSelect*4)]/10)%10;
            lcd240x128.DISPLAY_GRAPHIC_XY(_vdZoneNo[i][2].X,_vdZoneNo[i][2].Y,word8x16[_tmpData1],_vdZoneNo[i][2].height,_vdZoneNo[i][2].width/8);
            _tmpData1 = displanNo[i+(cSelect*4)]%10;
            lcd240x128.DISPLAY_GRAPHIC_XY(_vdZoneNo[i][3].X,_vdZoneNo[i][3].Y,word8x16[_tmpData1],_vdZoneNo[i][3].height,_vdZoneNo[i][3].width/8);
        }else if(displanNo[i+(cSelect*4)]>=1000 && displanNo[i+(cSelect*4)]<10000){
            _tmpData1 = displanNo[i+(cSelect*4)]/1000;
            lcd240x128.DISPLAY_GRAPHIC_XY(_vdZoneNo[i][0].X,_vdZoneNo[i][0].Y,word8x16[_tmpData1],_vdZoneNo[i][0].height,_vdZoneNo[i][0].width/8);
            _tmpData1 = (displanNo[i+(cSelect*4)]/100)%10;
            lcd240x128.DISPLAY_GRAPHIC_XY(_vdZoneNo[i][1].X,_vdZoneNo[i][1].Y,word8x16[_tmpData1],_vdZoneNo[i][1].height,_vdZoneNo[i][1].width/8);
            _tmpData1 = (displanNo[i+(cSelect*4)]/10)%10;
            lcd240x128.DISPLAY_GRAPHIC_XY(_vdZoneNo[i][2].X,_vdZoneNo[i][2].Y,word8x16[_tmpData1],_vdZoneNo[i][2].height,_vdZoneNo[i][2].width/8);
            _tmpData1 = displanNo[i+(cSelect*4)]%10;
            lcd240x128.DISPLAY_GRAPHIC_XY(_vdZoneNo[i][3].X,_vdZoneNo[i][3].Y,word8x16[_tmpData1],_vdZoneNo[i][3].height,_vdZoneNo[i][3].width/8);
        }else if(displanNo[i+(cSelect*4)]>=10000){
            _tmpData1 = (displanNo[i+(cSelect*4)]/1000)%10;
            lcd240x128.DISPLAY_GRAPHIC_XY(_vdZoneNo[i][0].X,_vdZoneNo[i][0].Y,word8x16[_tmpData1],_vdZoneNo[i][0].height,_vdZoneNo[i][0].width/8);
            _tmpData1 = (displanNo[i+(cSelect*4)]/100)%10;
            lcd240x128.DISPLAY_GRAPHIC_XY(_vdZoneNo[i][1].X,_vdZoneNo[i][1].Y,word8x16[_tmpData1],_vdZoneNo[i][1].height,_vdZoneNo[i][1].width/8);
            _tmpData1 = (displanNo[i+(cSelect*4)]/10)%10;
            lcd240x128.DISPLAY_GRAPHIC_XY(_vdZoneNo[i][2].X,_vdZoneNo[i][2].Y,word8x16[_tmpData1],_vdZoneNo[i][2].height,_vdZoneNo[i][2].width/8);
            _tmpData1 = displanNo[i+(cSelect*4)]%10;
            lcd240x128.DISPLAY_GRAPHIC_XY(_vdZoneNo[i][3].X,_vdZoneNo[i][3].Y,word8x16[_tmpData1],_vdZoneNo[i][3].height,_vdZoneNo[i][3].width/8);
        }
        if(displayWatchDogSec[i+(cSelect*4)]<=3){
        //Status
        for(int k=0;k<4;k++){
            lcd240x128.DISPLAY_GRAPHIC_XY(_vdZoneStatus[i][k].X,_vdZoneStatus[i][k].Y,word8x16[displanStatus[i+(cSelect*4)][k]],_vdZoneStatus[i][k].height,_vdZoneStatus[i][k].width/8);
            //printf("zone[%d][%d]----ShowVDSta[%d][%d]:%d\n",i,k,i+(cSelect*4),k,displanStatus[i+(cSelect*4)][k]);
        }
        /*
        lcd240x128.DISPLAY_GRAPHIC_XY(_vdZoneStatus[i][0].X,_vdZoneStatus[i][0].Y,word8x16[displanStatus[i+(cSelect*4)][0]],_vdZoneStatus[i][0].height,_vdZoneStatus[i][0].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(_vdZoneStatus[i][1].X,_vdZoneStatus[i][1].Y,word8x16[displanStatus[i+(cSelect*4)][1]],_vdZoneStatus[i][1].height,_vdZoneStatus[i][1].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(_vdZoneStatus[i][2].X,_vdZoneStatus[i][2].Y,word8x16[displanStatus[i+(cSelect*4)][2]],_vdZoneStatus[i][2].height,_vdZoneStatus[i][2].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(_vdZoneStatus[i][3].X,_vdZoneStatus[i][3].Y,word8x16[displanStatus[i+(cSelect*4)][3]],_vdZoneStatus[i][3].height,_vdZoneStatus[i][3].width/8);
        */
        //Stopped

        for(int k=0;k<4;k++){
            if(k==0){_tmpCount=0;}else if(k==1){_tmpCount=3;}else if(k==2){_tmpCount=6;}else if(k==3){_tmpCount=9;}
        if(displanStopped[i+(cSelect*4)][k]<=99 && displanStopped[i+(cSelect*4)][k]>=0){
            _tmpData1 = displanStopped[i+(cSelect*4)][k]/100;
            lcd240x128.DISPLAY_GRAPHIC_XY(_vdZoneStopped[i][k][_tmpCount].X,_vdZoneStopped[i][k][_tmpCount].Y,word8x16[_tmpData1],_vdZoneStopped[i][k][_tmpCount].height,_vdZoneStopped[i][k][_tmpCount].width/8);
            _tmpData1 = displanStopped[i+(cSelect*4)][k]/10;
            lcd240x128.DISPLAY_GRAPHIC_XY(_vdZoneStopped[i][k][_tmpCount+1].X,_vdZoneStopped[i][k][_tmpCount+1].Y,word8x16[_tmpData1],_vdZoneStopped[i][k][_tmpCount+1].height,_vdZoneStopped[i][k][_tmpCount+1].width/8);
            _tmpData1 = displanStopped[i+(cSelect*4)][k]%10;
            lcd240x128.DISPLAY_GRAPHIC_XY(_vdZoneStopped[i][k][_tmpCount+2].X,_vdZoneStopped[i][k][_tmpCount+2].Y,word8x16[_tmpData1],_vdZoneStopped[i][k][_tmpCount+2].height,_vdZoneStopped[i][k][_tmpCount+2].width/8);

        }else if(displanStopped[i+(cSelect*4)][k] >=100 && displanStopped[i+(cSelect*4)][k]<=999){
            _tmpData1 = displanStopped[i+(cSelect*4)][k]/100;
            lcd240x128.DISPLAY_GRAPHIC_XY(_vdZoneStopped[i][k][_tmpCount].X,_vdZoneStopped[i][k][_tmpCount].Y,word8x16[_tmpData1],_vdZoneStopped[i][k][_tmpCount].height,_vdZoneStopped[i][k][_tmpCount].width/8);
            _tmpData1 = (displanStopped[i+(cSelect*4)][k]/10)%10;
            lcd240x128.DISPLAY_GRAPHIC_XY(_vdZoneStopped[i][k][_tmpCount+1].X,_vdZoneStopped[i][k][_tmpCount+1].Y,word8x16[_tmpData1],_vdZoneStopped[i][k][_tmpCount+1].height,_vdZoneStopped[i][k][_tmpCount+1].width/8);
            _tmpData1 = displanStopped[i+(cSelect*4)][k]%10;
            lcd240x128.DISPLAY_GRAPHIC_XY(_vdZoneStopped[i][k][_tmpCount+2].X,_vdZoneStopped[i][k][_tmpCount+2].Y,word8x16[_tmpData1],_vdZoneStopped[i][k][_tmpCount+2].height,_vdZoneStopped[i][k][_tmpCount+2].width/8);
        }else if(displanStopped[i+(cSelect*4)][k]>999){
            lcd240x128.DISPLAY_GRAPHIC_XY(_vdZoneStopped[i][k][_tmpCount].X,_vdZoneStopped[i][k][_tmpCount].Y,word8x16[0x9],_vdZoneStopped[i][k][_tmpCount].height,_vdZoneStopped[i][k][_tmpCount].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(_vdZoneStopped[i][k][_tmpCount+1].X,_vdZoneStopped[i][k][_tmpCount+1].Y,word8x16[0x9],_vdZoneStopped[i][k][_tmpCount+1].height,_vdZoneStopped[i][k][_tmpCount+1].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(_vdZoneStopped[i][k][_tmpCount+2].X,_vdZoneStopped[i][k][_tmpCount+2].Y,word8x16[0x9],_vdZoneStopped[i][k][_tmpCount+2].height,_vdZoneStopped[i][k][_tmpCount+2].width/8);
        }
        }

        //Line
        lcd240x128.DISPLAY_GRAPHIC_XY(_vdZoneWatchDog[i].X,_vdZoneWatchDog[i].Y,word8x16[1],_vdZoneWatchDog[i].height,_vdZoneWatchDog[i].width/8);
        }else{
            //Status
        lcd240x128.DISPLAY_GRAPHIC_XY(_vdZoneStatus[i][0].X,_vdZoneStatus[i][0].Y,word8x16[0xF],_vdZoneStatus[i][0].height,_vdZoneStatus[i][0].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(_vdZoneStatus[i][1].X,_vdZoneStatus[i][1].Y,word8x16[0xF],_vdZoneStatus[i][1].height,_vdZoneStatus[i][1].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(_vdZoneStatus[i][2].X,_vdZoneStatus[i][2].Y,word8x16[0xF],_vdZoneStatus[i][2].height,_vdZoneStatus[i][2].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(_vdZoneStatus[i][3].X,_vdZoneStatus[i][3].Y,word8x16[0xF],_vdZoneStatus[i][3].height,_vdZoneStatus[i][3].width/8);
        for(int k=0;k<4;k++){
            if(k==0){_tmpCount=0;}else if(k==1){_tmpCount=3;}else if(k==2){_tmpCount=6;}else if(k==3){_tmpCount=9;}
            lcd240x128.DISPLAY_GRAPHIC_XY(_vdZoneStopped[i][k][_tmpCount].X,_vdZoneStopped[i][k][_tmpCount].Y,word8x16[0xF],_vdZoneStopped[i][k][_tmpCount].height,_vdZoneStopped[i][k][_tmpCount].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(_vdZoneStopped[i][k][_tmpCount+1].X,_vdZoneStopped[i][k][_tmpCount+1].Y,word8x16[0xF],_vdZoneStopped[i][k][_tmpCount+1].height,_vdZoneStopped[i][k][_tmpCount+1].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(_vdZoneStopped[i][k][_tmpCount+2].X,_vdZoneStopped[i][k][_tmpCount+2].Y,word8x16[0xF],_vdZoneStopped[i][k][_tmpCount+2].height,_vdZoneStopped[i][k][_tmpCount+2].width/8);
        }
        //Line
        lcd240x128.DISPLAY_GRAPHIC_XY(_vdZoneWatchDog[i].X,_vdZoneWatchDog[i].Y,word8x16[0],_vdZoneWatchDog[i].height,_vdZoneWatchDog[i].width/8);
        }
    }
    }
}

//---------------------------------------------------------------------------
void ScreenActuateArwenVDINFO::initDispWord(void)
{
try {
    for(int i=0;i<5;i++){
        for(int j=0;j<4;j++){
            for(int k=0;k<12;k++){
            if(i==0){
                if(k>=0 && k<3 && j==0){
                _vdZoneStopped[i][j][k].X = 56+(8*k);
                _vdZoneStopped[i][j][k].Y = 48;
                }else if(k>=3 && k<6 && j==1){
                _vdZoneStopped[i][j][k].X = 104+(8*(k-3));
                _vdZoneStopped[i][j][k].Y = 48;
                }else if(k>=6 && k<9 && j==2){
                _vdZoneStopped[i][j][k].X = 152+(8*(k-6));
                _vdZoneStopped[i][j][k].Y = 48;
                }else if(k>=9 && k<12 && j==3){
                _vdZoneStopped[i][j][k].X = 200+(8*(k-9));
                _vdZoneStopped[i][j][k].Y = 48;
                }
            }else if(i==1){
                if(k>=0 && k<3 && j==0){
                _vdZoneStopped[i][j][k].X = 56+(8*k);
                _vdZoneStopped[i][j][k].Y = 64;
                }else if(k>=3 && k<6 && j==1){
                _vdZoneStopped[i][j][k].X = 104+(8*(k-3));
                _vdZoneStopped[i][j][k].Y = 64;
                }else if(k>=6 && k<9 && j==2){
                _vdZoneStopped[i][j][k].X = 152+(8*(k-6));
                _vdZoneStopped[i][j][k].Y = 64;
                }else if(k>=9 && k<12 && j==3){
                _vdZoneStopped[i][j][k].X = 200+(8*(k-9));
                _vdZoneStopped[i][j][k].Y = 64;
                }
            }else if(i==2){
                if(k>=0 && k<3 && j==0){
                _vdZoneStopped[i][j][k].X = 56+(8*k);
                _vdZoneStopped[i][j][k].Y = 80;
                }else if(k>=3 && k<6 && j==1){
                _vdZoneStopped[i][j][k].X = 104+(8*(k-3));
                _vdZoneStopped[i][j][k].Y = 80;
                }else if(k>=6 && k<9 && j==2){
                _vdZoneStopped[i][j][k].X = 152+(8*(k-6));
                _vdZoneStopped[i][j][k].Y = 80;
                }else if(k>=9 && k<12 && j==3){
                _vdZoneStopped[i][j][k].X = 200+(8*(k-9));
                _vdZoneStopped[i][j][k].Y = 80;
                }
            }else if(i==3){
                if(k>=0 && k<3 && j==0){
                _vdZoneStopped[i][j][k].X = 56+(8*k);
                _vdZoneStopped[i][j][k].Y = 96;
                }else if(k>=3 && k<6 && j==1){
                _vdZoneStopped[i][j][k].X = 104+(8*(k-3));
                _vdZoneStopped[i][j][k].Y = 96;
                }else if(k>=6 && k<9 && j==2){
                _vdZoneStopped[i][j][k].X = 152+(8*(k-6));
                _vdZoneStopped[i][j][k].Y = 96;
                }else if(k>=9 && k<12 && j==3){
                _vdZoneStopped[i][j][k].X = 200+(8*(k-9));
                _vdZoneStopped[i][j][k].Y = 96;
                }
            }else if(i==4){
                if(k>=0 && k<3 && j==0){
                _vdZoneStopped[i][j][k].X = 56+(8*k);
                _vdZoneStopped[i][j][k].Y = 112;
                }else if(k>=3 && k<6 && j==1){
                _vdZoneStopped[i][j][k].X = 104+(8*(k-3));
                _vdZoneStopped[i][j][k].Y = 112;
                }else if(k>=6 && k<9 && j==2){
                _vdZoneStopped[i][j][k].X = 152+(8*(k-6));
                _vdZoneStopped[i][j][k].Y = 112;
                }else if(k>=9 && k<12 && j==3){
                _vdZoneStopped[i][j][k].X = 200+(8*(k-9));
                _vdZoneStopped[i][j][k].Y = 112;
                }
            }

            _vdZoneStopped[i][j][k].width = 8;
            _vdZoneStopped[i][j][k].height= 16;
            }
        }
    }


  for(int i=0;i<5;i++){
      for(int j=0;j<4;j++){
          if(i==0){
            _vdZoneNo[i][j].X = 0+(8*j);
            _vdZoneNo[i][j].Y = 48;

            _vdZoneStatus[i][j].X = 40+(48*j);
            _vdZoneStatus[i][j].Y = 48;

          }else if(i==1){
            _vdZoneNo[i][j].X = 0+(8*j);
            _vdZoneNo[i][j].Y = 64;

            _vdZoneStatus[i][j].X = 40+(48*j);
            _vdZoneStatus[i][j].Y = 64;


          }else if(i==2){
            _vdZoneNo[i][j].X = 0+(8*j);
            _vdZoneNo[i][j].Y = 80;

            _vdZoneStatus[i][j].X = 40+(48*j);
            _vdZoneStatus[i][j].Y = 80;

          }else if(i==3){
            _vdZoneNo[i][j].X = 0+(8*j);
            _vdZoneNo[i][j].Y = 96;

            _vdZoneStatus[i][j].X = 40+(48*j);
            _vdZoneStatus[i][j].Y = 96;

          }else if(i==4){
            _vdZoneNo[i][j].X = 0+(8*j);
            _vdZoneNo[i][j].Y = 112;

            _vdZoneStatus[i][j].X = 40+(48*j);
            _vdZoneStatus[i][j].Y = 112;

          }
        _vdZoneNo[i][j].width = 8;
        _vdZoneNo[i][j].height= 16;

        _vdZoneStatus[i][j].width = 8;
        _vdZoneStatus[i][j].height= 16;
      }
      _vdZoneWatchDog[i].X= 232;
      _vdZoneWatchDog[i].Y= 48+(i*16);
      _vdZoneWatchDog[i].width= 8;
      _vdZoneWatchDog[i].height= 16;
  }




  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenActuateArwenVDINFO::doKeyWork(BYTE key)
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
void ScreenActuateArwenVDINFO::doKey0Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenActuateArwenVDINFO::doKey1Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenActuateArwenVDINFO::doKey2Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenActuateArwenVDINFO::doKey3Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenActuateArwenVDINFO::doKey4Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenActuateArwenVDINFO::doKey5Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenActuateArwenVDINFO::doKey6Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenActuateArwenVDINFO::doKey7Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenActuateArwenVDINFO::doKey8Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenActuateArwenVDINFO::doKey9Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenActuateArwenVDINFO::doKeyAWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenActuateArwenVDINFO::doKeyBWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenActuateArwenVDINFO::doKeyCWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenActuateArwenVDINFO::doKeyDWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenActuateArwenVDINFO::doKeyEWork(void)
{
    if(cSelect<=0){
        cSelect=0;
    }else{
        cSelect--;
    }
    lcd240x128.DISPLAY_GRAPHIC(0,ucChainStatusBitmap,128,30);
}
//---------------------------------------------------------------------------
void ScreenActuateArwenVDINFO::doKeyFWork(void)
{
    int siShowPage=m_vdNowCount/4;
    if(cSelect<siShowPage){
        cSelect++;
    }
    if(cSelect>=siShowPage){
        cSelect=siShowPage-1;
    }
    lcd240x128.DISPLAY_GRAPHIC(0,ucChainStatusBitmap,128,30);
}
//---------------------------------------------------------------------------
void ScreenActuateArwenVDINFO::doKeyF1Work(void)
{
//    screenLogo.DisplayLogo();
    screenMain.DisplayMain();
}
//---------------------------------------------------------------------------
void ScreenActuateArwenVDINFO::doKeyF2Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenActuateArwenVDINFO::doKeyF3Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenActuateArwenVDINFO::doKeyF4Work(void)
{
//    screenMain.DisplayMain();
    screenActuateArwenMenu.DisplayActuateArwenMenu();
}
//---------------------------------------------------------------------------
void ScreenActuateArwenVDINFO::doKeyUPWork(void)
{
try {
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenActuateArwenVDINFO::doKeyDOWNWork(void)
{
try {
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenActuateArwenVDINFO::doKeyLEFTWork(void)
{
try {
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenActuateArwenVDINFO::doKeyRIGHTWork(void)
{
try {
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenActuateArwenVDINFO::doKeyEnterWork(void)
{
    switch (cSelect+1) {
        case 1:
          doKey1Work();
        break;
        case 2:
          doKey2Work();
        break;
        case 3:
          doKey3Work();
        break;
        case 4:
          doKey4Work();
        break;
        case 5:
          doKey5Work();
        break;
        case 6:
          doKey6Work();
        break;
        case 7:
          doKey7Work();
        break;
        case 8:
          doKey8Work();
        break;
        case 9:
          doKey9Work();
        break;
        case 10:
          doKey0Work();
        break;
        default:
        break;
    }
}
//---------------------------------------------------------------------------
void ScreenActuateArwenVDINFO::SetactTOTCDetectorCount(unsigned short int tmp)
{
try {
    if(tmp!=m_vdNowCount){
        m_vdNowCount=tmp;
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenActuateArwenVDINFO::SetDetectorLCN(unsigned short int id,unsigned short int lcn)
{
try {
    //printf("ScreenActuateArwenVDINFO::SetDetectorLCN %d lcn:%d\n",id,lcn);
    m_VDlcn[id]=lcn;

  } catch (...) {}
}
