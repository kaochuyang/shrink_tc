#include "CTIMER.h"
#include "var.h"
#include "CDataToMessageOK.h"
#include "WRITEJOB.h"
#include "SMEM.h"
#include "DIGITALIO.h"

#include "SCREENMain.h"
#include "SCREENCtlSetup.h"
#include "SCREENModifyDT.h"
#include "SCREENNone.h"
#include "SCREENOnline.h"
#include "SCREENCommStat.h"
#include "SCREENTotalRun.h"
#include "screenCurrentLightStatus.h"
#include "screenChainStatus.h"
#include "SCREENOperStat.h"
#include "screenActuateArwenStatus.h"
#include "screenActuateArwenManyStatus.h"
#include "screenActuateArwenManyStatus2.h"
#include "screenReverseMenu.h"
#include "SCREENControlStrategyMenu.h"
#include "ScreenActuateArwenFlashManyStatus.h"
#include "SCREENPedSwitchStatus.h"
#include "screenActuateArwenVDInfo.h"
#include "screenPriority.h"  //jacky20140324
#include "screenPriorityExtendsec.h"    //jacky20140521
#include "screenCommunication.h"    //jacky20140522
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>

#include <pthread.h>
#include <signal.h>
#include <time.h>
#include <string.h>

#include "CSTC.h"
//--------------------------------------------------------------------------
intervalTimer _intervalTimer;

pthread_t intervalTimer::thread_forVDTimer;

timer_t intervalTimer::_t1;
timer_t intervalTimer::_t2;
timer_t intervalTimer::_t3;
timer_t intervalTimer::_t4;
timer_t intervalTimer::_t5;
timer_t intervalTimer::_t6;
timer_t intervalTimer::_t7;
timer_t intervalTimer::_t8;
timer_t intervalTimer::_t9;
//OTSS
timer_t intervalTimer::_t10;
timer_t intervalTimer::_tTrafficeLight;
timer_t intervalTimer::_t11;//arwen add
//timer_t intervalTimer::_tBRTLightctrl; //jacky20140411

struct itimerspec intervalTimer::_it1;
struct itimerspec intervalTimer::_it2;
struct itimerspec intervalTimer::_it3;
struct itimerspec intervalTimer::_it4;
struct itimerspec intervalTimer::_it5;
struct itimerspec intervalTimer::_it6;
struct itimerspec intervalTimer::_it7;
struct itimerspec intervalTimer::_it8;
struct itimerspec intervalTimer::_it9;
//OTSS
struct itimerspec intervalTimer::_it10;
struct itimerspec intervalTimer::_itTrafficeLight;

struct itimerspec intervalTimer::_it11;//arwen add
//struct itimerspec intervalTimer::_itBRTLightctrl; //jacky20140411

unsigned int CheckOnTimeCount = 0;

pthread_mutex_t intervalTimer::_ctimer_mutex = PTHREAD_MUTEX_INITIALIZER;

