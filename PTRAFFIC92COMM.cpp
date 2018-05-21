//---------------------------------------------------------------------------
#include "PTRAFFIC92COMM.h"
#include "CDataToMessageOK.h"
#include "WRITEJOB.h"
#include "CTIMER.h"
#include "DIGITALIO.h"
#include "SCREENOnline.h"
#include "SCREENHWReset.h"
#include "CHexToInt.h"r
#include "CSTC.h"

//OT Pass 2
#include "READJOB.h"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <error.h>

//---------------------------------------------------------------------------
PTRAFFIC92COMM::PTRAFFIC92COMM(void)
{
try {
    int i;
    for (i = 0 ; i < 256; i++) {
         bProtocol0FStatus[i] = true;
         bProtocol5FStatus[i] = true;
//         bProtocol6FStatus[i] = true;
    }

//0F
    bProtocol0FStatus[0x80] = false;
    bProtocol0FStatus[0x81] = false;

    bProtocol0FStatus[0x8F] = false;
    bProtocol0FStatus[0x8E] = false;

    bProtocol0FStatus[0x10] = false;
    bProtocol0FStatus[0x90] = false;

    bProtocol0FStatus[0x40] = false;
    bProtocol0FStatus[0xC0] = false;

    bProtocol0FStatus[0x00] = false;

    bProtocol0FStatus[0x11] = false;
    bProtocol0FStatus[0x91] = false;
    bProtocol0FStatus[0x41] = false;
    bProtocol0FStatus[0xC1] = false;

    bProtocol0FStatus[0x12] = false;
    bProtocol0FStatus[0x92] = false;
    bProtocol0FStatus[0x42] = false;
    bProtocol0FStatus[0xC2] = false;
    bProtocol0FStatus[0x02] = false;

    bProtocol0FStatus[0x13] = false;
    bProtocol0FStatus[0x43] = false;
    bProtocol0FStatus[0xC3] = false;

    bProtocol0FStatus[0x14] = false;
    bProtocol0FStatus[0x44] = false;
    bProtocol0FStatus[0xC4] = false;
    bProtocol0FStatus[0x04] = false;

    bProtocol0FStatus[0x15] = false;
    bProtocol0FStatus[0x45] = false;
    bProtocol0FStatus[0xC5] = false;

    bProtocol0FStatus[0x16] = false;
    bProtocol0FStatus[0x46] = false;
    bProtocol0FStatus[0xC6] = false;
    bProtocol0FStatus[0x47] = false;
    bProtocol0FStatus[0xC7] = false;


//------------------------------------- 5F
    bProtocol5FStatus[0x10] = false;
    bProtocol5FStatus[0x40] = false;
    bProtocol5FStatus[0xC0] = false;
    bProtocol5FStatus[0x00] = false;

    bProtocol5FStatus[0x11] = true;
    bProtocol5FStatus[0x41] = true;
    bProtocol5FStatus[0xC1] = true;
    bProtocol5FStatus[0x01] = true;
    bProtocol5FStatus[0x12] = true;
    bProtocol5FStatus[0x42] = true;
    bProtocol5FStatus[0xC2] = true;
    bProtocol5FStatus[0x02] = true;

    bProtocol5FStatus[0x13] = false;
    bProtocol5FStatus[0x43] = false;
    bProtocol5FStatus[0xC3] = false;
    bProtocol5FStatus[0x03] = false;

    bProtocol5FStatus[0x14] = false;
    bProtocol5FStatus[0x44] = false;
    bProtocol5FStatus[0xC4] = false;

    bProtocol5FStatus[0x15] = false;
    bProtocol5FStatus[0x45] = false;
    bProtocol5FStatus[0xC5] = false;

    bProtocol5FStatus[0x16] = false;
    bProtocol5FStatus[0x46] = false;
    bProtocol5FStatus[0xC6] = false;

    bProtocol5FStatus[0x17] = false;
    bProtocol5FStatus[0x47] = false;
    bProtocol5FStatus[0xC7] = false;

    bProtocol5FStatus[0x18] = false;
    bProtocol5FStatus[0x48] = false;
    bProtocol5FStatus[0xC8] = false;

    bProtocol5FStatus[0x19] = true;
    bProtocol5FStatus[0x49] = true;
    bProtocol5FStatus[0xC9] = true;
    bProtocol5FStatus[0x09] = true;
    bProtocol5FStatus[0x1A] = true;
    bProtocol5FStatus[0x4A] = true;
    bProtocol5FStatus[0xCA] = true;
    bProtocol5FStatus[0x1B] = true;
    bProtocol5FStatus[0x4B] = true;
    bProtocol5FStatus[0xCB] = true;

    bProtocol5FStatus[0x1C] = false;
    bProtocol5FStatus[0x4C] = false;
    bProtocol5FStatus[0xCC] = false;
    bProtocol5FStatus[0x0C] = false;

    bProtocol5FStatus[0x1D] = true;

    bProtocol5FStatus[0x1E] = true;
    bProtocol5FStatus[0x4E] = true;
    bProtocol5FStatus[0xCE] = true;

    bProtocol5FStatus[0x2F] = false;
    bProtocol5FStatus[0x5F] = false;
    bProtocol5FStatus[0xDF] = false;

    bProtocol5FStatus[0x31] = true;
    bProtocol5FStatus[0x61] = true;
    bProtocol5FStatus[0xE1] = true;
    bProtocol5FStatus[0x32] = true;
    bProtocol5FStatus[0x62] = true;
    bProtocol5FStatus[0xE2] = true;
    bProtocol5FStatus[0x33] = true;
    bProtocol5FStatus[0x63] = true;
    bProtocol5FStatus[0xE3] = true;
    bProtocol5FStatus[0x31] = true;

    bProtocol5FStatus[0x08] = false;

    bProtocol5FStatus[0x0A] = false;
    bProtocol5FStatus[0x0B] = false;

    bProtocol5FStatus[0x3F] = false;
    bProtocol5FStatus[0x6F] = false;
    bProtocol5FStatus[0xEF] = false;
    bProtocol5FStatus[0x0F] = false;

    bProtocol5FStatus[0x3E] = true;
    bProtocol5FStatus[0x6E] = true;
    bProtocol5FStatus[0xEE] = true;
    bProtocol5FStatus[0x0E] = true;

  } catch (...) {}
}
//---------------------------------------------------------------------------
PTRAFFIC92COMM::~PTRAFFIC92COMM(void)
{
}
//---------------------------------------------------------------------------
bool PTRAFFIC92COMM::DoWorkViaPTraffic92(MESSAGEOK message)
{
try {
    switch (message.packet[8]) {
            case 0x10:
              vRebootIPC_0F10(message);
            break;

            case 0x40:
              vQueryEquipmentNo_0F40(message);
            break;

            case 0x11:
              vSetCommunicationReset_0F11(message);
            break;

            case 0x41:
              vQueryCommunicationReset_0F41(message);

            break;
            case 0x12:
              vSetIPCTime_0F12(message);
            break;

            case 0x42:
              vQueryIPCTime_0F42(message);

            break;
            case 0x13:
              vSetCommandSet_0F13(message);

            break;

            case 0x43:
              vQueryVersion_0F43();
            break;

            case 0x14:
              vSetHardwareCycle_0F14(message);

            break;
            case 0x44:
              vQueryHardwareCycle_0F44(message);

            break;

            case 0x15:
              vSetDBPassword_0F15(message);

            break;
            case 0x45:
              vQueryPassword_0F45();

            break;

            case 0x16:
              vSetDBOperatingMode_0F16(message);

            break;
            case 0x46:
              vQueryDBOperatingMode_0F46();

            break;

            case 0x47:
              vQueryProtocolStatus_0F47(message);

            break;

//OT Pass
            case 0x8E:
              vPass_0F8E(message);
            break;

            //PhysicalOut
            case 0x80:
              writeJob.WritePhysicalOut(message.packet, message.packetLength, DEVICECENTER92);
            break;
            case 0x81:
              writeJob.WritePhysicalOut(message.packet, message.packetLength, DEVICECENTER92);
            break;
            case 0x90:
              writeJob.WritePhysicalOut(message.packet, message.packetLength, DEVICECENTER92);
            break;
            case 0xC0:
              writeJob.WritePhysicalOut(message.packet, message.packetLength, DEVICECENTER92);
            break;
            case 0x00:
              writeJob.WritePhysicalOut(message.packet, message.packetLength, DEVICECENTER92);
            break;
            case 0x91:
              writeJob.WritePhysicalOut(message.packet, message.packetLength, DEVICECENTER92);
            break;
            case 0xC1:
              writeJob.WritePhysicalOut(message.packet, message.packetLength, DEVICECENTER92);
            break;
            case 0x92:
              writeJob.WritePhysicalOut(message.packet, message.packetLength, DEVICECENTER92);
            break;
            case 0xC2:
              writeJob.WritePhysicalOut(message.packet, message.packetLength, DEVICECENTER92);
            break;
            case 0x02:
              writeJob.WritePhysicalOut(message.packet, message.packetLength, DEVICECENTER92);
            break;
            case 0xC3:
              writeJob.WritePhysicalOut(message.packet, message.packetLength, DEVICECENTER92);
            break;
            case 0xC4:
              writeJob.WritePhysicalOut(message.packet, message.packetLength, DEVICECENTER92);
            break;
            case 0x04:
              writeJob.WritePhysicalOut(message.packet, message.packetLength, DEVICECENTER92);
            break;
            case 0xC5:
              writeJob.WritePhysicalOut(message.packet, message.packetLength, DEVICECENTER92);
            break;
            case 0xC6:
              writeJob.WritePhysicalOut(message.packet, message.packetLength, DEVICECENTER92);
            break;
            case 0xC7:
              writeJob.WritePhysicalOut(message.packet, message.packetLength, DEVICECENTER92);
            break;
            case 0x06:
              writeJob.WritePhysicalOut(message.packet, message.packetLength, DEVICECENTER92);
            break;

            default:
              vReturnToCenterNACK(message.packet[7], message.packet[8], 0x0, 0x0);
            break;

//Special case
/*
            case 0x0F:

            if(message.InnerOrOutWard == cOutWard){                                 //92VD�^����
              writeJob.WritePhysicalOut(message.packet, message.packetLength, DEVICECENTER92);
              _intervalTimer.TimersRead_BeforeResetCMOSTime();
              _intervalTimer.TimersReset_AfterResetCMOSTime();
            }
            else if(message.InnerOrOutWard == cInner)                             //SS��92VD
//              vToCenterVehicleData6F0F(message);
            break;

            default:
            break;
*/
    }

    return true;

  } catch (...) {}
}
//---------------------------------------------------------------------------
bool PTRAFFIC92COMM::vRebootIPC_0F10(MESSAGEOK DataMessageIn)
{
try{
    if (DataMessageIn.packet[9] == 0x52 && DataMessageIn.packet[10] == 0x52 && DataMessageIn.packetLength == 14) {


      smem.vSetBOOLData(IFGetResetTCSignal, true);
      smem.vWriteMsgToDOM("Reset TC Signal By Center!");

     //OTMARKPRINTF  printf("%s[Message] [Message] [Message] REBOOT SYSTEM!!!%s\n", ColorRed, ColorNormal);


     //OTMARKPRINTF  printf("ABC 9:%x 10:%x \n", DataMessageIn.packet[9], DataMessageIn.packet[10]);


       vReturnToCenterACK(0x0F, 0x10);


//cheat, for fuck chen-min-zui
       MESSAGEOK _MSG;
       unsigned char data3[4];
       data3[0]  = 0x0F;
       data3[1]  = 0x90;
       data3[2]  = 0x52;
       data3[3]  = 0x52;
       _MSG = oDataToMessageOK.vPackageINFOTo92Protocol(data3, 4, true);
       _MSG.InnerOrOutWard = cOutWard;
       writeJob.WritePhysicalOut(_MSG.packet, _MSG.packetLength, DEVICECENTER92);

//Mask       digitalIO.WriteDigitalIO(0xFF);     //�q�T���m
//Mask       usleep(1500000);
//Mask       stc.Lock_to_Set_Control_Strategy(STRATEGY_FLASH);                        //OTFix 940804
//Mask    digitalIO.WriteDigitalIO(0x00);
                                           //�w�魫�m

// /*Move To CSTC*/
       system("sync");
       system("sync");
       system("reboot");

       screenHWReset.DisplayHWReset();
/* */

       return true;
    }

    else
    {
       if( DataMessageIn.packetLength < 14 ) { vReturnToCenterNACK(0x0F, 0x10, 0x08, 0x00); return false; }
       else if( DataMessageIn.packetLength > 14 ) { vReturnToCenterNACK(0x0F, 0x10, 0x08, DataMessageIn.packetLength - 12); return false; }
       if(DataMessageIn.packet[9] != 0x52) { vReturnToCenterNACK(0x0F, 0x10, 0x04, 0x01); return false; }
       if(DataMessageIn.packet[10] != 0x52) { vReturnToCenterNACK(0x0F, 0x10, 0x04, 0x02); return false; }
    }

  } catch(...) {}
}
//-------------------------------------------------------------------
bool PTRAFFIC92COMM::vReportRebootMsg_0F90()
{
try {
    unsigned char data[4];

    data[0]  = 0x0F;
    data[1]  = 0x90;
    data[2]  = 0x52;
    data[3]  = 0x52;

    MESSAGEOK _MsgOK;
    _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, 4,true);
    _MsgOK.InnerOrOutWard = cOutWard;
//    writeJob.WriteWorkByMESSAGEOUT(_MsgOK);
    writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICECENTER92);

    return true;
  } catch (...) {}
}
//-------------------------------------------------------------------
bool PTRAFFIC92COMM::vQueryEquipmentNo_0F40(MESSAGEOK DataMessageIn)
{
try{

    int iReturnCommandSet = smem.vGet0FCommandSet();
    if(iReturnCommandSet <= 2) { vReturnToCenterNACK(0x0F, 0x40, 0x80, 0x00); return false; } // not include level "O"

    unsigned char cEquipmentNo = DataMessageIn.packet[9];
    if ( (cEquipmentNo <= 0x08  || cEquipmentNo == 0xFF) && DataMessageIn.packetLength == 13)
        vReportEquipmentNo_0FC0(cEquipmentNo);
    else
    {
      if( DataMessageIn.packetLength > 13 ) { vReturnToCenterNACK(0x0F, 0x40, 0x08, DataMessageIn.packetLength - 12); return false; }
      else if( DataMessageIn.packetLength < 13 ) { vReturnToCenterNACK(0x0F, 0x40, 0x08, 0x00); return false; }
      if( cEquipmentNo > 0x08 && cEquipmentNo != 0xFF ) { vReturnToCenterNACK(0x0F, 0x40, 0x04,0x01); return false; }
    }
  } catch(...) {}
}
//-------------------------------------------------------------------
bool PTRAFFIC92COMM::vReportEquipmentNo_0FC0(unsigned char cEquipmentNo)
{
try{

    int iReturnCommandSet = smem.vGet0FCommandSet();
    if(iReturnCommandSet <= 2) { vReturnToCenterNACK(0x0F, 0xC0, 0x80, 0x00); return false; } // not include level "O"

    unsigned char data[6];
    MESSAGEOK _MsgOK;
    _MsgOK.packetLcn=smem.GetAddress();

    if(cEquipmentNo == 0) {

      data[0]  = 0x0F;
      data[1]  = 0xC0;
      data[2]  = 0x00;
      data[3]  = 0x01;

      if( smem.vGetAdjudicateReturnAddressBCDorHEX() == cBCD ) {                     //Select BCD mode
        data[4]=oHexToInt.vIntToFuckingBCD(1,_MsgOK.packetLcn);
        data[5]=oHexToInt.vIntToFuckingBCD(0,_MsgOK.packetLcn);
      }
      else {                                                                      //Normal(HEX) mode
        data[4]=HI(_MsgOK.packetLcn);
        data[5]=LO(_MsgOK.packetLcn);
      }

    _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, 6,true);
    _MsgOK.InnerOrOutWard = cOutWard;
//    writeJob.WriteWorkByMESSAGEOUT(_MsgOK);
    writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICECENTER92);
    return true;
    }
    else {
      data[0]  = 0x0F;
      data[1]  = 0xC0;
      data[2]  = cEquipmentNo;
      data[3]  = 0x00;

      _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, 4,true);
      _MsgOK.InnerOrOutWard = cOutWard;
