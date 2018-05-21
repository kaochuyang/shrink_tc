//---------------------------------------------------------------------------
#include "WRITEJOB.h"
#include "SMEM.h"
#include "PROTOCOL.h"
#include "CHexToInt.h"
#include <unistd.h>

#include "screenCurrentCommPacket.h"

WRITEJOB writeJob;
//---------------------------------------------------------------------------
WRITEJOB::WRITEJOB(void)
{
}
//---------------------------------------------------------------------------
WRITEJOB::~WRITEJOB(void)
{
}
//---------------------------------------------------------------------------
bool WRITEJOB::WriteWorkByMESSAGEOUT(MESSAGEOK messageOut)
{
try {
    messageOut.ReadorWrite=cWRITE;
    protocol.DoWork(messageOut);
    return true;
  } catch (...) {}
}
//---------------------------------------------------------------------------
bool WRITEJOB::WriteSUCCESS92(BYTE firstCmd,BYTE secondCmd)
{    //AA BB SEQ LCN LCN LEN LEN OF 80 CommandID1 CommandID2 AA CC CKS
try {
    MESSAGEOK messageOut;
    messageOut.protocol=PROTOCOL92;
    messageOut.packetLcn=smem.GetAddress();
    messageOut.packetLength=14;
    messageOut.cksStatus=true;
    messageOut.success=true;
    messageOut.ReadorWrite=cWRITE;

    messageOut.packet[0]=0xAA;
    messageOut.packet[1]=0xBB;
    messageOut.packet[2]=(BYTE)smem.GetSequence();

    if (smem.vGetAdjudicateReturnAddressBCDorHEX() == cBCD ) {                     //Select BCD mode
      messageOut.packet[3]=oHexToInt.vIntToFuckingBCD(1, messageOut.packetLcn);
      messageOut.packet[4]=oHexToInt.vIntToFuckingBCD(0, messageOut.packetLcn);
    }
    else {                                                                      //Normal(HEX) mode
      messageOut.packet[3]=HI(messageOut.packetLcn);
      messageOut.packet[4]=LO(messageOut.packetLcn);
    }

    messageOut.packet[5]=HI(messageOut.packetLength);
    messageOut.packet[6]=LO(messageOut.packetLength);
    messageOut.packet[7]=0x0F;
    messageOut.packet[8]=0x80;
    messageOut.packet[9]=firstCmd;
    messageOut.packet[10]=secondCmd;
    messageOut.packet[11]=0xAA;
    messageOut.packet[12]=0xCC;
    messageOut.packet[13]=0;
    for (int i=0;i<13;i++) messageOut.packet[13]^=messageOut.packet[i];

    protocol.DoWork(messageOut);
    return true;
  } catch (...) {}
}
//---------------------------------------------------------------------------
bool WRITEJOB::WriteFAIL92(BYTE firstCmd,BYTE secondCmd,BYTE errorCode,BYTE parameterNum)
{    //AA BB SEQ LCN LCN LEN LEN OF 81 CommandID1 CommandID2 ErrorCode ParameterNumber AA CC CKS
try {
    MESSAGEOK messageOut;
    messageOut.protocol=PROTOCOL92;
    messageOut.packetLcn=smem.GetAddress();
    messageOut.packetLength=16;
    messageOut.cksStatus=true;
    messageOut.success=true;
    messageOut.ReadorWrite=cWRITE;

    messageOut.packet[0]=0xAA;
    messageOut.packet[1]=0xBB;
    messageOut.packet[2]=(BYTE)smem.GetSequence();

    if (smem.vGetAdjudicateReturnAddressBCDorHEX() == cBCD ) {                     //Select BCD mode
      messageOut.packet[3]=oHexToInt.vIntToFuckingBCD(1, messageOut.packetLcn);
      messageOut.packet[4]=oHexToInt.vIntToFuckingBCD(0, messageOut.packetLcn);
    }
    else {                                                                      //Normal(HEX) mode
      messageOut.packet[3]=HI(messageOut.packetLcn);
      messageOut.packet[4]=LO(messageOut.packetLcn);
    }

    messageOut.packet[5]=HI(messageOut.packetLength);
    messageOut.packet[6]=LO(messageOut.packetLength);
    messageOut.packet[7]=0x0F;
    messageOut.packet[8]=0x81;
    messageOut.packet[9]=firstCmd;
    messageOut.packet[10]=secondCmd;
    messageOut.packet[11]=errorCode;
    messageOut.packet[12]=parameterNum;
    messageOut.packet[13]=0xAA;
    messageOut.packet[14]=0xCC;
    messageOut.packet[15]=0;
    for (int i=0;i<15;i++) messageOut.packet[15]^=messageOut.packet[i];

    protocol.DoWork(messageOut);
    return true;
  } catch (...) {}
}
//---------------------------------------------------------------------------
bool WRITEJOB::WriteSUCCESS87(void)
{
try {
    MESSAGEOK messageOut;
    messageOut.protocol=PROTOCOL87;

    protocol.DoWork(messageOut);
    return true;
  } catch (...) {}
}
//---------------------------------------------------------------------------
bool WRITEJOB::WriteFAIL87(void)
{
try {
    MESSAGEOK messageOut;
    messageOut.protocol=PROTOCOL87;

    protocol.DoWork(messageOut);
    return true;
  } catch (...) {}
}
//---------------------------------------------------------------------------
bool WRITEJOB::WriteACK92(BYTE sequence)
{    //AA DD SEQ LCN LCN LEN LEN CKS
try {
    MESSAGEOK messageOut;
    messageOut.protocol=PROTOCOL92;
    messageOut.packetLcn=smem.GetAddress();
    messageOut.packetLength=8;
    messageOut.cksStatus=true;
    messageOut.success=true;
    messageOut.ReadorWrite=cWRITE;

    messageOut.packet[0]=0xAA;
    messageOut.packet[1]=0xDD;
    messageOut.packet[2]=sequence;

    if (smem.vGetAdjudicateReturnAddressBCDorHEX() == cBCD) {                     //Select BCD mode
      messageOut.packet[3]=oHexToInt.vIntToFuckingBCD(1, messageOut.packetLcn);
      messageOut.packet[4]=oHexToInt.vIntToFuckingBCD(0, messageOut.packetLcn);
    }
    else {                                                                      //Normal(HEX) mode
      messageOut.packet[3]=HI(messageOut.packetLcn);
      messageOut.packet[4]=LO(messageOut.packetLcn);
    }

    messageOut.packet[5]=HI(messageOut.packetLength);
    messageOut.packet[6]=LO(messageOut.packetLength);
    messageOut.packet[7]=0;
    for (int i=0;i<7;i++) messageOut.packet[7]^=messageOut.packet[i];

    protocol.DoWork(messageOut);
    return true;
  } catch (...) {}
}
//---------------------------------------------------------------------------
bool WRITEJOB::WriteNAK92(BYTE sequence,BYTE error)
{    //AA EE SEQ LCN LCN LEN LEN ERR CKS
try {
    MESSAGEOK messageOut;
    messageOut.protocol=PROTOCOL92;
    messageOut.packetLcn=smem.GetAddress();
    messageOut.packetLength=9;
    messageOut.cksStatus=true;
    messageOut.success=true;
    messageOut.ReadorWrite=cWRITE;

    messageOut.packet[0]=0xAA;
    messageOut.packet[1]=0xEE;
    messageOut.packet[2]=sequence;

    if (smem.vGetAdjudicateReturnAddressBCDorHEX() == cBCD) {                     //Select BCD mode
      messageOut.packet[3]=oHexToInt.vIntToFuckingBCD(1, messageOut.packetLcn);
      messageOut.packet[4]=oHexToInt.vIntToFuckingBCD(0, messageOut.packetLcn);
    }
    else {                                                                      //Normal(HEX) mode
      messageOut.packet[3]=HI(messageOut.packetLcn);
      messageOut.packet[4]=LO(messageOut.packetLcn);
    }

    messageOut.packet[5]=HI(messageOut.packetLength);
    messageOut.packet[6]=LO(messageOut.packetLength);
    messageOut.packet[7]=error;
    messageOut.packet[8]=0;
    for (int i=0;i<8;i++) messageOut.packet[8]^=messageOut.packet[i];

    protocol.DoWork(messageOut);
    return true;
  } catch (...) {}
}
//---------------------------------------------------------------------------
bool WRITEJOB::WriteACK77(void)
{    //AA DD LCN LCN CKS
try {
    MESSAGEOK messageOut;
    messageOut.protocol=PROTOCOL77;
    messageOut.packetLcn=smem.GetAddress();
    messageOut.packetLength=5;
    messageOut.cksStatus=true;
    messageOut.success=true;
    messageOut.ReadorWrite=cWRITE;

    messageOut.packet[0]=0xAA;
    messageOut.packet[1]=0xDD;

    if( smem.vGetAdjudicateReturnAddressBCDorHEX() == cBCD ) {                  //Select BCD mode
      messageOut.packet[2]=oHexToInt.vIntToFuckingBCD(1, messageOut.packetLcn);
      messageOut.packet[3]=oHexToInt.vIntToFuckingBCD(0, messageOut.packetLcn);
    }
    else {                                                                      //Normal(HEX) mode
      messageOut.packet[2]=HI(messageOut.packetLcn);
      messageOut.packet[3]=LO(messageOut.packetLcn);
    }

    messageOut.packet[4]=0;
    for (int i=0;i<4;i++) messageOut.packet[4]^=messageOut.packet[i];

    protocol.DoWork(messageOut);
    return true;
  } catch (...) {}
}
//---------------------------------------------------------------------------
bool WRITEJOB::WriteNAK77(BYTE error)
{    //AA EE LCN LCN ERR CKS
try {
    MESSAGEOK messageOut;
    messageOut.protocol=PROTOCOL77;
    messageOut.packetLcn=smem.GetAddress();
    messageOut.packetLength=6;
    messageOut.cksStatus=true;
    messageOut.success=true;
    messageOut.ReadorWrite=cWRITE;

    messageOut.packet[0]=0xAA;
    messageOut.packet[1]=0xEE;

    if( smem.vGetAdjudicateReturnAddressBCDorHEX() == cBCD ) {                     //Select BCD mode
      messageOut.packet[2]=oHexToInt.vIntToFuckingBCD(1, messageOut.packetLcn);
      messageOut.packet[3]=oHexToInt.vIntToFuckingBCD(0, messageOut.packetLcn);
    }
    else {                                                                      //Normal(HEX) mode
      messageOut.packet[2]=HI(messageOut.packetLcn);
      messageOut.packet[3]=LO(messageOut.packetLcn);
    }

    messageOut.packet[4]=error;
    messageOut.packet[5]=0;
    for (int i=0;i<5;i++) messageOut.packet[5]^=messageOut.packet[i];

    protocol.DoWork(messageOut);
    return true;
  } catch (...) {}
}
//---------------------------------------------------------------------------
bool WRITEJOB::WritePhysicalOut(BYTE *packet,int length,int device)
{
try {
    bool statusRs232=false,statusUdp=false;
    int iPacketLcn, iTCLCN;

//OT Debug Direct
    unsigned char ucTmpLight[24];                                               // for traffic light change
    int iRet;

    //OT Pass 2
    int iLEN;
    unsigned short int usiPassS_LCN;

    //OT resend
    bool bSetResendFlag = true;
    if( noRcvAckReSend == false ) bSetResendFlag = false;
    if(packet[0]==(const BYTE)0xAA && packet[1]==(const BYTE)0xBB) { }          // do nothing
    else { bSetResendFlag = false; }

    /*  don't mark will let connect detect error
    if( packet[7]==(const BYTE)0x0F && packet[8]==(const BYTE)0x04 ) { bSetResendFlag = false; }
    if( packet[7]==(const BYTE)0x5F && packet[8]==(const BYTE)0x0F ) { bSetResendFlag = false; }
    if( packet[7]==(const BYTE)0x5F && packet[8]==(const BYTE)0x03 ) { bSetResendFlag = false; }
    if( packet[7]==(const BYTE)0x5F && packet[8]==(const BYTE)0x80 ) { bSetResendFlag = false; }
    */

    switch (device) {

            case DEVICECENTER92:                                                //92¦~ª©¥æ±±¤¤¤ß

//cal iPacketLcn
                 if (smem.vGetAdjudicateReturnAddressBCDorHEX() == cBCD) {  //BCD code½s½X
                     bcdSwitchHi.bcdCode=packet[3];
                     bcdSwitchLo.bcdCode=packet[4];
                     iPacketLcn=bcdSwitchHi.bcdHiLo.b2*1000+bcdSwitchHi.bcdHiLo.b1*100+bcdSwitchLo.bcdHiLo.b2*10+bcdSwitchLo.bcdHiLo.b1;
                 } else {                                                 //16¶i¦ì½s½X
                     iPacketLcn=(int)packet[3]*256+(int)packet[4];
                 }
                 iTCLCN = smem.GetAddress();

                 //porcess for 0F8F OT Pass
//-------------------------------------------------------------------
                 if(smem.vGetPassMode() == cTermPassMode && packet[0] == 0xAA && packet[1] == 0xBB) {
                 if(packet[7] == 0x0F && packet[8] == 0x8F) {}                  // do nothing
                 else {
                 //check rule first
                 //add 4 space
                 for(int i = length - 1; i >= 7; i--) {
                   packet[i+4] = packet[i];
                 }
                 length += 4;

                 // change len
                 iLEN = packet[5] * 256 + packet[6];
                 iLEN += 4;
                 packet[5] = HI(iLEN);
                 packet[6] = LO(iLEN);

                 // change address
                 packet[9] =  packet[3];
                 packet[10] = packet[4];

                 usiPassS_LCN = smem.vGetPassServerLCN();
                 printf("usiPassS_LCN = smem.vGetPassServerLCN() : %d\n", usiPassS_LCN);
                 if (smem.vGetAdjudicateReturnAddressBCDorHEX() == cBCD) {                     //Select BCD mode
                   packet[3]=oHexToInt.vIntToFuckingBCD(1, usiPassS_LCN );
                   packet[4]=oHexToInt.vIntToFuckingBCD(0, usiPassS_LCN );
                 }
                 else {                                                                      //Normal(HEX) mode
                   packet[3]=HI(usiPassS_LCN);
                   packet[4]=LO(usiPassS_LCN);
                 }

                 packet[7] = 0x0F;
                 packet[8] = 0x8F;

                 packet[length-1] = 0;                                          // init checksum
                 for (int j=0; j<length-1; j++) packet[length-1] ^= packet[j];

                 }
                 } // passmode check

//OT FIX 950726 LCN0000 crash center soft bug

//                 printf("smem.centerPort.GetPortAlreadyOpen():%d,iPacketLcn:%d\n", smem.centerPort.GetPortAlreadyOpen(), iPacketLcn);
                 if (smem.centerPort.GetPortAlreadyOpen() && (iPacketLcn != 0) ) {
                     statusRs232=smem.centerPort.Rs232Write(packet,length,"/dev/ttyS0");
//                 if (smem.centerSocket.GetPortAlreadyOpen())
//                     statusUdp=smem.centerSocket.UdpSend(packet,length,"192.168.1.101:6003");
                 //¼g¦^¤¤¤ßªº¤]­n¼g¦^¸ô¤f´ú¸Õ¾÷
                 }


                 if(smem.vGetINTData(Com2_TYPE) == Com2IsTesterPort) {
                 }

//                 if(iTCLCN == iPacketLcn) {                                     //iTCLCN¬°TCªº¸ô¤f½s¸¹¡AiPacketLcn¬°¨ó©w°e¥X¤§¸ô¤f½s¸¹

//OT FIX 950726 LCN0000 crash center soft bug
                   if (smem.DynCalServerInCrossSocket.GetPortAlreadyOpen()  && (iPacketLcn != 0) )
                       statusUdp=smem.DynCalServerInCrossSocket.UdpSend(packet,length,"DynCalServerInCrossSocket");

//OT FIX 950726 LCN0000 crash center soft bug
                   if (smem.DynCalServerInCenterSocket.GetPortAlreadyOpen()  && (iPacketLcn != 0) )
                       statusUdp=smem.DynCalServerInCenterSocket.UdpSend(packet,length,"DynCalServerInCenterSocket");

//OT ADD 961219
//                   printf("smem.centerSocket.GetPortAlreadyOpen():%d,iPacketLcn:%d\n", smem.centerSocket.GetPortAlreadyOpen(), iPacketLcn);
                   if (smem.centerSocket.GetPortAlreadyOpen() && (iPacketLcn != 0)) {
                       statusUdp=smem.centerSocket.UdpSend(packet,length,"centerSocket");
                   }
                   if (smem.testerSocket.GetPortAlreadyOpen() && (iPacketLcn != 0)) {
                       statusUdp=smem.testerSocket.UdpSend(packet,length,"testerSocket");
                   }

//                 }

                 if (bSetResendFlag) {
                     smem.SetWaitMsg(packet[2],packet,length);
                 }

                 screenCurrentCommPacket.vRefreshCurrentScreenPacket(packet, length, "1");

            break;

//OTSS++
            case DEVICESS:                                                      //SMART SENSOR
                 if (smem.ssPort.GetPortAlreadyOpen())
                     statusRs232=smem.ssPort.Rs232Write(packet,length,"/dev/ttyN1");
                 statusUdp=true;
            break;
//OTSS--

            case DEVICEKEYPAD:                                                  //ªF¥Í­±ªO±±¨îÁä½L
                 if (smem.keypadPort.GetPortAlreadyOpen())
                     statusRs232=smem.keypadPort.Rs232Write(packet,length,"/dev/ttyS2");
                 statusUdp=true;
            break;

/*OTCombo0713
            case DEVICETESTER92:                                                //92¦~ª©²{³õ¤â´£´ú¸Õ¾÷
                 if (smem.testerPort.GetPortAlreadyOpen())
                     statusRs232=smem.testerPort.Rs232Write(packet,length,"/dev/ttyS3");
                 if (smem.testerSocket.GetPortAlreadyOpen())
                     statusUdp=smem.testerSocket.UdpSend(packet,length,"192.168.1.102:6003");
                 //¼g¦^¸ô¤f´ú¸Õ¾÷ªº¤]­n¼g¦^¤¤¤ß
            break;
*/
            case DEVICETRAFFICLIGHT:                                            //«Øµ{¬õºñ¿ORS232±±¨îªO
            /*
                 ucTmpLight[0] = 0x55;
                 ucTmpLight[1] = 0x55;
                 ucTmpLight[2] = 0x55;
                 ucTmpLight[3] = 0x55;
                 ucTmpLight[4] = 0x55;
                 ucTmpLight[5] = 0x55;
                 ucTmpLight[6] = 0x55;
                 ucTmpLight[7] = 0x55;
                 ucTmpLight[8] = 0x55;
                 ucTmpLight[9] = 0x55;
                 ucTmpLight[10] = 0x55;
                 ucTmpLight[11] = 0x55;
                 ucTmpLight[12] = 0x55;
                 ucTmpLight[13] = 0x55;
                 ucTmpLight[14] = 0x55;
                 ucTmpLight[15] = 0x55;
                 ucTmpLight[16] = 0x55;
                 ucTmpLight[17] = 0x55;
                 ucTmpLight[18] = 0x55;
                 ucTmpLight[19] = 0x55;
                 ucTmpLight[20] = 0x55;
                 smem.lightPort.Rs232Write(ucTmpLight, 10,"dev-EmptyTrafficLight");
              */

                 if (smem.lightPort.GetPortAlreadyOpen()) {
                   printf("Send writeJob DEVICETRAFFICLIGHT.\n");

                   for(int i = 0; i < 12; i++) {
                     ucTmpLight[i] = 0;
                   }

                   if(packet[2] == 0x13) {  //Do nothing
                   } else {

                     for(int i = 0; i < 6; i++) {
                       iRet = smem.vGetSignamMapMappingLightBoard(i);
                       ucTmpLight[i*2   ] = packet[iRet*2 + 3];
                       ucTmpLight[i*2 +1] = packet[iRet*2 + 4];
                     }
/*
                   for(int i = 0; i < 6; i++) {
                     iRet = smem.vGetSignamMapMappingLightBoard(i);
                     ucTmpLight[iRet*2] = packet[i*2 + 3];
                     ucTmpLight[iRet*2 + 1] = packet[i*2 + 4];
                   }
*/
                     for(int i = 0; i < 12; i++) {
                       packet[i+3] = ucTmpLight[i];
                     }
                     packet[20] = 0;
                     for (int i = 0; i < 20; i++) {
                       packet[20] ^= packet[i];
                     }

                   }
                     statusRs232=smem.lightPort.Rs232Write(packet, length, "/dev/ttyS3" );
//                     smem.lightPort.Rs232Write(ucTmpLight, 2,"dev-EmptyTrafficLight");
//                     usleep(40000);                                             //delay 0.04sec

                 }
                 statusUdp=true;
            break;

            case DEVICEREDCOUNTVER94:
                 ucTmpLight[0] = 0x55;
                 ucTmpLight[1] = 0x55;
                 ucTmpLight[2] = 0x55;
                 ucTmpLight[3] = 0x55;
                 ucTmpLight[4] = 0x55;
                 ucTmpLight[5] = 0x55;
                 ucTmpLight[6] = 0x55;
                 ucTmpLight[7] = 0x55;
                 ucTmpLight[8] = 0x55;
                 ucTmpLight[9] = 0x55;
                 ucTmpLight[10] = 0x55;
                 ucTmpLight[11] = 0x55;
                 ucTmpLight[12] = 0x55;
                 ucTmpLight[13] = 0x55;
                 ucTmpLight[14] = 0x55;
                 ucTmpLight[15] = 0x55;
                 ucTmpLight[16] = 0x55;
                 ucTmpLight[17] = 0x55;
                 ucTmpLight[18] = 0x55;
                 ucTmpLight[19] = 0x55;
                 ucTmpLight[20] = 0x55;

                 if (smem.redCountPort.GetPortAlreadyOpen()) {
//Disable                    smem.redCountPort.Rs232Write(ucTmpLight, 13, "dev-EmptyRedCount");
                   statusRs232 = smem.redCountPort.Rs232Write(packet,length,"dev-RedCount");
//Disable, will afferic return message                   smem.redCountPort.Rs232Write(ucTmpLight, 2,"dev-EmptyRedCount");
//                   statusRs232 = smem.redCountPort.Rs232Write(packet,length,"dev-RedCount");
                   statusUdp=true;
                 }
            break;

            case DEVICEREDCOUNTVERHK:
                 if (smem.redCountPort.GetPortAlreadyOpen()) {
                   ucTmpLight[0] = 0x55;
                   ucTmpLight[1] = 0x55;
                   ucTmpLight[2] = 0x55;
                   ucTmpLight[3] = 0x55;
                   ucTmpLight[4] = 0x55;
                   ucTmpLight[5] = 0x55;
                   ucTmpLight[6] = 0x55;
                   ucTmpLight[7] = 0x55;
                   ucTmpLight[8] = 0x55;
                   ucTmpLight[9] = 0x55;
//Disable                   smem.redCountPort.Rs232Write(ucTmpLight, 10,"dev-EmptyRedCount");
                   statusRs232 = smem.redCountPort.Rs232Write(packet,length,"dev-RedCount");
//                   smem.redCountPort.Rs232Write(ucTmpLight, 10,"dev-EmptyRedCount");
                   statusUdp=true;
                 }
            break;

            case DEVICEREDCOUNTVERCCT97:
                 if (smem.redCountPort.GetPortAlreadyOpen()) {
                   ucTmpLight[0] = 0x55;
                   ucTmpLight[1] = 0x55;
                   ucTmpLight[2] = 0x55;
                   ucTmpLight[3] = 0x55;
                   ucTmpLight[4] = 0x55;
                   ucTmpLight[5] = 0x55;
                   ucTmpLight[6] = 0x55;
                   ucTmpLight[7] = 0x55;
                   ucTmpLight[8] = 0x55;
                   ucTmpLight[9] = 0x55;
                   ucTmpLight[10] = 0x55;
                   ucTmpLight[11] = 0x55;
                   ucTmpLight[12] = 0x55;
                   ucTmpLight[13] = 0x55;
                   ucTmpLight[14] = 0x55;
                   ucTmpLight[15] = 0x55;
                   ucTmpLight[16] = 0x55;
                   ucTmpLight[17] = 0x55;
                   ucTmpLight[18] = 0x55;
                   ucTmpLight[19] = 0x55;
                   ucTmpLight[20] = 0x55;

//Disable                   smem.redCountPort.Rs232Write(ucTmpLight, 13,"dev-EmptyRedCount");
                   statusRs232 = smem.redCountPort.Rs232Write(packet,length,"dev-RedCount");
//                   smem.redCountPort.Rs232Write(ucTmpLight, 10,"dev-EmptyRedCount");
                   statusUdp=true;
                 }
            break;

            case DEVICETAINANPEOPLELIGHT:                                                //92¦~ª©¥æ±±¤¤¤ß
              if (smem.com2Port.GetPortAlreadyOpen())
                  statusRs232 = smem.com2Port.Rs232Write(packet,length,"dev-PGLight");
              statusUdp = true;
            break;
            case DEVICEGREENMANCOUNT:                                                     //chou ++ 2013/05/29
             if(smem.greenmanPort.GetPortAlreadyOpen() ){            //聯嘉綠人倒數專用
                 statusRs232 = smem.greenmanPort.Rs232Write(packet,length,"dev-PedCount");
             }else{}
            break;
            case DEVICENSPREDCOUNT:                                                     //chou ++ 2013/05/29
//OT20131219             if(smem.NSPredCountPort.GetPortAlreadyOpen() && (iPacketLcn != 0) ){
            // printf("smem.NSPredCountPort.GetPortAlreadyOpen():%d\n", smem.NSPredCountPort.GetPortAlreadyOpen());
            // printf("iPacketLcn:%d\n", iPacketLcn);
            if(smem.NSPredCountPort.GetPortAlreadyOpen()){
                 statusRs232 = smem.NSPredCountPort.Rs232Write(packet,length,"dev-redCountNSP"); //chou ++ 2013/06/05
             }else{}                                                                      //聯嘉紅燈倒數專用
            break;

            case DEVICECCJDYNCTL:
            if (smem.vGetAdjudicateReturnAddressBCDorHEX()==cBCD) {  //BCD code½s½X
                bcdSwitchHi.bcdCode=packet[3];
                bcdSwitchLo.bcdCode=packet[4];
                iPacketLcn=bcdSwitchHi.bcdHiLo.b2*1000+bcdSwitchHi.bcdHiLo.b1*100+bcdSwitchLo.bcdHiLo.b2*10+bcdSwitchLo.bcdHiLo.b1;
            } else {                                                 //16¶i¦ì½s½X
                iPacketLcn=(int)packet[3]*256+(int)packet[4];
            }
            iTCLCN = smem.GetAddress();

            if (smem.CCJDynCtlSocket.GetPortAlreadyOpen()  && (iPacketLcn != 0) ) {
                       statusUdp=smem.CCJDynCtlSocket.UdpSend(packet,length,"CCJDynCtlSocket");
            }
            if (smem.CCJDynCtl2Socket.GetPortAlreadyOpen()  && (iPacketLcn != 0) ) {
                       statusUdp=smem.CCJDynCtl2Socket.UdpSend(packet,length,"CCJDynCtl2Socket");
            }


            break;

            case DEVICEREVMANUALPAD:

            if (smem.revLaneManualPort.GetPortAlreadyOpen() )
                statusRs232=smem.revLaneManualPort.Rs232Write(packet,length,"[RevManualPad]-");
            break;

            case DEVICEREVSYNC:
            if (smem.revSyncSocket.GetPortAlreadyOpen()) {
                statusUdp=smem.revSyncSocket.UdpSend(packet,length,"revSyncSocket");
                statusUdp=true;
            }
            break;

            case DEVICEAMEGIDS:
            if (smem.amegidsSocket.GetPortAlreadyOpen()) {
                statusUdp=smem.amegidsSocket.UdpSend(packet,length,"amegidsSocket");
                statusUdp=true;
            }
            break;

            case DEVICEPEDESTRIAN:
            if(smem.PedPushButtonPort.GetPortAlreadyOpen()){
                statusRs232=smem.PedPushButtonPort.Rs232Write(packet,length,"PedPushButtonPort");
            }
            break;
            case DEVICEARWENVDINFO://arwen ++
            if(smem.ArwenVDInfoSocket.GetPortAlreadyOpen() && (iPacketLcn != 0) ){
                statusUdp=smem.ArwenVDInfoSocket.UdpSend(packet,length,"ArwenVDInfoSocket",false);
            }
            break;
            case DEVICEWEBSET://arwen++
            if(smem.WebSetSocket.GetPortAlreadyOpen() && (iPacketLcn != 0) ){
                statusUdp=smem.WebSetSocket.UdpSend(packet,length,"WebSetSocket");
            }
            break;


            //¥H¤U¼È®É¤£³B²z
            case DEVICEUNKNOW:                                                  //¤£ª¾¨ì¬O¤°ªFªF
            case DEVICEMUTILPROTOCOL:                                           //¥i¯à¦³¦hºØ¨ó©w±q³o­Óport¶i¨Ó
            case DEVICECENTER77:                                                //77¦~ª©¥æ±±¤¤¤ß
            case DEVICECENTER87:                                                //87¦~ª©¥æ±±¤¤¤ß
            case DEVICECENTERNTCIP:                                             //NTCIP¦~ª©¥æ±±¤¤¤ß
            case DEVICETESTER77:                                                //77¦~ª©²{³õ¤â´£´ú¸Õ¾÷
            case DEVICETESTER87:                                                //87¦~ª©²{³õ¤â´£´ú¸Õ¾÷
            case DEVICETESTERNTCIP:                                             //NTCIP¦~ª©²{³õ¤â´£´ú¸Õ¾÷
            case DEVICE77TC:                                                    //77¦~ª©¸¹»x±±¨î¾¹
            case DEVICE87TC:                                                    //87¦~ª©¸¹»x±±¨î¾¹
            case DEVICE87CMS:                                                   //87¦~ª©¸ê°T¥iÅÜ±±¨î¾¹
            case DEVICE87VD:                                                    //87¦~ª©°»´ú¾¹±±¨î¾¹
            case DEVICE92TC:                                                    //92¦~ª©¸¹»x±±¨î¾¹
            case DEVICE92CMS:                                                   //92¦~ª©¸ê°T¥iÅÜ±±¨î¾¹
            case DEVICE92VD:                                                    //92¦~ª©°»´ú¾¹±±¨î¾¹
            case DEVICENTCIPTC:                                                 //NTCIP¸¹»x±±¨î¾¹
            case DEVICENTCIPCMS:                                                //NTCIP¸ê°T¥iÅÜ±±¨î¾¹
            case DEVICENTCIPVD:                                                 //NTCIP°»´ú¾¹±±¨î¾¹
            case DEVICERTMS:                                                    //RTMS
            case DEVICEOWNWAY:                                                  //§»·¢¬õ¿O­Ë¼Æ
            case DEVICEEOI:                                                     //«í¹Å¬õ¿O­Ë¼Æ
            case DEVICELISCMS:                                                  //¥x¥_¥ÃµaCMS
            break;

            default:
            break;
    }

    return (statusRs232 && statusUdp);

  } catch (...) {}
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
bool WRITEJOB::WritePhysicalOutNoSetSeqNoResend(BYTE *packet,int length,int device)
{
try {
    bool statusRs232=false,statusUdp=false;
    int iPacketLcn, iTCLCN;

    switch (device) {

            case DEVICECENTER92:                                                //92¦~ª©¥æ±±¤¤¤ß

                 if (smem.vGetAdjudicateReturnAddressBCDorHEX()==cBCD) {  //BCD code½s½X
                     bcdSwitchHi.bcdCode=packet[3];
                     bcdSwitchLo.bcdCode=packet[4];
                     iPacketLcn=bcdSwitchHi.bcdHiLo.b2*1000+bcdSwitchHi.bcdHiLo.b1*100+bcdSwitchLo.bcdHiLo.b2*10+bcdSwitchLo.bcdHiLo.b1;
                 } else {                                                 //16¶i¦ì½s½X
                     iPacketLcn=(int)packet[3]*256+(int)packet[4];
                 }
                 iTCLCN = smem.GetAddress();

                 if (smem.centerPort.GetPortAlreadyOpen() && (iPacketLcn != 0) )
                     statusRs232=smem.centerPort.Rs232Write(packet,length,"/dev/ttyS0");


                 if(smem.vGetINTData(Com2_TYPE) == Com2IsTesterPort) {
                 }

                   if (smem.DynCalServerInCrossSocket.GetPortAlreadyOpen() && (iPacketLcn != 0) )
                       statusUdp=smem.DynCalServerInCrossSocket.UdpSend(packet,length,"DynCalServerInCrossSocket");

                   if (smem.DynCalServerInCenterSocket.GetPortAlreadyOpen() && (iPacketLcn != 0) )
                       statusUdp=smem.DynCalServerInCenterSocket.UdpSend(packet,length,"DynCalServerInCenterSocket");

//OT ADD 961219
                   if (smem.centerSocket.GetPortAlreadyOpen()  && (iPacketLcn != 0) ) {
                       statusUdp=smem.centerSocket.UdpSend(packet,length,"centerSocket");
                   }
                   if (smem.testerSocket.GetPortAlreadyOpen()  && (iPacketLcn != 0) ) {
                       statusUdp=smem.testerSocket.UdpSend(packet,length,"testerSocket");
                   }


                 screenCurrentCommPacket.vRefreshCurrentScreenPacket(packet, length, "1");

            break;

            case DEVICEKEYPAD:                                                  //ªF¥Í­±ªO±±¨îÁä½L
                 if (smem.keypadPort.GetPortAlreadyOpen())
                     statusRs232=smem.keypadPort.Rs232Write(packet,length,"/dev/ttyS2");
                 statusUdp=true;
            break;

            case DEVICETRAFFICLIGHT:                                            //«Øµ{¬õºñ¿ORS232±±¨îªO
                 if (smem.lightPort.GetPortAlreadyOpen()){
                     statusRs232=smem.lightPort.Rs232Write(packet,length,"/dev/ttyS3");
                 }
                 statusUdp=true;
            break;

            case DEVICEREDCOUNTVER94:
                 if (smem.redCountPort.GetPortAlreadyOpen())
                     statusRs232=smem.redCountPort.Rs232Write(packet,length,"dev-RedCount");
                 statusUdp=true;
            break;

            case DEVICEREDCOUNTVERHK:
                 if (smem.redCountPort.GetPortAlreadyOpen())
                     statusRs232=smem.redCountPort.Rs232Write(packet,length,"dev-RedCount");
                 statusUdp=true;
            break;

            case DEVICEREDCOUNTVERCCT97:
                 if (smem.redCountPort.GetPortAlreadyOpen())
                     statusRs232=smem.redCountPort.Rs232Write(packet,length,"dev-RedCountCCT97");
                 statusUdp=true;
            break;

            case DEVICEREVSYNC:
            if (smem.revSyncSocket.GetPortAlreadyOpen())
                statusUdp=smem.revSyncSocket.UdpSend(packet,length,"revSyncSocket");
                statusUdp=true;
            break;

            case DEVICEAMEGIDS:
            if (smem.amegidsSocket.GetPortAlreadyOpen()) {
                statusUdp=smem.amegidsSocket.UdpSend(packet,length,"amegidsSocket");
                statusUdp=true;
            }
            break;

            //OT20140224
            case DEVICEBRT:
            if (smem.BRTSetSocket.GetPortAlreadyOpen()) {
                statusUdp=smem.BRTSetSocket.UdpSend(packet,length,"brtSocket");
                statusUdp=true;
            }
            break;

            //jacky20140327
            case DEVICEJSON:
            if (smem.JsonSetSocket.GetPortAlreadyOpen()) {
                statusUdp=smem.JsonSetSocket.UdpSend(packet,length,"jsonSocket");
                statusUdp=true;
            }
            break;


            //¥H¤U¼È®É¤£³B²z
            case DEVICEUNKNOW:                                                  //¤£ª¾¨ì¬O¤°ªFªF
            case DEVICEMUTILPROTOCOL:                                           //¥i¯à¦³¦hºØ¨ó©w±q³o­Óport¶i¨Ó
            case DEVICECENTER77:                                                //77¦~ª©¥æ±±¤¤¤ß
            case DEVICECENTER87:                                                //87¦~ª©¥æ±±¤¤¤ß
            case DEVICECENTERNTCIP:                                             //NTCIP¦~ª©¥æ±±¤¤¤ß
            case DEVICETESTER77:                                                //77¦~ª©²{³õ¤â´£´ú¸Õ¾÷
            case DEVICETESTER87:                                                //87¦~ª©²{³õ¤â´£´ú¸Õ¾÷
            case DEVICETESTERNTCIP:                                             //NTCIP¦~ª©²{³õ¤â´£´ú¸Õ¾÷
            case DEVICE77TC:                                                    //77¦~ª©¸¹»x±±¨î¾¹
            case DEVICE87TC:                                                    //87¦~ª©¸¹»x±±¨î¾¹
            case DEVICE87CMS:                                                   //87¦~ª©¸ê°T¥iÅÜ±±¨î¾¹
            case DEVICE87VD:                                                    //87¦~ª©°»´ú¾¹±±¨î¾¹
            case DEVICE92TC:                                                    //92¦~ª©¸¹»x±±¨î¾¹
            case DEVICE92CMS:                                                   //92¦~ª©¸ê°T¥iÅÜ±±¨î¾¹
            case DEVICE92VD:                                                    //92¦~ª©°»´ú¾¹±±¨î¾¹
            case DEVICENTCIPTC:                                                 //NTCIP¸¹»x±±¨î¾¹
            case DEVICENTCIPCMS:                                                //NTCIP¸ê°T¥iÅÜ±±¨î¾¹
            case DEVICENTCIPVD:                                                 //NTCIP°»´ú¾¹±±¨î¾¹
            case DEVICERTMS:                                                    //RTMS
            case DEVICEOWNWAY:                                                  //§»·¢¬õ¿O­Ë¼Æ
            case DEVICEEOI:                                                     //«í¹Å¬õ¿O­Ë¼Æ
            case DEVICELISCMS:                                                  //¥x¥_¥ÃµaCMS
            break;

            default:
            break;
    }

    return (statusRs232 && statusUdp);

  } catch (...) {}
}
//---------------------------------------------------------------------------

//OT Pass
//---------------------------------------------------------------------------
bool WRITEJOB::WriteACK92_To_Term(BYTE sequence, BYTE LCNHI, BYTE LCNLO, unsigned char ucDevFlowIN)
{    //AA DD SEQ LCN LCN LEN LEN CKS
try {
    MESSAGEOK messageOut;
    messageOut.protocol=PROTOCOL92;
//    messageOut.packetLcn=smem.GetAddress();
    messageOut.packetLength=8;
    messageOut.cksStatus=true;
    messageOut.success=true;
    messageOut.ReadorWrite=cWRITE;

    messageOut.packet[0]=0xAA;
    messageOut.packet[1]=0xDD;
    messageOut.packet[2]=sequence;

    messageOut.packet[3] = LCNHI;
    messageOut.packet[4] = LCNLO;

    messageOut.packet[5]=HI(messageOut.packetLength);
    messageOut.packet[6]=LO(messageOut.packetLength);
    messageOut.packet[7]=0;
    for (int i=0;i<7;i++) messageOut.packet[7]^=messageOut.packet[i];

//    protocol.DoWork(messageOut);
//    printf("printfMsg ucDevFlowIN:%d\n", ucDevFlowIN);

    switch(ucDevFlowIN) {
      case cVD01ToCenter:
        smem.SSVD01Socket.UdpSend(messageOut.packet, messageOut.packetLength, "SSVD01-ACK->");
      break;
      case cVD02ToCenter:
        smem.SSVD02Socket.UdpSend(messageOut.packet, messageOut.packetLength, "SSVD02-ACK->");
      break;
      case cVD03ToCenter:
        smem.SSVD03Socket.UdpSend(messageOut.packet, messageOut.packetLength, "SSVD03-ACK->");
      break;
      case cVD04ToCenter:
        smem.SSVD04Socket.UdpSend(messageOut.packet, messageOut.packetLength, "SSVD04-ACK->");
      break;
      case cVD05ToCenter:
        smem.SSVD05Socket.UdpSend(messageOut.packet, messageOut.packetLength, "SSVD05-ACK->");
      break;
      case cVD06ToCenter:
        smem.SSVD06Socket.UdpSend(messageOut.packet, messageOut.packetLength, "SSVD06-ACK->");
      break;
      case cVD07ToCenter:
        smem.SSVD07Socket.UdpSend(messageOut.packet, messageOut.packetLength, "SSVD07-ACK->");
      break;
      case cVD08ToCenter:
        smem.SSVD08Socket.UdpSend(messageOut.packet, messageOut.packetLength, "SSVD08-ACK->");
      break;
      case cVD09ToCenter:
        smem.SSVD09Socket.UdpSend(messageOut.packet, messageOut.packetLength, "SSVD09-ACK->");
      break;
      case cVD10ToCenter:
        smem.SSVD10Socket.UdpSend(messageOut.packet, messageOut.packetLength, "SSVD10-ACK->");
      break;
      case cVD11ToCenter:
        smem.SSVD11Socket.UdpSend(messageOut.packet, messageOut.packetLength, "SSVD11-ACK->");
      break;
      case cVD12ToCenter:
        smem.SSVD12Socket.UdpSend(messageOut.packet, messageOut.packetLength, "SSVD12-ACK->");
      break;
      case cVD13ToCenter:
        smem.SSVD13Socket.UdpSend(messageOut.packet, messageOut.packetLength, "SSVD13-ACK->");
      break;
      case cVD14ToCenter:
        smem.SSVD14Socket.UdpSend(messageOut.packet, messageOut.packetLength, "SSVD14-ACK->");
      break;
      case cVD15ToCenter:
        smem.SSVD15Socket.UdpSend(messageOut.packet, messageOut.packetLength, "SSVD15-ACK->");
      break;
      case cVD16ToCenter:
        smem.SSVD16Socket.UdpSend(messageOut.packet, messageOut.packetLength, "SSVD16-ACK->");
      break;
      case cVD17ToCenter:
        smem.SSVD17Socket.UdpSend(messageOut.packet, messageOut.packetLength, "SSVD17-ACK->");
      break;
      case cVD18ToCenter:
        smem.SSVD18Socket.UdpSend(messageOut.packet, messageOut.packetLength, "SSVD18-ACK->");
      break;
      case cVD19ToCenter:
        smem.SSVD19Socket.UdpSend(messageOut.packet, messageOut.packetLength, "SSVD19-ACK->");
      break;
      case cVD20ToCenter:
        smem.SSVD20Socket.UdpSend(messageOut.packet, messageOut.packetLength, "SSVD20-ACK->");
      break;
      case cVD21ToCenter:
        smem.SSVD21Socket.UdpSend(messageOut.packet, messageOut.packetLength, "SSVD21-ACK->");
      break;
      case cVD22ToCenter:
        smem.SSVD22Socket.UdpSend(messageOut.packet, messageOut.packetLength, "SSVD22-ACK->");
      break;
      case cVD23ToCenter:
        smem.SSVD23Socket.UdpSend(messageOut.packet, messageOut.packetLength, "SSVD23-ACK->");
      break;
      case cVD24ToCenter:
        smem.SSVD24Socket.UdpSend(messageOut.packet, messageOut.packetLength, "SSVD24-ACK->");
      break;
      case cVD25ToCenter:
        smem.SSVD25Socket.UdpSend(messageOut.packet, messageOut.packetLength, "SSVD25-ACK->");
      break;
      case cVD26ToCenter:
        smem.SSVD26Socket.UdpSend(messageOut.packet, messageOut.packetLength, "SSVD26-ACK->");
      break;
      case cVD27ToCenter:
        smem.SSVD27Socket.UdpSend(messageOut.packet, messageOut.packetLength, "SSVD27-ACK->");
      break;
      case cVD28ToCenter:
        smem.SSVD28Socket.UdpSend(messageOut.packet, messageOut.packetLength, "SSVD28-ACK->");
      break;
      case cVD29ToCenter:
        smem.SSVD29Socket.UdpSend(messageOut.packet, messageOut.packetLength, "SSVD29-ACK->");
      break;
      case cVD30ToCenter:
        smem.SSVD30Socket.UdpSend(messageOut.packet, messageOut.packetLength, "SSVD30-ACK->");
      break;
/*
      case cVD31ToCenter:
        smem.SSVD31Socket.UdpSend(messageOut.packet, messageOut.packetLength, "SSVD31-ACK->");
      break;
      case cVD32ToCenter:
        smem.SSVD32Socket.UdpSend(messageOut.packet, messageOut.packetLength, "SSVD32-ACK->");
      break;
*/

      default:
      break;
    }

    return true;
  } catch (...) {}
}

//---------------------------------------------------------------------------
bool WRITEJOB::WriteAskW77E58FWVer()
{
try {
  unsigned char ucSendTMP[24];

  ucSendTMP[0] = 0xAA;
  ucSendTMP[1] = 0xBB;
  ucSendTMP[2] = 0x31;
  ucSendTMP[3] = 0;
  ucSendTMP[4] = 0;
  ucSendTMP[5] = 0;
  ucSendTMP[6] = 0;
  ucSendTMP[7] = 0;
  ucSendTMP[8] = 0;
  ucSendTMP[9] = 0;
  ucSendTMP[10] = 0;
  ucSendTMP[11] = 0;
  ucSendTMP[12] = 0;
  ucSendTMP[13] = 0;
  ucSendTMP[14] = 0;
  ucSendTMP[15] = 0;
  ucSendTMP[16] = 0;
  ucSendTMP[17] = 0x00;
  ucSendTMP[18] = 0xAA;
  ucSendTMP[19] = 0xCC;
  for (int i=0; i<20; i++) {
    ucSendTMP[20]^=ucSendTMP[i];
  }

  WritePhysicalOut(ucSendTMP, 21, DEVICETRAFFICLIGHT);
  return true;
} catch (...) {}
}

//---------------------------------------------------------------------------
bool WRITEJOB::WriteLetW77E58AutoControl()
{
try {
  unsigned char ucSendTMP[24];

  printf("go WriteLetW77E58AutoControl\n");
  smem.vWriteMsgToDOM("WriteLetW77E58AutoControl");

  ucSendTMP[0] = 0xAA;
  ucSendTMP[1] = 0xBB;
  ucSendTMP[2] = 0x32;
  ucSendTMP[3] = 0;
  ucSendTMP[4] = 0;
  ucSendTMP[5] = 0;
  ucSendTMP[6] = 0;
  ucSendTMP[7] = 0;
  ucSendTMP[8] = 0;
  ucSendTMP[9] = 0;
  ucSendTMP[10] = 0;
  ucSendTMP[11] = 0;
  ucSendTMP[12] = 0;
  ucSendTMP[13] = 0;
  ucSendTMP[14] = 0;
  ucSendTMP[15] = 0;
  ucSendTMP[16] = 0;
  ucSendTMP[17] = 0x00;
  ucSendTMP[18] = 0xAA;
  ucSendTMP[19] = 0xCC;
  for (int i=0; i<20; i++) {
    ucSendTMP[20] ^= ucSendTMP[i];
  }

  WritePhysicalOut(ucSendTMP, 21, DEVICETRAFFICLIGHT);
  return true;
} catch (...) {}
}