//intervalTimer-------------------------------------------------------------
intervalTimer::intervalTimer()
{
//  iS0Count = 0;
//  iSKCount = 0;
    usiGreenConflictCount = 0;
}
//--------------------------------------------------------------------------
void intervalTimer::ThreadsGenerate(void)
{
    try
    {
        //OTMARKPRINTF  printf("\nMAIN:                 pid=%d\n",getpid());

        /*
            sigset_t main_mask;
            sigfillset( & main_mask ); //block all signal, Ctrl+C not workable
            sigprocmask( SIG_SETMASK, & main_mask, NULL );
          //OTMARKPRINTF  printf("CSTC::CSTC(): blocking all signals in main!!\n");
        */

        printf("CTimer Starting\n");

        pthread_attr_t attr;
        pthread_attr_init( & attr );
        pthread_attr_setdetachstate( & attr, PTHREAD_CREATE_DETACHED );

        pthread_create( &(intervalTimer::thread_forVDTimer) , & attr, &(intervalTimer::PTime), NULL );

        pthread_attr_destroy( & attr );
    }
    catch (...) {}
}
//--------------------------------------------------------------------------
//**********************************************************
//****         Creating and Setting Timers
//**********************************************************
//--------------------------------------------------------------------------
void intervalTimer::TimersCreating(void)
{
    try
    {

        struct sigevent _sig1, _sig2, _sig3, _sig4, _sig5, _sig6, _sig7, _sig8, _sig9;
        //OTSS
        struct sigevent _sig10;

        struct sigevent _sigTrafficeLight;

        struct sigevent _sig11;//arwen add

        //struct sigevent _sigBRTLightctrl;  //jacky20140411


        _sig1.sigev_notify = SIGEV_SIGNAL;
        _sig1.sigev_signo = RTSIGNAL_Timer;
        _sig1.sigev_value.sival_int = 10;
        if ( timer_create( CLOCK_REALTIME, & _sig1, & _t1 ) ) exit( 1 );

        _sig2.sigev_notify = SIGEV_SIGNAL;
        _sig2.sigev_signo = RTSIGNAL_Timer;
        _sig2.sigev_value.sival_int = 11;
        if ( timer_create( CLOCK_REALTIME, & _sig2, & _t2 ) ) exit( 1 );

        _sig3.sigev_notify = SIGEV_SIGNAL;
        _sig3.sigev_signo = RTSIGNAL_Timer;
        _sig3.sigev_value.sival_int = 12;
        if ( timer_create( CLOCK_REALTIME, & _sig3, & _t3 ) ) exit( 1 );

        _sig4.sigev_notify = SIGEV_SIGNAL;
        _sig4.sigev_signo = RTSIGNAL_Timer;
        _sig4.sigev_value.sival_int = 13;
        if ( timer_create( CLOCK_REALTIME, & _sig4, & _t4 ) ) exit( 1 );

        _sig5.sigev_notify = SIGEV_SIGNAL;
        _sig5.sigev_signo = RTSIGNAL_Timer;
        _sig5.sigev_value.sival_int = 14;
        if ( timer_create( CLOCK_REALTIME, & _sig5, & _t5 ) ) exit( 1 );

        _sig6.sigev_notify = SIGEV_SIGNAL;
        _sig6.sigev_signo = RTSIGNAL_Timer;
        _sig6.sigev_value.sival_int = 15;
        if ( timer_create( CLOCK_REALTIME, & _sig6, & _t6 ) ) exit( 1 );

        _sig7.sigev_notify = SIGEV_SIGNAL;
        _sig7.sigev_signo = RTSIGNAL_Timer;
        _sig7.sigev_value.sival_int = 100;
        if ( timer_create( CLOCK_REALTIME, & _sig7, & _t7 ) ) exit( 1 );

        _sig8.sigev_notify = SIGEV_SIGNAL;
        _sig8.sigev_signo = RTSIGNAL_Timer;
        _sig8.sigev_value.sival_int = 101;
        if ( timer_create( CLOCK_REALTIME, & _sig8, & _t8 ) ) exit( 1 );

        _sig9.sigev_notify = SIGEV_SIGNAL;
        _sig9.sigev_signo = RTSIGNAL_Timer;
        _sig9.sigev_value.sival_int = 500;
        if ( timer_create( CLOCK_REALTIME, & _sig9, & _t9 ) ) exit( 1 );

        _sig10.sigev_notify = SIGEV_SIGNAL;
        _sig10.sigev_signo = RTSIGNAL_Timer;
        _sig10.sigev_value.sival_int = 501;
        if ( timer_create( CLOCK_REALTIME, & _sig10, & _t10 ) ) exit( 1 );

        _sigTrafficeLight.sigev_notify = SIGEV_SIGNAL;
        _sigTrafficeLight.sigev_signo = RTSIGNAL_Timer;
        _sigTrafficeLight.sigev_value.sival_int = 600;
        if ( timer_create( CLOCK_REALTIME, & _sigTrafficeLight, & _tTrafficeLight ) ) exit( 1 );

        _sig11.sigev_notify = SIGEV_SIGNAL;
        _sig11.sigev_signo = RTSIGNAL_Timer;
        _sig11.sigev_value.sival_int = 601;
        if ( timer_create( CLOCK_REALTIME, & _sig11, & _t11 ) ) exit( 1 );

        //_sigBRTLightctrl.sigev_notify = SIGEV_SIGNAL;  //jacky20140411
        //_sigBRTLightctrl.sigev_signo = RTSIGNAL_Timer;
        //_sigBRTLightctrl.sigev_value.sival_int = 602;
        //if ( timer_create( CLOCK_REALTIME, & _sigBRTLightctrl, & _tBRTLightctrl ) ) exit( 1 );

        printf("CTimer Create OK!\n");


    }
    catch (...) {}
}
//--------------------------------------------------------------------------
void intervalTimer::TimersSetting(void)
{
    try
    {
        unsigned char ucTmp;
        int iTmp;

        _it1.it_value.tv_sec = 1;
        _it1.it_value.tv_nsec = 0;
        _it1.it_interval.tv_sec = 1;
        _it1.it_interval.tv_nsec = 0;
        if ( timer_settime( _t1, 0, & _it1, NULL ) ) exit( 1 );

        _it2.it_value.tv_sec = 30;
        _it2.it_value.tv_nsec = 0;
        _it2.it_interval.tv_sec = 1;
        _it2.it_interval.tv_nsec = 0;
        if ( timer_settime( _t2, 0, & _it2, NULL ) ) exit( 1 );

        _it3.it_value.tv_sec = 40;
        _it3.it_value.tv_nsec = 0;
        _it3.it_interval.tv_sec = 8;
        _it3.it_interval.tv_nsec = 0;
        if ( timer_settime( _t3, 0, & _it3, NULL ) ) exit( 1 );

        _it4.it_value.tv_sec = 3;
        _it4.it_value.tv_nsec = 0;
        _it4.it_interval.tv_sec = 0;
        _it4.it_interval.tv_nsec = 400000000;//0.4sec
        if ( timer_settime( _t4, 0, & _it4, NULL ) ) exit( 1 );

        _it5.it_value.tv_sec = 10;
        _it5.it_value.tv_nsec = 0;
        _it5.it_interval.tv_sec = 8;
        _it5.it_interval.tv_nsec = 0;
        if ( timer_settime( _t5, 0, & _it5, NULL ) ) exit( 1 );

        ucTmp = smem.vGetHWCycleCodeFor_0F14_0FC4();
        switch (ucTmp)
        {
        case(0):
            iTmp = 0;
            break;
        case(1):
            iTmp = 1;
            break;
        case(2):
            iTmp = 2;
            break;
        case(3):
            iTmp = 5;
            break;
        case(4):
            iTmp = 60;
            break;
        case(5):
            iTmp = 300;
            break;
        }
        _it6.it_value.tv_sec = iTmp;
        _it6.it_value.tv_nsec = 0;
        _it6.it_interval.tv_sec = iTmp;
        _it6.it_interval.tv_nsec = 0;
        if ( timer_settime( _t6, 0, & _it6, NULL ) ) exit( 1 );

        _it7.it_value.tv_sec = 0;
        _it7.it_value.tv_nsec = 0;
        _it7.it_interval.tv_sec = 0;
        _it7.it_interval.tv_nsec = 0;
        if ( timer_settime( _t7, 0, & _it7, NULL ) ) exit( 1 );

        _it8.it_value.tv_sec = 0;
        _it8.it_value.tv_nsec = 0;
        _it8.it_interval.tv_sec = 0;
        _it8.it_interval.tv_nsec = 0;
        if ( timer_settime( _t8, 0, & _it8, NULL ) ) exit( 1 );

        /*
            _it9.it_value.tv_sec = 0;                                                   //5F1C
            _it9.it_value.tv_nsec = 0;
            _it9.it_interval.tv_sec = 0;
            _it9.it_interval.tv_nsec = 0;
            if ( timer_settime( _t9, 0, & _it9, NULL ) ) exit( 1 );
        */

//OTSS
        _it10.it_value.tv_sec = 20;                                                 //W77E58, only one
        _it10.it_value.tv_nsec = 0;
        _it10.it_interval.tv_sec = 0;
        _it10.it_interval.tv_nsec = 0;                                  // 1/10 sec
        if ( timer_settime( _t10, 0, & _it10, NULL ) ) exit( 1 );

        _itTrafficeLight.it_value.tv_sec = 0;
        _itTrafficeLight.it_value.tv_nsec = 200000000;  //0.2 sec
        _itTrafficeLight.it_interval.tv_sec = 2;
        _itTrafficeLight.it_interval.tv_nsec = 0;
        if ( timer_settime( _tTrafficeLight, 0, & _itTrafficeLight, NULL ) ) exit( 1 );

        _it11.it_value.tv_sec = 2;                                                 //W77E58, only one
        _it11.it_value.tv_nsec = 0;
        _it11.it_interval.tv_sec = 0;
        _it11.it_interval.tv_nsec = 500000000;  //0.5 sec;                                  // 1/10 sec
        if ( timer_settime( _t11, 0, & _it11, NULL ) ) exit( 1 );

        //_itBRTLightctrl.it_value.tv_sec = 0;                                                 //W77E58, only one
        //_itBRTLightctrl.it_value.tv_nsec = 100000000; //0.1 sec;
        //_itBRTLightctrl.it_interval.tv_sec = 0;
        //_itBRTLightctrl.it_interval.tv_nsec = 100000000;  //0.1 sec;                                  // 1/10 sec
        //if ( timer_settime( _tBRTLightctrl, 0, & _itBRTLightctrl, NULL ) ) exit( 1 );

        printf("CTimer Set OK!\n");


    }
    catch (...) {}
}
//--------------------------------------------------------------------------
void * intervalTimer::PTime(void *arg)
{
    try
    {
        int tempFace=0;

        unsigned char uc6F00[2];
        uc6F00[0] = 0x6F;
        uc6F00[1] = 0x00;

        unsigned char uc0F04[4];
        uc0F04[0] = 0x0F;
        uc0F04[1] = 0x04;
        MESSAGEOK _MSG;

        DATA_Bit DIOByte;

        //OTSS +3
        unsigned char ucXG[4];
        ucXG[0] = 0x58;       // X
        ucXG[1] = 0x47;       // G
        ucXG[2] = 0x0D;       // CR

        //OTMARKPRINTF  printf( "THREAD_VDINFO: pid=%d\n", getpid() );

        TimersCreating();
        TimersSetting();

        int VDsignum = 0;
        int VDrid = 9999;

        int SSInitCount = 0;

        int iCommuncationResetCount = 0;
        int iDBLockCount = 0;

        int iLastAliveStatusCount = 0;

        int iTC92SignalLightStatusVal = 5;
        int iTransmitCycle5F0F_Count = 0;
        iTC92SignalLightStatusVal = smem.vGetINTData(TC92SignalLightStatus_5F0F_IntervalTime);

        int iTC92SignalStepStatusVal = 5;
        int iTransmitCycle5F03_Count = 0;
        iTC92SignalStepStatusVal = smem.vGetINTData(TC92SignalStepStatus_5F03_IntervalTime);

        int prioritycount = 0;      //jacky20140325 BRT priority
        int Query8051 = 0;  //查面板8051
        unsigned short int numtime; //jacky20140424 計算手動優先號誌啟用時間
        bool Permanent = false; //jacky20140424 判斷手動優先是否永久

        //jacky20141202 PGPR shutdown Count
        unsigned short int LearningPGPRThree = 0;
        unsigned short int LearningPGPROne = 0;
        //jacky20141203 GPS Status Report
        unsigned short int GPSStatusReport = 0;
        //jacky20141227 Clear BF0B
        unsigned short int ClearBF0BEast = 0;
        unsigned short int ClearBF0BWest = 0;
        //jacky20151210
        unsigned char RequestKeypad = 0;

        unsigned int SetSystemTimeCount = 0;

        time_t _RunSec, _SSLastRun;

        //BT9512260001 START
        char msg[256];
        memset(msg,0,sizeof(msg));
        //BT9512260001 END


        sigset_t Timer_mask;
        sigfillset(& Timer_mask);
        sigprocmask( SIG_SETMASK, & Timer_mask, NULL);
        sigemptyset( & Timer_mask ); //unblock all signal
        sigaddset( & Timer_mask, RTSIGNAL_Timer );    //block VD_SIGNAL
        sigaddset( & Timer_mask, RTSIGNAL_RECODE_SMEM );    //block VD_SIGNAL


        siginfo_t VDTimer_siginfo;
        unsigned short int usiTmp;
        bool bConnectCenterStatus;
        DATA_Bit _ControlStrategy;

        unsigned short int usiCurrentSubphaseStep;
        unsigned short int usiCurrentSubphaseStepN;
        bool bStepChangeOK;

        int iTmp;
        unsigned char ucCounterWDTForLightboardRedcount;

        unsigned char ucCSTCControlStrategy;  //jacky20140411

//OT20110526
        int iMinCycleTime;
        unsigned char ucTmp;
        time_t currentTime;
        struct tm *now;
        time_t tmpTime;

        while(1)
        {
            while(smem.vGetTimerMutexRESET() == 1)
            {
//          printf("timer set, ctimer wait!\n");
                usleep(100);
            }
            smem.vSetTimerMutexCTIMER(1);

            ucCSTCControlStrategy = smem.vGetUCData(TC_CSTC_ControlStrategy); //jacky20140411

            VDsignum = sigwaitinfo( & Timer_mask, & VDTimer_siginfo );

            switch ( VDsignum )
            {
            case( RTSIGNAL_Timer ):
                VDrid = VDTimer_siginfo.si_value.sival_int;

                switch (VDrid)
                {
                case( 10 ):
                    //WatchDog
                    //OT20110526
                    currentTime = time(NULL);
                    now = localtime(&currentTime);

                    //OT20140214
                    smem.vPlusStepRunCounter();

                    _intervalTimer.vRefreshWatchDog();

                    smem.vSet5F18EffectTimeCountDown();

                    smem.vSet_DIOByte(digitalIO.vReadDigitalIO());

                    _intervalTimer.GetDoorStatus();

                    _intervalTimer.vCheckSMEM();

                    tempFace=smem.GetcFace();

//                                  stc.BRTImmediateReport();  //jacky20140326  BRT Report

                    if (tempFace==cMAIN) screenMain.DisplayDateTime();
                    else if (tempFace==cCTLSETUP) screenCtlSetup.DisplayDateTime();
                    else if (tempFace==cMODIFYDT) screenModifyDT.DisplayCurrentDateTime();
                    else if (tempFace==cCOMMSTAT) screenCommStat.UpdateComm();
                    else if (tempFace==cTOTALRUN) screenTotalRun.DisplayTotalTime();
                    else if (tempFace==cCURRENTLIGHTSTATUS) screenCurrentLightStatus.vRefreshStepSec();
                    else if (tempFace==cCHAINSTATUS) screenChainStatus.vRefreshChainStatusData();
                    else if (tempFace == cOPERSTAT) screenOperStat.vShowGreenConflict();
                    else if (tempFace == cREVERSETIMTMENU) screenReverseMenu.vRefreshStepSec();
                    else if (tempFace == cCTRLSTRATEGY) screenControlStrategyMenu.DisplayDateTime();
                    else if (tempFace == cACTUATEARWENMANYSTATUS) screenActuateArwenManyStatus.vRefreshEverySec();
                    else if (tempFace == cACTUATEARWENMANYSTATUS2) screenActuateArwenManyStatus2.vRefreshEverySec();
                    else if (tempFace == cACTUATEARWENFLASHMANYSTATUS) screenActuateArwenFlashManyStatus.vRefreshEverySec();

                    else if (tempFace == cPEDSWITCH)screenPedSwitchStatus.vRefreshSwitchStatus();

                    else if (tempFace == cACTDetector)
                    {
                        //if(smem.cPedPushButton.GetVirtualVDMode()==0){//虛擬VD
                        screenActuateArwenVDINFO.vRefreshshowDetectorData();
                        //}else{
                        //screenActuateArwenVDINFO.vRefreshshowPEDData();
                        //}
                    }
                    else if(tempFace == cBRTPRIORITY)
                    {
                        screenPriority.vRefreshStepSec();
                        screenPriority.vPrioritytState();
                    }
                    else if(tempFace == cBRTExtendsec)
                    {
                        screenPriorityExtendsec.PlanPhase();
                        screenPriorityExtendsec.information();
                    }
                    else if(tempFace == cCommunication)
                    {
                        screenCommunication.status();
                    }


//                                  else if (tempFace == cACTUATEARWENSTATUS) screenActuateArwenStatus.vRefreshEverySec();
                    screenActuateArwenStatus.vRefreshEverySec();

//Remove vSend92VD_6F02
//Remove smem.GetSSComm() == false

                    if(iLastAliveStatusCount > 300)
                    {
                        smem.vSaveLastAliveStatus();
                        iLastAliveStatusCount = 0;
                    }
                    iLastAliveStatusCount++;

//---------------------------------------------------------
                    if(smem.vGetBOOLData(TC92_TransmitCycle5F0F_Change))
                    {
                        iTC92SignalLightStatusVal = smem.vGetINTData(TC92SignalLightStatus_5F0F_IntervalTime);
                        smem.vSetBOOLData(TC92_TransmitCycle5F0F_Change, false);
                    }
                    if (iTransmitCycle5F0F_Count >= iTC92SignalLightStatusVal)
                    {
                        if((iTC92SignalLightStatusVal != 0x00) && (iTC92SignalLightStatusVal != 0xFF))
                        {
                            //OTMARKPRINTF  printf("[Message] Send Interval:%d\n", iTC92SignalLightStatusVal);
                            stc.ReportCurrentSignalMap_5F0F();
                        }
                        iTransmitCycle5F0F_Count = 0;
                    }
                    iTransmitCycle5F0F_Count++;
//---------------------------------------------------------
                    if(smem.vGetBOOLData(TC92_TransmitCycle5F03_Change))
                    {
                        iTC92SignalStepStatusVal = smem.vGetINTData(TC92SignalStepStatus_5F03_IntervalTime);
                        smem.vSetBOOLData(TC92_TransmitCycle5F03_Change, false);
                    }

                    if (iTransmitCycle5F03_Count >= iTC92SignalStepStatusVal)
                    {
                        if((iTC92SignalStepStatusVal != 0x00) && (iTC92SignalStepStatusVal != 0xFF))
                        {
                            //OTMARKPRINTF  printf("[Message] Send Step IntervalQQ:%d\n", iTC92SignalStepStatusVal);
                            stc.ReportCurrentStepStatus();    //5F03
                            //OT disable 5F80 for standard soft
                            stc.vReport5F80CCTProtocalSendStep();

                            stc.ReportCurrentOperationMode_5F08();            //new add by cct
                            stc.vReportCCTRevStatus5F82();            //OT990629

                            smem.vSetTC5F08StatusChange(true);  //interval data, force send.

                            stc.BRTImmediateReport();  //jacky20140326
                        }
                        iTransmitCycle5F03_Count = 0;
                    }
                    iTransmitCycle5F03_Count++;
//---------------------------------------------------------

                    /* chain detect */
                    _intervalTimer.vChildChain();

//MOveTo600                                  _intervalTimer.vSendHeartBeatToLCX405();

                    /*For CCJ DongDa XiBin */
                    if(smem.vGetBOOLData(CCJ_SendStepToCCJ))
                    {
                        stc.ReportCurrentStepStatus5F03toCCJ();
                    }

                    //check train chain
                    if(smem.vGetUCData(TC_TrainChainEnable) == 1)    //Railway switch Enable.
                    {
                        if(smem.vGetUCData(TC_TrainChainNOW) == 1)    //train coming
                        {

                            if(smem.vGetUCData(TC_TrainChainLast) == 0)    //log and colose redcount
                            {

                                //OT990617
                                smem.vSetUCData(TC_TrainChainNOW, 1); // set status repeat, avoid TC_TrainChainLast retragger
                                smem.vWriteMsgToDOM("Get Train Coming");

//                                          smem.vSetUCData(TC_Redcount_Display_Enable, 0);
                                stc.CalculateAndSendRedCount(10); //close redcount

                                stc.ReportCurrentOperationMode_5F08();
                                smem.vSetTC5F08StatusChange(true);  //interval data, force send.
                            }

                            stc.vTrainComing();
                            smem.vSetUCData(TC_TrainChainNOW, 1);
                        }
                        else                                        //No Train
                        {
                            if(smem.vGetUCData(TC_TrainChainLast) == 1)    //train leaving yet. //log and open redcount
                            {
                                smem.vSetUCData(TC_TrainChainNOW, 0); // set status repeat, avoid TC_TrainChainLast retragger
                                stc.vTrainLeaving();
//                                      printf("TC_TrainChainLast = 1\n");
                                smem.vWriteMsgToDOM("Get Train Leaving");

//                                        smem.vSetUCData(TC_Redcount_Display_Enable, 1);
                                stc.ReportCurrentOperationMode_5F08();
                                smem.vSetTC5F08StatusChange(true);  //interval data, force send.
                            }
                            smem.vSetUCData(TC_TrainChainNOW, 0);
                        }
                    }

                    //check Rev Control Status
                    if(smem.vGetUCData(TC_ReverseLane_Manual_Enable_Status) == 1)    //Enable Button
                    {
                        _RunSec = time(NULL);
                        _SSLastRun = smem.GetRevManualPadTime();    //RevManualpad, borrow form SS.
//                                    printf("Rev _RunSec:%d\n", _RunSec);
//                                    printf("Rev _SSLastRun:%d\n", _SSLastRun);
                        if( _RunSec >= _SSLastRun && _SSLastRun != 0 &&
                                (_RunSec - _SSLastRun) > 6 )
                        {
                            printf("Rev Comm Timeout!\n");
                            stc.vSetRevStep(3);                       // XX
                            stc.vLightRevSingal();
                            smem.vSetUCData(TC_ReverseLane_Control_Mode, 0);
                        }
                    }

                    //990402
                    if(smem.vGetTC5F08StatusChange() == true)
                    {
                        smem.vSetTC5F08StatusChange(false);
                        _intervalTimer.vReportCurrentOperationMode_5F08_inCtimer();
                    }

                    //OT990618
                    if(smem.vGetUCData(revSyncEnable) == true)
                    {
                        if(iLastAliveStatusCount % smem.vGetUCData(revSyncInterval) == 0)
                        {
                            smem.vCheckRevSyncStatus();
                            stc.vSendRevSync();
                        }
                    }

                    //jacky20140325  BRT priority
                    if(prioritycount > 2)
                    {
                        smem.SetBF08Condition(0);
                        smem.SetBF08Strategy(0);
                        smem.SetBF0BCarposition(0);
                        smem.SetBF0BRCarposition(0);
                        prioritycount=0;
                    }
                    prioritycount++;

                    if(Query8051 > 5)
                    {
                        BRT_Query8051Status(); //jacky20140115 Query 8051 Status in 10s
                        Query8051 =0 ;
                    }
                    Query8051++;

                    //jacky20140730 中心對時 or GPS對時
                    if(SetSystemTimeCount == 3000)
                    {
                        smem.vSetSystemClockTimeSelect(0);
                        SetSystemTimeCount = 0;
                    }
                    SetSystemTimeCount++;

                    if(CheckOnTimeCount == 3900)
                    {
                        if(smem.vGetSystemClockTimeSelect() == 0)
                        {
                            smem.sGPSSetTimeSwitch(1);
                        }
                        else
                            smem.sGPSSetTimeSwitch(0);

                        CheckOnTimeCount = 0;
                    }
                    CheckOnTimeCount++;

                    //jacky20141203
//                                  if(GPSStatusReport >= 5){
//                                    stc.BRTGPSStatusReport();
//                                    GPSStatusReport = 0;
//                                  }
//                                  GPSStatusReport++;

                    //OT20140415
                    unsigned short int usiAdjRemainSec;
                    if ( smem.vGetBOOLData(TCDynSegSwitch) )
                    {
                        usiAdjRemainSec = smem.vGetUSIData(TCDynSegRemainSec);
                        if(usiAdjRemainSec > 0)
                        {
                            smem.vSetUSIData(TCDynSegRemainSec, (usiAdjRemainSec-1) );
                        }
                        else
                        {
                            smem.vSetBOOLData(TCDynSegSwitch, false);
                            smem.vSetUCData(TCDynSegAdjustHour, 0);
                            smem.vSetUCData(TCDynSegAdjustMin, 0);
                            smem.vSetUSIData(TCDynSegAdjustKeepTime, 0);
                        }
                    }

                    //OT20140515
                    //OT20140520
//                                  if(smem.vGetBOOLData(TCDynSegStatus) == true) {
                    if(smem.vGetBOOLData(TCDynSegSwitch) == true)
                    {
                        _intervalTimer.vReportDynSegStatus_5F0D_inCtimer();
                    }

                    //OT20131210
                    stc.vReportBF01CCTProtocalSendKaikinStep();

                    smem.cPedPushButton.SendPedSWLightOpenAndClose();//add Arwen
                    screenActuateArwenVDINFO.detectorWatchDog();//add Arwen
                    smem.cPedPushButton.sendVirtualVDData();//add Arwen假VD
//                                  smem.cPedPushButton.QueryPEDState();//add Arwen
//                                  smem.cPedPushButton.SetWatchDogAdd();//add Arwen
                    //OT20110526
                    smem.vSetSystemTime(currentTime);

                    //test
//only for test.                                  stc.CalculateAndSendRedCountInDyn(0);


                    //  ++ucCounterWDTForLightboardRedcount;
                    //  if(ucCounterWDTForLightboardRedcount > 1) {
                    //    ucCounterWDTForLightboardRedcount = 0;
                    //    _itTrafficeLight.it_value.tv_sec = 0;
                    //    _itTrafficeLight.it_value.tv_nsec = 100000000;  // 0.5 sec.
                    //    _itTrafficeLight.it_interval.tv_sec = 0;
                    //    _itTrafficeLight.it_interval.tv_nsec = 0;
                    //    timer_settime( _tTrafficeLight, 0, & _itTrafficeLight, NULL );

                    //  }

                    //stc.CalculateAndSendGreenmanCount(0);         //chou ++ 2013/05/17 綠人倒數啟動
                    // stc.CalculateAndSendNSPredCount(0);
//OT20131219                              stc.vsendPedgreencount(); //chou ++ 2013/05/17 綠人倒數啟動
//OT20131219                              stc.vSendNSPredcount(); //chou ++ 2013/05/17 紅燈倒數啟動

                    //OT20131219
                    smem.vSendPedAndRedCountEverySec();  //送聯嘉 紅燈倒數 行人倒數

                    //jacky20140424
                    numtime = smem.GetBF11Time(); //單位：秒
                    //printf("\n\n!! %d !!\n\n",numtime);
                    if(smem.GetBF11onoff() == 1 && numtime==0) Permanent = true;
                    else Permanent = false;

                    if(smem.GetBF11onoff() == 0)
                    {
                        smem.SetPriorityswitch(false);
                        smem.SetBF11onoff(2);
                        smem.SetCompensationcycle(2);   //SET two cycle
                    }
                    else if(smem.GetBF11onoff() == 1 || smem.GetDefiniteTimeonoff() == 1)
                    {
                        if(ucCSTCControlStrategy != 70)
                            smem.SetPriorityswitch(true);
                        smem.SetCompensationcycle(1); //SET one cycle
                        if(numtime > 0)
                        {
                            numtime--;
                            smem.SetBF11Time(numtime);
                            if (numtime <= 0 && Permanent == false)
                            {
                                smem.SetBF11onoff(2);
                                smem.SetPriorityswitch(false);
                                smem.SetCompensationcycle(2);
                            }
                        }
                    }
                    else
                    {
                        smem.SetPriorityswitch(false);
                        smem.SetBF11onoff(2);
                        smem.SetCompensationcycle(2);
                    }

                    //jacky20141202 shutdown PGPR Count
                    if(smem.GetLearningPGPRsubphase())
                    {
                        if(LearningPGPRThree==3)
                        {
                            smem.SetLearningPGPRThreeSecondoff(true);
                            smem.SetLearningPGPRsubphase(false);
                            LearningPGPRThree = 0;
                            stc.SetLightAfterExtendTimerReSet();
                        }
                        else
                            LearningPGPRThree++;
                    }
                    if(smem.GetLearningPGPROneSecondon())
                    {
                        if(LearningPGPROne==1)
                        {
                            smem.SetLearningPGPROneSecondon(false);
                            smem.SetLearningPGPRThreeSecondoff(false);
                            LearningPGPROne = 0;
                            stc.SetLightAfterExtendTimerReSet();
                        }
                        else
                            LearningPGPROne++;
                    }

                    //jacky20141227 Clear BF0B
                    if(smem.GetClearBF0BEast() && ClearBF0BEast == 0)
                    {
                        ClearBF0BEast++;
                        smem.SetClearBF0BEast(false);
                    }
                    else if(smem.GetClearBF0BEast() && ClearBF0BEast != 0)
                    {
                        ClearBF0BEast = 0;
                    }
                    else if(~(smem.GetClearBF0BEast()) && ClearBF0BEast != 0)
                    {
                        ClearBF0BEast++;
                        if(ClearBF0BEast >= 10)
                        {
                            ClearBF0BEast = 0;
                            stc.ClearBF0B(1);
                        }
                    }
                    if(smem.GetClearBF0BWest() && ClearBF0BWest == 0)
                    {
                        ClearBF0BWest++;
                        smem.SetClearBF0BWest(false);
                    }
                    else if(smem.GetClearBF0BWest() && ClearBF0BWest != 0)
                    {
                        ClearBF0BWest = 0;
                    }
                    else if(~(smem.GetClearBF0BWest()) && ClearBF0BWest != 0)
                    {
                        ClearBF0BWest++;
                        if(ClearBF0BWest >= 10)
                        {
                            ClearBF0BWest = 0;
                            stc.ClearBF0B(0);
                        }
                    }

                    if(now->tm_hour == 0 && now->tm_min == 0)
                        smem.SetPriorityexecute(1);

                    if(smem.vGetBOOLData(TCDynSegNextPlanIdSendToCCJ) == true)
                    {
                        stc.CheckDynSegAndSendNextPlanId();
                        smem.vSetBOOLData(TCDynSegNextPlanIdSendToCCJ, false);
                    }

                    if(smem.GetRequestKeypad() == 1)
                    {
                        if(RequestKeypad < 5)
                        {
                            SendRequestToKeypad();
                            RequestKeypad++;
                        }
                        else if(RequestKeypad >= 5)
                        {
                            smem.SetRequestKeypad(0);
                            RequestKeypad = 0;
                            stc.Lock_to_Set_Control_Strategy(STRATEGY_TOD);
                        }
                    }
                    else if(smem.GetRequestKeypad() == 0)
                    {
                        RequestKeypad = 0;
                    }

                    break;
                case( 11 ):
//Remove  _intervalTimer.vCheckAndReSendSS_S0_Status();
                    smem.vCheckConnectStatus();

                    //OT20110526
                    iMinCycleTime = 600;

                    ucTmp = smem.vGetHWCycleCodeFor_0F14_0FC4();
                    switch (ucTmp)
                    {
                    case(0):  //means stop
                        iTmp = 600;
                        break;
                    case(1):
                        iTmp = 1;
                        break;
                    case(2):
                        iTmp = 2;
                        break;
                    case(3):
                        iTmp = 5;
                        break;
                    case(4):
                        iTmp = 60;
                        break;
                    case(5):
                        iTmp = 300;
                        break;
                    }
                    if(iTmp > 0 && iTmp < iMinCycleTime)
                    {
                        iMinCycleTime = iTmp;
                    }

                    iTmp = smem.vGetINTData(TC92SignalLightStatus_5F0F_IntervalTime);
                    if(iTmp == 0xFF)    //stop send
                    {
                        iTmp = 600;
                    }
                    if(iTmp > 0 && iTmp < iMinCycleTime)
                    {
                        iMinCycleTime = iTmp;
                    }

                    iTmp = smem.vGetINTData(TC92SignalStepStatus_5F03_IntervalTime);
                    if(iTmp == 0xFF)    //stop send
                    {
                        iTmp = 600;
                    }
                    if(iTmp > 0 && iTmp < iMinCycleTime)
                    {
                        iMinCycleTime = iTmp;
                    }
                    iMinCycleTime += 2;  //set delay time.

                    tmpTime = smem.vGetLastGetProtocolTime();
                    if(currentTime >= tmpTime && (currentTime - tmpTime) <= iMinCycleTime)
                    {
                        smem.vSaveCenterConnectStatus(true);
                    }
                    else
                    {
                        smem.vSaveCenterConnectStatus(false);
                    }

                    //OT20110825
                    tmpTime = smem.vGetLastTrafficeLightTime();
//                                  printf("currentTime - tmpTime:%d\n", currentTime - tmpTime);
                    if(tmpTime >= currentTime)    //log time > system time
                    {
                        smem.vSetTrafficLightBoardConnectStatus(true);
                    }
                    else
                    {
                        if( (currentTime - tmpTime) <= 60)
                        {
                            smem.vSetTrafficLightBoardConnectStatus(true);
                        }
                        else
                        {
                            smem.vSetTrafficLightBoardConnectStatus(false);
                        }
                    }


                    break;
                case( 12 ):
//Remove                                   _intervalTimer.vCheckAndReSendSS_SK_Status();
                    _intervalTimer.vCheckScreenAndDoSomething();

//Remove                                    _SSLastRun = smem.vGetSSCommTime();         //檢�\uFFFDSmartSensor
                    _RunSec = time(NULL);
//                                    bConnectCenterStatus = smem.vLoadCenterConnectStatus();
//                                    if(bConnectCenterStatus)  usiTmp = smem.vGet92VDTransmitType(1);
//                                    else usiTmp = smem.vGet92VDTimeGap();
//                                    if( ((_RunSec - _SSLastRun) > usiTmp*2) && (_RunSec > _SSLastRun) )
//                                      smem.SetSSComm(false);

                    _intervalTimer.vCheckVDUpdateDBAndDoSomething();

                    if((_RunSec - smem.GetLastKeypadTime()) > 300)    //TEST, 990325 for amegids, don't clear screen
                    {
                        screenNone.DisplayNone();
                    }

//                                    if (smem.GetKeypadReturn()==false)  SendRequestToKeypad();
                    SendRequestToKeypad();

                    break;
                case( 13 ):                                                           //VD SIM
//                                   _MSG = oDataToMessageOK.vPackageINFOTo92Protocol(uc6F00, 2,false);
//                                   _MSG.InnerOrOutWard = cInner;
//                                   writeJob.WriteWorkByMESSAGEOUT(_MSG);
                    smem.cPedPushButton.vCheckQueueMsgForWriteOut();

                    break;
                case( 14 ):
//                                     _intervalTimer.vSendHeartBeatToLCX405();
//                                   _intervalTimer.vSSInit(SSInitCount);
//                                   SSInitCount++;
                    break;


                case( 15 ):  //0F04, HwStatus AutoReport
                    uc0F04[2] = smem.vGetHardwareStatus(3);
                    uc0F04[3] = smem.vGetHardwareStatus(4);
                    _MSG = oDataToMessageOK.vPackageINFOTo92Protocol(uc0F04, 4, true);
                    _MSG.InnerOrOutWard = cOutWard;
                    writeJob.WritePhysicalOut(_MSG.packet, _MSG.packetLength, DEVICECENTER92);

                    stc.BRTGPSStatusReport();  //jacky20141203

                    smem.cPedPushButton.SendPedSWConnetState0F08(0);
                    break;

                case( 100 ):

                    _intervalTimer.vCommuncationReset(iCommuncationResetCount);                        //default not start
                    iCommuncationResetCount++;
                    if(iCommuncationResetCount >= 3) iCommuncationResetCount = 0;
                    break;

                case( 101 ):
                    _intervalTimer.vDBLockRequest(iDBLockCount);                        //default not start
                    iDBLockCount++;
                    if(iDBLockCount >= 6)                        //TimeOut
                    {
                        iDBLockCount = 0;
                        screenOnline.RequestTimeOut();
                    }
                    if(smem.GetDbOperStat()!=1 || smem.GetcFace()!=cONLINE)
                    {
                        iDBLockCount = 0;
                        _intervalTimer.vDBLockRequest(10);
                    }
                    break;

                    /*
                                                  case( 500 ):                                      //\uFFFD��\uFFFD\uFFFD\uFFFD\uFFFDTOD

                                                       memset(msg,0,sizeof(msg));

                                                       usiCurrentSubphaseStep = stc.vGetUSIData(CSTC_exec_phase_current_subphase_step);
                                                       printf("printfMsg [In Dyn] currentStep:%d %d\n", usiCurrentSubphaseStep, Protocal5F1CStopStep);
                                                       sprintf(msg,"printfMsg [In Dyn] currentStep:%d %d", usiCurrentSubphaseStep, Protocal5F1CStopStep);

                                                       //smem.vWriteMsgToDOM(msg);
                                                       memset(msg,0,sizeof(msg));

                                                       if((usiCurrentSubphaseStep == 0 || usiCurrentSubphaseStep == 2) && usiCurrentSubphaseStep == Protocal5F1CStopStep){
                                                         if( stc.Lock_to_LoadControlStrategy() == STRATEGY_ALLDYNAMIC ) {
                                                        // bereplace stc.Lock_to_Set_Next_Step();
                                                         stc.vSetNextStepNotUseSignal();
                                                           usleep(100);

                                                           printf("Lock_to_Set_Control_Strategy by Timer!!!\n");

                                                           stc.Lock_to_Set_Control_Strategy(STRATEGY_TOD);

                                                           sprintf(msg,"[Debug] Change to TOD at CTIMER.cpp, currentStep:%d", usiCurrentSubphaseStep);
                                                           smem.vWriteMsgToDOM(msg);
                                                           memset(msg,0,sizeof(msg));

                                                           _ControlStrategy.DBit = smem.vGetUCData(TC92_ucControlStrategy);
                                                           _ControlStrategy.switchBit.b1 = true;
                                                           _ControlStrategy.switchBit.b5 = false;
                                                           _ControlStrategy.switchBit.b6 = false;

                                                           smem.vSetUCData(TC92_ucControlStrategy, _ControlStrategy.DBit);
                                                           smem.vSetINTData(TC92_iEffectTime, 0);
                                                         }//if( stc.Lock_to_LoadControlStrategy() == STRATEGY_ALLDYNAMIC )
                                                       }//if((usiCurrentSubphaseStep == 0 || usiCurrentSubphaseStep == 2) && usiCurrentSubphaseStep == Protocal5F1CStopStep)
                                                       else {

                                                                smem.vSetBOOLData(TC_SIGNAL_NEXT_STEP_OK, false);
                                                         // bereplace stc.Lock_to_Set_Next_Step();
                                                         stc.vSetNextStepNotUseSignal();

                                                         //usleep(1000);
                                                         //usleep(50);
                                                         printf("--------------------------------------------------------\n");

                    //                                     for(int ii = 0; ii < 2000; ii++) {
                    //                                              if(smem.vGetBOOLData(TC_SIGNAL_NEXT_STEP_OK) == true){
                    //                                                smem.vSetBOOLData(TC_SIGNAL_NEXT_STEP_OK, false);
                    //                                                printf("OK finish CSTC NEXT_STEP\n");
                    //                                                //ii = 100;//100,000
                    //                                                ii=2000;
                    //                                                break;
                    //                                              }//if(smem.vGetBOOLData(TC_SIGNAL_NEXT_STEP_OK) == true)
                    //                                                     printf("\nCTIMER ii = %d\n\n", ii);
                    //                                                      //usleep(1000);
                    //                                                      usleep(50);
                    //                                             }//for(int ii = 0; ii < 100; ii++)

                                                         usiCurrentSubphaseStepN = stc.vGetUSIData(CSTC_exec_phase_current_subphase_step);
                                                         printf("\nusiCurrentSubphaseStep %d usiCurrentSubphaseStepN %d\n",usiCurrentSubphaseStep, usiCurrentSubphaseStepN);

                                                         if(usiCurrentSubphaseStepN == 0) {
                                                           if(Protocal5F1CStopStep == usiCurrentSubphaseStepN) {
                                                                  iTmp = smem.vGetINTData(TC92_iEffectTime);
                                                                   printf("CTIMER usiCurrentSubphaseStepN = 0 effectime t=%d\n", iTmp);
                                                                   sprintf(msg,"CTIMER usiCurrentSubphaseStepN = 0 effectime t=%d", iTmp);
                                                                   if(iTmp > 10) _intervalTimer.vAllDynamicToTODCount(iTmp);
                                                                   else _intervalTimer.vAllDynamicToTODCount(120);  //error happened
                                                           }//if(Protocal5F1CStopStep == usiCurrentSubphaseStepN)
                                                           else {
                                                             //OT20111020 fix
                                                                   iTmp = stc.vGetUSIData(CSTC_exec_plan_green_time);
                                                                   printf("CTIMER usiCurrentSubphaseStepN = 0 TOD t= %d\n",iTmp);
                                                                   sprintf(msg,"CTIMER usiCurrentSubphaseStepN = 0 TOD t= %d",iTmp);
                                                                   if(iTmp > 0) _intervalTimer.vAllDynamicToTODCount(iTmp);
                                                                   else _intervalTimer.vAllDynamicToTODCount(1);
                                                           }//if(Protocal5F1CStopStep == usiCurrentSubphaseStepN)
                                                         }//if(usiCurrentSubphaseStepN == 0)
                                                         else if(usiCurrentSubphaseStepN == 1) {
                                                                  iTmp = stc.vGetUSIData(CSTC_exec_plan_pedgreenflash_time);
                                                                  printf("CTIMER usiCurrentSubphaseStepN = 1 t=%d\n", iTmp);
                                                                  sprintf(msg,"CTIMER usiCurrentSubphaseStepN = 1 t=%d", iTmp);
                                                                  if(iTmp > 0) _intervalTimer.vAllDynamicToTODCount(iTmp);
                                                                  else _intervalTimer.vAllDynamicToTODCount(1);
                                                         }//if(usiCurrentSubphaseStepN == 1)
                                                         else if(usiCurrentSubphaseStepN == 2) {
                                                           if(Protocal5F1CStopStep == usiCurrentSubphaseStepN) {
                                                                   iTmp = smem.vGetINTData(TC92_iEffectTime);
                                                                   printf("CTIMER usiCurrentSubphaseStepN = 2 t=%d\n", iTmp);
                                                                   sprintf(msg,"CTIMER usiCurrentSubphaseStepN = 2 t=%d", iTmp);
                                                                   if(iTmp > 10) _intervalTimer.vAllDynamicToTODCount(iTmp);
                                                                   else _intervalTimer.vAllDynamicToTODCount(120);
                                                           }//if(Protocal5F1CStopStep == usiCurrentSubphaseStepN)
                                                           else {
                                                                   iTmp = stc.vGetUSIData(CSTC_exec_plan_pedred_time);
                                                                   printf("CTIMER usiCurrentSubphaseStepN = 2 t=%d\n", iTmp);
                                                                   sprintf(msg,"CTIMER usiCurrentSubphaseStepN = 2 t=%d", iTmp);
                                                             if(iTmp > 0) _intervalTimer.vAllDynamicToTODCount(iTmp);
                                                             else _intervalTimer.vAllDynamicToTODCount(1);
                                                           }//if(Protocal5F1CStopStep == usiCurrentSubphaseStepN)
                                                         }//if(usiCurrentSubphaseStepN == 2)
                                                         else if(usiCurrentSubphaseStepN == 3) {
                                                           iTmp = stc.vGetUSIData(CSTC_exec_plan_yellow_time);
                                                           printf("CTIMER usiCurrentSubphaseStepN = 3 t=%d\n", iTmp);
                                                           sprintf(msg,"CTIMER usiCurrentSubphaseStepN = 3 t=%d",iTmp);
                                                           if(iTmp > 0) _intervalTimer.vAllDynamicToTODCount(iTmp);
                                                           else _intervalTimer.vAllDynamicToTODCount(3);
                                                         }//else if(usiCurrentSubphaseStepN == 3)
                                                         else if(usiCurrentSubphaseStepN == 4) {
                                                           iTmp = stc.vGetUSIData(CSTC_exec_plan_allred_time);
                                                           printf("CTIMER usiCurrentSubphaseStepN = 4 t=%d\n", iTmp);
                                                           sprintf(msg,"CTIMER usiCurrentSubphaseStepN = 4 t=%d", iTmp);
                                                           if(iTmp > 0) _intervalTimer.vAllDynamicToTODCount(iTmp);
                                                           else _intervalTimer.vAllDynamicToTODCount(3);
                                                         }//else if(usiCurrentSubphaseStepN == 4)
                                                         else{
                                                           printf("CTIMER usiCurrentSubphaseStepN != [0-4] t=1\n");
                                                           sprintf(msg,"CTIMER usiCurrentSubphaseStepN != [0-4] t=1");
                                                           _intervalTimer.vAllDynamicToTODCount(1);
                                                         }//else (usiCurrentSubphaseStepN != [0-4])
                                                         printf("--------------------------------------------------------\n");
                                                         //BT9512260001 START
                                                         //smem.vWriteMsgToDOM(msg);
                                                         //BT9512260001 END
                                                       }
                                                  break;
                    */

                case( 500 ):                                      //\uFFFDï¿½ï¿½\uFFFD\uFFFD\uFFFD\uFFFDTOD,
                    //vAllDynamicToTODCount()

                    memset(msg,0,sizeof(msg));

                    usiCurrentSubphaseStep = stc.vGetUSIData(CSTC_exec_phase_current_subphase_step);
                    printf("printfMsg [In Dyn] currentStep:%d %d\n", usiCurrentSubphaseStep, Protocal5F1CStopStep);
                    sprintf(msg,"printfMsg [In Dyn] currentStep:%d %d", usiCurrentSubphaseStep, Protocal5F1CStopStep);

                    //smem.vWriteMsgToDOM(msg);
                    memset(msg,0,sizeof(msg));

                    if( stc.Lock_to_LoadControlStrategy() == STRATEGY_ALLDYNAMIC )
                    {
                        // bereplace stc.Lock_to_Set_Next_Step();
                        // stc.vSetNextStepNotUseSignal();
                        // usleep(100);

                        printf("Lock_to_Set_Control_Strategy by Timer!!!\n");

                        stc.Lock_to_Set_Control_Strategy(STRATEGY_TOD);

                        sprintf(msg,"[Debug] Change to TOD at CTIMER.cpp, currentStep:%d", usiCurrentSubphaseStep);
                        smem.vWriteMsgToDOM(msg);
                        memset(msg,0,sizeof(msg));

                        _ControlStrategy.DBit = smem.vGetUCData(TC92_ucControlStrategy);
                        _ControlStrategy.switchBit.b1 = true;
                        _ControlStrategy.switchBit.b5 = false;
                        _ControlStrategy.switchBit.b6 = false;

                        smem.vSetUCData(TC92_ucControlStrategy, _ControlStrategy.DBit);
                        smem.vSetINTData(TC92_iEffectTime, 0);
                    }//if( stc.Lock_to_LoadControlStrategy() == STRATEGY_ALLDYNAMIC )
                    printf("--------------------------------------------------------\n");
                    //BT9512260001 START
                    //smem.vWriteMsgToDOM(msg);
                    //BT9512260001 END
                    break;


//OTSS
                case( 501 ):
                    sprintf(msg, "Ask W77E58 Version, booting version year is :%d", smem.vGetW77E58FWVer(0));
                    smem.vWriteMsgToDOM(msg);
                    memset(msg,0,sizeof(msg));

//OT20110825, move out
//OT20111026, coming back
                    if(smem.vGetW77E58FWVer(0) == 0)
                    {
                        writeJob.WriteAskW77E58FWVer();                 //ask W77e58 Fw
//                                  _it10.it_value.tv_sec = 0;                      //clear time.
                    }
                    break;

                case( 600 ):
                    _intervalTimer.vSendHeartBeatToLCX405();
                    /* OT20111026
                    writeJob.WriteAskW77E58FWVer();                 //ask W77e58 Fw, OT20110825
                    */
                    break;
                case( 601 ):
                    smem.cPedPushButton.QueryPEDState();//add Arwen
                    break;

                    /*   case(602):
                             if(ucCSTCControlStrategy == 70)
                                 stc.Lock_to_Set_Next_Step();

                             printf("\n------- TEST -------\n");
                             printf("\n------- TEST -------\n");
                             printf("\n------- TEST -------\n");
                             printf("\n------- TEST -------\n");
                             printf("\n------- TEST -------\n");
                       break;
                     */
                default:
                    perror("CSTC TIMERID: error!!");
                    //OTMARKPRINTF  printf("VDrid: \n", VDrid);
                }
                break;


            case( RTSIGNAL_RECODE_SMEM ):
                smem.vSaveShareMemoryDataToDOM();
                break;

            default:
                break;

            }
            smem.vSetTimerMutexCTIMER(0);


        }
    }
    catch (...) {}
}
//--------------------------------------------------------------------------
bool intervalTimer::GetDoorStatus(void)
{
    try
    {

        unsigned char uc0F04[4];
        bool bShouldSendMsg = false;
        bool bCmpRet;

        DATA_Bit tempDIOByte=smem.Get_DIOByte();
        DATA_Bit tempOldDIOByte = smem.Get_OldDIOByte();

        smem.vSetUCData(TC_GreenConflictDetFromDIO, tempDIOByte.DBit);
        bCmpRet = smem.vCmpGreenConflict();

        if(bCmpRet == false )
        {
            usiGreenConflictCount++;
        }
        else
        {
            usiGreenConflictCount = 0;
        }

        if(usiGreenConflictCount > 30000) usiGreenConflictCount = 0;

        if(SignalConflictVersion)
        {
            if(usiGreenConflictCount > 2)
            {
//      bShouldSendMsg = true;
                smem.vSetBOOLData(TC_SignalConflictError, true);

                printf("Lock_to_Set_Control_Strategy by usiGreenConflictCount!!!\n");
                printf("Lock_to_Set_Control_Strategy by usiGreenConflictCount!!!\n");
                printf("Lock_to_Set_Control_Strategy by usiGreenConflictCount!!!\n");


                stc.Lock_to_Set_Control_Strategy(STRATEGY_FLASH);
            }
        }

        if (usiGreenConflictCount == 3)
        {

            uc0F04[0] = 0x0F;
            uc0F04[1] = 0x04;
            MESSAGEOK _MSG;
            //send HW Status
            uc0F04[2] = smem.vGetHardwareStatus(3);
            uc0F04[3] = smem.vGetHardwareStatus(4);
            _MSG = oDataToMessageOK.vPackageINFOTo92Protocol(uc0F04, 4, true);
            _MSG.InnerOrOutWard = cOutWard;
            writeJob.WritePhysicalOut(_MSG.packet, _MSG.packetLength, DEVICECENTER92);

            return true;
        }

        if(smem.vGetBOOLData(TC_SignalConflictError) == true)
        {
            uc0F04[0] = 0x0F;
            uc0F04[1] = 0x04;
            MESSAGEOK _MSG;
            //send HW Status
            uc0F04[2] = smem.vGetHardwareStatus(3);
            uc0F04[3] = smem.vGetHardwareStatus(4);
            _MSG = oDataToMessageOK.vPackageINFOTo92Protocol(uc0F04, 4, true);
            _MSG.InnerOrOutWard = cOutWard;
            writeJob.WritePhysicalOut(_MSG.packet, _MSG.packetLength, DEVICECENTER92);

            return true;

        }

        return false;
    }
    catch(...) {}
}