//      writeJob.WriteWorkByMESSAGEOUT(_MsgOK);
      writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICECENTER92);
      return true;
      }
  } catch(...) {}
}
//-------------------------------------------------------------------
bool PTRAFFIC92COMM::vReportPowerOff_0F00()                                     //special case
{
try{

    int iReturnCommandSet = smem.vGet0FCommandSet();
    if(iReturnCommandSet <= 1) { vReturnToCenterNACK(0x0F, 0x00, 0x80, 0x00); return false; } // not include level "A"

    unsigned char data[6];
    YMDHMS ResetTime;
    ResetTime = smem.vGetLastResetTime();
    data[0] = 0x0F;
    data[1] = 0x00;
    data[2] = ResetTime.Month;
    data[3] = ResetTime.Day;
    data[4] = ResetTime.Hour;
    data[5] = ResetTime.Min;

    MESSAGEOK _MsgOK;
    _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, 6,true);
    _MsgOK.InnerOrOutWard = cOutWard;
//    writeJob.WriteWorkByMESSAGEOUT(_MsgOK);
    writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICECENTER92);
  } catch(...) {}
}
//---------------------------------------------------------------------------
bool PTRAFFIC92COMM::vSetIPCTime_0F12(MESSAGEOK DataMessageIn)
{
try{

    bool bEnableUpdate;
    char cWriteMsgTmp[128]={0};

//    fprintf(stderr, "%s[MeSsAgE] Receiver 0F12, Lengths is %d.%s\n", ColorRed, DataMessageIn.packetLength, ColorNormal);

    if( DataMessageIn.packetLength < 19 ) { vReturnToCenterNACK(0x0F, 0x12, 0x08, 0x00); return false; }
    if( DataMessageIn.packetLength > 19 ) { vReturnToCenterNACK(0x0F, 0x12, 0x08, DataMessageIn.packetLength - 12); return false; }
    if( DataMessageIn.packet[9] < 59 ) { vReturnToCenterNACK(0x0F, 0x12, 0x04, 0x01); return false; }
    if( DataMessageIn.packet[10] > 12 || DataMessageIn.packet[10] == 0 ) { vReturnToCenterNACK(0x0F, 0x12, 0x04, 0x02); return false; }
    if( DataMessageIn.packet[11] > 31 || DataMessageIn.packet[11] == 0 ) { vReturnToCenterNACK(0x0F, 0x12, 0x04, 0x03); return false; }
    if( DataMessageIn.packet[13] > 23 ) { vReturnToCenterNACK(0x0F, 0x12, 0x04, 0x04); return false; }
    if( DataMessageIn.packet[14] > 60 ) { vReturnToCenterNACK(0x0F, 0x12, 0x04, 0x05); return false; }
    if( DataMessageIn.packet[15] > 60 ) { vReturnToCenterNACK(0x0F, 0x12, 0x04, 0x06); return false; }

//  else
//  {
    unsigned int YY = DataMessageIn.packet[9],MM=DataMessageIn.packet[10],DD=DataMessageIn.packet[11],
                 hh = DataMessageIn.packet[13],mm=DataMessageIn.packet[14],ss=DataMessageIn.packet[15];
    unsigned int YYYY = YY + 1911;

    struct tm _CenterTime;                                                        //����SecDif�Ϊ�
    time_t _CenterTimeSec;
    unsigned int tmpYY = YY+11;
    unsigned int tmpMM = MM - 1;

    _CenterTime.tm_year = tmpYY;  // to AD
    _CenterTime.tm_mon = tmpMM;
    _CenterTime.tm_mday = DD;
    _CenterTime.tm_hour = hh;
    _CenterTime.tm_min = mm;
    _CenterTime.tm_sec = ss;
    _CenterTime.tm_isdst = -1;
    _CenterTimeSec = mktime(&_CenterTime);

    //�o��{�b�ɶ�,���F�]date -s MMDDhhmmYYYY
    time_t currenttime=time(NULL);

    if(_CenterTimeSec >= currenttime)                                             //�DSecDif
       SecDif = _CenterTimeSec - currenttime;
    else
       SecDif = currenttime - _CenterTimeSec;

//    printf("[Message] SecDif is %d\n", SecDif);


    if(SecDif > 127) SecDif = 127;

    if(SecDif >3 ) {
      vReportSecDif_0F92();
    } else { vReturnToCenterACK(0x0F, 0x12); }

    bEnableUpdate = smem.vGetBOOLData(EnableUpdateRTC);
    if(bEnableUpdate == true) {

      smem.vSetBOOLData(EnableUpdateRTC, false);

      struct tm *now=localtime(&currenttime);
      unsigned int temphh=(now->tm_hour),tempmm=(now->tm_min);

      //CCJ20150901 start move from down side...
      //lock first then change timer...
      smem.vSetTimerMutexRESET(1);
      while(smem.vGetTimerMutexCTIMER() == 0 || smem.vGetTimerMutexCSTC() == 0) {
        usleep(100);
//              printf("screen data while\n");
      }
      //end

      stc.TimersRead_BeforeResetCMOSTime();  //OTBUG =1 //(v)
      _intervalTimer.TimersRead_BeforeResetCMOSTime();

      SendTimeisChangetoArwenActuate();//arwen++

      //CCJ20150901 Start move to up...
      //lock first then change timer...
      //smem.vSetTimerMutexRESET(1);
      //while(smem.vGetTimerMutexCTIMER() == 0 || smem.vGetTimerMutexCSTC() == 0) {
      //  usleep(100);
//              printf("screen data while\n");
      //}
      //CCJ20150901 End

//      stc.TimersRead_BeforeResetCMOSTime();  //OTBUG =1
//      _intervalTimer.TimersRead_BeforeResetCMOSTime();


      smem.vSetSystemClockTime(YYYY, MM, DD, hh, mm, ss);
      smem.vSetSystemClockTimeSelect(1);
      CheckOnTimeCount = 0;
      //smem.SetSegmentChange(true);

/*move to smem
      sprintf(date,"date -s %#02d%#02d%#02d%#02d%#04d",MM,DD,temphh,tempmm,YYYY);

  //OTMARKPRINTF  printf("\n%s\n",date);

      system(date);

      sprintf(ctime,"date -s %#02d:%#02d:%#02d",hh,mm,ss);

  //OTMARKPRINTF  printf("\n%s\n",time);

      system(ctime);
*/


      stc.TimersReset_AfterResetCMOSTime();  //OTBUG =1
      _intervalTimer.TimersReset_AfterResetCMOSTime();

//move to smem      system("hwclock -w");

//      smem.vSetTimerMutexCSTC(false);
//      smem.vSetTimerMutexCTIMER(false);
      smem.vSystemClockTimetoAct();//arwen++

      smem.vSetTimerMutexRESET(0);

      smem.vSendTimerUpdateToCCJ_5F9E();

      system("hwclock -w");

    } else {
      //Mask
    }

//OTMark    strcat(cWriteMsgTmp, "Get time adjust protocol\n");
//OTMark    strcat(cWriteMsgTmp, date);
//OTMark    strcat(cWriteMsgTmp, ctime);
//OTMark    smem.vWriteMsgToDOM(cWriteMsgTmp);

//    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
bool PTRAFFIC92COMM::vReportSecDif_0F92()
{
try{
    unsigned char data[3];
    data[0] = 0x0F;
    data[1] = 0x92;
    data[2] = SecDif;

    MESSAGEOK _MsgOK;
    _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, 3,true);
    _MsgOK.InnerOrOutWard = cOutWard;
//    writeJob.WriteWorkByMESSAGEOUT(_MsgOK);
    writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICECENTER92);
  } catch(...) {}
}
//---------------------------------------------------------------------------
bool PTRAFFIC92COMM::vQueryIPCTime_0F42(MESSAGEOK DataMessageIn)
{
try{

    int iReturnCommandSet = smem.vGet0FCommandSet();
    if(iReturnCommandSet <= 2) { vReturnToCenterNACK(0x0F, 0x42, 0x80, 0x00); return false; } // not include level "O"

    vReportIPCTime_0FC2();
    return true;
  } catch(...) {}
}
//---------------------------------------------------------------------------
bool PTRAFFIC92COMM::vReportIPCTime_0FC2()
{
try{

   int iReturnCommandSet = smem.vGet0FCommandSet();
   if(iReturnCommandSet <= 2) { vReturnToCenterNACK(0x0F, 0xC2, 0x80, 0x00); return false; } // not include level "O"

    time_t currenttime=time(NULL);
    struct tm *now=localtime(&currenttime);
    unsigned int temphh=(now->tm_hour),tempmm=(now->tm_min);
    unsigned char data[9];

    data[0] = 0x0F;
    data[1] = 0xC2;
    data[2] = (now->tm_year) - 11;
    data[3] = (now->tm_mon) + 1;
    data[4] = (now->tm_mday);
    data[5] = (now->tm_wday);
    data[6] = (now->tm_hour);
    data[7] = (now->tm_min);
    data[8] = (now->tm_sec);

    MESSAGEOK _MsgOK;
    _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, 9,true);
    _MsgOK.InnerOrOutWard = cOutWard;
//    writeJob.WriteWorkByMESSAGEOUT(_MsgOK);
    writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICECENTER92);
  } catch(...) {}
}
//---------------------------------------------------------------------------
bool PTRAFFIC92COMM::vReportIPCTime_0F02()
{
try{
    unsigned char data[2];

    data[0] = 0x0F;
    data[1] = 0x02;

    MESSAGEOK _MsgOK;
    _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, 2,true);

    _MsgOK.InnerOrOutWard = cOutWard;
//    writeJob.WriteWorkByMESSAGEOUT(_MsgOK);
    writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICECENTER92);

    return true;
  } catch(...) {}
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
bool PTRAFFIC92COMM::vQueryVersion_0F43()
{
try{

    vReportVersion_0FC3();
    return true;

  } catch(...) {}
}
//---------------------------------------------------------------------------
bool PTRAFFIC92COMM::vReportVersion_0FC3()
{
try{
    unsigned char data[10];

//    printf("\n+++++++++++++++++++++++++++++++++++++++++");
//    printf("\n+++PTRAFFIC92COMM::vReportVersion_0FC3+++");
//    printf("\n+++++++++++++++++++++++++++++++++++++++++\n\n");

    int iFV = smem.vGetFirmwareInfo_0FC3(4);
    int iSV = smem.vGetFirmwareInfo_0FC3(5);
    DATA_Bit _Version;
    _Version.DBit = 0x00;

    switch(iFV) {
      case(0):
        _Version.switchBit.b5 = false; _Version.switchBit.b6 = false;
        _Version.switchBit.b7 = false; _Version.switchBit.b8 = false;
        break;
      case(1):
        _Version.switchBit.b5 = true;  _Version.switchBit.b6 = false;
        _Version.switchBit.b7 = false; _Version.switchBit.b8 = false;
        break;
      case(2):
        _Version.switchBit.b5 = false; _Version.switchBit.b6 = true;
        _Version.switchBit.b7 = false; _Version.switchBit.b8 = false;
        break;
      case(3):
        _Version.switchBit.b5 = true;  _Version.switchBit.b6 = true;
        _Version.switchBit.b7 = false; _Version.switchBit.b8 = false;
        break;
      case(4):
        _Version.switchBit.b5 = false; _Version.switchBit.b6 = false;
        _Version.switchBit.b7 = true;  _Version.switchBit.b8 = false;
        break;
      case(5):
        _Version.switchBit.b5 = true; _Version.switchBit.b6 = false;
        _Version.switchBit.b7 = true; _Version.switchBit.b8 = false;
        break;
      case(6):
        _Version.switchBit.b5 = false; _Version.switchBit.b6 = true;
        _Version.switchBit.b7 = true;  _Version.switchBit.b8 = false;
        break;
      case(7):
        _Version.switchBit.b5 = true; _Version.switchBit.b6 = true;
        _Version.switchBit.b7 = true; _Version.switchBit.b8 = false;
        break;
      case(8):
        _Version.switchBit.b5 = false; _Version.switchBit.b6 = false;
        _Version.switchBit.b7 = false; _Version.switchBit.b8 = true;
        break;
      case(9):
        _Version.switchBit.b5 = true;  _Version.switchBit.b6 = false;
        _Version.switchBit.b7 = false; _Version.switchBit.b8 = true;
        break;
      default:
        break;
    }

    switch(iSV) {
      case(0):
        _Version.switchBit.b1 = false; _Version.switchBit.b2 = false;
        _Version.switchBit.b3 = false; _Version.switchBit.b4 = false;
        break;
      case(1):
        _Version.switchBit.b1 = true;  _Version.switchBit.b2 = false;
        _Version.switchBit.b3 = false; _Version.switchBit.b4 = false;
        break;
      case(2):
        _Version.switchBit.b1 = false; _Version.switchBit.b2 = true;
        _Version.switchBit.b3 = false; _Version.switchBit.b4 = false;
        break;
      case(3):
        _Version.switchBit.b1 = true;  _Version.switchBit.b2 = true;
        _Version.switchBit.b3 = false; _Version.switchBit.b4 = false;
        break;
      case(4):
        _Version.switchBit.b1 = false; _Version.switchBit.b2 = false;
        _Version.switchBit.b3 = true;  _Version.switchBit.b4 = false;
        break;
      case(5):
        _Version.switchBit.b1 = true; _Version.switchBit.b2 = false;
        _Version.switchBit.b3 = true; _Version.switchBit.b4 = false;
        break;
      case(6):
        _Version.switchBit.b1 = false; _Version.switchBit.b2 = true;
        _Version.switchBit.b3 = true;  _Version.switchBit.b4 = false;
        break;
      case(7):
        _Version.switchBit.b1 = true; _Version.switchBit.b2 = true;
        _Version.switchBit.b3 = true; _Version.switchBit.b4 = false;
        break;
      case(8):
        _Version.switchBit.b1 = false; _Version.switchBit.b2 = false;
        _Version.switchBit.b3 = false; _Version.switchBit.b4 = true;
        break;
      case(9):
        _Version.switchBit.b1 = true;  _Version.switchBit.b2 = false;
        _Version.switchBit.b3 = false; _Version.switchBit.b4 = true;
        break;
      default:
        break;
    }

    data[0] = 0x0F;
    data[1] = 0xC3;
    data[2] = smem.vGetFirmwareInfo_0FC3(0) - 1911;                             //get year
    data[3] = smem.vGetFirmwareInfo_0FC3(1);
    data[4] = smem.vGetFirmwareInfo_0FC3(2);
    data[5] = smem.vGetFirmwareInfo_0FC3(3);
    data[6] = _Version.DBit;
    data[7] = smem.vGet0FCommandSet();

    MESSAGEOK _MsgOK;
    _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, 8,true);
    _MsgOK.InnerOrOutWard = cOutWard;
//    writeJob.WriteWorkByMESSAGEOUT(_MsgOK);
    writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICECENTER92);
  //call storage
  }catch(...){}
}
//---------------------------------------------------------------------------
 //��Ӵ���
//---------------------------------------------------------------------------
bool PTRAFFIC92COMM::vSetDBPassword_0F15(MESSAGEOK DataMessageIn)
{
try{
    if( DataMessageIn.packetLength < 18 ) { vReturnToCenterNACK(0x0F, 0x15, 0x08, 0x00); return false; }
    if( DataMessageIn.packetLength > 18 ) { vReturnToCenterNACK(0x0F, 0x15, 0x08, DataMessageIn.packetLength - 12); return false; }
    int i;
    //check first
    for(i = 0; i < 6; i++)
    {
        if(

           (( DataMessageIn.packet[9+i] >= 48 && DataMessageIn.packet[9+i] <= 57) ||
             (DataMessageIn.packet[9+i] >= 65 && DataMessageIn.packet[9+i] <= 70) ))
        {  }  //do nothing
        else {
             vReturnToCenterNACK( 0x0F, 0x15, 0x04, i+1);
             return false;
        }
    }

    vReturnToCenterACK(0x0F, 0x15);

    char ucPassword[6];
    ucPassword[0] = DataMessageIn.packet[9];
    ucPassword[1] = DataMessageIn.packet[10];
    ucPassword[2] = DataMessageIn.packet[11];
    ucPassword[3] = DataMessageIn.packet[12];
    ucPassword[4] = DataMessageIn.packet[13];
    ucPassword[5] = DataMessageIn.packet[14];

    smem.SetPassword(ucPassword[0], ucPassword[1], ucPassword[2],
                     ucPassword[3], ucPassword[4], ucPassword[5] );

  } catch (...) {}
}
//---------------------------------------------------------------------------
bool PTRAFFIC92COMM::vQueryPassword_0F45()
{
try{

    vReportPassword_0FC5();
    return true;

  }catch(...){}
}
//---------------------------------------------------------------------------
bool PTRAFFIC92COMM::vReportPassword_0FC5()
{
try{
    //call read password from smem
    char cTmpPassword[6];
    unsigned char data[8];
    int i;

    char *_TmpPasswdPtr = smem.GetPassword();
    for(i =0; i < 6; i++)
      cTmpPassword[i] = _TmpPasswdPtr[i];

    data[0] = 0x0F;
    data[1] = 0xC5;
    data[2] = cTmpPassword[0];
    data[3] = cTmpPassword[1];
    data[4] = cTmpPassword[2];
    data[5] = cTmpPassword[3];
    data[6] = cTmpPassword[4];
    data[7] = cTmpPassword[5];

    MESSAGEOK _MsgOK;
    _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, 8,true);
    _MsgOK.InnerOrOutWard = cOutWard;
//    writeJob.WriteWorkByMESSAGEOUT(_MsgOK);
    writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICECENTER92);

  } catch(...){}
}
//---------------------------------------------------------------------------
bool PTRAFFIC92COMM::vSetDBOperatingMode_0F16(MESSAGEOK DataMessageIn)
{
try{

    int iReturnCommandSet = smem.vGet0FCommandSet();
    if(iReturnCommandSet <= 2) { vReturnToCenterNACK(0x0F, 0x16, 0x80, 0x00); return false; } // not include level "O"

    int i;
    int iLockDB;
    if( DataMessageIn.packetLength < 13 ) { vReturnToCenterNACK(0x0F, 0x16, 0x08, 0x00); return false; }
    else if( DataMessageIn.packetLength > 13 ) { vReturnToCenterNACK(0x0F, 0x16, 0x08, DataMessageIn.packetLength - 12); return false; }
    if ( DataMessageIn.packet[9] > 2 )  { vReturnToCenterNACK( 0x0F, 0x16, 0x04, 0x01); return false; }

    vReturnToCenterACK(0x0F, 0x16);

    iLockDB = DataMessageIn.packet[9];
    smem.SetDbOperStat(iLockDB);

    if (smem.GetcFace()==cONLINE) {
        if (iLockDB==0) screenOnline.RequestAccept();
        else if (iLockDB==1) screenOnline.RequestNoAccept();
        else if (iLockDB==2) screenOnline.RequestReadOnly();
        else screenOnline.RequestNoAccept();
    }

  } catch (...) {}
}
//---------------------------------------------------------------------------
bool PTRAFFIC92COMM::vQueryDBOperatingMode_0F46()
{
try{

    int iReturnCommandSet = smem.vGet0FCommandSet();
    if(iReturnCommandSet <= 2) { vReturnToCenterNACK(0x0F, 0x46, 0x80, 0x00); return false; } // not include level "O"

    vReportDBOperatingMode_0FC6();
    return true;

  } catch(...) {}
}
//---------------------------------------------------------------------------
bool PTRAFFIC92COMM::vReportDBOperatingMode_0FC6()
{
try{

    int iReturnCommandSet = smem.vGet0FCommandSet();
    if(iReturnCommandSet <= 2) { vReturnToCenterNACK(0x0F, 0xC6, 0x80, 0x00); return false; } // not include level "O"

    int iLockDB;

    iLockDB = smem.GetDbOperStat();
    unsigned char data[4];
    data[0] = 0x0F;
    data[1] = 0xC6;
    data[2] = iLockDB;

    MESSAGEOK _MsgOK;
    _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, 3,true);
    _MsgOK.InnerOrOutWard = cOutWard;
//    writeJob.WriteWorkByMESSAGEOUT(_MsgOK);
    writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICECENTER92);

  } catch(...) {}
}
//---------------------------------------------------------------------------
int PTRAFFIC92COMM::vQueryProtocolStatus_0F47(MESSAGEOK DataMessageIn)
{
try{

    int iReturnCommandSet = smem.vGet0FCommandSet();
    if(iReturnCommandSet <= 2) { vReturnToCenterNACK(0x0F, 0x47, 0x80, 0x00); return false; } // not include level "O"

    int iP;
    unsigned char iC;

    iC = DataMessageIn.packet[9];
    iP = DataMessageIn.packet[10];

    if( DataMessageIn.packetLength < 14 ) { vReturnToCenterNACK(0x0F, 0x47, 0x08, 0x00); return false; }
    else if( DataMessageIn.packetLength > 14 ) { vReturnToCenterNACK(0x0F, 0x47, 0x08, DataMessageIn.packetLength - 12); return false; }

    vReportProtocolStatus_0FC7(iC, iP);
    return true;
  } catch(...) {}
}
//---------------------------------------------------------------------------
bool PTRAFFIC92COMM::vReportProtocolStatus_0FC7(unsigned char iC, int iP)
{
try{

    int iReturnCommandSet = smem.vGet0FCommandSet();
    if(iReturnCommandSet <= 2) { vReturnToCenterNACK(0x0F, 0xC7, 0x80, 0x00); return false; } // not include level "O"

    bool bIPStatus;
    //OT Debug 0705 default-> true
    bIPStatus = true;

    if(iC == 0x0F) {
       bIPStatus = bProtocol0FStatus[iP];
    } else if(iC == 0x5F) {
       bIPStatus = bProtocol5FStatus[iP];
    }

    unsigned char data[4];
    data[0] = 0x0F;
    data[1] = 0xC7;
    data[2] = iC;
    data[3] = iP;
    data[4] = bIPStatus;

    MESSAGEOK _MsgOK;
    _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, 5,true);
    _MsgOK.InnerOrOutWard = cOutWard;
//    writeJob.WriteWorkByMESSAGEOUT(_MsgOK);
    writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICECENTER92);
  } catch(...) {}
}
//ACK------------------------------------------------------------------------
int PTRAFFIC92COMM::vReturnToCenterACK(unsigned char ucDevCode,
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
//    writeJob.WriteWorkByMESSAGEOUT(_MsgOK);
    writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICECENTER92);
    //jacky20140415 to jsonv3
    //writeJob.WritePhysicalOutNoSetSeqNoResend(_MsgOK.packet, _MsgOK.packetLength, DEVICEJSON);

    return 0;
  } catch (...) {}
}
//---------------------------------------------------------------------------
int PTRAFFIC92COMM::vReturnToCenterNACK(unsigned char ucDevCode,
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
//    writeJob.WriteWorkByMESSAGEOUT(_MsgOK);
    writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICECENTER92);
    //jacky20140415 to jsonv3
    //writeJob.WritePhysicalOutNoSetSeqNoResend(_MsgOK.packet, _MsgOK.packetLength, DEVICEJSON);

    return 0;
  } catch (...) {}
}
//---------------------------------------------------------------------------
bool PTRAFFIC92COMM::vSetCommunicationReset_0F11(MESSAGEOK DataMessageIn)                                     //special case
{
try{
  smem.vSet0F11CommuncationResetChangeStatus(true);
  return true;
}catch(...){}
}

