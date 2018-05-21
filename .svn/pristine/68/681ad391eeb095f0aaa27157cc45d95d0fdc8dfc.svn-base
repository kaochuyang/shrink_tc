//---------------------------------------------------------------------------
#include "READJOB.h"
#include "PROTOCOL.h"
#include "SMEM.h"

#include "WRITEJOB.h"

#include "screenRedCountHWCheck.h"

READJOB readJob;
//---------------------------------------------------------------------------
READJOB::READJOB(void)
{
  for(int i = 0; i < MaxOpenUDPPort; i++)
    _sUDPMappingLCNinREADJOB[i] = smem.vGetUDPMappingLCNData(i);
}
//---------------------------------------------------------------------------
READJOB::~READJOB(void)
{
}
//---------------------------------------------------------------------------
bool READJOB::SetInterfaceAndReadFlag(int *maxMessageIndex,MESSAGEOK *messageIn,int interface)    //設定此封包Read的flag
{
try {
    if (*maxMessageIndex >= 0) {
        for (int i=0;i<=*maxMessageIndex;i++) {                                 //檢查每一個messageIn[i]
             if (messageIn[i].cksStatus==true) {                                //通過CheckSum檢測
                 if (messageIn[i].success==true) {                              //通過檢查封包合理性
                     messageIn[i].UDPorRS232=interface;

                     messageIn[i].ReadorWrite=cREAD;
//OT Pass  should set to cWrite, no return ACK to center, return ACK to VD myself
//                     messageIn[i].ReadorWrite=cWRITE;
                 }
             }
        }
    }

    return true;

  } catch (...) {}
}

//---------------------------------------------------------------------------
bool READJOB::vCheckIfBcastingForwardToUDP(int *maxMessageIndex,MESSAGEOK *messageIn)
{
try {
    if (*maxMessageIndex >= 0) {
        for (int i=0;i<=*maxMessageIndex;i++) {                                 //檢查每一個messageIn[i]
             if (messageIn[i].cksStatus==true) {                                //通過CheckSum檢測
                 if (messageIn[i].success==true) {                              //通過檢查封包合理性

//                     if (messageIn[i].packetLcn !=smem.GetAddress()) {           //只要該封包不是給TC本身
//                       if (smem.VDBcastForListen5003Socket.GetPortAlreadyOpen())             //檢查，然後送出到UDP廣撥
//                         smem.VDBcastForListen5003Socket.UdpSend(messageIn[i].packet, messageIn[i].packetLength,"192.168.1.255:6003");
//                     }

                     if (messageIn[i].packetLcn !=smem.GetAddress()) {           //只要該封包不是給TC本身

                       //OTMARKPRINTF  printf("[For UDP Debug] messageIn[i].packetLcn is %d.\n", messageIn[i].packetLcn);
//OT Pass disable
/*
                         if(  ( messageIn[i].packetLcn == _sUDPMappingLCNinREADJOB[2].iMachineLCN || messageIn[i].packetLcn == (int)0xFFFF)
                           && _sUDPMappingLCNinREADJOB[2].iListenPort != 0 )
                           smem.SSVD01Socket.UdpSend(messageIn[i].packet, messageIn[i].packetLength,"SSVD01");

                         if(  ( messageIn[i].packetLcn == _sUDPMappingLCNinREADJOB[3].iMachineLCN || messageIn[i].packetLcn == (int)0xFFFF)
                           && _sUDPMappingLCNinREADJOB[3].iListenPort != 0 )
                           smem.SSVD02Socket.UdpSend(messageIn[i].packet, messageIn[i].packetLength,"SSVD02");

                         if(  ( messageIn[i].packetLcn == _sUDPMappingLCNinREADJOB[4].iMachineLCN || messageIn[i].packetLcn == (int)0xFFFF)
                           && _sUDPMappingLCNinREADJOB[4].iListenPort != 0 )
                           smem.SSVD03Socket.UdpSend(messageIn[i].packet, messageIn[i].packetLength,"SSVD03");

                         if(  ( messageIn[i].packetLcn == _sUDPMappingLCNinREADJOB[5].iMachineLCN || messageIn[i].packetLcn == (int)0xFFFF)
                           && _sUDPMappingLCNinREADJOB[5].iListenPort != 0 )
                           smem.SSVD04Socket.UdpSend(messageIn[i].packet, messageIn[i].packetLength,"SSVD04");
*/
                         if(smem.vGetINTData(Com2_TYPE) == Com2IsPassingPort) {
                           printf("OT Forward. messageIn[i].packetLcn=%d, smem.GetAddress=%d\n", messageIn[i].packetLcn, smem.GetAddress());
                           smem.passingPort.Rs232Write(messageIn[i].packet, messageIn[i].packetLength,"CMSPassing");
                         }

                     }

                 }
             }
        }
    }

    return true;

  } catch (...) {}
}


