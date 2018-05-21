//---------------------------------------------------------------------------
#include "PARSESS.h"

#include <stdio.h>
#include <string.h>
//---------------------------------------------------------------------------
PARSESS::PARSESS(void)
{
}
//---------------------------------------------------------------------------
PARSESS::~PARSESS(void)
{
}
//---------------------------------------------------------------------------
bool PARSESS::ParseBlock(int receiveBlockLength,BYTE *block,MESSAGEOK *messageIn,int *lastPacketIndex,int *maxMessageIndex)
{                  //解析封包
try {
    int i,j,k;

    i=0;                         //block[i]
    j=0;                         //messageIn[j]
    k=*lastPacketIndex+1;        //messageIn[j].packet[k]

    for (i=0;i<receiveBlockLength;i++) {              //掃描block將資料解出來
      if (
          ((block[i] == 'X') && ((block[i+1] == 'D') || (block[i+1] == '1') || (block[i+1] == 'G')   || (block[i+1] == 'A'))) ||
          ((block[i] == 'S') && ((block[i+1] == 'K') || (block[i+1] == 'J') || (block[i+1] == '4'))) ||
          ((block[i] == 'S') && (block[i+1] == '0')) ||
          ((block[i] == 'Z') && (block[i+1] == '0'))
         ) {

               messageIn[j].packetLength=k;            //上一筆messageIn[j]的長度就此定下來
               j++;                                    //移到下一筆
               k=0;                                    //歸0,一定放在messageIn[j].packet[0]
               messageIn[j].packet[k]=block[i];        //放入AA
               k++;                                    //將messageIn[j].packet[0]變成messageIn[j].packet[1]

         }  else  {

              if (k < BUFFERSIZE) {                    //block的最大值
                  messageIn[j].packet[k]=block[i];     //BB ,DD ,EE之後由此放入

                    if (k>1) {
                      if (
                          ((messageIn[j].packet[k]== 'D' || messageIn[j].packet[k]== '1' || messageIn[j].packet[k]== 'G' || messageIn[j].packet[k]== 'A') &&
                          (messageIn[j].packet[k-1]== 'X')) ||
                          ((messageIn[j].packet[k]== 'K' || messageIn[j].packet[k]== 'J' || messageIn[j].packet[k]== '4') &&
                          (messageIn[j].packet[k-1]== 'S')) ||
                          ((messageIn[j].packet[k]== '0') && (messageIn[j].packet[k-1]== 'Z'))
                         ) {

                          j++;

                          if(messageIn[j].packet[k-1]== 'X')
                          {
                            messageIn[j].packet[0]= 'X';                          //新的一行messageIn

                            if (messageIn[j-1].packet[k]== 'D')    messageIn[j].packet[1]= 'D';
                            else if (messageIn[j-1].packet[k]== '1')    messageIn[j].packet[1]= '1';
                            else if (messageIn[j-1].packet[k]== 'G')    messageIn[j].packet[1]= 'G';
                            else if (messageIn[j-1].packet[k]== 'A')    messageIn[j].packet[1]= 'A';
                          }
                          else if(messageIn[j].packet[k-1]== 'S')
                          {
                            messageIn[j].packet[0]= 'S';

                            if (messageIn[j-1].packet[k]== 'K')    messageIn[j].packet[1]= 'K';
                            else if (messageIn[j-1].packet[k]== 'J')    messageIn[j].packet[1]= 'J';
                            else if (messageIn[j-1].packet[k]== '4')    messageIn[j].packet[1]= '4';
                          }
                          else if(messageIn[j].packet[k-1] == 'Z')
                          {
                            messageIn[j].packet[0]= 'Z';
                            if (messageIn[j-1].packet[k]== '0')    messageIn[j].packet[1]= '0';
                          }

                          messageIn[j-1].packet[k]=0;
                          messageIn[j-1].packet[k-1]=0;
                          messageIn[j-1].packetLength=k-1;  //因為CHECKSUM的位置應為messageIn[j].packet[k-2],所以messageIn[j]的長度應為k-2+1

                          k=1;
                      }

                  }   //end if (k > 1)

                  k++;   //messageIn[j].packet[k]往後移一個位置

              } else {     //此messageIn滿了,那就是有問題了
                           //全部清空重新來
                      for (int a=0;a<MSGINSIZE;a++)           //清空messageIn
                      for (int b=0;b<BUFFERSIZE;b++) {        //清空messageIn[].packet
                           messageIn[a].packet[b]=0;
                           messageIn[a].cksStatus = false;
                           messageIn[a].success = false;
                      }

                      *lastPacketIndex=-1;
                      *maxMessageIndex=0;

              }   //end if (k < BUFFERSIZE)

         }     //end if
    }    //end for

    *maxMessageIndex=j;            //最後放到messageIn有j+1的
    messageIn[j].packetLength=k;   //最後一個messageIn[j]放到k

    return true;
  } catch(...) {}
}

