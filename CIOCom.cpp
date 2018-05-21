#include "CIOCom.h"

#include <stdio.h>       //printf()
#include <sys/types.h>   //open()
#include <sys/stat.h>    //open()
#include <fcntl.h>       //open()
#include <unistd.h>      //close() read() write()
#include <stdlib.h>      //exit()
#include <strings.h>

//#define DETECTOR_DEVICE "/dev/ttyN1"
#define DETECTOR_DEVICE "/dev/ttyS5"

using namespace std;

//---------------------------------------------------------------------
CIOCom::CIOCom():fd(0),opened(false)
{
    Open();
}

//---------------------------------------------------------------------
CIOCom::~CIOCom()
{
    Close();
}

//---------------------------------------------------------------------
void CIOCom::Open()
{
  Close();  //close first if ever opened
  fd = open(DETECTOR_DEVICE, O_RDWR|O_NOCTTY);
  if(fd<0) {
    perror("ERROR: error opening DETECTOR_DEVICE!!\n");
//    exit(-1);
  }
  opened=true;

  tcgetattr(fd,&oldtio);
  bzero(&newtio, sizeof(newtio));

  newtio.c_cflag = ( B9600 | CLOCAL | CREAD );  //9600
  newtio.c_cflag &= ~PARENB; // N
  newtio.c_cflag &= ~CSTOPB; // 1
  newtio.c_cflag &= ~CSIZE;
  newtio.c_cflag |= CS8;    // 8

  newtio.c_oflag = 0;

  newtio.c_cc[VTIME]	= DETECTOR_TIMEOUT; // use DETECTOR_TIMEOUT seconds for inter-character timer
  newtio.c_cc[VMIN]	= 0;   // use 0 for getting any char
//  newtio.c_cc[VTIME]	= 0; // block read indefinitely
//  newtio.c_cc[VMIN]	= 1;

  tcsetattr(fd, TCSAFLUSH, &newtio);
}


//---------------------------------------------------------------------
void CIOCom::Close()
{
  if(opened){
    tcsetattr(fd,TCSANOW,&oldtio);
    if(close(fd)<0){
      perror("ERROR: error closing DETECTOR_DEVICE!!\n");
      exit(-1);
    }
    opened=false;
  }
}

//---------------------------------------------------------------------
int CIOCom::Read(unsigned char *block, unsigned long &b_length)
{
  if(opened){
    b_length = read(fd, block, 255);
    block[b_length]=0;

    return b_length;
  }//end if(fd)
  return (-1);
}
//---------------------------------------------------------------------