//---------------------------------------------------------------------------
bool READJOB::CheckLcn(int *maxMessageIndex,MESSAGEOK *messageIn)
{
try {
    if (*maxMessageIndex >= 0) {
        for (int i=0;i<=*maxMessageIndex;i++) {                                 //檢查每一個messageIn[i]

/*
        if ((messageIn[i].packet[0]==(const BYTE)0xAA) && (messageIn[i].packet[1]==(const BYTE)0xDD)) {
          printf("printfMsgAADD5 cksStatus:%d, success:%d, SEQ:%d\n",
          messageIn[i].cksStatus, messageIn[i].success, messageIn[i].packet[2]);
        }
*/

             if (messageIn[i].cksStatus==true) {                                //通過CheckSum檢測
                 if (messageIn[i].success==true) {                              //通過檢查封包合理性

                     //printf("packetLcn: %d == smem.GetAddress(): %d\n",messageIn[i].packetLcn,smem.GetAddress());

                     if (messageIn[i].packetLcn==smem.GetAddress() || messageIn[i].packetLcn==(int)0xFFFF)
                         messageIn[i].success=true;
                     else {
                       messageIn[i].success=false; printf("go lcn error!?\n");
                       for(int j = 0; j < messageIn[i].packetLength; j++) {
                         printf("%X ", messageIn[i].packet[j]);
                       }
                       printf("packetLcn:%d, ", messageIn[i].packetLcn);printf("TClcn:%d\n", smem.GetAddress());

                     }
                 }
             }
        }
    }

    return true;

  } catch (...) {}
}
//---------------------------------------------------------------------------
bool READJOB::SetCenterComm(int *maxMessageIndex,MESSAGEOK *messageIn)
{
try {

    if (*maxMessageIndex >= 0) {
        for (int i=0;i<=*maxMessageIndex;i++) {                                 //檢查每一個messageIn[i]
             if (messageIn[i].cksStatus==true) {                                //通過CheckSum檢測
                 if (messageIn[i].success==true) {                              //通過檢查封包合理性
                     if (messageIn[i].protocol==PROTOCOL77 || messageIn[i].protocol==PROTOCOL87
                         || messageIn[i].protocol==PROTOCOL92 || messageIn[i].protocol==PROTOCOLNTCIP) {
                         smem.vSaveCenterConnectStatus(true);

                         /*
                         printf("-------------------smem.vSaveCenterConnectStatus(true);-------------------\n");
                         printf("-------------------smem.vSaveCenterConnectStatus(true);-------------------\n");
                         printf("-------------------smem.vSaveCenterConnectStatus(true);-------------------\n");
                         */
                         break;                 //作一次就好
                     }
                 }
             }
        }
    }

    return true;

  } catch (...) {}
}
//---------------------------------------------------------------------------
bool READJOB::SetKeypadComm(int *maxMessageIndex,MESSAGEOK *messageIn)
{
try {

    if (*maxMessageIndex >= 0) {
        for (int i=0;i<=*maxMessageIndex;i++) {                                 //檢查每一個messageIn[i]
             if (messageIn[i].cksStatus==true) {                                //通過CheckSum檢測
                 if (messageIn[i].success==true) {                              //通過檢查封包合理性
                     if (messageIn[i].protocol==PROTOCOLKEYPAD) {
//Move                         smem.SetLastKeypadTime();
                         break;
                     }
                 }
             }
        }
    }

    return true;

  } catch (...) {}
}
//---------------------------------------------------------------------------
bool READJOB::DoWorkByMESSAGEIN(int *maxMessageIndex,MESSAGEOK *messageIn)
{
try {
    if (*maxMessageIndex >= 0) {
        for (int i=0;i<=*maxMessageIndex;i++) {                                 //檢查每一個messageIn[i]
           //printf("[OTMsg] Go readJob. C:%d S:%d, P:%d \n", messageIn[i].cksStatus, messageIn[i].success, messageIn[i].protocol);
             if (messageIn[i].cksStatus==true) {                                //通過CheckSum檢測
                 if (messageIn[i].success==true) {                              //通過檢查封包合理性

/*
                 if ((messageIn[i].packet[0]==(const BYTE)0xAA) && (messageIn[i].packet[1]==(const BYTE)0xDD)) {
                   printf("printfMsgAADD8 cksStatus:%d, success:%d, SEQ:%d\n",
                   messageIn[i].cksStatus, messageIn[i].success, messageIn[i].packet[2]);
                 }
*/

/*
                 if(messageIn[i].packet[7] == (const BYTE)0x6F) printf("printfMsg DoWorkByMESSAGEIN this is 6f!\n");
                 for(int ii = 0; ii < 12; ii++)
                   printf("%x ",messageIn[i].packet[ii]);
                 printf("\n");
*/
                     protocol.DoWork(messageIn[i]);
//                     printf("[OTMsg] Go readJob. S:OK C:OK\n");
                 }
             }
        }
    }

    return true;

  } catch(...) {}
}

