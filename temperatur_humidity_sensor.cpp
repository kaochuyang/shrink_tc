#include "temperatur_humidity_sensor.h"
#include "CTIMER.h"
#include "SMEM.h"
#include "CSTC.h"
temperatur_humidity_sensor::temperatur_humidity_sensor()
{
    //ctor
}

temperatur_humidity_sensor::~temperatur_humidity_sensor()
{
    //dtor
}
void temperatur_humidity_sensor::block_receive(MESSAGEOK mes)
{

    try
    {
        BYTE DATA_P1=mes.packet[3];//
        DATA_Bit DATA_P2;
        DATA_P2.DBit=mes.packet[4];//


        bool box_switch_bit6=false;
        bool train_chain_bit5=false;
        bool manual_button_bit4=false;
        bool manual_red_button_bit3=false;
        bool flash_button_bit2=false;
        bool ped_button_bit1=false;
        bool manual_step_button_bit0=false;
        bool temperature_humidity_state_bit1=false;
        bool GPS_state_bit0=false;
        keypad.shrink_tc_set(mes);
        /* if(!(DATA_P1&0x40))box_switch_bit6=true;

         if(!(DATA_P1&0x20))train_chain_bit5=true;//5f09//5f08 5f09
         if(!(DATA_P1&0x10))manual_button_bit4=true;//5f08 01
         if(!(DATA_P1&0x8))manual_red_button_bit3=true;//5f08 02
         if(!(DATA_P1&0x4))flash_button_bit2=true;//5f08 40
         if(!(DATA_P1&0x2))ped_button_bit1=true;
         if(!(DATA_P1&0x1))manual_step_button_bit0=true;
         if(!(DATA_P2&0x2))temperature_humidity_state_bit1=true;
         if(!(DATA_P1&0x1))GPS_state_bit0=true;

        */

        YMDHMS GPS;
        temperature tempermachine;
        Humidity humidity;
        if(DATA_P2.switchBit.b1==0)//b1 is protocol's b0
        {
            smem.Set_temper_humi_state(true);
            smem.SetTemperHumi(mes.packet[11],mes.packet[12],mes.packet[13],mes.packet[14]);

        }
        else smem.Set_temper_humi_state(false); //sensor not found

        if(DATA_P2.switchBit.b2==0)
        {


            GPS.Hour=(mes.packet[5]&0x0f)*10+(mes.packet[6]&0x0f);//data_  ASCII to int
            GPS.Min=(mes.packet[7]&0x0f)*10+(mes.packet[8]&0x0f);//data_P4   ASCII to int
            GPS.Sec=(mes.packet[9]&0x0f)*10+(mes.packet[10]&0x0f);//data_P5    ASCII to int
            GPS.Day=(mes.packet[15]&0x0f)*10+(mes.packet[16]&0x0f);//  ASCII to int
            GPS.Month=(mes.packet[17]&0x0f)*10+(mes.packet[18]&0x0f);//data_P3  ASCII to int
            GPS.Year=(mes.packet[19]&0x0f)*10+(mes.packet[20]&0x0f);//format to ROC year ASCII to int

vAdjTimeByGPS(GPS);

            printf("DATE by GPS %d %d %d hour=%d min=%d sec=%d\n",GPS.Year,GPS.Month,GPS.Day,GPS.Hour,GPS.Min,GPS.Sec);

        }
        printf("COM3 information!!!======");
        for(int i=0; i<mes.packetLength; i++)printf("%x ",mes.packet[i]);
        printf("\n\n");

    }
    catch(...) {}

}