//-------------------------------------------------------------------
bool PTRAFFIC92COMM::vReportCommunicationReset_0F91()
{
try{
  unsigned char data[2];
  data[0] = 0x0F;
  data[1] = 0x91;

  MESSAGEOK _MsgOK;
  _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, 2,true);
  _MsgOK.InnerOrOutWard = cOutWard;
//  writeJob.WriteWorkByMESSAGEOUT(_MsgOK);
  writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICECENTER92);
} catch(...) {}
}
//-------------------------------------------------------------------
bool PTRAFFIC92COMM::vQueryCommunicationReset_0F41(MESSAGEOK DataMessageIn)                                     //special case
{
try{

  vReportCommunicationStatus_0FC1();

  return true;
}catch(...){}
}
//-------------------------------------------------------------------
bool PTRAFFIC92COMM::vReportCommunicationStatus_0FC1()
{
  try{

    unsigned char data[4];
    data[0] = 0x0F;
    data[1] = 0xC1;
    data[2] = smem.vGetHardwareStatus(3);
    data[3] = smem.vGetHardwareStatus(4);

    MESSAGEOK _MsgOK;
    _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, 4,true);
    _MsgOK.InnerOrOutWard = cOutWard;
//    writeJob.WriteWorkByMESSAGEOUT(_MsgOK);
    writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICECENTER92);

    return true;
  }catch(...){}
}