//---------------------------------------------------------------------------
bool PARSESS::vMultiDropProcess(int *maxMessageIndex,MESSAGEOK *messageIn)
{
try {
    if (*maxMessageIndex >= 0) {
        char cMultiDrop[8];
//      bzero(cMultiDrop, 8);
        int PL;
        for (int i=0;i<=*maxMessageIndex;i++) {
             int j=0;
             int k = 0;
             BYTE tempcks=0;
             PL = messageIn[i].packetLength;
             if (messageIn[i].packet[j] == 'Z' && messageIn[i].packet[j+1] == '0')
             {
                for(k=0; k <6; k++)
                    cMultiDrop[k] = messageIn[i].packet[k];
                for(k=6; k < PL; k++);
                    messageIn[i].packet[k-6] = messageIn[i].packet[k];
                messageIn[i].packet[k] = 0x00;
                messageIn[i].packetLength = PL - 6;

                //should sync to smem
             }
        }
    }
    return true;
  } catch(...) {}
}
//---------------------------------------------------------------------------
bool PARSESS::CheckSum(int *maxMessageIndex,MESSAGEOK *messageIn)          //核對檢查碼
{
try {
    if (*maxMessageIndex >= 0) {                      //確保
        int PL;
        for (int i=0;i<=*maxMessageIndex;i++) {       //檢查每一個messageIn[i]
             int j=0;                                 //計算messageIn[].packet[j];
             BYTE tempcks=0;                          //暫存要比對的checkSum;
             PL = messageIn[i].packetLength;
             messageIn[i].cksStatus=false;            //default為false

/* process XD */
            if (PL>8 && messageIn[i].packet[PL-1] == 0x0D && messageIn[i].packet[PL-2] == 0x0D)
            {        //此封包長度至少要有9 X10000~\r\r
                if(messageIn[i].packet[0] == 'X' && messageIn[i].packet[1] == 'D') {  //只有XD有checksum
                   if( (PL-13) / 29 == 0) {
                      printf("NoChkSum\n");
                      messageIn[i].cksStatus = true;
                   }
                   else {
                      messageIn[i].cksStatus = doCheckSumXD(i,messageIn);
                   }
                }
/* process XD END*/
                else
                     messageIn[i].cksStatus= true;
            }

            messageIn[i].protocol = PROTOCOLSS;
//            messageIn[i].packetLcn = smem.GetAddress();
            messageIn[i].success=true;
            messageIn[i].InnerOrOutWard = cInner;

        }
    }

    return true;

  } catch(...) {}
}
//---------------------------------------------------------------------------
bool PARSESS::doCheckSumXD(int index, MESSAGEOK *messageIN)
{
try{
    int i, j, tmpCheckSum = 0;
    int tcmp = 0;
    char tmpCheckSumASCII[4];
    char *ptmpCheckSumASCII = tmpCheckSumASCII;
    int checkSumBase = messageIN[index].packetLength - 7;

    long hi = 0;

    for(i = 2; i < checkSumBase; i++) //why -7? checksum + Terminator
        tmpCheckSum = tmpCheckSum + messageIN[index].packet[i];

    sprintf(tmpCheckSumASCII, "%4X", tmpCheckSum);
    for(i = 0; i < 4; i++) {
        if(tmpCheckSumASCII[i] == 0x20)
           tmpCheckSumASCII[i] = '0';
    }

    for(i = 0; i < 4 ;i++) {
        if(tmpCheckSumASCII[i] == messageIN[index].packet[checkSumBase+i])
           tcmp++;
    }

    hi = oHexToInt.htoi(tmpCheckSumASCII, 4);

    if(tcmp == 4)  return true;
    else  return false;

  } catch(...){}
}

