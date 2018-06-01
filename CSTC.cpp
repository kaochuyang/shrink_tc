#include "CSTC.h"
#include "math.h"
#include "light.h"
#include "CTools.h"
/*+++++++++++++++++*/
//#include "ownwayRed.h"
//#include "center.h"
//#include "keypad.h"
#include "SMEM.h"
#include "WRITEJOB.h"
#include "CDataToMessageOK.h"
#include "var.h"
/*-----------------*/
#include <stdio.h>     //for printf()
#include <stdlib.h>    //for exit
#include <string.h>    //for strlen()
#include <unistd.h>    //for _POSIX_REALTIME_SIGNALS, getpid(), write(), read(), select()
#include <pthread.h>   //for thread functions
#include <signal.h>    //for signal functions
#include <time.h>      //for timer_create()
#include <mntent.h>
#include <sstream>
#include <iomanip>
#include "screenCurrentLightStatus.h"
#include "screenStrategy.h"
#include "screenPriority.h"     //jacky20140324
#include "screenPriorityExtendsec.h" //jacky20140521
#include "CTIMER.h"
#include "SCREENHWReset.h"
#include "screenRedCountHWCheckSel.h"
#include "CSTORINI.h"
//Down_crossing::Down_crossing(void){};
#include "CREATEXML.h"//arwen ++ read xml
#include <vector>
//jacky20141209
#include "PTRAFFIC92COMM.h"
#include<iostream>
using namespace std;
//----------------------------------------------------------
CSTC stc;
//----------------------------------------------------------
//**********************************************************
//****  static members declaration and assignment here
//**********************************************************
//----------------------------------------------------------
pthread_mutex_t CSTC::_stc_mutex=PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t CSTC::_record_traffic_mutex=PTHREAD_MUTEX_INITIALIZER;
// pthread_t CSTC::_stc_thread_detector_info;
pthread_t CSTC::_stc_thread_light_control;
timer_t CSTC::_timer_plan;
timer_t CSTC::_timer_redcount;
timer_t CSTC::_timer_panelcount;
timer_t CSTC::_timer_reportcount;
timer_t CSTC::_timer_record_traffic;
timer_t CSTC::_timer_reversetime;
timer_t CSTC::_timer_plan_WDT;   //in special case, _timer_plan will stop
struct itimerspec CSTC::_itimer_plan;
struct itimerspec CSTC::_itimer_redcount;
struct itimerspec CSTC::_itimer_panelcount;
struct itimerspec CSTC::_itimer_reportcount;
struct itimerspec CSTC::_itimer_record_traffic;
struct itimerspec CSTC::_itimer_reversetime;
struct itimerspec CSTC::_itimer_plan_WDT;
#define LCN_NON_INITIALIZED_VALUE        9999
#define PHASEORDER_NON_INITIALIZED_VALUE 9999
unsigned short int CSTC::LCN=LCN_NON_INITIALIZED_VALUE;
unsigned short int CSTC::_default_phaseorder=PHASEORDER_NON_INITIALIZED_VALUE;
CPhaseInfo   CSTC::phase[AMOUNT_PHASEORDER];
CPhaseInfo   CSTC::_exec_phase;
CPhaseInfo   CSTC::_panel_phase;
CPhaseInfo   CSTC::_for_center_phase;
CTrafficAnalyzer CSTC::traffic_analyzer;
ControlStrategy CSTC::_current_strategy=STRATEGY_ALLRED;
ControlStrategy CSTC::_old_strategy=STRATEGY_ALLRED;
CPlanInfo    CSTC::plan[AMOUNT_PLANID];
CPlanInfo    CSTC::BRTplan[AMOUNT_PLANID];  //jacky20140423
CPlanInfo    CSTC::_exec_plan;
CPlanInfo    CSTC::_exec_BRTPriority_plan;
CPlanInfo    CSTC::_panel_plan;
CPlanInfo    CSTC::_for_center_plan;
CPlanInfo    CSTC::_for_BRT_plan; //jacky20140418
CSegmentInfo CSTC::segment[AMOUNT_SEGMENT];
CSegmentInfo CSTC::BRTsegment[AMOUNT_SEGMENT]; //jacky20140423
CSegmentInfo CSTC::_exec_segment;
CSegmentInfo CSTC::_exec_BRTPriority_segment; //jacky20140423
CSegmentInfo CSTC::_panel_segment;
CSegmentInfo CSTC::_for_center_segment;
CSegmentInfo CSTC::_for_web_segment;//arwen ++
CSegmentInfo CSTC::_for_BRT_segment; //jacky20140418
/////////////////////////////////////////Arwen add 110210
CSegmentInfo CSTC::new_data_segment[AMOUNT_SEGMENT];
CWeekDaySegType CSTC::weekdayseg[AMOUNT_WEEKDAY_SEG];
CWeekDaySegType CSTC::_panel_weekdayseg[AMOUNT_WEEKDAY_SEG];
CWeekDaySegType CSTC::_for_center_weekdayseg[AMOUNT_WEEKDAY_SEG];
CWeekDaySegType CSTC::_for_web_weekdayseg[AMOUNT_WEEKDAY_SEG];
CHoliDaySegType CSTC::holidayseg[AMOUNT_HOLIDAY_SEG];
CHoliDaySegType CSTC::_panel_holidayseg;
CHoliDaySegType CSTC::_for_center_holidayseg;
CReverseTimeInfo CSTC::reversetime[AMOUNT_REVERSETIME];
CReverseTimeInfo CSTC::_exec_rev;
CReverseTimeInfo CSTC::_panel_reversetime;
CReverseTimeInfo CSTC::_for_center_reversetime;
CWeekDayRevType  CSTC::weekdayrev[AMOUNT_WEEKDAY_REV];
CWeekDayRevType  CSTC::_panel_weekdayrev[AMOUNT_WEEKDAY_REV];
CWeekDayRevType  CSTC::_for_center_weekdayrev[AMOUNT_WEEKDAY_REV];
CHoliDayRevType  CSTC::holidayrev[AMOUNT_HOLIDAY_REV];
CHoliDayRevType  CSTC::_panel_holidayrev;
CHoliDayRevType  CSTC::_for_center_holidayrev;
unsigned short int CSTC::_exec_phase_current_subphase=0;  //FIRST_TIME setting running subphase value
unsigned short int CSTC::_exec_phase_current_subphase_step=0;  //FIRST_TIME setting running step value
unsigned short int CSTC::_exec_segment_current_seg_no=0;  //FIRST_TIME setting running seg_no value
unsigned short int CSTC::_exec_reversetime_current_rev_no = 0;
unsigned short int CSTC::_exec_reversetime_current_rev_step = 0;
timespec CSTC::strategy_start_time= {0,0};
// CIOCom CSTC::_detector_io;
SBuffer CSTC::_buffer;    // store the read buffer
CPacketCluster CSTC::_packet_c;  // store the read packets
// CRTMSDecoder CSTC::_detector_decoder;
// CRTMSInformation CSTC::_rtms_info;
Down_crossing CSTC::down_crossing_from_DC;
Down_crossing CSTC::down_crossing_STC;
bool CSTC::recording_traffic=false;
ofstream CSTC::rawfile;
ofstream CSTC::refinedfile;
ofstream CSTC::trafficfile;
ofstream CSTC::targetfile;
unsigned long int CSTC::inrecordno=0;
unsigned long int CSTC::inperiodno=0;
//OTADD 941215
//OTBUG +1
bool CSTC::PlanUpdate = false;
bool CSTC::SegmentTypeUpdate = false;
bool CSTC::ReverseTimeDataUpdate = false;
unsigned short int CSTC::OLD_TOD_PLAN_ID = 0;
unsigned short int CSTC::NEW_TOD_PLAN_ID = 0;
/*otaru0514--*/
bool CSTC::bRefreshLight = true;
int offset_not_been_adjusted_by_CADC=0;
bool CSTC::bCSTCInitialOK = false;
bool CSTC::bJumpSubEnable = false;
unsigned char CSTC::ucJumpSubPhase = 0;
//OT980303
unsigned short int CSTC::usiTrainComingSec = 0;
unsigned short int CSTC::usiLockPhaseSec = 0;
//arwen ++ 1001007 lastactMode
unsigned short int CSTC::m_segment_lastActMode = 255;
bool CSTC::Fixgreen = false;
//OT20140415
int CSTC::iDynSeg_SegType;
int CSTC::iDynSeg_SegCount;
int CSTC::iDynSeg_PlanID;
//jacky20141209
PTRAFFIC92COMM CSTC::pTraffic92Comm;
//----------------------------------------------聯嘉倒數燈專區------------------------------------------------------
unsigned short int RedData[8] = {0, 0, 0, 0, 0, 0, 0, 0};        //八組輸出 一般只用四組 2013/06/03 chou tip
unsigned short int GreenData[8] = {0, 0, 0, 0, 0, 0, 0, 0};        //八組輸出 一般只用四組 2013/06/03 chou tip
unsigned short int m_greenman[8] = {0,0,0,0,0,0,0,0};               //紀錄綠車倒數時間
unsigned short int m_redman[8] = {0,0,0,0,0,0,0,0};                 //紀錄紅車倒數時間
unsigned short int m_redremainder[8] = {0,0,0,0,0,0,0,0};         //紀錄倒數剩餘秒數
bool Carsign[8]  = {0,0,0,0,0,0,0,0};                               //紀錄現在該進行綠車倒數還是紅車倒數
unsigned short int Cargreencontrol = 0x00;                             //紀錄綠車倒數執行分相
unsigned short int Carredcontrol = 0x00;                               //紀錄紅車倒數執行分相
unsigned short int pedRedData[8] = {0, 0, 0, 0, 0, 0, 0, 0};        //八組輸出 一般只用四組 2013/05/17 chou tip
unsigned short int pedGreenData[8] = {0, 0, 0, 0, 0, 0, 0, 0};        //八組輸出 一般只用四組 2013/05/17 chou tip
unsigned short int m_pedgreenman[8] = {0,0,0,0,0,0,0,0};               //紀錄綠人倒數時間
unsigned short int m_pedredman[8] = {0,0,0,0,0,0,0,0};                 //紀錄紅人倒數時間
unsigned short int m_greenremainder[8] = {0,0,0,0,0,0,0,0};         //紀錄行人倒數剩餘秒數
bool Pedsign[8]  = {0,0,0,0,0,0,0,0};                               //紀錄現在該進行行人倒數還是紅燈倒數
unsigned short int m_greencount[8] = {0,0,0,0,0,0,0,0};
unsigned short int m_redcount[8] = {0,0,0,0,0,0,0,0};
bool redcountsign[8] = {0,0,0,0,0,0,0,0};
unsigned short int usigreenPhaseTmp[8][8];                          //紀錄綠燈倒數燈頭狀態
unsigned short int usiredPhaseTmp[8][8];                            //紀錄紅燈倒數燈頭狀態
unsigned short int usipedgreenPhaseTmp[8][8];                       //紀錄行人綠燈頭狀態   2013/06/06 chou ++
unsigned short int usipedredPhaseTmp[8][8];                         //紀錄行人紅燈燈頭狀態 2013/06/06
int pedcyclecount = 0; //由於剛開機倒數秒數很不準,等3個cycle在開始顯示 2013/06/06 chou++
//----------------------------------------------聯嘉倒數燈專區-----------------------------------------------------
unsigned short int CycleRunCount = 0;
unsigned short int CountBF08 = 0;
unsigned short int TenCycle = 0;
unsigned short int LearningPGPRCycle = 0;
unsigned char vTempStep = 1;
//ccc
//tt static time_t now;
//tt static struct tm* currenttime;
//----------------------------------------------------------
//**********************************************************
//****                   Constructor
//**********************************************************
//----------------------------------------------------------
CSTC::CSTC()
{
    try
    {
        bCSTCInitialOK = false;
        sigset_t main_mask;
        sigfillset( & main_mask ); //block all signal, Ctrl+C not workable
        sigprocmask( SIG_SETMASK, & main_mask, NULL );
        printf("CSTC::CSTC(): blocking all signals in main!!\n");
        for(unsigned short int i=0; i<AMOUNT_PHASEORDER; i++)
            phase[i]._phase_order=i;
        for(unsigned short int i=0; i<AMOUNT_PLANID; i++)
            plan[i]._planid=i;
        for(unsigned short int i=0; i<AMOUNT_SEGMENT; i++)
            segment[i]._segment_type=i;
        for(unsigned short int i=0; i<AMOUNT_WEEKDAY_SEG; i++)
        {
            weekdayseg[i]._segment_type=0;
            weekdayseg[i]._weekday=(i<7)?(i+1):(i+4); //{0-6,7-13} according to {1-7,11-17}
        }
        for(unsigned short int i=0; i<AMOUNT_HOLIDAY_SEG; i++)
            holidayseg[i]._segment_type=0;
//  down_crossing_from_DC.current_step = 254;
//  down_crossing_STC.current_step = 254;
    }
    catch (...) {}
}
//----------------------------------------------------------
//**********************************************************
//****            Data Reading and Initializing
//**********************************************************
//----------------------------------------------------------
void CSTC::ReadAllData( void )
{
    try
    {
        //jacky20140317
//  jsonequipment();  //creat equipment table
        ReadPhaseData();
        //OT20110629
        vCheckPhase80beFlash();
        ReadPrioritySegment(); //jacky20140418
        ReadPlanData();
        ReadSegmentData();
        ReadReverseTimeTypeData();
        ReadActCtrlSegment();//arwen ++
        ReadPriorityParameter(); //jacky20140421
        SetFlashAllRedPhaseInfo();
        SetFlashAllRedPlanInfo();
        if(!ReadDefaultLCNPhaseOrder())  perror("ERROR: Missing Default LCN or PhaseInfo Files!!\n");
//  for(int i=0; i<3; i++) pTraffic92Comm.vReportVersion_0FC3(); //jacky20141209 開機送三次韌體版本
    }
    catch (...) {}
}
//----------------------------------------------------------
void CSTC::ReadPhaseData( void )
{
    try
    {
        FILE * phase_rfile = NULL;
        char filename[22];
        for( int i=0; i<AMOUNT_PHASEORDER; i++)
        {
            sprintf( filename, "/Data/PhaseInfo%X.bin\0", i);
            phase_rfile = fopen(filename, "r"); //fopen return NULL if file not exist
            if(phase_rfile)
            {
#ifdef ShowInfoToSTD
                printf("Reading File PhaseOrder: %3d (%#X)\n",i,i);
#endif
                //OTADD
                smem.vSetTCPhasePlanSegTypeData(TC_Phase, i, true);
                fread( &phase[i]._phase_order,      sizeof( unsigned short int ), 1, phase_rfile );
                fread( &phase[i]._signal_map,       sizeof( unsigned short int ), 1, phase_rfile );
                fread( &phase[i]._subphase_count,   sizeof( unsigned short int ), 1, phase_rfile );
                fread( &phase[i]._signal_count,     sizeof( unsigned short int ), 1, phase_rfile );
                fread( &phase[i]._total_step_count, sizeof( unsigned short int ), 1, phase_rfile );
                //jacky20140314
//      jsonPhase(i,phase[i]._subphase_count);
                if(phase[i]._phase_order!=i) perror("ERROR: Reading PhaseOrder != FileName\n");
#ifdef ShowInfoToSTD
                printf("  i:%d _phase_order:%#X _signal_map:%d\n  _subphase_count:%d _signal_count:%d _total_step_count:%d\n"
                       ,i , phase[i]._phase_order, phase[i]._signal_map, phase[i]._subphase_count
                       , phase[i]._signal_count, phase[i]._total_step_count);
#endif
//mallocFuck      phase[i]._ptr_subphase_step_count    = (unsigned short int *)  malloc(sizeof(unsigned short int)   *phase[i]._subphase_count);
//mallocFuck      phase[i]._ptr_subphase_step_signal_status = (unsigned short int ***)malloc(sizeof(unsigned short int **)*phase[i]._subphase_count);
                for( int j=0; j<phase[i]._subphase_count; j++ )
                {
                    fread( &phase[i]._ptr_subphase_step_count[j], sizeof(unsigned short int), 1, phase_rfile);
#ifdef ShowInfoToSTD
                    printf("  _step_count[%d]:%d\n",j,phase[i]._ptr_subphase_step_count[j]);
#endif
//mallocFuck        phase[i]._ptr_subphase_step_signal_status[j] = (unsigned short int **)malloc(sizeof(unsigned short int *)*phase[i]._ptr_subphase_step_count[j]);
                    for( int k=0; k<phase[i]._ptr_subphase_step_count[j]; k++)
                    {
//mallocFuck          phase[i]._ptr_subphase_step_signal_status[j][k] = (unsigned short int *)malloc(sizeof(unsigned short int)*phase[i]._signal_count);
                        for( int l=0; l<phase[i]._signal_count; l++)
                        {
                            fread( & phase[i]._ptr_subphase_step_signal_status[j][k][l], sizeof(unsigned short int), 1, phase_rfile );
#ifdef ShowInfoToSTD
                            printf("    phase[%#X]._ptr_subphase_step_signal_status[%d][%d][%d]:%#X\n",i,j,k,l,phase[i]._ptr_subphase_step_signal_status[j][k][l]);
#endif
                        }
                    }
                }
                if(phase[i]._total_step_count!=phase[i].calculated_total_step_count()) perror("ERROR: PhaseOrder File _total_step_count != calculated_total_step_count()\n");
                phase[i]._total_step_count=phase[i].calculated_total_step_count();

                fclose( phase_rfile );
            } //end if(fopen() succeed)
        }
    }
    catch (...) {}
}
//20110629
//----------------------------------------------------------
void CSTC::vCheckPhase80beFlash( void )
{
    try
    {
        printf("phase[%d]._phase_order:%d\n", 0x80, phase[0x80]._phase_order);
        printf("phase[%d]._signal_map:%d\n", 0x80, phase[0x80]._signal_map);
        printf("phase[%d]._subphase_count:%d\n", 0x80, phase[0x80]._subphase_count);
        printf("phase[%d]._signal_count:%d\n", 0x80, phase[0x80]._signal_count);
        printf("phase[%d]._total_step_count:%d\n", 0x80, phase[0x80]._total_step_count);
        for( int j = 0; j < phase[0x80]._subphase_count; j++ )
        {
            printf("phase[%d]._ptr_subphase_step_count[%d]:%d\n", 0x80, j, phase[0x80]._ptr_subphase_step_count[j]);
            for( int k=0; k<phase[0x80]._ptr_subphase_step_count[j]; k++)
            {
                for( int l=0; l<phase[0x80]._signal_count; l++)
                {
                    printf("    phase[%d]._ptr_subphase_step_signal_status[%d][%d][%d]:%#X\n",0x80,j,k,l,phase[0x80]._ptr_subphase_step_signal_status[j][k][l]);
                }
            }
        }
//  if(phase[0x80]._total_step_count == 10 &&  //fucking phase, not flash
//     phase[0x80]._ptr_subphase_step_signal_status[0][0][0] == 0xCC30) {
        if(phase[0x80]._total_step_count == 10 &&  //fucking phase, not flash
                phase[0x80]._ptr_subphase_step_signal_status[0][0][0] == 0xCC30)
        {
            _for_center_phase = phase[0xB0];  //copy flash 0xB0 to 0x80
            _for_center_phase._phase_order = 0x80;
            Lock_to_Save_Phase_from_Center();
        }
        else if (phase[0x80]._total_step_count == 5 &&    //fucking phase, not flash
                 phase[0x80]._ptr_subphase_step_signal_status[0][0][0] == 0x2008)
        {
            _for_center_phase = phase[0xB0];  //copy flash 0xB0 to 0x80
            _for_center_phase._phase_order = 0x80;
            Lock_to_Save_Phase_from_Center();
        }
    }
    catch (...) {}
}
//----------------------------------------------------------
void CSTC::ReadPlanData( void )
{
    try
    {
        FILE * plan_rfile = NULL;
        char filename[22];
        for( int i=0; i<AMOUNT_PLANID; i++)
        {
            sprintf( filename, "/Data/PlanInfo%d.bin\0", i);

            plan_rfile = fopen(filename, "r"); //fopen return NULL if file not exist
            if(plan_rfile)
            {
#ifdef ShowInfoToSTD
                printf("Reading File PlanID: %2d\n",i);
#endif
                //OTADD
                smem.vSetTCPhasePlanSegTypeData(TC_Plan, i, true);
                fread( &plan[i]._planid,         sizeof( unsigned short int ), 1, plan_rfile );
                fread( &plan[i]._dir,            sizeof( unsigned short int ), 1, plan_rfile );
                fread( &plan[i]._phase_order,    sizeof( unsigned short int ), 1, plan_rfile );
                fread( &plan[i]._subphase_count, sizeof( unsigned short int ), 1, plan_rfile );
                fread( &plan[i]._cycle_time,     sizeof( unsigned short int ), 1, plan_rfile );
                fread( &plan[i]._offset,         sizeof( unsigned short int ), 1, plan_rfile );
                //jacky 20140314
//      if(i>0 && i<41)  jsonstd_plan_ID(plan[i]._planid, plan[i]._phase_order, plan[i]._subphase_count, plan[i]._cycle_time, plan[i]._offset);
                if(plan[i]._planid!=i) perror("ERROR: Reading PlanID != FileName\n");
#ifdef ShowInfoToSTD
                printf("  i:%d _planid:%d _dir:%d _phase_order:%#X\n  _subphase:%d _cycle_time:%d _offset:%d\n"
                       , i, plan[i]._planid, plan[i]._dir, plan[i]._phase_order
                       , plan[i]._subphase_count, plan[i]._cycle_time, plan[i]._offset);
#endif
//mallocFuck      plan[i]._ptr_subplaninfo = (SSubPlanInfo *)malloc(sizeof(SSubPlanInfo)*plan[i]._subphase_count);
                for(int j=0; j<plan[i]._subphase_count; j++)
                {
                    fread( &(plan[i]._ptr_subplaninfo[j]._green),          sizeof( unsigned short int ), 1, plan_rfile );
                    fread( &(plan[i]._ptr_subplaninfo[j]._min_green),      sizeof( unsigned short int ), 1, plan_rfile );
                    fread( &(plan[i]._ptr_subplaninfo[j]._max_green),      sizeof( unsigned short int ), 1, plan_rfile );
                    fread( &(plan[i]._ptr_subplaninfo[j]._yellow),         sizeof( unsigned short int ), 1, plan_rfile );
                    fread( &(plan[i]._ptr_subplaninfo[j]._allred),         sizeof( unsigned short int ), 1, plan_rfile );
                    fread( &(plan[i]._ptr_subplaninfo[j]._pedgreen_flash), sizeof( unsigned short int ), 1, plan_rfile );
                    fread( &(plan[i]._ptr_subplaninfo[j]._pedred),         sizeof( unsigned short int ), 1, plan_rfile );
                    //jacky 20140314
//        if(i>0 && i<41){
//            jsonstd_plan_light(plan[i]._ptr_subplaninfo[j]._allred, plan[i]._ptr_subplaninfo[j]._yellow, plan[i]._ptr_subplaninfo[j]._green,
//                      plan[i]._ptr_subplaninfo[j]._pedgreen_flash, plan[i]._ptr_subplaninfo[j]._pedred, plan[i]._ptr_subplaninfo[j]._min_green,
//                      plan[i]._ptr_subplaninfo[j]._max_green, j+1);
//        }
#ifdef ShowInfoToSTD
                    printf("    plan[%d]._ptr_subplaninfo[%d]._green:%d\n",i,j,plan[i]._ptr_subplaninfo[j]._green);
                    printf("    plan[%d]._ptr_subplaninfo[%d]._min_green:%d\n",i,j,plan[i]._ptr_subplaninfo[j]._min_green);
                    printf("    plan[%d]._ptr_subplaninfo[%d]._max_green:%d\n",i,j,plan[i]._ptr_subplaninfo[j]._max_green);
                    printf("    plan[%d]._ptr_subplaninfo[%d]._yellow:%d\n",i,j,plan[i]._ptr_subplaninfo[j]._yellow);
                    printf("    plan[%d]._ptr_subplaninfo[%d]._allred:%d\n",i,j,plan[i]._ptr_subplaninfo[j]._allred);
                    printf("    plan[%d]._ptr_subplaninfo[%d]._pedgreen_flash:%d\n",i,j,plan[i]._ptr_subplaninfo[j]._pedgreen_flash);
                    printf("    plan[%d]._ptr_subplaninfo[%d]._pedred:%d\n",i,j,plan[i]._ptr_subplaninfo[j]._pedred);
#endif
                    plan[i]._ptr_subplaninfo[j]._green_compensation           = 0;
                    plan[i]._ptr_subplaninfo[j]._yellow_compensation          = 0;
                    plan[i]._ptr_subplaninfo[j]._allred_compensation          = 0;
                    plan[i]._ptr_subplaninfo[j]._pedgreen_flash_compensation  = 0;
                    plan[i]._ptr_subplaninfo[j]._pedred_compensation          = 0;
                }
                //jacky 20140314
//      if(i>0 && i<41){
//        for(int j=plan[i]._subphase_count ; j<8; j++)
//            jsonstd_plan_light(0, 0, 0, 0, 0, 0, 0, j+1);
//      }
                plan[i]._shorten_cycle = false;
                if(plan[i]._cycle_time != plan[i].calculated_cycle_time())
                {
                    perror("ERROR: cycle_time error!!");
                    printf("ERROR: cycle_time error!! plan[%d]._cycle_time:%d plan[%d].calculated_cycle_time():%d\n", i, plan[i]._cycle_time, i, plan[i].calculated_cycle_time());
                    plan[i]._cycle_time = plan[i].calculated_cycle_time();
                }
                fclose( plan_rfile );
            } //end if(fopen() succeed)
        }
    }
    catch (...) {}
}
//----------------------------------------------------------
void CSTC::ReadSegmentData( void )
{
    try
    {
        FILE * segment_rfile = NULL;
        char filename[25];
        for( int i=0; i<AMOUNT_SEGMENT; i++)
        {
            sprintf( filename, "/Data/SegmentInfo%d.bin\0", i);

            segment_rfile = fopen(filename, "r"); //fopen return NULL if file not exist
            if(segment_rfile)
            {
#ifdef ShowInfoToSTD
                printf("Reading File SegmentType: %2d\n",i);
#endif

                //OTADD
                smem.vSetTCPhasePlanSegTypeData(TC_SegType, i, true);

                fread( &segment[i]._segment_type,  sizeof( unsigned short int ), 1, segment_rfile );
                fread( &segment[i]._segment_count, sizeof( unsigned short int ), 1, segment_rfile );
                if(segment[i]._segment_type!=i) perror("ERROR: Reading SegmentType != FileName\n");
//==        printf("  i:%2d _segment_type:%2d _segment_count:%2d\n",i,segment[i]._segment_type,segment[i]._segment_count);

//mallocFuck      segment[i]._ptr_seg_exec_time = (SSegExecTime *)malloc(sizeof(SSegExecTime)*segment[i]._segment_count);
                for(int j=0; j<segment[i]._segment_count; j++)
                {
                    fread( &(segment[i]._ptr_seg_exec_time[j]._hour),     sizeof( unsigned short int ), 1, segment_rfile );
                    fread( &(segment[i]._ptr_seg_exec_time[j]._minute),   sizeof( unsigned short int ), 1, segment_rfile );
                    fread( &(segment[i]._ptr_seg_exec_time[j]._planid),   sizeof( unsigned short int ), 1, segment_rfile );
                    fread( &(segment[i]._ptr_seg_exec_time[j]._cadc_seg), sizeof( unsigned short int ), 1, segment_rfile );
//==          printf("segment[%2d]._ptr_seg_exec_time[%2d]._hour:    %2d\n",i,j,segment[i]._ptr_seg_exec_time[j]._hour);
//==          printf("segment[%2d]._ptr_seg_exec_time[%2d]._minute:  %2d\n",i,j,segment[i]._ptr_seg_exec_time[j]._minute);
//==          printf("segment[%2d]._ptr_seg_exec_time[%2d]._planid:  %2d\n",i,j,segment[i]._ptr_seg_exec_time[j]._planid);
//==          printf("segment[%2d]._ptr_seg_exec_time[%2d]._cadc_seg:%2d\n",i,j,segment[i]._ptr_seg_exec_time[j]._cadc_seg);
                    //jacky20140314
//        if(i > 0){
//            jsontod_plan(segment[i]._segment_type,segment[i]._ptr_seg_exec_time[j]._hour,segment[i]._ptr_seg_exec_time[j]._minute,
//                    segment[i]._ptr_seg_exec_time[j]._planid,BRTsegment[i]._ptr_seg_exec_time[j]._onoff);
//        }
                }
                fclose( segment_rfile );
            } //end if(fopen() succeed)
        }

        sprintf( filename,"/Data/WeekDaySegType.bin\0" );
        segment_rfile = fopen(filename, "r"); //fopen return NULL if file not exist
        if(segment_rfile)
        {
#ifdef ShowInfoToSTD
            printf("Reading File WeekDaySegType\n");
#endif
            for(int i=0; i<AMOUNT_WEEKDAY_SEG; i++)
            {
                fread( &weekdayseg[i]._segment_type, sizeof( unsigned short int ), 1, segment_rfile );
                fread( &weekdayseg[i]._weekday,      sizeof( unsigned short int ), 1, segment_rfile );
                //jacky20140318
//      jsonday_segtype_weekday(weekdayseg[i]._weekday,weekdayseg[i]._segment_type);

                if(weekdayseg[i]._segment_type>7)
                    perror("ERROR: WeekDaySegment not in {0-7}\n");
                if(    (i< 7 && (i!=weekdayseg[i]._weekday-1))
                        || (i>=7 && (i!=weekdayseg[i]._weekday-4)) )
                    perror("ERROR: WeekDayFile WeekDay != weekdayseg[i]._weekday\n");
//==          printf("  i:%2d _segment_type:%2d _segment_weekday:%2d\n",i,weekdayseg[i]._segment_type,weekdayseg[i]._weekday);
                if     (i< 7) weekdayseg[i]._weekday=(i+1);
                else if(i>=7) weekdayseg[i]._weekday=(i+4);
            }
            fclose( segment_rfile );
        }
        else perror("ERROR: Missing File WeekDaySegType");

        sprintf( filename,"/Data/HoliDaySegType.bin\0" );
        segment_rfile = fopen(filename, "r"); //fopen return NULL if file not exist
        if(segment_rfile)
        {
#ifdef ShowInfoToSTD
            printf("Reading File HoliDaySegType\n");
#endif
            for(int i=0; i<AMOUNT_HOLIDAY_SEG; i++)
            {
                fread( &holidayseg[i]._segment_type, sizeof( unsigned short int ), 1, segment_rfile );
                fread( &holidayseg[i]._start_year,   sizeof( unsigned short int ), 1, segment_rfile );
                fread( &holidayseg[i]._start_month,  sizeof( unsigned short int ), 1, segment_rfile );
                fread( &holidayseg[i]._start_day,    sizeof( unsigned short int ), 1, segment_rfile );
                fread( &holidayseg[i]._end_year,     sizeof( unsigned short int ), 1, segment_rfile );
                fread( &holidayseg[i]._end_month,    sizeof( unsigned short int ), 1, segment_rfile );
                fread( &holidayseg[i]._end_day,      sizeof( unsigned short int ), 1, segment_rfile );
                //jacky20140318
//      jsonday_segtype_holiday(holidayseg[i]._segment_type,holidayseg[i]._start_year,holidayseg[i]._start_month,holidayseg[i]._start_day,
//                              holidayseg[i]._end_year,holidayseg[i]._end_month,holidayseg[i]._end_day);

                if(holidayseg[i]._segment_type<8||holidayseg[i]._segment_type>20)
                    perror("ERROR: HoliDaySegment not in {8-20}\n");
#ifdef ShowInfoToSTD
                printf("\nA\nA\nA\nA\nA\nA\nA\nA\nA\nA\nA\nA\nA\nA\nA\nA\nA\nA\nA\nA\nA\nA\nA\nA");
                printf("  holidayseg[%2d]._segment_type: %4d\n",i,holidayseg[i]._segment_type);
                printf("    holidayseg[%2d]._start_year: %4d\n",i,holidayseg[i]._start_year);
                printf("    holidayseg[%2d]._start_month:%4d\n",i,holidayseg[i]._start_month);
                printf("    holidayseg[%2d]._start_day:  %4d\n",i,holidayseg[i]._start_day);
                printf("    holidayseg[%2d]._end_year:   %4d\n",i,holidayseg[i]._end_year);
                printf("    holidayseg[%2d]._end_month:  %4d\n",i,holidayseg[i]._end_month);
                printf("    holidayseg[%2d]._end_day:    %4d\n",i,holidayseg[i]._end_day);
#endif
            }
            fclose( segment_rfile );
        }
        else perror("ERROR: Missinging File HoliDaySchedule");
    }
    catch (...) {}
}

//----------------------------------------------------------
void CSTC::ReadReverseTimeTypeData( void )
{
    try
    {
        FILE *reversetime_rfile = NULL;
        char filename[36];
        for( int i=0; i<AMOUNT_REVERSETIME; i++)
        {
            sprintf( filename, "/Data/ReverseTimeType%d.bin\0", i);

            reversetime_rfile = fopen(filename, "r"); //fopen return NULL if file not exist
            if(reversetime_rfile)
            {
#ifdef ShowInfoToSTD
                printf("Reading File ReverseTimeType: %2d\n",i);
#endif

                //OTADD
//      smem.vSetTCPhasePlanSegTypeData(TC_SegType, i, true);

                fread( &reversetime[i]._reverse_time_type,  sizeof( unsigned short int ), 1, reversetime_rfile );
                if(reversetime[i]._reverse_time_type != i) perror("ERROR: Reading ReverseTimeType != FileName\n");

                fread( &(reversetime[i].usiDirectIn),     sizeof( unsigned short int ), 1, reversetime_rfile );
                fread( &(reversetime[i].usiHourStartIn),  sizeof( unsigned short int ), 1, reversetime_rfile );
                fread( &(reversetime[i].usiMinStartIn),   sizeof( unsigned short int ), 1, reversetime_rfile );
                fread( &(reversetime[i].usiHourEndIn),    sizeof( unsigned short int ), 1, reversetime_rfile );
                fread( &(reversetime[i].usiMinEndIn),     sizeof( unsigned short int ), 1, reversetime_rfile );

                fread( &(reversetime[i].usiDirectOut),     sizeof( unsigned short int ), 1, reversetime_rfile );
                fread( &(reversetime[i].usiHourStartOut),  sizeof( unsigned short int ), 1, reversetime_rfile );
                fread( &(reversetime[i].usiMinStartOut),   sizeof( unsigned short int ), 1, reversetime_rfile );
                fread( &(reversetime[i].usiHourEndOut),    sizeof( unsigned short int ), 1, reversetime_rfile );
                fread( &(reversetime[i].usiMinEndOut),     sizeof( unsigned short int ), 1, reversetime_rfile );

                fread( &(reversetime[i].usiClearTime),     sizeof( unsigned short int ), 1, reversetime_rfile );
                fread( &(reversetime[i].usiFlashGreen),    sizeof( unsigned short int ), 1, reversetime_rfile );
                fread( &(reversetime[i].usiGreenTime),     sizeof( unsigned short int ), 1, reversetime_rfile );

                fread( &(reversetime[i].ucNonRevLight),    sizeof( unsigned char ), 1, reversetime_rfile );

                fclose( reversetime_rfile );
#ifdef ShowInfoToSTD
                printf("OT load reverse time id: %d\n", reversetime[i]._reverse_time_type);
                printf("OT load reverse: %d\n", reversetime[i].usiDirectIn);
                printf("OT load reverse: %d\n", reversetime[i].usiHourStartIn);
                printf("OT load reverse: %d\n", reversetime[i].usiMinStartIn);
                printf("OT load reverse: %d\n", reversetime[i].usiHourEndIn);
                printf("OT load reverse: %d\n", reversetime[i].usiMinEndIn);
                printf("OT load reverse: %d\n", reversetime[i].usiDirectOut);
                printf("OT load reverse: %d\n", reversetime[i].usiHourStartOut);
                printf("OT load reverse: %d\n", reversetime[i].usiMinStartOut);
                printf("OT load reverse: %d\n", reversetime[i].usiHourEndOut);
                printf("OT load reverse: %d\n", reversetime[i].usiMinEndOut);
                printf("OT load reverse: %d\n", reversetime[i].usiClearTime);
                printf("OT load reverse: %d\n", reversetime[i].usiFlashGreen);
                printf("OT load reverse: %d\n", reversetime[i].usiGreenTime);
                printf("OT load reverse: %d\n", reversetime[i].ucNonRevLight);
#endif

            } //end if(fopen() succeed)
        }

        sprintf( filename,"/Data/WeekDayRevType.bin\0" );
        reversetime_rfile = fopen(filename, "r"); //fopen return NULL if file not exist
        if(reversetime_rfile)
        {
#ifdef ShowInfoToSTD
            printf("Reading File WeekDayRevType\n");
#endif
            for(int i=0; i<AMOUNT_WEEKDAY_REV; i++)
            {
                fread( &weekdayrev[i]._reverse_time_type, sizeof( unsigned short int ), 1, reversetime_rfile );
                fread( &weekdayrev[i]._weekday,           sizeof( unsigned short int ), 1, reversetime_rfile );

                if(weekdayrev[i]._reverse_time_type>7)
                    perror("ERROR: WeekDayReverseTimeData not in {0-7}\n");
                if(    (i< 7 && (i!=weekdayrev[i]._weekday-1))
                        || (i>=7 && (i!=weekdayrev[i]._weekday-4)) )
                    perror("ERROR: WeekDayFile WeekDay != weekdayrev[i]._weekday\n");
//==          printf("  i:%2d _segment_type:%2d _segment_weekday:%2d\n",i,weekdayseg[i]._segment_type,weekdayseg[i]._weekday);
                if     (i< 7) weekdayrev[i]._weekday=(i+1);
                else if(i>=7) weekdayrev[i]._weekday=(i+4);
            }
            fclose( reversetime_rfile );
        }
        else perror("ERROR: Missing File WeekDayRevType");

        sprintf( filename,"/Data/HoliDayRevType.bin\0" );
        reversetime_rfile = fopen(filename, "r"); //fopen return NULL if file not exist
        if(reversetime_rfile)
        {
#ifdef ShowInfoToSTD
            printf("Reading File HoliDayRevType\n");
#endif
            for(int i=0; i<AMOUNT_HOLIDAY_REV; i++)
            {
                fread( &holidayrev[i]._reverse_time_type, sizeof( unsigned short int ), 1, reversetime_rfile );
                fread( &holidayrev[i]._start_year,        sizeof( unsigned short int ), 1, reversetime_rfile );
                fread( &holidayrev[i]._start_month,       sizeof( unsigned short int ), 1, reversetime_rfile );
                fread( &holidayrev[i]._start_day,         sizeof( unsigned short int ), 1, reversetime_rfile );
                fread( &holidayrev[i]._end_year,          sizeof( unsigned short int ), 1, reversetime_rfile );
                fread( &holidayrev[i]._end_month,         sizeof( unsigned short int ), 1, reversetime_rfile );
                fread( &holidayrev[i]._end_day,           sizeof( unsigned short int ), 1, reversetime_rfile );

                /*
                printf("holidayrev[%d]._start_year: %d\n", i, holidayrev[i]._start_year);
                printf("holidayrev[%d]._start_month: %d\n", i, holidayrev[i]._start_month);
                printf("holidayrev[%d]._start_day: %d\n", i, holidayrev[i]._start_day);
                printf("holidayrev[%d]._end_year: %d\n", i, holidayrev[i]._end_year);
                printf("holidayrev[%d]._end_month: %d\n", i, holidayrev[i]._end_month);
                printf("holidayrev[%d]._end_day: %d\n", i, holidayrev[i]._end_day);
                */

                if(holidayrev[i]._reverse_time_type<4||holidayrev[i]._reverse_time_type>16)
                    perror("ERROR: HoliDayReverseTime not in {4-6}\n");
//==        printf("  holidayseg[%2d]._segment_type: %4d\n",i,holidayseg[i]._segment_type);
//==        printf("    holidayseg[%2d]._start_year: %4d\n",i,holidayseg[i]._start_year);
//==        printf("    holidayseg[%2d]._start_month:%4d\n",i,holidayseg[i]._start_month);
//==        printf("    holidayseg[%2d]._start_day:  %4d\n",i,holidayseg[i]._start_day);
//==        printf("    holidayseg[%2d]._end_year:   %4d\n",i,holidayseg[i]._end_year);
//==        printf("    holidayseg[%2d]._end_month:  %4d\n",i,holidayseg[i]._end_month);
//==        printf("    holidayseg[%2d]._end_day:    %4d\n",i,holidayseg[i]._end_day);
            }
            fclose( reversetime_rfile );
        }
        else perror("ERROR: Missinging File HoliDaySchedule");
    }
    catch (...) {}
}

//----------------------------------------------------------
bool CSTC::ReadDefaultLCNPhaseOrder(void)
{
    try
    {

        /* OTDebug 951121 */
        /*
          FILE * lcn_rfile = fopen("/Data/DefaultLCN.bin", "r");
          if(lcn_rfile){
            fread( &LCN, sizeof(unsigned short int), 1, lcn_rfile);
            printf("Reading File DefaultLCN: %d\n",LCN);
          }
          else return false;
          fclose(lcn_rfile);
        */

        FILE * phaseorder_rfile = fopen("/Data/DefaultPhaseOrder.bin", "r");
        if(phaseorder_rfile)
        {
            fread( &_default_phaseorder, sizeof(unsigned short int), 1, phaseorder_rfile );
            printf("Reading File DefaultPhaseOrder: %d\n", _default_phaseorder);
        }
        else return false;
        fclose(phaseorder_rfile);

        return true;
    }
    catch(...) {}
}
//----------------------------------------------------------
//----------------------------------------------------------Arwen NEW ACT SegmentInfo
void CSTC::ReadActCtrlSegment( void )
{
    try
    {
        FILE * segment_rfile = NULL;
        char filename[25];
        for( int i=0; i<AMOUNT_SEGMENT; i++)
        {
            sprintf( filename, "/Data/ActCtrlSegment%d.bin\0", i); //cant find file chou 2013/01/30

            segment_rfile = fopen(filename, "r"); //fopen return NULL if file not exist
            if(segment_rfile)
            {
//#ifdef ShowInfoToSTD
                printf("Reading File ActCtrlSegment: %2d\n",i);
//#endif
                //ARWENADD
                fread( &segment[i]._actSegmentType1,  sizeof( unsigned short int ), 1, segment_rfile );
                fread( &segment[i]._actSegmentType2,  sizeof( unsigned short int ), 1, segment_rfile );

                for(int j=0; j<segment[i]._segment_count; j++)
                {
                    fread( &(segment[i]._ptr_seg_exec_time[j]._actMode),  sizeof( unsigned short int ), 1, segment_rfile );
                    //fread( &(segment[i]._ptr_seg_exec_time[j]._compensateMode),  sizeof( unsigned short int ), 1, segment_rfile );
                    fread( &(segment[i]._ptr_seg_exec_time[j]._openRunnig),  sizeof( unsigned short int ), 1, segment_rfile );

                    //segment[i]._ptr_seg_exec_time[j]._actMode = 0;
//==          printf("segment[%2d]._ptr_seg_exec_time[%2d]._hour:    %2d\n",i,j,segment[i]._ptr_seg_exec_time[j]._hour);
//==          printf("segment[%2d]._ptr_seg_exec_time[%2d]._minute:  %2d\n",i,j,segment[i]._ptr_seg_exec_time[j]._minute);
//==          printf("segment[%2d]._ptr_seg_exec_time[%2d]._planid:  %2d\n",i,j,segment[i]._ptr_seg_exec_time[j]._planid);
//==          printf("segment[%2d]._ptr_seg_exec_time[%2d]._cadc_seg:%2d\n",i,j,segment[i]._ptr_seg_exec_time[j]._cadc_seg);
                }
                fclose( segment_rfile );
            } //end if(fopen() succeed)
            else
            {
                segment[i]._actSegmentType1 = i;
                segment[i]._actSegmentType2 = i;
                printf("Arwen segment[%d]._actSegmentType1:%d",i,segment[i]._actSegmentType1);
                printf(" segment[%d]._actSegmentType2:%d\n",i,segment[i]._actSegmentType2);
                for(int j=0; j<segment[i]._segment_count; j++)
                {
                    segment[i]._ptr_seg_exec_time[j]._actMode = 0;
                    segment[i]._ptr_seg_exec_time[j]._compensateMode = 0;
                    segment[i]._ptr_seg_exec_time[j]._openRunnig = 1;
                }
            }
        }
    }
    catch(...) {}
}
//----------------------------------------------------------
void CSTC::ReadPrioritySegment(void)
{
    try
    {
        FILE * segment_rfile = NULL;
        char filename[25];
        for( int i=0; i<AMOUNT_SEGMENT; i++)
        {
            sprintf( filename, "/Data/PrioritySegmentInfo%d.bin\0", i);

            segment_rfile = fopen(filename, "r"); //fopen return NULL if file not exist
            if(segment_rfile)
            {
#ifdef ShowInfoToSTD
                printf("Reading File SegmentType: %2d\n",i);
#endif

                //OTADD
                // smem.vSetTCPhasePlanSegTypeData(TC_SegType, i, true);
                fread( &(BRTsegment[i]._segment_type) , sizeof( unsigned short int ), 1, segment_rfile);
                fread( &(BRTsegment[i]._segment_count) , sizeof( unsigned short int ), 1, segment_rfile);

                for(int j=0; j<BRTsegment[i]._segment_count; j++)
                {
                    fread( &(BRTsegment[i]._ptr_seg_exec_time[j]._hour),    sizeof( unsigned short int ), 1, segment_rfile );
                    fread( &(BRTsegment[i]._ptr_seg_exec_time[j]._minute),    sizeof( unsigned short int ), 1, segment_rfile );
                    fread( &(BRTsegment[i]._ptr_seg_exec_time[j]._planid),    sizeof( unsigned short int ), 1, segment_rfile );
                    fread( &(BRTsegment[i]._ptr_seg_exec_time[j]._onoff),    sizeof( unsigned short int ), 1, segment_rfile );
                }

                fclose( segment_rfile );
            } //end if(fopen() succeed)
        }
    }
    catch(...) {}
}
//----------------------------------------------------------
void CSTC::ReadPriorityParameter(void)
{
    try
    {
        FILE * plan_rfile = NULL;
        char filename[22];
        for( int i=0; i<AMOUNT_PLANID; i++)
        {
            sprintf( filename, "/Data/PriorityParameter%d.bin\0", i);

            plan_rfile = fopen(filename, "r"); //fopen return NULL if file not exist
            if(plan_rfile)
            {
#ifdef ShowInfoToSTD
                printf("Reading File PlanID: %2d\n",i);
#endif

                //OTADD
                //smem.vSetTCPhasePlanSegTypeData(TC_Plan, i, true);

                fread( &BRTplan[i]._planid,         sizeof( unsigned short int ), 1, plan_rfile );
                fread( &BRTplan[i]._phase_order,    sizeof( unsigned short int ), 1, plan_rfile );
                fread( &BRTplan[i]._east,           sizeof( unsigned short int ), 1, plan_rfile );
                fread( &BRTplan[i]._west,           sizeof( unsigned short int ), 1, plan_rfile );
                fread( &BRTplan[i]._th1_east,       sizeof( unsigned short int ), 1, plan_rfile );
                fread( &BRTplan[i]._th1_west,       sizeof( unsigned short int ), 1, plan_rfile );
                fread( &BRTplan[i]._th2,            sizeof( unsigned short int ), 1, plan_rfile );
                fread( &BRTplan[i]._time_east,      sizeof( unsigned short int ), 1, plan_rfile );
                fread( &BRTplan[i]._time_west,      sizeof( unsigned short int ), 1, plan_rfile );
                fread( &BRTplan[i]._BF13phasecount, sizeof( unsigned short int ), 1, plan_rfile );

                //jacky20141230
//      if(i>0 && i<41){
//        jsonPriority_Parameters(BRTplan[i]._planid,BRTplan[i]._east,BRTplan[i]._west,BRTplan[i]._th1_east,
//                              BRTplan[i]._th1_west,BRTplan[i]._time_east,BRTplan[i]._time_west,BRTplan[i]._th2);
//      }
                for(int k=0; k<2; k++)
                {
                    for(int j=0; j<BRTplan[i]._BF13phasecount; j++)
                    {
                        fread( &(BRTplan[i]._ptr_subplaninfo[j]._percentage[k]),   sizeof( unsigned short int ), 1, plan_rfile );
                        //jacky20141230
//            if(i>0 && i<41){
//                jsonPriority_Percentage(k,BRTplan[i]._ptr_subplaninfo[j]._percentage[k],j+1);
//            }
                    }
                    //jacky20141230
//          if(i>0 && i<41){
//              for(int j=BRTplan[i]._BF13phasecount; j<8; j++){
//                jsonPriority_Percentage(k,0,j+1);
//              }
//          }
                }
                fclose( plan_rfile );
            } //end if(fopen() succeed)
        }
    }
    catch (...) {}
}
//----------------------------------------------------------
//OT Debug
void CSTC::SetFlashAllRedPhaseInfo(void)
{
    try
    {
        printf("Setting Flash and AllRed Phases\n");

        if(phase[FLASH_PHASEORDER_HSINCHU]._subphase_count == 1)
        {
        }
        else
        {

            //Flash Phase
            phase[FLASH_PHASEORDER]._phase_order=FLASH_PHASEORDER;
            phase[FLASH_PHASEORDER]._signal_map = phase[FLASH_PHASEORDER_HSINCHU]._signal_map;  //constant
            phase[FLASH_PHASEORDER]._subphase_count = 1;
            phase[FLASH_PHASEORDER]._signal_count = phase[FLASH_PHASEORDER_HSINCHU]._signal_count;
            phase[FLASH_PHASEORDER]._total_step_count=1;
//mallocFuck    phase[FLASH_PHASEORDER]._ptr_subphase_step_count = (unsigned short int *)malloc(sizeof(unsigned short int)*phase[FLASH_PHASEORDER_HSINCHU]._subphase_count);
//mallocFuck    phase[FLASH_PHASEORDER]._ptr_subphase_step_signal_status = (unsigned short int ***)malloc(sizeof(unsigned short int **)*phase[FLASH_PHASEORDER_HSINCHU]._subphase_count);
            phase[FLASH_PHASEORDER]._ptr_subphase_step_count[0] = 1;
//mallocFuck  phase[FLASH_PHASEORDER]._ptr_subphase_step_signal_status[0] = (unsigned short int **)malloc(sizeof(unsigned short int *)*phase[FLASH_PHASEORDER_HSINCHU]._ptr_subphase_step_count[0]);
//mallocFuck  phase[FLASH_PHASEORDER]._ptr_subphase_step_signal_status[0][0] = (unsigned short int *)malloc(sizeof(unsigned short int)*phase[FLASH_PHASEORDER_HSINCHU]._signal_count);
            for(int i = 0; i < phase[FLASH_PHASEORDER_HSINCHU]._signal_count; i++)
            {
                phase[FLASH_PHASEORDER]._ptr_subphase_step_signal_status[0][0][i] = phase[FLASH_PHASEORDER_HSINCHU]._ptr_subphase_step_signal_status[0][0][i];
            }

        }

        //AllRed Phase
        phase[ALLRED_PHASEORDER]._phase_order=ALLRED_PHASEORDER;
        phase[ALLRED_PHASEORDER]._signal_map=0x77;  //constant
        phase[ALLRED_PHASEORDER]._subphase_count=1;
        phase[ALLRED_PHASEORDER]._signal_count=6;
        phase[ALLRED_PHASEORDER]._total_step_count=1;
//mallocFuck    phase[ALLRED_PHASEORDER]._ptr_subphase_step_count = (unsigned short int *)malloc(sizeof(unsigned short int)*phase[ALLRED_PHASEORDER]._subphase_count);
//mallocFuck    phase[ALLRED_PHASEORDER]._ptr_subphase_step_signal_status = (unsigned short int ***)malloc(sizeof(unsigned short int **)*phase[ALLRED_PHASEORDER]._subphase_count);
        phase[ALLRED_PHASEORDER]._ptr_subphase_step_count[0] = 1;
//mallocFuck  phase[ALLRED_PHASEORDER]._ptr_subphase_step_signal_status[0] = (unsigned short int **)malloc(sizeof(unsigned short int *)*phase[ALLRED_PHASEORDER]._ptr_subphase_step_count[0]);
//mallocFuck  phase[ALLRED_PHASEORDER]._ptr_subphase_step_signal_status[0][0] = (unsigned short int *)malloc(sizeof(unsigned short int)*phase[ALLRED_PHASEORDER]._signal_count);
        phase[ALLRED_PHASEORDER]._ptr_subphase_step_signal_status[0][0][0]=0x3003;
        phase[ALLRED_PHASEORDER]._ptr_subphase_step_signal_status[0][0][1]=0x3003;
        phase[ALLRED_PHASEORDER]._ptr_subphase_step_signal_status[0][0][2]=0x3003;
        phase[ALLRED_PHASEORDER]._ptr_subphase_step_signal_status[0][0][3]=0x3003;
        phase[ALLRED_PHASEORDER]._ptr_subphase_step_signal_status[0][0][4]=0x3003;
        phase[ALLRED_PHASEORDER]._ptr_subphase_step_signal_status[0][0][5]=0x3003;
    }
    catch(...) {}
}
//----------------------------------------------------------
void CSTC::SetFlashAllRedPlanInfo(void)
{
    try
    {
        printf("Setting Flash and AllRed Plans\n");
        //Flash Plan
        plan[FLASH_PLANID]._planid=FLASH_PLANID;
        plan[FLASH_PLANID]._dir=0x77;
        plan[FLASH_PLANID]._phase_order=FLASH_PHASEORDER;
        plan[FLASH_PLANID]._subphase_count=phase[FLASH_PHASEORDER]._subphase_count;
        plan[FLASH_PLANID]._cycle_time=9999;
        plan[FLASH_PLANID]._offset=0;
//mallocFuck    plan[FLASH_PLANID]._ptr_subplaninfo = (SSubPlanInfo *)malloc(sizeof(SSubPlanInfo)*plan[FLASH_PLANID]._subphase_count);
        plan[FLASH_PLANID]._ptr_subplaninfo[0]._green=9999;
        plan[FLASH_PLANID]._ptr_subplaninfo[0]._min_green=0;
        plan[FLASH_PLANID]._ptr_subplaninfo[0]._max_green=9999;
        plan[FLASH_PLANID]._ptr_subplaninfo[0]._yellow=0;
        plan[FLASH_PLANID]._ptr_subplaninfo[0]._allred=0;
        plan[FLASH_PLANID]._ptr_subplaninfo[0]._pedgreen_flash=0;
        plan[FLASH_PLANID]._ptr_subplaninfo[0]._pedred=0;

        //AllRed Plan
        plan[ALLRED_PLANID]._planid=ALLRED_PLANID;
        plan[ALLRED_PLANID]._dir=0x77;
        plan[ALLRED_PLANID]._phase_order=ALLRED_PHASEORDER;
        plan[ALLRED_PLANID]._subphase_count=phase[ALLRED_PHASEORDER]._subphase_count;
        plan[ALLRED_PLANID]._cycle_time=9999;
        plan[ALLRED_PLANID]._offset=0;
//mallocFuck    plan[ALLRED_PLANID]._ptr_subplaninfo = (SSubPlanInfo *)malloc(sizeof(SSubPlanInfo)*plan[ALLRED_PLANID]._subphase_count);
        plan[ALLRED_PLANID]._ptr_subplaninfo[0]._green=9999;
        plan[ALLRED_PLANID]._ptr_subplaninfo[0]._min_green=0;
        plan[ALLRED_PLANID]._ptr_subplaninfo[0]._max_green=9999;
        plan[ALLRED_PLANID]._ptr_subplaninfo[0]._yellow=0;
        plan[ALLRED_PLANID]._ptr_subplaninfo[0]._allred=0;
        plan[ALLRED_PLANID]._ptr_subplaninfo[0]._pedgreen_flash=0;
        plan[ALLRED_PLANID]._ptr_subplaninfo[0]._pedred=0;
    }
    catch(...) {}
}

//----------------------------------------------------------
//**********************************************************
//****  Set Default LCN, PhaseOrder, Plan and Segment Data
//**********************************************************
//----------------------------------------------------------
bool CSTC::SetDefaultLCNPhaseOrder(unsigned short int lcn, unsigned short int phase_order)
{
    try
    {
        printf("Setting Default LCN and PhaseOrder from Dip-Switch\n");
        if(LCN==LCN_NON_INITIALIZED_VALUE) LCN=lcn; //meaning that LCN not yet initialized
        if(_default_phaseorder==PHASEORDER_NON_INITIALIZED_VALUE) _default_phaseorder=phase_order; //meaning that _default_phaseorder not yet initialized
        _exec_phase = phase[_default_phaseorder];  //FIRST_TIME setting _exec_phase
        printf("    DEFAULT_LCN:%3d   DEFAULT_PHASE: %#X\n", LCN, _default_phaseorder);

        SetDefaultPlanSegmentData();

        if(phase[_default_phaseorder]._ptr_subphase_step_count) return true;
        else perror("PANIC ERROR: Default PhaseOrder Information not exist!!\nQuit...\n");
        return false;
    }
    catch(...) {}
}
//----------------------------------------------------------
void CSTC::SetDefaultPlanSegmentData(void)
{
    try
    {
        printf("Setting Default Plan and Segment\n");
        //Default Plan
        plan[DEFAULT_PLANID]._planid=DEFAULT_PLANID;
        plan[DEFAULT_PLANID]._dir=0x55;
        plan[DEFAULT_PLANID]._phase_order=_default_phaseorder;
        plan[DEFAULT_PLANID]._subphase_count=phase[_default_phaseorder]._subphase_count;
        plan[DEFAULT_PLANID]._offset=0;
//mallocFuck    plan[DEFAULT_PLANID]._ptr_subplaninfo = (SSubPlanInfo *)malloc(sizeof(SSubPlanInfo)*plan[DEFAULT_PLANID]._subphase_count);
        for(int i=0; i<plan[DEFAULT_PLANID]._subphase_count; i++)
        {
            plan[DEFAULT_PLANID]._ptr_subplaninfo[i]._green =          DEFAULT_PLAN_GREEN_TIME;
            plan[DEFAULT_PLANID]._ptr_subplaninfo[i]._min_green=0;
            plan[DEFAULT_PLANID]._ptr_subplaninfo[i]._max_green=9999;
            plan[DEFAULT_PLANID]._ptr_subplaninfo[i]._yellow =         DEFAULT_PLAN_YELLOW_TIME;
            plan[DEFAULT_PLANID]._ptr_subplaninfo[i]._allred =         DEFAULT_PLAN_ALLRED_TIME;
            plan[DEFAULT_PLANID]._ptr_subplaninfo[i]._pedgreen_flash = DEFAULT_PLAN_PGFLASH_TIME;
            plan[DEFAULT_PLANID]._ptr_subplaninfo[i]._pedred=0;
            plan[DEFAULT_PLANID]._cycle_time = plan[DEFAULT_PLANID].calculated_cycle_time();
        }
        _exec_plan = plan[DEFAULT_PLANID];  //FIRST_TIME setting _exec_plan
        _exec_BRTPriority_plan = BRTplan[DEFAULT_PLANID]; //jacky20140514

        //Default Segment
        segment[DEFAULT_SEGMENTTYPE]._segment_type=DEFAULT_SEGMENTTYPE;
        segment[DEFAULT_SEGMENTTYPE]._segment_count=1;
//mallocFuck    segment[DEFAULT_SEGMENTTYPE]._ptr_seg_exec_time = (SSegExecTime *)malloc(sizeof(SSegExecTime)*segment[DEFAULT_SEGMENTTYPE]._segment_count);
        segment[DEFAULT_SEGMENTTYPE]._ptr_seg_exec_time[0]._hour     = 0;
        segment[DEFAULT_SEGMENTTYPE]._ptr_seg_exec_time[0]._minute   = 0;
        segment[DEFAULT_SEGMENTTYPE]._ptr_seg_exec_time[0]._planid   = DEFAULT_PLANID;
        segment[DEFAULT_SEGMENTTYPE]._ptr_seg_exec_time[0]._cadc_seg = 0;
        _exec_segment = segment[DEFAULT_SEGMENTTYPE];  //FIRST_TIME setting _exec_segment
        _exec_BRTPriority_segment = BRTsegment[DEFAULT_SEGMENTTYPE]; //jacky20140423

        printf("    DEFAULT_SEGMENTTYPE:%2d   DEFAULT_PLANID: %2d\n", DEFAULT_PLANID, DEFAULT_SEGMENTTYPE);

    }
    catch(...) {}
}
//----------------------------------------------------------
//**********************************************************
//****      Set Light All Red for 3 seconds
//**********************************************************
//----------------------------------------------------------

bool CSTC::AllRed5Seconds(void)
{
    try
    {

        _current_strategy = STRATEGY_ALLRED;

        _exec_plan  = plan[ALLRED_PLANID];
        _exec_phase = phase[ALLRED_PHASEORDER];

        printf("Start All Red 3 second\n");
        if(clock_gettime(CLOCK_REALTIME, &strategy_start_time)<0) perror("Can not get strategy start time");

        ReSetStep(false);
        ReSetExtendTimer();
        SetLightAfterExtendTimerReSet();

        sleep(3);
//   sleep(1);
        printf("End All Red 5 second\n");

        SendRequestToKeypad();  //request keypad status
        BRT_QueryKeypadStatus(); //jacky20140114 request keypad status
    }
    catch (...) {}
}
//----------------------------------------------------------
//**********************************************************
//****      Threads Generation and Threads Functions
//**********************************************************
//----------------------------------------------------------
void CSTC::ThreadsGenerate()
{
    try
    {
        printf("STC Starting!\n");
        printf("\nMAIN:                 pid=%d\n",getpid());
        pthread_attr_t attr;
        pthread_attr_init( & attr );
        pthread_attr_setdetachstate( & attr, PTHREAD_CREATE_DETACHED );
//  pthread_create( & ( CSTC::_stc_thread_detector_info ), & attr, & ( CSTC::_stc_thread_detector_info_func ), NULL );
        pthread_create( & ( CSTC::_stc_thread_light_control ), & attr, & ( CSTC::_stc_thread_light_control_func ), NULL );

        pthread_attr_destroy( & attr );

    }
    catch (...) {}
}

//----------------------------------------------------------
//**********************************************************
//****       Report Traffic Information to Center
//**********************************************************
//----------------------------------------------------------
void CSTC::ReportLastPeriodTraffic(void)
{
    try
    {
        /*
        _rtms_info.display_status();

        static time_t now;
        static struct tm* currenttime;

        now = time(NULL);
        currenttime = localtime(&now);
        unsigned short int data_length = 18;
        unsigned char *data = (unsigned char *)malloc( 18*sizeof(unsigned char));

        data[0]  = 0x6F;
        data[1]  = 0x01;
        data[2]  = (currenttime->tm_year-11);
        data[3]  = (currenttime->tm_mon+1);
        data[4]  =  currenttime->tm_mday;
        data[5]  =  currenttime->tm_hour;
        data[6]  =  currenttime->tm_min;
        data[7]  =  currenttime->tm_sec;
        data[8]  = 0x01; //lane count = 1
        data[9]  = _rtms_info.vollong[0];
        data[10] = 0xFF;
        data[11] = _rtms_info.vol[0];
        data[12] = _rtms_info.sp[0];
        data[13] = 0xFF;
        data[14] = 0xFF;
        data[15] = _rtms_info.avgspeed;
        data[16] = _rtms_info.oc[0];
        data[17] = 0xFF;

        printf("%s[MESSAGE] Report Traffic Information During Last Period to Center.%s\n", ColorGreen, ColorNormal);
        //-------------------------

        //OTCombo0713
        MESSAGEOK _MsgOK;
        _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, data_length, true);
        _MsgOK.InnerOrOutWard = cOutWard;
        //  writeJob.WriteWorkByMESSAGEOUT(_MsgOK);
        writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICECENTER92);

        //-----------------------
        free(data);

        */
    }
    catch(...) {}
}
//----------------------------------------------------------
void CSTC::ReportAnalyzedTrafficStatus(const unsigned char &status)
{
    try
    {
        unsigned short int data_length = 3;
//mallocFuck  unsigned char *data = (unsigned char *)malloc( 3*sizeof(unsigned char) );
        unsigned char data[4];

        data[0] = 0x6F;
        data[1] = 0x18;
        data[2] = status;

        printf("%s[MESSAGE] Report Analyzed Traffic Status to Center: %#X.%s\n", ColorGreen, status, ColorNormal);

        /*+++++++++++++++++*/

        //OTCombo0713
        MESSAGEOK _MsgOK;
        _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, data_length, true);
//  _MsgOK.InnerOrOutWard = cOutWard;
//  writeJob.WriteWorkByMESSAGEOUT(_MsgOK);
        writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICECENTER92);

        /*-----------------*/
//mallocFuck  free(data);
    }
    catch(...) {}
}

//----------------------------------------------------------
//**********************************************************
//****      have Recoring info to Center
//**********************************************************
//----------------------------------------------------------
void CSTC::ReportHDDRecoding(const unsigned char &status)
{
    try
    {
        /*
        static time_t now;
        static struct tm* currenttime;

        now = time(NULL);
        currenttime = localtime(&now);
        unsigned short int data_length = 45;
        unsigned char *data = (unsigned char *)malloc( 45*sizeof(unsigned char));

        data[0]  = 0x6F;
        //OTCombo
        //  data[1]  = 0x0B;
        data[1]  = 0xFF;
        data[2]  = (currenttime->tm_year-11);
        data[3]  = (currenttime->tm_mon+1);
        data[4]  =  currenttime->tm_mday;
        data[5]  =  currenttime->tm_hour;
        data[6]  =  currenttime->tm_min;
        data[7]  =  currenttime->tm_sec;
        data[8]  =  (LCN / 256);
        data[9]  =  (LCN % 256);
        data[10]  = _rtms_info.id;
        data[11] = status;
        for(int k = 12; k < 20; k++)
          data[k] = _rtms_info.vol[k-12];
        for(int k = 20; k < 28; k++)
          data[k] = _rtms_info.vollong[k-20];
        for(int k = 28; k < 36; k++)
          data[k] = _rtms_info.sp[k-28];
        data[36] = _rtms_info.avgspeed;
        for(int k = 37; k < 45; k++)
          data[k] = _rtms_info.oc[k-37] / 10;


        //+++++++++++++++++

        //OTCombo0713
        MESSAGEOK _MsgOK;
        _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, data_length, true);
        //  _MsgOK.InnerOrOutWard = cOutWard;
        //  writeJob.WriteWorkByMESSAGEOUT(_MsgOK);
        writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICECENTER92);
        //-----------------
        free(data);
        */
    }
    catch(...) {}
}

//----------------------------------------------------------
//**********1************************************************
//****           Light Control Thread Function
//**********************************************************
//----------------------------------------------------------
void * CSTC::_stc_thread_light_control_func( void * )
{
    try
    {
        printf( "THREAD_LIGHT_CONTROL: pid=%d\n", getpid() );

        DATA_Bit uc5F10_ControlStrategy;

        ReadAllData();
        stc.Lock_to_Set_Control_Strategy(STRATEGY_ALLRED);
        AllRed5Seconds();
        Lock_to_Set_Control_Strategy(STRATEGY_TOD);
        SetDefaultLCNPhaseOrder(1,0);

/*        ConvertSegmentData(true);//Arwen add 110210

        TimersCreating();
        TimersSetting();
        usleep(1000);                                                                  //sleep, wait for keypad return strategy.
        ReSetStep(false);
        ReSetExtendTimer();
        SetLightAfterExtendTimerReSet();

        xml.SAVEALLXML();//arwen ++ read xml

        int signum = 0;
        int timerid = 9999;

        static time_t now;
        static struct tm* currenttime;

        sigset_t light_control_mask;
        sigemptyset( & light_control_mask ); //unblock all signal
        sigaddset( & light_control_mask, SIGNAL_RECORD_TRAFFIC );    //block VD_SIGNAL  59
        sigaddset( & light_control_mask, SIGNAL_TRAFFIC_CHANGED );    //block VD_SIGNAL  60
        sigaddset( & light_control_mask, SIGNAL_STRATEGY_CHANGED ); //block CHANGE_STRATEGY_SIGNAL  61
        sigaddset( & light_control_mask, SIGNAL_NEXT_STEP ); //block NEXT_STEP_SIGNAL  62
        sigaddset( & light_control_mask, SIGNAL_TIMER ); //block TIMER_SIGNAL  63

        sigaddset( & light_control_mask, SIGNAL_CONGESTED_threshold ); //block SIGNAL_CONGESTED_threshold  57
        sigaddset( & light_control_mask, SIGNAL_D_threshold ); //block SIGNAL_D_threshold  51
        sigaddset( & light_control_mask, SIGNAL_C_threshold ); //block SIGNAL_C_threshold  55
        sigaddset( & light_control_mask, SIGNAL_B_threshold ); //block SIGNAL_B_threshold  54

        sigaddset( & light_control_mask, RTSIGNAL_WDT_PLAN ); //block RTSIGNAL_WDT_PLAN,  56

        siginfo_t light_control_siginfo;

        unsigned char ucSayHelloToCard[21];  //OTCombo0713 SayHelloToCard
        unsigned char ucSayHelloToCardForDetSignalDriverUnit[21];  //OTCombo0713 SayHelloToCard
        unsigned char ucEA00[4];
        ucSayHelloToCard[0] = 0xAA;
        ucSayHelloToCard[1] = 0xBB;
        ucSayHelloToCard[2] = 0x12;
        ucSayHelloToCard[3] = 0x00;
        ucSayHelloToCard[4] = 0x00;
        ucSayHelloToCard[5] = 0x00;
        ucSayHelloToCard[6] = 0x00;
        ucSayHelloToCard[7] = 0x00;
        ucSayHelloToCard[8] = 0x00;
        ucSayHelloToCard[9] = 0x00;
        ucSayHelloToCard[10] = 0x00;
        ucSayHelloToCard[11] = 0x00;
        ucSayHelloToCard[12] = 0x00;
        ucSayHelloToCard[13] = 0x12;
        ucSayHelloToCard[14] = 0x00;
        ucSayHelloToCard[15] = 0x00;
        ucSayHelloToCard[16] = 0x00;
        ucSayHelloToCard[17] = 0x00;
        ucSayHelloToCard[18] = 0xAA;
        ucSayHelloToCard[19] = 0xCC;
        ucSayHelloToCard[20] = 0x00;
        for (int i=0; i<20; i++)
        {
            ucSayHelloToCard[20]^=ucSayHelloToCard[i];
        }

        ucEA00[0] = 0xEA;
        ucEA00[1] = 0x00;


        MESSAGEOK _MsgOK;
        unsigned short int usiRedCount[5] = {0, 0, 0, 0, 0};                       //For RedCount

        bCSTCInitialOK = true;

        smem.vSendRequestToKeypad();

        printf("TC Create OK!\n");
        smem.vSetCommEnable(true);

        while ( true )
        {

            while(smem.vGetTimerMutexRESET() == 1)
            {
//      printf("timer set, cstc wait!\n");
                usleep(100);
            }
            smem.vSetTimerMutexCSTC(1);
            signum = sigwaitinfo( & light_control_mask, & light_control_siginfo );

            switch ( signum )
            {
            case( SIGNAL_RECORD_TRAFFIC ):
                smem.vSetBOOLData(IFGetResetTCSignal, true);
                printf("Get Reset TC Signal By Console!\n");
                smem.vWriteMsgToDOM("Reset TC Signal By Console!");
                break;

            case( SIGNAL_TRAFFIC_CHANGED ):
//==        printf( "THREAD_LIGHT_CONTROL: getting signal from detector.\n" );
                break;

            case( SIGNAL_STRATEGY_CHANGED ):
                printf( "%s[MESSAGE] THREAD_LIGHT_CONTROL: getting signal from main for strategy changed.\n",  ColorGreen);
                if(clock_gettime(CLOCK_REALTIME, &strategy_start_time)<0) perror("Can not get strategy start time");
                switch(_current_strategy)
                {
                case(STRATEGY_TOD):
                    printf( "    strategy changed to: TOD.%s\n", ColorNormal);
                    switch(_old_strategy)
                    {
                        //OT Debug 951207
                    case(STRATEGY_ALLRED):
                        ReSetStep(false);
                        ReSetExtendTimer();
                        SetLightAfterExtendTimerReSet();

                        break;
                    case(STRATEGY_FLASH):
                        stc.Lock_to_Set_Control_Strategy(STRATEGY_ALLRED);
                        AllRed5Seconds();
                        stc.Lock_to_Set_Control_Strategy(STRATEGY_TOD);

                        //cheat, reask keypad again.
                        SendRequestToKeypad();

                        break;
                    case(STRATEGY_MANUAL):
                        ReSetStep(true);
                        ReSetExtendTimer();
                        SetLightAfterExtendTimerReSet();

                        break;
//OTADD like MANUAL
                    case(STRATEGY_ALLDYNAMIC):
                        ReSetStep(false);
                        ReSetExtendTimer();
                        SetLightAfterExtendTimerReSet();

                        break;
                    case(STRATEGY_CADC):

                        break;
                    case(STRATEGY_AUTO_CADC):

                        break;
                    }
                    break;

                case(STRATEGY_AUTO_CADC):
                    printf( "    strategy changed to: CADC.%s\n", ColorNormal);
                    switch(_old_strategy)
                    {
                    case(STRATEGY_ALLRED):
                    case(STRATEGY_FLASH):
                    case(STRATEGY_MANUAL):
                        //OTADD like MANUAL
                    case(STRATEGY_ALLDYNAMIC):
                        _current_strategy = _old_strategy; //could not be changed in these _old_strategy!!
                        break;
                    case(STRATEGY_CADC):

                        break;
                    case(STRATEGY_TOD):

                        break;
                    }
                    break;

                case(STRATEGY_CADC):
                    printf( "    strategy changed to: CADC.\n" );
                    switch(_old_strategy)
                    {
                    case(STRATEGY_ALLRED):
                    case(STRATEGY_FLASH):
                    case(STRATEGY_MANUAL):
                        //OTADD like MANUAL
                    case(STRATEGY_ALLDYNAMIC):
                        _current_strategy = _old_strategy; //could not be changed in these _old_strategy!!
                        break;
                    case(STRATEGY_AUTO_CADC):

                        break;
                    case(STRATEGY_TOD):

                        break;
                    }
                    break;

//OTADD like MANUAL
                case(STRATEGY_ALLDYNAMIC):
                    ReSetStep(false);
                    ReSetExtendTimer();
                    SetLightAfterExtendTimerReSet();

                    if(clock_gettime(CLOCK_REALTIME, &strategy_start_time)<0) perror("Can not get strategy start time");
                    if(_current_strategy==STRATEGY_ALLDYNAMIC)  printf( "    strategy changed to: ALLDYNAMIC.%s\n", ColorNormal);
                    break;

                case(STRATEGY_FLASH):
                case(STRATEGY_ALLRED):
                    if(clock_gettime(CLOCK_REALTIME, &strategy_start_time)<0) perror("Can not get strategy start time");
                    if(_current_strategy==STRATEGY_FLASH)  printf( "    strategy changed to: FLASH.%s\n", ColorNormal);
                    else if(_current_strategy==STRATEGY_ALLRED) printf( "    strategy changed to: ALLRED.%s\n", ColorNormal);
                    vSend0x16ToUnlockLCX405();                                          //OT970214NEWLCX405
                    ReSetStep(false);
                    ReSetExtendTimer();
                    SetLightAfterExtendTimerReSet();

                    break;

                case(STRATEGY_MANUAL):
                    if(_current_strategy==STRATEGY_MANUAL)      printf( "    strategy changed to: MANUAL.%s\n", ColorNormal);
                    //Check Current Phase is not 0xB0 (flash light)

                    //OT980406
                    if(_exec_phase._phase_order == 0xB0 || _exec_phase._phase_order == 0x80)
                    {
                        ReSetStep(true);
                    }
                    else
                    {
                        ReSetStep(false);
                    }


                    ReSetExtendTimer();
                    SetLightAfterExtendTimerReSet();

                    break;
                }
                break;

            case( SIGNAL_NEXT_STEP ):
//==        printf( "THREAD_LIGHT_CONTROL: getting signal from main to next step.\n" );

                uc5F10_ControlStrategy.DBit = smem.vGetUCData(TC92_ucControlStrategy);

                pthread_mutex_lock(&CSTC::_stc_mutex);
                if(_current_strategy==STRATEGY_MANUAL || _current_strategy==STRATEGY_ALLDYNAMIC
                        || uc5F10_ControlStrategy.DBit == 0x30)   //OT1000218
                {

                    if(clock_gettime(CLOCK_REALTIME, &strategy_start_time)<0) perror("Can not get strategy start time");
                    ReSetStep(true);
                    ReSetExtendTimer();
                    SetLightAfterExtendTimerReSet();

                    if(_current_strategy==STRATEGY_ALLDYNAMIC)
                    {
                        stc.vReportGoToNextPhaseStep_5F0C();
                    }
                }

                pthread_mutex_unlock(&CSTC::_stc_mutex);

                //OT Debug Signal 951116
                smem.vSetBOOLData(TC_SIGNAL_NEXT_STEP_OK, true);
                break;

            case( SIGNAL_TIMER ):
                timerid = light_control_siginfo.si_value.sival_int; //timerid={0,1,2,3,4}
//==        printf("THREAD_LIGHT_CONTROL: getting signal from ");
                switch (timerid)
                {
                case( 1000 ):  //_timer_plan
//==            printf( "TIMER: PLAN\n" );

                    pthread_mutex_lock(&CSTC::_stc_mutex);
                    if(_current_strategy==STRATEGY_TOD||_current_strategy==STRATEGY_AUTO_CADC||_current_strategy==STRATEGY_CADC)
                    {
                        ReSetStep(true);
                        ReSetExtendTimer();
                        SetLightAfterExtendTimerReSet();
                        if(smem.vGetBOOLData(TC_CCTActuate_TOD_Running) == true)
                        {
                            vCheckPhaseForTFDActuateExtendTime_5FCF();
                        }
                    }

                    pthread_mutex_unlock(&CSTC::_stc_mutex);
                    break;

                case( 1001 ):  //_timer_redcount
//==            printf( "TIMER: REDCOUNT_TIMEOUT\n" );

//            _intervalTimer.vSendHeartBeatToLCX405();  //let 8051 alive.
//MaskTest            stc.vSendHeartBeatToLCX405inCSTC();  //let 8051 alive.

                    if( smem.vGetINTData(TC92_RedCountVer) == TC_RedCountVer94 || smem.vGetINTData(TC92_RedCountVer) == TC_RedCountVer94v2)
                    {
                        _MsgOK = oDataToMessageOK.vPackageINFOToVer94RedCount(0x83, usiRedCount[0], usiRedCount[1], usiRedCount[2], usiRedCount[3], usiRedCount[4]);
                        _MsgOK.InnerOrOutWard = cOutWard;
                        writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICEREDCOUNTVER94);

                        ++usiRedCount[0];
                        if(usiRedCount[0] >= 5)
                        {
                            usiRedCount[0] = 0;
                        }
                    }
                    else if( smem.vGetINTData(TC92_RedCountVer) == TC_RedCountVerHK )
                    {
                        _MsgOK = oDataToMessageOK.vPackageINFOToVerHKRedCountComm();
                        _MsgOK.InnerOrOutWard = cOutWard;
                        writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICEREDCOUNTVERHK);
                    }
                    else if( smem.vGetINTData(TC92_RedCountVer) == TC_RedCountVerCCT97 )
                    {
                        _MsgOK = oDataToMessageOK.vPackageINFOTo92ProtocolSetADDR(ucEA00, 2, 0x36, 65535);      //brocasting
                        _MsgOK.InnerOrOutWard = cOutWard;
                        writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICEREDCOUNTVERCCT97);
                    }

                    break;

                case( 1002 ):  //_timer_panelcount
//==            printf( "TIMER: LIGHT_TIMEOUT\n" );

                    break;

                case( 1003 ):  //_timer_reportcount
//==            printf( "TIMER: REPORT_TIMEOUT\n" );

                    break;

                case( 1004 ):  //_timer_record_traffic


                    break;

                case (1005):
                    if(smem.vGetUCData(TC_ReverseLane_Manual_Enable_Status) == 0)
                    {
                        vDetermine_ReverseTime();
                        printf("smem.vGetUCData(TC_ReverseLane_Manual_Enable_Status) = 0\n");
                    }
                    else
                    {
                        printf("smem.vGetUCData(TC_ReverseLane_Manual_Enable_Status) = 1\n");
                    }
                    break;


                default:
                    perror( "CSTC TIMERID: error!!\n" );
                    break;
                }
                break;

            case (SIGNAL_CONGESTED_threshold):
                printf("old threshold_E %d\n", traffic_analyzer.threshold_E);
                pthread_mutex_lock(&CSTC::_stc_mutex);
                traffic_analyzer.threshold_E_by_signal = light_control_siginfo.si_int;
                traffic_analyzer.threshold_E = light_control_siginfo.si_int;
                pthread_mutex_unlock(&CSTC::_stc_mutex);
                printf("new threshold_E %d\n", traffic_analyzer.threshold_E);

                break;

            case (SIGNAL_D_threshold):
                printf("old threshold_D %d\n", traffic_analyzer.threshold_D);
                pthread_mutex_lock(&CSTC::_stc_mutex);
                traffic_analyzer.threshold_D = light_control_siginfo.si_int;
                pthread_mutex_unlock(&CSTC::_stc_mutex);
                printf("new threshold_D %d\n", traffic_analyzer.threshold_D);

                break;

            case (SIGNAL_C_threshold):
                printf("old threshold_C %d\n", traffic_analyzer.threshold_C);
                pthread_mutex_lock(&CSTC::_stc_mutex);
                traffic_analyzer.threshold_C = light_control_siginfo.si_int;
                pthread_mutex_unlock(&CSTC::_stc_mutex);
                printf("new threshold_C %d\n", traffic_analyzer.threshold_C);

                break;

            case (SIGNAL_B_threshold):
                printf("old threshold_B %d\n", traffic_analyzer.threshold_B);
                pthread_mutex_lock(&CSTC::_stc_mutex);
                traffic_analyzer.threshold_B = light_control_siginfo.si_int;
                pthread_mutex_unlock(&CSTC::_stc_mutex);
                printf("new threshold_B %d\n", traffic_analyzer.threshold_B);

                break;

            case( RTSIGNAL_WDT_PLAN ):  //_timer_plan_WDT

                pthread_mutex_lock(&CSTC::_stc_mutex);

                printf("WDT _current_strategy:%d\n", _current_strategy);

                if(_current_strategy==STRATEGY_TOD||_current_strategy==STRATEGY_AUTO_CADC||_current_strategy==STRATEGY_CADC)
                {
                    printf("WDT plan Start!\n");
//          printf("WDT plan Start!\n");

                    _itimer_plan_WDT.it_value.tv_sec = 5;                                 //default every 5 sec trigger.
                    timer_settime( _timer_plan_WDT, 0, &_itimer_plan_WDT, NULL );
                    timer_settime( _timer_redcount, 0, & _itimer_redcount, NULL );
                    timer_settime( _timer_plan_WDT, 0, &_itimer_plan_WDT, NULL );
                    timer_settime( _timer_redcount, 0, & _itimer_redcount, NULL );
                    timer_settime( _timer_plan_WDT, 0, &_itimer_plan_WDT, NULL );
                    timer_settime( _timer_redcount, 0, & _itimer_redcount, NULL );
                    timer_settime( _timer_plan_WDT, 0, &_itimer_plan_WDT, NULL );
                    timer_settime( _timer_redcount, 0, & _itimer_redcount, NULL );

                    smem.vWriteMsgToDOM("Plan WDT Start, what's happened?");

//==            printf( "TIMER: PLAN\n" );

                    ReSetStep(true);
                    ReSetExtendTimer();
                    SetLightAfterExtendTimerReSet();

                    if(smem.vGetBOOLData(TC_CCTActuate_TOD_Running) == true)
                    {
                        vCheckPhaseForTFDActuateExtendTime_5FCF();
                    }
                }
                else
                {
                    _itimer_plan_WDT.it_value.tv_sec = 0;                                 //default every 5 sec trigger.
                    _itimer_plan_WDT.it_interval.tv_sec = 0;                                 //default every 5 sec trigger.
                    timer_settime( _timer_plan_WDT, 0, &_itimer_plan_WDT, NULL );
                    timer_settime( _timer_redcount, 0, & _itimer_redcount, NULL );
                    timer_settime( _timer_plan_WDT, 0, &_itimer_plan_WDT, NULL );
                    timer_settime( _timer_redcount, 0, & _itimer_redcount, NULL );
                    timer_settime( _timer_plan_WDT, 0, &_itimer_plan_WDT, NULL );
                    timer_settime( _timer_redcount, 0, & _itimer_redcount, NULL );
                    timer_settime( _timer_plan_WDT, 0, &_itimer_plan_WDT, NULL );
                    timer_settime( _timer_redcount, 0, & _itimer_redcount, NULL );
                }
                pthread_mutex_unlock(&CSTC::_stc_mutex);

                break;



            default:
                perror( "THREAD_LIGHT_CONTROL: getting a strange signal!!\n" );
                exit (1);
            }
            //55123  ------2012/09/28-----chou-----------------
            //for 5F1C subphase = 1 ,subphase_step = 0 green = mingreen

            int Nowgreen = _exec_plan._ptr_subplaninfo[_exec_phase_current_subphase].compensated_green(_exec_plan._shorten_cycle) - stc.vGetStepTime();
            if (_exec_phase_current_subphase == 1 && _exec_phase_current_subphase_step == 4 && smem.Restartcheck5F1C == true)
            {
                smem.Restartcheck5F1C = false;
                stc.Fixgreen = false;
                //      printf("\n\n\n\n\nRestartcheck5F1C == 0!!\n\n\n\n");
            }
            else
            {
            }
            if(smem.Restartcheck5F1C == true)
            {
                //      printf("\n\n\n\nRestartcheck5F1C = true!\n\n\n\n\n");
            }
            Check5F1C();
            CheckPed();
            //----------------------------------------------------------
            smem.vSetTimerMutexCSTC(0);
        }*/
    }
    catch (...) {}
}
//----------------------------------------------------------
//**********************************************************
//****         Creating and Setting Timers
//**********************************************************
//----------------------------------------------------------
void CSTC::TimersCreating(void)
{
    try
    {
        struct sigevent evt_plan, evt_redcount, evt_panelcount, evt_reportcount, evt_record_traffic, evt_plan_WDT;
        struct sigevent evt_reversetime;
        evt_plan.sigev_notify = SIGEV_SIGNAL;
        evt_plan.sigev_signo = SIGNAL_TIMER;
        evt_plan.sigev_value.sival_int = 1000; //timerid=0
        if ( timer_create( CLOCK_REALTIME, & evt_plan, & _timer_plan ) ) exit( 1 );
        evt_redcount.sigev_notify = SIGEV_SIGNAL;
        evt_redcount.sigev_signo = SIGNAL_TIMER;
        evt_redcount.sigev_value.sival_int = 1001; //timerid=1
        if ( timer_create( CLOCK_REALTIME, & evt_redcount, & _timer_redcount ) ) exit( 1 );
        evt_panelcount.sigev_notify = SIGEV_SIGNAL;
        evt_panelcount.sigev_signo = SIGNAL_TIMER;
        evt_panelcount.sigev_value.sival_int = 1002; //timerid=2
        if ( timer_create( CLOCK_REALTIME, & evt_panelcount, & _timer_panelcount ) ) exit( 1 );
        evt_reportcount.sigev_notify = SIGEV_SIGNAL;
        evt_reportcount.sigev_signo = SIGNAL_TIMER;
        evt_reportcount.sigev_value.sival_int = 1003; //timerid=3
        if ( timer_create( CLOCK_REALTIME, & evt_reportcount, & _timer_reportcount ) ) exit( 1 );
        evt_record_traffic.sigev_notify = SIGEV_SIGNAL;
        evt_record_traffic.sigev_signo = SIGNAL_TIMER;
        evt_record_traffic.sigev_value.sival_int = 1004; //timerid=4
        if ( timer_create( CLOCK_REALTIME, & evt_record_traffic, & _timer_record_traffic ) ) exit( 1 );
        evt_reversetime.sigev_notify = SIGEV_SIGNAL;
        evt_reversetime.sigev_signo = SIGNAL_TIMER;
        evt_reversetime.sigev_value.sival_int = 1005; //timerid= ?
        if ( timer_create( CLOCK_REALTIME, & evt_reversetime, & _timer_reversetime ) ) exit( 1 );

        evt_plan_WDT.sigev_notify = SIGEV_SIGNAL;
        evt_plan_WDT.sigev_signo = RTSIGNAL_WDT_PLAN;
        evt_plan_WDT.sigev_value.sival_int = 0; //timerid=0
        if ( timer_create( CLOCK_REALTIME, & evt_plan_WDT, & _timer_plan_WDT ) ) exit( 1 );


    }
    catch (...) {}
}
//----------------------------------------------------------
void CSTC::TimersSetting(void)
{
    try
    {

        _itimer_panelcount.it_value.tv_sec = LIGHT_TIMEOUT;
        _itimer_panelcount.it_value.tv_nsec = 0;
        _itimer_panelcount.it_interval.tv_sec = LIGHT_TIMEOUT;
        _itimer_panelcount.it_interval.tv_nsec = 0;
        if ( timer_settime( _timer_panelcount, 0, & _itimer_panelcount, NULL ) ) exit( 1 );
        _itimer_reportcount.it_value.tv_sec = REPORT_TIMEOUT;
        _itimer_reportcount.it_value.tv_nsec = 0;
        _itimer_reportcount.it_interval.tv_sec = REPORT_TIMEOUT;
        _itimer_reportcount.it_interval.tv_nsec = 0;
        if ( timer_settime( _timer_reportcount, 0, & _itimer_reportcount, NULL ) ) exit( 1 );
        _itimer_record_traffic.it_value.tv_sec = RECORD_TRAFFIC_TIMEOUT;
        _itimer_record_traffic.it_value.tv_nsec = 0;
        _itimer_record_traffic.it_interval.tv_sec = RECORD_TRAFFIC_TIMEOUT;
        _itimer_record_traffic.it_interval.tv_nsec = 0;
        if ( timer_settime( _timer_record_traffic, 0, & _itimer_record_traffic, NULL ) ) exit( 1 );

        _itimer_reversetime.it_value.tv_sec = REVERSETIME_TIMEOUT;
        _itimer_reversetime.it_value.tv_nsec = 0;
        _itimer_reversetime.it_interval.tv_sec = REVERSETIME_TIMEOUT;
        _itimer_reversetime.it_interval.tv_nsec = 0;
        if ( timer_settime( _timer_reversetime, 0, & _itimer_reversetime, NULL ) ) exit( 1 );

        _itimer_redcount.it_value.tv_sec = 0;
//    _itimer_redcount.it_value.tv_nsec = 250000000;  // 0.25 sec.
        _itimer_redcount.it_value.tv_nsec = 500000000;  // 0.50 sec.
        _itimer_redcount.it_interval.tv_sec = REDCOUNT_TIMEOUT;
        _itimer_redcount.it_interval.tv_nsec = 0;
        if ( timer_settime( _timer_redcount, 0, & _itimer_redcount, NULL ) ) exit( 1 );


    }
    catch (...) {}
}
//----------------------------------------------------------
//**********************************************************
//****        Reset Step, Timer and Set Light
//**********************************************************
//----------------------------------------------------------
void CSTC::ReSetStep(bool step_up)
{
    try
    {
        FILE *fileToReset = 0;
        char cWriteMsg[255] = { 0 };
        unsigned char ucTmp;
        unsigned char ucMotherOutputStartSubPhaseId;
        unsigned char ucMotherOutputStartStepId;
        unsigned char ucMotherOutputEndSubPhaseId;
        unsigned char ucMotherOutputEndStepId;
        DATA_Bit tempDIOByte;
        tempDIOByte.DBit = 0;
        bool bSendDIO = false;
        //990527
        if(smem.vGetThisCycleRunCCJActure5FA2() == true)
        {
            smem.vSetThisCycleRunCCJActure5FA2(false);
        }
//////////////////////////////////////////////////////
        if( _current_strategy==STRATEGY_ALLRED )
        {
            _exec_plan=plan[ALLRED_PLANID];
            _exec_phase=phase[ALLRED_PHASEORDER];
            _exec_phase_current_subphase=0;
            _exec_phase_current_subphase_step=0;
            return;
        }//end if(STRATEGY_ALLRED)
////////////////////////////////////////////////////
        else if( _current_strategy==STRATEGY_FLASH )
        {
            _exec_plan=plan[FLASH_PLANID];
            _exec_phase=phase[FLASH_PHASEORDER];
            _exec_phase_current_subphase=0;
            _exec_phase_current_subphase_step=0;
            return;
        }//end else if(STRATEGY_FLASH)
////////////////////////////////////////////////////
        else if( _current_strategy==STRATEGY_MANUAL || _current_strategy==STRATEGY_CADC
                 || _current_strategy==STRATEGY_AUTO_CADC || _current_strategy==STRATEGY_TOD || _current_strategy==STRATEGY_ALLDYNAMIC)
        {
            bool finding=true;
            smem.vSetStepRunCounter(0); //OT20140214
            while(finding)    //步階 分相 轉換
            {
                if(step_up)
                {
                    printf("step_up is True, current _exec_phase_current_subphase_step is %d, ", _exec_phase_current_subphase_step);
                    _exec_phase_current_subphase_step += 1; //go to next step
                    //unsigned short int vTmp = smem.vGetStepRunCounter();
                    if(_exec_phase_current_subphase_step >= _exec_phase._ptr_subphase_step_count[_exec_phase_current_subphase])
                    {
                        _exec_phase_current_subphase_step=0;  //go the the first step of next step
                        smem.usCutRedAlready=0;
                        _exec_phase_current_subphase += 1;  //go to next phase
                        smem.SetExtendedswitch(0);  //jacky20140731
                        if(smem.GetLearningPGPRAcrossCycle())
                        {
//            if(_exec_phase_current_subphase%2==1)
                            smem.SetLearningPGPRsubphase(true); //jacky20141202
//            else
//                smem.SetLearningPGPRsubphase(false);
                        }
                        else smem.SetLearningPGPRsubphase(false);
                        if(_exec_phase_current_subphase >= _exec_phase._subphase_count)
                            _exec_phase_current_subphase=0; //go to the first subphase

                        /* if(smem.GetPriorityswitch() == true && smem.GetBF08Parameter() > 0){   //Priority是否啟動 是否延長秒數
                             BF02_Reduce(_exec_phase_current_subphase);
                             printf("\n\n ****** BF02_Reduce ****** \n\n");
                         }*/
                        if(_exec_phase_current_subphase > 0)
                            BF02_PriorityParameters();  //jacky20140520  BF02
                    }
                    printf("after _exec_phase_current_subphase_step is %d", _exec_phase_current_subphase_step);
                }
                else
                {
                    step_up=true;
                }
                /*OT+9080205*/
                if(_exec_phase_current_subphase_step == 0 && bJumpSubEnable == true)      //next new subphase, force jump to define psubphase
                {
                    bJumpSubEnable = false;
                    ucTmp = smem.vGetUCData(TC_TrainComingForceJumpSubphase);
                    if(ucTmp < _exec_phase._subphase_count)
                    {
                        _exec_phase_current_subphase = ucTmp;
                    }
                }
                if(_exec_phase_current_subphase==0 && _exec_phase_current_subphase_step==0)
                {
                    //990527
                    //OT20111028 Disable
                    /*
                    if(smem.vGetBOOLData(TC92_PlanOneTime5F18_Change) == false) {  //close this cycle.
                      if(smem.vGetThisCycleRunCCJPlan5F18() == true) {
                        smem.vSetThisCycleRunCCJPlan5F18(false);
                      }
                    }
                    */
                    //20110702, check file for reset enable
                    fileToReset = fopen("/tmp/resetTCmyself.txt", "r");
                    if(fileToReset)
                    {
                        fclose(fileToReset);
                        smem.vSetBOOLData(IFGetResetTCSignal, true);
                        system("rm -rf /tmp/resetTCmyself.txt");
                        smem.vWriteMsgToDOM("Reset TC Signal By Console File!");
                        printf("!! Reset TC Signal By Console File! !!");
                        system("sync");
                        system("sync");
                    }
                    //jacky20140821
                    if(smem.GetPriorityswitch())
                    {
                        if(smem.GetSegmentChange())
                        {
                            if(CycleRunCount >= 4)
                            {
                                smem.SetSegmentChange(false);
                                CycleRunCount = 0;
                            }
                            else
                                CycleRunCount++;
                        }
                        if(smem.GetPriorityStrategiesIN())
                        {
                            CountBF08++;
                            smem.SetPriorityStrategiesIN(false);
                        }
                        TenCycle++;
                        if(TenCycle == 10)
                        {
                            if(CountBF08 >= TenCycle)
                                smem.SetSegmentChange(true);
                            CountBF08 = 0;
                            TenCycle = 0;
                        }
                    }
                    //jacky20141216 shutdown LearningPGPR count
                    if(smem.GetPriorityswitch() == true || smem.GetGreenManonoff5014() == 1 || smem.GetCaronoff5015() == 1)
                    {
                        if(smem.GetCarLearnSwitch() == 1 || smem.GetPedLearnSwitch() == 1)
                        {
                            if(LearningPGPRCycle==10) LearningPGPRCycle = 0;
                            if((LearningPGPRCycle % 2) == 0)
                            {
                                smem.SetLearningPGPRAcrossCycle(true);
                            }
                            else
                            {
                                smem.SetLearningPGPRAcrossCycle(false);
                            }
                            LearningPGPRCycle++;
                        }
                        else smem.SetLearningPGPRAcrossCycle(false);
                    }
                    else smem.SetLearningPGPRAcrossCycle(false);
                    printf("\n     ***************************************\n");
                    printf(  "     ***        Cycle Complete!!         ***\n");
                    printf(  "     *** Checking Segment, Plan, Phase!! ***\n");
                    printf(  "     ***************************************\n\n");
                    //OT20140415
                    bool bDynSegSwitch = smem.vGetBOOLData(TCDynSegSwitch);
                    if(bDynSegSwitch)
                    {
                        Lock_to_Determine_SegmentPlanPhase_DynSeg();  //OT20140415
                    }
                    Lock_to_Determine_SegmentPlanPhase(); //determine the undergoing segment, plan, phase
                    if(pedcyclecount > 2 || false == smem.vGetAdjcount())       //如有GPS訊號就不做保護
                    {
                        smem.SetPedopen(true);  //開啟倒數燈 2013/05/31 chou++
                        pedcyclecount++;
                    }
                    else
                    {
//OT1218            smem.SetPedopen(false);
                        smem.SetPedopen(true);
                        pedcyclecount++;
                    }
                }
                switch(_exec_phase_current_subphase_step)
                {
                case(0):
                    if(_exec_plan._ptr_subplaninfo[_exec_phase_current_subphase]._green > 0)
                        finding=false;
                    break;
                case(1):
                    if(_exec_plan._ptr_subplaninfo[_exec_phase_current_subphase]._pedgreen_flash>0)
                        finding=false;
                    break;
                case(2):
                    if(_exec_plan._ptr_subplaninfo[_exec_phase_current_subphase]._pedred>0)
                        finding=false;
                    break;
                case(3):
                    if(_exec_plan._ptr_subplaninfo[_exec_phase_current_subphase]._yellow>0)
                        finding=false;
                    break;
                case(4):
                    if(_exec_plan._ptr_subplaninfo[_exec_phase_current_subphase]._allred>0)
                        finding=false;
                    break;
                }//switch(step_time>0)

                /*OT20110707
                      //for the sake of dealing the TOD plans holding step_time==0 and _phase_order==FLASH or ALLRED
                      if(_exec_plan._phase_order==FLASH_PHASEORDER||_exec_plan._phase_order==ALLRED_PHASEORDER) {
                //OT20110705
                        _exec_phase_current_subphase_step = 0;

                        finding=false;
                      }
                */
            }

            if(_current_strategy==STRATEGY_MANUAL || _current_strategy==STRATEGY_ALLDYNAMIC)
            {
                printf("ReSetStep: phase:(%d/%d) step:(%d/%d)\n"
                       , _exec_phase_current_subphase+1,      _exec_phase._subphase_count
                       , _exec_phase_current_subphase_step+1, _exec_phase._ptr_subphase_step_count[_exec_phase_current_subphase]);
            }
            //Here, new add mother chain
            if(_current_strategy==STRATEGY_MANUAL)
            {
                if(smem.vGetUCData(TC92_5F31Manual))
                {
                    bSendDIO = true;
                }
            }
            if(_current_strategy==STRATEGY_TOD)
            {
                if(smem.vGetUCData(TC92_5F31TOD))
                {
                    bSendDIO = true;
                }
            }
            if(bSendDIO)
            {
                ucMotherOutputStartSubPhaseId = smem.vGetUCData(TC92_5F31StartSubPhaseId);
                ucMotherOutputStartStepId = smem.vGetUCData(TC_MotherChainStartStepId);
                ucMotherOutputEndSubPhaseId = smem.vGetUCData(TC92_5F31EndSubPhaseId);
                ucMotherOutputEndStepId = smem.vGetUCData(TC_MotherChainEndStepId);
                if(ucMotherOutputStartSubPhaseId !=0 && ucMotherOutputStartStepId != 0 && ucMotherOutputEndSubPhaseId !=0 && ucMotherOutputEndStepId != 0 )
                {
                    if(_exec_phase_current_subphase == ucMotherOutputStartSubPhaseId-1 && _exec_phase_current_subphase_step == ucMotherOutputStartStepId -1)
                    {
                        tempDIOByte.switchBit.b1 = TC_CHAIN_SIGNAL_IN_START_SUB_PAHSE;
                        smem.vWriteDIO(tempDIOByte.DBit);
                    }
                    if(_exec_phase_current_subphase == ucMotherOutputEndSubPhaseId-1 && _exec_phase_current_subphase_step == ucMotherOutputEndStepId -1)
                    {
                        tempDIOByte.switchBit.b1 = TC_CHAIN_SIGNAL_IN_END_SUB_PAHSE;
                        smem.vWriteDIO(tempDIOByte.DBit);
                    }
                }
            }
        }//end else if(STRATEGY_MANUAL||STRATEGY_CADC||STRATEGY_AUTO_CADC||STRATEGY_TOD)
//  sprintf(cWriteMsg, "ReSetNew, P:%d, S:%d", _exec_phase_current_subphase, _exec_phase_current_subphase_step);
//  smem.vWriteMsgToDOM(cWriteMsg);

    }
    catch (...) {}
}
//--------------jacky20140520----------------------------------------
void CSTC::BF02_PriorityParameters(void)
{
    try
    {
        unsigned char data[50];
        int iDataPtr = 14;
        data[0] = 0xBF;
        data[1] = 0x02;
        data[2] = _exec_BRTPriority_plan._planid;
        data[3] = _exec_BRTPriority_plan._phase_order;
        data[4] = _exec_BRTPriority_plan._east;
        data[5] = _exec_BRTPriority_plan._west;
        data[6] = _exec_BRTPriority_plan._th1_east;
        data[7] = _exec_BRTPriority_plan._th1_west;
        data[8] = _exec_BRTPriority_plan._th2;
        data[9] = _exec_BRTPriority_plan._time_east / 256;
        data[10] = _exec_BRTPriority_plan._time_east % 256;
        data[11] = _exec_BRTPriority_plan._time_west / 256;
        data[12] = _exec_BRTPriority_plan._time_west % 256;
        data[13] = _exec_BRTPriority_plan._BF13phasecount;

        for(int k=0; k<2; k++)
        {
            for (int j=0 ; j < _exec_BRTPriority_plan._BF13phasecount; j++ )
            {
                data[iDataPtr] = _exec_BRTPriority_plan._ptr_subplaninfo[j]._percentage[k];
                iDataPtr++;
            }
        }
        if(smem.GetBF11onoff() == 1)
        {
            data[iDataPtr] = 0x02;
            iDataPtr++;
        }
        else if(smem.GetDefiniteTimeonoff() == 1)
        {
            data[iDataPtr] = 0x03;
            iDataPtr++;
        }
        else
        {
            data[iDataPtr] = 0x00;
            iDataPtr++;
        }

        unsigned char *tmpA;
        tmpA = BF02_Extendsec(_exec_phase_current_subphase);
        for(int i=0; i< _exec_BRTPriority_plan._BF13phasecount*2; i++)
        {
            data[iDataPtr] = *(tmpA+i);
            iDataPtr++;
        }

        printf("\n\n!! ");
        printf(" subphase %d !! ",_exec_phase_current_subphase);
        for(int j=0; j<iDataPtr; j++)
        {
            printf("%02X ",data[j]);
        }
        printf("!!\n\n");

        MESSAGEOK _MsgOK;
        _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, iDataPtr, true);
        _MsgOK.InnerOrOutWard = cOutWard;
        writeJob.WritePhysicalOutNoSetSeqNoResend(_MsgOK.packet, _MsgOK.packetLength, DEVICEBRT);
        writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICECENTER92);    //中心

    }
    catch (...) {}
}
//--------------jacky20140515--------------------------------
unsigned char * CSTC::BF02_Extendsec(unsigned short int e_subphase)
{
    try
    {
        unsigned char data[_exec_BRTPriority_plan._BF13phasecount*2];
        unsigned char east = _exec_BRTPriority_plan._east;  //東行可延長分相
        unsigned char west = _exec_BRTPriority_plan._west;  //西行可延長分相
        unsigned char tmpshift=0x0;
        unsigned int iDataPtr = 0;

        memset(data, '\0', sizeof(data));

        for(int i=0; i<e_subphase; i++)
        {
            data[iDataPtr] = 0;
            iDataPtr++;
        }
        for(int i=e_subphase+1; i<=_exec_BRTPriority_plan._BF13phasecount; i++)
        {
            tmpshift = (east << (8-i)) & 0x80;      //檢查是否為可延長的subphase
            if(tmpshift == 0x80)
            {
                if(i < _exec_BRTPriority_plan._BF13phasecount)
                {
                    for(int j=i; j<_exec_BRTPriority_plan._BF13phasecount; j++)
                    {
                        data[iDataPtr] = data[iDataPtr]+(_exec_plan._ptr_subplaninfo[j]._green - _exec_plan._ptr_subplaninfo[j]._min_green);
                        //printf("\n\n 1##  %d  %d  %d  %d  %d  ##1\n\n",j,_exec_plan._ptr_subplaninfo[j]._green,_exec_plan._ptr_subplaninfo[j]._min_green,data[iDataPtr],iDataPtr);
                    }
                    iDataPtr++;
                }
                else
                {
                    data[iDataPtr] = 0;
                    iDataPtr++;
                }
            }
            else
            {
                data[iDataPtr] = 0;
                iDataPtr++;
            }
        }

        for(int i=0; i<e_subphase; i++)
        {
            data[iDataPtr] = 0;
            iDataPtr++;
        }
        for(int i=e_subphase+1; i<=_exec_BRTPriority_plan._BF13phasecount; i++)
        {
            tmpshift = (west << (8-i)) & 0x80;
            if(tmpshift == 0x80)
            {
                if(i < _exec_BRTPriority_plan._BF13phasecount)
                {
                    for(int j=i; j<_exec_BRTPriority_plan._BF13phasecount; j++)
                    {
                        data[iDataPtr] = data[iDataPtr]+(_exec_plan._ptr_subplaninfo[j]._green - _exec_plan._ptr_subplaninfo[j]._min_green);
                        //printf("\n\n 2##  %d  %d  %d  %d  %d  ##2\n\n",j,_exec_plan._ptr_subplaninfo[j]._green,_exec_plan._ptr_subplaninfo[j]._min_green,data[iDataPtr],iDataPtr);
                    }
                    iDataPtr++;
                }
                else
                {
                    data[iDataPtr] = 0;
                    iDataPtr++;
                }
            }
            else
            {
                data[iDataPtr] = 0;
                iDataPtr++;
            }
        }

        return data;
    }
    catch (...) {}
}
//--------------jacky20140515---------------------------------
void CSTC::BF02_Reduce(unsigned short int r_subphase)
{
    try
    {
        unsigned int tmpParameter = smem.GetBF08Parameter();  //延長時間
        unsigned int tmpDir = smem.GetBF0BDIR();
        unsigned int surplus = 0, tmpX = 0, j = 0;
        bool eastbool = true;
        bool westbool = true;
        unsigned int tmpPercentage[16]= {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}; //max :_exec_BRTPriority_plan._BF13phasecount*2=16
//unsigned int *tmpPercentage=new (unsigned int)(_exec_BRTPriority_plan._BF13phasecount*2);

        // memset(tmpPercentage,'\0',sizeof(tmpPercentage));

        unsigned int k=0;
        for(int j=0; j<2; j++)
        {
            for(int i=0; i<_exec_BRTPriority_plan._BF13phasecount; i++)
            {
                tmpPercentage[k] = _exec_BRTPriority_plan._ptr_subplaninfo[i]._percentage[j];
                k++;
            }
        }
        if(tmpDir == 0)      //east
        {
            while(eastbool)
            {
                for(int i=r_subphase+1; i<_exec_BRTPriority_plan._BF13phasecount; i++)
                {
                    if(_exec_plan._ptr_subplaninfo[i]._green <= _exec_plan._ptr_subplaninfo[i]._min_green) {}
                    else
                    {
                        tmpX = floor(tmpParameter * ((float)tmpPercentage[i]/100));
                        if((_exec_plan._ptr_subplaninfo[i]._green - tmpX) < _exec_plan._ptr_subplaninfo[i]._min_green)
                        {
                            surplus = surplus +(_exec_plan._ptr_subplaninfo[i]._min_green-(_exec_plan._ptr_subplaninfo[i]._green - tmpX));
                            _exec_plan._ptr_subplaninfo[i]._green = _exec_plan._ptr_subplaninfo[i]._min_green;
                        }
                        else
                        {
                            _exec_plan._ptr_subplaninfo[i]._green = _exec_plan._ptr_subplaninfo[i]._green - tmpX;
                        }
                    }
                }
                if (surplus > 1)
                {
                    tmpParameter = surplus;
                    surplus = 0;
                }
                else
                {
                    surplus = 0;
                    eastbool = false;
                }
            }
        }
        else if(tmpDir == 1)
        {
            while(westbool)
            {
                for(int i=r_subphase+1; i<_exec_BRTPriority_plan._BF13phasecount; i++)
                {
                    if(_exec_plan._ptr_subplaninfo[i]._green <= _exec_plan._ptr_subplaninfo[i]._min_green) {}
                    else
                    {
                        tmpX = floor(tmpParameter * ((float)tmpPercentage[i]/100));
                        if((_exec_plan._ptr_subplaninfo[i]._green - tmpX) < _exec_plan._ptr_subplaninfo[i]._min_green)
                        {
                            surplus = surplus +(_exec_plan._ptr_subplaninfo[i]._min_green-(_exec_plan._ptr_subplaninfo[i]._green - tmpX));
                            _exec_plan._ptr_subplaninfo[i]._green = _exec_plan._ptr_subplaninfo[i]._min_green;
                        }
                        else
                        {
                            _exec_plan._ptr_subplaninfo[i]._green = _exec_plan._ptr_subplaninfo[i]._green - tmpX;
                        }
                    }
                }
                if (surplus > 1)
                {
                    tmpParameter = surplus;
                    surplus = 0;
                }
                else
                {
                    surplus = 0;
                    westbool = false;
                }
            }
        }
        smem.SetBF08Parameter(0);
        PlanUpdate = true;
    }
    catch (...) {}
}

//----------------------------------------------------------
void CSTC::ReSetExtendTimer(void)
{
    try
    {
        char cTMP[128];
        sChildChain sCCTMP;
        unsigned short int usiTmp;
        unsigned char ucBanSubphase;
        unsigned char ucChildInputEnable = 0;
        unsigned char ucTmp1, ucTmp2;
        ucTmp1 = smem.vGetUCData(TC92_5F32StartSubPhaseId);           //ScreenChildChainSet use
        ucTmp2 = smem.vGetUCData(TC92_5F32EndSubPhaseId);
        if(ucTmp1 != 0 && ucTmp2 != 0)
        {
            ucChildInputEnable = 1;
        }


        if(_current_strategy==STRATEGY_ALLRED || _current_strategy==STRATEGY_FLASH ||
                _current_strategy==STRATEGY_MANUAL || _current_strategy==STRATEGY_ALLDYNAMIC)
//    CalculateAndSendRedCount(0);
//OT Debug 950816
            CalculateAndSendRedCount(10);

        else if( _current_strategy==STRATEGY_CADC || _current_strategy==STRATEGY_AUTO_CADC || _current_strategy==STRATEGY_TOD )
        {
            if( _exec_plan._phase_order==FLASH_PHASEORDER||_exec_plan._phase_order==ALLRED_PHASEORDER )
            {
                _itimer_plan.it_value.tv_sec=10;
            }

            else
            {
                if( _exec_phase_current_subphase==0 && _exec_phase_current_subphase_step==0 )
                {
                    if(_current_strategy==STRATEGY_TOD)
                    {
                        sCCTMP = smem.vGetChildChainStruct();
                        printf("In STRATEGY_TOD, bHaveReasonableChildChainSignal:%d, ucChildInputEnable:%d\n",
                               sCCTMP.bHaveReasonableChildChainSignal, ucChildInputEnable);
                        if(sCCTMP.bHaveReasonableChildChainSignal && ucChildInputEnable)                            //should check childchain enable?
                        {
                            printf("in CSTC, sCCTMP.bHaveReasonableChildChainSignal is true\n");
                            vCalculateCompensation_in_CHAIN();
                        }
                        else
                        {
                            CalculateCompensation_in_TOD();
                        }
                    }
                    else
                    {
                        CalculateCompensation_in_CADC();
                    }
                }

                if( _exec_phase_current_subphase_step==0 )
                {
                    CalculateAndSendRedCount(0); //calculate red count after compensation calculated
//OT20131221          CalculateAndSendNSPredCount(0);
//OT20131221          CalculateAndSendGreenmanCount(0);                                         //計算行人倒數秒數

                    //OT20131221
                    CalculatePgCount();
                    //jacky20141121
                    CalculatePrCount();
                }
                pthread_mutex_lock(&CPlanInfo::_plan_mutex);
                vCalculateAndSendPeopleLightCount();
                switch(_exec_phase_current_subphase_step)
                {
                case(0):
                    printf("  %d green_time=%3d compensation_time=%3d \n", _exec_plan._shorten_cycle?-1:1
                           , _exec_plan._ptr_subplaninfo[_exec_phase_current_subphase]._green
                           , _exec_plan._ptr_subplaninfo[_exec_phase_current_subphase]._green_compensation );

                    _itimer_plan.it_value.tv_sec = _exec_plan._ptr_subplaninfo[_exec_phase_current_subphase].compensated_green(_exec_plan._shorten_cycle);
                    //jacky20141203
//            if(smem.GetPriorityswitch() || smem.GetGreenManonoff5014()==1){
//                if(smem.GetLearningPGPRAcrossCycle()){  //shotdown Learning PGPR Count
//                    if(smem.GetLearningPGPRsubphase())
//                        _itimer_plan.it_value.tv_sec+=2;
//                    else
//                        _itimer_plan.it_value.tv_sec-=2;
//                } else {
//                    if(smem.GetLearningPGPRsubphase())
//                    _itimer_plan.it_value.tv_sec-=2;
//                else
//                    _itimer_plan.it_value.tv_sec+=2;
//                }
//            }
                    break;

                case(1):
                    printf("  %d green_PG=%3d compensation_time=%3d \n", _exec_plan._shorten_cycle?-1:1
                           , _exec_plan._ptr_subplaninfo[_exec_phase_current_subphase]._pedgreen_flash
                           , _exec_plan._ptr_subplaninfo[_exec_phase_current_subphase]._pedgreen_flash_compensation );

                    _itimer_plan.it_value.tv_sec =
                        _exec_plan._ptr_subplaninfo[_exec_phase_current_subphase].compensated_pedgreen_flash(_exec_plan._shorten_cycle);
                    break;

                case(2):
                    printf("  %d green_PR=%3d compensation_time=%3d \n", _exec_plan._shorten_cycle?-1:1
                           , _exec_plan._ptr_subplaninfo[_exec_phase_current_subphase]._pedred
                           , _exec_plan._ptr_subplaninfo[_exec_phase_current_subphase]._pedred_compensation );

                    _itimer_plan.it_value.tv_sec = _exec_plan._ptr_subplaninfo[_exec_phase_current_subphase].compensated_pedred(_exec_plan._shorten_cycle);
                    break;

                case(3):
                    printf("  %d green_yellow=%3d compensation_time=%3d \n", _exec_plan._shorten_cycle?-1:1
                           , _exec_plan._ptr_subplaninfo[_exec_phase_current_subphase]._yellow
                           , _exec_plan._ptr_subplaninfo[_exec_phase_current_subphase]._yellow_compensation );

                    _itimer_plan.it_value.tv_sec = _exec_plan._ptr_subplaninfo[_exec_phase_current_subphase].compensated_yellow(_exec_plan._shorten_cycle);
                    break;

                case(4):
                    printf("  %d green_allred=%3d compensation_time=%3d \n", _exec_plan._shorten_cycle?-1:1
                           , _exec_plan._ptr_subplaninfo[_exec_phase_current_subphase]._allred
                           , _exec_plan._ptr_subplaninfo[_exec_phase_current_subphase]._allred_compensation );

                    _itimer_plan.it_value.tv_sec = _exec_plan._ptr_subplaninfo[_exec_phase_current_subphase].compensated_allred(_exec_plan._shorten_cycle);
                    break;
                }

                pthread_mutex_unlock(&CPlanInfo::_plan_mutex);
            }

            if(_current_strategy == STRATEGY_TOD && _exec_segment._ptr_seg_exec_time[_exec_segment_current_seg_no]._cadc_seg == 0xDDDD)   //test
            {
                switch(_exec_phase_current_subphase_step)
                {
                case(0):
                    if(_exec_plan._ptr_subplaninfo[_exec_phase_current_subphase]._min_green > 0)
                    {
                        _itimer_plan.it_value.tv_sec = _exec_plan._ptr_subplaninfo[_exec_phase_current_subphase]._min_green;
                    }
                    printf(" %d green_min_time=%3d\n", _exec_plan._ptr_subplaninfo[_exec_phase_current_subphase]._min_green);

                    break;
                }
            }

            //OT20140214
            if(_exec_phase_current_subphase_step != 0)
            {
                smem.vSetCurrentSubphaseRunning5F1C(_exec_phase_current_subphase, false);
            }
            if(_exec_phase_current_subphase_step == 0)
            {
                if(smem.vGet5F1CEffectTimeStatus())    //have got 5F1C
                {
                    usiTmp = smem.vGet5F1CEffectTime(_exec_phase_current_subphase);
                    if(usiTmp > 0)
                    {
                        // _itimer_plan.it_value.tv_sec = usiTmp; //should check max/min green
                        smem.vSet5F1CEffectTime(_exec_phase_current_subphase, 0);
                        smem.vSetCurrentSubphaseRunning5F1C(_exec_phase_current_subphase, true);
                    }
                    else
                    {
                        _itimer_plan.it_value.tv_sec = _exec_plan._ptr_subplaninfo[_exec_phase_current_subphase]._min_green;
                    }
                }
            }



            _itimer_plan.it_value.tv_nsec = 0;                      //reset step time
            _itimer_plan.it_interval.tv_sec = 0;
            _itimer_plan.it_interval.tv_nsec = 0;

            _itimer_plan_WDT.it_value.tv_nsec = 0;
            _itimer_plan_WDT.it_interval.tv_sec = 0;
            _itimer_plan_WDT.it_interval.tv_nsec = 0;


            if(RESET_TIMER_BEFORE_SET_LIGHT)
            {
                for(int ii = 0; ii < 4; ii++)
                {

                    if ( timer_settime( _timer_plan, 0, & _itimer_plan, NULL ) )
                    {
                        printf("Settime Error!.\n");
                        exit( 1 );
                    }

                    _itimer_plan_WDT = _itimer_plan; /*OT_PLAN_WDT*/
                    _itimer_plan_WDT.it_value.tv_sec += 2;
                    if(timer_settime( _timer_plan_WDT, 0, &_itimer_plan_WDT, NULL ))
                    {
                        printf("PlanWDT Settime Error!.\n");
                        exit( 1 );
                    }
                    timer_settime( _timer_redcount, 0, & _itimer_redcount, NULL );

                }

            }

            printf("ReSetStep: phase:(%d/%d) step:(%d/%d)"
                   , _exec_phase_current_subphase+1,      _exec_phase._subphase_count
                   , _exec_phase_current_subphase_step+1, _exec_phase._ptr_subphase_step_count[_exec_phase_current_subphase]);

            if(_exec_plan._shorten_cycle)
            {

                printf("            shorten:(---)  %3d(s)  (---)\n", _itimer_plan.it_value.tv_sec);

            }
            else
            {

                printf("            extend: (+++)  %3d(s)  (+++)\n", _itimer_plan.it_value.tv_sec);

            }

            /*For CCJ TOM*/
//    if(_current_strategy==STRATEGY_TOD && smem.vGetBOOLData(TC_CCT_In_LongTanu_ActuateType_FunctionEnable) == true) {
            if(_current_strategy==STRATEGY_TOD)
            {
//        printf("OT c1\n");
                if(_exec_phase_current_subphase_step == 0 && _exec_phase_current_subphase == 0)
                {
//          printf("OT c2\n");
                    vCCJTOMSendPhaseInfo();
                    vCCJTOMSendTODNextPlanID();
                }
//OT990329        if(smem.vGetActuatePhaseExtend() ==  (_exec_phase_current_subphase+1) && smem.vGetBOOLData(TC_CCT_In_LongTanu_ActuateType_FunctionEnable) == true ) {
//OT990401 using min/max green       if(smem.vGetVDPhaseTriggerSwitch(_exec_phase_current_subphase) > 0 && smem.vGetBOOLData(TC_CCT_In_LongTanu_ActuateType_FunctionEnable) == true ) {
                if(smem.vGetBOOLData(TC_CCT_In_LongTanu_ActuateType_FunctionEnable) == true )  //判斷觸動是否要延長 2013/01/28 chou check
                {
//   printf("OT c3\n");
                    if(_exec_phase_current_subphase_step == 0)
                    {
//            printf("OT c4\n");
                        if(smem.vGetVDPhaseTriggerSwitch(_exec_phase_current_subphase)>0) //arwen ++ 1001006
                        {
                            vCCJTOMSendStartStopInfo(1);    //2013/01/28 chou check
                        }
                        else
                        {
                            vCCJTOMSendStartStopInfo(0);
                        }
                        vCCJTOMSendActuatePhaseInfo_5FA0();
                        smem.vCleanActCountEnd();
                    }
                    else if(_exec_phase_current_subphase_step == 3)
                    {
                        vCCJTOMSendStartStopInfo(0);
                    }
                }
            }  //EndCCJ

        }//end elseif(CADC, AUTO_CADC, TOD)

        if(_exec_phase_current_subphase_step == 0)
        {
            smem.vSaveCurrentAsGreenStartTime();
        }

        //OT20140304
        bzero(cTMP, 128);
        sprintf(cTMP, "sp:%d, st:%d, t:%d", _exec_phase_current_subphase+1, _exec_phase_current_subphase_step+1, _itimer_plan.it_value.tv_sec );
        smem.vWriteMsgToTmp(cTMP);

    }
    catch (...) {}
}
//----------------------------------------------------------
void CSTC::SetLightAfterExtendTimerReSet(void)          //2013/06/17 chou check 點燈
{
    try
    {
        /*OTCombo0713*/
        unsigned char signal_bit_map[14];
        unsigned char Green_map;
        unsigned char StraightRightPed_map;
        unsigned char ucSendTMP[21];
        unsigned char ucSendTMP_BRT[20]; //jacky20140107
        unsigned char ucSendTMP_BRTLightcar[21]; //jacky20140123

        DATA_Bit DGreenConflict, DGreenConflict2;

        for(int i=0, j=0; i<14; i++, j++)
        {
            if(j<_exec_phase._signal_count)
            {
                if(smem.GetLearningPGPRThreeSecondoff()==false || _current_strategy == STRATEGY_FLASH || _current_strategy == STRATEGY_ALLRED)
                {
                    signal_bit_map[i] =
                        (_exec_phase._ptr_subphase_step_signal_status[_exec_phase_current_subphase][_exec_phase_current_subphase_step][j] ) &0xFF;
                    signal_bit_map[++i] =
                        (_exec_phase._ptr_subphase_step_signal_status[_exec_phase_current_subphase][_exec_phase_current_subphase_step][j] >>8) &0xFF;
                }
                else
                {
                    if(smem.GetCarLearnSwitch() == 1 && smem.GetCarCountdownProperty() == 2)  //行車倒數關閉 && 行車為學習式
                    {
                        Green_map =
                            (_exec_phase._ptr_subphase_step_signal_status[_exec_phase_current_subphase][_exec_phase_current_subphase_step][j] ) & 0x3F; //圓 不亮
                        StraightRightPed_map =
                            (_exec_phase._ptr_subphase_step_signal_status[_exec_phase_current_subphase][_exec_phase_current_subphase_step][j] >>8) & 0xF0; //直 右 不亮
                    }
                    else
                    {
                        Green_map =
                            (_exec_phase._ptr_subphase_step_signal_status[_exec_phase_current_subphase][_exec_phase_current_subphase_step][j] ) & 0xFF;
                        StraightRightPed_map =
                            (_exec_phase._ptr_subphase_step_signal_status[_exec_phase_current_subphase][_exec_phase_current_subphase_step][j] >>8) & 0xFF;
                    }
                    if(smem.GetPedLearnSwitch() == 1 && smem.GetPedCountdownProperty() == 2)  //行人倒數關閉 && 行人為學習式
                    {
                        StraightRightPed_map = StraightRightPed_map | 0xF0; //PGPR 全亮
                    }
                    else
                    {
                        StraightRightPed_map =
                            StraightRightPed_map & 0xFF;
                    }
                    signal_bit_map[i] =
                        (_exec_phase._ptr_subphase_step_signal_status[_exec_phase_current_subphase][_exec_phase_current_subphase_step][j] ) & Green_map;
                    signal_bit_map[++i] = StraightRightPed_map;
                    smem.SetLearningPGPROneSecondon(true);
                }
            }
            else
            {
                signal_bit_map[i] = 0;
                signal_bit_map[++i] = 0;
            }
            printf("signal_bit_map[%d] = %x\n",i,signal_bit_map[i]);
        }

        for(int i = 0; i < 12; i++)
        {
            smem.vSetUCData(200+i, signal_bit_map[i]);                                  //Save Signal Light Data
        }
        for(int i = 12; i < 14; i++)                                                  //Get ReverseTime Data
        {
            signal_bit_map[i] = smem.vGetUCData(200+i);
        }

        ucSendTMP[0] = 0xAA;
        ucSendTMP[1] = 0xBB;
        ucSendTMP[2] = 0x22;

        ucSendTMP[3] = _exec_phase._phase_order;
        ucSendTMP[4] = 0;
        ucSendTMP[5] = 0;
        ucSendTMP[6] = 0;
        ucSendTMP[7] = 0;
        ucSendTMP[8] = 0;
        ucSendTMP[9] = 0;
        ucSendTMP[10] = 0;
        ucSendTMP[11] = 0;
        ucSendTMP[12] = 0;
        ucSendTMP[13] = 0;
        ucSendTMP[14] = 0;
        ucSendTMP[15] = 0;
        ucSendTMP[16] = 0;
        ucSendTMP[17] = 0x00;
        ucSendTMP[17] = (_exec_phase_current_subphase)*5 + _exec_phase_current_subphase_step;
        ucSendTMP[18] = 0xAA;
        ucSendTMP[19] = 0xCC;
        for (int i=0; i<20; i++)
        {
            ucSendTMP[20]^=ucSendTMP[i];
        }

        if(bRefreshLight == true && _exec_phase_current_subphase == 0 && _exec_phase_current_subphase_step == 0)
        {
            /*+++++++++++++++++*/
            //990401 BUG FIX
            if(smem.vGetStopSend0x22() == false)
            {
                writeJob.WritePhysicalOut(ucSendTMP, 21, DEVICETRAFFICLIGHT);             //0x22
            }
            else
            {
                printf("find bug, stop send 0x22 to light board");
            }
            /*-----------------*/
        }

        ucSendTMP[2] = 0x11;
        ucSendTMP[3] = signal_bit_map[1];
        ucSendTMP[4] = signal_bit_map[0];
        ucSendTMP[5] = signal_bit_map[3];
        ucSendTMP[6] = signal_bit_map[2];
        ucSendTMP[7] = signal_bit_map[5];
        ucSendTMP[8] = signal_bit_map[4];
        ucSendTMP[9] = signal_bit_map[7];
        ucSendTMP[10] = signal_bit_map[6];
        ucSendTMP[11] = signal_bit_map[9];
        ucSendTMP[12] = signal_bit_map[8];
        ucSendTMP[13] = signal_bit_map[11];
        ucSendTMP[14] = signal_bit_map[10];
        ucSendTMP[15] = signal_bit_map[13];
        ucSendTMP[16] = signal_bit_map[12];
        for (int i=0; i<20; i++)
            ucSendTMP[20]^=ucSendTMP[i];


        //jacky20140107 BRT set LED on board
        ucSendTMP_BRT[0] = 0xAA;
        ucSendTMP_BRT[1] = 0xBB;
        ucSendTMP_BRT[2] = 0x22;
        if( smem.vGetUCData(BRT_Light_Display_Enable) == 1)   //Boot completed BRT board lights
        {
            ucSendTMP_BRT[3] = signal_bit_map[0];
            ucSendTMP_BRT[4] = signal_bit_map[1];
            ucSendTMP_BRT[5] = signal_bit_map[2];
            ucSendTMP_BRT[6] = signal_bit_map[3];
            ucSendTMP_BRT[7] = signal_bit_map[4];
            ucSendTMP_BRT[8] = signal_bit_map[5];
            ucSendTMP_BRT[9] = signal_bit_map[6];
            ucSendTMP_BRT[10] = signal_bit_map[7];
            ucSendTMP_BRT[11] = signal_bit_map[8];
            ucSendTMP_BRT[12] = signal_bit_map[9];
            ucSendTMP_BRT[13] = signal_bit_map[10];
            ucSendTMP_BRT[14] = signal_bit_map[11];
            if (smem.bConnectWithCenterStatus == false)    //中心通訊 斷訊才亮燈
            {
                ucSendTMP_BRT[15] = 0x0C;
            }
            else ucSendTMP_BRT[15] = 0x00;

            if(smem.GPSStatus == true)     //GPS正常才亮燈
            {
                ucSendTMP_BRT[16] = 0x03;
            }
            else ucSendTMP_BRT[16] = 0x00;

            if(smem.GetPriorityswitch())
            {
                ucSendTMP_BRT[16] |= 0xC0;
            }
            else ucSendTMP_BRT[16] |= 0x00;

        }
        else     //BRT_Light_Display_Enable = 0  Boot unfinished
        {
            for (int i = 3; i<15; i++)
            {
                ucSendTMP_BRT[i] = 0x00;
            }
            ucSendTMP_BRT[15] = 0x03;
            ucSendTMP_BRT[16] = 0x00;
        }
        ucSendTMP_BRT[17] = 0xAA;
        ucSendTMP_BRT[18] = 0xCC;
        ucSendTMP_BRT[19] = 0x00;
        for (int i=0; i<19; i++)
            ucSendTMP_BRT[19]^=ucSendTMP_BRT[i];

        smem.BRTOnBoardLED(ucSendTMP_BRT);
        //jacky20140107

        /*otaru 0614++*/
        if(bRefreshLight == true)
        {
            /*+++++++++++++++++*/
            //990401 BUG FIX
            //OT20110706
//    if(smem.vGetStopSend0x22() == false) {
//      ReSetStep(true);

//        writeJob.WritePhysicalOut(ucSendTMP, 21, DEVICETRAFFICLIGHT);             //0x11
//        writeJob.WritePhysicalOut(ucSendTMP, 21, DEVICETRAFFICLIGHT);             //0x11
            writeJob.WritePhysicalOut(ucSendTMP, 21, DEVICETRAFFICLIGHT);             //0x11
            writeJob.WritePhysicalOut(ucSendTMP_BRT, 20, DEVICEKEYPAD);     // BRT Light
            writeJob.WritePhysicalOut(ucSendTMP_BRT, 20, DEVICEKEYPAD);
            writeJob.WritePhysicalOut(ucSendTMP_BRT, 20, DEVICEKEYPAD);
//    } else {
//      printf("find bug, stop send 0x11 to light board");
//    }

            /*-----------------*/
        }
        else
        {
            printf("[Warning] Not Refresh Light! Not Refresh Light! Not Refresh Light!\n");
        }
        bRefreshLight = true;

        //jacky20140123
        ucSendTMP_BRTLightcar[0] = 0xAA;
        ucSendTMP_BRTLightcar[1] = 0xBB;
        ucSendTMP_BRTLightcar[2] = 0x31;
        ucSendTMP_BRTLightcar[3] = 0x01;
        ucSendTMP_BRTLightcar[4] = 0x01;
        ucSendTMP_BRTLightcar[5] = 0x01;
        ucSendTMP_BRTLightcar[6] = 0x01;
        for(int i = 7; i<18; i++)
        {
            ucSendTMP_BRTLightcar[i] = 0;
        }
        ucSendTMP_BRTLightcar[18] = 0xAA;
        ucSendTMP_BRTLightcar[19] = 0xCC;
        ucSendTMP_BRTLightcar[20] = 0x00;
        for (int i=0; i<20; i++)
        {
            ucSendTMP_BRTLightcar[20]^=ucSendTMP_BRTLightcar[i];
        }
//  writeJob.WritePhysicalOut(ucSendTMP_BRTLightcar, 21, DEVICETRAFFICLIGHT);

        printf("SetLightAfterExtendTimerReSet:  ");

        for(int i=0; i<14; i++)
        {

            printf(" %x", signal_bit_map[i]);

        }

        printf("\n");


        if( _current_strategy==STRATEGY_CADC || _current_strategy==STRATEGY_AUTO_CADC || _current_strategy==STRATEGY_TOD )  //jacky20140109 "{"
        {
            if(!RESET_TIMER_BEFORE_SET_LIGHT)
            {
                for(int ii = 0; ii < 4; ii++)
                {
                    if(timer_settime(_timer_plan, 0, & _itimer_plan, NULL)) exit( 1 );

                    _itimer_plan_WDT = _itimer_plan; /*OT_PLAN_WDT*/
                    _itimer_plan_WDT.it_value.tv_sec += 2;
                    if(timer_settime( _timer_plan_WDT, 0, & _itimer_plan_WDT, NULL ))
                    {
                        printf("PlanWDT Settime Error!.\n");
                        exit( 1 );
                    }
                    timer_settime( _timer_redcount, 0, & _itimer_redcount, NULL );
                }
            }
        }  //jacky20140109 "}"

        if(smem.vGetINTData(TC92SignalStepStatus_5F03_IntervalTime)  == 0)
        {
            ReportCurrentStepStatus();  //By 5F3F, if transmitCycle == 0, send  change, if transmitCycle == 0xFF,  don't send.
            vReport5F80CCTProtocalSendStep();

            ReportCurrentOperationMode_5F08();  //add for cct.
            smem.vSetTC5F08StatusChange(true);  //interval data, force send.

        }
        if(smem.vGetINTData(TC92SignalLightStatus_5F0F_IntervalTime) == 0)
        {
            ReportCurrentSignalMap_5F0F();
        }
//  if( _current_strategy == STRATEGY_ALLDYNAMIC) stc.vReport5F1CWorkingStepChange();

        if( cCURRENTLIGHTSTATUS == smem.GetcFace() )
        {
            screenCurrentLightStatus.vRefreshCurrentLightStatusData();
        }

        //jacky20140324
        if(cBRTPRIORITY == smem.GetcFace())
            screenPriority.vRefreshCurrentStep();
        //jacky20140521
        if(cBRTExtendsec == smem.GetcFace())
            screenPriorityExtendsec.PlanPhase();

        //Debug 951128 for dongSignal driver unit

        ucSendTMP[2] = 0x12;
//121212
        ucSendTMP[17] = 0x00;
//  ucSendTMP[17] = 0xFA;
        ucSendTMP[20] = 0x00;

        for (int i=0; i<20; i++)
        {
            ucSendTMP[20]^=ucSendTMP[i];
        }
//FOROTNEWBOARD  usleep(500);

        smem.vTrafficLightSendAndCheck();                                         //For record lightboard status.

        //990401 BUG FIX
        //OT20110706
//  if(smem.vGetStopSend0x22() == false) {

        writeJob.WritePhysicalOut(ucSendTMP, 21, DEVICETRAFFICLIGHT);             //0x12

//  } else {
//    printf("find bug, stop send 0x21 to light board");
//  }



//OT20110825, will change very fast, so disable
//  smem.vSetBOOLData(TC_SIGNAL_DRIVER_UNIT, false);

//OT970214NEWLCX405
        /*
          ucSendTMP[2] = 0x13;
          ucSendTMP[20] = 0x00;
          for (int i=0; i<20; i++) {
            ucSendTMP[20]^=ucSendTMP[i];
          }
        //FOROTNEWBOARD  usleep(500);
          writeJob.WritePhysicalOut(ucSendTMP, 21, DEVICETRAFFICLIGHT);
        */

        DGreenConflict2.DBit = 0;
        for(int i = 0; i < 6; i++)
        {

//here is cycle green
            DGreenConflict.DBit = ucSendTMP[i*2+4];
            if(DGreenConflict.switchBit.b7 == 1 && DGreenConflict.switchBit.b8 == 1)    // signal light
            {
                switch(i)
                {
                case(0):
                    DGreenConflict2.switchBit.b1 = 1;
                    break;
                case(1):
                    DGreenConflict2.switchBit.b2 = 1;
                    break;
                case(2):
                    DGreenConflict2.switchBit.b3 = 1;
                    break;
                case(3):
                    DGreenConflict2.switchBit.b4 = 1;
                    break;
                case(4):
                    DGreenConflict2.switchBit.b5 = 1;
                    break;
                case(5):
                    DGreenConflict2.switchBit.b6 = 1;
                    break;
                }
            }
//here is up arrow
            DGreenConflict.DBit = ucSendTMP[i*2+3];
            if(DGreenConflict.switchBit.b1 == 1 && DGreenConflict.switchBit.b2 == 1)    // signal light
            {
                switch(i)
                {
                case(0):
                    DGreenConflict2.switchBit.b1 = 1;
                    break;
                case(1):
                    DGreenConflict2.switchBit.b2 = 1;
                    break;
                case(2):
                    DGreenConflict2.switchBit.b3 = 1;
                    break;
                case(3):
                    DGreenConflict2.switchBit.b4 = 1;
                    break;
                case(4):
                    DGreenConflict2.switchBit.b5 = 1;
                    break;
                case(5):
                    DGreenConflict2.switchBit.b6 = 1;
                    break;
                }
            }

        }
        DGreenConflict2.switchBit.b7 = 1;                                             // because control return 1
        DGreenConflict2.switchBit.b8 = 1;
        smem.vSetUCData(TC_GreenConflictDetFromCSTC, DGreenConflict2.DBit);


        //Debug what!?
        /*
          unsigned short int time_difference=0;
          timer_gettime(_timer_reversetime,&_itimer_reversetime);
          time_difference = (_itimer_reversetime.it_value.tv_sec) + 1;

          printf("Reversetime: %d.\n", time_difference);
          printf("_exec_reversetime_current_rev_step: %d.\n", _exec_reversetime_current_rev_step);
        */


//OT Force add.
        if( smem.vGetBOOLData(IFGetResetTCSignal) == true &&
                _exec_phase_current_subphase ==  (_exec_phase._subphase_count - 1) &&
                _exec_phase_current_subphase_step == 0)
        {

//let redcount close
            CalculateAndSendRedCount(10);

            /*
                 MESSAGEOK _MSG;
                 unsigned char data3[4];
                 data3[0]  = 0x0F;
                 data3[1]  = 0x90;
                 data3[2]  = 0x52;
                 data3[3]  = 0x52;
                 _MSG = oDataToMessageOK.vPackageINFOTo92Protocol(data3, 4, true);
                 _MSG.InnerOrOutWard = cOutWard;
                 writeJob.WritePhysicalOut(_MSG.packet, _MSG.packetLength, DEVICECENTER92);
            */

            screenHWReset.DisplayHWReset();
            system("sync");
            stc.CalculateAndSendRedCount(10);
            writeJob.WriteLetW77E58AutoControl();                                      //let W77e58 autoControl.
            system("sync");
            system("sync");
            system("reboot");
        }

        if(_exec_phase_current_subphase ==  0 &&  _exec_phase_current_subphase_step == 0 )    //new pahse,step.
        {
            smem.vSetBOOLData(EnableUpdateRTC, true);
        }
    }
    catch (...) {}
}

//--------------------------------------------------------------------
bool CSTC::vDetermine_ReverseTime(void)     //調撥
{
    try
    {
        int iExecSeg;
        printf("Finding ReverserTime\n");

        static time_t now;
        static struct tm* currenttime;

        unsigned short int usiReverseLight = 0x0;
        unsigned short int usiTmp = 0x0;
        unsigned char ucSendTMP[21];
        unsigned char signal_bit_map[14];
        unsigned char ucTmp;
        unsigned char ucTmpArray[2];

        now = time(NULL);
        currenttime = localtime(&now);


        printf("Date: %4d/%2d/%2d  Time: %2d:%2d:%2d\n"
               , currenttime->tm_year+1900,currenttime->tm_mon+1,currenttime->tm_mday
               , currenttime->tm_hour, currenttime->tm_min, currenttime->tm_sec);


        //***********************************
        //****   check if in holidayseg
        bool _exec_reversetime_is_holiday=false;
        for(int i=0; i<AMOUNT_HOLIDAY_REV; i++)
        {
#ifdef ShowInfoToSTD
            printf("holidayrev[%d]._start_year: %d\n", i, holidayrev[i]._start_year);
            printf("holidayrev[%d]._start_month: %d\n", i, holidayrev[i]._start_month);
            printf("holidayrev[%d]._start_day: %d\n", i, holidayrev[i]._start_day);
            printf("holidayrev[%d]._end_year: %d\n", i, holidayrev[i]._end_year);
            printf("holidayrev[%d]._end_month: %d\n", i, holidayrev[i]._end_month);
            printf("holidayrev[%d]._end_day: %d\n", i, holidayrev[i]._end_day);
#endif

            if(    (    ( (currenttime->tm_year+1900) > holidayrev[i]._start_year )
                        || ( (currenttime->tm_year+1900)== holidayrev[i]._start_year && (currenttime->tm_mon+1) > holidayrev[i]._start_month )
                        || ( (currenttime->tm_year+1900)== holidayrev[i]._start_year && (currenttime->tm_mon+1)== holidayrev[i]._start_month
                             && (currenttime->tm_mday)>= holidayrev[i]._start_day ) )
                    && (    ( (currenttime->tm_year+1900) < holidayrev[i]._end_year )
                            || ( (currenttime->tm_year+1900)== holidayrev[i]._end_year   && (currenttime->tm_mon+1) < holidayrev[i]._end_month )
                            || ( (currenttime->tm_year+1900)== holidayrev[i]._end_year   && (currenttime->tm_mon+1)== holidayrev[i]._end_month   && (currenttime->tm_mday)<= holidayrev[i]._end_day ) ) )
            {
                printf(" go holiday!\n");
                if(_exec_rev._reverse_time_type != reversetime[holidayrev[i]._reverse_time_type]._reverse_time_type
                        || ReverseTimeDataUpdate == true
                  )
                {
                    /******** lock mutex ********/
                    pthread_mutex_lock(&CReverseTimeInfo::_rev_mutex);
//          if(reversetime[holidayrev[i]._reverse_time_type]._ptr_seg_exec_time)
                    _exec_rev = reversetime[holidayrev[i]._reverse_time_type];
//          else _exec_segment = segment[DEFAULT_SEGMENTTYPE];
                    /******** unlock mutex ********/
                    pthread_mutex_unlock(&CReverseTimeInfo::_rev_mutex);
                    if(ReverseTimeDataUpdate == true) ReverseTimeDataUpdate = false;
                }
                _exec_reversetime_is_holiday=true;

                printf("Update Today is in Holiday Rev:%d\n",i);

                break;
            }
        }

        //***********************************
        //****  check in which weekdayseg
        if(_exec_reversetime_is_holiday == false)
        {
            printf("[OTMsg] Now checking Normal Day!\n");
            if(    ((((((currenttime->tm_mday-1)/7)+1)%2)==0 ) && (currenttime->tm_wday>=((currenttime->tm_mday-1)%7)))
                    || ((((((currenttime->tm_mday-1)/7)+1)%2)> 0 ) && (currenttime->tm_wday< ((currenttime->tm_mday-1)%7))) )
            {

                int wday = (currenttime->tm_wday==0)?13:(currenttime->tm_wday+6);
                if(_exec_rev._reverse_time_type != weekdayrev[wday]._reverse_time_type
                        || ReverseTimeDataUpdate == true
                  )
                {
                    /******** lock mutex ********/
                    pthread_mutex_lock(&CReverseTimeInfo::_rev_mutex);
//          if(reversetime[weekdayrev[wday]._reverse_time_type]._ptr_seg_exec_time)
                    _exec_rev = reversetime[weekdayrev[wday]._reverse_time_type]; //tm_wday:{1-6} => P92:{11-16} => weekdayseg:{7-12}
//          else _exec_segment = segment[DEFAULT_SEGMENTTYPE];
                    /******** unlock mutex ********/
                    pthread_mutex_unlock(&CReverseTimeInfo::_rev_mutex);
                    if(ReverseTimeDataUpdate == true) ReverseTimeDataUpdate = false;
                    printf("Update This week is an even week Rev.%d\n", _exec_rev._reverse_time_type);
                }
            }//end if(even week)

            else  //odd week
            {

                int wday = (currenttime->tm_wday==0)?6:(currenttime->tm_wday-1);
                if(_exec_rev._reverse_time_type != weekdayrev[wday]._reverse_time_type
                        || ReverseTimeDataUpdate == true
                  )
                {
                    /******** lock mutex ********/
                    pthread_mutex_lock(&CReverseTimeInfo::_rev_mutex);
//          if(reversetime[weekdayrev[wday]._reverse_time_type]._ptr_seg_exec_time)
                    _exec_rev = reversetime[weekdayrev[wday]._reverse_time_type]; //tm_wday:{1-6} => P92:{1-6} => weekdayseg:{0-5}
//          else _exec_segment = segment[DEFAULT_SEGMENTTYPE];
                    /******** unlock mutex ********/
                    pthread_mutex_unlock(&CReverseTimeInfo::_rev_mutex);
                    if(ReverseTimeDataUpdate == true) ReverseTimeDataUpdate = false;
                    printf("Update This week is an odd week Rev.%d\n", _exec_rev._reverse_time_type);
                }
            }//end else(odd week)
        }//end else(holiday)

//---------------------------------------------------------------------------------

        int iCurrentSec, iRevInCitySec, iRevOutCitySec;
        int iRevInCityEndSec, iRevOutCityEndSec;
        int iTmpSec = 100000;
        iCurrentSec = currenttime->tm_hour * 3600 + currenttime->tm_min * 60 + currenttime->tm_sec;
        iRevInCitySec = _exec_rev.usiHourStartIn * 3600 + _exec_rev.usiMinStartIn * 60;
        iRevInCityEndSec = _exec_rev.usiHourEndIn * 3600 + _exec_rev.usiMinEndIn * 60;
        iRevOutCitySec = _exec_rev.usiHourStartOut * 3600 + _exec_rev.usiMinStartOut * 60;
        iRevOutCityEndSec = _exec_rev.usiHourEndOut * 3600 + _exec_rev.usiMinEndOut * 60;

        //OT990628
        /*
        if(iRevInCitySec == iRevInCityEndSec && iRevInCityEndSec == 0) {  //not running.
          iRevInCitySec = 99999;
          iRevInCityEndSec = 99999;
        }
        if(iRevOutCitySec == iRevOutCityEndSec && iRevOutCityEndSec == 0) {
          iRevOutCitySec = 99999;
          iRevOutCityEndSec = 99999;
        }
        */

        printf("iCurrentSec:%d iRevInCitySec:%d iRevInCityEndSec:%d\niRevOutCitySec:%d iRevOutCityEndSec:%d _exec_reversetime_current_rev_step:%d\n",
               iCurrentSec, iRevInCitySec, iRevInCityEndSec, iRevOutCitySec, iRevOutCityEndSec, _exec_reversetime_current_rev_step);

//OT990621
        if( smem.vGetUCData(revSyncEnable) == true && smem.vGetRevSyncVar() == -1)
        {

            printf("[OTMsg] Rev Sync Error Happened\n");
            smem.vWriteMsgToDOM("[!!!] Rev Sync Error Happened");

            // when error Happened
            usiReverseLight = 0x0033; //All X
            signal_bit_map[12] = usiReverseLight & 0xFF;
            signal_bit_map[13] = ( usiReverseLight >> 8) & 0xFF;
            iTmpSec = 10;
            _exec_reversetime_current_rev_step = 0;

            /*
                _itimer_reversetime.it_value.tv_sec = iTmpSec;
                _itimer_reversetime.it_value.tv_nsec = 0;
                _itimer_reversetime.it_interval.tv_sec = 0;
                _itimer_reversetime.it_interval.tv_nsec = 0;
                timer_settime( _timer_reversetime, 0, & _itimer_reversetime, NULL );
                timer_settime( _timer_reversetime, 0, & _itimer_reversetime, NULL );
                timer_settime( _timer_reversetime, 0, & _itimer_reversetime, NULL );
                timer_settime( _timer_reversetime, 0, & _itimer_reversetime, NULL );
            */


//    stc.vReportRevStatus5F02(0);  //send msg to center
        }
        else if (_exec_reversetime_current_rev_step == 0)
        {
            usiReverseLight = vStartReverseLaneInStep0(now, &iTmpSec);
            //----------------------------------------------------------
        }
        else                                                                          // _exec_reversetime_current_rev_step not 0
        {
            usiReverseLight = vStartReverseLane(now, &iTmpSec);
//    signal_bit_map[12] = usiReverseLight & 0xFF;
//    signal_bit_map[13] = ( usiReverseLight >> 8) & 0xFF;
        }

//-------------Error Prototec
        if(_exec_reversetime_current_rev_step == 3 ||
                _exec_reversetime_current_rev_step == 13  )
        {
            usiReverseLight = 0x0033; //All X
        }

        ucTmpArray[0] = smem.vGetUCData(200+12);
        ucTmpArray[1] = smem.vGetUCData(200+13);
        usiTmp = 0;
        usiTmp  = ucTmpArray[1];
        usiTmp = usiTmp << 8;
        usiTmp += ucTmpArray[0];

        if(usiTmp == 0x003C && usiReverseLight == 0x00C3)
        {
            _exec_reversetime_current_rev_step = 0;
            iTmpSec = _exec_rev.usiClearTime;
            usiReverseLight = 0x0033;
            smem.vWriteMsgToDOM("[!!!] inAr->OutAr 0x003C-> 0x00C3, Dangerous");

        }
        else if(usiTmp == 0x0038 && usiReverseLight == 0x00C3)
        {
            _exec_reversetime_current_rev_step = 0;
            iTmpSec = _exec_rev.usiClearTime;
            usiReverseLight = 0x0033;
            smem.vWriteMsgToDOM("[!!!] inArF->OutAr 0x0038->0x00C3, Dangerous");

        }
        else if(usiTmp == 0x00C3 && usiReverseLight == 0x003C)
        {
            _exec_reversetime_current_rev_step = 0;
            iTmpSec = _exec_rev.usiClearTime;
            usiReverseLight = 0x0033;
            smem.vWriteMsgToDOM("[!!!] OutAr->InAr 0x00C3->0x003C, Dangerous");

        }
        else if(usiTmp == 0x0083 && usiReverseLight == 0x003C)
        {
            _exec_reversetime_current_rev_step = 0;
            iTmpSec = _exec_rev.usiClearTime;
            usiReverseLight = 0x0033;
            smem.vWriteMsgToDOM("[!!!] OutArF->InAr 0x0083->0x003C, Dangerous");

        }
        else if(usiTmp == 0x00C3 && usiReverseLight == 0x0038)
        {
            _exec_reversetime_current_rev_step = 0;
            iTmpSec = _exec_rev.usiClearTime;
            usiReverseLight = 0x0033;
            smem.vWriteMsgToDOM("[!!!] OutAr->InArF 0x00C3->0x0038, Dangerous");

        }
        else if(usiTmp == 0x0083 && usiReverseLight == 0x0038)
        {
            _exec_reversetime_current_rev_step = 0;
            iTmpSec = _exec_rev.usiClearTime;
            usiReverseLight = 0x0033;
            smem.vWriteMsgToDOM("[!!!] OutArF->InArF 0x0083->0x0038, Dangerous");

        }
        else if(usiTmp == 0x003C && usiReverseLight == 0x0083)
        {
            _exec_reversetime_current_rev_step = 0;
            iTmpSec = _exec_rev.usiClearTime;
            usiReverseLight = 0x0033;
            smem.vWriteMsgToDOM("[!!!] inAr->OutArF 0x003C->0x0083, Dangerous");

        }
        else if(usiTmp == 0x0038 && usiReverseLight == 0x0083)
        {
            _exec_reversetime_current_rev_step = 0;
            iTmpSec = _exec_rev.usiClearTime;
            usiReverseLight = 0x0033;
            smem.vWriteMsgToDOM("[!!!] inArF->OutArF 0x0038->0x0083, Dangerous");
        }


        vSetReverseTimer(iTmpSec);
//---------------------------------------------

        signal_bit_map[12] = usiReverseLight & 0xFF;
        signal_bit_map[13] = ( usiReverseLight >> 8) & 0xFF;


        for(int i = 0; i < 12; i++)
        {
            signal_bit_map[i] = smem.vGetUCData(200+i);                                 //Save Signal Light Data
        }

        for(int i = 12; i < 14; i++)                                                  //Get ReverseTime Data
        {
            smem.vSetUCData(200+i, signal_bit_map[i]);
        }

        printf("*******************---------------_*****************\n");
        printf("*******************---------------_*****************\n");
        printf("light OOTTT: %x %x.\n", signal_bit_map[13], signal_bit_map[12]);
        printf("[OTMsg] _exec_reversetime_current_rev_step: %d\n", _exec_reversetime_current_rev_step);
        printf("[OTMsg] iTmpSec: %d\n", iTmpSec);
        printf("*******************---------------_*****************\n");
        printf("*******************---------------_*****************\n");

        //Debug
        char cTMP[256];
        if(smem.vGetUCData(revLogToFile) > 0)
        {
            sprintf(cTMP, "light:%X %X, revStep:%d, revSec:%d", signal_bit_map[13], signal_bit_map[12], _exec_reversetime_current_rev_step, iTmpSec);
            smem.vWriteMsgToDOM(cTMP);
        }

        if(smem.vGetINTData(TC92SignalStepStatus_5F03_IntervalTime)  == 0)            //Using 5F03 Cycle as this protocol
        {
            stc.vReportCCTRevStatus5F82();
        }

        ucSendTMP[0] = 0xAA;
        ucSendTMP[1] = 0xBB;
        ucSendTMP[2] = 0x11;
        ucSendTMP[3] = signal_bit_map[1];
        ucSendTMP[4] = signal_bit_map[0];
        ucSendTMP[5] = signal_bit_map[3];
        ucSendTMP[6] = signal_bit_map[2];
        ucSendTMP[7] = signal_bit_map[5];
        ucSendTMP[8] = signal_bit_map[4];
        ucSendTMP[9] = signal_bit_map[7];
        ucSendTMP[10] = signal_bit_map[6];
        ucSendTMP[11] = signal_bit_map[9];
        ucSendTMP[12] = signal_bit_map[8];
        ucSendTMP[13] = signal_bit_map[11];
        ucSendTMP[14] = signal_bit_map[10];
        ucSendTMP[15] = signal_bit_map[13];
        ucSendTMP[16] = signal_bit_map[12];
        ucSendTMP[17] = 0x00;
        ucSendTMP[18] = 0xAA;
        ucSendTMP[19] = 0xCC;
        ucSendTMP[20] = 0x00;
        for (int i=0; i<20; i++)
        {
            ucSendTMP[20]^=ucSendTMP[i];
        }

        writeJob.WritePhysicalOut(ucSendTMP, 21, DEVICETRAFFICLIGHT);


        /*+++++++++++++++++*/
//  writeJob.WritePhysicalOut(ucSendTMP, 21, DEVICETRAFFICLIGHT);                 //0x11
//  printf("Send writeJob DEVICETRAFFICLIGHT. (ReverseTime)\n");
        /*-----------------*/

        if( cCURRENTLIGHTSTATUS == smem.GetcFace() )
            screenCurrentLightStatus.vRefreshCurrentLightStatusData();

        //***********************************
    }
    catch (...) {}
}

//----------------------------------------------------
unsigned short CSTC::vStartReverseLaneInStep0(time_t timeIn, int *iRetTmpSec)   //調撥
{
    try
    {

        /*TRASH
          if(iRevInCitySec != 99999 && iRevOutCitySec == 99999) {  //in have setting, out have not setting
            if(iCurrentSec <= iRevInCitySec) {                                        // Cal to iRevInCitySec
              iTmpSec = iRevInCitySec - iCurrentSec;
              _exec_reversetime_current_rev_step = 1;
              printf("[OTMsg] iCurrentSec < iRevInCitySec && iRevOutCitySec == 99999\n");
              stc.vReportRevStatus5F02(1);  //send msg to center
            } else {
              iTmpSec = 86400 - iCurrentSec;                                          //more one sec
              _exec_reversetime_current_rev_step = 0;
              stc.vReportRevStatus5F02(0);  //send msg to center
            }
          } else if(iRevInCitySec == 99999 && iRevOutCitySec != 99999) {
            if(iCurrentSec <= iRevOutCitySec) {
              iTmpSec = iRevOutCitySec - iCurrentSec;
              _exec_reversetime_current_rev_step = 11;
              printf("[OTMsg] iCurrentSec < iRevInCitySec && iRevInCitySec == 99999\n");
              stc.vReportRevStatus5F02(1);  //send msg to center
            } else {
              iTmpSec = 86400 - iCurrentSec;                                          //more one sec
              _exec_reversetime_current_rev_step = 0;
              stc.vReportRevStatus5F02(0);  //send msg to center
            }
        */


        /* ----------------
        case 1: N

        case 1: N, I1, I2
        case 2: I1, N, I2
        case 3: I1, I2, N
        case 4: N, O1, O2
        case 5: O1, N, O2
        case 6: O1, O2, N

        case 1: N, I1, I2, O1, O2 ->  N, I1, O1
        case 2: I1, N, I2, O1, O2 -> I1, N, I2, O1
        case 3: I1, I2, N, O1, O2 -> I2, N, O1
        case 4: I1, I2, O1, N, O2 -> I2, O1, N, O2
        case 5: I1, I2, O1, O2, N -> I2, O2, N
        case 6: N, O1, O2, I1, I2 ->
        case 7: O1, N, O2, I1, I2
        case 8: O1, O2, N, I1, I2
        case 9: O1, O2, I1, N, I2
        case 10:O1, O2, I1, I2, N
        */
        unsigned char ucTmp;
        int iCurrentSec, iRevInCitySec, iRevOutCitySec;
        int iRevInCityEndSec, iRevOutCityEndSec;
        unsigned short int usiReverseLight;
//int iTmpSec = 10;
        struct tm* currenttime;
        currenttime = localtime(&timeIn);

        iCurrentSec = currenttime->tm_hour * 3600 + currenttime->tm_min * 60 + currenttime->tm_sec;
        iRevInCitySec = _exec_rev.usiHourStartIn * 3600 + _exec_rev.usiMinStartIn * 60;
        iRevInCityEndSec = _exec_rev.usiHourEndIn * 3600 + _exec_rev.usiMinEndIn * 60;
        iRevOutCitySec = _exec_rev.usiHourStartOut * 3600 + _exec_rev.usiMinStartOut * 60;
        iRevOutCityEndSec = _exec_rev.usiHourEndOut * 3600 + _exec_rev.usiMinEndOut * 60;

//All close case.  case 1
        if(        iRevInCitySec == 0 &&
                   iRevInCityEndSec == 0&&
                   iRevOutCitySec == 0 &&
                   iRevOutCityEndSec == 0 )
        {
            *iRetTmpSec = 86400 - iCurrentSec;                                            //more one sec
            _exec_reversetime_current_rev_step = 0;
//      stc.vReportRevStatus5F02(0);  //send msg to center

//Case 1
        }
        else if(iRevOutCitySec == 0 &&
                iRevOutCityEndSec == 0 &&
                iCurrentSec < iRevInCitySec &&
                iRevInCitySec < iRevInCityEndSec)
        {
            _exec_reversetime_current_rev_step = 1;
            *iRetTmpSec = iRevInCitySec - iCurrentSec;
//Case 2
        }
        else if(iRevOutCitySec == 0 &&
                iRevOutCityEndSec == 0 &&
                iRevInCitySec <= iCurrentSec &&
                iCurrentSec < iRevInCityEndSec)
        {
            _exec_reversetime_current_rev_step = 3;
            *iRetTmpSec = 1;
//Case 3
        }
        else if(iRevOutCitySec == 0 &&
                iRevOutCityEndSec == 0 &&
                iRevInCitySec < iRevInCityEndSec &&
                iRevInCityEndSec <= iCurrentSec)
        {
            _exec_reversetime_current_rev_step = 0;
            *iRetTmpSec = 86400 - iCurrentSec;                                           //more one sec
//Case 4
        }
        else if(iRevInCitySec == 0 &&
                iRevInCityEndSec == 0 &&
                iCurrentSec < iRevOutCitySec &&
                iRevOutCitySec < iRevOutCityEndSec)
        {
            _exec_reversetime_current_rev_step = 11;
            *iRetTmpSec = iRevOutCitySec - iCurrentSec;
//Case 5
        }
        else if(iRevInCitySec == 0 &&
                iRevInCityEndSec == 0 &&
                iRevOutCitySec <= iCurrentSec &&
                iCurrentSec < iRevOutCityEndSec)
        {
            _exec_reversetime_current_rev_step = 13;
            *iRetTmpSec = 1;
//Case 6
        }
        else if(iRevInCitySec == 0 &&
                iRevInCityEndSec == 0 &&
                iRevOutCitySec < iRevOutCityEndSec &&
                iRevOutCityEndSec <= iCurrentSec)
        {
            _exec_reversetime_current_rev_step = 0;
            *iRetTmpSec = 86400 - iCurrentSec;                                           //more one sec
//Case 1
        }
        else if(iCurrentSec < iRevInCitySec &&
                iRevInCitySec < iRevInCityEndSec &&
                iRevInCityEndSec < iRevOutCitySec &&
                iRevOutCitySec < iRevOutCityEndSec   )
        {
            _exec_reversetime_current_rev_step = 1;
            *iRetTmpSec = iRevInCitySec - iCurrentSec;
//Case 2
        }
        else if(iRevInCitySec <= iCurrentSec &&
                iCurrentSec < iRevInCityEndSec &&
                iRevInCityEndSec < iRevOutCitySec &&
                iRevOutCitySec < iRevOutCityEndSec   )
        {
            _exec_reversetime_current_rev_step = 3;                                 //immd. go to All "X"
            *iRetTmpSec = 1;
//Case 3
        }
        else if(iRevInCitySec < iRevInCityEndSec &&
                iRevInCityEndSec <= iCurrentSec &&
                iCurrentSec < iRevOutCitySec &&
                iRevOutCitySec < iRevOutCityEndSec   )
        {
            _exec_reversetime_current_rev_step = 11;
            *iRetTmpSec = iRevOutCitySec - iCurrentSec;
//Case 4
        }
        else if(iRevInCitySec < iRevInCityEndSec &&
                iRevInCityEndSec < iRevOutCitySec &&
                iRevOutCitySec <= iCurrentSec &&
                iCurrentSec < iRevOutCityEndSec         )
        {
            _exec_reversetime_current_rev_step = 13;
            *iRetTmpSec = 1;
//Case 5
        }
        else if(iRevInCitySec < iRevInCityEndSec &&
                iRevInCityEndSec < iRevOutCitySec &&
                iRevOutCitySec < iRevOutCityEndSec &&
                iRevOutCityEndSec <= iCurrentSec  )
        {
            _exec_reversetime_current_rev_step = 0;
            *iRetTmpSec = 86400 - iCurrentSec;
//Case 6
        }
        else if(iCurrentSec < iRevOutCitySec &&
                iRevOutCitySec < iRevOutCityEndSec &&
                iRevOutCityEndSec < iRevInCitySec &&
                iRevInCitySec < iRevInCityEndSec   )
        {
            _exec_reversetime_current_rev_step = 11;
            *iRetTmpSec = iRevOutCitySec - iCurrentSec;
//Case 7
        }
        else if(iRevOutCitySec <= iCurrentSec &&
                iCurrentSec < iRevOutCityEndSec &&
                iRevOutCityEndSec < iRevInCitySec &&
                iRevInCitySec < iRevInCityEndSec   )
        {
            _exec_reversetime_current_rev_step = 13;                              //immd. go to All "X"
            *iRetTmpSec = 1;
//Case 8
        }
        else if(iRevOutCitySec < iRevOutCityEndSec &&
                iRevOutCityEndSec <= iCurrentSec &&
                iCurrentSec < iRevInCitySec &&
                iRevInCitySec < iRevInCityEndSec   )
        {
            _exec_reversetime_current_rev_step = 1;
            *iRetTmpSec = iRevInCitySec - iCurrentSec;
//Case 9
        }
        else if(iRevOutCitySec < iRevOutCityEndSec &&
                iRevOutCityEndSec < iRevInCitySec &&
                iRevInCitySec <= iCurrentSec &&
                iCurrentSec < iRevInCityEndSec         )
        {
            _exec_reversetime_current_rev_step = 3;                                 //immd. go to All "X"
            *iRetTmpSec = 1;
//Case 10
        }
        else if(iRevOutCitySec < iRevOutCityEndSec &&
                iRevOutCityEndSec < iRevInCitySec &&
                iRevInCitySec < iRevInCityEndSec &&
                iRevInCityEndSec <= iCurrentSec  )
        {
            _exec_reversetime_current_rev_step = 0;
            *iRetTmpSec = 86400 - iCurrentSec;

        }
        else      //Error
        {
            _exec_reversetime_current_rev_step = 0;
            *iRetTmpSec = 10;
        }

        if(_exec_reversetime_current_rev_step == 0 ||
                _exec_reversetime_current_rev_step == 1 ||
                _exec_reversetime_current_rev_step == 11  )
        {
            printf("**************------------\n");
            printf("_exec_rev.ucNonRevLight:%d\n", _exec_rev.ucNonRevLight);
            printf("**************------------\n");

            if(_exec_rev.ucNonRevLight == 0)    //input and output has setting.
            {
                usiReverseLight = 0x0;
            }
            else if(_exec_rev.ucNonRevLight == 1)      //out town light
            {
                usiReverseLight = 0x00C3;
            }
            else if(_exec_rev.ucNonRevLight == 2)      //in town light
            {
                usiReverseLight = 0x003C;
            }
            else
            {
                usiReverseLight = 0x0;
            }

            if(usiReverseLight == 0x0)
            {
                ucTmp = smem.vGetUCData(revDefaultVehWay);
                if(ucTmp == 1)    //default to inTown
                {
                    usiReverseLight = 0x003C;
                }
                else if(ucTmp == 2)
                {
                    usiReverseLight = 0x00C3;
                }
            }

        }
        else if(_exec_reversetime_current_rev_step == 3 ||
                _exec_reversetime_current_rev_step == 13  )
        {
            usiReverseLight = 0x0033; //All X
        }


//-------------------------------------------


//OT990628
        /*
          if(iTmpSec <= 0) { iTmpSec = 10; }
          _itimer_reversetime.it_value.tv_sec = iTmpSec;
          _itimer_reversetime.it_value.tv_nsec = 0;
          _itimer_reversetime.it_interval.tv_sec = 0;
          _itimer_reversetime.it_interval.tv_nsec = 0;
          timer_settime( _timer_reversetime, 0, & _itimer_reversetime, NULL );
          timer_settime( _timer_reversetime, 0, & _itimer_reversetime, NULL );
          timer_settime( _timer_reversetime, 0, & _itimer_reversetime, NULL );
          timer_settime( _timer_reversetime, 0, & _itimer_reversetime, NULL );
        */

        return usiReverseLight;

    }
    catch(...) {}
}

//----------------------------------------------------
unsigned short CSTC::vStartReverseLane(time_t timeIn, int *iRetTmpSec) //Set 調撥步階, 秒數
{
    try
    {

        static time_t now;
        static struct tm* currenttime;
        int iRevEndSec;
        unsigned short int usiLight1 = 0x0;
        unsigned short int usiLight2 = 0x0;
        unsigned short int usiTmp = 0x0;

        currenttime = localtime(&timeIn);

        int iCurrentSec, iRevInCitySec, iRevOutCitySec;
        int iRevInCityEndSec, iRevOutCityEndSec;
        iCurrentSec = currenttime->tm_hour * 3600 + currenttime->tm_min * 60 + currenttime->tm_sec;
        iRevInCitySec = _exec_rev.usiHourStartIn * 3600 + _exec_rev.usiMinStartIn * 60;
        iRevInCityEndSec = _exec_rev.usiHourEndIn * 3600 + _exec_rev.usiMinEndIn * 60;
        iRevOutCitySec = _exec_rev.usiHourStartOut * 3600 + _exec_rev.usiMinStartOut * 60;
        iRevOutCityEndSec = _exec_rev.usiHourEndOut * 3600 + _exec_rev.usiMinEndOut * 60;

//  int iTmp;

// C: arrow
// 8: flash arrow
// 3: X

        // RevInCity
        if( _exec_reversetime_current_rev_step == 1 && _exec_rev.usiDirectIn < 9 )    // first step.
        {
            *iRetTmpSec = _exec_rev.usiGreenTime;
            usiLight1 = 0x00C3;                                                         // 00001001
//    usiLight1 = 0xC300;                                                         // 10010000
            _exec_reversetime_current_rev_step++;
        }
        else if( _exec_reversetime_current_rev_step == 2 && _exec_rev.usiDirectIn < 9 )
        {
            *iRetTmpSec = _exec_rev.usiFlashGreen;
            usiLight1 = 0x0083;                                                         // 00002001
//    usiLight1 = 0x8300;                                                         // 20010000
            _exec_reversetime_current_rev_step++;
        }
        else if( _exec_reversetime_current_rev_step == 3 && _exec_rev.usiDirectIn < 9 )
        {
            *iRetTmpSec = _exec_rev.usiClearTime;
            usiLight1 = 0x0033;                                                         // 00000101
//    usiLight1 = 0x3300;                                                         // 01010000
            _exec_reversetime_current_rev_step++;
        }
        else if( _exec_reversetime_current_rev_step == 4 && _exec_rev.usiDirectIn < 9 )
        {

//    int iRevInCityEndSec;
            iCurrentSec = currenttime->tm_hour * 3600 + currenttime->tm_min * 60 + currenttime->tm_sec;
            iRevEndSec = _exec_rev.usiHourEndIn * 3600 + _exec_rev.usiMinEndIn * 60;

            if(iRevEndSec <= iCurrentSec)    //Bug!!
            {
                _exec_reversetime_current_rev_step = 0;
                usiLight1 = 0x0033;
                *iRetTmpSec = _exec_rev.usiClearTime;
            }
            else
            {
                *iRetTmpSec = iRevEndSec - iCurrentSec;
                if(*iRetTmpSec > 0 && *iRetTmpSec <= 86400 && (iRevInCitySec <= iCurrentSec && iCurrentSec < iRevInCityEndSec) ) {}
                else
                {
                    *iRetTmpSec = _exec_rev.usiClearTime;
                }
                usiLight1 = 0x003C;                                                         // 00000110
//    usiLight1 = 0x3C00;                                                         // 01100000
                _exec_reversetime_current_rev_step++;
            }
        }
        else if( _exec_reversetime_current_rev_step == 5 && _exec_rev.usiDirectIn < 9 )
        {
            *iRetTmpSec = _exec_rev.usiFlashGreen;
            usiLight1 = 0x0038;                                                         // 00000120
//    usiLight1 = 0x3800;                                                         // 01200000
            _exec_reversetime_current_rev_step++;
        }
        else if( _exec_reversetime_current_rev_step == 6 && _exec_rev.usiDirectIn < 9 )
        {
            *iRetTmpSec = _exec_rev.usiClearTime;
            usiLight1 = 0x0033;                                                         // 00000101
//    usiLight1 = 0x3300;                                                         // 01010000
            _exec_reversetime_current_rev_step++;
        }
        else if( _exec_reversetime_current_rev_step == 7 && _exec_rev.usiDirectIn < 9 )
        {
            *iRetTmpSec = _exec_rev.usiGreenTime;
            usiLight1 = 0x00C3;                                                         // 00001001
//    usiLight1 = 0xC300;                                                         // 10010000
            _exec_reversetime_current_rev_step++;
        }
        else if( _exec_reversetime_current_rev_step == 8 && _exec_rev.usiDirectIn < 9 )
        {
            *iRetTmpSec = 1;
            usiLight1 = 0x0;                                                            // 00000000
            _exec_reversetime_current_rev_step = 0;
            stc.vReportRevStatus5F02(0);  //send msg to center
        }

        // RevOutCity
        if( _exec_reversetime_current_rev_step == 11 && _exec_rev.usiDirectOut < 9 )
        {
            *iRetTmpSec = _exec_rev.usiGreenTime;
//    usiLight2 = 0x3C00;                                                         // 01100000
            usiLight2 = 0x003C;                                                         // 00000110
            _exec_reversetime_current_rev_step++;
        }
        else if( _exec_reversetime_current_rev_step == 12 && _exec_rev.usiDirectOut < 9 )
        {
            *iRetTmpSec = _exec_rev.usiFlashGreen;
//    usiLight2 = 0x3800;                                                         // 01200000
            usiLight2 = 0x0038;                                                         // 00000120
            _exec_reversetime_current_rev_step++;
        }
        else if( _exec_reversetime_current_rev_step == 13 && _exec_rev.usiDirectOut < 9 )
        {
            *iRetTmpSec = _exec_rev.usiClearTime;
//    usiLight2 = 0x3300;                                                         // 01010000
            usiLight2 = 0x0033;                                                         // 00000101
            _exec_reversetime_current_rev_step++;
        }
        else if( _exec_reversetime_current_rev_step == 14 && _exec_rev.usiDirectOut < 9 )
        {
//    int iRevOutCityEndSec;
            iCurrentSec = currenttime->tm_hour * 3600 + currenttime->tm_min * 60 + currenttime->tm_sec;
            iRevEndSec = _exec_rev.usiHourEndOut * 3600 + _exec_rev.usiMinEndOut * 60;

            if(iRevEndSec <= iCurrentSec)    //Bug!!
            {
                _exec_reversetime_current_rev_step = 0;
                usiLight2 = 0x0033;
                *iRetTmpSec = _exec_rev.usiClearTime;
            }

            else
            {
                *iRetTmpSec = iRevEndSec - iCurrentSec;
                if(*iRetTmpSec > 0 && *iRetTmpSec <= 86400 && (iRevOutCitySec <= iCurrentSec && iCurrentSec < iRevOutCityEndSec) ) {}
                else
                {
                    *iRetTmpSec = _exec_rev.usiClearTime;
                }
//    usiLight2 = 0xC300;                                                         // 10010000
                usiLight2 = 0x00C3;                                                         // 00001001
                _exec_reversetime_current_rev_step++;
            }
        }
        else if( _exec_reversetime_current_rev_step == 15 && _exec_rev.usiDirectOut < 9 )
        {
            *iRetTmpSec = _exec_rev.usiFlashGreen;
//    usiLight2 = 0x8300;                                                         // 20010000
            usiLight2 = 0x0083;                                                         // 00002001
            _exec_reversetime_current_rev_step++;
        }
        else if( _exec_reversetime_current_rev_step == 16 && _exec_rev.usiDirectOut < 9 )
        {
            *iRetTmpSec = _exec_rev.usiClearTime;
//    usiLight2 = 0x3300;                                                         // 01010000
            usiLight2 = 0x0033;                                                         // 00000101
            _exec_reversetime_current_rev_step++;
        }
        else if( _exec_reversetime_current_rev_step == 17 && _exec_rev.usiDirectOut < 9 )
        {
            *iRetTmpSec = _exec_rev.usiGreenTime;
//    usiLight2 = 0x3C00;                                                         // 01100000
            usiLight2 = 0x003C;                                                         // 00000110
            _exec_reversetime_current_rev_step++;
        }
        else if( _exec_reversetime_current_rev_step == 18 && _exec_rev.usiDirectOut < 9 )
        {
            *iRetTmpSec = 1;
            usiLight2 = 0x0;
            _exec_reversetime_current_rev_step = 0;
            stc.vReportRevStatus5F02(0);  //send msg to center
        }
        else      //Find Bug.
        {
            usiLight1 = 0x0033;
            _exec_reversetime_current_rev_step = 0;
            *iRetTmpSec = 1;
//    usiLight2 = 0x0033;
        }

//OT Debug
        /*
          if( iTmp > 0) {
            _itimer_reversetime.it_value.tv_sec = iTmp;
            _itimer_reversetime.it_value.tv_nsec = 0;
          }
          else {
            _itimer_reversetime.it_value.tv_sec = 0;
            _itimer_reversetime.it_value.tv_nsec = 100;
          }
          _itimer_reversetime.it_interval.tv_sec = 0;
          _itimer_reversetime.it_interval.tv_nsec = 0;

          timer_settime( _timer_reversetime, 0, & _itimer_reversetime, NULL );
          timer_settime( _timer_reversetime, 0, & _itimer_reversetime, NULL );
          timer_settime( _timer_reversetime, 0, & _itimer_reversetime, NULL );
          timer_settime( _timer_reversetime, 0, & _itimer_reversetime, NULL );
        */

//Debug
        char cTMP[256];
        if(smem.vGetUCData(revLogToFile) > 0)
        {

            printf("*******************---------------_*****************\n");
            printf("[OTMsg] usiLight1:%04X, usiLight2:%04X\n", usiLight1, usiLight2);
            printf("*******************---------------_*****************\n");

            sprintf(cTMP, "[OTMsg] usiLight1:%04X, usiLight2:%04X", usiLight1, usiLight2);
            smem.vWriteMsgToDOM(cTMP);
        }


        if(usiLight1 != 0 && usiLight2 == 0)
        {
            return usiLight1;
        }
        if(usiLight2 != 0 && usiLight1 == 0)
        {
            return usiLight2;
        }
        return 0;
//  usiTmp = usiLight1 + usiLight2;
//  return usiTmp;

    }
    catch(...) {}
}
//----------------------------------------------------
bool CSTC::vSetReverseTimer(int iTimer)
{
    try
    {
        if( iTimer > 0)
        {
            _itimer_reversetime.it_value.tv_sec = iTimer;
            _itimer_reversetime.it_value.tv_nsec = 0;
        }
        else
        {
            _itimer_reversetime.it_value.tv_sec = 1;
            _itimer_reversetime.it_value.tv_nsec = 0;
        }
        _itimer_reversetime.it_interval.tv_sec = 0;
        _itimer_reversetime.it_interval.tv_nsec = 0;

        timer_settime( _timer_reversetime, 0, & _itimer_reversetime, NULL );
        timer_settime( _timer_reversetime, 0, & _itimer_reversetime, NULL );
        timer_settime( _timer_reversetime, 0, & _itimer_reversetime, NULL );
        timer_settime( _timer_reversetime, 0, & _itimer_reversetime, NULL );

    }
    catch(...) {}
}
//----------------------------------------------------
void CSTC::vResetReverseTime(void)
{
    try
    {
        printf("GoTo vResetReverseTime()!\n");
        _exec_reversetime_current_rev_step = 0;
        ReverseTimeDataUpdate = true;
        if(smem.vGetUCData(TC_ReverseLane_Manual_Enable_Status) == 0)
        {
            printf("Rev Auto Mode running!\n");
            vDetermine_ReverseTime();
        }
        else
        {
            printf("Rev Manule Mode running!\n");
        }
    }
    catch(...) {}
}


//----------------------------------------------------------
void CSTC::Lock_to_Determine_SegmentPlanPhase(void) //若執行觸動模式則會進入此funtion chou check 2013/01/30    //Cycle start set
{
    try
    {
        // printf("1\n");
        int iExecSeg;
        unsigned char ucTmp;
        DATA_Bit _ControlStrategy;

        //OT20131225
        unsigned short int usiCCJ_HeartBeatCount;

        if( smem.vGetBOOLData(TC_CCTActuate_TOD_Running) == true)
        {
            //Every New Dete, Reset.
            // printf("2\n");
            smem.vSetBOOLData(TC_CCTActuate_TOD_Running, false);
            PlanUpdate = true;
        }
        else
        {
            //  printf("3\n");
        }

        if(smem.vGetBOOLData(TCSegmentTypeUpdate) == true)
        {
            //  printf("4\n");
            smem.vSetBOOLData(TCSegmentTypeUpdate, false);
            SegmentTypeUpdate = true;
        }
        else
        {
            //    printf("5\n");
        }
        printf("Lock_to_Determine_SegmentPlanPhase\n");

        static time_t now;
        static struct tm* currenttime;
        now = time(NULL);
        currenttime = localtime(&now);
        printf("Date: %4d/%2d/%2d  Time: %2d:%2d:%2d\n"
               , currenttime->tm_year+1900,currenttime->tm_mon+1,currenttime->tm_mday
               , currenttime->tm_hour, currenttime->tm_min, currenttime->tm_sec);
        //***********************************
        //****   check if in holidayseg
        bool _exec_segment_is_holiday=false;
        for(int i=0; i<AMOUNT_HOLIDAY_SEG; i++)
        {
            //特別日
            if(    (    ( (currenttime->tm_year+1900) > holidayseg[i]._start_year )
                        || ( (currenttime->tm_year+1900)== holidayseg[i]._start_year && (currenttime->tm_mon+1) > holidayseg[i]._start_month )
                        || ( (currenttime->tm_year+1900)== holidayseg[i]._start_year && (currenttime->tm_mon+1)== holidayseg[i]._start_month
                             && (currenttime->tm_mday)>= holidayseg[i]._start_day ) )
                    && (    ( (currenttime->tm_year+1900) < holidayseg[i]._end_year )
                            || ( (currenttime->tm_year+1900)== holidayseg[i]._end_year   && (currenttime->tm_mon+1) < holidayseg[i]._end_month )
                            || ( (currenttime->tm_year+1900)== holidayseg[i]._end_year   && (currenttime->tm_mon+1)== holidayseg[i]._end_month
                                 && (currenttime->tm_mday)<= holidayseg[i]._end_day ) ) )
            {
                if(_exec_segment._segment_type!=segment[holidayseg[i]._segment_type]._segment_type|| SegmentTypeUpdate == true)
                {
                    /******** lock mutex ********/
                    pthread_mutex_lock(&CSegmentInfo::_segment_mutex);
                    if(segment[holidayseg[i]._segment_type]._ptr_seg_exec_time)
                    {
                        //Modify Arwen _exec_segment = segment[holidayseg[i]._segment_type];
                        //!!!Arwen ++ 1000411 Actuate Run Segment
                        _exec_segment = new_data_segment[holidayseg[i]._segment_type];
                        _exec_BRTPriority_segment = BRTsegment[holidayseg[i]._segment_type]; //jacky20140423
                        if(smem.cPedPushButton.GetOpenSegmentAlter()>0)
                        {
                            printf("Arwen-- Holidayseg Before Segment:%d ----> After Segment:%d\n",
                                   holidayseg[i]._segment_type,_exec_segment._actSegmentType1);
                            _exec_segment = new_data_segment[_exec_segment._actSegmentType1];
                            _exec_BRTPriority_segment = BRTsegment[_exec_segment._actSegmentType1]; //jacky20140423
                        }
                        //!!!Arwen ++ 1000411 Actuate Run Segment End
                    }
                    else
                    {
                        _exec_segment = segment[DEFAULT_SEGMENTTYPE];
                        _exec_BRTPriority_segment = BRTsegment[DEFAULT_SEGMENTTYPE]; //jacky20140423
                    }
                    /******** unlock mutex ********/
                    pthread_mutex_unlock(&CSegmentInfo::_segment_mutex);
                    if(SegmentTypeUpdate == true)
                        SegmentTypeUpdate = false;

                    smem.vSetUCData(CSTC_SegmentNoRunning, i+1);
                    smem.SetSegmentChange(true);  //jacky20140812
                }
                _exec_segment_is_holiday=true;

                printf("Today is in Holiday Segment:%d\n",i);

                break;
            }
        }

        //***********************************
        //****  check in which weekdayseg
        if(_exec_segment_is_holiday==false)
        {
            //一般日
            if(    ((((((currenttime->tm_mday-1)/7)+1)%2)==0 ) && (currenttime->tm_wday>=((currenttime->tm_mday-1)%7)))
                    || ((((((currenttime->tm_mday-1)/7)+1)%2)> 0 ) && (currenttime->tm_wday< ((currenttime->tm_mday-1)%7))) )
            {
                printf("This week is an even week\n");
                int wday = (currenttime->tm_wday==0)?13:(currenttime->tm_wday+6);
                if(_exec_segment._segment_type != weekdayseg[wday]._segment_type || SegmentTypeUpdate == true)
                {
                    //odd false
                    //    printf("6\n");
                    /******** lock mutex ********/
                    pthread_mutex_lock(&CSegmentInfo::_segment_mutex);
                    if(segment[weekdayseg[wday]._segment_type]._ptr_seg_exec_time)
                    {
                        //      printf("7\n");
                        //Modify Arwen _exec_segment = segment[weekdayseg[wday]._segment_type]; //tm_wday:{1-6} => P92:{11-16} => weekdayseg:{7-12}
                        //              printf("OT_exec_segment:%d", _exec_segment._segment_type);
                        //!!!Arwen ++ 1000411 Actuate Run Segment
                        _exec_segment = new_data_segment[weekdayseg[wday]._segment_type];
                        _exec_BRTPriority_segment = BRTsegment[weekdayseg[wday]._segment_type]; //jacky20140423
                        if(smem.cPedPushButton.GetOpenSegmentAlter()>0)
                        {
                            //      printf("8\n");
                            printf("Arwen-- Even Week Before Segment:%d ----> After Segment:%d\n",
                                   weekdayseg[wday]._segment_type,_exec_segment._actSegmentType1);
                            _exec_segment = new_data_segment[_exec_segment._actSegmentType1];
                            _exec_BRTPriority_segment = BRTsegment[_exec_segment._actSegmentType1]; //jacky20140423
                        }
                        //!!!Arwen ++ 1000411 Actuate Run Segment End
                    }
                    else
                    {
                        _exec_segment = segment[DEFAULT_SEGMENTTYPE];
                        _exec_BRTPriority_segment = BRTsegment[DEFAULT_SEGMENTTYPE]; //jacky20140423
                    }
                    /******** unlock mutex ********/
                    pthread_mutex_unlock(&CSegmentInfo::_segment_mutex);
                    if(SegmentTypeUpdate == true)
                        SegmentTypeUpdate = false;

                    smem.SetSegmentChange(true);  //jacky20140812
                }
            }//end if(even week)
            else
            {
                //odd week
                printf("This week is an odd week\n");
                printf("9\n");
                int wday = (currenttime->tm_wday==0)?6:(currenttime->tm_wday-1);
                if(_exec_segment._segment_type != weekdayseg[wday]._segment_type || SegmentTypeUpdate == true)
                {
                    /******** lock mutex ********/
                    pthread_mutex_lock(&CSegmentInfo::_segment_mutex);
                    if(segment[weekdayseg[wday]._segment_type]._ptr_seg_exec_time)
                    {
                        //normal true
                        printf("10\n");
                        //Modify Arwen _exec_segment = segment[weekdayseg[wday]._segment_type]; //tm_wday:{1-6} => P92:{1-6} => weekdayseg:{0-5}
                        //!!!Arwen ++ 1000411 Actuate Run Segment
                        _exec_segment = new_data_segment[weekdayseg[wday]._segment_type];
                        _exec_BRTPriority_segment = BRTsegment[weekdayseg[wday]._segment_type]; //jacky20140423
                        if(smem.cPedPushButton.GetOpenSegmentAlter()>0)
                        {
                            printf("10\n");
                            printf("Arwen-- odd Week Before Segment:%d ----> After Segment:%d\n",
                                   weekdayseg[wday]._segment_type,_exec_segment._actSegmentType1);
                            _exec_segment = new_data_segment[_exec_segment._actSegmentType1];
                            _exec_BRTPriority_segment = BRTsegment[_exec_segment._actSegmentType1]; //jacky20140423
                        }
                        //!!!Arwen ++ 1000411 Actuate Run Segment End
                    }
                    else
                    {
                        _exec_segment = segment[DEFAULT_SEGMENTTYPE];
                        _exec_BRTPriority_segment = BRTsegment[DEFAULT_SEGMENTTYPE]; //jacky20140423
                    }
                    /******** unlock mutex ********/
                    pthread_mutex_unlock(&CSegmentInfo::_segment_mutex);
                    if(SegmentTypeUpdate == true)
                        SegmentTypeUpdate = false;

                    printf("11\n");
                    smem.SetSegmentChange(true);  //jacky20140812
                }
            }//end else(odd week)
        }//end else(holiday)


        //***********************************
        //**** check and modify _exec_plan
        for(int i=0; i<_exec_segment._segment_count; i++)
        {
            iExecSeg = i;
            if(     ( i == (_exec_segment._segment_count-1) )  //last segment
                    ||(  ( i <  (_exec_segment._segment_count-1) )  //not last segment
                         &&(    (    ( currenttime->tm_hour > _exec_segment._ptr_seg_exec_time[i]._hour )
                                     || ( currenttime->tm_hour ==_exec_segment._ptr_seg_exec_time[i]._hour
                                          && currenttime->tm_min >=_exec_segment._ptr_seg_exec_time[i]._minute  ) )
                                && (    ( currenttime->tm_hour  <_exec_segment._ptr_seg_exec_time[i+1]._hour)
                                        || ( currenttime->tm_hour ==_exec_segment._ptr_seg_exec_time[i+1]._hour
                                             && currenttime->tm_min  <_exec_segment._ptr_seg_exec_time[i+1]._minute) ) ) ) )
            {


                /* otaru debug
                        printf("i = %d\n", i);
                        printf("currenttime->tm_hour = %d, _exec_segment._ptr_seg_exec_time[i]._hour = %d\n",
                               currenttime->tm_hour, _exec_segment._ptr_seg_exec_time[i]._hour);
                        printf("currenttime->tm_min = %d, _exec_segment._ptr_seg_exec_time[i]._minute = %d\n",
                               currenttime->tm_min, _exec_segment._ptr_seg_exec_time[i]._minute);
                        printf("_exec_segment._ptr_seg_exec_time[i+1]._hour = %d, _exec_segment._ptr_seg_exec_time[i+1]._minute = %d\n",
                               _exec_segment._ptr_seg_exec_time[i+1]._hour, _exec_segment._ptr_seg_exec_time[i+1]._minute);
                       */
                //OT20110607
                smem.vSetUCData(CSTC_SegmentNoRunning, i+1);

                pthread_mutex_lock(&CPlanInfo::_plan_mutex);


                OLD_TOD_PLAN_ID = NEW_TOD_PLAN_ID;
                NEW_TOD_PLAN_ID = _exec_segment._ptr_seg_exec_time[i]._planid;
                //ucTODNextPlanID = NEW_TOD_PLAN_ID;//CCJ20140812 記錄下一個TOD
                smem.vSetUSIData(TOD_PLAN_ID, NEW_TOD_PLAN_ID);
                //printf("\n\n!! %d !!\n\n",_exec_BRTPriority_segment._ptr_seg_exec_time[i]._onoff);
                smem.SetDefiniteTimeonoff(_exec_BRTPriority_segment._ptr_seg_exec_time[i]._onoff);

                printf("OLD_TOD_PLAN_ID = %d\n",OLD_TOD_PLAN_ID);
                printf("NEW_TOD_PLAN_ID = %d\n",NEW_TOD_PLAN_ID);
                if(OLD_TOD_PLAN_ID != NEW_TOD_PLAN_ID)
                {
                    PlanUpdate = true;
                    //jacky20141217 Taichung BRT
                    smem.SetPlantransition(true);
                    unsigned char uc0F04[4];
                    uc0F04[0] = 0x0F;
                    uc0F04[1] = 0x04;
                    MESSAGEOK _MSG;
                    //send HW Status
                    uc0F04[2] = smem.vGetHardwareStatus(3);        //取得HWstat
                    uc0F04[3] = smem.vGetHardwareStatus(4);        //取得HWstat
                    _MSG = oDataToMessageOK.vPackageINFOTo92Protocol(uc0F04, 4, true);
                    _MSG.InnerOrOutWard = cOutWard;
                    writeJob.WritePhysicalOut(_MSG.packet, _MSG.packetLength, DEVICECENTER92);
                    smem.SetPlantransition(false);
                }
                //printf("12\n");
                //----------------------------------------------------------------------------------------------------
                //OTFix 940804
                /*
                        if( smem.vGetBOOLData(TC92_PlanOneTime5F18_Change) ) {
                          int iRunPlanID = smem.vGetINTData(TC92_PlanOneTime5F18_PlanID);
                          _exec_segment._ptr_seg_exec_time[i]._planid = iRunPlanID;
                //        _exec_plan = plan[iRunPlanID]
                          PlanUpdate = true;

                          smem.vSetBOOLData(TC92_PlanOneTime5F18_Change, false);
                        }
                */

//5F18, change plan few times by protocol center.
                /*OT990322
                        _ControlStrategy.DBit = smem.vGetUCData(TC92_ucControlStrategy);
                        if( _ControlStrategy.switchBit.b4 == true && OLD_TOD_PLAN_ID == NEW_TOD_PLAN_ID) {
                          int iRunPlanID = smem.vGetINTData(TC92_PlanOneTime5F18_PlanID);
                          _exec_segment._ptr_seg_exec_time[i]._planid = iRunPlanID;
                          _exec_plan = plan[_exec_segment._ptr_seg_exec_time[i]._planid];
                //        _exec_plan = plan[iRunPlanID]
                //          PlanUpdate = true;
                        }
                */
//OT990322
                _ControlStrategy.DBit = smem.vGetUCData(TC92_ucControlStrategy);
                //OT20131225
                //check heart bert, when no heart beat got, force reset to TOD
                usiCCJ_HeartBeatCount = smem.vGetUSIData(CCJ_HeartBeatCount);
                if(_ControlStrategy.switchBit.b4 == true && _ControlStrategy.switchBit.b2 == true)
                {
                    //means in CCJ Dyn Ctl now.
                    if(usiCCJ_HeartBeatCount == 0)
                    {
                        //means get null heartBeat
                        //reset to TOD
                        _ControlStrategy.DBit = smem.vGetUCData(TC92_ucControlStrategy);
                        _ControlStrategy.switchBit.b4 = false;
                        _ControlStrategy.switchBit.b2 = false;
                        smem.vSetUCData(TC92_ucControlStrategy, _ControlStrategy.DBit);
                        smem.vSetBOOLData(TC92_PlanOneTime5F18_Change, false);
                        if(smem.vGet5F18EffectTime() > 0)
                        {
                            smem.vSet5F18EffectTime(2);
                        }
                    }
                    else
                    {
                        smem.vAddCCJHeartBeatCount(0);
                    }
                }
                if( OLD_TOD_PLAN_ID != NEW_TOD_PLAN_ID)
                {
                }
                /*OT990322
                        if(smem.vGetBOOLData(TC92_PlanOneTime5F18_Change) == true) {
                //ot990316bug?        if(smem.vSetBOOLData(TC92_PlanOneTime5F18_Change, true)) {
                          PlanUpdate = true;
                          SegmentTypeUpdate = true;
                        }
                */
                //動態時段開始
                smem.vSetBOOLData(TCDynSegStatus, false);
                bool bDynSegSwitch = smem.vGetBOOLData(TCDynSegSwitch);

                if(bDynSegSwitch)
                {
                    if(iDynSeg_SegType != _exec_segment._segment_type)
                    {
                        // lock mutex
                        pthread_mutex_lock(&CSegmentInfo::_segment_mutex);
                        if(segment[iDynSeg_SegType]._ptr_seg_exec_time)
                            _exec_segment = segment[iDynSeg_SegType];

                        else
                            _exec_segment = segment[DEFAULT_SEGMENTTYPE];

                        // unlock mutex
                        pthread_mutex_unlock(&CSegmentInfo::_segment_mutex);
                        smem.vSetBOOLData(TCDynSegStatus, true);
                    }
                    if(iDynSeg_PlanID != _exec_plan._planid)
                    {
                        // lock mutex
                        printf("BBB start\n");
                        //pthread_mutex_lock(&CPlanInfo::_plan_mutex);
                        if(plan[iDynSeg_PlanID]._ptr_subplaninfo)
                        {
                            _exec_plan = plan[iDynSeg_PlanID];
                            //CCJ20150812
                            OLD_TOD_PLAN_ID = NEW_TOD_PLAN_ID;
                            NEW_TOD_PLAN_ID = iDynSeg_PlanID;
                            printf("iDynSeg_PlanID NEW_TOD_PLAN_ID: %d\n",NEW_TOD_PLAN_ID);
                            smem.vSetUSIData(TOD_PLAN_ID, NEW_TOD_PLAN_ID);
                        }
                        else
                        {
                            _exec_plan = plan[DEFAULT_PLANID];
                        }
                        // unlock mutex
                        //pthread_mutex_unlock(&CPlanInfo::_plan_mutex);
                        smem.vSetBOOLData(TCDynSegStatus, true);
                    }
                    else
                    {
                        //CCJ20150812
                        OLD_TOD_PLAN_ID = NEW_TOD_PLAN_ID;
                        NEW_TOD_PLAN_ID = iDynSeg_PlanID;
                        printf("iDynSeg_PlanID = _exec_plan.planid NEW_TOD_PLAN_ID: %d\n",NEW_TOD_PLAN_ID);
                        smem.vSetUSIData(TOD_PLAN_ID, NEW_TOD_PLAN_ID);
                    }
                }
                printf("finish DynsegSwitch\n");
                //動態時段結束

//OT990322
                if(smem.vGetBOOLData(TC92_PlanOneTime5F18_Change) == true)
                {
                    //printf("13\n");                       //false
                    printf("This cycle run CCJ 5F18 changed PLAN.\n");
                    smem.vSetThisCycleRunCCJPlan5F18(true);
                    smem.vSetBOOLData(TC92_PlanOneTime5F18_Change, false);
                    int iRunPlanID = smem.vGetINTData(TC92_PlanOneTime5F18_PlanID);
                    printf("iRunPlanID = %d\n",iRunPlanID);
                    if(iRunPlanID <= 48)
                    {
                        //預設值為40 原為255 chou check 2013/01/30
                        //  printf("14\n");
                        _exec_segment._ptr_seg_exec_time[i]._planid = iRunPlanID;
                        _exec_plan = plan[_exec_segment._ptr_seg_exec_time[i]._planid];
                        _exec_BRTPriority_plan = BRTplan[_exec_segment._ptr_seg_exec_time[i]._planid];  //jacky20140514
                        PlanUpdate = true;
                        SegmentTypeUpdate = true;
//OT20110609
                        OLD_TOD_PLAN_ID = NEW_TOD_PLAN_ID;
                        NEW_TOD_PLAN_ID = iRunPlanID;
                        smem.vSetUSIData(TOD_PLAN_ID, NEW_TOD_PLAN_ID);

                        printf("in 5F18 cycle, runPlanID:%d\n", iRunPlanID);
                    }
                }
                else
                {
                    //不觸發閃光觸動
                    //printf("15\n");
                    //printf("iRunPlanID = %d iRunPlanID > 48 \n", smem.vGetINTData(TC92_PlanOneTime5F18_PlanID));
                    printf("smem.vGetBOOLData(TC92_PlanOneTime5F18_Change) = false");
                }

                if(smem.vGetUCData(actuateSwitchChange_for_Arwen) >= 1)
                {
                    //arwen ++ 1000711
                    // printf("16\n");
                    m_segment_lastActMode=99;
                    smem.vSetUCData(actuateSwitchChange_for_Arwen,0);
                }
                printf("m_segment_lastActMode = %d _exec_segment._ptr_seg_exec_time[%d]._actMode = %d\n\n",m_segment_lastActMode,i,_exec_segment._ptr_seg_exec_time[i]._actMode);
                /////////////////////////////////////////////////////////////////////////////////
                if(m_segment_lastActMode !=_exec_segment._ptr_seg_exec_time[i]._actMode)
                {
                    //arwen ++
                    //printf("17\n");
                    changeActModeToArwenAct(_exec_segment._ptr_seg_exec_time[i]._actMode);//arwen ++
                    m_segment_lastActMode =_exec_segment._ptr_seg_exec_time[i]._actMode;//arwen ++
                }
                ///////////////////////////////////////////////////////////////////////////////////
//OT1000218
                if(smem.vGet5F18EffectTime() > 0)
                {
                    //this cycle run 5F18Plan
                    //printf("18\n");
                    //int iRunPlanID = smem.vGetINTData(TC92_PlanOneTime5F18_PlanID);
                    int iRunPlanID = smem.vGetINTData(TC92_PlanOneTime5F18_PlanID);
                    if(iRunPlanID <= 48)
                    {
                        _exec_segment._ptr_seg_exec_time[i]._planid = iRunPlanID;
                        _exec_plan = plan[_exec_segment._ptr_seg_exec_time[i]._planid];
                        _exec_BRTPriority_plan = BRTplan[_exec_segment._ptr_seg_exec_time[i]._planid];  //jacky20140514
                        //OT20110609
                        OLD_TOD_PLAN_ID = NEW_TOD_PLAN_ID;
                        NEW_TOD_PLAN_ID = iRunPlanID;
                        smem.vSetUSIData(TOD_PLAN_ID, NEW_TOD_PLAN_ID);
                        PlanUpdate = true;
                        SegmentTypeUpdate = true;
                        printf("in 5F18 effect time mode, runPlanID:%d\n", iRunPlanID);
                        //OT20111028
                        smem.vSetThisCycleRunCCJPlan5F18(true);
                    }
                    else
                    {
                        smem.vSetThisCycleRunCCJPlan5F18(false);
                    }
                }
                else
                {
                    smem.vSetThisCycleRunCCJPlan5F18(false);
                }//end if(smem.vGet5F18EffectTime() > 0)

//20140526~20140527
//若是亮光／長綠則載入對應時制計劃切回亮光或長綠
//若是半觸動和固定則時是用原來TOD
                printf("ACT Type:%d Plan:%d\n",smem.vGetUCData(ActType),smem.vGetUSIData(ActPlan));
                if(smem.vGetUCData(ActType)==0x01)
                {
                    //半觸動
                    printf("smem.vGetUCData(ActType)==0x01\n");
                }
                else if(smem.vGetUCData(ActType)==0x02)
                {
                    //固定時制
                    printf("smem.vGetUCData(ActType)==0x02\n");
                }
                else if(smem.vGetUCData(ActType)==0x03)
                {
                    //閃光觸動
                    printf("smem.vGetUCData(ActType)==0x03\n");
                    if(smem.vGetThisCycleRunCCJPlan5F18() == true)
                    {
                        printf("on 5F18\n");
                        smem.vSet5F18EffectTime(2);//20140527縮短5F18有效時間，確保只執行一次把時間短
                    }
                    else
                    {
                        _exec_segment._ptr_seg_exec_time[i]._planid = smem.vGetUSIData(ActPlan);
                        PlanUpdate = true;
                        printf("change plan: %d\n",_exec_segment._ptr_seg_exec_time[i]._planid);
                    }
                }
                else if(smem.vGetUCData(ActType)==0x04)
                {
                    //長綠觸動
                    printf("smem.vGetUCData(ActType)==0x04\n");
                    if(smem.vGetThisCycleRunCCJPlan5F18() == true)
                    {
                        printf("on 5F18\n");
                        smem.vSet5F18EffectTime(2);//20140527縮短5F18有效時間，確保只執行一次把時間短
                    }
                    else
                    {
                        _exec_segment._ptr_seg_exec_time[i]._planid = smem.vGetUSIData(ActPlan);
                        PlanUpdate = true;
                        printf("change plan: %d\n",_exec_segment._ptr_seg_exec_time[i]._planid);
                    }
                }
                /* CCJ20150723 mark
                //For Normol Actuate
                        if(smem.vGetUCData(TC_CCT_ActuateType_By_TOD) == 0) {                    //When actuate, change to special plan
                          //printf("20\n");
                          printf("TC_CCT_ActuateType_By_TOD is 0, When actuate, change to special plan\n");
                          if( smem.vGetBOOLData(TC_CCT_In_LongTanu_ActuateType_Switch)) {       //Actuate By WalkManButton 暫時不用 chou check 2013/01/30
                            //  printf("21\n");
                              printf("TC_CCT_In_LongTanu_ActuateType_Switch is true, actuate By WalkManButton\n");
                              _exec_segment._ptr_seg_exec_time[i]._planid = smem.vGetUSIData(TC_CCT_In_LongTanu_ActuateType_Switch);
                              PlanUpdate = true;
                              smem.vSetBOOLData(TC_CCTActuate_TOD_Running, true);
                          }
                          if ( smem.vGetBOOLData(TC_Actuate_By_TFD) ) {
                            //  printf("22\n");
                              printf("TC_Actuate_By_TFD is true, _exec_segment._ptr_seg_exec_time[%d]._cadc_seg:%x\n", i, _exec_segment._ptr_seg_exec_time[i]._cadc_seg);
                              if(_exec_segment._ptr_seg_exec_time[i]._cadc_seg == 0xEEEE) {     //Actuate By TFD
                            //    printf("23\n");
                                _exec_segment._ptr_seg_exec_time[i]._planid = smem.vGetUSIData(TC_CCT_In_LongTanu_ActuateType_Switch);
                                PlanUpdate = true;
                                smem.vSetBOOLData(TC_CCTActuate_TOD_Running, true);
                              }
                          }
                        } else if(smem.vGetUCData(TC_CCT_ActuateType_By_TOD) == 1) {
                              printf("smem.vGetUCData(TC_CCT_ActuateType_By_TOD) == 1\n");
                              //printf("24\n"); //act mode do
                              if(smem.vGetBOOLData(TC_Actuate_By_TFD) == 1 && _exec_segment._ptr_seg_exec_time[i]._cadc_seg == 0xEEEE) {
                              //  printf("25\n");     //false
                                PlanUpdate = true;
                                smem.vSetBOOLData(TC_CCTActuate_TOD_Running, true);
                              } else if(smem.vGetBOOLData(TC_CCT_In_LongTanu_ActuateType_Switch) == 1) {
                              //  printf("26\n");    //true
                                PlanUpdate = true;
                                smem.vSetBOOLData(TC_CCTActuate_TOD_Running, true);
                              } else if(smem.vGetThisCycleRunCCJPlan5F18() == true) {
                              //  printf("27\n");     //true
                              } else {
                               // printf("28\n");     //false
                                printf("_exec_segment._ptr_seg_exec_time[%d]._planid = smem.vGetUSIData(TC_CCT_In_LongTanu_ActuateType_Switch)\n",i);
                                _exec_segment._ptr_seg_exec_time[i]._planid = smem.vGetUSIData(TC_CCT_In_LongTanu_ActuateType_Switch);
                                PlanUpdate = true;
                              }
                        }

                //        }
                */
                /*
                //Special Actuate chou check 2013/01/25
                //************************************************
                        bool bTmp1;
                        bool bTmp2;
                        bool bTmp3;
                        //printf("29\n");
                        bTmp1 = smem.vGetBOOLData(TC_CCT_In_LongTanu_ActuateType_FunctionEnable);   //是否為觸動模式
                        bTmp2 = smem.vGetBOOLData(TC_CCT_In_LongTanu_ActuateType_Switch);
                        bTmp3 = smem.vGetBOOLData(TC_Actuate_By_TFD);  //影像偵測卡
                        printf("\nsmem.vGetBOOLData(TC_CCT_In_LongTanu_ActuateType_FunctionEnable): %d\n", bTmp1);
                        printf("Switch = %d\nTC_Actuate_By_TFD = %d\n",smem.vGetBOOLData(TC_CCT_In_LongTanu_ActuateType_Switch),smem.vGetBOOLData(TC_Actuate_By_TFD));
                        if(bTmp1 == true) {
                        //  printf("30\n");   //true
                          if( bTmp2 == true || bTmp3 == true ) {    //暫時沒用
                            // donothing
                           // printf("31\n"); //true
                              PlanUpdate = true;
                              smem.vSetBOOLData(TC_CCTActuate_TOD_Running, true);
                              printf("_exec_segment._ptr_seg_exec_time[i]._planid: %d\n" ,_exec_segment._ptr_seg_exec_time[i]._planid);
                              printf("_exec_segment[i]: %d\n" ,i);
                //OTTEST              _exec_plan = plan[_exec_segment._ptr_seg_exec_time[i]._planid];
                             // printf("\nFlag111\n");
                          } else {
                           // printf("32\n");     //true
                            if((smem.vGetThisCycleRunCCJPlan5F18() == true)){
                            _exec_segment._ptr_seg_exec_time[i]._planid = 0x28; //觸動專用PLAN
                            }else{              //沒觸動5f18則繼續原來的plan chou fix 2013/01/30
                            _exec_segment._ptr_seg_exec_time[i]._planid = smem.vGetUSIData(TC_CCT_In_LongTanu_ActuateType_Switch);
                            }
                            //printf("smem.vGetUSIData(TC_CCT_In_LongTanu_ActuateType_Switch) = %d\n",smem.vGetUSIData(TC_CCT_In_LongTanu_ActuateType_Switch));
                            PlanUpdate = true;
                            //read tod
                            //  printf("\nFlag222\n");
                          }
                        }
                //***************************************************************
                */
//Manule Change
//OT980406
                if( (_exec_phase._phase_order == 0xB0 && _current_strategy==STRATEGY_MANUAL)  ||
                        (_exec_phase._phase_order == 0x80 && _current_strategy==STRATEGY_MANUAL)  )
                {
//          ucTmp = smem.vGetUCData(TC_KeyPadP6Value);
                    printf("33\n");     //false
//change to default plan no.48
                    if(plan[48]._planid == 48 && plan[48]._cycle_time >= 10)
                    {
                        //ChengePlan
                        printf("34\n");     //false
                        _exec_segment._ptr_seg_exec_time[i]._planid = 48;
                        _exec_plan = plan[48];
                        _exec_BRTPriority_plan = BRTplan[48];  //jacky20140514
                        PlanUpdate = true;
                        SegmentTypeUpdate = true;
                    }
                }

                if( _current_strategy==STRATEGY_MANUAL || _current_strategy==STRATEGY_ALLDYNAMIC)
                {
                    //OTFIX 961220
                    if(_exec_plan._planid == 48)
                    {
                        printf("run plan 48\n");
//            _exec_plan = plan[_exec_segment._ptr_seg_exec_time[i]._planid];
                    }
                }
                else if( _current_strategy==STRATEGY_TOD
                         && (_exec_segment._ptr_seg_exec_time[i]._cadc_seg==0x0000
                             || _exec_segment._ptr_seg_exec_time[i]._cadc_seg==0xEEEE || _exec_segment._ptr_seg_exec_time[i]._cadc_seg==0xDDDD) )
                {
                    //OT Debug 951121
                    printf("OT_exec_segment._ptr_seg_exec_time[%d]._plan: %d\n", i, _exec_segment._ptr_seg_exec_time[i]._planid);
                    if(false == smem.vGetBOOLData(TCDynSegSwitch))
                    {
                        //CCJ20150812
                        if(_exec_plan._planid!=_exec_segment._ptr_seg_exec_time[i]._planid || PlanUpdate == true)
                        {
                            //OTBUG =1
                            if(plan[_exec_segment._ptr_seg_exec_time[i]._planid]._ptr_subplaninfo)
                            {
                                _exec_plan = plan[_exec_segment._ptr_seg_exec_time[i]._planid];
                                _exec_BRTPriority_plan = BRTplan[_exec_segment._ptr_seg_exec_time[i]._planid];  //jacky20140514
                            }
                            else
                            {
                                _exec_plan = plan[DEFAULT_PLANID];
                                _exec_BRTPriority_plan = BRTplan[DEFAULT_PLANID];  //jacky20140514
                            }
                            PlanUpdate = false;  //OTBUG = 1
                        }
                    }
                    else
                    {
                        printf("in DynSeg Status\n");//CCJ20150812 fixed dynseg location
                    }
                }
                else if( _current_strategy==STRATEGY_TOD && _exec_segment._ptr_seg_exec_time[i]._cadc_seg==0xFFFF )
                {
                    //only occured at the beginning of AUTO_CADC segment
                    _current_strategy=STRATEGY_AUTO_CADC;
                    if(plan[_exec_segment._ptr_seg_exec_time[i]._planid]._ptr_subplaninfo) plan[CADC_PLANID] = plan[_exec_segment._ptr_seg_exec_time[i]._planid];
                    else plan[CADC_PLANID] = plan[DEFAULT_PLANID];
                    plan[CADC_PLANID]._planid = CADC_PLANID;  //because of the operator= changes planid
                    _exec_plan = plan[CADC_PLANID];
                    _exec_BRTPriority_plan = BRTplan[CADC_PLANID];  //jacky20140514
                    /*+++++++++++++++++*/
                    //          keypad.keypadPort.doDisplayLcdWorkByData_P3();
                    /*-----------------*/
                }
                else if( _current_strategy==STRATEGY_AUTO_CADC && _exec_segment._ptr_seg_exec_time[i]._cadc_seg==0x0000 )
                {
                    //only occured at the ending of AUTO_CADC segment
                    _current_strategy=STRATEGY_TOD;
                    if(plan[_exec_segment._ptr_seg_exec_time[i]._planid]._ptr_subplaninfo)
                    {
                        _exec_plan = plan[_exec_segment._ptr_seg_exec_time[i]._planid];
                        _exec_BRTPriority_plan = BRTplan[_exec_segment._ptr_seg_exec_time[i]._planid];  //jacky20140514
                    }
                    else
                    {
                        _exec_plan = plan[DEFAULT_PLANID];
                        _exec_BRTPriority_plan = BRTplan[DEFAULT_PLANID];  //jacky20140514
                    }
                    /*+++++++++++++++++*/
//          keypad.keypadPort.doDisplayLcdWorkByData_P3();
                    /*-----------------*/
                }
                else if( _current_strategy==STRATEGY_AUTO_CADC && _exec_segment._ptr_seg_exec_time[i]._cadc_seg==0xFFFF )
                {
                    //occured during the AUTO_CADC segment
                    _exec_plan = plan[CADC_PLANID];
                    _exec_BRTPriority_plan = BRTplan[CADC_PLANID];  //jacky20140514
                }
                else if( _current_strategy==STRATEGY_CADC )
                {
                    if( _exec_plan._planid!=CADC_PLANID )
                    {
                        //meaning that the begining of CADC mode (first time change to plan[CADC]) or plan[CADC] been modified
                        if(plan[_exec_segment._ptr_seg_exec_time[i]._planid]._ptr_subplaninfo)
                            plan[CADC_PLANID] = plan[_exec_segment._ptr_seg_exec_time[i]._planid];

                        else
                            plan[CADC_PLANID] = plan[DEFAULT_PLANID];

                        plan[CADC_PLANID]._planid = CADC_PLANID;  //because of the operator= changes planid
                        _exec_plan = plan[CADC_PLANID];
                        _exec_BRTPriority_plan = BRTplan[CADC_PLANID];  //jacky20140514
                    }
                    else
                        _exec_plan = plan[CADC_PLANID];

                    _exec_BRTPriority_plan = BRTplan[CADC_PLANID];  //jacky20140514
                }
                else
                {
                    //OT20110517
                    printf("fail!! unknow status!!!\n");
                    _exec_plan = plan[_exec_segment._ptr_seg_exec_time[i]._planid];
                    _exec_BRTPriority_plan = BRTplan[_exec_segment._ptr_seg_exec_time[i]._planid];  //jacky20140514
                }
                BF02_PriorityParameters();    //jacky20140520  BF02
                smem.Set_exeBF13(_exec_BRTPriority_plan);   //jacky20140521
                if(smem.GetLearningPGPRAcrossCycle())
                {
                    smem.SetLearningPGPRsubphase(true); //jacky20141202
                }
                else
                    smem.SetLearningPGPRsubphase(false);

                pthread_mutex_unlock(&CPlanInfo::_plan_mutex);
//----------------------------------------------------------------------------------------------------
                _exec_segment_current_seg_no = iExecSeg;  //BUG!?
//      _exec_segment_current_seg_no=i;
//OT Debug 951121
//        printf("_exec_segment._segment_count=%d.\n", _exec_segment._segment_count);
//        printf("_exec_segment_current_seg_no=%d.\n", _exec_segment_current_seg_no);
                break;
            }//end if(_exec_segment_current_seg_no)
        }//end for(segment_count)

        //OT20140415 start
        /*
            smem.vSetBOOLData(TCDynSegStatus, false);
            bool bDynSegSwitch = smem.vGetBOOLData(TCDynSegSwitch);
            if(bDynSegSwitch) {
              if(iDynSeg_SegType != _exec_segment._segment_type) {
            	 // lock mutex
                pthread_mutex_lock(&CSegmentInfo::_segment_mutex);
                if(segment[iDynSeg_SegType]._ptr_seg_exec_time)
                  _exec_segment = segment[iDynSeg_SegType];
                else _exec_segment = segment[DEFAULT_SEGMENTTYPE];
                // unlock mutex
                pthread_mutex_unlock(&CSegmentInfo::_segment_mutex);
                smem.vSetBOOLData(TCDynSegStatus, true);
              }
              if(iDynSeg_PlanID != _exec_plan._planid) {
                // lock mutex
                pthread_mutex_lock(&CPlanInfo::_plan_mutex);
                if(plan[iDynSeg_PlanID]._ptr_subplaninfo) {
                  _exec_plan = plan[iDynSeg_PlanID];
                } else { _exec_plan = plan[DEFAULT_PLANID]; }
                // unlock mutex
                pthread_mutex_unlock(&CPlanInfo::_plan_mutex);
                smem.vSetBOOLData(TCDynSegStatus, true);
              }
            }
            //OT20140415 end
        */
        //***********************************
        //****    re-define _exec_phase
        if( (_exec_phase._phase_order!=_exec_plan._phase_order) ||(smem.vGetINTData(TC92_iUpdatePhaseData) == 1) )
        {
            printf("35\n");    //do
            /******** lock mutex ********/
            pthread_mutex_lock(&CPhaseInfo::_phase_mutex);
            if(phase[_exec_plan._phase_order]._ptr_subphase_step_count)
                _exec_phase = phase[_exec_plan._phase_order];

            else
                _exec_phase = phase[_default_phaseorder];

            smem.vSetINTData(TC92_iUpdatePhaseData, 0);
            /******** unlock mutex ********/
            pthread_mutex_unlock(&CPhaseInfo::_phase_mutex);
        }
        /*otaru0514++*/
        else
        {
            if(_exec_plan._phase_order == FLASH_PHASEORDER)
            {
                //when phase still in flash, not update light status
                bRefreshLight = false;
            }
        }

        if( _current_strategy != STRATEGY_FLASH || _current_strategy != STRATEGY_ALLRED)
        {
            if( _exec_phase._subphase_count != _exec_plan._subphase_count )
            {
                //ï¿½pï¿½Gphaseï¿½Pplanï¿½wï¿½qï¿½ï¿½ï¿½ï¿½Oï¿\uFFFDVï¿½Æ¤ï¿½ï¿½Pï¿½ï¿½
                smem.vSetBOOLData(TC92_SubPhaseOfPhasePlanIncorrent, true);
                _exec_plan = plan[FLASH_PLANID];
            }
            else
            {
                if( smem.vGetBOOLData(TC92_SubPhaseOfPhasePlanIncorrent) == true )
                {
                    smem.vSetBOOLData(TC92_SubPhaseOfPhasePlanIncorrent, false);
                }
            }
        }

        printf("STRATEGY:%2d SEGMENTTYPE:%2d SEGMENT_NO:%2d PLANID:%2d PHASE:%#X OFFSET:%3d\n"
               , _current_strategy, _exec_segment._segment_type, (_exec_segment_current_seg_no+1)
               , _exec_plan._planid, _exec_phase._phase_order, _exec_plan._offset);

        if( smem.vGetBOOLData(TC_CCT_In_LongTanu_ActuateType_Switch))
        {
            //close ActuateType
            printf("36\n");       //falsh
            smem.vSetBOOLData(TC_CCT_In_LongTanu_ActuateType_Switch, false);
            SegmentTypeUpdate = true;
            PlanUpdate = true;
        }
        if (smem.vGetBOOLData(TC_Actuate_By_TFD))
        {
            //close ActuateType
            printf("37\n");
            smem.vSetBOOLData(TC_Actuate_By_TFD, false);
            SegmentTypeUpdate = true;
            PlanUpdate = true;
        }
    }
    catch (...) {}

}

//OT20140415
//----------------------------------------------------------
void CSTC::Lock_to_Determine_SegmentPlanPhase_DynSeg(void)
{
    try
    {

        printf("Lock_to_Determine_SegmentPlanPhase\n");

        static time_t now;
        static struct tm* currenttime;
        int timeOffset;

        now = time(NULL);

        currenttime = localtime(&now);
//OT20140520
        unsigned short int usiAdjRemainSec = smem.vGetUSIData(TCDynSegRemainSec);
        if(usiAdjRemainSec > 0)
        {
            unsigned char adjHour = smem.vGetUCData(TCDynSegAdjustHour);
            unsigned char adjMin = smem.vGetUCData(TCDynSegAdjustMin);
            currenttime->tm_hour = adjHour;
            currenttime->tm_min = adjMin;
            currenttime->tm_sec = 0;
        }

        printf("Offset Date: %4d/%2d/%2d  Offset Time: %2d:%2d:%2d\n"
               , currenttime->tm_year+1900,currenttime->tm_mon+1,currenttime->tm_mday
               , currenttime->tm_hour, currenttime->tm_min, currenttime->tm_sec);


        //***********************************
        //****   check if in holidayseg
        bool _exec_segment_is_holiday=false;
        for(int i=0; i<AMOUNT_HOLIDAY_SEG; i++)
        {
            if(    (    ( (currenttime->tm_year+1900) > holidayseg[i]._start_year )
                        || ( (currenttime->tm_year+1900)== holidayseg[i]._start_year && (currenttime->tm_mon+1) > holidayseg[i]._start_month )
                        || ( (currenttime->tm_year+1900)== holidayseg[i]._start_year && (currenttime->tm_mon+1)== holidayseg[i]._start_month && (currenttime->tm_mday)>= holidayseg[i]._start_day ) )
                    && (    ( (currenttime->tm_year+1900) < holidayseg[i]._end_year )
                            || ( (currenttime->tm_year+1900)== holidayseg[i]._end_year   && (currenttime->tm_mon+1) < holidayseg[i]._end_month )
                            || ( (currenttime->tm_year+1900)== holidayseg[i]._end_year   && (currenttime->tm_mon+1)== holidayseg[i]._end_month   && (currenttime->tm_mday)<= holidayseg[i]._end_day ) ) )
            {
                //iDynSeg_SegType = segment[holidayseg[i]._segment_type]._segment_type;
                iDynSeg_SegType = holidayseg[i]._segment_type;
                _exec_segment_is_holiday=true;
                printf("Today is in Holiday Segment:%d\n",i);
                break;
            }
        }
        //***********************************
        //****  check in which weekdayseg
        if(_exec_segment_is_holiday==false)
        {
            if(    ((((((currenttime->tm_mday-1)/7)+1)%2)==0 ) && (currenttime->tm_wday>=((currenttime->tm_mday-1)%7)))
                    || ((((((currenttime->tm_mday-1)/7)+1)%2)> 0 ) && (currenttime->tm_wday< ((currenttime->tm_mday-1)%7))) )
            {

                printf("This week is an even week\n");

                int wday = (currenttime->tm_wday==0)?13:(currenttime->tm_wday+6);
                iDynSeg_SegType = weekdayseg[wday]._segment_type;
            }//end if(even week)

            else   //odd week
            {

                printf("This week is an odd week\n");

                int wday = (currenttime->tm_wday==0)?6:(currenttime->tm_wday-1);
                iDynSeg_SegType = weekdayseg[wday]._segment_type;
            }//end else(odd week)
        }//end else(holiday)


        //***********************************
        //**** check and modify _exec_plan
        for(int i=0; i< segment[iDynSeg_SegType]._segment_count; i++)
        {
            iDynSeg_SegCount = i;
            if(     ( i == (segment[iDynSeg_SegType]._segment_count-1) )  //last segment
                    ||(  ( i <  (segment[iDynSeg_SegType]._segment_count-1) )  //not last segment
                         &&(    (    ( currenttime->tm_hour > segment[iDynSeg_SegType]._ptr_seg_exec_time[i]._hour )
                                     || ( currenttime->tm_hour ==segment[iDynSeg_SegType]._ptr_seg_exec_time[i]._hour    && currenttime->tm_min >=segment[iDynSeg_SegType]._ptr_seg_exec_time[i]._minute  ) )
                                && (    ( currenttime->tm_hour  <segment[iDynSeg_SegType]._ptr_seg_exec_time[i+1]._hour)
                                        || ( currenttime->tm_hour ==segment[iDynSeg_SegType]._ptr_seg_exec_time[i+1]._hour  && currenttime->tm_min  <segment[iDynSeg_SegType]._ptr_seg_exec_time[i+1]._minute) ) ) ) )
            {
                iDynSeg_SegCount = i;
                iDynSeg_PlanID = segment[iDynSeg_SegType]._ptr_seg_exec_time[i]._planid;
                break;
            }
        }

    }
    catch (...) {}

}



//----------------------------------------------------------
//**********************************************************
//****       Calculate Compensation of _exec_plan
//**********************************************************
//----------------------------------------------------------
unsigned short int CSTC::CalculateCompensationBase(void)
{
    try
    {

        bool SET_COMPENSATE_BASE_TO_SEGMENT = true;                                  //true:  set compensation caculated base from start of current segment (tainan)
        //false: set compensation caculated base from 00:00 (hsinchu)

        offset_not_been_adjusted_by_CADC = 0;

        static time_t now;
        static struct tm* currenttime;

        now = time(NULL);
        currenttime = localtime(&now);

        printf("    Current Time: %2d:%2d:%2d\n", currenttime->tm_hour, currenttime->tm_min, currenttime->tm_sec);


        unsigned short int compensation_base_hour=0;
        unsigned short int compensation_base_minute=0;
        unsigned short int compensation_base_second=0;

        //OT Change 951114
        // if(MACHINELOCATE == MACHINELOCATEATHSINCHU) {
        //OT20111128
        if(smem.vGetUCData(TC_CCT_MachineLocation) == MACHINELOCATEATHSINCHU)
        {
            SET_COMPENSATE_BASE_TO_SEGMENT = false;
        }

        if(SET_COMPENSATE_BASE_TO_SEGMENT)
        {
            for(int i=0; i<_exec_segment._segment_count; i++)
            {
                if( i < (_exec_segment._segment_count-1) )  //if(not last segment)
                {
                    if(    (    ( currenttime->tm_hour >  _exec_segment._ptr_seg_exec_time[i]._hour )
                                || ( currenttime->tm_hour == _exec_segment._ptr_seg_exec_time[i]._hour    && currenttime->tm_min >=_exec_segment._ptr_seg_exec_time[i]._minute  ) )
                            && (    ( currenttime->tm_hour  < _exec_segment._ptr_seg_exec_time[i+1]._hour)
                                    || ( currenttime->tm_hour == _exec_segment._ptr_seg_exec_time[i+1]._hour  && currenttime->tm_min  <_exec_segment._ptr_seg_exec_time[i+1]._minute) ) )
                    {
                        compensation_base_hour   = _exec_segment._ptr_seg_exec_time[i]._hour;
                        compensation_base_minute = _exec_segment._ptr_seg_exec_time[i]._minute;
                        break;
                    }
                } //end if(not last segment)
                else  //else(last segment)
                {
                    compensation_base_hour   = _exec_segment._ptr_seg_exec_time[i]._hour;
                    compensation_base_minute = _exec_segment._ptr_seg_exec_time[i]._minute;
                }
            }
        }

        compensation_base_second += _exec_plan._offset;
        if(compensation_base_second >= 60)
        {
            compensation_base_minute += (compensation_base_second/60);
            compensation_base_second %= 60;
            if(compensation_base_minute >= 60)
            {
                compensation_base_hour = ( (compensation_base_hour+(compensation_base_minute/60))%24 );
                compensation_base_minute %= 60;
            }
        }

//OT Debug compensation 950814
//OT Debug compensation 951109
        long remainder
        = (  (currenttime->tm_hour*3600   + currenttime->tm_min*60      + currenttime->tm_sec)
             - (compensation_base_hour*3600 + compensation_base_minute*60 + compensation_base_second) );
        if(remainder < 0)  return 0;   // no compensation

        printf("    compensation_base_hour remainder=%d\n", remainder);
        remainder = remainder % _exec_plan.calculated_cycle_time();

        printf("    Base Time:    %2d:%2d:%2d  cycle_time:%3d  remainder:%3d\n"
               , compensation_base_hour, compensation_base_minute, compensation_base_second
               , _exec_plan.calculated_cycle_time(), remainder);


        unsigned short int total_compensation=0;
        if(remainder>=(_exec_plan.calculated_cycle_time()/2))
        {
            _exec_plan._shorten_cycle=false;  //extend the cycle_tiem
            total_compensation = _exec_plan.calculated_cycle_time() - remainder;

            printf("    Extending the next cycle via %3d(s)\n", total_compensation);

        }
        else
        {
            _exec_plan._shorten_cycle=true;  //shorten the cycle_time
            total_compensation = remainder;

            printf("    Shortening the next cycle via %3d(s)\n", total_compensation);

        }
        return total_compensation;
    }
    catch(...) {}
}
//----------------jacky20140407-------------------------------------------------
unsigned short int CSTC::CalculateCompensationBase_twocycle(void)
{
    try
    {

        bool SET_COMPENSATE_BASE_TO_SEGMENT = true;                                  //true:  set compensation caculated base from start of current segment (tainan)
        //false: set compensation caculated base from 00:00 (hsinchu)

        offset_not_been_adjusted_by_CADC = 0;

        static time_t now;
        static struct tm* currenttime;

        now = time(NULL);
        currenttime = localtime(&now);

        printf("    Current Time: %2d:%2d:%2d\n", currenttime->tm_hour, currenttime->tm_min, currenttime->tm_sec);


        unsigned short int compensation_base_hour=0;
        unsigned short int compensation_base_minute=0;
        unsigned short int compensation_base_second=0;

        //OT Change 951114
        // if(MACHINELOCATE == MACHINELOCATEATHSINCHU) {
        //OT20111128
        if(smem.vGetUCData(TC_CCT_MachineLocation) == MACHINELOCATEATHSINCHU)
        {
            SET_COMPENSATE_BASE_TO_SEGMENT = false;
        }

        if(SET_COMPENSATE_BASE_TO_SEGMENT)
        {
            for(int i=0; i<_exec_segment._segment_count; i++)
            {
                if( i < (_exec_segment._segment_count-1) )  //if(not last segment)
                {
                    if(    (    ( currenttime->tm_hour >  _exec_segment._ptr_seg_exec_time[i]._hour )
                                || ( currenttime->tm_hour == _exec_segment._ptr_seg_exec_time[i]._hour    && currenttime->tm_min >=_exec_segment._ptr_seg_exec_time[i]._minute  ) )
                            && (    ( currenttime->tm_hour  < _exec_segment._ptr_seg_exec_time[i+1]._hour)
                                    || ( currenttime->tm_hour == _exec_segment._ptr_seg_exec_time[i+1]._hour  && currenttime->tm_min  <_exec_segment._ptr_seg_exec_time[i+1]._minute) ) )
                    {
                        compensation_base_hour   = _exec_segment._ptr_seg_exec_time[i]._hour;
                        compensation_base_minute = _exec_segment._ptr_seg_exec_time[i]._minute;
                        break;
                    }
                } //end if(not last segment)
                else  //else(last segment)
                {
                    compensation_base_hour   = _exec_segment._ptr_seg_exec_time[i]._hour;
                    compensation_base_minute = _exec_segment._ptr_seg_exec_time[i]._minute;
                }
            }
        }

        compensation_base_second += _exec_plan._offset;
        if(compensation_base_second >= 60)
        {
            compensation_base_minute += (compensation_base_second/60);
            compensation_base_second %= 60;
            if(compensation_base_minute >= 60)
            {
                compensation_base_hour = ( (compensation_base_hour+(compensation_base_minute/60))%24 );
                compensation_base_minute %= 60;
            }
        }

//OT Debug compensation 950814
//OT Debug compensation 951109
        long remainder
        = (  (currenttime->tm_hour*3600   + currenttime->tm_min*60      + currenttime->tm_sec)
             - (compensation_base_hour*3600 + compensation_base_minute*60 + compensation_base_second) );
        if(remainder < 0)  return 0;   // no compensation

        printf("    compensation_base_hour remainder=%d\n", remainder);
        remainder = remainder % _exec_plan.calculated_cycle_time();

        // printf("\n\n----------------------------**************----------------------------------------\n\n");
        printf("    Base Time:    %2d:%2d:%2d  cycle_time:%3d  remainder:%3d\n"
               , compensation_base_hour, compensation_base_minute, compensation_base_second
               , _exec_plan.calculated_cycle_time(), remainder);
        // printf("\n----------------------------**************----------------------------------------\n\n");

        unsigned short int total_compensation=0;
        if(remainder > (_exec_plan.calculated_cycle_time()*0.25)+1 && remainder < (_exec_plan.calculated_cycle_time()*0.75)-1)
        {
            unsigned short int remainder_twocycle = remainder - (_exec_plan.calculated_cycle_time()*0.25);
            if(remainder_twocycle >= (_exec_plan.calculated_cycle_time()/4))
            {
                _exec_plan._shorten_cycle=false;  //extend the cycle_tiem
                total_compensation = (_exec_plan.calculated_cycle_time()/2) - remainder_twocycle;
                printf("    Extending the next twocycle via %3d(s)\n", total_compensation);
            }
            else
            {
                _exec_plan._shorten_cycle=true;  //shorten the cycle_time
                total_compensation = remainder_twocycle;
                printf("    Shortening the next twocycle via %3d(s)\n", total_compensation);
            }
        }
        else
        {
            if(remainder>=(_exec_plan.calculated_cycle_time()/2))
            {
                _exec_plan._shorten_cycle=false;  //extend the cycle_tiem
                total_compensation = _exec_plan.calculated_cycle_time() - remainder;
                printf("    Extending the next cycle via %3d(s)\n", total_compensation);
            }
            else
            {
                _exec_plan._shorten_cycle=true;  //shorten the cycle_time
                total_compensation = remainder;
                printf("    Shortening the next cycle via %3d(s)\n", total_compensation);
            }
        }

        return total_compensation;
    }
    catch(...) {}
}

//----------------------------------------------------------
//**********************************************************
//****       Calculate Compensation in Chain
//**********************************************************
//----------------------------------------------------------
unsigned short int CSTC::CalculateCompensationBaseInChain(void)
{
    try
    {
        unsigned short int total_compensation=0;
        int iOriCycle;
        sChildChain sCCTMP;
        int iTmp;
        bool bCycleShort = false;

        int StartOffset;

        iOriCycle = _exec_plan.calculated_cycle_time();

        static time_t now;
        static struct tm* currenttime;
        static struct tm* basetime;

        unsigned short int now_hour=0;
        unsigned short int now_minute=0;
        unsigned short int now_second=0;
        unsigned short int compensation_base_hour=0;
        unsigned short int compensation_base_minute=0;
        unsigned short int compensation_base_second=0;

        int iSSP, iESP;

        StartOffset = smem.vGetChainOffset(1, 0);

        now = time(NULL);
        currenttime = localtime(&now);

        now_hour = currenttime->tm_hour;
        now_minute = currenttime->tm_min;
        now_second = currenttime->tm_sec;

        sCCTMP = smem.vGetChildChainStruct();
        sCCTMP.oldStartTime += StartOffset;

        iSSP = smem.vGetUCData(TC92_5F32StartSubPhaseId);
        iESP = smem.vGetUCData(TC92_5F32EndSubPhaseId);

        if(iSSP>iESP) sCCTMP.oldStartTime += sCCTMP.iStartKeepTime;

        basetime = localtime(&sCCTMP.oldStartTime);
        compensation_base_hour = basetime->tm_hour;
        compensation_base_minute = basetime->tm_min;
        compensation_base_second = basetime->tm_sec;

        printf("    Current Time: %2d:%2d:%2d\n ChainBase Time(oldStartTime): %d:%d:%d",
               now_hour, now_minute, now_second,
               compensation_base_hour, compensation_base_minute, compensation_base_second);

        long remainder
        = (  (now_hour*3600   + now_minute*60      + now_second)
             - (compensation_base_hour*3600 + compensation_base_minute*60 + compensation_base_second) );
        if(remainder < 0) return 0;                                                 // no compensation
        printf("    compensation_base_hour remainder=%d\n", remainder);
        remainder = remainder % sCCTMP.iChainCycle;

//cycle sync.
        if(iOriCycle > sCCTMP.iChainCycle)
        {
            iTmp = iOriCycle - sCCTMP.iChainCycle;                                    //short
            bCycleShort = true;
        }
        else
        {
            iTmp = sCCTMP.iChainCycle - iOriCycle;                                    //extern
            bCycleShort = false;
        }
        if(remainder>=(sCCTMP.iChainCycle/2))
        {
            _exec_plan._shorten_cycle=false;  //extend the cycle_tiem
            total_compensation = sCCTMP.iChainCycle - remainder;
            printf("    Extending the next cycle via %3d(s)\n", total_compensation);

            /*
                  //pass 2
                  if(bCycleShort == false) {                                                //alse extern
                    total_compensation += iTmp;
                  } else {                                                                  // fuck station, --
                    if(total_compensation > iTmp) {
                      total_compensation = total_compensation - iTmp;
                      printf("P2  Extending the next cycle via %3d(s)\n", total_compensation);
                    }
                    else {                                                                  // change to short
                      total_compensation = iTmp - total_compensation;
                      _exec_plan._shorten_cycle=true;
                      printf("P2  Shortening the next cycle via %3d(s)\n", total_compensation);
                    }
                  }
            */

        }
        else
        {
            _exec_plan._shorten_cycle=true;  //shorten the cycle_time
            total_compensation = remainder;

            printf("    Shortening the next cycle via %3d(s)\n", total_compensation);

            /*
                  //pass 2
                  if(bCycleShort == true) {                                                 //also short
                    total_compensation += iTmp;
                  } else {                                                                  // fuck station, --
                    if(total_compensation > iTmp) {
                      total_compensation = total_compensation - iTmp;
                      printf("P2  Shortening the next cycle via %3d(s)\n", total_compensation);
                    }
                    else {                                                                  // change to short
                      total_compensation = iTmp - total_compensation;
                      _exec_plan._shorten_cycle=false;
                      printf("P2  Extending the next cycle via %3d(s)\n", total_compensation);
                    }
                  }
            */

        }



        return total_compensation;
    }
    catch(...) {}
}



//----------------------------------------------------------
void CSTC::CalculateCompensation_in_TOD(void)
{
    try
    {
        DATA_Bit _ControlStrategy;

        _ControlStrategy.DBit = smem.vGetUCData(TC92_ucControlStrategy);

        printf("in CalculateCompensation_in_TOD, _current_strategy:%x\n", _ControlStrategy.DBit);

        if( _ControlStrategy.switchBit.b4 == true )
        {
            return;
        }

        //OT1000218, may be need compensation
        if( _ControlStrategy.DBit == 0x30)
        {
            return;
        }

        //OT20111107
        if(smem.vGetThisCycleRunCCJPlan5F18() == true)
        {
            return;
        }

//OT1000310  if(smem.vGet5F18EffectTime() > 0) {
//OT1000310    return;
//OT1000310  }
        for(int j=0; j<_exec_plan._subphase_count; j++)
        {
            _exec_plan._ptr_subplaninfo[j]._green_compensation = 0;
        }

        if( _exec_plan._planid==FLASH_PLANID || _exec_plan._planid==ALLRED_PLANID || _current_strategy==STRATEGY_MANUAL || _current_strategy==STRATEGY_ALLDYNAMIC) return;
        if( smem.vGetBOOLData(TC_CCTActuate_TOD_Running) == true ) return;
        printf("\n    CalculateCompensation_in_TOD\n");
        int iActualCompensationValue = 0;
        unsigned short int total_compensation;
        //jacky20140411
        if(smem.GetSegmentChange() == true)
        {
            if(smem.GetCompensationcycle()==2)
            {
                total_compensation = CalculateCompensationBase_twocycle();  //賠償TIME(two cycle end)
            }
            else
                total_compensation = CalculateCompensationBase();   //賠償TIME

            printf("\n***************** smem.GetSegmentChange() == true *****************\n");
            printf("\n***************** smem.GetSegmentChange() == true *****************\n");
            smem.SetBRTRemainCompensation(0);
        }
        else if(smem.GetPriorityswitch() == true)
        {
            //_exec_plan._shorten_cycle =false;
            //total_compensation = smem.vGetBRTCompensationSec() + smem.GetBRTRemainCompensation();
            total_compensation = CalculateCompensationBase();
            smem.vSetBRTCompensationSec(0);
            printf("/n!!!!!!!!!!!!!!!! total_compensation : %d !!!!!!!!!!",total_compensation);
            printf("/n!!!!!!!!!!!!!!!! total_compensation : %d !!!!!!!!!!",total_compensation);
        }
        else
        {
            if(smem.GetCompensationcycle()==2)
            {
                total_compensation = CalculateCompensationBase_twocycle();  //賠償TIME(two cycle end)
            }
            else
                total_compensation = CalculateCompensationBase();   //賠償TIME
            printf("\n~~~~~~~~~~~~~~~~~ smem.GetSegmentChange() == false ~~~~~~~~~~~~~~~~~");
            printf("\n~~~~~~~~~~~~~~~~~ smem.GetSegmentChange() == false ~~~~~~~~~~~~~~~~~\n");
//  return;
        }

        unsigned short int divisor=0;
        unsigned short int TmpMinCompensation = 0;
        unsigned short int TmpMaxCompensation = 0;
        if(smem.cPedPushButton.GetCompensationAct()>=1) //arwen ++
        {
            for(int j=0; j<_exec_plan._subphase_count; j++)
            {
                // if(smem.vGetVDPhaseTriggerSwitch(j)<=0){
                divisor += _exec_plan._ptr_subplaninfo[j]._green;
                // }
            }

            for(int j=0; j<_exec_plan._subphase_count; j++)
            {
                // if(smem.vGetVDPhaseTriggerSwitch(j)<=0){
                _exec_plan._ptr_subplaninfo[j]._green_compensation = (total_compensation*_exec_plan._ptr_subplaninfo[j]._green)/divisor;
                printf("\n\n\ntotal_compensation*_exec_plan._ptr_subplaninfo[j]._green = %d \ndivisor= %d\ngreen_compensation = %d\n\n\n",
                       total_compensation*_exec_plan._ptr_subplaninfo[j]._green,divisor,_exec_plan._ptr_subplaninfo[j]._green_compensation);

                //jacky
                int iMin = _exec_plan._ptr_subplaninfo[j]._min_green;
                int iMAX = _exec_plan._ptr_subplaninfo[j]._max_green;
                if(_exec_plan._shorten_cycle)
                {
                    int tmpgreen = _exec_plan._ptr_subplaninfo[j]._green-_exec_plan._ptr_subplaninfo[j]._green_compensation;
                    if(tmpgreen < iMin && iMin < smem.GetCompensationMingreen())
                        _exec_plan._ptr_subplaninfo[j]._green_compensation = _exec_plan._ptr_subplaninfo[j]._green - smem.GetCompensationMingreen();
                    else if (tmpgreen < iMin)
                        _exec_plan._ptr_subplaninfo[j]._green_compensation = _exec_plan._ptr_subplaninfo[j]._green - iMin;
                }
                else
                {
                    int tmpmaxgreen = _exec_plan._ptr_subplaninfo[j]._green + _exec_plan._ptr_subplaninfo[j]._green_compensation;
                    if(iMAX <= 0) {}
                    else if(tmpmaxgreen > iMAX)
                        _exec_plan._ptr_subplaninfo[j]._green_compensation = iMAX - _exec_plan._ptr_subplaninfo[j]._green;
                }

                _exec_plan._ptr_subplaninfo[j]._yellow_compensation         = 0;
                _exec_plan._ptr_subplaninfo[j]._allred_compensation         = 0;
                _exec_plan._ptr_subplaninfo[j]._pedgreen_flash_compensation = 0;
                _exec_plan._ptr_subplaninfo[j]._pedred_compensation         = 0;
                iActualCompensationValue += _exec_plan._ptr_subplaninfo[j]._green_compensation;
                //}
            }
        }
        else
        {
            if((smem.GetPriorityswitch() == true) && (smem.GetSegmentChange() == false))    //for BRT
            {
                for(int j=1; j<_exec_plan._subphase_count; j++)
                    divisor += _exec_plan._ptr_subplaninfo[j]._green;

                for(int j=0; j<_exec_plan._subphase_count; j++)
                {
                    if(_exec_plan._shorten_cycle == false)
                    {
                        int iMAX = _exec_plan._ptr_subplaninfo[j]._max_green;
                        if(j == 0)   //step 1
                        {
                            int tmpmaxgreen = _exec_plan._ptr_subplaninfo[j]._green + total_compensation;
                            if(iMAX <= 0)
                                _exec_plan._ptr_subplaninfo[j]._green_compensation = total_compensation;
                            else
                            {
                                if(tmpmaxgreen > iMAX)
                                {
                                    _exec_plan._ptr_subplaninfo[j]._green_compensation =  iMAX - _exec_plan._ptr_subplaninfo[j]._green;
                                    TmpMaxCompensation = total_compensation - _exec_plan._ptr_subplaninfo[j]._green_compensation;
                                }
                                else
                                {
                                    _exec_plan._ptr_subplaninfo[j]._green_compensation = total_compensation;
                                    TmpMaxCompensation = 0;
                                }
                            }
                        }
                        else
                        {
                            for(int i=0; i<_exec_plan._subphase_count; i++)
                                divisor += _exec_plan._ptr_subplaninfo[i]._green;
                            if(TmpMaxCompensation != 0)
                            {
                                int tmpmaxgreen = _exec_plan._ptr_subplaninfo[j]._green + TmpMaxCompensation;
                                if(iMAX <= 0)
                                    _exec_plan._ptr_subplaninfo[j]._green_compensation = TmpMaxCompensation;
                                else
                                {
                                    if(tmpmaxgreen > iMAX)
                                    {
                                        _exec_plan._ptr_subplaninfo[j]._green_compensation =  iMAX - _exec_plan._ptr_subplaninfo[j]._green;
                                        TmpMaxCompensation = TmpMaxCompensation - _exec_plan._ptr_subplaninfo[j]._green_compensation;
                                    }
                                    else
                                    {
                                        _exec_plan._ptr_subplaninfo[j]._green_compensation = TmpMaxCompensation;
                                        TmpMaxCompensation = 0;
                                    }
                                }
                            }
                            else
                                _exec_plan._ptr_subplaninfo[j]._green_compensation = 0;
                        }
                    }
                    else
                    {
                        if(j > 0)
                        {
                            int iMin = _exec_plan._ptr_subplaninfo[j]._min_green;
                            _exec_plan._ptr_subplaninfo[j]._green_compensation = (total_compensation*_exec_plan._ptr_subplaninfo[j]._green)/divisor;
                            int tmpmingreen = _exec_plan._ptr_subplaninfo[j]._green - _exec_plan._ptr_subplaninfo[j]._green_compensation;
                            if(tmpmingreen <= iMin)
                            {
                                _exec_plan._ptr_subplaninfo[j]._green_compensation = _exec_plan._ptr_subplaninfo[j]._green - iMin;
                            }
                        }
                        else
                            _exec_plan._ptr_subplaninfo[j]._green_compensation = 0;
                    }

                    _exec_plan._ptr_subplaninfo[j]._yellow_compensation         = 0;
                    _exec_plan._ptr_subplaninfo[j]._allred_compensation         = 0;
                    _exec_plan._ptr_subplaninfo[j]._pedgreen_flash_compensation = 0;
                    _exec_plan._ptr_subplaninfo[j]._pedred_compensation         = 0;
                    iActualCompensationValue += _exec_plan._ptr_subplaninfo[j]._green_compensation;
                }
                smem.SetBRTRemainCompensation(TmpMaxCompensation);
            }
            else
            {
                for(int j=0; j<_exec_plan._subphase_count; j++)
                    divisor += _exec_plan._ptr_subplaninfo[j]._green;
                for(int j=0; j<_exec_plan._subphase_count; j++)
                {
                    _exec_plan._ptr_subplaninfo[j]._green_compensation = (total_compensation*_exec_plan._ptr_subplaninfo[j]._green)/divisor;
                    printf("\n\n\ntotal_compensation*_exec_plan._ptr_subplaninfo[j]._green = %d \ndivisor= %d\ngreen_compensation = %d\n\n\n",
                           total_compensation*_exec_plan._ptr_subplaninfo[j]._green,divisor,_exec_plan._ptr_subplaninfo[j]._green_compensation);

                    //jacky
                    int iMin = _exec_plan._ptr_subplaninfo[j]._min_green;
                    int iMAX = _exec_plan._ptr_subplaninfo[j]._max_green;
                    if(_exec_plan._shorten_cycle)
                    {
                        int tmpmingreen = _exec_plan._ptr_subplaninfo[j]._green - _exec_plan._ptr_subplaninfo[j]._green_compensation;
                        if(tmpmingreen < iMin && iMin < smem.GetCompensationMingreen())
                            _exec_plan._ptr_subplaninfo[j]._green_compensation = _exec_plan._ptr_subplaninfo[j]._green - smem.GetCompensationMingreen();
                        else if (tmpmingreen < iMin)
                            _exec_plan._ptr_subplaninfo[j]._green_compensation = _exec_plan._ptr_subplaninfo[j]._green - iMin;
                    }
                    else
                    {
                        int tmpmaxgreen = _exec_plan._ptr_subplaninfo[j]._green + _exec_plan._ptr_subplaninfo[j]._green_compensation;
                        if(iMAX <= 0) {}
                        else if(tmpmaxgreen > iMAX)
                            _exec_plan._ptr_subplaninfo[j]._green_compensation = iMAX - _exec_plan._ptr_subplaninfo[j]._green;
                    }

                    _exec_plan._ptr_subplaninfo[j]._yellow_compensation         = 0;
                    _exec_plan._ptr_subplaninfo[j]._allred_compensation         = 0;
                    _exec_plan._ptr_subplaninfo[j]._pedgreen_flash_compensation = 0;
                    _exec_plan._ptr_subplaninfo[j]._pedred_compensation         = 0;
                    iActualCompensationValue += _exec_plan._ptr_subplaninfo[j]._green_compensation;
                }
                printf("\n------------ smem.GetPriorityswitch() == false && smem.GetSegmentChange() == true ------------");
                printf("\n------------ smem.GetPriorityswitch() == false && smem.GetSegmentChange() == true ------------\n");
            }
        }

        if(total_compensation != 0 && iActualCompensationValue == 0 && smem.GetPriorityswitch() == false)                  //upper not compensation
        {
            _exec_plan._ptr_subplaninfo[0]._green_compensation = total_compensation;
        }
    }
    catch (...) {}
}

//----------------------------------------------------------
void CSTC::vCalculateCompensation_in_CHAIN(void)
{
    try
    {
        if( _exec_plan._planid==FLASH_PLANID || _exec_plan._planid==ALLRED_PLANID
                || _current_strategy==STRATEGY_MANUAL || _current_strategy==STRATEGY_ALLDYNAMIC) return;
        printf("\n    CalculateCompensation_in_CHAIN\n");
        int iActualCompensationValue = 0;
        unsigned short int total_compensation = CalculateCompensationBaseInChain();
        unsigned short int divisor=0;

        sChildChain sCCTMP;
        int iOriCycle;
        int iChainCycle;
        int iStartTime = 0;
        int iEndTime = 0;
        int iChainStartTime = 0;
        int iChainEndTime = 0;

        int iSSP = 1;
        int iESP = 2;
        int iSSPCount = 0;
        int iESPCount = 0;
        bool bHiLowPhase[8];
        bool bStartShort = false;
        bool bEndShort = false;
        int iStartCompensation = 0;
        int iEndCompensation = 0;

        int StartOffset = 0;
        int EndOffset = 0;

        float fCP1 = 0;
        float fCP2 = 0;
        int iTMP;
        int _green_compensationTMP;

        sCCTMP = smem.vGetChildChainStruct();
        StartOffset = smem.vGetChainOffset(1, 0);
        EndOffset = smem.vGetChainOffset(2, 0);
        iSSP = smem.vGetUCData(TC92_5F32StartSubPhaseId);
        iESP = smem.vGetUCData(TC92_5F32EndSubPhaseId);

        iOriCycle = _exec_plan.calculated_cycle_time();
        iChainCycle = sCCTMP.iChainCycle;
        iChainStartTime = sCCTMP.iStartKeepTime;
        iChainEndTime = sCCTMP.iEndKeepTime;

        printf("MotherChain Cycle:%d OriCycle:%d\n", iChainCycle, iOriCycle);

        for(int j=0; j<_exec_plan._subphase_count; j++)
            divisor += _exec_plan._ptr_subplaninfo[j]._green;
        for(int j=0; j<_exec_plan._subphase_count; j++)
        {
            _exec_plan._ptr_subplaninfo[j]._green_compensation          = (total_compensation*_exec_plan._ptr_subplaninfo[j]._green)/divisor;
            _exec_plan._ptr_subplaninfo[j]._yellow_compensation         = 0;
            _exec_plan._ptr_subplaninfo[j]._allred_compensation         = 0;
            _exec_plan._ptr_subplaninfo[j]._pedgreen_flash_compensation = 0;
            _exec_plan._ptr_subplaninfo[j]._pedred_compensation         = 0;
            iActualCompensationValue += _exec_plan._ptr_subplaninfo[j]._green_compensation;
        }
//  if(total_compensation != 0 && iActualCompensationValue == 0) {                //upper not compensation
//    _exec_plan._ptr_subplaninfo[0]._green_compensation = total_compensation;
//  }
        if(total_compensation > iActualCompensationValue)
        {
            _exec_plan._ptr_subplaninfo[0]._green_compensation += (total_compensation - iActualCompensationValue);
        }

//pass 2, turn start and end
        if(iSSP < iESP)
        {
            for(int i = 0; i < _exec_plan._subphase_count; i++)
            {
                if(i >= iSSP-1 && i < iESP-1)
                {
                    bHiLowPhase[i] = true;
                    iSSPCount++;
                }
                else
                {
                    bHiLowPhase[i] = false;
                    iESPCount++;
                }
            }
        }
        else
        {
            for(int i = 0; i < _exec_plan._subphase_count; i++)
            {
                if(i >= iESP-1 && i < iSSP-1)
                {
                    bHiLowPhase[i] = false;
                    iESPCount++;
                }
                else
                {
                    bHiLowPhase[i] = true;
                    iSSPCount++;
                }
            }
        }
        for(int i = 0; i < _exec_plan._subphase_count; i++)
        {
            if(bHiLowPhase[i] == true)
            {
                iStartTime += _exec_plan._ptr_subplaninfo[i]._green;
                iStartTime += _exec_plan._ptr_subplaninfo[i]._pedgreen_flash;
                iStartTime += _exec_plan._ptr_subplaninfo[i]._pedred;
//      iChainStartTime -= _exec_plan._ptr_subplaninfo[i]._pedred;
                iChainStartTime -= _exec_plan._ptr_subplaninfo[i]._yellow;
                iChainStartTime -= _exec_plan._ptr_subplaninfo[i]._allred;
            }
            else
            {
                iEndTime += _exec_plan._ptr_subplaninfo[i]._green;
                iEndTime += _exec_plan._ptr_subplaninfo[i]._pedgreen_flash;
                iEndTime += _exec_plan._ptr_subplaninfo[i]._pedred;
                iChainEndTime -= _exec_plan._ptr_subplaninfo[i]._yellow;
                iChainEndTime -= _exec_plan._ptr_subplaninfo[i]._allred;
            }
        }
        printf("iStartTime:%d iEndTime:%d iChainStartTime:%d iChainEndTime:%d iSSPCount:%d iESPCount:%d\n", iStartTime, iEndTime, iChainStartTime, iChainEndTime, iSSPCount, iESPCount);

        iStartCompensation = iChainStartTime - iStartTime;
        iEndCompensation = iChainEndTime - iEndTime;

        iStartCompensation -= StartOffset;
        iEndCompensation += StartOffset;

        iStartCompensation += EndOffset;
        iEndCompensation -= EndOffset;


        printf("iStartCompensation:%d iEndCompensation:%d \n", iStartCompensation, iEndCompensation);

        /*
          for(int i = 0; i < _exec_plan._subphase_count; i++) {
            printf("Ori %d _green_compensation:%d\n", i, _exec_plan._ptr_subplaninfo[i]._green_compensation);
            if(bHiLowPhase[i] == true) {
              if(bStartShort == _exec_plan._shorten_cycle) { _exec_plan._ptr_subplaninfo[i]._green_compensation += (iStartCompensation/iSSPCount); }
              else { _exec_plan._ptr_subplaninfo[i]._green_compensation = abs(_exec_plan._ptr_subplaninfo[i]._green_compensation - (iStartCompensation/iSSPCount));}
            }
            else {
              if(bEndShort == _exec_plan._shorten_cycle) { _exec_plan._ptr_subplaninfo[i]._green_compensation += (iEndCompensation/iESPCount); }
              else { _exec_plan._ptr_subplaninfo[i]._green_compensation = abs(_exec_plan._ptr_subplaninfo[i]._green_compensation - (iEndCompensation/iESPCount));}
            }
            printf("AfterOri %d _green_compensation:%d\n", i, _exec_plan._ptr_subplaninfo[i]._green_compensation);
          }
        */


        for(int i = 0; i < _exec_plan._subphase_count; i++)
        {
            if(_exec_plan._shorten_cycle)
            {
                iTMP = 0 - _exec_plan._ptr_subplaninfo[i]._green_compensation;
            }
            else
            {
                iTMP = _exec_plan._ptr_subplaninfo[i]._green_compensation;
            }
            printf("Ori %d _green_compensation:%d\n", i, iTMP);
            if(bHiLowPhase[i] == true)
            {
                _exec_plan._ptr_subplaninfo[i]._green_compensation = iTMP + (iStartCompensation/iSSPCount);
            }
            else
            {
                _exec_plan._ptr_subplaninfo[i]._green_compensation = iTMP + (iEndCompensation/iESPCount);
            }

            iTMP = _exec_plan._ptr_subplaninfo[i]._green_compensation + _exec_plan._ptr_subplaninfo[i]._green;
            iTMP -= 5;

            if(iTMP < 0)
            {
                printf("Chain Error!");
                _exec_plan._ptr_subplaninfo[i]._green_compensation -= iTMP;

            }

            printf("AfterOri %d _green_compensation:%d\n", i, _exec_plan._ptr_subplaninfo[i]._green_compensation);
        }
        _exec_plan._shorten_cycle = false;


    }
    catch (...) {}
}

//----------------------------------------------------------
void CSTC::CalculateCompensation_in_CADC(void)
{
    try
    {
        if( _exec_plan._planid==FLASH_PLANID || _exec_plan._planid==ALLRED_PLANID || _current_strategy==STRATEGY_MANUAL || _current_strategy==STRATEGY_ALLDYNAMIC) return;

        printf("\n    CalculateCompensation_in_CADC\n");


        unsigned short int total_compensation = CalculateCompensationBase();

        /* Compensation in CADC
         * always compensate only non-artieral phase
         */
        unsigned short int divisor=0;
        for(int j=0; j<_exec_plan._subphase_count; j++)
            if(j!=ARTIERAL_PHASENO)
                divisor += _exec_plan._ptr_subplaninfo[j]._green;

        for(int j=0; j<_exec_plan._subphase_count; j++)
        {
            if(j!=ARTIERAL_PHASENO)
            {
                _exec_plan._ptr_subplaninfo[j]._green_compensation          = (total_compensation*_exec_plan._ptr_subplaninfo[j]._green)         /(divisor==0?1:divisor);
                _exec_plan._ptr_subplaninfo[j]._yellow_compensation         = 0;
                _exec_plan._ptr_subplaninfo[j]._allred_compensation         = 0;
                _exec_plan._ptr_subplaninfo[j]._pedgreen_flash_compensation = 0;
                _exec_plan._ptr_subplaninfo[j]._pedred_compensation         = 0;
            }
        }

        _exec_plan._ptr_subplaninfo[ARTIERAL_PHASENO]._green_compensation          = 0;
        _exec_plan._ptr_subplaninfo[ARTIERAL_PHASENO]._yellow_compensation         = 0;
        _exec_plan._ptr_subplaninfo[ARTIERAL_PHASENO]._allred_compensation         = 0;
        _exec_plan._ptr_subplaninfo[ARTIERAL_PHASENO]._pedgreen_flash_compensation = 0;
        _exec_plan._ptr_subplaninfo[ARTIERAL_PHASENO]._pedred_compensation         = 0;

    }
    catch (...)
    {
        perror("ERROR: Compensation_in_CADC");
    }
}
//----------------------------------------------------------
void CSTC::AdjustOffset_of_CurrentCycle_in_CADC(const short int &adjust_offset)
{
    try
    {

        printf("========  Adjust_Offset=%3d  _exec_plan._shorten_cycle=%d  ========\n",adjust_offset,_exec_plan._shorten_cycle?1:0);

        if(adjust_offset==0) return;

        else if(adjust_offset>0)  //extend the cycle time of current cycle
        {
            CalculateAndSendRedCount(adjust_offset);
            unsigned short int divisor=0, rest_compensation=0;
            timer_gettime(_timer_plan,&_itimer_plan);  //get the remained timer
            if(!_exec_plan._shorten_cycle)   //if(originally extending cycle time)
            {
                if( _exec_phase_current_subphase_step==0 && _itimer_plan.it_value.tv_sec >1 )   //in the begining of the subphase
                {
                    for(int j=_exec_phase_current_subphase; j<_exec_plan._subphase_count; j++) divisor += _exec_plan._ptr_subplaninfo[j]._green;
                    _itimer_plan.it_value.tv_sec += abs(adjust_offset*_exec_plan._ptr_subplaninfo[_exec_phase_current_subphase]._green/divisor);

                    for(int ii = 0; ii < 4; ii++)
                    {
                        timer_settime(_timer_plan, 0, &_itimer_plan, NULL);

                        _itimer_plan_WDT = _itimer_plan; /*OT_PLAN_WDT*/
                        _itimer_plan_WDT.it_value.tv_sec += 2;
                        if(timer_settime( _timer_plan_WDT, 0, & _itimer_plan_WDT, NULL ))
                        {
                            printf("PlanWDT Settime Error!.\n");
                            exit( 1 );
                        }
                        timer_settime( _timer_redcount, 0, & _itimer_redcount, NULL );
                    }

                }//endif(extend including current subphase)
                else
                {
                    for(int j=(_exec_phase_current_subphase+1); j<_exec_plan._subphase_count; j++) divisor += _exec_plan._ptr_subplaninfo[j]._green;
                }//endelse(extend only rest subphases)

                for(int j=(_exec_phase_current_subphase+1); j<_exec_plan._subphase_count; j++)
                    _exec_plan._ptr_subplaninfo[j]._green_compensation += abs(adjust_offset*_exec_plan._ptr_subplaninfo[j]._green/divisor);
            }//endif(originally extending cycle time)

            else
            {
                if( _exec_phase_current_subphase_step==0 && _itimer_plan.it_value.tv_sec >1 )
                {
                    for(int j=_exec_phase_current_subphase; j<_exec_plan._subphase_count; j++) divisor += _exec_plan._ptr_subplaninfo[j]._green;
                    _itimer_plan.it_value.tv_sec += abs(adjust_offset*_exec_plan._ptr_subplaninfo[_exec_phase_current_subphase]._green/divisor);

                    for(int ii = 0; ii < 4; ii++)
                    {
                        timer_settime(_timer_plan, 0, &_itimer_plan, NULL);

                        _itimer_plan_WDT = _itimer_plan; /*OT_PLAN_WDT*/
                        _itimer_plan_WDT.it_value.tv_sec += 2;
                        if(timer_settime( _timer_plan_WDT, 0, & _itimer_plan_WDT, NULL ))
                        {
                            printf("PlanWDT Settime Error!.\n");
                            exit( 1 );
                        }
                        timer_settime( _timer_redcount, 0, & _itimer_redcount, NULL );
                    }


                }//endif(extend including current subphase)

                else
                {
                    for(int j=(_exec_phase_current_subphase+1); j<_exec_plan._subphase_count; j++) divisor += _exec_plan._ptr_subplaninfo[j]._green;
                }//endelse(extend only rest subphases)

                for(int j=(_exec_phase_current_subphase+1); j<_exec_plan._subphase_count; j++) rest_compensation+=_exec_plan._ptr_subplaninfo[j]._green_compensation;
                if( rest_compensation<=2 )   //ignore the original compensations, re-calculated the compensation
                {
                    _exec_plan._shorten_cycle=false;
                    for(int j=(_exec_phase_current_subphase+1); j<_exec_plan._subphase_count; j++)
                        _exec_plan._ptr_subplaninfo[j]._green_compensation = abs(adjust_offset*_exec_plan._ptr_subplaninfo[j]._green/divisor);
                }
                else   //revise the rest_compensation
                {
                    if( rest_compensation >= abs(adjust_offset) )
                    {
                        for(int j=(_exec_phase_current_subphase+1); j<_exec_plan._subphase_count; j++)
                            _exec_plan._ptr_subplaninfo[j]._green_compensation
                            = (_exec_plan._ptr_subplaninfo[j]._green_compensation-abs(adjust_offset*_exec_plan._ptr_subplaninfo[j]._green/divisor))<=0?0:
                              (_exec_plan._ptr_subplaninfo[j]._green_compensation-abs(adjust_offset*_exec_plan._ptr_subplaninfo[j]._green/divisor));
                    }//endif(extend time smaller than rest_compensation, adjust via decreasing compensation)
                    else
                    {
                        _exec_plan._shorten_cycle=false;
                        for(int j=(_exec_phase_current_subphase+1); j<_exec_plan._subphase_count; j++)
                            _exec_plan._ptr_subplaninfo[j]._green_compensation
                            = (abs(adjust_offset*_exec_plan._ptr_subplaninfo[j]._green/divisor)-_exec_plan._ptr_subplaninfo[j]._green_compensation)<=0?0:
                              (abs(adjust_offset*_exec_plan._ptr_subplaninfo[j]._green/divisor)-_exec_plan._ptr_subplaninfo[j]._green_compensation);
                    }//endelse(extend time greater than rest_compensation, adjust via revising compensation to extend cycle)
                }//endelse(revise the rest_compensation)
            }//endelse(originally shortening cycle time)
        }//endif(adjust_offset>0)

        else if(adjust_offset<0)  //shorten the cycle time of current cycle
        {
            CalculateAndSendRedCount(adjust_offset);
            unsigned short int divisor=0, rest_compensation=0;
            timer_gettime(_timer_plan,&_itimer_plan);  //get the remained timer
            if(!_exec_plan._shorten_cycle)
            {
                if( _exec_phase_current_subphase_step==0 && _itimer_plan.it_value.tv_sec >1 )
                {
                    for(int j=_exec_phase_current_subphase; j<_exec_plan._subphase_count; j++) divisor += _exec_plan._ptr_subplaninfo[j]._green;
                    _itimer_plan.it_value.tv_sec
                    = (_itimer_plan.it_value.tv_sec-abs(adjust_offset*_exec_plan._ptr_subplaninfo[_exec_phase_current_subphase]._green/divisor))<=1?1:
                      (_itimer_plan.it_value.tv_sec-abs(adjust_offset*_exec_plan._ptr_subplaninfo[_exec_phase_current_subphase]._green/divisor));

                    for(int ii = 0; ii < 4; ii++)
                    {
                        timer_settime(_timer_plan, 0, &_itimer_plan, NULL);

                        _itimer_plan_WDT = _itimer_plan; /*OT_PLAN_WDT*/
                        _itimer_plan_WDT.it_value.tv_sec += 2;
                        if(timer_settime( _timer_plan_WDT, 0, & _itimer_plan_WDT, NULL ))
                        {
                            printf("PlanWDT Settime Error!.\n");
                            exit( 1 );
                        }
                        timer_settime( _timer_redcount, 0, & _itimer_redcount, NULL );
                    }


                }//endif(shorten including current subphase)

                else
                {
                    for(int j=(_exec_phase_current_subphase+1); j<_exec_plan._subphase_count; j++) divisor += _exec_plan._ptr_subplaninfo[j]._green;
                }//endelse(shorten only rest subphases)

                for(int j=(_exec_phase_current_subphase+1); j<_exec_plan._subphase_count; j++) rest_compensation+=_exec_plan._ptr_subplaninfo[j]._green_compensation;
                if( rest_compensation<=2 )   //ignore the original compensations, re-calculated the compensation
                {
                    _exec_plan._shorten_cycle=true;
                    for(int j=(_exec_phase_current_subphase+1); j<_exec_plan._subphase_count; j++)
                        _exec_plan._ptr_subplaninfo[j]._green_compensation = abs(adjust_offset*_exec_plan._ptr_subplaninfo[j]._green/divisor);
                }
                else   //revise the rest_compensation
                {
                    if( rest_compensation >= abs(adjust_offset))
                    {
                        for(int j=(_exec_phase_current_subphase+1); j<_exec_plan._subphase_count; j++)
                            _exec_plan._ptr_subplaninfo[j]._green_compensation
                            = (_exec_plan._ptr_subplaninfo[j]._green_compensation-abs(adjust_offset*_exec_plan._ptr_subplaninfo[j]._green/divisor))<=0?0:
                              (_exec_plan._ptr_subplaninfo[j]._green_compensation-abs(adjust_offset*_exec_plan._ptr_subplaninfo[j]._green/divisor));
                    }//endif(shorten time smaller than rest_compensation, adjust via decreasing compensation)
                    else
                    {
                        _exec_plan._shorten_cycle=true;
                        for(int j=(_exec_phase_current_subphase+1); j<_exec_plan._subphase_count; j++)
                            _exec_plan._ptr_subplaninfo[j]._green_compensation
                            = (abs(adjust_offset*_exec_plan._ptr_subplaninfo[j]._green/divisor)-_exec_plan._ptr_subplaninfo[j]._green_compensation)<=0?0:
                              (abs(adjust_offset*_exec_plan._ptr_subplaninfo[j]._green/divisor)-_exec_plan._ptr_subplaninfo[j]._green_compensation);
                    }//endelse(shorten time greater than rest_compensation, adjust via revising compensation to extend cycle)
                }//endelse(revise the rest_compensation)
            }//endif(originally extending cycle time)

            else
            {
                if( _exec_phase_current_subphase_step==0 && _itimer_plan.it_value.tv_sec >1 )   //in the begining of the subphase
                {
                    for(int j=_exec_phase_current_subphase; j<_exec_plan._subphase_count; j++) divisor += _exec_plan._ptr_subplaninfo[j]._green;
                    _itimer_plan.it_value.tv_sec
                    = (_itimer_plan.it_value.tv_sec-abs(adjust_offset*_exec_plan._ptr_subplaninfo[_exec_phase_current_subphase]._green/divisor))<=1?1:
                      (_itimer_plan.it_value.tv_sec-abs(adjust_offset*_exec_plan._ptr_subplaninfo[_exec_phase_current_subphase]._green/divisor));

                    for(int ii = 0; ii < 4; ii++)
                    {
                        timer_settime(_timer_plan, 0, &_itimer_plan, NULL);

                        _itimer_plan_WDT = _itimer_plan; /*OT_PLAN_WDT*/
                        _itimer_plan_WDT.it_value.tv_sec += 2;
                        if(timer_settime( _timer_plan_WDT, 0, & _itimer_plan_WDT, NULL ))
                        {
                            printf("PlanWDT Settime Error!.\n");
                            exit( 1 );
                        }
                        timer_settime( _timer_redcount, 0, & _itimer_redcount, NULL );
                    }


                }//endif(shorten including current subphase)
                else
                {
                    for(int j=(_exec_phase_current_subphase+1); j<_exec_plan._subphase_count; j++) divisor += _exec_plan._ptr_subplaninfo[j]._green;
                }//endelse(shorten only rest subphases)

                for(int j=(_exec_phase_current_subphase+1); j<_exec_plan._subphase_count; j++)
                    _exec_plan._ptr_subplaninfo[j]._green_compensation += abs(adjust_offset*_exec_plan._ptr_subplaninfo[j]._green/divisor);
            }//endelse(originally shortening cycle time)
        }//endif(adjust_offset<0)

    }
    catch(...) {}
}
//----------------------------------------------------------
//**********************************************************
//****    Report the Status of Current Step to Center
//**********************************************************
//----------------------------------------------------------
//OT Debug Direct
void CSTC::ReportCurrentStepStatus(void)  //5F03
{
    try
    {

        DATA_Bit _SignalMap;
        int iLightOutNo;
        int iDataPtr = 9;
        unsigned char data[255];
        //Should be mutex

        unsigned short int time_difference=0;

        if(_timer_plan != NULL)
        {

            if(_current_strategy==STRATEGY_MANUAL||_current_strategy==STRATEGY_FLASH||_current_strategy==STRATEGY_ALLRED || _current_strategy==STRATEGY_ALLDYNAMIC)
            {
                timespec strategy_current_time= {0,0};
                if(clock_gettime(CLOCK_REALTIME, &strategy_current_time)<0) perror("Can not get current time");
                time_difference = (strategy_current_time.tv_sec - strategy_start_time.tv_sec);
            }
            else
            {
                if(_exec_plan._planid!=FLASH_PLANID && _exec_plan._planid!=ALLRED_PLANID)
                {
                    timer_gettime(_timer_plan,&_itimer_plan);
                    time_difference = (_itimer_plan.it_value.tv_sec) + 1;
                    //OT debug
                    printf("\ntime_difference: %d\n", time_difference);

                }
            }

            _SignalMap.DBit = _exec_phase._signal_map;

            //  unsigned short int data_length = ( 9 + _exec_phase._signal_count + 4 );
            unsigned short int data_length = ( 9 + _exec_phase._signal_count);
//mallocFuck  unsigned char *data = (unsigned char *)malloc( data_length*sizeof(unsigned char) );

            data[0] = 0x5F;
            data[1] = 0x03;
            data[2] = _exec_phase._phase_order;
            data[3] = _exec_phase._signal_map;
            data[4] = _exec_phase._signal_count;
            data[5] = (_exec_phase_current_subphase+1);
            printf("(_exec_phase_current_subphase+1)=%d.\n", (_exec_phase_current_subphase+1));

            if( smem.vGetBOOLData(TC_SignalConflictError) == true )
            {
                data[6] = 0xCF;
            }
            else if(smem.vGetUCData(TC_TrainChainEnable) == 1 && smem.vGetUCData(TC_TrainChainNOW) == 1)
            {
                data[6] = 0x8F;
            }
            else if(_exec_plan._planid==ALLRED_PLANID)
            {
                data[6] = 0x9F;
            }
            else if(_exec_plan._planid==FLASH_PLANID)
            {
                data[6] = 0xDF;
            }
            else if( smem.vGetBOOLData(TC92_SubPhaseOfPhasePlanIncorrent) == true )
            {
                data[6] = 0xFF;
            }
            else
            {
                data[6] = (_exec_phase_current_subphase_step+1);
            }

            printf("(_exec_phase_current_subphase_step+1)=%d.\n", (_exec_phase_current_subphase_step+1));

            //OT debug
            printf("time_difference2: %d\n", time_difference);

            data[7] = (time_difference&0xFF00)>>8;
            data[8] = (time_difference&0xFF);

            printf("OTDEBUG _exec_phase._signal_count:%d\n", _exec_phase._signal_count);

            if(_SignalMap.switchBit.b1 == true && (iDataPtr-9) < _exec_phase._signal_count)                                           //ï¿½qSignalMapï¿½ï¿½3CCTï¿½ï¿½ï¿½Oï¿½I
            {
                iLightOutNo = smem.vGetSignamMapMappingDir(dirN);
                printf("B1 is True! iLightOutNo:%d\n", iLightOutNo);
                if(iLightOutNo < _exec_phase._signal_count)
                {
                    data[iDataPtr] = oTools.vCCTLightToVer30Light(_exec_phase._ptr_subphase_step_signal_status[_exec_phase_current_subphase][_exec_phase_current_subphase_step][iLightOutNo]);
                    iDataPtr++;
                }
            }
            if(_SignalMap.switchBit.b2 == true && (iDataPtr-9) < _exec_phase._signal_count)                                           //ï¿½qSignalMapï¿½ï¿½3CCTï¿½ï¿½ï¿½Oï¿½I
            {
                iLightOutNo = smem.vGetSignamMapMappingDir(dirNE);
                printf("B2 is True! iLightOutNo:%d\n", iLightOutNo);
                if(iLightOutNo < _exec_phase._signal_count)
                {
                    data[iDataPtr] = oTools.vCCTLightToVer30Light(_exec_phase._ptr_subphase_step_signal_status[_exec_phase_current_subphase][_exec_phase_current_subphase_step][iLightOutNo]);
                    iDataPtr++;
                }
            }
            if(_SignalMap.switchBit.b3 == true && (iDataPtr-9) < _exec_phase._signal_count)
            {
                iLightOutNo = smem.vGetSignamMapMappingDir(dirE);
                printf("B3 is True! iLightOutNo:%d\n", iLightOutNo);
                if(iLightOutNo < _exec_phase._signal_count)
                {
                    data[iDataPtr] = oTools.vCCTLightToVer30Light(_exec_phase._ptr_subphase_step_signal_status[_exec_phase_current_subphase][_exec_phase_current_subphase_step][iLightOutNo]);
                    iDataPtr++;
                }
            }
            if(_SignalMap.switchBit.b4 == true && (iDataPtr-9) < _exec_phase._signal_count)
            {
                iLightOutNo = smem.vGetSignamMapMappingDir(dirES);
                printf("B4 is True! iLightOutNo:%d\n", iLightOutNo);
                if(iLightOutNo < _exec_phase._signal_count)
                {
                    data[iDataPtr] = oTools.vCCTLightToVer30Light(_exec_phase._ptr_subphase_step_signal_status[_exec_phase_current_subphase][_exec_phase_current_subphase_step][iLightOutNo]);
                    iDataPtr++;
                }
            }
            if(_SignalMap.switchBit.b5 == true && (iDataPtr-9) < _exec_phase._signal_count)
            {
                iLightOutNo = smem.vGetSignamMapMappingDir(dirS);
                printf("B5 is True! iLightOutNo:%d\n", iLightOutNo);
                if(iLightOutNo < _exec_phase._signal_count)
                {
                    data[iDataPtr] = oTools.vCCTLightToVer30Light(_exec_phase._ptr_subphase_step_signal_status[_exec_phase_current_subphase][_exec_phase_current_subphase_step][iLightOutNo]);
                    iDataPtr++;
                }
            }
            if(_SignalMap.switchBit.b6 == true && (iDataPtr-9) < _exec_phase._signal_count)
            {
                iLightOutNo = smem.vGetSignamMapMappingDir(dirSW);
                printf("B6 is True! iLightOutNo:%d\n", iLightOutNo);
                if(iLightOutNo < _exec_phase._signal_count)
                {
                    data[iDataPtr] = oTools.vCCTLightToVer30Light(_exec_phase._ptr_subphase_step_signal_status[_exec_phase_current_subphase][_exec_phase_current_subphase_step][iLightOutNo]);
                    iDataPtr++;
                }
            }
            if(_SignalMap.switchBit.b7 == true && (iDataPtr-9) < _exec_phase._signal_count)
            {
                iLightOutNo = smem.vGetSignamMapMappingDir(dirW);
                printf("B7 is True! iLightOutNo:%d\n", iLightOutNo);
                if(iLightOutNo < _exec_phase._signal_count)
                {
                    data[iDataPtr] = oTools.vCCTLightToVer30Light(_exec_phase._ptr_subphase_step_signal_status[_exec_phase_current_subphase][_exec_phase_current_subphase_step][iLightOutNo]);
                    iDataPtr++;
                }
            }
            if(_SignalMap.switchBit.b8 == true && (iDataPtr-9) < _exec_phase._signal_count)
            {
                iLightOutNo = smem.vGetSignamMapMappingDir(dirWN);
                printf("B8 is True! iLightOutNo:%d\n", iLightOutNo);
                if(iLightOutNo < _exec_phase._signal_count)
                {
                    data[iDataPtr] = oTools.vCCTLightToVer30Light(_exec_phase._ptr_subphase_step_signal_status[_exec_phase_current_subphase][_exec_phase_current_subphase_step][iLightOutNo]);
                    iDataPtr++;
                }
            }

            /*92 no this */
//  data[data_length-4] = (_exec_segment._segment_type)&0xFF;
//  data[data_length-3] = (_exec_segment_current_seg_no+1)&0xFF;
//  data[data_length-2] = (_exec_phase.calculated_total_step_count())&0xFF;
//  data[data_length-1] = (_exec_phase.step_no_of_all(_exec_phase_current_subphase, _exec_phase_current_subphase_step)+1)&0xFF;

            printf("%s[MESSAGE] Report Current Step Status to Center: %d-%d   timer:%3d.%s\n",
                   ColorGreen, _exec_phase_current_subphase+1, _exec_phase_current_subphase_step+1,
                   time_difference, ColorNormal);

            /*+++++++++++++++++*/

            //OTCombo0713
            MESSAGEOK _MsgOK;
            _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, iDataPtr, true);
//  _MsgOK.InnerOrOutWard = cOutWard;
//  writeJob.WriteWorkByMESSAGEOUT(_MsgOK);
            writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICECENTER92);

            /*-----------------*/
//mallocFuck    free(data);

        }
    }
    catch (...) {}
}
//-------------
void CSTC::ReportCurrentStepStatusCCT5F04(void)  //From 5F03
{
    try
    {

        DATA_Bit _SignalMap;
        int iLightOutNo;
        int iDataPtr = 12;
        unsigned char data[255];
        //Should be mutex

        unsigned short int time_difference=0;

        if(_timer_plan != NULL)
        {

            if(_current_strategy==STRATEGY_MANUAL||_current_strategy==STRATEGY_FLASH||_current_strategy==STRATEGY_ALLRED || _current_strategy==STRATEGY_ALLDYNAMIC)
            {
                timespec strategy_current_time= {0,0};
                if(clock_gettime(CLOCK_REALTIME, &strategy_current_time)<0) perror("Can not get current time");
                time_difference = (strategy_current_time.tv_sec - strategy_start_time.tv_sec);
            }
            else
            {
                if(_exec_plan._planid!=FLASH_PLANID && _exec_plan._planid!=ALLRED_PLANID)
                {
                    timer_gettime(_timer_plan,&_itimer_plan);
                    time_difference = (_itimer_plan.it_value.tv_sec) + 1;
                    //OT debug
                    printf("\ntime_difference: %d\n", time_difference);

                }
            }

            _SignalMap.DBit = _exec_phase._signal_map;

            //  unsigned short int data_length = ( 9 + _exec_phase._signal_count + 4 );
            unsigned short int data_length = ( 9 + _exec_phase._signal_count);
//mallocFuck  unsigned char *data = (unsigned char *)malloc( data_length*sizeof(unsigned char) );

            data[0] = 0x5F;
            data[1] = 0x04;
            data[2] = _exec_phase._phase_order;
            data[3] = _exec_phase._signal_map;
            data[4] = _exec_phase._signal_count;
            data[5] = (_exec_phase_current_subphase+1);
            printf("(_exec_phase_current_subphase+1)=%d.\n", (_exec_phase_current_subphase+1));
            if( smem.vGetBOOLData(TC_SignalConflictError) == true )
            {
                data[6] = 0xCF;
            }
            else if(smem.vGetUCData(TC_TrainChainEnable) == 1 && smem.vGetUCData(TC_TrainChainNOW) == 1)
            {
                data[6] = 0x8F;
            }
            else if(_exec_plan._planid==ALLRED_PLANID)
            {
                data[6] = 0x9F;
            }
            else if(_exec_plan._planid==FLASH_PLANID)
            {
                data[6] = 0xDF;
            }
            else if( smem.vGetBOOLData(TC92_SubPhaseOfPhasePlanIncorrent) == true )
            {
                data[6] = 0xFF;
            }
            else
            {
                data[6] = (_exec_phase_current_subphase_step+1);
            }
            printf("(_exec_phase_current_subphase_step+1)=%d.\n", (_exec_phase_current_subphase_step+1));

            //OT debug
            printf("time_difference2: %d\n", time_difference);

            data[7] = (time_difference&0xFF00)>>8;
            data[8] = (time_difference&0xFF);

            data[9] = smem.vGetUCData(TC_CSTC_FieldOperate);
            data[10] = _exec_segment_current_seg_no+1;
            data[11] = _exec_plan._planid;

            printf("OTDEBUG _exec_phase._signal_count:%d\n", _exec_phase._signal_count);

            if(_SignalMap.switchBit.b1 == true && (iDataPtr-9) < _exec_phase._signal_count)                                           //ï¿½qSignalMapï¿½ï¿½3CCTï¿½ï¿½ï¿½Oï¿½I
            {
                iLightOutNo = smem.vGetSignamMapMappingDir(dirN);
                printf("B1 is True! iLightOutNo:%d\n", iLightOutNo);
                if(iLightOutNo < _exec_phase._signal_count)
                {
                    data[iDataPtr] = oTools.vCCTLightToVer30Light(_exec_phase._ptr_subphase_step_signal_status[_exec_phase_current_subphase][_exec_phase_current_subphase_step][iLightOutNo]);
                    iDataPtr++;
                }
            }
            if(_SignalMap.switchBit.b2 == true && (iDataPtr-9) < _exec_phase._signal_count)                                           //ï¿½qSignalMapï¿½ï¿½3CCTï¿½ï¿½ï¿½Oï¿½I
            {
                iLightOutNo = smem.vGetSignamMapMappingDir(dirNE);
                printf("B2 is True! iLightOutNo:%d\n", iLightOutNo);
                if(iLightOutNo < _exec_phase._signal_count)
                {
                    data[iDataPtr] = oTools.vCCTLightToVer30Light(_exec_phase._ptr_subphase_step_signal_status[_exec_phase_current_subphase][_exec_phase_current_subphase_step][iLightOutNo]);
                    iDataPtr++;
                }
            }
            if(_SignalMap.switchBit.b3 == true && (iDataPtr-9) < _exec_phase._signal_count)
            {
                iLightOutNo = smem.vGetSignamMapMappingDir(dirE);
                printf("B3 is True! iLightOutNo:%d\n", iLightOutNo);
                if(iLightOutNo < _exec_phase._signal_count)
                {
                    data[iDataPtr] = oTools.vCCTLightToVer30Light(_exec_phase._ptr_subphase_step_signal_status[_exec_phase_current_subphase][_exec_phase_current_subphase_step][iLightOutNo]);
                    iDataPtr++;
                }
            }
            if(_SignalMap.switchBit.b4 == true && (iDataPtr-9) < _exec_phase._signal_count)
            {
                iLightOutNo = smem.vGetSignamMapMappingDir(dirES);
                printf("B4 is True! iLightOutNo:%d\n", iLightOutNo);
                if(iLightOutNo < _exec_phase._signal_count)
                {
                    data[iDataPtr] = oTools.vCCTLightToVer30Light(_exec_phase._ptr_subphase_step_signal_status[_exec_phase_current_subphase][_exec_phase_current_subphase_step][iLightOutNo]);
                    iDataPtr++;
                }
            }
            if(_SignalMap.switchBit.b5 == true && (iDataPtr-9) < _exec_phase._signal_count)
            {
                iLightOutNo = smem.vGetSignamMapMappingDir(dirS);
                printf("B5 is True! iLightOutNo:%d\n", iLightOutNo);
                if(iLightOutNo < _exec_phase._signal_count)
                {
                    data[iDataPtr] = oTools.vCCTLightToVer30Light(_exec_phase._ptr_subphase_step_signal_status[_exec_phase_current_subphase][_exec_phase_current_subphase_step][iLightOutNo]);
                    iDataPtr++;
                }
            }
            if(_SignalMap.switchBit.b6 == true && (iDataPtr-9) < _exec_phase._signal_count)
            {
                iLightOutNo = smem.vGetSignamMapMappingDir(dirSW);
                printf("B6 is True! iLightOutNo:%d\n", iLightOutNo);
                if(iLightOutNo < _exec_phase._signal_count)
                {
                    data[iDataPtr] = oTools.vCCTLightToVer30Light(_exec_phase._ptr_subphase_step_signal_status[_exec_phase_current_subphase][_exec_phase_current_subphase_step][iLightOutNo]);
                    iDataPtr++;
                }
            }
            if(_SignalMap.switchBit.b7 == true && (iDataPtr-9) < _exec_phase._signal_count)
            {
                iLightOutNo = smem.vGetSignamMapMappingDir(dirW);
                printf("B7 is True! iLightOutNo:%d\n", iLightOutNo);
                if(iLightOutNo < _exec_phase._signal_count)
                {
                    data[iDataPtr] = oTools.vCCTLightToVer30Light(_exec_phase._ptr_subphase_step_signal_status[_exec_phase_current_subphase][_exec_phase_current_subphase_step][iLightOutNo]);
                    iDataPtr++;
                }
            }
            if(_SignalMap.switchBit.b8 == true && (iDataPtr-9) < _exec_phase._signal_count)
            {
                iLightOutNo = smem.vGetSignamMapMappingDir(dirWN);
                printf("B8 is True! iLightOutNo:%d\n", iLightOutNo);
                if(iLightOutNo < _exec_phase._signal_count)
                {
                    data[iDataPtr] = oTools.vCCTLightToVer30Light(_exec_phase._ptr_subphase_step_signal_status[_exec_phase_current_subphase][_exec_phase_current_subphase_step][iLightOutNo]);
                    iDataPtr++;
                }
            }

            /*92 no this */
//  data[data_length-4] = (_exec_segment._segment_type)&0xFF;
//  data[data_length-3] = (_exec_segment_current_seg_no+1)&0xFF;
//  data[data_length-2] = (_exec_phase.calculated_total_step_count())&0xFF;
//  data[data_length-1] = (_exec_phase.step_no_of_all(_exec_phase_current_subphase, _exec_phase_current_subphase_step)+1)&0xFF;



            printf("%s[MESSAGE] Report Current Step Status to Center: %d-%d   timer:%3d.%s\n",
                   ColorGreen, _exec_phase_current_subphase+1, _exec_phase_current_subphase_step+1,
                   time_difference, ColorNormal);

            /*+++++++++++++++++*/

            //OTCombo0713
            MESSAGEOK _MsgOK;
            _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, iDataPtr, true);
//  _MsgOK.InnerOrOutWard = cOutWard;
//  writeJob.WriteWorkByMESSAGEOUT(_MsgOK);
            writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICECENTER92);

            /*-----------------*/
//mallocFuck    free(data);

        }
    }
    catch (...) {}
}

//----------------------------------------------------------
//OTCombo0720
//OT Debug Direct
void CSTC::ReportCurrentSignalMap_5F0F(void)
{
    try
    {

        if(_timer_plan != NULL)
        {

            DATA_Bit _SignalMap, _GYRSignalMap[3];
            _SignalMap.DBit = _exec_phase._signal_map;
            _GYRSignalMap[0].DBit = 0;
            _GYRSignalMap[1].DBit = 0;
            _GYRSignalMap[2].DBit = 0;
            int iLightOutNo = 0;
            unsigned short int usiSignalStatus;

            int iGYRSignalMap[3][8];
            for(int i = 0; i < 3; i++)
                for(int j = 0; j < 8; j++)
                    iGYRSignalMap[i][j] = 0;

            if(_SignalMap.switchBit.b1 == true)                                           //ï¿½qSignalMapï¿½ï¿½3CCTï¿½ï¿½ï¿½Oï¿½I
            {
                iLightOutNo = smem.vGetSignamMapMappingDir(dirN);
                if(iLightOutNo < _exec_phase._signal_count)
                {
                    usiSignalStatus = _exec_phase._ptr_subphase_step_signal_status[_exec_phase_current_subphase][_exec_phase_current_subphase_step][iLightOutNo];
                    iGYRSignalMap[0][0] = oTools.vCCTLightToVer30SignalMapLight_5F0F(0, usiSignalStatus);
                    iGYRSignalMap[1][0] = oTools.vCCTLightToVer30SignalMapLight_5F0F(1, usiSignalStatus);
                    iGYRSignalMap[2][0] = oTools.vCCTLightToVer30SignalMapLight_5F0F(2, usiSignalStatus);
                }
            }

            if(_SignalMap.switchBit.b2 == true)
            {
                iLightOutNo = smem.vGetSignamMapMappingDir(dirNE);
                if(iLightOutNo < _exec_phase._signal_count)
                {
                    usiSignalStatus = _exec_phase._ptr_subphase_step_signal_status[_exec_phase_current_subphase][_exec_phase_current_subphase_step][iLightOutNo];
                    iGYRSignalMap[0][1] = oTools.vCCTLightToVer30SignalMapLight_5F0F(0, usiSignalStatus);
                    iGYRSignalMap[1][1] = oTools.vCCTLightToVer30SignalMapLight_5F0F(1, usiSignalStatus);
                    iGYRSignalMap[2][1] = oTools.vCCTLightToVer30SignalMapLight_5F0F(2, usiSignalStatus);
                }
            }

            if(_SignalMap.switchBit.b3 == true)
            {
                iLightOutNo = smem.vGetSignamMapMappingDir(dirE);
                if(iLightOutNo < _exec_phase._signal_count)
                {
                    usiSignalStatus = _exec_phase._ptr_subphase_step_signal_status[_exec_phase_current_subphase][_exec_phase_current_subphase_step][iLightOutNo];
                    iGYRSignalMap[0][2] = oTools.vCCTLightToVer30SignalMapLight_5F0F(0, usiSignalStatus);
                    iGYRSignalMap[1][2] = oTools.vCCTLightToVer30SignalMapLight_5F0F(1, usiSignalStatus);
                    iGYRSignalMap[2][2] = oTools.vCCTLightToVer30SignalMapLight_5F0F(2, usiSignalStatus);
                }
            }

            if(_SignalMap.switchBit.b4 == true)
            {
                iLightOutNo = smem.vGetSignamMapMappingDir(dirES);
                if(iLightOutNo < _exec_phase._signal_count)
                {
                    usiSignalStatus = _exec_phase._ptr_subphase_step_signal_status[_exec_phase_current_subphase][_exec_phase_current_subphase_step][iLightOutNo];
                    iGYRSignalMap[0][3] = oTools.vCCTLightToVer30SignalMapLight_5F0F(0, usiSignalStatus);
                    iGYRSignalMap[1][3] = oTools.vCCTLightToVer30SignalMapLight_5F0F(1, usiSignalStatus);
                    iGYRSignalMap[2][3] = oTools.vCCTLightToVer30SignalMapLight_5F0F(2, usiSignalStatus);
                }
            }

            if(_SignalMap.switchBit.b5 == true)
            {
                iLightOutNo = smem.vGetSignamMapMappingDir(dirS);
                if(iLightOutNo < _exec_phase._signal_count)
                {
                    usiSignalStatus = _exec_phase._ptr_subphase_step_signal_status[_exec_phase_current_subphase][_exec_phase_current_subphase_step][iLightOutNo];
                    iGYRSignalMap[0][4] = oTools.vCCTLightToVer30SignalMapLight_5F0F(0, usiSignalStatus);
                    iGYRSignalMap[1][4] = oTools.vCCTLightToVer30SignalMapLight_5F0F(1, usiSignalStatus);
                    iGYRSignalMap[2][4] = oTools.vCCTLightToVer30SignalMapLight_5F0F(2, usiSignalStatus);
                }
            }

            if(_SignalMap.switchBit.b6 == true)
            {
                iLightOutNo = smem.vGetSignamMapMappingDir(dirSW);
                if(iLightOutNo < _exec_phase._signal_count)
                {
                    usiSignalStatus = _exec_phase._ptr_subphase_step_signal_status[_exec_phase_current_subphase][_exec_phase_current_subphase_step][iLightOutNo];
                    iGYRSignalMap[0][5] = oTools.vCCTLightToVer30SignalMapLight_5F0F(0, usiSignalStatus);
                    iGYRSignalMap[1][5] = oTools.vCCTLightToVer30SignalMapLight_5F0F(1, usiSignalStatus);
                    iGYRSignalMap[2][5] = oTools.vCCTLightToVer30SignalMapLight_5F0F(2, usiSignalStatus);
                }
            }

            if(_SignalMap.switchBit.b7 == true)
            {
                iLightOutNo = smem.vGetSignamMapMappingDir(dirW);
                if(iLightOutNo < _exec_phase._signal_count)
                {
                    usiSignalStatus = _exec_phase._ptr_subphase_step_signal_status[_exec_phase_current_subphase][_exec_phase_current_subphase_step][iLightOutNo];
                    iGYRSignalMap[0][6] = oTools.vCCTLightToVer30SignalMapLight_5F0F(0, usiSignalStatus);
                    iGYRSignalMap[1][6] = oTools.vCCTLightToVer30SignalMapLight_5F0F(1, usiSignalStatus);
                    iGYRSignalMap[2][6] = oTools.vCCTLightToVer30SignalMapLight_5F0F(2, usiSignalStatus);
                }
            }

            if(_SignalMap.switchBit.b8 == true)
            {
                iLightOutNo = smem.vGetSignamMapMappingDir(dirWN);
                if(iLightOutNo < _exec_phase._signal_count)
                {
                    usiSignalStatus = _exec_phase._ptr_subphase_step_signal_status[_exec_phase_current_subphase][_exec_phase_current_subphase_step][iLightOutNo];
                    iGYRSignalMap[0][7] = oTools.vCCTLightToVer30SignalMapLight_5F0F(0, usiSignalStatus);
                    iGYRSignalMap[1][7] = oTools.vCCTLightToVer30SignalMapLight_5F0F(1, usiSignalStatus);
                    iGYRSignalMap[2][7] = oTools.vCCTLightToVer30SignalMapLight_5F0F(2, usiSignalStatus);
                }
            }

            for(int i = 0; i < 3; i++)                                                    //ï¿½ï¿½}ï¿½Cï¿½ï¿½^Bit
            {
                for(int j = 0; j < 8; j++)
                {
                    if(iGYRSignalMap[i][j] == 1)
                    {
                        if(j == 0) _GYRSignalMap[i].switchBit.b1 = true;
                        if(j == 1) _GYRSignalMap[i].switchBit.b2 = true;
                        if(j == 2) _GYRSignalMap[i].switchBit.b3 = true;
                        if(j == 3) _GYRSignalMap[i].switchBit.b4 = true;
                        if(j == 4) _GYRSignalMap[i].switchBit.b5 = true;
                        if(j == 5) _GYRSignalMap[i].switchBit.b6 = true;
                        if(j == 6) _GYRSignalMap[i].switchBit.b7 = true;
                        if(j == 7) _GYRSignalMap[i].switchBit.b8 = true;
                    }
                }
            }

            unsigned char data[6];
            data[0] = 0x5F;
            data[1] = 0x0F;
            data[2] = _exec_phase._signal_map & 0xFF;
            data[3] = _GYRSignalMap[0].DBit;
            data[4] = _GYRSignalMap[1].DBit;
            data[5] = _GYRSignalMap[2].DBit;

            MESSAGEOK _MsgOK;
            _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, 6, true);
            _MsgOK.InnerOrOutWard = cOutWard;
//  writeJob.WriteWorkByMESSAGEOUT(_MsgOK);
            writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICECENTER92);

        }

    }
    catch (...) {}
}
//--------------jacky20140326---------------------------------
void CSTC::BRTImmediateReport(void)
{
    try
    {
        unsigned char data[33];
        MESSAGEOK _MsgOK;
//    int lcn = smem.GetAddress();
//
//    int iCurrentSubphase = stc.vGetUSIData(CSTC_exec_phase_current_subphase);
//    int iCurrentStep     = stc.vGetUSIData(CSTC_exec_phase_current_subphase_step);
//    iCurrentStep = iCurrentStep + ((iCurrentSubphase)*5);
//    iCurrentStep++;

        unsigned short int usiStepSec = stc.vGetStepTime();
//    unsigned char _condition = smem.GetBF08Condition();
        unsigned char _strategy = smem.GetBF08Strategy();
        MESSAGEOK _tmpmessage;
//    unsigned char tmpdata[28];
//    memset(tmpdata,'\0', 28);
//    DATA_Bit tmppoint;
//    tmppoint.DBit = _tmpmessage.packet[26];

        for(unsigned short int k=0; k<2; k++)
        {
            if(k==0)
                _tmpmessage = smem.GetBF0BmessageWest();
            else
                _tmpmessage = smem.GetBF0BmessageEast();
//    if(tmppoint.switchBit.b6 == 1 && tmppoint.switchBit.b5 == 0){
//        _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(tmpdata, 28, true);
//        smem.SetBF0Bmessage(_MsgOK);
//    } else {
            data[0] = 0xFF;   //傳 BF0B to Web
            data[1] = 0x00;
            data[2] = _exec_phase_current_subphase+1;
            data[3] = _exec_phase_current_subphase_step+1; //目前步階
            data[4] = usiStepSec;
            if(smem.GetBF11onoff() == 1)
            {
                data[5] = 0x02;
            }
            else if(smem.GetDefiniteTimeonoff() == 1)
            {
                data[5] = 0x03;
            }
            else if(smem.GetBF11onoff() == 0 || smem.GetDefiniteTimeonoff() == 0)
            {
                data[5] = 0x00;
            }
            else
            {
                data[5] = 0x00;
            }
            data[6] = _strategy;
            int j=9;
            for(int i=7; i<23; i++)
            {
                data[i] = _tmpmessage.packet[j];
                j++;
            }
            data[23] = _tmpmessage.packet[25];
            data[24] = _tmpmessage.packet[26];
            data[25] = _tmpmessage.packet[27];
            data[26] = _tmpmessage.packet[28];
            data[27] = _tmpmessage.packet[29];
            data[28] = _tmpmessage.packet[30];
            data[29] = _tmpmessage.packet[31];
            data[30] = _tmpmessage.packet[32];
            data[31] = _tmpmessage.packet[33];
            data[32] = _tmpmessage.packet[34];

            /* printf("\n\n @@ ");
             for(int i=0; i<32; i++){
                 printf("%02X ",data[i]);
             }
             printf("@@ \n\n"); */
            _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, 33, true);
            _MsgOK.InnerOrOutWard = cOutWard;
            // writeJob.WritePhysicalOutNoSetSeqNoResend(_MsgOK.packet, _MsgOK.packetLength, DEVICEJSON);
            writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICECENTER92); //中心port
        }


        int iSignalCnt = _exec_phase._signal_count;
        unsigned char datalight[26];
        memset(datalight,'\0', 26);
        datalight[0] = 0xFF;     //每張卡的即時燈態
        datalight[1] = 0x01;
        int j = 2;
        for(int i=0 ; i < iSignalCnt; i++)
        {
            // printf("\n\n***************************\n\n");
            // printf("%04X  %d  %d  %d",_exec_phase._ptr_subphase_step_signal_status[_exec_phase_current_subphase][_exec_phase_current_subphase_step][i],_exec_phase_current_subphase,_exec_phase_current_subphase_step,i);
            // printf("\n\n***************************\n\n");
            datalight[j] = i+1;
            datalight[j+1]= _exec_phase._ptr_subphase_step_signal_status[_exec_phase_current_subphase][_exec_phase_current_subphase_step][i]/0x100;
            datalight[j+2]= _exec_phase._ptr_subphase_step_signal_status[_exec_phase_current_subphase][_exec_phase_current_subphase_step][i]%0x100;
            j+=3;
        }
        _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(datalight, 26, true);
        _MsgOK.InnerOrOutWard = cOutWard;
        // writeJob.WritePhysicalOutNoSetSeqNoResend(_MsgOK.packet, _MsgOK.packetLength, DEVICEJSON);
        writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICECENTER92); //中心port


        /*-----------------*/
//mallocFuck    free(data);

    }
    catch (...) {}
}
//--------------jacky20141203---------------------------------
void CSTC::BRTGPSStatusReport(void)
{
    try
    {
        unsigned char data[3];
        MESSAGEOK _MsgOK;

        data[0] = 0xFF;
        data[1] = 0x02;
        if(smem.GPSStatus)
            data[2] = 1;
        else
            data[2] = 0;

        _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, 3, true);
        _MsgOK.InnerOrOutWard = cOutWard;
        writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICECENTER92); //中心port

    }
    catch (...) {}
}
//------------jacky20141227 Clear BF0B-------------------------------
void CSTC::ClearBF0B(unsigned char c)
{
    MESSAGEOK _tmpmessage;

    if(c==0)
    {
        _tmpmessage = smem.GetBF0BmessageWest();
        _tmpmessage.packet[26] = 0;
        smem.SetBF0BmessageWest(_tmpmessage);
    }
    else
    {
        _tmpmessage = smem.GetBF0BmessageEast();
        _tmpmessage.packet[26] = 0;
        smem.SetBF0BmessageEast(_tmpmessage);
    }

}

//----------------------------------------------------------
//**********************************************************
//****     Report Current Control Strategy to Center
//**********************************************************
//----------------------------------------------------------
void CSTC::ReportCurrentControlStrategy(int iInput)  //5F00
{
    try
    {

        unsigned short int data_length = 4;
//mallocFuck  unsigned char *data = (unsigned char *)malloc( data_length*sizeof(unsigned char) );
        unsigned char data[5];
        data[0] = 0x5F;
        data[1] = 0x00;

        if(iInput == 1)    //START
        {
            switch(_current_strategy)
            {
            case(STRATEGY_ALLRED):
                data[2]=0x04;
                break;
            case(STRATEGY_FLASH):
                data[2]=0x04;
                break;
            case(STRATEGY_MANUAL):
                data[2]=0x04;
                break;
            case(STRATEGY_CADC):
                data[2]=0x02;
                break;
            case(STRATEGY_AUTO_CADC):
                data[2]=0x02;
                break;

//test for Amegids, 990322
            case(STRATEGY_ALLDYNAMIC):
                data[2]=0x02;
                break;
//    case(STRATEGY_ALLDYNAMIC): data[2]=0x30; break;

            case(STRATEGY_TOD):
                data[2]=0x01;
                break;
            }
            data[3] = 0x01;
        }
        else if (iInput == 0)
        {
            switch(_current_strategy)
            {
            case(STRATEGY_ALLRED):
                data[2]=0x04;
                break;
            case(STRATEGY_FLASH):
                data[2]=0x04;
                break;
            case(STRATEGY_MANUAL):
                data[2]=0x04;
                break;
            case(STRATEGY_CADC):
                data[2]=0x02;
                break;
            case(STRATEGY_AUTO_CADC):
                data[2]=0x02;
                break;
//test for Amegids, 990322
            case(STRATEGY_ALLDYNAMIC):
                data[2]=0x02;
                break;
//      case(STRATEGY_ALLDYNAMIC): data[2]=0x30; break;

            case(STRATEGY_TOD):
                data[2]=0x01;
                break;
            }
            data[3] = 0x00;
        }


        printf("%s[MESSAGE] Report Current Control Strategy to Center: %2d.%s\n", ColorGreen, _current_strategy, ColorNormal);

        /*+++++++++++++++++*/


        //OTCombo0713
        MESSAGEOK _MsgOK;
        _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, data_length, true);
        _MsgOK.InnerOrOutWard = cOutWard;
//  writeJob.WriteWorkByMESSAGEOUT(_MsgOK);
        writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICECENTER92);
        /*-----------------*/
//mallocFuck  free(data);

    }
    catch (...) {}
}
//----------------------------------------------------------
//**********************************************************
//****      Report Current Operation Mode to Center
//**********************************************************
//----------------------------------------------------------
void CSTC::ReportCurrentOperationMode_5F08(void)
{
    try
    {

        /*TEST
        unsigned short int data_length = 3;
        //mallocFuck  unsigned char *data = (unsigned char *)malloc( data_length*sizeof(unsigned char) );
        unsigned char data[4];
        data[0] = 0x5F;
        data[1] = 0x08;
        data[2] = 0x00;
        switch(_current_strategy){
          case(STRATEGY_ALLRED):    data[2]=0x02; break;
          case(STRATEGY_FLASH):     data[2]=0x40; break;
          case(STRATEGY_MANUAL):    data[2]=0x01; break;
          case(STRATEGY_ALLDYNAMIC): data[2]=0x00; break;
          case(STRATEGY_CADC):      data[2]=0x80; break;
          case(STRATEGY_AUTO_CADC): data[2]=0x80; break;
          case(STRATEGY_TOD):       data[2]=0x80; break;

          default:
          break;
        }

        if(data[2] == 0x80 && smem.vGetUCData(TC_TrainChainEnable) == 1) {  //Railway switch Enable.
          if(smem.vGetUCData(TC_TrainChainNOW) == 1) {  //train coming
            data[2] = 0x10;               //Define by CCT
          }
        }

        //OT990402
        if(smem.vGetThisCycleRunCCJPlan5F18() == true && _current_strategy == STRATEGY_TOD) { data[2] = 0x03; }
        if(smem.vGetThisCycleRunCCJActure5FA2() == true && _current_strategy == STRATEGY_TOD) { data[2] = 0x04; }
        */
        unsigned short int data_length = 3;
//mallocFuck  unsigned char *data = (unsigned char *)malloc( data_length*sizeof(unsigned char) );
        unsigned char data[4];
        data[0] = 0x5F;
        data[1] = 0x08;
        data[2] = 0x03;
        switch(_current_strategy)
        {

        case(STRATEGY_ALLRED):
            data[2]=0x02;
            break;
        case(STRATEGY_FLASH):
            data[2]=0x40;
            break;
        case(STRATEGY_MANUAL):
            data[2]=0x01;
            break;
        case(STRATEGY_ALLDYNAMIC):
            data[2]=0x00;
            break;
        case(STRATEGY_CADC):
            data[2]=0x80;
            break;
        case(STRATEGY_AUTO_CADC):
            data[2]=0x80;
            break;
        case(STRATEGY_TOD):
            data[2]=0x80;
            break;


        default:
            break;
        }

        if(data[2] == 0x80 && smem.vGetUCData(TC_TrainChainEnable) == 1)    //Railway switch Enable.
        {
            if(smem.vGetUCData(TC_TrainChainNOW) == 1)    //train coming
            {
                data[2] = 0x03;               //Define by CCT
            }
        }

//OT990402
        if(smem.vGetThisCycleRunCCJPlan5F18() == true && _current_strategy == STRATEGY_TOD)
        {
            data[2] = 0x03;
        }
        if(smem.vGetThisCycleRunCCJActure5FA2() == true && _current_strategy == STRATEGY_TOD)
        {
            data[2] = 0x04;
        }


        printf("%s[MESSAGE] Report Current Operation Mode to Center:   %2d.%s\n", ColorGreen, _current_strategy, ColorNormal);
        printf("[MESSAGE Debug] 5F18:%d, 5FA2:%d\n", smem.vGetThisCycleRunCCJPlan5F18(), smem.vGetThisCycleRunCCJActure5FA2());

        /*+++++++++++++++++*/

        //OTCombo0713

        smem.vSetTC5F08Status(data[2]);
        /*Send by CTimer check 990402

        MESSAGEOK _MsgOK;
        _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, data_length, true);
        //  _MsgOK.InnerOrOutWard = cOutWard;
        //  writeJob.WriteWorkByMESSAGEOUT(_MsgOK);
        writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICECENTER92);

        */

        /*-----------------*/
//mallocFuck  free(data);

    }
    catch (...) {}
}
//----------------------------------------------------------
//**********************************************************
//****      Report Current Hardware Status to Center
//**********************************************************
//----------------------------------------------------------
void CSTC::ReportCurrentHardwareStatus(void)
{
    try
    {
        /*+++++++++++++++++*/

        printf("Report Current Hardware Status to Center");

//  center.centerPort.writeErrorDetect(LCN);
        /*-----------------*/
    }
    catch(...) {}
}
//----------------------------------------------------------
//**********************************************************
//****     Calculate RedCount Data and Send it out
//**********************************************************
// should always be called after CalculatedCompensation()
// or in _current_strategy=(ALLRED, FLASH, MANUAL)
//----------------------------------------------------------
void CSTC::CalculateAndSendRedCount(const short int diff)    //聯嘉 紅燈倒數  using
{
    try
    {

        unsigned char ucSend[5];
        unsigned short int Data[8] = {0, 0, 0, 0, 0, 0, 0, 0};
        unsigned short int usiHCRedcountMapping[8] = { 0, 0, 0, 0, 0 };
        unsigned short int usiHKRedcountMapping[4] = {0, 0, 0, 0};

        bool bCountIF[8][8];   // 8output, 8subphase
        bool bCountIFEnable[8];   // 8output

        unsigned short int usiPhaseTmp;

        int iSubCnt;
        int iSignalCnt;
        int iSubCalCntTmp;
//no stepCnt, because only see step0

//OT20110624
        unsigned char ucRedCountRepeatCount = smem.vGetUCData(CSTC_RedcountRepeatCount);
        printf("ucRedCountRepeatCount:%d\n", ucRedCountRepeatCount);

//OT1000107
        unsigned char ucNextLightStatus[8];
        for(int i = 0; i < 8; i++)
        {
            ucNextLightStatus[i] = 255;
        }

        iSubCnt = _exec_phase._subphase_count;
        iSignalCnt = _exec_phase._signal_count;


        //printf("redcount, iSubCnt:%d, iSignalCnt:%d\n", iSubCnt, iSignalCnt);

        for(int i = 0; i < iSubCnt; i++)    //(分相)
        {
            for(int j = 0; j < iSignalCnt; j++)     //(卡數)
            {
                bCountIFEnable[j] = true;

                bCountIF[i][j] = false;

//            printf("light:%X\n", _exec_phase._ptr_subphase_step_signal_status[i][0][j]);

                usiPhaseTmp = _exec_phase._ptr_subphase_step_signal_status[i][0][j] & 0x03F0;  //when green light.
//            usiPhaseTmp = _exec_phase._ptr_subphase_step_signal_status[i][0][j] & 0xF003;  //when green light.

                //OT20140329, no left version!! case.2
                //usiPhaseTmp = _exec_phase._ptr_subphase_step_signal_status[i][0][j] & 0x03C0;  //when ^ / G light.

                if(usiPhaseTmp == 0)
                {
//              printf("i:%d, j:%d, bCountIF = true\n", i, j);
                    bCountIF[i][j] = true;
                }
            }
        }

        //OT20140329, only left version light!
        int last_i;
        bool bHaveOnlyLeft;
        printf("iSubCnt:%d, iSignalCnt:%d\n", iSubCnt, iSignalCnt);
        for(int i = 0; i < iSubCnt; i++)
        {
            for(int j = 0; j < iSignalCnt; j++)
            {
                bHaveOnlyLeft = false;
                if(j == 0)
                {
                    printf("light:%d:%X\n", i, _exec_phase._ptr_subphase_step_signal_status[i][0][j]);
                    printf("light:%d:%X\n", i, (_exec_phase._ptr_subphase_step_signal_status[i][0][j] & 0x03F0));
                }
                if( (_exec_phase._ptr_subphase_step_signal_status[i][0][j] & 0x03F0) == 0x0030 )
                {
                    bHaveOnlyLeft = true;
                }

                if(bHaveOnlyLeft == true)
                {
                    printf("have only left green in: %d\n", i);
                    last_i = i - 1;
                    if(last_i < 0)
                    {
                        last_i = iSubCnt - 1;
                    }
                    if(bCountIF[last_i][j] == true)
                    {
                        bCountIF[last_i][j] = false;
                    }
                }
            }
        }

        iSubCalCntTmp = 0;
        if( _exec_plan._phase_order==FLASH_PHASEORDER || _exec_plan._phase_order==ALLRED_PHASEORDER
                || _current_strategy==STRATEGY_MANUAL || diff!=0 || _current_strategy==STRATEGY_ALLDYNAMIC
                || _exec_plan._subphase_count > 8 || smem.vGetForceClockRedCountOneTime() == true)
        {
            smem.vSetForceClockRedCountOneTime(false);    //Do nothing
        }
        else
        {
            for(int i = _exec_phase_current_subphase; iSubCalCntTmp < iSubCnt; i++)
            {
                if(i == iSubCnt) i = 0;         //round..

                for(int j = 0; j < iSignalCnt; j++)
                {
//                printf("for while, i:%d, j:%d\n", i, j);
                    if(bCountIF[i][j] == false)      //don't shining, after
                    {
                        bCountIFEnable[j] = false;

                        //OT1000106
                        if(ucNextLightStatus[j] == 255)
                        {
                        }

//                    printf("i:%d bCountIFEnable[%d] = false\n", i, j);
                    }

                    /*
                                    if(bCountIF[i][j] == true) printf("bCountIF[%d][%d] = true\n", i, j);
                                    else printf("bCountIF[%d][%d] = false\n", i, j);
                                    if(bCountIFEnable[j] == true) printf("bCountIFEnable[%d] = true\n", j);
                                    else printf("bCountIFEnable[%d] = false\n", j);
                    */

                    if(bCountIF[i][j] == true && bCountIFEnable[j] == true)
                    {
                        Data[j] += _exec_plan._ptr_subplaninfo[i].compensated_phase_time(_exec_plan._shorten_cycle);
//                    printf("time++, i:%d, j:%d, t:%d\n", i, j,_exec_plan._ptr_subplaninfo[i].compensated_phase_time(_exec_plan._shorten_cycle));
                    }
                }

                iSubCalCntTmp++;
            }
        }


//OT1000107
//let double red left don't show it.
        /* not ok
          iSubCalCntTmp = 0;
          for(int i = _exec_phase_current_subphase; iSubCalCntTmp < iSubCnt; i++) {
            for(int j = 0; j < iSignalCnt; j++) {
              if(bCountIF[i][j] == false) {    //don't shining, after
                usiPhaseTmp = _exec_phase._ptr_subphase_step_signal_status[i][0][j] & 0x03F0;  //when green light.
                if(usiPhaseTmp == 0x0030) {
        //          bCountCleanLeft[j]
                  Data[j] = 0;
                }
            }
          }
        */


//recheck
        if( _exec_plan._phase_order == FLASH_PHASEORDER ||
                _exec_plan._phase_order == FLASH_PHASEORDER_HSINCHU ||
                _exec_plan._phase_order == ALLRED_PHASEORDER ||
                _exec_plan._phase_order == FLASH_PHASEORDER_HSINCHU2 ||
                _current_strategy==STRATEGY_MANUAL ||
                _current_strategy==STRATEGY_ALLDYNAMIC ||
                _exec_plan._phase_order ==0xB1 || _exec_plan._phase_order ==0xB0)  //arwen++ 1001206
        {
            for(int i = 0; i < 8; i++)
            {
                Data[i] = 0;
            }
        }
        if( smem.vGetBOOLData(TC_SignalConflictError) == true)
        {
            printf("TC SignalConflictError, Redcount Display Disable!\n");
            for(int i = 0; i < 8; i++)
            {
                Data[i] = 0;
            }
        }

        //OT980420
        if(     _exec_phase._ptr_subphase_step_signal_status[0][0][0] == 0x1004 ||
                _exec_phase._ptr_subphase_step_signal_status[0][0][1] == 0x1004 ||
                _exec_phase._ptr_subphase_step_signal_status[0][0][0] == 0x2008 ||
                _exec_phase._ptr_subphase_step_signal_status[0][0][1] == 0x2008 ||
                _exec_phase._ptr_subphase_step_signal_status[0][0][0] == 0x2002 ||
                _exec_phase._ptr_subphase_step_signal_status[0][0][1] == 0x2002    )
        {
            printf("Flash light!, no redcount\n");
            for(int i = 0; i < 8; i++)
            {
                Data[i] = 0;
            }
        }

        if(_exec_phase._subphase_count == 1 && _exec_phase._total_step_count == 1)
        {
            printf("special phase, no redcount\n");
            for(int i = 0; i < 8; i++)
            {
                Data[i] = 0;
            }
        }


        if( smem.vGetUCData(TC_Redcount_Display_Enable) == 0)
        {
            printf("\nTC Redcount Display Disable!\n");
            for(int i = 0; i < 8; i++)
            {
                Data[i] = 0;
            }
        }

        if(smem.vGetUCData(TC_TrainChainNOW) == 1)
        {
            printf("\nTrain coming, TC Redcount Display Disable!\n");
            for(int i = 0; i < 8; i++)
            {
                Data[i] = 0;
            }
        }

        for(int i = 0 ; i < 8; i++)
        {
            if( Data[i] > 255 )
            {
                Data[i] = 0;    //Data[i] only 1 Byte
            }
            if( Data[i] > 0)
            {
                Data[i] = Data[i]+1;    // add 1 sec
            }
        }

        printf("\n AABB2013    ********  RedCountTimer: %3d %3d %3d %3d %3d %3d %3d %3d********\n\n",
               Data[0],Data[1],Data[2],Data[3], Data[4],Data[5],Data[6], Data[7]);

        if( smem.vGetINTData(TC92_RedCountVer) == TC_RedCountVer94 )
        {
            //OT080422, -1 sec for LinChu Board
            for(int i = 0; i < 8; i++)
            {
                if(Data[i] > 0)
                {
                    Data[i]--;
                }
            }

            for(int i = 0; i < 5; i ++)
            {
                if(smem.vGetWayMappingRedCount(i) == 0)
                    usiHCRedcountMapping[i] = Data[0] & 0xFF;
                else if(smem.vGetWayMappingRedCount(i) == 1)
                    usiHCRedcountMapping[i] = Data[1] & 0xFF;
                else if(smem.vGetWayMappingRedCount(i) == 2)
                    usiHCRedcountMapping[i] = Data[2] & 0xFF;
                else if(smem.vGetWayMappingRedCount(i) == 3)
                    usiHCRedcountMapping[i] = Data[3] & 0xFF;
                else if(smem.vGetWayMappingRedCount(i) == 4)
                    usiHCRedcountMapping[i] = Data[4] & 0xFF;
                else if(smem.vGetWayMappingRedCount(i) == 5)
                    usiHCRedcountMapping[i] = Data[5] & 0xFF;
                else if(smem.vGetWayMappingRedCount(i) == 6)
                    usiHCRedcountMapping[i] = Data[6] & 0xFF;
                else if(smem.vGetWayMappingRedCount(i) == 7)
                    usiHCRedcountMapping[i] = Data[7] & 0xFF;
                else
                    usiHCRedcountMapping[i] = 0;
            }

            MESSAGEOK _MsgOK;
            _MsgOK = oDataToMessageOK.vPackageINFOToVer94RedCount
                     (0x82, usiHCRedcountMapping[0], usiHCRedcountMapping[1], usiHCRedcountMapping[2], usiHCRedcountMapping[3], usiHCRedcountMapping[4]);
            _MsgOK.InnerOrOutWard = cOutWard;

            for(int i = 0; i < ucRedCountRepeatCount; i++)
            {
                writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICEREDCOUNTVER94);
            }

            printf("\n     ********  Mapped RedCountTimer: %3d %3d %3d %3d %3d********\n\n", usiHCRedcountMapping[0],usiHCRedcountMapping[1],usiHCRedcountMapping[2],usiHCRedcountMapping[3], usiHCRedcountMapping[4]);
        }
        else if( smem.vGetINTData(TC92_RedCountVer) == TC_RedCountVerHK )
        {

            for(int i = 0; i < 4; i ++)
            {
                if(smem.vGetWayMappingRedCount(i) == 0)
                    usiHKRedcountMapping[i] = Data[0] & 0xFF;
                else if(smem.vGetWayMappingRedCount(i) == 1)
                    usiHKRedcountMapping[i] = Data[1] & 0xFF;
                else if(smem.vGetWayMappingRedCount(i) == 2)
                    usiHKRedcountMapping[i] = Data[2] & 0xFF;
                else if(smem.vGetWayMappingRedCount(i) == 3)
                    usiHKRedcountMapping[i] = Data[3] & 0xFF;
                else if(smem.vGetWayMappingRedCount(i) == 4)
                    usiHKRedcountMapping[i] = Data[4] & 0xFF;
                else if(smem.vGetWayMappingRedCount(i) == 5)
                    usiHKRedcountMapping[i] = Data[5] & 0xFF;
                else if(smem.vGetWayMappingRedCount(i) == 6)
                    usiHKRedcountMapping[i] = Data[6] & 0xFF;
                else if(smem.vGetWayMappingRedCount(i) == 7)
                    usiHKRedcountMapping[i] = Data[7] & 0xFF;
                else
                    usiHKRedcountMapping[i] = 0;
            }

            MESSAGEOK _MsgOK;
            _MsgOK = oDataToMessageOK.vPackageINFOToVerHKRedCount(usiHKRedcountMapping[0], usiHKRedcountMapping[1], usiHKRedcountMapping[2], usiHKRedcountMapping[3]);
            _MsgOK.InnerOrOutWard = cOutWard;

            for(int i = 0; i < ucRedCountRepeatCount; i++)
            {
                writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICEREDCOUNTVERHK);
            }

            printf("\n     ********  Mapped RedCountTimer: %3d %3d %3d %3d ********\n\n", usiHKRedcountMapping[0],usiHKRedcountMapping[1],usiHKRedcountMapping[2],usiHKRedcountMapping[3]);
        }
        else if( smem.vGetINTData(TC92_RedCountVer) == TC_RedCountVer94v2 )
        {
            for(int i = 0; i < 8; i ++)
            {
                if(smem.vGetWayMappingRedCount(i) == 0)
                    usiHCRedcountMapping[i] = Data[0] & 0xFF;
                else if(smem.vGetWayMappingRedCount(i) == 1)
                    usiHCRedcountMapping[i] = Data[1] & 0xFF;
                else if(smem.vGetWayMappingRedCount(i) == 2)
                    usiHCRedcountMapping[i] = Data[2] & 0xFF;
                else if(smem.vGetWayMappingRedCount(i) == 3)
                    usiHCRedcountMapping[i] = Data[3] & 0xFF;
                else if(smem.vGetWayMappingRedCount(i) == 4)
                    usiHCRedcountMapping[i] = Data[4] & 0xFF;
                else if(smem.vGetWayMappingRedCount(i) == 5)
                    usiHCRedcountMapping[i] = Data[5] & 0xFF;
                else if(smem.vGetWayMappingRedCount(i) == 6)
                    usiHCRedcountMapping[i] = Data[6] & 0xFF;
                else if(smem.vGetWayMappingRedCount(i) == 7)
                    usiHCRedcountMapping[i] = Data[7] & 0xFF;
                else
                    usiHCRedcountMapping[i] = 0;
            }

            MESSAGEOK _MsgOK;
            _MsgOK = oDataToMessageOK.vPackageINFOToVer94v2RedCount(0x82, usiHCRedcountMapping[0], usiHCRedcountMapping[1], usiHCRedcountMapping[2], usiHCRedcountMapping[3],
                     usiHCRedcountMapping[4], usiHCRedcountMapping[5], usiHCRedcountMapping[6], usiHCRedcountMapping[7]);
            _MsgOK.InnerOrOutWard = cOutWard;

//OT20110624
            for(int i = 0; i < ucRedCountRepeatCount; i++)
            {
                writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICEREDCOUNTVER94);
            }

            //OT980421
//cct should not twice.      writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICEREDCOUNTVER94);

            printf("\n     ********  Mapped RedCountTimer: %3d %3d %3d %3d %3d %3d %3d %3d********\n\n", usiHCRedcountMapping[0],usiHCRedcountMapping[1],usiHCRedcountMapping[2],usiHCRedcountMapping[3], usiHCRedcountMapping[4], usiHCRedcountMapping[5], usiHCRedcountMapping[6], usiHCRedcountMapping[7]);
        }
        else if( smem.vGetINTData(TC92_RedCountVer) == TC_RedCountVerCCT97 )
        {
            MESSAGEOK _MsgOK;
            for(int i = 0; i < 8; i ++)
            {
                if(smem.vGetWayMappingRedCount(i) == 0)
                    usiHCRedcountMapping[i] = Data[0] & 0xFF;
                else if(smem.vGetWayMappingRedCount(i) == 1)
                    usiHCRedcountMapping[i] = Data[1] & 0xFF;
                else if(smem.vGetWayMappingRedCount(i) == 2)
                    usiHCRedcountMapping[i] = Data[2] & 0xFF;
                else if(smem.vGetWayMappingRedCount(i) == 3)
                    usiHCRedcountMapping[i] = Data[3] & 0xFF;
                else if(smem.vGetWayMappingRedCount(i) == 4)
                    usiHCRedcountMapping[i] = Data[4] & 0xFF;
                else if(smem.vGetWayMappingRedCount(i) == 5)
                    usiHCRedcountMapping[i] = Data[5] & 0xFF;
                else if(smem.vGetWayMappingRedCount(i) == 6)
                    usiHCRedcountMapping[i] = Data[6] & 0xFF;
                else if(smem.vGetWayMappingRedCount(i) == 7)
                    usiHCRedcountMapping[i] = Data[7] & 0xFF;
                else
                    usiHCRedcountMapping[i] = 0;
            }

            ucSend[0] = 0xEA;
            ucSend[1] = 0x14;
            ucSend[2] = 8;
            for(int i = 0; i < 8; i++)
            {
                ucSend[3+i] = usiHCRedcountMapping[i];
            }
            _MsgOK = oDataToMessageOK.vPackageINFOTo92ProtocolSetADDR(ucSend, 11, 0x36, 65535);
            _MsgOK.InnerOrOutWard = cOutWard;

            for(int i = 0; i < ucRedCountRepeatCount; i++)
            {
                writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICEREDCOUNTVERCCT97);
            }
            //arwen++ 1001128
            _MsgOK = oDataToMessageOK.vPackageINFOTo92ProtocolSetADDR(ucSend, 11, 0x36, smem.GetAddress());
            _MsgOK.InnerOrOutWard = cOutWard;
            writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICECCJDYNCTL);//arwen++ 1001128

            /*
                  for(int i = 0; i < 8; i++) {  //set max RedCountID is 8
                      ucSend[0] = 0xEA;
                      ucSend[1] = 0x10;
                      ucSend[2] = usiHCRedcountMapping[i];
                      _MsgOK = oDataToMessageOK.vPackageINFOTo92ProtocolSetADDR(ucSend, 3, i);
                      _MsgOK.InnerOrOutWard = cOutWard;
                      writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICEREDCOUNTVERCCT97);
                  }
            */
//Send twice.
            /*
                  for(int i = 0; i < 8; i++) {  //set max RedCountID is 8
                      ucSend[0] = 0xEA;
                      ucSend[1] = 0x10;
                      ucSend[2] = usiHCRedcountMapping[i];
                      _MsgOK = oDataToMessageOK.vPackageINFOTo92ProtocolSetADDR(ucSend, 3, i);
                      _MsgOK.InnerOrOutWard = cOutWard;
                      writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICEREDCOUNTVERCCT97);
                  }
            */

            printf("\n     ********  Mapped RedCountTimer: %3d %3d %3d %3d %3d %3d %3d %3d********\n\n", usiHCRedcountMapping[0],usiHCRedcountMapping[1],usiHCRedcountMapping[2],usiHCRedcountMapping[3], usiHCRedcountMapping[4], usiHCRedcountMapping[5], usiHCRedcountMapping[6], usiHCRedcountMapping[7]);

        }

        //OT20131219
        for(int i = 0; i < 8; i ++)
        {
            if(smem.vGetWayMappingRedCount(i) == 0)
                usiHCRedcountMapping[i] = Data[0] & 0xFF;
            else if(smem.vGetWayMappingRedCount(i) == 1)
                usiHCRedcountMapping[i] = Data[1] & 0xFF;
            else if(smem.vGetWayMappingRedCount(i) == 2)
                usiHCRedcountMapping[i] = Data[2] & 0xFF;
            else if(smem.vGetWayMappingRedCount(i) == 3)
                usiHCRedcountMapping[i] = Data[3] & 0xFF;
            else if(smem.vGetWayMappingRedCount(i) == 4)
                usiHCRedcountMapping[i] = Data[4] & 0xFF;
            else if(smem.vGetWayMappingRedCount(i) == 5)
                usiHCRedcountMapping[i] = Data[5] & 0xFF;
            else if(smem.vGetWayMappingRedCount(i) == 6)
                usiHCRedcountMapping[i] = Data[6] & 0xFF;
            else if(smem.vGetWayMappingRedCount(i) == 7)
                usiHCRedcountMapping[i] = Data[7] & 0xFF;
            else
                usiHCRedcountMapping[i] = 0;
        }
        smem.vSetRedCountRemainder(usiHCRedcountMapping);

        int iTempScreenID;
        iTempScreenID = smem.GetcFace();
        if(iTempScreenID == cREDCOUNTHWCHECKSEL)
        {
            screenRedCountHWCheckSel.DisplayRedCountSec(
                usiHCRedcountMapping[0], usiHCRedcountMapping[1], usiHCRedcountMapping[2], usiHCRedcountMapping[3],
                usiHCRedcountMapping[4], usiHCRedcountMapping[5], usiHCRedcountMapping[6], usiHCRedcountMapping[7]);
        }

        //OT20110528
        smem.vSetShowRedCountVar(usiHCRedcountMapping[0], usiHCRedcountMapping[1], usiHCRedcountMapping[2], usiHCRedcountMapping[3],
                                 usiHCRedcountMapping[4], usiHCRedcountMapping[5], usiHCRedcountMapping[6], usiHCRedcountMapping[7]);


    }
    catch(...) {}
}

//------------------------------------------------------------------------------------------------------------------
void CSTC::CalculateAndSendNSPredCount(const short int diff)    //"Not Use" 紅燈倒數南科用 聯嘉小88 chou ++ 2013/06/03
{
    try
    {
        unsigned char ucSend[5];
        //unsigned short int usiHCRedcountMapping[8] = { 0, 0, 0, 0, 0 };
        //unsigned short int usiHKRedcountMapping[4] = {0, 0, 0, 0};

        bool bgreenCountIF[8][8];   // 8output, 8subphase
        bool bredCountIF[8][8];
        bool bgreenCountIFEnable[8];   // 8output
        bool bredCountIFEnable[8];   // 8output
        unsigned short int totalredtime[8] = {0,0,0,0,0,0,0,0};
        unsigned short int phaseredtime[8] = {0,0,0,0,0,0,0,0};
        int iSubCnt;
        int iSignalCnt;
        int iSubCalCntTmp;

        unsigned char ucNextLightStatus[8];
        for(int i = 0; i < 8; i++)
        {
            ucNextLightStatus[i] = 255;
            RedData[i] = 0;
            GreenData[i] = 0;                      //清空各燈號內內容
        }
        iSubCnt = _exec_phase._subphase_count;                  //紀錄有幾個phase
        iSignalCnt = _exec_phase._signal_count;                 //紀錄signal,大多為4

        // printf("\n\n\nsubphase_count = %d\n",iSubCnt);
        // printf("_signal_count = %d\n" ,iSignalCnt);

        // printf("redcount, iSubCnt:%d, iSignalCnt:%d\n", iSubCnt, iSignalCnt);

        for(int i = 0; i < iSubCnt; i++)
        {
            printf("i = %d\n",i);
            for(int j = 0; j < iSignalCnt; j++)
            {
                // bredCountIF[i][j] = false;
                bgreenCountIFEnable[j] = false;
                bredCountIFEnable[j] = true;

                //        printf("light:%X\n", _exec_phase._ptr_subphase_step_signal_status[i][0][j]);
                usigreenPhaseTmp[i][j] = _exec_phase._ptr_subphase_step_signal_status[i][0][j] & 0x0000;  //when PG light.判斷是否有行人綠燈
                usiredPhaseTmp[i][j]  = _exec_phase._ptr_subphase_step_signal_status[i][0][j] & 0x03F0;  //紅燈倒數
                //        printf("i = %d j = %d usiPhaseTmp = %x \n",i,j,usiredPhaseTmp[i][j]);
                if(usigreenPhaseTmp[i][j] == 0)
                {
                    bgreenCountIF[i][j] = true;
                    bgreenCountIFEnable[j] = true;
                }
                else
                {
                    bgreenCountIF[i][j] = false;
                }

                if(usiredPhaseTmp[i][j] == 0)
                {
                    bredCountIF[i][j] = true;
                    //      bredCountIFEnable[j] = true;
                }
                else
                {
                    bredCountIF[i][j] = false;
                }

            }
        }

        iSubCalCntTmp = 0;
        if( _exec_plan._phase_order==FLASH_PHASEORDER || _exec_plan._phase_order==ALLRED_PHASEORDER || _current_strategy==STRATEGY_MANUAL || diff!=0 || _current_strategy==STRATEGY_ALLDYNAMIC || _exec_plan._subphase_count > 8 || smem.vGetForceClockRedCountOneTime() == true)
        {
            //不會跳進IF  chou tip 2013/05/23
            //   printf("_exec_plan._phase_order = %d\n",_exec_plan._phase_order);
            smem.vSetForceClockRedCountOneTime(false);
        }  //Do nothing
        else                    //always    chou tip 2013/05/23
        {
            //printf("_exec_plan._phase_order = %d\n",_exec_plan._phase_order);
            //printf("test -- \n" );
            for(int i = _exec_phase_current_subphase; iSubCalCntTmp < iSubCnt; i++)
            {

                //for(int i = 0; iSubCalCntTmp < iSubCnt; i++) {
                //int setflag=0; //一個phase只會加一次 chou tip 2013/06/06
                if(i == iSubCnt) i = 0;         //round..
                //     printf("iSignalCnt = %d\n",iSignalCnt);
                for(int j = 0; j < iSignalCnt; j++)
                {
                    //bgreenCountIFEnable[j] = false;
                    //bredCountIFEnable[j] = true;
                    if(bredCountIF[i][j] == false)      //don't shining, after
                    {
                        bredCountIFEnable[j] = false;
                        if(ucNextLightStatus[j] == 255)
                        {
                        }
                    }
                    else {}


                    if(bredCountIF[i][j] == true/* && bredCountIFEnable[j] == true*/)
                    {
                        printf("totalredtime[%d] = %d + [%d]%d \n",j,totalredtime[j],i,_exec_plan._ptr_subplaninfo[i].compensated_phase_time(_exec_plan._shorten_cycle));
                        //    if(setflag >0){
                        //    }else{

                        totalredtime[j] += _exec_plan._ptr_subplaninfo[i].compensated_phase_time(_exec_plan._shorten_cycle);
                        //      setflag++;
                        //    }
                        if(totalredtime[j] > 0)
                        {
                            RedData[j] = totalredtime[j];
                        }
                        else {}
                        //         printf("bredCountIF[%d][%d] = %d\n",i,j,bredCountIF[i][j]);
                        //         printf("bredCountIFEnable[%d] = %d\n",j,bredCountIFEnable[j]);
                    }
                    else {}
                }
                iSubCalCntTmp++;
            }
            for(int i = _exec_phase_current_subphase; iSubCalCntTmp < iSubCnt; i++)
            {
                if(i == iSubCnt) i = 0;         //round..
                //printf("iSignalCnt = %d\n",iSignalCnt);
                /*      for(int j = 0; j < iSignalCnt; j++) {
                                  if(bpedredCountIF[i][j] == false) {
                                      bpedredCountIFEnable[j] = false;
                                  }else{}
                                  if(bpedredCountIF[i][j] == true && bpedredCountIFEnable[j] == true){
                                      totalredtime[j] += _exec_plan._ptr_subplaninfo[i].compensated_phase_time(_exec_plan._shorten_cycle);
                                        _exec_plan._ptr_subplaninfo[i].compensated_green(_exec_plan._shorten_cycle) +
                                                       _exec_plan._ptr_subplaninfo[i].compensated_pedgreen_flash(_exec_plan._shorten_cycle);
                                      printf("totalredtime[%d] = %d\n",j,totalredtime[j]);
                                  }else{
                                      phaseredtime[j] += _exec_plan._ptr_subplaninfo[i].compensated_pedred(_exec_plan._shorten_cycle);
                                      phaseredtime[j] += _exec_plan._ptr_subplaninfo[i].compensated_yellow(_exec_plan._shorten_cycle);
                                      phaseredtime[j] += _exec_plan._ptr_subplaninfo[i].compensated_allred(_exec_plan._shorten_cycle);
                                      printf("phaseredtime[%d] = %d\n",j,phaseredtime[j]);
                                  }
                      }
                */
                iSubCalCntTmp++;
            }
            /*         for(int l = 0;l < 4;l++){
                         if(RedData[l] != totalredtime[l] && totalredtime[l] >= 0)
                         {
                              RedData[l] = totalredtime[l] + phaseredtime[l]  ;
                         }else{}
                     }*/
            /*  }else{
                      if(_exec_phase_current_subphase_step == 4){                 //因為是綠人倒數只要綠燈秒數和綠閃秒數
                       Data[j] = _exec_plan._ptr_subplaninfo[i].compensated_green(_exec_plan._shorten_cycle) +
                       _exec_plan._ptr_subplaninfo[i].compensated_pedgreen_flash(_exec_plan._shorten_cycle) +
                       _exec_plan._ptr_subplaninfo[i].compensated_allred(_exec_plan._shorten_cycle) +
                       _exec_plan._ptr_subplaninfo[i].compensated_yellow(_exec_plan._shorten_cycle) ;
                       }else{}
              }*/
            //printf("bCountIF[%d][%d] = %d\n",i,j,bCountIF[i][j]);
            //printf("bCountIFEnable[%d] = %d\n",j,bCountIFEnable[j]);

            //         }else{}


        }
        for(int k = 0; k<=7; k++)    //2013/05/21 chou check 在每次PHASE一開始時給各燈頭秒數
        {
            if(RedData[k] > 0)
            {
                if(abs(smem.vgetredcount(k) - RedData[k]) > 2)              //2013/06/13 chou ++ 若時間變化太劇烈則暫停輸出 ex:phase 切換
                {
                    printf("smem.vgetredcount(%d) = %d RedData[%d] = %d \n",k,smem.vgetredcount(k),k,RedData[k]);
//OT1218                smem.SetPedopen(false);
                    smem.SetPedopen(true);
                }
                else {}
                smem.vsetredcount(0,k,RedData[k]);
            }
            else {}
            if(GreenData[k] > 0)
            {
                smem.vsetredcount(1,k,GreenData[k]);
            }
            else {}
            //printf("greenData[%d] = %d\n",k,smem.vgetgreencount(k));
        }
        for(int k = 0; k<=7; k++)                   //2013/05/21 chou check 在每次PHASE一開始時給各燈頭秒數
        {
            printf("redData[%d] = %d\n",k,smem.vgetredcount(k));
        }
    }
    catch(...) {}
}
//---------------------每秒送出紅燈倒數秒數-------------chou ++ 2013/05/29------------------------------------------
void CSTC::vSendNSPredcount()        //  Not use
{
    try
    {
        /*    unsigned short int m_greenman[8] = {0,0,0,0,0,0,0,0};               //紀錄綠人倒數時間
              unsigned short int m_redman[8] = {0,0,0,0,0,0,0,0};                 //紀錄紅人倒數時間
              unsigned short int m_greenremainder[8] = {0,0,0,0,0,0,0,0};         //紀錄行人倒數剩餘秒數
              bool Pedsign[8]  = {0,0,0,0,0,0,0,0};      */
        //紀錄現在燈號為綠人OR紅人 true = green false = red chou ++ 2013/05/29
        //printf("m_greenman = ");

        if(_exec_phase_current_subphase_step == 4)
        {
            for(int clr =0; clr <8 ; clr++)
            {
                redcountsign[clr] = false ;
            }
        }
        unsigned short int redcontrol[8] = {0,0,0,0,0,0,0,0};
        printf("\n[%d][%d] m_redremainder = ",_exec_phase_current_subphase,_exec_phase_current_subphase_step);
        for(int i = 0; i < 8 ; i ++)
        {
            m_greencount[i] = smem.vgetgreencount(i) ;
            m_redcount[i] = smem.vgetredcount(i);
            if(m_redremainder[i] > 0)
            {
                m_redremainder[i]--;

            }
            else {}
            printf(" %d ",m_redremainder[i]);
        }
        printf("\n");
        if( _exec_plan._phase_order==FLASH_PHASEORDER || _exec_plan._phase_order==ALLRED_PHASEORDER || _current_strategy==STRATEGY_MANUAL  || _current_strategy==STRATEGY_ALLDYNAMIC || _exec_plan._subphase_count > 8)
        {
        }
        else {}
        for(int phasecount=0; phasecount< _exec_phase._subphase_count; phasecount++)
        {
            for(int singalcount=0; singalcount < _exec_phase._signal_count ; singalcount++)
            {
                if(usiredPhaseTmp[phasecount][singalcount] == 0)
                {
                    switch(singalcount)
                    {
                    case 0:
                        redcontrol[phasecount] += 0x80;
                        break;
                    case 1:
                        redcontrol[phasecount] += 0x40;
                        break;
                    case 2:
                        redcontrol[phasecount] += 0x20;
                        break;
                    case 3:
                        redcontrol[phasecount] += 0x10;
                        break;
                    default:
                        break;
                    }
                }
                else {}
            }
        }
        unsigned int lightlist[8] = {0,0,0,0,0,0,0,0};  //紀錄各燈頭的顯示順序
        int countstart[8] = {0,0,0,0,0,0,0,0};         //紀錄該從第幾個phase開始倒數
        int lightSP[15] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};   //特別處理分相 專門處理lightlist = 5 chou++ 2013/06/14
        //case5 3phase 5/ case9 4phase 9 /case11 4phase 11 /case13 4phase 13/
        for(int singalcount=0; singalcount < _exec_phase._signal_count ; singalcount++)
        {
            if((redcontrol[singalcount] & 0x80) != 0x0)
            {
                switch(singalcount)
                {
                case 0:
                    lightlist[0] +=0x1;
                    break;
                case 1:
                    lightlist[0] +=0x2;
                    break;
                case 2:
                    lightlist[0] +=0x4;
                    break;
                case 3:
                    lightlist[0] +=0x8;
                    break;
                }
            }
            else {}
            if((redcontrol[singalcount] & 0x40) != 0x0)
            {
                switch(singalcount)
                {
                case 0:
                    lightlist[1] +=0x1;
                    break;
                case 1:
                    lightlist[1] +=0x2;
                    break;
                case 2:
                    lightlist[1] +=0x4;
                    break;
                case 3:
                    lightlist[1] +=0x8;
                    break;
                }
            }
            else {}
            if((redcontrol[singalcount] & 0x20) != 0x0)
            {
                switch(singalcount)
                {
                case 0:
                    lightlist[2] +=0x1;
                    break;
                case 1:
                    lightlist[2] +=0x2;
                    break;
                case 2:
                    lightlist[2] +=0x4;
                    break;
                case 3:
                    lightlist[2] +=0x8;
                    break;
                }
            }
            else {}
            if((redcontrol[singalcount] & 0x10) != 0x0)
            {
                switch(singalcount)
                {
                case 0:
                    lightlist[3] +=0x1;
                    break;
                case 1:
                    lightlist[3] +=0x2;
                    break;
                case 2:
                    lightlist[3] +=0x4;
                    break;
                case 3:
                    lightlist[3] +=0x8;
                    break;
                }
            }
            else {}
            for(int t=0; t<4; t++) //若lightlist==0x5 則紀錄是哪塊控制板 chou++ 2013/06/14
            {
                if(lightlist[t] == 0x5 && _exec_phase._subphase_count == 3)
                {
                    lightSP[5] = t ;
                }
                else {}
                if(lightlist[t] == 0x9 && _exec_phase._subphase_count == 4)
                {
                    lightSP[9] = t ;
                }
                else {}
                if(lightlist[t] == 0xb && _exec_phase._subphase_count == 4)
                {
                    lightSP[11] = t ;
                }
                else {}
                if(lightlist[t] == 0xd && _exec_phase._subphase_count == 4)
                {
                    lightSP[13] = t ;
                }
                else {}
            }
        }
        for(int p = 0; p<4; p++)
        {
            printf("lightlist[%d] = %x\n",p,lightlist[p]);
        }
        //printf("0x70 & 0x40 = %x",0x70 & 0x40);
        if(_exec_phase._subphase_count == 4)
        {
            for(int signalcount = 0; signalcount < _exec_phase._signal_count; signalcount++)
            {
                switch(lightlist[signalcount])
                {
                case 0:
                    break;
                case 0x1:
                    countstart[signalcount] = 0;
                    break;
                case 0x2:
                    countstart[signalcount] = 1;
                    break;
                case 0x3:
                    countstart[signalcount] = 0;
                    break;
                case 0x4:
                    countstart[signalcount] = 2;
                    break;
                case 0x5:
                    countstart[signalcount] = 0;
                    break;
                case 0x6:
                    countstart[signalcount] = 1;
                    break;
                case 0x7:
                    countstart[signalcount] = 0;
                    break;
                case 0x8:
                    countstart[signalcount] = 3;
                    break;
                case 0x9:
                    countstart[signalcount] = 3;
                    //sp
                    break;
                case 0xa:
                    countstart[signalcount] = 0;
                    break;
                case 0xb:
                    countstart[signalcount] = 3;
                    break;
                case 0xc:
                    countstart[signalcount] = 2;
                    break;
                case 0xd:
                    countstart[signalcount] = 2;
                    //sp
                    break;
                case 0xe:
                    countstart[signalcount] = 1;
                    break;
                case 0xf:
                    countstart[signalcount] = 0;
                    break;
                default:
                    break;
                }
                printf("countstart[%d] = %d\n",signalcount,countstart[signalcount]);
            }
        }
        else {}
        if(_exec_phase._subphase_count == 3)
        {
            for(int signalcount = 0; signalcount < _exec_phase._signal_count; signalcount++)
            {
                switch(lightlist[signalcount])
                {
                case 0:
                    break;
                case 1:
                    countstart[signalcount] = 0;
                    break;
                case 2:
                    countstart[signalcount] = 1;
                    break;
                case 3:
                    countstart[signalcount] = 0;
                    break;
                case 4:
                    countstart[signalcount] = 2;
                    break;
                case 5:
                    countstart[signalcount] = 2;
                    break;
                case 6:
                    countstart[signalcount] = 1;
                    break;
                case 7:
                    countstart[signalcount] = 0;
                    break;
                default:
                    break;
                }
                printf("countstart[%d] = %d\n",signalcount,countstart[signalcount]);
            }
        }
        else
        {
            if(_exec_phase._subphase_count == 2)
            {
                for(int signalcount = 0; signalcount < _exec_phase._signal_count; signalcount++)
                {
                    countstart[signalcount] = lightlist[signalcount]-1;

                }
            }
            else {}
        }

        printf("redcontrol[%d] = %x \n",_exec_phase_current_subphase,redcontrol[_exec_phase_current_subphase]);
        if(_exec_phase_current_subphase_step == 0 && redcountsign[_exec_phase_current_subphase] == false)
        {
            printf("\n\ntest\n\n");
            redcountsign[_exec_phase_current_subphase] = true;
            switch(redcontrol[_exec_phase_current_subphase])
            {
            case 0x00:
                break;
            case 0x10:
                if(m_redremainder[3] != 0 && countstart[3] != _exec_phase_current_subphase)
                {
                }
                else
                {
                    m_redremainder[3] = m_redcount[3];
                }
                break;
            case 0x20:
                if(m_redremainder[2] != 0 && countstart[2] != _exec_phase_current_subphase)
                {
                }
                else
                {
                    m_redremainder[2] = m_redcount[2];
                }
                break;
            case 0x30:
                if(m_redremainder[2] != 0 && countstart[2] != _exec_phase_current_subphase)
                {
                }
                else
                {
                    m_redremainder[2] = m_redcount[2];
                }
                if(m_redremainder[3] != 0 && countstart[3] != _exec_phase_current_subphase)
                {
                }
                else
                {
                    m_redremainder[3] = m_redcount[3];
                }
                break;
            case 0x40:
                if(m_redremainder[1] != 0 && countstart[1] != _exec_phase_current_subphase)
                {
                }
                else
                {
                    m_redremainder[1] = m_redcount[1];
                }
                break;
            case 0x50:
                if(m_redremainder[1] != 0 && (countstart[1] != _exec_phase_current_subphase))
                {
                }
                else
                {
                    m_redremainder[1] = m_redcount[1];
                }
                if(m_redremainder[3] != 0 && (countstart[3] != _exec_phase_current_subphase))
                {
                }
                else
                {
                    m_redremainder[3] = m_redcount[3];
                }
                break;
            case 0x60:
                if(m_redremainder[1] != 0 && countstart[1] != _exec_phase_current_subphase)
                {
                }
                else
                {
                    m_redremainder[1] = m_redcount[1];
                }
                if(m_redremainder[2] != 0 && countstart[2] != _exec_phase_current_subphase)
                {
                }
                else
                {
                    m_redremainder[2] = m_redcount[2];
                }
                break;
            case 0x70:
                if((m_redremainder[1] != 0) && (countstart[1] != _exec_phase_current_subphase))
                {
                }
                else
                {
                    m_redremainder[1] = m_redcount[1];
                }
                if(m_redremainder[2] != 0 && (countstart[2] != _exec_phase_current_subphase))
                {
                }
                else
                {
                    m_redremainder[2] = m_redcount[2];
                }
                if(m_redremainder[3] != 0 && (countstart[3] != _exec_phase_current_subphase))
                {
                }
                else
                {
                    m_redremainder[3] = m_redcount[3];
                }
                break;
            case 0x80:
                if(m_redremainder[0] != 0 && (countstart[0] != _exec_phase_current_subphase))
                {
                }
                else
                {
                    m_redremainder[0] = m_redcount[0];
                }
                break;
            case 0x90:
                if(m_redremainder[0] != 0 && (countstart[0] != _exec_phase_current_subphase))
                {
                }
                else
                {
                    m_redremainder[0] = m_redcount[0];
                }
                if(m_redremainder[3] != 0 && (countstart[3] != _exec_phase_current_subphase))
                {
                }
                else
                {
                    m_redremainder[3] = m_redcount[3];
                }
                break;
            case 0xA0:
                if(m_redremainder[0] != 0 && (countstart[0] != _exec_phase_current_subphase))
                {
                }
                else
                {
                    m_redremainder[0] = m_redcount[0];
                }
                if(m_redremainder[2] != 0 && (countstart[2] != _exec_phase_current_subphase))
                {
                }
                else
                {
                    m_redremainder[2] = m_redcount[2];
                }
                break;
            case 0xB0:
                if(m_redremainder[0] != 0 && countstart[0] != _exec_phase_current_subphase)
                {
                }
                else
                {
                    m_redremainder[0] = m_redcount[0];
                }
                if(m_redremainder[2] != 0 && countstart[2] != _exec_phase_current_subphase)
                {
                }
                else
                {
                    m_redremainder[2] = m_redcount[2];
                }
                if(m_redremainder[3] != 0 && countstart[3] != _exec_phase_current_subphase)
                {
                }
                else
                {
                    m_redremainder[3] = m_redcount[3];
                }
                break;
            case 0xC0:
                if(m_redremainder[0] != 0 && countstart[0] != _exec_phase_current_subphase)
                {
                }
                else
                {
                    m_redremainder[0] = m_redcount[0];
                }
                if(m_redremainder[1] != 0 && countstart[1] != _exec_phase_current_subphase)
                {
                }
                else
                {
                    m_redremainder[1] = m_redcount[1];
                }
                break;
            case 0xD0:
                if(m_redremainder[0] != 0 && countstart[0] != _exec_phase_current_subphase)
                {
                }
                else
                {
                    m_redremainder[0] = m_redcount[0];
                }
                if(m_redremainder[1] != 0 && countstart[1] != _exec_phase_current_subphase)
                {
                }
                else
                {
                    m_redremainder[1] = m_redcount[1];
                }
                if(m_redremainder[3] != 0 && countstart[3] != _exec_phase_current_subphase)
                {
                }
                else
                {
                    m_redremainder[3] = m_redcount[3];
                }
                break;
            case 0xE0:
                if(m_redremainder[0] != 0 && countstart[0] != _exec_phase_current_subphase)
                {
                }
                else
                {
                    m_redremainder[0] = m_redcount[0];
                }
                if(m_redremainder[1] != 0 && countstart[1] != _exec_phase_current_subphase)
                {
                }
                else
                {
                    m_redremainder[1] = m_redcount[1];
                }
                if(m_redremainder[2] != 0 && countstart[2] != _exec_phase_current_subphase)
                {
                }
                else
                {
                    m_redremainder[2] = m_redcount[2];
                }
                break;
            case 0xF0:
                if(m_redremainder[0] != 0 && countstart[0] != _exec_phase_current_subphase)
                {
                }
                else
                {
                    m_redremainder[0] = m_redcount[0];
                }
                if(m_redremainder[1] != 0 && countstart[1] != _exec_phase_current_subphase)
                {
                }
                else
                {
                    m_redremainder[1] = m_redcount[1];
                }
                if(m_redremainder[2] != 0 && countstart[2] != _exec_phase_current_subphase)
                {
                }
                else
                {
                    m_redremainder[2] = m_redcount[2];
                }
                if(m_redremainder[3] != 0 && countstart[3] != _exec_phase_current_subphase)
                {
                }
                else
                {
                    m_redremainder[3] = m_redcount[3];
                }
                break;
            default:
                break;
            }
            if(_exec_phase._subphase_count == 3 && lightlist[lightSP[5]] == 0x5 && m_redremainder[lightSP[5]] != 0 && countstart[lightSP[5]] != _exec_phase_current_subphase)
            {
                printf("\n\ncase 5\n\n");
                m_redremainder[lightSP[5]] = _exec_plan._ptr_subplaninfo[0].compensated_phase_time(_exec_plan._shorten_cycle);
            }
            else
            {
                printf("lightSP = %d\n",lightSP);
            }
            if(_exec_phase._subphase_count == 4)
            {
                if(lightlist[lightSP[9]] == 0x9 && m_redremainder[lightSP[9]] != 0 && countstart[lightSP[9]] != lightSP[9] && _exec_phase_current_subphase == 0)
                {
                    m_redremainder[lightSP[9]] = _exec_plan._ptr_subplaninfo[0].compensated_phase_time(_exec_plan._shorten_cycle);
                }
                else {}
                if(lightlist[lightSP[11]] == 0xb && m_redremainder[lightSP[11]] != 0 && countstart[lightSP[11]] != lightSP[11] && _exec_phase_current_subphase == 0)
                {
                    m_redremainder[lightSP[11]] = _exec_plan._ptr_subplaninfo[0].compensated_phase_time(_exec_plan._shorten_cycle) +
                                                  _exec_plan._ptr_subplaninfo[1].compensated_phase_time(_exec_plan._shorten_cycle);
                }
                else {}
                if(lightlist[lightSP[13]] == 0xd && m_redremainder[lightSP[13]] != 0 && countstart[lightSP[13]] != _exec_phase_current_subphase && _exec_phase_current_subphase == 0)
                {
                    m_redremainder[lightSP[13]] = _exec_plan._ptr_subplaninfo[0].compensated_phase_time(_exec_plan._shorten_cycle);
                }
                else {}

            }
            else {}
        }
        else {}
        MESSAGEOK _MsgOK;
        // printf("\n\n");
        if( _exec_plan._phase_order==FLASH_PHASEORDER || _exec_plan._phase_order==ALLRED_PHASEORDER || _current_strategy==STRATEGY_MANUAL  || _current_strategy==STRATEGY_ALLDYNAMIC || _exec_plan._subphase_count > 8)
        {
            _MsgOK = oDataToMessageOK.vPackageINFOToredcountNSP(99,99,99,99,99,99,99,99 , 0);
            _MsgOK.InnerOrOutWard = cOutWard;

            if(smem.GetPedopen())
            {
                writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICENSPREDCOUNT);
            }

            _MsgOK = oDataToMessageOK.vPackageINFOToredcountNSP(99,99,99,99,99,99,99,99 , 1);
            _MsgOK.InnerOrOutWard = cOutWard;

            if(smem.GetPedopen())
            {
                writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICENSPREDCOUNT);
            }
        }
        else
        {
            int redchgINT[8] = {0,0,0,0,0,0,0,0};
            int redtmpH = 0;
            int redtmpL = 0;
            printf("redchgINT = ");
            for(int i = 0; i<4; i++)
            {
                redtmpH = m_redremainder[i] / 100;
                redtmpL = m_redremainder[i] % 100;
                printf(" \i= %d ",i);
                if(redtmpH > 10)
                {
                    redchgINT[2*i] = (redtmpH / 10 * 16) +  (redtmpH % 10);
                }
                else
                {
                    redchgINT[2*i] = redtmpH % 10;
                }
                if(redtmpL > 10)
                {
                    redchgINT[2*i+1] = (redtmpL / 10 * 16) +  (redtmpL % 10);
                }
                else
                {
                    redchgINT[2*i+1] = redtmpL % 10;
                }
                printf(" %d %d",redchgINT[2*i],redchgINT[2*i +1]);
            }
            printf("\n");
            _MsgOK = oDataToMessageOK.vPackageINFOToredcountNSP(redchgINT[0],redchgINT[1], redchgINT[2],redchgINT[3],redchgINT[4],redchgINT[5],redchgINT[6],redchgINT[7], 0);
            _MsgOK.InnerOrOutWard = cOutWard;

            if(smem.GetPedopen())
            {
                writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICENSPREDCOUNT);
            }

            //--
            int redchgINT2[8] = {0,0,0,0,0,0,0,0};
            redtmpH = 0;
            redtmpL = 0;
            printf("redchgINT2 = ");
            for(int i = 0; i<4; i++)
            {
                redtmpH = m_redremainder[i+4] / 100;
                redtmpL = m_redremainder[i+4] % 100;
                printf(" \i= %d ",i);
                if(redtmpH > 10)
                {
                    redchgINT2[2*i] = (redtmpH / 10 * 16) +  (redtmpH % 10);
                }
                else
                {
                    redchgINT2[2*i] = redtmpH % 10;
                }
                if(redtmpL > 10)
                {
                    redchgINT2[2*i+1] = (redtmpL / 10 * 16) +  (redtmpL % 10);
                }
                else
                {
                    redchgINT2[2*i+1] = redtmpL % 10;
                }
                printf(" %d %d",redchgINT2[2*i],redchgINT2[2*i +1]);
            }
            printf("\n");
            _MsgOK = oDataToMessageOK.vPackageINFOToredcountNSP(redchgINT2[0],redchgINT2[1], redchgINT2[2],redchgINT2[3],redchgINT2[4],redchgINT2[5],redchgINT2[6],redchgINT2[7], 0);
            _MsgOK.InnerOrOutWard = cOutWard;

            if(smem.GetPedopen())
            {
                writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICENSPREDCOUNT);
            }
        }

        // m_greenman[8];
        // m_greenremainder[8];
    }
    catch(...) {}
}

//------------------------------------------------------------------------------------------------------------------
void CSTC::CalculateAndSendGreenmanCount(const short int diff)      //計算綠人燈倒數 chou++ 2013/05/17   "Not use"
{
    try
    {
        unsigned char ucSend[5];
        //unsigned short int usiHCRedcountMapping[8] = { 0, 0, 0, 0, 0 };
        //unsigned short int usiHKRedcountMapping[4] = {0, 0, 0, 0};

        bool bpedgreenCountIF[8][8];   // 8output, 8subphase
        bool bpedredCountIF[8][8];
        bool bpedgreenCountIFEnable[8];   // 8output
        bool bpedredCountIFEnable[8];   // 8output
        unsigned short int totalgreentime[8] = {0,0,0,0,0,0,0,0};
        unsigned short int totalredtime[8] = {0,0,0,0,0,0,0,0};
        int iSubCnt;
        int iSignalCnt;
        int iSubCalCntTmp;
        int isetphase[8] = {9,9,9,9,9,9,9,9};   //紀錄第一次加總的phase值
        unsigned char ucNextLightStatus[8];
        for(int i = 0; i < 8; i++)
        {
            ucNextLightStatus[i] = 255;
            pedRedData[i] = 0;
            pedGreenData[i] = 0;                      //清空各燈號內內容
        }
        iSubCnt = _exec_phase._subphase_count;                  //紀錄有幾個phase
        iSignalCnt = _exec_phase._signal_count;                 //紀錄signal,大多為4

        //printf("\n\n\nsubphase_count = %d\n",iSubCnt);
        //printf("_signal_count = %d\n" ,iSignalCnt);

        //printf("redcount, iSubCnt:%d, iSignalCnt:%d\n", iSubCnt, iSignalCnt);

        for(int i = 0; i < iSubCnt; i++)
        {
            //  printf("i = %d\n",i);
            for(int j = 0; j < iSignalCnt; j++)
            {
                bpedgreenCountIF[i][j] = false;
                bpedgreenCountIFEnable[j] = true;
                bpedredCountIFEnable[j] = true;

                printf("light:%X\n", _exec_phase._ptr_subphase_step_signal_status[i][0][j]);
                usipedgreenPhaseTmp[i][j] = _exec_phase._ptr_subphase_step_signal_status[i][0][j] & 0x3000;  //when PG light.判斷是否有行人綠燈,左轉燈保護
                usipedredPhaseTmp[i][j]   = _exec_phase._ptr_subphase_step_signal_status[i][0][j] & 0xC000;  //行人紅燈
                printf("i = %d j = %d usiPhaseTmp = %x \n",i,j,usipedredPhaseTmp);
                if(usipedgreenPhaseTmp[i][j] == 0)
                {
                    bpedgreenCountIF[i][j] = true;
                }
                else
                {
                    bpedgreenCountIF[i][j] = false;
                }
                if(usipedredPhaseTmp[i][j] == 0)
                {
                    bpedredCountIF[i][j] = true;
                }
                else
                {
                    bpedredCountIF[i][j] = false;
                }

            }
        }

        iSubCalCntTmp = 0;
        if( _exec_plan._phase_order==FLASH_PHASEORDER || _exec_plan._phase_order==ALLRED_PHASEORDER || _current_strategy==STRATEGY_MANUAL || diff!=0 || _current_strategy==STRATEGY_ALLDYNAMIC || _exec_plan._subphase_count > 8 || smem.vGetForceClockRedCountOneTime() == true)
        {
            //不會跳進IF  chou tip 2013/05/23
            //printf("_exec_plan._phase_order = %d\n",_exec_plan._phase_order);
            smem.vSetForceClockRedCountOneTime(false);
        }  //Do nothing
        else                    //always    chou tip 2013/05/23
        {
            //printf("_exec_plan._phase_order = %d\n",_exec_plan._phase_order);
            //printf("test -- \n" );
            for(int i = _exec_phase_current_subphase; iSubCalCntTmp < iSubCnt; i++)
            {
                //int setflag=0; //一個phase只會加一次 chou tip 2013/06/06
                if(i == iSubCnt) i = 0;         //round..
                //printf("iSignalCnt = %d\n",iSignalCnt);
                for(int j = 0; j < iSignalCnt; j++)
                {
                    if(bpedgreenCountIF[i][j] == false)      //don't shining, after
                    {
                        bpedgreenCountIFEnable[j] = false;
                        if(ucNextLightStatus[j] == 255)
                        {
                        }
                    }
                    else {}

                    if(bpedgreenCountIF[i][j] == true/* && bpedgreenCountIFEnable[j] == true*/)
                    {
                        /*    if(_exec_plan._ptr_subplaninfo[i].compensated_pedred(_exec_plan._shorten_cycle) != 0)
                            {*/
                        //            if(_exec_phase_current_subphase_step == 3){                 //因為是綠人倒數只要綠燈秒數和綠閃秒數
                        printf("totalgreentime[%d] = %d + [%d]%d \n",j,totalgreentime[j],i,(_exec_plan._ptr_subplaninfo[i].compensated_green(_exec_plan._shorten_cycle)+_exec_plan._ptr_subplaninfo[i].compensated_pedgreen_flash(_exec_plan._shorten_cycle)));

                        totalgreentime[j] += _exec_plan._ptr_subplaninfo[i].compensated_green(_exec_plan._shorten_cycle) +
                                             _exec_plan._ptr_subplaninfo[i].compensated_pedgreen_flash(_exec_plan._shorten_cycle);
                        if(isetphase[j] == 9)
                        {
                            isetphase[j] = i; //把第一次紀錄的phase值給記錄下來
                        }
                        else
                        {
                            if(isetphase[j] != i)
                            {
                                totalgreentime[j] += _exec_plan._ptr_subplaninfo[isetphase[j]].compensated_allred(_exec_plan._shorten_cycle);
                                totalgreentime[j] += _exec_plan._ptr_subplaninfo[isetphase[j]].compensated_pedred(_exec_plan._shorten_cycle);
                                totalgreentime[j] += _exec_plan._ptr_subplaninfo[isetphase[j]].compensated_yellow(_exec_plan._shorten_cycle);
                                isetphase[j] = i;
                            }
                            else {}
                        }
                        /*}else{
                            if(i > isetphase[j]){
                                totalgreentime[j] += _exec_plan._ptr_subplaninfo[i].compensated_phase_time(_exec_plan._shorten_cycle);
                                totalgreentime[j] -= (_exec_plan._ptr_subplaninfo[i].compensated_allred(_exec_plan._shorten_cycle)
                                + _exec_plan._ptr_subplaninfo[i].compensated_yellow(_exec_plan._shorten_cycle)
                                + _exec_plan._ptr_subplaninfo[i].compensated_pedred(_exec_plan._shorten_cycle));
                            }else{
                                totalgreentime[j] += _exec_plan._ptr_subplaninfo[i].compensated_phase_time(_exec_plan._shorten_cycle);
                                totalgreentime[j] -= (_exec_plan._ptr_subplaninfo[isetphase[j]].compensated_allred(_exec_plan._shorten_cycle)
                                + _exec_plan._ptr_subplaninfo[isetphase[j]].compensated_yellow(_exec_plan._shorten_cycle)
                                + _exec_plan._ptr_subplaninfo[isetphase[j]].compensated_pedred(_exec_plan._shorten_cycle));
                            }
                        }*/
                        /*totalgreentime[j] +=  _exec_plan._ptr_subplaninfo[i].compensated_green(_exec_plan._shorten_cycle) +
                                        _exec_plan._ptr_subplaninfo[i].compensated_pedgreen_flash(_exec_plan._shorten_cycle);*/

                        /* if(_exec_plan._ptr_subplaninfo[i].compensated_pedred(_exec_plan._shorten_cycle) == 0 && _exec_plan._ptr_subplaninfo[i].compensated_yellow(_exec_plan._shorten_cycle) == 0 && _exec_plan._ptr_subplaninfo[i].compensated_allred(_exec_plan._shorten_cycle) == 0){
                             totalgreentime[j] += _exec_plan._ptr_subplaninfo[i+1].compensated_green(_exec_plan._shorten_cycle) +
                                                  _exec_plan._ptr_subplaninfo[i+1].compensated_pedgreen_flash(_exec_plan._shorten_cycle);
                         }else{}*/
                        if(totalgreentime[j] > 0)
                        {
                            pedGreenData[j] = totalgreentime[j];
                        }
                        else {}
                        // setflag++; //一個phase只會加一次 chou tip 2013/06/06
                    }
                    else {}

                }

                iSubCalCntTmp++;
            }
            unsigned short int totalredtime[8] = {0,0,0,0,0,0,0,0};
            unsigned short int phaseredtime[8] = {0,0,0,0,0,0,0,0};
            for(int i = _exec_phase_current_subphase; iSubCalCntTmp < iSubCnt; i++)
            {
                if(i == iSubCnt) i = 0;         //round..
                //printf("iSignalCnt = %d\n",iSignalCnt);
                for(int j = 0; j < iSignalCnt; j++)
                {
                    if(bpedredCountIF[i][j] == false)
                    {
                        bpedredCountIFEnable[j] = false;
                    }
                    else {}
                    if(bpedredCountIF[i][j] == true && bpedredCountIFEnable[j] == true)
                    {
                        totalredtime[i] += _exec_plan._ptr_subplaninfo[i].compensated_phase_time(_exec_plan._shorten_cycle);
                        //                  printf("totalredtime[%d] = %d\n",j,totalredtime[j]);
                    }
                    else
                    {
                        phaseredtime[i] += _exec_plan._ptr_subplaninfo[i].compensated_pedred(_exec_plan._shorten_cycle);
                        phaseredtime[i] += _exec_plan._ptr_subplaninfo[i].compensated_yellow(_exec_plan._shorten_cycle);
                        phaseredtime[i] += _exec_plan._ptr_subplaninfo[i].compensated_allred(_exec_plan._shorten_cycle);
                        //                  printf("phaseredtime[%d] = %d\n",j,phaseredtime[j]);
                    }
                }

                iSubCalCntTmp++;
            }
            for(int l = 0; l < 4; l++)
            {
                if(pedRedData[l] != totalredtime[l] && totalredtime[l] >= 0)
                {
                    pedRedData[l] = totalredtime[l] + phaseredtime[l]  ;
                }
                else {}
            }
            /*  }else{
                      if(_exec_phase_current_subphase_step == 4){                 //因為是綠人倒數只要綠燈秒數和綠閃秒數
                       Data[j] = _exec_plan._ptr_subplaninfo[i].compensated_green(_exec_plan._shorten_cycle) +
                       _exec_plan._ptr_subplaninfo[i].compensated_pedgreen_flash(_exec_plan._shorten_cycle) +
                       _exec_plan._ptr_subplaninfo[i].compensated_allred(_exec_plan._shorten_cycle) +
                       _exec_plan._ptr_subplaninfo[i].compensated_yellow(_exec_plan._shorten_cycle) ;
                       }else{}
              }*/
            //printf("bCountIF[%d][%d] = %d\n",i,j,bCountIF[i][j]);
            //printf("bCountIFEnable[%d] = %d\n",j,bCountIFEnable[j]);

            //         }else{}


        }
        for(int k = 0; k<=7; k++)    //2013/05/21 chou check 在每次PHASE一開始時給各燈頭秒數
        {
            if(pedRedData[k] > 0)
            {
                smem.vsetGreenmancount(0,k,pedRedData[k]);
            }
            else {}
            if(pedGreenData[k] > 0)
            {
                smem.vsetGreenmancount(1,k,pedGreenData[k]);
            }
            else {}
            printf("pedgreenData[%d] = %d\n",k,smem.vgetGreenmancount(k));
        }
        //for(int k = 0;k<=7;k++){                    //2013/05/21 chou check 在每次PHASE一開始時給各燈頭秒數
        //printf("pedredData[%d] = %d\n",k,smem.vgetredmancount(k));
        //}
        //--------------------------------------------------------------------------------------------------------
        //----------------------------------------------------------------------------------------------------------
    }
    catch(...) {}
}

//---------------------每秒送出綠人倒數秒數-------------chou ++ 2013/05/29-----"Not use"------------------------------------
void CSTC::vsendPedgreencount()
{
    try
    {
        unsigned short int greencontrol[4] = {0,0,0,0};                             //紀錄綠人倒數執行分相
        unsigned short int redcontrol[4] = {0,0,0,0};                               //紀錄紅人倒數執行分相

        if(_exec_phase_current_subphase_step == 4)          //全紅時作flag清除
        {
            for(int clr=0; clr < 4 ; clr++)
            {
                if(Pedsign[clr] !=false)
                {
                    Pedsign[clr] = false;
                }
                else {}
            }
        }
        else {}
        /*    unsigned short int m_greenman[8] = {0,0,0,0,0,0,0,0};               //紀錄綠人倒數時間
              unsigned short int m_redman[8] = {0,0,0,0,0,0,0,0};                 //紀錄紅人倒數時間
              unsigned short int m_greenremainder[8] = {0,0,0,0,0,0,0,0};         //紀錄行人倒數剩餘秒數
              bool Pedsign[8]  = {0,0,0,0,0,0,0,0};      */
        //紀錄現在燈號為綠人OR紅人 true = green false = red chou ++ 2013/05/29
        //printf("m_greenman = ");
        if( _exec_plan._phase_order==FLASH_PHASEORDER || _exec_plan._phase_order==ALLRED_PHASEORDER || _current_strategy==STRATEGY_MANUAL  || _current_strategy==STRATEGY_ALLDYNAMIC || _exec_plan._subphase_count > 8)
        {
            for(int i = 0; i < 8 ; i++)
            {
                Pedsign[i] = false;
            }
            pedcyclecount = 1;
            smem.SetPedopen(false);
        }
        else {}
        printf("m_pedgreenman = ");
        for(int i = 0; i<4; i++)
        {
            m_pedgreenman[i] = smem.vgetGreenmancount(i)  ;
            m_pedredman[i] = smem.vgetredmancount(i);
            printf(" %d ",m_pedgreenman[i]);
        }
        printf("\n");

        printf("m_pedredman = ");
        for(int i = 0; i<4; i++)
        {
            printf(" %d ",m_pedredman[i]);
        }
        printf("\n");

        for(int phasecount=0; phasecount< _exec_phase._subphase_count; phasecount++)
        {
            for(int singalcount=0; singalcount < _exec_phase._signal_count ; singalcount++)
            {
                if(usipedgreenPhaseTmp[phasecount][singalcount] == 0)
                {
                    // 0: means that no Pr lighting.
                    switch(singalcount)
                    {
                    case 0:
                        greencontrol[phasecount] += 0x80;  // greencontrol is Phase1 in protocol AB0A
                        break;
                    case 1:
                        greencontrol[phasecount] += 0x40;
                        break;
                    case 2:
                        greencontrol[phasecount] += 0x20;
                        break;
                    case 3:
                        greencontrol[phasecount] += 0x10;
                        break;
                    default:
                        break;
                    }
                }
                else {}
            }
        }
        unsigned int lightlist[8] = {0,0,0,0,0,0,0,0};  //紀錄各燈頭的顯示順序
        int countstart[8] = {0,0,0,0,0,0,0,0};         //紀錄該從第幾個phase開始倒數
        for(int singalcount=0; singalcount < _exec_phase._signal_count ; singalcount++)
        {
            if((greencontrol[singalcount] & 0x80) != 0x0)   //first light borad is Pg.
            {
                switch(singalcount)
                {
                case 0:
                    lightlist[0] +=0x1;
                    break;
                case 1:
                    lightlist[0] +=0x2;
                    break;
                case 2:
                    lightlist[0] +=0x4;
                    break;
                case 3:
                    lightlist[0] +=0x8;
                    break;
                }
            }
            else {}
            if((greencontrol[singalcount] & 0x40) != 0x0)   //second light borad is Pg.
            {
                switch(singalcount)
                {
                case 0:
                    lightlist[1] +=0x1;
                    break;
                case 1:
                    lightlist[1] +=0x2;
                    break;
                case 2:
                    lightlist[1] +=0x4;
                    break;
                case 3:
                    lightlist[1] +=0x8;
                    break;
                }
            }
            else {}
            if((greencontrol[singalcount] & 0x20) != 0x0)
            {
                switch(singalcount)
                {
                case 0:
                    lightlist[2] +=0x1;
                    break;
                case 1:
                    lightlist[2] +=0x2;
                    break;
                case 2:
                    lightlist[2] +=0x4;
                    break;
                case 3:
                    lightlist[2] +=0x8;
                    break;
                }
            }
            else {}
            if((greencontrol[singalcount] & 0x10) != 0x0)
            {
                switch(singalcount)
                {
                case 0:
                    lightlist[3] +=0x1;
                    break;
                case 1:
                    lightlist[3] +=0x2;
                    break;
                case 2:
                    lightlist[3] +=0x4;
                    break;
                case 3:
                    lightlist[3] +=0x8;
                    break;
                }
            }
            else {}
        }
        //printf("0x70 & 0x40 = %x",0x70 & 0x40);
        for(int p = 0; p<4; p++)
        {
//        printf("lightlist[%d] = %x\n",p,lightlist[p]);
        }
        if(_exec_phase._subphase_count == 3)
        {
            for(int signalcount = 0; signalcount < _exec_phase._signal_count; signalcount++)
            {
                switch(lightlist[signalcount])
                {
                case 0:
                    break;
                case 1:
                    countstart[signalcount] = 0;
                    break;
                case 2:
                    countstart[signalcount] = 1;
                    break;
                case 3:
                    countstart[signalcount] = 0;
                    break;
                case 4:
                    countstart[signalcount] = 2;
                    break;
                case 5:
                    countstart[signalcount] = 2;
                    break;
                case 6:
                    countstart[signalcount] = 1;
                    break;
                case 7:
                    countstart[signalcount] = 0;
                    break;
                default:
                    break;
                }
                printf("countstart[%d] = %d\n",signalcount,countstart[signalcount]);
            }
        }
        else
        {
            if(_exec_phase._subphase_count == 2)
            {
                for(int signalcount = 0; signalcount < _exec_phase._signal_count; signalcount++)
                {
                    countstart[signalcount] = lightlist[signalcount]-1;

                }
            }
            else {}
        }

        /*
           for(int k = 0;k < _exec_phase._signal_count ;k++){
               if(usipedgreenPhaseTmp[_exec_phase_current_subphase][k]>0){
                   if(_exec_phase_current_subphase_step < 1){
                       printf("greencontrol[%d] = %x\n",_exec_phase_current_subphase,greencontrol[_exec_phase_current_subphase]);
                       switch(_exec_phase_current_subphase){
                           case 0:
                               greencontrol[_exec_phase_current_subphase] = 0x80;
                           break;
                           case 1:
                               greencontrol[_exec_phase_current_subphase] = 0x40;
                           break;
                           case 2:
                               greencontrol[_exec_phase_current_subphase] = 0x20;
                           break;
                           case 3:
                               greencontrol[_exec_phase_current_subphase] = 0x10;
                           break;
                           default:
                           break;

                       }for(int j = _exec_phase_current_subphase-1;j>= 0;j--){
                               printf("usipedgreenPhaseTmp[%d][%d] = %x usipedgreenPhaseTmp[%d][%d] = %x\n",_exec_phase_current_subphase,k,usipedgreenPhaseTmp[_exec_phase_current_subphase][k],j,_exec_phase_current_subphase_step,usipedgreenPhaseTmp[j][k]);
                           if(usipedgreenPhaseTmp[_exec_phase_current_subphase][k] == usipedgreenPhaseTmp[j][k]){
                                   switch(j){
                                       case 0:
                                           greencontrol[j+1] += 0x80;
                                           printf("+80greencontrol[%d] = %x\n",j,greencontrol[j]);
                                       break;
                                       case 1:
                                           greencontrol[j+1] += 0x40;
                                           printf("+40greencontrol[%d] = %x\n",j,greencontrol[j]);
                                       break;
                                       case 2:
                                           greencontrol[j+1] += 0x20;
                                           printf("+20greencontrol[%d] = %x\n",j,greencontrol[j]);
                                       break;
                                       case 3:
                                           greencontrol[j+1] += 0x10;
                                           printf("+10greencontrol[%d] = %x\n",j,greencontrol[j]);
                                       break;
                                       default:
                                       printf("no plus\n");
                                       break;
                                   }
                               }else{}

                       }
                   }else{}

                   printf("greencontrol[%d]=%x\n",_exec_phase_current_subphase,greencontrol[_exec_phase_current_subphase]);
               }else{
                   if(_exec_phase_current_subphase_step < 1){
                       switch(_exec_phase_current_subphase){
                           case 0:
                               greencontrol[_exec_phase_current_subphase] = 0x80;
                           break;
                           case 1:
                               greencontrol[_exec_phase_current_subphase] = 0x40;
                           break;
                           case 2:
                               greencontrol[_exec_phase_current_subphase] = 0x20;
                           break;
                           case 3:
                               greencontrol[_exec_phase_current_subphase] = 0x10;
                           break;
                           default:
                           break;

                       }
                   }else{}
               }
        */
        if(_exec_phase_current_subphase_step < 2)
        {
            //if(usipedredPhaseTmp[_exec_phase_current_subphase][_exec_phase_current_subphase_step]>0 ){
            redcontrol[_exec_phase_current_subphase] = 0xF0 - greencontrol[_exec_phase_current_subphase];
        }
        else
        {
            redcontrol[_exec_phase_current_subphase] = 0xF0;
        }
        printf("usipedredPhaseTmp[%d][%d] = %x\n",_exec_phase_current_subphase,_exec_phase._signal_count,usipedredPhaseTmp[_exec_phase_current_subphase][_exec_phase._signal_count]);

        printf("Pedsign[%d] = %d\n",_exec_phase_current_subphase,Pedsign[_exec_phase_current_subphase]);
        if(_exec_phase_current_subphase_step == 0 && Pedsign[_exec_phase_current_subphase] != true )
        {
            Pedsign[_exec_phase_current_subphase] = true ;
            switch(greencontrol[_exec_phase_current_subphase])
            {
            case 0x00:
                break;
            case 0x10:
                if(m_greenremainder[3] != 0 && countstart[3] != _exec_phase_current_subphase)
                {
                }
                else
                {
                    m_greenremainder[3] = m_pedgreenman[3];
                }
                break;
            case 0x20:
                if(m_greenremainder[2] != 0 && countstart[2] != _exec_phase_current_subphase)
                {
                }
                else
                {
                    m_greenremainder[2] = m_pedgreenman[2];
                }
                break;
            case 0x30:
                if(m_greenremainder[3] != 0 && countstart[3] != _exec_phase_current_subphase)
                {
                }
                else
                {
                    m_greenremainder[3] = m_pedgreenman[3];
                }
                if(m_greenremainder[2] != 0 && countstart[2] != _exec_phase_current_subphase)
                {
                }
                else
                {
                    m_greenremainder[2] = m_pedgreenman[2];
                }
                break;
            case 0x40:
                if(m_greenremainder[1] != 0 && countstart[1] != _exec_phase_current_subphase)
                {
                }
                else
                {
                    m_greenremainder[1] = m_pedgreenman[1];
                }
                break;
            case 0x50:
                if(m_greenremainder[1] != 0 && countstart[1] != _exec_phase_current_subphase)
                {
                }
                else
                {
                    m_greenremainder[1] = m_pedgreenman[1];
                }
                if(m_greenremainder[3] != 0 && countstart[3] != _exec_phase_current_subphase)
                {
                }
                else
                {
                    m_greenremainder[3] = m_pedgreenman[3];
                }
                break;
            case 0x60:
                if(m_greenremainder[1] != 0 && countstart[1] != _exec_phase_current_subphase)
                {
                }
                else
                {
                    m_greenremainder[1] = m_pedgreenman[1];
                }
                if(m_greenremainder[2] != 0 && countstart[2] != _exec_phase_current_subphase)
                {
                }
                else
                {
                    m_greenremainder[2] = m_pedgreenman[2];
                }
                break;
            case 0x70:
                if(m_greenremainder[1] != 0 && countstart[1] != _exec_phase_current_subphase)
                {
                }
                else
                {
                    m_greenremainder[1] = m_pedgreenman[1];
                }
                if(m_greenremainder[2] != 0 && countstart[2] != _exec_phase_current_subphase)
                {
                }
                else
                {
                    m_greenremainder[2] = m_pedgreenman[2];
                }
                if(m_greenremainder[3] != 0 && countstart[3] != _exec_phase_current_subphase)
                {
                }
                else
                {
                    m_greenremainder[3] = m_pedgreenman[3];
                }
                break;
            case 0x80:
                if(m_greenremainder[0] != 0 && countstart[0] != _exec_phase_current_subphase)
                {
                }
                else
                {
                    m_greenremainder[0] = m_pedgreenman[0];
                }
                break;
            case 0x90:
                if(m_greenremainder[0] != 0 && countstart[0] != _exec_phase_current_subphase)
                {
                }
                else
                {
                    m_greenremainder[0] = m_pedgreenman[0];
                }
                if(m_greenremainder[3] != 0 && countstart[3] != _exec_phase_current_subphase)
                {
                }
                else
                {
                    m_greenremainder[3] = m_pedgreenman[3];
                }
                break;
            case 0xA0:
                if(m_greenremainder[0] != 0 && countstart[0] != _exec_phase_current_subphase)
                {
                }
                else
                {
                    m_greenremainder[0] = m_pedgreenman[0];
                }
                if(m_greenremainder[2] != 0 && countstart[2] != _exec_phase_current_subphase)
                {
                }
                else
                {
                    m_greenremainder[2] = m_pedgreenman[2];
                }
                break;
            case 0xB0:
                if(m_greenremainder[0] != 0 && countstart[0] != _exec_phase_current_subphase)
                {
                }
                else
                {
                    m_greenremainder[0] = m_pedgreenman[0];
                }
                if(m_greenremainder[2] != 0 && countstart[2] != _exec_phase_current_subphase)
                {
                }
                else
                {
                    m_greenremainder[2] = m_pedgreenman[2];
                }
                if(m_greenremainder[3] != 0 && countstart[3] != _exec_phase_current_subphase)
                {
                }
                else
                {
                    m_greenremainder[3] = m_pedgreenman[3];
                }
                break;
            case 0xC0:
                if(m_greenremainder[0] != 0 && countstart[0] != _exec_phase_current_subphase)
                {
                }
                else
                {
                    m_greenremainder[0] = m_pedgreenman[0];
                }
                if(m_greenremainder[1] != 0 && countstart[1] != _exec_phase_current_subphase)
                {
                }
                else
                {
                    m_greenremainder[1] = m_pedgreenman[1];
                }
                break;
            case 0xD0:
                if(m_greenremainder[0] != 0 && countstart[0] != _exec_phase_current_subphase)
                {
                }
                else
                {
                    m_greenremainder[0] = m_pedgreenman[0];
                }
                if(m_greenremainder[1] != 0 && countstart[1] != _exec_phase_current_subphase)
                {
                }
                else
                {
                    m_greenremainder[1] = m_pedgreenman[1];
                }
                if(m_greenremainder[3] != 0 && countstart[3] != _exec_phase_current_subphase)
                {
                }
                else
                {
                    m_greenremainder[3] = m_pedgreenman[3];
                }
                break;
            case 0xE0:
                if(m_greenremainder[0] != 0 && countstart[0] != _exec_phase_current_subphase)
                {
                }
                else
                {
                    m_greenremainder[0] = m_pedgreenman[0];
                }
                if(m_greenremainder[1] != 0 && countstart[1] != _exec_phase_current_subphase)
                {
                }
                else
                {
                    m_greenremainder[1] = m_pedgreenman[1];
                }
                if(m_greenremainder[2] != 0 && countstart[2] != _exec_phase_current_subphase)
                {
                }
                else
                {
                    m_greenremainder[2] = m_pedgreenman[2];
                }
                break;
            case 0xF0:
                if(m_greenremainder[0] != 0 && countstart[0] != _exec_phase_current_subphase)
                {
                }
                else
                {
                    m_greenremainder[0] = m_pedgreenman[0];
                }
                if(m_greenremainder[1] != 0 && countstart[1] != _exec_phase_current_subphase)
                {
                }
                else
                {
                    m_greenremainder[1] = m_pedgreenman[1];
                }
                if(m_greenremainder[2] != 0 && countstart[2] != _exec_phase_current_subphase)
                {
                }
                else
                {
                    m_greenremainder[2] = m_pedgreenman[2];
                }
                if(m_greenremainder[3] != 0 && countstart[3] != _exec_phase_current_subphase)
                {
                }
                else
                {
                    m_greenremainder[3] = m_pedgreenman[3];
                }
                break;
            default:
                break;
            }
        }
        else {}
        printf("m_greenremainder[%d] = %d \n",_exec_phase_current_subphase,m_greenremainder[_exec_phase_current_subphase]);

        // printf("\n");
        //printf("\n\n\nPedsign[0] = %d Pedsign[2] = %d \ntest\n\n",Pedsign[0],Pedsign[2]);

        printf("\n[%d][%d] m_greenremainder = ",_exec_phase_current_subphase,_exec_phase_current_subphase_step);
        for(int i = 0; i < 8 ; i ++)
        {
            if(m_greenremainder[i] > 0)
            {
                //   printf("m_greenremainder[1] = %d -1\n", m_greenremainder[1]);
                m_greenremainder[i]--;
                /*      if(m_greenremainder[i] == 0){

                          if(Pedsign[i] == true && _exec_phase_current_subphase_step >1){
                                  m_greenremainder[i] = m_greenman[i] ;
                //              printf("m_greenman[%d] = %d \n",i,m_greenman[i]);
                          }else{
                                if(_exec_phase_current_subphase_step < 2){
                                  m_greenremainder[i] = m_redman[i]   ;
                              }else{}
                          }
                    //          printf("m_greenremainder[%d] = %d\n",i,m_greenremainder[i]);
                      }else{}
                            //  printf("m_greenremainder[%d] = %d -1 \n",i,m_greenremainder[i]);*/
            }
            else
            {
                /*          if(m_greenremainder[i] == 0 &&  _exec_phase_current_subphase_step > 1){
                              if(Pedsign[i] == true){
                                      m_greenremainder[i] = m_greenman[i]  ;
                    //              printf("m_greenman[%d] = %d \n",i,m_greenman[i]);
                              }else{
                                  if(_exec_phase_current_subphase_step < 2){
                                      m_greenremainder[i] = m_redman[i]   ;
                                  }else{}
                              }
                        //          printf("m_greenremainder[%d] = %d\n",i,m_greenremainder[i]);*/
            }
            //  }

            //printf("m_greenremainder[i] = ", m_greenremainder[i]);

            printf(" %d ",m_greenremainder[i]);
        }
        if(_exec_phase_current_subphase_step == 1)
        {
            printf("greencontrol = %x \n",greencontrol[_exec_phase_current_subphase]);

            //if((greencontrol[_exec_phase_current_subphase] & 0x0F) == 0x0 ){
            for(int k=0; k<8; k++)
            {
                if(m_greenremainder[k] > 0 && m_greenremainder[k] <= _exec_plan._ptr_subplaninfo[_exec_phase_current_subphase].compensated_pedgreen_flash(_exec_plan._shorten_cycle))
                {
                    switch(k)
                    {
                    case 0:
                        greencontrol[_exec_phase_current_subphase] +=  0x08;
                        break;
                    case 1:
                        greencontrol[_exec_phase_current_subphase] +=  0x04;
                        break;
                    case 2:
                        greencontrol[_exec_phase_current_subphase] +=  0x02;
                        break;
                    case 3:
                        greencontrol[_exec_phase_current_subphase] +=  0x01;
                        break;
                    default:
                        break;
                    }
                }
                else {}
            }
            // }else{}
        }
        else
        {
            if(_exec_phase_current_subphase_step > 1)   //若全部倒數歸零則停止綠人顯示 chou++ 2013/06/14
            {
                if(
                    m_greenremainder[0] == 0 && m_greenremainder[1] == 0 && m_greenremainder[2] == 0 && m_greenremainder[3] == 0 &&
                    m_greenremainder[4] == 0 && m_greenremainder[5] == 0 && m_greenremainder[6] == 0 && m_greenremainder[7] == 0

                )
                    greencontrol[_exec_phase_current_subphase] = 0x00;
                //  redcontrol[_exec_phase_current_subphase] = 0xF0;
            }
        }
        MESSAGEOK _MsgOK;
        printf("\n\n");
        if( _exec_plan._phase_order==FLASH_PHASEORDER || _exec_plan._phase_order==ALLRED_PHASEORDER || _current_strategy==STRATEGY_MANUAL  || _current_strategy==STRATEGY_ALLDYNAMIC || _exec_plan._subphase_count > 8)
        {
            _MsgOK = oDataToMessageOK.vPackageINFOToGreenmanCount(greencontrol[_exec_phase_current_subphase],redcontrol[_exec_phase_current_subphase], 0xFF,0xFF,0xFF,0xFF, 0);
            _MsgOK.InnerOrOutWard = cOutWard;
            if(smem.GetPedopen())
            {
                writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICEGREENMANCOUNT);
                // writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICENSPREDCOUNT);  //jacky20140328
            }

            _MsgOK = oDataToMessageOK.vPackageINFOToGreenmanCount(greencontrol[_exec_phase_current_subphase],redcontrol[_exec_phase_current_subphase], 0xFF,0xFF,0xFF,0xFF, 1);
            _MsgOK.InnerOrOutWard = cOutWard;
            if(smem.GetPedopen())
            {
                writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICEGREENMANCOUNT);
                // writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICENSPREDCOUNT);  //jacky20140328
            }
        }
        else
        {
            _MsgOK = oDataToMessageOK.vPackageINFOToGreenmanCount(greencontrol[_exec_phase_current_subphase],redcontrol[_exec_phase_current_subphase], m_greenremainder[0],m_greenremainder[1],m_greenremainder[2],m_greenremainder[3], 0);
            _MsgOK.InnerOrOutWard = cOutWard;
            if(smem.GetPedopen())
            {
                writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICEGREENMANCOUNT);
                // writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICENSPREDCOUNT);  //jacky20140328
            }
            _MsgOK = oDataToMessageOK.vPackageINFOToGreenmanCount(greencontrol[_exec_phase_current_subphase],redcontrol[_exec_phase_current_subphase], m_greenremainder[4],m_greenremainder[5],m_greenremainder[6],m_greenremainder[7], 1);
            _MsgOK.InnerOrOutWard = cOutWard;
            if(smem.GetPedopen())
            {
                writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICEGREENMANCOUNT);
                // writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICENSPREDCOUNT);  //jacky20140328
            }
        }
        printf("pedcyclecount = %d\n",pedcyclecount);

        /*OT20140329
        if(smem.GetPedopen()){
            writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICEGREENMANCOUNT);
            // writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICENSPREDCOUNT);  //jacky20140328
        }
        */

        // m_greenman[8];
        // m_greenremainder[8];
    }
    catch(...) {}
}
//----------------------------------------------------------
//OT990419
void CSTC::CalculateAndSendRedCountInDyn(const short int diff)
{
    try
    {

        printf("go CalculateAndSendRedCountInDyn\n");
        printf("go CalculateAndSendRedCountInDyn\n");
        printf("go CalculateAndSendRedCountInDyn\n");

        unsigned char ucSend[5];
        unsigned short int Data[8] = {0, 0, 0, 0, 0, 0, 0, 0};
        unsigned short int usiHCRedcountMapping[8] = { 0, 0, 0, 0, 0 };
        unsigned short int usiHKRedcountMapping[4] = {0, 0, 0, 0};

        bool bCountIF[8][8];   // 8output, 8subphase
        bool bCountIFEnable[8];   // 8output

        unsigned short int usiPhaseTmp;

        int iSubCnt;
        int iSignalCnt;
        int iSubCalCntTmp;

        iSubCnt = _exec_phase._subphase_count;
        iSignalCnt = _exec_phase._signal_count;

        for(int i = 0; i < iSubCnt; i++)
        {
            for(int j = 0; j < iSignalCnt; j++)
            {
                bCountIF[i][j] = false;
                bCountIFEnable[j] = true;
                usiPhaseTmp = _exec_phase._ptr_subphase_step_signal_status[i][0][j] & 0x03F0;  //when green light.
                //OT20140329, no left version!!
                //usiPhaseTmp = _exec_phase._ptr_subphase_step_signal_status[i][0][j] & 0x0xx0;  //when ^ / G light.

                if(usiPhaseTmp == 0)
                {
                    bCountIF[i][j] = true;
                }
            }
        }

        iSubCalCntTmp = 0;
        if( _exec_plan._phase_order==FLASH_PHASEORDER || _exec_plan._phase_order==ALLRED_PHASEORDER || _current_strategy==STRATEGY_MANUAL || diff!=0 || _current_strategy==STRATEGY_ALLDYNAMIC || _exec_plan._subphase_count > 8 || smem.vGetForceClockRedCountOneTime() == true)
        {
            smem.vSetForceClockRedCountOneTime(false);    //Do nothing
        }
        else
        {
            for(int i = _exec_phase_current_subphase; iSubCalCntTmp < iSubCnt; i++)
            {
                if(i == iSubCnt) i = 0;         //round..

                for(int j = 0; j < iSignalCnt; j++)
                {
                    if(bCountIF[i][j] == false)      //don't shining, after
                    {
                        bCountIFEnable[j] = false;
                    }

                    if(bCountIF[i][j] == true && bCountIFEnable[j] == true)
                    {

                        if(_exec_phase_current_subphase == i)    //now running. get timer val
                        {
                            Data[j] += stc.vGetStepTime();
                            switch(_exec_phase_current_subphase_step)
                            {
                            case(0):
                                Data[j] += _exec_plan._ptr_subplaninfo[_exec_phase_current_subphase].compensated_pedgreen_flash(_exec_plan._shorten_cycle);
                                Data[j] += _exec_plan._ptr_subplaninfo[_exec_phase_current_subphase].compensated_pedred(_exec_plan._shorten_cycle);
                                Data[j] += _exec_plan._ptr_subplaninfo[_exec_phase_current_subphase].compensated_yellow(_exec_plan._shorten_cycle);
                                Data[j] += _exec_plan._ptr_subplaninfo[_exec_phase_current_subphase].compensated_allred(_exec_plan._shorten_cycle);
                                break;
                            case(1):
                                Data[j] += _exec_plan._ptr_subplaninfo[_exec_phase_current_subphase].compensated_pedred(_exec_plan._shorten_cycle);
                                Data[j] += _exec_plan._ptr_subplaninfo[_exec_phase_current_subphase].compensated_yellow(_exec_plan._shorten_cycle);
                                Data[j] += _exec_plan._ptr_subplaninfo[_exec_phase_current_subphase].compensated_allred(_exec_plan._shorten_cycle);
                                break;
                            case(2):
                                Data[j] += _exec_plan._ptr_subplaninfo[_exec_phase_current_subphase].compensated_yellow(_exec_plan._shorten_cycle);
                                Data[j] += _exec_plan._ptr_subplaninfo[_exec_phase_current_subphase].compensated_allred(_exec_plan._shorten_cycle);
                                break;
                            case(3):
                                Data[j] += _exec_plan._ptr_subplaninfo[_exec_phase_current_subphase].compensated_allred(_exec_plan._shorten_cycle);
                                break;

                            default:
                                break;
                            }
                        }
                        else
                        {
                            Data[j] += _exec_plan._ptr_subplaninfo[i].compensated_phase_time(_exec_plan._shorten_cycle);
                        }
                    }
                }

                iSubCalCntTmp++;
            }
        }


//recheck
        if( _exec_plan._phase_order == FLASH_PHASEORDER ||
                _exec_plan._phase_order == FLASH_PHASEORDER_HSINCHU ||
                _exec_plan._phase_order == ALLRED_PHASEORDER ||
                _exec_plan._phase_order == FLASH_PHASEORDER_HSINCHU2 ||
                _current_strategy==STRATEGY_MANUAL ||
                _current_strategy==STRATEGY_ALLDYNAMIC )
        {
            for(int i = 0; i < 8; i++)
            {
                Data[i] = 0;
            }
        }
        if( smem.vGetBOOLData(TC_SignalConflictError) == true)
        {
            printf("TC SignalConflictError, Redcount Display Disable!\n");
            for(int i = 0; i < 8; i++)
            {
                Data[i] = 0;
            }
        }

        //OT980420
        if(_exec_phase._ptr_subphase_step_signal_status[0][0][0] == 0x1004 ||
                _exec_phase._ptr_subphase_step_signal_status[0][0][1] == 0x1004 ||
                _exec_phase._ptr_subphase_step_signal_status[0][0][0] == 0x2008 ||
                _exec_phase._ptr_subphase_step_signal_status[0][0][1] == 0x2008 ||
                _exec_phase._ptr_subphase_step_signal_status[0][0][0] == 0x2002 ||
                _exec_phase._ptr_subphase_step_signal_status[0][0][1] == 0x2002    )
        {
            printf("Flash light!, no redcount\n");
            for(int i = 0; i < 8; i++)
            {
                Data[i] = 0;
            }
        }

        if(_exec_phase._subphase_count == 1 && _exec_phase._total_step_count == 1)
        {
            printf("special phase, no redcount\n");
            for(int i = 0; i < 8; i++)
            {
                Data[i] = 0;
            }
        }


        if( smem.vGetUCData(TC_Redcount_Display_Enable) == 0)
        {
            printf("\nTC Redcount Display Disable!\n");
            for(int i = 0; i < 8; i++)
            {
                Data[i] = 0;
            }
        }

        if(smem.vGetUCData(TC_TrainChainNOW) == 1)
        {
            printf("\nTrain coming, TC Redcount Display Disable!\n");
            for(int i = 0; i < 8; i++)
            {
                Data[i] = 0;
            }
        }

        for(int i = 0 ; i < 8; i++)
        {
            if( Data[i] > 255 )
            {
                Data[i] = 0;    //Data[i] only 1 Byte
            }
            if( Data[i] > 0)
            {
                Data[i] = Data[i]+1;    // add 1 sec
            }
        }

        printf("\n     ********  RedCountTimer: %3d %3d %3d %3d %3d %3d %3d %3d********\n\n", Data[0],Data[1],Data[2],Data[3], Data[4],Data[5],Data[6], Data[7]);

        if( smem.vGetINTData(TC92_RedCountVer) == TC_RedCountVer94 )
        {
            //OT080422, -1 sec for LinChu Board
            for(int i = 0; i < 8; i++)
            {
                if(Data[i] > 0)
                {
                    Data[i]--;
                }
            }

            for(int i = 0; i < 5; i ++)
            {
                if(smem.vGetWayMappingRedCount(i) == 0)
                    usiHCRedcountMapping[i] = Data[0] & 0xFF;
                else if(smem.vGetWayMappingRedCount(i) == 1)
                    usiHCRedcountMapping[i] = Data[1] & 0xFF;
                else if(smem.vGetWayMappingRedCount(i) == 2)
                    usiHCRedcountMapping[i] = Data[2] & 0xFF;
                else if(smem.vGetWayMappingRedCount(i) == 3)
                    usiHCRedcountMapping[i] = Data[3] & 0xFF;
                else if(smem.vGetWayMappingRedCount(i) == 4)
                    usiHCRedcountMapping[i] = Data[4] & 0xFF;
                else if(smem.vGetWayMappingRedCount(i) == 5)
                    usiHCRedcountMapping[i] = Data[5] & 0xFF;
                else if(smem.vGetWayMappingRedCount(i) == 6)
                    usiHCRedcountMapping[i] = Data[6] & 0xFF;
                else if(smem.vGetWayMappingRedCount(i) == 7)
                    usiHCRedcountMapping[i] = Data[7] & 0xFF;
                else
                    usiHCRedcountMapping[i] = 0;
            }

            MESSAGEOK _MsgOK;
            _MsgOK = oDataToMessageOK.vPackageINFOToVer94RedCount(0x82, usiHCRedcountMapping[0], usiHCRedcountMapping[1], usiHCRedcountMapping[2], usiHCRedcountMapping[3], usiHCRedcountMapping[4]);
            _MsgOK.InnerOrOutWard = cOutWard;
            writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICEREDCOUNTVER94);
            //OT980421
            writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICEREDCOUNTVER94);

            printf("\n     ********  Mapped RedCountTimer: %3d %3d %3d %3d %3d********\n\n", usiHCRedcountMapping[0],usiHCRedcountMapping[1],usiHCRedcountMapping[2],usiHCRedcountMapping[3], usiHCRedcountMapping[4]);
        }
        else if( smem.vGetINTData(TC92_RedCountVer) == TC_RedCountVerHK )
        {

            for(int i = 0; i < 4; i ++)
            {
                if(smem.vGetWayMappingRedCount(i) == 0)
                    usiHKRedcountMapping[i] = Data[0] & 0xFF;
                else if(smem.vGetWayMappingRedCount(i) == 1)
                    usiHKRedcountMapping[i] = Data[1] & 0xFF;
                else if(smem.vGetWayMappingRedCount(i) == 2)
                    usiHKRedcountMapping[i] = Data[2] & 0xFF;
                else if(smem.vGetWayMappingRedCount(i) == 3)
                    usiHKRedcountMapping[i] = Data[3] & 0xFF;
                else if(smem.vGetWayMappingRedCount(i) == 4)
                    usiHKRedcountMapping[i] = Data[4] & 0xFF;
                else if(smem.vGetWayMappingRedCount(i) == 5)
                    usiHKRedcountMapping[i] = Data[5] & 0xFF;
                else if(smem.vGetWayMappingRedCount(i) == 6)
                    usiHKRedcountMapping[i] = Data[6] & 0xFF;
                else if(smem.vGetWayMappingRedCount(i) == 7)
                    usiHKRedcountMapping[i] = Data[7] & 0xFF;
                else
                    usiHKRedcountMapping[i] = 0;
            }

            MESSAGEOK _MsgOK;
            _MsgOK = oDataToMessageOK.vPackageINFOToVerHKRedCount(usiHKRedcountMapping[0], usiHKRedcountMapping[1], usiHKRedcountMapping[2], usiHKRedcountMapping[3]);
            _MsgOK.InnerOrOutWard = cOutWard;
            writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICEREDCOUNTVERHK);

            printf("\n     ********  Mapped RedCountTimer: %3d %3d %3d %3d ********\n\n", usiHKRedcountMapping[0],usiHKRedcountMapping[1],usiHKRedcountMapping[2],usiHKRedcountMapping[3]);
        }
        else if( smem.vGetINTData(TC92_RedCountVer) == TC_RedCountVer94v2 )
        {
            for(int i = 0; i < 8; i ++)
            {
                if(smem.vGetWayMappingRedCount(i) == 0)
                    usiHCRedcountMapping[i] = Data[0] & 0xFF;
                else if(smem.vGetWayMappingRedCount(i) == 1)
                    usiHCRedcountMapping[i] = Data[1] & 0xFF;
                else if(smem.vGetWayMappingRedCount(i) == 2)
                    usiHCRedcountMapping[i] = Data[2] & 0xFF;
                else if(smem.vGetWayMappingRedCount(i) == 3)
                    usiHCRedcountMapping[i] = Data[3] & 0xFF;
                else if(smem.vGetWayMappingRedCount(i) == 4)
                    usiHCRedcountMapping[i] = Data[4] & 0xFF;
                else if(smem.vGetWayMappingRedCount(i) == 5)
                    usiHCRedcountMapping[i] = Data[5] & 0xFF;
                else if(smem.vGetWayMappingRedCount(i) == 6)
                    usiHCRedcountMapping[i] = Data[6] & 0xFF;
                else if(smem.vGetWayMappingRedCount(i) == 7)
                    usiHCRedcountMapping[i] = Data[7] & 0xFF;
                else
                    usiHCRedcountMapping[i] = 0;
            }

            MESSAGEOK _MsgOK;
            _MsgOK = oDataToMessageOK.vPackageINFOToVer94v2RedCount(0x82, usiHCRedcountMapping[0], usiHCRedcountMapping[1], usiHCRedcountMapping[2], usiHCRedcountMapping[3],
                     usiHCRedcountMapping[4], usiHCRedcountMapping[5], usiHCRedcountMapping[6], usiHCRedcountMapping[7]);
            _MsgOK.InnerOrOutWard = cOutWard;
            writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICEREDCOUNTVER94);

            //OT980421
//cct should not twice.      writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICEREDCOUNTVER94);

            printf("\n     ********  Mapped RedCountTimer: %3d %3d %3d %3d %3d %3d %3d %3d********\n\n", usiHCRedcountMapping[0],usiHCRedcountMapping[1],usiHCRedcountMapping[2],usiHCRedcountMapping[3], usiHCRedcountMapping[4], usiHCRedcountMapping[5], usiHCRedcountMapping[6], usiHCRedcountMapping[7]);
        }
        else if( smem.vGetINTData(TC92_RedCountVer) == TC_RedCountVerCCT97 )
        {
            MESSAGEOK _MsgOK;
            for(int i = 0; i < 8; i ++)
            {
                if(smem.vGetWayMappingRedCount(i) == 0)
                    usiHCRedcountMapping[i] = Data[0] & 0xFF;
                else if(smem.vGetWayMappingRedCount(i) == 1)
                    usiHCRedcountMapping[i] = Data[1] & 0xFF;
                else if(smem.vGetWayMappingRedCount(i) == 2)
                    usiHCRedcountMapping[i] = Data[2] & 0xFF;
                else if(smem.vGetWayMappingRedCount(i) == 3)
                    usiHCRedcountMapping[i] = Data[3] & 0xFF;
                else if(smem.vGetWayMappingRedCount(i) == 4)
                    usiHCRedcountMapping[i] = Data[4] & 0xFF;
                else if(smem.vGetWayMappingRedCount(i) == 5)
                    usiHCRedcountMapping[i] = Data[5] & 0xFF;
                else if(smem.vGetWayMappingRedCount(i) == 6)
                    usiHCRedcountMapping[i] = Data[6] & 0xFF;
                else if(smem.vGetWayMappingRedCount(i) == 7)
                    usiHCRedcountMapping[i] = Data[7] & 0xFF;
                else
                    usiHCRedcountMapping[i] = 0;
            }

            ucSend[0] = 0xEA;
            ucSend[1] = 0x14;
            ucSend[2] = 8;
            for(int i = 0; i < 8; i++)
            {
                ucSend[3+i] = usiHCRedcountMapping[i];
            }
            _MsgOK = oDataToMessageOK.vPackageINFOTo92ProtocolSetADDR(ucSend, 11, 0x36, 65535);
            _MsgOK.InnerOrOutWard = cOutWard;
            writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICEREDCOUNTVERCCT97);

            /*
                  for(int i = 0; i < 8; i++) {  //set max RedCountID is 8
                      ucSend[0] = 0xEA;
                      ucSend[1] = 0x10;
                      ucSend[2] = usiHCRedcountMapping[i];
                      _MsgOK = oDataToMessageOK.vPackageINFOTo92ProtocolSetADDR(ucSend, 3, i);
                      _MsgOK.InnerOrOutWard = cOutWard;
                      writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICEREDCOUNTVERCCT97);
                  }
            */
//Send twice.
            /*
                  for(int i = 0; i < 8; i++) {  //set max RedCountID is 8
                      ucSend[0] = 0xEA;
                      ucSend[1] = 0x10;
                      ucSend[2] = usiHCRedcountMapping[i];
                      _MsgOK = oDataToMessageOK.vPackageINFOTo92ProtocolSetADDR(ucSend, 3, i);
                      _MsgOK.InnerOrOutWard = cOutWard;
                      writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICEREDCOUNTVERCCT97);
                  }
            */

            printf("\n     ********  Mapped RedCountTimer: %3d %3d %3d %3d %3d %3d %3d %3d********\n\n", usiHCRedcountMapping[0],usiHCRedcountMapping[1],usiHCRedcountMapping[2],usiHCRedcountMapping[3], usiHCRedcountMapping[4], usiHCRedcountMapping[5], usiHCRedcountMapping[6], usiHCRedcountMapping[7]);

        }

        int iTempScreenID;
        iTempScreenID = smem.GetcFace();
        if(iTempScreenID == cREDCOUNTHWCHECKSEL)
        {
            screenRedCountHWCheckSel.DisplayRedCountSec(
                usiHCRedcountMapping[0], usiHCRedcountMapping[1], usiHCRedcountMapping[2], usiHCRedcountMapping[3],
                usiHCRedcountMapping[4], usiHCRedcountMapping[5], usiHCRedcountMapping[6], usiHCRedcountMapping[7] );
        }





    }
    catch(...) {}
}
//----------------------------------------------------------
//**********************************************************
//****            Lock to Load or Save Data
//**********************************************************
//----------------------------------------------------------
bool CSTC::Lock_to_SaveDefaultLCNPhaseOrder(const unsigned short lcn, const unsigned short phaseorder)
{
    try
    {
        FILE * lcn_wfile = fopen("/Data/DefaultLCN.bin", "w");
        if(lcn_wfile) fwrite( &lcn, sizeof(unsigned short int), 1, lcn_wfile);
        else return false;
        fclose(lcn_wfile);
        LCN=lcn;

        FILE * phaseorder_wfile = fopen("/Data/DefaultPhaseOrder.bin", "w");
        if(phaseorder_wfile) fwrite( &phaseorder, sizeof(unsigned short int), 1, phaseorder_wfile);
        else return false;
        fclose(phaseorder_wfile);
        _default_phaseorder=phaseorder;

        return true;
    }
    catch(...) {}
}

//----------------------------------------------------------
//----------------------------------------------------------
bool CSTC::Lock_to_Load_Phase(CPhaseInfo &lphase, const unsigned short int &phase_order)
{
    try
    {
        if(phase[phase_order]._ptr_subphase_step_count)
        {
            /******** lock mutex ********/
            pthread_mutex_lock(&CPhaseInfo::_phase_mutex);
            lphase = phase[phase_order];
            /******** unlock mutex ********/
            pthread_mutex_unlock(&CPhaseInfo::_phase_mutex);
            return true;
        }
        else return false;
    }
    catch (...) {}
}
//OTCombo0719
//----------------------------------------------------------
bool CSTC::Lock_to_Load_Phase_for_Panel(const unsigned short int &phase_order)
{
    return Lock_to_Load_Phase(_panel_phase, phase_order);
}
bool CSTC::Lock_to_Load_Phase_for_Center(const unsigned short int &phase_order)
{
    return Lock_to_Load_Phase(_for_center_phase, phase_order);
}
//OT 940622
//----------------------------------------------------------
bool CSTC::Lock_to_Reset_Phase(CPhaseInfo &lphase, const unsigned short int &phase_order, const unsigned short int &subphase_count, const unsigned short int &signal_count)
{
    try
    {
//  if(phase[phase_order]._subphase_count==subphase_count && phase[phase_order]._signal_count==signal_count){    //when subphase count and signal count is the  same, just copy.
        if( phase[phase_order]._subphase_count==subphase_count &&
                phase[phase_order]._signal_count==signal_count &&
                phase[phase_order]._total_step_count == subphase_count*5 )     //when subphase count and signal count is the  same, just copy.
        {
            /******** lock mutex ********/
            pthread_mutex_lock(&CPhaseInfo::_phase_mutex);
            lphase = phase[phase_order];
            /******** unlock mutex ********/
            pthread_mutex_unlock(&CPhaseInfo::_phase_mutex);
            return true;
        }

        else
        {
            /******** lock mutex ********/
            pthread_mutex_lock(&CPhaseInfo::_phase_mutex);

//mallocFuck
            /*
                if(lphase._ptr_subphase_step_count){  //if(_ptr_subphase_step_count!=NULL), so that this CPhaseInfo is not empty
                    if(lphase._subphase_count==0||lphase._signal_count==0) perror("ERROR: CPhaseInfo::operator=\n");
                  for(int j=0;j<lphase._subphase_count;j++){
                    for(int k=0;k<lphase._ptr_subphase_step_count[j];k++){
                      free(lphase._ptr_subphase_step_signal_status[j][k]);
                    }
                    free(lphase._ptr_subphase_step_signal_status[j]);
                  }
                  free(lphase._ptr_subphase_step_count);
                  free(lphase._ptr_subphase_step_signal_status);
                }
            */

            lphase._phase_order       = phase_order;
            lphase._signal_map        = 0x55;
            lphase._subphase_count    = subphase_count;
            lphase._signal_count      = signal_count;
            lphase._total_step_count  = 5*subphase_count;                               //default 5 step

//mallocFuck    lphase._ptr_subphase_step_count    = (unsigned short int *)  malloc(sizeof(unsigned short int)   *lphase._subphase_count);
//mallocFuck    lphase._ptr_subphase_step_signal_status = (unsigned short int ***)malloc(sizeof(unsigned short int **)*lphase._subphase_count);
            for( int j=0; j<lphase._subphase_count; j++ )
            {
                lphase._ptr_subphase_step_count[j] = 5;
//mallocFuck      lphase._ptr_subphase_step_signal_status[j] = (unsigned short int **)malloc(sizeof(unsigned short int*)*lphase._ptr_subphase_step_count[j]);
                for( int k=0; k<lphase._ptr_subphase_step_count[j]; k++ )
                {
//mallocFuck        lphase._ptr_subphase_step_signal_status[j][k] = (unsigned short int *)malloc(sizeof(unsigned short int)*lphase._signal_count);
                    for(int l=0; l<lphase._signal_count; l++)
                        lphase._ptr_subphase_step_signal_status[j][k][l] = 0x1001;            //default red flash
                }
            }
        }
        /******** unlock mutex ********/
        pthread_mutex_unlock(&CPhaseInfo::_phase_mutex);

        return true;

    }
    catch (...) {}
}
//----------------------------------------------------------
bool CSTC::Lock_to_Save_Phase(const CPhaseInfo &sphase)
{
    try
    {
        /******** lock mutex ********/
        pthread_mutex_lock(&CPhaseInfo::_phase_mutex);

        //** 1: write to file
        FILE* phase_wfile=NULL;
        char filename[22];
        sprintf(filename,"/Data/PhaseInfo%X.bin\0",sphase._phase_order);
        phase_wfile = fopen( filename, "w" );//fopen return NULL if file not exist

        if(phase_wfile)
        {

            printf("Writing PhaseOrder: %3d (%#X)\n",sphase._phase_order,sphase._phase_order);

            fwrite(&sphase._phase_order,      sizeof(unsigned short int),1, phase_wfile);
            fwrite(&sphase._signal_map,       sizeof(unsigned short int),1, phase_wfile);
            fwrite(&sphase._subphase_count,   sizeof(unsigned short int),1, phase_wfile);
            fwrite(&sphase._signal_count,     sizeof(unsigned short int),1, phase_wfile);
            fwrite(&sphase._total_step_count, sizeof(unsigned short int),1, phase_wfile);

            for( int j=0; j<sphase._subphase_count; j++ )
            {
                fwrite(&sphase._ptr_subphase_step_count[j], sizeof(unsigned short int),1, phase_wfile);
                for( int k=0; k<sphase._ptr_subphase_step_count[j]; k++ )
                    for( int l=0; l<sphase._signal_count; l++)
                        fwrite(&sphase._ptr_subphase_step_signal_status[j][k][l], sizeof(unsigned short int),1, phase_wfile);
            }
            fclose(phase_wfile);
            //jacky20140319 為了寫Json檔
//    ReadPhaseData();
        } //end if(fopen() succeed)
        else perror("ERROR: write phase to file\n");

        //** 2: sync to phase[]
        phase[sphase._phase_order] = sphase;

        /******** unlock mutex ********/
        pthread_mutex_unlock(&CPhaseInfo::_phase_mutex);

        //OT Debug 951116
        return true;

    }
    catch (...) {}
}
//----------------------------------------------------------
//----------------------------------------------------------
bool CSTC::Lock_to_Load_Plan(CPlanInfo &lplan, const unsigned short int &planid)
{
    try
    {

        if(plan[planid]._planid != planid)
        {
            plan[planid]._planid         = planid;
            plan[planid]._dir            = 0;
            plan[planid]._phase_order    = 0;
            plan[planid]._subphase_count = 0;
            plan[planid]._cycle_time     = 0;
            plan[planid]._offset         = 0;
            plan[planid]._shorten_cycle = false;

//mallocFuck    lplan._ptr_subplaninfo = (SSubPlanInfo *)malloc(sizeof(SSubPlanInfo)*lplan._subphase_count);
            for( int j=0; j<8; j++ )
            {
                plan[planid]._ptr_subplaninfo[j]._green          = 0;
                plan[planid]._ptr_subplaninfo[j]._min_green      = 0;
                plan[planid]._ptr_subplaninfo[j]._max_green      = 0;
                plan[planid]._ptr_subplaninfo[j]._yellow         = 0;
                plan[planid]._ptr_subplaninfo[j]._allred         = 0;
                plan[planid]._ptr_subplaninfo[j]._pedgreen_flash = 0;
                plan[planid]._ptr_subplaninfo[j]._pedred         = 0;

                plan[planid]._ptr_subplaninfo[j]._green_compensation           = 0;
                plan[planid]._ptr_subplaninfo[j]._yellow_compensation          = 0;
                plan[planid]._ptr_subplaninfo[j]._allred_compensation          = 0;
                plan[planid]._ptr_subplaninfo[j]._pedgreen_flash_compensation  = 0;
                plan[planid]._ptr_subplaninfo[j]._pedred_compensation          = 0;
            }

        }

        if(plan[planid]._ptr_subplaninfo)
        {

            printf("STC Lock_to_Load_Plan, planid:%d\n", planid);

            /******** lock mutex ********/
            pthread_mutex_lock(&CPlanInfo::_plan_mutex);

            lplan = plan[planid];
            /******** unlock mutex ********/
            pthread_mutex_unlock(&CPlanInfo::_plan_mutex);
            return true;
        }
        else return false;
    }
    catch (...) {}
}
//------------------jacky20140418----------------------------------------
bool CSTC::Lock_to_Load_PriorityParameter(CPlanInfo &lplan, const unsigned short int &planid)
{
    try
    {
        //printf("\n\nplan[planid]._planid : %02X , planid : %02X\n\n",plan[planid]._planid,planid);
        if(BRTplan[planid]._planid != planid)
        {
            BRTplan[planid]._planid         = planid;
            BRTplan[planid]._phase_order    = 0;
            BRTplan[planid]._east           = 0;
            BRTplan[planid]._west           = 0;
            BRTplan[planid]._th1_east       = 0;
            BRTplan[planid]._th1_west       = 0;
            BRTplan[planid]._th2            = 0;
            BRTplan[planid]._time_east      = 0;
            BRTplan[planid]._time_west      = 0;
            BRTplan[planid]._BF13phasecount = 0;
            for(int i=0; i<2; i++)
            {
                for( int j=0; j<8; j++ )
                {
                    BRTplan[planid]._ptr_subplaninfo[j]._percentage[i]   = 0;
                }
            }
        }

        if(BRTplan[planid]._ptr_subplaninfo)
        {

            printf("STC Lock_to_Load_Plan, planid:%d\n", planid);

            /******** lock mutex ********/
            pthread_mutex_lock(&CPlanInfo::_plan_mutex);

            lplan = BRTplan[planid];

            //   printf("_planid:%02X _phase_order:%02X _east:%02X _west:%02X _th1_east:%02X _th1_west:%02X _th2:%02X _time_east:%d _time_west:%d _BF13phasecount:%d\n",
            //          lplan._planid, lplan._phase_order, lplan._east, lplan._west, lplan._th1_east,
            //         lplan._th1_west, lplan._th2, lplan._time_east, lplan._time_west, lplan._BF13phasecount);
            /******** unlock mutex ********/
            pthread_mutex_unlock(&CPlanInfo::_plan_mutex);
            return true;
        }
        else return false;
    }
    catch (...) {}
}
//----------------------------------------------------------
bool CSTC::Lock_to_Reset_Plan(CPlanInfo &lplan, const unsigned short int &planid, const unsigned short int &subphase_count)
{
    try
    {
        if(plan[planid]._subphase_count==subphase_count)
        {
            printf("Lock_to_Reset_Plan == start\n");
            /******** lock mutex ********/
            pthread_mutex_lock(&CPlanInfo::_plan_mutex);
            lplan = plan[planid];
            /******** unlock mutex ********/
            pthread_mutex_unlock(&CPlanInfo::_plan_mutex);
            printf("Lock_to_Reset_Plan == end\n");
            return true;
        }

        else
        {
            /******** lock mutex ********/
            pthread_mutex_lock(&CPlanInfo::_plan_mutex);
            printf("Lock_to_Reset_Plan != start\n");

//mallocFuck
            /*
                if(lplan._ptr_subplaninfo){  //if(_ptr_subplaninfo!=NULL), so that this CPlanInfo is not empty
                    if(lplan._subphase_count==0) perror("ERROR: CPlanInfo::operator=\n");
                  free(lplan._ptr_subplaninfo);
                }
            */
            lplan._planid         = planid;
            lplan._dir            = 0;
            lplan._phase_order    = 0;
            lplan._subphase_count = subphase_count;
            lplan._cycle_time     = 0;
            lplan._offset         = 0;
            lplan._shorten_cycle = false;

//mallocFuck    lplan._ptr_subplaninfo = (SSubPlanInfo *)malloc(sizeof(SSubPlanInfo)*lplan._subphase_count);
            for( int j=0; j<lplan._subphase_count; j++ )
            {
                lplan._ptr_subplaninfo[j]._green          = 0;
                lplan._ptr_subplaninfo[j]._min_green      = 0;
                lplan._ptr_subplaninfo[j]._max_green      = 0;
                lplan._ptr_subplaninfo[j]._yellow         = 0;
                lplan._ptr_subplaninfo[j]._allred         = 0;
                lplan._ptr_subplaninfo[j]._pedgreen_flash = 0;
                lplan._ptr_subplaninfo[j]._pedred         = 0;

                lplan._ptr_subplaninfo[j]._green_compensation           = 0;
                lplan._ptr_subplaninfo[j]._yellow_compensation          = 0;
                lplan._ptr_subplaninfo[j]._allred_compensation          = 0;
                lplan._ptr_subplaninfo[j]._pedgreen_flash_compensation  = 0;
                lplan._ptr_subplaninfo[j]._pedred_compensation          = 0;
            }

            /******** unlock mutex ********/
            pthread_mutex_unlock(&CPlanInfo::_plan_mutex);
            printf("Lock_to_Reset_Plan != end\n");

            return true;
        }
    }
    catch (...) {}
}
//----------------------------------------------------------
bool CSTC::Lock_to_Reset_PriorityParameter(CPlanInfo &lplan, const unsigned short int &planid, const unsigned short int &count)
{
    try
    {

        /******** lock mutex ********/
        pthread_mutex_lock(&CPlanInfo::_plan_mutex);
        printf("Lock_to_Reset_PriorityParameter != start\n");

//mallocFuck
        /*
            if(lplan._ptr_subplaninfo){  //if(_ptr_subplaninfo!=NULL), so that this CPlanInfo is not empty
                if(lplan._subphase_count==0) perror("ERROR: CPlanInfo::operator=\n");
              free(lplan._ptr_subplaninfo);
            }
        */
        lplan._planid         = planid;
        lplan._phase_order    = 0;
        lplan._east           = 0;
        lplan._west           = 0;
        lplan._th1_east       = 0;
        lplan._th1_west       = 0;
        lplan._th2            = 0;
        lplan._time_east      = 0;
        lplan._time_west      = 0;
        lplan._BF13phasecount = count ;

//mallocFuck    lplan._ptr_subplaninfo = (SSubPlanInfo *)malloc(sizeof(SSubPlanInfo)*lplan._subphase_count);
        for(int i=0; i<2; i++)
        {
            for( int j=0; j<lplan._BF13phasecount; j++ )
            {
                lplan._ptr_subplaninfo[j]._percentage[i] = 0;
            }
        }
        /******** unlock mutex ********/
        pthread_mutex_unlock(&CPlanInfo::_plan_mutex);
        printf("Lock_to_Reset_PriorityParameter != end\n");

        return true;

    }
    catch (...) {}
}
//----------------------------------------------------------
bool CSTC::Lock_to_Save_Plan(const CPlanInfo &splan)
{
    try
    {
        if(splan._ptr_subplaninfo==NULL||splan._subphase_count==0)
        {
            printf("ERROR: Plan is NULL!! Save failed!!\n");
            return false;
        }

        //OT Debug 0523
        if(splan._ptr_subplaninfo[0]._green==0 && splan._ptr_subplaninfo[0]._yellow==0
                && splan._phase_order!= FLASH_PHASEORDER && splan._phase_order!= ALLRED_PHASEORDER && splan._phase_order)
        {
            printf("ERROR: Plan not completed!! Save failed!!\n");
            return false;
        }

        /******** lock mutex ********/
        pthread_mutex_lock(&CPlanInfo::_plan_mutex);
        printf("Lock_to_Save_Plan Start\n");

        //** 1: write to file
        FILE* plan_wfile=NULL;
        char filename[22];
        sprintf(filename,"/Data/PlanInfo%d.bin",splan._planid);
        plan_wfile = fopen( filename, "w" );//fopen return NULL if file not exist

        if(plan_wfile)
        {

            printf("Writing PlanID: %2d\n",splan._planid);

            fwrite( &splan._planid,         sizeof( unsigned short int ), 1, plan_wfile );
            fwrite( &splan._dir,            sizeof( unsigned short int ), 1, plan_wfile );
            fwrite( &splan._phase_order,    sizeof( unsigned short int ), 1, plan_wfile );
            fwrite( &splan._subphase_count, sizeof( unsigned short int ), 1, plan_wfile );
            fwrite( &splan._cycle_time,     sizeof( unsigned short int ), 1, plan_wfile );
            fwrite( &splan._offset,         sizeof( unsigned short int ), 1, plan_wfile );
//***************************************chou check 2013/01/23
            for(int j=0; j<splan._subphase_count; j++)
            {
                fwrite( &(splan._ptr_subplaninfo[j]._green),          sizeof( unsigned short int ), 1, plan_wfile );
                fwrite( &(splan._ptr_subplaninfo[j]._min_green),      sizeof( unsigned short int ), 1, plan_wfile );
                fwrite( &(splan._ptr_subplaninfo[j]._max_green),      sizeof( unsigned short int ), 1, plan_wfile );
                fwrite( &(splan._ptr_subplaninfo[j]._yellow),         sizeof( unsigned short int ), 1, plan_wfile );
                fwrite( &(splan._ptr_subplaninfo[j]._allred),         sizeof( unsigned short int ), 1, plan_wfile );
                fwrite( &(splan._ptr_subplaninfo[j]._pedgreen_flash), sizeof( unsigned short int ), 1, plan_wfile );
                fwrite( &(splan._ptr_subplaninfo[j]._pedred),         sizeof( unsigned short int ), 1, plan_wfile );


                printf("\nOT _green:%d, _min_green:%d, _pedred:%d.\n\n", splan._ptr_subplaninfo[j]._green, splan._ptr_subplaninfo[j]._min_green, splan._ptr_subplaninfo[j]._pedred);

            }
            fclose(plan_wfile);
            printf("Lock_to_Save_Plan End\n");
            //jacky20140319 為了寫Json檔
//    ReadPlanData();

        } //end if(fopen() succeed)
        else perror("ERROR: write phase to file\n");
//***************************************chou check 2013/01/23
        //** 2: sync to plan[]
        plan[splan._planid] = splan;
        printf("Lock_to_Save_Plan Sync OK\n");

        //OTADD 941215
        PlanUpdate = true;

        //** 3: send to CCJ, Segment update
        vSendUpdateSegmentPlanInfoToCCJ_5F9D();
        printf("Lock_to_Save_Plan vSendUpdateSegmentPlanInfoToCCJ_5F9D OK\n");

        /******** unlock mutex ********/
        pthread_mutex_unlock(&CPlanInfo::_plan_mutex);

        //OT Debug 951118
        return true;

    }
    catch (...) {}
}
//------------jacky20140418--------------------------------------
bool CSTC::Lock_to_Save_PriorityParameter(const CPlanInfo &splan)
{
    try
    {
        /******** lock mutex ********/
        pthread_mutex_lock(&CPlanInfo::_plan_mutex);
        printf("Lock_to_Save_PriorityParameter Start\n");

        //** 1: write to file
        FILE* plan_wfile=NULL;
        char filename[22];
        sprintf(filename,"/Data/PriorityParameter%d.bin",splan._planid);
        plan_wfile = fopen( filename, "w" );//fopen return NULL if file not exist

        if(plan_wfile)
        {
            printf("Writing PlanID: %2d\n",splan._planid);

            fwrite( &splan._planid,         sizeof( unsigned short int ), 1, plan_wfile );
            fwrite( &splan._phase_order,    sizeof( unsigned short int ), 1, plan_wfile );
            fwrite( &splan._east,           sizeof( unsigned short int ), 1, plan_wfile );
            fwrite( &splan._west,           sizeof( unsigned short int ), 1, plan_wfile );
            fwrite( &splan._th1_east,       sizeof( unsigned short int ), 1, plan_wfile );
            fwrite( &splan._th1_west,       sizeof( unsigned short int ), 1, plan_wfile );
            fwrite( &splan._th2,            sizeof( unsigned short int ), 1, plan_wfile );
            fwrite( &splan._time_east,      sizeof( unsigned short int ), 1, plan_wfile );
            fwrite( &splan._time_west,      sizeof( unsigned short int ), 1, plan_wfile );
            fwrite( &splan._BF13phasecount, sizeof( unsigned short int ), 1, plan_wfile );

            for(int i=0; i<2; i++)
            {
                for(int j=0; j<splan._BF13phasecount; j++)
                {
                    fwrite( &(splan._ptr_subplaninfo[j]._percentage[i]), sizeof( unsigned short int ), 1, plan_wfile );
                }
            }
        }
        fclose(plan_wfile);

        //jacky20141231 為了寫Json檔
//    ReadPriorityParameter();

        BRTplan[splan._planid] = splan ;
        PlanUpdate = true;

        /******** unlock mutex ********/
        pthread_mutex_unlock(&CPlanInfo::_plan_mutex);
        printf("Lock_to_Save_PriorityParameter End\n");
    }
    catch (...) {}
}
//----------------------------------------------------------
bool CSTC::Lock_to_Load_Plan_for_Panel(const unsigned short int &planid)
{
    return Lock_to_Load_Plan(_panel_plan, planid);
}
bool CSTC::Lock_to_Load_Plan_for_Center(const unsigned short int &planid)
{
    return Lock_to_Load_Plan(_for_center_plan, planid);
}
bool CSTC::Lock_to_Load_Plan_for_BRT(const unsigned short int &planid)
{
    return Lock_to_Load_PriorityParameter(_for_BRT_plan, planid);
}
//OT940622
//----------------------------------------------------------
bool CSTC::Lock_to_Reset_Phase_for_Panel(const unsigned short int &phase_order, const unsigned short int &subphase_count, const unsigned short int &signal_count)
{
    return Lock_to_Reset_Phase(_panel_phase, phase_order, subphase_count, signal_count);
}
bool CSTC::Lock_to_Reset_Phase_for_Center(const unsigned short int &phase_order, const unsigned short int &subphase_count, const unsigned short int &signal_count)
{
    return Lock_to_Reset_Phase(_for_center_phase, phase_order, subphase_count, signal_count);
}

//----------------------------------------------------------
bool CSTC::Lock_to_Reset_Plan_for_Panel(const unsigned short int &planid, const unsigned short int &subphase_count)
{
    return Lock_to_Reset_Plan(_panel_plan, planid, subphase_count);
}
bool CSTC::Lock_to_Reset_Plan_for_Center(const unsigned short int &planid, const unsigned short int &subphase_count)
{
    return Lock_to_Reset_Plan(_for_center_plan, planid, subphase_count);
}
bool CSTC::Lock_to_Reset_Plan_for_BRT(const unsigned short int &planid, const unsigned short int &count)
{
    return Lock_to_Reset_Plan(_for_BRT_plan, planid, count);
}
//----------------------------------------------------------
bool CSTC::Check_Editing_Plan_for_Panel (const unsigned short int &planid, const unsigned short int &subphase_count)
{
    if(_panel_plan._planid == planid && _panel_plan._subphase_count == subphase_count ) return true;
    else return false;
}
bool CSTC::Check_Editing_Plan_for_Center(const unsigned short int &planid, const unsigned short int &subphase_count)
{
    if(_for_center_plan._planid == planid && _for_center_plan._subphase_count == subphase_count ) return true;
    else return false;
}
//----------------------------------------------------------
//OT 940622
bool CSTC::Lock_to_Save_Phase_from_Panel(void)
{
    return Lock_to_Save_Phase(_panel_phase);
}
bool CSTC::Lock_to_Save_Phase_from_Center(void)
{
    return Lock_to_Save_Phase(_for_center_phase);
}

bool CSTC::Lock_to_Save_Plan_from_Panel(void)
{
    return Lock_to_Save_Plan(_panel_plan);
}
bool CSTC::Lock_to_Save_Plan_from_Center(void)
{
    return Lock_to_Save_Plan(_for_center_plan);
}
bool CSTC::Lock_to_Save_Plan_from_BRT(void)
{
    return Lock_to_Save_PriorityParameter(_for_BRT_plan);
}
//----------------------------------------------------------
//----------------------------------------------------------
bool CSTC::Lock_to_Load_Segment(CSegmentInfo &lsegment, const unsigned short int &segment_type)
{
    try
    {

        if(segment[segment_type]._segment_type != segment_type || segment[segment_type]._segment_count == 0)
        {
            segment[segment_type]._segment_type  = segment_type;
            segment[segment_type]._segment_count = 0;

//mallocFuck    lsegment._ptr_seg_exec_time = (SSegExecTime *)malloc(sizeof(SSegExecTime)*lsegment._segment_count);
            for( int j=0; j<32; j++ )
            {
                segment[segment_type]._ptr_seg_exec_time[j]._hour     = 0;
                segment[segment_type]._ptr_seg_exec_time[j]._minute   = 0;
                segment[segment_type]._ptr_seg_exec_time[j]._planid   = 0;
                segment[segment_type]._ptr_seg_exec_time[j]._cadc_seg = 0;
            }

        }

        if(segment[segment_type]._ptr_seg_exec_time)
        {
            /******** lock mutex ********/
            pthread_mutex_lock(&CSegmentInfo::_segment_mutex);
            lsegment = segment[segment_type];
            /******** unlock mutex ********/
            pthread_mutex_unlock(&CSegmentInfo::_segment_mutex);
            return true;
        }
        else return false;
    }
    catch (...) {}
}
//----------------------------------------------------------
bool CSTC::Lock_to_Load_PrioritySegment(CSegmentInfo &lsegment, const unsigned short int &segment_type)
{
    try
    {

        if(BRTsegment[segment_type]._segment_type != segment_type || BRTsegment[segment_type]._segment_count == 0)
        {
            BRTsegment[segment_type]._segment_type  = segment_type;
            BRTsegment[segment_type]._segment_count = 0;

//mallocFuck    lsegment._ptr_seg_exec_time = (SSegExecTime *)malloc(sizeof(SSegExecTime)*lsegment._segment_count);
            for( int j=0; j<32; j++ )
            {
                BRTsegment[segment_type]._ptr_seg_exec_time[j]._hour     = 0;
                BRTsegment[segment_type]._ptr_seg_exec_time[j]._minute   = 0;
                BRTsegment[segment_type]._ptr_seg_exec_time[j]._planid   = 0;
                BRTsegment[segment_type]._ptr_seg_exec_time[j]._onoff = 0;
            }

        }

        if(segment[segment_type]._ptr_seg_exec_time)
        {
            /******** lock mutex ********/
            pthread_mutex_lock(&CSegmentInfo::_segment_mutex);
            lsegment = BRTsegment[segment_type];
            /******** unlock mutex ********/
            pthread_mutex_unlock(&CSegmentInfo::_segment_mutex);
            return true;
        }
        else return false;
    }
    catch (...) {}
}
//----------------------------------------------------------
bool CSTC::Lock_to_Reset_Segment(CSegmentInfo &lsegment, const unsigned short int &segment_type, const unsigned short int &segment_count)
{
    try
    {
        if(segment[segment_type]._segment_count==segment_count)
        {
            printf("Lock_to_Reset_Segment ==Start\n");
            /******** lock mutex ********/
            pthread_mutex_lock(&CSegmentInfo::_segment_mutex);
            lsegment = segment[segment_type];
            /******** unlock mutex ********/
            pthread_mutex_unlock(&CSegmentInfo::_segment_mutex);
            printf("Lock_to_Reset_Segment ==End\n");
            return true;
        }

        else
        {
            printf("Lock_to_Reset_Segment !=Start\n");
            /******** lock mutex ********/
            pthread_mutex_lock(&CSegmentInfo::_segment_mutex);

//mallocFuck
            /*
                if(lsegment._ptr_seg_exec_time){  //if(_ptr_plan_exec!=NULL), so that this CSegmentInfo is not empty
                    if(lsegment._segment_count==0) perror("ERROR: CSegmentInfo::operator=\n");
                  free(lsegment._ptr_seg_exec_time);
                }
            */
            lsegment._segment_type  = segment_type;
            lsegment._segment_count = segment_count;

//mallocFuck    lsegment._ptr_seg_exec_time = (SSegExecTime *)malloc(sizeof(SSegExecTime)*lsegment._segment_count);
            for( int j=0; j<lsegment._segment_count; j++ )
            {
                lsegment._ptr_seg_exec_time[j]._hour     = 0;
                lsegment._ptr_seg_exec_time[j]._minute   = 0;
                lsegment._ptr_seg_exec_time[j]._planid   = 0;
                lsegment._ptr_seg_exec_time[j]._cadc_seg = 0;
            }
            /******** unlock mutex ********/
            pthread_mutex_unlock(&CSegmentInfo::_segment_mutex);
            printf("Lock_to_Reset_Segment !=End\n");

            return true;
        }
    }
    catch (...) {}
}
//----------------------------------------------------------
//OT20110517
// void CSTC::Lock_to_Save_Segment(const CSegmentInfo &ssegment)
void CSTC::Lock_to_Save_Segment(CSegmentInfo &ssegment)
{
    try
    {
        printf("Lock_to_Save_Segment Start\n");
        /******** lock mutex ********/
        pthread_mutex_lock(&CSegmentInfo::_segment_mutex);

        //** 1: write to file
        FILE* segment_wfile=NULL;
        char filename[25];
        sprintf(filename,"/Data/SegmentInfo%d.bin\0",ssegment._segment_type);
        segment_wfile = fopen( filename, "w" );//fopen return NULL if file not exist

        if(segment_wfile)
        {

            printf("Writing SegmentType: %2d\n",ssegment._segment_type);

            fwrite( &ssegment._segment_type,  sizeof( unsigned short int ), 1, segment_wfile );
            fwrite( &ssegment._segment_count, sizeof( unsigned short int ), 1, segment_wfile );
            for(int j=0; j<ssegment._segment_count; j++)
            {

                //OT20110517
                if(ssegment._ptr_seg_exec_time[j]._cadc_seg == 0 ||
                        ssegment._ptr_seg_exec_time[j]._cadc_seg == 0xDDDD ||
                        ssegment._ptr_seg_exec_time[j]._cadc_seg == 0xEEEE ||
                        ssegment._ptr_seg_exec_time[j]._cadc_seg == 0xFFFF   )
                {
                }
                else      //reset
                {
                    ssegment._ptr_seg_exec_time[j]._cadc_seg = 0;
                }

                fwrite( &(ssegment._ptr_seg_exec_time[j]._hour),     sizeof( unsigned short int ), 1, segment_wfile );
                fwrite( &(ssegment._ptr_seg_exec_time[j]._minute),   sizeof( unsigned short int ), 1, segment_wfile );
                fwrite( &(ssegment._ptr_seg_exec_time[j]._planid),   sizeof( unsigned short int ), 1, segment_wfile );
                fwrite( &(ssegment._ptr_seg_exec_time[j]._cadc_seg), sizeof( unsigned short int ), 1, segment_wfile );
                //fwrite( &(ssegment._ptr_seg_exec_time[j]._actMode), sizeof( unsigned short int ), 1, segment_wfile );//arwen++ 1000930
            }
            fclose(segment_wfile);
            //jacky20140319 為了寫Json檔
//    ReadSegmentData();
        }
        else perror("ERROR: write phase to file\n");

        //** re 1 : save act_Mode arwen++ 1001006
        sprintf(filename,"/Data/ActCtrlSegment%d.bin\0",ssegment._segment_type);
        segment_wfile = fopen( filename, "w" );//fopen return NULL if file not exist

        if(segment_wfile)
        {
            printf("Writing ActCtrl SegmentType: %2d\n",ssegment._segment_type);
            fwrite( &ssegment._actSegmentType1,  sizeof( unsigned short int ), 1, segment_wfile );
            fwrite( &ssegment._actSegmentType2,  sizeof( unsigned short int ), 1, segment_wfile );
            for(int j=0; j<ssegment._segment_count; j++)
            {
                fwrite( &(ssegment._ptr_seg_exec_time[j]._actMode), sizeof( unsigned short int ), 1, segment_wfile );//arwen++ 1000930
                //fwrite( &(ssegment._ptr_seg_exec_time[j]._compensateMode), sizeof( unsigned short int ), 1, segment_wfile );//arwen++ 1000930
                fwrite( &(ssegment._ptr_seg_exec_time[j]._openRunnig), sizeof( unsigned short int ), 1, segment_wfile );//arwen++ 1000930
            }
            fclose(segment_wfile);
        }
        else perror("ERROR: write ActCtrl phase to file\n");
        //↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑↑Arwen end++

        //** 2: sync to segment[]
        segment[ssegment._segment_type] = ssegment;
        SegmentTypeUpdate = true;  //2005/4/25

        //** 3: send to CCJ, Segment update
        vSendUpdateSegmentPlanInfoToCCJ_5F9D();

        /******** unlock mutex ********/
        pthread_mutex_unlock(&CSegmentInfo::_segment_mutex);

        printf("Lock_to_Save_Segment End\n");
    }
    catch (...) {}
}
//-------------jacky20140418-------------------------------------------------------
void CSTC::Lock_to_Save_Segment_for_Priority(CSegmentInfo &ssegment)
{
    try
    {
        printf("\nLock_to_Save_Segment_for_Priority Start\n");
        /******** lock mutex ********/
        pthread_mutex_lock(&CSegmentInfo::_segment_mutex);

        //** 1: write to file
        FILE* segment_wfile=NULL;
        char filename[25];
        sprintf(filename,"/Data/PrioritySegmentInfo%d.bin\0",ssegment._segment_type);
        segment_wfile = fopen( filename, "w" );//fopen return NULL if file not exist

        if(segment_wfile)
        {

            printf("Writing SegmentType: %2d\n",ssegment._segment_type);

            fwrite( &ssegment._segment_type,  sizeof( unsigned short int ), 1, segment_wfile );
            fwrite( &ssegment._segment_count, sizeof( unsigned short int ), 1, segment_wfile );

            for(int j=0; j<ssegment._segment_count; j++)
            {
                fwrite( &(ssegment._ptr_seg_exec_time[j]._hour),     sizeof( unsigned short int ), 1, segment_wfile );
                fwrite( &(ssegment._ptr_seg_exec_time[j]._minute),   sizeof( unsigned short int ), 1, segment_wfile );
                fwrite( &(ssegment._ptr_seg_exec_time[j]._planid),   sizeof( unsigned short int ), 1, segment_wfile );
                fwrite( &(ssegment._ptr_seg_exec_time[j]._onoff),    sizeof( unsigned short int ), 1, segment_wfile );
                //fwrite( &(ssegment._ptr_seg_exec_time[j]._actMode), sizeof( unsigned short int ), 1, segment_wfile );//arwen++ 1000930
            }
            fclose(segment_wfile);
            //jacky20140418 為了寫Json檔
//        ReadPrioritySegment();
//        ReadSegmentData();
        }
        else perror("ERROR: write phase to file\n");

        BRTsegment[ssegment._segment_type] = ssegment;
        SegmentTypeUpdate = true;

        /******** unlock mutex ********/
        pthread_mutex_unlock(&CSegmentInfo::_segment_mutex);
        printf("Lock_to_Save_Segment_for_Priority End\n");
    }
    catch (...) {}
}
//----------------------------------------------------------
/*  have not written
void CSTC::Lock_to_Load_ReverseTime_Step1(const unsigned short int &CReverseTimeInfo &srev)
{
}
*/
//----------------------------------------------------------
bool CSTC::Lock_to_Load_Segment_for_Panel(const unsigned short int &segment_type)
{
    return Lock_to_Load_Segment(_panel_segment, segment_type);
}
bool CSTC::Lock_to_Load_Segment_for_Center(const unsigned short int &segment_type)
{
    return Lock_to_Load_Segment(_for_center_segment, segment_type);
}
bool CSTC::Lock_to_Load_Segment_for_BRT(const unsigned short int &segment_type)
{
    return Lock_to_Load_PrioritySegment(_for_BRT_segment, segment_type);
}
//----------------------------------------------------------
bool CSTC::Lock_to_Reset_Segment_for_Panel (const unsigned short int &segment_type, const unsigned short int &segment_count)
{
    return Lock_to_Reset_Segment(_panel_segment, segment_type, segment_count);
}
bool CSTC::Lock_to_Reset_Segment_for_Center (const unsigned short int &segment_type, const unsigned short int &segment_count)
{
    return Lock_to_Reset_Segment(_for_center_segment, segment_type, segment_count);
}
bool CSTC::Lock_to_Reset_Segment_for_Web (const unsigned short int &segment_type, const unsigned short int &segment_count)
{
    return Lock_to_Reset_Segment(_for_web_segment, segment_type, segment_count);
}
bool CSTC::Lock_to_Reset_Segment_for_BRT (const unsigned short int &segment_type, const unsigned short int &segment_count)
{
    return Lock_to_Reset_Segment(_for_web_segment, segment_type, segment_count);
}
//----------------------------------------------------------
bool CSTC::Lock_to_Load_Segment_for_Panel_inWeekDay(const unsigned short int &weekday)
{
    try
    {
        if(weekday<1||(weekday>7&&weekday<11)||weekday>17) return false;

        unsigned short int weekday_in_array, segment_type;
        if(weekday<=7) weekday_in_array = weekday-1;
        else           weekday_in_array = weekday-4;

        /******** lock mutex ********/
        pthread_mutex_lock(&CSegmentInfo::_segment_mutex);
        segment_type = weekdayseg[weekday_in_array]._segment_type;
        /******** unlock mutex ********/
        pthread_mutex_unlock(&CSegmentInfo::_segment_mutex);

        return Lock_to_Load_Segment(_panel_segment, segment_type);
    }
    catch (...) {}
}
//----------------------------------------------------------
bool CSTC::Lock_to_Load_Segment_for_Center_inWeekDay(const unsigned short int &weekday)
{
    try
    {
        if(weekday<1||(weekday>7&&weekday<11)||weekday>17) return false;

        unsigned short int weekday_in_array, segment_type;
        if(weekday<=7) weekday_in_array = weekday-1;
        else           weekday_in_array = weekday-4;

        /******** lock mutex ********/
        pthread_mutex_lock(&CSegmentInfo::_segment_mutex);
        segment_type = weekdayseg[weekday_in_array]._segment_type;
        /******** unlock mutex ********/
        pthread_mutex_unlock(&CSegmentInfo::_segment_mutex);

        return Lock_to_Load_Segment(_for_center_segment, segment_type);
    }
    catch (...) {}
}
//----------------------------------------------------------
bool CSTC::Lock_to_Load_WeekDaySegment_for_Panel(void)
{
    try
    {
        /******** lock mutex ********/
        pthread_mutex_lock(&CSegmentInfo::_segment_mutex);
        for(int i=0; i<AMOUNT_WEEKDAY_SEG; i++)
            _panel_weekdayseg[i] = weekdayseg[i];
        /******** unlock mutex ********/
        pthread_mutex_unlock(&CSegmentInfo::_segment_mutex);
    }
    catch (...) {}
}
bool CSTC::Lock_to_Load_WeekDaySegment_for_Center(void)
{
    try
    {
        /******** lock mutex ********/
        pthread_mutex_lock(&CSegmentInfo::_segment_mutex);
        for(int i=0; i<AMOUNT_WEEKDAY_SEG; i++)
            _for_center_weekdayseg[i] = weekdayseg[i];
        /******** unlock mutex ********/
        pthread_mutex_unlock(&CSegmentInfo::_segment_mutex);
    }
    catch (...) {}
}
//----------------------------------------------------------
bool CSTC::Lock_to_Load_WeekDaySegment_for_Web(void)
{
    try
    {
        /******** lock mutex ********/
        pthread_mutex_lock(&CSegmentInfo::_segment_mutex);
        for(int i=0; i<AMOUNT_WEEKDAY_SEG; i++)
            _for_web_weekdayseg[i] = weekdayseg[i];
        /******** unlock mutex ********/
        pthread_mutex_unlock(&CSegmentInfo::_segment_mutex);
    }
    catch (...) {}
}
//----------------------------------------------------------
bool CSTC::Lock_to_Load_HoliDaySegment_for_Panel(const unsigned short int &holiday_segment_type)
{
    try
    {
        if(holiday_segment_type<8||holiday_segment_type>20) return false;

        /******** lock mutex ********/
        pthread_mutex_lock(&CSegmentInfo::_segment_mutex);
        _panel_holidayseg = holidayseg[holiday_segment_type-8];
        printf("IN CSTC, LOAD HOLIDAYSEGMENT FOR PANEL holiday_segment_type:%d\n", holiday_segment_type);
        /******** unlock mutex ********/
        pthread_mutex_unlock(&CSegmentInfo::_segment_mutex);

        return Lock_to_Load_Segment(_panel_segment, holiday_segment_type);
    }
    catch (...) {}
}
//----------------------------------------------------------
bool CSTC::Lock_to_Load_HoliDaySegment_for_Center(const unsigned short int &holiday_segment_type)
{
    try
    {
        printf("[OT]* Query holiday_segment_type:%d\n", holiday_segment_type);
        if(holiday_segment_type<8||holiday_segment_type>20) return false;

        /******** lock mutex ********/
        pthread_mutex_lock(&CSegmentInfo::_segment_mutex);
        _for_center_holidayseg = holidayseg[holiday_segment_type-8];
        /******** unlock mutex ********/
        pthread_mutex_unlock(&CSegmentInfo::_segment_mutex);

        return Lock_to_Load_Segment(_for_center_segment, holiday_segment_type);
    }
    catch (...) {}
}
//----------------------------------------------------------
bool CSTC::Lock_to_Load_Current_Segment_for_Panel(void)
{
    try
    {
        /******** lock mutex ********/
        pthread_mutex_lock(&CSegmentInfo::_segment_mutex);
        _panel_segment = _exec_segment;
        /******** unlock mutex ********/
        pthread_mutex_unlock(&CSegmentInfo::_segment_mutex);
        return true;
    }
    catch (...) {}
}

//----------------------------------------------------------
bool CSTC::Lock_to_Load_Current_Plan_for_Panel(void)
{
    try
    {
        /******** lock mutex ********/
        pthread_mutex_lock(&CPlanInfo::_plan_mutex);
        _panel_plan = _exec_plan;
        /******** unlock mutex ********/
        pthread_mutex_unlock(&CPlanInfo::_plan_mutex);
        return true;
    }
    catch (...) {}
}
//----------------------------------------------------------
void CSTC::Lock_to_Save_Segment_from_Panel(void)
{
    Lock_to_Save_Segment(_panel_segment);
    ///Arwen add 110210 Save new_data_segment
    //oStorINI.vSaveIntSegmentToINI("UPDate","UPDateStart",1);//open Updete
    //ConvertSegmentData(true);
    ACT_Save_Segment_Data(_panel_segment);//arwen++
}
void CSTC::Lock_to_Save_Segment_from_Center(void)
{
    Lock_to_Save_Segment(_for_center_segment);
    ///Arwen add 110210 Save new_data_segment
    //oStorINI.vSaveIntSegmentToINI("UPDate","UPDateStart",1);//open Updete
    //ConvertSegmentData(true);
    ACT_Save_Segment_Data(_for_center_segment);//arwen++
}
//----------------------------------------------------------
void CSTC::Lock_to_Save_Segment_from_Web(void)
{
    Lock_to_Save_Segment(_for_web_segment);
    ///Arwen add 110210 Save new_data_segment
    //oStorINI.vSaveIntSegmentToINI("UPDate","UPDateStart",1);//open Updete
    //ConvertSegmentData(true);
    ACT_Save_Segment_Data(_for_web_segment);//arwen++
}
//----------------------------------------------------------
void CSTC::Lock_to_Save_Segment_from_BRT(void)
{
    Lock_to_Save_Segment_for_Priority(_for_BRT_segment);
    //oStorINI.vSaveIntSegmentToINI("UPDate","UPDateStart",1);//open Updete
    //ConvertSegmentData(true);
    //ACT_Save_Segment_Data(_for_web_segment);//arwen++
}
//----------------------------------------------------------
bool CSTC::Lock_to_Load_WeekDayReverseTime_for_Panel(void)
{
    try
    {
        /******** lock mutex ********/
        pthread_mutex_lock(&CReverseTimeInfo::_rev_mutex);
        for(int i=0; i<AMOUNT_WEEKDAY_REV; i++)
        {
            printf("%d %d\n", weekdayrev[i]._reverse_time_type, weekdayrev[i]._weekday);
            _panel_weekdayrev[i] = weekdayrev[i];
        }
        /******** unlock mutex ********/
        pthread_mutex_unlock(&CReverseTimeInfo::_rev_mutex);
    }
    catch (...) {}
}
//----------------------------------------------------------
bool CSTC::Lock_to_Load_WeekDayReverseTime_for_Center(void)
{
    try
    {
        /******** lock mutex ********/
        pthread_mutex_lock(&CReverseTimeInfo::_rev_mutex);
        for(int i=0; i<AMOUNT_WEEKDAY_REV; i++)
        {
            _for_center_weekdayrev[i] = weekdayrev[i];
        }
        /******** unlock mutex ********/
        pthread_mutex_unlock(&CReverseTimeInfo::_rev_mutex);
    }
    catch (...) {}
}
//----------------------------------------------------------
bool CSTC::Lock_to_Load_HoliDayReverseTime_for_Panel(const unsigned short int &holiday_reversetime)
{
    try
    {
        if(holiday_reversetime<4 || holiday_reversetime>16) return false;

        /******** lock mutex ********/
        pthread_mutex_lock(&CReverseTimeInfo::_rev_mutex);
        _panel_holidayrev = holidayrev[holiday_reversetime - 4];
        /******** unlock mutex ********/
        pthread_mutex_unlock(&CReverseTimeInfo::_rev_mutex);

        return Lock_to_Load_ReverseTime_Step2(_panel_reversetime, holiday_reversetime);
    }
    catch (...) {}
}

//----------------------------------------------------------
bool CSTC::Lock_to_Load_HoliDayReverseTime_for_Center(const unsigned short int &holiday_reversetime)
{
    try
    {

        if(holiday_reversetime<4||holiday_reversetime>16) return false;

        /******** lock mutex ********/
        pthread_mutex_lock(&CReverseTimeInfo::_rev_mutex);
        _for_center_holidayrev = holidayrev[holiday_reversetime-4];
        /******** unlock mutex ********/
        pthread_mutex_unlock(&CReverseTimeInfo::_rev_mutex);

        return Lock_to_Load_ReverseTime_Step2(_for_center_reversetime, holiday_reversetime);
    }
    catch (...) {}
}
//----------------------------------------------------------
void CSTC::Lock_to_Save_ReverseTime_Step1(int iSelectSource)
{
    if(iSelectSource == 0)
    {
        Lock_to_Save_ReverseTime_Step2(_for_center_reversetime);
    }
    else
    {
        Lock_to_Save_ReverseTime_Step2(_panel_reversetime);
    }

}
//-------------------------------------------------------------
void CSTC::Lock_to_Save_ReverseTime_Step2(const CReverseTimeInfo &srev)
{
    try
    {

        /******** lock mutex ********/
        pthread_mutex_lock(&CReverseTimeInfo::_rev_mutex);

        //** 1: write to file
        FILE* rev_wfile=NULL;
        char filename[25];
// check iSelectSource
        sprintf(filename,"/Data/ReverseTimeType%d.bin\0", srev._reverse_time_type, rev_wfile);
        rev_wfile = fopen( filename, "w" );//fopen return NULL if file not exist

        if(rev_wfile)
        {
// check iSelectSource
            printf("Writing ReverseTimeData: %2d\n",srev._reverse_time_type);

            fwrite( &(srev._reverse_time_type), sizeof( unsigned short int ), 1, rev_wfile );
            fwrite( &(srev.usiDirectIn),     sizeof( unsigned short int ), 1, rev_wfile );
            fwrite( &(srev.usiHourStartIn),  sizeof( unsigned short int ), 1, rev_wfile );
            fwrite( &(srev.usiMinStartIn),   sizeof( unsigned short int ), 1, rev_wfile );
            fwrite( &(srev.usiHourEndIn),    sizeof( unsigned short int ), 1, rev_wfile );
            fwrite( &(srev.usiMinEndIn),     sizeof( unsigned short int ), 1, rev_wfile );

            fwrite( &(srev.usiDirectOut),     sizeof( unsigned short int ), 1, rev_wfile );
            fwrite( &(srev.usiHourStartOut),  sizeof( unsigned short int ), 1, rev_wfile );
            fwrite( &(srev.usiMinStartOut),   sizeof( unsigned short int ), 1, rev_wfile );
            fwrite( &(srev.usiHourEndOut),    sizeof( unsigned short int ), 1, rev_wfile );
            fwrite( &(srev.usiMinEndOut),     sizeof( unsigned short int ), 1, rev_wfile );

            fwrite( &(srev.usiClearTime),     sizeof( unsigned short int ), 1, rev_wfile );
            fwrite( &(srev.usiFlashGreen),    sizeof( unsigned short int ), 1, rev_wfile );
            fwrite( &(srev.usiGreenTime),     sizeof( unsigned short int ), 1, rev_wfile );

            fwrite( &(srev.ucNonRevLight),    sizeof( unsigned char ), 1, rev_wfile );


            fclose(rev_wfile);
        }
        else perror("ERROR: write phase to file\n");

        //** 2: sync to segment[]

        //check iSelectSource
        reversetime[srev._reverse_time_type] = srev;

        /******** unlock mutex ********/
        pthread_mutex_unlock(&CReverseTimeInfo::_rev_mutex);

        ReverseTimeDataUpdate = true;

        /*
          if(smem.vGetUCData(TC_ReverseLane_Manual_Enable_Status) == 0) {
            vDetermine_ReverseTime();
          }
        */
        if(_exec_rev._reverse_time_type == srev._reverse_time_type)
        {
            vResetReverseTime();
        }

    }
    catch(...) {}
}
//----------------------------------------------------------
bool CSTC::Lock_to_Load_ReverseTime_Step1(const unsigned short int &usiSelectSource1, const unsigned short int &usiSelectSource2)
{
    try
    {
        if(usiSelectSource1 == 0)  //for center
        {
            Lock_to_Load_ReverseTime_Step2(_for_center_reversetime, usiSelectSource2);

        }
        else if (usiSelectSource1 == 1)       //for panel
        {
            Lock_to_Load_ReverseTime_Step2(_panel_reversetime, usiSelectSource2);

        }
        else if (usiSelectSource1 == 2 || usiSelectSource1 == 3)      //for in weekday
        {

            if(usiSelectSource2<1||(usiSelectSource2>7 && usiSelectSource2<11)||usiSelectSource2>17)
            {
                return false;
            }

            unsigned short int weekday_in_array, rev_data;
            if(usiSelectSource2<=7)
            {
                weekday_in_array = usiSelectSource2 - 1;                                  //0~6
            }
            else
            {
                weekday_in_array = usiSelectSource2 - 4;                                  //7~13
            }

            /******** lock mutex ********/
            pthread_mutex_lock(&CReverseTimeInfo::_rev_mutex);
            rev_data = weekdayrev[weekday_in_array]._reverse_time_type;
            /******** unlock mutex ********/
            pthread_mutex_unlock(&CReverseTimeInfo::_rev_mutex);

            if(usiSelectSource1 == 2)      // for center
                Lock_to_Load_ReverseTime_Step2(_for_center_reversetime, rev_data);
            else if(usiSelectSource1 == 3)   //for canel
                Lock_to_Load_ReverseTime_Step2(_panel_reversetime, rev_data);

        }
    }
    catch(...) {}
}
//----------------------------------------------------------
bool CSTC::Lock_to_Load_ReverseTime_Step2(CReverseTimeInfo &lrev, const unsigned short int &rev_data)
{
    try
    {
//  if(segment[segment_type]._ptr_seg_exec_time){
        /******** lock mutex ********/
        pthread_mutex_lock(&CReverseTimeInfo::_rev_mutex);
        lrev = reversetime[rev_data];

//      printf("OT load reverse time id: %d\n", reversetime._reverse_time_type);
        printf("OT load reverse usiDirectIn: %d\n", reversetime[rev_data].usiDirectIn);
        printf("OT load reverse usiHourStartIn: %d\n", reversetime[rev_data].usiHourStartIn);
        printf("OT load reverse usiMinStartIn: %d\n", reversetime[rev_data].usiMinStartIn);
        printf("OT load reverse usiHourEndIn: %d\n", reversetime[rev_data].usiHourEndIn);
        printf("OT load reverse usiMinEndIn: %d\n", reversetime[rev_data].usiMinEndIn);
        printf("OT load reverse usiDirectOut: %d\n", reversetime[rev_data].usiDirectOut);
        printf("OT load reverse usiHourStartOut: %d\n", reversetime[rev_data].usiHourStartOut);
        printf("OT load reverse usiMinStartOut: %d\n", reversetime[rev_data].usiMinStartOut);
        printf("OT load reverse usiHourEndOut: %d\n", reversetime[rev_data].usiHourEndOut);
        printf("OT load reverse usiMinEndOut: %d\n", reversetime[rev_data].usiMinEndOut);
        printf("OT load reverse usiClearTime: %d\n", reversetime[rev_data].usiClearTime);
        printf("OT load reverse usiFlashGreen: %d\n", reversetime[rev_data].usiFlashGreen);
        printf("OT load reverse usiGreenTime: %d\n", reversetime[rev_data].usiGreenTime);
        printf("OT load reverse ucNonRevLight: %d\n", reversetime[rev_data].ucNonRevLight);

        /******** unlock mutex ********/
        pthread_mutex_unlock(&CReverseTimeInfo::_rev_mutex);
        return true;
//  }
//  else return false;
    }
    catch (...) {}
}
//----------------------------------------------------------
void CSTC::Lock_to_Save_WeekDaySegment_from_Panel(void)
{
    try
    {
        /******** lock mutex ********/
        pthread_mutex_lock(&CSegmentInfo::_segment_mutex);

        //** 1: write to file
        FILE* weekdayseg_wfile=NULL;
        char filename[25];
        sprintf( filename,"/Data/WeekDaySegType.bin\0" );
        weekdayseg_wfile = fopen( filename, "w" );//fopen return NULL if file not exist

        if(weekdayseg_wfile)
        {

            printf("Writing File WeekDaySegType\n");

            for(int i=0; i<AMOUNT_WEEKDAY_SEG; i++)
            {
                fwrite( &_panel_weekdayseg[i]._segment_type, sizeof( unsigned short int ), 1, weekdayseg_wfile );
                fwrite( &_panel_weekdayseg[i]._weekday,      sizeof( unsigned short int ), 1, weekdayseg_wfile );
            }
            fclose( weekdayseg_wfile );
            //jacky20140319 為了寫Json檔
//    ReadSegmentData();
        }
        else perror("ERROR: write WeekDaySeg to file\n");

        //** 2: sync to weekdayseg[]
        for(int i=0; i<AMOUNT_WEEKDAY_SEG; i++)
            weekdayseg[i] = _panel_weekdayseg[i];

        /******** unlock mutex ********/
        pthread_mutex_unlock(&CSegmentInfo::_segment_mutex);
    }
    catch (...) {}
}
//----------------------------------------------------------
void CSTC::Lock_to_Save_WeekDaySegment_from_Center(void)
{
    try
    {
        /******** lock mutex ********/
        pthread_mutex_lock(&CSegmentInfo::_segment_mutex);

        //** 1: write to file
        FILE* weekdayseg_wfile=NULL;
        char filename[25];
        sprintf( filename,"/Data/WeekDaySegType.bin\0" );
        weekdayseg_wfile = fopen( filename, "w" );//fopen return NULL if file not exist

        if(weekdayseg_wfile)
        {

            printf("Writing File WeekDaySegType\n");

            for(int i=0; i<AMOUNT_WEEKDAY_SEG; i++)
            {
                fwrite( &_for_center_weekdayseg[i]._segment_type, sizeof( unsigned short int ), 1, weekdayseg_wfile );
                fwrite( &_for_center_weekdayseg[i]._weekday,      sizeof( unsigned short int ), 1, weekdayseg_wfile );
            }
            fclose( weekdayseg_wfile );
            //jacky20140319 為了寫Json檔
//    ReadSegmentData();
        }
        else perror("ERROR: write WeekDaySeg to file\n");

        //** 2: sync to weekdayseg[]
        for(int i=0; i<AMOUNT_WEEKDAY_SEG; i++)
            weekdayseg[i] = _for_center_weekdayseg[i];

        /******** unlock mutex ********/
        pthread_mutex_unlock(&CSegmentInfo::_segment_mutex);
    }
    catch (...) {}
}
//----------------------------------------------------------
void CSTC::Lock_to_Save_WeekDaySegment_from_Web(void)
{
    try
    {
        /******** lock mutex ********/
        pthread_mutex_lock(&CSegmentInfo::_segment_mutex);

        //** 1: write to file
        FILE* weekdayseg_wfile=NULL;
        char filename[25];
        sprintf( filename,"/Data/WeekDaySegType.bin\0" );
        weekdayseg_wfile = fopen( filename, "w" );//fopen return NULL if file not exist

        if(weekdayseg_wfile)
        {

            printf("Writing File WeekDaySegType\n");

            for(int i=0; i<AMOUNT_WEEKDAY_SEG; i++)
            {
                fwrite( &_for_web_weekdayseg[i]._segment_type, sizeof( unsigned short int ), 1, weekdayseg_wfile );
                fwrite( &_for_web_weekdayseg[i]._weekday,      sizeof( unsigned short int ), 1, weekdayseg_wfile );
            }
            fclose( weekdayseg_wfile );
            //jacky20140319 為了寫Json檔
//    ReadSegmentData();
        }
        else perror("ERROR: write WeekDaySeg to file\n");

        //** 2: sync to weekdayseg[]
        for(int i=0; i<AMOUNT_WEEKDAY_SEG; i++)
            weekdayseg[i] = _for_web_weekdayseg[i];

        /******** unlock mutex ********/
        pthread_mutex_unlock(&CSegmentInfo::_segment_mutex);
    }
    catch (...) {}
}
//----------------------------------------------------------
void CSTC::Lock_to_Save_HoliDaySegment(const CHoliDaySegType &sholidaysegtype)
{
    try
    {
        /******** lock mutex ********/
        pthread_mutex_lock(&CSegmentInfo::_segment_mutex);

        //** 1: write to file
        FILE* holidayseg_wfile=NULL;
        char filename[25];
        sprintf( filename,"/Data/HoliDaySegType.bin\0" );
        holidayseg_wfile = fopen(filename, "w"); //fopen return NULL if file not exist
        if(holidayseg_wfile)
        {

            printf("Writing File HoliDaySegType\n");

            for(int i=0; i<AMOUNT_HOLIDAY_SEG; i++)
            {
                if(i == (sholidaysegtype._segment_type-8) )
                {
                    fwrite( &sholidaysegtype._segment_type, sizeof( unsigned short int ), 1, holidayseg_wfile );
                    fwrite( &sholidaysegtype._start_year,   sizeof( unsigned short int ), 1, holidayseg_wfile );
                    fwrite( &sholidaysegtype._start_month,  sizeof( unsigned short int ), 1, holidayseg_wfile );
                    fwrite( &sholidaysegtype._start_day,    sizeof( unsigned short int ), 1, holidayseg_wfile );
                    fwrite( &sholidaysegtype._end_year,     sizeof( unsigned short int ), 1, holidayseg_wfile );
                    fwrite( &sholidaysegtype._end_month,    sizeof( unsigned short int ), 1, holidayseg_wfile );
                    fwrite( &sholidaysegtype._end_day,      sizeof( unsigned short int ), 1, holidayseg_wfile );
                }
                else
                {
                    fwrite( &holidayseg[i]._segment_type, sizeof( unsigned short int ), 1, holidayseg_wfile );
                    fwrite( &holidayseg[i]._start_year,   sizeof( unsigned short int ), 1, holidayseg_wfile );
                    fwrite( &holidayseg[i]._start_month,  sizeof( unsigned short int ), 1, holidayseg_wfile );
                    fwrite( &holidayseg[i]._start_day,    sizeof( unsigned short int ), 1, holidayseg_wfile );
                    fwrite( &holidayseg[i]._end_year,     sizeof( unsigned short int ), 1, holidayseg_wfile );
                    fwrite( &holidayseg[i]._end_month,    sizeof( unsigned short int ), 1, holidayseg_wfile );
                    fwrite( &holidayseg[i]._end_day,      sizeof( unsigned short int ), 1, holidayseg_wfile );
                }
            }
            fclose( holidayseg_wfile );
            //jacky20140319 為了寫Json檔
//    ReadSegmentData();
        }

        //** 2: sync to holidayseg[]
        holidayseg[sholidaysegtype._segment_type-8] = sholidaysegtype;

        /******** unlock mutex ********/
        pthread_mutex_unlock(&CSegmentInfo::_segment_mutex);
    }
    catch (...) {}
}
//----------------------------------------------------------
void CSTC::Lock_to_Save_HoliDaySegment_from_Panel(void)
{
    Lock_to_Save_HoliDaySegment(_panel_holidayseg);
}
//----------------------------------------------------------
void CSTC::Lock_to_Save_HoliDaySegment_from_Center(void)
{
    Lock_to_Save_HoliDaySegment(_for_center_holidayseg);
}
//---------------------------------------------------------------------
void CSTC::Lock_to_Save_HoliDayReverseTime_Step1(int iSelectSource)
{
    if(iSelectSource == 0)
    {
        Lock_to_Save_HolidayReverseTime_Step2(_for_center_holidayrev);
    }
    else
    {
        Lock_to_Save_HolidayReverseTime_Step2(_panel_holidayrev);
    }
}
//----------------------------------------------------------------
void CSTC::Lock_to_Save_HolidayReverseTime_Step2(const CHoliDayRevType &sholidayrev)
{
    try
    {
        /******** lock mutex ********/
        pthread_mutex_lock(&CReverseTimeInfo::_rev_mutex);

        //** 1: write to file
        FILE* holidayrev_wfile=NULL;
        char filename[25];
        sprintf( filename,"/Data/HoliDayRevType.bin\0" );
        holidayrev_wfile = fopen(filename, "w"); //fopen return NULL if file not exist
        if(holidayrev_wfile)
        {

            printf("Writing File HoliDayRevType\n");
            printf( "sholidayrev._reverse_time_type %d\n", sholidayrev._reverse_time_type );
            printf( "sholidayrev._start_year %d\n",   sholidayrev._start_year);


            for(int i=0; i<AMOUNT_HOLIDAY_REV; i++)
            {
                if(i == (sholidayrev._reverse_time_type - 4) )
                {
                    fwrite( &sholidayrev._reverse_time_type, sizeof( unsigned short int ), 1, holidayrev_wfile );
                    fwrite( &sholidayrev._start_year,   sizeof( unsigned short int ), 1, holidayrev_wfile );
                    fwrite( &sholidayrev._start_month,  sizeof( unsigned short int ), 1, holidayrev_wfile );
                    fwrite( &sholidayrev._start_day,    sizeof( unsigned short int ), 1, holidayrev_wfile );
                    fwrite( &sholidayrev._end_year,     sizeof( unsigned short int ), 1, holidayrev_wfile );
                    fwrite( &sholidayrev._end_month,    sizeof( unsigned short int ), 1, holidayrev_wfile );
                    fwrite( &sholidayrev._end_day,      sizeof( unsigned short int ), 1, holidayrev_wfile );
                }
                else
                {
                    fwrite( &holidayrev[i]._reverse_time_type, sizeof( unsigned short int ), 1, holidayrev_wfile );
                    fwrite( &holidayrev[i]._start_year,   sizeof( unsigned short int ), 1, holidayrev_wfile );
                    fwrite( &holidayrev[i]._start_month,  sizeof( unsigned short int ), 1, holidayrev_wfile );
                    fwrite( &holidayrev[i]._start_day,    sizeof( unsigned short int ), 1, holidayrev_wfile );
                    fwrite( &holidayrev[i]._end_year,     sizeof( unsigned short int ), 1, holidayrev_wfile );
                    fwrite( &holidayrev[i]._end_month,    sizeof( unsigned short int ), 1, holidayrev_wfile );
                    fwrite( &holidayrev[i]._end_day,      sizeof( unsigned short int ), 1, holidayrev_wfile );
                }
            }
            fclose( holidayrev_wfile );
        }

        //** 2: sync to holidayseg[]
        holidayrev[sholidayrev._reverse_time_type - 4] = sholidayrev;

        /******** unlock mutex ********/
        pthread_mutex_unlock(&CReverseTimeInfo::_rev_mutex);
    }
    catch (...) {}
}
//ReverseTime
//----------------------------------------------------------
void CSTC::Lock_to_Save_WeekDayReversetime_from_Panel(void)
{
    try
    {
        /******** lock mutex ********/
        pthread_mutex_lock(&CReverseTimeInfo::_rev_mutex);

        //** 1: write to file
        FILE* weekdayrev_wfile=NULL;
        char filename[25];
        sprintf( filename,"/Data/WeekDayRevType.bin\0" );
        weekdayrev_wfile = fopen( filename, "w" );//fopen return NULL if file not exist

        if(weekdayrev_wfile)
        {

            printf("Writing File WeekDayRevType\n");

            for(int i=0; i<AMOUNT_WEEKDAY_REV; i++)
            {
                fwrite( &_panel_weekdayrev[i]._reverse_time_type, sizeof( unsigned short int ), 1, weekdayrev_wfile );
                fwrite( &_panel_weekdayrev[i]._weekday,      sizeof( unsigned short int ), 1, weekdayrev_wfile );
            }
            fclose( weekdayrev_wfile );
        }
        else perror("ERROR: write WeekDayRev to file\n");

        //** 2: sync to weekdayseg[]
        for(int i=0; i<AMOUNT_WEEKDAY_REV; i++)
            weekdayrev[i] = _panel_weekdayrev[i];

        /******** unlock mutex ********/
        pthread_mutex_unlock(&CReverseTimeInfo::_rev_mutex);
    }
    catch (...) {}
}
//----------------------------------------------------------
void CSTC::Lock_to_Save_WeekDayReversetime_from_Center(void)
{
    try
    {
        /******** lock mutex ********/
        pthread_mutex_lock(&CReverseTimeInfo::_rev_mutex);

        //** 1: write to file
        FILE* weekdayrev_wfile=NULL;
        char filename[25];
        sprintf( filename,"/Data/WeekDayRevType.bin\0" );
        weekdayrev_wfile = fopen( filename, "w" );//fopen return NULL if file not exist

        if(weekdayrev_wfile)
        {

            printf("Writing File WeekDayRevType\n");

            for(int i=0; i<AMOUNT_WEEKDAY_REV; i++)
            {
                fwrite( &_for_center_weekdayrev[i]._reverse_time_type, sizeof( unsigned short int ), 1, weekdayrev_wfile );
                fwrite( &_for_center_weekdayrev[i]._weekday,      sizeof( unsigned short int ), 1, weekdayrev_wfile );
            }
            fclose( weekdayrev_wfile );
        }
        else perror("ERROR: write WeekDayRev to file\n");

        //** 2: sync to weekdayseg[]
        for(int i=0; i<AMOUNT_WEEKDAY_REV; i++)
            weekdayrev[i] = _for_center_weekdayrev[i];

        /******** unlock mutex ********/
        pthread_mutex_unlock(&CReverseTimeInfo::_rev_mutex);
    }
    catch (...) {}
}
//----------------------------------------------------------
//**********************************************************
//      Read and Reset Timers for Resetting CMOS Time
//**********************************************************
//----------------------------------------------------------
bool CSTC::TimersRead_BeforeResetCMOSTime(void)
{
    try
    {

        pthread_mutex_lock(&CSTC::_stc_mutex);
        struct itimerspec _itZero;
        _itZero.it_value.tv_sec = 0;
        _itZero.it_value.tv_nsec = 0;
        _itZero.it_interval.tv_sec = 0;
        _itZero.it_interval.tv_nsec = 0;

        timer_gettime(_timer_plan,&_itimer_plan);

        //For ReverseTime
        timer_gettime(_timer_reversetime,&_itimer_reversetime);

        timer_settime(_timer_plan, 0, &_itZero, NULL);
        timer_settime(_timer_reversetime, 0, &_itZero, NULL);
        timer_settime(_timer_panelcount, 0, &_itZero, NULL);
        timer_settime(_timer_reportcount, 0, &_itZero, NULL);
        timer_settime(_timer_record_traffic, 0, &_itZero, NULL);
        timer_settime(_timer_redcount, 0, &_itZero, NULL);

        pthread_mutex_unlock(&CSTC::_stc_mutex);

        return true;
    }
    catch(...) {}
}
//----------------------------------------------------------
bool CSTC::TimersReset_AfterResetCMOSTime(void)
{
    try
    {

        pthread_mutex_lock(&CSTC::_stc_mutex);

        for(int i = 0; i < 4; i++)
        {

            TimersSetting();

            if(_itimer_plan.it_value.tv_sec == 0 && _itimer_plan.it_value.tv_nsec == 0)
            {
                smem.vWriteMsgToDOM("timer plan, all zero!");
                _itimer_plan.it_value.tv_sec = 1;
            }
            timer_settime(_timer_plan, 0, &_itimer_plan, NULL);

            _itimer_plan_WDT = _itimer_plan; /*OT_PLAN_WDT*/
            _itimer_plan_WDT.it_value.tv_sec += 2;
            if(timer_settime( _timer_plan_WDT, 0, & _itimer_plan_WDT, NULL ))
            {
                printf("PlanWDT Settime Error!.\n");
                exit( 1 );
            }
//  timer_settime( _timer_redcount, 0, & _itimer_redcount, NULL );



            //For ReverseTime
            if( _exec_reversetime_current_rev_step == 0 ||                                //wait for next reverseTime
                    _exec_reversetime_current_rev_step == 1 ||
                    _exec_reversetime_current_rev_step == 11  )
            {
                //OTBUG +12
                _exec_reversetime_current_rev_step = 0;
                if(smem.vGetUCData(TC_ReverseLane_Manual_Enable_Status) == 0)
                {
                    vDetermine_ReverseTime();
                }
            }
            else if (_exec_reversetime_current_rev_step == 5)
            {
                _exec_reversetime_current_rev_step = 4;
                if(smem.vGetUCData(TC_ReverseLane_Manual_Enable_Status) == 0)
                {
                    vDetermine_ReverseTime();
                }
            }
            else if (_exec_reversetime_current_rev_step == 15)
            {
                _exec_reversetime_current_rev_step = 14;
                if(smem.vGetUCData(TC_ReverseLane_Manual_Enable_Status) == 0)
                {
                    vDetermine_ReverseTime();
                }
            }
            else
            {
                for(int ii = 0; ii < 4; ii++)
                {
                    timer_settime(_timer_reversetime, 0, &_itimer_reversetime, NULL);
                }
            }

        }

        pthread_mutex_unlock(&CSTC::_stc_mutex);


        return true;


    }
    catch(...) {}
}
//----------------------------------------------------------
//**********************************************************
//              Lock to Set Control Strategy
//**********************************************************
//----------------------------------------------------------
bool CSTC::Lock_to_Set_Control_Strategy(const ControlStrategy new_cs)
{
    try
    {

        printf("\n     !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
        printf(  "     !!!    Lock_to_Set_Control_Strategy     !!!\n");
        printf(  "     !!!         old:%2d     new:%2d          !!!\n",_current_strategy, new_cs);
        printf(  "     !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n\n");


        bool success=false;
        if(new_cs==_current_strategy) return success;

        ReportCurrentControlStrategy(0);
        /******** lock mutex ********/
        pthread_mutex_lock(&CSTC::_stc_mutex);
        _old_strategy=_current_strategy;
        _current_strategy=new_cs;
        //For Kernel 2.6
        if(_stc_thread_light_control != 0)
            if(pthread_kill(_stc_thread_light_control,SIGNAL_STRATEGY_CHANGED)==0) success=true;

        pthread_mutex_unlock(&CSTC::_stc_mutex);
        ReportCurrentControlStrategy(1);

        if(_current_strategy == STRATEGY_ALLRED)
        {
//    stc.vReportFieldOperate_5F08(0x02);
            smem.vSetTC5F08StatusChange(true);  //interval data, force send.

            smem.vSetUCData(TC92_ucControlStrategy, 0x04);
            smem.vSetUCData(TC_CSTC_ControlStrategy, 90);
            smem.vWriteMsgToDOM("ChangeControlStrategy To ALLRED");
        }
        else if (_current_strategy == STRATEGY_FLASH)
        {
//    stc.vReportFieldOperate_5F08(0x40);
            smem.vSetTC5F08StatusChange(true);  //interval data, force send.

            smem.vSetUCData(TC92_ucControlStrategy, 0x04);
            smem.vSetUCData(TC_CSTC_ControlStrategy, 80);
            smem.vWriteMsgToDOM("ChangeControlStrategy To FLASH");
        }
        else if (_current_strategy == STRATEGY_MANUAL)
        {
//    stc.vReportFieldOperate_5F08(0x01);
            smem.vSetTC5F08StatusChange(true);  //interval data, force send.

            smem.vSetUCData(TC92_ucControlStrategy, 0x04);
            smem.vSetUCData(TC_CSTC_ControlStrategy, 70);
            smem.vWriteMsgToDOM("ChangeControlStrategy To MANUAL");
        }
        else if (_current_strategy == STRATEGY_ALLDYNAMIC)
        {
//    stc.vReportFieldOperate_5F08(0x01);
//ot why 0x10?    smem.vSetUCData(TC92_ucControlStrategy, 0x10);
            smem.vSetUCData(TC92_ucControlStrategy, 0x30);
            smem.vSetUCData(TC_CSTC_ControlStrategy, 95);
            smem.vWriteMsgToDOM("ChangeControlStrategy To ALLDYNAMIC");
        }
        else
        {
//    stc.vReportFieldOperate_5F08(0x80);
            smem.vSetTC5F08StatusChange(true);  //interval data, force send.

            smem.vSetUCData(TC92_ucControlStrategy, 0x01);
            smem.vSetUCData(TC_CSTC_ControlStrategy, 10);
            smem.vWriteMsgToDOM("ChangeControlStrategy To DEFAULT(TOD)");
        }

        stc.vSendControlStrategyToCCJ();

        screenStrategy.DisplayCStrategy();

        return success;
    }
    catch (...) {}
}
//----------------------------------------------------------
//**********************************************************
//              Lock to Set Next Step
//**********************************************************
//----------------------------------------------------------
bool CSTC::Lock_to_Set_Next_Step(void)
{
    try
    {
        if(pthread_kill(_stc_thread_light_control,SIGNAL_NEXT_STEP)!=0) return false;
        return true;
    }
    catch (...) {}
}
//----------------------------------------------------------
//**********************************************************
//              Lock to Modify CADC offset
//**********************************************************
//----------------------------------------------------------
void CSTC::TestMode_Modify_CADC_offset(int offset)
{
    try
    {
        pthread_mutex_lock(&CSTC::_stc_mutex);
        if( (_current_strategy==STRATEGY_CADC || _current_strategy==STRATEGY_AUTO_CADC )
                && _exec_plan._planid == CADC_PLANID &&_exec_plan._phase_order != FLASH_PHASEORDER  &&_exec_plan._phase_order!=ALLRED_PHASEORDER )
        {
            pthread_mutex_lock(&CPlanInfo::_plan_mutex);


            printf("\n     #####################################################\n");
            printf(  "     ###         STRATEGY:%2d   plan:%2d  phase:%#X        ###\n",_current_strategy, _exec_plan._planid, _exec_phase._phase_order);
            printf(  "     ### plan[CADC]._offset=%3d _exec_plan._offset=%3d ###\n", plan[CADC_PLANID]._offset, _exec_plan._offset);
            printf(  "     #####################################################\n");


            offset = ( (offset>=0) ? (offset%plan[CADC_PLANID].calculated_cycle_time())
                       : (plan[CADC_PLANID].calculated_cycle_time()+(offset%plan[CADC_PLANID].calculated_cycle_time())));

            printf("    offset = %d\n", offset);


            int diff = (offset+offset_not_been_adjusted_by_CADC) - plan[CADC_PLANID]._offset;
            if( diff>0 )
            {
                plan[CADC_PLANID]._offset = offset;
                AdjustOffset_of_CurrentCycle_in_CADC( diff );
                offset_not_been_adjusted_by_CADC=0;
            }
            else if( diff<0)
            {
                plan[CADC_PLANID]._offset = offset;
                AdjustOffset_of_CurrentCycle_in_CADC( diff/2 );
                offset_not_been_adjusted_by_CADC = (diff-(diff/2));
            }

            printf("     #####################################################\n");
            printf("     ###         STRATEGY:%2d   plan:%2d  phase:%#X        ###\n",_current_strategy, _exec_plan._planid, _exec_phase._phase_order);
            printf("     ### plan[CADC]._offset=%3d _exec_plan._offset=%3d ###\n", plan[CADC_PLANID]._offset, _exec_plan._offset);
            printf("     #####################################################\n\n");


            pthread_mutex_unlock(&CPlanInfo::_plan_mutex);
        }
        pthread_mutex_unlock(&CSTC::_stc_mutex);
    }
    catch(...) {}
}
//----------------------------------------------------------
//----------------------------------------------------------
//**********************************************************
//              Lock to Save DownCrossing signal
//**********************************************************
//----------------------------------------------------------
bool CSTC::Lock_to_Save_DownCrossing(void)
{
    try
    {
        bool success=false;
        if (down_crossing_STC.current_step == down_crossing_from_DC.current_step)  return success;  // error?

        /******** lock mutex ********/
        pthread_mutex_lock(&CSTC::_stc_mutex);
        down_crossing_STC = down_crossing_from_DC;
        /******** unlock mutex ********/
        pthread_mutex_unlock(&CSTC::_stc_mutex);
        return success;
    }
    catch (...) {}
}

//OTCombo
//--------------------------------------------------------
unsigned short int CSTC::vGetUSIData(int iSelect)
{
    try
    {

        int usiRet;
        switch(iSelect)
        {

        case(CSTC_exec_phase_current_subphase):
//      pthread_mutex_lock(&mutexCSTCSmem);
            usiRet = _exec_phase_current_subphase;
//      pthread_mutex_unlock(&mutexCSTCSmem);
            break;

        case(CSTC_exec_phase_current_subphase_step):
//      pthread_mutex_lock(&mutexCSTCSmem);
            usiRet = _exec_phase_current_subphase_step;
//      pthread_mutex_unlock(&mutexCSTCSmem);
            break;

        case(CSTC_exec_segment_current_seg_no):
//      pthread_mutex_lock(&mutexCSTCSmem);
            usiRet = _exec_segment_current_seg_no;
//      pthread_mutex_unlock(&mutexCSTCSmem);
            break;

        case(CSTC_exec_plan_phase_order):
            usiRet = _exec_plan._phase_order;
            break;

        case(CSTC_exec_plan_plan_ID):
            usiRet = _exec_plan._planid;
            break;

        case(CSTC_exec_plan_green_time):
            pthread_mutex_lock(&CPlanInfo::_plan_mutex);
            usiRet = _exec_plan._ptr_subplaninfo[_exec_phase_current_subphase]._green;
            pthread_mutex_unlock(&CPlanInfo::_plan_mutex);
            break;
        case(CSTC_exec_plan_pedgreenflash_time):
            pthread_mutex_lock(&CPlanInfo::_plan_mutex);
            usiRet = _exec_plan._ptr_subplaninfo[_exec_phase_current_subphase]._pedgreen_flash;
            pthread_mutex_unlock(&CPlanInfo::_plan_mutex);
            break;
        case(CSTC_exec_plan_pedred_time):
            pthread_mutex_lock(&CPlanInfo::_plan_mutex);
            usiRet = _exec_plan._ptr_subplaninfo[_exec_phase_current_subphase]._pedred;
            pthread_mutex_unlock(&CPlanInfo::_plan_mutex);
            break;
        case(CSTC_exec_plan_yellow_time):
            pthread_mutex_lock(&CPlanInfo::_plan_mutex);
            usiRet = _exec_plan._ptr_subplaninfo[_exec_phase_current_subphase]._yellow;
            pthread_mutex_unlock(&CPlanInfo::_plan_mutex);
            break;
        case(CSTC_exec_plan_allred_time):
            pthread_mutex_lock(&CPlanInfo::_plan_mutex);
            usiRet = _exec_plan._ptr_subplaninfo[_exec_phase_current_subphase]._allred;
            pthread_mutex_unlock(&CPlanInfo::_plan_mutex);
            break;
        case(CSTC_exec_segment_type)://arwen++
//      pthread_mutex_lock(&mutexCSTCSmem);
            usiRet = _exec_segment._segment_type;
//      pthread_mutex_unlock(&mutexCSTCSmem);
            break;

            //OT20140214
        case(CSTC_exec_plan_maxgreen_time):
            pthread_mutex_lock(&CPlanInfo::_plan_mutex);
            usiRet = _exec_plan._ptr_subplaninfo[_exec_phase_current_subphase]._max_green;
            pthread_mutex_unlock(&CPlanInfo::_plan_mutex);
            break;
        case(CSTC_exec_plan_mingreen_time):
            pthread_mutex_lock(&CPlanInfo::_plan_mutex);
            usiRet = _exec_plan._ptr_subplaninfo[_exec_phase_current_subphase]._min_green;
            pthread_mutex_unlock(&CPlanInfo::_plan_mutex);
            break;
        case(CSTC_exec_plan_green_time_compesated):
            pthread_mutex_lock(&CPlanInfo::_plan_mutex);
            usiRet = _exec_plan._ptr_subplaninfo[_exec_phase_current_subphase].compensated_green(_exec_plan._shorten_cycle);
            pthread_mutex_unlock(&CPlanInfo::_plan_mutex);
            break;
        case(CSTC_exec_BF13plan_Extend_East):
            pthread_mutex_lock(&CPlanInfo::_plan_mutex);
            usiRet = _exec_BRTPriority_plan._east;
            pthread_mutex_unlock(&CPlanInfo::_plan_mutex);
            break;
        case(CSTC_exec_BF13plan_Extend_West):
            pthread_mutex_lock(&CPlanInfo::_plan_mutex);
            usiRet = _exec_BRTPriority_plan._west;
            pthread_mutex_unlock(&CPlanInfo::_plan_mutex);
            break;
            //OT20140415
        case(CSTC_exec_plan_cycle):
            pthread_mutex_lock(&CPlanInfo::_plan_mutex);
            usiRet = _exec_plan.calculated_cycle_time();
            pthread_mutex_unlock(&CPlanInfo::_plan_mutex);
            break;
        default:
            usiRet = 0;
            break;
        }
        return usiRet;

    }
    catch(...) {}
}


//OTADD
//----------------------------------------------------------
bool CSTC::vLockToSetControlStrategyToManualNotBySignal(void)
{
    try
    {

        bool success=false;
        if(_current_strategy == STRATEGY_ALLDYNAMIC) return success;
        //sould add addred & flash


        pthread_mutex_lock(&CSTC::_stc_mutex);
        _old_strategy=_current_strategy;
        _current_strategy=STRATEGY_ALLDYNAMIC;
        pthread_mutex_unlock(&CSTC::_stc_mutex);

        ReSetStep(false);
        ReSetExtendTimer();
//  SetLightAfterExtendTimerReSet();

        return success;
    }
    catch (...) {}
}

//OTADD
//----------------------------------------------------------
bool CSTC::vLockToSetControlStrategyToTODNotBySignal(void)
{
    try
    {

        bool success=false;
        if(STRATEGY_TOD == _current_strategy) return success;
        //sould add addred & flash

        pthread_mutex_lock(&CSTC::_stc_mutex);
        _old_strategy=_current_strategy;
        _current_strategy=STRATEGY_TOD;
        pthread_mutex_unlock(&CSTC::_stc_mutex);

        ReSetStep(true);
        ReSetExtendTimer();
        SetLightAfterExtendTimerReSet();

        return success;
    }
    catch (...) {}
}

//------------------------------------------------------------------------
bool CSTC::vReportFieldOperate_5F08(int iFieldOperate)
{
    try
    {
        unsigned char data[3];

        smem.vSetUCData(TC_CSTC_FieldOperate, iFieldOperate);

        data[0] = 0x5F;
        data[1] = 0x08;
        data[2] = iFieldOperate;

        if(data[2] == 0x80 && smem.vGetUCData(TC_TrainChainEnable) == 1)    //Railway switch Enable.
        {
            if(smem.vGetUCData(TC_TrainChainNOW) == 1)    //train coming
            {
                data[2] = 0x10;               //Define by CCT
            }
        }

        smem.vSetTC5F08Status(data[2]);

        MESSAGEOK _MsgOK;

        _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, 3, true);
        _MsgOK.InnerOrOutWard = cOutWard;
//    writeJob.WriteWorkByMESSAGEOUT(_MsgOK);
        writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICECENTER92);

        return true;
    }
    catch (...) {}
}
//----------------------------------------------------------
unsigned short int CSTC::vGetStepTime(void)
{
    try
    {

        //Should be mutex

        unsigned short int time_difference=0;
//  if(_current_strategy==STRATEGY_MANUAL||_current_strategy==STRATEGY_FLASH||_current_strategy==STRATEGY_ALLRED || _current_strategy==STRATEGY_ALLDYNAMIC){
        if(_current_strategy==STRATEGY_MANUAL||_current_strategy==STRATEGY_FLASH||_current_strategy==STRATEGY_ALLRED)
        {
            timespec strategy_current_time= {0,0};
            if(clock_gettime(CLOCK_REALTIME, &strategy_current_time)<0) perror("Can not get current time");
            time_difference = (strategy_current_time.tv_sec - strategy_start_time.tv_sec);
        }
        else
        {
            if(_exec_plan._planid!=FLASH_PLANID && _exec_plan._planid!=ALLRED_PLANID)
            {
                timer_gettime(_timer_plan,&_itimer_plan);
                time_difference = (_itimer_plan.it_value.tv_sec) + 1;
            }
        }

        if(_current_strategy == STRATEGY_ALLDYNAMIC)
            time_difference = _intervalTimer.vGetEffectTime();

        return time_difference;

    }
    catch(...) {}
}
// time_difference&0xFF00)>>8;
// time_difference&0xFF);
//----------------------------------------------------------
bool CSTC::vReportGoToNextPhaseStep_5F0C(void)
{
    try
    {
        unsigned char data[5];

        data[0] = 0x5F;
//OT Debug 0410
        data[1] = 0x0C;
        data[2] = smem.vGetUCData(TC92_ucControlStrategy);
        data[3] = _exec_phase_current_subphase + 1;
        data[4] = _exec_phase_current_subphase_step + 1;

        MESSAGEOK _MsgOK;

        _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, 5, true);
        _MsgOK.InnerOrOutWard = cOutWard;
//  writeJob.WriteWorkByMESSAGEOUT(_MsgOK);
        writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICECENTER92);

//OT20111128
        // if(MACHINELOCATE == MACHINELOCATEATYULIN)                                     // For CCJ
        if(smem.vGetUCData(TC_CCT_MachineLocation) == MACHINELOCATEATYULIN)
        {
            writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICECCJDYNCTL);
        }


        return true;

    }
    catch(...) {}
}
//----------------------------------------------------------
void CSTC::vReport5F1CWorkingStepChange(void)
{
    try
    {
        unsigned char data[6];
        data[0] = 0x5F;
        data[1] = 0x0C;
        data[2] = smem.vGetUCData(TC92_ucControlStrategy);
        data[3] = _exec_phase_current_subphase + 1;
        data[4] = _exec_phase_current_subphase_step + 1;

        MESSAGEOK _MsgOK;
        _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, 5, true);
        _MsgOK.InnerOrOutWard = cOutWard;
//  writeJob.WriteWorkByMESSAGEOUT(_MsgOK);
        writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICECENTER92);

    }
    catch (...) {}
}

//----------------------------------------------------------
void CSTC::vReport5F80CCTProtocalSendStep(void)
{
    try
    {
        unsigned char data[6];
        /*mask for CECI fuck comm server*/
        /*20091110 come back*/
        /*test for tainan 20090226*  (--5f80)*/
        data[0] = 0x5F;
        data[1] = 0x80;
        data[2] = (_exec_segment._segment_type)&0xFF;
        data[3] = (_exec_segment_current_seg_no+1)&0xFF;
        data[4] = (_exec_phase.calculated_total_step_count())&0xFF;
        data[5] = (_exec_phase.step_no_of_all(_exec_phase_current_subphase, _exec_phase_current_subphase_step)+1)&0xFF;

        MESSAGEOK _MsgOK;
        _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, 6, true);
        _MsgOK.InnerOrOutWard = cOutWard;
        //5F80 don't resend
        writeJob.WritePhysicalOutNoSetSeqNoResend(_MsgOK.packet, _MsgOK.packetLength, DEVICECENTER92);
        /**/

        /**/

    }
    catch (...) {}
}

//OT20131221
//----------------------------------------------------------
void CSTC::vCalculateAndSendPeopleLightCount(void)
{
    try
    {
        unsigned char data[10];
        unsigned char ucPGTime[4] = {0, 0, 0, 0};
        unsigned short int usiPG[4] = {0, 0, 0, 0};
//  bool bPF[4] = {false, false, false, false};

        unsigned short int usiPGTime = 0;
//  unsigned char ucGC;
//  unsigned char ucRC;
        unsigned short int usiPGF = 0;
//  unsigned short int usi
        DATA_Bit DBGC;
        DATA_Bit DBRC;
//  unsigned short int usiLightAt;

        if(smem.vGetINTData(Com2_TYPE) == Com2IsTainanPeopleLight)
        {

            //init GC RC
            DBGC.DBit = 0;             // all no light
            DBRC.DBit = 0xF0;          // all red light

            if( _exec_plan._phase_order==FLASH_PHASEORDER || _exec_plan._phase_order==ALLRED_PHASEORDER || _current_strategy==STRATEGY_MANUAL || _exec_plan._subphase_count > 4)
            {
                ;    //Do nothing
            }
            else
            {
                if(_exec_phase_current_subphase_step == 0)
                {
                    usiPGTime =  _exec_plan._ptr_subplaninfo[_exec_phase_current_subphase].compensated_green(_exec_plan._shorten_cycle);
                    usiPGTime += _exec_plan._ptr_subplaninfo[_exec_phase_current_subphase].compensated_pedgreen_flash(_exec_plan._shorten_cycle);
                    usiPGF = 1;
                }
                else if (_exec_phase_current_subphase_step == 1)
                {
                    usiPGTime = _exec_plan._ptr_subplaninfo[_exec_phase_current_subphase].compensated_pedgreen_flash(_exec_plan._shorten_cycle);
                    usiPGF = 2;
                }
            }

            //put usiPGTime to right place
            if(_exec_phase_current_subphase < 4)
            {
                ucPGTime[_exec_phase_current_subphase] = usiPGTime & 0xFF;
                usiPG[_exec_phase_current_subphase] = usiPGF;
            }
            else
            {
                printf("printfMsg error, too many subphase.\n");
            }

            for(int i = 0; i < 4; i ++)
            {
                if(smem.vGetWayMappingRedCount(i) == 0)
                {
                    data[i+5] = ucPGTime[0] & 0xFF;
                }
                else if(smem.vGetWayMappingRedCount(i) == 1)
                {
                    data[i+5] = ucPGTime[1] & 0xFF;
                }
                else if(smem.vGetWayMappingRedCount(i) == 2)
                {
                    data[i+5] = ucPGTime[2] & 0xFF;
                }
                else if(smem.vGetWayMappingRedCount(i) == 3)
                {
                    data[i+5] = ucPGTime[3] & 0xFF;
                }
                else
                {
                    data[i+5] = 0;
                }
            }

            for(int i = 0; i < 4; i++)
            {
                if(smem.vGetWayMappingRedCount(i) == 0)
                {
                    if(i==0)
                    {
                        if( usiPG[0] == 1 )
                        {
                            DBGC.switchBit.b8 = true;        //G1
                            DBRC.switchBit.b8 = false;       //R1
                        }
                        else if ( usiPG[0] == 2 )
                        {
                            DBGC.switchBit.b8 = true;        //G1
                            DBGC.switchBit.b4 = true;        //G1 flash
                            DBRC.switchBit.b8 = false;       //R1
                        }
                    }
                    else if(i==1)
                    {
                        if( usiPG[0] == 1 )
                        {
                            DBGC.switchBit.b7 = true;        //G2
                            DBRC.switchBit.b7 = false;       //R2
                        }
                        else if ( usiPG[0] == 2 )
                        {
                            DBGC.switchBit.b7 = true;        //G2
                            DBGC.switchBit.b3 = true;        //G2 flash
                            DBRC.switchBit.b7 = false;       //R2
                        }
                    }
                    else if(i==2)
                    {
                        if( usiPG[0] == 1 )
                        {
                            DBGC.switchBit.b6 = true;        //G3
                            DBRC.switchBit.b6 = false;       //R3
                        }
                        else if ( usiPG[0] == 2 )
                        {
                            DBGC.switchBit.b6 = true;        //G3
                            DBGC.switchBit.b2 = true;        //G3 flash
                            DBRC.switchBit.b6 = false;       //R3
                        }
                    }
                    else if(i==3)
                    {
                        if( usiPG[0] == 1 )
                        {
                            DBGC.switchBit.b5 = true;        //G4
                            DBRC.switchBit.b5 = false;       //R4
                        }
                        else if ( usiPG[0] == 2 )
                        {
                            DBGC.switchBit.b5 = true;        //G4
                            DBGC.switchBit.b1 = true;        //G4 flash
                            DBRC.switchBit.b5 = false;       //R4
                        }
                    }
                }
                else if(smem.vGetWayMappingRedCount(i) == 1)
                {
                    if(i==0)
                    {
                        if( usiPG[1] == 1 )
                        {
                            DBGC.switchBit.b8 = true;        //G1
                            DBRC.switchBit.b8 = false;       //R1
                        }
                        else if ( usiPG[1] == 2 )
                        {
                            DBGC.switchBit.b8 = true;        //G1
                            DBGC.switchBit.b4 = true;        //G1 flash
                            DBRC.switchBit.b8 = false;       //R1
                        }
                    }
                    else if(i==1)
                    {
                        if( usiPG[1] == 1 )
                        {
                            DBGC.switchBit.b7 = true;        //G2
                            DBRC.switchBit.b7 = false;       //R2
                        }
                        else if ( usiPG[1] == 2 )
                        {
                            DBGC.switchBit.b7 = true;        //G2
                            DBGC.switchBit.b3 = true;        //G2 flash
                            DBRC.switchBit.b7 = false;       //R2
                        }
                    }
                    else if(i==2)
                    {
                        if( usiPG[1] == 1 )
                        {
                            DBGC.switchBit.b6 = true;        //G3
                            DBRC.switchBit.b6 = false;       //R3
                        }
                        else if ( usiPG[1] == 2 )
                        {
                            DBGC.switchBit.b6 = true;        //G3
                            DBGC.switchBit.b2 = true;        //G3 flash
                            DBRC.switchBit.b6 = false;       //R3
                        }
                    }
                    else if(i==3)
                    {
                        if( usiPG[1] == 1 )
                        {
                            DBGC.switchBit.b5 = true;        //G4
                            DBRC.switchBit.b5 = false;       //R4
                        }
                        else if ( usiPG[1] == 2 )
                        {
                            DBGC.switchBit.b5 = true;        //G4
                            DBGC.switchBit.b1 = true;        //G4 flash
                            DBRC.switchBit.b5 = false;       //R4
                        }
                    }
                }
                else if(smem.vGetWayMappingRedCount(i) == 2)
                {
                    if(i==0)
                    {
                        if( usiPG[2] == 1 )
                        {
                            DBGC.switchBit.b8 = true;        //G1
                            DBRC.switchBit.b8 = false;       //R1
                        }
                        else if ( usiPG[2] == 2 )
                        {
                            DBGC.switchBit.b8 = true;        //G1
                            DBGC.switchBit.b4 = true;        //G1 flash
                            DBRC.switchBit.b8 = false;       //R1
                        }
                    }
                    else if(i==1)
                    {
                        if( usiPG[2] == 1 )
                        {
                            DBGC.switchBit.b7 = true;        //G2
                            DBRC.switchBit.b7 = false;       //R2
                        }
                        else if ( usiPG[2] == 2 )
                        {
                            DBGC.switchBit.b7 = true;        //G2
                            DBGC.switchBit.b3 = true;        //G2 flash
                            DBRC.switchBit.b7 = false;       //R2
                        }
                    }
                    else if(i==2)
                    {
                        if( usiPG[2] == 1 )
                        {
                            DBGC.switchBit.b6 = true;        //G3
                            DBRC.switchBit.b6 = false;       //R3
                        }
                        else if ( usiPG[2] == 2 )
                        {
                            DBGC.switchBit.b6 = true;        //G3
                            DBGC.switchBit.b2 = true;        //G3 flash
                            DBRC.switchBit.b6 = false;       //R3
                        }
                    }
                    else if(i==3)
                    {
                        if( usiPG[2] == 1 )
                        {
                            DBGC.switchBit.b5 = true;        //G4
                            DBRC.switchBit.b5 = false;       //R4
                        }
                        else if ( usiPG[2] == 2 )
                        {
                            DBGC.switchBit.b5 = true;        //G4
                            DBGC.switchBit.b1 = true;        //G4 flash
                            DBRC.switchBit.b5 = false;       //R4
                        }
                    }

                }
                else if(smem.vGetWayMappingRedCount(i) == 3)
                {
                    if(i==0)
                    {
                        if( usiPG[3] == 1 )
                        {
                            DBGC.switchBit.b8 = true;        //G1
                            DBRC.switchBit.b8 = false;       //R1
                        }
                        else if ( usiPG[3] == 2 )
                        {
                            DBGC.switchBit.b8 = true;        //G1
                            DBGC.switchBit.b4 = true;        //G1 flash
                            DBRC.switchBit.b8 = false;       //R1
                        }
                    }
                    else if(i==1)
                    {
                        if( usiPG[3] == 1 )
                        {
                            DBGC.switchBit.b7 = true;        //G2
                            DBRC.switchBit.b7 = false;       //R2
                        }
                        else if ( usiPG[3] == 2 )
                        {
                            DBGC.switchBit.b7 = true;        //G2
                            DBGC.switchBit.b3 = true;        //G2 flash
                            DBRC.switchBit.b7 = false;       //R2
                        }
                    }
                    else if(i==2)
                    {
                        if( usiPG[3] == 1 )
                        {
                            DBGC.switchBit.b6 = true;        //G3
                            DBRC.switchBit.b6 = false;       //R3
                        }
                        else if ( usiPG[3] == 2 )
                        {
                            DBGC.switchBit.b6 = true;        //G3
                            DBGC.switchBit.b2 = true;        //G3 flash
                            DBRC.switchBit.b6 = false;       //R3
                        }
                    }
                    else if(i==3)
                    {
                        if( usiPG[3] == 1 )
                        {
                            DBGC.switchBit.b5 = true;        //G4
                            DBRC.switchBit.b5 = false;       //R4
                        }
                        else if ( usiPG[3] == 2 )
                        {
                            DBGC.switchBit.b5 = true;        //G4
                            DBGC.switchBit.b1 = true;        //G4 flash
                            DBRC.switchBit.b5 = false;       //R4
                        }
                    }
                }
                else {}    // do nothing
            }


            data[0] = 0xAB;                                                             // header
            data[1] = 0x0A;                                                             // length
            data[2] = 0x01;                                                             // control code
            data[3] = DBGC.DBit;
            data[4] = DBRC.DBit;
            //date 5~8 have edit before
            data[9] = 0;                                                                //cks
            for (int i = 0; i < 9; i++) data[9] ^= data[i];

            writeJob.WritePhysicalOut(data, 10, DEVICETAINANPEOPLELIGHT);

            //OT20131219
            // _MsgOK = oDataToMessageOK.vPackageINFOToGreenmanCount(_exec_plan._ptr_subplaninfo[_exec_phase_current_subphase],0 , m_greenremainder[0],m_greenremainder[1],m_greenremainder[2],m_greenremainder[3]);
            // _MsgOK.InnerOrOutWard = cOutWard;

        }

    }
    catch(...) {}
}

//----------------------------------------------------------
unsigned short int CSTC::vGetStepTimeUntilNextGreen(void)
{
    try
    {

        //Should be mutex

        unsigned short int time_difference=0;
        unsigned short int usiCurrentStep = _exec_phase_current_subphase_step;
        unsigned short int usiCurrentSubphase = _exec_phase_current_subphase;
//  if(_current_strategy==STRATEGY_MANUAL||_current_strategy==STRATEGY_FLASH||_current_strategy==STRATEGY_ALLRED || _current_strategy==STRATEGY_ALLDYNAMIC){
        if(_current_strategy == STRATEGY_TOD)
        {
            timer_gettime(_timer_plan,&_itimer_plan);
            time_difference = (_itimer_plan.it_value.tv_sec) + 1;

            if(usiCurrentStep < 4 )
            {
                time_difference += _exec_plan._ptr_subplaninfo[usiCurrentSubphase]._allred;
            }
            if(usiCurrentStep < 3 )
            {
                time_difference += _exec_plan._ptr_subplaninfo[usiCurrentSubphase]._yellow;
            }
            if(usiCurrentStep < 2 )
            {
                time_difference += _exec_plan._ptr_subplaninfo[usiCurrentSubphase]._pedred;
            }
            if(usiCurrentStep < 1 )
            {
                time_difference += _exec_plan._ptr_subplaninfo[usiCurrentSubphase]._pedgreen_flash;
            }

        }

        return time_difference;

    }
    catch(...) {}
}
//----------------------------------------------------------
unsigned short int CSTC::vGetStepTimeUntilNextCycle(void)
{
    try
    {

        //Should be mutex

        unsigned short int time_difference=0;
        unsigned short int usiCurrentStep = _exec_phase_current_subphase_step;
        unsigned short int usiCurrentSubphase = _exec_phase_current_subphase;
//  if(_current_strategy==STRATEGY_MANUAL||_current_strategy==STRATEGY_FLASH||_current_strategy==STRATEGY_ALLRED || _current_strategy==STRATEGY_ALLDYNAMIC){
        if(_current_strategy==STRATEGY_TOD)
        {
            timer_gettime(_timer_plan,&_itimer_plan);
            time_difference = (_itimer_plan.it_value.tv_sec) + 1;

            if(usiCurrentStep < 4 )
            {
                time_difference += _exec_plan._ptr_subplaninfo[usiCurrentSubphase]._allred;
            }
            if(usiCurrentStep < 3 )
            {
                time_difference += _exec_plan._ptr_subplaninfo[usiCurrentSubphase]._yellow;
            }
            if(usiCurrentStep < 2 )
            {
                time_difference += _exec_plan._ptr_subplaninfo[usiCurrentSubphase]._pedred;
            }
            if(usiCurrentStep < 1 )
            {
                time_difference += _exec_plan._ptr_subplaninfo[usiCurrentSubphase]._pedgreen_flash;
            }

            for(int i = _exec_phase_current_subphase+1 ; i < _exec_phase._subphase_count; i++)
            {
                time_difference += _exec_plan._ptr_subplaninfo[i].compensated_green(_exec_plan._shorten_cycle);
                time_difference += _exec_plan._ptr_subplaninfo[i]._allred;
                time_difference += _exec_plan._ptr_subplaninfo[i]._yellow;
                time_difference += _exec_plan._ptr_subplaninfo[i]._pedred;
                time_difference += _exec_plan._ptr_subplaninfo[i]._pedgreen_flash;
            }
            printf("[OTMsg] _exec_phase_current_subphase:%d, _exec_phase._subphase_count:%d\n", _exec_phase_current_subphase, _exec_phase._subphase_count);

        }

        return time_difference;

    }
    catch(...) {}
}
//----------------------------------------------------------
bool CSTC::vCompareNextSubphase(unsigned short int usiGuestSubphase)
{
    try
    {

        bool bCompareSubphaseStatus = false;
        bool bCompareSubphaseStepStatus = false;

        printf("[OTMsg] wish next subphase is %d, current subphase is %d, _subphase_count:%d\n", usiGuestSubphase, _exec_phase_current_subphase+1, _exec_phase._subphase_count);

        if(_exec_phase._subphase_count == (_exec_phase_current_subphase+1))           // in last phase
        {
            if(usiGuestSubphase == 1)                                                   // next phase is first
            {
                bCompareSubphaseStatus = true;
            }
            else
            {
                bCompareSubphaseStatus = false;
            }
        }
        else
        {
            if((usiGuestSubphase - (_exec_phase_current_subphase+1) ) == 1)
            {
                bCompareSubphaseStatus = true;
            }
            else
            {
                bCompareSubphaseStatus = false;
            }
        }

        if(_exec_phase_current_subphase_step == 0)
            bCompareSubphaseStepStatus = true;

        if(bCompareSubphaseStatus == true && bCompareSubphaseStepStatus == true)
        {
            return true;
        }
        else
        {
            return false;
        }

    }
    catch(...) {}
}
//----------------------------------------------------------
bool CSTC::vReportCCJPhaseStep_5F7C(void)
{
    try
    {
        unsigned char data[5];
        MESSAGEOK _MsgOK;

        data[0] = 0x5F;
        data[1] = 0x7C;
        data[2] = smem.vGetUCData(TC92_ucControlStrategy);
        data[3] = _exec_phase_current_subphase + 1;
        data[4] = _exec_phase_current_subphase_step + 1;

        _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, 5, false);
        _MsgOK.InnerOrOutWard = cOutWard;
        writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICECCJDYNCTL);

        printf("[OTMsg~~~~~~~~~~~~~~~~~~~] send 5F7C!!!\n");

        return true;

    }
    catch(...) {}
}

//----------------------------------------------------------
bool CSTC::vSendControlStrategyToCCJ(void)
{
    try
    {
        unsigned char data[5];

        data[0] = 0x5F;
        data[1] = 0x91;
        data[2] = smem.vGetUCData(TC92_ucControlStrategy);
        data[3] = smem.vGetINTData(TC92_iEffectTime);


        MESSAGEOK _MsgOK;
        _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, 4, false);
        _MsgOK.InnerOrOutWard = cOutWard;
        writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICECCJDYNCTL);


        printf("[OTMsg ControlStrategy change, %d\n", data[2]);

        return true;

    }
    catch(...) {}
}
//----------------------------------------------------------
//**********************************************************
//              Lock to Set Next Step
//**********************************************************
//----------------------------------------------------------
bool CSTC::vSetNextStepNotUseSignal(void)
{
    try
    {
        printf( "vSetNextStepNotUseSignal.\n" );
        /******** lock mutex ********/
        pthread_mutex_lock(&CSTC::_stc_mutex);
        if(_current_strategy==STRATEGY_MANUAL || _current_strategy==STRATEGY_ALLDYNAMIC || smem.vGetBOOLData(NextStepForceEnable) == true)
        {
            smem.vSetBOOLData(NextStepForceEnable, false);
            ReSetStep(true);
            ReSetExtendTimer();
            SetLightAfterExtendTimerReSet();

            if(_current_strategy==STRATEGY_ALLDYNAMIC)
            {
                stc.vReportGoToNextPhaseStep_5F0C();
            }
        }
        /******** unlock mutex ********/
        pthread_mutex_unlock(&CSTC::_stc_mutex);
//OT Debug Signal 951116
        smem.vSetBOOLData(TC_SIGNAL_NEXT_STEP_OK, true);

        return true;

    }
    catch (...) {}
}
//OTADD
//OT970214NEWLCX405
//----------------------------------------------------------
bool CSTC::vGetLCX405ControlPower(void)        // 1. check TOD, 2. change to ALLRED,  3. change to TOD
{
    try
    {

        unsigned char ucSendTMP[22];
        ControlStrategy CStmp;

        if(bCSTCInitialOK == true)
        {

            CStmp = _current_strategy;

            if(STRATEGY_TOD == _current_strategy || STRATEGY_ALLRED == _current_strategy || STRATEGY_FLASH == _current_strategy || STRATEGY_MANUAL == _current_strategy)
            {
                vSend0x16ToUnlockLCX405();

                printf("Lock_to_Set_Control_Strategy by vGetLCX405ControlPower!!!\n");
                printf("Lock_to_Set_Control_Strategy by vGetLCX405ControlPower!!!\n");
                printf("Lock_to_Set_Control_Strategy by vGetLCX405ControlPower!!!\n");

                stc.Lock_to_Set_Control_Strategy(STRATEGY_ALLRED);
                AllRed5Seconds();

                /*
                pthread_mutex_lock(&CSTC::_stc_mutex);
                _old_strategy=_current_strategy;
                _current_strategy=STRATEGY_ALLRED;
                pthread_mutex_unlock(&CSTC::_stc_mutex);

                ReSetStep(false);
                ReSetExtendTimer();
                SetLightAfterExtendTimerReSet();
                */

//      usleep(1000000);
//      sleep(3);

//OT980420, let redcount display open.
//      smem.vWriteMsgToDOM("From AutoRun To TC in CSTC");
//      smem.vSetUCData(TC_Redcount_Display_Enable, 1);

                /*
                        pthread_mutex_lock(&CSTC::_stc_mutex);
                        _old_strategy=_current_strategy;
                        _current_strategy=CStmp;
                        pthread_mutex_unlock(&CSTC::_stc_mutex);
                        ReSetStep(false);
                        ReSetExtendTimer();
                        SetLightAfterExtendTimerReSet();
                */

//      SendRequestToKeypad();

                /*
                        pthread_mutex_unlock(&CSTC::_stc_mutex);
                        AllRed5Seconds();
                        pthread_mutex_lock(&CSTC::_stc_mutex);
                        _current_strategy = STRATEGY_TOD;
                        ReSetStep(false);
                        ReSetExtendTimer();
                        SetLightAfterExtendTimerReSet();
                        pthread_mutex_unlock(&CSTC::_stc_mutex);
                */

            }
        }

        return 0;
    }
    catch (...) {}
}
//OT970214NEWLCX405
//----------------------------------------------------------
bool CSTC::vSend0x16ToUnlockLCX405(void)
{
    try
    {

        unsigned char ucSendTMP[22];

        //OT990401 BUG FIX
        if(smem.vGetStopSend0x22() == true)
        {
            smem.vSetStopSend0x22(false);
        }

        ucSendTMP[0] = 0xAA;
        ucSendTMP[1] = 0xBB;
        ucSendTMP[2] = 0x16;
//...
        ucSendTMP[17] = 0x00;
        ucSendTMP[18] = 0xAA;
        ucSendTMP[19] = 0xCC;
        ucSendTMP[20] = 0x00;
        for (int i=0; i<20; i++)
        {
            ucSendTMP[20]^=ucSendTMP[i];
        }
        writeJob.WritePhysicalOut(ucSendTMP, 21, DEVICETRAFFICLIGHT);               //0x16

        return 0;
    }
    catch (...) {}
}
//----------------------------------------------------------
bool CSTC::vCheckPhaseForTFDActuateExtendTime_5FCF(void)
{
    try
    {

        unsigned char data[5];
        unsigned char ucActuatePhaseExtend;
        MESSAGEOK _MsgOK;
        static bool bCCJTFDExtend5FCFSent = false;

        ucActuatePhaseExtend = smem.vGetActuatePhaseExtend();

        if(bCCJTFDExtend5FCFSent == true)
        {
            bCCJTFDExtend5FCFSent = false;

            data[0] = 0x5F;
            data[1] = 0xCF;
            data[2] = _exec_phase_current_subphase+1;
            data[3] = 0x00;

            _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, 4, true);
            _MsgOK.InnerOrOutWard = cOutWard;
            writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICECCJDYNCTL);
        }

        if( _exec_phase_current_subphase +1 == ucActuatePhaseExtend
                && _exec_phase_current_subphase_step == 0 )
        {

            data[0] = 0x5F;
            data[1] = 0xCF;
            data[2] = _exec_phase_current_subphase+1;
            data[3] = 0x01;

            _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, 4, true);
            _MsgOK.InnerOrOutWard = cOutWard;
            writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICECCJDYNCTL);

            bCCJTFDExtend5FCFSent = true;
        }

        return true;

        return 0;
    }
    catch (...) {}
}
//----------------------------------------------------------
bool CSTC::vActuateExtendTODTime(unsigned char ucExtenPhase, unsigned char ucExtenSec)
{
    try
    {
        bool bActuateStatus;
        unsigned char ucActuateExtendPhase;
        unsigned short int usiActuatePlanID;
        bActuateStatus = smem.vGetBOOLData(TC_CCTActuate_TOD_Running);  //check if now running
        ucActuateExtendPhase = smem.vGetActuatePhaseExtend();  //set by screen.
        usiActuatePlanID = smem.vGetUSIData(TC_CCT_In_LongTanu_ActuateType_Switch);

        printf("Get Actuate Extend From TFD-CCJ, ucExtenPhase:%d, ucExtenSec:%d\n", ucExtenPhase, ucExtenSec);
        printf("bActuateStatus %d == true\n",bActuateStatus);
        printf("ucActuateExtendPhase %d == %d _exec_phase_current_subphase+1\n",ucActuateExtendPhase,_exec_phase_current_subphase+1);
        printf("_exec_phase_current_subphase_step %d = 0\n",_exec_phase_current_subphase_step);
        printf("ucExtenPhase %d == %d ucActuateExtendPhase\n",ucExtenPhase,ucActuateExtendPhase);
        printf("_exec_plan._planid %d == %d usiActuatePlanID",_exec_plan._planid,usiActuatePlanID);

        /*
        if(bActuateStatus == true && ucActuateExtendPhase == _exec_phase_current_subphase+1 &&
           _exec_phase_current_subphase_step == 0 && ucExtenPhase == ucActuateExtendPhase &&
           _exec_plan._planid == usiActuatePlanID) {
             printf("ExtendProcess\n\n\n");
            timer_gettime(_timer_plan,&_itimer_plan);
            _itimer_plan.it_value.tv_sec += ucExtenSec;
            timer_settime(_timer_plan, 0, &_itimer_plan, NULL);
        }
        */

        if(bActuateStatus == true && ucActuateExtendPhase == _exec_phase_current_subphase+1 &&
                _exec_phase_current_subphase_step == 0 && ucExtenPhase == ucActuateExtendPhase &&
                _exec_plan._planid != usiActuatePlanID)
        {
            printf("ExtendProcess\n\n\n");
            timer_gettime(_timer_plan,&_itimer_plan);
            _itimer_plan.it_value.tv_sec += ucExtenSec;

            for(int ii = 0; ii < 4; ii++)
            {
                timer_settime(_timer_plan, 0, &_itimer_plan, NULL);

                _itimer_plan_WDT = _itimer_plan; /*OT_PLAN_WDT*/
                _itimer_plan_WDT.it_value.tv_sec += 2;
                if(timer_settime( _timer_plan_WDT, 0, &_itimer_plan_WDT, NULL ))
                {
                    printf("PlanWDT Settime Error!.\n");
                    exit( 1 );
                }
                timer_settime( _timer_redcount, 0, & _itimer_redcount, NULL );
            }

        }

        return 0;
    }
    catch (...) {}
}
//----------------------------------------------------------
bool CSTC::vExtendGreenTime(unsigned char Extendgreen,bool Extendstep)  //if phase1 Extendgreen=Extendgreen
{
    //if phase2 Extendgreen=mingreen
    try
    {
        unsigned short int usiCSTC_RunningPhaseCurrentPhase = stc.vGetUSIData(CSTC_exec_plan_phase_order);  //Should +1
        unsigned short int usiCSTC_RunningPhaseCurrentSubphase = stc.vGetUSIData(CSTC_exec_phase_current_subphase);  //Should +1
        unsigned short int usiCSTC_RunningPhaseCurrentSubphaseStep = stc.vGetUSIData(CSTC_exec_phase_current_subphase_step);
        //printf("\n\n\n\n\n%d\n%d\n\n\n\n",Extendgreen,Extendstep);
        unsigned short int usiStepSec = stc.vGetStepTime();
        if(smem.vGetVDPhaseTriggerSwitch(_exec_phase_current_subphase) == 0)
        {
            if(Extendgreen >0)
            {
                //       printf("\n\n\n\n\nExtendProcess\n\n\n");
                timer_gettime(_timer_plan,&_itimer_plan);
                _itimer_plan.it_value.tv_sec = Extendgreen;
            }
            for(int ii = 0; ii < 4; ii++)
            {
                timer_settime(_timer_plan, 0, &_itimer_plan, NULL);

                _itimer_plan_WDT = _itimer_plan; //OT_PLAN_WDT
                _itimer_plan_WDT.it_value.tv_sec += 2;
                if(timer_settime( _timer_plan_WDT, 0, &_itimer_plan_WDT, NULL ))
                {
                    printf("PlanWDT Settime Error!.\n");
                    exit( 1 );
                }
                timer_settime( _timer_redcount, 0, & _itimer_redcount, NULL );
            }

        }
        else
        {
        }

        //printf("\n\n\n\n\n%d\n%d\n\n",Extendgreen,Extendgreen);
        if(smem.vGetVDPhaseTriggerSwitch(_exec_phase_current_subphase) > 0)
        {
            if(Extendgreen > 0)
            {
                //       printf("\n\n\n\n\nMingreen!!!\nMingreen!!!\n\n\n\n");
                timer_gettime(_timer_plan,&_itimer_plan);
                _itimer_plan.it_value.tv_sec = Extendgreen;
            }


            for(int ii = 0; ii < 4; ii++)
            {
                timer_settime(_timer_plan, 0, &_itimer_plan, NULL);

                _itimer_plan_WDT = _itimer_plan; /*OT_PLAN_WDT*/
                _itimer_plan_WDT.it_value.tv_sec += 2;
                if(timer_settime( _timer_plan_WDT, 0, &_itimer_plan_WDT, NULL ))
                {
                    printf("PlanWDT Settime Error!.\n");
                    exit( 1 );
                }
                timer_settime( _timer_redcount, 0, & _itimer_redcount, NULL );
            }

        }
        else
        {
        }

        return 0;
    }
    catch(...) {}
}
//----------------------------------------------------------
bool CSTC::vActuateChangeTODTimeForCCJTOMNanLu(unsigned char ucExtenPhase, unsigned char ucExtenSec)
{
    try
    {
        unsigned char ucActuateExtendPhase;
        unsigned short int usiActuatePlanID;

//OT990329  ucActuateExtendPhase = smem.vGetActuatePhaseExtend();
        usiActuatePlanID = smem.vGetUSIData(TC_CCT_In_LongTanu_ActuateType_Switch);

        printf("Get Actuate Extend From VD-CCJ, ucExtenPhase:%d, ucExtenSec:%d\n", ucExtenPhase, ucExtenSec);
        printf("ucActuateExtendPhase %d == %d _exec_phase_current_subphase+1\n",ucActuateExtendPhase,_exec_phase_current_subphase+1);
        printf("_exec_phase_current_subphase_step %d = 0\n",_exec_phase_current_subphase_step);
        printf("ucExtenPhase %d == %d ucActuateExtendPhase\n",ucExtenPhase,ucActuateExtendPhase);
        printf("_exec_plan._planid %d == %d usiActuatePlanID",_exec_plan._planid,usiActuatePlanID);

//OT990329  if(ucActuateExtendPhase == _exec_phase_current_subphase+1 &&
//OT990329     _exec_phase_current_subphase_step == 0 && ucExtenPhase == ucActuateExtendPhase ) {
        if(smem.vGetVDPhaseTriggerSwitch(_exec_phase_current_subphase) > 0)
        {
            if(ucExtenSec >0)
            {
                printf("ExtendProcess\n\n\n");
                timer_gettime(_timer_plan,&_itimer_plan);
                _itimer_plan.it_value.tv_sec += ucExtenSec;

                for(int ii = 0; ii < 4; ii++)
                {
                    timer_settime(_timer_plan, 0, &_itimer_plan, NULL);

                    _itimer_plan_WDT = _itimer_plan; /*OT_PLAN_WDT*/
                    _itimer_plan_WDT.it_value.tv_sec += 2;
                    if(timer_settime( _timer_plan_WDT, 0, &_itimer_plan_WDT, NULL ))
                    {
                        printf("PlanWDT Settime Error!.\n");
                        exit( 1 );
                    }
                    timer_settime( _timer_redcount, 0, & _itimer_redcount, NULL );
                }
            }
            else
            {

            }
        }

        return 0;
    }
    catch (...) {}
}
//----------------------------------------------------------
bool CSTC::vChangeTODCurrentStepSec(unsigned char ucnSec, unsigned char ucStepID)
{
    try
    {
        printf("\n00\n");
        if(_current_strategy == STRATEGY_TOD && ucStepID == _exec_phase_current_subphase_step )
        {

//OT970703
//OT980406
            printf("11\n");
            if(_exec_phase._phase_order == 0xB0 || _exec_phase._phase_order == 0xB1 || _exec_phase._phase_order == 0x80)
            {
                printf("22\n\n");
                timer_gettime(_timer_plan,&_itimer_plan);
//    _itimer_plan.it_value.tv_sec = ucSec;
                _itimer_plan.it_value.tv_sec = 0;
                _itimer_plan.it_value.tv_nsec = ucnSec * 100000000;

                for(int ii = 0; ii < 4; ii++)
                {
                    timer_settime(_timer_plan, 0, &_itimer_plan, NULL);

                    _itimer_plan_WDT = _itimer_plan; /*OT_PLAN_WDT*/
                    _itimer_plan_WDT.it_value.tv_sec += 2;
                    if(timer_settime( _timer_plan_WDT, 0, &_itimer_plan_WDT, NULL ))
                    {
                        printf("PlanWDT Settime Error!.\n");
                        exit( 1 );
                    }
                    timer_settime( _timer_redcount, 0, & _itimer_redcount, NULL );
                }

                SegmentTypeUpdate = true;
            }
        }
        else
        {
            printf("\nerror\n_current_strategy == %d\n",_current_strategy);
        }

        return 0;
    }
    catch (...) {}
}
//----------------------------------------------------------
bool CSTC::vSendUpdateSegmentPlanInfoToCCJ_5F9D(void)
{
    try
    {

        unsigned char data[5];
        MESSAGEOK _MsgOK;

        data[0] = 0x5F;
        data[1] = 0x9D;

        _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, 2, true);
        _MsgOK.InnerOrOutWard = cOutWard;
        writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICECCJDYNCTL);

        return true;

        return 0;
    }
    catch (...) {}
}
//----------------------------------------------------------
void CSTC::ReportCurrentStepStatus5F03toCCJ(void)  //5F03
{
    try
    {

        DATA_Bit _ControlStrategy;
        int iLightOutNo;
        int iDataPtr = 9;
        unsigned char data[255];
        //Should be mutex

        unsigned short int time_difference=0;

        if(_timer_plan != NULL)
        {

            if(_current_strategy==STRATEGY_MANUAL||_current_strategy==STRATEGY_FLASH||_current_strategy==STRATEGY_ALLRED || _current_strategy==STRATEGY_ALLDYNAMIC)
            {
                timespec strategy_current_time= {0,0};
                if(clock_gettime(CLOCK_REALTIME, &strategy_current_time)<0) perror("Can not get current time");
                time_difference = (strategy_current_time.tv_sec - strategy_start_time.tv_sec);
            }
            else
            {
                if(_exec_plan._planid!=FLASH_PLANID && _exec_plan._planid!=ALLRED_PLANID)
                {
                    timer_gettime(_timer_plan,&_itimer_plan);
                    time_difference = (_itimer_plan.it_value.tv_sec) + 1;
                    //OT debug
                    printf("\ntime_difference: %d\n", time_difference);

                }
            }

            //  unsigned short int data_length = ( 9 + _exec_phase._signal_count + 4 );
            unsigned short int data_length = ( 9 + _exec_phase._signal_count);
//mallocFuck  unsigned char *data = (unsigned char *)malloc( data_length*sizeof(unsigned char) );

            data[0] = 0x5F;
            data[1] = 0x03;
            data[2] = _exec_phase._phase_order;

            /*Special For CCJ*/
            _ControlStrategy.DBit = smem.vGetUCData(TC92_ucControlStrategy);
            if(_ControlStrategy.switchBit.b4 == true)
            {
                data[3] = 1;
            }
            else
            {
                data[3] = 0;
            }
            data[4] = smem.vGetINTData(TC92_PlanOneTime5F18_PlanID) & 0xFF;

            data[5] = (_exec_phase_current_subphase+1);
            printf("(_exec_phase_current_subphase+1)=%d.\n", (_exec_phase_current_subphase+1));
            if(_exec_plan._planid==ALLRED_PLANID)
                data[6] = 0x9F;
            else if(_exec_plan._planid==FLASH_PLANID)
                data[6] = 0xDF;
            else if( smem.vGetBOOLData(TC92_SubPhaseOfPhasePlanIncorrent) == true )
                data[6] = 0xFF;
            else if( smem.vGetBOOLData(TC_SignalConflictError) == true )
                data[6] = 0xCF;
            else
                data[6] = (_exec_phase_current_subphase_step+1);
            printf("(_exec_phase_current_subphase_step+1)=%d.\n", (_exec_phase_current_subphase_step+1));

            //OT debug
            printf("time_difference2: %d\n", time_difference);

            data[7] = (time_difference&0xFF00)>>8;
            data[8] = (time_difference&0xFF);

            printf("OTDEBUG _exec_phase._signal_count:%d\n", _exec_phase._signal_count);


            /*92 no this */
//  data[data_length-4] = (_exec_segment._segment_type)&0xFF;
//  data[data_length-3] = (_exec_segment_current_seg_no+1)&0xFF;
//  data[data_length-2] = (_exec_phase.calculated_total_step_count())&0xFF;
//  data[data_length-1] = (_exec_phase.step_no_of_all(_exec_phase_current_subphase, _exec_phase_current_subphase_step)+1)&0xFF;



            /*+++++++++++++++++*/

            //OTCombo0713
            MESSAGEOK _MsgOK;
            _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, iDataPtr, true);
//  _MsgOK.InnerOrOutWard = cOutWard;
//  writeJob.WriteWorkByMESSAGEOUT(_MsgOK);
            writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICECCJDYNCTL);

//--------------------------------------------------then, report plan.

            iDataPtr = 6;
            unsigned short int usiGreen;
            unsigned short int usiRunningPlanID = _exec_plan._planid;
//    stc.Lock_to_Load_Plan_for_Center(usiRunningPlanID);
            data[0] = 0x5F;
            data[1] = 0xC8;
            data[2] = usiRunningPlanID;
            data[3] = _exec_plan._dir;
            data[4] = _exec_plan._phase_order;
            data[5] = _exec_plan._subphase_count;

            for(int i = 0; i < data[5]; i++)                                              //data[5] = subphasecount
            {
                usiGreen = 0;

                //OT980406
                if(_exec_plan._phase_order == 0x80 || _exec_plan._phase_order == 0x80)
                {
                }
                else
                {
                    usiGreen += _exec_plan._ptr_subplaninfo[i]._green;
                }
                /*
                if(_exec_plan._phase_order != 0xB0) {                             //Flash PhaseOrder In HsinChu
                usiGreen += _exec_plan._ptr_subplaninfo[i]._green;
                } else { }
                */
                usiGreen += _exec_plan._ptr_subplaninfo[i]._pedgreen_flash;
                usiGreen += _exec_plan._ptr_subplaninfo[i]._pedred;

                data[iDataPtr] = HI(usiGreen);
                iDataPtr++;
                data[iDataPtr] = LO(usiGreen);
                iDataPtr++;
            }

//OT090406
            if(_exec_plan._phase_order == 0xB0 || _exec_plan._phase_order == 0x80)                                 //Flash PhaseOrder In HsinChu
            {
                data[iDataPtr] = 0;
                data[iDataPtr+1] = 0;
                iDataPtr+=2;
            }
            else
            {
                data[iDataPtr] = HI(_exec_plan._cycle_time);
                iDataPtr++;
                data[iDataPtr] = LO(_exec_plan._cycle_time);
                iDataPtr++;
            }
            /*
                if(_exec_plan._phase_order != 0xB0) {                               //Flash PhaseOrder In HsinChu
                  data[iDataPtr] = HI(_exec_plan._cycle_time);
                  iDataPtr++;
                  data[iDataPtr] = LO(_exec_plan._cycle_time);
                  iDataPtr++;
                } else {
                  data[iDataPtr] = 0; data[iDataPtr+1] = 0; iDataPtr+=2;
                }
            */
            data[iDataPtr] = HI(_exec_plan._offset);
            iDataPtr++;
            data[iDataPtr] = LO(_exec_plan._offset);
            iDataPtr++;

            _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, iDataPtr, true);
            _MsgOK.InnerOrOutWard = cOutWard;
//  writeJob.WriteWorkByMESSAGEOUT(_MsgOK);
            writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICECCJDYNCTL);

        }
    }
    catch (...) {}
}
//----------------------------------------------------------
void CSTC::vCCJTOMSendActuatePhaseInfo_5FA0(void)               //半觸動邏輯
{
    try
    {
        unsigned char data[36];
        unsigned short int usiTmp;
        unsigned short int usiLCN[8];
        int iMaxVDNum = 0;
        //int iPtr = 11;
        int iPtr = 15;

//  usiTmp = smem.vGetUSIData(TC_ActuateVDID);
//  data[3] = (usiTmp>>8) & 0x00ff;
//  data[4] = usiTmp & 0x00ff;

        data[0] = 0x5F;
        data[1] = 0xA0;
        data[2] = _exec_phase_current_subphase + 1;

        usiTmp = _exec_plan._ptr_subplaninfo[_exec_phase_current_subphase]._green +
                 _exec_plan._ptr_subplaninfo[_exec_phase_current_subphase]._pedgreen_flash +
                 _exec_plan._ptr_subplaninfo[_exec_phase_current_subphase]._pedred;

        data[3] = (usiTmp>>8) & 0x00ff;          // right shift 8bit
        data[4] = usiTmp & 0x00ff;


        usiTmp = _exec_plan._ptr_subplaninfo[_exec_phase_current_subphase].compensated_green(_exec_plan._shorten_cycle) +
                 _exec_plan._ptr_subplaninfo[_exec_phase_current_subphase].compensated_pedgreen_flash(_exec_plan._shorten_cycle) +
                 _exec_plan._ptr_subplaninfo[_exec_phase_current_subphase].compensated_pedred(_exec_plan._shorten_cycle);

        data[5] = (usiTmp>>8) & 0x00ff;
        data[6] = usiTmp & 0x00ff;

        usiTmp = _exec_plan._ptr_subplaninfo[_exec_phase_current_subphase]._min_green;
        data[7] = (usiTmp>>8) & 0x00ff;
        data[8] = usiTmp & 0x00ff;

        usiTmp = _exec_plan._ptr_subplaninfo[_exec_phase_current_subphase]._max_green;
        data[9] = (usiTmp>>8) & 0x00ff;
        data[10] = usiTmp & 0x00ff;

        //------------Arwen ++ 1001125
        usiTmp = _exec_plan._ptr_subplaninfo[_exec_phase_current_subphase].compensated_pedgreen_flash(_exec_plan._shorten_cycle);//PG
        data[11] = usiTmp;//PG person green
        usiTmp = _exec_plan._ptr_subplaninfo[_exec_phase_current_subphase].compensated_pedred(_exec_plan._shorten_cycle);//PR
        data[12] = usiTmp;//PR person red


        unsigned short int iRunSegActMode = smem.getRunSegmentActMode();//act plan OK
        unsigned short int iSegActType = smem.getSegmentActType(iRunSegActMode);//act type OK
        printf("iRunSegActMode:%d iSegActType:%d\n",iRunSegActMode,iSegActType);
        if((iRunSegActMode !=12 && iRunSegActMode<10 )&& (iSegActType==1 || iSegActType==2 || iSegActType == 6) ) //20150727　原本１，２才可以半觸動，增加第6種模式也可以半觸動
        {
            printf("SegmentActExtendSec:%d _exec_phase_current_subphase:%d SegmentZoneTimes:%d\n",smem.getSegmentActExtendSec(iRunSegActMode,_exec_phase_current_subphase),_exec_phase_current_subphase,smem.getSegmentZoneTimes(iRunSegActMode));
            data[13] = smem.getSegmentActExtendSec(iRunSegActMode,_exec_phase_current_subphase);
            data[14] = smem.getSegmentZoneTimes(iRunSegActMode);
        }
        else
        {
            data[13] = 0x00;
            data[14] = 0x00;
        }
        //-----------end


        for(int i = 0; i < 8; i++)
        {
            usiTmp = smem.vGetVDPhaseMapTable(_exec_phase_current_subphase, i);
            if(usiTmp > 0)
            {
                usiLCN[iMaxVDNum] = usiTmp;
                iMaxVDNum++;
            }
        }

        data[iPtr] = iMaxVDNum;
        iPtr++;
        for(int i = 0; i < iMaxVDNum; i++)
        {
            data[iPtr] = HI(usiLCN[i]);
            iPtr++;
            data[iPtr] = LO(usiLCN[i]);
            iPtr++;
        }



        MESSAGEOK _MsgOK;
        _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, iPtr, false);
        _MsgOK.InnerOrOutWard = cOutWard;
        writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICECCJDYNCTL);

        printf("[OTMsg] vSendActuatePhaseInfo\n");

    }
    catch(...) {}
}
//----------------------------------------------------------
void CSTC::vCCJTOMSendStartStopInfo(unsigned char ucStats)
{
    try
    {
        unsigned char data[18];
        unsigned short int usiTmp;

        printf("go to vCCJTOMSendStartStopInfo!!\n");

        if(_exec_plan._ptr_subplaninfo[_exec_phase_current_subphase]._min_green >= 5 &&
                _exec_plan._ptr_subplaninfo[_exec_phase_current_subphase]._max_green <= 255  )
        {

            data[0] = 0x5F;
            data[1] = 0xA1;
            data[2] = _exec_phase_current_subphase + 1;
            data[3] = ucStats;

            MESSAGEOK _MsgOK;
            _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, 4, false);
            _MsgOK.InnerOrOutWard = cOutWard;
            writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICECCJDYNCTL);
        }
        else
        {
            printf("for dyn control, minGreen or maxGreen over limit\n"); //dyn == 動態
        }

    }
    catch(...) {}
}
//----------------------------------------------------------
void CSTC::vCCJTOMSendPhaseInfo()          //抓時制計畫的所有資料
{
    try
    {
        unsigned char data[256];
        unsigned short int usiTmp;
        unsigned char ucPtr;
        unsigned short int cycleTotal;
        unsigned char ucCycle2Ptr;

        printf("[OTMsg] do vCCJTOMSendPhaseInfo 5FB0\n");
        printf("[OTMsg] do vCCJTOMSendPhaseInfo 5FB0\n");
        printf("[OTMsg] do vCCJTOMSendPhaseInfo 5FB0\n");

        data[0] = 0x5F;
        data[1] = 0xB0;
        //2,3 is total sec


        cycleTotal = plan[NEW_TOD_PLAN_ID].calculated_cycle_time();
        data[2] = plan[NEW_TOD_PLAN_ID]._planid;
        data[3] = (cycleTotal >> 8) & 0x00ff;
        data[4] = (cycleTotal) & 0x00ff;
        data[5] = _exec_phase._subphase_count;

        cycleTotal = 0;
        ucPtr = 8;

        for(int i = 0; i < data[5]; i++)
        {

            data[ucPtr] = i+1;  //phaseID
            ucPtr++;
            usiTmp = _exec_plan._ptr_subplaninfo[i].compensated_green(_exec_plan._shorten_cycle);  //G
            cycleTotal += usiTmp;
            data[ucPtr] = (usiTmp>>8) & 0x00ff;
            ucPtr++;
            data[ucPtr] = usiTmp & 0x00ff;
            ucPtr++;
            data[ucPtr] = _exec_plan._ptr_subplaninfo[i].compensated_pedgreen_flash(_exec_plan._shorten_cycle) & 0xFF;  //PG
            cycleTotal += data[ucPtr];
            ucPtr++;
            data[ucPtr] = _exec_plan._ptr_subplaninfo[i].compensated_pedred(_exec_plan._shorten_cycle) & 0xFF;
            cycleTotal += data[ucPtr];
            ucPtr++;
            data[ucPtr] = _exec_plan._ptr_subplaninfo[i].compensated_yellow(_exec_plan._shorten_cycle) & 0xFF;
            cycleTotal += data[ucPtr];
            ucPtr++;
            data[ucPtr] = _exec_plan._ptr_subplaninfo[i].compensated_allred(_exec_plan._shorten_cycle) & 0xFF;
            cycleTotal += data[ucPtr];
            ucPtr++;
        }

        data[6] = (cycleTotal>>8) & 0x00ff;
        data[7] = cycleTotal & 0x00ff;

        //OT20110607
        cycleTotal = 0;
        ucCycle2Ptr = ucPtr;
        ucPtr += 2;
        for(int i = 0; i < data[5]; i++)
        {
            data[ucPtr] = i+1;  //phaseID
            ucPtr++;
            usiTmp = _exec_plan._ptr_subplaninfo[i]._green;  //G
            cycleTotal += usiTmp;
            data[ucPtr] = (usiTmp>>8) & 0x00ff;
            ucPtr++;
            data[ucPtr] = usiTmp & 0x00ff;
            ucPtr++;
            data[ucPtr] = _exec_plan._ptr_subplaninfo[i]._pedgreen_flash & 0xFF;  //PG
            cycleTotal += data[ucPtr];
            ucPtr++;
            data[ucPtr] = _exec_plan._ptr_subplaninfo[i]._pedred & 0xFF;
            cycleTotal += data[ucPtr];
            ucPtr++;
            data[ucPtr] = _exec_plan._ptr_subplaninfo[i]._yellow & 0xFF;
            cycleTotal += data[ucPtr];
            ucPtr++;
            data[ucPtr] = _exec_plan._ptr_subplaninfo[i]._allred & 0xFF;
            cycleTotal += data[ucPtr];
            ucPtr++;
        }
        data[ucPtr] = smem.vGetUCData(CSTC_SegmentNoRunning);
        ucPtr++;

        data[ucCycle2Ptr] = (cycleTotal>>8) & 0x00ff;
        data[ucCycle2Ptr+1] = cycleTotal & 0x00ff;

//arwen++ min max Green
        for(int i = 0; i < data[5]; i++)
        {
            usiTmp = _exec_plan._ptr_subplaninfo[i]._green +
                     _exec_plan._ptr_subplaninfo[i]._pedgreen_flash +
                     _exec_plan._ptr_subplaninfo[i]._pedred;

            data[ucPtr] = (usiTmp>>8) & 0x00ff;
            ucPtr++;
            data[ucPtr] = usiTmp & 0x00ff;
            ucPtr++;

            usiTmp = _exec_plan._ptr_subplaninfo[i]._min_green;
            data[ucPtr] = (usiTmp>>8) & 0x00ff;
            ucPtr++;
            data[ucPtr] = usiTmp & 0x00ff;
            ucPtr++;

            usiTmp = _exec_plan._ptr_subplaninfo[i]._max_green;
            data[ucPtr] = (usiTmp>>8) & 0x00ff;
            ucPtr++;
            data[ucPtr] = usiTmp & 0x00ff;
            ucPtr++;
        }
//arwen++end


        MESSAGEOK _MsgOK;
        _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, ucPtr, false);
        _MsgOK.InnerOrOutWard = cOutWard;
        writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICECCJDYNCTL);
        if(smem.vGetUCData(TC_CCT_SendCCTPhaseCycleProtocalForCenter))
        {
            writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICECENTER92);
        }

    }
    catch(...) {}
}
//----------------------------------------------------------
void CSTC::vCCJTOMSendTODNextPlanID()
{
    printf("vCCJTOMSendTODNextPlanID()\n");
    static time_t now;
    static struct tm* currenttime;
    CSegmentInfo nextSegment;
    unsigned short int usiTODNextPlanID;
    unsigned short int usiCurrentCycleTime = _exec_plan.compensated_cycle_time(_exec_plan._shorten_cycle);
    printf("usiCurrentCycleTime: %d\n",usiCurrentCycleTime);
    now = time(NULL);
    now = now + usiCurrentCycleTime;
    currenttime = localtime(&now);


    printf("Next Chang Plan Date: %4d/%2d/%2d  Time: %2d:%2d:%2d\n"
           , currenttime->tm_year+1900,currenttime->tm_mon+1,currenttime->tm_mday
           , currenttime->tm_hour, currenttime->tm_min, currenttime->tm_sec);


    //***********************************
    //****   check if in holidayseg
    bool _exec_segment_is_holiday=false;
    for(int i=0; i<AMOUNT_HOLIDAY_SEG; i++)
    {
        if(    (    ( (currenttime->tm_year+1900) > holidayseg[i]._start_year )
                    || ( (currenttime->tm_year+1900)== holidayseg[i]._start_year && (currenttime->tm_mon+1) > holidayseg[i]._start_month )
                    || ( (currenttime->tm_year+1900)== holidayseg[i]._start_year && (currenttime->tm_mon+1)== holidayseg[i]._start_month && (currenttime->tm_mday)>= holidayseg[i]._start_day ) )
                && (    ( (currenttime->tm_year+1900) < holidayseg[i]._end_year )
                        || ( (currenttime->tm_year+1900)== holidayseg[i]._end_year   && (currenttime->tm_mon+1) < holidayseg[i]._end_month )
                        || ( (currenttime->tm_year+1900)== holidayseg[i]._end_year   && (currenttime->tm_mon+1)== holidayseg[i]._end_month   && (currenttime->tm_mday)<= holidayseg[i]._end_day ) ) )
        {
            _exec_segment_is_holiday=true;
            nextSegment = segment[holidayseg[i]._segment_type];
            printf("Today is in Holiday Segment:%d\n",i);

            break;
        }
    }

    //***********************************
    //****  check in which weekdayseg
    if(_exec_segment_is_holiday==false)
    {
        if(    ((((((currenttime->tm_mday-1)/7)+1)%2)==0 ) && (currenttime->tm_wday>=((currenttime->tm_mday-1)%7)))
                || ((((((currenttime->tm_mday-1)/7)+1)%2)> 0 ) && (currenttime->tm_wday< ((currenttime->tm_mday-1)%7))) )
        {

            printf("This week is an even week\n");

            int wday = (currenttime->tm_wday==0)?13:(currenttime->tm_wday+6);
            nextSegment = segment[weekdayseg[wday]._segment_type];
        }//end if(even week)

        else   //odd week
        {
            printf("This week is an odd week\n");
            int wday = (currenttime->tm_wday==0)?6:(currenttime->tm_wday-1);
            nextSegment = segment[weekdayseg[wday]._segment_type];
        }//end else(odd week)
    }//end else(holiday)


    for(int i=0; i<_exec_segment._segment_count; i++)
    {
        //iExecSeg = i;
        if(     ( i == (_exec_segment._segment_count-1) )  //last segment
                ||(  ( i <  (_exec_segment._segment_count-1) )  //not last segment
                     &&(    (    ( currenttime->tm_hour > _exec_segment._ptr_seg_exec_time[i]._hour )
                                 || ( currenttime->tm_hour ==_exec_segment._ptr_seg_exec_time[i]._hour    && currenttime->tm_min >=_exec_segment._ptr_seg_exec_time[i]._minute  ) )
                            && (    ( currenttime->tm_hour  <_exec_segment._ptr_seg_exec_time[i+1]._hour)
                                    || ( currenttime->tm_hour ==_exec_segment._ptr_seg_exec_time[i+1]._hour  && currenttime->tm_min  <_exec_segment._ptr_seg_exec_time[i+1]._minute) ) ) ) )
        {

        }
    }

    //***********************************
    //**** check and modify _exec_plan
    for(int i=0; i<nextSegment._segment_count; i++)
    {
        if(     ( i == (nextSegment._segment_count-1) )  //last segment
                ||(  ( i <  (nextSegment._segment_count-1) )  //not last segment
                     &&(    (    ( currenttime->tm_hour > nextSegment._ptr_seg_exec_time[i]._hour )
                                 || ( currenttime->tm_hour ==nextSegment._ptr_seg_exec_time[i]._hour    && currenttime->tm_min >=nextSegment._ptr_seg_exec_time[i]._minute  ) )
                            && (    ( currenttime->tm_hour  <nextSegment._ptr_seg_exec_time[i+1]._hour)
                                    || ( currenttime->tm_hour ==nextSegment._ptr_seg_exec_time[i+1]._hour  && currenttime->tm_min  <nextSegment._ptr_seg_exec_time[i+1]._minute) ) ) ) )
        {
            printf("i %d\n");
            usiTODNextPlanID = nextSegment._ptr_seg_exec_time[i]._planid;
            break;
        }
    }

    bool bDynSegSwitch = smem.vGetBOOLData(TCDynSegSwitch);
    if(bDynSegSwitch)
    {
        usiTODNextPlanID = iDynSeg_PlanID;
    }

    unsigned char data[256];
    unsigned char ucPtr;
    ucPtr = 0;
    data[0] = 0x5F;
    ucPtr++;
    data[1] = 0xB1;
    ucPtr++;
    data[2] = usiTODNextPlanID;
    ucPtr++;
    MESSAGEOK _MsgOK;

    _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, ucPtr, false);
    _MsgOK.InnerOrOutWard = cOutWard;
    writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICECCJDYNCTL);
    if(smem.vGetUCData(TC_CCT_SendCCTPhaseCycleProtocalForCenter))
    {
        writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICECENTER92);
    }
}
//----------------------------------------------------------

void CSTC::vTrainComing(void)       //火車連鎖
{
    try
    {
        unsigned char ucBanSubphase;
        unsigned char ucBanSubphaseLast;
        unsigned char ucBanSubphaseJumpLockSubphase;

        /*
          printf("go CSTC vTrainComing\n");
          printf("go CSTC vTrainComing\n");
          printf("go CSTC vTrainComing\n");
        */

        ucBanSubphase = smem.vGetUCData(TC_TrainComingBanSubphase);
        ucBanSubphaseJumpLockSubphase = smem.vGetUCData(TC_TrainComingForceJumpSubphase);

        if(ucBanSubphase > 0)
        {
            ucBanSubphaseLast = ucBanSubphase - 1;
        }
        else
        {
            ucBanSubphaseLast = _exec_phase._subphase_count - 1;
        }

        if( _current_strategy == STRATEGY_TOD )
        {

            //in last subphase
            if(_exec_phase_current_subphase == ucBanSubphaseJumpLockSubphase &&
                    //OT990617, FIX, add peopleRed
                    (_exec_phase_current_subphase_step == 0 || _exec_phase_current_subphase_step == 1 || _exec_phase_current_subphase_step == 2) )
            {

                ++usiTrainComingSec;                                                     //cal. lock phase sec

                printf("ExtendProcess, set sec 2\n\n\n");
                timer_gettime(_timer_plan,&_itimer_plan);

                if(_itimer_plan.it_value.tv_sec > 3)
                {
                    usiLockPhaseSec = _itimer_plan.it_value.tv_sec;
                }

                _itimer_plan.it_value.tv_sec = 3;

                for(int ii = 0; ii < 4; ii++)
                {
                    timer_settime(_timer_plan, 0, &_itimer_plan, NULL);

                    _itimer_plan_WDT = _itimer_plan; /*OT_PLAN_WDT*/
                    _itimer_plan_WDT.it_value.tv_sec = 5;
                    timer_settime( _timer_plan_WDT, 0, &_itimer_plan_WDT, NULL );
                    timer_settime( _timer_redcount, 0, & _itimer_redcount, NULL );
                }

                //close redcount
            }

            //in ban subphase
            if(_exec_phase_current_subphase == ucBanSubphase &&
                    //OT990617, FIX, only jump GREEN, no flashGreen
                    (_exec_phase_current_subphase_step == 0) )
            {
                timer_gettime(_timer_plan,&_itimer_plan);
                usiTrainComingSec += (_itimer_plan.it_value.tv_sec) + 1;

                smem.vSetBOOLData(NextStepForceEnable, true);
                stc.vSetNextStepNotUseSignal();
            }

            CalculateAndSendRedCount(0);
            bJumpSubEnable = true;

        }

    }
    catch(...) {}
}
//----------------------------------------------------------
void CSTC::vTrainLeaving(void)
{
    try
    {

        unsigned char ucBanSubphase;
        unsigned char ucBanSubphaseLast;

        /*
          printf("go CSTC vTrainLeaving\n");
          printf("go CSTC vTrainLeaving\n");
          printf("go CSTC vTrainLeaving\n");
        */

        ucBanSubphase = smem.vGetUCData(TC_TrainComingBanSubphase);

        if(ucBanSubphase > 0)
        {
            ucBanSubphaseLast = ucBanSubphase - 1;
        }
        else
        {
            ucBanSubphaseLast = _exec_phase._subphase_count - 1;
        }

        //in ban subphase last
        if(_exec_phase_current_subphase == ucBanSubphaseLast &&
                (_exec_phase_current_subphase_step == 0 || _exec_phase_current_subphase_step == 1) )
        {
            smem.vSetBOOLData(NextStepForceEnable, true);
            stc.vSetNextStepNotUseSignal();
        }

        bJumpSubEnable = false;

    }
    catch(...) {}
}
//----------------------------------------------------------
bool CSTC::vSetRevStep(unsigned char ucStep)
{
    try
    {

        /*------------------
        seq:
          1-> 2-> 3-> 4-> 11-> 12-> 13-> 14-> 1->
          OX  xX  XX  XO  XO   Xx   XX   OX  OX
        */

        if(smem.vGetUCData(TC_ReverseLane_Manual_Enable_Status) == 1 &&
                smem.vGetUCData(TC_ReverseLane_Control_Mode) == 1        )                 //manual
        {
            switch(ucStep)
            {
            case 0:
                _exec_reversetime_current_rev_step = ucStep;
                return true;
                break;

            case 1:      //O, X
                if(_exec_reversetime_current_rev_step == 14 || 1 == _exec_reversetime_current_rev_step)
                {
                    _exec_reversetime_current_rev_step = ucStep;
                    return true;
                }
                else
                {
                    _exec_reversetime_current_rev_step = 3;
                    return false;
                }
                break;

            case 2:  // xX
                if(_exec_reversetime_current_rev_step == 1 || 2 == _exec_reversetime_current_rev_step)      // OX
                {
                    _exec_reversetime_current_rev_step = ucStep;
                    return true;
                }
                else
                {
                    _exec_reversetime_current_rev_step = 3;
                    return false;
                }
                break;

            case 3:  // XX
                _exec_reversetime_current_rev_step = ucStep;
                return true;
                break;

            case 4:  // XO
                if(_exec_reversetime_current_rev_step == 3 || 4 == _exec_reversetime_current_rev_step)      //X, X
                {
                    _exec_reversetime_current_rev_step = ucStep;
                    return true;
                }
                else
                {
                    _exec_reversetime_current_rev_step = 3;
                    return false;
                }
                break;

            case 11:      //X0
                if(_exec_reversetime_current_rev_step == 4 || 11 == _exec_reversetime_current_rev_step)
                {
                    _exec_reversetime_current_rev_step = ucStep;
                    return true;
                }
                else
                {
                    _exec_reversetime_current_rev_step = 13;
                    return false;
                }
                break;

            case 12:  // Xx
                if(_exec_reversetime_current_rev_step == 11 || 12 == _exec_reversetime_current_rev_step)      // XO
                {
                    _exec_reversetime_current_rev_step = ucStep;
                    return true;
                }
                else
                {
                    _exec_reversetime_current_rev_step = 13;
                    return false;
                }
                break;

            case 13:  // XX
                _exec_reversetime_current_rev_step = 13;
                return true;
                break;

            case 14:  // OX
                if(_exec_reversetime_current_rev_step == 13 || 14 == _exec_reversetime_current_rev_step)      //X, X
                {
                    _exec_reversetime_current_rev_step = ucStep;
                    return true;
                }
                else
                {
                    _exec_reversetime_current_rev_step = 13;
                    return false;
                }
                break;

            default:
                _exec_reversetime_current_rev_step = 3;
                return false;
                break;
            }
        }

        return false;

    }
    catch(...) {}
}
//----------------------------------------------------------
unsigned char CSTC::vGetRevStep(void)
{
    try
    {
        unsigned char ucRet;

        ucRet = _exec_reversetime_current_rev_step;
        return ucRet;

    }
    catch(...) {}
}
//----------------------------------------------------------
bool CSTC::vLightRevSingal(void)
{
    try
    {
        unsigned char signal_bit_map[14];
        unsigned char ucSendTMP[32];
        unsigned char ucTmp[2];

        switch(_exec_reversetime_current_rev_step)
        {
        case(0):
            signal_bit_map[12] = 0x00;
            signal_bit_map[13] = 0x00;
            break;

        case(1):
            signal_bit_map[12] = 0xC3;
            signal_bit_map[13] = 0x00;
            break;
        case(2):
            signal_bit_map[12] = 0x83;
            signal_bit_map[13] = 0x00;
            break;
        case(3):
            signal_bit_map[12] = 0x33;
            signal_bit_map[13] = 0x00;
            break;
        case(4):
            signal_bit_map[12] = 0x3C;
            signal_bit_map[13] = 0x00;
            break;
        case(5):
            signal_bit_map[12] = 0x38;
            signal_bit_map[13] = 0x00;
            break;
        case(6):
            signal_bit_map[12] = 0x33;
            signal_bit_map[13] = 0x00;
            break;
        case(7):
            signal_bit_map[12] = 0xC3;
            signal_bit_map[13] = 0x00;
            break;
        case(8):
            signal_bit_map[12] = 0x00;
            signal_bit_map[13] = 0x00;
            break;

        case(11):
            signal_bit_map[12] = 0x3C;
            signal_bit_map[13] = 0x00;
            break;
        case(12):
            signal_bit_map[12] = 0x38;
            signal_bit_map[13] = 0x00;
            break;
        case(13):
            signal_bit_map[12] = 0x33;
            signal_bit_map[13] = 0x00;
            break;
        case(14):
            signal_bit_map[12] = 0xC3;
            signal_bit_map[13] = 0x00;
            break;
        case(15):
            signal_bit_map[12] = 0x83;
            signal_bit_map[13] = 0x00;
            break;
        case(16):
            signal_bit_map[12] = 0x33;
            signal_bit_map[13] = 0x00;
            break;
        case(17):
            signal_bit_map[12] = 0x3C;
            signal_bit_map[13] = 0x00;
            break;
        case(18):
            signal_bit_map[12] = 0x00;
            signal_bit_map[13] = 0x00;
            break;

        default:
            signal_bit_map[12] = 0x00;
            signal_bit_map[13] = 0x00;
            break;

        }

        for(int i = 0; i < 12; i++)
        {
            signal_bit_map[i] = smem.vGetUCData(200+i);                                 //Get Signal Light Data
        }

        ucTmp[0] = smem.vGetUCData(200+12);
        ucTmp[1] = smem.vGetUCData(200+13);

        if(ucTmp[0] == signal_bit_map[12] && ucTmp[1] == signal_bit_map[13])    //every is the same, do nothing.
        {
            return true;
        }

        for(int i = 12; i < 14; i++)                                                  //Save ReverseTime Data
        {
            smem.vSetUCData(200+i, signal_bit_map[i]);
        }

        ucSendTMP[0] = 0xAA;
        ucSendTMP[1] = 0xBB;
        ucSendTMP[2] = 0x11;
        ucSendTMP[3] = signal_bit_map[1];
        ucSendTMP[4] = signal_bit_map[0];
        ucSendTMP[5] = signal_bit_map[3];
        ucSendTMP[6] = signal_bit_map[2];
        ucSendTMP[7] = signal_bit_map[5];
        ucSendTMP[8] = signal_bit_map[4];
        ucSendTMP[9] = signal_bit_map[7];
        ucSendTMP[10] = signal_bit_map[6];
        ucSendTMP[11] = signal_bit_map[9];
        ucSendTMP[12] = signal_bit_map[8];
        ucSendTMP[13] = signal_bit_map[11];
        ucSendTMP[14] = signal_bit_map[10];
        ucSendTMP[15] = signal_bit_map[13];
        ucSendTMP[16] = signal_bit_map[12];
        ucSendTMP[17] = 0x00;
        ucSendTMP[18] = 0xAA;
        ucSendTMP[19] = 0xCC;
        ucSendTMP[20] = 0x00;
        for (int i=0; i<20; i++)
        {
            ucSendTMP[20]^=ucSendTMP[i];
        }

        /*+++++++++++++++++*/
        writeJob.WritePhysicalOut(ucSendTMP, 21, DEVICETRAFFICLIGHT);                 //0x11
//  printf("Send writeJob DEVICETRAFFICLIGHT. (ReverseTime)\n");
        /*-----------------*/

        if( cCURRENTLIGHTSTATUS == smem.GetcFace() )
            screenCurrentLightStatus.vRefreshCurrentLightStatusData();

    }
    catch(...) {}
}
//--------------------------------------------------------------------------
bool CSTC::vSendHeartBeatToLCX405inCSTC(void)
{
    try
    {

        unsigned char ucHeartBeat[22];  //OTCombo0713 SayHelloToCard
        char msg[64];

        /*
          ++usiLCX405WatchDogCount;

          if(usiLCX405WatchDogCount > 1) {
            usiLCX405WatchDogCount = 0;
        */
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

//  sprintf(msg,"SendLightHeartBeatCSTC");
//  smem.vWriteMsgToDOM(msg);


//  }

        return 0;

    }
    catch(...) {}
}
//--------------------------------------------------------------------------
bool CSTC::vReportRevStatus5F02(unsigned char ucBeginEnd)  //5F02, Begin:1 End:0
{
    try
    {

        unsigned char data[4];
        data[0] = 0x5F;
        data[1] = 0x02;
        data[2] = ucBeginEnd;

        //OTCombo0713
        MESSAGEOK _MsgOK;
        _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, 3, true);
        _MsgOK.InnerOrOutWard = cOutWard;
//  writeJob.WriteWorkByMESSAGEOUT(_MsgOK);
        writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICECENTER92);
        /*-----------------*/
//mallocFuck  free(data);

    }
    catch (...) {}
}
//--------------------------------------------------------------------------
short int CSTC::vGetAdjCurrentMaxGreen(void)
{
    try
    {
        int iOrgGreen = _exec_plan._ptr_subplaninfo[_exec_phase_current_subphase]._green;
        int iGreen = _exec_plan._ptr_subplaninfo[_exec_phase_current_subphase].compensated_green(_exec_plan._shorten_cycle);
        int iMax = _exec_plan._ptr_subplaninfo[_exec_phase_current_subphase]._max_green;
        short int siTmp, siRet;
        if(iMax <= 255 && iMax >= iOrgGreen)
        {
            siTmp = iMax - iOrgGreen;
            siRet = iGreen + siTmp;
        }
        else
        {
            siRet = iMax;
        }
        return siRet;

    }
    catch (...) {}
}
//--------------------------------------------------------------------------
short int CSTC::vGetAdjCurrentMinGreen(void)
{
    try
    {
        int iOrgGreen = _exec_plan._ptr_subplaninfo[_exec_phase_current_subphase]._green;
        int iGreen = _exec_plan._ptr_subplaninfo[_exec_phase_current_subphase].compensated_green(_exec_plan._shorten_cycle);
        int iMin = _exec_plan._ptr_subplaninfo[_exec_phase_current_subphase]._min_green;
        short int siTmp, siRet;
        if(iMin > 0 && iMin <= iOrgGreen)
        {
            siTmp = iOrgGreen - iMin;
            siRet = iGreen - siTmp;
        }
        else
        {
            siRet = iMin;
        }
        return siRet;

    }
    catch (...) {}
}
//--------------------------------------------------------------------------
short int CSTC::vGetAdjCurrentPhaseGreenTime(void)
{
    try
    {
        int iGreen = _exec_plan._ptr_subplaninfo[_exec_phase_current_subphase].compensated_green(_exec_plan._shorten_cycle);
        int ipedGreenFlash = _exec_plan._ptr_subplaninfo[_exec_phase_current_subphase].compensated_pedgreen_flash(_exec_plan._shorten_cycle);
        int ipedRed = _exec_plan._ptr_subplaninfo[_exec_phase_current_subphase].compensated_pedred(_exec_plan._shorten_cycle);
        printf("Cstc, iGreen:%d, ipedGreenFlash:%d, ipedRed:%d\n", iGreen, ipedGreenFlash, ipedRed);
        short int siRet;
        siRet = iGreen + ipedGreenFlash + ipedRed;
        return siRet;

    }
    catch (...) {}
}
//--------------------------------------------------------------------------
unsigned short int CSTC::vGetTODRunningPlanID(void)
{
    try
    {
        unsigned short int usiRet;
        usiRet = plan[NEW_TOD_PLAN_ID]._planid;
        return usiRet;

    }
    catch (...) {}
}
//----------------------------------------------------------
void CSTC::Lock_to_Determine_SegmentPlanPhase_For_Act(time_t *t, CPhaseInfo *ret_phase, CPlanInfo *ret_plan, CSegmentInfo *ret_segment, CPlanInfo *running_plan)
{
    try
    {

        int iRunningDaySegNo;
        int iRunningPlanID;

        int iExecSeg;
        unsigned char ucTmp;
        DATA_Bit _ControlStrategy;
        CSegmentInfo _should_exec_segment;

        static struct tm* currenttime;

        *t = time(NULL);
        currenttime = localtime(t);

        bool _exec_segment_is_holiday=false;
        for(int i=0; i<AMOUNT_HOLIDAY_SEG; i++)
        {
            if(    (    ( (currenttime->tm_year+1900) > holidayseg[i]._start_year )
                        || ( (currenttime->tm_year+1900)== holidayseg[i]._start_year && (currenttime->tm_mon+1) > holidayseg[i]._start_month )
                        || ( (currenttime->tm_year+1900)== holidayseg[i]._start_year && (currenttime->tm_mon+1)== holidayseg[i]._start_month && (currenttime->tm_mday)>= holidayseg[i]._start_day ) )
                    && (    ( (currenttime->tm_year+1900) < holidayseg[i]._end_year )
                            || ( (currenttime->tm_year+1900)== holidayseg[i]._end_year   && (currenttime->tm_mon+1) < holidayseg[i]._end_month )
                            || ( (currenttime->tm_year+1900)== holidayseg[i]._end_year   && (currenttime->tm_mon+1)== holidayseg[i]._end_month   && (currenttime->tm_mday)<= holidayseg[i]._end_day ) ) )
            {
                if(segment[holidayseg[i]._segment_type]._ptr_seg_exec_time)
                {
                    _should_exec_segment = segment[holidayseg[i]._segment_type];
                    printf("in holiday OT_exec_segment:%d", _should_exec_segment._segment_type);
                }
                else
                {
                    printf("Error in act finding\n");
                    _should_exec_segment = segment[DEFAULT_SEGMENTTYPE];
                }
                _exec_segment_is_holiday=true;

                printf("Today is in Holiday Segment:%d\n",i);

                break;
            }
        }

        //***********************************
        //****  check in which weekdayseg
        if(_exec_segment_is_holiday==false)
        {
            if(    ((((((currenttime->tm_mday-1)/7)+1)%2)==0 ) && (currenttime->tm_wday>=((currenttime->tm_mday-1)%7)))
                    || ((((((currenttime->tm_mday-1)/7)+1)%2)> 0 ) && (currenttime->tm_wday< ((currenttime->tm_mday-1)%7))) )
            {

                int wday = (currenttime->tm_wday==0)?13:(currenttime->tm_wday+6);

                if(segment[weekdayseg[wday]._segment_type]._ptr_seg_exec_time)
                {
                    _should_exec_segment = segment[weekdayseg[wday]._segment_type]; //tm_wday:{1-6} => P92:{11-16} => weekdayseg:{7-12}
                    printf("in even OT_exec_segment:%d", _should_exec_segment._segment_type);
                }
                else
                {
                    _should_exec_segment = segment[DEFAULT_SEGMENTTYPE];
                    printf("Err\n");
                }

            }//end if(even week)

            else   //odd week
            {

                int wday = (currenttime->tm_wday==0)?6:(currenttime->tm_wday-1);

                if(segment[weekdayseg[wday]._segment_type]._ptr_seg_exec_time)
                {
                    _should_exec_segment = segment[weekdayseg[wday]._segment_type]; //tm_wday:{1-6} => P92:{1-6} => weekdayseg:{0-5}
                    printf("in odd OT_exec_segment:%d", _should_exec_segment._segment_type);
                }
                else
                {
                    _should_exec_segment = segment[DEFAULT_SEGMENTTYPE];
                    printf("Error\n");
                }

            }//end else(odd week)
        }//end else(holiday)


        //***********************************
        //**** check and modify _exec_plan
        for(int i=0; i<_should_exec_segment._segment_count; i++)
        {
            iExecSeg = i;

            printf("i:%d, hour:%d, min:%d, sHour:%d\n", i, currenttime->tm_hour, currenttime->tm_min, _should_exec_segment._ptr_seg_exec_time[i]._hour);

            if(     ( i == (_should_exec_segment._segment_count-1) )  //last segment
                    ||(  ( i <  (_should_exec_segment._segment_count-1) )  //not last segment
                         &&(    (    ( currenttime->tm_hour > _should_exec_segment._ptr_seg_exec_time[i]._hour )
                                     || ( currenttime->tm_hour ==_should_exec_segment._ptr_seg_exec_time[i]._hour    && currenttime->tm_min >=_should_exec_segment._ptr_seg_exec_time[i]._minute  ) )
                                && (    ( currenttime->tm_hour  <_should_exec_segment._ptr_seg_exec_time[i+1]._hour)
                                        || ( currenttime->tm_hour ==_should_exec_segment._ptr_seg_exec_time[i+1]._hour  && currenttime->tm_min  <_should_exec_segment._ptr_seg_exec_time[i+1]._minute) ) ) ) )
            {
                iRunningDaySegNo = i;
                iRunningPlanID = _should_exec_segment._ptr_seg_exec_time[i]._planid;
                printf("iRunningPlanID:%d\n", iRunningPlanID);
                break;
            }
        }

        *ret_segment = _should_exec_segment;
        *ret_plan = plan[iRunningPlanID];
        *running_plan = _exec_plan;
        *ret_phase = phase[plan[iRunningPlanID]._phase_order];

    }
    catch (...) {}
}
//----------------------------------------------------------
bool CSTC::vSendRevSync()
{
    try
    {
        time_t currenttime=time(NULL);
        struct tm *now=localtime(&currenttime);
        unsigned int temphh=(now->tm_hour),tempmm=(now->tm_min);
        unsigned char data[32];
        MESSAGEOK _MSG;

        data[0] = 0x5F;
        data[1] = 0xA1;
        data[2] = (now->tm_year) - 11;
        data[3] = (now->tm_mon) + 1;
        data[4] = (now->tm_mday);
        data[5] = (now->tm_hour);
        data[6] = (now->tm_min);
        data[7] = (now->tm_sec);
        data[8] = _exec_reversetime_current_rev_step;
        data[9] = _exec_rev.usiHourStartIn;
        data[10] = _exec_rev.usiMinStartIn;
        data[11] = _exec_rev.usiHourEndIn;
        data[12] = _exec_rev.usiMinEndIn;
        data[13] = _exec_rev.usiHourStartOut;
        data[14] = _exec_rev.usiMinStartOut;
        data[15] = _exec_rev.usiHourEndOut;
        data[16] = _exec_rev.usiMinEndOut;

        data[17] = HI(_exec_rev.usiClearTime);
        data[18] = LO(_exec_rev.usiClearTime);
        data[19] = HI(_exec_rev.usiFlashGreen);
        data[20] = LO(_exec_rev.usiFlashGreen);
        data[21] = HI(_exec_rev.usiGreenTime);
        data[22] = LO(_exec_rev.usiGreenTime);



        _MSG = oDataToMessageOK.vPackageINFOTo92Protocol(data, 23, true);
        _MSG.InnerOrOutWard = cOutWard;
        writeJob.WritePhysicalOut(_MSG.packet, _MSG.packetLength, DEVICEREVSYNC);

        printf("%d:%d:%d, step:%d, SI-%d:%d, EI-%d:%d, SO-%d:%d, EO-%d:%d, %d,%d,%d\n",
               data[5], data[6], data[7], data[8],
               data[9], data[10], data[11], data[12], data[13], data[14], data[15], data[16],
               _exec_rev.usiClearTime, _exec_rev.usiFlashGreen, _exec_rev.usiGreenTime
              );


    }
    catch (...) {}
}
//----------------------------------------------------------
void CSTC::vGetRevInfo(unsigned short int *usiExecRevStep, CReverseTimeInfo *ret_rev)
{
    try
    {

        *usiExecRevStep = _exec_reversetime_current_rev_step;
        *ret_rev = _exec_rev;

    }
    catch (...) {}
}
//----------------------------------------------------------
unsigned int CSTC::vGetRevTimerSec(void)
{
    try
    {

        //Should be mutex

        unsigned int time_difference=0;
        timer_gettime(_timer_reversetime, &_itimer_reversetime);
        time_difference = (_itimer_reversetime.it_value.tv_sec) + 1;

        return time_difference;

    }
    catch(...) {}
}
//--------------------------------------------------------------------------
bool CSTC::vReportCCTRevStatus5F82(void)
{
    try
    {
        unsigned short int data_length = 9;
        unsigned char data[16];
        unsigned int uiStepSec;
        unsigned short int usiStepSec;

        if(smem.vGetUCData(revSendProtocol) > 0)
        {

            uiStepSec = vGetRevTimerSec();
            if(uiStepSec > 65565)
            {
                uiStepSec = 65535;
            }
            usiStepSec = uiStepSec;

            data[0] = 0x5F;
            data[1] = 0x82;
            data[2] = _exec_reversetime_current_rev_step;
            data[3] = smem.vGetUCData(200+12);
            data[4] = smem.vGetUCData(200+13);
            data[5] = HI(usiStepSec);
            data[6] = LO(usiStepSec);
            data[7] = smem.vGetUCData(revSyncByteStatus);
            data[8] = smem.vGetUCData(revDefaultVehWay);

            MESSAGEOK _MsgOK;
            _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, data_length, true);
            writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICECENTER92);
        }

    }
    catch (...) {}
}
//----------------------------------------------------------
unsigned int CSTC::vDetermineTimeToNextPlan(void)
{
    try
    {
        int iExecSegNo;
        unsigned char ucTmp;
        DATA_Bit _ControlStrategy;

        unsigned int uiNowSec = 0;
        unsigned int uiNextSegSec = 86400;

        CSegmentInfo _tmpSegment;


        static time_t now;
        static struct tm* currenttime;

        now = time(NULL);
        currenttime = localtime(&now);

        uiNowSec = currenttime->tm_hour*3600 + currenttime->tm_min*60 + currenttime->tm_sec;


        //***********************************
        //****   check if in holidayseg
        bool _exec_segment_is_holiday=false;

        for(int i=0; i<AMOUNT_HOLIDAY_SEG; i++)
        {
            if(    (    ( (currenttime->tm_year+1900) > holidayseg[i]._start_year )
                        || ( (currenttime->tm_year+1900)== holidayseg[i]._start_year && (currenttime->tm_mon+1) > holidayseg[i]._start_month )
                        || ( (currenttime->tm_year+1900)== holidayseg[i]._start_year && (currenttime->tm_mon+1)== holidayseg[i]._start_month && (currenttime->tm_mday)>= holidayseg[i]._start_day ) )
                    && (    ( (currenttime->tm_year+1900) < holidayseg[i]._end_year )
                            || ( (currenttime->tm_year+1900)== holidayseg[i]._end_year   && (currenttime->tm_mon+1) < holidayseg[i]._end_month )
                            || ( (currenttime->tm_year+1900)== holidayseg[i]._end_year   && (currenttime->tm_mon+1)== holidayseg[i]._end_month   && (currenttime->tm_mday)<= holidayseg[i]._end_day ) ) )
            {
                _tmpSegment = segment[holidayseg[i]._segment_type];
                _exec_segment_is_holiday=true;
                break;
            }
        }

        //***********************************
        //****  check in which weekdayseg
        if(_exec_segment_is_holiday==false)
        {
            if(    ((((((currenttime->tm_mday-1)/7)+1)%2)==0 ) && (currenttime->tm_wday>=((currenttime->tm_mday-1)%7)))
                    || ((((((currenttime->tm_mday-1)/7)+1)%2)> 0 ) && (currenttime->tm_wday< ((currenttime->tm_mday-1)%7))) )
            {

                int wday = (currenttime->tm_wday==0)?13:(currenttime->tm_wday+6);
                _tmpSegment = segment[weekdayseg[wday]._segment_type];
            }//end if(even week)
            else   //odd week
            {
                int wday = (currenttime->tm_wday==0)?6:(currenttime->tm_wday-1);
                _tmpSegment = segment[weekdayseg[wday]._segment_type];
            }//end else(odd week)
        }//end else(holiday)


        //***********************************
        //**** check and modify _exec_plan
        for(iExecSegNo = 0; iExecSegNo < _tmpSegment._segment_count; iExecSegNo++)
        {
            if(     ( iExecSegNo == (_tmpSegment._segment_count - 1) )  //last segment
                    ||(  ( iExecSegNo < (_tmpSegment._segment_count - 1) )  //not last segment
                         &&(    (    ( currenttime->tm_hour > _tmpSegment._ptr_seg_exec_time[iExecSegNo]._hour )
                                     || ( currenttime->tm_hour ==_tmpSegment._ptr_seg_exec_time[iExecSegNo]._hour    && currenttime->tm_min >=_tmpSegment._ptr_seg_exec_time[iExecSegNo]._minute  ) )
                                && (    ( currenttime->tm_hour  <_tmpSegment._ptr_seg_exec_time[iExecSegNo+1]._hour)
                                        || ( currenttime->tm_hour ==_tmpSegment._ptr_seg_exec_time[iExecSegNo+1]._hour  && currenttime->tm_min  <_tmpSegment._ptr_seg_exec_time[iExecSegNo+1]._minute) ) ) ) )
            {

                //find it
                break;

            }//end if(_exec_segment_current_seg_no)
        }//end for(segment_count)

        if(iExecSegNo == (_tmpSegment._segment_count - 1))    //in last seg
        {
            uiNextSegSec = 86400;
            printf("[OT]next seg hour:%d, min:%d\n", _tmpSegment._ptr_seg_exec_time[0]._hour, _tmpSegment._ptr_seg_exec_time[0]._minute);
            printf("[OT]iExecSegNo:%d\n", 0);
        }
        else
        {
            uiNextSegSec = _tmpSegment._ptr_seg_exec_time[iExecSegNo+1]._hour*3600 +
                           _tmpSegment._ptr_seg_exec_time[iExecSegNo+1]._minute*60;

            printf("[OT]next seg hour:%d, min:%d\n", _tmpSegment._ptr_seg_exec_time[iExecSegNo+1]._hour, _tmpSegment._ptr_seg_exec_time[iExecSegNo+1]._minute);
            printf("[OT]iExecSegNo:%d\n", iExecSegNo+1);
        }
        if(uiNextSegSec > 0)
        {
            uiNextSegSec -= 1;
        }

        printf("[OT] uiNowSec is %d\n", uiNowSec);
        printf("[OT] uiNextSegSec is %d\n", uiNextSegSec);
        printf("[OT] uiDiffSegSec is %d\n", (uiNextSegSec - uiNowSec));


        if(uiNextSegSec > uiNowSec)
        {
            return (uiNextSegSec - uiNowSec);
        }
        else
        {
            return 0;
        }


    }
    catch (...) {}
}
//////////////////////////////////////////////////////////////////Arwen add 110211
void CSTC::ConvertSegmentData(bool ucSave)
{
    try
    {
        unsigned short int iTmp;
        char name1[20];
        char name2[20];
        iTmp=oStorINI.vLoadSegmentIntFromINI("UPDate","UPDateStart",1);//if == 1 UpData
        if(iTmp == 1 || ucSave)
        {
            for(int i=1; i<21; i++)
            {
                if (stc.Lock_to_Load_Segment_for_Panel(i))
                {

                    sprintf(name1,"Segment%#03d",i);
                    /*
                    sprintf(name2,"SegmentType"); if(new_data_segment[i]._segment_type!=stc._panel_segment._segment_type || iTmp == 1){oStorINI.vSaveIntSegmentToINI(name1, name2, stc._panel_segment._segment_type);}
                    sprintf(name2,"SegmentCount");if(new_data_segment[i]._segment_count!=stc._panel_segment._segment_count || iTmp == 1) {oStorINI.vSaveIntSegmentToINI(name1, name2, stc._panel_segment._segment_count);}
                    */
                    new_data_segment[i]._segment_type=stc._panel_segment._segment_type;
                    new_data_segment[i]._segment_count=stc._panel_segment._segment_count;
                    //printf("Segment%#03d SegmentType:%d SegmentCount:%d\n",i,stc._panel_segment._segment_type,stc._panel_segment._segment_count);

                    sprintf(name2,"actSegmentType1");
                    iTmp=oStorINI.vLoadSegmentIntFromINI(name1, name2, stc._panel_segment._segment_type);
                    new_data_segment[i]._actSegmentType1 = iTmp;
                    sprintf(name2,"actSegmentType2");
                    iTmp=oStorINI.vLoadSegmentIntFromINI(name1, name2, stc._panel_segment._segment_type);
                    new_data_segment[i]._actSegmentType2 = iTmp;
                    //printf("actSegmentType1:%d actSegmentType2:%d\n",new_data_segment[i]._actSegmentType1,new_data_segment[i]._actSegmentType2);

                    for (int j=0; j<stc._panel_segment._segment_count; j++)
                    {
                        /*
                        sprintf(name2,"Hour%#02d",j);if(new_data_segment[i]._ptr_seg_exec_time[j]._hour!=stc._panel_segment._ptr_seg_exec_time[j]._hour || iTmp == 1){oStorINI.vSaveIntSegmentToINI(name1, name2, stc._panel_segment._ptr_seg_exec_time[j]._hour);}
                        sprintf(name2,"Minute%#02d",j);if(new_data_segment[i]._ptr_seg_exec_time[j]._minute!=stc._panel_segment._ptr_seg_exec_time[j]._minute || iTmp == 1){oStorINI.vSaveIntSegmentToINI(name1, name2, stc._panel_segment._ptr_seg_exec_time[j]._minute);}
                        */
                        sprintf(name2,"PlanId%#02d",j);
                        if(new_data_segment[i]._ptr_seg_exec_time[j]._planid!=stc._panel_segment._ptr_seg_exec_time[j]._planid || iTmp == 1)
                        {
                            oStorINI.vSaveIntSegmentToINI(name1, name2, stc._panel_segment._ptr_seg_exec_time[j]._planid);
                        }
                        /*
                        sprintf(name2,"Cadc_seg%#02d",j);if(new_data_segment[i]._ptr_seg_exec_time[j]._cadc_seg!=stc._panel_segment._ptr_seg_exec_time[j]._cadc_seg || iTmp == 1){oStorINI.vSaveIntSegmentToINI(name1, name2, stc._panel_segment._ptr_seg_exec_time[j]._cadc_seg);}
                        */

                        new_data_segment[i]._ptr_seg_exec_time[j]._hour=stc._panel_segment._ptr_seg_exec_time[j]._hour ;
                        new_data_segment[i]._ptr_seg_exec_time[j]._minute=stc._panel_segment._ptr_seg_exec_time[j]._minute ;
                        new_data_segment[i]._ptr_seg_exec_time[j]._planid=stc._panel_segment._ptr_seg_exec_time[j]._planid ;
                        new_data_segment[i]._ptr_seg_exec_time[j]._cadc_seg=stc._panel_segment._ptr_seg_exec_time[j]._cadc_seg ;

                        sprintf(name2,"compensateMode%#02d",j);
                        iTmp=oStorINI.vLoadSegmentIntFromINI(name1, name2, 0);
                        new_data_segment[i]._ptr_seg_exec_time[j]._compensateMode = iTmp;
                        sprintf(name2,"openRunnig%#02d",j);
                        iTmp=oStorINI.vLoadSegmentIntFromINI(name1, name2, 1);
                        new_data_segment[i]._ptr_seg_exec_time[j]._openRunnig = iTmp;
                        //printf("segmentCount:%d Time %#02d:%#02d PlandID:%d Cadc_seg:%d\n",j,new_data_segment[i]._ptr_seg_exec_time[j]._hour,new_data_segment[i]._ptr_seg_exec_time[j]._minute,new_data_segment[i]._ptr_seg_exec_time[j]._planid,new_data_segment[i]._ptr_seg_exec_time[j]._cadc_seg);
                        //printf("compensateMode%#02d openRunnig%#02d\n",j,new_data_segment[i]._ptr_seg_exec_time[j]._compensateMode,j,new_data_segment[i]._ptr_seg_exec_time[j]._openRunnig);
                        new_data_segment[i]._ptr_seg_exec_time[j]._actMode = stc._panel_segment._ptr_seg_exec_time[j]._actMode;
                    }
                }
            }
            oStorINI.vSaveIntSegmentToINI("UPDate","UPDateStart",0);//Updata Close
        }/*else{
        for(int i=1;i<21;i++){
            sprintf(name1,"Segment%#03d",i);
            sprintf(name2,"SegmentCount");
            if( oStorINI.vLoadSegmentIntFromINI(name1, name2,0) >0){
                new_data_segment[i]._segment_count=oStorINI.vLoadSegmentIntFromINI(name1, name2,new_data_segment[i]._segment_count);
                sprintf(name2,"SegmentType");

                new_data_segment[i]._segment_type =oStorINI.vLoadSegmentIntFromINI(name1, name2,new_data_segment[i]._segment_type);
                sprintf(name2,"actSegmentType1");
                new_data_segment[i]._actSegmentType1=oStorINI.vLoadSegmentIntFromINI(name1, name2, new_data_segment[i]._segment_type);
                sprintf(name2,"actSegmentType2");
                new_data_segment[i]._actSegmentType2=oStorINI.vLoadSegmentIntFromINI(name1, name2, new_data_segment[i]._segment_type);

                for(int j=0;j<new_data_segment[i]._segment_count;j++){
                    sprintf(name2,"Hour%#02d",j);
                    new_data_segment[i]._ptr_seg_exec_time[j]._hour=oStorINI.vLoadSegmentIntFromINI(name1, name2, 0);

                    sprintf(name2,"Minute%#02d",j);
                    new_data_segment[i]._ptr_seg_exec_time[j]._minute=oStorINI.vLoadSegmentIntFromINI(name1, name2, 0);

                    sprintf(name2,"PlanId%#02d",j);
                    new_data_segment[i]._ptr_seg_exec_time[j]._planid=oStorINI.vLoadSegmentIntFromINI(name1, name2, 0);

                    sprintf(name2,"Cadc_seg%#02d",j);
                    new_data_segment[i]._ptr_seg_exec_time[j]._cadc_seg=oStorINI.vLoadSegmentIntFromINI(name1, name2, 0);

                    sprintf(name2,"compensateMode%#02d",j);
                    new_data_segment[i]._ptr_seg_exec_time[j]._compensateMode=oStorINI.vLoadSegmentIntFromINI(name1, name2, 0);

                    sprintf(name2,"openRunnig%#02d",j);
                    new_data_segment[i]._ptr_seg_exec_time[j]._openRunnig = oStorINI.vLoadSegmentIntFromINI(name1, name2, 1);

                }
            }
        }

    }
    */

    }
    catch (...) {}
}
//-----------------------------------------------------------
void CSTC::ACT_Save_Segment_Data(const CSegmentInfo &actsegment)
{
    printf("ACT_Save_Segment_Data Start Arwen++\n");
    unsigned short int iTmp;
    char name1[20];
    char name2[20];
    /******** lock mutex ********/
    pthread_mutex_lock(&CSegmentInfo::_segment_mutex);

    new_data_segment[actsegment._segment_type] = actsegment;

    sprintf(name1,"Segment%#03d",actsegment._segment_type);
    /*
    sprintf(name2,"SegmentType"); oStorINI.vSaveIntSegmentToINI(name1, name2, actsegment._segment_type);
    sprintf(name2,"SegmentCount");oStorINI.vSaveIntSegmentToINI(name1, name2, actsegment._segment_count);
    */
    printf("Segment%#03d SegmentType:%d SegmentCount:%d\n",actsegment._segment_type,actsegment._segment_type,actsegment._segment_count);

    sprintf(name2,"actSegmentType1");
    iTmp=oStorINI.vLoadSegmentIntFromINI(name1, name2, actsegment._actSegmentType1);
    new_data_segment[actsegment._segment_type]._actSegmentType1 = iTmp;
    sprintf(name2,"actSegmentType2");
    iTmp=oStorINI.vLoadSegmentIntFromINI(name1, name2, actsegment._actSegmentType2);
    new_data_segment[actsegment._segment_type]._actSegmentType2 = iTmp;
    printf("actSegmentType1:%d actSegmentType2:%d",new_data_segment[actsegment._segment_type]._actSegmentType1,new_data_segment[actsegment._segment_type]._actSegmentType2);

    for(int j=0; j<actsegment._segment_count; j++)
    {
        /*
        sprintf(name2,"Hour%#02d",j);oStorINI.vSaveIntSegmentToINI(name1, name2, actsegment._ptr_seg_exec_time[j]._hour);
        sprintf(name2,"Minute%#02d",j);oStorINI.vSaveIntSegmentToINI(name1, name2, actsegment._ptr_seg_exec_time[j]._minute);
        */
        sprintf(name2,"PlanId%#02d",j);
        oStorINI.vSaveIntSegmentToINI(name1, name2, actsegment._ptr_seg_exec_time[j]._planid);
        /*
        sprintf(name2,"Cadc_seg%#02d",j);oStorINI.vSaveIntSegmentToINI(name1, name2, actsegment._ptr_seg_exec_time[j]._cadc_seg);
        */

        sprintf(name2,"compensateMode%#02d",j);
        iTmp=oStorINI.vLoadSegmentIntFromINI(name1, name2, 0);
        new_data_segment[actsegment._segment_type]._ptr_seg_exec_time[j]._compensateMode = iTmp;
        sprintf(name2,"openRunnig%#02d",j);
        iTmp=oStorINI.vLoadSegmentIntFromINI(name1, name2, 1);
        new_data_segment[actsegment._segment_type]._ptr_seg_exec_time[j]._openRunnig = iTmp;
        //printf("segmentCount:%d Time %#02d:%#02d PlandID:%d Cadc_seg:%d\n",j,new_data_segment[actsegment._segment_type]._ptr_seg_exec_time[j]._hour,new_data_segment[actsegment._segment_type]._ptr_seg_exec_time[j]._minute,new_data_segment[actsegment._segment_type]._ptr_seg_exec_time[j]._planid,new_data_segment[actsegment._segment_type]._ptr_seg_exec_time[j]._cadc_seg);
        //printf("compensateMode%#02d openRunnig%#02d\n",j,new_data_segment[actsegment._segment_type]._ptr_seg_exec_time[j]._compensateMode,j,new_data_segment[actsegment._segment_type]._ptr_seg_exec_time[j]._openRunnig);
    }
    printf("ACT_Save_Segment_Data End Arwen++\n");
    /******** unlock mutex ********/
    pthread_mutex_unlock(&CSegmentInfo::_segment_mutex);
}
////////////////////////////////////////////////////////////////////
//----------------------------------------------------------
void CSTC::Lock_to_Determine_SegmentPlanPhase_For_Act2
(time_t *t, CPhaseInfo *ret_phase, CPlanInfo *ret_plan, CSegmentInfo *ret_segment,
 CPlanInfo *running_plan,
 unsigned short int &iRunCompensateMode,unsigned short int &iOpenRunnig,unsigned short int actSegmentType)
{
    try
    {

        int iRunningDaySegNo;
        int iRunningPlanID;

        int iExecSeg;
        unsigned char ucTmp;
        DATA_Bit _ControlStrategy;
        CSegmentInfo _should_exec_segment;

        static struct tm* currenttime;

        *t = time(NULL);
        currenttime = localtime(t);

        bool _exec_segment_is_holiday=false;
        for(int i=0; i<AMOUNT_HOLIDAY_SEG; i++)
        {
            if(    (    ( (currenttime->tm_year+1900) > holidayseg[i]._start_year )
                        || ( (currenttime->tm_year+1900)== holidayseg[i]._start_year && (currenttime->tm_mon+1) > holidayseg[i]._start_month )
                        || ( (currenttime->tm_year+1900)== holidayseg[i]._start_year && (currenttime->tm_mon+1)== holidayseg[i]._start_month && (currenttime->tm_mday)>= holidayseg[i]._start_day ) )
                    && (    ( (currenttime->tm_year+1900) < holidayseg[i]._end_year )
                            || ( (currenttime->tm_year+1900)== holidayseg[i]._end_year   && (currenttime->tm_mon+1) < holidayseg[i]._end_month )
                            || ( (currenttime->tm_year+1900)== holidayseg[i]._end_year   && (currenttime->tm_mon+1)== holidayseg[i]._end_month   && (currenttime->tm_mday)<= holidayseg[i]._end_day ) ) )
            {
                if(new_data_segment[holidayseg[i]._segment_type]._ptr_seg_exec_time)
                {
                    _should_exec_segment = new_data_segment[holidayseg[i]._segment_type];

                    if(smem.cPedPushButton.GetOpenSegmentAlter()<=0)
                    {
                        if(actSegmentType==60000)
                        {
                            _should_exec_segment = new_data_segment[_should_exec_segment._actSegmentType1];
                        }
                        else
                        {
                            _should_exec_segment = new_data_segment[_should_exec_segment._actSegmentType2];
                        }
                    }

                    printf("in holiday ARWEN_exec_segment:%d", _should_exec_segment._segment_type);
                }
                else
                {
                    printf("Error in act finding\n");
                    _should_exec_segment = new_data_segment[DEFAULT_SEGMENTTYPE];
                }
                _exec_segment_is_holiday=true;

                printf("Today is in Holiday Segment:%d\n",i);

                break;
            }
        }

        //***********************************
        //****  check in which weekdayseg
        if(_exec_segment_is_holiday==false)
        {
            if(    ((((((currenttime->tm_mday-1)/7)+1)%2)==0 ) && (currenttime->tm_wday>=((currenttime->tm_mday-1)%7)))
                    || ((((((currenttime->tm_mday-1)/7)+1)%2)> 0 ) && (currenttime->tm_wday< ((currenttime->tm_mday-1)%7))) )
            {

                int wday = (currenttime->tm_wday==0)?13:(currenttime->tm_wday+6);

                if(new_data_segment[weekdayseg[wday]._segment_type]._ptr_seg_exec_time)
                {
                    _should_exec_segment = new_data_segment[weekdayseg[wday]._segment_type]; //tm_wday:{1-6} => P92:{11-16} => weekdayseg:{7-12}
                    if(smem.cPedPushButton.GetOpenSegmentAlter()<=0)
                    {
                        if(actSegmentType==60000)
                        {
                            printf("actSegmentType1:%d weekdayseg[%d]._segment_type:%d\n",_should_exec_segment._actSegmentType1,wday,weekdayseg[wday]._segment_type);
                            _should_exec_segment = new_data_segment[_should_exec_segment._actSegmentType1];
                        }
                        else
                        {
                            printf("actSegmentType2:%d weekdayseg[%d]._segment_type:%d\n",_should_exec_segment._actSegmentType2,wday,weekdayseg[wday]._segment_type);
                            _should_exec_segment = new_data_segment[_should_exec_segment._actSegmentType2];
                        }
                    }

                    printf("in even ARWEN_exec_segment:%d actSegmentType:%d", _should_exec_segment._segment_type,actSegmentType);
                }
                else
                {
                    _should_exec_segment = new_data_segment[DEFAULT_SEGMENTTYPE];
                    printf("Err\n");
                }

            }//end if(even week)

            else   //odd week
            {

                int wday = (currenttime->tm_wday==0)?6:(currenttime->tm_wday-1); //if true wday = 6 if false wday = currenttime->tm_wday-1

                if(new_data_segment[weekdayseg[wday]._segment_type]._ptr_seg_exec_time)
                {
                    _should_exec_segment = new_data_segment[weekdayseg[wday]._segment_type]; //tm_wday:{1-6} => P92:{1-6} => weekdayseg:{0-5}
                    if(smem.cPedPushButton.GetOpenSegmentAlter()<=0)
                    {
                        if(actSegmentType==60000)
                        {
                            printf("_should_exec_segment._actSegmentType1:%d\n",_should_exec_segment._actSegmentType1);
                            _should_exec_segment = new_data_segment[_should_exec_segment._actSegmentType1];
                        }
                        else
                        {
                            printf("_should_exec_segment._actSegmentType2:%d\n",_should_exec_segment._actSegmentType2);
                            _should_exec_segment = new_data_segment[_should_exec_segment._actSegmentType2];
                        }
                    }
                    printf("in odd ARWEN_exec_segment:%d", _should_exec_segment._segment_type);
                }
                else
                {
                    _should_exec_segment = new_data_segment[DEFAULT_SEGMENTTYPE];
                    printf("Error\n");
                }

            }//end else(odd week)
        }//end else(holiday)


        //***********************************
        //**** check and modify _exec_plan
        for(int i=0; i<_should_exec_segment._segment_count; i++)
        {
            iExecSeg = i;

            printf("i:%d, hour:%d, min:%d, sHour:%d\n", i, currenttime->tm_hour, currenttime->tm_min, _should_exec_segment._ptr_seg_exec_time[i]._hour);

            if(     ( i == (_should_exec_segment._segment_count-1) )  //last segment
                    ||(  ( i <  (_should_exec_segment._segment_count-1) )  //not last segment
                         &&(    (    ( currenttime->tm_hour > _should_exec_segment._ptr_seg_exec_time[i]._hour )
                                     || ( currenttime->tm_hour ==_should_exec_segment._ptr_seg_exec_time[i]._hour    && currenttime->tm_min >=_should_exec_segment._ptr_seg_exec_time[i]._minute  ) )
                                && (    ( currenttime->tm_hour  <_should_exec_segment._ptr_seg_exec_time[i+1]._hour)
                                        || ( currenttime->tm_hour ==_should_exec_segment._ptr_seg_exec_time[i+1]._hour  && currenttime->tm_min  <_should_exec_segment._ptr_seg_exec_time[i+1]._minute) ) ) ) )
            {
                iRunningDaySegNo = i;
                iRunningPlanID = _should_exec_segment._ptr_seg_exec_time[i]._planid;
                printf("iRunningPlanID:%d\n", iRunningPlanID);
                //iRunCompensateMode=_should_exec_segment._ptr_seg_exec_time[i]._compensateMode;
                iRunCompensateMode = smem.getSegmentCompensateMode(_should_exec_segment._ptr_seg_exec_time[i]._actMode);//arwen ++ 1001013
                iOpenRunnig=_should_exec_segment._ptr_seg_exec_time[i]._openRunnig;
                printf("iRunCompensateMode:%d\n", iRunCompensateMode);
                printf("iOpenRunnig:%d\n", iOpenRunnig);
                break;
            }
        }

        *ret_segment = _should_exec_segment;
        *ret_plan = plan[iRunningPlanID];
        *running_plan = _exec_plan;
        *ret_phase = phase[plan[iRunningPlanID]._phase_order];

    }
    catch (...) {}
}
//------------------------------------------------------------------------------------------------
void CSTC::changeActModeToArwenAct(unsigned short int segActNum)    //切換觸動模式
{
    //20150727 tmpSegActType 1:TouchGreen 2:FixedGreen 3:Flash 4:LongGreen 5:Ped 6:時制變動加半觸動 0:noOpen
    try
    {
        printf("%sCSTC::changeActModeToArwenAct segActType:%d %s\n",ColorBlue,segActNum,ColorNormal);
        unsigned short int tmpSegActType;
        unsigned short int tmpSegActPlan;
        unsigned short int tmpSegActZoneTimes;
        unsigned short int tmpSegActCompensateMode;

        unsigned short int tmpPhaseTrggerSwitch[8];
        unsigned short int tmpPhaseExtendSec[8];
        char tmp[30];

        smem.setRunSegmentActMode(segActNum);//set act plan

        if(segActNum != 12 && segActNum<10 )
        {
            tmpSegActType = smem.getSegmentActType(segActNum);
            tmpSegActPlan = smem.getSegmentActPlan(segActNum);
            tmpSegActZoneTimes = smem.getSegmentZoneTimes(segActNum);
            tmpSegActCompensateMode = smem.getSegmentCompensateMode(segActNum);
            printf("to ArwenAct SegActType:%d SegActPlan:%d SegActZoneTimes:%d\n",tmpSegActType,tmpSegActPlan,tmpSegActZoneTimes);
            for(int i = 0 ; i < 8 ; i++)
            {
                tmpPhaseTrggerSwitch[i] = smem.getSegmentActVDPhaseTriggerSwitch(segActNum,i);
                tmpPhaseExtendSec[i] = smem.getSegmentActExtendSec(segActNum,i);
                printf("PhaseTrggerSwitch[%d]:%d PhaseExtendSec[%d]:%d\n",i,tmpPhaseTrggerSwitch[i],i,tmpPhaseExtendSec[i]);
                for(int j=0 ; j <8 ; j++)
                {
                }
            }
        }
        else  //close
        {
            tmpSegActType = 0 ;
            tmpSegActZoneTimes = 3;
            tmpSegActCompensateMode = 0;
            for(int i = 0 ; i < 8 ; i++)
            {
                tmpPhaseTrggerSwitch[i] = 0;
                tmpPhaseExtendSec[i] = 0;
            }
        }
        printf("\ntmpSegActType== %d\n",tmpSegActType);
        //tmpSegActType 1:TouchGreen 2:FixedGreen 3:Flash 4:LongGreen 0:noOpen
        if(tmpSegActType == 1)    //2013/01/28 chou check
        {
            //CCJ 20140526
            smem.vSetUCData(fixedGreenModeOpen,0x01);//FixedGreen true
            smem.vSetUCData(ActType, 0x01);//設定為半觸
            smem.vSetUSIData(ActPlan, tmpSegActPlan);//設定對應的時制計畫
            //CCJ 20140526
            //smem.vSetUCData(fixedGreenModeOpen,0);//FixedGreen Close
            oStorINI.vSaveIntToINI("CCTSwitch","fixedGreenModeOpen",0);
            oStorINI.vSaveIntToINI("CCTSwitch", "OpenCompensationAct", 1);
            smem.cPedPushButton.SetCompensationAct(1);//compensationAct
            smem.vSetBOOLData(TC_CCT_In_LongTanu_ActuateType_FunctionEnable, true/*bTMP_ActuateSwitch*/);//20140526 這應該是有用的，主要的開關
            smem.vSetBOOLData(TC_Actuateautoswitch, true/*b_TMP_Actuateautoswitch*/);
            smem.vSetUSIData(TC_CCT_In_LongTanu_ActuateType_Switch,smem.vGetUSIData(TC_CCT_In_LongTanu_ActuateType_Switch) /*usiActuatePlan*/);
            smem.vSetActuatePhaseExtend(/*ucActuatePhaseID*/0);//20140526 TFD觸動使用
            smem.vSetUSIData(TC_ActuateVDID, /*usiActuateVDID*/11111);//20140526 似乎是畫面edit之用而已
            smem.vSetUCData(TC_CCT_ActuateType_By_TOD, /*ucActuateType_By_TOD*/false);
            smem.vSetBOOLData(TCSegmentTypeUpdate, true);
            if(smem.vGetUCData(TC92_ucControlStrategy)==0x01 || smem.vGetUCData(TC92_ucControlStrategy)==0x0B)
            {
            }
            else
            {
                smem.vSetUCData(TC92_ucControlStrategy,0x01);
            }
        }
        else if(tmpSegActType==2)  //固定時制
        {
            //CCJ 20140526
            smem.vSetUCData(fixedGreenModeOpen,0x01);//FixedGreen true
            smem.vSetUCData(ActType, 0x02);//設定為固定時制
            smem.vSetUSIData(ActPlan, tmpSegActPlan);//設定對應的時制計畫
            //CCJ 20140526
            smem.vSetUCData(fixedGreenModeOpen,1);//FixedGreen Open
            oStorINI.vSaveIntToINI("CCTSwitch","fixedGreenModeOpen",1);
            oStorINI.vSaveIntToINI("CCTSwitch", "OpenCompensationAct", 0);
            smem.cPedPushButton.SetCompensationAct(0);//compensationAct
            smem.vSetBOOLData(TC_CCT_In_LongTanu_ActuateType_FunctionEnable, true/*bTMP_ActuateSwitch*/);//20140526 這應該是有用的，主要的開關
            smem.vSetUSIData(TC_CCT_In_LongTanu_ActuateType_Switch,smem.vGetUSIData(TC_CCT_In_LongTanu_ActuateType_Switch) /*usiActuatePlan*/);
            smem.vSetBOOLData(TC_Actuateautoswitch, true/*b_TMP_Actuateautoswitch*/);
            smem.vSetActuatePhaseExtend(/*ucActuatePhaseID*/0);//20140526 TFD觸動使用
            smem.vSetUSIData(TC_ActuateVDID, /*usiActuateVDID*/11111);//20140526 似乎是畫面edit之用而已
            smem.vSetUCData(TC_CCT_ActuateType_By_TOD, /*ucActuateType_By_TOD*/false);
            smem.vSetBOOLData(TCSegmentTypeUpdate, true);
            if(smem.vGetUCData(TC92_ucControlStrategy)==0x01 || smem.vGetUCData(TC92_ucControlStrategy)==0x0B)
            {
            }
            else
            {
                smem.vSetUCData(TC92_ucControlStrategy,0x01);
            }
        }
        else if(tmpSegActType==3 || tmpSegActType==4)  //長綠和閃光
        {
            //CCJ 20140526
            smem.vSetUCData(fixedGreenModeOpen,0x01);//FixedGreen true
            smem.vSetUCData(ActType, tmpSegActType);//設定為半觸
            smem.vSetUSIData(ActPlan, tmpSegActPlan);//設定對應的時制計畫
            //CCJ 20140526
            smem.vSetBOOLData(TC_CCT_In_LongTanu_ActuateType_FunctionEnable, true/*bTMP_ActuateSwitch*/);//20140526 這應該是有用的，主要的開關，觸動策略開關
            smem.vSetBOOLData(TC_Actuateautoswitch, true/*b_TMP_Actuateautoswitch*/);//公車觸動開關
            //smem.vSetUSIData(TC_CCT_In_LongTanu_ActuateType_Switch,smem.vGetUSIData(TC_CCT_In_LongTanu_ActuateType_Switch) /*usiActuatePlan*/);
            smem.vSetUSIData(TC_CCT_In_LongTanu_ActuateType_Switch,tmpSegActPlan);
            smem.vSetActuatePhaseExtend(/*ucActuatePhaseID*/1);//20140526 TFD觸動使用
            smem.vSetUSIData(TC_ActuateVDID, /*usiActuateVDID*/11111);//20140526 似乎是畫面edit之用而已
            smem.vSetUCData(TC_CCT_ActuateType_By_TOD, /*ucActuateType_By_TOD*/true);
            smem.vSetBOOLData(TCSegmentTypeUpdate, true);
        }
        else if(tmpSegActType == 6)  //20150727 時制變動加半觸動
        {
            //CCJ 20140526
            smem.vSetUCData(fixedGreenModeOpen,0x01);//FixedGreen true
            smem.vSetUCData(ActType, 0x06);//設定為半觸
            smem.vSetUSIData(ActPlan, tmpSegActPlan);//設定對應的時制計畫
            //CCJ 20140526
            //smem.vSetUCData(fixedGreenModeOpen,0);//FixedGreen Close
            oStorINI.vSaveIntToINI("CCTSwitch","fixedGreenModeOpen",0);
            oStorINI.vSaveIntToINI("CCTSwitch", "OpenCompensationAct", 1);
            smem.cPedPushButton.SetCompensationAct(1);//compensationAct
            smem.vSetBOOLData(TC_CCT_In_LongTanu_ActuateType_FunctionEnable, true/*bTMP_ActuateSwitch*/);//20140526 這應該是有用的，主要的開關
            smem.vSetBOOLData(TC_Actuateautoswitch, true/*b_TMP_Actuateautoswitch*/);
            smem.vSetUSIData(TC_CCT_In_LongTanu_ActuateType_Switch,smem.vGetUSIData(TC_CCT_In_LongTanu_ActuateType_Switch) /*usiActuatePlan*/);
            smem.vSetActuatePhaseExtend(/*ucActuatePhaseID*/0);//20140526 TFD觸動使用
            smem.vSetUSIData(TC_ActuateVDID, /*usiActuateVDID*/11111);//20140526 似乎是畫面edit之用而已
            smem.vSetUCData(TC_CCT_ActuateType_By_TOD, /*ucActuateType_By_TOD*/false);
            smem.vSetBOOLData(TCSegmentTypeUpdate, true);
            if(smem.vGetUCData(TC92_ucControlStrategy)==0x01 || smem.vGetUCData(TC92_ucControlStrategy)==0x0B)
            {
            }
            else
            {
                smem.vSetUCData(TC92_ucControlStrategy,0x01);
            }
        }
        else
        {
            //CCJ 20140526
            smem.vSetUCData(fixedGreenModeOpen,0x01);//FixedGreen true
            smem.vSetUCData(ActType, 0x00);//設定為關閉
            smem.vSetUSIData(ActPlan, tmpSegActPlan);//設定對應的時制計畫
            //CCJ 20140526
            printf("tmpSegActType No OPEN vGetBOOLData(TC_CCT_In_LongTanu_ActuateType_FunctionEnable):%d\n",smem.vGetBOOLData(TC_CCT_In_LongTanu_ActuateType_FunctionEnable));
            printf("tmpSegActType No OPEN vGetActuatePhaseExtend():%d\n",smem.vGetActuatePhaseExtend());
            printf("tmpSegActType No OPEN smem.vGetUCData(TC_CCT_ActuateType_By_TOD):%d\n",smem.vGetUCData(TC_CCT_ActuateType_By_TOD));
            if(smem.vGetBOOLData(TC_CCT_In_LongTanu_ActuateType_FunctionEnable)!=true || smem.vGetActuatePhaseExtend()!=0 || smem.vGetUCData(TC_CCT_ActuateType_By_TOD)!=true)
            {
                smem.vSetBOOLData(TC_CCT_In_LongTanu_ActuateType_FunctionEnable,false/*bTMP_ActuateSwitch*/);
                smem.vSetActuatePhaseExtend(/*ucActuatePhaseID*/0);
                smem.vSetUCData(TC_CCT_ActuateType_By_TOD, /*ucActuateType_By_TOD*/false);
                smem.vSetBOOLData(TCSegmentTypeUpdate, true);
            }
            if(smem.vGetUCData(TC92_ucControlStrategy)==0x01 || smem.vGetUCData(TC92_ucControlStrategy)==0x0B)
            {
            }
            else
            {
                smem.vSetUCData(TC92_ucControlStrategy,0x01);
            }
        }
        if(segActNum != 12 )
        {
            for(int i=0; i<8; i++)
            {
                sprintf(tmp,"PhaseTriggerSwitch%d",i);
                smem.vSetVDPhaseTriggerSwitch(i,tmpPhaseTrggerSwitch[i]);
                oStorINI.vSaveIntToINI("VDTrigger", tmp, tmpPhaseTrggerSwitch[i]);
                printf("p[%d]=%d ",i,smem.vGetVDPhaseTriggerSwitch(i));
            }
            if(segActNum == 0)
            {
                tmpSegActType = 0;
            }
        }

        unsigned char data[50];
        int data_length = 0;
        data[data_length] = 0x5F;
        data_length++;
        data[data_length] = 0xA8;
        data_length++;
        data[data_length] = tmpSegActType;
        data_length++;
        data[data_length] = tmpSegActZoneTimes;
        data_length++;
        for(int i = 0 ; i<8; i++)
        {
            data[data_length] = tmpPhaseTrggerSwitch[i];
            data_length++;
        }
        for(int i = 0 ; i<8; i++)
        {
            data[data_length] = tmpPhaseExtendSec[i];
            data_length++;
        }
        MESSAGEOK _MsgOK;
        _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, data_length,false);
        _MsgOK.InnerOrOutWard = cOutWard;
        writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICECCJDYNCTL);


    }
    catch (...) {}
}
void CSTC::Check5F1C(void)
{
    if(smem.Restartcheck5F1C == true)
    {
        if (stc.Fixgreen == true)
        {
            printf("\n\n\n\nFixgreen == true !!\n Fixgreen == true !! \n%d\n\n\n\n\n\n\n",_exec_phase_current_subphase);
            if(_exec_phase_current_subphase > 0)
            {
                int iMin = _exec_plan._ptr_subplaninfo[_exec_phase_current_subphase]._min_green;

                printf("\n\n\n\n\nMingreen!!!\n mingreen = %d!!!\n\n\n\n",iMin);
                timer_gettime(_timer_plan,&_itimer_plan);
                _itimer_plan.it_value.tv_sec = iMin;

                for(int ii = 0; ii < 4; ii++)
                {
                    timer_settime(_timer_plan, 0, &_itimer_plan, NULL);

                    _itimer_plan_WDT = _itimer_plan; /*OT_PLAN_WDT*/
                    _itimer_plan_WDT.it_value.tv_sec += 2;
                    if(timer_settime( _timer_plan_WDT, 0, &_itimer_plan_WDT, NULL ))
                    {
                        printf("PlanWDT Settime Error!.\n");
                        exit( 1 );
                    }
                    timer_settime( _timer_redcount, 0, & _itimer_redcount, NULL );
                }
                stc.Fixgreen = false;
            }
            else
            {
            }
        }
        else
        {
        }
    }
    else
    {
        if (stc.Fixgreen == true)
        {
            printf("\n\nyou can't call 5F1C again!!\n\n");
        }
    }

}
void CSTC::CheckPed(void)   //check fix 若閃光或全紅開關on則閃燈off,若觸動開始後tod到第二分相的全紅步階警示燈off 2013/01/31
{
    unsigned char dtat[3];
    MESSAGEOK _MsgOK;
    if( _exec_plan._phase_order==FLASH_PHASEORDER||_exec_plan._phase_order==ALLRED_PHASEORDER )
    {
        for(int i=0 ; i<smem.cPedPushButton.GetDevCount(); i++)
        {

            printf("\n\nPed action over!!\n\n");
            dtat[0]=0xEA;
            dtat[1]=0x17;
            dtat[2]=0x00;

            _MsgOK= oDataToMessageOK.vPackageINFOTo92PEDProtocol(dtat,3,true,smem.cPedPushButton.GetDevID(i));
            //_MsgOK= oDataToMessageOK.vPackageINFOTo92PEDProtocol(dtat,3,true,0xFFFF);
            //writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICEPEDESTRIAN);
            smem.cPedPushButton.vPushWriteOutMsgToQueue(0,_MsgOK);
        }
        smem.SetPedlightcheck(false);
    }
    else
    {
    }
    if(_exec_phase_current_subphase == 1 && _exec_phase_current_subphase_step == 4 /*&& smem.GetPedlightcheck() == true*/)
    {
        for(int i=0 ; i<smem.cPedPushButton.GetDevCount(); i++)
        {

            printf("\n\nPed action over!!\n\n");
            dtat[0]=0xEA;
            dtat[1]=0x17;
            dtat[2]=0x00;

            _MsgOK= oDataToMessageOK.vPackageINFOTo92PEDProtocol(dtat,3,true,smem.cPedPushButton.GetDevID(i));
            //_MsgOK= oDataToMessageOK.vPackageINFOTo92PEDProtocol(dtat,3,true,0xFFFF);
            //writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICEPEDESTRIAN);
            smem.cPedPushButton.vPushWriteOutMsgToQueue(0,_MsgOK);
        }
        smem.SetPedlightcheck(false);
    }
    else
    {
    }
}
//------------------------------------------------------------------------------------------------------------------
void CSTC::CalculatePgCount(void) //OT20131221   //聯嘉 小綠人 using
{
    try
    {
        unsigned char ucSend[5];
        unsigned short int Data[8] = {0, 0, 0, 0, 0, 0, 0, 0};
        unsigned short int usiHCPGcountMapping[8] = {0, 0, 0, 0, 0, 0, 0, 0};

        bool bCountIF[8][5][8];   // 8subphase, 5 steps, 8output
        bool bCountIFEnable[8];   // 8output

        unsigned short int usiPhaseTmp;

        int iSubCnt;
        int iSignalCnt;
        int iSubCalCntTmp;
//no stepCnt, because only see step0

//OT20110624
        unsigned char ucRedCountRepeatCount = smem.vGetUCData(CSTC_RedcountRepeatCount);
        printf("in Pg-Send, ucRedCountRepeatCount:%d\n", ucRedCountRepeatCount);

//OT1000107
        unsigned char ucNextLightStatus[8];
        for(int i = 0; i < 8; i++)
        {
            ucNextLightStatus[i] = 255;
        }

        iSubCnt = _exec_phase._subphase_count;
        iSignalCnt = _exec_phase._signal_count;

//init. check val.
        for(int i = 0; i < 8; i++)
        {
            Data[i] = 0;
            bCountIFEnable[i] = true;   //[分相]
            for(int j = 0; j < 5; j++)
            {
                for(int k = 0; k < 8; k++)
                {
                    bCountIF[i][j][k] = false;  //[分相][步階][卡號]
                }
            }
        }

        for(int i = 0; i < iSignalCnt; i++)    //max 8 singal output (卡數)
        {
            iSubCalCntTmp = 0;
            for(int j = _exec_phase_current_subphase; iSubCalCntTmp < iSubCnt; j++)    //(分相)
            {
                if(j >= iSubCnt)
                {
                    j = 0;
                }
                for(int k = 0; k < 5; k++)    //check 5 steps. (步階)
                {
                    usiPhaseTmp = _exec_phase._ptr_subphase_step_signal_status[j][k][i] & 0xC000;  //[分相][步階][卡號]
                    if(usiPhaseTmp > 0)    //means Pg have light/flash
                    {
                        bCountIF[j][k][i] = true;
                    }
                    else if(_exec_phase_current_subphase_step >=2 )      //in Pr, Y, R light status
                    {
                        bCountIF[j][k][i] = true;
                    }
                    else
                    {
                        bCountIFEnable[i] = false;
                    }

                    if(bCountIFEnable[i] == true && bCountIF[j][k][i] == true)    //can add Pg Sec.
                    {
                        switch(k)
                        {
                        case(0):
                            Data[i] += _exec_plan._ptr_subplaninfo[j].compensated_green(_exec_plan._shorten_cycle);
                            break;
                        case(1):
                            Data[i] += _exec_plan._ptr_subplaninfo[j].compensated_pedgreen_flash(_exec_plan._shorten_cycle);
                            break;
                        case(2):
                            Data[i] += _exec_plan._ptr_subplaninfo[j].compensated_pedred(_exec_plan._shorten_cycle);
                            break;
                        case(3):
                            Data[i] += _exec_plan._ptr_subplaninfo[j].compensated_yellow(_exec_plan._shorten_cycle);
                            break;
                        case(4):
                            Data[i] += _exec_plan._ptr_subplaninfo[j].compensated_allred(_exec_plan._shorten_cycle);
                            break;
                        default:
                            break;
                        }
                    }
                }
                iSubCalCntTmp++;
            }
        }


//recheck
        if( _exec_plan._phase_order == FLASH_PHASEORDER ||
                _exec_plan._phase_order == FLASH_PHASEORDER_HSINCHU ||
                _exec_plan._phase_order == ALLRED_PHASEORDER ||
                _exec_plan._phase_order == FLASH_PHASEORDER_HSINCHU2 ||
                _current_strategy==STRATEGY_MANUAL ||
                _current_strategy==STRATEGY_ALLDYNAMIC ||
                _exec_plan._phase_order ==0xB1 || _exec_plan._phase_order ==0xB0)  //arwen++ 1001206
        {
            for(int i = 0; i < 8; i++)
            {
                Data[i] = 0;
            }
        }
        if( smem.vGetBOOLData(TC_SignalConflictError) == true)
        {
            printf("TC SignalConflictError, Redcount Display Disable!\n");
            for(int i = 0; i < 8; i++)
            {
                Data[i] = 0;
            }
        }

        //OT980420
        if(_exec_phase._ptr_subphase_step_signal_status[0][0][0] == 0x1004 ||
                _exec_phase._ptr_subphase_step_signal_status[0][0][1] == 0x1004 ||
                _exec_phase._ptr_subphase_step_signal_status[0][0][0] == 0x2008 ||
                _exec_phase._ptr_subphase_step_signal_status[0][0][1] == 0x2008 ||
                _exec_phase._ptr_subphase_step_signal_status[0][0][0] == 0x2002 ||
                _exec_phase._ptr_subphase_step_signal_status[0][0][1] == 0x2002    )
        {
            printf("Flash light!, no redcount\n");
            for(int i = 0; i < 8; i++)
            {
                Data[i] = 0;
            }
        }

        if(_exec_phase._subphase_count == 1 && _exec_phase._total_step_count == 1)
        {
            printf("special phase, no redcount\n");
            for(int i = 0; i < 8; i++)
            {
                Data[i] = 0;
            }
        }


        if( smem.vGetUCData(TC_Redcount_Display_Enable) == 0)
        {
            printf("\nTC Redcount Display Disable!\n");
            for(int i = 0; i < 8; i++)
            {
                Data[i] = 0;
            }
        }

        if(smem.vGetUCData(TC_TrainChainNOW) == 1)
        {
            printf("\nTrain coming, TC Redcount Display Disable!\n");
            for(int i = 0; i < 8; i++)
            {
                Data[i] = 0;
            }
        }

        for(int i = 0 ; i < 8; i++)
        {
            if( Data[i] > 255 )
            {
                Data[i] = 0;    //Data[i] only 1 Byte
            }
            if( Data[i] > 0)
            {
                Data[i] = Data[i]+1;    // add 1 sec
            }
        }

        printf("\n    ********  PgCountTimer: %3d %3d %3d %3d %3d %3d %3d %3d  ********\n\n", Data[0],Data[1],Data[2],Data[3], Data[4],Data[5],Data[6], Data[7]);

        //OT20131219
        for(int i = 0; i < 8; i ++)
        {
            if(smem.vGetWayMappingRedCount(i) == 0)
            {
                usiHCPGcountMapping[i] = Data[0] & 0xFF;
            }
            else if(smem.vGetWayMappingRedCount(i) == 1)
            {
                usiHCPGcountMapping[i] = Data[1] & 0xFF;
            }
            else if(smem.vGetWayMappingRedCount(i) == 2)
            {
                usiHCPGcountMapping[i] = Data[2] & 0xFF;
            }
            else if(smem.vGetWayMappingRedCount(i) == 3)
            {
                usiHCPGcountMapping[i] = Data[3] & 0xFF;
            }
            else if(smem.vGetWayMappingRedCount(i) == 4)
            {
                usiHCPGcountMapping[i] = Data[4] & 0xFF;
            }
            else if(smem.vGetWayMappingRedCount(i) == 5)
            {
                usiHCPGcountMapping[i] = Data[5] & 0xFF;
            }
            else if(smem.vGetWayMappingRedCount(i) == 6)
            {
                usiHCPGcountMapping[i] = Data[6] & 0xFF;
            }
            else if(smem.vGetWayMappingRedCount(i) == 7)
            {
                usiHCPGcountMapping[i] = Data[7] & 0xFF;
            }
            else
            {
                usiHCPGcountMapping[i] = 0;
            }
        }

//    if(_current_strategy == STRATEGY_MANUAL) {

//    if(smem.vGetUCData(TC92_ucControlStrategy) = STRATEGY_MANUAL){
        smem.GreenmanManual(bCountIF);
//    }

        smem.vSetPgRemainder(usiHCPGcountMapping);
    }
    catch(...) {}
}
//----------------------------------------------------------------------------------
void CSTC::CalculatePrCount(void) //jacky20141121   //聯嘉 小紅人 using
{
    try
    {
        unsigned char ucSend[5];
        unsigned short int PRData[8] = {0, 0, 0, 0, 0, 0, 0, 0};
        unsigned short int usiHCPRcountMapping[8] = {0, 0, 0, 0, 0, 0, 0, 0};

//    bool bCountIF[8][5][8];   // 8subphase, 5 steps, 8output
//    bool bCountIFEnable[8];   // 8output
        bool bPRCountIF[8][5][8];   // 8subphase, 5 steps, 8output
        bool bPRCountIFEnable[8];   // 8output
        bool bPRCountIFEnableTwo[8];

        unsigned short int usiPhaseTmp;

        int iSubCnt;
        int iSignalCnt;
        int iSubCalCntTmp;
//no stepCnt, because only see step0

//OT20110624
        unsigned char ucRedCountRepeatCount = smem.vGetUCData(CSTC_RedcountRepeatCount);
//    printf("in Pg-Send, ucRedCountRepeatCount:%d\n", ucRedCountRepeatCount);

//OT1000107
        unsigned char ucNextLightStatus[8];
        for(int i = 0; i < 8; i++)
        {
            ucNextLightStatus[i] = 255;
        }

        iSubCnt = _exec_phase._subphase_count;
        iSignalCnt = _exec_phase._signal_count;

//init. check val.
        for(int i = 0; i < 8; i++)
        {
//        bCountIFEnable[i] = true;   //[分相]
            PRData[i] = 0;  //jacky20141114
            bPRCountIFEnable[i] = true;
            bPRCountIFEnableTwo[i] = false;
            for(int j = 0; j < 5; j++)
            {
                for(int k = 0; k < 8; k++)
                {
//                bCountIF[i][j][k] = false;  //[分相][步階][卡號]
                    bPRCountIF[i][j][k] = false;  //jacky20141114
                }
            }
        }

        for(int i = 0; i < iSignalCnt; i++)    //max 8 singal output (卡數)
        {
            iSubCalCntTmp = 0;
            for(int j = _exec_phase_current_subphase; iSubCalCntTmp < iSubCnt; j++)    //(分相)
            {
                if(j >= iSubCnt)
                {
                    j = 0;
                }
                usiPhaseTmp = _exec_phase._ptr_subphase_step_signal_status[j][0][i] & 0x3000;  //[分相][步階][卡號]
                if(usiPhaseTmp > 0) bPRCountIFEnableTwo[i] = true;
                for(int k = 0; k < 5; k++)    //check 5 steps. (步階)
                {
                    usiPhaseTmp = _exec_phase._ptr_subphase_step_signal_status[j][k][i] & 0x3000;  //[分相][步階][卡號]
                    if(usiPhaseTmp > 0) bPRCountIF[j][k][i] = true;
                    if(bPRCountIFEnableTwo[i] == true)
                    {
                        usiPhaseTmp = _exec_phase._ptr_subphase_step_signal_status[j][k][i] & 0xC000;  //[分相][步階][卡號]
                        if(usiPhaseTmp > 0) bPRCountIFEnable[i] = false;
                    }

                    if(bPRCountIFEnable[i] == true && bPRCountIF[j][k][i] == true)
                    {
                        switch(k)
                        {
                        case(0):
                            PRData[i] += _exec_plan._ptr_subplaninfo[j].compensated_green(_exec_plan._shorten_cycle);
                            break;
                        case(1):
                            PRData[i] += _exec_plan._ptr_subplaninfo[j].compensated_pedgreen_flash(_exec_plan._shorten_cycle);
                            break;
                        case(2):
                            PRData[i] += _exec_plan._ptr_subplaninfo[j].compensated_pedred(_exec_plan._shorten_cycle);
                            break;
                        case(3):
                            PRData[i] += _exec_plan._ptr_subplaninfo[j].compensated_yellow(_exec_plan._shorten_cycle);
                            break;
                        case(4):
                            PRData[i] += _exec_plan._ptr_subplaninfo[j].compensated_allred(_exec_plan._shorten_cycle);
                            break;
                        default:
                            break;
                        }
                    }

                }
                iSubCalCntTmp++;
            }
        }


//recheck
        if( _exec_plan._phase_order == FLASH_PHASEORDER ||
                _exec_plan._phase_order == FLASH_PHASEORDER_HSINCHU ||
                _exec_plan._phase_order == ALLRED_PHASEORDER ||
                _exec_plan._phase_order == FLASH_PHASEORDER_HSINCHU2 ||
                _current_strategy==STRATEGY_MANUAL ||
                _current_strategy==STRATEGY_ALLDYNAMIC ||
                _exec_plan._phase_order ==0xB1 || _exec_plan._phase_order ==0xB0)  //arwen++ 1001206
        {
            for(int i = 0; i < 8; i++)
            {
                PRData[i] = 0;
            }
        }
        if( smem.vGetBOOLData(TC_SignalConflictError) == true)
        {
            printf("TC SignalConflictError, Redcount Display Disable!\n");
            for(int i = 0; i < 8; i++)
            {
                PRData[i] = 0;
            }
        }

        //OT980420
        if(_exec_phase._ptr_subphase_step_signal_status[0][0][0] == 0x1004 ||
                _exec_phase._ptr_subphase_step_signal_status[0][0][1] == 0x1004 ||
                _exec_phase._ptr_subphase_step_signal_status[0][0][0] == 0x2008 ||
                _exec_phase._ptr_subphase_step_signal_status[0][0][1] == 0x2008 ||
                _exec_phase._ptr_subphase_step_signal_status[0][0][0] == 0x2002 ||
                _exec_phase._ptr_subphase_step_signal_status[0][0][1] == 0x2002    )
        {
            printf("Flash light!, no redcount\n");
            for(int i = 0; i < 8; i++)
            {
                PRData[i] = 0;
            }
        }

        if(_exec_phase._subphase_count == 1 && _exec_phase._total_step_count == 1)
        {
            printf("special phase, no redcount\n");
            for(int i = 0; i < 8; i++)
            {
                PRData[i] = 0;
            }
        }


        if( smem.vGetUCData(TC_Redcount_Display_Enable) == 0)
        {
            printf("\nTC Redcount Display Disable!\n");
            for(int i = 0; i < 8; i++)
            {
                PRData[i] = 0;
            }
        }

        if(smem.vGetUCData(TC_TrainChainNOW) == 1)
        {
            printf("\nTrain coming, TC Redcount Display Disable!\n");
            for(int i = 0; i < 8; i++)
            {
                PRData[i] = 0;
            }
        }

        for(int i = 0 ; i < 8; i++)
        {
            if( PRData[i] > 255 )
            {
                PRData[i] = 0;
            }
            if( PRData[i] > 0 )
            {
                PRData[i] = PRData[i]+1;
            }
        }

        printf("\n    ********  PrCountTimer: %3d %3d %3d %3d %3d %3d %3d %3d  ********\n\n", PRData[0],PRData[1],PRData[2],PRData[3], PRData[4],PRData[5],PRData[6], PRData[7]);

        //OT20131219
        for(int i = 0; i < 8; i ++)
        {
            if(smem.vGetWayMappingRedCount(i) == 0)
            {
                usiHCPRcountMapping[i] = PRData[0] & 0xFF;
            }
            else if(smem.vGetWayMappingRedCount(i) == 1)
            {
                usiHCPRcountMapping[i] = PRData[1] & 0xFF;
            }
            else if(smem.vGetWayMappingRedCount(i) == 2)
            {
                usiHCPRcountMapping[i] = PRData[2] & 0xFF;
            }
            else if(smem.vGetWayMappingRedCount(i) == 3)
            {
                usiHCPRcountMapping[i] = PRData[3] & 0xFF;
            }
            else if(smem.vGetWayMappingRedCount(i) == 4)
            {
                usiHCPRcountMapping[i] = PRData[4] & 0xFF;
            }
            else if(smem.vGetWayMappingRedCount(i) == 5)
            {
                usiHCPRcountMapping[i] = PRData[5] & 0xFF;
            }
            else if(smem.vGetWayMappingRedCount(i) == 6)
            {
                usiHCPRcountMapping[i] = PRData[6] & 0xFF;
            }
            else if(smem.vGetWayMappingRedCount(i) == 7)
            {
                usiHCPRcountMapping[i] = PRData[7] & 0xFF;
            }
            else
            {
                usiHCPRcountMapping[i] = 0;
            }
        }
        smem.vSetPrRemainder(usiHCPRcountMapping);
    }
    catch(...) {}
}
//OT20140214
//----------------------------------------------------------
bool CSTC::vSetTODCurrentTime(unsigned short int usiTime)
{
    try
    {

        if(_exec_phase_current_subphase_step <= 2)
        {
            printf("change time Process\n\n\n");
            timer_gettime(_timer_plan,&_itimer_plan);
            _itimer_plan.it_value.tv_sec = usiTime;

            for(int ii = 0; ii < 4; ii++)
            {
                timer_settime(_timer_plan, 0, &_itimer_plan, NULL);

                _itimer_plan_WDT = _itimer_plan; /*OT_PLAN_WDT*/
                _itimer_plan_WDT.it_value.tv_sec += 2;
                if(timer_settime( _timer_plan_WDT, 0, &_itimer_plan_WDT, NULL ))
                {
                    printf("PlanWDT Settime Error!.\n");
                    exit( 1 );
                }
                timer_settime( _timer_redcount, 0, & _itimer_redcount, NULL );
            }
        }

        return 0;
    }
    catch (...) {}
}
//----------------------------------------------------------
bool CSTC::vGoToNextStepByTimer(void)
{
    try
    {

        if(_exec_phase_current_subphase_step <= 2)
        {
            timer_gettime(_timer_plan,&_itimer_plan);
            _itimer_plan.it_value.tv_sec = 0;
            _itimer_plan.it_value.tv_nsec = 100000;

            for(int ii = 0; ii < 1; ii++)
            {
                timer_settime(_timer_plan, 0, &_itimer_plan, NULL);

                _itimer_plan_WDT = _itimer_plan; /*OT_PLAN_WDT*/
                _itimer_plan_WDT.it_value.tv_nsec = 0;
                _itimer_plan_WDT.it_value.tv_sec = 1;
                _itimer_plan_WDT.it_value.tv_nsec = 950000;
                if(timer_settime( _timer_plan_WDT, 0, &_itimer_plan_WDT, NULL ))
                {
                    printf("PlanWDT Settime Error!.\n");
                    // exit( 1 );
                }
                timer_settime( _timer_redcount, 0, & _itimer_redcount, NULL );
            }
        }

        return 0;
    }
    catch (...) {}
}
//OT20131210
//----------------------------------------------------------
void CSTC::vReportBF01CCTProtocalSendKaikinStep(void)
{
    try
    {
        unsigned char data[64] = {0};
        /*mask for CECI fuck comm server*/
        /*20091110 come back*/
        /*test for tainan 20090226*  (--5f80)*/
        unsigned int data_length = 0;
        data[0] = 0xBF;
        data_length++;
        data[1] = 0x01;
        data_length++;

        data[2] = _exec_phase._subphase_count;
        data_length++;
        data[3] = _exec_phase_current_subphase +1;
        data_length++;
        data[4] = _exec_phase_current_subphase_step + 1;
        data_length++;

        data[5] = stc.vGetStepTime();
        data_length++;
        switch(_exec_phase_current_subphase_step)
        {
        case(0):
            data[5] += _exec_plan._ptr_subplaninfo[_exec_phase_current_subphase].compensated_pedgreen_flash(_exec_plan._shorten_cycle);
            data[5] += _exec_plan._ptr_subplaninfo[_exec_phase_current_subphase].compensated_pedred(_exec_plan._shorten_cycle);
            data[5] += _exec_plan._ptr_subplaninfo[_exec_phase_current_subphase].compensated_yellow(_exec_plan._shorten_cycle);
            data[5] += _exec_plan._ptr_subplaninfo[_exec_phase_current_subphase].compensated_allred(_exec_plan._shorten_cycle);
            break;
        case(1):
            data[5] += _exec_plan._ptr_subplaninfo[_exec_phase_current_subphase].compensated_pedred(_exec_plan._shorten_cycle);
            data[5] += _exec_plan._ptr_subplaninfo[_exec_phase_current_subphase].compensated_yellow(_exec_plan._shorten_cycle);
            data[5] += _exec_plan._ptr_subplaninfo[_exec_phase_current_subphase].compensated_allred(_exec_plan._shorten_cycle);
            break;
        case(2):
            data[5] += _exec_plan._ptr_subplaninfo[_exec_phase_current_subphase].compensated_yellow(_exec_plan._shorten_cycle);
            data[5] += _exec_plan._ptr_subplaninfo[_exec_phase_current_subphase].compensated_allred(_exec_plan._shorten_cycle);
            break;
        case(3):
            data[5] += _exec_plan._ptr_subplaninfo[_exec_phase_current_subphase].compensated_allred(_exec_plan._shorten_cycle);
            break;
        case(4):
            break;
        default:
            break;
        }

        for(int i = 0; i < _exec_phase._subphase_count; i++)
        {
            data[data_length] =  _exec_plan._ptr_subplaninfo[i].compensated_green(_exec_plan._shorten_cycle) +
                                 _exec_plan._ptr_subplaninfo[i].compensated_pedgreen_flash(_exec_plan._shorten_cycle) +
                                 _exec_plan._ptr_subplaninfo[i].compensated_pedred(_exec_plan._shorten_cycle);
            data_length++;
            data[data_length] = _exec_plan._ptr_subplaninfo[i].compensated_yellow(_exec_plan._shorten_cycle);
            data_length++;
            data[data_length] = _exec_plan._ptr_subplaninfo[i].compensated_allred(_exec_plan._shorten_cycle);
            data_length++;
        }

        MESSAGEOK _MsgOK;
        _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, data_length, true);
        _MsgOK.InnerOrOutWard = cOutWard;
        //5F80 don't resend
        writeJob.WritePhysicalOutNoSetSeqNoResend(_MsgOK.packet, _MsgOK.packetLength, DEVICEBRT);
        /**/

        /**/

    }
    catch (...) {}
}
//--------------jacky 20140314------json---------------------------------------------------
void CSTC::jsonPhase(int j_phaseorder, unsigned short int j_subphasecount)
{
    try
    {
        FILE *tmp;
        //printf("---------------------------------%d\n", jsonphaseorder);
        if(j_phaseorder == 0)
        {
            tmp = fopen("/cct/Data/jsondata/Phase.txt","w");
            if(tmp == NULL)
            {
                printf("open Phase.txt error\n");
            }
            else fprintf(tmp,"{\"phase_no\":\"%d\",\"phase_name\":\"%02X\",\"phase_total\":%d}",j_phaseorder,j_phaseorder,j_subphasecount);
        }
        else
        {
            tmp = fopen("/cct/Data/jsondata/Phase.txt","a");
            if(tmp == NULL)
            {
                printf("open Phase.txt error\n");
            }
            else fprintf(tmp,",{\"phase_no\":\"%d\",\"phase_name\":\"%02X\",\"phase_total\":%d}",j_phaseorder,j_phaseorder,j_subphasecount);
        }
        fclose(tmp);
    }
    catch (...) {}
}
//--------------jacky 20140314-------json--------------------------------------------------
void CSTC::jsonstd_plan_ID(unsigned short int j_plan, unsigned short int j_phaseorder, unsigned short int j_subphasecont,
                           unsigned short int j_cycletime, unsigned short int j_timeoffest)
{
    try
    {
        int j_LCN;
        FILE *tmp;

        j_LCN = smem.GetAddress();
        if(j_plan == 1)
        {
            tmp = fopen("/cct/Data/jsondata/std_plan.txt","w");
            if(tmp == NULL)
            {
                printf("open std_plan.txt error\n");
            }
            else fprintf(tmp,"{\"equip_id\":%d,\"plan_id\":%d,\"phase_no\":\"%d\",\"phase_name\":\"%02X\",\"phase_total\":%d,\"cycletime\":%d,\"time_offset\":%d"
                             ,j_LCN,j_plan,j_phaseorder,j_phaseorder,j_subphasecont,j_cycletime,j_timeoffest);
        }
        else
        {
            tmp = fopen("/cct/Data/jsondata/std_plan.txt","a");
            if(tmp == NULL)
            {
                printf("open std_plan.txt error\n");
            }
            else fprintf(tmp,",{\"equip_id\":%d,\"plan_id\":%d,\"phase_no\":\"%d\",\"phase_name\":\"%02X\",\"phase_total\":%d,\"cycletime\":%d,\"time_offset\":%d"
                             ,j_LCN,j_plan,j_phaseorder,j_phaseorder,j_subphasecont,j_cycletime,j_timeoffest);
        }
        fclose(tmp);
    }
    catch (...) {}
}
//---------------jacky 20140314---json-------------------------------------------
void CSTC::jsonstd_plan_light(unsigned short int j_allred, unsigned short int j_yellow, unsigned short int j_pgreen, unsigned short int j_pgflash,
                              unsigned short int j_pred, unsigned short int j_ming, unsigned short int j_maxg, int num)
{
    try
    {
        FILE *tmp;
        unsigned short int j_g;

        j_g = j_pgreen + (j_pgflash +j_pred);
        if(num != 8)
        {
            tmp = fopen("/cct/Data/jsondata/std_plan.txt","a");
            if(tmp == NULL)
            {
                printf("open std_plan.txt error\n");
            }
            else fprintf(tmp,",\"allred%d\":%d,\"yellow%d\":%d,\"g%d\":%d,\"pgreen%d\":%d,\"pgflash%d\":%d,\"pred%d\":%d,\"ming%d\":%d,\"maxg%d\":%d"
                             ,num,j_allred,num,j_yellow,num,j_g,num,j_pgreen,num,j_pgflash,num,j_pred,num,j_ming,num,j_maxg);
        }
        else
        {
            tmp = fopen("/cct/Data/jsondata/std_plan.txt","a");
            if(tmp == NULL)
            {
                printf("open std_plan.txt error\n");
            }
            else fprintf(tmp,",\"allred%d\":%d,\"yellow%d\":%d,\"g%d\":%d,\"pgreen%d\":%d,\"pgflash%d\":%d,\"pred%d\":%d,\"ming%d\":%d,\"maxg%d\":%d}"
                             ,num,j_allred,num,j_yellow,num,j_g,num,j_pgreen,num,j_pgflash,num,j_pred,num,j_ming,num,j_maxg);
        }
        fclose(tmp);
    }
    catch (...) {}
}
//---------------jacky 20140314---json-------------------------------------------
void CSTC::jsontod_plan(unsigned short int j_seqtype, unsigned short int j_hour, unsigned short int j_minute,
                        unsigned short int j_planid, unsigned short int j_priorityswitch)
{
    try
    {
        int j_LCN;
        FILE *tmp;
        char timebuf[5];

        j_LCN = smem.GetAddress();
        sprintf(timebuf,"%02d:%02d",j_hour,j_minute);
        if(j_seqtype==1 && strcmp (timebuf,"00:00")==0)
        {
            tmp = fopen("/cct/Data/jsondata/tod_plan.txt","w");
            if(tmp == NULL)
            {
                printf("open tod_plan.txt error\n");
            }
            else fprintf(tmp,"{\"equip_id\":%d,\"seg_type\":%d,\"begin_time\":\"%s\",\"plan_id\":%d,\"car_countdown\":%d,\"ped_countdown\":%d,\"priority_switch\":%d}"
                             ,j_LCN,j_seqtype,timebuf,j_planid,0,0,j_priorityswitch);
        }
        else
        {
            tmp = fopen("/cct/Data/jsondata/tod_plan.txt","a");
            if(tmp == NULL)
            {
                printf("open tod_plan.txt error\n");
            }
            else fprintf(tmp,",{\"equip_id\":%d,\"seg_type\":%d,\"begin_time\":\"%s\",\"plan_id\":%d,\"car_countdown\":%d,\"ped_countdown\":%d,\"priority_switch\":%d}"
                             ,j_LCN,j_seqtype,timebuf,j_planid,0,0,j_priorityswitch);
        }
        fclose(tmp);
    }
    catch (...) {}
}
//------------jacky 20140317---json---------------------------------------------
void CSTC::jsonequipment(void)
{
    try
    {
        int j_LCN;
        FILE *tmp;

        j_LCN = smem.GetAddress();
        tmp = fopen("/cct/Data/jsondata/equipment.txt","w");
        if(tmp == NULL)
        {
            printf("open equipment.txt error\n");
        }
        else fprintf(tmp,"{\"equip_id\":%d,\"intersection_id\":%d,\"type_id\":\"TC\",\"location\":%d}",j_LCN,j_LCN,j_LCN);
        fclose(tmp);
    }
    catch (...) {}
}
//------------jacky 20140318---json---------------------------------------------
void CSTC::jsonday_segtype_weekday(unsigned short int j_weekday, unsigned short int j_segtype)
{
    try
    {
        int j_LCN;
        FILE *tmp;

        j_LCN = smem.GetAddress();
        if (j_weekday == 1)
        {
            tmp = fopen("/cct/Data/jsondata/day_segtype.txt","w");
            if(tmp == NULL)
            {
                printf("open day_segtype.txt error\n");
            }
            else fprintf(tmp,"{\"equip_id\":%d,\"mon\":%d",j_LCN,j_segtype);
            fclose(tmp);
        }
        else
        {
            tmp = fopen("/cct/Data/jsondata/day_segtype.txt","a");
            if(tmp == NULL)
            {
                printf("open day_segtype.txt error\n");
            }
            else
            {
                switch (j_weekday)
                {
                case 2:
                    fprintf(tmp,",\"tue\":%d",j_segtype);
                    fclose(tmp);
                    break;
                case 3:
                    fprintf(tmp,",\"wed\":%d",j_segtype);
                    fclose(tmp);
                    break;
                case 4:
                    fprintf(tmp,",\"thu\":%d",j_segtype);
                    fclose(tmp);
                    break;
                case 5:
                    fprintf(tmp,",\"fri\":%d",j_segtype);
                    fclose(tmp);
                    break;
                case 6:
                    fprintf(tmp,",\"sat\":%d",j_segtype);
                    fclose(tmp);
                    break;
                case 7:
                    fprintf(tmp,",\"sun\":%d",j_segtype);
                    fclose(tmp);
                    break;
                case 11:
                    fprintf(tmp,",\"even_mon\":%d",j_segtype);
                    fclose(tmp);
                    break;
                case 12:
                    fprintf(tmp,",\"even_tue\":%d",j_segtype);
                    fclose(tmp);
                    break;
                case 13:
                    fprintf(tmp,",\"even_wed\":%d",j_segtype);
                    fclose(tmp);
                    break;
                case 14:
                    fprintf(tmp,",\"even_thu\":%d",j_segtype);
                    fclose(tmp);
                    break;
                case 15:
                    fprintf(tmp,",\"even_fri\":%d",j_segtype);
                    fclose(tmp);
                    break;
                case 16:
                    fprintf(tmp,",\"even_sat\":%d",j_segtype);
                    fclose(tmp);
                    break;
                case 17:
                    fprintf(tmp,",\"even_sun\":%d",j_segtype);
                    fclose(tmp);
                    break;
                }  //switch
            } //else
        } //else
    }
    catch (...) {}
}
//------------jacky 20140318---json---------------------------------------------
void CSTC::jsonday_segtype_holiday(unsigned short int j_hsegtype, unsigned short int j_syear, unsigned short int j_smonth,unsigned short int j_sday,
                                   unsigned short int j_eyear, unsigned short int j_emonth, unsigned short int j_eday)
{
    try
    {
        FILE *tmp;
        char jstartdate[10];
        char jenddate[10];

        sprintf(jstartdate,"%d-%02d-%02d",j_syear,j_smonth,j_sday);
        sprintf(jenddate,"%d-%02d-%02d",j_eyear,j_emonth,j_eday);
        tmp = fopen("/cct/Data/jsondata/day_segtype.txt","a");
        if(tmp == NULL)
        {
            printf("open day_segtype.txt error\n");
        }
        else
        {
            switch (j_hsegtype)
            {
            case 8:
                fprintf(tmp,",\"spc1_startdate\":\"%s\",\"spc1_enddate\":\"%s\",\"spc1\":%d",jstartdate,jenddate,j_hsegtype);
                fclose(tmp);
                break ;
            case 9:
                fprintf(tmp,",\"spc2_startdate\":\"%s\",\"spc2_enddate\":\"%s\",\"spc2\":%d",jstartdate,jenddate,j_hsegtype);
                fclose(tmp);
                break ;
            case 10:
                fprintf(tmp,",\"spc3_startdate\":\"%s\",\"spc3_enddate\":\"%s\",\"spc3\":%d",jstartdate,jenddate,j_hsegtype);
                fclose(tmp);
                break ;
            case 11:
                fprintf(tmp,",\"spc4_startdate\":\"%s\",\"spc4_enddate\":\"%s\",\"spc4\":%d",jstartdate,jenddate,j_hsegtype);
                fclose(tmp);
                break ;
            case 12:
                fprintf(tmp,",\"spc5_startdate\":\"%s\",\"spc5_enddate\":\"%s\",\"spc5\":%d",jstartdate,jenddate,j_hsegtype);
                fclose(tmp);
                break ;
            case 13:
                fprintf(tmp,",\"spc6_startdate\":\"%s\",\"spc6_enddate\":\"%s\",\"spc6\":%d",jstartdate,jenddate,j_hsegtype);
                fclose(tmp);
                break ;
            case 14:
                fprintf(tmp,",\"spc7_startdate\":\"%s\",\"spc7_enddate\":\"%s\",\"spc7\":%d",jstartdate,jenddate,j_hsegtype);
                fclose(tmp);
                break ;
            case 15:
                fprintf(tmp,",\"spc8_startdate\":\"%s\",\"spc8_enddate\":\"%s\",\"spc8\":%d",jstartdate,jenddate,j_hsegtype);
                fclose(tmp);
                break ;
            case 16:
                fprintf(tmp,",\"spc9_startdate\":\"%s\",\"spc9_enddate\":\"%s\",\"spc9\":%d",jstartdate,jenddate,j_hsegtype);
                fclose(tmp);
                break ;
            case 17:
                fprintf(tmp,",\"spc10_startdate\":\"%s\",\"spc10_enddate\":\"%s\",\"spc10\":%d",jstartdate,jenddate,j_hsegtype);
                fclose(tmp);
                break ;
            case 18:
                fprintf(tmp,",\"spc11_startdate\":\"%s\",\"spc11_enddate\":\"%s\",\"spc11\":%d",jstartdate,jenddate,j_hsegtype);
                fclose(tmp);
                break ;
            case 19:
                fprintf(tmp,",\"spc12_startdate\":\"%s\",\"spc12_enddate\":\"%s\",\"spc12\":%d",jstartdate,jenddate,j_hsegtype);
                fclose(tmp);
                break ;
            case 20:
                fprintf(tmp,",\"spc13_startdate\":\"%s\",\"spc13_enddate\":\"%s\",\"spc13\":%d}",jstartdate,jenddate,j_hsegtype);
                fclose(tmp);
                break ;

            }
        }
    }
    catch (...) {}
}
//------------jacky 20141230---json---------------------------------------------
void CSTC::jsonPriority_Parameters(unsigned short int j_priority, unsigned short int j_pasteast, unsigned short int j_pastwest,
                                   unsigned short int j_doortriggerup, unsigned short int j_doortriggerdown, unsigned short int j_headwayup,
                                   unsigned short int j_headwqaydown, unsigned short int j_lowspeed)
{
    try
    {
        int j_LCN;
        FILE *tmp;

        j_LCN = smem.GetAddress();
        if(j_priority == 1)
        {
            tmp = fopen("/cct/Data/jsondata/priority.txt","w");
            if(tmp == NULL)
            {
                printf("open priority.txt error\n");
            }
            else fprintf(tmp,"{\"equip_id\":%d,\"priority_id\":%d,\"past_east\":\"%02x\",\"past_west\":\"%02x\",\"door_trigger_up\":%d,\"door_trigger_down\":%d,\"headway_up\":%d,\"headway_down\":%d,\"lowspeed\":%d"
                             ,j_LCN,j_priority,j_pasteast,j_pastwest,j_doortriggerup,j_doortriggerdown,j_headwayup,j_headwqaydown,j_lowspeed);
        }
        else
        {
            tmp = fopen("/cct/Data/jsondata/priority.txt","a");
            if(tmp == NULL)
            {
                printf("open priority.txt error\n");
            }
            else fprintf(tmp,",{\"equip_id\":%d,\"priority_id\":%d,\"past_east\":\"%02x\",\"past_west\":\"%02x\",\"door_trigger_up\":%d,\"door_trigger_down\":%d,\"headway_up\":%d,\"headway_down\":%d,\"lowspeed\":%d"
                             ,j_LCN,j_priority,j_pasteast,j_pastwest,j_doortriggerup,j_doortriggerdown,j_headwayup,j_headwqaydown,j_lowspeed);
        }
        fclose(tmp);
    }
    catch (...) {}
}
//------------jacky 20141230---json---------------------------------------------
void CSTC::jsonPriority_Percentage(unsigned short int dir,unsigned short int percentage,unsigned short int num)
{
    try
    {
        FILE *tmp;

        tmp = fopen("/cct/Data/jsondata/priority.txt","a");
        if(tmp == NULL)
        {
            printf("open priority.txt error\n");
        }
        else if(dir == 0)
        {
            fprintf(tmp,",\"percentage_east%d\":%d",num,percentage);
        }
        else if(dir == 1)
        {
            if(num != 8)
                fprintf(tmp,",\"percentage_west%d\":%d",num,percentage);
            else
                fprintf(tmp,",\"percentage_west%d\":%d}",num,percentage);
        }
        fclose(tmp);
    }
    catch (...) {}
}
//-------------------------------------------------------------------------------------------------------
void  CSTC::CheckDynSegAndSendNextPlanId()
{
    Lock_to_Determine_SegmentPlanPhase_DynSeg();
    vCCJTOMSendTODNextPlanID();
}
CPhaseInfo CSTC::Get_stc_phase(int phase_num)
{
    try
    {

        return phase[phase_num];
    }
    catch(...) {}
}
CPlanInfo CSTC:: Get_stc_plan(int plan_num)
{
    try
    {
        return plan[plan_num];
    }
    catch(...)
    {
        printf("Get_stc_plan fault");


    }

}
CSegmentInfo CSTC::Get_stc_segment(int segment_num)
{
    try
    {
        return segment[segment_num];
    }
    catch(...) {}
}

CWeekDaySegType CSTC::Get_stc_weekdayseg(int weekdayseg_num)
{
    try
    {
        return weekdayseg[weekdayseg_num];
    }

    catch(...) {}
}
CHoliDaySegType CSTC::Get_stc_holidayseg(int holidayseg_num)
{
    try
    {
        return holidayseg[holidayseg_num];

    }
    catch(...) {}

}
CReverseTimeInfo  CSTC::  Get_stc_reversetime(int reversetime_num)
{
    try{
    return reversetime[reversetime_num];
    }catch(...){}
}
 CWeekDayRevType CSTC:: Get_stc_weekdayrev(int weekdayrev_num) //{0-6,7-13} according to {1-7,11-17}
 {
     try
     {
         return weekdayrev[weekdayrev_num];
     }catch(...){}
 }
 CHoliDayRevType CSTC::Get_stc_holidayrev(int holidayrev_num) //{0-12} according to {4-16}
 {
     try
     {
         return holidayrev[holidayrev_num];
     }catch(...){}
 }
unsigned short int CSTC::Get_exec_reversetime_current_rev_step()
{
    return _exec_reversetime_current_rev_step;
}

ControlStrategy CSTC::Get_current_stratage()
{
    return _current_strategy;
}
ControlStrategy CSTC::Get_old_stratage()
{
    return _old_strategy;
}
