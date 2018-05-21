#ifndef CSTC_H
#define CSTC_H

#include <fstream>

#include "CReverseTime.h"
#include "CSegmentInfo.h"
#include "CPhaseInfo.h"
#include "CPlanInfo.h"
#include "CIOCom.h"
#include "CPacketCluster.h"

#include "CRTMSDecoder.h"
#include "CRTMSInformation.h"
#include "CTrafficAnalyzer.h"
//jacky20141209
#include "PTRAFFIC92COMM.h"

#define LIGHT_IOPORT 0x3BC

//#define DEFAULT_LCN           47
// #define DEFAULT_PHASEORDER  0x00
#define DEFAULT_PHASEORDER  0x80
#define DEFAULT_PLANID      48
#define DEFAULT_SEGMENTTYPE  0
#define DEFAULT_REVERSETIMETYPE  0

#define DEFAULT_PLAN_GREEN_TIME   20
#define DEFAULT_PLAN_PGFLASH_TIME  2
#define DEFAULT_PLAN_YELLOW_TIME   2
#define DEFAULT_PLAN_ALLRED_TIME   2

#define SIGNAL_RECORD_TRAFFIC       59    //Using this signal to reboot TCC
#define SIGNAL_TRAFFIC_CHANGED      60
#define SIGNAL_STRATEGY_CHANGED     61
#define SIGNAL_NEXT_STEP            62
#define SIGNAL_TIMER                63

#define SIGNAL_CONGESTED_threshold  57
// #define SIGNAL_D_threshold  56
#define SIGNAL_D_threshold  51
#define SIGNAL_C_threshold  55
#define SIGNAL_B_threshold  54

#define REDCOUNT_TIMEOUT       3
#define LIGHT_TIMEOUT          99
#define REPORT_TIMEOUT         10
#define RECORD_TRAFFIC_TIMEOUT 86400
#define REVERSETIME_TIMEOUT    3                                                //after 3 sec, start reverse check

#define AMOUNT_PHASEORDER  256
#define AMOUNT_PLANID      256
#define AMOUNT_SEGMENT      21  //{1-7,8-20}
#define AMOUNT_WEEKDAY_SEG  14  //{1-7}: {1-7,11-17}
#define AMOUNT_HOLIDAY_SEG  13  //{8-20}

#define AMOUNT_REVERSETIME  17  //{1-3, 4-16}
#define AMOUNT_WEEKDAY_REV  14  //{1-7}: {1-7,11-17}
#define AMOUNT_HOLIDAY_REV  13  //{4-16}                                        //not for v92, more

#define FLASH_PLANID       128  //0x80
#define FLASH_PHASEORDER  0x80
#define ALLRED_PLANID      255  //0xFF
#define ALLRED_PHASEORDER 0xFF

//OT Debug 0523
#define FLASH_PHASEORDER_HSINCHU  0x80
#define FLASH_PHASEORDER_HSINCHU2  0x80

#define CADC_PLANID  0
#define CADC_INIT_OFFSET 15
#define ARTIERAL_PHASENO 0


#define RESET_TIMER_BEFORE_SET_LIGHT  true  //true:  timer_settime() invoked before light.setlight()
                                            //false: light.setlight() first, and then
                                            //       this might accumulating cycle time with the timer invoking light.setlight()
                                            //       meanwhile, it's considered as more reliable

/*
#define SET_COMPENSATE_BASE_TO_SEGMENT true  //true:  set compensation caculated base from start of current segment (tainan)
                                              //false: set compensation caculated base from 00:00 (hsinchu)
*/

#define MOUNTFILE "/proc/mounts"
#define MOUNTDIR  "/mnt/usb"  //caution: not exceed 15 characters

using namespace std;
//----------------------------------------------------------
enum ControlStrategy{
  STRATEGY_TOD       =10,
  STRATEGY_AUTO_CADC =40,  //STRATEGY_AUTO_CADC is triggered from TOD
  STRATEGY_CADC      =50,  //STRATEGY_CADC is triggered from outter
  STRATEGY_MANUAL    =70,
  STRATEGY_FLASH     =80,
  STRATEGY_ALLRED    =90,
  STRATEGY_ALLDYNAMIC =95,
  STRATEGY_TODDYN    =96
};