//-------------------------------------------------------------------
bool PTRAFFIC92COMM::vSetHardwareCycle_0F14(MESSAGEOK DataMessageIn)                                     //special case
{
try{
    unsigned short int usiHWCycle = DataMessageIn.packet[9];

    int iReturnCommandSet = smem.vGet0FCommandSet();
    if(iReturnCommandSet == 0 && (usiHWCycle>=1 && usiHWCycle<=4)) { vReturnToCenterNACK(0x0F, 0x14, 0x80, 0x00); return false; } // in wireless

    if( DataMessageIn.packetLength < 13 ) { vReturnToCenterNACK(0x0F, 0x14, 0x08, 0x00); return false; }
    if( DataMessageIn.packetLength > 13 ) { vReturnToCenterNACK(0x0F, 0x14, 0x08, DataMessageIn.packetLength - 12); return false; }
    if( usiHWCycle >= 6 ) { vReturnToCenterNACK(0x0F, 0x14, 0x04, 0x01); return false; }


    if(DataMessageIn.InnerOrOutWard != cComingFromScreen)
    vReturnToCenterACK(0x0F, 0x14);
    smem.vSetHWCycleCodeFor_0F14_0FC4(DataMessageIn.packet[9]);
    vSetHardWareReportCycleTransTo_itimer(usiHWCycle);                                        //¸ÑÄ¶®É¶¡
    smem.vSet0FHardwareCycle(_itimer_HardwareCycle);                                 //³]©w®É¶¡
    return true;

}catch(...){}
}
//------------------------------------------------------------------------
bool PTRAFFIC92COMM::vSetHardWareReportCycleTransTo_itimer(unsigned short int usiHWCycle)
{
try {
  if(usiHWCycle == 0)
  {
    _itimer_HardwareCycle.it_value.tv_sec = 0;
    _itimer_HardwareCycle.it_value.tv_nsec = 0;
    _itimer_HardwareCycle.it_interval.tv_sec = 0;
    _itimer_HardwareCycle.it_interval.tv_nsec = 0;
  }
  if(usiHWCycle == 1)
  {
    _itimer_HardwareCycle.it_value.tv_sec = 1;
    _itimer_HardwareCycle.it_value.tv_nsec = 0;
    _itimer_HardwareCycle.it_interval.tv_sec = 1;
    _itimer_HardwareCycle.it_interval.tv_nsec = 0;
  }
  if(usiHWCycle == 2)
  {
    _itimer_HardwareCycle.it_value.tv_sec = 2;
    _itimer_HardwareCycle.it_value.tv_nsec = 0;
    _itimer_HardwareCycle.it_interval.tv_sec = 2;
    _itimer_HardwareCycle.it_interval.tv_nsec = 0;
  }
  if(usiHWCycle == 3)
  {
    _itimer_HardwareCycle.it_value.tv_sec = 5;
    _itimer_HardwareCycle.it_value.tv_nsec = 0;
    _itimer_HardwareCycle.it_interval.tv_sec = 5;
    _itimer_HardwareCycle.it_interval.tv_nsec = 0;
  }
  if(usiHWCycle == 4)
  {
    _itimer_HardwareCycle.it_value.tv_sec = 60;
    _itimer_HardwareCycle.it_value.tv_nsec = 0;
    _itimer_HardwareCycle.it_interval.tv_sec = 60;
    _itimer_HardwareCycle.it_interval.tv_nsec = 0;
  }
  if(usiHWCycle == 5)
  {
    _itimer_HardwareCycle.it_value.tv_sec = 300;
    _itimer_HardwareCycle.it_value.tv_nsec = 0;
    _itimer_HardwareCycle.it_interval.tv_sec = 300;
    _itimer_HardwareCycle.it_interval.tv_nsec = 0;
  }
  return true;
} catch (...) {}
}
//-------------------------------------------------------------------
bool PTRAFFIC92COMM::vQueryHardwareCycle_0F44(MESSAGEOK DataMessageIn)
{
try{

  vReportHardwareCycle_0FC4();

  return true;

}catch(...){}
}
//-------------------------------------------------------------------
bool PTRAFFIC92COMM::vReportHardwareCycle_0FC4()
{
try{
  unsigned char data[3];
  data[0] = 0x0F;
  data[1] = 0xC4;
  data[2] = smem.vGetHWCycleCodeFor_0F14_0FC4();

  MESSAGEOK _MsgOK;
  _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, 3,true);
  _MsgOK.InnerOrOutWard = cOutWard;
//  writeJob.WriteWorkByMESSAGEOUT(_MsgOK);
  writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICECENTER92);

}catch(...){}
}
//-------------------------------------------------------------------
bool PTRAFFIC92COMM::vSetCommandSet_0F13(MESSAGEOK DataMessageIn)
{
try{

  // 0: basic with wireless
  // 1: basic
  // 2: baseic, advance
  // 3: baseic, advance, option

  if(DataMessageIn.packet[9] < 4)
  {
    ucCommandSet = DataMessageIn.packet[9];
    smem.vSet0FCommandSet(ucCommandSet);
    vReturnToCenterACK(0x0F, 0x13);
    return true;
  }
  else
  {
     vReturnToCenterNACK(0x0F, 0x13, 0x04,0x01);
  }
  } catch(...) {}
}

