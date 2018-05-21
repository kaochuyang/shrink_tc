//---------------------------------------------------------------------------
#include "PARSETRAFFIC.h"
#include "SMEM.h"

#include <stdio.h>
#include <string.h>

#include "screenCurrentCommPacket.h"
//---------------------------------------------------------------------------
PARSETRAFFIC::PARSETRAFFIC(void)
{
}
//---------------------------------------------------------------------------
PARSETRAFFIC::~PARSETRAFFIC(void)
{
}
//---------------------------------------------------------------------------
bool PARSETRAFFIC::ParseBlock(int receiveBlockLength,BYTE *block,MESSAGEOK *messageIn,int *lastPacketIndex,int *maxMessageIndex)
{
try {
    int i,j,k;
    unsigned char tempcks;

    i = 0;                         //block[i]
    j = *maxMessageIndex;          //messageIn[j]
    k = *lastPacketIndex;        //messageIn[j].packet[k]
    if(k < 0) {k = 0;}
    if(messageIn[j].uiPacketTEXTLEN == 0) { messageIn[j].uiPacketTEXTLEN = 65530; }
    if(k >= BUFFERSIZE-1) { vClearMsg(messageIn, j); } //should change buffer size

    for (i = 0; i < receiveBlockLength; i++) {

//        printf("%X  ", block[i]);
//        printf("messageIn[%d].cksStatus:%d\n", j, messageIn[j].cksStatus);
//        printf("messageIn[%d].success:%d\n", j, messageIn[j].success);

        messageIn[j].packet[k] = block[i];

        if(k == 0) {
            if(messageIn[j].packet[k] == 0xAA) {
                messageIn[j].bGetDLE = true;
                k++;
            } else {
                if(j > 0) {
                    //special special special case!!
                    messageIn[j] = messageIn[j-1];
                    messageIn[j].cksStatus = false;
                    k = messageIn[j].uiPacketTEXTLEN;
                    messageIn[j].uiPacketTEXTLEN = 65530;
                    messageIn[j].packet[k] = block[i];
                    k++;

                    // printf("\n%X %X\n", messageIn[j].packet[k-1], messageIn[j].packet[k]);

                } else {
                  vClearMsg(messageIn, j);
                  k = 0;
                }

            }
        }
        else if(k == 1) {  //bGetDLE must be true
            if(messageIn[j].packet[k] == 0xBB) {
                messageIn[j].bGetSOH = true;
                k++;
            } else if(messageIn[j].packet[k] == 0xDD) {
                messageIn[j].bGetACK = true;
                k++;
                messageIn[j].uiPacketTEXTLEN = 5;  //set default to 77
                // messageIn[j].uiPacketTEXTLEN = 8;  //set default to 92
            } else if(messageIn[j].packet[k] == 0xEE) {
                messageIn[j].bGetNAK = true;
                k++;
                messageIn[j].uiPacketTEXTLEN = 6;  //set default to 77
                // messageIn[j].uiPacketTEXTLEN = 9;  //set default to 92
            } else {
                vClearMsg(messageIn, j);
                k = 0;
            }
        } else {  //other byte
            if(messageIn[j].packet[k-1] == 0xAA) {
                messageIn[j].bGetDLE = true;
            }
            if(messageIn[j].bGetSOH == true) {  //now in 0xAA 0xBB status
                if(messageIn[j].bGetDLE == true && messageIn[j].packet[k] == 0xCC) {
                    messageIn[j].uiPacketTEXTLEN = k+2;
                }
            }
            k++;
        }
        messageIn[j].bGetDLE = false;  //force clean every time

//        printf("messageIn[%d].uiPacketTEXTLEN:%d\n", j, messageIn[j].uiPacketTEXTLEN);

        if(k == messageIn[j].uiPacketTEXTLEN) {  //check ok or fail by checksum

            for(int n = 0; n < k-2; n++) {
                if(messageIn[j].packet[n] == 0xAA &&
                   (messageIn[j].packet[n+1] == 0xBB ||
                   messageIn[j].packet[n+1] == 0xDD ||
                   messageIn[j].packet[n+1] == 0xEE)   ) {  //find start

                        tempcks = 0;
                        for(int m = n; m < k - 1; m++) {
                           tempcks ^= messageIn[j].packet[m];
                        }
                        // printf("cks:%2X, %d-%d\n", tempcks, n, k);
                        if (messageIn[j].packet[ messageIn[j].uiPacketTEXTLEN-1 ] == tempcks) {
                            messageIn[j].cksStatus = true;
                            //OTTEST messageIn[j].success = true;  //OTTEST

                            if( n != 0) {  //move packet
                                int iMovePtr = 0;
                                for(int m = n; m < k; m++) {
                                    messageIn[j].packet[iMovePtr] = messageIn[j].packet[m];
                                    iMovePtr++;
                                }
                                messageIn[j].uiPacketTEXTLEN -= n;
                            }
                            break;
                        }
                   }
            }

            if(messageIn[j].cksStatus == false) {
                //extern
                if(messageIn[j].bGetSOH == true) {
                    messageIn[j].uiPacketTEXTLEN = 65530;
                    // messageIn[j].usiExternCounter++;
                }
                if(messageIn[j].bGetACK == true) {
                    if(messageIn[j].uiPacketTEXTLEN == 5) {
                      messageIn[j].uiPacketTEXTLEN = 8;  //set to 92
                      // messageIn[j].usiExternCounter++;
                    } else if(messageIn[j].uiPacketTEXTLEN == 8) {  //fail
                        vClearMsg(messageIn, j);
                        k = 0;
                    }
                }
                if(messageIn[j].bGetNAK == true) {
                    if(messageIn[j].uiPacketTEXTLEN == 6) {
                      messageIn[j].uiPacketTEXTLEN = 9;  //set to 92
                      // messageIn[j].usiExternCounter++;
                    } else if(messageIn[j].uiPacketTEXTLEN == 9) {  //fail
                        vClearMsg(messageIn, j);
                        k = 0;
                    }
                }
            } else {
               messageIn[j].packetLength = messageIn[j].uiPacketTEXTLEN;
               j++;

               //20110922
               if(j >= MSGINSIZE) {
                 j--;
               }

               vClearMsg(messageIn, j);
               k = 0;
            }
        }
    }

    *maxMessageIndex = j;
    *lastPacketIndex = k;


    return true;
  } catch(...) {}
}

//---------------------------------------------------------------------------
bool PARSETRAFFIC::vClearMsg(MESSAGEOK *messageIn, unsigned short int msgID)
{
  messageIn[msgID].cksStatus = false;
  messageIn[msgID].success=false;
  messageIn[msgID].UDPorRS232=0;
  messageIn[msgID].ReadorWrite=0;
  messageIn[msgID].bGetDLE = false;
  messageIn[msgID].bGetSOH = false; //the same with STX
  messageIn[msgID].bGetACK = false;
  messageIn[msgID].bGetNAK = false;
  messageIn[msgID].uiPacketTEXTLEN = 65535;

  // messageIn[msgID].usiExternCounter = 0;;

  /*
  for (int b = 0; b < BUFFERSIZE; b++) {
    messageIn[msgID].packet[b]=0;
  }
  */
  bzero(messageIn[msgID].packet, BUFFERSIZE);
}


//---------------------------------------------------------------------------
bool PARSETRAFFIC::DecideProtocol(int *maxMessageIndex,MESSAGEOK *messageIn,int connDevice)    //給予此封包協定代碼
{
try {
    if (*maxMessageIndex >= 0) {
        for (int i=0;i<=*maxMessageIndex;i++) {                                 //檢查每一個messageIn[i]
//        printf("[OTMsg] Go DecideProtocol. C:%d S:%d \n", messageIn[i].cksStatus, messageIn[i].success);
             if (messageIn[i].cksStatus==true) {                                //通過CheckSum檢測

/*Debug Code
             if ((messageIn[i].packet[0]==(const BYTE)0xAA) && (messageIn[i].packet[1]==(const BYTE)0xDD)) {
               printf("printfMsgAADD3 cksStatus:%d, success:%d, SEQ:%d\n",
               messageIn[i].cksStatus, messageIn[i].success, messageIn[i].packet[2]);
             }
*/

                 switch (connDevice) {
                         case DEVICEUNKNOW:                                     //不知道是什東東
                              messageIn[i].protocol=PROTOCOLUNKNOW;
                         break;     //以後再詳細處理
                         case DEVICEMUTILPROTOCOL:                              //可能有多種協定從這個port進來
                              messageIn[i].protocol=PROTOCOLUNKNOW;
                         break;     //以後再詳細處理

                         case DEVICECENTER77:                                   //77年版交控中心
                              messageIn[i].protocol=PROTOCOL77;
                         break;
                         case DEVICECENTER87:                                   //87年版交控中心
                              messageIn[i].protocol=PROTOCOL87;
                         break;
                         case DEVICECENTER92:                                   //92年版交控中心
                              messageIn[i].protocol=PROTOCOL92;
                         break;
                         case DEVICETESTER77:                                   //77年版現場手提測試機
                              messageIn[i].protocol=PROTOCOL77;
                         break;
                         case DEVICETESTER87:                                   //87年版現場手提測試機
                              messageIn[i].protocol=PROTOCOL87;
                         break;
                         case DEVICETESTER92:                                   //92年版現場手提測試機
                              messageIn[i].protocol=PROTOCOL92;
                         break;
                         case DEVICE77TC:                                       //77年版號誌控制器
                              messageIn[i].protocol=PROTOCOL77;
                         break;
                         case DEVICE87TC:                                       //87年版號誌控制器
                              messageIn[i].protocol=PROTOCOL87;
                         break;
                         case DEVICE87CMS:                                      //87年版資訊可變控制器
                              messageIn[i].protocol=PROTOCOL87;
                         break;
                         case DEVICE87VD:                                       //87年版偵測器控制器
                              messageIn[i].protocol=PROTOCOL87;
                         break;
                         case DEVICE92TC:                                       //92年版號誌控制器
                              messageIn[i].protocol=PROTOCOL92;
                         break;
                         case DEVICE92CMS:                                      //92年版資訊可變控制器
                              messageIn[i].protocol=PROTOCOL92;
                         break;
                         case DEVICE92VD:                                       //92年版偵測器控制器
                              messageIn[i].protocol=PROTOCOL92;
                         break;
                         case DEVICEKEYPAD:                                     //東生面板控制鍵盤
                              messageIn[i].protocol=PROTOCOLKEYPAD;
                         break;
                         case DEVICEOWNWAY:                                     //宏楷紅燈倒數
                              messageIn[i].protocol=PROTOCOLOWNWAY;
                         break;
                         case DEVICEEOI:                                        //恆嘉紅燈倒數
                              messageIn[i].protocol=PROTOCOLEOI;
                         break;
                         case DEVICELISCMS:                                     //台北永琦CMS
                              messageIn[i].protocol=PROTOCOLLISCMS;
                         break;
                         /*OTCombo0713*/
                         case DEVICETRAFFICLIGHT:                               //建程RS232紅綠燈面板
                              messageIn[i].protocol=PROTOCOLTRAFFICLIGHT;
                         break;


                         case DEVICERTMS:                                       //RTMS
                         case DEVICESS:                                         //SMART SENSOR
                         case DEVICECENTERNTCIP:                                //NTCIP年版交控中心
                         case DEVICETESTERNTCIP:                                //NTCIP年版現場手提測試機
                         case DEVICENTCIPTC:                                    //NTCIP號誌控制器
                         case DEVICENTCIPCMS:                                   //NTCIP資訊可變控制器
                         case DEVICENTCIPVD:                                    //NTCIP偵測器控制器
                              messageIn[i].protocol=PROTOCOLUNKNOW;
                         break;                                                 //這是不可能出現在PARSETRAFFIC裡面的 只有AA BB,AA DD,AA EE

                         case DEVICEREDCOUNTVER94:
                              messageIn[i].protocol = PROTOCOLREDCOUNTVER94;    //新版紅燈到數器(恆嘉)
                         break;

                         case DEVICEREDCOUNTVERHK:
                              messageIn[i].protocol = PROTOCOLREDCOUNTVERHK;    //舊版紅燈到數器(宏凱)
                         break;

                         case DEVICEREDCOUNTVERCCT97:
                              messageIn[i].protocol = PROTOCOLREDCOUNTVERCCT97;
                         break;


                         case DEVICECCJDYNCTL:
                              messageIn[i].protocol=PROTOCOLCCJ;
                         break;

                         case DEVICEREVMANUALPAD:
                              messageIn[i].protocol = PROTOCOLREVMANUALPAD;
                         break;

                         case DEVICEREVSYNC:
                              messageIn[i].protocol = PROTOCOLREVSYNC;
                         break;

                         case DEVICEAMEGIDS:
                              messageIn[i].protocol = PROTOCOLAMEGIDS;
                         break;
                         case DEVICEPEDESTRIAN:
                              messageIn[i].protocol = PROTOCOLPEDESTRIAN;
                         break;
                         case DEVICEARWENVDINFO://arwen ++
                              messageIn[i].protocol=PROTOCOLARWENVD;
                         break;
                         case DEVICEWEBSET://arwen ++
                              messageIn[i].protocol=PROTOCOLWEBSET;
                         break;

                         //OT20140224
                         case DEVICEBRT:                                   //92年版現場手提測試機
                              messageIn[i].protocol=PROTOCOLBRT;
                         break;

                         //jacky20140426
                         case DEVICECOUNTDOWN:
                              messageIn[i].protocol=PROTOCOLCOUNTDOWN;

                         default:
                         break;
                 }

             }

        }
    }

    return true;

  } catch (...) {}
}