//----------------------------------------------------------
struct bytebcd {
  unsigned char b1:1;
  unsigned char b2:1;
  unsigned char b3:1;
  unsigned char b4:1;
  unsigned char b5:1;
  unsigned char b6:1;
  unsigned char b7:1;
  unsigned char b8:1;
};

union byteunion{
  bytebcd block;
  unsigned char byte;
};

/* ¤U´å¸ô¤fÂà¶Çªº­ÈFor RTMS */
class Down_crossing
{
  public:
  Down_crossing(void){
  current_step = 254;
    };
//Down_crossing(void);

  short int protocol_type;
  unsigned short int lcn;
  unsigned char current_phase;
  unsigned short int total_step;
  unsigned short int current_step;
  unsigned short int left_time;
  unsigned char mode;
  unsigned short int type;
  unsigned short int segment;
  unsigned char fault;
};


//----------------------------------------------------------
class CSTC
{
  static pthread_mutex_t _stc_mutex;
  static pthread_mutex_t _record_traffic_mutex;

  static void *_stc_thread_light_control_func(void *);

  //for timers used in _stc_thread_light_control
  static timer_t _timer_plan, _timer_redcount, _timer_panelcount, _timer_reportcount, _timer_record_traffic, _timer_plan_WDT;
  static timer_t _timer_reversetime;
  static struct itimerspec _itimer_plan, _itimer_redcount, _itimer_panelcount, _itimer_reportcount, _itimer_record_traffic, _itimer_plan_WDT;
  static struct itimerspec _itimer_reversetime;
  static void TimersCreating(void);
  static void TimersSetting(void);

  static CPhaseInfo      phase[AMOUNT_PHASEORDER];
  static CPlanInfo       plan[AMOUNT_PLANID];
  static CSegmentInfo    segment[AMOUNT_SEGMENT];
  static CWeekDaySegType weekdayseg[AMOUNT_WEEKDAY_SEG]; //{0-6,7-13} according to {1-7,11-17}
  static CHoliDaySegType holidayseg[AMOUNT_HOLIDAY_SEG]; //{0-12} according to {8-20}
  static CPlanInfo       BRTplan[AMOUNT_PLANID];    //jacky20140423
  static CSegmentInfo    BRTsegment[AMOUNT_SEGMENT]; //jacky20140423


  static CSegmentInfo    new_data_segment[AMOUNT_SEGMENT];


  static CReverseTimeInfo    reversetime[AMOUNT_REVERSETIME];
  static CWeekDayRevType weekdayrev[AMOUNT_WEEKDAY_REV]; //{0-6,7-13} according to {1-7,11-17}
  static CHoliDayRevType holidayrev[AMOUNT_HOLIDAY_REV]; //{0-12} according to {4-16}

  static unsigned short int LCN;
  static unsigned short int _default_phaseorder;

  static CPhaseInfo   _exec_phase;
  static CPlanInfo    _exec_plan;
  static CPlanInfo    _exec_BRTPriority_plan;   //jacky20140514
  static CSegmentInfo _exec_segment;
  static CSegmentInfo _exec_BRTPriority_segment; //jacky20140423
  static CReverseTimeInfo _exec_rev;

  static unsigned short int _exec_phase_current_subphase;  //start from 0
  static unsigned short int _exec_phase_current_subphase_step;  //start from 0
  static unsigned short int _exec_segment_current_seg_no;  //start from 0
  static unsigned short int _exec_reversetime_current_rev_no;  //start from 0
  static unsigned short int _exec_reversetime_current_rev_step;  //start from 0

  static ControlStrategy _current_strategy;
  static ControlStrategy _old_strategy;

  static Down_crossing down_crossing_STC;

  static bool ReadDefaultLCNPhaseOrder(void);
  static void ReadPhaseData(void);

//OT20110629
  static void vCheckPhase80beFlash(void);