//---------------------------------------------------------------------------
bool PARSESS::EchoToGUI(int *maxMessageIndex,MESSAGEOK *messageIn,char *deviceName)      //作此協定相對的事情
{
try {
    if (*maxMessageIndex>=0) {                                //確保;
        for (int i=0;i<=*maxMessageIndex;i++) {
             if (messageIn[i].cksStatus==true) {              //通過CheckSum檢測

                 char tempBuff[256],buff[2048]="";
                 char portName[200]="[RECEIVE] ";
                 char temp[6]=" --";

                 strcat(portName,deviceName);
                 strcat(portName,temp);

                 strcat(buff,portName);

                 for (int j=0;j<messageIn[i].packetLength;j++) {
                   if(messageIn[i].packet[j] == 0x0D)
                     sprintf(tempBuff,"\\r");
                   else sprintf(tempBuff,"%2C",messageIn[i].packet[j]);
                     strcat(buff,tempBuff);
                 }

                 printf("%s%s%s\n",ColorGreen, buff, ColorNormal);

             }
        }  //end for (int i=0 ; i < maxMessageIndex ; i++)
    }  //end if (maxMessageIndex >= 0)
  } catch(...) {}
}
//---------------------------------------------------------------------------
bool PARSESS::MoveLastData(int *maxMessageIndex,int *lastPacketIndex,MESSAGEOK *messageIn)      //搬移最後未作完的封包
{
try {
    int iLPI, iMMI;
    iMMI = *maxMessageIndex;
    iLPI = *lastPacketIndex;

    if (messageIn[iMMI].cksStatus == false) {                       //首先最後一行必須要是不完整的再搬
        for (int j=0;j<messageIn[iMMI].packetLength;j++)
             messageIn[0].packet[j]=messageIn[iMMI].packet[j];
             //將最後一行放到第零行的動作(即使同一行也沒關係)
        messageIn[0].packetLength=messageIn[iMMI].packetLength;
             //長度也放過去
        *lastPacketIndex=messageIn[iMMI].packetLength-1;
             //將lastPacketIndex變成從最後一個放到第一個messageIn[]放進去的長度的Index
             //當有資料再從ComPort進來時就會再重作一次,參考ComPort的OnTriggerAvail事件
        for (int a=1;a<MSGINSIZE;a++)          //清空第一行以外的messageIn
        for (int b=0;b<BUFFERSIZE;b++) {
             messageIn[a].packet[b]=0;
             messageIn[a].cksStatus = false;
             messageIn[a].success = false;
        }

    } else
      {
        int length = messageIn[iMMI].packetLength;

      if (length > 3) {

        if (1)  {

          messageIn[0].packet[0]='X';

          for (int a=1;a<MSGINSIZE;a++) {         //清空messageIn
               messageIn[a].cksStatus = false;
               messageIn[a].success = false;
          for (int b=0;b<BUFFERSIZE;b++) {
               messageIn[a].packet[b]=0;
          }
          }

          *lastPacketIndex=0;
          *maxMessageIndex=0;

      } else {  //如果最後一個messageIn是完整訊息而且最後沒有殘留X
          //就回歸最原始狀態

          for (int a=0;a<MSGINSIZE;a++) {         //清空messageIn
               messageIn[a].cksStatus = false;
               messageIn[a].success = false;
          for (int b=0;b<BUFFERSIZE;b++) {
               messageIn[a].packet[b]=0;
          }
          }

          *lastPacketIndex=-1;
          *maxMessageIndex=0;
    }

  } else {
          for (int a=0;a<MSGINSIZE;a++) {         //清空messageIn
               messageIn[a].cksStatus = false;
               messageIn[a].success = false;
          for (int b=0;b<BUFFERSIZE;b++) {
               messageIn[a].packet[b]=0;
          }
          }

          *lastPacketIndex=-1;
          *maxMessageIndex=0;
  	}
}
  } catch(...) {}
}
//---------------------------------------------------------------------------