//--------------------------------------------------------------------------
bool intervalTimer::vChildChain(void)
{
    try
    {
        static time_t now;

        static struct tm* basetime;

        static struct tm* currenttime;

        static int iChainSignalNotChange;

        sChildChain sCCTMP;
        long lCycleTime;

        bool bChildChainStatus;
        bool bChildChainStatusOld;
        unsigned char ucMBType;

        DATA_Bit tempDIOByte = smem.Get_DIOByte();
//  printf("DIO:%X\n", tempDIOByte.DBit);
        DATA_Bit tempOldDIOByte = smem.Get_OldDIOByte();

        sCCTMP = smem.vGetChildChainStruct();
        now = time(NULL);
//  printf(" now:%d\n", now);

        ucMBType = smem.vGetMotherBoardType();

        if(ucMBType == MB_ICOP6115)
        {
            bChildChainStatus = tempDIOByte.switchBit.b2;
            bChildChainStatusOld = tempOldDIOByte.switchBit.b2;
        }
        else
        {
            bChildChainStatus = tempDIOByte.switchBit.b1;
            bChildChainStatusOld = tempOldDIOByte.switchBit.b1;
        }

        if(bChildChainStatus != bChildChainStatusOld )                  //if status have change.
        {
            sCCTMP = smem.vGetChildChainStruct();
            now = time(NULL);
            printf(" now:%d\n", now);
            currenttime = localtime(&now);
            if(bChildChainStatus == TC_CHAIN_SIGNAL_IN_START_SUB_PAHSE)          //get StartLock Signal
            {
                printf("GetChildChain Start Time: %2d:%2d:%2d\n", currenttime->tm_hour, currenttime->tm_min, currenttime->tm_sec);
                printf("GetChildChain Start Time: %2d:%2d:%2d\n", currenttime->tm_hour, currenttime->tm_min, currenttime->tm_sec);
                printf("GetChildChain Start Time: %2d:%2d:%2d\n", currenttime->tm_hour, currenttime->tm_min, currenttime->tm_sec);
                sCCTMP.oldStartTime = sCCTMP.newStartTime;
                sCCTMP.newStartTime = now;

                lCycleTime = sCCTMP.newStartTime - sCCTMP.oldStartTime;
                if(sCCTMP.oldStartTime != 0 && lCycleTime > 0 && lCycleTime < 300)        //300 means max cycle for protect
                {
                    sCCTMP.iEndKeepTime = sCCTMP.newStartTime - sCCTMP.newEndTime;
                    sCCTMP.iChainCycle = lCycleTime;
                }
                else                                                                      //error signal cycle
                {
                    sCCTMP.iEndKeepTime = 0;
                }
            }
            else                                                                        //get EndLock Signal (TC_CHAIN_SIGNAL_IN_END_SUB_PAHSE)
            {
                printf("GetChildChain End Time: %2d:%2d:%2d\n", currenttime->tm_hour, currenttime->tm_min, currenttime->tm_sec);
                printf("GetChildChain End Time: %2d:%2d:%2d\n", currenttime->tm_hour, currenttime->tm_min, currenttime->tm_sec);
                printf("GetChildChain End Time: %2d:%2d:%2d\n", currenttime->tm_hour, currenttime->tm_min, currenttime->tm_sec);
                sCCTMP.oldEndTime = sCCTMP.newEndTime;
                sCCTMP.newEndTime = now;

                lCycleTime = sCCTMP.newEndTime - sCCTMP.oldEndTime;
                if(sCCTMP.oldEndTime != 0 && lCycleTime > 0 && lCycleTime < 300)          //300 means max cycle for protect
                {
                    sCCTMP.iStartKeepTime = sCCTMP.newEndTime - sCCTMP.newStartTime;
                    sCCTMP.iChainCycle = lCycleTime;
                }
                else                                                                      //error signal cycle
                {
                    sCCTMP.iStartKeepTime = 0;
                }

            }

            if(sCCTMP.iStartKeepTime != 0 && sCCTMP.iEndKeepTime != 0)                  //Check value reasonable
            {
                sCCTMP.bHaveReasonableChildChainSignal = true;                            //get right chain signal
                printf("GetChildChainbHaveReasonableChildChainSignal\n");
                printf("GetChildChainbHaveReasonableChildChainSignal\n");
                printf("GetChildChainbHaveReasonableChildChainSignal\n");
            }
            else
            {
                sCCTMP.bHaveReasonableChildChainSignal = false;
            }

            smem.vSetChildChainStruct(sCCTMP);
            iChainSignalNotChange = 0;

        }
        else
        {
            iChainSignalNotChange++;
        }

        if(iChainSignalNotChange > 600)
        {
            smem.vClearChildChainStruct();
        }

        return true;
    }
    catch(...) {}
}