  static void ReadPlanData(void);
  static void ReadSegmentData(void);
  static void ReadReverseTimeTypeData(void);
  static void ReadActCtrlSegment(void);//arwen 1001006 read actmode
  static void ReadPrioritySegment(void); //jacky 20140418
  static void ReadPriorityParameter(void); //jacky 20140421

  static void SetFlashAllRedPhaseInfo(void);
  static void SetFlashAllRedPlanInfo(void);
  static void SetDefaultPlanSegmentData(void);

  static bool Lock_to_Load_Phase(CPhaseInfo &lphase, const unsigned short int &phase_order);
  static bool Lock_to_Save_Phase(const CPhaseInfo &sphase);                     //OT 940622
  //OT 940622
  static bool Lock_to_Reset_Phase(CPhaseInfo &lphase, const unsigned short int &phase_order, const unsigned short int &subphase_count, const unsigned short int &signal_count);

  static bool Lock_to_Load_Plan(CPlanInfo &lplan, const unsigned short int &planid);
  static bool Lock_to_Load_PriorityParameter(CPlanInfo &lplan, const unsigned short int &planid); //jacky20140418
  static bool Lock_to_Save_Plan(const CPlanInfo &splan);
  static bool Lock_to_Save_PriorityParameter(const CPlanInfo &splan); //jacky20140418
  static bool Lock_to_Reset_Plan(CPlanInfo &lplan, const unsigned short int &planid, const unsigned short int &subphase_count);
  static bool Lock_to_Reset_PriorityParameter(CPlanInfo &lplan, const unsigned short int &planid, const unsigned short int &count); //jacky20140418
//  static bool _for_center_plan_completed, _panel_plan_completed;

  static bool Lock_to_Load_Segment(CSegmentInfo &lsegment, const unsigned short int &segment_type);
  static bool Lock_to_Load_PrioritySegment(CSegmentInfo &lsegment, const unsigned short int &segment_type); //jacky20140418
//OT20110517  static void Lock_to_Save_Segment(const CSegmentInfo &ssegment);
  static void Lock_to_Save_Segment(CSegmentInfo &ssegment);
  static void Lock_to_Save_Segment_for_Priority(CSegmentInfo &ssegment); //jacky20140418
  static bool Lock_to_Reset_Segment(CSegmentInfo &lsegment, const unsigned short int &segment_type, const unsigned short int &segment_count);

  static void Lock_to_Save_HoliDaySegment(const CHoliDaySegType &sholidaysegtype);

  static void Lock_to_Determine_SegmentPlanPhase(void);
  //OT20140414
  static void Lock_to_Determine_SegmentPlanPhase_DynSeg(void);

  static unsigned short int vStartReverseLaneInStep0(time_t, int *);
  static unsigned short int vStartReverseLane(time_t, int *);
  static bool vSetReverseTimer(int);
  static bool vDetermine_ReverseTime(void);

  static void ReSetStep(bool step_up);
  static void ReSetExtendTimer(void);
//  static void SetLightAfterExtendTimerReSet(void);

  //jacky20140515
  static unsigned char * BF02_Extendsec(unsigned short int);
  //static void BF02_Reduce(unsigned short int);
  //jacky20140520
  static void BF02_PriorityParameters(void);

//move to public  static void CalculateAndSendRedCount(const short int diff);

  //OT Debug 950816
  static void vCalculateAndSendPeopleLightCount(void);

  static unsigned short int CalculateCompensationBase(void);
  static unsigned short int CalculateCompensationBase_twocycle(void);  //jacky20140407 兩個cycle完成補償
  static void CalculateCompensation_in_TOD(void);
  static void CalculateCompensation_in_CADC(void);
  static void AdjustOffset_of_CurrentCycle_in_CADC(const short int &adjust_offset=0);

//OTCombo
  static void ReportCurrentControlStrategy(int);  //called by REPORT_TIMEOUT
//move to public  static void ReportCurrentOperationMode(void);  //called by REPORT_TIMEOUT
  static void ReportCurrentHardwareStatus(void);  //called by REPORT_TIMEOUT
  static timespec strategy_start_time;

