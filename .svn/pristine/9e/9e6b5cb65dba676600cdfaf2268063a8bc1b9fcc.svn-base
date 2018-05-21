//---------------------------------------------------------------------------
#include "PTRAFFIC92BRT.h"
#include "CDataToMessageOK.h"
#include "WRITEJOB.h"
#include "CTIMER.h"
#include <stdio.h>
#include <stdlib.h>
#include <error.h>
#include <math.h>

#include "CDataToMessageOK.h"
#include "WRITEJOB.h"
#include "CTIMER.h"
#include "DIGITALIO.h"
#include "CHexToInt.h"
#include "CSTC.h"
#include "CTools.h"
#include "SMEM.h"
#include "time.h"

clock_t start, finish;
double  duration;

//---------------------------------------------------------------------------
PTRAFFIC92BRT::PTRAFFIC92BRT(void)
{
try {
  } catch (...) {}
}
//---------------------------------------------------------------------------
PTRAFFIC92BRT::~PTRAFFIC92BRT(void)
{
}
//---------------------------------------------------------------------------
bool PTRAFFIC92BRT::DoWorkViaPTraffic92(MESSAGEOK message)
{
try {

  if(message.packet[7] == 0xBF) //printf(" this is 6F!\n");
  //printf("\nprintfMsg GoTo 92VD error page!!\n\n\n");
  start = clock();
  switch (message.packet[8]) {
            case 0x08:
                StepControl_BF08(message);
            break;
            case 0x0B:
               // writeJob.WritePhysicalOut(message.packet, message.packetLength, DEVICECENTER92); //中心port
                CarPosition_BF0B(message);
            break;
            case 0x00:
                writeJob.WritePhysicalOut(message.packet, message.packetLength, DEVICECENTER92); //中心port
                DeviceStatus_BF00(message);
            break;
            case 0x0C:
                writeJob.WritePhysicalOut(message.packet, message.packetLength, DEVICECENTER92); //中心port
            break;
            case 0x0D:
                writeJob.WritePhysicalOut(message.packet, message.packetLength, DEVICECENTER92); //中心port
                BRTDelay_BF0D(message);
            break;
            case 0x11:
                ManualPriority_BF11(message);
            break;
            case 0x41:
                vQueryManual_BF41(message);
            break;
            case 0x12:
                WritePrioritySegment_BF12(message);
            break;
            case 0x42:
                vQuerySegment_BF42(message);
            break;
            case 0x13:
                WritePriorityParameter_BF13(message);
            break;
            case 0x43:
                vQueryPriorityParameter_BF43(message);
            break;

          default:
            vReturnToCenterNACK(message.packet[7], message.packet[8], 0x0, 0x0); //protocol error
          break;
  }

  writeJob.WritePhysicalOutNoSetSeqNoResend(message.packet, message.packetLength, DEVICEBRT);
  //writeJob.WritePhysicalOutNoSetSeqNoResend(message.packet, message.packetLength, DEVICEJSON);
  return true;

  } catch (...) {}
}
//---------------------------------------------------------------------------
bool PTRAFFIC92BRT::DeviceStatus_BF00(MESSAGEOK DataMessageIn)
{
try{
    if(DataMessageIn.packetLength < 16) return false;
    else if(DataMessageIn.packetLength > 16) return false;
    else if(DataMessageIn.packet[9] > 1 || DataMessageIn.packet[10] >1) return false;
    printf("\n\nPTRAFFIC92BRT::DeviceStatus_BF00\n\n");

    smem.SetSRC1BF00(DataMessageIn.packet[9]);
    smem.SetSRC2BF00(DataMessageIn.packet[10]);
 } catch (...) {}
}
//---------------------------------------------------------------------------
bool PTRAFFIC92BRT::StepControl_BF08(MESSAGEOK DataMessageIn)
{
try {
    if(DataMessageIn.packetLength < 15) return false;
    else if(DataMessageIn.packetLength > 15) return false;
    else if(DataMessageIn.packet[9] > 5 || DataMessageIn.packet[10] > 2 ||
            DataMessageIn.packet[11] > 255) return false;

    int iTmp;
    unsigned char ucCondition;
    unsigned char usStrategy;
    unsigned char ucParameter;
    short int siSubPhaseID, siStepID, siEffectTime;
    //bool bFlag1, bFlag2;
    ucCondition = DataMessageIn.packet[9];  //號誌優先控制執行狀態
    usStrategy = DataMessageIn.packet[10];  //號誌優先控制執行策略
    ucParameter = DataMessageIn.packet[11]; //號誌優先控制執行參數
    unsigned short int usiCSTC_RunningPhaseCurrentPhase = stc.vGetUSIData(CSTC_exec_plan_phase_order);  //Should +1  //目前時相
    unsigned short int usiCSTC_RunningPhaseCurrentSubphase = stc.vGetUSIData(CSTC_exec_phase_current_subphase);  //Should +1  //目前綠燈分相
    unsigned short int usiCSTC_RunningPhaseCurrentSubphaseStep = stc.vGetUSIData(CSTC_exec_phase_current_subphase_step);  //目前綠燈分相步階
    //unsigned short int usiCSTC_RunningPhaseCurrentSubphaseStepGreenTime = stc.vGetUSIData(CSTC_exec_plan_green_time_compesated);
    unsigned short int usiCSTC_RunningPhaseCurrentSubphaseStepPG = stc.vGetUSIData(CSTC_exec_plan_pedgreenflash_time);  //行綠閃
    unsigned short int usiCSTC_RunningPhaseCurrentSubphaseStepPR = stc.vGetUSIData(CSTC_exec_plan_pedred_time);

    unsigned short int usiCSTC_StepTime = stc.vGetStepTime();  //剩餘秒數
    unsigned short int usiCSTC_RunningPlanGreen = stc.vGetUSIData(CSTC_exec_plan_green_time);
    unsigned short int usiCSTC_CompesationGreen = stc.vGetUSIData(CSTC_exec_plan_green_time_compesated);
    unsigned short int usiCSTC_RunningPlanMaxGreen = stc.vGetUSIData(CSTC_exec_plan_maxgreen_time); //最大綠
    unsigned short int usiCSTC_RunningPlanMinGreen = stc.vGetUSIData(CSTC_exec_plan_mingreen_time);
    unsigned short int usiCSTC_RunningStepTime = smem.vGetStepRunCounter(); //已過時間
    unsigned short int usiPgPrTime = usiCSTC_RunningPhaseCurrentSubphaseStepPG + usiCSTC_RunningPhaseCurrentSubphaseStepPR;
    unsigned short int usiGNewTime = 5;
    unsigned short int usiPgNewTime = 5;    //行綠閃
    unsigned short int usiPrNewTime = 5;
    unsigned short int usiThisStepRunTime = 5;
    unsigned char   usiCSTC_RunningExtendEast = stc.vGetUSIData(CSTC_exec_BF13plan_Extend_East);
    unsigned char   usiCSTC_RunningExtendWest = stc.vGetUSIData(CSTC_exec_BF13plan_Extend_West);
    unsigned char East = 0;
    unsigned char West = 0;

    static unsigned short int more = 0;
    static unsigned short int less = 0;

    char msg[256];
    char cTMP[30];
    //printf("\n\n!! %d !!\n\n",smem.GetPriorityswitch());
    if(smem.GetPriorityswitch() == false || smem.GetSegmentChange() == true) {
        sprintf(cTMP, "Priority Disable ! ! !");
        smem.vWriteMsgToDOM(cTMP);
        printf("\n\nPriority Disable\n\n");
        return false;
    }
    else{
        if(usStrategy == 1){
            East = (usiCSTC_RunningExtendEast >> usiCSTC_RunningPhaseCurrentSubphase) & 0x01;
            West = (usiCSTC_RunningExtendWest >> usiCSTC_RunningPhaseCurrentSubphase) & 0x01;
            if(East == 0x01 || West == 0x01){}
            else return false;

           // if(ucParameter == 0 || smem.GetExtendedswitch()) return false;
        }
//        else if(usStrategy == 2){
//            if(usiCSTC_RunningPhaseCurrentSubphase < 1) return false;
//            if(usiCSTC_RunningPhaseCurrentSubphaseStep > 0) return false;
//        }
    }

    if(usiCSTC_RunningStepTime >= usiCSTC_RunningPlanMaxGreen ) return false;

    int iReturnCommandSet = smem.vGet0FCommandSet();
    smem.SetPriorityStrategiesIN(true);
    printf("\n\n\n\nGet BH+08 actuate!\n\n\n\n");

    smem.SetBF08Condition(ucCondition);
    if(ucCondition == 2 || ucCondition == 3) {
//        if(usiPgPrTime == 0 ) { //行綠閃 行紅 = 0
//        //case 1, check step1 time
//            usiGNewTime = usiCSTC_RunningPlanMinGreen;
//            usiPrNewTime = 0;
//            usiPgNewTime = 0;
//        }
//        else if(usiPgPrTime < usiCSTC_RunningPlanMinGreen) {
//        //case 2, do nothing.
//            usiGNewTime = 0;
////            usiGNewTime = usiCSTC_RunningPlanMinGreen;
//            usiPrNewTime = usiCSTC_RunningPhaseCurrentSubphaseStepPR;
//            usiPgNewTime = usiCSTC_RunningPhaseCurrentSubphaseStepPG;
//        } else {
//        //case 3, reset pg/pr time
//            if(usiCSTC_RunningPhaseCurrentSubphaseStepPR > usiCSTC_RunningPlanMinGreen) {
//                usiGNewTime = 0;
//                usiPrNewTime = usiCSTC_RunningPlanMinGreen;
//                usiPgNewTime = 0;
//            } else {
//                usiGNewTime = 0;
//                usiPrNewTime = usiCSTC_RunningPhaseCurrentSubphaseStepPR;
//                usiPgNewTime = usiCSTC_RunningPlanMinGreen - usiCSTC_RunningPhaseCurrentSubphaseStepPR;
//            }
//        }

        smem.SetBF08Strategy(usStrategy);
        if(usStrategy == 1) {
            if(usiCSTC_RunningPhaseCurrentSubphaseStep < 1) {
                printf("\n^^^^^^^^ usiCSTC_RunningPlanGreen : %d", usiCSTC_RunningPlanGreen);
                printf("\n^^^^^^^^ usiCSTC_CompesationGreen : %d", usiCSTC_CompesationGreen);
                printf("\n^^^^^^^^  %d %d \n\n", usiCSTC_RunningStepTime, usiCSTC_StepTime);
                if(ucParameter > 0 && !smem.GetExtendedswitch()) {
                   if((usiCSTC_RunningStepTime+ usiCSTC_StepTime) >= usiCSTC_RunningPlanMaxGreen){
                        smem.SetExtendedswitch(1);
                        //stc.vSetTODCurrentTime(usiCSTC_RunningPlanMaxGreen);
                    }
                    else{
                        if( (ucParameter + usiCSTC_RunningStepTime+ usiCSTC_StepTime) <= usiCSTC_RunningPlanMaxGreen) {
                            //stc.vSetTODCurrentTime(ucParameter);
                            stc.vSetTODCurrentTime(ucParameter + usiCSTC_StepTime);
                            smem.SetBF08Parameter(ucParameter);
                            if((ucParameter + usiCSTC_RunningStepTime+ usiCSTC_StepTime) == usiCSTC_RunningPlanMaxGreen)
                                smem.SetExtendedswitch(1);
                        } else {
                                iTmp = usiCSTC_RunningPlanMaxGreen - usiCSTC_RunningStepTime;
                                stc.vSetTODCurrentTime(iTmp);
                                smem.SetBF08Parameter(iTmp - usiCSTC_StepTime);
                                smem.SetExtendedswitch(1);
                            //iTmp = usiCSTC_RunningPlanMaxGreen - usiCSTC_RunningStepTime;
                            //if(iTmp > 0) { stc.vSetTODCurrentTime(iTmp); }
                        }
                      if(smem.GetBF08Parameter() > 0){   //Priority是否啟動 是否延長秒數
                            printf("\n\n ****** BF02_Reduce  %d****** \n\n",smem.GetBF08Parameter());
                            stc.BF02_Reduce(usiCSTC_RunningPhaseCurrentSubphase);
                            //printf("\n\n ****** BF02_Reduce  %d****** \n\n",smem.GetBF08Parameter());
                      }
                    }
                }
            }
        } else if(usStrategy == 2) {
            if(usiCSTC_RunningPhaseCurrentSubphase > 0 && smem.usCutRedAlready==0){
                switch(usiCSTC_RunningPhaseCurrentSubphaseStep){
                    case(0):
                        printf("\n@@@@@@@ usiCSTC_RunningPlanGreen : %d @@@@@@",usiCSTC_RunningPlanGreen);
                        printf("\n@@@@@@@ usiCSTC_RunningPlanGreen : %d @@@@@@",usiCSTC_RunningPlanGreen);
                        printf("\n@@@@@@@ usiCSTC_RunningPlanGreen : %d @@@@@@\n",usiCSTC_RunningPlanGreen);
                        //if(usiCSTC_RunningPlanGreen != usiCSTC_RunningPlanMinGreen){
                            if(usiCSTC_RunningStepTime > usiCSTC_RunningPlanMinGreen ){
                          //go to next step.
                                stc.vGoToNextStepByTimer();
                                smem.vPlusBRTCompensationSec(usiCSTC_RunningPlanGreen - usiCSTC_RunningStepTime);
                                smem.usCutRedAlready=1;
                            } else {
                                iTmp = usiCSTC_RunningPlanMinGreen - usiCSTC_RunningStepTime;
                                printf("\n////////// iTmp : %d //////////",usiCSTC_RunningPlanGreen - usiCSTC_RunningPlanMinGreen);
                                printf("\n////////// iTmp : %d //////////",usiCSTC_RunningPlanGreen - usiCSTC_RunningPlanMinGreen);
                                printf("\n////////// iTmp : %d //////////\n",usiCSTC_RunningPlanGreen - usiCSTC_RunningPlanMinGreen);
                                if(iTmp > 0) { stc.vSetTODCurrentTime(iTmp); }
                                smem.vPlusBRTCompensationSec(usiCSTC_RunningPlanGreen - usiCSTC_RunningPlanMinGreen);
                                smem.usCutRedAlready=1;
                            }
                        //}
                    break;
//                    case(1):
//                        if(usiCSTC_RunningStepTime > usiPgNewTime ) {
//                      //go to next step.
//                            stc.vGoToNextStepByTimer();
//                        } else {
//                            iTmp = usiPgNewTime - usiCSTC_RunningStepTime;
//                            if(iTmp > 0) { stc.vSetTODCurrentTime(iTmp); }
//                            smem.vPlusBRTCompensationSec(iTmp);
//                        }
//                    break;
//                    case(2):
//                        if(usiCSTC_RunningStepTime > usiPrNewTime ) {
//                      //go to next step.
//                            stc.vGoToNextStepByTimer();
//                        } else {
//                            iTmp = usiPrNewTime - usiCSTC_RunningStepTime;
//                            if(iTmp > 0) { stc.vSetTODCurrentTime(iTmp); }
//                            smem.vPlusBRTCompensationSec(iTmp);
//                        }
//                    break;
//
                    default:
                    break;
                }
            }
        }
    }

    finish = clock();
//    printf("!!!!! start: %ld \n",start);
//    printf("!!!!! finish: %ld \n",finish);
    duration = (double)(finish - start) / CLOCKS_PER_SEC;
//    printf("******** Operation time: %f ms\n", duration );
//    printf("******** Operation time: %f ms\n\n", duration*1000 );

    if(smem.GetPriorityexecute()){
        more = 0; less = 0;
        smem.SetPriorityexecute(0);
    }

    if(duration > 1) more++;
    else less++;

    FILE *tmp;
    struct tm* currenttime;
    time_t now = time(NULL);
    currenttime = localtime(&now);

    tmp = fopen("/tmp/Priorityexecute.txt","w");
    if(tmp == NULL){
        printf("open Priorityexecute.txt error\n");
    }
    else{
        fprintf(tmp,"%#04d%#02d%#02d %#02d:%#02d:%#02d <1:%d >1:%d ",currenttime->tm_year+1900, currenttime->tm_mon+1, currenttime->tm_mday
                ,currenttime->tm_hour, currenttime->tm_min, currenttime->tm_sec, less, more);
    }
    fclose(tmp);

    return false;


} catch (...) {}
}
//-------------------jacky20140325-------------------------------------------------
bool PTRAFFIC92BRT::CarPosition_BF0B(MESSAGEOK DataMessageIn)
{
try{
    unsigned char ucDIR;
    DATA_Bit ucCarposition;

    if(DataMessageIn.packetLength < 38) return false;
    else if(DataMessageIn.packetLength > 38) return false;
    else if(DataMessageIn.packet[25] > 1 || DataMessageIn.packet[26] > 0x7F) return false;

    ucDIR = DataMessageIn.packet[25];
    ucCarposition.DBit = DataMessageIn.packet[26];
    printf("\n\n***** GET BF0B *****\n\n");

    if(ucDIR == 0){
        smem.SetBF0BmessageWest(DataMessageIn);
        smem.SetClearBF0BWest(true);
        smem.SetBF0BDIR(ucDIR);
        if (ucCarposition.switchBit.b2 == 1 && ucCarposition.switchBit.b3 == 0)
            smem.SetBF0BCarposition(1);
        else if (ucCarposition.switchBit.b2 == 1 && ucCarposition.switchBit.b3 == 1)
            smem.SetBF0BCarposition(2);
        else if (ucCarposition.switchBit.b3 == 1 && ucCarposition.switchBit.b4 == 0)
            smem.SetBF0BCarposition(3);
        else if (ucCarposition.switchBit.b3 == 1 && ucCarposition.switchBit.b4 == 1)
            smem.SetBF0BCarposition(4);
        else if (ucCarposition.switchBit.b4 == 1 && ucCarposition.switchBit.b5 == 0)
            smem.SetBF0BCarposition(5);
        else if (ucCarposition.switchBit.b4 == 1 && ucCarposition.switchBit.b5 == 1)
            smem.SetBF0BCarposition(6);
        else if (ucCarposition.switchBit.b5 == 1 && ucCarposition.switchBit.b6 == 0)
            smem.SetBF0BCarposition(7);
        else if (ucCarposition.switchBit.b6 == 1)
            smem.SetBF0BCarposition(0);
        else if (ucCarposition.switchBit.b7 == 1)
            smem.SetBF0BCarposition(8);
    } else if (ucDIR == 1){
        smem.SetBF0BmessageEast(DataMessageIn);
        smem.SetClearBF0BEast(true);
        smem.SetBF0BDIR(ucDIR);
        if (ucCarposition.switchBit.b2 == 1 && ucCarposition.switchBit.b3 == 0)
            smem.SetBF0BRCarposition(1);
        else if (ucCarposition.switchBit.b2 == 1 && ucCarposition.switchBit.b3 == 1)
            smem.SetBF0BRCarposition(2);
        else if (ucCarposition.switchBit.b3 == 1 && ucCarposition.switchBit.b4 == 0)
            smem.SetBF0BRCarposition(3);
        else if (ucCarposition.switchBit.b3 == 1 && ucCarposition.switchBit.b4 == 1)
            smem.SetBF0BRCarposition(4);
        else if (ucCarposition.switchBit.b4 == 1 && ucCarposition.switchBit.b5 == 0)
            smem.SetBF0BRCarposition(5);
        else if (ucCarposition.switchBit.b4 == 1 && ucCarposition.switchBit.b5 == 1)
            smem.SetBF0BRCarposition(6);
        else if (ucCarposition.switchBit.b5 == 1 && ucCarposition.switchBit.b6 == 0)
            smem.SetBF0BRCarposition(7);
        else if(ucCarposition.switchBit.b6 == 1)
            smem.SetBF0BRCarposition(0);
        else if (ucCarposition.switchBit.b7 == 1)
            smem.SetBF0BRCarposition(8);
    } else{
        smem.SetBF0BCarposition(0);
        smem.SetBF0BRCarposition(0);
    }

    return true;
 } catch (...) {}
}
//-------------jacky20140430-----------------------------------------------
bool PTRAFFIC92BRT::BRTDelay_BF0D(MESSAGEOK DataMessageIn)
{
try{
    unsigned int ScheduleTH;
    unsigned int DelayTime;

    ScheduleTH = DataMessageIn.packet[16]*256+DataMessageIn.packet[17];
    DelayTime = DataMessageIn.packet[19]*256+DataMessageIn.packet[20];

    if(DataMessageIn.packetLength < 24) return false;
    else if(DataMessageIn.packetLength > 24) return false;
    else if(DataMessageIn.packet[15] > 1 || ScheduleTH > 0xFFFF) return false;
    else if(DataMessageIn.packet[18] < 1 || DataMessageIn.packet[18] > 2) return false;
    else if(DelayTime > 0xFFFF) return false;

    printf("\n\nPTRAFFIC92BRT::BRTDelay_BF0D\n\n");

    smem.SetOperateConditionBF0D(DataMessageIn.packet[15]);

    smem.SetScheduleTHBF0D(ScheduleTH);

    smem.SetStatusBF0D(DataMessageIn.packet[18]);

    smem.SetDelayTimeBF0D(DelayTime);

 } catch (...) {}
}
//-------------jacky20140424-----------------------------------------------
bool PTRAFFIC92BRT::ManualPriority_BF11(MESSAGEOK DataMessageIn)
{
try{
    printf("\n\nPTRAFFIC92BRT::ManualPriority_BF11\n\n");

    if( DataMessageIn.packetLength < 14 ){ vReturnToCenterNACK(0xBF, 0x11, 0x08, 0x00); return false; }
    else if( DataMessageIn.packetLength > 14 ){ vReturnToCenterNACK(0xBF, 0x11, 0x08, DataMessageIn.packetLength - 12); return false; }
    vReturnToCenterACK(0xBF, 0x11);

    smem.SetBF11onoff(DataMessageIn.packet[9]);
    smem.SetBF11Time(DataMessageIn.packet[10]*60);
 } catch (...) {}
}
//-------------jacky20140424-----------------------------------------------
bool PTRAFFIC92BRT::vQueryManual_BF41(MESSAGEOK DataMessageIn)
{
try{
    printf("\n\nPTRAFFIC92BRT::vQueryManual_BF41\n\n");

    if( DataMessageIn.packetLength < 12 ){ vReturnToCenterNACK(0xBF, 0x41, 0x08, 0x00); return false; }
    else if( DataMessageIn.packetLength > 12 ){ vReturnToCenterNACK(0xBF, 0x41, 0x08, DataMessageIn.packetLength - 12); return false; }

    unsigned char data[4];
    data[0] = 0xBF;
    data[1] = 0xC1;
    data[2] = smem.GetBF11onoff();
    data[3] = ceil((float)smem.GetBF11Time()/60);

    MESSAGEOK _MsgOK;
    _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, 4, true);
    _MsgOK.InnerOrOutWard = cOutWard;
    writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICECENTER92); //中心port

 } catch (...) {}
}
//-------------jacky20140418-----------------------------------------------
bool PTRAFFIC92BRT::WritePrioritySegment_BF12(MESSAGEOK DataMessageIn)
{
try{
    printf("\n\nPTRAFFIC92BRT::Priorityswitch_BF12\n\n");
    int i=0 ,j=0;
    unsigned char hour,minute,planid;

    stc.Lock_to_Load_Segment_for_Center(DataMessageIn.packet[9]);
   // printf("\n Segment type : %02X packet[9] : %02X",stc._for_center_segment._segment_type,DataMessageIn.packet[9]);
   // printf("\n Segment count : %02X packet[9] : %02X",stc._for_center_segment._segment_count,DataMessageIn.packet[10]);

    if(stc._for_center_segment._segment_type != DataMessageIn.packet[9]){
        vReturnToCenterNACK(0xBF, 0x12, 0x02, 0x01); return false;
    }
    else if (stc._for_center_segment._segment_count != DataMessageIn.packet[10] ){
        vReturnToCenterNACK(0xBF, 0x12, 0x02, 0x02); return false;
    }

    for (int i=0;i<stc._for_center_segment._segment_count;i++) {
        hour = stc._for_center_segment._ptr_seg_exec_time[i]._hour;
        minute = stc._for_center_segment._ptr_seg_exec_time[i]._minute;
        planid = stc._for_center_segment._ptr_seg_exec_time[i]._planid;
       // printf(" \n\n!!! hour: %02X packet[%d] : %02X\n\n",hour,11+4*i,DataMessageIn.packet[11+4*i]);

        if (hour != DataMessageIn.packet[11+4*i]){
            vReturnToCenterNACK(0xBF, 0x12, 0x02, 3+4*i); return false; }
        else if (minute != DataMessageIn.packet[12+4*i]){
            vReturnToCenterNACK(0xBF, 0x12, 0x02, 4+4*i); return false; }
        else if(planid != DataMessageIn.packet[13+4*i]){
            vReturnToCenterNACK(0xBF, 0x12, 0x02, 5+4*i); return false; }
    }

    stc.Lock_to_Reset_Segment_for_BRT(DataMessageIn.packet[9],DataMessageIn.packet[10]);

    stc._for_BRT_segment._segment_type=DataMessageIn.packet[9];
    stc._for_BRT_segment._segment_count=DataMessageIn.packet[10];
    //printf("\nSegment type : %02X Segment count : %02X\n\n",stc._for_BRT_segment._segment_type,stc._for_BRT_segment._segment_count);

    for (j=0;j<stc._for_BRT_segment._segment_count;j++) {
        stc._for_BRT_segment._ptr_seg_exec_time[j]._hour=DataMessageIn.packet[11+4*j];
        stc._for_BRT_segment._ptr_seg_exec_time[j]._minute=DataMessageIn.packet[12+4*j];
        stc._for_BRT_segment._ptr_seg_exec_time[j]._planid=DataMessageIn.packet[13+4*j];
        stc._for_BRT_segment._ptr_seg_exec_time[j]._onoff=DataMessageIn.packet[14+4*j];
       /* printf("\nhour:%02X minute:%02X planid:%02X onoff:%02X\n\n",
               stc._for_BRT_segment._ptr_seg_exec_time[j]._hour,
               stc._for_BRT_segment._ptr_seg_exec_time[j]._minute,
               stc._for_BRT_segment._ptr_seg_exec_time[j]._planid,
               stc._for_BRT_segment._ptr_seg_exec_time[j]._onoff);
       */
        if(smem.vGetTCPhasePlanSegTypeData(TC_Plan, DataMessageIn.packet[13+4*j]) == false) {
            vReturnToCenterNACK(0xBF, 0x12, 0x04, 4*j + 5); return false;
        }
    }

    if( DataMessageIn.packetLength < 14+(4*DataMessageIn.packet[10])) { vReturnToCenterNACK(0xBF, 0x12, 0x08, 0x00); return false; }
    else if( DataMessageIn.packetLength > 14+(4*DataMessageIn.packet[10])) { vReturnToCenterNACK(0xBF, 0x12, 0x08, DataMessageIn.packetLength - 12); return false; }
    vReturnToCenterACK(0xBF, 0x12);

    stc.Lock_to_Save_Segment_from_BRT();
    smem.vSetTCPhasePlanSegTypeData(TC_SegType, DataMessageIn.packet[9], true);

    return true;

 } catch(...){}
}
//-------------jacky20140418---------------------------------------------------
bool PTRAFFIC92BRT::vQuerySegment_BF42(MESSAGEOK DataMessageIn)
{
try{
    bool bSegStatus;
    int iQuerySegmentType = DataMessageIn.packet[9];

    printf("\n\nPTRAFFIC92BRT::vQuerySegment_BF42\n\n");

    if( DataMessageIn.packetLength < 13) { vReturnToCenterNACK(0xBF, 0x42, 0x08, 0x00); return false; }
    else if( DataMessageIn.packetLength > 13) { vReturnToCenterNACK(0xBF, 0x42, 0x08, DataMessageIn.packetLength - 12); return false; }

    bSegStatus = smem.vGetTCPhasePlanSegTypeData(TC_SegType, iQuerySegmentType);
    if(bSegStatus == false) {
      vReturnToCenterNACK(0xBF, 0x42, 0x02, 0x0); return false;
    }

    stc.Lock_to_Load_Segment_for_BRT(iQuerySegmentType);

    unsigned char data[256];
    int iDataPtr = 4;

    data[0] = 0xBF;
    data[1] = 0xC2;
    data[2] = stc._for_BRT_segment._segment_type;
    data[3] = stc._for_BRT_segment._segment_count;

    for (int i=0;i<stc._for_BRT_segment._segment_count;i++) {
        data[iDataPtr] = stc._for_BRT_segment._ptr_seg_exec_time[i]._hour; iDataPtr++;
        data[iDataPtr] = stc._for_BRT_segment._ptr_seg_exec_time[i]._minute; iDataPtr++;
        data[iDataPtr] = stc._for_BRT_segment._ptr_seg_exec_time[i]._planid; iDataPtr++;
        data[iDataPtr] = stc._for_BRT_segment._ptr_seg_exec_time[i]._onoff; iDataPtr++;
    }

    MESSAGEOK _MsgOK;
    _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, iDataPtr, true);
    _MsgOK.InnerOrOutWard = cOutWard;
    writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICECENTER92); //中心port

    return true;
 } catch(...){}
}
//-----------jacky20140418----------------------------------------------
bool PTRAFFIC92BRT::WritePriorityParameter_BF13(MESSAGEOK DataMessageIn)
{
try{
    printf("\nPTRAFFIC92BRT::WritePriorityParameter_BF13\n\n");

    if( DataMessageIn.packetLength < 24+DataMessageIn.packet[20]*2 ) { vReturnToCenterNACK(0xBF, 0x13, 0x08, 0x00); return false; }
    else if( DataMessageIn.packetLength > 24+DataMessageIn.packet[20]*2 ) { vReturnToCenterNACK(0xBF, 0x13, 0x08, DataMessageIn.packetLength - 12); return false; }

    stc.Lock_to_Load_Plan_for_Center(DataMessageIn.packet[9]);
    if(stc._for_center_plan._planid != DataMessageIn.packet[9]){
        vReturnToCenterNACK(0xBF, 0x13, 0x02, 0x01); return false;
    }
    else if(stc._for_center_plan._phase_order != DataMessageIn.packet[10] ){
        vReturnToCenterNACK(0xBF, 0x13, 0x02, 0x02); return false;
    }

    stc.Lock_to_Reset_Plan_for_BRT(DataMessageIn.packet[9],DataMessageIn.packet[20]);

    stc._for_BRT_plan._planid=DataMessageIn.packet[9];
    stc._for_BRT_plan._phase_order=DataMessageIn.packet[10];
    stc._for_BRT_plan._east=DataMessageIn.packet[11];
    stc._for_BRT_plan._west=DataMessageIn.packet[12];
    stc._for_BRT_plan._th1_east=DataMessageIn.packet[13];
    stc._for_BRT_plan._th1_west=DataMessageIn.packet[14];
    stc._for_BRT_plan._th2=DataMessageIn.packet[15];
    stc._for_BRT_plan._time_east=DataMessageIn.packet[16]*256+DataMessageIn.packet[17];
    stc._for_BRT_plan._time_west=DataMessageIn.packet[18]*256+DataMessageIn.packet[19];
    stc._for_BRT_plan._BF13phasecount=DataMessageIn.packet[20];

    for(int j=0; j<2; j++){
        for(int i=0; i < stc._for_BRT_plan._BF13phasecount; i++ ){
            if(j==0) stc._for_BRT_plan._ptr_subplaninfo[i]._percentage[j] = DataMessageIn.packet[21+i];
            else if(j==1) stc._for_BRT_plan._ptr_subplaninfo[i]._percentage[j] = DataMessageIn.packet[21+i+stc._for_BRT_plan._BF13phasecount];
        }
    }
    vReturnToCenterACK(0xBF, 0x13);
    stc.Lock_to_Save_Plan_from_BRT();

    return true;
 } catch (...) {}
}
//-----------jacky20140418-------------------------------------
bool PTRAFFIC92BRT::vQueryPriorityParameter_BF43(MESSAGEOK DataMessageIn)
{
try{
    printf("\n\nPTRAFFIC92BRT::vQueryPriorityParameter_BF43\n\n");

    int iQueryPlan = DataMessageIn.packet[9];

    bool bPlanStatus;
    bPlanStatus = smem.vGetTCPhasePlanSegTypeData(TC_Plan, iQueryPlan);
    if(bPlanStatus == false) {
        vReturnToCenterNACK(0xBF, 0x43, 0x02, 0x0); return false;
    }

    if( DataMessageIn.packetLength < 13) { vReturnToCenterNACK(0xBF, 0x43, 0x08, 0x00); return false; }
    else if( DataMessageIn.packetLength > 13) { vReturnToCenterNACK(0xBF, 0x43, 0x08, DataMessageIn.packetLength - 12); return false; }

    stc.Lock_to_Load_Plan_for_BRT(iQueryPlan);

    unsigned char data[256];
    int iDataPtr = 14; //Form 6 Start to send SignalStatus
    data[0] = 0xBF;
    data[1] = 0xC3;
    data[2] = stc._for_BRT_plan._planid;
    data[3] = stc._for_BRT_plan._phase_order;
    data[4] = stc._for_BRT_plan._east;
    data[5] = stc._for_BRT_plan._west;
    data[6] = stc._for_BRT_plan._th1_east;
    data[7] = stc._for_BRT_plan._th1_west;
    data[8] = stc._for_BRT_plan._th2;
    data[9] = stc._for_BRT_plan._time_east / 256;
    data[10] = stc._for_BRT_plan._time_east % 256;
    data[11] = stc._for_BRT_plan._time_west / 256;
    data[12] = stc._for_BRT_plan._time_west % 256;
    data[13] = stc._for_BRT_plan._BF13phasecount;
    for(int j=0; j<2; j++){
        for (int i=0 ; i < stc._for_BRT_plan._BF13phasecount; i++ ){
            data[iDataPtr] = stc._for_BRT_plan._ptr_subplaninfo[i]._percentage[j]; iDataPtr++;
        }
    }
    /*
    printf("\n\n");
    for (int i=0; i<iDataPtr; i++){
        printf("%02X ", data[i]);
    }
    printf("\n\n");
    */
    MESSAGEOK _MsgOK;
   _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, iDataPtr, true);
   _MsgOK.InnerOrOutWard = cOutWard;
   writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICECENTER92); //中心port

  return true;

 } catch (...) {}
}

