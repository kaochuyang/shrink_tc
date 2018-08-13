#ifndef TEMPERATUR_HUMIDITY_SENSOR_H
#define TEMPERATUR_HUMIDITY_SENSOR_H
#include "RS232.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <iostream>
#include <time.h>
#include <signal.h>
#include <pthread.h>
#include "PDEVICEKEYPAD.h"
#include "CDataToMessageOK.h"
#include "WRITEJOB.h"
#include "var.h"

class temperatur_humidity_sensor
{
    public:
        temperatur_humidity_sensor();
        virtual ~temperatur_humidity_sensor();
 bool CheckSum(int *,MESSAGEOK *);
 bool MoveLastData(int *maxMessageIndex,int *lastPacketIndex,MESSAGEOK *messageIn);
 bool DoWorkByMESSAGEIN(int *maxMessageIndex,MESSAGEOK *messageIn);
         bool ParseBlock(int receiveBlockLength,BYTE *block,MESSAGEOK *messageIn,int *lastPacketIndex,int *maxMessageIndex);
         bool vClearMsg(MESSAGEOK *messageIn);
         void block_receive(MESSAGEOK messagein);
         void parseblockA(MESSAGEOK *mes,int length);
         void vAdjTimeByGPS(YMDHMS GPS);


         PDEVICEKEYPAD keypad;
    protected:
    private:
    bool record_clear_flag;
    MESSAGEOK T_H;
    int record_length;

};

#endif // TEMPERATUR_HUMIDITY_SENSOR_H