  static CIOCom _detector_io;
  static SBuffer _buffer;    // store the read buffer
  static CPacketCluster _packet_c;  // store the read packets
//  static CRTMSDecoder _detector_decoder;
//  static CRTMSInformation _rtms_info;
  static void ReportLastPeriodTraffic(void);
  static void ReportHDDRecoding(const unsigned char &status);
  static void ReportAnalyzedTrafficStatus(const unsigned char &status);

  static bool recording_traffic;

  static ofstream rawfile;
  static ofstream refinedfile;
  static ofstream trafficfile;
  static ofstream targetfile;

  static unsigned long int inrecordno;
  static unsigned long int inperiodno;
  static bool PlanUpdate;  //OTBUG =1
  static bool SegmentTypeUpdate;
  static bool ReverseTimeDataUpdate;

  static unsigned short int OLD_TOD_PLAN_ID;
  static unsigned short int NEW_TOD_PLAN_ID;

  /*otaru0514--*/
  static bool bRefreshLight;

  static unsigned short int CalculateCompensationBaseInChain(void);
  static void vCalculateCompensation_in_CHAIN(void);

  static bool bJumpSubEnable;
  static unsigned char ucJumpSubPhase;
  static unsigned short int usiTrainComingSec;
  static unsigned short int usiLockPhaseSec;

  static bool m_segmentsave;
  static int m_segmentCount;

  //OT20140415
  static int iDynSeg_SegType;
  static int iDynSeg_SegCount;
  static int iDynSeg_PlanID;

  static unsigned short int m_segment_lastActMode;//arewn ++ 1001007

  //jacky20140514
//  static DATA_Bit east;
//  static DATA_Bit west;

  public:
    static pthread_t _stc_thread_detector_info;
    static pthread_t _stc_thread_light_control;

    static CTrafficAnalyzer traffic_analyzer;

    CSTC(void);
    static void ReadAllData(void);
    static bool AllRed5Seconds(void);

    static bool SetDefaultLCNPhaseOrder(unsigned short int lcn, unsigned short int phase_order);

    static void ThreadsGenerate();  //argc, argv is the parameter of main

    static void SetLightAfterExtendTimerReSet(void);  //jacky20141202
//    static void vShutdownLearningSec(void); //jacky20150119

    static CPhaseInfo   _panel_phase,   _for_center_phase;
    static CPlanInfo    _panel_plan,    _for_center_plan;
    static CPlanInfo    _for_BRT_plan; //jacky20140418
    static CSegmentInfo _panel_segment, _for_center_segment, _for_web_segment;//arwen++
    static CSegmentInfo _for_BRT_segment; //jacky20140418
    static CWeekDaySegType _panel_weekdayseg[AMOUNT_WEEKDAY_SEG];
    static CWeekDaySegType _for_center_weekdayseg[AMOUNT_WEEKDAY_SEG];
    static CWeekDaySegType _for_web_weekdayseg[AMOUNT_WEEKDAY_SEG];//arwen++
    static CHoliDaySegType _panel_holidayseg, _for_center_holidayseg;

    static CReverseTimeInfo _panel_reversetime, _for_center_reversetime;
    static CWeekDayRevType _panel_weekdayrev[AMOUNT_WEEKDAY_REV];
    static CWeekDayRevType _for_center_weekdayrev[AMOUNT_WEEKDAY_REV];
    static CHoliDayRevType _panel_holidayrev, _for_center_holidayrev;

    static Down_crossing down_crossing_from_DC;

    static bool TimersRead_BeforeResetCMOSTime(void);
    static bool TimersReset_AfterResetCMOSTime(void);

    static bool Lock_to_SaveDefaultLCNPhaseOrder(const unsigned short lcn=47, const unsigned short phaseorder=0x00);
    static int  Lock_to_LoadLCN(void){ return LCN;}
    static ControlStrategy Lock_to_LoadControlStrategy(void){ return _current_strategy;}

    //phase_order={0x00-0xFF}, planid={0-48}
    static bool Lock_to_Load_Phase_for_Panel (const unsigned short int &phase_order);
    static bool Lock_to_Load_Phase_for_Center(const unsigned short int &phase_order);
    static bool Lock_to_Load_Plan_for_Panel (const unsigned short int &planid);
    static bool Lock_to_Load_Plan_for_Center(const unsigned short int &planid);
    static bool Lock_to_Load_Plan_for_BRT(const unsigned short int &planid); //jacky20140418