//---------------------------------------------------------------------------
bool PARSETRAFFIC::CheckReasonable(int *maxMessageIndex,MESSAGEOK *messageIn)   //檢查封包合理性
{
try {
    if (*maxMessageIndex >= 0) {
        for (int i=0;i<=*maxMessageIndex;i++) {                                 //檢查每一個messageIn[i]
             if (messageIn[i].cksStatus==true) {                                //通過CheckSum檢測

/*Debug Code
             if ((messageIn[i].packet[0]==(const BYTE)0xAA) && (messageIn[i].packet[1]==(const BYTE)0xDD)) {
               printf("printfMsgAADD4 cksStatus:%d, success:%d, SEQ:%d\n",
               messageIn[i].cksStatus, messageIn[i].success, messageIn[i].packet[2]);
             }
*/

                 switch (messageIn[i].protocol) {
                         case PROTOCOLUNKNOW :                                  //不清楚的協定
                              messageIn[i].success=false;
                         break;
                         case PROTOCOL77:                                       //交通部77年版通訊協定
                              if (messageIn[i].packet[0]==(const BYTE)0xAA) {
                                  if (messageIn[i].packet[1]==(const BYTE)0xBB) {
                                      if (messageIn[i].packetLength>=8) messageIn[i].success=true;
                                      else messageIn[i].success=false;
                                  } else if (messageIn[i].packet[1]==(const BYTE)0xDD) {
                                      if (messageIn[i].packetLength==5) messageIn[i].success=true;
                                      else messageIn[i].success=false;
                                  } else if (messageIn[i].packet[1]==(const BYTE)0xEE) {
                                      if (messageIn[i].packetLength==6) messageIn[i].success=true;
                                      else messageIn[i].success=false;
                                  } else messageIn[i].success=false;
                              } else messageIn[i].success=false;
                         break;
                         case PROTOCOL87:                                       //交通部87年版通訊協定
                              messageIn[i].success=true;
                         break;  //以後再詳細處理
                         case PROTOCOL92:                                       //交通部92年版通訊協定
                              if (messageIn[i].packet[0]==(const BYTE)0xAA) {
                                  if (messageIn[i].packet[1]==(const BYTE)0xBB) {
                                      if (messageIn[i].packetLength>=12 && messageIn[i].packetLength==((int)messageIn[i].packet[5]*256+(int)messageIn[i].packet[6]))
                                          messageIn[i].success=true;
                                      else messageIn[i].success=false;
                                  } else if (messageIn[i].packet[1]==(const BYTE)0xDD) {
                                      if (messageIn[i].packetLength==8) messageIn[i].success=true;
                                      else messageIn[i].success=false;
                                  } else if (messageIn[i].packet[1]==(const BYTE)0xEE) {
                                      if (messageIn[i].packetLength==9) messageIn[i].success=true;
                                      else messageIn[i].success=false;
                                  } else messageIn[i].success=false;
                              } else messageIn[i].success=false;
                         break;
                         case PROTOCOLKEYPAD:                                   //東生面板控制鍵盤通訊協定
                         case PROTOCOLTRAFFICLIGHT:
                              if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                              else messageIn[i].success=false;

                         break;
                         case PROTOCOLOWNWAY:                                   //宏楷紅燈倒數通訊協定
                              messageIn[i].success=false;
                         break;  //宏楷紅燈倒數:不會來到這裡,因為是單向的(控制器至看板)

                         case PROTOCOLREDCOUNTVER94:
                              if (messageIn[i].packet[4] == 0x82 ||
                                  messageIn[i].packet[4] == 0x83 ||
                                  messageIn[i].packet[4] == 0x84    )
                                messageIn[i].success = true;
                              else
                                messageIn[i].success = false;
                         break;

                         case PROTOCOLREDCOUNTVERCCT97:                                       //交通部92年版通訊協定
                              if (messageIn[i].packet[0]==(const BYTE)0xAA) {
                                  if (messageIn[i].packet[1]==(const BYTE)0xBB) {
                                      if (messageIn[i].packetLength>=12 && messageIn[i].packetLength==((int)messageIn[i].packet[5]*256+(int)messageIn[i].packet[6]))
                                          messageIn[i].success=true;
                                      else messageIn[i].success=false;
                                  } else if (messageIn[i].packet[1]==(const BYTE)0xDD) {
                                      if (messageIn[i].packetLength==8) messageIn[i].success=true;
                                      else messageIn[i].success=false;
                                  } else if (messageIn[i].packet[1]==(const BYTE)0xEE) {
                                      if (messageIn[i].packetLength==9) messageIn[i].success=true;
                                      else messageIn[i].success=false;
                                  } else messageIn[i].success=false;
                              } else messageIn[i].success=false;
                         break;


                         case PROTOCOLEOI:                                      //恆嘉紅燈倒數通訊協定
                              messageIn[i].success=true;
                         break;  //以後再詳細處理

                         case PROTOCOLLISCMS:                                   //台北永琦CMS通訊協定
                              messageIn[i].success=true;
                         break;  //以後再詳細處理

                         case PROTOCOLNTCIP:                                    //交通部NTCIP通訊協定
                         case PROTOCOLRTMS:                                     //RTMS通訊協定
                         case PROTOCOLSS:                                       //SMART SENSOR通訊協定
                              messageIn[i].success=false;
                         break;                                                 //此類協定不會在這裡處理

                         case PROTOCOLCCJ:
                              messageIn[i].success=true;
                         break;

                         //OT20140224
                         case PROTOCOLBRT:
                              messageIn[i].success=true;
                         break;

                         //jacky20140426
                         case PROTOCOLCOUNTDOWN:
                              messageIn[i].success=true;
                         break;

                         case PROTOCOLREVMANUALPAD:
                         case PROTOCOLREVSYNC:
                         case PROTOCOLAMEGIDS:
                              messageIn[i].success = true;
                         break;
                         case PROTOCOLPEDESTRIAN:
                              messageIn[i].success = true;
                         break;
                         case PROTOCOLARWENVD:
                              messageIn[i].success = true;
                         break;
                         case PROTOCOLWEBSET:
                              messageIn[i].success = true;
                         break;

                         default:
                         break;
                 }
             }
        }
    }

    return true;

  } catch (...) {}
}
//---------------------------------------------------------------------------
bool PARSETRAFFIC::AssignLcn(int *maxMessageIndex,MESSAGEOK *messageIn)         //根據封包計算LCN
{
try {
    if (*maxMessageIndex >= 0) {
        for (int i=0;i<=*maxMessageIndex;i++) {                                 //檢查每一個messageIn[i]
//        printf("[OTMsg] Go AssignLcn. C:%d S:%d \n", messageIn[i].cksStatus, messageIn[i].success);
             if (messageIn[i].cksStatus==true) {                                //通過CheckSum檢測
                 if (messageIn[i].success==true) {                              //通過檢查封包合理性

                     switch (messageIn[i].protocol) {
                             case PROTOCOL77:                                   //交通部77年版通訊協定
                             case PROTOCOL87:                                   //交通部87年版通訊協定
                                  if (smem.vGetAdjudicateReturnAddressBCDorHEX()==cBCD) {  //BCD code編碼
                                      bcdSwitchHi.bcdCode=messageIn[i].packet[2];
                                      bcdSwitchLo.bcdCode=messageIn[i].packet[3];
                                      messageIn[i].packetLcn=bcdSwitchHi.bcdHiLo.b2*1000+bcdSwitchHi.bcdHiLo.b1*100+bcdSwitchLo.bcdHiLo.b2*10+bcdSwitchLo.bcdHiLo.b1;
                                  } else {                                                 //16進位編碼
                                      messageIn[i].packetLcn=(int)messageIn[i].packet[2]*256+(int)messageIn[i].packet[3];
                                  }
                             break;
                             case PROTOCOL92:                                   //交通部92年版通訊協定
                             case PROTOCOLCCJ:
                             //OT20140224
                             case PROTOCOLBRT:
                             //jacky20140426
                             case PROTOCOLCOUNTDOWN:
                             case PROTOCOLREVMANUALPAD:
                             case PROTOCOLREVSYNC:
                             case PROTOCOLAMEGIDS:
                             case PROTOCOLARWENVD://arwen ++
                             case PROTOCOLWEBSET://arwen ++
                                  if (smem.vGetAdjudicateReturnAddressBCDorHEX()==cBCD) {  //BCD code編碼
                                      bcdSwitchHi.bcdCode=messageIn[i].packet[3];
                                      bcdSwitchLo.bcdCode=messageIn[i].packet[4];
                                      messageIn[i].packetLcn=bcdSwitchHi.bcdHiLo.b2*1000+bcdSwitchHi.bcdHiLo.b1*100+bcdSwitchLo.bcdHiLo.b2*10+bcdSwitchLo.bcdHiLo.b1;
                                  } else {                                                 //16進位編碼
                                      messageIn[i].packetLcn=(int)messageIn[i].packet[3]*256+(int)messageIn[i].packet[4];
                                  }
                             break;
                             case PROTOCOLUNKNOW:                               //不清楚的協定
                             case PROTOCOLNTCIP:                                //交通部NTCIP通訊協定
                             case PROTOCOLKEYPAD:                               //東生面板控制鍵盤通訊協定
                             case PROTOCOLRTMS:                                 //RTMS通訊協定
                             case PROTOCOLSS:                                   //SMART SENSOR通訊協定
                             case PROTOCOLOWNWAY:                               //宏楷紅燈倒數通訊協定
                             case PROTOCOLEOI:                                  //恆嘉紅燈倒數通訊協定
                             case PROTOCOLLISCMS:                               //台北永琦CMS通訊協定
                             case PROTOCOLPEDESTRIAN:
                                  messageIn[i].packetLcn=0;
                             break;     //LCD在此並不適用
                             default:
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
bool PARSETRAFFIC::EchoToGUI(int *maxMessageIndex,MESSAGEOK *messageIn,char *deviceName)      //作此協定相對的事情
{
try {
    if (*maxMessageIndex>=0) {                                                  //確保
        for (int i=0;i<=*maxMessageIndex;i++) {
             if (messageIn[i].cksStatus==true) {                                //通過CheckSum檢測
                 if (messageIn[i].success==true) {                              //通過檢查封包含合理性

                 //OTADD for packet display to screen
                   screenCurrentCommPacket.vRefreshCurrentScreenPacket(messageIn[i].packet, messageIn[i].packetLength, "0");

                     char tempBuff[256],buff[2048]="";
                     char portName[200]="[RECEIVE] ";
                     char temp[6]=" --";

                     strcat(portName,deviceName);
                     strcat(portName,temp);

                     strcat(buff,portName);

                     for (int j=0;j<messageIn[i].packetLength;j++) {
                          sprintf(tempBuff,"%3X",messageIn[i].packet[j]);
                          strcat(buff,tempBuff);
                     }


                     printf("%s\n",buff);

                 }
             }
        }  //end for (int i=0 ; i < maxMessageIndex ; i++)
    }  //end if (maxMessageIndex >= 0)

    return true;
  } catch(...) {}
}

//---------------------------------------------------------------------------
bool PARSETRAFFIC::MoveLastData(int *maxMessageIndex,int *lastPacketIndex,MESSAGEOK *messageIn)
{
try {

    if(messageIn[*maxMessageIndex].cksStatus == true) {
        for(int j = 0; j <= *maxMessageIndex; j++) {
//            printf("messageIn[%d].cks:%d\n", *maxMessageIndex, messageIn[*maxMessageIndex].cksStatus);
            vClearMsg(messageIn, j);
        }
        *maxMessageIndex = 0;
        *lastPacketIndex = 0;
    } else if(*maxMessageIndex > 0) {

        messageIn[0] = messageIn[*maxMessageIndex];

        for(int j = 1; j <= *maxMessageIndex; j++) {
            vClearMsg(messageIn, j);
        }
        *maxMessageIndex = 0;
//        printf("messageIn[%d].cks:%d\n", 0, messageIn[0].cksStatus);
    }

    return true;

  } catch(...) {}
}

//---------------------------------------------------------------------------

/*
//---------------------------------------------------------------------------
//bool PARSETRAFFIC::decide77Or87(int messageInIdx,BYTE controlCode,int length)
bool PARSETRAFFIC::decide77Or87(MESSAGEOK *messageIn)
{
try {
    switch(messageIn.packet[4]) {      //77或87要由控制碼判斷
         //號誌控制器送回中心
         case 0xE7:          //77年版號誌控制器:現行步階與剩餘秒數應答
              messageIn.protocol = PROTOCOL77;
              if (messageIn.packetLength>=12) messageIn.success=true;
         break;
         case 0xE2:          //77年版號誌控制器:現行操作模式應答
         case 0xD2:          //32時段控制器使用
              messageIn.protocol = PROTOCOL77;
              if (messageIn.packetLength>=8) messageIn.success=true;
         break;
         case 0xEC:          //77年版號誌控制器:控制器運作狀態應答
              messageIn.protocol = PROTOCOL77;
              if (messageIn.packetLength>=8) messageIn.success=true;
         break;
         case 0x0E:          //77年版號誌控制器:for 宏眾
              messageIn.protocol = PROTOCOL77;
              if (messageIn.packetLength>=8) messageIn.success=true;
         break;
         case 0x0F:          //77年版號誌控制器:
              messageIn.protocol = PROTOCOL77;
              if (messageIn.packetLength>=8) messageIn.success=true;
         break;
         case 0x0C:          //77年版號誌控制器:Plan2傳送成功
              messageIn.protocol = PROTOCOL77;
              if (messageIn.packetLength>=8) messageIn.success=true;
         break;
         case 0xEF:          //77年版號誌控制器:特殊日時段型態應答
              messageIn.protocol = PROTOCOL77;
              if (messageIn.packetLength>=27) messageIn.success=true;
         break;
         case 0xEB:          //77年版號誌控制器:最近一次箱門開啟及關閉時間
              messageIn.protocol = PROTOCOL77;
              if (messageIn.packetLength>=15) messageIn.success=true;
         break;
         case 0xE3:          //77年版號誌控制器:現行時段起始時間應答
              messageIn.protocol = PROTOCOL77;
              if (messageIn.packetLength>=12) messageIn.success=true;
         break;
         case 0xE9:          //77年版號誌控制器:時段應答
              messageIn.protocol = PROTOCOL77;
              if (messageIn.packetLength>=10) messageIn.success=true;
         break;
         case 0xEA:          //77年版號誌控制器:時段內時制計劃應答
              messageIn.protocol = PROTOCOL77;
              if (messageIn.packetLength>=10) messageIn.success=true;
         break;
         case 0xE0:          //77年版號誌控制器:即時時間應答
              messageIn.protocol = PROTOCOL77;
              if (messageIn.packetLength>=10) messageIn.success=true;
         break;
         case 0xE1:          //77年版號誌控制器:當日日期應答
              messageIn.protocol = PROTOCOL77;
              if (messageIn.packetLength>=11) messageIn.success=true;
         break;
         case 0xE4:          //77年版號誌控制器:第一類訊息應答
              messageIn.protocol = PROTOCOL77;
              if (messageIn.packetLength>=14) messageIn.success=true;
         break;
         case 0xE5:          //77年版號誌控制器:第二類訊息應答
              messageIn.protocol = PROTOCOL77;
              if (messageIn.packetLength>=14) messageIn.success=true;
         break;
         case 0xE8:          //77年版號誌控制器:週內日時段型態應答
              messageIn.protocol = PROTOCOL77;
              if (messageIn.packetLength>=12) messageIn.success=true;
         break;

         //中心送至號誌控制器


         //資訊可變系統控制器送回中心
         case 0x00:          //87年版資訊可變系統控制器:傳送成功
              messageIn.protocol = PROTOCOL87;
              if (messageIn.packetLength>=9) messageIn.success=true;
         break;
         case 0x01:          //87年版資訊可變系統控制器:傳送失敗
              messageIn.protocol = PROTOCOL87;
              if (messageIn.packetLength>=9) messageIn.success=true;
         break;
         case 0x15:          //87年版資訊可變系統控制器:硬體重置回報
              messageIn.protocol = PROTOCOL87;
              if (messageIn.packetLength>=8) messageIn.success=true;
         break;
         case 0x19:          //87年版資訊可變系統控制器:軟體重置回報
              messageIn.protocol = PROTOCOL87;
              if (messageIn.packetLength>=8) messageIn.success=true;
         break;
         case 0x13:          //87年版資訊可變系統控制器:對時誤差過大要求中心對時
              messageIn.protocol = PROTOCOL87;
              if (messageIn.packetLength>=9) messageIn.success=true;
         break;
         case 0x14:          //87年版資訊可變系統控制器:現場設備日期時間回報
              messageIn.protocol = PROTOCOL87;
              if (messageIn.packetLength>=13) messageIn.success=true;
         break;
         case 0x2F:          //87年版資訊可變系統控制器:現場設備傳輸週期回報
              messageIn.protocol = PROTOCOL87;
              if (messageIn.packetLength>=8) messageIn.success=true;
         break;
         case 0x1D:          //87年版資訊可變系統控制器:現場設備硬體狀態回報
              messageIn.protocol = PROTOCOL87;
              if (messageIn.packetLength>=8) messageIn.success=true;
         break;
         case 0x1E:          //87年版資訊可變系統控制器:電源中斷回報
              messageIn.protocol = PROTOCOL87;
              if (messageIn.packetLength>=11) messageIn.success=true;
         break;
         case 0x25:          //87年版資訊可變系統控制器:控制器版本日期回報
              messageIn.protocol = PROTOCOL87;
              if (messageIn.packetLength>=12) messageIn.success=true;
         break;
         case 0x26:          //87年版資訊可變系統控制器:減光控制管理回報
              messageIn.protocol = PROTOCOL87;
              if (messageIn.packetLength>=12) messageIn.success=true;
         break;
         case 0x2A:          //87年版資訊可變系統控制器:減光開始與結束回報
              messageIn.protocol = PROTOCOL87;
              if (messageIn.packetLength>=8) messageIn.success=true;
         break;
         case 0x2E:          //87年版資訊可變系統控制器:迴路測試
              messageIn.protocol = PROTOCOL87;
              if (messageIn.packetLength>=8) messageIn.success=true;
         break;
         case 0x2D:          //87年版資訊可變系統控制器:要求中心下傳控制器資料
              messageIn.protocol = PROTOCOL87;
              if (messageIn.packetLength>=8) messageIn.success=true;
         break;
         case 0x76:          //87年版資訊可變系統控制器:全文訊息回報
              messageIn.protocol = PROTOCOL87;
              if (messageIn.packetLength>=38) messageIn.success=true;
         break;
         case 0x86:          //87年版資訊可變系統控制器:全文色彩回報
              messageIn.protocol = PROTOCOL87;
              if (messageIn.packetLength>=22) messageIn.success=true;
         break;
         case 0xA8:          //87年版資訊可變系統控制器:全文時間回報
              messageIn.protocol = PROTOCOL87;
              if (messageIn.packetLength>=20) messageIn.success=true;
         break;
         case 0x52:          //87年版資訊可變系統控制器:顯示秒數與間距回報
              messageIn.protocol = PROTOCOL87;
              if (messageIn.packetLength>=10) messageIn.success=true;
         break;
         case 0x7F:          //87年版資訊可變系統控制器:單點測試結果回報
              messageIn.protocol = PROTOCOL87;
              if (messageIn.packetLength>=16) messageIn.success=true;
         break;

         //77,87都有
         case 0x10:
              if (messageIn.packetLength==12) {
                     messageIn.protocol = PROTOCOL77;
                     messageIn.success=true;
              } else {
                  messageIn.protocol = PROTOCOL87;
                  if (messageIn.packetLength>=6) messageIn.success=true;
              }
         break;
         case 0x29:          //87年版資訊可變系統控制器:現場設備編號回報
              messageIn.protocol = PROTOCOL87;
              if (messageIn.packetLength>=8) messageIn.success=true;
         break;

         case 0x11:
              if (messageIn.packetLength==11) {
                     messageIn.protocol = PROTOCOL77;
                     messageIn.success=true;
              } else {
                  messageIn.protocol = PROTOCOL87;
                  if (messageIn.packetLength>=6) messageIn.success=true;
              }
         break;
         case 0x21:
              messageIn.protocol = PROTOCOL87;
              if (messageIn.packetLength>=6) messageIn.success=true;
         break;
         case 0x22:
              messageIn.protocol = PROTOCOL87;
              if (messageIn.packetLength>=6) messageIn.success=true;
         break;
         case 0x24:
              messageIn.protocol = PROTOCOL87;
              if (messageIn.packetLength>=6) messageIn.success=true;
         break;
         case 0x27:
              messageIn.protocol = PROTOCOL87;
              if (messageIn.packetLength>=6) messageIn.success=true;
         break;
         case 0x28:
              messageIn.protocol = PROTOCOL87;
              if (messageIn.packetLength>=6) messageIn.success=true;
         break;
         case 0x2C:
              messageIn.protocol = PROTOCOL87;
              if (messageIn.packetLength>=6) messageIn.success=true;
         break;



         //中心送至資訊可變系統控制器
         case 0x12:
              messageIn.protocol = PROTOCOL87;
              if (messageIn.packetLength>=6) messageIn.success=true;
         break;
         case 0x2B:
              messageIn.protocol = PROTOCOL87;
              if (messageIn.packetLength>=6) messageIn.success=true;
         break;
         case 0x16:
              messageIn.protocol = PROTOCOL87;
              if (messageIn.packetLength>=6) messageIn.success=true;
         break;
         case 0x17:
              messageIn.protocol = PROTOCOL87;
              if (messageIn.packetLength>=6) messageIn.success=true;
         break;
         case 0x18:
              messageIn.protocol = PROTOCOL87;
              if (messageIn.packetLength>=6) messageIn.success=true;
         break;
         case 0x1C:
              messageIn.protocol = PROTOCOL87;
              if (messageIn.packetLength>=6) messageIn.success=true;
         break;
         case 0x1F:
              messageIn.protocol = PROTOCOL87;
              if (messageIn.packetLength>=6) messageIn.success=true;
         break;

         case 0x74:
              messageIn.protocol = PROTOCOL87;
              if (messageIn.packetLength>=6) messageIn.success=true;
         break;
         case 0x77:
              messageIn.protocol = PROTOCOL87;
              if (messageIn.packetLength>=6) messageIn.success=true;
         break;
         case 0x75:
              messageIn.protocol = PROTOCOL87;
              if (messageIn.packetLength>=6) messageIn.success=true;
         break;
         case 0x85:
              messageIn.protocol = PROTOCOL87;
              if (messageIn.packetLength>=6) messageIn.success=true;
         break;
         case 0x79:
              messageIn.protocol = PROTOCOL87;
              if (messageIn.packetLength>=6) messageIn.success=true;
         break;
         case 0x7C:
              messageIn.protocol = PROTOCOL87;
              if (messageIn.packetLength>=6) messageIn.success=true;
         break;
         case 0x7E:
              messageIn.protocol = PROTOCOL87;
              if (messageIn.packetLength>=6) messageIn.success=true;
         break;
         case 0xA5:
              messageIn.protocol = PROTOCOL87;
              if (messageIn.packetLength>=6) messageIn.success=true;
         break;
         case 0xA7:
              messageIn.protocol = PROTOCOL87;
              if (messageIn.packetLength>=6) messageIn.success=true;
         break;
         case 0xA6:
              messageIn.protocol = PROTOCOL87;
              if (messageIn.packetLength>=6) messageIn.success=true;
         break;
         case 0x50:
              messageIn.protocol = PROTOCOL87;
              if (messageIn.packetLength>=6) messageIn.success=true;
         break;
         case 0x51:
              messageIn.protocol = PROTOCOL87;
              if (messageIn.packetLength>=6) messageIn.success=true;
         break;
         case 0x55:
              messageIn.protocol = PROTOCOL87;
              if (messageIn.packetLength>=6) messageIn.success=true;
         break;
         case 0x56:
              messageIn.protocol = PROTOCOL87;
              if (messageIn.packetLength>=6) messageIn.success=true;
         break;
         case 0x59:
              messageIn.protocol = PROTOCOL87;
              if (messageIn.packetLength>=6) messageIn.success=true;
         break;

         default: //主要為了中心送往77年版路口控制器的協定
              messageIn.protocol = PROTOCOL77;
              if (messageIn.packetLength>=7) messageIn.success=true;
         break;
    } //end switch
  } catch(...) {}
}
//---------------------------------------------------------------------------
bool PARSETRAFFIC::decideProtocol92Normal(MESSAGEOK *messageIn)
{
try {
    //共用訊息 0F+controlCode
    switch (messageIn.packet[8]) {
        case 0x80:       //回報指令有效       終端控制器->區域控制器(交控中心)
             if (messageIn.packetLength>=13) messageIn.success=true;
        break;
        case 0x81:       //回報指令無效       終端控制器->區域控制器(交控中心)
             if (messageIn.packetLength>=15) messageIn.success=true;
        break;
        case 0x8F:       //現場設備代傳指令上傳    終端控制器->區域控制器->控制中心
             if (messageIn.packetLength>=16) messageIn.success=true;
        break;
        case 0x8E:       //下載代傳指令至路口設備  控制中心->區域控制器->終端控制器
             if (messageIn.packetLength>=16) messageIn.success=true;
        break;
        case 0x10:       //重設定現場設備(硬體重置)   控制中心->終端控制器
             if (messageIn.packetLength>=12) messageIn.success=true;
        break;
        case 0x90:       //回報現場設備重設定 終端控制器->區域控制器(交控中心)
             if (messageIn.packetLength>=12) messageIn.success=true;
        break;
        case 0x40:       //查詢現場設備編號   控制中心->終端控制器
             if (messageIn.packetLength>=12) messageIn.success=true;
        break;
        case 0xC0:       //回報現場設備編號   終端控制器->區域控制器(交控中心)
             if (messageIn.packetLength>=14) messageIn.success=true;
        break;
        case 0x00:       //回報前次電源中斷(設備重新啟動)  終端控制器->區域控制器(交控中心)
             if (messageIn.packetLength>=15) messageIn.success=true;
        break;
        case 0x11:       //重設現場設備之通訊(軟體重置)    控制中心->終端控制器
             if (messageIn.packetLength>=11) messageIn.success=true;
        break;
        case 0x91:       //回報通訊重設定    終端控制器->區域控制器(交控中心)
             if (messageIn.packetLength>=11) messageIn.success=true;
        break;
        case 0x41:       //查詢現場設備狀態     控制中心->終端控制器
             if (messageIn.packetLength>=11) messageIn.success=true;
        break;
        case 0xC1:       //回報查詢現場設備狀態     終端控制器->區域控制器(交控中心)
             if (messageIn.packetLength>=12) messageIn.success=true;
        break;
        case 0x12:       //設定現場設備之日期時間    控制中心->終端控制器
             if (messageIn.packetLength>=18) messageIn.success=true;
        break;
        case 0x92:       //對時誤差回報及要求中心對時   終端控制器->區域控制器(交控中心)
             if (messageIn.packetLength>=12) messageIn.success=true;
        break;
        case 0x42:       //查詢現場設備之日期時間    控制中心->終端控制器
             if (messageIn.packetLength>=11) messageIn.success=true;
        break;
        case 0xC2:       //回報現場設備之日期時間    終端控制器->區域控制器(交控中心)
             if (messageIn.packetLength>=18) messageIn.success=true;
        break;
        case 0x02:       //回報終端設備現場手動更改時間  終端控制器->區域控制器(交控中心)
             if (messageIn.packetLength>=11) messageIn.success=true;
        break;
        case 0x13:       //設定現場設備指令等級    控制中心->終端控制器
             if (messageIn.packetLength>=12) messageIn.success=true;
        break;
        case 0x43:       //查詢軔體燒錄日期版本及指令等級   控制中心->終端控制器
             if (messageIn.packetLength>=11) messageIn.success=true;
        break;
        case 0xC3:       //回報軔體燒錄日期版本及指令等級   終端控制器->區域控制器(交控中心)
             if (messageIn.packetLength>=17) messageIn.success=true;
        break;
        case 0x14:       //設定現場設備回報硬體狀態之週期   控制中心->終端控制器
             if (messageIn.packetLength>=12) messageIn.success=true;
        break;
        case 0x44:       //查詢現場設備回報狀況之傳輸週期   控制中心->終端控制器
             if (messageIn.packetLength>=11) messageIn.success=true;
        break;
        case 0xC4:       //回報現場設備回報狀況之傳輸週期   終端控制器->區域控制器(交控中心)
             if (messageIn.packetLength>=12) messageIn.success=true;
        break;
        case 0x04:       //現場設備回報狀態    終端控制器->區域控制器(交控中心)
             if (messageIn.packetLength>=12) messageIn.success=true;
        break;
        case 0x15:       //設定現場設備操作密碼     控制中心->終端控制器
             if (messageIn.packetLength>=17) messageIn.success=true;
        break;
        case 0x45:       //查詢現場設備操作鎖定密碼    控制中心->終端控制器
             if (messageIn.packetLength>=11) messageIn.success=true;
        break;
        case 0xC5:       //回覆現場設備操作密碼    終端控制器->區域控制器(交控中心)
             if (messageIn.packetLength>=17) messageIn.success=true;
        break;
        case 0x16:       //設定,解除現場資料庫操作鎖定   控制中心->終端控制器
             if (messageIn.packetLength>=12) messageIn.success=true;
        break;
        case 0x46:       //查詢現場資料庫鎖定   控制中心->終端控制器
             if (messageIn.packetLength>=11) messageIn.success=true;
        break;
        case 0xC6:       //回報現場資料庫鎖定   終端控制器->區域控制器(交控中心)
             if (messageIn.packetLength>=12) messageIn.success=true;
        break;
        case 0x47:       //查詢通訊協定回測指令   控制中心->終端控制器
             if (messageIn.packetLength>=12) messageIn.success=true;
        break;
        case 0xC7:       //回報通訊協定回測指令   終端控制器->區域控制器(交控中心)
             if (messageIn.packetLength>=13) messageIn.success=true;
        break;
        default:
        break;
    }
  } catch(...) {}
}
//---------------------------------------------------------------------------
bool PARSETRAFFIC::decideProtocol92Tc(MESSAGEOK *messageIn)
{
try {
    //號誌控制器訊息 0x5F+controlCode
    messageIn.success=true;
    switch (messageIn.packet[8]) {
        case 0x10:       //設定現行控制策略之內容
        break;
        case 0x40:       //查詢現行控制策略之設定內容
        break;
        case 0xC0:       //回報現行控制策略之內容
        break;
        case 0x00:       //自動回報控制策略之現行執行內容
        break;
        case 0x11:       //設定一般日車道調撥控制之參數
        break;
        case 0x41:       //查詢一般日車道調撥控制參數之設定內容
        break;
        case 0xC1:       //回報一般日車道調撥控制參數之設定內容
        break;
        case 0x01:       //自動回報一般車道調撥控制之起迄
        break;
        case 0x12:       //設定特殊日車道調撥之參數
        break;
        case 0x42:       //查詢特殊日車道調撥控制參數之設定內容
        break;
        case 0xC2:       //回報特殊日車道調撥控制參數之設定內容
        break;
        case 0x02:       //自動回報特殊日車道調撥控制之起迄
        break;
        case 0x13:       //設定號誌控制器時相排列
        break;
        case 0x43:       //查詢號誌控制器時相排列之資料
        break;
        case 0xC3:       //回報號誌控制器時相排列之資料
        break;
        case 0x03:       //主動回報號誌控制器步階轉換之資料
        break;
        case 0x14:       //設定路口時制計劃之基本參數
        break;
        case 0x44:       //查詢路口時制計劃的基本參數
        break;
        case 0xC4:       //回報路口時制計畫的基本參數
        break;
        case 0x15:       //設定時制計劃內容
        break;
        case 0x45:       //查詢時制計劃之設定內容
        break;
        case 0xC5:       //回報時制計劃之設定內容
        break;
        case 0x16:       //設定一般日時段型態
        break;
        case 0x46:       //查詢一般日時段型態之設定內容
        break;
        case 0xC6:       //回報一般日時段型態之設定內容
        break;
        case 0x17:       //設定特殊日時段型態
        break;
        case 0x47:       //查詢特殊日時段型態之設定內容
        break;
        case 0xC7:       //回報特殊日時段型態之設定內容
        break;
        case 0x18:       //選擇執行之時制計劃
        break;
        case 0x48:       //查詢目前時制計劃內容
        break;
        case 0xC8:       //回報目前時制計劃內容
        break;
        case 0x19:       //設定觸動控制組態
        break;
        case 0x49:       //查詢觸動控制組態內容
        break;
        case 0xC9:       //回報觸動控制組態內容
        break;
        case 0x09:       //觸動後回報觸動控制之進行
        break;
        case 0x1A:       //設定一般日觸動時段型態
        break;
        case 0x4A:       //查詢一般日觸動型態之設定內容
        break;
        case 0xCA:       //回報一般日觸動時段型態之設定內容
        break;
        case 0x1B:       //設定特殊日期觸動時段行態
        break;
        case 0x4B:       //查詢特殊日期觸動時段型態之設定內容
        break;
        case 0xCB:       //回報特殊日觸動時段型態之設定內容
        break;
        case 0x1C:       //時相或步階變換控制
        break;
        case 0x4C:       //查詢號誌控制器現行時相及步階
        break;
        case 0xCC:       //回報號誌控制器現行時相及步階與執行秒數
        break;
        case 0x0C:       //主動回報號誌控制器現行時相及步階
        break;
        case 0x1D:       //臨界路口控制
        break;
        case 0x1E:       //設定特勤路線控制參數
        break;
        case 0x4E:       //查詢特勤路線控制參數之設定內容
        break;
        case 0xCE:       //回報特勤路線控制參數之設定內容
        break;
        case 0x2F:       //設定號制控制器時相排列
        break;
        case 0x5F:       //查詢號制控制器時相排列之資料
        break;
        case 0xDF:       //回報號制控制器時相排列之資料
        break;
        case 0x31:       //設定母機連鎖輸出組態
        break;
        case 0x61:       //查詢母機連鎖輸出組態
        break;
        case 0xE1:       //回報母機連鎖輸出組態
        break;
        case 0x32:       //設定子機連鎖設定組態
        break;
        case 0x62:       //查詢子機連鎖設定組態
        break;
        case 0xE2:       //回報子機連鎖設定組態
        break;
        case 0x33:       //設定子機控制組態
        break;
        case 0x63:       //查詢子機控制組態的設定內容
        break;
        case 0xE3:       //回報子機控制組態的設定內容
        break;
        case 0x08:       //回報號誌控制器現場操作
        break;
        case 0x0A:       //回報現場資料更動
        break;
        case 0x0B:       //要求控制中心下傳資料庫資料
        break;
        case 0x3F:       //設定資料回報傳輸週期
        break;
        case 0x6F:       //查詢現場設備交通參數之傳輸週期
        break;
        case 0xEF:       //查詢後回報現場燈態或步階轉換資料之回報傳輸週期
        break;
        case 0x0F:       //重覆傳輸路口號制控制之燈態狀態
        break;
        case 0x3E:       //設定現場設備減光控制狀態
        break;
        case 0x6E:       //查詢現場設備減光設定
        break;
        case 0xEE:       //回報現場設備減光控制狀態
        break;
        case 0x0E:       //回報排程減光控制之起迄
        break;
        default:
        break;
    }
  } catch(...) {}
}
//---------------------------------------------------------------------------
bool PARSETRAFFIC::decideProtocol92Vd(MESSAGEOK *messageIn)
{
try {
    //車輛偵側器訊息 0x6F+controlCode
    switch (messageIn.packet[8]) {
        default:
            messageIn.success=true;
        break;
    }
  } catch(...) {}
}
//---------------------------------------------------------------------------
bool PARSETRAFFIC::decideProtocol92Cms(MESSAGEOK *messageIn)
{
try {
    //資訊可變標誌訊息 0xAF+controlCode
    switch (messageIn.packet[8]) {
        default:
            messageIn.success=true;
        break;
    }
  } catch(...) {}
}
*/

//---------------------------------------------------------------------------
bool PARSETRAFFIC::vJudgeProtocolAndCheckLength(int *maxMessageIndex,MESSAGEOK *messageIn)
{
try {
//0:無連接 1:交控中心 2:鍵盤開關輸入介面卡 3:RTMS 4:代傳路口77年號誌控制器 5:代傳路口92年號誌控制器 6.代傳路口87年資訊可變控制器 7.代傳路口92年資訊可變控制器 8.宏楷紅燈倒數 9.恆嘉紅燈倒數
      //以上標準77,87,92通訊協定,皆為AA BB.....AA CC CKS 或 AA DD..... 或 AA EE.....


      if (*maxMessageIndex >= 0) {
          for (int i=0;i<=*maxMessageIndex;i++) {                                 //檢查每一個messageIn[i]
               if (messageIn[i].cksStatus==true) {                                //通過CheckSum檢測

/*Debug Code
               if ((messageIn[i].packet[0]==(const BYTE)0xAA) && (messageIn[i].packet[1]==(const BYTE)0xDD)) {
                 printf("printfMsgAADD2 cksStatus:%d, success:%d, SEQ:%d\n",
                 messageIn[i].cksStatus, messageIn[i].success, messageIn[i].packet[2]);
               }
*/


            if (messageIn[i].packet[0]==(const BYTE)0xAA) {
                if (messageIn[i].packet[1]==(const BYTE)0xBB) {

                    if (messageIn[i].packetLength>=12) {                      //長度必須要>12才有可能為92年的版本 aa bb 0 hiLcn loLcn hiLen loLen eqpCode cmdCode aa cc cks
                        if (messageIn[i].packetLength==((int)messageIn[i].packet[5]*256+(int)messageIn[i].packet[6])) {    //92年版通訊協定
                            messageIn[i].protocol = PROTOCOL92;                          //92年版通訊協定

                            //OT Debug 0706 fuck change
                            messageIn[i].success=true;

                            if (messageIn[i].packet[7]==0x0F) {               //92年版共用訊息
//                                decideProtocol92Normal(messageIn[i]);
//================================================================
                                //共用訊息 0F+controlCode
                                switch (messageIn[i].packet[8]) {
                                    case 0x80:       //回報指令有效       終端控制器->區域控制器(交控中心)
                                         if (messageIn[i].packetLength>=13) messageIn[i].success=true;
                                    break;
                                    case 0x81:       //回報指令無效       終端控制器->區域控制器(交控中心)
                                         if (messageIn[i].packetLength>=15) messageIn[i].success=true;
                                    break;
                                    case 0x8F:       //現場設備代傳指令上傳    終端控制器->區域控制器->控制中心
                                         if (messageIn[i].packetLength>=16) messageIn[i].success=true;
                                    break;
                                    case 0x8E:       //下載代傳指令至路口設備  控制中心->區域控制器->終端控制器
                                         if (messageIn[i].packetLength>=16) messageIn[i].success=true;
                                    break;
                                    case 0x10:       //重設定現場設備(硬體重置)   控制中心->終端控制器
                                         if (messageIn[i].packetLength>=12) messageIn[i].success=true;
                                    break;
                                    case 0x90:       //回報現場設備重設定 終端控制器->區域控制器(交控中心)
                                         if (messageIn[i].packetLength>=12) messageIn[i].success=true;
                                    break;
                                    case 0x40:       //查詢現場設備編號   控制中心->終端控制器
                                         if (messageIn[i].packetLength>=12) messageIn[i].success=true;
                                    break;
                                    case 0xC0:       //回報現場設備編號   終端控制器->區域控制器(交控中心)
                                         if (messageIn[i].packetLength>=14) messageIn[i].success=true;
                                    break;
                                    case 0x00:       //回報前次電源中斷(設備重新啟動)  終端控制器->區域控制器(交控中心)
                                         if (messageIn[i].packetLength>=15) messageIn[i].success=true;
                                    break;
                                    case 0x11:       //重設現場設備之通訊(軟體重置)    控制中心->終端控制器
                                         if (messageIn[i].packetLength>=11) messageIn[i].success=true;
                                    break;
                                    case 0x91:       //回報通訊重設定    終端控制器->區域控制器(交控中心)
                                         if (messageIn[i].packetLength>=11) messageIn[i].success=true;
                                    break;
                                    case 0x41:       //查詢現場設備狀態     控制中心->終端控制器
                                         if (messageIn[i].packetLength>=11) messageIn[i].success=true;
                                    break;
                                    case 0xC1:       //回報查詢現場設備狀態     終端控制器->區域控制器(交控中心)
                                         if (messageIn[i].packetLength>=12) messageIn[i].success=true;
                                    break;
                                    case 0x12:       //設定現場設備之日期時間    控制中心->終端控制器
                                         if (messageIn[i].packetLength>=18) messageIn[i].success=true;
                                    break;
                                    case 0x92:       //對時誤差回報及要求中心對時   終端控制器->區域控制器(交控中心)
                                         if (messageIn[i].packetLength>=12) messageIn[i].success=true;
                                    break;
                                    case 0x42:       //查詢現場設備之日期時間    控制中心->終端控制器
                                         if (messageIn[i].packetLength>=11) messageIn[i].success=true;
                                    break;
                                    case 0xC2:       //回報現場設備之日期時間    終端控制器->區域控制器(交控中心)
                                         if (messageIn[i].packetLength>=18) messageIn[i].success=true;
                                    break;
                                    case 0x02:       //回報終端設備現場手動更改時間  終端控制器->區域控制器(交控中心)
                                         if (messageIn[i].packetLength>=11) messageIn[i].success=true;
                                    break;
                                    case 0x13:       //設定現場設備指令等級    控制中心->終端控制器
                                         if (messageIn[i].packetLength>=12) messageIn[i].success=true;
                                    break;
                                    case 0x43:       //查詢軔體燒錄日期版本及指令等級   控制中心->終端控制器
                                         if (messageIn[i].packetLength>=11) messageIn[i].success=true;
                                    break;
                                    case 0xC3:       //回報軔體燒錄日期版本及指令等級   終端控制器->區域控制器(交控中心)
                                         if (messageIn[i].packetLength>=17) messageIn[i].success=true;
                                    break;
                                    case 0x14:       //設定現場設備回報硬體狀態之週期   控制中心->終端控制器
                                         if (messageIn[i].packetLength>=12) messageIn[i].success=true;
                                    break;
                                    case 0x44:       //查詢現場設備回報狀況之傳輸週期   控制中心->終端控制器
                                         if (messageIn[i].packetLength>=11) messageIn[i].success=true;
                                    break;
                                    case 0xC4:       //回報現場設備回報狀況之傳輸週期   終端控制器->區域控制器(交控中心)
                                         if (messageIn[i].packetLength>=12) messageIn[i].success=true;
                                    break;
                                    case 0x04:       //現場設備回報狀態    終端控制器->區域控制器(交控中心)
                                         if (messageIn[i].packetLength>=12) messageIn[i].success=true;
                                    break;
                                    case 0x15:       //設定現場設備操作密碼     控制中心->終端控制器
                                         if (messageIn[i].packetLength>=17) messageIn[i].success=true;
                                    break;
                                    case 0x45:       //查詢現場設備操作鎖定密碼    控制中心->終端控制器
                                         if (messageIn[i].packetLength>=11) messageIn[i].success=true;
                                    break;
                                    case 0xC5:       //回覆現場設備操作密碼    終端控制器->區域控制器(交控中心)
                                         if (messageIn[i].packetLength>=17) messageIn[i].success=true;
                                    break;
                                    case 0x16:       //設定,解除現場資料庫操作鎖定   控制中心->終端控制器
                                         if (messageIn[i].packetLength>=12) messageIn[i].success=true;
                                    break;
                                    case 0x46:       //查詢現場資料庫鎖定   控制中心->終端控制器
                                         if (messageIn[i].packetLength>=11) messageIn[i].success=true;
                                    break;
                                    case 0xC6:       //回報現場資料庫鎖定   終端控制器->區域控制器(交控中心)
                                         if (messageIn[i].packetLength>=12) messageIn[i].success=true;
                                    break;
                                    case 0x47:       //查詢通訊協定回測指令   控制中心->終端控制器
                                         if (messageIn[i].packetLength>=12) messageIn[i].success=true;
                                    break;
                                    case 0xC7:       //回報通訊協定回測指令   終端控制器->區域控制器(交控中心)
                                         if (messageIn[i].packetLength>=13) messageIn[i].success=true;
                                    break;
                                    default:
                                    break;
                                }
//================================================================

                            } else if (messageIn[i].packet[7]==0x5F) {        //92年版號誌控制器訊息
//                                decideProtocol92Tc(messageIn[i]);
//================================================================
                                //號誌控制器訊息 0x5F+controlCode
                                messageIn[i].success=true;
                                switch (messageIn[i].packet[8]) {
                                    case 0x10:       //設定現行控制策略之內容
                                    break;
                                    case 0x40:       //查詢現行控制策略之設定內容
                                    break;
                                    case 0xC0:       //回報現行控制策略之內容
                                    break;
                                    case 0x00:       //自動回報控制策略之現行執行內容
                                    break;
                                    case 0x11:       //設定一般日車道調撥控制之參數
                                    break;
                                    case 0x41:       //查詢一般日車道調撥控制參數之設定內容
                                    break;
                                    case 0xC1:       //回報一般日車道調撥控制參數之設定內容
                                    break;
                                    case 0x01:       //自動回報一般車道調撥控制之起迄
                                    break;
                                    case 0x12:       //設定特殊日車道調撥之參數
                                    break;
                                    case 0x42:       //查詢特殊日車道調撥控制參數之設定內容
                                    break;
                                    case 0xC2:       //回報特殊日車道調撥控制參數之設定內容
                                    break;
                                    case 0x02:       //自動回報特殊日車道調撥控制之起迄
                                    break;
                                    case 0x13:       //設定號誌控制器時相排列
                                    break;
                                    case 0x43:       //查詢號誌控制器時相排列之資料
                                    break;
                                    case 0xC3:       //回報號誌控制器時相排列之資料
                                    break;
                                    case 0x03:       //主動回報號誌控制器步階轉換之資料
                                    break;
                                    case 0x14:       //設定路口時制計劃之基本參數
                                    break;
                                    case 0x44:       //查詢路口時制計劃的基本參數
                                    break;
                                    case 0xC4:       //回報路口時制計畫的基本參數
                                    break;
                                    case 0x15:       //設定時制計劃內容
                                    break;
                                    case 0x45:       //查詢時制計劃之設定內容
                                    break;
                                    case 0xC5:       //回報時制計劃之設定內容
                                    break;
                                    case 0x16:       //設定一般日時段型態
                                    break;
                                    case 0x46:       //查詢一般日時段型態之設定內容
                                    break;
                                    case 0xC6:       //回報一般日時段型態之設定內容
                                    break;
                                    case 0x17:       //設定特殊日時段型態
                                    break;
                                    case 0x47:       //查詢特殊日時段型態之設定內容
                                    break;
                                    case 0xC7:       //回報特殊日時段型態之設定內容
                                    break;
                                    case 0x18:       //選擇執行之時制計劃
                                    break;
                                    case 0x48:       //查詢目前時制計劃內容
                                    break;
                                    case 0xC8:       //回報目前時制計劃內容
                                    break;
                                    case 0x19:       //設定觸動控制組態
                                    break;
                                    case 0x49:       //查詢觸動控制組態內容
                                    break;
                                    case 0xC9:       //回報觸動控制組態內容
                                    break;
                                    case 0x09:       //觸動後回報觸動控制之進行
                                    break;
                                    case 0x1A:       //設定一般日觸動時段型態
                                    break;
                                    case 0x4A:       //查詢一般日觸動型態之設定內容
                                    break;
                                    case 0xCA:       //回報一般日觸動時段型態之設定內容
                                    break;
                                    case 0x1B:       //設定特殊日期觸動時段行態
                                    break;
                                    case 0x4B:       //查詢特殊日期觸動時段型態之設定內容
                                    break;
                                    case 0xCB:       //回報特殊日觸動時段型態之設定內容
                                    break;
                                    case 0x1C:       //時相或步階變換控制
                                    break;
                                    case 0x4C:       //查詢號誌控制器現行時相及步階
                                    break;
                                    case 0xCC:       //回報號誌控制器現行時相及步階與執行秒數
                                    break;
                                    case 0x0C:       //主動回報號誌控制器現行時相及步階
                                    break;
                                    case 0x1D:       //臨界路口控制
                                    break;
                                    case 0x1E:       //設定特勤路線控制參數
                                    break;
                                    case 0x4E:       //查詢特勤路線控制參數之設定內容
                                    break;
                                    case 0xCE:       //回報特勤路線控制參數之設定內容
                                    break;
                                    case 0x2F:       //設定號制控制器時相排列
                                    break;
                                    case 0x5F:       //查詢號制控制器時相排列之資料
                                    break;
                                    case 0xDF:       //回報號制控制器時相排列之資料
                                    break;
                                    case 0x31:       //設定母機連鎖輸出組態
                                    break;
                                    case 0x61:       //查詢母機連鎖輸出組態
                                    break;
                                    case 0xE1:       //回報母機連鎖輸出組態
                                    break;
                                    case 0x32:       //設定子機連鎖設定組態
                                    break;
                                    case 0x62:       //查詢子機連鎖設定組態
                                    break;
                                    case 0xE2:       //回報子機連鎖設定組態
                                    break;
                                    case 0x33:       //設定子機控制組態
                                    break;
                                    case 0x63:       //查詢子機控制組態的設定內容
                                    break;
                                    case 0xE3:       //回報子機控制組態的設定內容
                                    break;
                                    case 0x08:       //回報號誌控制器現場操作
                                    break;
                                    case 0x0A:       //回報現場資料更動
                                    break;
                                    case 0x0B:       //要求控制中心下傳資料庫資料
                                    break;
                                    case 0x3F:       //設定資料回報傳輸週期
                                    break;
                                    case 0x6F:       //查詢現場設備交通參數之傳輸週期
                                    break;
                                    case 0xEF:       //查詢後回報現場燈態或步階轉換資料之回報傳輸週期
                                    break;
                                    case 0x0F:       //重覆傳輸路口號制控制之燈態狀態
                                    break;
                                    case 0x3E:       //設定現場設備減光控制狀態
                                    break;
                                    case 0x6E:       //查詢現場設備減光設定
                                    break;
                                    case 0xEE:       //回報現場設備減光控制狀態
                                    break;
                                    case 0x0E:       //回報排程減光控制之起迄
                                    break;
                                    default:
                                    break;
                                }

                            } else if (messageIn[i].packet[7]==0x6F) {        //92年版車輛偵測器訊息
//                                decideProtocol92Vd(messageIn[i]);
//================================================================
                                //車輛偵側器訊息 0x6F+controlCode
                                switch (messageIn[i].packet[8]) {
                                    default:
                                        messageIn[i].success=true;
                                    break;
                                }

                            } else if (messageIn[i].packet[7]==0xAF) {        //92年版資訊可變控制器訊息
//                                decideProtocol92Cms(messageIn[i]);
//================================================================
                                switch (messageIn[i].packet[8]) {
                                    default:
                                        messageIn[i].success=true;
                                    break;
                                }

                            }

                        } else {                                                         //77或87年版通訊協定
//                            decide77Or87(messageIn[i]);
//================================================================
                            switch(messageIn[i].packet[4]) {      //77或87要由控制碼判斷
                                 //號誌控制器送回中心
                                 case 0xE7:          //77年版號誌控制器:現行步階與剩餘秒數應答
                                      messageIn[i].protocol = PROTOCOL77;
                                      if (messageIn[i].packetLength>=12) messageIn[i].success=true;
                                 break;
                                 case 0xE2:          //77年版號誌控制器:現行操作模式應答
                                 case 0xD2:          //32時段控制器使用
                                      messageIn[i].protocol = PROTOCOL77;
                                      if (messageIn[i].packetLength>=8) messageIn[i].success=true;
                                 break;
                                 case 0xEC:          //77年版號誌控制器:控制器運作狀態應答
                                      messageIn[i].protocol = PROTOCOL77;
                                      if (messageIn[i].packetLength>=8) messageIn[i].success=true;
                                 break;
                                 case 0x0E:          //77年版號誌控制器:for 宏眾
                                      messageIn[i].protocol = PROTOCOL77;
                                      if (messageIn[i].packetLength>=8) messageIn[i].success=true;
                                 break;
                                 case 0x0F:          //77年版號誌控制器:
                                      messageIn[i].protocol = PROTOCOL77;
                                      if (messageIn[i].packetLength>=8) messageIn[i].success=true;
                                 break;
                                 case 0x0C:          //77年版號誌控制器:Plan2傳送成功
                                      messageIn[i].protocol = PROTOCOL77;
                                      if (messageIn[i].packetLength>=8) messageIn[i].success=true;
                                 break;
                                 case 0xEF:          //77年版號誌控制器:特殊日時段型態應答
                                      messageIn[i].protocol = PROTOCOL77;
                                      if (messageIn[i].packetLength>=27) messageIn[i].success=true;
                                 break;
                                 case 0xEB:          //77年版號誌控制器:最近一次箱門開啟及關閉時間
                                      messageIn[i].protocol = PROTOCOL77;
                                      if (messageIn[i].packetLength>=15) messageIn[i].success=true;
                                 break;
                                 case 0xE3:          //77年版號誌控制器:現行時段起始時間應答
                                      messageIn[i].protocol = PROTOCOL77;
                                      if (messageIn[i].packetLength>=12) messageIn[i].success=true;
                                 break;
                                 case 0xE9:          //77年版號誌控制器:時段應答
                                      messageIn[i].protocol = PROTOCOL77;
                                      if (messageIn[i].packetLength>=10) messageIn[i].success=true;
                                 break;
                                 case 0xEA:          //77年版號誌控制器:時段內時制計劃應答
                                      messageIn[i].protocol = PROTOCOL77;
                                      if (messageIn[i].packetLength>=10) messageIn[i].success=true;
                                 break;
                                 case 0xE0:          //77年版號誌控制器:即時時間應答
                                      messageIn[i].protocol = PROTOCOL77;
                                      if (messageIn[i].packetLength>=10) messageIn[i].success=true;
                                 break;
                                 case 0xE1:          //77年版號誌控制器:當日日期應答
                                      messageIn[i].protocol = PROTOCOL77;
                                      if (messageIn[i].packetLength>=11) messageIn[i].success=true;
                                 break;
                                 case 0xE4:          //77年版號誌控制器:第一類訊息應答
                                      messageIn[i].protocol = PROTOCOL77;
                                      if (messageIn[i].packetLength>=14) messageIn[i].success=true;
                                 break;
                                 case 0xE5:          //77年版號誌控制器:第二類訊息應答
                                      messageIn[i].protocol = PROTOCOL77;
                                      if (messageIn[i].packetLength>=14) messageIn[i].success=true;
                                 break;
                                 case 0xE8:          //77年版號誌控制器:週內日時段型態應答
                                      messageIn[i].protocol = PROTOCOL77;
                                      if (messageIn[i].packetLength>=12) messageIn[i].success=true;
                                 break;

                                 //中心送至號誌控制器


                                 //資訊可變系統控制器送回中心
                                 case 0x00:          //87年版資訊可變系統控制器:傳送成功
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=9) messageIn[i].success=true;
                                 break;
                                 case 0x01:          //87年版資訊可變系統控制器:傳送失敗
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=9) messageIn[i].success=true;
                                 break;
                                 case 0x15:          //87年版資訊可變系統控制器:硬體重置回報
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=8) messageIn[i].success=true;
                                 break;
                                 case 0x19:          //87年版資訊可變系統控制器:軟體重置回報
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=8) messageIn[i].success=true;
                                 break;
                                 case 0x13:          //87年版資訊可變系統控制器:對時誤差過大要求中心對時
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=9) messageIn[i].success=true;
                                 break;
                                 case 0x14:          //87年版資訊可變系統控制器:現場設備日期時間回報
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=13) messageIn[i].success=true;
                                 break;
                                 case 0x2F:          //87年版資訊可變系統控制器:現場設備傳輸週期回報
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=8) messageIn[i].success=true;
                                 break;
                                 case 0x1D:          //87年版資訊可變系統控制器:現場設備硬體狀態回報
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=8) messageIn[i].success=true;
                                 break;
                                 case 0x1E:          //87年版資訊可變系統控制器:電源中斷回報
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=11) messageIn[i].success=true;
                                 break;
                                 case 0x25:          //87年版資訊可變系統控制器:控制器版本日期回報
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=12) messageIn[i].success=true;
                                 break;
                                 case 0x26:          //87年版資訊可變系統控制器:減光控制管理回報
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=12) messageIn[i].success=true;
                                 break;
                                 case 0x2A:          //87年版資訊可變系統控制器:減光開始與結束回報
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=8) messageIn[i].success=true;
                                 break;
                                 case 0x2E:          //87年版資訊可變系統控制器:迴路測試
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=8) messageIn[i].success=true;
                                 break;
                                 case 0x2D:          //87年版資訊可變系統控制器:要求中心下傳控制器資料
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=8) messageIn[i].success=true;
                                 break;
                                 case 0x76:          //87年版資訊可變系統控制器:全文訊息回報
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=38) messageIn[i].success=true;
                                 break;
                                 case 0x86:          //87年版資訊可變系統控制器:全文色彩回報
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=22) messageIn[i].success=true;
                                 break;
                                 case 0xA8:          //87年版資訊可變系統控制器:全文時間回報
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=20) messageIn[i].success=true;
                                 break;
                                 case 0x52:          //87年版資訊可變系統控制器:顯示秒數與間距回報
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=10) messageIn[i].success=true;
                                 break;
                                 case 0x7F:          //87年版資訊可變系統控制器:單點測試結果回報
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=16) messageIn[i].success=true;
                                 break;

                                 //77,87都有
                                 case 0x10:
                                      if (messageIn[i].packetLength==12) {
                                             messageIn[i].protocol = PROTOCOL77;
                                             messageIn[i].success=true;
                                      } else {
                                          messageIn[i].protocol = PROTOCOL87;
                                          if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                      }
                                 break;
                                 case 0x29:          //87年版資訊可變系統控制器:現場設備編號回報
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=8) messageIn[i].success=true;
                                 break;

                                 case 0x11:
                                      if (messageIn[i].packetLength==11) {
                                             messageIn[i].protocol = PROTOCOL77;
                                             messageIn[i].success=true;
                                      } else {
                                          messageIn[i].protocol = PROTOCOL87;
                                          if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                      }
                                 break;
                                 case 0x21:
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                 break;
                                 case 0x22:
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                 break;
                                 case 0x24:
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                 break;
                                 case 0x27:
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                 break;
                                 case 0x28:
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                 break;
                                 case 0x2C:
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                 break;



                                 //中心送至資訊可變系統控制器
                                 case 0x12:
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                 break;
                                 case 0x2B:
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                 break;
                                 case 0x16:
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                 break;
                                 case 0x17:
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                 break;
                                 case 0x18:
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                 break;
                                 case 0x1C:
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                 break;
                                 case 0x1F:
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                 break;

                                 case 0x74:
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                 break;
                                 case 0x77:
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                 break;
                                 case 0x75:
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                 break;
                                 case 0x85:
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                 break;
                                 case 0x79:
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                 break;
                                 case 0x7C:
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                 break;
                                 case 0x7E:
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                 break;
                                 case 0xA5:
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                 break;
                                 case 0xA7:
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                 break;
                                 case 0xA6:
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                 break;
                                 case 0x50:
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                 break;
                                 case 0x51:
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                 break;
                                 case 0x55:
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                 break;
                                 case 0x56:
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                 break;
                                 case 0x59:
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                 break;

                                 default: //主要為了中心送往77年版路口控制器的協定
                                      messageIn[i].protocol = PROTOCOL77;
                                      if (messageIn[i].packetLength>=7) messageIn[i].success=true;
                                 break;
                            } //end switch

                        }

                    } else if (messageIn[i].packetLength>=8 && messageIn[i].packetLength<12) {      //必為77年或87年版本,不可能為92年版
//                         decide77Or87(messageIn[i]);
//================================================================
                            switch(messageIn[i].packet[4]) {      //77或87要由控制碼判斷
                                 //號誌控制器送回中心
                                 case 0xE7:          //77年版號誌控制器:現行步階與剩餘秒數應答
                                      messageIn[i].protocol = PROTOCOL77;
                                      if (messageIn[i].packetLength>=12) messageIn[i].success=true;
                                 break;
                                 case 0xE2:          //77年版號誌控制器:現行操作模式應答
                                 case 0xD2:          //32時段控制器使用
                                      messageIn[i].protocol = PROTOCOL77;
                                      if (messageIn[i].packetLength>=8) messageIn[i].success=true;
                                 break;
                                 case 0xEC:          //77年版號誌控制器:控制器運作狀態應答
                                      messageIn[i].protocol = PROTOCOL77;
                                      if (messageIn[i].packetLength>=8) messageIn[i].success=true;
                                 break;
                                 case 0x0E:          //77年版號誌控制器:for 宏眾
                                      messageIn[i].protocol = PROTOCOL77;
                                      if (messageIn[i].packetLength>=8) messageIn[i].success=true;
                                 break;
                                 case 0x0F:          //77年版號誌控制器:
                                      messageIn[i].protocol = PROTOCOL77;
                                      if (messageIn[i].packetLength>=8) messageIn[i].success=true;
                                 break;
                                 case 0x0C:          //77年版號誌控制器:Plan2傳送成功
                                      messageIn[i].protocol = PROTOCOL77;
                                      if (messageIn[i].packetLength>=8) messageIn[i].success=true;
                                 break;
                                 case 0xEF:          //77年版號誌控制器:特殊日時段型態應答
                                      messageIn[i].protocol = PROTOCOL77;
                                      if (messageIn[i].packetLength>=27) messageIn[i].success=true;
                                 break;
                                 case 0xEB:          //77年版號誌控制器:最近一次箱門開啟及關閉時間
                                      messageIn[i].protocol = PROTOCOL77;
                                      if (messageIn[i].packetLength>=15) messageIn[i].success=true;
                                 break;
                                 case 0xE3:          //77年版號誌控制器:現行時段起始時間應答
                                      messageIn[i].protocol = PROTOCOL77;
                                      if (messageIn[i].packetLength>=12) messageIn[i].success=true;
                                 break;
                                 case 0xE9:          //77年版號誌控制器:時段應答
                                      messageIn[i].protocol = PROTOCOL77;
                                      if (messageIn[i].packetLength>=10) messageIn[i].success=true;
                                 break;
                                 case 0xEA:          //77年版號誌控制器:時段內時制計劃應答
                                      messageIn[i].protocol = PROTOCOL77;
                                      if (messageIn[i].packetLength>=10) messageIn[i].success=true;
                                 break;
                                 case 0xE0:          //77年版號誌控制器:即時時間應答
                                      messageIn[i].protocol = PROTOCOL77;
                                      if (messageIn[i].packetLength>=10) messageIn[i].success=true;
                                 break;
                                 case 0xE1:          //77年版號誌控制器:當日日期應答
                                      messageIn[i].protocol = PROTOCOL77;
                                      if (messageIn[i].packetLength>=11) messageIn[i].success=true;
                                 break;
                                 case 0xE4:          //77年版號誌控制器:第一類訊息應答
                                      messageIn[i].protocol = PROTOCOL77;
                                      if (messageIn[i].packetLength>=14) messageIn[i].success=true;
                                 break;
                                 case 0xE5:          //77年版號誌控制器:第二類訊息應答
                                      messageIn[i].protocol = PROTOCOL77;
                                      if (messageIn[i].packetLength>=14) messageIn[i].success=true;
                                 break;
                                 case 0xE8:          //77年版號誌控制器:週內日時段型態應答
                                      messageIn[i].protocol = PROTOCOL77;
                                      if (messageIn[i].packetLength>=12) messageIn[i].success=true;
                                 break;

                                 //中心送至號誌控制器


                                 //資訊可變系統控制器送回中心
                                 case 0x00:          //87年版資訊可變系統控制器:傳送成功
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=9) messageIn[i].success=true;
                                 break;
                                 case 0x01:          //87年版資訊可變系統控制器:傳送失敗
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=9) messageIn[i].success=true;
                                 break;
                                 case 0x15:          //87年版資訊可變系統控制器:硬體重置回報
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=8) messageIn[i].success=true;
                                 break;
                                 case 0x19:          //87年版資訊可變系統控制器:軟體重置回報
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=8) messageIn[i].success=true;
                                 break;
                                 case 0x13:          //87年版資訊可變系統控制器:對時誤差過大要求中心對時
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=9) messageIn[i].success=true;
                                 break;
                                 case 0x14:          //87年版資訊可變系統控制器:現場設備日期時間回報
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=13) messageIn[i].success=true;
                                 break;
                                 case 0x2F:          //87年版資訊可變系統控制器:現場設備傳輸週期回報
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=8) messageIn[i].success=true;
                                 break;
                                 case 0x1D:          //87年版資訊可變系統控制器:現場設備硬體狀態回報
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=8) messageIn[i].success=true;
                                 break;
                                 case 0x1E:          //87年版資訊可變系統控制器:電源中斷回報
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=11) messageIn[i].success=true;
                                 break;
                                 case 0x25:          //87年版資訊可變系統控制器:控制器版本日期回報
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=12) messageIn[i].success=true;
                                 break;
                                 case 0x26:          //87年版資訊可變系統控制器:減光控制管理回報
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=12) messageIn[i].success=true;
                                 break;
                                 case 0x2A:          //87年版資訊可變系統控制器:減光開始與結束回報
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=8) messageIn[i].success=true;
                                 break;
                                 case 0x2E:          //87年版資訊可變系統控制器:迴路測試
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=8) messageIn[i].success=true;
                                 break;
                                 case 0x2D:          //87年版資訊可變系統控制器:要求中心下傳控制器資料
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=8) messageIn[i].success=true;
                                 break;
                                 case 0x76:          //87年版資訊可變系統控制器:全文訊息回報
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=38) messageIn[i].success=true;
                                 break;
                                 case 0x86:          //87年版資訊可變系統控制器:全文色彩回報
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=22) messageIn[i].success=true;
                                 break;
                                 case 0xA8:          //87年版資訊可變系統控制器:全文時間回報
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=20) messageIn[i].success=true;
                                 break;
                                 case 0x52:          //87年版資訊可變系統控制器:顯示秒數與間距回報
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=10) messageIn[i].success=true;
                                 break;
                                 case 0x7F:          //87年版資訊可變系統控制器:單點測試結果回報
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=16) messageIn[i].success=true;
                                 break;

                                 //77,87都有
                                 case 0x10:
                                      if (messageIn[i].packetLength==12) {
                                             messageIn[i].protocol = PROTOCOL77;
                                             messageIn[i].success=true;
                                      } else {
                                          messageIn[i].protocol = PROTOCOL87;
                                          if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                      }
                                 break;
                                 case 0x29:          //87年版資訊可變系統控制器:現場設備編號回報
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=8) messageIn[i].success=true;
                                 break;

                                 case 0x11:
                                      if (messageIn[i].packetLength==11) {
                                             messageIn[i].protocol = PROTOCOL77;
                                             messageIn[i].success=true;
                                      } else {
                                          messageIn[i].protocol = PROTOCOL87;
                                          if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                      }
                                 break;
                                 case 0x21:
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                 break;
                                 case 0x22:
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                 break;
                                 case 0x24:
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                 break;
                                 case 0x27:
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                 break;
                                 case 0x28:
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                 break;
                                 case 0x2C:
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                 break;



                                 //中心送至資訊可變系統控制器
                                 case 0x12:
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                 break;
                                 case 0x2B:
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                 break;
                                 case 0x16:
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                 break;
                                 case 0x17:
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                 break;
                                 case 0x18:
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                 break;
                                 case 0x1C:
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                 break;
                                 case 0x1F:
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                 break;

                                 case 0x74:
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                 break;
                                 case 0x77:
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                 break;
                                 case 0x75:
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                 break;
                                 case 0x85:
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                 break;
                                 case 0x79:
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                 break;
                                 case 0x7C:
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                 break;
                                 case 0x7E:
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                 break;
                                 case 0xA5:
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                 break;
                                 case 0xA7:
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                 break;
                                 case 0xA6:
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                 break;
                                 case 0x50:
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                 break;
                                 case 0x51:
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                 break;
                                 case 0x55:
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                 break;
                                 case 0x56:
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                 break;
                                 case 0x59:
                                      messageIn[i].protocol = PROTOCOL87;
                                      if (messageIn[i].packetLength>=6) messageIn[i].success=true;
                                 break;

                                 default: //主要為了中心送往77年版路口控制器的協定
                                      messageIn[i].protocol = PROTOCOL77;
                                      if (messageIn[i].packetLength>=7) messageIn[i].success=true;
                                 break;
                            } //end switch

                    }

                } else if (messageIn[i].packet[1]==(const BYTE)0xDD){         //正認知碼
                    if (messageIn[i].packetLength==5) {
                        messageIn[i].protocol = PROTOCOL77;                              //77年版正認知碼
                        messageIn[i].success=true;
                    } else if (messageIn[i].packetLength==8) {
                               messageIn[i].protocol = PROTOCOL92;                       //92年版正認知碼
                               messageIn[i].success=true;
                    }

                } else if (messageIn[i].packet[1]==(const BYTE)0xEE){         //負認知碼
                    if (messageIn[i].packetLength==6) {
                        messageIn[i].protocol = PROTOCOL77;                              //77年版負認知碼
                        messageIn[i].success=true;
                    } else if (messageIn[i].packetLength==9) {
                               messageIn[i].protocol = PROTOCOL92;                       //92年版負認知碼
                               messageIn[i].success=true;
                    }

                }
            }

        }
          }
      }
  } catch(...) {}
}