//--------------------------------------------------------------------------
bool intervalTimer::vCheckSMEM(void)
{
    try
    {
//    if( smem.vGetSimIntervalTimeChangeStatus() ) { //for VD SimDataSend
//        _it4 = smem.vReadSimIntervalTime();

//      //OTMARKPRINTF  printf("[Message] Change Sim Send Interval:%d\n", _it4.it_interval.tv_sec);

//        if ( timer_settime( _t4, 0, & _it4, NULL ) ) exit( 1 );
//        smem.vSetSimIntervalTimeChangeStatus(false);
//    }
        if( smem.vGet0FHardwareCycleChangeStatus() )
        {
            _it6 = smem.vGet0FHardwareCycle();

            //OTMARKPRINTF  printf("[Message] Change HWStatus Send Interval:%d\n", _it6.it_interval.tv_sec);

            if ( timer_settime( _t6, 0, & _it6, NULL ) ) exit( 1 );
            smem.vSet0FHardwareCycleChangeStatus(false);
        }
        if( smem.vGet0F11CommuncationResetChangeStatus() )
        {
            _it7.it_value.tv_sec = 0;
            _it7.it_value.tv_nsec = 500;
            _it7.it_interval.tv_sec = 3;
            _it7.it_interval.tv_nsec = 0;
            //OTMARKPRINTF  printf("[Message] StartCommuncationReset!\n");
            if ( timer_settime( _t7, 0, & _it7, NULL ) ) exit( 1 );
            smem.vSet0F11CommuncationResetChangeStatus(false);
        }
        if( smem.vGet0F16LockDBRequestStatus() )
        {
            _it8.it_value.tv_sec = 1;
            _it8.it_value.tv_nsec = 0;
            _it8.it_interval.tv_sec = 30;
            _it8.it_interval.tv_nsec = 0;
            //OTMARKPRINTF  printf("[Message] Start Request LockDB!\n");
            if ( timer_settime( _t8, 0, & _it8, NULL ) ) exit( 1 );
            smem.vSet0F16LockDBRequestStatus(false);
        }

        return true;
    }
    catch(...) {}
}