//---------------------------------------------------------------------------
void temperatur_humidity_sensor::vAdjTimeByGPS(YMDHMS GPS)
{
    try
    {
//smem.vSetBOOLData(GPS_SYNC, true);
        bool bEnableUpdate;



        struct tm time_str;
        struct tm *TSP = &time_str;
        time_t RunSec;


        time_str.tm_year = GPS.Year + 100;
        time_str.tm_mon = GPS.Month ;
        time_str.tm_mday = GPS.Day;
        time_str.tm_hour=GPS.Hour;
        time_str.tm_min=GPS.Min;
        time_str.tm_sec=GPS.Sec;

        time_str.tm_isdst = -1;
        RunSec = mktime(&time_str);

        RunSec = RunSec + 28800;                                         // now RunSec is GPS time


        TSP = localtime(&RunSec);

        char date[22] = {0},time[17] = {0};


        if(TSP->tm_year > 105 &&
                ( (smem.vGetBOOLData(GPS_SYNC) == true ) ||
                  (TSP->tm_min == 6 && TSP->tm_sec == 0) || (TSP->tm_min == 36 && TSP->tm_sec == 0)
                )
          )
        {
            bEnableUpdate = smem.vGetBOOLData(EnableUpdateRTC);
            if(bEnableUpdate == true)
            {
                stc.TimersRead_BeforeResetCMOSTime();  //OTBUG =1
                _intervalTimer.TimersRead_BeforeResetCMOSTime();
                smem.vSetTimerMutexRESET(1);
                while(smem.vGetTimerMutexCTIMER() == 0 || smem.vGetTimerMutexCSTC() == 0)
                {
                    usleep(100);
                }
                if(smem.sGPSGetTimeSwitch() == 1)
                {
                    smem.vSetSystemClockTime(TSP->tm_year+1900, TSP->tm_mon+1, TSP->tm_mday, TSP->tm_hour, TSP->tm_min, TSP->tm_sec);
                    //smem.SetSegmentChange(true);
                }
                _intervalTimer.TimersReset_AfterResetCMOSTime();  //OTBUG =1
                stc.TimersReset_AfterResetCMOSTime();
                smem.vSetTimerMutexRESET(0);
                smem.vSetAdjcount(0);
                smem.vSendTimerUpdateToCCJ_5F9E();

                system("hwclock -w");
            }

            smem.vSetBOOLData(GPS_SYNC, false);

        }

        smem.vSaveGPSStatus(true);


    }


    catch(...) {}
}

