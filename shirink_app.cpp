#include "shirink_app.h"
#include "SMEM.h"
#include "CSTC.h"
shirink_app::shirink_app()
{
    //ctor
}

shirink_app::~shirink_app()
{
    //dtor
}
/*shirink_app::
{

    try
    {

    }catch(...)
    {

    }



}*/
void shirink_app::refresh_ip()
{

    try
    {
        for(int i=1; i<5; i++)
        {
            s_ip.localIP1[i-1]=smem.GetLocalIP1(i);
            s_ip.distIP0[i-1]=smem.GetdistIp0(i);
            s_ip.distIP1[i-1]=smem.GetDistIP(i);
            s_ip.netmask[i-1]=smem.GetNetmask(i);
            s_ip.gateway[i-1]=smem.GetGateway(i);
        }
        s_ip.localIP1_port=smem.GetLocalIP1(5);
        s_ip.distIP0_port=smem.GetdistIp0(5);
        s_ip.distIP1_port=smem.GetDistIP(5);

    }
    catch(...)
    {

    }
}

void shirink_app::refresh_railchain_parama()
{

    try
    {
        s_railchain_parama.sTC_TrainChainEnable=smem.vGetUCData(TC_TrainChainEnable);
        s_railchain_parama.sTrainComingBanSubphase=smem.vGetUCData(TC_TrainComingBanSubphase);
        s_railchain_parama.sTrainComingForceJumpSubphase=smem.vGetUCData(TC_TrainComingForceJumpSubphase);


    }
    catch(...)
    {

    }



}
 void shirink_app::refresh_manual_setting()
 {
     try
     {
   /*      s_tc_manual_setting.dbOperStat=smem.GetDbOperStat();
       //  s_tc_manual_setting.HWCycleCodeIN
         s_tc_manual_setting.WayMappingRedCountIN=smem.vGetWayMappingRedCount();*/
     }catch(...){}

 }


void shirink_app::refresh_proxy_transfer()
{


    try
    {

        s_proxy_transfer.PassMode_0F8E8FIN=smem.vGetPassMode();
        s_proxy_transfer.proxy_LCN=smem.vGetPassServerLCN();



    }
    catch(...) {}

}
void shirink_app::refresh_signal_card_direction()
{
    try
    {

        for(int i=0; i<8; i++)
            s_signal_card_direction.SignamMappingDirIN[i]=smem.vGetSignamMapMappingDir(i);
        for(int i=0; i<6; i++)
            s_signal_card_direction.SignamMappingLightBoardIN[i]=smem.vGetSignamMapMappingLightBoard(i);

    }
    catch(...) {}



}

void shirink_app::refresh_tc_stratage_set()
{
    try
    {
        s_tc_stratage_set.TC_ControlStrategyIN=smem.vGetUCData(TC92_ucControlStrategy);
        s_tc_stratage_set.SignalStepStatus_5F03_IntervalTimeIN=smem.vGetINTData(TC92SignalLightStatus_5F0F_IntervalTime);
        s_tc_stratage_set.TC_SignalStepStatus_5F03_IntervalTimeIN=smem.vGetINTData(TC92SignalStepStatus_5F03_IntervalTime);
        s_tc_stratage_set.RedCountVerSelectIN=smem.vGetINTData(TC92_RedCountVer);
        s_tc_stratage_set.TC_ActuateTypeFunctionEnableIN=smem.vGetBOOLData(TC_CCT_In_LongTanu_ActuateType_FunctionEnable);
        s_tc_stratage_set.TC_CCT_In_LongTanu_ActuateType_PlanID=smem.vGetUSIData(TC_CCT_In_LongTanu_ActuateType_Switch);
        s_tc_stratage_set.ActuatePhaseExtendIN=smem.vGetActuatePhaseExtend();
        s_tc_stratage_set.ActuateautoswitchIN=smem.vGetBOOLData(TC_Actuateautoswitch);

    }
    catch(...) {}


}