//--------------------------------------------------------------------------
//**********************************************************
//      Read and Reset Timers for Resetting CMOS Time
//**********************************************************
//--------------------------------------------------------------------------
bool intervalTimer::TimersRead_BeforeResetCMOSTime(void)
{
    try
    {

        pthread_mutex_lock(&intervalTimer::_ctimer_mutex);
        struct itimerspec _itZero;

        _itZero.it_value.tv_sec = 0;
        _itZero.it_value.tv_nsec = 0;
        _itZero.it_interval.tv_sec = 0;
        _itZero.it_interval.tv_nsec = 0;

//     while(smem.vGetTimerMutexCTIMER() == true) { }
//     smem.vSetTimerMutexCTIMER(true);
        /*
          if(timer_gettime(_t1,&_it1)) return false;
          if(timer_gettime(_t2,&_it2)) return false;
          if(timer_gettime(_t3,&_it3)) return false;
          if(timer_gettime(_t4,&_it4)) return false;
          if(timer_gettime(_t5,&_it5)) return false;
          if(timer_gettime(_t6,&_it6)) return false;
          if(timer_gettime(_t7,&_it7)) return false;
          if(timer_gettime(_t8,&_it8)) return false;
        */
        if(timer_gettime(_t9,&_it9)) return false;

        timer_settime(_t1, 0, &_itZero, NULL);
        timer_settime(_t2, 0, &_itZero, NULL);
        timer_settime(_t3, 0, &_itZero, NULL);
        timer_settime(_t4, 0, &_itZero, NULL);
        timer_settime(_t5, 0, &_itZero, NULL);
        timer_settime(_t6, 0, &_itZero, NULL);
        timer_settime(_t7, 0, &_itZero, NULL);
        timer_settime(_t8, 0, &_itZero, NULL);
        timer_settime(_t9, 0, &_itZero, NULL);
        timer_settime(_t10, 0, &_itZero, NULL);
        timer_settime(_tTrafficeLight, 0, &_itZero, NULL);
        //timer_settime(_tBRTLightctrl, 0, &_itZero, NULL);

        pthread_mutex_unlock(&intervalTimer::_ctimer_mutex);

        return true;
    }
    catch(...) {}
}
//--------------------------------------------------------------------------
bool intervalTimer::TimersReset_AfterResetCMOSTime(void)
{
    try
    {

        pthread_mutex_lock(&intervalTimer::_ctimer_mutex);

        for(int i = 0; i < 4; i++)
        {

            TimersSetting();
            /*
            if(timer_settime(_t1, 0, &_it1, NULL)) return false;
            if(timer_settime(_t2, 0, &_it2, NULL)) return false;
            if(timer_settime(_t3, 0, &_it3, NULL)) return false;
            if(timer_settime(_t4, 0, &_it4, NULL)) return false;
            if(timer_settime(_t5, 0, &_it5, NULL)) return false;
            if(timer_settime(_t6, 0, &_it6, NULL)) return false;
            if(timer_settime(_t7, 0, &_it7, NULL)) return false;
            if(timer_settime(_t8, 0, &_it8, NULL)) return false;
            */
            if(timer_settime(_t9, 0, &_it9, NULL)) return false;

//    smem.vSetTimerMutexCTIMER(false);

        }

        pthread_mutex_unlock(&intervalTimer::_ctimer_mutex);

        return true;
    }
    catch(...) {}
}