//
//bool temperatur_humidity_sensor::ParseBlock(int receiveBlockLength,BYTE *block,MESSAGEOK *messageIn,int *lastPacketIndex,int *maxMessageIndex)
//{
//    try
//    {
//
//        printf(" pasres _T_H \n");
//        printf(" pasres _T_H \n");
//        printf(" pasres _T_H \n");
//
//        int i,j,k;
//        unsigned char tempcks;
//
//        i = 0;                         //block[i]
//        j = *maxMessageIndex;          //messageIn[j]
//        k = *lastPacketIndex;        //messageIn[j].packet[k]
//        if(k < 0)
//        {
//            k = 0;
//        }
//        if(messageIn[j].uiPacketTEXTLEN == 0)
//        {
//            messageIn[j].uiPacketTEXTLEN = 65530;
//        }
//        if(k >= BUFFERSIZE-1)
//        {
//            vClearMsg(messageIn, j);    //should change buffer size
//        }
//
//        for (i = 0; i < receiveBlockLength; i++)
//        {
//
////        printf("%X  ", block[i]);
////        printf("messageIn[%d].cksStatus:%d\n", j, messageIn[j].cksStatus);
////        printf("messageIn[%d].success:%d\n", j, messageIn[j].success);
//
//            messageIn[j].packet[k] = block[i];
//
//            if(k == 0)
//            {
//                if(messageIn[j].packet[k] == 0xAA)
//                {
//                    messageIn[j].bGetDLE = true;
//                    k++;
//                }
//                else
//                {
//                    if(j > 0)
//                    {
//                        //special special special case!!
//                        messageIn[j] = messageIn[j-1];
//                        messageIn[j].cksStatus = false;
//                        k = messageIn[j].uiPacketTEXTLEN;
//                        messageIn[j].uiPacketTEXTLEN = 65530;
//                        messageIn[j].packet[k] = block[i];
//                        k++;
//
//                        // printf("\n%X %X\n", messageIn[j].packet[k-1], messageIn[j].packet[k]);
//
//                    }
//                    else
//                    {
//                        vClearMsg(messageIn, j);
//                        k = 0;
//                    }
//
//                }
//            }
//            else if(k == 1)    //bGetDLE must be true
//            {
//                if(messageIn[j].packet[k] == 0xBB)
//                {
//                    messageIn[j].bGetSOH = true;
//                    k++;
//                }
//                else if(messageIn[j].packet[k] == 0xDD)
//                {
//                    messageIn[j].bGetACK = true;
//                    k++;
//                    messageIn[j].uiPacketTEXTLEN = 5;  //set default to 77
//                    // messageIn[j].uiPacketTEXTLEN = 8;  //set default to 92
//                }
//                else if(messageIn[j].packet[k] == 0xEE)
//                {
//                    messageIn[j].bGetNAK = true;
//                    k++;
//                    messageIn[j].uiPacketTEXTLEN = 6;  //set default to 77
//                    // messageIn[j].uiPacketTEXTLEN = 9;  //set default to 92
//                }
//                else
//                {
//                    vClearMsg(messageIn, j);
//                    k = 0;
//                }
//            }
//            else      //other byte
//            {
//                if(messageIn[j].packet[k-1] == 0xAA)
//                {
//                    messageIn[j].bGetDLE = true;
//                }
//                if(messageIn[j].bGetSOH == true)    //now in 0xAA 0xBB status
//                {
//                    if(messageIn[j].bGetDLE == true && messageIn[j].packet[k] == 0xCC)
//                    {
//                        messageIn[j].uiPacketTEXTLEN = k+2;
//                    }
//                }
//                k++;
//            }
//            messageIn[j].bGetDLE = false;  //force clean every time
//
////        printf("messageIn[%d].uiPacketTEXTLEN:%d\n", j, messageIn[j].uiPacketTEXTLEN);
//
//            if(k == messageIn[j].uiPacketTEXTLEN)    //check ok or fail by checksum
//            {
//
//                for(int n = 0; n < k-2; n++)
//                {
//                    if(messageIn[j].packet[n] == 0xAA &&
//                            (messageIn[j].packet[n+1] == 0xBB ||
//                             messageIn[j].packet[n+1] == 0xDD ||
//                             messageIn[j].packet[n+1] == 0xEE)   )    //find start
//                    {
//
//                        tempcks = 0;
//                        for(int m = n; m < k - 1; m++)
//                        {
//                            tempcks ^= messageIn[j].packet[m];
//                        }
//                        // printf("cks:%2X, %d-%d\n", tempcks, n, k);
//                        if (messageIn[j].packet[ messageIn[j].uiPacketTEXTLEN-1 ] == tempcks)
//                        {
//                            messageIn[j].cksStatus = true;
//                            //OTTEST messageIn[j].success = true;  //OTTEST
//
//                            if( n != 0)    //move packet
//                            {
//                                int iMovePtr = 0;
//                                for(int m = n; m < k; m++)
//                                {
//                                    messageIn[j].packet[iMovePtr] = messageIn[j].packet[m];
//                                    iMovePtr++;
//                                }
//                                messageIn[j].uiPacketTEXTLEN -= n;
//                            }
//                            break;
//                        }
//                    }
//                }
//
//                if(messageIn[j].cksStatus == false)
//                {
//                    //extern
//                    if(messageIn[j].bGetSOH == true)
//                    {
//                        messageIn[j].uiPacketTEXTLEN = 65530;
//                        // messageIn[j].usiExternCounter++;
//                    }
//                    if(messageIn[j].bGetACK == true)
//                    {
//                        if(messageIn[j].uiPacketTEXTLEN == 5)
//                        {
//                            messageIn[j].uiPacketTEXTLEN = 8;  //set to 92
//                            // messageIn[j].usiExternCounter++;
//                        }
//                        else if(messageIn[j].uiPacketTEXTLEN == 8)      //fail
//                        {
//                            vClearMsg(messageIn, j);
//                            k = 0;
//                        }
//                    }
//                    if(messageIn[j].bGetNAK == true)
//                    {
//                        if(messageIn[j].uiPacketTEXTLEN == 6)
//                        {
//                            messageIn[j].uiPacketTEXTLEN = 9;  //set to 92
//                            // messageIn[j].usiExternCounter++;
//                        }
//                        else if(messageIn[j].uiPacketTEXTLEN == 9)      //fail
//                        {
//                            vClearMsg(messageIn, j);
//                            k = 0;
//                        }
//                    }
//                }
//                else
//                {
//                    messageIn[j].packetLength = messageIn[j].uiPacketTEXTLEN;
//                    j++;
//
//                    //20110922
//                    if(j >= MSGINSIZE)
//                    {
//                        j--;
//                    }
//
//                    vClearMsg(messageIn, j);
//                    k = 0;
//                }
//            }
//        }
//
//        printf("\n\n block=");
//        for(int h=0; h<*lastPacketIndex; h++)
//            printf("%x ",messageIn[j].packet[h]);
//        printf("\n\n");
//        *maxMessageIndex = j;
//        *lastPacketIndex = k;
//
//
//        return true;
//    }
//    catch(...) {}
//}