void shirink_app::refresh_compensation()
{
    try
    {
        //  s_compensation.enable_switch=smem.
        s_compensation.cycle_num=smem.GetCompensationcycle();

    }
    catch(...) {}



}
void shirink_app::refresh_learn_mode_group()
{
    try
    {
        // s_learn_mode_group.enable_car_learn;
        s_learn_mode_group.CarLearn=smem.GetCarLearnSwitch();
        //   s_learn_mode_group.enable_ped_learn;
        s_learn_mode_group.PedLearn=smem.GetPedLearnSwitch();
        // s_learn_mode_group.enable_CarCountDown_learn;
        s_learn_mode_group.CarCountDownLearn=smem.GetCarCountdownProperty();
        // s_learn_mode_group.enable_PedCount_learn;
        s_learn_mode_group.PedCountLearn=smem.GetPedCountdownProperty();

    }
    catch(...) {}



}

void shirink_app::refresh_ped_control_set()
{
    try
    {
        //s_ped_control_set.enable_SWCount=smem.;
        s_ped_control_set.SWDevCount=smem.cPedPushButton.GetDevCount();
    }
    catch(...) {}
}


void shirink_app::refresh_chain_set_group()
{
    try
    {

        s_chain_set_group.sTC_5F31Manual=smem.vGetUCData(TC92_5F31Manual);
        s_chain_set_group.sTC_5F31TOD=smem.vGetUCData(TC92_5F31TOD);
        s_chain_set_group.sTC_5F31StartSubPhaseId=smem.vGetUCData(TC92_5F31StartSubPhaseId);
        s_chain_set_group.sTC_5F32EndSubPhaseId=smem.vGetUCData(TC92_5F31EndSubPhaseId);
        s_chain_set_group.sTC_5F32StartSubPhaseId=smem.vGetUCData(TC92_5F32StartSubPhaseId);
        s_chain_set_group.sTC_5F32EndSubPhaseId=smem.vGetUCData(TC92_5F32EndSubPhaseId);
        s_chain_set_group.sTC_MotherChainStartStepId=smem.vGetUCData(TC_MotherChainStartStepId);
        s_chain_set_group.sTC_MotherChainEndStepId=smem.vGetUCData(TC_MotherChainEndStepId);
        for(int i=0; i<64; i++)
        {
            s_chain_set_group.TC_5F33StartOffset[i]=smem.vGetChainOffset(1,i);
            s_chain_set_group.TC_5F33EndOffset[i]=smem.vGetChainOffset(2,i);
        }

    }
    catch(...) {}



}

void shirink_app::refresh_tc_project_data()
{
    try
    {
        for(int i=0; i<256; i++)
        {

            s_tc_project_data.phase[i]=stc.Get_stc_phase(i);//256
            s_tc_project_data.plan[i]=stc.Get_stc_plan(i);//256
            if(i<21)
                s_tc_project_data.segment[i]=stc.Get_stc_segment(i);//21
            if(i<17)
                s_tc_project_data.reversetime[i]=stc.Get_stc_reversetime(i);//17
            if(i<14)
            {
                s_tc_project_data.weekdayseg[i]=stc.Get_stc_weekdayseg(i);//14 //{0-6,7-13} according to {1-7,11-17}
                s_tc_project_data.weekdayrev[i]=stc.Get_stc_weekdayrev(i);
            }//14 //{0-6,7-13} according to {1-7,11-17}
            if(i<13)
            {
                s_tc_project_data.holidayrev[i]=stc.Get_stc_holidayrev(i);//13 //{0-12} according to {4-16}
                s_tc_project_data.holidayseg[i]=stc.Get_stc_holidayseg(i);//13 //{0-12} according to {8-20}
            }
        }

    }
    catch(...) {}


}

