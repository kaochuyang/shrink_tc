//---------------------------------------------------------------------------
#include "PTRAFFIC92VD.h"
#include "CDataToMessageOK.h"
#include "WRITEJOB.h"
#include "CTIMER.h"
#include <stdio.h>
#include <stdlib.h>
#include <error.h>
#include "screenActuateArwenVDInfo.h"
//---------------------------------------------------------------------------
PTRAFFIC92VD::PTRAFFIC92VD(void)
{
try {
  } catch (...) {}
}
//---------------------------------------------------------------------------
PTRAFFIC92VD::~PTRAFFIC92VD(void)
{
}
//---------------------------------------------------------------------------
bool PTRAFFIC92VD::DoWorkViaPTraffic92(MESSAGEOK message)
{
try {

  if(message.packet[7] == 0x6F) //printf(" this is 6F!\n");
  //printf("\nprintfMsg GoTo 92VD error page!!\n\n\n");
  switch (message.packet[8]) {
            case 0x05:
                //printf("ProcessZoneStatus6F05\n");
                //ProcessZoneStatus6F05(message);
            break;
            case 0x08:
                ProcessACTZoneAllStatus6F08(message);
            break;
          default:
            vReturnToCenterNACK(message.packet[7], message.packet[8], 0x0, 0x0);
          break;
  }

  return true;

  } catch (...) {}
}
//---------------------------------------------------------------------------
bool PTRAFFIC92VD::ProcessZoneStatus6F05(MESSAGEOK message)
{
try {
        unsigned short int phase=0,id=0;
        unsigned short int _Lcn=0;

        int status  = 0x00;
	    int stopped = 0;
        for(int i=0;i<8;i++){
            for(int j =0 ;j<8;j++){
                //printf("smem.vGetVDPhaseTriggerSwitch(%d)=%d\n",i,smem.vGetVDPhaseTriggerSwitch(i));
                //printf("LCN:%d smem.vGetVDPhaseMapTable(%d,%d)=:%d\n",message.packet[3]*256+message.packet[4],i,j,smem.vGetVDPhaseMapTable(i,j));

                if(smem.vGetVDPhaseTriggerSwitch(i)){
                    if(smem.vGetVDPhaseMapTable(i,j) == message.packet[3]*256+message.packet[4]){
                        //printf("vGetVDPhaseMapTable [%d][%d]== Lcn\n",i,j);
                        //printf("vGetVDPhaseMapTable [%d][%d]== Lcn\n",i,j);
                        _Lcn = smem.vGetVDPhaseMapTable(i,j);
                        phase = i;
                        id=j;
                        break;
                    }
                }
            }
        }
        if (_Lcn == 0 ){
            return false;
        }

       /*
        for(int i = 0; i < 4; i++){
		  status  = message.packet[9+i*3];
		  stopped = message.packet[10+i*3]*256 + message.packet[11+i*3];
		  screenActuateArwenVDINFO.SetDetectorZoneStatus(phase,id,i,status);//單筆Status
		  screenActuateArwenVDINFO.SetDetectorZoneStopped(phase,id,i,stopped);//單筆Stopped
		  screenActuateArwenVDINFO.detectorClear(phase,id);//watch dog clear
		  //printf("Zone %d: Status:%d Stopped:%d ",i, status,stopped);
	   }
	   */
	   //printf("\n");

} catch (...) {}
}
//---------------------------------------------------------------------------
bool PTRAFFIC92VD::ProcessACTZoneAllStatus6F08(MESSAGEOK message)
{
try{
    int status  = 0x00;
    int stopped = 0;
    //printf("ProcessACTZoneAllStatus6F08 len:%d\n",message.packetLength);
    //printf("ProcessACTZoneAllStatus6F08 len:%d\n",message.packetLength);
    unsigned short int phase=0;
    unsigned short int vdCount = message.packet[9];
    int data_lenght = 10;
    int lcn = 0 ;
    bool unClear = false;
    screenActuateArwenVDINFO.SetactTOTCDetectorCount(vdCount);
    for(int i = 0; i < vdCount ;i++){
        lcn = message.packet[data_lenght]*256+message.packet[data_lenght+1];data_lenght++;data_lenght++;
        //printf("vdLcn:%d\n",lcn);
        screenActuateArwenVDINFO.SetDetectorLCN(i,lcn);
        for(int j = 0; j < 4; j++){
              unClear=false;
              status  = message.packet[data_lenght];                                        data_lenght++;
              stopped = message.packet[data_lenght]*256 + message.packet[data_lenght+1];    data_lenght++;data_lenght++;
              screenActuateArwenVDINFO.SetDetectorZoneStatus(i,j,status);//單筆Status
              screenActuateArwenVDINFO.SetDetectorZoneStopped(i,j,stopped);//單筆Stopped
              //printf("Zone %d: Status:%d Stopped:%d ",i, status,stopped);
              //printf("vd:%d status:%d stopped:%d\n",j,status,stopped);
              if(status==0x0FF){
                  unClear=true;
              }
        }
        if(unClear==false){
        screenActuateArwenVDINFO.detectorClear(i);//watch dog clear
        }
    }
    printf("\n");
} catch (...) {}
}
//ACK------------------------------------------------------------------------
int PTRAFFIC92VD::vReturnToCenterACK(unsigned char ucDevCode,
                                     unsigned char ucControlCode)
{
try{
    unsigned char data[4];

    data[0] = 0x0F;
    data[1] = 0x80;
    data[2] = ucDevCode;
    data[3] = ucControlCode;

    MESSAGEOK _MsgOK;

    _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, 4,true);
    _MsgOK.InnerOrOutWard = cOutWard;
    writeJob.WriteWorkByMESSAGEOUT(_MsgOK);

    return 0;
  } catch (...) {}
}
//---------------------------------------------------------------------------
int PTRAFFIC92VD::vReturnToCenterNACK(unsigned char ucDevCode,
                                     unsigned char ucControlCode,
                                     unsigned char ErrorCode,
                                     unsigned char ParameterNumber)
{
try{
    unsigned char data[6];

    data[0] = 0x0F;
    data[1] = 0x81;
    data[2] = ucDevCode;
    data[3] = ucControlCode;
    data[4] = ErrorCode;
    data[5] = ParameterNumber;

    MESSAGEOK _MsgOK;

    _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, 6,true);
    _MsgOK.InnerOrOutWard = cOutWard;
    writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICECENTER92);

    return 0;
  } catch (...) {}
}