void temperatur_humidity_sensor::parseblockA(MESSAGEOK *mes,int length)
{
//    printf("parseblockA");
//    printf("record length=%d\n",record_length);
//            printf("context= ");
//            for(int i=0; i<length; i++)
//                printf("%x ",T_H.packet[i]);
//
//                printf("/n");
    try
    {
        BYTE cks=0x0;
        if((mes->packet[0]==0xaa)&&(mes->packet[1]==0xbb))
        {
//printf("phase 1\n\n");
            vClearMsg(&T_H);
            record_length=0;
            for(int i=0; i<length; i++)
                T_H.packet[i]=mes->packet[i];


            record_length=length;

            if(length==24)
            {
                for(int k=0; k<23; k++)
                    cks^=T_H.packet[k];
                if(cks==T_H.packet[23])
                {
                    T_H.packetLength=24;
                    block_receive(T_H);
                }


            }


        }

        else
        {

            //    printf("phase 2\n");

            for(int i=0; i<length; i++)
            {
                T_H.packet[record_length+i]=mes->packet[i];
                //    printf("%x ",T_H.packet[record_length+i]);
            }

            if((mes->packet[length-2]!=0xaa)&&(mes->packet[length-2]!=0xbb)){record_length+=length;}
            // printf("\n phase 2.1\n");
            for(int i=0; i<record_length; i++)
            {
                cks^=T_H.packet[i];
                //   printf("CKS=%x\n",cks);
            }
//  printf("phase 2.2\n");
//            for(int i=0; i<length-1; i++)
//            {//printf("CKS check doing\n");
//                cks^=mes->packet[i];
//                //  printf("CKS=%x\n",cks);
//            }
//   printf("phase 2.3\n");
//   printf("CKS=%x\n",cks);
            //   printf("pack[%d]=%x\n",length-1,mes->packet[length-1]);

            if(cks==mes->packet[record_length-1])
                //    {printf("phase 3\n");
                if((cks==T_H.packet[24])&&(T_H.packet[0]==0xaa)&&(T_H.packet[1]=0xbb))
                {printf("Pass CKS check\n");
//printf("phase 4\n");
                    T_H.packetLength=24;
                    block_receive(T_H);
                }
                else
                {
                    //      printf("phase 5\n");
                    vClearMsg(&T_H);
                    record_length=0;
                };
        }


    }





    catch(...) {}


}



bool temperatur_humidity_sensor::vClearMsg(MESSAGEOK *messageIn)
{



    messageIn->cksStatus = false;
    messageIn->success=false;
    messageIn->UDPorRS232=0;
    messageIn->ReadorWrite=0;
    messageIn->bGetDLE = false;
    messageIn->bGetSOH = false; //the same with STX
    messageIn->bGetACK = false;
    messageIn->bGetNAK = false;
    messageIn->uiPacketTEXTLEN = 65535;

    // messageIn[msgID].usiExternCounter = 0;;

    /*
    for (int b = 0; b < BUFFERSIZE; b++) {
      messageIn[msgID].packet[b]=0;
    }
    */
    bzero(messageIn->packet, BUFFERSIZE);
}