void shirink_app::refresh_execute_data()
{
    execute_data. _exec_phase_current_subphase=stc.vGetUSIData(CSTC_exec_phase_current_subphase);;  //start from 0
    execute_data. _exec_phase_current_subphase_step= stc.vGetUSIData(CSTC_exec_phase_current_subphase_step);
    execute_data. _exec_segment_current_seg_no= stc.vGetUSIData(CSTC_exec_segment_current_seg_no);
    execute_data. _exec_reversetime_current_rev_step=stc.Get_exec_reversetime_current_rev_step();  //start from 0
    execute_data._old_strategy=stc.Get_current_stratage();
    execute_data._current_strategy=stc.Get_old_stratage();
}


/***-------SET SERIRES----------------------****/
void shirink_app::set_ip()
{
    try {
    /*    for(int i=1; i<6; i++)
        {
            smem.SetLocalIP1(i,localIp1_[i]);
            smem.SetdistIp0(i,distIp0_[i]);
            smem.SetDistIP(i,distIp1_[i]);
        }
        for(int i=1; i<5; i++)
        {
            smem.SetNetmask(i,netmask[i]);
            smem.SetGateway(i,gateway[i]);
        }


 char cRunString[256];
        int localIp1_[6];//localIp[5]=localport
        int distIp0_[6];//distIp0_[5]=distIp0_port
        int distIp1_[6];//distIp1_[5]=distIp1_port
        int netmask[5];
        int gateway[5];

   system("rm -rf /bin/quickNetwork2");
        system("rm -rf /cct/quickNetwork2");
        bzero(cRunString, sizeof(cRunString));
        sprintf(cRunString, "cp /bin/spaceSH /bin/quickNetwork2");
        sprintf(cRunString, "cp /bin/spaceSH /cct/quickNetwork2");
        system(cRunString);
        bzero(cRunString, sizeof(cRunString));
        sprintf(cRunString, "echo /sbin/ifconfig eth0:3 %d.%d.%d.%d netmask %d.%d.%d.%d >> /bin/quickNetwork2", localIp1_[1], localIp1_[2], localIp1_[3], localIp1_[4], netmask[1], netmask[2], netmask[3], netmask[4]);
        system(cRunString);
        bzero(cRunString, sizeof(cRunString));
        sprintf(cRunString, "echo /sbin/route add default gw %d.%d.%d.%d >> /bin/quickNetwork2", gateway[1], gateway[2], gateway[3], gateway[4]);
        system(cRunString);
        bzero(cRunString, sizeof(cRunString));
        sprintf(cRunString, "chmod +x /bin/quickNetwork2");
        system(cRunString);


        system("sync");
        system("sync");
        //  system("reboot");



    */

    }
    catch(...) {}}
    void shirink_app::set_railchain_parama()
    {
        try{

 /*       smem.vSetUCData(TC_TrainChainEnable);
          smem.vSetUCData(TC_TrainComingBanSubphase);
            smem.vSetUCData(TC_TrainComingForceJumpSubphase);*/

        }catch(...){}
    }
void shirink_app::set_manual_setting()
{
    try {

    /*


    smem.setDbOperastat();
    smem.vSetWayMappingRedCount(1,2);


    */
    }
    catch(...) {}}
void shirink_app::set_proxy_transfer()
{
    try {
    /*
    smem.vSetPassMode()
    smem.vSetPassServerLCN()

    */
    }
    catch(...) {}}
void shirink_app::set_signal_card_direction()
{
    try {
    /*smem.vSetSignamMapMappingDir(i);
    smem.vSetSignamMapMappingLightBoard(i);*/
    }
    catch(...) {}}
