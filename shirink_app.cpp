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
    }
    catch(...) {}

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
    try
    {
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
    try
    {

        /*       smem.vSetUCData(TC_TrainChainEnable);
                 smem.vSetUCData(TC_TrainComingBanSubphase);
                   smem.vSetUCData(TC_TrainComingForceJumpSubphase);*/

    }
    catch(...) {}
}
void shirink_app::set_manual_setting()
{
    try
    {

        /*


        smem.setDbOperastat();
        smem.vSetWayMappingRedCount(1,2);


        */
    }
    catch(...) {}}
void shirink_app::set_proxy_transfer()
{
    try
    {
        /*
        smem.vSetPassMode()
        smem.vSetPassServerLCN()

        */
    }
    catch(...) {}}
void shirink_app::set_signal_card_direction()
{
    try
    {
        /*smem.vSetSignamMapMappingDir(i);
        smem.vSetSignamMapMappingLightBoard(i);*/
    }
    catch(...) {}}
void shirink_app::set_tc_stratage_set()
{
    try
    {
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
    try
    {
        /*
        smem.SetCompensationcycle();
        */

    }
    catch(...) {}}
void shirink_app::set_learn_mode_group()
{
    try
    {
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
    try
    {
        //smem.cPedPushButton.SetDevCount();
    }
    catch(...) {}}
void shirink_app::set_chain_set_group()
{
    try
    {
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
    try
    {
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

void shirink_app::send_tc_project_data()
{

    try
    {
        refresh_tc_project_data();

        Json::Value segcontext;
        stc.Lock_to_Load_WeekDaySegment_for_Panel();
        for(int i=0; i<14; i++)
        {
            string_to_app["weekdaysegment"][i]=stc._panel_weekdayseg[i]._segment_type;
        }
  /*     Json::Value specialdaycontext;
        for(int i=8; i<21; i++)
        {
            stc.Lock_to_Load_HoliDaySegment_for_Panel(i);
            specialdaycontext["segmenttype"]=stc._panel_holidayseg._segment_type;
            specialdaycontext["start_year"]=stc._panel_holidayseg._start_year;
            specialdaycontext["start_month"]=stc._panel_holidayseg._start_month;
            specialdaycontext["start_day"]=stc._panel_holidayseg._start_day;
            specialdaycontext["end_year"]=stc._panel_holidayseg._end_year;
            specialdaycontext["end_month"]=stc._panel_holidayseg._end_month;
            specialdaycontext["end_day"]=stc._panel_holidayseg._end_day;
            string_to_app["specialdaycontext"][i-8]=specialdaycontext;
        }


        for(int i=0; i<21; i++)
        {
            stc.Lock_to_Load_Segment_for_Panel(i);


            // segmentinfo["segment_type"][i]=stc._panel_segment._segment_type;
            segmentinfo["segment_count"][i]=stc._panel_segment._segment_count;
             for(int j=0; j<32; j++) //32
            {    segcontext["hour"][j]=0;
                segcontext["minute"][j]=0;
                segcontext["plan"][j]=0;
                segcontext["actMode"][j]=0;
                if(j<stc._panel_segment._segment_count)
                {segcontext["hour"][j]=stc._panel_segment._ptr_seg_exec_time[j]._hour;
                segcontext["minute"][j]=stc._panel_segment._ptr_seg_exec_time[j]._minute;
                segcontext["plan"][j]=stc._panel_segment._ptr_seg_exec_time[j]._planid;
                segcontext["actMode"][j]=stc._panel_segment._ptr_seg_exec_time[j]._actMode;}
            }
            segcontext["segment_count"]=stc._panel_segment._segment_count;
            segmentinfo["segcontext"][i]=segcontext;
            //segmentinfo["segcontext"]=segcontext;
        }

        string_to_app["segmentinfo"]=segmentinfo;

        Json::Value plancontext;
        for(int j=0; j<49; j++)
        {
            stc.Lock_to_Load_Plan_for_Panel(j);

            plancontext["plan_id"]=stc._panel_plan._planid;
            plancontext["dir"]=stc._panel_plan._dir;

            plancontext["phase_order"]=stc._panel_plan._phase_order;
            plancontext["subphase_count"]=stc._panel_plan._subphase_count;
            plancontext["cycle_time"]=stc._panel_plan._cycle_time;
            plancontext["offset"]=stc._panel_plan._offset;
            for (int i=0; i<8; i++)       //8
            {
                plancontext["subphase_green"][i]=stc._panel_plan._ptr_subplaninfo[i]._green;
                plancontext["subphase_min_green"][i]=stc._panel_plan._ptr_subplaninfo[i]._min_green;
                plancontext["subphase_max_green"][i]=stc._panel_plan._ptr_subplaninfo[i]._max_green;
                plancontext["subphase_yellow"][i]=stc._panel_plan._ptr_subplaninfo[i]._yellow;
                plancontext["subphase_allred"][i]=stc._panel_plan._ptr_subplaninfo[i]._allred;
                plancontext["subphase_pedgreen_flash"][i]=stc._panel_plan._ptr_subplaninfo[i]._pedgreen_flash;
                plancontext["subphase_pedred"][i]=stc._panel_plan._ptr_subplaninfo[i]._pedred;

            }
            string_to_app["plancontext"][j]=plancontext;
        }

        Json::Value step;
        Json::Value subphase;
        Json::Value light;
        Json::Value stepdetail;
        for(int i=0; i<256; i++)

        {
            stc.Lock_to_Load_Phase_for_Panel(i);
            step["phase_ID"]=i;
            step["phase_order"] = stc._panel_phase._phase_order;
            step["signal_map"]= stc._panel_phase._signal_map;
            step["signal_count"]= stc._panel_phase._signal_count;
            step["subphase_count"]= stc._panel_phase._subphase_count;
            for(int j=0; j<6; j++)
                step["sub_stepcount"][j]= stc._panel_phase._ptr_subphase_step_count[j];

            for(int i = 0; i < stc._panel_phase._subphase_count; i++)
            {
                for(int j = 0; j <  stc._panel_phase._ptr_subphase_step_count[i]; j++)
                {
                    for(int k = 0; k < stc._panel_phase._signal_count; k++)
                    {
                        light["light"][k] = stc._panel_phase._ptr_subphase_step_signal_status[i][j][k];

                    }
                    stepdetail["stepdetail"][j]=light;
                }
                subphase["subphase"][i]=stepdetail;
            }
                step["stepcontext"]=subphase;
            string_to_app["step"][i]=step;
        }

*/

     //   printf("%s\n",string_to_app.toStyledString().c_str());
    }
    catch(...)
    {

    }


}


void shirink_app::send_execute_data()
{
    try
    {

        unsigned short int iCurrentPhaseID  = stc.vGetUSIData(CSTC_exec_plan_phase_order);
        unsigned short int iCurrentPlanID   = stc.vGetUSIData(CSTC_exec_plan_plan_ID);
        current_state["current_phaseID"]=iCurrentPhaseID;
        current_state["current_planID"]=iCurrentPlanID;
        /*        stc.Lock_to_Load_Plan_for_Panel(iCurrentPhaseID);
                stc.Lock_to_Load_Phase_for_Panel(iCurrentPlanID);
        */
        current_state["current_second"]=stc.vGetStepTime();
        int iCurrentSubphase = stc.vGetUSIData(CSTC_exec_phase_current_subphase);

        int iCurrentSubphaseTotal = stc._panel_phase._subphase_count;
        current_state["current_subphase"]= iCurrentSubphase+1;
        current_state["current_total_subphase"]= iCurrentSubphaseTotal;
        int iCurrentStep     = stc.vGetUSIData(CSTC_exec_phase_current_subphase_step);
        int iCurrentStepTotal = stc._panel_phase._total_step_count;
        printf("current_total_subphase=%d,current_total_step=%d\n",iCurrentSubphaseTotal,iCurrentStepTotal);
        current_state["current_step"]=iCurrentStep + ((iCurrentSubphase)*5)+1;
        current_state["current_total_step"]=iCurrentStepTotal;


        printf("%s\n",current_state.toStyledString().c_str());

    }
    catch(...) {}
}

void shirink_app::send_ip()
{
    try
    {



    }
    catch(...) {}
}
void shirink_app::send_manual_setting()
{
    try
    {
        Json::Value manual_setting;
        manual_setting["DbOperStat"]=smem.GetDbOperStat();

 string_to_app["manual_setting"]=manual_setting;
    }
    catch(...) {}
}

void shirink_app::send_railchain_parama()
{
    try
    {
        Json::Value railchaininfo;
        railchaininfo["TC_TrainChainEnable"]=smem.vGetUCData(TC_TrainChainEnable);
        railchaininfo["TrainComingBanSubphase"]=smem.vGetUCData(TC_TrainComingBanSubphase);
        railchaininfo["TrainComingForceJumpSubphase"]=smem.vGetUCData(TC_TrainComingForceJumpSubphase);

        string_to_app["railchaininfo"]=railchaininfo;
    }
    catch(...) {}
}

void shirink_app::send_proxy_transfer()
{
    try
    {
        Json::Value proxy_transfer;
        proxy_transfer["proxy_PassMode"]=smem.vGetPassMode();
        proxy_transfer["proxy_LCN"]=smem.vGetPassServerLCN();

        string_to_app["proxy_transfer"]=proxy_transfer;
    }
    catch(...) {}}
void shirink_app::send_signal_card_direction()
{
    try
    {

        Json::Value signal_card_dir;
        for(int i=0; i<8; i++)
        {
            signal_card_dir["signalMapMappingDir"][i]=smem.vGetSignamMapMappingDir(i);
            if(i<6)
                signal_card_dir["signalMappingLightBoard"][i]=smem.vGetSignamMapMappingLightBoard(i);

        }

 string_to_app["signal_car_dir"]=signal_card_dir;
    }
    catch(...) {}}
void shirink_app::send_tc_stratage_send()
{
    try
    {
        Json::Value tc_stratage;
        tc_stratage["ControlStratage"]=smem.vGetUCData(TC92_ucControlStrategy);
        tc_stratage["LightStatus_5F05_report_period"]=smem.vGetINTData(TC92SignalLightStatus_5F0F_IntervalTime);
        tc_stratage["StepStatus_report_period"]=smem.vGetINTData(TC92SignalStepStatus_5F03_IntervalTime);
        tc_stratage["Redcount_version"]=smem.vGetINTData(TC92_RedCountVer);
        tc_stratage["ActuateTypeFunction_enable_switch"]=smem.vGetBOOLData(TC_CCT_In_LongTanu_ActuateType_FunctionEnable);
        tc_stratage["In_LongTanu_ActuateType_PlanID"]=smem.vGetUSIData(TC_CCT_In_LongTanu_ActuateType_Switch);
        tc_stratage["ActuatePhaseExtend"]=smem.vGetActuatePhaseExtend();
        tc_stratage["Actuateautoswitch"]=smem.vGetBOOLData(TC_Actuateautoswitch);
 string_to_app["tc_stratage"]=tc_stratage;


    }
    catch(...) {}}
void shirink_app::send_compensation()
{
    try
    {
        Json::Value compensation;
        compensation["compensation_cycle"]=smem.GetCompensationcycle();
    string_to_app["compensation"]=compensation;
    }
    catch(...) {}}
void shirink_app::send_learn_mode_group()
{
    try
    {

        Json::Value learn_mode;
        learn_mode["CarLearn"]=smem.GetCarLearnSwitch();
        learn_mode["PedLearn"]=smem.GetPedLearnSwitch();
        learn_mode["CarCountDownLearn"]=smem.GetCarCountdownProperty();
        learn_mode["PedCountLearn"]=smem.GetPedCountdownProperty();
 string_to_app["learn_mode"]=learn_mode;
    }
    catch(...) {}}
void shirink_app::send_ped_control_send()
{
    try
    {
        Json::Value ped_control;
        ped_control["SWDevCount"]=smem.cPedPushButton.GetDevCount();


    string_to_app["ped_control"]=ped_control;
    }
    catch(...) {}}





















