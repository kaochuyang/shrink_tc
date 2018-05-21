//---------------------------------------------------------------------------
#include "PARSEV3.h"
#include "SMEM.h"
#include "WRITEJOB.h"

#include <stdio.h>
#include <string.h>
//---------------------------------------------------------------------------
PARSEV3::PARSEV3(void)
{
}
//---------------------------------------------------------------------------
PARSEV3::~PARSEV3(void)
{
}
//---------------------------------------------------------------------------
bool PARSEV3::ParseBlock(int receiveBlockLength,BYTE *block,MESSAGEOK *messageIn,int *lastPacketIndex,int *maxMessageIndex)
{
try {
/*
  int i,j,k;
  unsigned char ucTmpCKS;

  i=0;                         //block[i]
  j=0;                         //messageIn[j]
  k=*lastPacketIndex+1;        //messageIn[j].packet[k]

  for (i=0;i<receiveBlockLength;i++) {

    if(k >= BUFFERSIZE - 10) {
        printf("Parse Error At:%d\n", k);
        k = 0;
        j = 0;
        messageIn[j].success   = false;
        messageIn[j].cksStatus = false;
        messageIn[j].bGetDLE = false;
        messageIn[j].bGetSOH = false;
        messageIn[j].bGetACK = false;
        messageIn[j].bGetNAK = false;
        messageIn[j].uiPacketTEXTLEN = 0;
        messageIn[j].bChkSum1 = false;
        messageIn[j].bChkSum2 = false;
        messageIn[j].bFormatOK = false;
    }

//    printf("i:%d, receiveBlockLength:%d block:%X\n", i, receiveBlockLength, block[i]);

//DLE
    if (block[i]==(const BYTE)0x10) {
      if(messageIn[j].bGetDLE == false && messageIn[j].bGetSOH == false && messageIn[j].bGetACK == false && messageIn[j].bGetNAK == false) {
        messageIn[j].bGetDLE = true;
      }
    }

    messageIn[j].packet[k] = block[i];
    k++;


//SOH
    if (block[i]==(const BYTE)0x01) {
      if(messageIn[j].bGetDLE == true && messageIn[j].bGetSOH == false && messageIn[j].bGetACK == false && messageIn[j].bGetNAK == false) {
        messageIn[j].bGetSOH = true;
      }
    }

//ACK
    if (block[i]==(const BYTE)0x06) {
      if(messageIn[j].bGetDLE == true && messageIn[j].bGetSOH == false && messageIn[j].bGetACK == false && messageIn[j].bGetNAK == false) {
        messageIn[j].bGetACK = true;
      }
    }
//NACK
    if (block[i]==(const BYTE)0x15) {
      if(messageIn[j].bGetDLE == true && messageIn[j].bGetSOH == false && messageIn[j].bGetACK == false && messageIn[j].bGetNAK == false) {
        messageIn[j].bGetNAK = true;
      }
    }

//SEQ do nothing

//HEADER LEN
    if (k == 7 && messageIn[j].bGetSOH == true) {
      printf("go set TEXT LEN:%d\n", messageIn[j].packet[5]*256 + messageIn[j].packet[6]);
      messageIn[j].uiPacketTEXTLEN = messageIn[j].packet[5]*256 + messageIn[j].packet[6];
      printf("go set TEXT LEN2:%d\n", messageIn[j].uiPacketTEXTLEN);
    }
//    printf("i:%d, messageIn[j].uiPacketTEXTLEN:%d \n",i, messageIn[j].uiPacketTEXTLEN);

//HEADER LRC
    if (k == 8 && messageIn[j].bGetSOH == true) {
      ucTmpCKS = 0;
      for(int ii = 3; ii < 7; ii++) {
        ucTmpCKS ^= messageIn[j].packet[ii];
      }
      if(ucTmpCKS == messageIn[j].packet[7]) {
        messageIn[j].bChkSum1 = true;
      } else {
        //Send NAK
        writeJob.WriteNACKNEEB(messageIn[j].packet[2], 0x04);  //LRC Error

        printf("Header LRC error, ucTmpCKS:%X, messageIn[j].packet[7]:%X\n", ucTmpCKS, messageIn[j].packet[7]);
      //CLEAR
        messageIn[j].packetLength=k;
        j++;
        k=0;

        messageIn[j].success   = false;
        messageIn[j].cksStatus = false;
        messageIn[j].bGetDLE = false;
        messageIn[j].bGetSOH = false;
        messageIn[j].bGetACK = false;
        messageIn[j].bGetNAK = false;
        messageIn[j].uiPacketTEXTLEN = 0;
        messageIn[j].bChkSum1 = false;
        messageIn[j].bChkSum2 = false;
        messageIn[j].bFormatOK = false;

      }
    }

    if(k >= 9 && k < 9 + messageIn[j].uiPacketTEXTLEN + 1 && messageIn[j].bGetSOH == true) {
      //move
    }

//SOH TAIL LRC
    if (k == messageIn[j].uiPacketTEXTLEN + 9 && messageIn[j].bGetSOH == true) {
//      printf("OTT cal TAIL LRC\n");
      ucTmpCKS = 0;
      for(int ii = 0; ii < 8 + messageIn[j].uiPacketTEXTLEN; ii++) {
        ucTmpCKS ^= messageIn[j].packet[ii];
      }
      if(ucTmpCKS == messageIn[j].packet[k-1]) {
        messageIn[j].bChkSum2 = true;
      }
      else {
        //Send NAK
        writeJob.WriteNACKNEEB(messageIn[j].packet[2], 0x04);  //LRC Error
      //CLEAR
        printf("Parse Error At:%d, CheckSum!?\n", k);
        messageIn[j].packetLength=k;
        j++;
        k=0;
        messageIn[j].success   = false;
        messageIn[j].cksStatus = false;
        messageIn[j].bGetDLE = false;
        messageIn[j].bGetSOH = false;
        messageIn[j].bGetACK = false;
        messageIn[j].bGetNAK = false;
        messageIn[j].uiPacketTEXTLEN = 0;
        messageIn[j].bChkSum1 = false;
        messageIn[j].bChkSum2 = false;
        messageIn[j].bFormatOK = false;

      }

//CHECK AND CHANGE LINE
      if(messageIn[j].bChkSum1 == true && messageIn[j].bChkSum2 == true) {
        messageIn[j].cksStatus = true;
        messageIn[j].success = true;
      }

      messageIn[j].packetLength=k;
      j++;
      k=0;

      messageIn[j].success   = false;
      messageIn[j].cksStatus = false;
      messageIn[j].bGetDLE = false;
      messageIn[j].bGetSOH = false;
      messageIn[j].bGetACK = false;
      messageIn[j].bGetNAK = false;
      messageIn[j].uiPacketTEXTLEN = 0;
      messageIn[j].bChkSum1 = false;
      messageIn[j].bChkSum2 = false;
      messageIn[j].bFormatOK = false;
    }

    //ACK TAIL LRC
    if (k == 6 && messageIn[j].bGetACK == true) {
      ucTmpCKS = 0;
      for(int ii = 0; ii < 5; ii++) {
        ucTmpCKS ^= messageIn[j].packet[ii];
      }
      if(ucTmpCKS == messageIn[j].packet[k-1]) {
        messageIn[j].cksStatus = true;
        messageIn[j].success = true;
      }
      else {
      //CLEAR
        messageIn[j].success   = false;
        messageIn[j].cksStatus = false;
      }
      messageIn[j].packetLength=k;
      j++;
      k=0;

      messageIn[j].success   = false;
      messageIn[j].cksStatus = false;
      messageIn[j].bGetDLE = false;
      messageIn[j].bGetSOH = false;
      messageIn[j].bGetACK = false;
      messageIn[j].bGetNAK = false;
      messageIn[j].uiPacketTEXTLEN = 0;
      messageIn[j].bChkSum1 = false;
      messageIn[j].bChkSum2 = false;
      messageIn[j].bFormatOK = false;

    }

    //NAK TAIL LRC
    if (k == 8 && messageIn[j].bGetNAK == true) {
      ucTmpCKS = 0;
      for(int ii = 0; ii < 7; ii++) {
        ucTmpCKS ^= messageIn[j].packet[ii];
      }
      if(ucTmpCKS == messageIn[j].packet[k-1]) {
        messageIn[j].cksStatus = true;
        messageIn[j].success = true;
      }
      else {
      //CLEAR
        messageIn[j].success   = false;
        messageIn[j].cksStatus = false;
      }
      messageIn[j].packetLength=k;
      j++;
      k=0;

      messageIn[j].success   = false;
      messageIn[j].cksStatus = false;
      messageIn[j].bGetDLE = false;
      messageIn[j].bGetSOH = false;
      messageIn[j].bGetACK = false;
      messageIn[j].bGetNAK = false;
      messageIn[j].uiPacketTEXTLEN = 0;
      messageIn[j].bChkSum1 = false;
      messageIn[j].bChkSum2 = false;
      messageIn[j].bFormatOK = false;
    }


//DROP ERROR DLE
    if( k == 1 ) {
      if(messageIn[j].packet[0] == 0x10) {} //Do nothing
      else {
        printf("Parse Error At:%d\n, first byte error", k);
        for (int b=0;b<BUFFERSIZE;b++) {        //²MªÅmessageIn[].packet
          messageIn[j].packet[b]=0;
        }
        k=0;
        messageIn[j].UDPorRS232=0;
        messageIn[j].ReadorWrite=0;
        messageIn[j].success   = false;
        messageIn[j].cksStatus = false;
        messageIn[j].bGetDLE = false;
        messageIn[j].bGetSOH = false;
        messageIn[j].bGetACK = false;
        messageIn[j].bGetNAK = false;
        messageIn[j].uiPacketTEXTLEN = 0;
        messageIn[j].bChkSum1 = false;
        messageIn[j].bChkSum2 = false;
        messageIn[j].bFormatOK = false;
      }
    }
    if( k == 2 ) {
      if( messageIn[j].packet[1] == 0x01 || messageIn[j].packet[1] == 0x06 || messageIn[j].packet[1] == 0x15) {} //Do nothing
      else {
        if(messageIn[j].packet[1] == 0x10) {                                    // when case in 10 10 01 balabala
          messageIn[j].packet[1] = 0;
          k--;
        }
        else {
          printf("Parse Error At:%d\n, second byte error", k);

          bzero(messageIn[j].packet, BUFFERSIZE);
          k=0;
          messageIn[j].UDPorRS232=0;
          messageIn[j].ReadorWrite=0;
          messageIn[j].success   = false;
          messageIn[j].cksStatus = false;
          messageIn[j].bGetDLE = false;
          messageIn[j].bGetSOH = false;
          messageIn[j].bGetACK = false;
          messageIn[j].bGetNAK = false;
          messageIn[j].uiPacketTEXTLEN = 0;
          messageIn[j].bChkSum1 = false;
          messageIn[j].bChkSum2 = false;
          messageIn[j].bFormatOK = false;
        }

      }
    }

  }

  *maxMessageIndex=j;
  messageIn[j].packetLength=k;
  return true;
*/
  } catch(...) {}
}