//-------------------------------------------------------------------
bool PTRAFFIC92COMM::vReportHardwareCycle_0F04(MESSAGEOK DataMessageIn)         //���ӬO�Τ���
{
try{
// OT Debug
/*
  unsigned char data[3];
  data[0] = 0x0F;
  data[1] = 0x04;
  data[2] = smem.vGetHWCycleCodeFor_0F14_0FC4();

  MESSAGEOK _MsgOK;
  _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, 3,true);
  _MsgOK.InnerOrOutWard = cOutWard;
  writeJob.WriteWorkByMESSAGEOUT(_MsgOK);
*/
}catch(...){}
}

//OT PASS
//---------------------------------------------------------------------------
bool PTRAFFIC92COMM::vPass_0F8E(MESSAGEOK DataMessageIn)
{
try{

  printf("printfMsg Enter 0F8E. length=%d\n", DataMessageIn.packetLength);
  for(int i = 0; i < DataMessageIn.packetLength; i++) {
    printf("%3x", DataMessageIn.packet[i]);
  }
  printf("printfMsg\n");

  // write 2 mode, select first
  unsigned char ucPM;                                                           //PassMode
  int iLEN;
  int iCksLocate;
  BCDSW bcdSwitchHi;
  BCDSW bcdSwitchLo;

  ucPM = smem.vGetPassMode();
  printf("printfMsg ucPM:%d\n", ucPM);

  if(ucPM == cTermPassMode)
  {
    printf("printfMsg Goto cTermPassMode\n");
    //cal iLEN
    iLEN = DataMessageIn.packet[5] * 256 + DataMessageIn.packet[6];
    iLEN = iLEN - 4;
    DataMessageIn.packet[5] = HI(iLEN);
    DataMessageIn.packet[6] = LO(iLEN);

//    vConvert0F8E2Normal(DataMessageIn);
    for(int i = 11; i < DataMessageIn.packetLength; i++) {                      //move
      DataMessageIn.packet[i-4] = DataMessageIn.packet[i];
    }
    DataMessageIn.packetLength = DataMessageIn.packetLength - 4;

    //re-do cks
    iCksLocate = DataMessageIn.packetLength - 1;
    DataMessageIn.packet[iCksLocate] = 0;                                       // init checksum
    for (int j=0; j<iCksLocate; j++) DataMessageIn.packet[iCksLocate] ^= DataMessageIn.packet[j];

    readJob.ReadWorkByMESSAGEIN(DataMessageIn);

  }
  else if(ucPM == cPassServerMode)
  {
  printf("printfMsg Goto cPassServerMode\n");

  tsUDPMappingLCN _sUDPMappingLCNinREADJOB[MaxOpenUDPPort];
  int iTermLcn;
  for(int i = 0; i < MaxOpenUDPPort; i++)
    _sUDPMappingLCNinREADJOB[i] = smem.vGetUDPMappingLCNData(i);

  if (smem.vGetAdjudicateReturnAddressBCDorHEX()==cBCD) {  //BCD code�s�X
      bcdSwitchHi.bcdCode=DataMessageIn.packet[9];
      bcdSwitchLo.bcdCode=DataMessageIn.packet[10];
      iTermLcn=bcdSwitchHi.bcdHiLo.b2*1000+bcdSwitchHi.bcdHiLo.b1*100+bcdSwitchLo.bcdHiLo.b2*10+bcdSwitchLo.bcdHiLo.b1;
  } else {                                                 //16�i��s�X
      iTermLcn=(int)DataMessageIn.packet[9]*256+(int)DataMessageIn.packet[10];
  }

  printf("Send to iTermLcn: %d\n", iTermLcn);

  iCksLocate = DataMessageIn.packetLength - 1;                              // last byte

  // change address
  DataMessageIn.packet[3] = DataMessageIn.packet[9];
  DataMessageIn.packet[4] = DataMessageIn.packet[10];

  // change cks
  DataMessageIn.packet[iCksLocate] = 0;                                       // init checksum
  for (int j=0; j<iCksLocate; j++) DataMessageIn.packet[iCksLocate] ^= DataMessageIn.packet[j];

  if( (iTermLcn == _sUDPMappingLCNinREADJOB[2].iMachineLCN  || iTermLcn == 0xFFFF)
       && _sUDPMappingLCNinREADJOB[2].iListenPort != 0 )
  {
    smem.SSVD01Socket.UdpSend(DataMessageIn.packet, DataMessageIn.packetLength,"ToVD01");
  }

  if( (iTermLcn == _sUDPMappingLCNinREADJOB[3].iMachineLCN  || iTermLcn == 0xFFFF)
       && _sUDPMappingLCNinREADJOB[3].iListenPort != 0 )
  {
    smem.SSVD02Socket.UdpSend(DataMessageIn.packet, DataMessageIn.packetLength,"ToVD02");
  }
  if( (iTermLcn == _sUDPMappingLCNinREADJOB[4].iMachineLCN  || iTermLcn == 0xFFFF)
       && _sUDPMappingLCNinREADJOB[4].iListenPort != 0 )
  {
    smem.SSVD03Socket.UdpSend(DataMessageIn.packet, DataMessageIn.packetLength,"ToVD03");
  }
  if( (iTermLcn == _sUDPMappingLCNinREADJOB[5].iMachineLCN  || iTermLcn == 0xFFFF)
       && _sUDPMappingLCNinREADJOB[5].iListenPort != 0 )
  {
    smem.SSVD04Socket.UdpSend(DataMessageIn.packet, DataMessageIn.packetLength,"ToVD04");
  }
  if( (iTermLcn == _sUDPMappingLCNinREADJOB[6].iMachineLCN  || iTermLcn == 0xFFFF)
       && _sUDPMappingLCNinREADJOB[6].iListenPort != 0 )
  {
    smem.SSVD05Socket.UdpSend(DataMessageIn.packet, DataMessageIn.packetLength,"ToVD05");
  }
  if( (iTermLcn == _sUDPMappingLCNinREADJOB[7].iMachineLCN  || iTermLcn == 0xFFFF)
       && _sUDPMappingLCNinREADJOB[7].iListenPort != 0 )
  {
    smem.SSVD06Socket.UdpSend(DataMessageIn.packet, DataMessageIn.packetLength,"ToVD06");
  }
  if( (iTermLcn == _sUDPMappingLCNinREADJOB[8].iMachineLCN  || iTermLcn == 0xFFFF)
       && _sUDPMappingLCNinREADJOB[8].iListenPort != 0 )
  {
    smem.SSVD07Socket.UdpSend(DataMessageIn.packet, DataMessageIn.packetLength,"ToVD07");
  }
  if( (iTermLcn == _sUDPMappingLCNinREADJOB[9].iMachineLCN  || iTermLcn == 0xFFFF)
       && _sUDPMappingLCNinREADJOB[9].iListenPort != 0 )
  {
    smem.SSVD08Socket.UdpSend(DataMessageIn.packet, DataMessageIn.packetLength,"ToVD08");
  }
  if( (iTermLcn == _sUDPMappingLCNinREADJOB[10].iMachineLCN  || iTermLcn == 0xFFFF)
       && _sUDPMappingLCNinREADJOB[10].iListenPort != 0 )
  {
    smem.SSVD09Socket.UdpSend(DataMessageIn.packet, DataMessageIn.packetLength,"ToVD09");
  }
  if( (iTermLcn == _sUDPMappingLCNinREADJOB[11].iMachineLCN  || iTermLcn == 0xFFFF)
       && _sUDPMappingLCNinREADJOB[11].iListenPort != 0 )
  {
    smem.SSVD10Socket.UdpSend(DataMessageIn.packet, DataMessageIn.packetLength,"ToVD10");
  }
  if( (iTermLcn == _sUDPMappingLCNinREADJOB[12].iMachineLCN  || iTermLcn == 0xFFFF)
       && _sUDPMappingLCNinREADJOB[12].iListenPort != 0 )
  {
    smem.SSVD11Socket.UdpSend(DataMessageIn.packet, DataMessageIn.packetLength,"ToVD11");
  }
  if( (iTermLcn == _sUDPMappingLCNinREADJOB[13].iMachineLCN  || iTermLcn == 0xFFFF)
       && _sUDPMappingLCNinREADJOB[13].iListenPort != 0 )
  {
    smem.SSVD12Socket.UdpSend(DataMessageIn.packet, DataMessageIn.packetLength,"ToVD12");
  }
  if( (iTermLcn == _sUDPMappingLCNinREADJOB[14].iMachineLCN  || iTermLcn == 0xFFFF)
       && _sUDPMappingLCNinREADJOB[14].iListenPort != 0 )
  {
    smem.SSVD13Socket.UdpSend(DataMessageIn.packet, DataMessageIn.packetLength,"ToVD13");
  }
  if( (iTermLcn == _sUDPMappingLCNinREADJOB[15].iMachineLCN  || iTermLcn == 0xFFFF)
       && _sUDPMappingLCNinREADJOB[15].iListenPort != 0 )
  {
    smem.SSVD14Socket.UdpSend(DataMessageIn.packet, DataMessageIn.packetLength,"ToVD14");
  }
  if( (iTermLcn == _sUDPMappingLCNinREADJOB[16].iMachineLCN  || iTermLcn == 0xFFFF)
       && _sUDPMappingLCNinREADJOB[16].iListenPort != 0 )
  {
    smem.SSVD15Socket.UdpSend(DataMessageIn.packet, DataMessageIn.packetLength,"ToVD15");
  }
  if( (iTermLcn == _sUDPMappingLCNinREADJOB[17].iMachineLCN  || iTermLcn == 0xFFFF)
       && _sUDPMappingLCNinREADJOB[17].iListenPort != 0 )
  {
    smem.SSVD16Socket.UdpSend(DataMessageIn.packet, DataMessageIn.packetLength,"ToVD16");
  }
  if( (iTermLcn == _sUDPMappingLCNinREADJOB[18].iMachineLCN  || iTermLcn == 0xFFFF)
       && _sUDPMappingLCNinREADJOB[18].iListenPort != 0 )
  {
    smem.SSVD17Socket.UdpSend(DataMessageIn.packet, DataMessageIn.packetLength,"ToVD17");
  }
  if( (iTermLcn == _sUDPMappingLCNinREADJOB[19].iMachineLCN  || iTermLcn == 0xFFFF)
       && _sUDPMappingLCNinREADJOB[19].iListenPort != 0 )
  {
    smem.SSVD18Socket.UdpSend(DataMessageIn.packet, DataMessageIn.packetLength,"ToVD18");
  }
  if( (iTermLcn == _sUDPMappingLCNinREADJOB[20].iMachineLCN  || iTermLcn == 0xFFFF)
       && _sUDPMappingLCNinREADJOB[20].iListenPort != 0 )
  {
    smem.SSVD19Socket.UdpSend(DataMessageIn.packet, DataMessageIn.packetLength,"ToVD19");
  }
  if( (iTermLcn == _sUDPMappingLCNinREADJOB[21].iMachineLCN  || iTermLcn == 0xFFFF)
       && _sUDPMappingLCNinREADJOB[21].iListenPort != 0 )
  {
    smem.SSVD20Socket.UdpSend(DataMessageIn.packet, DataMessageIn.packetLength,"ToVD20");
  }
  if( (iTermLcn == _sUDPMappingLCNinREADJOB[22].iMachineLCN  || iTermLcn == 0xFFFF)
       && _sUDPMappingLCNinREADJOB[22].iListenPort != 0 )
  {
    smem.SSVD21Socket.UdpSend(DataMessageIn.packet, DataMessageIn.packetLength,"ToVD21");
  }
  if( (iTermLcn == _sUDPMappingLCNinREADJOB[23].iMachineLCN  || iTermLcn == 0xFFFF)
       && _sUDPMappingLCNinREADJOB[23].iListenPort != 0 )
  {
    smem.SSVD22Socket.UdpSend(DataMessageIn.packet, DataMessageIn.packetLength,"ToVD22");
  }
  if( (iTermLcn == _sUDPMappingLCNinREADJOB[24].iMachineLCN  || iTermLcn == 0xFFFF)
       && _sUDPMappingLCNinREADJOB[24].iListenPort != 0 )
  {
    smem.SSVD23Socket.UdpSend(DataMessageIn.packet, DataMessageIn.packetLength,"ToVD23");
  }
  if( (iTermLcn == _sUDPMappingLCNinREADJOB[25].iMachineLCN  || iTermLcn == 0xFFFF)
       && _sUDPMappingLCNinREADJOB[25].iListenPort != 0 )
  {
    smem.SSVD24Socket.UdpSend(DataMessageIn.packet, DataMessageIn.packetLength,"ToVD24");
  }
  if( (iTermLcn == _sUDPMappingLCNinREADJOB[26].iMachineLCN  || iTermLcn == 0xFFFF)
       && _sUDPMappingLCNinREADJOB[26].iListenPort != 0 )
  {
    smem.SSVD25Socket.UdpSend(DataMessageIn.packet, DataMessageIn.packetLength,"ToVD25");
  }
  if( (iTermLcn == _sUDPMappingLCNinREADJOB[27].iMachineLCN  || iTermLcn == 0xFFFF)
       && _sUDPMappingLCNinREADJOB[27].iListenPort != 0 )
  {
    smem.SSVD26Socket.UdpSend(DataMessageIn.packet, DataMessageIn.packetLength,"ToVD26");
  }
  if( (iTermLcn == _sUDPMappingLCNinREADJOB[28].iMachineLCN  || iTermLcn == 0xFFFF)
       && _sUDPMappingLCNinREADJOB[28].iListenPort != 0 )
  {
    smem.SSVD27Socket.UdpSend(DataMessageIn.packet, DataMessageIn.packetLength,"ToVD27");
  }
  if( (iTermLcn == _sUDPMappingLCNinREADJOB[29].iMachineLCN  || iTermLcn == 0xFFFF)
       && _sUDPMappingLCNinREADJOB[29].iListenPort != 0 )
  {
    smem.SSVD28Socket.UdpSend(DataMessageIn.packet, DataMessageIn.packetLength,"ToVD28");
  }
  if( (iTermLcn == _sUDPMappingLCNinREADJOB[30].iMachineLCN  || iTermLcn == 0xFFFF)
       && _sUDPMappingLCNinREADJOB[30].iListenPort != 0 )
  {
    smem.SSVD29Socket.UdpSend(DataMessageIn.packet, DataMessageIn.packetLength,"ToVD29");
  }
  if( (iTermLcn == _sUDPMappingLCNinREADJOB[31].iMachineLCN  || iTermLcn == 0xFFFF)
       && _sUDPMappingLCNinREADJOB[31].iListenPort != 0 )
  {
    smem.SSVD30Socket.UdpSend(DataMessageIn.packet, DataMessageIn.packetLength,"ToVD30");
  }
/*
  if( (iTermLcn == _sUDPMappingLCNinREADJOB[32].iMachineLCN  || iTermLcn == 0xFFFF)
       && _sUDPMappingLCNinREADJOB[32].iListenPort != 0 )
  {
    smem.SSVD31Socket.UdpSend(DataMessageIn.packet, DataMessageIn.packetLength,"ToVD31");
  }
  if( (iTermLcn == _sUDPMappingLCNinREADJOB[33].iMachineLCN  || iTermLcn == 0xFFFF)
       && _sUDPMappingLCNinREADJOB[33].iListenPort != 0 )
  {
    smem.SSVD32Socket.UdpSend(DataMessageIn.packet, DataMessageIn.packetLength,"ToVD32");
  }
*/

  }
  //comment when broadcast, should forward to term.

  return true;

} catch(...) {}
}
bool PTRAFFIC92COMM::SendTimeisChangetoArwenActuate()
{
	unsigned char data[20]={0};
	data[0]=0x0F;
	data[1]=0x12;
	MESSAGEOK MsgOK;
	MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, 2, true);
    MsgOK.InnerOrOutWard = cOutWard;
    writeJob.WritePhysicalOut(MsgOK.packet, MsgOK.packetLength, DEVICECCJDYNCTL);

}