//-------------------------------------------------------------------------
bool intervalTimer::vCheckScreenAndDoSomething(void)
{
    try
    {

        if( smem.GetcFace() == 0)
        {
            return true;
        }
        time_t NowSec = time(NULL);
//  time_t _TmpLastPress =smem.Get???

//  if( (NowSec - _TmpLastPress > 150)
        {
            //change cfase
        }
        return true;

    }
    catch(...) {}
}
//-------------------------------------------------------------------------
bool intervalTimer::vCheckVDUpdateDBAndDoSomething(void)
{
    try
    {

//OT Debug 0410
//change for TC

        if( smem.vGetVDUpdateDB(888) )
        {
            if( smem.GetConnectCenterStatus() )
            {
                unsigned char data[4];
                data[2] = 0;
                data[2] = data[2] + smem.vGetVDUpdateDB(0);
                data[2] = data[2] + smem.vGetVDUpdateDB(1)*2;
                data[2] = data[2] + smem.vGetVDUpdateDB(2)*4;
//OT Debug 951128
                data[2] = data[2] + smem.vGetVDUpdateDB(3)*8;

                data[0] = 0x5F;
                data[1] = 0x0A;

                data[3] = 0xFF;

                MESSAGEOK _MSG;
                _MSG = oDataToMessageOK.vPackageINFOTo92Protocol(data, 4,false);
                _MSG.InnerOrOutWard = cInner;
                writeJob.WriteWorkByMESSAGEOUT(_MSG);

                for(int i = 0; i < 4; i++)
                    smem.vSetVDUpdateDB(i, false);
            }
            smem.vSetVDUpdateDB(888, false);                                          // send to center OK.

        }
        return true;
    }
    catch(...) {}
}
//--------------------------------------------------------------------------

