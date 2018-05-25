#ifndef SHIRINK_APP_H
#define SHIRINK_APP_H
#include "var.h"
#include "CReverseTime.h"
#include "CSegmentInfo.h"
#include "CPhaseInfo.h"
#include "CPlanInfo.h"
#include "CIOCom.h"
#include "CPacketCluster.h"

#include "CRTMSDecoder.h"
#include "CRTMSInformation.h"
#include "CTrafficAnalyzer.h"
#include "CSTC.h"
class shirink_app
{
public:
    shirink_app();
    virtual ~shirink_app();

    typedef struct s_IP
    {
        int localIP1[4];
        int localIP1_port;
        int distIP0[4];
        int distIP0_port;
        int distIP1[4];
        int distIP1_port;
        int netmask[4];
        int gateway[4];

    } ;

    typedef struct s_Railchain_prama
    {
        BYTE sTC_TrainChainEnable;
        BYTE sTrainComingForceJumpSubphase;
        BYTE sTrainComingBanSubphase;

    } ;

    typedef struct s_TC_manual_setting
    {
        int dbOperStat;
        BYTE HWCycleCodeIN;
        unsigned short int WayMappingRedCountIN[8];


    } ;

    typedef struct s_Proxy_transfer//0f8e 0f8f
    {
        BYTE PassMode_0F8E8FIN;
        unsigned short int proxy_LCN;

    } ;

    typedef struct s_Signal_card_direcion
    {
        unsigned short int SignamMappingDirIN[8];
        unsigned short int SignamMappingLightBoardIN[6];
    } ;

    typedef struct s_TC_stratage_set
    {
        BYTE TC_ControlStrategyIN;
        int SignalStepStatus_5F03_IntervalTimeIN;
        int TC_SignalStepStatus_5F03_IntervalTimeIN;
        int RedCountVerSelectIN;
        bool TC_ActuateTypeFunctionEnableIN;
        unsigned short int TC_CCT_In_LongTanu_ActuateType_PlanID;
        BYTE ActuatePhaseExtendIN;
        bool ActuateautoswitchIN;


    };

    typedef struct s_Compensation_param
    {
      //  bool enable_switch;
        unsigned short int cycle_num;
    };

    typedef struct s_Learn_mode_group
    {
    //    bool enable_car_learn;
        unsigned short int CarLearn;
      //  bool enable_ped_learn;
        unsigned short int PedLearn;
        //bool enable_CarCountDown_learn;
        unsigned short int CarCountDownLearn;
        //bool enable_PedCount_learn;
        unsigned short int PedCountLearn;

    };

    typedef struct s_Ped_control_set
    {
      //  bool enable_SWCount;
        unsigned short int SWDevCount;

    };

    typedef struct s_Chain_set_group
    {
        BYTE sTC_5F31Manual;
        BYTE sTC_5F31TOD;
        BYTE sTC_5F31StartSubPhaseId;
        BYTE sTC_5F31EndSubPhaseId;
        BYTE sTC_5F32StartSubPhaseId;
        BYTE sTC_5F32EndSubPhaseId;
        BYTE sTC_MotherChainStartStepId;
        BYTE sTC_MotherChainEndStepId;
        short int TC_5F33StartOffset[64];
        short int TC_5F33EndOffset[64];
    };

    typedef struct s_TC_project_data
    {
        CPhaseInfo      phase[AMOUNT_PHASEORDER];
        CPlanInfo       plan[AMOUNT_PLANID];
        CSegmentInfo    segment[AMOUNT_SEGMENT];
        CWeekDaySegType weekdayseg[AMOUNT_WEEKDAY_SEG]; //{0-6,7-13} according to {1-7,11-17}
        CHoliDaySegType holidayseg[AMOUNT_HOLIDAY_SEG]; //{0-12} according to {8-20}
        CReverseTimeInfo    reversetime[AMOUNT_REVERSETIME];
        CWeekDayRevType weekdayrev[AMOUNT_WEEKDAY_REV]; //{0-6,7-13} according to {1-7,11-17}
        CHoliDayRevType holidayrev[AMOUNT_HOLIDAY_REV]; //{0-12} according to {4-16}


    };
    typedef struct Execute_data
    {
        unsigned short int _exec_phase_current_subphase;  //start from 0
        unsigned short int _exec_phase_current_subphase_step;  //start from 0
        unsigned short int _exec_segment_current_seg_no;  //start from 0
        unsigned short int _exec_reversetime_current_rev_no;  //start from 0
        unsigned short int _exec_reversetime_current_rev_step;  //start from 0
        ControlStrategy _current_strategy;
        ControlStrategy _old_strategy;
    };
/***    refresh_series catch tc data to shrink_app class       ****/
    void refresh_ip();
    void refresh_railchan_parama();
    void refresh_manual_setting();
    void refresh_proxy_transfer();
    void refresh_signal_card_direction();
    void refresh_tc_stratage_set();
    void refresh_compensation();
    void refresh_learn_mode_group();
    void refresh_ped_control_set();
    void refresh_chain_set_group();
    void refresh_tc_project_data();
    void refresh_execute_data();
/***     "set series" receive shrink_app_pad'data than set in tc, then use refresh_series update shrink_app's object    ***/
    void set_ip();
    void set_manual_setting();
    void set_proxy_transfer();
    void set_signal_card_direction();
    void set_tc_stratage_set();
    void set_compensation();
    void set_learn_mode_group();
    void set_ped_control_set();
    void set_chain_set_group();
    void set_tc_project_data();
    void set_execute_data();
 /***   "send series" send the tc's data with json form to shrink_app_pad  ***/
    void send_ip();
    void send_manual_sendting();
    void send_proxy_transfer();
    void send_signal_card_direction();
    void send_tc_stratage_send();
    void send_compensation();
    void send_learn_mode_group();
    void send_ped_control_send();
    void send_chain_send_group();
    void send_tc_project_data();
    void send_execute_data();


//protected:
private:
    s_IP s_ip;
    s_Railchain_prama s_railchain_parama;
    s_TC_manual_setting s_tc_manual_setting;
    s_Proxy_transfer s_proxy_transfer;
    s_Signal_card_direcion s_signal_card_direction;
    s_TC_stratage_set s_tc_stratage_set;
    s_Compensation_param s_compensation;
    s_Learn_mode_group s_learn_mode_group;
    s_Ped_control_set s_ped_control_set;
    s_Chain_set_group s_chain_set_group;
    s_TC_project_data s_tc_project_data;
    Execute_data execute_data;




    long lastRestSect;
    unsigned int LCN;
    time_t last_get_5F15time;
    int Com2Type;
    BYTE MBType;


};

#endif // SHIRINK_APP_H