bool PTRAFFIC92COMM::vRebootIPC_0F10_revAPP(MESSAGEOK DataMessageIn)
{
    try
    {
        if (DataMessageIn.packet[2] == 0x52 && DataMessageIn.packet[3] == 0x52 && DataMessageIn.packetLength ==4 )
        {


            smem.vSetBOOLData(IFGetResetTCSignal, true);
            smem.vWriteMsgToDOM("Reset TC Signal By revAPP!");

            //OTMARKPRINTF  printf("%s[Message] [Message] [Message] REBOOT SYSTEM!!!%s\n", ColorRed, ColorNormal);


            //OTMARKPRINTF  printf("ABC 9:%x 10:%x \n", DataMessageIn.packet[9], DataMessageIn.packet[10]);


            vReturnToCenterACK(0x0F, 0x10);

//cheat, for fuck chen-min-zui
            MESSAGEOK _MSG;
            unsigned char data3[4];
            data3[0]  = 0x0F;
            data3[1]  = 0x90;
            data3[2]  = 0x52;
            data3[3]  = 0x52;
            _MSG = oDataToMessageOK.vPackageINFOTo92Protocol(data3, 4, true);
            _MSG.InnerOrOutWard = cOutWard;
            writeJob.WritePhysicalOut(_MSG.packet, _MSG.packetLength, DEVICECENTER92);
       //     writeJob.WritePhysicalOut(data3, 4, revAPP);


//Mask       digitalIO.WriteDigitalIO(0xFF);     //³q°T­«¸m
//Mask       usleep(1500000);
//Mask       stc.Lock_to_Set_Control_Strategy(STRATEGY_FLASH);                        //OTFix 940804
//Mask    digitalIO.WriteDigitalIO(0x00);
            //µwÅé­«¸m

// /*Move To CSTC*/
            system("sync");
            system("sync");
            system("reboot");

            screenHWReset.DisplayHWReset();
            /* */

            return true;
        }


    }
    catch(...) {}
}

