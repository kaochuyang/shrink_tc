//---------------------------------------------------------------------------
#pragma hdrstop

#include "var.h"
#include "SMEM.h"
#include "PARSETRAFFIC.h"
#include "PARSERTMS.h"
#include "PARSENMEA.h"
#include "PARSESS.h"

#include "READJOB.h"
#include "WRITEJOB.h"

#include "LCD240x128.h"
#include "DIGITALIO.h"

#include "CTIMER.h"
#include "CDataToMessageOK.h"
#include "temperatur_humidity_sensor.h"
#include "SCREENLogo.h"

#include "STORAGE.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
/*Combo++*/
#include "CSTC.h"
#include "CSTORINI.h"
/*Combo--*/
#include "CREATEXML.h"//arwen ++


#include <iostream>

#include "json/json.h"

#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
//#include <pthread.h>
#include <climits>
#define MAXLINE 4096
#include "TCPserver.h"

//---------------------------------------------------------------------------
//void *timerHandle (void *);                                                     //另一個thread,專門控制Timer
//---------------------------------------------------------------------------



void SendRequestToKeypad(void)
{
    try
    {

        MESSAGEOK messageOut;
        messageOut.protocol=PROTOCOLKEYPAD;
        messageOut.packetLength=6;
        messageOut.cksStatus=true;
        messageOut.success=true;
        messageOut.ReadorWrite=cWRITE;

        messageOut.packet[0]=0xAA;
        messageOut.packet[1]=0xBB;
        messageOut.packet[2]=0x03;
        messageOut.packet[3]=0xAA;
        messageOut.packet[4]=0xCC;
        messageOut.packet[5]=0;
        for (int i=0; i<5; i++) messageOut.packet[5]^=messageOut.packet[i];

        writeJob.WriteWorkByMESSAGEOUT(messageOut);

    }
    catch (...) {}
}

//--------------------jacky20140108 Query 8051 Status-------------------------
void BRT_Query8051Status(void)
{
    try
    {
        MESSAGEOK messageOut;
        messageOut.protocol=PROTOCOLKEYPAD;
        messageOut.packetLength=7;
        messageOut.cksStatus=true;
        messageOut.success=true;
        messageOut.ReadorWrite=cWRITE;

        messageOut.packet[0]=0xAA;
        messageOut.packet[1]=0xBB;
        messageOut.packet[2]=0x24;
        messageOut.packet[3]=0x00;  //全面板資訊回報
        messageOut.packet[4]=0xAA;
        messageOut.packet[5]=0xCC;
        messageOut.packet[6]=0;
        for (int i=0; i<6; i++) messageOut.packet[6]^=messageOut.packet[i];

        writeJob.WritePhysicalOut(messageOut.packet, 7, DEVICEKEYPAD);

    }
    catch (...) {}
}
//---------------------jacky20140114 Query Keypad Status----------------------
void BRT_QueryKeypadStatus(void)
{
    try
    {
        MESSAGEOK messageOut;
        messageOut.protocol=PROTOCOLKEYPAD;
        messageOut.packetLength=7;
        messageOut.cksStatus=true;
        messageOut.success=true;
        messageOut.ReadorWrite=cWRITE;

        messageOut.packet[0]=0xAA;
        messageOut.packet[1]=0xBB;
        messageOut.packet[2]=0x24;
        messageOut.packet[3]=0x01;  //回報鍵盤狀態
        messageOut.packet[4]=0xAA;
        messageOut.packet[5]=0xCC;
        messageOut.packet[6]=0;
        for (int i=0; i<6; i++) messageOut.packet[6]^=messageOut.packet[i];

        writeJob.WritePhysicalOut(messageOut.packet, 7, DEVICEKEYPAD);

    }
    catch (...) {}
}