//---------------------------------------------------------------------------
bool PARSETRAFFIC::CheckSum(int *maxMessageIndex,MESSAGEOK *messageIn)          //核對檢查碼
{
try {
    if (*maxMessageIndex >= 0) {                      //確保
        for (int i=0;i<=*maxMessageIndex;i++) {       //檢查每一個messageIn[i]

/*Debug Code
        if ((messageIn[i].packet[0]==(const BYTE)0xAA) && (messageIn[i].packet[1]==(const BYTE)0xDD)) {
          printf("printfMsgAADD1 cksStatus:%d, success:%d, SEQ:%d\n",
          messageIn[i].cksStatus, messageIn[i].success, messageIn[i].packet[2]);
        }
*/


//    if(messageIn[i].packet[7] == (const BYTE)0x6F) printf("printfMsg  this is 6f!\n");
//    for(int ii = 0; ii < 12; ii++)
//      printf("%x ",messageIn[i].packet[ii]);
//    printf("\n");

             int j=0;                                 //計算messageIn[].packet[j];
             BYTE tempcks=0;                          //暫存要比對的checkSum;
             messageIn[i].cksStatus=false;            //default為false

             if (messageIn[i].packetLength>4) {        //此封包長度至少要有5(AA DD 00 01 cks-77年,AA DD 00 00 01 00 08 cks-92年)

                 if ((messageIn[i].packet[0]==(const BYTE)0xAA) &&
                     (messageIn[i].packet[1]==(const BYTE)0xDD || messageIn[i].packet[1]==(const BYTE)0xEE) &&
                     (messageIn[i].packetLength==5 || messageIn[i].packetLength==6 || messageIn[i].packetLength==8 || messageIn[i].packetLength==9)) {

                       printf("p2\n");

                      for (int k=0;k<messageIn[i].packetLength-1;k++)           //AA DD(EE) HI LO CKS
                           tempcks^=messageIn[i].packet[k];                     //找packetIn的CheckSum

                           if (messageIn[i].packet[messageIn[i].packetLength-1]==tempcks)
                               messageIn[i].cksStatus=true;

                 } else {

                      for (j=4;j<messageIn[i].packetLength;j++) {

//                           if ( (messageIn[i].packet[j-3]!=(const BYTE)0xAA && messageIn[i].packet[j-2]==(const BYTE)0xAA && messageIn[i].packet[j-1]==(const BYTE)0xCC) ||     //第二判斷:aa cc一定相連//避免AA AA CC因為AA是預設捕上的byte
                           if ( (messageIn[i].packet[j-4]!=(const BYTE)0xAA && messageIn[i].packet[j-3]!=(const BYTE)0xBB && messageIn[i].packet[j-2]==(const BYTE)0xAA && messageIn[i].packet[j-1]==(const BYTE)0xCC) ||     //第二判斷:aa cc一定相連//避免AA AA CC因為AA是預設捕上的byte
                                ((messageIn[i].packet[4]==(const BYTE)0x55 || messageIn[i].packet[4]==(const BYTE)0x56) && messageIn[i].packet[j-2]==(const BYTE)0xAA && messageIn[i].packet[j-1]==(const BYTE)0xCC) )     //協定碼為56的有可能傳送AA BB XX XX 56 ...... AA AA CC Cks的協定過來
                              {

//OTDebug                                 printf("p4\n");

                                 for (int k=0 ; k<j ; k++)                      //j=packetInLength-1=cks所在的[Index]
                                      tempcks^=messageIn[i].packet[k];          //找packetIn的CheckSum

                                 if (messageIn[i].packet[j]==tempcks)  {        //第三判斷:aa cc後的cks
                                                                                //一定和此陣列相符..
                                     messageIn[i].packetLength=j+1;             //如果三個判斷都對,此陣列的長度就是j+1(此處必須重新assign)
                                     messageIn[i].cksStatus=true;               //將此陣列設為正確的
//OTDebug                                     printf("cks OK, i:%d\n", i);
                                     break;                                     //如果三個判斷都正確則跳出此迴圈

                                 }  //endIf
                           }

                      }  //end for (j=3 ; j <  messageIn[i].packetLength ; j++)
                 }
             }  //end if (messageIn[i].packetLength > 4)
        }  //end for (int i=0 ; i < maxMessageIndex ; i++)
    }  //end if (maxMessageIndex >= 0)

    return true;


  } catch(...) {}
}