bool PTRAFFIC92COMM::vReportIPCTime_0FC2_revAPP()
{
    try
    {

//  int iReturnCommandSet = smem.vGet0FCommandSet();
        // if(iReturnCommandSet <= 2) { vReturnToCenterNACK(0x0F, 0xC2, 0x80, 0x00); return false; } // not include level "O"

        time_t currenttime=time(NULL);
        struct tm *now=localtime(&currenttime);
        unsigned int temphh=(now->tm_hour),tempmm=(now->tm_min);
        BYTE data[9];

        data[0] = 0x0F;
        data[1] = 0xC2;


        data[2]=currenttime>>24;
         data[3]=currenttime>>16;
          data[4]=currenttime>>8;
           data[5]=currenttime;

      /*  data[2] = (now->tm_year) - 11;
        data[3] = (now->tm_mon) + 1;
        data[4] = (now->tm_mday);
        data[5] = (now->tm_wday);
        data[6] = (now->tm_hour);
        data[7] = (now->tm_min);
        data[8] = (now->tm_sec);*/


        writeJob.WritePhysicalOut(data,6,revAPP);


         return true;
    }
    catch(...) {}
}

bool PTRAFFIC92COMM::check_link_revAPP()
{
    try
    {

//  int iReturnCommandSet = smem.vGet0FCommandSet();
        // if(iReturnCommandSet <= 2) { vReturnToCenterNACK(0x0F, 0xC2, 0x80, 0x00); return false; } // not include level "O"


        BYTE data[9];

        data[0] = 0x0F;
        data[1] = 0x7b;



        writeJob.WritePhysicalOut(data,2,revAPP);


         return true;
    }
    catch(...) {}
}

MESSAGEOK PTRAFFIC92COMM::revAPP_packet(int readSelectLength,BYTE block[1024])
{
    MESSAGEOK messagein;

    for(int i=0; i<readSelectLength; i++)
    {
        messagein.packet[i]=block[i];
    }
    messagein.packetLength=readSelectLength;

    return messagein;
}
