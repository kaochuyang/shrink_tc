#include "TCP.h"
#include <stdio.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

TCP::~TCP()
{
    //dtor
}
TCP::TCP(void)
{
try {

    pthread_mutex_init(&mutexTCp,NULL);

    for (int i=0;i<BUFFERSIZE;i++) block[i]=0;                                  //²MªÅblock

    TCpfd;
    alreadyOpen=false;
    connDevice=0;

    maxMessageIndex=0;
    lastPacketIndex=-1;

  } catch (...) {}
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
int TCP::Getfd(void)
{
try {
    int tempfd=0;
    pthread_mutex_lock(&mutexTCp);
    tempfd=TCpfd;
    pthread_mutex_unlock(&mutexTCp);
    return tempfd;
  } catch (...) {}
}
//---------------------------------------------------------------------------
bool TCP::GetPortAlreadyOpen(void)
{
try {
    bool tempOpen=false;
    pthread_mutex_lock(&mutexTCp);
    tempOpen=alreadyOpen;
    pthread_mutex_unlock(&mutexTCp);
    return tempOpen;
  } catch (...) {}
}
//---------------------------------------------------------------------------
int TCP::GetConnDevice(void)
{
try {
    int tempDevice=0;
    pthread_mutex_lock(&mutexTCp);
    tempDevice=connDevice;
    pthread_mutex_unlock(&mutexTCp);
    return tempDevice;
  } catch (...) {}
}
//---------------------------------------------------------------------------
bool TCP::SetConnDevice(int device)
{
try {
    pthread_mutex_lock(&mutexTCp);
    connDevice=device;
    pthread_mutex_unlock(&mutexTCp);
    return true;
  } catch (...) {}
}
//---------------------------------------------------------------------------
int TCP::OpenTCpSocket(char *local_ip,int listen_port)
{
try {

    int so_broadcast,so_sndbuf=131070,so_rcvbuf=131070;        //³Ì¤jÀ³¸Ó¥u¦³131070 (default:65535) ³]¶W¹L¤]À³¥u¦³131070

    pthread_mutex_lock(&mutexTCp);

    memset(&listen_addr,0,sizeof(listen_addr));                //listen port(¥»¥x¹q¸£ºÊÅ¥ªºport)
    listen_addr.sin_family=AF_INET;
    listen_addr.sin_port=htons(listen_port);                   //³]©wlisten_port
    listen_addr.sin_addr.s_addr=inet_addr(local_ip);           //³]©wlocal_ip


    memset(&send_addr,0,sizeof(send_addr));                    //­n°e¨ìºô¸ô¤Wªº»·ºÝ¹q¸£ªºport
    send_addr.sin_family=AF_INET;




    if ((TCpfd=socket(AF_INET,SOCK_STREAM,0))==-1) {            //¶}°ðSOCK_DGRAM(TCP)
         alreadyOpen=false;
          printf(" create tcp socket error: %s (errno :%d)\n",strerror(errno),errno);
         pthread_mutex_unlock(&mutexTCp);
         return false;
    }

  /*  //enable broadcasting
    if (setsockopt(TCpfd,SOL_SOCKET,SO_BROADCAST,&so_broadcast,sizeof(so_broadcast))==-1) {
        alreadyOpen=false;
        pthread_mutex_unlock(&mutexTCp);
        return false;
    }
*/
    //set max sendbuffer
    if (setsockopt(TCpfd,SOL_SOCKET,SO_REUSEADDR,&so_sndbuf,sizeof(so_sndbuf))==-1) {
        alreadyOpen=false;
        pthread_mutex_unlock(&mutexTCp);
        return false;
    }
/*
    //set max receivebuffer
    if (setsockopt(TCpfd,SOL_SOCKET,SO_RCVBUF,&so_rcvbuf,sizeof(so_rcvbuf))==-1) {
        alreadyOpen=false;
        pthread_mutex_unlock(&mutexTCp);
        return false;
    }*/

    //bind socket³s±µºô¸ô¥d
    if (bind(TCpfd,(struct sockaddr *)&listen_addr,sizeof(listen_addr))==-1) {
        alreadyOpen=false;
          printf(" bind tcp socket error: %s (errno :%d)\n",strerror(errno),errno);
        pthread_mutex_unlock(&mutexTCp);

        return false;
    }


if(listen(TCpfd,1)==-1)
{
      printf(" listen tcp socket error: %s (errno :%d)\n",strerror(errno),errno);
            return false;
}

    alreadyOpen=true;

    pthread_mutex_unlock(&mutexTCp);

    return TCpfd;

  } catch(...) {}
}
//---------------------------------------------------------------------------
bool TCP::CloseTCpSocket(void)
{
try {
    pthread_mutex_lock(&mutexTCp);
    if (alreadyOpen) close(TCpfd);
    alreadyOpen=false;
    pthread_mutex_unlock(&mutexTCp);
    return true;
  } catch(...) {}
}
//---------------------------------------------------------------------------
int TCP::TCpRead(void)
{
try {

    struct sockaddr_in pc_addr;
    socklen_t len = sizeof(pc_addr);;

    int recvPacketLength=0;
    pthread_mutex_lock(&mutexTCp);
    if (alreadyOpen) recvPacketLength=recvfrom(TCpfd,block,sizeof(block),0,(struct sockaddr *)&pc_addr,&len);
    pthread_mutex_unlock(&mutexTCp);
    return recvPacketLength;

  } catch(...) {}
}
//---------------------------------------------------------------------------
bool TCP::TCpSend(BYTE *writeMessage,int length,char *deviceName)
{
try {

    int sendLength=0;
    pthread_mutex_lock(&mutexTCp);
    //printf("%s: open? :%d, len:%d\n", deviceName, alreadyOpen, length);
    if (alreadyOpen) sendLength=sendto(TCpfd,writeMessage,length,0,(struct sockaddr *)&send_addr,sizeof(send_addr));
    pthread_mutex_unlock(&mutexTCp);

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
//bool TCP::TCpSend(BYTE *writeMessage,int length,char *deviceName, bool isNeedtoDisplay)
//@version 960416 ­«¸ü­ì¦³¨ç¦¡¼W¥[¬O§_Åã¥Ü°e¥X°T®§¥\¯à
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool TCP::TCpSend(BYTE *writeMessage,int length,char *deviceName, bool isNeedtoDisplay)
{
try {

    int sendLength=0;
    //printf("%s: 2open? :%d, len:%d\n", deviceName, alreadyOpen, length);
    pthread_mutex_lock(&mutexTCp);
    if (alreadyOpen) sendLength=sendto(TCpfd,writeMessage,length,0,(struct sockaddr *)&send_addr,sizeof(send_addr));
    pthread_mutex_unlock(&mutexTCp);

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
    printf("TCP::TCpSend() send size Error!!!\n");
    return false;
  } catch(...) {
	  printf("TCP::TCpSend() catch Error!!!\n");
	  return false;
  }
}
//---------------------------------------------------------------------------