//OT Pass 2
//---------------------------------------------------------------------------
bool READJOB::ReadWorkByMESSAGEIN(MESSAGEOK messageIn)
{
try {
    printf("GOTO RE_DDDD\n\n");
    messageIn.ReadorWrite=cREAD;
    protocol.DoWork(messageIn);
    return true;
  } catch (...) {}
}

//---------------------------------------------------------------------------
bool READJOB::DoSendUDP_MESSAGEIN(int *maxMessageIndex,MESSAGEOK *messageIn)
{
try {
    if (*maxMessageIndex >= 0) {
        for (int i=0;i<=*maxMessageIndex;i++) {                                 //檢查每一個messageIn[i]
             if (messageIn[i].cksStatus==true) {                                //通過CheckSum檢測
                 if (messageIn[i].success==true) {                              //通過檢查封包合理性
                     writeJob.WritePhysicalOutNoSetSeqNoResend(messageIn[i].packet, messageIn[i].packetLength, DEVICECENTER92);
                 }
             }
        }
    }

    return true;

  } catch(...) {}
}
//---------------------------------------------------------------------------
bool READJOB::vDoDisplay(int *maxMessageIndex,MESSAGEOK *messageIn)             //show in redcount page.
{
try {
  unsigned char ucID, ucLeft, ucRight;
  if (*maxMessageIndex >= 0) {


      for (int i=0;i<=*maxMessageIndex;i++) {

        for(int iii = 0; iii < messageIn[i].packetLength; iii++)
        //OTMARKPRINTF  printf("%3X", messageIn[i].packet[iii]);
      //OTMARKPRINTF  printf("\n");

           if (messageIn[i].cksStatus==true) {
             //OTMARKPRINTF  printf("chkStatus ok.\n");
               if (messageIn[i].success==true) {
               //OTMARKPRINTF  printf("success ok.\n");

                 if( smem.vGetINTData(TC92_RedCountVer) == TC_RedCountVerCCT97 ) {
                   if ( messageIn[i].packet[7] == 0xEA && messageIn[i].packet[8] == 0xC3) {
                     ucID = messageIn[i].packet[4];
                     ucLeft = messageIn[i].packet[9];
                     ucRight = messageIn[i].packet[10];
                     screenRedCountHWCheck.vRefreshRedCountHWCheck(ucID, ucLeft, ucRight);
                   }

                 } else {

                   if ( messageIn[i].packet[4] == 0x84 ) {
                     ucID = messageIn[i].packet[5];
                     ucLeft = messageIn[i].packet[6];
                     ucRight = messageIn[i].packet[7];
                     screenRedCountHWCheck.vRefreshRedCountHWCheck(ucID, ucLeft, ucRight);
                   }

                 }

               }
           }
      }
  }

  return true;
} catch(...) {}
}

//OT Pass add
//---------------------------------------------------------------------------
bool READJOB::vDoRelayUDP_MESSAGEIN_0F8F(int *maxMessageIndex,MESSAGEOK *messageIn, unsigned char ucDevFlowIN)
{
try {
  if (*maxMessageIndex >= 0) {
      for (int i=0;i<=*maxMessageIndex;i++) {                                 //檢查每一個messageIn[i]
           if (messageIn[i].cksStatus==true) {                                //通過CheckSum檢測
               if (messageIn[i].success==true                                //通過檢查封包合理性
                && messageIn[i].packet[0] == 0xAA
                && messageIn[i].packet[1] == 0xBB
                && messageIn[i].packet[7] == 0x0F
                && ( messageIn[i].packet[8] == 0x8E || messageIn[i].packet[8] == 0x8F ) //bug?
                ) {
                 writeJob.WritePhysicalOut(messageIn[i].packet, messageIn[i].packetLength, DEVICECENTER92);
                 //then send ack
                 writeJob.WriteACK92_To_Term(messageIn[i].packet[2], messageIn[i].packet[9], messageIn[i].packet[10], ucDevFlowIN);
//                   writeJob.WritePhysicalOutNoSetSeqNoResend(messageIn[i].packet, messageIn[i].packetLength, DEVICECENTER92);
//                   if( messageIn[i].packet[0] == 0xAA && messageIn[i].packet[1] == 0xBB && messageIn[i].packet[7] == 0x0F && messageIn[i].packet[8] == 0x8F) {  //set flag waitting for repsoned of center
//                     smem.vSetACK_W2W(messageIn[i].packet[2], ucDevFlowIN);
//                   }
               }
           }
      }
  }

  return true;

} catch(...) {}
}