    //segment_type={0-20}, weekday={1-7,11-17}, holiday_segment={8-20}
    static bool Lock_to_Load_Segment_for_Panel (const unsigned short int &segment_type);
    static bool Lock_to_Load_Segment_for_Center(const unsigned short int &segment_type);
    static bool Lock_to_Load_Segment_for_BRT(const unsigned short int &segment_type); //jacky20140418
    static bool Lock_to_Load_Segment_for_Panel_inWeekDay (const unsigned short int &weekday);
    static bool Lock_to_Load_Segment_for_Center_inWeekDay(const unsigned short int &weekday);

    static bool Lock_to_Load_WeekDaySegment_for_Panel (void);
    static bool Lock_to_Load_WeekDaySegment_for_Center(void);
    static bool Lock_to_Load_WeekDaySegment_for_Web(void);//arwen++
    static bool Lock_to_Load_WeekDaySegment_for_BRT(void); //jacky20140418
    static bool Lock_to_Load_HoliDaySegment_for_Panel (const unsigned short int &holiday_segment_type);
    static bool Lock_to_Load_HoliDaySegment_for_Center(const unsigned short int &holiday_segment_type);

    static bool Lock_to_Load_Current_Segment_for_Panel(void);
    static bool Lock_to_Load_Current_Plan_for_Panel(void);

    static bool Lock_to_Reset_Segment_for_Panel (const unsigned short int &segment_type, const unsigned short int &segment_count);
    static bool Lock_to_Reset_Segment_for_Center(const unsigned short int &segment_type, const unsigned short int &segment_count);
    static bool Lock_to_Reset_Segment_for_Web(const unsigned short int &segment_type, const unsigned short int &segment_count);//arwen ++
    static bool Lock_to_Reset_Segment_for_BRT(const unsigned short int &segment_type, const unsigned short int &segment_count); //jacky20140418
    //OT940622
    static bool Lock_to_Reset_Phase_for_Panel (const unsigned short int &phase_order, const unsigned short int &subphase_count, const unsigned short int &signal_count);
    static bool Lock_to_Reset_Phase_for_Center(const unsigned short int &phase_order, const unsigned short int &subphase_count, const unsigned short int &signal_count);

    static bool Lock_to_Reset_Plan_for_Panel (const unsigned short int &planid, const unsigned short int &subphase_count);
    static bool Lock_to_Reset_Plan_for_Center(const unsigned short int &planid, const unsigned short int &subphase_count);
    static bool Lock_to_Reset_Plan_for_BRT(const unsigned short int &planid, const unsigned short int &count);
    static bool Check_Editing_Plan_for_Panel (const unsigned short int &planid, const unsigned short int &subphase_count);
    static bool Check_Editing_Plan_for_Center(const unsigned short int &planid, const unsigned short int &subphase_count);

    static bool Lock_to_Save_Phase_from_Panel(void);
    static bool Lock_to_Save_Phase_from_Center(void);
    static bool Lock_to_Save_Plan_from_Panel(void);
    static bool Lock_to_Save_Plan_from_Center(void);
    static bool Lock_to_Save_Plan_from_BRT(void); //jacky20140418

    static void Lock_to_Save_Segment_from_Panel(void);
    static void Lock_to_Save_Segment_from_Center(void);
    static void Lock_to_Save_Segment_from_Web(void);
    static void Lock_to_Save_Segment_from_BRT(void); //jacky20140418
    static void Lock_to_Save_WeekDaySegment_from_Panel(void);
    static void Lock_to_Save_WeekDaySegment_from_Center(void);
    static void Lock_to_Save_WeekDaySegment_from_Web(void);//arwen++
    static void Lock_to_Save_HoliDaySegment_from_Panel(void);
    static void Lock_to_Save_HoliDaySegment_from_Center(void);