bool temperatur_humidity_sensor::CheckSum(int *maxMessageIndex,MESSAGEOK *messageIn)          //®Ö¹ïÀË¬d½X
{
    try
    {

        printf("cks T_H \n");
        printf("cks T_H \n");
        printf("cks T_H \n");

        if (*maxMessageIndex >= 0)                        //½T«O
        {
            for (int i=0; i<=*maxMessageIndex; i++)       //ÀË¬d¨C¤@­ÓmessageIn[i]
            {



                int j=0;                                 //­pºâmessageIn[].packet[j];
                BYTE tempcks=0;                          //¼È¦s­n¤ñ¹ïªºcheckSum;
                messageIn[i].cksStatus=false;            //default¬°false

                if (messageIn[i].packetLength>4)          //¦¹«Ê¥]ªø«×¦Ü¤Ö­n¦³5(AA DD 00 01 cks-77¦~,AA DD 00 00 01 00 08 cks-92¦~)
                {

                    if ((messageIn[i].packet[0]==(const BYTE)0xAA) &&
                            (messageIn[i].packet[1]==(const BYTE)0xDD || messageIn[i].packet[1]==(const BYTE)0xEE) &&
                            (messageIn[i].packetLength==5 || messageIn[i].packetLength==6 || messageIn[i].packetLength==8 || messageIn[i].packetLength==9))
                    {

                        printf("p2\n");

                        for (int k=0; k<messageIn[i].packetLength-1; k++)         //AA DD(EE) HI LO CKS
                            tempcks^=messageIn[i].packet[k];                     //§äpacketInªºCheckSum

                        if (messageIn[i].packet[messageIn[i].packetLength-1]==tempcks)
                            messageIn[i].cksStatus=true;

                    }
                    else
                    {

                        for (j=4; j<messageIn[i].packetLength; j++)
                        {

//                           if ( (messageIn[i].packet[j-3]!=(const BYTE)0xAA && messageIn[i].packet[j-2]==(const BYTE)0xAA && messageIn[i].packet[j-1]==(const BYTE)0xCC) ||     //²Ä¤G§PÂ_:aa cc¤@©w¬Û³s//Á×§KAA AA CC¦]¬°AA¬O¹w³]®·¤Wªºbyte
                            if ( (messageIn[i].packet[j-4]!=(const BYTE)0xAA && messageIn[i].packet[j-3]!=(const BYTE)0xBB && messageIn[i].packet[j-2]==(const BYTE)0xAA && messageIn[i].packet[j-1]==(const BYTE)0xCC) ||     //²Ä¤G§PÂ_:aa cc¤@©w¬Û³s//Á×§KAA AA CC¦]¬°AA¬O¹w³]®·¤Wªºbyte
                                    ((messageIn[i].packet[4]==(const BYTE)0x55 || messageIn[i].packet[4]==(const BYTE)0x56) && messageIn[i].packet[j-2]==(const BYTE)0xAA && messageIn[i].packet[j-1]==(const BYTE)0xCC) )     //¨ó©w½X¬°56ªº¦³¥i¯à¶Ç°eAA BB XX XX 56 ...... AA AA CC Cksªº¨ó©w¹L¨Ó
                            {

//OTDebug                                 printf("p4\n");

                                for (int k=0 ; k<j ; k++)                      //j=packetInLength-1=cks©Ò¦bªº[Index]
                                    tempcks^=messageIn[i].packet[k];          //§äpacketInªºCheckSum

                                if (messageIn[i].packet[j]==tempcks)           //²Ä¤T§PÂ_:aa cc«áªºcks
                                {
                                    //¤@©w©M¦¹°}¦C¬Û²Å..
                                    messageIn[i].packetLength=j+1;             //¦pªG¤T­Ó§PÂ_³£¹ï,¦¹°}¦Cªºªø«×´N¬Oj+1(¦¹³B¥²¶·­«·sassign)
                                    messageIn[i].cksStatus=true;               //±N¦¹°}¦C³]¬°¥¿½Tªº
//OTDebug                                     printf("cks OK, i:%d\n", i);
                                    break;                                     //¦pªG¤T­Ó§PÂ_³£¥¿½T«h¸õ¥X¦¹°j°é

                                }  //endIf
                            }

                        }  //end for (j=3 ; j <  messageIn[i].packetLength ; j++)
                    }
                }  //end if (messageIn[i].packetLength > 4)
            }  //end for (int i=0 ; i < maxMessageIndex ; i++)
        }  //end if (maxMessageIndex >= 0)

        return true;


    }
    catch(...) {}
}
bool temperatur_humidity_sensor::DoWorkByMESSAGEIN(int *maxMessageIndex,MESSAGEOK *messageIn)
{
    try
    {

        printf("do TH_H\n");
        printf("do TH_H\n");
        printf("do TH_H\n");

        if (*maxMessageIndex >= 0)
        {
            for (int i=0; i<=*maxMessageIndex; i++)                                 //ÀË¬d¨C¤@­ÓmessageIn[i]
            {
//           printf("[OTMsg] Go readJob. C:%d S:%d, P:%d \n", messageIn[i].cksStatus, messageIn[i].success, messageIn[i].protocol);
                if (messageIn[i].cksStatus==true)                                  //³q¹LCheckSumÀË´ú
                {
                    if (messageIn[i].success==true)                                //³q¹LÀË¬d«Ê¥]¦X²z©Ê
                    {

                        block_receive(messageIn[i]);
//                     printf("[OTMsg] Go readJob. S:OK C:OK\n");
                    }
                }
            }
        }

        return true;

    }
    catch(...) {}
}