void shirink_app::set_tc_stratage_set()
{
    try {
  /*
    smem.vSetUCData(TC92_ucControlStrategy);
        smem.vSetINTData(TC92SignalLightStatus_5F0F_IntervalTime);
        smem.vSetINTData(TC92SignalStepStatus_5F03_IntervalTime);
        smem.vSetINTData(TC92_RedCountVer);
        smem.vSetBOOLData(TC_CCT_In_LongTanu_ActuateType_FunctionEnable);
        smem.vSetUSIData(TC_CCT_In_LongTanu_ActuateType_Switch);
        smem.vSetActuatePhaseExtend();
        smem.vSetBOOLData(TC_Actuateautoswitch);

    */

    }
    catch(...) {}}
void shirink_app::set_compensation()
{
    try {
        /*
        smem.SetCompensationcycle();
        */

        }
    catch(...) {}}
void shirink_app::set_learn_mode_group()
{
    try {
 /*
        smem.SetCarLearnSwitch();

        smem.SetPedLearnSwitch();

        smem.SetCarCountdownProperty();

        smem.SetPedCountdownProperty();

    */

    }
    catch(...) {}}
void shirink_app::set_ped_control_set()
{
    try {
    //smem.cPedPushButton.SetDevCount();
    }
    catch(...) {}}
void shirink_app::set_chain_set_group()
{
    try {
  /*
     smem.vSetUCData(TC92_5F31Manual);
        smem.vSetUCData(TC92_5F31TOD);
        smem.vSetUCData(TC92_5F31StartSubPhaseId);
        smem.vSetUCData(TC92_5F31EndSubPhaseId);
        smem.vSetUCData(TC92_5F32StartSubPhaseId);
        smem.vSetUCData(TC92_5F32EndSubPhaseId);
        smem.vSetUCData(TC_MotherChainStartStepId);
        smem.vSetUCData(TC_MotherChainEndStepId);
        for(int i=0; i<64; i++)
        {
            smem.vSetChainOffset(1,i);
            smem.vSetChainOffset(2,i);
        }
    */
    }
    catch(...) {}}
void shirink_app::set_tc_project_data()
{
    try {
 /*   stc.Lock_to_Save_Phase();//phase
    stc.Lock_to_Save_Plan();//plan
    stc.Lock_to_Save_Segment();//segment
    stc.Lock_to_Save_Segment_from_Panel()//weekday
    stc.Lock_to_Save_HoliDaySegment();
    stc.Lock_to_Save_WeekDayReversetime_from_Center();
    stc.Lock_to_Save_HoliDaySegment()
    stc.Lock_to_Save_ReverseTime_Step2();
    */

    }
    catch(...) {}}
void shirink_app::set_execute_data()
{
    try {

  /*   iCurrentPhaseID  = stc.vGetUSIData(CSTC_exec_plan_phase_order);
  iCurrentPlanID   = stc.vGetUSIData(CSTC_exec_plan_plan_ID);

//  stc.Lock_to_Load_Segment_for_Panel(iCurrentSegNo);
  stc.Lock_to_Load_Plan_for_Panel(iCurrentPlanID);
  stc.Lock_to_Load_Phase_for_Panel(iCurrentPhaseID);

  iCurrentSubphase = stc.vGetUSIData(CSTC_exec_phase_current_subphase);
  iCurrentSubphaseTotal = stc._panel_phase._subphase_count;

  iLastStep = iCurrentStep;
  iCurrentStep     = stc.vGetUSIData(CSTC_exec_phase_current_subphase_step);
//  iCurrentStepTotal = stc._panel_phase._total_step_count;
  if(stc._panel_phase._total_step_count > 1) {
    iCurrentStepTotal = 5; //default set to 5
  }

  //Fuck Chen Ming Zu
//  iCurrentStep = iCurrentStep + ((iCurrentSubphase)*5);
  iCurrentStepTotal = iCurrentStepTotal*iCurrentSubphaseTotal;

  usiStepSec = stc.vGetStepTime();

  ucTCControlStrategy = smem.vGetUCData(TC92_ucControlStrategy);

  vCalActTuningStatus();
  vDisplayActionChange();
  vRefreshActuateTimes();
  vRefreshActSwitch();

    */

    }
    catch(...) {}}


