    static void Lock_to_Save_WeekDayReversetime_from_Panel(void);
    static void Lock_to_Save_WeekDayReversetime_from_Center(void);
//    static void Lock_to_Save_HoliDayReversetime_from_Panel(void);
//    static void Lock_to_Save_HoliDayReversetime_from_Center(void);

    static bool Lock_to_Load_WeekDayReverseTime_for_Panel (void);
    static bool Lock_to_Load_WeekDayReverseTime_for_Center (void);
    static bool Lock_to_Load_HoliDayReverseTime_for_Panel (const unsigned short int &holiday_reversetime);
    static bool Lock_to_Load_HoliDayReverseTime_for_Center(const unsigned short int &holiday_reversetime);
    static void Lock_to_Save_ReverseTime_Step1(int);                            // can select from panel or center, 0 = center, 1 = panel
    static void Lock_to_Save_ReverseTime_Step2(const CReverseTimeInfo &srev);
    static void Lock_to_Save_HoliDayReverseTime_Step1(int);                     // can select from panel or center
    static void Lock_to_Save_HolidayReverseTime_Step2(const CHoliDayRevType &sholidayrev);


    static bool Lock_to_Load_ReverseTime_Step1(const unsigned short int &usiSelectSource1, const unsigned short int &usiSelectSource2);
    static bool Lock_to_Load_ReverseTime_Step2(CReverseTimeInfo &lrev, const unsigned short int &rev_data);

    static bool Lock_to_Set_Control_Strategy(const ControlStrategy new_cs);
    static bool Lock_to_Set_Next_Step(void);

    static void TestMode_Modify_CADC_offset(int offset=0);

    bool Lock_to_Save_DownCrossing(void);

    //OTCombo
    unsigned short int vGetUSIData(int);

    static void BF02_Reduce(unsigned short int);

    static void ReportCurrentSignalMap_5F0F(void);
    static void ReportCurrentStepStatus(void);  //called by REPORT_TIMEOUT and ReSetStepExtendTimerReSetStepExtendTimer()
    static void ReportCurrentStepStatusCCT5F04(void);

    bool vLockToSetControlStrategyToManualNotBySignal();
    bool vLockToSetControlStrategyToTODNotBySignal();

    bool vReportFieldOperate_5F08(int);

    unsigned short int vGetStepTime(void);
    bool vReportGoToNextPhaseStep_5F0C(void);

    void vReport5F1CWorkingStepChange(void);
    static void vReport5F80CCTProtocalSendStep(void);

    static void vResetReverseTime(void);

    unsigned short int vGetStepTimeUntilNextGreen(void);
    unsigned short int vGetStepTimeUntilNextCycle(void);
    bool vCompareNextSubphase(unsigned short int);
    bool vReportCCJPhaseStep_5F7C(void);

    bool vSendControlStrategyToCCJ(void);

    bool vSetNextStepNotUseSignal(void);

    bool vGetLCX405ControlPower(void);
    static bool vSend0x16ToUnlockLCX405(void);

    static bool bCSTCInitialOK;

    static bool vCheckPhaseForTFDActuateExtendTime_5FCF(void);
    static bool vActuateExtendTODTime(unsigned char, unsigned char);
    static bool vActuateChangeTODTimeForCCJTOMNanLu(unsigned char, unsigned char);
    static bool vExtendGreenTime(unsigned char,bool);        //----20120927 chou for 5F1C
    static bool vChangeTODCurrentStepSec(unsigned char, unsigned char);
    static bool vSendUpdateSegmentPlanInfoToCCJ_5F9D(void);

    static void ReportCurrentStepStatus5F03toCCJ(void);
    static void vCCJTOMSendActuatePhaseInfo_5FA0(void);
    static void vCCJTOMSendStartStopInfo(unsigned char);
    static void vCCJTOMSendPhaseInfo(void);
    static void vCCJTOMSendTODNextPlanID(void);

    static void BRTImmediateReport(void);  //jacky20140327
    static void BRTGPSStatusReport(void);   //jacky20141203
    static void ClearBF0B(unsigned char);

    void vTrainComing(void);
    void vTrainLeaving(void);

    bool vSetRevStep(unsigned char);
    unsigned char vGetRevStep(void);
    bool vLightRevSingal(void);