//---------------------------------------------------------------------------
bool PARSEV3::EchoToGUI(int *maxMessageIndex, MESSAGEOK *messageIn, char *deviceName)
{
try {
    if (*maxMessageIndex>=0) {
        for (int i=0;i<=*maxMessageIndex;i++) {
             if (messageIn[i].cksStatus==true) {
                 if (messageIn[i].success==true) {
                     char tempBuff[5],buff[2048]="";
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
bool PARSEV3::MoveLastData(int *maxMessageIndex,int *lastPacketIndex,MESSAGEOK *messageIn)
{
try {
/*
    if (messageIn[*maxMessageIndex].cksStatus == false) {
        for (int j=0;j<messageIn[*maxMessageIndex].packetLength;j++) {
             messageIn[0].packet[j]=messageIn[*maxMessageIndex].packet[j];
        }
        messageIn[0].packetLength=messageIn[*maxMessageIndex].packetLength;
        messageIn[0].bGetDLE = messageIn[*maxMessageIndex].bGetDLE;
        messageIn[0].bGetSOH = messageIn[*maxMessageIndex].bGetSOH;
        messageIn[0].bGetACK = messageIn[*maxMessageIndex].bGetACK;
        messageIn[0].bGetNAK = messageIn[*maxMessageIndex].bGetNAK;
        messageIn[0].uiPacketTEXTLEN = messageIn[*maxMessageIndex].uiPacketTEXTLEN;
        messageIn[0].bChkSum1 = messageIn[*maxMessageIndex].bChkSum1;
        messageIn[0].bChkSum2 = messageIn[*maxMessageIndex].bChkSum2;
        messageIn[0].success = messageIn[*maxMessageIndex].success;
        messageIn[0].cksStatus = messageIn[*maxMessageIndex].cksStatus;
        *lastPacketIndex=messageIn[*maxMessageIndex].packetLength-1;
        *maxMessageIndex = 0;

        for (int a=1;a<MSGINSIZE;a++) {
             messageIn[a].cksStatus=false;
             messageIn[a].success=false;
             messageIn[a].UDPorRS232=0;
             messageIn[a].ReadorWrite=0;

             messageIn[a].bGetDLE = false;
             messageIn[a].bGetSOH = false;
             messageIn[a].bGetACK = false;
             messageIn[a].bGetNAK = false;
             messageIn[a].uiPacketTEXTLEN = 0;
             messageIn[a].bChkSum1 = false;
             messageIn[a].bChkSum2 = false;
             messageIn[a].bFormatOK = false;

             bzero( messageIn[a].packet, BUFFERSIZE);
        }
    } else {
      for (int a=0;a<MSGINSIZE;a++) {
             messageIn[a].cksStatus=false;
             messageIn[a].success=false;
             messageIn[a].UDPorRS232=0;
             messageIn[a].ReadorWrite=0;
             messageIn[a].bGetDLE = false;
             messageIn[a].bGetSOH = false;
             messageIn[a].bGetACK = false;
             messageIn[a].bGetNAK = false;
             messageIn[a].uiPacketTEXTLEN = 0;
             messageIn[a].bChkSum1 = false;
             messageIn[a].bChkSum2 = false;
             messageIn[a].bFormatOK = false;

             bzero( messageIn[a].packet, BUFFERSIZE);
      }

              *lastPacketIndex=-1;
              *maxMessageIndex=0;
    }


    return true;
*/

  } catch(...) {}
}

//---------------------------------------------------------------------------
bool PARSEV3::EchoOriginalPacket(BYTE *packet,int packetLength)
{
try {
    char tempBuff[5],buff[2048]="";
    char portName[200]="[NEEB VER ORIGINAL] ";
    char temp[6]=" --";

    strcat(portName,temp);
    strcat(buff,portName);

    for (int j=0;j<packetLength;j++) {
         sprintf(tempBuff,"%3X",packet[j]);
         strcat(buff,tempBuff);
    }

    printf("%s%s%s\n",ColorWhiteBlue,buff,ColorNormal);
  } catch (...) {}
}


//---------------------------------------------------------------------------
BYTE PARSEV3::GetEquipCode(BYTE controlCode)
{
try {
  } catch (...) {}
}