bool intervalTimer::vCommuncationReset(int iTMP)
{
    try
    {
        unsigned char data[2];
        data[0] = 0x0F;
        data[1] = 0x91;

        switch(iTMP)
        {
        case (0):
            MESSAGEOK _MsgOK;
            _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, 2,true);
            _MsgOK.InnerOrOutWard = cOutWard;
            writeJob.WriteWorkByMESSAGEOUT(_MsgOK);

            digitalIO.WriteDigitalIO(0xFF);
            break;
        case (1):
            digitalIO.WriteDigitalIO(0x00);
            smem.vSetTimeInterVal(60, INVAILDVALUE);
//             smem.vSet92VDTransmitType(1, 60);
            break;

        default:
            _it7.it_value.tv_sec = 0;
            _it7.it_value.tv_nsec = 0;
            _it7.it_interval.tv_sec = 0;
            _it7.it_interval.tv_nsec = 0;
            if ( timer_settime( _t7, 0, & _it7, NULL ) ) exit( 1 );

            break;
        }
    }
    catch(...) {}
}

//--------------------------------------------------------------------------
bool intervalTimer::vDBLockRequest(int iTMP)
{
    try
    {
        unsigned char data[2];
        data[0] = 0x0F;
        data[1] = 0x06;
        MESSAGEOK _MsgOK;

        switch(iTMP)
        {
        case (0):
        case (1):
        case (2):
        case (3):
        case (4):
            _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, 2,true);
            _MsgOK.InnerOrOutWard = cOutWard;
            writeJob.WriteWorkByMESSAGEOUT(_MsgOK);

            if(smem.GetDbOperStat() != 1)
            {
                _it8.it_value.tv_sec = 0;
                _it8.it_value.tv_nsec = 0;
                _it8.it_interval.tv_sec = 0;
                _it8.it_interval.tv_nsec = 0;
                if ( timer_settime( _t8, 0, & _it8, NULL ) ) exit( 1 );
            }
            break;
        default:
            _it8.it_value.tv_sec = 0;
            _it8.it_value.tv_nsec = 0;
            _it8.it_interval.tv_sec = 0;
            _it8.it_interval.tv_nsec = 0;
            if ( timer_settime( _t8, 0, & _it8, NULL ) ) exit( 1 );

            break;
        }
    }
    catch(...) {}
}