    bool vSendHeartBeatToLCX405inCSTC(void);

    static void CalculateAndSendRedCount(const short int diff);
    static void CalculateAndSendRedCountInDyn(short int diff);
    //-----------------------------------------------------------------------------------------------
    static void CalculateAndSendGreenmanCount(const short int diff);    //2013/05/17 chou++ greenman
    static void CalculateAndSendNSPredCount(const short int diff);      //2013/05/23 chou++ redcount
    //-----------------------------------------------------------------------------------------------
    static void ReportCurrentOperationMode_5F08(void);  //called by REPORT_TIMEOUT
    bool vReportRevStatus5F02(unsigned char);

    short int vGetAdjCurrentMaxGreen(void);
    short int vGetAdjCurrentMinGreen(void);
    short int vGetAdjCurrentPhaseGreenTime(void);

    unsigned short int vGetTODRunningPlanID(void);

    static void Lock_to_Determine_SegmentPlanPhase_For_Act(time_t *t, CPhaseInfo *, CPlanInfo *, CSegmentInfo *, CPlanInfo *);

    static bool vSendRevSync(void);
    static void vGetRevInfo(unsigned short int *, CReverseTimeInfo *);
    unsigned int vGetRevTimerSec(void);

    bool vReportCCTRevStatus5F82(void);

    static unsigned int vDetermineTimeToNextPlan(void);

    //arwen add 110210
    static void ConvertSegmentData(bool);
    static void ACT_Save_Segment_Data(const CSegmentInfo &actsegment);
    static void Lock_to_Determine_SegmentPlanPhase_For_Act2(time_t *t, CPhaseInfo *, CPlanInfo *, CSegmentInfo *, CPlanInfo *,unsigned short int&,unsigned short int&,unsigned short int);
    //arwen add 111004
    static void changeActModeToArwenAct(unsigned short int);
    //-------------------------------------------chou-----------------------------------
    static bool Fixgreen ;                                         //判斷是否要更改第二分向綠燈時間 ---2012/09/24 chou                                                 //for 5F1C
    static void Check5F1C(void);
    static void CheckPed(void);                                 //判斷觸發的行人按鈕是否已結束該次tod -----2013/01/17 chou
    static void vsendPedgreencount(void);
    static void vSendNSPredcount(void);
    //------------------------------------------------------------------------------------------------

    //OT20131221
    static void CalculatePgCount(void);
    static void CalculatePrCount(void);

    //OT20140214
    static bool vSetTODCurrentTime(unsigned short int);

    //OT20140224
    static bool vGoToNextStepByTimer(void);
    static void vReportBF01CCTProtocalSendKaikinStep(void);

    //jackt20140314
    static void jsonPhase(int, unsigned short int);
    static void jsonstd_plan_ID(unsigned short int, unsigned short int, unsigned short int, unsigned short int, unsigned short int);
    static void jsonstd_plan_light(unsigned short int, unsigned short int, unsigned short int, unsigned short int, unsigned short int,
                              unsigned short int, unsigned short int, int);
    static void jsontod_plan(unsigned short int, unsigned short int, unsigned short int, unsigned short int, unsigned short int);
    static void jsonequipment(void);
    static void jsonday_segtype_weekday(unsigned short int, unsigned short int);
    static void jsonday_segtype_holiday(unsigned short int, unsigned short int, unsigned short int, unsigned short int,
                                        unsigned short int, unsigned short int, unsigned short int);
    static void jsonPriority_Parameters(unsigned short int, unsigned short int, unsigned short int, unsigned short int,
                             unsigned short int, unsigned short int, unsigned short int, unsigned short int);
    static void jsonPriority_Percentage(unsigned short int, unsigned short int, unsigned short int);

    //jacky20140422
    static void PriorityParameterBF02(unsigned short int, unsigned short int);
    //jacky20141209
    static PTRAFFIC92COMM pTraffic92Comm;

    //CCJ20140812
    static unsigned char ucTODNextPlanID;

    static void CheckDynSegAndSendNextPlanId();


};
//----------------------------------------------------------
extern CSTC stc;
#endif
