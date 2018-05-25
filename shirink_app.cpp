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

void shirink_app::refresh_railchan_parama()
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




