//--------------------------------------------------------------------------
bool intervalTimer::vAllDynamicToTODCount(unsigned short int siTMP)
{
    try
    {
        _it9.it_value.tv_sec = siTMP;
        /* ot add 960802 */
        _it9.it_value.tv_nsec = 0;
        _it9.it_interval.tv_sec = 0;
        _it9.it_interval.tv_nsec = 0;

        if ( timer_settime( _t9, 0, & _it9, NULL ) ) exit( 1 );

    }
    catch(...) {}
}

//----------------------------------------------------------
unsigned short int intervalTimer::vGetEffectTime(void)
{
    try
    {

        //Should be mutex

        unsigned short int time_difference=0;
        timer_gettime(_t9,&_it9);
        time_difference = (_it9.it_value.tv_sec);

        return time_difference;

    }
    catch(...) {}
}


//--------------------------------------------------------------------------
bool intervalTimer::vRefreshWatchDog(void)
{
    try
    {
//  usiWatchDogCount++;
//  printf("usiWatchDogCount:%d\n", usiWatchDogCount);
//  if(usiWatchDogCount >= 1) {
//    usiWatchDogCount = 0;
        digitalIO.vRefresfWatchDog0x444();
//  }

    }
    catch(...) {}
}

//OT970214NEWLCX405
//--------------------------------------------------------------------------
bool intervalTimer::vSendHeartBeatToLCX405(void)
{
    try
    {

        unsigned char ucHeartBeat[22];  //OTCombo0713 SayHelloToCard
        char msg[64];

//  ++usiLCX405WatchDogCount;

//  if(usiLCX405WatchDogCount > 1) {
//    usiLCX405WatchDogCount = 0;

        ucHeartBeat[0] = 0xAA;
        ucHeartBeat[1] = 0xBB;
        ucHeartBeat[2] = 0x13;
        ucHeartBeat[3] = 0x00;
        ucHeartBeat[4] = 0x00;
        ucHeartBeat[5] = 0x00;
        ucHeartBeat[6] = 0x00;
        ucHeartBeat[7] = 0x00;
        ucHeartBeat[8] = 0x00;
        ucHeartBeat[9] = 0x00;
        ucHeartBeat[10] = 0x00;
        ucHeartBeat[11] = 0x00;
        ucHeartBeat[12] = 0x00;
        ucHeartBeat[13] = 0x12;
        ucHeartBeat[14] = 0x00;
        ucHeartBeat[15] = 0x00;
        ucHeartBeat[16] = 0x00;
        ucHeartBeat[17] = 0x00;
        ucHeartBeat[18] = 0xAA;
        ucHeartBeat[19] = 0xCC;
        ucHeartBeat[20] = 0x00;
        for (int i=0; i<20; i++)
        {
            ucHeartBeat[20] ^= ucHeartBeat[i];
        }
        writeJob.WritePhysicalOut(ucHeartBeat, 21, DEVICETRAFFICLIGHT);  //OTCombo0713

//  sprintf(msg,"SendLightHeartBeat");
//  smem.vWriteMsgToDOM(msg);


//  }

        return 0;

    }
    catch(...) {}
}

//----------------------------------------------------------
void intervalTimer::vReportCurrentOperationMode_5F08_inCtimer(void)
{
    try
    {
        unsigned short int data_length = 3;
//mallocFuck  unsigned char *data = (unsigned char *)malloc( data_length*sizeof(unsigned char) );
        unsigned char data[4];
        data[0] = 0x5F;
        data[1] = 0x08;
        data[2] = 0x00;
        data[2] = smem.vGetTC5F08Status();

        printf("[MESSAGE] Ctimer Report 5F08 Current Operation Mode to Center: %d\n", data[2]);

        /*+++++++++++++++++*/

        MESSAGEOK _MsgOK;
        _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, data_length, true);
        writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICECENTER92);

    }
    catch (...) {}
}

//OT20140515
//---------------------------------------------------------------------------
bool intervalTimer::vReportDynSegStatus_5F0D_inCtimer(void)
{
    try
    {

        unsigned char ucAdjHour = smem.vGetUCData(TCDynSegAdjustHour);
        unsigned char ucAdjMin = smem.vGetUCData(TCDynSegAdjustMin);
        unsigned short int usiAdjKeepTime = smem.vGetUSIData(TCDynSegAdjustKeepTime);
        unsigned short int usiRemainSec = smem.vGetUSIData(TCDynSegRemainSec);
        bool bSW = smem.vGetBOOLData(TCDynSegSwitch);
        bool bStatus = smem.vGetBOOLData(TCDynSegStatus);
        unsigned short int usiPlanCycle = stc.vGetUSIData(CSTC_exec_plan_cycle);
        time_t now = smem.vGetSystemTime();

        unsigned char data[17];
        data[0] = 0x5F;
        data[1] = 0x0D;
        data[2] = ucAdjHour;
        data[3] = ucAdjMin;
        data[4] = usiAdjKeepTime / 256;
        data[5] = usiAdjKeepTime % 256;
        data[6] = bSW;
        data[7] = bStatus;
        data[8] = usiRemainSec / 256;
        data[9] = usiRemainSec % 256;
        data[10] = usiPlanCycle / 256;
        data[11] = usiPlanCycle % 256;
        data[12] = ( now >> 24) & 0xff;
        data[13] = ( now >> 16) & 0xff;
        data[14] = ( now >> 8)  & 0xff;
        data[15] = ( now ) & 0xff;

        MESSAGEOK _MsgOK;
        _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, 16, true);
        _MsgOK.InnerOrOutWard = cOutWard;
        //  writeJob.WriteWorkByMESSAGEOUT(_MsgOK);
        //20140519
        writeJob.WritePhysicalOutNoSetSeqNoResend(_MsgOK.packet, _MsgOK.packetLength, DEVICECENTER92);
        return true;

    }
    catch(...) {}
}



