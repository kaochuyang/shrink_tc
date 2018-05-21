//---------------------------------------------------------------------------
#include "UDP.h"

#include <stdio.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#define dShowPrintfMsg

//---------------------------------------------------------------------------
UDP::UDP(void)
{
try {

    pthread_mutex_init(&mutexUdp,NULL);

    for (int i=0;i<BUFFERSIZE;i++) block[i]=0;                                  //清空block

    udpfd;
    alreadyOpen=false;
    connDevice=0;

    maxMessageIndex=0;
    lastPacketIndex=-1;

  } catch (...) {}
}
//---------------------------------------------------------------------------
UDP::~UDP(void)
{
try {

  } catch (...) {}
}
//---------------------------------------------------------------------------
int UDP::Getfd(void)
{
try {
    int tempfd=0;
    pthread_mutex_lock(&mutexUdp);
    tempfd=udpfd;
    pthread_mutex_unlock(&mutexUdp);
    return tempfd;
  } catch (...) {}
}
//---------------------------------------------------------------------------
bool UDP::GetPortAlreadyOpen(void)
{
try {
    bool tempOpen=false;
    pthread_mutex_lock(&mutexUdp);
    tempOpen=alreadyOpen;
    pthread_mutex_unlock(&mutexUdp);
    return tempOpen;
  } catch (...) {}
}
//---------------------------------------------------------------------------
int UDP::GetConnDevice(void)
{
try {
    int tempDevice=0;
    pthread_mutex_lock(&mutexUdp);
    tempDevice=connDevice;
    pthread_mutex_unlock(&mutexUdp);
    return tempDevice;
  } catch (...) {}
}
//---------------------------------------------------------------------------
bool UDP::SetConnDevice(int device)
{
try {
    pthread_mutex_lock(&mutexUdp);
    connDevice=device;
    pthread_mutex_unlock(&mutexUdp);
    return true;
  } catch (...) {}
}
//---------------------------------------------------------------------------
int UDP::OpenUdpSocket(char *local_ip,int listen_port,char *send_ip,int send_port)
{
try {

    int so_broadcast,so_sndbuf=131070,so_rcvbuf=131070;        //最大應該只有131070 (default:65535) 設超過也應只有131070

    pthread_mutex_lock(&mutexUdp);

    memset(&listen_addr,0,sizeof(listen_addr));                //listen port(本台電腦監聽的port)
    listen_addr.sin_family=AF_INET;
    listen_addr.sin_port=htons(listen_port);                   //設定listen_port
    listen_addr.sin_addr.s_addr=inet_addr(local_ip);           //設定local_ip


    memset(&send_addr,0,sizeof(send_addr));                    //要送到網路上的遠端電腦的port
    send_addr.sin_family=AF_INET;
    send_addr.sin_port=htons(send_port);                       //設定send_port
    send_addr.sin_addr.s_addr=inet_addr(send_ip);              //設定send_ip


    if ((udpfd=socket(AF_INET,SOCK_DGRAM,0))==-1) {            //開埠SOCK_DGRAM(UDP)
         alreadyOpen=false;
         pthread_mutex_unlock(&mutexUdp);
         return false;
    }

    //enable broadcasting
    if (setsockopt(udpfd,SOL_SOCKET,SO_BROADCAST,&so_broadcast,sizeof(so_broadcast))==-1) {
        alreadyOpen=false;
        pthread_mutex_unlock(&mutexUdp);
        return false;
    }

    //set max sendbuffer
    if (setsockopt(udpfd,SOL_SOCKET,SO_SNDBUF,&so_sndbuf,sizeof(so_sndbuf))==-1) {
        alreadyOpen=false;
        pthread_mutex_unlock(&mutexUdp);
        return false;
    }

    //set max receivebuffer
    if (setsockopt(udpfd,SOL_SOCKET,SO_RCVBUF,&so_rcvbuf,sizeof(so_rcvbuf))==-1) {
        alreadyOpen=false;
        pthread_mutex_unlock(&mutexUdp);
        return false;
    }

    //bind socket連接網路卡
    if (bind(udpfd,(struct sockaddr *)&listen_addr,sizeof(listen_addr))==-1) {
        alreadyOpen=false;
        pthread_mutex_unlock(&mutexUdp);
        return false;
    }


    alreadyOpen=true;

    pthread_mutex_unlock(&mutexUdp);

    return udpfd;

  } catch(...) {}
}
//---------------------------------------------------------------------------
bool UDP::CloseUdpSocket(void)
{
try {
    pthread_mutex_lock(&mutexUdp);
    if (alreadyOpen) close(udpfd);
    alreadyOpen=false;
    pthread_mutex_unlock(&mutexUdp);
    return true;
  } catch(...) {}
}
//---------------------------------------------------------------------------
int UDP::UdpRead(void)
{
try {

    struct sockaddr_in pc_addr;
    socklen_t len = sizeof(pc_addr);;

    int recvPacketLength=0;
    pthread_mutex_lock(&mutexUdp);
    if (alreadyOpen) recvPacketLength=recvfrom(udpfd,block,sizeof(block),0,(struct sockaddr *)&pc_addr,&len);
    pthread_mutex_unlock(&mutexUdp);
    return recvPacketLength;

  } catch(...) {}
}
//---------------------------------------------------------------------------
bool UDP::UdpSend(BYTE *writeMessage,int length,char *deviceName)
{
try {

    int sendLength=0;
    pthread_mutex_lock(&mutexUdp);
    //printf("%s: open? :%d, len:%d\n", deviceName, alreadyOpen, length);
    if (alreadyOpen) sendLength=sendto(udpfd,writeMessage,length,0,(struct sockaddr *)&send_addr,sizeof(send_addr));
    pthread_mutex_unlock(&mutexUdp);

    if (sendLength==-1) { printf("send failure!!\n"); return false; }
    else if (sendLength==length){

        char tempBuff[256],buff[2048]="";
        char portName[200]="[Write] ";
        char temp[6]=" --";

        strcat(portName,deviceName);
        strcat(portName,temp);

        strcat(buff,portName);

#ifdef dShowPrintfMsg
        for (int i=0;i<sendLength;i++) {
             sprintf(tempBuff,"%3X",writeMessage[i]);
             strcat(buff,tempBuff);
        }
        printf("%s\n",buff);
#endif

        return true;
    }

  } catch(...) {}
}
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//bool UDP::UdpSend(BYTE *writeMessage,int length,char *deviceName, bool isNeedtoDisplay)
//@version 960416 重載原有函式增加是否顯示送出訊息功能
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool UDP::UdpSend(BYTE *writeMessage,int length,char *deviceName, bool isNeedtoDisplay)
{
try {

    int sendLength=0;
    //printf("%s: 2open? :%d, len:%d\n", deviceName, alreadyOpen, length);
    pthread_mutex_lock(&mutexUdp);
    if (alreadyOpen) sendLength=sendto(udpfd,writeMessage,length,0,(struct sockaddr *)&send_addr,sizeof(send_addr));
    pthread_mutex_unlock(&mutexUdp);

    if (sendLength==-1)  return false;
    else if (sendLength==length){
    	if(isNeedtoDisplay){
        char tempBuff[256],buff[2048]="";
        char portName[200]="[Write] ";
        char temp[6]=" --";

        strcat(portName,deviceName);
        strcat(portName,temp);

        strcat(buff,portName);

        for (int i=0;i<sendLength;i++) {
             sprintf(tempBuff,"%3X",writeMessage[i]);
             strcat(buff,tempBuff);
        }//for
        printf("%s\n",buff);
    	}//if(isNeedtoDisplay)
        return true;
    }//if
    printf("UDP::UdpSend() send size Error!!!\n");
    return false;
  } catch(...) {
	  printf("UDP::UdpSend() catch Error!!!\n");
	  return false;
  }
}
//---------------------------------------------------------------------------