//---------------------------------------------------------------------------
#pragma argsused
int main(int argc, char* argv[])
{
    try
    {
        bool notexit=true;                                                          //while loop 永不離開
        unsigned char ucTmp;

        int iFWYearTmp;
        int iFWMonthTmp;
        int iFWDayTmp;
        char cTMP[128];

        int rec=-1;                                                                 //Select Result
        fd_set readfs;                                                              //File Descriptor Set
        int maxport=0,tempmax=0;                                                    //maxnum file desciptor used
        struct timeval timeout;                                                     //假使若干秒select都沒有資料進來,就做timeout要作的事情
        int readSelectLength=0;                                                     //判斷讀到的長度
        temperatur_humidity_sensor T_H_senseor;
        PARSETRAFFIC parseAABB;                                                     //解析所有AABB開頭AACC結尾的物件
        PARSERTMS parseRtms;                                                        //解析RTMS的物件
        PARSESS parseSS;                                                            //解析SmartSensor的物件
        PARSENMEA parseNMEA;
        tsUDPMappingLCN _sUDPMappingTMP;
        tsUDPInfo _sUDPMappingInfoTMP;
        MESSAGEOK shrink_mes;
        int iCom2Type;
        iCom2Type = smem.vGetINTData(Com2_TYPE);

        //remove tc reset file
        system("rm -rf /tmp/resetTCmyself.txt");
        system("sync");
        int iTmp;
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase0_0", 0);
        smem.vSetVDPhaseMapTable(0, 0, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase0_1", 0);
        smem.vSetVDPhaseMapTable(0, 1, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase0_2", 0);
        smem.vSetVDPhaseMapTable(0, 2, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase0_3", 0);
        smem.vSetVDPhaseMapTable(0, 3, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase0_4", 0);
        smem.vSetVDPhaseMapTable(0, 4, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase0_5", 0);
        smem.vSetVDPhaseMapTable(0, 5, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase0_6", 0);
        smem.vSetVDPhaseMapTable(0, 6, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase0_7", 0);
        smem.vSetVDPhaseMapTable(0, 7, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase1_0", 0);
        smem.vSetVDPhaseMapTable(1, 0, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase1_1", 0);
        smem.vSetVDPhaseMapTable(1, 1, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase1_2", 0);
        smem.vSetVDPhaseMapTable(1, 2, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase1_3", 0);
        smem.vSetVDPhaseMapTable(1, 3, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase1_4", 0);
        smem.vSetVDPhaseMapTable(1, 4, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase1_5", 0);
        smem.vSetVDPhaseMapTable(1, 5, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase1_6", 0);
        smem.vSetVDPhaseMapTable(1, 6, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase1_7", 0);
        smem.vSetVDPhaseMapTable(1, 7, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase2_0", 0);
        smem.vSetVDPhaseMapTable(2, 0, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase2_1", 0);
        smem.vSetVDPhaseMapTable(2, 1, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase2_2", 0);
        smem.vSetVDPhaseMapTable(2, 2, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase2_3", 0);
        smem.vSetVDPhaseMapTable(2, 3, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase2_4", 0);
        smem.vSetVDPhaseMapTable(2, 4, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase2_5", 0);
        smem.vSetVDPhaseMapTable(2, 5, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase2_6", 0);
        smem.vSetVDPhaseMapTable(2, 6, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase2_7", 0);
        smem.vSetVDPhaseMapTable(2, 7, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase3_0", 0);
        smem.vSetVDPhaseMapTable(3, 0, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase3_1", 0);
        smem.vSetVDPhaseMapTable(3, 1, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase3_2", 0);
        smem.vSetVDPhaseMapTable(3, 2, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase3_3", 0);
        smem.vSetVDPhaseMapTable(3, 3, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase3_4", 0);
        smem.vSetVDPhaseMapTable(3, 4, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase3_5", 0);
        smem.vSetVDPhaseMapTable(3, 5, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase3_6", 0);
        smem.vSetVDPhaseMapTable(3, 6, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase3_7", 0);
        smem.vSetVDPhaseMapTable(3, 7, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase4_0", 0);
        smem.vSetVDPhaseMapTable(4, 0, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase4_1", 0);
        smem.vSetVDPhaseMapTable(4, 1, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase4_2", 0);
        smem.vSetVDPhaseMapTable(4, 2, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase4_3", 0);
        smem.vSetVDPhaseMapTable(4, 3, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase4_4", 0);
        smem.vSetVDPhaseMapTable(4, 4, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase4_5", 0);
        smem.vSetVDPhaseMapTable(4, 5, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase4_6", 0);
        smem.vSetVDPhaseMapTable(4, 6, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase4_7", 0);
        smem.vSetVDPhaseMapTable(4, 7, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase5_0", 0);
        smem.vSetVDPhaseMapTable(5, 0, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase5_1", 0);
        smem.vSetVDPhaseMapTable(5, 1, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase5_2", 0);
        smem.vSetVDPhaseMapTable(5, 2, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase5_3", 0);
        smem.vSetVDPhaseMapTable(5, 3, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase5_4", 0);
        smem.vSetVDPhaseMapTable(5, 4, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase5_5", 0);
        smem.vSetVDPhaseMapTable(5, 5, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase5_6", 0);
        smem.vSetVDPhaseMapTable(5, 6, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase5_7", 0);
        smem.vSetVDPhaseMapTable(5, 7, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase6_0", 0);
        smem.vSetVDPhaseMapTable(6, 0, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase6_1", 0);
        smem.vSetVDPhaseMapTable(6, 1, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase6_2", 0);
        smem.vSetVDPhaseMapTable(6, 2, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase6_3", 0);
        smem.vSetVDPhaseMapTable(6, 3, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase6_4", 0);
        smem.vSetVDPhaseMapTable(6, 4, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase6_5", 0);
        smem.vSetVDPhaseMapTable(6, 5, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase6_6", 0);
        smem.vSetVDPhaseMapTable(6, 6, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase6_7", 0);
        smem.vSetVDPhaseMapTable(6, 7, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase7_0", 0);
        smem.vSetVDPhaseMapTable(7, 0, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase7_1", 0);
        smem.vSetVDPhaseMapTable(7, 1, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase7_2", 0);
        smem.vSetVDPhaseMapTable(7, 2, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase7_3", 0);
        smem.vSetVDPhaseMapTable(7, 3, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase7_4", 0);
        smem.vSetVDPhaseMapTable(7, 4, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase7_5", 0);
        smem.vSetVDPhaseMapTable(7, 5, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase7_6", 0);
        smem.vSetVDPhaseMapTable(7, 6, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VD", "Phase7_7", 0);
        smem.vSetVDPhaseMapTable(7, 7, iTmp);

//OT990401 FIX BUG
//OT20110706
        smem.vSetStopSend0x22(true);  //means have autorun 51
        bool ExtendMode;
        ExtendMode = oStorINI.vLoadIntFromINI("TC", "ExtendMode", 0);
        smem.vSetExtendMode(ExtendMode);
        printf("smem.vGetExtendMode() = %d\n",smem.vGetExtendMode());

        /*
            int iAutoRun51;
            iAutoRun51 = oStorINI.vLoadIntFromINI("LightCtlBoard", "AutoRun51", 0);
            if(iAutoRun51 > 0) {
              smem.vSetStopSend0x22(true);  //means have autorun 51
            }

            iTmp = oStorINI.vLoadIntFromINI("LightCtlBoard", "FWVerYear", 0);
            if(iTmp == 9) {
              iTmp = oStorINI.vLoadIntFromINI("LightCtlBoard", "FWVerMonth", 0);
              if(iTmp == 7) {
                iTmp = oStorINI.vLoadIntFromINI("LightCtlBoard", "FWVerDay", 0);
                if(iTmp == 2) {
                  printf("find bug version 51 light board\n");
                  smem.vSetStopSend0x22(true);  //means have autorun 51

                  if(iAutoRun51 == 0) {
                    oStorINI.vSaveIntToINI("LightCtlBoard", "AutoRun51", 1);
                  }

                }
              }
            }
        */

        /*
        usiVDPhaseMap[0][2] = oStorINI.vLoadIntFromINI("VD", "Phase0_2", 0);
        usiVDPhaseMap[0][3] = oStorINI.vLoadIntFromINI("VD", "Phase0_3", 0);
        usiVDPhaseMap[0][4] = oStorINI.vLoadIntFromINI("VD", "Phase0_4", 0);
        usiVDPhaseMap[0][5] = oStorINI.vLoadIntFromINI("VD", "Phase0_5", 0);
        usiVDPhaseMap[0][6] = oStorINI.vLoadIntFromINI("VD", "Phase0_6", 0);
        usiVDPhaseMap[0][7] = oStorINI.vLoadIntFromINI("VD", "Phase0_7", 0);
        usiVDPhaseMap[1][0] = oStorINI.vLoadIntFromINI("VD", "Phase1_0", 0);
        usiVDPhaseMap[1][1] = oStorINI.vLoadIntFromINI("VD", "Phase1_1", 0);
        usiVDPhaseMap[1][2] = oStorINI.vLoadIntFromINI("VD", "Phase1_2", 0);
        usiVDPhaseMap[1][3] = oStorINI.vLoadIntFromINI("VD", "Phase1_3", 0);
        usiVDPhaseMap[1][4] = oStorINI.vLoadIntFromINI("VD", "Phase1_4", 0);
        usiVDPhaseMap[1][5] = oStorINI.vLoadIntFromINI("VD", "Phase1_5", 0);
        usiVDPhaseMap[1][6] = oStorINI.vLoadIntFromINI("VD", "Phase1_6", 0);
        usiVDPhaseMap[1][7] = oStorINI.vLoadIntFromINI("VD", "Phase1_7", 0);
        usiVDPhaseMap[2][0] = oStorINI.vLoadIntFromINI("VD", "Phase2_0", 0);
        usiVDPhaseMap[2][1] = oStorINI.vLoadIntFromINI("VD", "Phase2_1", 0);
        usiVDPhaseMap[2][2] = oStorINI.vLoadIntFromINI("VD", "Phase2_2", 0);
        usiVDPhaseMap[2][3] = oStorINI.vLoadIntFromINI("VD", "Phase2_3", 0);
        usiVDPhaseMap[2][4] = oStorINI.vLoadIntFromINI("VD", "Phase2_4", 0);
        usiVDPhaseMap[2][5] = oStorINI.vLoadIntFromINI("VD", "Phase2_5", 0);
        usiVDPhaseMap[2][6] = oStorINI.vLoadIntFromINI("VD", "Phase2_6", 0);
        usiVDPhaseMap[2][7] = oStorINI.vLoadIntFromINI("VD", "Phase2_7", 0);
        usiVDPhaseMap[3][0] = oStorINI.vLoadIntFromINI("VD", "Phase3_0", 0);
        usiVDPhaseMap[3][1] = oStorINI.vLoadIntFromINI("VD", "Phase3_1", 0);
        usiVDPhaseMap[3][2] = oStorINI.vLoadIntFromINI("VD", "Phase3_2", 0);
        usiVDPhaseMap[3][3] = oStorINI.vLoadIntFromINI("VD", "Phase3_3", 0);
        usiVDPhaseMap[3][4] = oStorINI.vLoadIntFromINI("VD", "Phase3_4", 0);
        usiVDPhaseMap[3][5] = oStorINI.vLoadIntFromINI("VD", "Phase3_5", 0);
        usiVDPhaseMap[3][6] = oStorINI.vLoadIntFromINI("VD", "Phase3_6", 0);
        usiVDPhaseMap[3][7] = oStorINI.vLoadIntFromINI("VD", "Phase3_7", 0);
        usiVDPhaseMap[4][0] = oStorINI.vLoadIntFromINI("VD", "Phase4_0", 0);
        usiVDPhaseMap[4][1] = oStorINI.vLoadIntFromINI("VD", "Phase4_1", 0);
        usiVDPhaseMap[4][2] = oStorINI.vLoadIntFromINI("VD", "Phase4_2", 0);
        usiVDPhaseMap[4][3] = oStorINI.vLoadIntFromINI("VD", "Phase4_3", 0);
        usiVDPhaseMap[4][4] = oStorINI.vLoadIntFromINI("VD", "Phase4_4", 0);
        usiVDPhaseMap[4][5] = oStorINI.vLoadIntFromINI("VD", "Phase4_5", 0);
        usiVDPhaseMap[4][6] = oStorINI.vLoadIntFromINI("VD", "Phase4_6", 0);
        usiVDPhaseMap[4][7] = oStorINI.vLoadIntFromINI("VD", "Phase4_7", 0);
        usiVDPhaseMap[5][0] = oStorINI.vLoadIntFromINI("VD", "Phase5_0", 0);
        usiVDPhaseMap[5][1] = oStorINI.vLoadIntFromINI("VD", "Phase5_1", 0);
        usiVDPhaseMap[5][2] = oStorINI.vLoadIntFromINI("VD", "Phase5_2", 0);
        usiVDPhaseMap[5][3] = oStorINI.vLoadIntFromINI("VD", "Phase5_3", 0);
        usiVDPhaseMap[5][4] = oStorINI.vLoadIntFromINI("VD", "Phase5_4", 0);
        usiVDPhaseMap[5][5] = oStorINI.vLoadIntFromINI("VD", "Phase5_5", 0);
        usiVDPhaseMap[5][6] = oStorINI.vLoadIntFromINI("VD", "Phase5_6", 0);
        usiVDPhaseMap[5][7] = oStorINI.vLoadIntFromINI("VD", "Phase5_7", 0);
        usiVDPhaseMap[6][0] = oStorINI.vLoadIntFromINI("VD", "Phase6_0", 0);
        usiVDPhaseMap[6][1] = oStorINI.vLoadIntFromINI("VD", "Phase6_1", 0);
        usiVDPhaseMap[6][2] = oStorINI.vLoadIntFromINI("VD", "Phase6_2", 0);
        usiVDPhaseMap[6][3] = oStorINI.vLoadIntFromINI("VD", "Phase6_3", 0);
        usiVDPhaseMap[6][4] = oStorINI.vLoadIntFromINI("VD", "Phase6_4", 0);
        usiVDPhaseMap[6][5] = oStorINI.vLoadIntFromINI("VD", "Phase6_5", 0);
        usiVDPhaseMap[6][6] = oStorINI.vLoadIntFromINI("VD", "Phase6_6", 0);
        usiVDPhaseMap[6][7] = oStorINI.vLoadIntFromINI("VD", "Phase6_7", 0);
        usiVDPhaseMap[7][0] = oStorINI.vLoadIntFromINI("VD", "Phase7_0", 0);
        usiVDPhaseMap[7][1] = oStorINI.vLoadIntFromINI("VD", "Phase7_1", 0);
        usiVDPhaseMap[7][2] = oStorINI.vLoadIntFromINI("VD", "Phase7_2", 0);
        usiVDPhaseMap[7][3] = oStorINI.vLoadIntFromINI("VD", "Phase7_3", 0);
        usiVDPhaseMap[7][4] = oStorINI.vLoadIntFromINI("VD", "Phase7_4", 0);
        usiVDPhaseMap[7][5] = oStorINI.vLoadIntFromINI("VD", "Phase7_5", 0);
        usiVDPhaseMap[7][6] = oStorINI.vLoadIntFromINI("VD", "Phase7_6", 0);
        usiVDPhaseMap[7][7] = oStorINI.vLoadIntFromINI("VD", "Phase7_7", 0);
        */
        iTmp = oStorINI.vLoadIntFromINI("VDTrigger", "PhaseTriggerSwitch0", 0);
        smem.vSetVDPhaseTriggerSwitch(0, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VDTrigger", "PhaseTriggerSwitch1", 0);
        smem.vSetVDPhaseTriggerSwitch(1, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VDTrigger", "PhaseTriggerSwitch2", 0);
        smem.vSetVDPhaseTriggerSwitch(2, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VDTrigger", "PhaseTriggerSwitch3", 0);
        smem.vSetVDPhaseTriggerSwitch(3, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VDTrigger", "PhaseTriggerSwitch4", 0);
        smem.vSetVDPhaseTriggerSwitch(4, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VDTrigger", "PhaseTriggerSwitch5", 0);
        smem.vSetVDPhaseTriggerSwitch(5, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VDTrigger", "PhaseTriggerSwitch6", 0);
        smem.vSetVDPhaseTriggerSwitch(6, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("VDTrigger", "PhaseTriggerSwitch7", 0);
        smem.vSetVDPhaseTriggerSwitch(7, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("TC", "SendCCTActArwenProtocal", 0);
        smem.vSetUCData(TC_CCT_Send_ActuateArwen_Protocal, iTmp);
//OT990419
        iTmp = oStorINI.vLoadIntFromINI("RedCount", "DynShowRedCountForAct", 0);
        smem.vSetUCData(DynShowRedCountForAct, iTmp);
//OT20110607
        iTmp = oStorINI.vLoadIntFromINI("TC", "SendCCTPhaseCycleProtocalForCenter", 1);
        smem.vSetUCData(TC_CCT_SendCCTPhaseCycleProtocalForCenter, iTmp);
//OT20110624
        iTmp = oStorINI.vLoadIntFromINI("RedCount", "RedCountRepeatCounter", 3);
        smem.vSetUCData(CSTC_RedcountRepeatCount, iTmp);
//OT20111128
        iTmp = oStorINI.vLoadIntFromINI("TC", "MachineLocation", 0);
        smem.vSetUCData(TC_CCT_MachineLocation, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("CCTSwitch","PedestrianModeOpen",0);
        smem.vSetUCData(PedestrianModeOpen, iTmp);//行人觸動開關
        iTmp = oStorINI.vLoadIntFromINI("CCTSwitch","fixedGreenModeOpen",0);
        smem.vSetUCData(fixedGreenModeOpen, iTmp);//固定綠模式
//ARWEN1001207
        iTmp = oStorINI.vLoadIntFromINI("RedCount","ShowArwenActRedCountSec",0);
        smem.vSetUCData(ArwenActRedCountSec, iTmp);//5FA6改變紅燈倒數

//jacky20140416
        iTmp = oStorINI.vLoadIntFromINI("TC", "SetCompensationMingreen", 0);
        smem.SetCompensationMingreen(iTmp);
        /*
        ucVDPhaseTriggerSwitch[1] = oStorINI.vLoadIntFromINI("VDTrigger", "PhaseTriggerSwitch1", 0);
        ucVDPhaseTriggerSwitch[2] = oStorINI.vLoadIntFromINI("VDTrigger", "PhaseTriggerSwitch2", 0);
        ucVDPhaseTriggerSwitch[3] = oStorINI.vLoadIntFromINI("VDTrigger", "PhaseTriggerSwitch3", 0);
        ucVDPhaseTriggerSwitch[4] = oStorINI.vLoadIntFromINI("VDTrigger", "PhaseTriggerSwitch4", 0);
        ucVDPhaseTriggerSwitch[5] = oStorINI.vLoadIntFromINI("VDTrigger", "PhaseTriggerSwitch5", 0);
        ucVDPhaseTriggerSwitch[6] = oStorINI.vLoadIntFromINI("VDTrigger", "PhaseTriggerSwitch6", 0);
        ucVDPhaseTriggerSwitch[7] = oStorINI.vLoadIntFromINI("VDTrigger", "PhaseTriggerSwitch7", 0);
        */
        /*
          通訊埠分佈
          RS232,422,485 通訊埠
          /dev/ttyS0(RS232)         交通控制中心-GPRS MODEM-(9600 N 8 1)
          /dev/ttyS1(RS232)         Resrever
          /dev/ttyS2(RS232)         KEYPAD-(9600 N 8 1)
          /dev/ttyS3(RS232)         燈點控制(9600 N 8 1)
          ??????????(RS485)         RTMS-(9600 N 8 1)
          UDP SOCKET 通訊埠
          192.168.1.101:6003        交通控制中心
          192.168.1.101:6789        保留測試用
          192.168.1.102:6003        現場測試機
          IO 通訊埠
          LPT1  LCD(240x128)  位址-0x378
          DGIO  Light         位址-In:0x200 Out:0x208
                           */


////-----------------------------------------------------------------------------------------------------//
          system("ifconfig eth1 192.168.2.1");//for VSX-6156
        if ( smem.revAPP_socket.SetConnDevice(1000))
            if ((tempmax= smem.revAPP_socket.OpenUdpSocket("192.168.2.1",7002,"192.168.2.2",7002))>0)
            {
                if (tempmax>maxport)  maxport=tempmax;
                printf("open rev_socket-%s:%d (fdValue:%d) Success!!\n","192.168.2.1",7002,tempmax);
            }
            else printf("open CenterSocket-%s:%d Fail!!\n","192.168.2.1",7002);
        //   PTRAFFIC92TC oRev_protocol_5F;
        PTRAFFIC92COMM oRev_protocol_0F;
        MESSAGEOK revAPP_messagein;
////-----------------------------------------------------------------------------------------------------//

        //開啟RS232,422,485 通訊埠
        //開啟交通控制中心通訊埠
        ucTmp = smem.vGetUCData(TC_ReverseLane_Manual_Enable_Status);
        printf("smem.vGetUCData(TC_ReverseLane_Manual_Enable_Status):%d\n", ucTmp);


        if(iCom2Type != Com2IsRedCountAndCom1IsPedPushButton)
        {
            //arwen ++ Com1不為行人按鈕
            if( 1 == ucTmp )
            {
                if (smem.revLaneManualPort.SetConnDevice(DEVICEREVMANUALPAD))
                    if ((tempmax=smem.revLaneManualPort.OpenRs232Port("/dev/ttyS0",2400, true))>0)
                    {
                        if (tempmax>maxport)
                        {
                            maxport = tempmax;
                        }
                        printf("open revLaneManualPort-/dev/ttyS0 (fdValue:%d) Success!!\n",tempmax);
                    }
                    else printf("open revLaneManualPort Fail!!\n");
            }
            else
            {
                if (smem.centerPort.SetConnDevice(DEVICECENTER92))
                    if ((tempmax=smem.centerPort.OpenRs232Port("/dev/ttyS0",9600, false))>0)
                    {
                        if (tempmax>maxport)
                        {
                            maxport = tempmax;
                        }
                        printf("open CenterPort-/dev/ttyS0 (fdValue:%d) Success!!\n",tempmax);
                    }
                    else printf("open CenterPort Fail!!\n");
            }
        }
        /*OTCombo0714 Remove DEVICESS*/
        //OT Debug 0523
        if(iCom2Type == Com2IsTesterPort)
        {
            //開啟test埠
            printf("Com2 is TesterPort Port!\n");
        }
        else if (iCom2Type == Com2IsGPSPort || iCom2Type == Com2IsHOLUXGPSPort)
        {
            //開啟GPS Port
            printf("Com2 is GPSPort Port!\n");
            if (smem.NMEAPort.SetConnDevice(DEVICEGPS))
            {
                if(iCom2Type == Com2IsGPSPort)
                    tempmax=smem.NMEAPort.OpenRs232Port("/dev/ttyS1",4800,false);
                else if(iCom2Type == Com2IsHOLUXGPSPort)
                    tempmax=smem.NMEAPort.OpenRs232Port("/dev/ttyS1",9600,false);
                if (tempmax>0)
                {
                    if (tempmax>maxport)  maxport=tempmax;
                    //OTMARKPRINTF  printf("open NMEAPort-/dev/ttyS1 (fdValue:%d) Success!!\n",tempmax);
                }
                else printf("open NMEAPort Fail!!\n");
            }
        }
        else if (iCom2Type == Com2IsPassingPort)
        {
            printf("Com2 is Passing Port!\n");
            if (smem.passingPort.SetConnDevice(DEVICE87CMS))                          //ForDongKungSF
                if ((tempmax=smem.passingPort.OpenRs232Port("/dev/ttyS1",9600,false))>0)
                {
                    if (tempmax>maxport)  maxport=tempmax;
                    printf("open passingPort-/dev/ttyS1 (fdValue:%d) Success!!\n",tempmax);
                }
                else printf("open passingPort Fail!!\n");
        }
        else if (iCom2Type == Com2IsRedCount)
        {
            if(smem.vGetINTData(TC92_RedCountVer) == TC_RedCountVer94 || smem.vGetINTData(TC92_RedCountVer) == TC_RedCountVer94v2)
            {
                if (smem.redCountPort.SetConnDevice(DEVICEREDCOUNTVER94))
                    if ((tempmax=smem.redCountPort.OpenRs232Port("/dev/ttyS1", 2400, true))>0)
                    {
                        if (tempmax>maxport)  maxport=tempmax;
                        printf("open RedCountPortHC-/dev/ttyS1 (fdValue:%d) Success!!\n",tempmax);
                    }
                    else printf("open RedCountPortHC Fail!!\n");
            }
            else if(smem.vGetINTData(TC92_RedCountVer) == TC_RedCountVerHK)
            {
                if (smem.redCountPort.SetConnDevice(DEVICEREDCOUNTVERHK))
                    if ((tempmax=smem.redCountPort.OpenRs232Port("/dev/ttyS1", 2400, true))>0)
                    {
                        if (tempmax>maxport)  maxport=tempmax;
                        printf("open RedCountPortHK-/dev/ttyS1 (fdValue:%d) Success!!\n",tempmax);
                    }
                    else printf("open RedCountPortHK Fail!!\n");
            }
            else if(smem.vGetINTData(TC92_RedCountVer) == TC_RedCountVerCCT97)
            {
                if (smem.redCountPort.SetConnDevice(DEVICEREDCOUNTVERCCT97))
                    //Change        if ((tempmax=smem.redCountPort.OpenRs232Port("/dev/ttyS1", 9600, false))>0)  {
                    if ((tempmax=smem.redCountPort.OpenRs232Port("/dev/ttyS1", 2400, true))>0)
                    {
                        if (tempmax>maxport)  maxport=tempmax;
                        printf("open RedCountPortCCT97-/dev/ttyS1 (fdValue:%d) Success!!\n",tempmax);
                    }
                    else printf("open RedCountPortCCT97 Fail!!\n");
            }
        }
        else if (iCom2Type == Com2IsTainanPeopleLight)
        {
            printf("Com2 is TainanPeopleLight Port!\n");
            if (smem.com2Port.SetConnDevice(DEVICETAINANPEOPLELIGHT))                 //ForTainan
                if ((tempmax=smem.com2Port.OpenRs232Port("/dev/ttyS1",9600,false))>0)
                {
                    if (tempmax>maxport)  maxport=tempmax;
                    printf("open TainanPeopleLightPort-/dev/ttyS1 (fdValue:%d) Success!!\n",tempmax);
                }
                else printf("open TainanPeopleLightPort Fail!!\n");
        }
        else if (iCom2Type == Com2IsPedestrianPushButton)
        {
            //add Arwen
            printf("Com2 is PedestrianPushButton Port!\n");
            if (smem.PedPushButtonPort.SetConnDevice(DEVICEPEDESTRIAN))
                if ((tempmax=smem.PedPushButtonPort.OpenRs232Port("/dev/ttyS1",2400,true))>0)
                {
                    //2400 8,E,1
                    if (tempmax>maxport)  maxport=tempmax;
                    printf("open PedestrianPushButtonPort-/dev/ttyS1 (fdValue:%d) Success!!\n",tempmax);
                }
                else printf("open PedestrianPushButtonPort Fail!!\n");
        }
        else if (iCom2Type == Com2IsRedCountAndCom1IsPedPushButton)
        {
            //add Arwen 紅燈&行人
            if(smem.vGetINTData(TC92_RedCountVer) == TC_RedCountVer94 || smem.vGetINTData(TC92_RedCountVer) == TC_RedCountVer94v2)
            {
                if (smem.redCountPort.SetConnDevice(DEVICEREDCOUNTVER94))
                    if ((tempmax=smem.redCountPort.OpenRs232Port("/dev/ttyS1", 2400, true))>0)
                    {
                        if (tempmax>maxport)  maxport=tempmax;
                        printf("open RedCountPortHC-/dev/ttyS1 (fdValue:%d) Success!!\n",tempmax);
                    }
                    else printf("open RedCountPortHC Fail!!\n");
            }
            else if(smem.vGetINTData(TC92_RedCountVer) == TC_RedCountVerHK)
            {
                if (smem.redCountPort.SetConnDevice(DEVICEREDCOUNTVERHK))
                    if ((tempmax=smem.redCountPort.OpenRs232Port("/dev/ttyS1", 2400, true))>0)
                    {
                        if (tempmax>maxport)  maxport=tempmax;
                        printf("open RedCountPortHK-/dev/ttyS1 (fdValue:%d) Success!!\n",tempmax);
                    }
                    else printf("open RedCountPortHK Fail!!\n");
            }
            else if(smem.vGetINTData(TC92_RedCountVer) == TC_RedCountVerCCT97)
            {
                if (smem.redCountPort.SetConnDevice(DEVICEREDCOUNTVERCCT97))
                    //Change        if ((tempmax=smem.redCountPort.OpenRs232Port("/dev/ttyS1", 9600, false))>0)  {
                    if ((tempmax=smem.redCountPort.OpenRs232Port("/dev/ttyS1", 2400, true))>0)
                    {
                        if (tempmax>maxport)  maxport=tempmax;
                        printf("open RedCountPortCCT97-/dev/ttyS1 (fdValue:%d) Success!!\n",tempmax);
                    }
                    else printf("open RedCountPortCCT97 Fail!!\n");
            }
            //--------------------------------------------------------------------------------
            printf("Com1 is PedestrianPushButton Port!\n");
            if (smem.PedPushButtonPort.SetConnDevice(DEVICEPEDESTRIAN))
                if ((tempmax=smem.PedPushButtonPort.OpenRs232Port("/dev/ttyS0",2400,true))>0)
                {
                    //2400 8,E,1
                    if (tempmax>maxport)  maxport=tempmax;
                    printf("open PedestrianPushButtonPort-/dev/ttyS0 (fdValue:%d) Success!!\n",tempmax);
                }
                else printf("open PedestrianPushButtonPort Fail!!\n");
        }
        //開啟KEYPAD通訊埠
        /*        if (smem.keypadPort.SetConnDevice(DEVICEKEYPAD))
                    if ((tempmax=smem.keypadPort.OpenRs232Port("/dev/ttyS2",9600,false))>0)
                    {
                        if (tempmax>maxport)  maxport=tempmax;
                        printf("open KeypadPort-/dev/ttyS2 (fdValue:%d) Success!!\n",tempmax);
                    }
                    else printf("open KeypadPort Fail!!\n");
        */
        if (smem.shrink_port_com3.SetConnDevice(DEVICEKEYPAD))
            if ((tempmax=smem.shrink_port_com3.OpenRs232Port("/dev/ttyS2",9600,false))>0)
            {
                if (tempmax>maxport)  maxport=tempmax;
                printf("open shrink_port_com3-/dev/ttyS2 (fdValue:%d) Success!!\n",tempmax);
            }
            else printf("open shrink_port_com3 Fail!!\n");
        //開啟Traffic Light
        if (smem.lightPort.SetConnDevice(DEVICETRAFFICLIGHT))
            if ((tempmax=smem.lightPort.OpenRs232Port("/dev/ttyS3",9600,false))>0)
            {
                if (tempmax>maxport)  maxport=tempmax;
                printf("open TrafficLightPort-/dev/ttyS3 (fdValue:%d) Success!!\n",tempmax);
            }
            else printf("open LightPort Fail!!\n");
//------------------------------------------chou ++ 2013/03/22 新增COM5GPS
        if (smem.GPSPort.SetConnDevice(DEVICEGPS))
            if ((tempmax=smem.GPSPort.OpenRs232Port("/dev/ttyS4",4800,false))>0)
            {
                if (tempmax>maxport)  maxport=tempmax;
                printf("open GPSPort-/dev/ttyS4 (fdValue:%d) Success!!\n",tempmax);
            }
            else printf("open GPSPort Fail!!\n");

//------------------------------------------CHOU ++ 2013/03/22 新增COM6紅燈倒數
        if (smem.NSPredCountPort.SetConnDevice(DEVICENSPREDCOUNT))
            if ((tempmax=smem.NSPredCountPort.OpenRs232Port("/dev/ttyS5",9600,false))>0)
            {
                if (tempmax>maxport)  maxport=tempmax;
                printf("open NSPredCountPort-/dev/ttyS5 (fdValue:%d) Success!!\n",tempmax);
            }
            else printf("open NSPredCountPort Fail!!\n");
//--------------------------------------------------------------------------------------
//------------------------------------------CHOU ++ 2013/05/16 新增COM7 小綠人
        if (smem.greenmanPort.SetConnDevice(DEVICEGREENMANCOUNT))
            if ((tempmax=smem.greenmanPort.OpenRs232Port("/dev/ttyS6",9600,false))>0)
            {
                if (tempmax>maxport)  maxport=tempmax;
                printf("open greenmanPort-/dev/ttyS6 (fdValue:%d) Success!!\n",tempmax);
            }
            else printf("open greenmanPort Fail!!\n");
//////////////////////////////////////////////////////////

//OTSS++
        //Open SmartPort
        //OT1000211 disable
        /*
        if (smem.ssPort.SetConnDevice(DEVICESS))
        if ((tempmax=smem.ssPort.OpenRs232Port("/dev/ttyN1",115200,false))>0)  {
             if (tempmax>maxport)  maxport=tempmax;
             printf("open SmartSensorPort-/dev/ttyN1 (fdValue:%d) Success!!\n",tempmax);
        } else printf("open SSPort Fail!!\n");
        */
//OTSS--


        //紅燈倒數

//OT Debug 0523
        if(iCom2Type != Com2IsRedCount)
        {
            if(smem.vGetINTData(TC92_RedCountVer) == TC_RedCountVer94)
            {
                if (smem.redCountPort.SetConnDevice(DEVICEREDCOUNTVER94))
                    if ((tempmax=smem.redCountPort.OpenRs232Port("/dev/ttyN0", 2400, true))>0)
                    {
                        if (tempmax>maxport)  maxport=tempmax;
                        printf("open RedCountPortHC-/dev/ttyN0 (fdValue:%d) Success!!\n",tempmax);
                    }
                    else printf("open RedCountPortHC Fail!!\n");
            }
            else if(smem.vGetINTData(TC92_RedCountVer) == TC_RedCountVerHK)
            {
                if (smem.redCountPort.SetConnDevice(DEVICEREDCOUNTVERHK))
                    if ((tempmax=smem.redCountPort.OpenRs232Port("/dev/ttyN0", 2400, true))>0)
                    {
                        if (tempmax>maxport)  maxport=tempmax;
                        printf("open RedCountPortHK-/dev/ttyN0 (fdValue:%d) Success!!\n",tempmax);
                    }
                    else printf("open RedCountPortHK Fail!!\n");
            }
        }

//頁面設定IP
        int localPort1=smem.GetLocalIP1(5);
        int distPort0=smem.GetdistIp0(5);
        int distPort=smem.GetDistIP(5);

        char localIP1[20]= {0},distIP0[20]= {0},distIP[20]= {0};
        char tmp[5]= {0};

        for (int ip=1; ip<=4; ip++)
        {
            sprintf(tmp,"%d",smem.GetLocalIP1(ip));
            strcat(localIP1,tmp);
            if (ip<4) strcat(localIP1,".");
            sprintf(tmp,"%d",smem.GetdistIp0(ip));
            strcat(distIP0,tmp);
            if (ip<4) strcat(distIP0,".");
            sprintf(tmp,"%d",smem.GetDistIP(ip));
            strcat(distIP,tmp);
            if (ip<4) strcat(distIP,".");
        }


        printf("\nIP1 : %s:%d\n",localIP1,localPort1);
        printf("DISTIP0 : %s:%d\n",distIP0, distPort0);
        printf("DISTIP : %s:%d\n\n",distIP,distPort);


        //開啟UDP SOCKET 通訊埠
        //開啟與交通控制中心通訊的UDP SOCKET


        if (smem.centerSocket.SetConnDevice(DEVICECENTER92))
            if ((tempmax=smem.centerSocket.OpenUdpSocket(localIP1,localPort1,distIP0,distPort0))>0)
            {
                if (tempmax>maxport)  maxport=tempmax;
                printf("open CenterSocket-%s:%d (fdValue:%d) Success!!\n",localIP1,localPort1,tempmax);
            }
            else printf("open CenterSocket-%s:%d Fail!!\n",localIP1,localPort1);

        if (smem.testerSocket.SetConnDevice(DEVICETESTER92))
            if ((tempmax=smem.testerSocket.OpenUdpSocket(localIP1,localPort1+64,distIP,distPort))>0)
            {
                if (tempmax>maxport)  maxport=tempmax;
                printf("open TesterSocket-%s:%d (fdValue:%d) Success!!\n",localIP1,localPort1+64,tempmax);
            }
            else printf("open TesterSocket-%s:%d Fail!!\n",localIP1,localPort1+64);

        //路口車輛偵測器，I/O UDP SOCKET

        /* OT ADD 950927 */
        _sUDPMappingTMP = smem.vGetCCJDynCtlIPData();
        if( _sUDPMappingTMP.iListenPort != 0 )
        {
            if (smem.CCJDynCtlSocket.SetConnDevice(DEVICECCJDYNCTL))
                if ((tempmax=smem.CCJDynCtlSocket.OpenUdpSocket(_sUDPMappingTMP.cLocalIP, _sUDPMappingTMP.iListenPort,
                             _sUDPMappingTMP.cRemoteIP,_sUDPMappingTMP.iSendPort)) > 0)
                {
                    if (tempmax>maxport)  maxport=tempmax;
                    printf("open CCJDynCtlSocket-%s:%d (fdValue:%d) Success!!\n", _sUDPMappingTMP.cLocalIP, _sUDPMappingTMP.iListenPort, tempmax);
                }
                else printf("open CCJDynCtlSocket-%s:%d %dFail!!\n", _sUDPMappingTMP.cLocalIP, _sUDPMappingTMP.iListenPort, tempmax);
        }

//OT ADD 990326
        _sUDPMappingTMP = smem.vGetCCJDynCtl2IPData();
        if( _sUDPMappingTMP.iListenPort != 0 )
        {
            if (smem.CCJDynCtl2Socket.SetConnDevice(DEVICECCJDYNCTL))
                if ((tempmax=smem.CCJDynCtl2Socket.OpenUdpSocket(_sUDPMappingTMP.cLocalIP, _sUDPMappingTMP.iListenPort,
                             _sUDPMappingTMP.cRemoteIP,_sUDPMappingTMP.iSendPort)) > 0)
                {
                    if (tempmax>maxport)  maxport=tempmax;
                    printf("open CCJDynCtl2Socket-%s:%d (fdValue:%d) Success!!\n", _sUDPMappingTMP.cLocalIP, _sUDPMappingTMP.iListenPort, tempmax);
                }
                else printf("open CCJDynCtl2Socket-%s:%d %dFail!!\n", _sUDPMappingTMP.cLocalIP, _sUDPMappingTMP.iListenPort, tempmax);
        }

        _sUDPMappingTMP = smem.vGetArwenVDInfoIPData();
        if( _sUDPMappingTMP.iListenPort != 0 )  //arwen ++ for VDINFO
        {
            if (smem.ArwenVDInfoSocket.SetConnDevice(DEVICEARWENVDINFO))
                if ((tempmax=smem.ArwenVDInfoSocket.OpenUdpSocket(_sUDPMappingTMP.cLocalIP, _sUDPMappingTMP.iListenPort,
                             _sUDPMappingTMP.cRemoteIP,_sUDPMappingTMP.iSendPort)) > 0)
                {
                    if (tempmax>maxport)  maxport=tempmax;
                    printf("open ArwenVDInfoSocket-%s:%d (fdValue:%d) Success!!\n", _sUDPMappingTMP.cLocalIP, _sUDPMappingTMP.iListenPort, tempmax);
                }
                else printf("open ArwenVDInfoSocket-%s:%d %dFail!!\n", _sUDPMappingTMP.cLocalIP, _sUDPMappingTMP.iListenPort, tempmax);
        }

        _sUDPMappingTMP = smem.vGetWebSetIPData();
        if( _sUDPMappingTMP.iListenPort != 0 )  //arwen ++ for WEBSET 1001101
        {
            if (smem.WebSetSocket.SetConnDevice(DEVICEWEBSET))
                if ((tempmax=smem.WebSetSocket.OpenUdpSocket(_sUDPMappingTMP.cLocalIP, _sUDPMappingTMP.iListenPort,
                             _sUDPMappingTMP.cRemoteIP,_sUDPMappingTMP.iSendPort)) > 0)
                {
                    if (tempmax>maxport)  maxport=tempmax;
                    printf("open WebSetSocket-%s:%d (fdValue:%d) Success!!\n", _sUDPMappingTMP.cLocalIP, _sUDPMappingTMP.iListenPort, tempmax);
                }
                else printf("open WebSetSocket-%s:%d %dFail!!\n", _sUDPMappingTMP.cLocalIP, _sUDPMappingTMP.iListenPort, tempmax);
        }

//OT20140224
        _sUDPMappingTMP = smem.vGetBRTSetIPData();
        if( _sUDPMappingTMP.iListenPort != 0 )
        {
            if (smem.BRTSetSocket.SetConnDevice(DEVICEBRT))
                if ((tempmax=smem.BRTSetSocket.OpenUdpSocket(_sUDPMappingTMP.cLocalIP, _sUDPMappingTMP.iListenPort,
                             _sUDPMappingTMP.cRemoteIP,_sUDPMappingTMP.iSendPort)) > 0)
                {
                    if (tempmax>maxport)  maxport=tempmax;
                    printf("open BRTSetSocket-%s:%d (fdValue:%d) Success!!\n", _sUDPMappingTMP.cLocalIP, _sUDPMappingTMP.iListenPort, tempmax);
                }
                else printf("open BRTSetSocket-%s:%d %dFail!!\n", _sUDPMappingTMP.cLocalIP, _sUDPMappingTMP.iListenPort, tempmax);
        }

//jacky20140327
        _sUDPMappingTMP = smem.vGetJsonSetIPData();
        if( _sUDPMappingTMP.iListenPort != 0 )
        {
            if (smem.JsonSetSocket.SetConnDevice(DEVICEJSON))
                if ((tempmax=smem.JsonSetSocket.OpenUdpSocket(_sUDPMappingTMP.cLocalIP, _sUDPMappingTMP.iListenPort,
                             _sUDPMappingTMP.cRemoteIP,_sUDPMappingTMP.iSendPort)) > 0)
                {
                    if (tempmax>maxport)  maxport=tempmax;
                    printf("open JsonSetSocket-%s:%d (fdValue:%d) Success!!\n", _sUDPMappingTMP.cLocalIP, _sUDPMappingTMP.iListenPort, tempmax);
                }
                else printf("open JsonSetSocket-%s:%d %dFail!!\n", _sUDPMappingTMP.cLocalIP, _sUDPMappingTMP.iListenPort, tempmax);
        }

        //OT990618
        iTmp = oStorINI.vLoadIntFromINI("revSyncSocket", "revSyncEnable", 0);
        smem.vSetUCData(revSyncEnable, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("revSyncSocket", "revSyncInterval", 3);
        smem.vSetUCData(revSyncInterval, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("revSyncSocket", "revSyncTimeDiffTolarence", 10);
        smem.vSetUCData(revSyncTimeDiffTolarence, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("revSyncSocket", "revSyncCountineStatusAdj", 3);
        smem.vSetUCData(revSyncCountineStatusAdj, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("revSyncSocket", "revDefaultVehWay", 0);
        smem.vSetUCData(revDefaultVehWay, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("revSyncSocket", "revLogToFile", 0);
        smem.vSetUCData(revLogToFile, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("revSyncSocket", "revSendProtocol", 0);
        smem.vSetUCData(revSendProtocol, iTmp);
        _sUDPMappingInfoTMP.cLocalIP = oStorINI.vLoadStringFromINI("revSyncSocket", "localIP", "127.0.0.1");
        _sUDPMappingInfoTMP.iListenPort = oStorINI.vLoadIntFromINI("revSyncSocket", "localPort", 0);
        _sUDPMappingInfoTMP.cRemoteIP = oStorINI.vLoadStringFromINI("revSyncSocket", "destIP", "127.0.0.1");
        _sUDPMappingInfoTMP.iSendPort = oStorINI.vLoadIntFromINI("revSyncSocket", "destPort", 0);
        if( _sUDPMappingInfoTMP.iListenPort != 0 )
        {
            if (smem.revSyncSocket.SetConnDevice(DEVICEREVSYNC))
                if ((tempmax=smem.revSyncSocket.OpenUdpSocket(_sUDPMappingInfoTMP.cLocalIP, _sUDPMappingInfoTMP.iListenPort,
                             _sUDPMappingInfoTMP.cRemoteIP,_sUDPMappingInfoTMP.iSendPort)) > 0)
                {
                    if (tempmax>maxport)  maxport=tempmax;
                    printf("open revSyncSocket-%s:%d (fdValue:%d) Success!!\n", _sUDPMappingInfoTMP.cLocalIP, _sUDPMappingInfoTMP.iListenPort, tempmax);
                }
                else printf("open revSyncSocket-%s:%d %dFail!!\n", _sUDPMappingInfoTMP.cLocalIP, _sUDPMappingInfoTMP.iListenPort, tempmax);
        }

        //OT990707
        iTmp = oStorINI.vLoadIntFromINI("amegidsSocket", "amegidsDynEnable", 0);
        smem.vSetUCData(amegidsDynEnable, iTmp);
        iTmp = oStorINI.vLoadIntFromINI("amegidsSocket", "amegidsDynDevLCN", 0);
        smem.vSetUSIData(amegidsDynDevLCN, iTmp);
        _sUDPMappingInfoTMP.cLocalIP = oStorINI.vLoadStringFromINI("amegidsSocket", "localIP", "127.0.0.1");
        _sUDPMappingInfoTMP.iListenPort = oStorINI.vLoadIntFromINI("amegidsSocket", "localPort", 0);
        _sUDPMappingInfoTMP.cRemoteIP = oStorINI.vLoadStringFromINI("amegidsSocket", "destIP", "127.0.0.1");
        _sUDPMappingInfoTMP.iSendPort = oStorINI.vLoadIntFromINI("amegidsSocket", "destPort", 0);
        if( _sUDPMappingInfoTMP.iListenPort != 0 )
        {
            if (smem.amegidsSocket.SetConnDevice(DEVICEAMEGIDS))
                if ((tempmax=smem.amegidsSocket.OpenUdpSocket(_sUDPMappingInfoTMP.cLocalIP, _sUDPMappingInfoTMP.iListenPort,
                             _sUDPMappingInfoTMP.cRemoteIP,_sUDPMappingInfoTMP.iSendPort)) > 0)
                {
                    if (tempmax>maxport)  maxport=tempmax;
                    printf("open amegidsSocket-%s:%d (fdValue:%d) Success!!\n", _sUDPMappingInfoTMP.cLocalIP, _sUDPMappingInfoTMP.iListenPort, tempmax);
                }
                else printf("open amegidsSocket-%s:%d %dFail!!\n", _sUDPMappingInfoTMP.cLocalIP, _sUDPMappingInfoTMP.iListenPort, tempmax);
        }


        /*-- OT ADD 950927 */

        _sUDPMappingTMP = smem.vGetUDPMappingLCNData(0);
        if( _sUDPMappingTMP.iListenPort != 0 )
        {
            if (smem.DynCalServerInCrossSocket.SetConnDevice(DEVICECENTER92))         //Amegids
//      if (smem.DynCalServerInCrossSocket.SetConnDevice(DEVICE77TC))         //Amegids
                if ((tempmax=smem.DynCalServerInCrossSocket.OpenUdpSocket(_sUDPMappingTMP.cLocalIP, _sUDPMappingTMP.iListenPort,
                             _sUDPMappingTMP.cRemoteIP,_sUDPMappingTMP.iSendPort)) > 0)
                {
                    if (tempmax>maxport)  maxport=tempmax;
                    printf("open DynCalServerInCrossSocket-%s:%d (fdValue:%d) Success!!\n", _sUDPMappingTMP.cLocalIP, _sUDPMappingTMP.iListenPort, tempmax);
                }
                else printf("open DynCalServerInCrossSocket-%s:%d %dFail!!\n", _sUDPMappingTMP.cLocalIP, _sUDPMappingTMP.iListenPort, tempmax);
        }

        _sUDPMappingTMP = smem.vGetUDPMappingLCNData(1);
        if( _sUDPMappingTMP.iListenPort != 0 )
        {
            if (smem.DynCalServerInCenterSocket.SetConnDevice(DEVICECENTER92))
                if ((tempmax=smem.DynCalServerInCenterSocket.OpenUdpSocket(_sUDPMappingTMP.cLocalIP, _sUDPMappingTMP.iListenPort,
                             _sUDPMappingTMP.cRemoteIP,_sUDPMappingTMP.iSendPort)) > 0)
                {
                    if (tempmax>maxport)  maxport=tempmax;
                    printf("open DynCalServerInCenterSocket-%s:%d (fdValue:%d) Success!!\n", _sUDPMappingTMP.cLocalIP, _sUDPMappingTMP.iListenPort, tempmax);
                }
                else printf("open DynCalServerInCenterSocket-%s:%d %dFail!!\n", _sUDPMappingTMP.cLocalIP, _sUDPMappingTMP.iListenPort, tempmax);
        }


        _sUDPMappingTMP = smem.vGetUDPMappingLCNData(2);
        if( _sUDPMappingTMP.iListenPort != 0 )
        {
            if (smem.SSVD01Socket.SetConnDevice(DEVICE92TC))                          //Amegids
                if ((tempmax=smem.SSVD01Socket.OpenUdpSocket(_sUDPMappingTMP.cLocalIP, _sUDPMappingTMP.iListenPort,
                             _sUDPMappingTMP.cRemoteIP,_sUDPMappingTMP.iSendPort)) > 0)
                {
                    if (tempmax>maxport)  maxport=tempmax;
                    printf("open SSVD01-%s:%d (fdValue:%d) Success!!\n", _sUDPMappingTMP.cLocalIP, _sUDPMappingTMP.iListenPort, tempmax);
                }
                else printf("open SSVD01Socket-%s:%d %d Fail!!\n", _sUDPMappingTMP.cLocalIP, _sUDPMappingTMP.iListenPort, tempmax);
        }

        _sUDPMappingTMP = smem.vGetUDPMappingLCNData(3);
        if( _sUDPMappingTMP.iListenPort != 0 )
        {
            if (smem.SSVD02Socket.SetConnDevice(DEVICE92TC))                          //Amegids
                if ((tempmax=smem.SSVD02Socket.OpenUdpSocket(_sUDPMappingTMP.cLocalIP, _sUDPMappingTMP.iListenPort,
                             _sUDPMappingTMP.cRemoteIP,_sUDPMappingTMP.iSendPort)) > 0)
                {
                    if (tempmax>maxport)  maxport=tempmax;
                    printf("open SSVD02-%s:%d (fdValue:%d) Success!!\n", _sUDPMappingTMP.cLocalIP, _sUDPMappingTMP.iListenPort, tempmax);
                }
                else printf("open SSVD02Socket-%s:%d %d Fail!!\n", _sUDPMappingTMP.cLocalIP, _sUDPMappingTMP.iListenPort, tempmax);
        }

        _sUDPMappingTMP = smem.vGetUDPMappingLCNData(4);
        if( _sUDPMappingTMP.iListenPort != 0 )
        {
            if (smem.SSVD03Socket.SetConnDevice(DEVICE92TC))                          //Amegids
                if ((tempmax=smem.SSVD03Socket.OpenUdpSocket(_sUDPMappingTMP.cLocalIP, _sUDPMappingTMP.iListenPort,
                             _sUDPMappingTMP.cRemoteIP,_sUDPMappingTMP.iSendPort)) > 0)
                {
                    if (tempmax>maxport)  maxport=tempmax;
                }
                else printf("open SSVD03Socket-%s:%d Fail!!\n", _sUDPMappingTMP.cLocalIP, _sUDPMappingTMP.iListenPort);
        }

        /*
            _sUDPMappingTMP = smem.vGetUDPMappingLCNData(5);
            if( _sUDPMappingTMP.iListenPort != 0 ) {
              if (smem.SSVD04Socket.SetConnDevice(DEVICE92TC))                          //Amegids
              if ((tempmax=smem.SSVD04Socket.OpenUdpSocket(_sUDPMappingTMP.cLocalIP, _sUDPMappingTMP.iListenPort,
                                                            _sUDPMappingTMP.cRemoteIP,_sUDPMappingTMP.iSendPort)) > 0)  {
                   if (tempmax>maxport)  maxport=tempmax;
              } else printf("open SSVD04Socket-%s:%d Fail!!\n", _sUDPMappingTMP.cLocalIP, _sUDPMappingTMP.iListenPort);
            }

        //OT Pass
            _sUDPMappingTMP = smem.vGetUDPMappingLCNData(6);
            if( _sUDPMappingTMP.iListenPort != 0 ) {
              if (smem.SSVD05Socket.SetConnDevice(DEVICE92TC))                          //Amegids
              if ((tempmax=smem.SSVD05Socket.OpenUdpSocket(_sUDPMappingTMP.cLocalIP, _sUDPMappingTMP.iListenPort,
                                                            _sUDPMappingTMP.cRemoteIP,_sUDPMappingTMP.iSendPort)) > 0)  {
                   if (tempmax>maxport)  maxport=tempmax;
              } else printf("open SSVD05Socket-%s:%d Fail!!\n", _sUDPMappingTMP.cLocalIP, _sUDPMappingTMP.iListenPort);
            }
            _sUDPMappingTMP = smem.vGetUDPMappingLCNData(7);
            if( _sUDPMappingTMP.iListenPort != 0 ) {
              if (smem.SSVD06Socket.SetConnDevice(DEVICE92TC))                          //Amegids
              if ((tempmax=smem.SSVD06Socket.OpenUdpSocket(_sUDPMappingTMP.cLocalIP, _sUDPMappingTMP.iListenPort,
                                                            _sUDPMappingTMP.cRemoteIP,_sUDPMappingTMP.iSendPort)) > 0)  {
                   if (tempmax>maxport)  maxport=tempmax;
              } else printf("open SSVD06Socket-%s:%d Fail!!\n", _sUDPMappingTMP.cLocalIP, _sUDPMappingTMP.iListenPort);
            }
            _sUDPMappingTMP = smem.vGetUDPMappingLCNData(8);
            if( _sUDPMappingTMP.iListenPort != 0 ) {
              if (smem.SSVD07Socket.SetConnDevice(DEVICE92TC))                          //Amegids
              if ((tempmax=smem.SSVD07Socket.OpenUdpSocket(_sUDPMappingTMP.cLocalIP, _sUDPMappingTMP.iListenPort,
                                                            _sUDPMappingTMP.cRemoteIP,_sUDPMappingTMP.iSendPort)) > 0)  {
                   if (tempmax>maxport)  maxport=tempmax;
              } else printf("open SSVD07Socket-%s:%d Fail!!\n", _sUDPMappingTMP.cLocalIP, _sUDPMappingTMP.iListenPort);
            }
            _sUDPMappingTMP = smem.vGetUDPMappingLCNData(9);
            if( _sUDPMappingTMP.iListenPort != 0 ) {
              if (smem.SSVD08Socket.SetConnDevice(DEVICE92TC))                          //Amegids
              if ((tempmax=smem.SSVD08Socket.OpenUdpSocket(_sUDPMappingTMP.cLocalIP, _sUDPMappingTMP.iListenPort,
                                                            _sUDPMappingTMP.cRemoteIP,_sUDPMappingTMP.iSendPort)) > 0)  {
                   if (tempmax>maxport)  maxport=tempmax;
              } else printf("open SSVD08Socket-%s:%d Fail!!\n", _sUDPMappingTMP.cLocalIP, _sUDPMappingTMP.iListenPort);
            }
            _sUDPMappingTMP = smem.vGetUDPMappingLCNData(10);
            if( _sUDPMappingTMP.iListenPort != 0 ) {
              if (smem.SSVD09Socket.SetConnDevice(DEVICE92TC))                          //Amegids
              if ((tempmax=smem.SSVD09Socket.OpenUdpSocket(_sUDPMappingTMP.cLocalIP, _sUDPMappingTMP.iListenPort,
                                                            _sUDPMappingTMP.cRemoteIP,_sUDPMappingTMP.iSendPort)) > 0)  {
                   if (tempmax>maxport)  maxport=tempmax;
              } else printf("open SSVD09Socket-%s:%d Fail!!\n", _sUDPMappingTMP.cLocalIP, _sUDPMappingTMP.iListenPort);
            }
            _sUDPMappingTMP = smem.vGetUDPMappingLCNData(11);
            if( _sUDPMappingTMP.iListenPort != 0 ) {
              if (smem.SSVD10Socket.SetConnDevice(DEVICE92TC))                          //Amegids
              if ((tempmax=smem.SSVD10Socket.OpenUdpSocket(_sUDPMappingTMP.cLocalIP, _sUDPMappingTMP.iListenPort,
                                                            _sUDPMappingTMP.cRemoteIP,_sUDPMappingTMP.iSendPort)) > 0)  {
                   if (tempmax>maxport)  maxport=tempmax;
              } else printf("open SSVD10Socket-%s:%d Fail!!\n", _sUDPMappingTMP.cLocalIP, _sUDPMappingTMP.iListenPort);
            }
            _sUDPMappingTMP = smem.vGetUDPMappingLCNData(12);
            if( _sUDPMappingTMP.iListenPort != 0 ) {
              if (smem.SSVD11Socket.SetConnDevice(DEVICE92TC))                          //Amegids
              if ((tempmax=smem.SSVD11Socket.OpenUdpSocket(_sUDPMappingTMP.cLocalIP, _sUDPMappingTMP.iListenPort,
                                                            _sUDPMappingTMP.cRemoteIP,_sUDPMappingTMP.iSendPort)) > 0)  {
                   if (tempmax>maxport)  maxport=tempmax;
              } else printf("open SSVD11Socket-%s:%d Fail!!\n", _sUDPMappingTMP.cLocalIP, _sUDPMappingTMP.iListenPort);
            }
            _sUDPMappingTMP = smem.vGetUDPMappingLCNData(13);
            if( _sUDPMappingTMP.iListenPort != 0 ) {
              if (smem.SSVD12Socket.SetConnDevice(DEVICE92TC))                          //Amegids
              if ((tempmax=smem.SSVD12Socket.OpenUdpSocket(_sUDPMappingTMP.cLocalIP, _sUDPMappingTMP.iListenPort,
                                                            _sUDPMappingTMP.cRemoteIP,_sUDPMappingTMP.iSendPort)) > 0)  {
                   if (tempmax>maxport)  maxport=tempmax;
              } else printf("open SSVD12Socket-%s:%d Fail!!\n", _sUDPMappingTMP.cLocalIP, _sUDPMappingTMP.iListenPort);
            }
            _sUDPMappingTMP = smem.vGetUDPMappingLCNData(14);
            if( _sUDPMappingTMP.iListenPort != 0 ) {
              if (smem.SSVD13Socket.SetConnDevice(DEVICE92TC))                          //Amegids
              if ((tempmax=smem.SSVD13Socket.OpenUdpSocket(_sUDPMappingTMP.cLocalIP, _sUDPMappingTMP.iListenPort,
                                                            _sUDPMappingTMP.cRemoteIP,_sUDPMappingTMP.iSendPort)) > 0)  {
                   if (tempmax>maxport)  maxport=tempmax;
              } else printf("open SSVD13Socket-%s:%d Fail!!\n", _sUDPMappingTMP.cLocalIP, _sUDPMappingTMP.iListenPort);
            }
            _sUDPMappingTMP = smem.vGetUDPMappingLCNData(15);
            if( _sUDPMappingTMP.iListenPort != 0 ) {
              if (smem.SSVD14Socket.SetConnDevice(DEVICE92TC))                          //Amegids
              if ((tempmax=smem.SSVD14Socket.OpenUdpSocket(_sUDPMappingTMP.cLocalIP, _sUDPMappingTMP.iListenPort,
                                                            _sUDPMappingTMP.cRemoteIP,_sUDPMappingTMP.iSendPort)) > 0)  {
                   if (tempmax>maxport)  maxport=tempmax;
              } else printf("open SSVD14Socket-%s:%d Fail!!\n", _sUDPMappingTMP.cLocalIP, _sUDPMappingTMP.iListenPort);
            }
            _sUDPMappingTMP = smem.vGetUDPMappingLCNData(16);
            if( _sUDPMappingTMP.iListenPort != 0 ) {
              if (smem.SSVD15Socket.SetConnDevice(DEVICE92TC))                          //Amegids
              if ((tempmax=smem.SSVD15Socket.OpenUdpSocket(_sUDPMappingTMP.cLocalIP, _sUDPMappingTMP.iListenPort,
                                                            _sUDPMappingTMP.cRemoteIP,_sUDPMappingTMP.iSendPort)) > 0)  {
                   if (tempmax>maxport)  maxport=tempmax;
              } else printf("open SSVD15Socket-%s:%d Fail!!\n", _sUDPMappingTMP.cLocalIP, _sUDPMappingTMP.iListenPort);
            }
            _sUDPMappingTMP = smem.vGetUDPMappingLCNData(17);
            if( _sUDPMappingTMP.iListenPort != 0 ) {
              if (smem.SSVD16Socket.SetConnDevice(DEVICE92TC))                          //Amegids
              if ((tempmax=smem.SSVD16Socket.OpenUdpSocket(_sUDPMappingTMP.cLocalIP, _sUDPMappingTMP.iListenPort,
                                                            _sUDPMappingTMP.cRemoteIP,_sUDPMappingTMP.iSendPort)) > 0)  {
                   if (tempmax>maxport)  maxport=tempmax;
                printf("open SSVD16-%s:%d (fdValue:%d) Success!!\n", _sUDPMappingTMP.cLocalIP, _sUDPMappingTMP.iListenPort, tempmax);
              } else printf("open SSVD16Socket-%s:%d %d Fail!!\n", _sUDPMappingTMP.cLocalIP, _sUDPMappingTMP.iListenPort, tempmax);
            }
            _sUDPMappingTMP = smem.vGetUDPMappingLCNData(18);
            if( _sUDPMappingTMP.iListenPort != 0 ) {
              if (smem.SSVD17Socket.SetConnDevice(DEVICE92TC))                          //Amegids
              if ((tempmax=smem.SSVD17Socket.OpenUdpSocket(_sUDPMappingTMP.cLocalIP, _sUDPMappingTMP.iListenPort,
                                                            _sUDPMappingTMP.cRemoteIP,_sUDPMappingTMP.iSendPort)) > 0)  {
                   if (tempmax>maxport)  maxport=tempmax;
              } else printf("open SSVD17Socket-%s:%d Fail!!\n", _sUDPMappingTMP.cLocalIP, _sUDPMappingTMP.iListenPort);
            }
            _sUDPMappingTMP = smem.vGetUDPMappingLCNData(19);
            if( _sUDPMappingTMP.iListenPort != 0 ) {
              if (smem.SSVD18Socket.SetConnDevice(DEVICE92TC))                          //Amegids
              if ((tempmax=smem.SSVD18Socket.OpenUdpSocket(_sUDPMappingTMP.cLocalIP, _sUDPMappingTMP.iListenPort,
                                                            _sUDPMappingTMP.cRemoteIP,_sUDPMappingTMP.iSendPort)) > 0)  {
                   if (tempmax>maxport)  maxport=tempmax;
              } else printf("open SSVD18Socket-%s:%d Fail!!\n", _sUDPMappingTMP.cLocalIP, _sUDPMappingTMP.iListenPort);
            }
            _sUDPMappingTMP = smem.vGetUDPMappingLCNData(20);
            if( _sUDPMappingTMP.iListenPort != 0 ) {
              if (smem.SSVD19Socket.SetConnDevice(DEVICE92TC))                          //Amegids
              if ((tempmax=smem.SSVD19Socket.OpenUdpSocket(_sUDPMappingTMP.cLocalIP, _sUDPMappingTMP.iListenPort,
                                                            _sUDPMappingTMP.cRemoteIP,_sUDPMappingTMP.iSendPort)) > 0)  {
                   if (tempmax>maxport)  maxport=tempmax;
              } else printf("open SSVD19Socket-%s:%d Fail!!\n", _sUDPMappingTMP.cLocalIP, _sUDPMappingTMP.iListenPort);
            }
            _sUDPMappingTMP = smem.vGetUDPMappingLCNData(21);
            if( _sUDPMappingTMP.iListenPort != 0 ) {
              if (smem.SSVD20Socket.SetConnDevice(DEVICE92TC))                          //Amegids
              if ((tempmax=smem.SSVD20Socket.OpenUdpSocket(_sUDPMappingTMP.cLocalIP, _sUDPMappingTMP.iListenPort,
                                                            _sUDPMappingTMP.cRemoteIP,_sUDPMappingTMP.iSendPort)) > 0)  {
                   if (tempmax>maxport)  maxport=tempmax;
              } else printf("open SSVD20Socket-%s:%d Fail!!\n", _sUDPMappingTMP.cLocalIP, _sUDPMappingTMP.iListenPort);
            }
            _sUDPMappingTMP = smem.vGetUDPMappingLCNData(22);
            if( _sUDPMappingTMP.iListenPort != 0 ) {
              if (smem.SSVD21Socket.SetConnDevice(DEVICE92TC))                          //Amegids
              if ((tempmax=smem.SSVD21Socket.OpenUdpSocket(_sUDPMappingTMP.cLocalIP, _sUDPMappingTMP.iListenPort,
                                                            _sUDPMappingTMP.cRemoteIP,_sUDPMappingTMP.iSendPort)) > 0)  {
                   if (tempmax>maxport)  maxport=tempmax;
              } else printf("open SSVD21Socket-%s:%d Fail!!\n", _sUDPMappingTMP.cLocalIP, _sUDPMappingTMP.iListenPort);
            }
            _sUDPMappingTMP = smem.vGetUDPMappingLCNData(23);
            if( _sUDPMappingTMP.iListenPort != 0 ) {
              if (smem.SSVD22Socket.SetConnDevice(DEVICE92TC))                          //Amegids
              if ((tempmax=smem.SSVD22Socket.OpenUdpSocket(_sUDPMappingTMP.cLocalIP, _sUDPMappingTMP.iListenPort,
                                                            _sUDPMappingTMP.cRemoteIP,_sUDPMappingTMP.iSendPort)) > 0)  {
                   if (tempmax>maxport)  maxport=tempmax;
              } else printf("open SSVD22Socket-%s:%d Fail!!\n", _sUDPMappingTMP.cLocalIP, _sUDPMappingTMP.iListenPort);
            }
            _sUDPMappingTMP = smem.vGetUDPMappingLCNData(24);
            if( _sUDPMappingTMP.iListenPort != 0 ) {
              if (smem.SSVD23Socket.SetConnDevice(DEVICE92TC))                          //Amegids
              if ((tempmax=smem.SSVD23Socket.OpenUdpSocket(_sUDPMappingTMP.cLocalIP, _sUDPMappingTMP.iListenPort,
                                                            _sUDPMappingTMP.cRemoteIP,_sUDPMappingTMP.iSendPort)) > 0)  {
                   if (tempmax>maxport)  maxport=tempmax;
              } else printf("open SSVD23Socket-%s:%d Fail!!\n", _sUDPMappingTMP.cLocalIP, _sUDPMappingTMP.iListenPort);
            }
            _sUDPMappingTMP = smem.vGetUDPMappingLCNData(25);
            if( _sUDPMappingTMP.iListenPort != 0 ) {
              if (smem.SSVD24Socket.SetConnDevice(DEVICE92TC))                          //Amegids
              if ((tempmax=smem.SSVD24Socket.OpenUdpSocket(_sUDPMappingTMP.cLocalIP, _sUDPMappingTMP.iListenPort,
                                                            _sUDPMappingTMP.cRemoteIP,_sUDPMappingTMP.iSendPort)) > 0)  {
                   if (tempmax>maxport)  maxport=tempmax;
              } else printf("open SSVD24Socket-%s:%d Fail!!\n", _sUDPMappingTMP.cLocalIP, _sUDPMappingTMP.iListenPort);
            }
            _sUDPMappingTMP = smem.vGetUDPMappingLCNData(26);
            if( _sUDPMappingTMP.iListenPort != 0 ) {
              if (smem.SSVD25Socket.SetConnDevice(DEVICE92TC))                          //Amegids
              if ((tempmax=smem.SSVD25Socket.OpenUdpSocket(_sUDPMappingTMP.cLocalIP, _sUDPMappingTMP.iListenPort,
                                                            _sUDPMappingTMP.cRemoteIP,_sUDPMappingTMP.iSendPort)) > 0)  {
                   if (tempmax>maxport)  maxport=tempmax;
              } else printf("open SSVD25Socket-%s:%d Fail!!\n", _sUDPMappingTMP.cLocalIP, _sUDPMappingTMP.iListenPort);
            }
            _sUDPMappingTMP = smem.vGetUDPMappingLCNData(27);
            if( _sUDPMappingTMP.iListenPort != 0 ) {
              if (smem.SSVD26Socket.SetConnDevice(DEVICE92TC))                          //Amegids
              if ((tempmax=smem.SSVD26Socket.OpenUdpSocket(_sUDPMappingTMP.cLocalIP, _sUDPMappingTMP.iListenPort,
                                                            _sUDPMappingTMP.cRemoteIP,_sUDPMappingTMP.iSendPort)) > 0)  {
                   if (tempmax>maxport)  maxport=tempmax;
              } else printf("open SSVD26Socket-%s:%d Fail!!\n", _sUDPMappingTMP.cLocalIP, _sUDPMappingTMP.iListenPort);
            }
            _sUDPMappingTMP = smem.vGetUDPMappingLCNData(28);
            if( _sUDPMappingTMP.iListenPort != 0 ) {
              if (smem.SSVD27Socket.SetConnDevice(DEVICE92TC))                          //Amegids
              if ((tempmax=smem.SSVD27Socket.OpenUdpSocket(_sUDPMappingTMP.cLocalIP, _sUDPMappingTMP.iListenPort,
                                                            _sUDPMappingTMP.cRemoteIP,_sUDPMappingTMP.iSendPort)) > 0)  {
                   if (tempmax>maxport)  maxport=tempmax;
              } else printf("open SSVD27Socket-%s:%d Fail!!\n", _sUDPMappingTMP.cLocalIP, _sUDPMappingTMP.iListenPort);
            }
            _sUDPMappingTMP = smem.vGetUDPMappingLCNData(29);
            if( _sUDPMappingTMP.iListenPort != 0 ) {
              if (smem.SSVD28Socket.SetConnDevice(DEVICE92TC))                          //Amegids
              if ((tempmax=smem.SSVD28Socket.OpenUdpSocket(_sUDPMappingTMP.cLocalIP, _sUDPMappingTMP.iListenPort,
                                                            _sUDPMappingTMP.cRemoteIP,_sUDPMappingTMP.iSendPort)) > 0)  {
                   if (tempmax>maxport)  maxport=tempmax;
              } else printf("open SSVD28Socket-%s:%d Fail!!\n", _sUDPMappingTMP.cLocalIP, _sUDPMappingTMP.iListenPort);
            }
            _sUDPMappingTMP = smem.vGetUDPMappingLCNData(30);
            if( _sUDPMappingTMP.iListenPort != 0 ) {
              if (smem.SSVD29Socket.SetConnDevice(DEVICE92TC))                          //Amegids
              if ((tempmax=smem.SSVD29Socket.OpenUdpSocket(_sUDPMappingTMP.cLocalIP, _sUDPMappingTMP.iListenPort,
                                                            _sUDPMappingTMP.cRemoteIP,_sUDPMappingTMP.iSendPort)) > 0)  {
                   if (tempmax>maxport)  maxport=tempmax;
              } else printf("open SSVD29Socket-%s:%d Fail!!\n", _sUDPMappingTMP.cLocalIP, _sUDPMappingTMP.iListenPort);
            }
            _sUDPMappingTMP = smem.vGetUDPMappingLCNData(31);
            if( _sUDPMappingTMP.iListenPort != 0 ) {
              if (smem.SSVD30Socket.SetConnDevice(DEVICE92TC))                          //Amegids
              if ((tempmax=smem.SSVD30Socket.OpenUdpSocket(_sUDPMappingTMP.cLocalIP, _sUDPMappingTMP.iListenPort,
                                                            _sUDPMappingTMP.cRemoteIP,_sUDPMappingTMP.iSendPort)) > 0)  {
                   if (tempmax>maxport)  maxport=tempmax;
              } else printf("open SSVD30Socket-%s:%d Fail!!\n", _sUDPMappingTMP.cLocalIP, _sUDPMappingTMP.iListenPort);
            }
        */

        /*
            _sUDPMappingTMP = smem.vGetUDPMappingLCNData(32);
            if( _sUDPMappingTMP.iListenPort != 0 ) {
              if (smem.SSVD31Socket.SetConnDevice(DEVICE92TC))                          //Amegids
              if ((tempmax=smem.SSVD31Socket.OpenUdpSocket(_sUDPMappingTMP.cLocalIP, _sUDPMappingTMP.iListenPort,
                                                            _sUDPMappingTMP.cRemoteIP,_sUDPMappingTMP.iSendPort)) > 0)  {
                   if (tempmax>maxport)  maxport=tempmax;
              } else printf("open SSVD31Socket-%s:%d Fail!!\n", _sUDPMappingTMP.cLocalIP, _sUDPMappingTMP.iListenPort);
            }
            _sUDPMappingTMP = smem.vGetUDPMappingLCNData(33);
            if( _sUDPMappingTMP.iListenPort != 0 ) {
              if (smem.SSVD32Socket.SetConnDevice(DEVICE92TC))                          //Amegids
              if ((tempmax=smem.SSVD32Socket.OpenUdpSocket(_sUDPMappingTMP.cLocalIP, _sUDPMappingTMP.iListenPort,
                                                            _sUDPMappingTMP.cRemoteIP,_sUDPMappingTMP.iSendPort)) > 0)  {
                   if (tempmax>maxport)  maxport=tempmax;
              } else printf("open SSVD32Socket-%s:%d Fail!!\n", _sUDPMappingTMP.cLocalIP, _sUDPMappingTMP.iListenPort);
            }
        */

        maxport=maxport+1;                                                          //important


        //開啟IO 通訊埠
        //LPT1(0x378) LCD面板
//ICOPBUG
//  if (lcd240x128.GetAuthority(0x278) ) {                                       //得到kernal權限,可以控制LPT1
        if (lcd240x128.GetAuthority(0x378))                                         //得到kernal權限,可以控制LPT1
        {
            lcd240x128.GRAPHIC_MODE();                                              //將Lcd設定為Graphic畫面
            //OTMARKPRINTF  printf("LCD GetKernalAuthority Successful!!\n");
        }
        else printf("LCD GetKernalAuthority Fail!!\n");

        //Digital Input Output
        if (digitalIO.GetAuthority(0x200,0x208))
        {
            //OTMARKPRINTF  printf("MotherBoard Digital I/O GetKernalAuthority Successful!!\n");
        }
        else printf("MotherBoard Digital I/O GetKernalAuthority Failure!!\n");

        smem.ReadPEDInfo();

        screenLogo.DisplayLogo();                                                   //顯示Logo頁
        smem.vSetAdjcount(true);                                                    //開啟倒數異常防護 chou fix for NSP
        printf("Main 1 OK! \n");

        _intervalTimer.ThreadsGenerate();                                           //起Interval Timer Thread
//smem.vSaveShareMemoryDataToDOM();

        //pthread_t tidTimerHandle;                                                   //開另一個thread,控制timer
        //pthread_create (&tidTimerHandle,NULL,&timerHandle,NULL);

        /*Combo++*/

        /*
            stc.ReadAllData();
            stc.AllRed5Seconds();
            stc.Lock_to_Set_Control_Strategy(STRATEGY_TOD);
        //    stc.Lock_to_Set_Control_Strategy(STRATEGY_ALLRED);
        //    usleep(100);
        //    printf("OT set to flash OK!\n");

        //    SendRequestToKeypad();                                                      //問keypad目前面板設定
        //    if(!stc.SetDefaultLCNPhaseOrder(47, 0x00)) exit(-1);
            stc.SetDefaultLCNPhaseOrder(1,0);
            //  stc.Lock_to_Set_Control_Strategy(STRATEGY_FLASH);
        */
        /*Combo--*/
        //SendStartMsg();
        /*OTCombo0714*/
        stc.ThreadsGenerate();
        sleep(2);
        _tcpserver.tcp_thread_generate();        //OT Fix 950727   LCN0000
        //for shrink_tc// SendRequestToKeypad();                                                      //問keypad目前面板設定

        printf("Main 2 OK! \n");

        bzero(cTMP, 128);
        iFWYearTmp = smem.vGetFirmwareInfo_0FC3(0);
        iFWMonthTmp = smem.vGetFirmwareInfo_0FC3(1);
        iFWDayTmp = smem.vGetFirmwareInfo_0FC3(2);
        sprintf(cTMP, "TC Booting, FW Ver:%02d-%02d-%02d", iFWYearTmp, iFWMonthTmp, iFWDayTmp);
        smem.vWriteMsgToDOM(cTMP);
        smem.readSegmentActInfo();//arwen ++ 1000929
        xml.ActModeData();
        while (notexit)
        {

            FD_ZERO(&readfs);
            if(smem.revAPP_socket.GetPortAlreadyOpen())FD_SET(smem.revAPP_socket.Getfd(),&readfs);//for revAPP

            //join rs232 fd_set
            if (smem.centerPort.GetPortAlreadyOpen()) FD_SET(smem.centerPort.Getfd(),&readfs);

            if(iCom2Type == Com2IsTesterPort)    //Com2
            {
            }
            else if (iCom2Type == Com2IsGPSPort)
            {
                if (smem.NMEAPort.GetPortAlreadyOpen()) FD_SET(smem.NMEAPort.Getfd(),&readfs);
            }
            else if (iCom2Type == Com2IsPassingPort)
            {
                if (smem.passingPort.GetPortAlreadyOpen()) FD_SET(smem.passingPort.Getfd(),&readfs);
            }
            else if (iCom2Type == Com2IsPedestrianPushButton )    //add Arwen
            {
                if (smem.PedPushButtonPort.GetPortAlreadyOpen()) FD_SET(smem.PedPushButtonPort.Getfd(),&readfs);
            }
            else if (iCom2Type == Com2IsRedCountAndCom1IsPedPushButton)    //add Arwen
            {
                if (smem.PedPushButtonPort.GetPortAlreadyOpen()) FD_SET(smem.PedPushButtonPort.Getfd(),&readfs);
            }

//OTSS++
            //OT1000211 disable        if (smem.ssPort.GetPortAlreadyOpen()) FD_SET(smem.ssPort.Getfd(),&readfs);
//OTSS--
            //if (smem.keypadPort.GetPortAlreadyOpen()) FD_SET(smem.keypadPort.Getfd(),&readfs);
            if (smem.shrink_port_com3.GetPortAlreadyOpen()) FD_SET(smem.shrink_port_com3.Getfd(),&readfs);
            if(iCom2Type != Com2IsRedCountAndCom1IsPedPushButton) //arwen ++ Com1不為行人按鈕
            {
                if (smem.revLaneManualPort.GetPortAlreadyOpen()) FD_SET(smem.revLaneManualPort.Getfd(),&readfs);
            }

//        if (smem.rtmsPort.GetPortAlreadyOpen()) FD_SET(smem.rtmsPort.Getfd(),&readfs);
            if (smem.lightPort.GetPortAlreadyOpen()) FD_SET(smem.lightPort.Getfd(),&readfs);
            //-------------------------------------------------------------------------------------------------------------------------
            if (smem.redCountPort.GetPortAlreadyOpen()) FD_SET(smem.redCountPort.Getfd(),&readfs);

            //-------------------------------------------------------------------------------------------chou ++ greenman 2013/05/22
            if (smem.greenmanPort.GetPortAlreadyOpen()) FD_SET(smem.greenmanPort.Getfd(),&readfs);
            if (smem.NSPredCountPort.GetPortAlreadyOpen()) FD_SET(smem.NSPredCountPort.Getfd(),&readfs);        //8Port 專用
            if (smem.NMEAPort.GetPortAlreadyOpen()) FD_SET(smem.NMEAPort.Getfd(),&readfs);
            //----------------------------------------------------------------------------------------------------------------------

            //join udp fd_set

            if (smem.centerSocket.GetPortAlreadyOpen()) FD_SET(smem.centerSocket.Getfd(),&readfs);
//        if (smem.tempSocket.GetPortAlreadyOpen()) FD_SET(smem.tempSocket.Getfd(),&readfs);
            if (smem.testerSocket.GetPortAlreadyOpen()) FD_SET(smem.testerSocket.Getfd(),&readfs);

            /* OT ADD 950927 */
            if (smem.CCJDynCtlSocket.GetPortAlreadyOpen()) FD_SET(smem.CCJDynCtlSocket.Getfd(),&readfs);
            if (smem.CCJDynCtl2Socket.GetPortAlreadyOpen()) FD_SET(smem.CCJDynCtl2Socket.Getfd(),&readfs);
            if (smem.ArwenVDInfoSocket.GetPortAlreadyOpen()) FD_SET(smem.ArwenVDInfoSocket.Getfd(),&readfs);//arwen ++
            if (smem.WebSetSocket.GetPortAlreadyOpen()) FD_SET(smem.WebSetSocket.Getfd(),&readfs);//arwen ++
            /*-- OT ADD 950927 */

//OT20140224
            if (smem.BRTSetSocket.GetPortAlreadyOpen()) FD_SET(smem.BRTSetSocket.Getfd(),&readfs);
//jacky20140327
            if (smem.JsonSetSocket.GetPortAlreadyOpen()) FD_SET(smem.JsonSetSocket.Getfd(),&readfs);

            if (smem.DynCalServerInCrossSocket.GetPortAlreadyOpen()) FD_SET(smem.DynCalServerInCrossSocket.Getfd(),&readfs);
            if (smem.DynCalServerInCenterSocket.GetPortAlreadyOpen()) FD_SET(smem.DynCalServerInCenterSocket.Getfd(),&readfs);

//OT990618
            if (smem.revSyncSocket.GetPortAlreadyOpen()) FD_SET(smem.revSyncSocket.Getfd(),&readfs);

            if (smem.amegidsSocket.GetPortAlreadyOpen()) FD_SET(smem.amegidsSocket.Getfd(),&readfs);

            if (smem.SSVD01Socket.GetPortAlreadyOpen()) FD_SET(smem.SSVD01Socket.Getfd(),&readfs);
            if (smem.SSVD02Socket.GetPortAlreadyOpen()) FD_SET(smem.SSVD02Socket.Getfd(),&readfs);
            if (smem.SSVD03Socket.GetPortAlreadyOpen()) FD_SET(smem.SSVD03Socket.Getfd(),&readfs);
            if (smem.SSVD04Socket.GetPortAlreadyOpen()) FD_SET(smem.SSVD04Socket.Getfd(),&readfs);
//OT Pass
            if (smem.SSVD05Socket.GetPortAlreadyOpen()) FD_SET(smem.SSVD05Socket.Getfd(),&readfs);
            if (smem.SSVD06Socket.GetPortAlreadyOpen()) FD_SET(smem.SSVD06Socket.Getfd(),&readfs);
            if (smem.SSVD07Socket.GetPortAlreadyOpen()) FD_SET(smem.SSVD07Socket.Getfd(),&readfs);
            if (smem.SSVD08Socket.GetPortAlreadyOpen()) FD_SET(smem.SSVD08Socket.Getfd(),&readfs);
            if (smem.SSVD09Socket.GetPortAlreadyOpen()) FD_SET(smem.SSVD09Socket.Getfd(),&readfs);
            if (smem.SSVD10Socket.GetPortAlreadyOpen()) FD_SET(smem.SSVD10Socket.Getfd(),&readfs);
            if (smem.SSVD11Socket.GetPortAlreadyOpen()) FD_SET(smem.SSVD11Socket.Getfd(),&readfs);
            if (smem.SSVD12Socket.GetPortAlreadyOpen()) FD_SET(smem.SSVD12Socket.Getfd(),&readfs);
            if (smem.SSVD13Socket.GetPortAlreadyOpen()) FD_SET(smem.SSVD13Socket.Getfd(),&readfs);
            if (smem.SSVD14Socket.GetPortAlreadyOpen()) FD_SET(smem.SSVD14Socket.Getfd(),&readfs);
            if (smem.SSVD15Socket.GetPortAlreadyOpen()) FD_SET(smem.SSVD15Socket.Getfd(),&readfs);
            if (smem.SSVD16Socket.GetPortAlreadyOpen()) FD_SET(smem.SSVD16Socket.Getfd(),&readfs);
            if (smem.SSVD17Socket.GetPortAlreadyOpen()) FD_SET(smem.SSVD17Socket.Getfd(),&readfs);
            if (smem.SSVD18Socket.GetPortAlreadyOpen()) FD_SET(smem.SSVD18Socket.Getfd(),&readfs);
            if (smem.SSVD19Socket.GetPortAlreadyOpen()) FD_SET(smem.SSVD19Socket.Getfd(),&readfs);
            if (smem.SSVD20Socket.GetPortAlreadyOpen()) FD_SET(smem.SSVD20Socket.Getfd(),&readfs);
            if (smem.SSVD21Socket.GetPortAlreadyOpen()) FD_SET(smem.SSVD21Socket.Getfd(),&readfs);
            if (smem.SSVD22Socket.GetPortAlreadyOpen()) FD_SET(smem.SSVD22Socket.Getfd(),&readfs);
            if (smem.SSVD23Socket.GetPortAlreadyOpen()) FD_SET(smem.SSVD23Socket.Getfd(),&readfs);
            if (smem.SSVD24Socket.GetPortAlreadyOpen()) FD_SET(smem.SSVD24Socket.Getfd(),&readfs);
            if (smem.SSVD25Socket.GetPortAlreadyOpen()) FD_SET(smem.SSVD25Socket.Getfd(),&readfs);
            if (smem.SSVD26Socket.GetPortAlreadyOpen()) FD_SET(smem.SSVD26Socket.Getfd(),&readfs);
            if (smem.SSVD27Socket.GetPortAlreadyOpen()) FD_SET(smem.SSVD27Socket.Getfd(),&readfs);
            if (smem.SSVD28Socket.GetPortAlreadyOpen()) FD_SET(smem.SSVD28Socket.Getfd(),&readfs);
            if (smem.SSVD29Socket.GetPortAlreadyOpen()) FD_SET(smem.SSVD29Socket.Getfd(),&readfs);
            if (smem.SSVD30Socket.GetPortAlreadyOpen()) FD_SET(smem.SSVD30Socket.Getfd(),&readfs);
            /*
                    if (smem.SSVD31Socket.GetPortAlreadyOpen()) FD_SET(smem.SSVD31Socket.Getfd(),&readfs);
                    if (smem.SSVD32Socket.GetPortAlreadyOpen()) FD_SET(smem.SSVD32Socket.Getfd(),&readfs);
            */

            timeout.tv_sec=60;                                                      //timeout秒數
            timeout.tv_usec=0;                                                      //這個是毫秒,暫不使用

            rec=select(maxport,&readfs,NULL,NULL,&timeout);                         //wait block for read

            if (rec<0)                                                              //Select Error
            {

            }
            else if (rec==0)                                                        //Select Time Out
            {
                //OTMARKPRINTF  printf("Select Time Out!!\n");
            }
            else
            {

                if (smem.centerPort.GetPortAlreadyOpen())                           //port是否已開啟
                {
                    if (FD_ISSET(smem.centerPort.Getfd(),&readfs))                  //port是否有資料
                    {
                        readSelectLength=smem.centerPort.Rs232Read();               //將rs232資料q1`讀進buffer中
                        if (readSelectLength>0)
                        {
                            if(smem.vGetCommEnable() == true)
                            {
                                //OT20110526
                                smem.vSetLastGetProtocolTime();
       writeJob.WritePhysicalOut(smem.centerPort.block,readSelectLength,revAPP);//for shrinkAPP v3 packet view
                                parseAABB.ParseBlock(readSelectLength,smem.centerPort.block,smem.centerPort.messageIn,&smem.centerPort.lastPacketIndex,&smem.centerPort.maxMessageIndex);
                                parseAABB.CheckSum(&smem.centerPort.maxMessageIndex,smem.centerPort.messageIn);
                                parseAABB.DecideProtocol(&smem.centerPort.maxMessageIndex,smem.centerPort.messageIn,smem.centerPort.GetConnDevice());
                                parseAABB.vJudgeProtocolAndCheckLength(&smem.centerPort.maxMessageIndex,smem.centerPort.messageIn);
                                parseAABB.CheckReasonable(&smem.centerPort.maxMessageIndex,smem.centerPort.messageIn);
                                parseAABB.AssignLcn(&smem.centerPort.maxMessageIndex,smem.centerPort.messageIn);
                                readJob.SetInterfaceAndReadFlag(&smem.centerPort.maxMessageIndex,smem.centerPort.messageIn,cRS232);
                                readJob.vCheckIfBcastingForwardToUDP(&smem.centerPort.maxMessageIndex,smem.centerPort.messageIn);
                                readJob.CheckLcn(&smem.centerPort.maxMessageIndex,smem.centerPort.messageIn);
                                readJob.SetCenterComm(&smem.centerPort.maxMessageIndex,smem.centerPort.messageIn);
                                readJob.DoWorkByMESSAGEIN(&smem.centerPort.maxMessageIndex,smem.centerPort.messageIn);
                                parseAABB.EchoToGUI(&smem.centerPort.maxMessageIndex,smem.centerPort.messageIn,"/dev/ttyS0");
                                parseAABB.MoveLastData(&smem.centerPort.maxMessageIndex,&smem.centerPort.lastPacketIndex,smem.centerPort.messageIn);
                            }
                        }
                    }
                }

                /*       if (smem.keypadPort.GetPortAlreadyOpen())
                       {
                           if (FD_ISSET(smem.keypadPort.Getfd(),&readfs))
                           {
                               readSelectLength=smem.keypadPort.Rs232Read();
                               if (readSelectLength>0)
                               {

                //OT20111116
                                   if(smem.vGetCommEnable() == true)
                                   {
                                       parseAABB.ParseBlock(readSelectLength,smem.keypadPort.block,smem.keypadPort.messageIn,&smem.keypadPort.lastPacketIndex,&smem.keypadPort.maxMessageIndex);
                                       parseAABB.CheckSum(&smem.keypadPort.maxMessageIndex,smem.keypadPort.messageIn);
                                       parseAABB.DecideProtocol(&smem.keypadPort.maxMessageIndex,smem.keypadPort.messageIn,smem.keypadPort.GetConnDevice());
                                       parseAABB.CheckReasonable(&smem.keypadPort.maxMessageIndex,smem.keypadPort.messageIn);
                                       parseAABB.AssignLcn(&smem.keypadPort.maxMessageIndex,smem.keypadPort.messageIn);
                                       readJob.SetInterfaceAndReadFlag(&smem.keypadPort.maxMessageIndex,smem.keypadPort.messageIn,cRS232);
                                       readJob.SetKeypadComm(&smem.keypadPort.maxMessageIndex,smem.keypadPort.messageIn);
                                       readJob.DoWorkByMESSAGEIN(&smem.keypadPort.maxMessageIndex,smem.keypadPort.messageIn);
                                       parseAABB.EchoToGUI(&smem.keypadPort.maxMessageIndex,smem.keypadPort.messageIn,"/dev/ttyS2");
                                       parseAABB.MoveLastData(&smem.keypadPort.maxMessageIndex,&smem.keypadPort.lastPacketIndex,smem.keypadPort.messageIn);
                                   }
                               }
                           }
                       }
                */

                if (smem.shrink_port_com3.GetPortAlreadyOpen())
                {
                    if (FD_ISSET(smem.shrink_port_com3.Getfd(),&readfs))
                    {
                        readSelectLength=smem.shrink_port_com3.Rs232Read();
                        bzero(shrink_mes.packet, BUFFERSIZE);
                        printf("\n\n call shrink_port!!!!\n");
                        for(int i=0; i<readSelectLength; i++)
                        {
                      //      printf("%x ",smem.shrink_port_com3.block[i]);
                            if(readSelectLength<=15)shrink_mes.packet[i]=smem.shrink_port_com3.block[i];

                        }
//                        printf("\n\n");
//                        printf("readlength =%d\n",readSelectLength);



                        if (readSelectLength<16)
                        {
//                             printf("after ~\n");
//                              printf("readlength =%d\n",readSelectLength);


//printf("\n\n read block ~~~\n\n");
                            T_H_senseor.parseblockA(&shrink_mes,readSelectLength);

//                                T_H_senseor.ParseBlock(readSelectLength
//                                                       ,smem.shrink_port_com3.block,
//                                                       smem.shrink_port_com3.messageIn,
//                                                       &smem.shrink_port_com3.lastPacketIndex,
//                                                       &smem.shrink_port_com3.maxMessageIndex);
//                                T_H_senseor.CheckSum(&smem.shrink_port_com3.maxMessageIndex,smem.shrink_port_com3.messageIn);
//                                T_H_senseor.DoWorkByMESSAGEIN(&smem.shrink_port_com3.maxMessageIndex,smem.shrink_port_com3.messageIn);
//                                T_H_senseor.MoveLastData(&smem.shrink_port_com3.maxMessageIndex,&smem.shrink_port_com3.lastPacketIndex,smem.shrink_port_com3.messageIn);

                        }
                    }
                }


                if (smem.revLaneManualPort.GetPortAlreadyOpen())
                {
                    if (FD_ISSET(smem.revLaneManualPort.Getfd(),&readfs))
                    {
                        readSelectLength=smem.revLaneManualPort.Rs232Read();
                        if (readSelectLength>0)
                        {

                            printf("Get Rev:");

                            for(int ii = 0; ii < readSelectLength; ii++)
                            {
                                printf("%X ", smem.revLaneManualPort.block[ii]);
                            }
                            printf("\n");

//OT20111116
                            if(smem.vGetCommEnable() == true)
                            {

                                parseAABB.ParseBlock(readSelectLength
                                                     ,smem.revLaneManualPort.block,
                                                     smem.revLaneManualPort.messageIn,
                                                     &smem.revLaneManualPort.lastPacketIndex,
                                                     &smem.revLaneManualPort.maxMessageIndex);
                                parseAABB.CheckSum(&smem.revLaneManualPort.maxMessageIndex,smem.revLaneManualPort.messageIn);
                                parseAABB.DecideProtocol(&smem.revLaneManualPort.maxMessageIndex,smem.revLaneManualPort.messageIn,smem.revLaneManualPort.GetConnDevice());
                                parseAABB.CheckReasonable(&smem.revLaneManualPort.maxMessageIndex,smem.revLaneManualPort.messageIn);
                                parseAABB.AssignLcn(&smem.revLaneManualPort.maxMessageIndex,smem.revLaneManualPort.messageIn);
                                readJob.SetInterfaceAndReadFlag(&smem.revLaneManualPort.maxMessageIndex,smem.revLaneManualPort.messageIn,cRS232);
                                readJob.CheckLcn(&smem.revLaneManualPort.maxMessageIndex,smem.revLaneManualPort.messageIn);
                                readJob.DoWorkByMESSAGEIN(&smem.revLaneManualPort.maxMessageIndex,smem.revLaneManualPort.messageIn);
                                parseAABB.EchoToGUI(&smem.revLaneManualPort.maxMessageIndex,smem.revLaneManualPort.messageIn,"/dev/ttyS0");
                                parseAABB.MoveLastData(&smem.revLaneManualPort.maxMessageIndex,&smem.revLaneManualPort.lastPacketIndex,smem.revLaneManualPort.messageIn);
                            }

                        }
                    }
                }

                if(iCom2Type == Com2IsTesterPort)                                       // selsec 1
                {

                }
                else if (iCom2Type == Com2IsGPSPort || iCom2Type == Com2IsHOLUXGPSPort)                                    // selsec 2
                {
                    if (smem.NMEAPort.GetPortAlreadyOpen())
                    {
                        if (FD_ISSET(smem.NMEAPort.Getfd(),&readfs))
                        {
                            readSelectLength=smem.NMEAPort.Rs232Read();
                            if (readSelectLength>0)
                            {
                                if(smem.vGetCommEnable() == true)    //OT20110728
                                {
                                    parseNMEA.ParseNMEABlock(readSelectLength,smem.NMEAPort.block,smem.NMEAPort.messageIn,&smem.NMEAPort.lastPacketIndex,&smem.NMEAPort.maxMessageIndex);
                                    parseNMEA.vFindGPRMC(&smem.NMEAPort.maxMessageIndex,smem.NMEAPort.messageIn);
                                    parseNMEA.vAdjTimeByGPS(&smem.NMEAPort.maxMessageIndex,smem.NMEAPort.messageIn);
                                    parseNMEA.EchoToGUI(&smem.NMEAPort.maxMessageIndex,smem.NMEAPort.messageIn,"/dev/ttyS1");
                                    parseNMEA.MoveLastData(&smem.NMEAPort.maxMessageIndex,&smem.NMEAPort.lastPacketIndex,smem.NMEAPort.messageIn);
                                }
                            }
                        }
                    }
                }
                else if (iCom2Type == Com2IsPassingPort)                                // selsec 3
                {
                    if (smem.passingPort.GetPortAlreadyOpen())
                    {
                        if (FD_ISSET(smem.passingPort.Getfd(),&readfs))
                        {
                            readSelectLength=smem.passingPort.Rs232Read();
                            if (readSelectLength>0)
                            {
                                parseAABB.ParseBlock(readSelectLength,smem.passingPort.block,smem.passingPort.messageIn,&smem.passingPort.lastPacketIndex,&smem.passingPort.maxMessageIndex);
                                parseAABB.CheckSum(&smem.passingPort.maxMessageIndex,smem.passingPort.messageIn);
                                parseAABB.DecideProtocol(&smem.passingPort.maxMessageIndex,smem.passingPort.messageIn,smem.passingPort.GetConnDevice());
                                parseAABB.vJudgeProtocolAndCheckLength(&smem.passingPort.maxMessageIndex,smem.passingPort.messageIn);
                                parseAABB.CheckReasonable(&smem.passingPort.maxMessageIndex,smem.passingPort.messageIn);
                                parseAABB.AssignLcn(&smem.passingPort.maxMessageIndex,smem.passingPort.messageIn);
                                readJob.SetInterfaceAndReadFlag(&smem.passingPort.maxMessageIndex,smem.passingPort.messageIn,cUDP);
                                readJob.DoSendUDP_MESSAGEIN(&smem.passingPort.maxMessageIndex,smem.passingPort.messageIn);
                                parseAABB.EchoToGUI(&smem.passingPort.maxMessageIndex,smem.passingPort.messageIn,"CMS Passing");
                                parseAABB.MoveLastData(&smem.passingPort.maxMessageIndex,&smem.passingPort.lastPacketIndex,smem.passingPort.messageIn);
                            }
                        }
                    }
                }
                else if (iCom2Type == Com2IsPedestrianPushButton || iCom2Type == Com2IsRedCountAndCom1IsPedPushButton)                               // add Arwen
                {
                    if (smem.PedPushButtonPort.GetPortAlreadyOpen())
                    {
                        if (FD_ISSET(smem.PedPushButtonPort.Getfd(),&readfs))
                        {
                            readSelectLength=smem.PedPushButtonPort.Rs232Read();
                            if (readSelectLength>0)
                            {
                                smem.cPedPushButton.SetNowRs232Status(true);
                                parseAABB.ParseBlock(readSelectLength,smem.PedPushButtonPort.block,smem.PedPushButtonPort.messageIn,&smem.PedPushButtonPort.lastPacketIndex,&smem.PedPushButtonPort.maxMessageIndex);
                                parseAABB.CheckSum(&smem.PedPushButtonPort.maxMessageIndex,smem.PedPushButtonPort.messageIn);
                                parseAABB.DecideProtocol(&smem.PedPushButtonPort.maxMessageIndex,smem.PedPushButtonPort.messageIn,smem.PedPushButtonPort.GetConnDevice());
                                parseAABB.CheckReasonable(&smem.PedPushButtonPort.maxMessageIndex,smem.PedPushButtonPort.messageIn);
                                parseAABB.AssignLcn(&smem.PedPushButtonPort.maxMessageIndex,smem.PedPushButtonPort.messageIn);
                                readJob.SetInterfaceAndReadFlag(&smem.PedPushButtonPort.maxMessageIndex,smem.PedPushButtonPort.messageIn,cRS232);
                                readJob.DoWorkByMESSAGEIN(&smem.PedPushButtonPort.maxMessageIndex,smem.PedPushButtonPort.messageIn);
                                parseAABB.EchoToGUI(&smem.PedPushButtonPort.maxMessageIndex,smem.PedPushButtonPort.messageIn,"PEDESTRIAN");
                                parseAABB.MoveLastData(&smem.PedPushButtonPort.maxMessageIndex,&smem.PedPushButtonPort.lastPacketIndex,smem.PedPushButtonPort.messageIn);
                                smem.cPedPushButton.SetNowRs232Status(false);
                            }
                        }
                    }

                }

                if (smem.lightPort.GetPortAlreadyOpen())
                {
                    if (FD_ISSET(smem.lightPort.Getfd(),&readfs))
                    {
                        readSelectLength=smem.lightPort.Rs232Read();
                        if (readSelectLength>0)
                        {
                            if(smem.vGetCommEnable() == true)    //OT20110728
                            {
                                parseAABB.ParseBlock(readSelectLength,smem.lightPort.block,smem.lightPort.messageIn,&smem.lightPort.lastPacketIndex,&smem.lightPort.maxMessageIndex);
                                parseAABB.CheckSum(&smem.lightPort.maxMessageIndex,smem.lightPort.messageIn);
                                parseAABB.DecideProtocol(&smem.lightPort.maxMessageIndex,smem.lightPort.messageIn,smem.lightPort.GetConnDevice());
                                parseAABB.CheckReasonable(&smem.lightPort.maxMessageIndex,smem.lightPort.messageIn);
                                parseAABB.AssignLcn(&smem.lightPort.maxMessageIndex,smem.lightPort.messageIn);
                                readJob.SetInterfaceAndReadFlag(&smem.lightPort.maxMessageIndex,smem.lightPort.messageIn,cRS232);
                                readJob.DoWorkByMESSAGEIN(&smem.lightPort.maxMessageIndex,smem.lightPort.messageIn);
                                parseAABB.EchoToGUI(&smem.lightPort.maxMessageIndex,smem.lightPort.messageIn,"/dev/ttyS3");
                                parseAABB.MoveLastData(&smem.lightPort.maxMessageIndex,&smem.lightPort.lastPacketIndex,smem.lightPort.messageIn);

                                /*
                                                        printf("lightPort: ");
                                                        for(int ii = 0; ii < readSelectLength; ii++) {
                                                          printf("%02X ", smem.lightPort.block[ii]);
                                                        }
                                                        printf("\n");
                                */

                                smem.vSetBOOLData(TC_SIGNAL_DRIVER_UNIT, true);
                            }
                        }
                    }
                }


//OTSS++
                //OT1000211 disable
                /*
                        if (smem.ssPort.GetPortAlreadyOpen()) {
                            if (FD_ISSET(smem.ssPort.Getfd(),&readfs)) {
                                readSelectLength=smem.ssPort.Rs232Read();
                                if (readSelectLength>0) {
                                    parseSS.ParseBlock(readSelectLength,smem.ssPort.block,smem.ssPort.messageIn,&smem.ssPort.lastPacketIndex,&smem.ssPort.maxMessageIndex);
                                    parseSS.vMultiDropProcess(&smem.ssPort.maxMessageIndex,smem.ssPort.messageIn);
                                    parseSS.CheckSum(&smem.ssPort.maxMessageIndex,smem.ssPort.messageIn);
                                    readJob.SetInterfaceAndReadFlag(&smem.ssPort.maxMessageIndex,smem.ssPort.messageIn,cRS232);
                                    readJob.DoWorkByMESSAGEIN(&smem.ssPort.maxMessageIndex,smem.ssPort.messageIn);
                                    parseSS.EchoToGUI(&smem.ssPort.maxMessageIndex,smem.ssPort.messageIn,"/dev/ttyS1");
                                    parseSS.MoveLastData(&smem.ssPort.maxMessageIndex,&smem.ssPort.lastPacketIndex,smem.ssPort.messageIn);
                                }
                            }
                        }
                */

//OTSS--

                if (smem.centerSocket.GetPortAlreadyOpen())
                {
                    if (FD_ISSET(smem.centerSocket.Getfd(),&readfs))
                    {
                        readSelectLength=smem.centerSocket.UdpRead();
                        if (readSelectLength>0)
                        {
                            if(smem.vGetCommEnable() == true)
                            {
                                //OT20110526
                                smem.vSetLastGetProtocolTime();

                                parseAABB.ParseBlock(readSelectLength,smem.centerSocket.block,smem.centerSocket.messageIn,&smem.centerSocket.lastPacketIndex,&smem.centerSocket.maxMessageIndex);
                                parseAABB.CheckSum(&smem.centerSocket.maxMessageIndex,smem.centerSocket.messageIn);
                                //enable vJudgeProtocolAndCheckLength
                                parseAABB.vJudgeProtocolAndCheckLength(&smem.centerSocket.maxMessageIndex,smem.centerSocket.messageIn);
                                parseAABB.DecideProtocol(&smem.centerSocket.maxMessageIndex,smem.centerSocket.messageIn,smem.centerSocket.GetConnDevice());
                                parseAABB.CheckReasonable(&smem.centerSocket.maxMessageIndex,smem.centerSocket.messageIn);
                                parseAABB.AssignLcn(&smem.centerSocket.maxMessageIndex,smem.centerSocket.messageIn);
                                readJob.SetInterfaceAndReadFlag(&smem.centerSocket.maxMessageIndex,smem.centerSocket.messageIn,cUDP);
                                readJob.vCheckIfBcastingForwardToUDP(&smem.centerSocket.maxMessageIndex,smem.centerSocket.messageIn);
                                readJob.CheckLcn(&smem.centerSocket.maxMessageIndex,smem.centerSocket.messageIn);
                                readJob.SetCenterComm(&smem.centerSocket.maxMessageIndex,smem.centerSocket.messageIn);
                                readJob.DoWorkByMESSAGEIN(&smem.centerSocket.maxMessageIndex,smem.centerSocket.messageIn);
                                parseAABB.EchoToGUI(&smem.centerSocket.maxMessageIndex,smem.centerSocket.messageIn,"192.168.1.101:6003");
                                parseAABB.MoveLastData(&smem.centerSocket.maxMessageIndex,&smem.centerSocket.lastPacketIndex,smem.centerSocket.messageIn);
                            }
                        }
                    }
                }

                if (smem.CCJDynCtlSocket.GetPortAlreadyOpen())
                {
                    if (FD_ISSET(smem.CCJDynCtlSocket.Getfd(),&readfs))
                    {
                        readSelectLength=smem.CCJDynCtlSocket.UdpRead();
                        if (readSelectLength>0)
                        {
                            if(smem.vGetCommEnable() == true)
                            {
                                parseAABB.ParseBlock(readSelectLength,smem.CCJDynCtlSocket.block,smem.CCJDynCtlSocket.messageIn,&smem.CCJDynCtlSocket.lastPacketIndex,&smem.CCJDynCtlSocket.maxMessageIndex);
                                parseAABB.CheckSum(&smem.CCJDynCtlSocket.maxMessageIndex,smem.CCJDynCtlSocket.messageIn);
//enable vJudgeProtocolAndCheckLength
//                        parseAABB.vJudgeProtocolAndCheckLength(&smem.CCJDynCtlSocket.maxMessageIndex,smem.CCJDynCtlSocket.messageIn);
                                parseAABB.DecideProtocol(&smem.CCJDynCtlSocket.maxMessageIndex,smem.CCJDynCtlSocket.messageIn,smem.CCJDynCtlSocket.GetConnDevice());
                                parseAABB.CheckReasonable(&smem.CCJDynCtlSocket.maxMessageIndex,smem.CCJDynCtlSocket.messageIn);
                                parseAABB.AssignLcn(&smem.CCJDynCtlSocket.maxMessageIndex,smem.CCJDynCtlSocket.messageIn);
                                readJob.SetInterfaceAndReadFlag(&smem.CCJDynCtlSocket.maxMessageIndex,smem.CCJDynCtlSocket.messageIn,cUDP);
//                        readJob.vCheckIfBcastingForwardToUDP(&smem.CCJDynCtlSocket.maxMessageIndex,smem.CCJDynCtlSocket.messageIn);
                                readJob.CheckLcn(&smem.CCJDynCtlSocket.maxMessageIndex,smem.CCJDynCtlSocket.messageIn);
//enable SetCenterComm
//OT20110526                        readJob.SetCenterComm(&smem.CCJDynCtlSocket.maxMessageIndex,smem.CCJDynCtlSocket.messageIn);
                                readJob.DoWorkByMESSAGEIN(&smem.CCJDynCtlSocket.maxMessageIndex,smem.CCJDynCtlSocket.messageIn);
                                parseAABB.EchoToGUI(&smem.CCJDynCtlSocket.maxMessageIndex,smem.CCJDynCtlSocket.messageIn,"CCJDynCtlSocket");
                                parseAABB.MoveLastData(&smem.CCJDynCtlSocket.maxMessageIndex,&smem.CCJDynCtlSocket.lastPacketIndex,smem.CCJDynCtlSocket.messageIn);
                            }
                        }
                    }
                }

                if (smem.CCJDynCtl2Socket.GetPortAlreadyOpen())
                {
                    if (FD_ISSET(smem.CCJDynCtl2Socket.Getfd(),&readfs))
                    {
                        readSelectLength=smem.CCJDynCtl2Socket.UdpRead();
                        if (readSelectLength>0)
                        {
                            if(smem.vGetCommEnable() == true)
                            {
                                parseAABB.ParseBlock(readSelectLength,smem.CCJDynCtl2Socket.block,smem.CCJDynCtl2Socket.messageIn,&smem.CCJDynCtl2Socket.lastPacketIndex,&smem.CCJDynCtl2Socket.maxMessageIndex);
                                parseAABB.CheckSum(&smem.CCJDynCtl2Socket.maxMessageIndex,smem.CCJDynCtl2Socket.messageIn);
//enable vJudgeProtocolAndCheckLength
//                        parseAABB.vJudgeProtocolAndCheckLength(&smem.CCJDynCtl2Socket.maxMessageIndex,smem.CCJDynCtl2Socket.messageIn);
                                parseAABB.DecideProtocol(&smem.CCJDynCtl2Socket.maxMessageIndex,smem.CCJDynCtl2Socket.messageIn,smem.CCJDynCtl2Socket.GetConnDevice());
                                parseAABB.CheckReasonable(&smem.CCJDynCtl2Socket.maxMessageIndex,smem.CCJDynCtl2Socket.messageIn);
                                parseAABB.AssignLcn(&smem.CCJDynCtl2Socket.maxMessageIndex,smem.CCJDynCtl2Socket.messageIn);
                                readJob.SetInterfaceAndReadFlag(&smem.CCJDynCtl2Socket.maxMessageIndex,smem.CCJDynCtl2Socket.messageIn,cUDP);
//                        readJob.vCheckIfBcastingForwardToUDP(&smem.CCJDynCtl2Socket.maxMessageIndex,smem.CCJDynCtl2Socket.messageIn);
                                readJob.CheckLcn(&smem.CCJDynCtl2Socket.maxMessageIndex,smem.CCJDynCtl2Socket.messageIn);
//enable SetCenterComm
//OT20110526                        readJob.SetCenterComm(&smem.CCJDynCtl2Socket.maxMessageIndex,smem.CCJDynCtl2Socket.messageIn);
                                readJob.DoWorkByMESSAGEIN(&smem.CCJDynCtl2Socket.maxMessageIndex,smem.CCJDynCtl2Socket.messageIn);
                                parseAABB.EchoToGUI(&smem.CCJDynCtl2Socket.maxMessageIndex,smem.CCJDynCtl2Socket.messageIn,"CCJDynCtl2Socket");
                                parseAABB.MoveLastData(&smem.CCJDynCtl2Socket.maxMessageIndex,&smem.CCJDynCtl2Socket.lastPacketIndex,smem.CCJDynCtl2Socket.messageIn);
                            }
                        }
                    }
                }
                if (smem.ArwenVDInfoSocket.GetPortAlreadyOpen())  //arwen ++
                {
                    if (FD_ISSET(smem.ArwenVDInfoSocket.Getfd(),&readfs))
                    {
                        readSelectLength=smem.ArwenVDInfoSocket.UdpRead();
                        if (readSelectLength>0)
                        {
                            parseAABB.ParseBlock(readSelectLength,smem.ArwenVDInfoSocket.block,smem.ArwenVDInfoSocket.messageIn,&smem.ArwenVDInfoSocket.lastPacketIndex,&smem.ArwenVDInfoSocket.maxMessageIndex);
                            parseAABB.CheckSum(&smem.ArwenVDInfoSocket.maxMessageIndex,smem.ArwenVDInfoSocket.messageIn);
//enable vJudgeProtocolAndCheckLength
//                        parseAABB.vJudgeProtocolAndCheckLength(&smem.ArwenVDInfoSocket.maxMessageIndex,smem.ArwenVDInfoSocket.messageIn);
                            parseAABB.DecideProtocol(&smem.ArwenVDInfoSocket.maxMessageIndex,smem.ArwenVDInfoSocket.messageIn,smem.ArwenVDInfoSocket.GetConnDevice());
                            parseAABB.CheckReasonable(&smem.ArwenVDInfoSocket.maxMessageIndex,smem.ArwenVDInfoSocket.messageIn);
                            parseAABB.AssignLcn(&smem.ArwenVDInfoSocket.maxMessageIndex,smem.ArwenVDInfoSocket.messageIn);
                            readJob.SetInterfaceAndReadFlag(&smem.ArwenVDInfoSocket.maxMessageIndex,smem.ArwenVDInfoSocket.messageIn,cUDP);
//                        readJob.vCheckIfBcastingForwardToUDP(&smem.ArwenVDInfoSocket.maxMessageIndex,smem.ArwenVDInfoSocket.messageIn);
                            //readJob.CheckLcn(&smem.ArwenVDInfoSocket.maxMessageIndex,smem.ArwenVDInfoSocket.messageIn);
//enable SetCenterComm
                            readJob.SetCenterComm(&smem.ArwenVDInfoSocket.maxMessageIndex,smem.ArwenVDInfoSocket.messageIn);
                            readJob.DoWorkByMESSAGEIN(&smem.ArwenVDInfoSocket.maxMessageIndex,smem.ArwenVDInfoSocket.messageIn);
                            //parseAABB.EchoToGUI(&smem.ArwenVDInfoSocket.maxMessageIndex,smem.ArwenVDInfoSocket.messageIn,"ArwenVDInfoSocket");
                            parseAABB.MoveLastData(&smem.ArwenVDInfoSocket.maxMessageIndex,&smem.ArwenVDInfoSocket.lastPacketIndex,smem.ArwenVDInfoSocket.messageIn);
                        }
                    }
                }
                if (smem.WebSetSocket.GetPortAlreadyOpen())  //arwen ++
                {
                    if (FD_ISSET(smem.WebSetSocket.Getfd(),&readfs))
                    {
                        readSelectLength=smem.WebSetSocket.UdpRead();
                        if (readSelectLength>0)
                        {
                            parseAABB.ParseBlock(readSelectLength,smem.WebSetSocket.block,smem.WebSetSocket.messageIn,&smem.WebSetSocket.lastPacketIndex,&smem.WebSetSocket.maxMessageIndex);
                            parseAABB.CheckSum(&smem.WebSetSocket.maxMessageIndex,smem.WebSetSocket.messageIn);
//enable vJudgeProtocolAndCheckLength
//                        parseAABB.vJudgeProtocolAndCheckLength(&smem.WebSetSocket.maxMessageIndex,smem.WebSetSocket.messageIn);
                            parseAABB.DecideProtocol(&smem.WebSetSocket.maxMessageIndex,smem.WebSetSocket.messageIn,smem.WebSetSocket.GetConnDevice());
                            parseAABB.CheckReasonable(&smem.WebSetSocket.maxMessageIndex,smem.WebSetSocket.messageIn);
                            parseAABB.AssignLcn(&smem.WebSetSocket.maxMessageIndex,smem.WebSetSocket.messageIn);
                            readJob.SetInterfaceAndReadFlag(&smem.WebSetSocket.maxMessageIndex,smem.WebSetSocket.messageIn,cUDP);
//                        readJob.vCheckIfBcastingForwardToUDP(&smem.WebSetSocket.maxMessageIndex,smem.WebSetSocket.messageIn);
                            //readJob.CheckLcn(&smem.WebSetSocket.maxMessageIndex,smem.WebSetSocket.messageIn);
//enable SetCenterComm
                            //readJob.SetCenterComm(&smem.WebSetSocket.maxMessageIndex,smem.WebSetSocket.messageIn);
                            readJob.DoWorkByMESSAGEIN(&smem.WebSetSocket.maxMessageIndex,smem.WebSetSocket.messageIn);
                            parseAABB.EchoToGUI(&smem.WebSetSocket.maxMessageIndex,smem.WebSetSocket.messageIn,"WebSetSocket");
                            parseAABB.MoveLastData(&smem.WebSetSocket.maxMessageIndex,&smem.WebSetSocket.lastPacketIndex,smem.WebSetSocket.messageIn);
                        }
                    }
                }

                //OT20140224
                if (smem.BRTSetSocket.GetPortAlreadyOpen())
                {
                    if (FD_ISSET(smem.BRTSetSocket.Getfd(),&readfs))
                    {
                        readSelectLength=smem.BRTSetSocket.UdpRead();
                        if (readSelectLength>0)
                        {
                            if(smem.vGetCommEnable() == true)
                            {
                                printf("get!!\n");
                                parseAABB.ParseBlock(readSelectLength,smem.BRTSetSocket.block,smem.BRTSetSocket.messageIn,&smem.BRTSetSocket.lastPacketIndex,&smem.BRTSetSocket.maxMessageIndex);
                                parseAABB.CheckSum(&smem.BRTSetSocket.maxMessageIndex,smem.BRTSetSocket.messageIn);
//enable vJudgeProtocolAndCheckLength
//                        parseAABB.vJudgeProtocolAndCheckLength(&smem.BRTSetSocket.maxMessageIndex,smem.BRTSetSocket.messageIn);
                                parseAABB.DecideProtocol(&smem.BRTSetSocket.maxMessageIndex,smem.BRTSetSocket.messageIn,smem.BRTSetSocket.GetConnDevice());
                                parseAABB.CheckReasonable(&smem.BRTSetSocket.maxMessageIndex,smem.BRTSetSocket.messageIn);
                                parseAABB.AssignLcn(&smem.BRTSetSocket.maxMessageIndex,smem.BRTSetSocket.messageIn);
                                readJob.SetInterfaceAndReadFlag(&smem.BRTSetSocket.maxMessageIndex,smem.BRTSetSocket.messageIn,cUDP);
//                        readJob.vCheckIfBcastingForwardToUDP(&smem.BRTSetSocket.maxMessageIndex,smem.BRTSetSocket.messageIn);
                                readJob.CheckLcn(&smem.BRTSetSocket.maxMessageIndex,smem.BRTSetSocket.messageIn);
//enable SetCenterComm
//OT20110526                        readJob.SetCenterComm(&smem.BRTSetSocket.maxMessageIndex,smem.BRTSetSocket.messageIn);
                                readJob.DoWorkByMESSAGEIN(&smem.BRTSetSocket.maxMessageIndex,smem.BRTSetSocket.messageIn);
                                parseAABB.EchoToGUI(&smem.BRTSetSocket.maxMessageIndex,smem.BRTSetSocket.messageIn,"BRTSetSocket");
                                parseAABB.MoveLastData(&smem.BRTSetSocket.maxMessageIndex,&smem.BRTSetSocket.lastPacketIndex,smem.BRTSetSocket.messageIn);
                            }
                        }
                    }
                }


                /*
                            if (smem.tempSocket.GetPortAlreadyOpen()) {
                                if (FD_ISSET(smem.tempSocket.Getfd(),&readfs)) {
                                    readSelectLength=smem.tempSocket.UdpRead();
                                    if (readSelectLength>0) {
                                        parseAABB.ParseBlock(readSelectLength,smem.tempSocket.block,smem.tempSocket.messageIn,&smem.tempSocket.lastPacketIndex,&smem.tempSocket.maxMessageIndex);
                                        parseAABB.CheckSum(&smem.tempSocket.maxMessageIndex,smem.tempSocket.messageIn);
                                        parseAABB.DecideProtocol(&smem.tempSocket.maxMessageIndex,smem.tempSocket.messageIn,smem.tempSocket.GetConnDevice());
                                        parseAABB.CheckReasonable(&smem.tempSocket.maxMessageIndex,smem.tempSocket.messageIn);
                                        parseAABB.AssignLcn(&smem.tempSocket.maxMessageIndex,smem.tempSocket.messageIn);
                                        readJob.SetInterfaceAndReadFlag(&smem.tempSocket.maxMessageIndex,smem.tempSocket.messageIn,cUDP);
                                        readJob.CheckLcn(&smem.tempSocket.maxMessageIndex,smem.tempSocket.messageIn);
                                        readJob.SetCenterComm(&smem.tempSocket.maxMessageIndex,smem.tempSocket.messageIn);
                                        readJob.DoWorkByMESSAGEIN(&smem.tempSocket.maxMessageIndex,smem.tempSocket.messageIn);
                                        parseAABB.EchoToGUI(&smem.tempSocket.maxMessageIndex,smem.tempSocket.messageIn,"192.168.1.101:6789");
                                        parseAABB.MoveLastData(&smem.tempSocket.maxMessageIndex,&smem.tempSocket.lastPacketIndex,smem.tempSocket.messageIn);
                                    }
                                }
                            }
                */

                if (smem.testerSocket.GetPortAlreadyOpen())
                {
                    if (FD_ISSET(smem.testerSocket.Getfd(),&readfs))
                    {
                        readSelectLength=smem.testerSocket.UdpRead();
                        if (readSelectLength>0)
                        {
                            if(smem.vGetCommEnable() == true)
                            {
                                //OT20110526
                                smem.vSetLastGetProtocolTime();

                                parseAABB.ParseBlock(readSelectLength,smem.testerSocket.block,smem.testerSocket.messageIn,&smem.testerSocket.lastPacketIndex,&smem.testerSocket.maxMessageIndex);
                                parseAABB.CheckSum(&smem.testerSocket.maxMessageIndex,smem.testerSocket.messageIn);
                                //enable vJudgeProtocolAndCheckLength
                                parseAABB.vJudgeProtocolAndCheckLength(&smem.testerSocket.maxMessageIndex,smem.testerSocket.messageIn);
                                parseAABB.DecideProtocol(&smem.testerSocket.maxMessageIndex,smem.testerSocket.messageIn,smem.testerSocket.GetConnDevice());
                                parseAABB.CheckReasonable(&smem.testerSocket.maxMessageIndex,smem.testerSocket.messageIn);
                                parseAABB.AssignLcn(&smem.testerSocket.maxMessageIndex,smem.testerSocket.messageIn);
                                readJob.SetInterfaceAndReadFlag(&smem.testerSocket.maxMessageIndex,smem.testerSocket.messageIn,cUDP);
                                readJob.vCheckIfBcastingForwardToUDP(&smem.testerSocket.maxMessageIndex,smem.testerSocket.messageIn);
                                readJob.CheckLcn(&smem.testerSocket.maxMessageIndex,smem.testerSocket.messageIn);
                                readJob.SetCenterComm(&smem.testerSocket.maxMessageIndex,smem.testerSocket.messageIn);
                                readJob.DoWorkByMESSAGEIN(&smem.testerSocket.maxMessageIndex,smem.testerSocket.messageIn);
                                parseAABB.EchoToGUI(&smem.testerSocket.maxMessageIndex,smem.testerSocket.messageIn,"192.168.1.102:6003");
                                parseAABB.MoveLastData(&smem.testerSocket.maxMessageIndex,&smem.testerSocket.lastPacketIndex,smem.testerSocket.messageIn);
                            }
                        }
                    }
                }





                if (smem.DynCalServerInCenterSocket.GetPortAlreadyOpen())
                {
                    if (FD_ISSET(smem.DynCalServerInCenterSocket.Getfd(),&readfs))
                    {
                        readSelectLength=smem.DynCalServerInCenterSocket.UdpRead();
                        if (readSelectLength>0)
                        {
                            if(smem.vGetCommEnable() == true)
                            {
                                //OT20110526

                                smem.vSetLastGetProtocolTime();
//                      printf("rev packet\n");
                                parseAABB.ParseBlock(readSelectLength,smem.DynCalServerInCenterSocket.block,smem.DynCalServerInCenterSocket.messageIn,&smem.DynCalServerInCenterSocket.lastPacketIndex,&smem.DynCalServerInCenterSocket.maxMessageIndex);
                                parseAABB.CheckSum(&smem.DynCalServerInCenterSocket.maxMessageIndex,smem.DynCalServerInCenterSocket.messageIn);
//enable vJudgeProtocolAndCheckLength
                                parseAABB.vJudgeProtocolAndCheckLength(&smem.DynCalServerInCenterSocket.maxMessageIndex,smem.DynCalServerInCenterSocket.messageIn);
                                parseAABB.DecideProtocol(&smem.DynCalServerInCenterSocket.maxMessageIndex,smem.DynCalServerInCenterSocket.messageIn,smem.DynCalServerInCenterSocket.GetConnDevice());
                                parseAABB.CheckReasonable(&smem.DynCalServerInCenterSocket.maxMessageIndex,smem.DynCalServerInCenterSocket.messageIn);
                                parseAABB.AssignLcn(&smem.DynCalServerInCenterSocket.maxMessageIndex,smem.DynCalServerInCenterSocket.messageIn);
                                readJob.SetInterfaceAndReadFlag(&smem.DynCalServerInCenterSocket.maxMessageIndex,smem.DynCalServerInCenterSocket.messageIn,cUDP);
                                readJob.vCheckIfBcastingForwardToUDP(&smem.DynCalServerInCenterSocket.maxMessageIndex,smem.DynCalServerInCenterSocket.messageIn);
                                readJob.CheckLcn(&smem.DynCalServerInCenterSocket.maxMessageIndex,smem.DynCalServerInCenterSocket.messageIn);
//enable SetCenterComm
                                readJob.SetCenterComm(&smem.DynCalServerInCenterSocket.maxMessageIndex,smem.DynCalServerInCenterSocket.messageIn);
                                readJob.DoWorkByMESSAGEIN(&smem.DynCalServerInCenterSocket.maxMessageIndex,smem.DynCalServerInCenterSocket.messageIn);
                                parseAABB.EchoToGUI(&smem.DynCalServerInCenterSocket.maxMessageIndex,smem.DynCalServerInCenterSocket.messageIn,"DynCalServerInCenterSocket");
                                parseAABB.MoveLastData(&smem.DynCalServerInCenterSocket.maxMessageIndex,&smem.DynCalServerInCenterSocket.lastPacketIndex,smem.DynCalServerInCenterSocket.messageIn);
                            }
                        }
                    }
                }

                if (smem.DynCalServerInCrossSocket.GetPortAlreadyOpen())
                {
                    if (FD_ISSET(smem.DynCalServerInCrossSocket.Getfd(),&readfs))
                    {
                        readSelectLength=smem.DynCalServerInCrossSocket.UdpRead();
                        if (readSelectLength>0)
                        {
                            if(smem.vGetCommEnable() == true)
                            {
                                //OT20110526
                                smem.vSetLastGetProtocolTime();

                                parseAABB.ParseBlock(readSelectLength,smem.DynCalServerInCrossSocket.block,smem.DynCalServerInCrossSocket.messageIn,&smem.DynCalServerInCrossSocket.lastPacketIndex,&smem.DynCalServerInCrossSocket.maxMessageIndex);
                                parseAABB.CheckSum(&smem.DynCalServerInCrossSocket.maxMessageIndex,smem.DynCalServerInCrossSocket.messageIn);
//enable vJudgeProtocolAndCheckLength
                                parseAABB.vJudgeProtocolAndCheckLength(&smem.DynCalServerInCrossSocket.maxMessageIndex,smem.DynCalServerInCrossSocket.messageIn);
                                parseAABB.DecideProtocol(&smem.DynCalServerInCrossSocket.maxMessageIndex,smem.DynCalServerInCrossSocket.messageIn,smem.DynCalServerInCrossSocket.GetConnDevice());
                                parseAABB.CheckReasonable(&smem.DynCalServerInCrossSocket.maxMessageIndex,smem.DynCalServerInCrossSocket.messageIn);
                                parseAABB.AssignLcn(&smem.DynCalServerInCrossSocket.maxMessageIndex,smem.DynCalServerInCrossSocket.messageIn);
//                        readJob.SetInterfaceAndReadFlag(&smem.DynCalServerInCrossSocket.maxMessageIndex,smem.DynCalServerInCrossSocket.messageIn,cUDP);
                                readJob.SetInterfaceAndReadFlag(&smem.DynCalServerInCrossSocket.maxMessageIndex,smem.DynCalServerInCrossSocket.messageIn, cComingFromAmegidsCrossServer);
                                readJob.vCheckIfBcastingForwardToUDP(&smem.DynCalServerInCrossSocket.maxMessageIndex,smem.DynCalServerInCrossSocket.messageIn);
                                readJob.CheckLcn(&smem.DynCalServerInCrossSocket.maxMessageIndex,smem.DynCalServerInCrossSocket.messageIn);
//enable SetCenterComm
                                readJob.SetCenterComm(&smem.DynCalServerInCrossSocket.maxMessageIndex,smem.DynCalServerInCrossSocket.messageIn);
                                readJob.DoWorkByMESSAGEIN(&smem.DynCalServerInCrossSocket.maxMessageIndex,smem.DynCalServerInCrossSocket.messageIn);
                                parseAABB.EchoToGUI(&smem.DynCalServerInCrossSocket.maxMessageIndex,smem.DynCalServerInCrossSocket.messageIn,"DynCalServerInCrossSocket");
                                parseAABB.MoveLastData(&smem.DynCalServerInCrossSocket.maxMessageIndex,&smem.DynCalServerInCrossSocket.lastPacketIndex,smem.DynCalServerInCrossSocket.messageIn);
                            }
                        }
                    }
                }

                if (smem.revSyncSocket.GetPortAlreadyOpen())
                {
                    if (FD_ISSET(smem.revSyncSocket.Getfd(),&readfs))
                    {
                        readSelectLength=smem.revSyncSocket.UdpRead();
                        if (readSelectLength>0)
                        {

                            printf("Get Rev:");

                            for(int ii = 0; ii < readSelectLength; ii++)
                            {
                                printf("%X ", smem.revSyncSocket.block[ii]);
                            }
                            printf("\n");

                            if(smem.vGetCommEnable() == true)
                            {

                                parseAABB.ParseBlock(readSelectLength,smem.revSyncSocket.block,smem.revSyncSocket.messageIn,&smem.revSyncSocket.lastPacketIndex,&smem.revSyncSocket.maxMessageIndex);
                                parseAABB.CheckSum(&smem.revSyncSocket.maxMessageIndex,smem.revSyncSocket.messageIn);
                                parseAABB.DecideProtocol(&smem.revSyncSocket.maxMessageIndex,smem.revSyncSocket.messageIn,smem.revSyncSocket.GetConnDevice());
                                parseAABB.CheckReasonable(&smem.revSyncSocket.maxMessageIndex,smem.revSyncSocket.messageIn);
                                parseAABB.AssignLcn(&smem.revSyncSocket.maxMessageIndex,smem.revSyncSocket.messageIn);
                                readJob.SetInterfaceAndReadFlag(&smem.revSyncSocket.maxMessageIndex,smem.revSyncSocket.messageIn,cUDP);
//don't do this        readJob.CheckLcn(&smem.revSyncSocket.maxMessageIndex,smem.revSyncSocket.messageIn);
                                readJob.DoWorkByMESSAGEIN(&smem.revSyncSocket.maxMessageIndex,smem.revSyncSocket.messageIn);
                                parseAABB.EchoToGUI(&smem.revSyncSocket.maxMessageIndex,smem.revSyncSocket.messageIn,"revSyncSocket");
                                parseAABB.MoveLastData(&smem.revSyncSocket.maxMessageIndex,&smem.revSyncSocket.lastPacketIndex,smem.revSyncSocket.messageIn);
                            }
                        }
                    }
                }
                if(smem.revAPP_socket.GetPortAlreadyOpen())
                {
                    if(FD_ISSET(smem.revAPP_socket.Getfd(),&readfs))
                    {
                        readSelectLength=smem.revAPP_socket.UdpRead();

                        printf("receive messgae REVAPP~~~~~~~~~~~\n");
                        if(readSelectLength>0)
                        {
                            //printf("readSelectLength=%d\n",readSelectLength);
                            if(smem.vGetCommEnable()==true)
                            {
                                int rev_select=0;
                                revAPP_messagein=oRev_protocol_0F.revAPP_packet(readSelectLength,smem.revAPP_socket.block);
                                if(revAPP_messagein.packet[0]==0x0F)
                                {
                                    switch(revAPP_messagein.packet[1])
                                    {
                                    case (0x10):
                                        oRev_protocol_0F.vRebootIPC_0F10_revAPP(revAPP_messagein);
                                        break;
                                    case (0x4a):
                                        smem.disk.WriteNetworkSetConfirm_RevAPP(revAPP_messagein);
                                        break;
                                    case (0x18):
                                        smem.disk.dispalyLCN_NetworkIP_to_RevAPP();
                                        break;
                                    case(0xc1):
                                        oRev_protocol_0F.vReportIPCTime_0FC2_revAPP();
                                        printf("hello\n\n\n");
                                        break;
                                    case(0x01):
                                        if((revAPP_messagein.packet[2]==0x02)&&(revAPP_messagein.packet[3]=0x03))
                                            oRev_protocol_0F.check_link_revAPP();
                                        else printf("check link packet miss\n");

                                        break;
                                    default:
                                        printf("wrong revAPP 0F protocol!!\n");
                                        break;
                                    }
                                }

                                memset(revAPP_messagein.packet,0x0,sizeof(revAPP_messagein.packet));
                            }
                        }
                    }
                }


                if (smem.revSyncSocket.GetPortAlreadyOpen())
                {
                    if (FD_ISSET(smem.revSyncSocket.Getfd(),&readfs))
                    {
                        readSelectLength=smem.revSyncSocket.UdpRead();
                        if (readSelectLength>0)
                        {

                            printf("Get Rev:");

                            for(int ii = 0; ii < readSelectLength; ii++)
                            {
                                printf("%X ", smem.revSyncSocket.block[ii]);
                            }
                            printf("\n");

                            if(smem.vGetCommEnable() == true)
                            {

                                parseAABB.ParseBlock(readSelectLength,smem.revSyncSocket.block,smem.revSyncSocket.messageIn,&smem.revSyncSocket.lastPacketIndex,&smem.revSyncSocket.maxMessageIndex);
                                parseAABB.CheckSum(&smem.revSyncSocket.maxMessageIndex,smem.revSyncSocket.messageIn);
                                parseAABB.DecideProtocol(&smem.revSyncSocket.maxMessageIndex,smem.revSyncSocket.messageIn,smem.revSyncSocket.GetConnDevice());
                                parseAABB.CheckReasonable(&smem.revSyncSocket.maxMessageIndex,smem.revSyncSocket.messageIn);
                                parseAABB.AssignLcn(&smem.revSyncSocket.maxMessageIndex,smem.revSyncSocket.messageIn);
                                readJob.SetInterfaceAndReadFlag(&smem.revSyncSocket.maxMessageIndex,smem.revSyncSocket.messageIn,cUDP);
//don't do this        readJob.CheckLcn(&smem.revSyncSocket.maxMessageIndex,smem.revSyncSocket.messageIn);
                                readJob.DoWorkByMESSAGEIN(&smem.revSyncSocket.maxMessageIndex,smem.revSyncSocket.messageIn);
                                parseAABB.EchoToGUI(&smem.revSyncSocket.maxMessageIndex,smem.revSyncSocket.messageIn,"revSyncSocket");
                                parseAABB.MoveLastData(&smem.revSyncSocket.maxMessageIndex,&smem.revSyncSocket.lastPacketIndex,smem.revSyncSocket.messageIn);
                            }
                        }
                    }
                }

                if (smem.amegidsSocket.GetPortAlreadyOpen())
                {
                    if (FD_ISSET(smem.amegidsSocket.Getfd(),&readfs))
                    {
                        readSelectLength=smem.amegidsSocket.UdpRead();
                        if (readSelectLength>0)
                        {

                            printf("Get Amegids:");

                            for(int ii = 0; ii < readSelectLength; ii++)
                            {
                                printf("%X ", smem.amegidsSocket.block[ii]);
                            }
                            printf("\n");

                            if(smem.vGetCommEnable() == true)
                            {

                                parseAABB.ParseBlock(readSelectLength,smem.amegidsSocket.block,smem.amegidsSocket.messageIn,&smem.amegidsSocket.lastPacketIndex,&smem.amegidsSocket.maxMessageIndex);
                                parseAABB.CheckSum(&smem.amegidsSocket.maxMessageIndex,smem.amegidsSocket.messageIn);
                                //enable vJudgeProtocolAndCheckLength
                                parseAABB.vJudgeProtocolAndCheckLength(&smem.amegidsSocket.maxMessageIndex,smem.amegidsSocket.messageIn);
                                parseAABB.DecideProtocol(&smem.amegidsSocket.maxMessageIndex,smem.amegidsSocket.messageIn,smem.amegidsSocket.GetConnDevice());
                                parseAABB.CheckReasonable(&smem.amegidsSocket.maxMessageIndex,smem.amegidsSocket.messageIn);
                                parseAABB.AssignLcn(&smem.amegidsSocket.maxMessageIndex,smem.amegidsSocket.messageIn);
                                readJob.SetInterfaceAndReadFlag(&smem.amegidsSocket.maxMessageIndex,smem.amegidsSocket.messageIn,cUDP);
//                      readJob.vCheckIfBcastingForwardToUDP(&smem.amegidsSocket.maxMessageIndex,smem.amegidsSocket.messageIn);
                                readJob.CheckLcn(&smem.amegidsSocket.maxMessageIndex,smem.amegidsSocket.messageIn);
//                      readJob.SetCenterComm(&smem.amegidsSocket.maxMessageIndex,smem.amegidsSocket.messageIn);
                                readJob.DoWorkByMESSAGEIN(&smem.amegidsSocket.maxMessageIndex,smem.amegidsSocket.messageIn);
                                parseAABB.EchoToGUI(&smem.amegidsSocket.maxMessageIndex,smem.amegidsSocket.messageIn,"192.168.1.101:6003");
                                parseAABB.MoveLastData(&smem.amegidsSocket.maxMessageIndex,&smem.amegidsSocket.lastPacketIndex,smem.amegidsSocket.messageIn);

                            }


                        }

                    }
                }



                if (smem.SSVD01Socket.GetPortAlreadyOpen())
                {
                    if (FD_ISSET(smem.SSVD01Socket.Getfd(),&readfs))
                    {
                        readSelectLength=smem.SSVD01Socket.UdpRead();
                        if (readSelectLength>0)
                        {
                            parseAABB.ParseBlock(readSelectLength,smem.SSVD01Socket.block,smem.SSVD01Socket.messageIn,&smem.SSVD01Socket.lastPacketIndex,&smem.SSVD01Socket.maxMessageIndex);
                            parseAABB.CheckSum(&smem.SSVD01Socket.maxMessageIndex,smem.SSVD01Socket.messageIn);
                            parseAABB.DecideProtocol(&smem.SSVD01Socket.maxMessageIndex,smem.SSVD01Socket.messageIn,smem.SSVD01Socket.GetConnDevice());
                            parseAABB.vJudgeProtocolAndCheckLength(&smem.SSVD01Socket.maxMessageIndex,smem.SSVD01Socket.messageIn);
                            parseAABB.CheckReasonable(&smem.SSVD01Socket.maxMessageIndex,smem.SSVD01Socket.messageIn);
                            parseAABB.AssignLcn(&smem.SSVD01Socket.maxMessageIndex,smem.SSVD01Socket.messageIn);
                            readJob.SetInterfaceAndReadFlag(&smem.SSVD01Socket.maxMessageIndex,smem.SSVD01Socket.messageIn,cUDP);

//OT Pass
                            readJob.vDoRelayUDP_MESSAGEIN_0F8F(&smem.SSVD01Socket.maxMessageIndex, smem.SSVD01Socket.messageIn, cVD01ToCenter);
//                      readJob.DoSendUDP_MESSAGEIN(&smem.SSVD01Socket.maxMessageIndex,smem.SSVD01Socket.messageIn);
//                      readJob.DoWorkByMESSAGEIN(&smem.SSVD01Socket.maxMessageIndex,smem.centerSocket.messageIn);

                            parseAABB.EchoToGUI(&smem.SSVD01Socket.maxMessageIndex,smem.SSVD01Socket.messageIn,"SSVD01");
                            parseAABB.MoveLastData(&smem.SSVD01Socket.maxMessageIndex,&smem.SSVD01Socket.lastPacketIndex,smem.SSVD01Socket.messageIn);

//                    //OTMARKPRINTF  printf("[MESSAGE (Should not receive this port)] Forwarding SSVD01 to Center: %3X\n", smem.SSVD01Socket.block);
//                      writeJob.WritePhysicalOut(smem.SSVD01Socket.block, readSelectLength, DEVICECENTER92);
                        }
                    }
                }

                if (smem.SSVD02Socket.GetPortAlreadyOpen())
                {
                    if (FD_ISSET(smem.SSVD02Socket.Getfd(),&readfs))
                    {
                        readSelectLength=smem.SSVD02Socket.UdpRead();
                        if (readSelectLength>0)
                        {
                            parseAABB.ParseBlock(readSelectLength,smem.SSVD02Socket.block,smem.SSVD02Socket.messageIn,&smem.SSVD02Socket.lastPacketIndex,&smem.SSVD02Socket.maxMessageIndex);
                            parseAABB.CheckSum(&smem.SSVD02Socket.maxMessageIndex,smem.SSVD02Socket.messageIn);
                            parseAABB.DecideProtocol(&smem.SSVD02Socket.maxMessageIndex,smem.SSVD02Socket.messageIn,smem.SSVD02Socket.GetConnDevice());
                            parseAABB.vJudgeProtocolAndCheckLength(&smem.SSVD02Socket.maxMessageIndex,smem.SSVD02Socket.messageIn);
                            parseAABB.CheckReasonable(&smem.SSVD02Socket.maxMessageIndex,smem.SSVD02Socket.messageIn);
                            parseAABB.AssignLcn(&smem.SSVD02Socket.maxMessageIndex,smem.SSVD02Socket.messageIn);
                            readJob.SetInterfaceAndReadFlag(&smem.SSVD02Socket.maxMessageIndex,smem.SSVD02Socket.messageIn,cUDP);
                            readJob.vDoRelayUDP_MESSAGEIN_0F8F(&smem.SSVD02Socket.maxMessageIndex,smem.SSVD02Socket.messageIn, cVD02ToCenter);
                            parseAABB.EchoToGUI(&smem.SSVD02Socket.maxMessageIndex,smem.SSVD02Socket.messageIn,"SSVD02");
                            parseAABB.MoveLastData(&smem.SSVD02Socket.maxMessageIndex,&smem.SSVD02Socket.lastPacketIndex,smem.SSVD02Socket.messageIn);
                        }
                    }
                }
                if (smem.SSVD03Socket.GetPortAlreadyOpen())
                {
                    if (FD_ISSET(smem.SSVD03Socket.Getfd(),&readfs))
                    {
                        readSelectLength=smem.SSVD03Socket.UdpRead();
                        if (readSelectLength>0)
                        {
                            parseAABB.ParseBlock(readSelectLength,smem.SSVD03Socket.block,smem.SSVD03Socket.messageIn,&smem.SSVD03Socket.lastPacketIndex,&smem.SSVD03Socket.maxMessageIndex);
                            parseAABB.CheckSum(&smem.SSVD03Socket.maxMessageIndex,smem.SSVD03Socket.messageIn);
                            parseAABB.DecideProtocol(&smem.SSVD03Socket.maxMessageIndex,smem.SSVD03Socket.messageIn,smem.SSVD03Socket.GetConnDevice());
                            parseAABB.vJudgeProtocolAndCheckLength(&smem.SSVD03Socket.maxMessageIndex,smem.SSVD03Socket.messageIn);
                            parseAABB.CheckReasonable(&smem.SSVD03Socket.maxMessageIndex,smem.SSVD03Socket.messageIn);
                            parseAABB.AssignLcn(&smem.SSVD03Socket.maxMessageIndex,smem.SSVD03Socket.messageIn);
                            readJob.SetInterfaceAndReadFlag(&smem.SSVD03Socket.maxMessageIndex,smem.SSVD03Socket.messageIn,cUDP);
                            readJob.vDoRelayUDP_MESSAGEIN_0F8F(&smem.SSVD03Socket.maxMessageIndex,smem.SSVD03Socket.messageIn, cVD03ToCenter);
                            parseAABB.EchoToGUI(&smem.SSVD03Socket.maxMessageIndex,smem.SSVD03Socket.messageIn,"SSVD03");
                            parseAABB.MoveLastData(&smem.SSVD03Socket.maxMessageIndex,&smem.SSVD03Socket.lastPacketIndex,smem.SSVD03Socket.messageIn);
                        }
                    }
                }
                if (smem.SSVD04Socket.GetPortAlreadyOpen())
                {
                    if (FD_ISSET(smem.SSVD04Socket.Getfd(),&readfs))
                    {
                        readSelectLength=smem.SSVD04Socket.UdpRead();
                        if (readSelectLength>0)
                        {
                            parseAABB.ParseBlock(readSelectLength,smem.SSVD04Socket.block,smem.SSVD04Socket.messageIn,&smem.SSVD04Socket.lastPacketIndex,&smem.SSVD04Socket.maxMessageIndex);
                            parseAABB.CheckSum(&smem.SSVD04Socket.maxMessageIndex,smem.SSVD04Socket.messageIn);
                            parseAABB.DecideProtocol(&smem.SSVD04Socket.maxMessageIndex,smem.SSVD04Socket.messageIn,smem.SSVD04Socket.GetConnDevice());
                            parseAABB.vJudgeProtocolAndCheckLength(&smem.SSVD04Socket.maxMessageIndex,smem.SSVD04Socket.messageIn);
                            parseAABB.CheckReasonable(&smem.SSVD04Socket.maxMessageIndex,smem.SSVD04Socket.messageIn);
                            parseAABB.AssignLcn(&smem.SSVD04Socket.maxMessageIndex,smem.SSVD04Socket.messageIn);
                            readJob.SetInterfaceAndReadFlag(&smem.SSVD04Socket.maxMessageIndex,smem.SSVD04Socket.messageIn,cUDP);
                            readJob.vDoRelayUDP_MESSAGEIN_0F8F(&smem.SSVD04Socket.maxMessageIndex,smem.SSVD04Socket.messageIn, cVD04ToCenter);
                            parseAABB.EchoToGUI(&smem.SSVD04Socket.maxMessageIndex,smem.SSVD04Socket.messageIn,"SSVD04");
                            parseAABB.MoveLastData(&smem.SSVD04Socket.maxMessageIndex,&smem.SSVD04Socket.lastPacketIndex,smem.SSVD04Socket.messageIn);
                        }
                    }
                }
//OT Pass
                if (smem.SSVD05Socket.GetPortAlreadyOpen())
                {
                    if (FD_ISSET(smem.SSVD05Socket.Getfd(),&readfs))
                    {
                        readSelectLength=smem.SSVD05Socket.UdpRead();
                        if (readSelectLength>0)
                        {
                            parseAABB.ParseBlock(readSelectLength,smem.SSVD05Socket.block,smem.SSVD05Socket.messageIn,&smem.SSVD05Socket.lastPacketIndex,&smem.SSVD05Socket.maxMessageIndex);
                            parseAABB.CheckSum(&smem.SSVD05Socket.maxMessageIndex,smem.SSVD05Socket.messageIn);
                            parseAABB.DecideProtocol(&smem.SSVD05Socket.maxMessageIndex,smem.SSVD05Socket.messageIn,smem.SSVD05Socket.GetConnDevice());
                            parseAABB.vJudgeProtocolAndCheckLength(&smem.SSVD05Socket.maxMessageIndex,smem.SSVD05Socket.messageIn);
                            parseAABB.CheckReasonable(&smem.SSVD05Socket.maxMessageIndex,smem.SSVD05Socket.messageIn);
                            parseAABB.AssignLcn(&smem.SSVD05Socket.maxMessageIndex,smem.SSVD05Socket.messageIn);
                            readJob.SetInterfaceAndReadFlag(&smem.SSVD05Socket.maxMessageIndex,smem.SSVD05Socket.messageIn,cUDP);
                            readJob.vDoRelayUDP_MESSAGEIN_0F8F(&smem.SSVD05Socket.maxMessageIndex, smem.SSVD05Socket.messageIn, cVD05ToCenter);
                            parseAABB.EchoToGUI(&smem.SSVD05Socket.maxMessageIndex,smem.SSVD05Socket.messageIn,"SSVD05");
                            parseAABB.MoveLastData(&smem.SSVD05Socket.maxMessageIndex,&smem.SSVD05Socket.lastPacketIndex,smem.SSVD05Socket.messageIn);
                        }
                    }
                }
                if (smem.SSVD06Socket.GetPortAlreadyOpen())
                {
                    if (FD_ISSET(smem.SSVD06Socket.Getfd(),&readfs))
                    {
                        readSelectLength=smem.SSVD06Socket.UdpRead();
                        if (readSelectLength>0)
                        {
                            parseAABB.ParseBlock(readSelectLength,smem.SSVD06Socket.block,smem.SSVD06Socket.messageIn,&smem.SSVD06Socket.lastPacketIndex,&smem.SSVD06Socket.maxMessageIndex);
                            parseAABB.CheckSum(&smem.SSVD06Socket.maxMessageIndex,smem.SSVD06Socket.messageIn);
                            parseAABB.DecideProtocol(&smem.SSVD06Socket.maxMessageIndex,smem.SSVD06Socket.messageIn,smem.SSVD06Socket.GetConnDevice());
                            parseAABB.vJudgeProtocolAndCheckLength(&smem.SSVD06Socket.maxMessageIndex,smem.SSVD06Socket.messageIn);
                            parseAABB.CheckReasonable(&smem.SSVD06Socket.maxMessageIndex,smem.SSVD06Socket.messageIn);
                            parseAABB.AssignLcn(&smem.SSVD06Socket.maxMessageIndex,smem.SSVD06Socket.messageIn);
                            readJob.SetInterfaceAndReadFlag(&smem.SSVD06Socket.maxMessageIndex,smem.SSVD06Socket.messageIn,cUDP);
                            readJob.vDoRelayUDP_MESSAGEIN_0F8F(&smem.SSVD06Socket.maxMessageIndex, smem.SSVD06Socket.messageIn, cVD06ToCenter);
                            parseAABB.EchoToGUI(&smem.SSVD06Socket.maxMessageIndex,smem.SSVD06Socket.messageIn,"SSVD06");
                            parseAABB.MoveLastData(&smem.SSVD06Socket.maxMessageIndex,&smem.SSVD06Socket.lastPacketIndex,smem.SSVD06Socket.messageIn);
                        }
                    }
                }
                if (smem.SSVD07Socket.GetPortAlreadyOpen())
                {
                    if (FD_ISSET(smem.SSVD07Socket.Getfd(),&readfs))
                    {
                        readSelectLength=smem.SSVD07Socket.UdpRead();
                        if (readSelectLength>0)
                        {
                            parseAABB.ParseBlock(readSelectLength,smem.SSVD07Socket.block,smem.SSVD07Socket.messageIn,&smem.SSVD07Socket.lastPacketIndex,&smem.SSVD07Socket.maxMessageIndex);
                            parseAABB.CheckSum(&smem.SSVD07Socket.maxMessageIndex,smem.SSVD07Socket.messageIn);
                            parseAABB.DecideProtocol(&smem.SSVD07Socket.maxMessageIndex,smem.SSVD07Socket.messageIn,smem.SSVD07Socket.GetConnDevice());
                            parseAABB.vJudgeProtocolAndCheckLength(&smem.SSVD07Socket.maxMessageIndex,smem.SSVD07Socket.messageIn);
                            parseAABB.CheckReasonable(&smem.SSVD07Socket.maxMessageIndex,smem.SSVD07Socket.messageIn);
                            parseAABB.AssignLcn(&smem.SSVD07Socket.maxMessageIndex,smem.SSVD07Socket.messageIn);
                            readJob.SetInterfaceAndReadFlag(&smem.SSVD07Socket.maxMessageIndex,smem.SSVD07Socket.messageIn,cUDP);
                            readJob.vDoRelayUDP_MESSAGEIN_0F8F(&smem.SSVD07Socket.maxMessageIndex, smem.SSVD07Socket.messageIn, cVD07ToCenter);
                            parseAABB.EchoToGUI(&smem.SSVD07Socket.maxMessageIndex,smem.SSVD07Socket.messageIn,"SSVD07");
                            parseAABB.MoveLastData(&smem.SSVD07Socket.maxMessageIndex,&smem.SSVD07Socket.lastPacketIndex,smem.SSVD07Socket.messageIn);
                        }
                    }
                }
                if (smem.SSVD08Socket.GetPortAlreadyOpen())
                {
                    if (FD_ISSET(smem.SSVD08Socket.Getfd(),&readfs))
                    {
                        readSelectLength=smem.SSVD08Socket.UdpRead();
                        if (readSelectLength>0)
                        {
                            parseAABB.ParseBlock(readSelectLength,smem.SSVD08Socket.block,smem.SSVD08Socket.messageIn,&smem.SSVD08Socket.lastPacketIndex,&smem.SSVD08Socket.maxMessageIndex);
                            parseAABB.CheckSum(&smem.SSVD08Socket.maxMessageIndex,smem.SSVD08Socket.messageIn);
                            parseAABB.DecideProtocol(&smem.SSVD08Socket.maxMessageIndex,smem.SSVD08Socket.messageIn,smem.SSVD08Socket.GetConnDevice());
                            parseAABB.vJudgeProtocolAndCheckLength(&smem.SSVD08Socket.maxMessageIndex,smem.SSVD08Socket.messageIn);
                            parseAABB.CheckReasonable(&smem.SSVD08Socket.maxMessageIndex,smem.SSVD08Socket.messageIn);
                            parseAABB.AssignLcn(&smem.SSVD08Socket.maxMessageIndex,smem.SSVD08Socket.messageIn);
                            readJob.SetInterfaceAndReadFlag(&smem.SSVD08Socket.maxMessageIndex,smem.SSVD08Socket.messageIn,cUDP);
                            readJob.vDoRelayUDP_MESSAGEIN_0F8F(&smem.SSVD08Socket.maxMessageIndex, smem.SSVD08Socket.messageIn, cVD08ToCenter);
                            parseAABB.EchoToGUI(&smem.SSVD08Socket.maxMessageIndex,smem.SSVD08Socket.messageIn,"SSVD08");
                            parseAABB.MoveLastData(&smem.SSVD08Socket.maxMessageIndex,&smem.SSVD08Socket.lastPacketIndex,smem.SSVD08Socket.messageIn);
                        }
                    }
                }
                if (smem.SSVD09Socket.GetPortAlreadyOpen())
                {
                    if (FD_ISSET(smem.SSVD09Socket.Getfd(),&readfs))
                    {
                        readSelectLength=smem.SSVD09Socket.UdpRead();
                        if (readSelectLength>0)
                        {
                            parseAABB.ParseBlock(readSelectLength,smem.SSVD09Socket.block,smem.SSVD09Socket.messageIn,&smem.SSVD09Socket.lastPacketIndex,&smem.SSVD09Socket.maxMessageIndex);
                            parseAABB.CheckSum(&smem.SSVD09Socket.maxMessageIndex,smem.SSVD09Socket.messageIn);
                            parseAABB.DecideProtocol(&smem.SSVD09Socket.maxMessageIndex,smem.SSVD09Socket.messageIn,smem.SSVD09Socket.GetConnDevice());
                            parseAABB.vJudgeProtocolAndCheckLength(&smem.SSVD09Socket.maxMessageIndex,smem.SSVD09Socket.messageIn);
                            parseAABB.CheckReasonable(&smem.SSVD09Socket.maxMessageIndex,smem.SSVD09Socket.messageIn);
                            parseAABB.AssignLcn(&smem.SSVD09Socket.maxMessageIndex,smem.SSVD09Socket.messageIn);
                            readJob.SetInterfaceAndReadFlag(&smem.SSVD09Socket.maxMessageIndex,smem.SSVD09Socket.messageIn,cUDP);
                            readJob.vDoRelayUDP_MESSAGEIN_0F8F(&smem.SSVD09Socket.maxMessageIndex, smem.SSVD09Socket.messageIn, cVD09ToCenter);
                            parseAABB.EchoToGUI(&smem.SSVD09Socket.maxMessageIndex,smem.SSVD09Socket.messageIn,"SSVD09");
                            parseAABB.MoveLastData(&smem.SSVD09Socket.maxMessageIndex,&smem.SSVD09Socket.lastPacketIndex,smem.SSVD09Socket.messageIn);
                        }
                    }
                }
                if (smem.SSVD10Socket.GetPortAlreadyOpen())
                {
                    if (FD_ISSET(smem.SSVD10Socket.Getfd(),&readfs))
                    {
                        readSelectLength=smem.SSVD10Socket.UdpRead();
                        if (readSelectLength>0)
                        {
                            parseAABB.ParseBlock(readSelectLength,smem.SSVD10Socket.block,smem.SSVD10Socket.messageIn,&smem.SSVD10Socket.lastPacketIndex,&smem.SSVD10Socket.maxMessageIndex);
                            parseAABB.CheckSum(&smem.SSVD10Socket.maxMessageIndex,smem.SSVD10Socket.messageIn);
                            parseAABB.DecideProtocol(&smem.SSVD10Socket.maxMessageIndex,smem.SSVD10Socket.messageIn,smem.SSVD10Socket.GetConnDevice());
                            parseAABB.vJudgeProtocolAndCheckLength(&smem.SSVD10Socket.maxMessageIndex,smem.SSVD10Socket.messageIn);
                            parseAABB.CheckReasonable(&smem.SSVD10Socket.maxMessageIndex,smem.SSVD10Socket.messageIn);
                            parseAABB.AssignLcn(&smem.SSVD10Socket.maxMessageIndex,smem.SSVD10Socket.messageIn);
                            readJob.SetInterfaceAndReadFlag(&smem.SSVD10Socket.maxMessageIndex,smem.SSVD10Socket.messageIn,cUDP);
                            readJob.vDoRelayUDP_MESSAGEIN_0F8F(&smem.SSVD10Socket.maxMessageIndex, smem.SSVD10Socket.messageIn, cVD10ToCenter);
                            parseAABB.EchoToGUI(&smem.SSVD10Socket.maxMessageIndex,smem.SSVD10Socket.messageIn,"SSVD10");
                            parseAABB.MoveLastData(&smem.SSVD10Socket.maxMessageIndex,&smem.SSVD10Socket.lastPacketIndex,smem.SSVD10Socket.messageIn);
                        }
                    }
                }
                if (smem.SSVD11Socket.GetPortAlreadyOpen())
                {
                    if (FD_ISSET(smem.SSVD11Socket.Getfd(),&readfs))
                    {
                        readSelectLength=smem.SSVD11Socket.UdpRead();
                        if (readSelectLength>0)
                        {
                            parseAABB.ParseBlock(readSelectLength,smem.SSVD11Socket.block,smem.SSVD11Socket.messageIn,&smem.SSVD11Socket.lastPacketIndex,&smem.SSVD11Socket.maxMessageIndex);
                            parseAABB.CheckSum(&smem.SSVD11Socket.maxMessageIndex,smem.SSVD11Socket.messageIn);
                            parseAABB.DecideProtocol(&smem.SSVD11Socket.maxMessageIndex,smem.SSVD11Socket.messageIn,smem.SSVD11Socket.GetConnDevice());
                            parseAABB.vJudgeProtocolAndCheckLength(&smem.SSVD11Socket.maxMessageIndex,smem.SSVD11Socket.messageIn);
                            parseAABB.CheckReasonable(&smem.SSVD11Socket.maxMessageIndex,smem.SSVD11Socket.messageIn);
                            parseAABB.AssignLcn(&smem.SSVD11Socket.maxMessageIndex,smem.SSVD11Socket.messageIn);
                            readJob.SetInterfaceAndReadFlag(&smem.SSVD11Socket.maxMessageIndex,smem.SSVD11Socket.messageIn,cUDP);
                            readJob.vDoRelayUDP_MESSAGEIN_0F8F(&smem.SSVD11Socket.maxMessageIndex, smem.SSVD11Socket.messageIn, cVD11ToCenter);
                            parseAABB.EchoToGUI(&smem.SSVD11Socket.maxMessageIndex,smem.SSVD11Socket.messageIn,"SSVD11");
                            parseAABB.MoveLastData(&smem.SSVD11Socket.maxMessageIndex,&smem.SSVD11Socket.lastPacketIndex,smem.SSVD11Socket.messageIn);
                        }
                    }
                }
                if (smem.SSVD12Socket.GetPortAlreadyOpen())
                {
                    if (FD_ISSET(smem.SSVD12Socket.Getfd(),&readfs))
                    {
                        readSelectLength=smem.SSVD12Socket.UdpRead();
                        if (readSelectLength>0)
                        {
                            parseAABB.ParseBlock(readSelectLength,smem.SSVD12Socket.block,smem.SSVD12Socket.messageIn,&smem.SSVD12Socket.lastPacketIndex,&smem.SSVD12Socket.maxMessageIndex);
                            parseAABB.CheckSum(&smem.SSVD12Socket.maxMessageIndex,smem.SSVD12Socket.messageIn);
                            parseAABB.DecideProtocol(&smem.SSVD12Socket.maxMessageIndex,smem.SSVD12Socket.messageIn,smem.SSVD12Socket.GetConnDevice());
                            parseAABB.vJudgeProtocolAndCheckLength(&smem.SSVD12Socket.maxMessageIndex,smem.SSVD12Socket.messageIn);
                            parseAABB.CheckReasonable(&smem.SSVD12Socket.maxMessageIndex,smem.SSVD12Socket.messageIn);
                            parseAABB.AssignLcn(&smem.SSVD12Socket.maxMessageIndex,smem.SSVD12Socket.messageIn);
                            readJob.SetInterfaceAndReadFlag(&smem.SSVD12Socket.maxMessageIndex,smem.SSVD12Socket.messageIn,cUDP);
                            readJob.vDoRelayUDP_MESSAGEIN_0F8F(&smem.SSVD12Socket.maxMessageIndex, smem.SSVD12Socket.messageIn, cVD12ToCenter);
                            parseAABB.EchoToGUI(&smem.SSVD12Socket.maxMessageIndex,smem.SSVD12Socket.messageIn,"SSVD12");
                            parseAABB.MoveLastData(&smem.SSVD12Socket.maxMessageIndex,&smem.SSVD12Socket.lastPacketIndex,smem.SSVD12Socket.messageIn);
                        }
                    }
                }
                if (smem.SSVD13Socket.GetPortAlreadyOpen())
                {
                    if (FD_ISSET(smem.SSVD13Socket.Getfd(),&readfs))
                    {
                        readSelectLength=smem.SSVD13Socket.UdpRead();
                        if (readSelectLength>0)
                        {
                            parseAABB.ParseBlock(readSelectLength,smem.SSVD13Socket.block,smem.SSVD13Socket.messageIn,&smem.SSVD13Socket.lastPacketIndex,&smem.SSVD13Socket.maxMessageIndex);
                            parseAABB.CheckSum(&smem.SSVD13Socket.maxMessageIndex,smem.SSVD13Socket.messageIn);
                            parseAABB.DecideProtocol(&smem.SSVD13Socket.maxMessageIndex,smem.SSVD13Socket.messageIn,smem.SSVD13Socket.GetConnDevice());
                            parseAABB.vJudgeProtocolAndCheckLength(&smem.SSVD13Socket.maxMessageIndex,smem.SSVD13Socket.messageIn);
                            parseAABB.CheckReasonable(&smem.SSVD13Socket.maxMessageIndex,smem.SSVD13Socket.messageIn);
                            parseAABB.AssignLcn(&smem.SSVD13Socket.maxMessageIndex,smem.SSVD13Socket.messageIn);
                            readJob.SetInterfaceAndReadFlag(&smem.SSVD13Socket.maxMessageIndex,smem.SSVD13Socket.messageIn,cUDP);
                            readJob.vDoRelayUDP_MESSAGEIN_0F8F(&smem.SSVD13Socket.maxMessageIndex, smem.SSVD13Socket.messageIn, cVD13ToCenter);
                            parseAABB.EchoToGUI(&smem.SSVD13Socket.maxMessageIndex,smem.SSVD13Socket.messageIn,"SSVD13");
                            parseAABB.MoveLastData(&smem.SSVD13Socket.maxMessageIndex,&smem.SSVD13Socket.lastPacketIndex,smem.SSVD13Socket.messageIn);
                        }
                    }
                }
                if (smem.SSVD14Socket.GetPortAlreadyOpen())
                {
                    if (FD_ISSET(smem.SSVD14Socket.Getfd(),&readfs))
                    {
                        readSelectLength=smem.SSVD14Socket.UdpRead();
                        if (readSelectLength>0)
                        {
                            parseAABB.ParseBlock(readSelectLength,smem.SSVD14Socket.block,smem.SSVD14Socket.messageIn,&smem.SSVD14Socket.lastPacketIndex,&smem.SSVD14Socket.maxMessageIndex);
                            parseAABB.CheckSum(&smem.SSVD14Socket.maxMessageIndex,smem.SSVD14Socket.messageIn);
                            parseAABB.DecideProtocol(&smem.SSVD14Socket.maxMessageIndex,smem.SSVD14Socket.messageIn,smem.SSVD14Socket.GetConnDevice());
                            parseAABB.vJudgeProtocolAndCheckLength(&smem.SSVD14Socket.maxMessageIndex,smem.SSVD14Socket.messageIn);
                            parseAABB.CheckReasonable(&smem.SSVD14Socket.maxMessageIndex,smem.SSVD14Socket.messageIn);
                            parseAABB.AssignLcn(&smem.SSVD14Socket.maxMessageIndex,smem.SSVD14Socket.messageIn);
                            readJob.SetInterfaceAndReadFlag(&smem.SSVD14Socket.maxMessageIndex,smem.SSVD14Socket.messageIn,cUDP);
                            readJob.vDoRelayUDP_MESSAGEIN_0F8F(&smem.SSVD14Socket.maxMessageIndex, smem.SSVD14Socket.messageIn, cVD14ToCenter);
                            parseAABB.EchoToGUI(&smem.SSVD14Socket.maxMessageIndex,smem.SSVD14Socket.messageIn,"SSVD14");
                            parseAABB.MoveLastData(&smem.SSVD14Socket.maxMessageIndex,&smem.SSVD14Socket.lastPacketIndex,smem.SSVD14Socket.messageIn);
                        }
                    }
                }
                if (smem.SSVD15Socket.GetPortAlreadyOpen())
                {
                    if (FD_ISSET(smem.SSVD15Socket.Getfd(),&readfs))
                    {
                        readSelectLength=smem.SSVD15Socket.UdpRead();
                        if (readSelectLength>0)
                        {
                            parseAABB.ParseBlock(readSelectLength,smem.SSVD15Socket.block,smem.SSVD15Socket.messageIn,&smem.SSVD15Socket.lastPacketIndex,&smem.SSVD15Socket.maxMessageIndex);
                            parseAABB.CheckSum(&smem.SSVD15Socket.maxMessageIndex,smem.SSVD15Socket.messageIn);
                            parseAABB.DecideProtocol(&smem.SSVD15Socket.maxMessageIndex,smem.SSVD15Socket.messageIn,smem.SSVD15Socket.GetConnDevice());
                            parseAABB.vJudgeProtocolAndCheckLength(&smem.SSVD15Socket.maxMessageIndex,smem.SSVD15Socket.messageIn);
                            parseAABB.CheckReasonable(&smem.SSVD15Socket.maxMessageIndex,smem.SSVD15Socket.messageIn);
                            parseAABB.AssignLcn(&smem.SSVD15Socket.maxMessageIndex,smem.SSVD15Socket.messageIn);
                            readJob.SetInterfaceAndReadFlag(&smem.SSVD15Socket.maxMessageIndex,smem.SSVD15Socket.messageIn,cUDP);
                            readJob.vDoRelayUDP_MESSAGEIN_0F8F(&smem.SSVD15Socket.maxMessageIndex, smem.SSVD15Socket.messageIn, cVD15ToCenter);
                            parseAABB.EchoToGUI(&smem.SSVD15Socket.maxMessageIndex,smem.SSVD15Socket.messageIn,"SSVD15");
                            parseAABB.MoveLastData(&smem.SSVD15Socket.maxMessageIndex,&smem.SSVD15Socket.lastPacketIndex,smem.SSVD15Socket.messageIn);
                        }
                    }
                }
                if (smem.SSVD16Socket.GetPortAlreadyOpen())
                {
                    if (FD_ISSET(smem.SSVD16Socket.Getfd(),&readfs))
                    {
                        readSelectLength=smem.SSVD16Socket.UdpRead();
                        if (readSelectLength>0)
                        {
                            parseAABB.ParseBlock(readSelectLength,smem.SSVD16Socket.block,smem.SSVD16Socket.messageIn,&smem.SSVD16Socket.lastPacketIndex,&smem.SSVD16Socket.maxMessageIndex);
                            parseAABB.CheckSum(&smem.SSVD16Socket.maxMessageIndex,smem.SSVD16Socket.messageIn);
                            parseAABB.DecideProtocol(&smem.SSVD16Socket.maxMessageIndex,smem.SSVD16Socket.messageIn,smem.SSVD16Socket.GetConnDevice());
                            parseAABB.vJudgeProtocolAndCheckLength(&smem.SSVD16Socket.maxMessageIndex,smem.SSVD16Socket.messageIn);
                            parseAABB.CheckReasonable(&smem.SSVD16Socket.maxMessageIndex,smem.SSVD16Socket.messageIn);
                            parseAABB.AssignLcn(&smem.SSVD16Socket.maxMessageIndex,smem.SSVD16Socket.messageIn);
                            readJob.SetInterfaceAndReadFlag(&smem.SSVD16Socket.maxMessageIndex,smem.SSVD16Socket.messageIn,cUDP);
                            readJob.vDoRelayUDP_MESSAGEIN_0F8F(&smem.SSVD16Socket.maxMessageIndex, smem.SSVD16Socket.messageIn, cVD16ToCenter);
                            parseAABB.EchoToGUI(&smem.SSVD16Socket.maxMessageIndex,smem.SSVD16Socket.messageIn,"SSVD16");
                            parseAABB.MoveLastData(&smem.SSVD16Socket.maxMessageIndex,&smem.SSVD16Socket.lastPacketIndex,smem.SSVD16Socket.messageIn);
                        }
                    }
                }
                if (smem.SSVD17Socket.GetPortAlreadyOpen())
                {
                    if (FD_ISSET(smem.SSVD17Socket.Getfd(),&readfs))
                    {
                        readSelectLength=smem.SSVD17Socket.UdpRead();
                        if (readSelectLength>0)
                        {
                            parseAABB.ParseBlock(readSelectLength,smem.SSVD17Socket.block,smem.SSVD17Socket.messageIn,&smem.SSVD17Socket.lastPacketIndex,&smem.SSVD17Socket.maxMessageIndex);
                            parseAABB.CheckSum(&smem.SSVD17Socket.maxMessageIndex,smem.SSVD17Socket.messageIn);
                            parseAABB.DecideProtocol(&smem.SSVD17Socket.maxMessageIndex,smem.SSVD17Socket.messageIn,smem.SSVD17Socket.GetConnDevice());
                            parseAABB.vJudgeProtocolAndCheckLength(&smem.SSVD17Socket.maxMessageIndex,smem.SSVD17Socket.messageIn);
                            parseAABB.CheckReasonable(&smem.SSVD17Socket.maxMessageIndex,smem.SSVD17Socket.messageIn);
                            parseAABB.AssignLcn(&smem.SSVD17Socket.maxMessageIndex,smem.SSVD17Socket.messageIn);
                            readJob.SetInterfaceAndReadFlag(&smem.SSVD17Socket.maxMessageIndex,smem.SSVD17Socket.messageIn,cUDP);
                            readJob.vDoRelayUDP_MESSAGEIN_0F8F(&smem.SSVD17Socket.maxMessageIndex, smem.SSVD17Socket.messageIn, cVD17ToCenter);
                            parseAABB.EchoToGUI(&smem.SSVD17Socket.maxMessageIndex,smem.SSVD17Socket.messageIn,"SSVD17");
                            parseAABB.MoveLastData(&smem.SSVD17Socket.maxMessageIndex,&smem.SSVD17Socket.lastPacketIndex,smem.SSVD17Socket.messageIn);
                        }
                    }
                }
                if (smem.SSVD18Socket.GetPortAlreadyOpen())
                {
                    if (FD_ISSET(smem.SSVD18Socket.Getfd(),&readfs))
                    {
                        readSelectLength=smem.SSVD18Socket.UdpRead();
                        if (readSelectLength>0)
                        {
                            parseAABB.ParseBlock(readSelectLength,smem.SSVD18Socket.block,smem.SSVD18Socket.messageIn,&smem.SSVD18Socket.lastPacketIndex,&smem.SSVD18Socket.maxMessageIndex);
                            parseAABB.CheckSum(&smem.SSVD18Socket.maxMessageIndex,smem.SSVD18Socket.messageIn);
                            parseAABB.DecideProtocol(&smem.SSVD18Socket.maxMessageIndex,smem.SSVD18Socket.messageIn,smem.SSVD18Socket.GetConnDevice());
                            parseAABB.vJudgeProtocolAndCheckLength(&smem.SSVD18Socket.maxMessageIndex,smem.SSVD18Socket.messageIn);
                            parseAABB.CheckReasonable(&smem.SSVD18Socket.maxMessageIndex,smem.SSVD18Socket.messageIn);
                            parseAABB.AssignLcn(&smem.SSVD18Socket.maxMessageIndex,smem.SSVD18Socket.messageIn);
                            readJob.SetInterfaceAndReadFlag(&smem.SSVD18Socket.maxMessageIndex,smem.SSVD18Socket.messageIn,cUDP);
                            readJob.vDoRelayUDP_MESSAGEIN_0F8F(&smem.SSVD18Socket.maxMessageIndex, smem.SSVD18Socket.messageIn, cVD18ToCenter);
                            parseAABB.EchoToGUI(&smem.SSVD18Socket.maxMessageIndex,smem.SSVD18Socket.messageIn,"SSVD18");
                            parseAABB.MoveLastData(&smem.SSVD18Socket.maxMessageIndex,&smem.SSVD18Socket.lastPacketIndex,smem.SSVD18Socket.messageIn);
                        }
                    }
                }
                if (smem.SSVD19Socket.GetPortAlreadyOpen())
                {
                    if (FD_ISSET(smem.SSVD19Socket.Getfd(),&readfs))
                    {
                        readSelectLength=smem.SSVD19Socket.UdpRead();
                        if (readSelectLength>0)
                        {
                            parseAABB.ParseBlock(readSelectLength,smem.SSVD19Socket.block,smem.SSVD19Socket.messageIn,&smem.SSVD19Socket.lastPacketIndex,&smem.SSVD19Socket.maxMessageIndex);
                            parseAABB.CheckSum(&smem.SSVD19Socket.maxMessageIndex,smem.SSVD19Socket.messageIn);
                            parseAABB.DecideProtocol(&smem.SSVD19Socket.maxMessageIndex,smem.SSVD19Socket.messageIn,smem.SSVD19Socket.GetConnDevice());
                            parseAABB.vJudgeProtocolAndCheckLength(&smem.SSVD19Socket.maxMessageIndex,smem.SSVD19Socket.messageIn);
                            parseAABB.CheckReasonable(&smem.SSVD19Socket.maxMessageIndex,smem.SSVD19Socket.messageIn);
                            parseAABB.AssignLcn(&smem.SSVD19Socket.maxMessageIndex,smem.SSVD19Socket.messageIn);
                            readJob.SetInterfaceAndReadFlag(&smem.SSVD19Socket.maxMessageIndex,smem.SSVD19Socket.messageIn,cUDP);
                            readJob.vDoRelayUDP_MESSAGEIN_0F8F(&smem.SSVD19Socket.maxMessageIndex, smem.SSVD19Socket.messageIn, cVD19ToCenter);
                            parseAABB.EchoToGUI(&smem.SSVD19Socket.maxMessageIndex,smem.SSVD19Socket.messageIn,"SSVD19");
                            parseAABB.MoveLastData(&smem.SSVD19Socket.maxMessageIndex,&smem.SSVD19Socket.lastPacketIndex,smem.SSVD19Socket.messageIn);
                        }
                    }
                }
                if (smem.SSVD20Socket.GetPortAlreadyOpen())
                {
                    if (FD_ISSET(smem.SSVD20Socket.Getfd(),&readfs))
                    {
                        readSelectLength=smem.SSVD20Socket.UdpRead();
                        if (readSelectLength>0)
                        {
                            parseAABB.ParseBlock(readSelectLength,smem.SSVD20Socket.block,smem.SSVD20Socket.messageIn,&smem.SSVD20Socket.lastPacketIndex,&smem.SSVD20Socket.maxMessageIndex);
                            parseAABB.CheckSum(&smem.SSVD20Socket.maxMessageIndex,smem.SSVD20Socket.messageIn);
                            parseAABB.DecideProtocol(&smem.SSVD20Socket.maxMessageIndex,smem.SSVD20Socket.messageIn,smem.SSVD20Socket.GetConnDevice());
                            parseAABB.vJudgeProtocolAndCheckLength(&smem.SSVD20Socket.maxMessageIndex,smem.SSVD20Socket.messageIn);
                            parseAABB.CheckReasonable(&smem.SSVD20Socket.maxMessageIndex,smem.SSVD20Socket.messageIn);
                            parseAABB.AssignLcn(&smem.SSVD20Socket.maxMessageIndex,smem.SSVD20Socket.messageIn);
                            readJob.SetInterfaceAndReadFlag(&smem.SSVD20Socket.maxMessageIndex,smem.SSVD20Socket.messageIn,cUDP);
                            readJob.vDoRelayUDP_MESSAGEIN_0F8F(&smem.SSVD20Socket.maxMessageIndex, smem.SSVD20Socket.messageIn, cVD20ToCenter);
                            parseAABB.EchoToGUI(&smem.SSVD20Socket.maxMessageIndex,smem.SSVD20Socket.messageIn,"SSVD20");
                            parseAABB.MoveLastData(&smem.SSVD20Socket.maxMessageIndex,&smem.SSVD20Socket.lastPacketIndex,smem.SSVD20Socket.messageIn);
                        }
                    }
                }
                if (smem.SSVD21Socket.GetPortAlreadyOpen())
                {
                    if (FD_ISSET(smem.SSVD21Socket.Getfd(),&readfs))
                    {
                        readSelectLength=smem.SSVD21Socket.UdpRead();
                        if (readSelectLength>0)
                        {
                            parseAABB.ParseBlock(readSelectLength,smem.SSVD21Socket.block,smem.SSVD21Socket.messageIn,&smem.SSVD21Socket.lastPacketIndex,&smem.SSVD21Socket.maxMessageIndex);
                            parseAABB.CheckSum(&smem.SSVD21Socket.maxMessageIndex,smem.SSVD21Socket.messageIn);
                            parseAABB.DecideProtocol(&smem.SSVD21Socket.maxMessageIndex,smem.SSVD21Socket.messageIn,smem.SSVD21Socket.GetConnDevice());
                            parseAABB.vJudgeProtocolAndCheckLength(&smem.SSVD21Socket.maxMessageIndex,smem.SSVD21Socket.messageIn);
                            parseAABB.CheckReasonable(&smem.SSVD21Socket.maxMessageIndex,smem.SSVD21Socket.messageIn);
                            parseAABB.AssignLcn(&smem.SSVD21Socket.maxMessageIndex,smem.SSVD21Socket.messageIn);
                            readJob.SetInterfaceAndReadFlag(&smem.SSVD21Socket.maxMessageIndex,smem.SSVD21Socket.messageIn,cUDP);
                            readJob.vDoRelayUDP_MESSAGEIN_0F8F(&smem.SSVD21Socket.maxMessageIndex, smem.SSVD21Socket.messageIn, cVD21ToCenter);
                            parseAABB.EchoToGUI(&smem.SSVD21Socket.maxMessageIndex,smem.SSVD21Socket.messageIn,"SSVD21");
                            parseAABB.MoveLastData(&smem.SSVD21Socket.maxMessageIndex,&smem.SSVD21Socket.lastPacketIndex,smem.SSVD21Socket.messageIn);
                        }
                    }
                }
                if (smem.SSVD22Socket.GetPortAlreadyOpen())
                {
                    if (FD_ISSET(smem.SSVD22Socket.Getfd(),&readfs))
                    {
                        readSelectLength=smem.SSVD22Socket.UdpRead();
                        if (readSelectLength>0)
                        {
                            parseAABB.ParseBlock(readSelectLength,smem.SSVD22Socket.block,smem.SSVD22Socket.messageIn,&smem.SSVD22Socket.lastPacketIndex,&smem.SSVD22Socket.maxMessageIndex);
                            parseAABB.CheckSum(&smem.SSVD22Socket.maxMessageIndex,smem.SSVD22Socket.messageIn);
                            parseAABB.DecideProtocol(&smem.SSVD22Socket.maxMessageIndex,smem.SSVD22Socket.messageIn,smem.SSVD22Socket.GetConnDevice());
                            parseAABB.vJudgeProtocolAndCheckLength(&smem.SSVD22Socket.maxMessageIndex,smem.SSVD22Socket.messageIn);
                            parseAABB.CheckReasonable(&smem.SSVD22Socket.maxMessageIndex,smem.SSVD22Socket.messageIn);
                            parseAABB.AssignLcn(&smem.SSVD22Socket.maxMessageIndex,smem.SSVD22Socket.messageIn);
                            readJob.SetInterfaceAndReadFlag(&smem.SSVD22Socket.maxMessageIndex,smem.SSVD22Socket.messageIn,cUDP);
                            readJob.vDoRelayUDP_MESSAGEIN_0F8F(&smem.SSVD22Socket.maxMessageIndex, smem.SSVD22Socket.messageIn, cVD22ToCenter);
                            parseAABB.EchoToGUI(&smem.SSVD22Socket.maxMessageIndex,smem.SSVD22Socket.messageIn,"SSVD22");
                            parseAABB.MoveLastData(&smem.SSVD22Socket.maxMessageIndex,&smem.SSVD22Socket.lastPacketIndex,smem.SSVD22Socket.messageIn);
                        }
                    }
                }
                if (smem.SSVD23Socket.GetPortAlreadyOpen())
                {
                    if (FD_ISSET(smem.SSVD23Socket.Getfd(),&readfs))
                    {
                        readSelectLength=smem.SSVD23Socket.UdpRead();
                        if (readSelectLength>0)
                        {
                            parseAABB.ParseBlock(readSelectLength,smem.SSVD23Socket.block,smem.SSVD23Socket.messageIn,&smem.SSVD23Socket.lastPacketIndex,&smem.SSVD23Socket.maxMessageIndex);
                            parseAABB.CheckSum(&smem.SSVD23Socket.maxMessageIndex,smem.SSVD23Socket.messageIn);
                            parseAABB.DecideProtocol(&smem.SSVD23Socket.maxMessageIndex,smem.SSVD23Socket.messageIn,smem.SSVD23Socket.GetConnDevice());
                            parseAABB.vJudgeProtocolAndCheckLength(&smem.SSVD23Socket.maxMessageIndex,smem.SSVD23Socket.messageIn);
                            parseAABB.CheckReasonable(&smem.SSVD23Socket.maxMessageIndex,smem.SSVD23Socket.messageIn);
                            parseAABB.AssignLcn(&smem.SSVD23Socket.maxMessageIndex,smem.SSVD23Socket.messageIn);
                            readJob.SetInterfaceAndReadFlag(&smem.SSVD23Socket.maxMessageIndex,smem.SSVD23Socket.messageIn,cUDP);
                            readJob.vDoRelayUDP_MESSAGEIN_0F8F(&smem.SSVD23Socket.maxMessageIndex, smem.SSVD23Socket.messageIn, cVD23ToCenter);
                            parseAABB.EchoToGUI(&smem.SSVD23Socket.maxMessageIndex,smem.SSVD23Socket.messageIn,"SSVD23");
                            parseAABB.MoveLastData(&smem.SSVD23Socket.maxMessageIndex,&smem.SSVD23Socket.lastPacketIndex,smem.SSVD23Socket.messageIn);
                        }
                    }
                }
                if (smem.SSVD24Socket.GetPortAlreadyOpen())
                {
                    if (FD_ISSET(smem.SSVD24Socket.Getfd(),&readfs))
                    {
                        readSelectLength=smem.SSVD24Socket.UdpRead();
                        if (readSelectLength>0)
                        {
                            parseAABB.ParseBlock(readSelectLength,smem.SSVD24Socket.block,smem.SSVD24Socket.messageIn,&smem.SSVD24Socket.lastPacketIndex,&smem.SSVD24Socket.maxMessageIndex);
                            parseAABB.CheckSum(&smem.SSVD24Socket.maxMessageIndex,smem.SSVD24Socket.messageIn);
                            parseAABB.DecideProtocol(&smem.SSVD24Socket.maxMessageIndex,smem.SSVD24Socket.messageIn,smem.SSVD24Socket.GetConnDevice());
                            parseAABB.vJudgeProtocolAndCheckLength(&smem.SSVD24Socket.maxMessageIndex,smem.SSVD24Socket.messageIn);
                            parseAABB.CheckReasonable(&smem.SSVD24Socket.maxMessageIndex,smem.SSVD24Socket.messageIn);
                            parseAABB.AssignLcn(&smem.SSVD24Socket.maxMessageIndex,smem.SSVD24Socket.messageIn);
                            readJob.SetInterfaceAndReadFlag(&smem.SSVD24Socket.maxMessageIndex,smem.SSVD24Socket.messageIn,cUDP);
                            readJob.vDoRelayUDP_MESSAGEIN_0F8F(&smem.SSVD24Socket.maxMessageIndex, smem.SSVD24Socket.messageIn, cVD24ToCenter);
                            parseAABB.EchoToGUI(&smem.SSVD24Socket.maxMessageIndex,smem.SSVD24Socket.messageIn,"SSVD24");
                            parseAABB.MoveLastData(&smem.SSVD24Socket.maxMessageIndex,&smem.SSVD24Socket.lastPacketIndex,smem.SSVD24Socket.messageIn);
                        }
                    }
                }
                if (smem.SSVD25Socket.GetPortAlreadyOpen())
                {
                    if (FD_ISSET(smem.SSVD25Socket.Getfd(),&readfs))
                    {
                        readSelectLength=smem.SSVD25Socket.UdpRead();
                        if (readSelectLength>0)
                        {
                            parseAABB.ParseBlock(readSelectLength,smem.SSVD25Socket.block,smem.SSVD25Socket.messageIn,&smem.SSVD25Socket.lastPacketIndex,&smem.SSVD25Socket.maxMessageIndex);
                            parseAABB.CheckSum(&smem.SSVD25Socket.maxMessageIndex,smem.SSVD25Socket.messageIn);
                            parseAABB.DecideProtocol(&smem.SSVD25Socket.maxMessageIndex,smem.SSVD25Socket.messageIn,smem.SSVD25Socket.GetConnDevice());
                            parseAABB.vJudgeProtocolAndCheckLength(&smem.SSVD25Socket.maxMessageIndex,smem.SSVD25Socket.messageIn);
                            parseAABB.CheckReasonable(&smem.SSVD25Socket.maxMessageIndex,smem.SSVD25Socket.messageIn);
                            parseAABB.AssignLcn(&smem.SSVD25Socket.maxMessageIndex,smem.SSVD25Socket.messageIn);
                            readJob.SetInterfaceAndReadFlag(&smem.SSVD25Socket.maxMessageIndex,smem.SSVD25Socket.messageIn,cUDP);
                            readJob.vDoRelayUDP_MESSAGEIN_0F8F(&smem.SSVD25Socket.maxMessageIndex, smem.SSVD25Socket.messageIn, cVD25ToCenter);
                            parseAABB.EchoToGUI(&smem.SSVD25Socket.maxMessageIndex,smem.SSVD25Socket.messageIn,"SSVD25");
                            parseAABB.MoveLastData(&smem.SSVD25Socket.maxMessageIndex,&smem.SSVD25Socket.lastPacketIndex,smem.SSVD25Socket.messageIn);
                        }
                    }
                }
                if (smem.SSVD26Socket.GetPortAlreadyOpen())
                {
                    if (FD_ISSET(smem.SSVD26Socket.Getfd(),&readfs))
                    {
                        readSelectLength=smem.SSVD26Socket.UdpRead();
                        if (readSelectLength>0)
                        {
                            parseAABB.ParseBlock(readSelectLength,smem.SSVD26Socket.block,smem.SSVD26Socket.messageIn,&smem.SSVD26Socket.lastPacketIndex,&smem.SSVD26Socket.maxMessageIndex);
                            parseAABB.CheckSum(&smem.SSVD26Socket.maxMessageIndex,smem.SSVD26Socket.messageIn);
                            parseAABB.DecideProtocol(&smem.SSVD26Socket.maxMessageIndex,smem.SSVD26Socket.messageIn,smem.SSVD26Socket.GetConnDevice());
                            parseAABB.vJudgeProtocolAndCheckLength(&smem.SSVD26Socket.maxMessageIndex,smem.SSVD26Socket.messageIn);
                            parseAABB.CheckReasonable(&smem.SSVD26Socket.maxMessageIndex,smem.SSVD26Socket.messageIn);
                            parseAABB.AssignLcn(&smem.SSVD26Socket.maxMessageIndex,smem.SSVD26Socket.messageIn);
                            readJob.SetInterfaceAndReadFlag(&smem.SSVD26Socket.maxMessageIndex,smem.SSVD26Socket.messageIn,cUDP);
                            readJob.vDoRelayUDP_MESSAGEIN_0F8F(&smem.SSVD26Socket.maxMessageIndex, smem.SSVD26Socket.messageIn, cVD26ToCenter);
                            parseAABB.EchoToGUI(&smem.SSVD26Socket.maxMessageIndex,smem.SSVD26Socket.messageIn,"SSVD26");
                            parseAABB.MoveLastData(&smem.SSVD26Socket.maxMessageIndex,&smem.SSVD26Socket.lastPacketIndex,smem.SSVD26Socket.messageIn);
                        }
                    }
                }
                if (smem.SSVD27Socket.GetPortAlreadyOpen())
                {
                    if (FD_ISSET(smem.SSVD27Socket.Getfd(),&readfs))
                    {
                        readSelectLength=smem.SSVD27Socket.UdpRead();
                        if (readSelectLength>0)
                        {
                            parseAABB.ParseBlock(readSelectLength,smem.SSVD27Socket.block,smem.SSVD27Socket.messageIn,&smem.SSVD27Socket.lastPacketIndex,&smem.SSVD27Socket.maxMessageIndex);
                            parseAABB.CheckSum(&smem.SSVD27Socket.maxMessageIndex,smem.SSVD27Socket.messageIn);
                            parseAABB.DecideProtocol(&smem.SSVD27Socket.maxMessageIndex,smem.SSVD27Socket.messageIn,smem.SSVD27Socket.GetConnDevice());
                            parseAABB.vJudgeProtocolAndCheckLength(&smem.SSVD27Socket.maxMessageIndex,smem.SSVD27Socket.messageIn);
                            parseAABB.CheckReasonable(&smem.SSVD27Socket.maxMessageIndex,smem.SSVD27Socket.messageIn);
                            parseAABB.AssignLcn(&smem.SSVD27Socket.maxMessageIndex,smem.SSVD27Socket.messageIn);
                            readJob.SetInterfaceAndReadFlag(&smem.SSVD27Socket.maxMessageIndex,smem.SSVD27Socket.messageIn,cUDP);
                            readJob.vDoRelayUDP_MESSAGEIN_0F8F(&smem.SSVD27Socket.maxMessageIndex, smem.SSVD27Socket.messageIn, cVD27ToCenter);
                            parseAABB.EchoToGUI(&smem.SSVD27Socket.maxMessageIndex,smem.SSVD27Socket.messageIn,"SSVD27");
                            parseAABB.MoveLastData(&smem.SSVD27Socket.maxMessageIndex,&smem.SSVD27Socket.lastPacketIndex,smem.SSVD27Socket.messageIn);
                        }
                    }
                }
                if (smem.SSVD28Socket.GetPortAlreadyOpen())
                {
                    if (FD_ISSET(smem.SSVD28Socket.Getfd(),&readfs))
                    {
                        readSelectLength=smem.SSVD28Socket.UdpRead();
                        if (readSelectLength>0)
                        {
                            parseAABB.ParseBlock(readSelectLength,smem.SSVD28Socket.block,smem.SSVD28Socket.messageIn,&smem.SSVD28Socket.lastPacketIndex,&smem.SSVD28Socket.maxMessageIndex);
                            parseAABB.CheckSum(&smem.SSVD28Socket.maxMessageIndex,smem.SSVD28Socket.messageIn);
                            parseAABB.DecideProtocol(&smem.SSVD28Socket.maxMessageIndex,smem.SSVD28Socket.messageIn,smem.SSVD28Socket.GetConnDevice());
                            parseAABB.vJudgeProtocolAndCheckLength(&smem.SSVD28Socket.maxMessageIndex,smem.SSVD28Socket.messageIn);
                            parseAABB.CheckReasonable(&smem.SSVD28Socket.maxMessageIndex,smem.SSVD28Socket.messageIn);
                            parseAABB.AssignLcn(&smem.SSVD28Socket.maxMessageIndex,smem.SSVD28Socket.messageIn);
                            readJob.SetInterfaceAndReadFlag(&smem.SSVD28Socket.maxMessageIndex,smem.SSVD28Socket.messageIn,cUDP);
                            readJob.vDoRelayUDP_MESSAGEIN_0F8F(&smem.SSVD28Socket.maxMessageIndex, smem.SSVD28Socket.messageIn, cVD28ToCenter);
                            parseAABB.EchoToGUI(&smem.SSVD28Socket.maxMessageIndex,smem.SSVD28Socket.messageIn,"SSVD28");
                            parseAABB.MoveLastData(&smem.SSVD28Socket.maxMessageIndex,&smem.SSVD28Socket.lastPacketIndex,smem.SSVD28Socket.messageIn);
                        }
                    }
                }
                if (smem.SSVD29Socket.GetPortAlreadyOpen())
                {
                    if (FD_ISSET(smem.SSVD29Socket.Getfd(),&readfs))
                    {
                        readSelectLength=smem.SSVD29Socket.UdpRead();
                        if (readSelectLength>0)
                        {
                            parseAABB.ParseBlock(readSelectLength,smem.SSVD29Socket.block,smem.SSVD29Socket.messageIn,&smem.SSVD29Socket.lastPacketIndex,&smem.SSVD29Socket.maxMessageIndex);
                            parseAABB.CheckSum(&smem.SSVD29Socket.maxMessageIndex,smem.SSVD29Socket.messageIn);
                            parseAABB.DecideProtocol(&smem.SSVD29Socket.maxMessageIndex,smem.SSVD29Socket.messageIn,smem.SSVD29Socket.GetConnDevice());
                            parseAABB.vJudgeProtocolAndCheckLength(&smem.SSVD29Socket.maxMessageIndex,smem.SSVD29Socket.messageIn);
                            parseAABB.CheckReasonable(&smem.SSVD29Socket.maxMessageIndex,smem.SSVD29Socket.messageIn);
                            parseAABB.AssignLcn(&smem.SSVD29Socket.maxMessageIndex,smem.SSVD29Socket.messageIn);
                            readJob.SetInterfaceAndReadFlag(&smem.SSVD29Socket.maxMessageIndex,smem.SSVD29Socket.messageIn,cUDP);
                            readJob.vDoRelayUDP_MESSAGEIN_0F8F(&smem.SSVD29Socket.maxMessageIndex, smem.SSVD29Socket.messageIn, cVD29ToCenter);
                            parseAABB.EchoToGUI(&smem.SSVD29Socket.maxMessageIndex,smem.SSVD29Socket.messageIn,"SSVD29");
                            parseAABB.MoveLastData(&smem.SSVD29Socket.maxMessageIndex,&smem.SSVD29Socket.lastPacketIndex,smem.SSVD29Socket.messageIn);
                        }
                    }
                }
                if (smem.SSVD30Socket.GetPortAlreadyOpen())
                {
                    if (FD_ISSET(smem.SSVD30Socket.Getfd(),&readfs))
                    {
                        readSelectLength=smem.SSVD30Socket.UdpRead();
                        if (readSelectLength>0)
                        {
                            parseAABB.ParseBlock(readSelectLength,smem.SSVD30Socket.block,smem.SSVD30Socket.messageIn,&smem.SSVD30Socket.lastPacketIndex,&smem.SSVD30Socket.maxMessageIndex);
                            parseAABB.CheckSum(&smem.SSVD30Socket.maxMessageIndex,smem.SSVD30Socket.messageIn);
                            parseAABB.DecideProtocol(&smem.SSVD30Socket.maxMessageIndex,smem.SSVD30Socket.messageIn,smem.SSVD30Socket.GetConnDevice());
                            parseAABB.vJudgeProtocolAndCheckLength(&smem.SSVD30Socket.maxMessageIndex,smem.SSVD30Socket.messageIn);
                            parseAABB.CheckReasonable(&smem.SSVD30Socket.maxMessageIndex,smem.SSVD30Socket.messageIn);
                            parseAABB.AssignLcn(&smem.SSVD30Socket.maxMessageIndex,smem.SSVD30Socket.messageIn);
                            readJob.SetInterfaceAndReadFlag(&smem.SSVD30Socket.maxMessageIndex,smem.SSVD30Socket.messageIn,cUDP);
                            readJob.vDoRelayUDP_MESSAGEIN_0F8F(&smem.SSVD30Socket.maxMessageIndex, smem.SSVD30Socket.messageIn, cVD30ToCenter);
                            parseAABB.EchoToGUI(&smem.SSVD30Socket.maxMessageIndex,smem.SSVD30Socket.messageIn,"SSVD30");
                            parseAABB.MoveLastData(&smem.SSVD30Socket.maxMessageIndex,&smem.SSVD30Socket.lastPacketIndex,smem.SSVD30Socket.messageIn);
                        }
                    }
                }
                /*
                            if (smem.SSVD31Socket.GetPortAlreadyOpen()) {
                                if (FD_ISSET(smem.SSVD31Socket.Getfd(),&readfs)) {
                                    readSelectLength=smem.SSVD31Socket.UdpRead();
                                    if (readSelectLength>0) {
                                      parseAABB.ParseBlock(readSelectLength,smem.SSVD31Socket.block,smem.SSVD31Socket.messageIn,&smem.SSVD31Socket.lastPacketIndex,&smem.SSVD31Socket.maxMessageIndex);
                                      parseAABB.CheckSum(&smem.SSVD31Socket.maxMessageIndex,smem.SSVD31Socket.messageIn);
                                      parseAABB.DecideProtocol(&smem.SSVD31Socket.maxMessageIndex,smem.SSVD31Socket.messageIn,smem.SSVD31Socket.GetConnDevice());
                                      parseAABB.vJudgeProtocolAndCheckLength(&smem.SSVD31Socket.maxMessageIndex,smem.SSVD31Socket.messageIn);
                                      parseAABB.CheckReasonable(&smem.SSVD31Socket.maxMessageIndex,smem.SSVD31Socket.messageIn);
                                      parseAABB.AssignLcn(&smem.SSVD31Socket.maxMessageIndex,smem.SSVD31Socket.messageIn);
                                      readJob.SetInterfaceAndReadFlag(&smem.SSVD31Socket.maxMessageIndex,smem.SSVD31Socket.messageIn,cUDP);
                                      readJob.vDoRelayUDP_MESSAGEIN_0F8F(&smem.SSVD31Socket.maxMessageIndex, smem.SSVD31Socket.messageIn, cVD31ToCenter);
                                      parseAABB.EchoToGUI(&smem.SSVD31Socket.maxMessageIndex,smem.SSVD31Socket.messageIn,"SSVD31");
                                      parseAABB.MoveLastData(&smem.SSVD31Socket.maxMessageIndex,&smem.SSVD31Socket.lastPacketIndex,smem.SSVD31Socket.messageIn);
                                    }
                                }
                            }
                            if (smem.SSVD32Socket.GetPortAlreadyOpen()) {
                                if (FD_ISSET(smem.SSVD32Socket.Getfd(),&readfs)) {
                                    readSelectLength=smem.SSVD32Socket.UdpRead();
                                    if (readSelectLength>0) {
                                      parseAABB.ParseBlock(readSelectLength,smem.SSVD32Socket.block,smem.SSVD32Socket.messageIn,&smem.SSVD32Socket.lastPacketIndex,&smem.SSVD32Socket.maxMessageIndex);
                                      parseAABB.CheckSum(&smem.SSVD32Socket.maxMessageIndex,smem.SSVD32Socket.messageIn);
                                      parseAABB.DecideProtocol(&smem.SSVD32Socket.maxMessageIndex,smem.SSVD32Socket.messageIn,smem.SSVD32Socket.GetConnDevice());
                                      parseAABB.vJudgeProtocolAndCheckLength(&smem.SSVD32Socket.maxMessageIndex,smem.SSVD32Socket.messageIn);
                                      parseAABB.CheckReasonable(&smem.SSVD32Socket.maxMessageIndex,smem.SSVD32Socket.messageIn);
                                      parseAABB.AssignLcn(&smem.SSVD32Socket.maxMessageIndex,smem.SSVD32Socket.messageIn);
                                      readJob.SetInterfaceAndReadFlag(&smem.SSVD32Socket.maxMessageIndex,smem.SSVD32Socket.messageIn,cUDP);
                                      readJob.vDoRelayUDP_MESSAGEIN_0F8F(&smem.SSVD32Socket.maxMessageIndex, smem.SSVD32Socket.messageIn, cVD32ToCenter);
                                      parseAABB.EchoToGUI(&smem.SSVD32Socket.maxMessageIndex,smem.SSVD32Socket.messageIn,"SSVD32");
                                      parseAABB.MoveLastData(&smem.SSVD32Socket.maxMessageIndex,&smem.SSVD32Socket.lastPacketIndex,smem.SSVD32Socket.messageIn);
                                    }
                                }
                            }
                */

                if (smem.redCountPort.GetPortAlreadyOpen())
                {
                    if (FD_ISSET(smem.redCountPort.Getfd(),&readfs))
                    {
                        readSelectLength=smem.redCountPort.Rs232Read();
                        if (readSelectLength>0)
                        {
                            if(smem.vGetCommEnable() == true)    //OT20110728
                            {
//                        printf("Get RedCount Return Msg!\n");
                                parseAABB.ParseBlock(readSelectLength,smem.redCountPort.block,smem.redCountPort.messageIn,&smem.redCountPort.lastPacketIndex,&smem.redCountPort.maxMessageIndex);
                                parseAABB.CheckSum(&smem.redCountPort.maxMessageIndex,smem.redCountPort.messageIn);
                                parseAABB.DecideProtocol(&smem.redCountPort.maxMessageIndex,smem.redCountPort.messageIn,smem.redCountPort.GetConnDevice());
                                parseAABB.CheckReasonable(&smem.redCountPort.maxMessageIndex,smem.redCountPort.messageIn);
                                readJob.vDoDisplay(&smem.redCountPort.maxMessageIndex,smem.redCountPort.messageIn);
                                parseAABB.EchoToGUI(&smem.redCountPort.maxMessageIndex,smem.redCountPort.messageIn,"RedCount");
                                parseAABB.MoveLastData(&smem.redCountPort.maxMessageIndex,&smem.redCountPort.lastPacketIndex,smem.redCountPort.messageIn);
                            }
                        }
                    }
                }
//------------------------------------------2013/05/22 chou ++ for 8 port-------------------------------------------------------------------------------
                if (smem.NSPredCountPort.GetPortAlreadyOpen())                           //port是否已開啟
                {
                    if (FD_ISSET(smem.NSPredCountPort.Getfd(),&readfs))                  //port是否有資料
                    {
                        readSelectLength=smem.NSPredCountPort.Rs232Read();               //將rs232資料q1`讀進buffer中
                        if (readSelectLength>0)
                        {
                            if(smem.vGetCommEnable() == true)
                            {
                                //OT20110526
                                smem.vSetLastGetProtocolTime();
                                parseAABB.ParseBlock(readSelectLength,smem.NSPredCountPort.block,smem.NSPredCountPort.messageIn,&smem.NSPredCountPort.lastPacketIndex,&smem.NSPredCountPort.maxMessageIndex);
                                parseAABB.CheckSum(&smem.NSPredCountPort.maxMessageIndex,smem.NSPredCountPort.messageIn);
                                parseAABB.DecideProtocol(&smem.NSPredCountPort.maxMessageIndex,smem.NSPredCountPort.messageIn,smem.NSPredCountPort.GetConnDevice());
                                parseAABB.vJudgeProtocolAndCheckLength(&smem.NSPredCountPort.maxMessageIndex,smem.NSPredCountPort.messageIn);
                                parseAABB.CheckReasonable(&smem.NSPredCountPort.maxMessageIndex,smem.NSPredCountPort.messageIn);
                                parseAABB.AssignLcn(&smem.NSPredCountPort.maxMessageIndex,smem.NSPredCountPort.messageIn);
                                readJob.SetInterfaceAndReadFlag(&smem.NSPredCountPort.maxMessageIndex,smem.NSPredCountPort.messageIn,cRS232);
                                readJob.vCheckIfBcastingForwardToUDP(&smem.NSPredCountPort.maxMessageIndex,smem.NSPredCountPort.messageIn);
                                readJob.CheckLcn(&smem.NSPredCountPort.maxMessageIndex,smem.NSPredCountPort.messageIn);
                                readJob.SetCenterComm(&smem.NSPredCountPort.maxMessageIndex,smem.NSPredCountPort.messageIn);
                                readJob.DoWorkByMESSAGEIN(&smem.NSPredCountPort.maxMessageIndex,smem.NSPredCountPort.messageIn);
                                parseAABB.EchoToGUI(&smem.NSPredCountPort.maxMessageIndex,smem.NSPredCountPort.messageIn,"/dev/ttyS5");
                                parseAABB.MoveLastData(&smem.NSPredCountPort.maxMessageIndex,&smem.NSPredCountPort.lastPacketIndex,smem.NSPredCountPort.messageIn);
                            }
                        }
                    }
                }
                if (smem.greenmanPort.GetPortAlreadyOpen())                           //port是否已開啟
                {
                    if (FD_ISSET(smem.greenmanPort.Getfd(),&readfs))                  //port是否有資料
                    {
                        readSelectLength=smem.greenmanPort.Rs232Read();               //將rs232資料q1`讀進buffer中
                        if (readSelectLength>0)
                        {
                            if(smem.vGetCommEnable() == true)
                            {
                                //OT20110526
                                smem.vSetLastGetProtocolTime();
                                parseAABB.ParseBlock(readSelectLength,smem.greenmanPort.block,smem.greenmanPort.messageIn,&smem.greenmanPort.lastPacketIndex,&smem.greenmanPort.maxMessageIndex);
                                parseAABB.CheckSum(&smem.greenmanPort.maxMessageIndex,smem.greenmanPort.messageIn);
                                parseAABB.DecideProtocol(&smem.greenmanPort.maxMessageIndex,smem.greenmanPort.messageIn,smem.greenmanPort.GetConnDevice());
                                parseAABB.vJudgeProtocolAndCheckLength(&smem.greenmanPort.maxMessageIndex,smem.greenmanPort.messageIn);
                                parseAABB.CheckReasonable(&smem.greenmanPort.maxMessageIndex,smem.greenmanPort.messageIn);
                                parseAABB.AssignLcn(&smem.greenmanPort.maxMessageIndex,smem.greenmanPort.messageIn);
                                readJob.SetInterfaceAndReadFlag(&smem.greenmanPort.maxMessageIndex,smem.greenmanPort.messageIn,cRS232);
                                readJob.vCheckIfBcastingForwardToUDP(&smem.greenmanPort.maxMessageIndex,smem.greenmanPort.messageIn);
                                readJob.CheckLcn(&smem.greenmanPort.maxMessageIndex,smem.greenmanPort.messageIn);
                                readJob.SetCenterComm(&smem.greenmanPort.maxMessageIndex,smem.greenmanPort.messageIn);
                                readJob.DoWorkByMESSAGEIN(&smem.greenmanPort.maxMessageIndex,smem.greenmanPort.messageIn);
                                parseAABB.EchoToGUI(&smem.greenmanPort.maxMessageIndex,smem.greenmanPort.messageIn,"/dev/ttyS6");
                                parseAABB.MoveLastData(&smem.greenmanPort.maxMessageIndex,&smem.greenmanPort.lastPacketIndex,smem.greenmanPort.messageIn);
                            }
                        }
                    }
                }
//------------------------------------------------------------------------------------------------------------------------
            }

        }

        //pthread_join(tidTimerHandle,NULL);                                          //結束另一個thread


        //關閉RS232,422,485 通訊埠
        if (smem.centerPort.CloseRs232Port()) printf("Close CenterPort Successful!!\n");
//    if (smem.ssPort.CloseRs232Port()) printf("Close SSPort Successful!!\n");
        //for shrink_Tc//if (smem.keypadPort.CloseRs232Port()) printf("Close KeypadPort Successful!!\n");
        if (smem.revLaneManualPort.CloseRs232Port()) printf("Close revLaneManualPort Successful!!\n");


        if(iCom2Type == Com2IsTesterPort)        //開啟test埠
        {
        }
        else if (iCom2Type == Com2IsGPSPort)           //開啟GPS Port
        {
            if (smem.NMEAPort.CloseRs232Port()) printf("Close NMEAPort Successful!!\n");
        }
        else if (iCom2Type == Com2IsPassingPort)
        {
            if (smem.passingPort.CloseRs232Port()) printf("Close passingPort Successful!!\n");
        }
        else if (iCom2Type == Com2IsPedestrianPushButton || iCom2Type == Com2IsRedCountAndCom1IsPedPushButton)
        {
            if (smem.PedPushButtonPort.CloseRs232Port()) printf("Close PedPushButtonPort Successful!!\n");
        }

//    if (smem.rtmsPort.CloseRs232Port()) printf("Close RtmsPort Successful!!\n");
        if (smem.redCountPort.CloseRs232Port()) printf("Close redCountPort Successful!!\n");


        //關閉UDP SOCKET 通訊埠
        if (smem.centerSocket.CloseUdpSocket()) printf("Close CenterSocket Successful!!\n");
//    if (smem.tempSocket.CloseUdpSocket()) printf("Close TempSocket Successful!!\n");
        if (smem.testerSocket.CloseUdpSocket()) printf("Close TesterSocket Successful!!\n");

        /* OT ADD 950927 */
        if (smem.CCJDynCtlSocket.CloseUdpSocket()) printf("Close CCJDynCtlSocket Successful!!\n");
        if (smem.CCJDynCtl2Socket.CloseUdpSocket()) printf("Close CCJDynCtl2Socket Successful!!\n");
        if (smem.ArwenVDInfoSocket.CloseUdpSocket()) printf("Close ArwenVDInfoSocket Successful!!\n");//arwen ++
        if (smem.WebSetSocket.CloseUdpSocket()) printf("Close WebSetSocket Successful!!\n");//arwen ++
        /*-- OT ADD 950927 */
        if (smem.DynCalServerInCrossSocket.CloseUdpSocket()) printf("Close DynCalServerInCrossSocket Successful!!\n");
        if (smem.DynCalServerInCenterSocket.CloseUdpSocket()) printf("Close DynCalServerInCenterSocket Successful!!\n");

//OT990618
        if (smem.revSyncSocket.CloseUdpSocket()) printf("Close revSyncSocket Successful!!\n");

        if (smem.amegidsSocket.CloseUdpSocket()) printf("Close amegidsSocket Successful!!\n");

        if (smem.SSVD01Socket.CloseUdpSocket()) printf("Close SSVD01Socket Successful!!\n");
        if (smem.SSVD02Socket.CloseUdpSocket()) printf("Close SSVD02Socket Successful!!\n");
        if (smem.SSVD03Socket.CloseUdpSocket()) printf("Close SSVD03Socket Successful!!\n");
        if (smem.SSVD04Socket.CloseUdpSocket()) printf("Close SSVD04Socket Successful!!\n");
//OT Pass
        smem.SSVD05Socket.CloseUdpSocket();
        smem.SSVD06Socket.CloseUdpSocket();
        smem.SSVD07Socket.CloseUdpSocket();
        smem.SSVD08Socket.CloseUdpSocket();
        smem.SSVD09Socket.CloseUdpSocket();
        smem.SSVD10Socket.CloseUdpSocket();
        smem.SSVD11Socket.CloseUdpSocket();
        smem.SSVD12Socket.CloseUdpSocket();
        smem.SSVD13Socket.CloseUdpSocket();
        smem.SSVD14Socket.CloseUdpSocket();
        smem.SSVD15Socket.CloseUdpSocket();
        smem.SSVD16Socket.CloseUdpSocket();
        smem.SSVD17Socket.CloseUdpSocket();
        smem.SSVD18Socket.CloseUdpSocket();
        smem.SSVD19Socket.CloseUdpSocket();
        smem.SSVD20Socket.CloseUdpSocket();
        smem.SSVD21Socket.CloseUdpSocket();
        smem.SSVD22Socket.CloseUdpSocket();
        smem.SSVD23Socket.CloseUdpSocket();
        smem.SSVD24Socket.CloseUdpSocket();
        smem.SSVD25Socket.CloseUdpSocket();
        smem.SSVD26Socket.CloseUdpSocket();
        smem.SSVD27Socket.CloseUdpSocket();
        smem.SSVD28Socket.CloseUdpSocket();
        smem.SSVD29Socket.CloseUdpSocket();
        smem.SSVD30Socket.CloseUdpSocket();
        /*
            smem.SSVD31Socket.CloseUdpSocket();
            smem.SSVD32Socket.CloseUdpSocket();
        */

        //關閉IO 通訊埠
        lcd240x128.ReleaseAuthority();
        digitalIO.ReleaseAuthority();


        return 0;

    }
    catch (...) {}
}
//---------------------------------------------------------------------------
void *timerHandle (void *)
{
}
//---------------------------------------------------------------------------


/*
            aaaaaaaaaa();
            if (smem.SSVD06Socket.GetPortAlreadyOpen()) {
                if (FD_ISSET(smem.SSVD06Socket.Getfd(),&readfs)) {
                    readSelectLength=smem.SSVD06Socket.UdpRead();
                    if (readSelectLength>0) {
                      parseAABB.ParseBlock(readSelectLength,smem.SSVD06Socket.block,smem.SSVD06Socket.messageIn,&smem.SSVD06Socket.lastPacketIndex,&smem.SSVD06Socket.maxMessageIndex);
                      parseAABB.CheckSum(&smem.SSVD06Socket.maxMessageIndex,smem.SSVD06Socket.messageIn);
                      parseAABB.DecideProtocol(&smem.SSVD06Socket.maxMessageIndex,smem.SSVD06Socket.messageIn,smem.SSVD06Socket.GetConnDevice());
                      parseAABB.vJudgeProtocolAndCheckLength(&smem.SSVD06Socket.maxMessageIndex,smem.SSVD06Socket.messageIn);
                      parseAABB.CheckReasonable(&smem.SSVD06Socket.maxMessageIndex,smem.SSVD06Socket.messageIn);
                      parseAABB.AssignLcn(&smem.SSVD06Socket.maxMessageIndex,smem.SSVD06Socket.messageIn);
                      readJob.SetInterfaceAndReadFlag(&smem.SSVD06Socket.maxMessageIndex,smem.SSVD06Socket.messageIn,cUDP);
                      readJob.vDoRelayUDP_MESSAGEIN_0F8F(&smem.SSVD06Socket.maxMessageIndex, smem.SSVD06Socket.messageIn, cVD06ToCenter);
                      parseAABB.EchoToGUI(&smem.SSVD06Socket.maxMessageIndex,smem.SSVD06Socket.messageIn,"SSVD06");
                      parseAABB.MoveLastData(&smem.SSVD06Socket.maxMessageIndex,&smem.SSVD06Socket.lastPacketIndex,smem.SSVD06Socket.messageIn);
                    }
                }
            }
*/