//ACK------------------------------------------------------------------------
int PTRAFFIC92BRT::vReturnToCenterACK(unsigned char ucDevCode,
                                     unsigned char ucControlCode)
{
try{
    unsigned char data[4];

    data[0] = 0x0F;
    data[1] = 0x80;
    data[2] = ucDevCode;
    data[3] = ucControlCode;

    MESSAGEOK _MsgOK;

    _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, 4,true);
    _MsgOK.InnerOrOutWard = cOutWard;
    writeJob.WriteWorkByMESSAGEOUT(_MsgOK);

    return 0;
  } catch (...) {}
}
//---------------------------------------------------------------------------
int PTRAFFIC92BRT::vReturnToCenterNACK(unsigned char ucDevCode,
                                     unsigned char ucControlCode,
                                     unsigned char ErrorCode,
                                     unsigned char ParameterNumber)
{
try{
    unsigned char data[6];

    data[0] = 0x0F;
    data[1] = 0x81;
    data[2] = ucDevCode;
    data[3] = ucControlCode;
    data[4] = ErrorCode;
    data[5] = ParameterNumber;

    MESSAGEOK _MsgOK;

    _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, 6,true);
    _MsgOK.InnerOrOutWard = cOutWard;
    writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICECENTER92);

    return 0;
  } catch (...) {}
}
