//---------------------------------------------------------------------------
#include "RS232.h"

#include <stdio.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <string.h>
//---------------------------------------------------------------------------
RS232::RS232(void)
{
try {

    pthread_mutex_init(&mutexRS232,NULL);

    for (int i=0;i<BUFFERSIZE;i++) block[i]=0;                                  //清空block

    rs232fd=0;
    alreadyOpen=false;
    connDevice=0;

    maxMessageIndex=0;
    lastPacketIndex=-1;

    printf("RS232 Class init. set 'alreadyOpen=false'");

  } catch(...) {}
}
//---------------------------------------------------------------------------
RS232::~RS232(void)
{
try {

  } catch(...) {}
}
//---------------------------------------------------------------------------
int RS232::Getfd(void)
{
try {
    int tempfd=0;
    pthread_mutex_lock(&mutexRS232);
    tempfd=rs232fd;
    pthread_mutex_unlock(&mutexRS232);
    return tempfd;
  } catch (...) {}
}
//---------------------------------------------------------------------------
bool RS232::GetPortAlreadyOpen(void)
{
try {
    bool tempOpen=false;
    pthread_mutex_lock(&mutexRS232);
    tempOpen=alreadyOpen;
    pthread_mutex_unlock(&mutexRS232);
    return tempOpen;
  } catch (...) {}
}
//---------------------------------------------------------------------------
int RS232::GetConnDevice(void)
{
try {
    int tempDevice=0;
    pthread_mutex_lock(&mutexRS232);
    tempDevice=connDevice;
    pthread_mutex_unlock(&mutexRS232);
    return tempDevice;
  } catch (...) {}
}
//---------------------------------------------------------------------------
bool RS232::SetConnDevice(int device)
{
try {
    pthread_mutex_lock(&mutexRS232);
    connDevice=device;
    pthread_mutex_unlock(&mutexRS232);
    return true;
  } catch (...) {}
}
//---------------------------------------------------------------------------
int RS232::OpenRs232Port(char *deviceName,int baudrate,bool isEvenParity)
{
try {

    struct termios oldtiocom,newtiocom;                                         //RS232設定

    pthread_mutex_lock(&mutexRS232);

    if ((rs232fd=open(deviceName,O_RDWR|O_NOCTTY|O_NONBLOCK))<0) {
         alreadyOpen=false;
         pthread_mutex_unlock(&mutexRS232);
         printf("RS232 Open init. set 'alreadyOpen=false'");
         return false;
    }


    alreadyOpen=true;

    tcgetattr(rs232fd,&oldtiocom);                                              //儲存舊設定

    //115200,true: 115200E81
    //115200,false:115200N81

    //9600,true: 9600E81
    //9600,false:9600N81

    //4800,true: 4800E81
    //4800,false:4800N81

    //2400,true: 2400E81
    //2400,false:2400N81

    //1200,true: 1200E81
    //1200,false:1200N81

    if (baudrate==115200) {

        if (isEvenParity==true)
            newtiocom.c_cflag = B115200 | CS8 | CLOCAL | CREAD | PARENB;
        else newtiocom.c_cflag = B115200 | CS8 | CLOCAL | CREAD;

    } else if (baudrate==9600) {

        if (isEvenParity==true)
            newtiocom.c_cflag = B9600 | CS8 | CLOCAL | CREAD | PARENB;
        else newtiocom.c_cflag = B9600 | CS8 | CLOCAL | CREAD;

    } else if (baudrate==4800) {

        if (isEvenParity==true)
            newtiocom.c_cflag = B4800 | CS8 | CLOCAL | CREAD | PARENB;
        else newtiocom.c_cflag = B4800 | CS8 | CLOCAL | CREAD;

    } else if (baudrate==2400) {

        if (isEvenParity==true)
            newtiocom.c_cflag = B2400 | CS8 | CLOCAL | CREAD | PARENB;
        else newtiocom.c_cflag = B2400 | CS8 | CLOCAL | CREAD;

    } else if (baudrate==1200) {

        if (isEvenParity==true)
            newtiocom.c_cflag = B1200 | CS8 | CLOCAL | CREAD | PARENB;
        else newtiocom.c_cflag = B1200 | CS8 | CLOCAL | CREAD;

    }

    newtiocom.c_iflag=IGNPAR;
    newtiocom.c_oflag=0;
    newtiocom.c_lflag=0;
    newtiocom.c_cc[VMIN]=1;                                                     //至少一個byte才會收下來
    newtiocom.c_cc[VTIME]=0;

    tcflush(rs232fd,TCIFLUSH);
    tcsetattr(rs232fd,TCSANOW,&newtiocom);                                      //設定新設定

    pthread_mutex_unlock(&mutexRS232);

    return rs232fd;

  } catch(...) {}
}
//---------------------------------------------------------------------------
bool RS232::CloseRs232Port(void)
{
try {
    pthread_mutex_lock(&mutexRS232);
    if (alreadyOpen) close(rs232fd);
    alreadyOpen=false;
    printf("RS232 Close set 'alreadyOpen=false'");
    pthread_mutex_unlock(&mutexRS232);
    return true;
  } catch(...) {}
}
//---------------------------------------------------------------------------
int RS232::Rs232Read(void)
{
try {
    int receiveBlockLength=0;
    pthread_mutex_lock(&mutexRS232);
    if (alreadyOpen)  receiveBlockLength=read(rs232fd,block,sizeof(block));
    pthread_mutex_unlock(&mutexRS232);
    return receiveBlockLength;
  } catch(...) {}
}
//---------------------------------------------------------------------------
bool RS232::Rs232Write(BYTE *writeMessage,int length,char *deviceName)
{
try {
    int writeLength=0;
    pthread_mutex_lock(&mutexRS232);
    if (alreadyOpen)  writeLength=write(rs232fd,writeMessage,length);
    pthread_mutex_unlock(&mutexRS232);
    if (writeLength==length) {

//Bug?        char tempBuff[5],buff[2048]="";
        char tempBuff[256],buff[2048]="";
        char portName[200]="[Write] ";
        char temp[6]=" --";

        strcat(portName,deviceName);
        strcat(portName,temp);

        strcat(buff,portName);

// #ifdef dShowPrintfMsg
        for (int i=0;i<writeLength;i++) {
             sprintf(tempBuff,"%3X",writeMessage[i]);
             strcat(buff,tempBuff);
        }

//      if(deviceName[9] != '3') {  // /dev/ttyS3 to large
        printf("%s\n",buff);
//      }
// #endif

        return true;
    } else  return false ;

  } catch(...) {}
}
//---------------------------------------------------------------------------
