#include "CREATEXML.h"
#include "CSTC.h"
#include "CSTORINI.h"
#include "SMEM.h"
/////////////////////////////
CREATEXML xml;
////////////////////////////
CREATEXML::CREATEXML()
{

}
CREATEXML::~CREATEXML()
{

}
bool CREATEXML::SAVEALLXML(){

    WeekDaySegtype();
    HoliDaySegtype();
    DeviceID();
    PhaseID();
    //SegmentPage();
    SegmentRePort();
    PlanData();
    return true;
}
bool CREATEXML::SegmentRePort()
{
    char temp[200];
    oStorINI.vSave_XML_HEAD("<?xml version='1.0' encoding='UTF-8' ?>",0,SEGMENTXMLFILE);
    oStorINI.vSave_XML_HEAD("<SegmentDatas>",1,SEGMENTXMLFILE);
    for(int i=1;i<21;i++){
        if (stc.Lock_to_Load_Segment_for_Panel(i)) {
            oStorINI.vSave_XML_HEAD("   <SegmentGroup>",1,SEGMENTXMLFILE);
            oStorINI.vSave_XML_HEAD("   <Segments>",1,SEGMENTXMLFILE);
            oStorINI.vSave_XML_INT("      <SegmentID>","</SegmentID>",i,SEGMENTXMLFILE);
            oStorINI.vSave_XML_INT("      <SegmentType>","</SegmentType>",stc._panel_segment._segment_type,SEGMENTXMLFILE);
            oStorINI.vSave_XML_INT("      <SegmentCount>","</SegmentCount>",stc._panel_segment._segment_count,SEGMENTXMLFILE);
            //new String
            oStorINI.vSave_XML_INT("      <SegmentACTSegType1>","</SegmentACTSegType1>",stc._panel_segment._actSegmentType1,SEGMENTXMLFILE);//arwen xml Add 1001006
            oStorINI.vSave_XML_INT("      <SegmentACTSegType2>","</SegmentACTSegType2>",stc._panel_segment._actSegmentType2,SEGMENTXMLFILE);//arwen xml Add 1001006
            oStorINI.vSave_XML_HEAD("   </Segments>",1,SEGMENTXMLFILE);
            for (int j=0;j<stc._panel_segment._segment_count;j++) {    //¦A­«·s¼g
                oStorINI.vSave_XML_HEAD("      <ExecTimes>",1,SEGMENTXMLFILE);
                sprintf(temp,"          <ExecTimeH var='%#02d'></ExecTimeH>",stc._panel_segment._ptr_seg_exec_time[j]._hour);
                oStorINI.vSave_XML_HEAD(temp,1,SEGMENTXMLFILE);
                sprintf(temp,"          <ExecTimeM var='%#02d'></ExecTimeM>",stc._panel_segment._ptr_seg_exec_time[j]._minute);
                oStorINI.vSave_XML_HEAD(temp,1,SEGMENTXMLFILE);
                sprintf(temp,"          <ExecTimePlanid var='%d'></ExecTimePlanid>",stc._panel_segment._ptr_seg_exec_time[j]._planid);
                oStorINI.vSave_XML_HEAD(temp,1,SEGMENTXMLFILE);
                sprintf(temp,"          <ExecTimeCadcSeg var='%d'></ExecTimeCadcSeg>",stc._panel_segment._ptr_seg_exec_time[j]._cadc_seg);
                oStorINI.vSave_XML_HEAD(temp,1,SEGMENTXMLFILE);
                //new String
                sprintf(temp,"          <ExecTimePhaseAtcMode var='%d'></ExecTimePhaseAtcMode>",stc._panel_segment._ptr_seg_exec_time[j]._actMode);
                oStorINI.vSave_XML_HEAD(temp,1,SEGMENTXMLFILE);
                sprintf(temp,"          <ExecTimePhaseOpenRunnig var='%d'></ExecTimePhaseOpenRunnig>",stc._panel_segment._ptr_seg_exec_time[j]._openRunnig);
                oStorINI.vSave_XML_HEAD(temp,1,SEGMENTXMLFILE);
////////////////////////////////////////////////////--PlanInfo
////////////////////////////////////////////////////--PlanInfo End


                oStorINI.vSave_XML_HEAD("      </ExecTimes>",1,SEGMENTXMLFILE);
            }
            oStorINI.vSave_XML_HEAD("   </SegmentGroup>",1,SEGMENTXMLFILE);
        }
    }
    oStorINI.vSave_XML_HEAD("</SegmentDatas>",1,SEGMENTXMLFILE);
    return true;
}
bool CREATEXML::WeekDaySegtype()
{
    stc.Lock_to_Load_WeekDaySegment_for_Panel();
    oStorINI.vSave_XML_HEAD("<?xml version='1.0' encoding='UTF-8' ?>",0,WEEKDAYXMLFILE);
    oStorINI.vSave_XML_HEAD("<WeekDaySegType>",1,WEEKDAYXMLFILE);
    for(int i=0;i<14;i++){
        oStorINI.vSave_XML_HEAD("   <WSegmentGroup>",1,WEEKDAYXMLFILE);
        oStorINI.vSave_XML_INT("      <WSegmentID>","</WSegmentID>",i,WEEKDAYXMLFILE);
        oStorINI.vSave_XML_INT("      <WSegmentType>","</WSegmentType>",stc._panel_weekdayseg[i]._segment_type,WEEKDAYXMLFILE);
        oStorINI.vSave_XML_INT("      <WSegmentWeekDay>","</WSegmentWeekDay>",stc._panel_weekdayseg[i]._weekday,WEEKDAYXMLFILE);
        oStorINI.vSave_XML_HEAD("   </WSegmentGroup>",1,WEEKDAYXMLFILE);
    }
    oStorINI.vSave_XML_HEAD("</WeekDaySegType>",1,WEEKDAYXMLFILE);
    return true;
}
bool CREATEXML::HoliDaySegtype()
{
    oStorINI.vSave_XML_HEAD("<?xml version='1.0' encoding='UTF-8' ?>",0,HOLIDAYXMLFILE);
    oStorINI.vSave_XML_HEAD("<HoliDaySegType>",1,HOLIDAYXMLFILE);
    for (int i=8;i<=20;i++) {
         stc.Lock_to_Load_HoliDaySegment_for_Panel(i);
         oStorINI.vSave_XML_HEAD("   <HSegmentGroup>",1,HOLIDAYXMLFILE);
         oStorINI.vSave_XML_INT("      <HSegmentID>","</HSegmentID>",i-8,HOLIDAYXMLFILE);
         oStorINI.vSave_XML_INT("      <HSegmentType>","</HSegmentType>",stc._panel_holidayseg._segment_type,HOLIDAYXMLFILE);
         oStorINI.vSave_XML_INT("      <HSegmentStartYear>","</HSegmentStartYear>",stc._panel_holidayseg._start_year,HOLIDAYXMLFILE);
         oStorINI.vSave_XML_INT("      <HSegmentStartMonth>","</HSegmentStartMonth>",stc._panel_holidayseg._start_month,HOLIDAYXMLFILE);
         oStorINI.vSave_XML_INT("      <HSegmentStartDay>","</HSegmentStartDay>",stc._panel_holidayseg._start_day,HOLIDAYXMLFILE);
         oStorINI.vSave_XML_INT("      <HSegmentEndYear>","</HSegmentEndYear>",stc._panel_holidayseg._end_year,HOLIDAYXMLFILE);
         oStorINI.vSave_XML_INT("      <HSegmentEndMonth>","</HSegmentEndMonth>",stc._panel_holidayseg._end_month,HOLIDAYXMLFILE);
         oStorINI.vSave_XML_INT("      <HSegmentEndDay>","</HSegmentEndDay>",stc._panel_holidayseg._end_day,HOLIDAYXMLFILE);
         oStorINI.vSave_XML_HEAD("   </HSegmentGroup>",1,HOLIDAYXMLFILE);

    }
    oStorINI.vSave_XML_HEAD("</HoliDaySegType>",1,HOLIDAYXMLFILE);
}
bool CREATEXML::DeviceID()
{
    oStorINI.vSave_XML_HEAD("<?xml version='1.0' encoding='UTF-8' ?>",0,DEVIDXMLFILE);
    oStorINI.vSave_XML_HEAD("<DEVs>",1,DEVIDXMLFILE);
    oStorINI.vSave_XML_INT("      <DevId>","</DevId>",smem.GetAddress(),DEVIDXMLFILE);
    oStorINI.vSave_XML_HEAD("</DEVs>",1,DEVIDXMLFILE);
}
bool CREATEXML::PhaseID(){
  unsigned short int iCurrentPhaseID  = stc.vGetUSIData(CSTC_exec_plan_phase_order);
  unsigned short int iCurrentPlanID   = stc.vGetUSIData(CSTC_exec_plan_plan_ID);

  stc.Lock_to_Load_Plan_for_Panel(iCurrentPlanID);
  stc.Lock_to_Load_Phase_for_Panel(iCurrentPhaseID);
  int iCurrentSubphaseTotal = stc._panel_phase._subphase_count;
  if (ucPhaseID!=iCurrentPhaseID || ucPlanID!=iCurrentPlanID){
        oStorINI.vSave_XML_HEAD("<?xml version='1.0' encoding='UTF-8' ?>",0,PHASEIDXMLFILE);
        oStorINI.vSave_XML_HEAD("<PHASE>",1,PHASEIDXMLFILE);
        oStorINI.vSave_XML_HEX("      <PHASEId>","</PHASEId>",iCurrentPhaseID,PHASEIDXMLFILE);
        oStorINI.vSave_XML_INT("      <PLANId>","</PLANId>",iCurrentPlanID,PHASEIDXMLFILE);
        oStorINI.vSave_XML_INT("      <SubphaseTotal>","</SubphaseTotal>",iCurrentSubphaseTotal,PHASEIDXMLFILE);
        int iCurrentSubphaseTotal = stc._panel_phase._subphase_count;
        oStorINI.vSave_XML_HEAD("</PHASE>",1,PHASEIDXMLFILE);
  }
  ucPhaseID=iCurrentPhaseID;
  ucPlanID=iCurrentPlanID;
}
bool CREATEXML::SegmentPage()
{
char temp[200];
char filename[200];
        sprintf(filename,"/tmp/www/xml/segment");
        oStorINI.vSave_XML_HEAD("<?xml version='1.0' encoding='UTF-8' ?>",0,filename);
        oStorINI.vSave_XML_HEAD("<SegmentDatas>",1,filename);
    for(int i=1;i<21;i++){

        if (stc.Lock_to_Load_Segment_for_Panel(i)) {
            oStorINI.vSave_XML_HEAD("   <SegmentGroup>",1,filename);
            oStorINI.vSave_XML_INT("      <SegmentID>","</SegmentID>",i,filename);
            oStorINI.vSave_XML_INT("      <SegmentType>","</SegmentType>",stc._panel_segment._segment_type,filename);
            oStorINI.vSave_XML_INT("      <SegmentCount>","</SegmentCount>",stc._panel_segment._segment_count,filename);


            for (int j=0;j<stc._panel_segment._segment_count;j++) {    //¦A­«·s¼g
                oStorINI.vSave_XML_HEAD("      <ExecTimes>",1,filename);
                /*
                sprintf(temp,"          <ExecTimeH>%#02d</ExecTimeH>",stc._panel_segment._ptr_seg_exec_time[j]._hour);
                oStorINI.vSave_XML_HEAD(temp,1,filename);
                sprintf(temp,"          <ExecTimeM>%#02d</ExecTimeM>",stc._panel_segment._ptr_seg_exec_time[j]._minute);
                oStorINI.vSave_XML_HEAD(temp,1,filename);
                */
                sprintf(temp,"          <SegID>%#02d</SegID>",i);
                oStorINI.vSave_XML_HEAD(temp,1,filename);
                sprintf(temp,"          <ExecTime>%#02d:%#02d</ExecTime>",stc._panel_segment._ptr_seg_exec_time[j]._hour,stc._panel_segment._ptr_seg_exec_time[j]._minute);
                oStorINI.vSave_XML_HEAD(temp,1,filename);
                sprintf(temp,"          <ExecTimePlanid>%d</ExecTimePlanid>",stc._panel_segment._ptr_seg_exec_time[j]._planid);
                oStorINI.vSave_XML_HEAD(temp,1,filename);
                sprintf(temp,"          <ExecTimeCadcSeg>%d</ExecTimeCadcSeg>",stc._panel_segment._ptr_seg_exec_time[j]._cadc_seg);
                oStorINI.vSave_XML_HEAD(temp,1,filename);




                stc.Lock_to_Load_Plan_for_Panel(stc._panel_segment._ptr_seg_exec_time[j]._planid);

                oStorINI.vSave_XML_HEAD("          <PLANINFO>",1,filename);
                oStorINI.vSave_XML_INT("            <PlanID>","</PlanID>",stc._panel_plan._planid,filename);
                oStorINI.vSave_XML_INT("            <PlanDIR>","</PlanDIR>",stc._panel_plan._dir,filename);
                oStorINI.vSave_XML_HEX("            <PlanPhaseOrder>","</PlanPhaseOrder>",stc._panel_plan._phase_order,filename);
                oStorINI.vSave_XML_INT("            <SubPhaseTotal>","</SubPhaseTotal>",stc._panel_plan._subphase_count,filename);
                oStorINI.vSave_XML_INT("            <Offset>","</Offset>",stc._panel_plan._offset,filename);
                oStorINI.vSave_XML_INT("            <CycleTime>","</CycleTime>",stc._panel_plan._cycle_time,filename);

                for(int k=0;k<6;k++){
                    //oStorINI.vSave_XML_HEAD("           <SubPhases>",1,filename);
                    if(k<stc._panel_plan._subphase_count){

                    sprintf(temp,"              <SubPhaseG%d>%d</SubPhaseG%d>",k,stc._panel_plan._ptr_subplaninfo[k]._green,k);
                    oStorINI.vSave_XML_HEAD(temp,1,filename);
                    sprintf(temp,"              <SubPhaseMin%d>%d</SubPhaseMin%d>",k,stc._panel_plan._ptr_subplaninfo[k]._min_green,k);
                    oStorINI.vSave_XML_HEAD(temp,1,filename);
                    sprintf(temp,"              <SubPhaseMax%d>%d</SubPhaseMax%d>",k,stc._panel_plan._ptr_subplaninfo[k]._max_green,k);
                    oStorINI.vSave_XML_HEAD(temp,1,filename);
                    sprintf(temp,"              <SubPhaseY%d>%d</SubPhaseY%d>",k,stc._panel_plan._ptr_subplaninfo[k]._yellow,k);
                    oStorINI.vSave_XML_HEAD(temp,1,filename);
                    sprintf(temp,"              <SubPhaseR%d>%d</SubPhaseR%d>",k,stc._panel_plan._ptr_subplaninfo[k]._allred,k);
                    oStorINI.vSave_XML_HEAD(temp,1,filename);
                    sprintf(temp,"              <SubPhasePF%d>%d</SubPhasePF%d>",k,stc._panel_plan._ptr_subplaninfo[k]._pedgreen_flash,k);
                    oStorINI.vSave_XML_HEAD(temp,1,filename);
                    sprintf(temp,"              <SubPhasePR%d>%d</SubPhasePR%d>",k,stc._panel_plan._ptr_subplaninfo[k]._pedred,k);
                    oStorINI.vSave_XML_HEAD(temp,1,filename);
                    sprintf(temp,"              <PhaseAllSec%d>%d</PhaseAllSec%d>",k,stc._panel_plan._ptr_subplaninfo[k]._pedgreen_flash+stc._panel_plan._ptr_subplaninfo[k]._allred+stc._panel_plan._ptr_subplaninfo[k]._yellow+stc._panel_plan._ptr_subplaninfo[k]._green+stc._panel_plan._ptr_subplaninfo[k]._pedred,k);
                    oStorINI.vSave_XML_HEAD(temp,1,filename);

                    }else{
                    sprintf(temp,"              <SubPhaseG%d>%d</SubPhaseG%d>",k,0,k);
                    oStorINI.vSave_XML_HEAD(temp,1,filename);
                    sprintf(temp,"              <SubPhaseMin%d>%d</SubPhaseMin%d>",k,0,k);
                    oStorINI.vSave_XML_HEAD(temp,1,filename);
                    sprintf(temp,"              <SubPhaseMax%d>%d</SubPhaseMax%d>",k,0,k);
                    oStorINI.vSave_XML_HEAD(temp,1,filename);
                    sprintf(temp,"              <SubPhaseY%d>%d</SubPhaseY%d>",k,0,k);
                    oStorINI.vSave_XML_HEAD(temp,1,filename);
                    sprintf(temp,"              <SubPhaseR%d>%d</SubPhaseR%d>",k,0,k);
                    oStorINI.vSave_XML_HEAD(temp,1,filename);
                    sprintf(temp,"              <SubPhasePF%d>%d</SubPhasePF%d>",k,0,k);
                    oStorINI.vSave_XML_HEAD(temp,1,filename);
                    sprintf(temp,"              <SubPhasePR%d>%d</SubPhasePR%d>",k,0,k);
                    oStorINI.vSave_XML_HEAD(temp,1,filename);
                    sprintf(temp,"              <PhaseAllSec%d>%d</PhaseAllSec%d>",k,0,k);
                    oStorINI.vSave_XML_HEAD(temp,1,filename);
                    }
                    //oStorINI.vSave_XML_HEAD("           </SubPhases>",1,filename);
                }
                oStorINI.vSave_XML_HEAD("          </PLANINFO>",1,filename);
////////////////////////////////////////////////////--PlanInfo
////////////////////////////////////////////////////--PlanInfo End


                oStorINI.vSave_XML_HEAD("      </ExecTimes>",1,filename);
            }
            oStorINI.vSave_XML_HEAD("   </SegmentGroup>",1,filename);

        }
    }
    oStorINI.vSave_XML_HEAD("</SegmentDatas>",1,filename);
    return true;
}
//----------------------------------------------------------------------------------------------
bool CREATEXML::PlanData()
{
    bool bPlanStatus;
    char temp[200];
    oStorINI.vSave_XML_HEAD("<?xml version='1.0' encoding='UTF-8' ?>",0,PLANXMLFILE);//arwen xml Add 1001006
    oStorINI.vSave_XML_HEAD("<PlanDatas>",1,PLANXMLFILE);//arwen xml Add 1001006

    for( int i=0; i<AMOUNT_PLANID; i++){
        bPlanStatus = smem.vGetTCPhasePlanSegTypeData(TC_Plan, i);
        if(bPlanStatus){
            stc.Lock_to_Load_Plan_for_Center(i);
            oStorINI.vSave_XML_HEAD("    <PLANINFO>",1,PLANXMLFILE);//arwen xml Add 1001006
            oStorINI.vSave_XML_INT("         <PlanID>","</PlanID>",stc._for_center_plan._planid,PLANXMLFILE);//arwen xml Add 1001006
            oStorINI.vSave_XML_INT("         <PlanDIR>","</PlanDIR>",stc._for_center_plan._dir,PLANXMLFILE);//arwen xml Add 1001006
            oStorINI.vSave_XML_HEX("         <PlanPhaseOrder>","</PlanPhaseOrder>",stc._for_center_plan._phase_order,PLANXMLFILE);//arwen xml Add 1001006
            oStorINI.vSave_XML_INT("         <SubPhaseTotal>","</SubPhaseTotal>",stc._for_center_plan._subphase_count,PLANXMLFILE);//arwen xml Add 1001006
            oStorINI.vSave_XML_INT("         <Offset>","</Offset>",stc._for_center_plan._offset,PLANXMLFILE);//arwen xml Add 1001006
            oStorINI.vSave_XML_HEAD("        <SubPhases>",1,PLANXMLFILE);//arwen xml Add 1001006

            for( int j = 0 ; j < stc._for_center_plan._subphase_count ; j++){
                //arwen xml Add 1001006
                sprintf(temp,"             <SubPhaseG%d var='%d'></SubPhaseG%d>",j,stc._for_center_plan._ptr_subplaninfo[j]._green,j);
                oStorINI.vSave_XML_HEAD(temp,1,PLANXMLFILE);
                sprintf(temp,"             <SubPhaseMin%d  var='%d'></SubPhaseMin%d>",j,stc._for_center_plan._ptr_subplaninfo[j]._min_green,j);
                oStorINI.vSave_XML_HEAD(temp,1,PLANXMLFILE);
                sprintf(temp,"             <SubPhaseMax%d var='%d'></SubPhaseMax%d>",j,stc._for_center_plan._ptr_subplaninfo[j]._max_green,j);
                oStorINI.vSave_XML_HEAD(temp,1,PLANXMLFILE);
                sprintf(temp,"             <SubPhaseY%d var='%d'></SubPhaseY%d>",j,stc._for_center_plan._ptr_subplaninfo[j]._yellow,j);
                oStorINI.vSave_XML_HEAD(temp,1,PLANXMLFILE);
                sprintf(temp,"             <SubPhaseR%d var='%d'></SubPhaseR%d>",j,stc._for_center_plan._ptr_subplaninfo[j]._allred,j);
                oStorINI.vSave_XML_HEAD(temp,1,PLANXMLFILE);
                sprintf(temp,"             <SubPhasePF%d var='%d'></SubPhasePF%d>",j,stc._for_center_plan._ptr_subplaninfo[j]._pedgreen_flash,j);
                oStorINI.vSave_XML_HEAD(temp,1,PLANXMLFILE);
                sprintf(temp,"             <SubPhasePR%d var='%d'></SubPhasePR%d>",j,stc._for_center_plan._ptr_subplaninfo[j]._pedred,j);
                oStorINI.vSave_XML_HEAD(temp,1,PLANXMLFILE);
                sprintf(temp,"             <PhaseAllSec%d var='%d'></PhaseAllSec%d>",j,stc._for_center_plan._ptr_subplaninfo[j]._pedgreen_flash+stc._for_center_plan._ptr_subplaninfo[j]._allred+stc._for_center_plan._ptr_subplaninfo[j]._yellow+stc._for_center_plan._ptr_subplaninfo[j]._green+stc._for_center_plan._ptr_subplaninfo[j]._pedred,j);
                oStorINI.vSave_XML_HEAD(temp,1,PLANXMLFILE);
            }
            oStorINI.vSave_XML_HEAD("         </SubPhases>",1,PLANXMLFILE);//arwen xml Add 1001006
            oStorINI.vSave_XML_INT("         <CycleTime>","</CycleTime>",stc._for_center_plan._cycle_time,PLANXMLFILE);//arwen xml Add 1001006
            oStorINI.vSave_XML_HEAD("    </PLANINFO>",1,PLANXMLFILE);//arwen xml Add 1001006
        }
    }

     oStorINI.vSave_XML_HEAD("</PlanDatas>",1,PLANXMLFILE);//arwen xml Add 1001006
}
//----------------------------------------------------------------------------------------------
bool CREATEXML::ActModeData()
{
    int bsegActType,bsegPlanId,bsegZonetime,bsegCompensateMode;
    int bsegphaseTriggerSwitch,bsegActExtendSec;
    char temp[200];
    oStorINI.vSave_XML_HEAD("<?xml version='1.0' encoding='UTF-8' ?>",0,ACTMODEPLANFILE);//arwen xml Add 1001006
    oStorINI.vSave_XML_HEAD("<ActPlanDatas>",1,ACTMODEPLANFILE);//arwen xml Add 1001006
    for( int i=0; i<10; i++){
        oStorINI.vSave_XML_HEAD("    <ACTPLANINFO>",1,ACTMODEPLANFILE);//arwen xml Add 1001006
        bsegActType = smem.getSegmentActType(i);
        bsegPlanId = smem.getSegmentActPlan(i);
        bsegZonetime = smem.getSegmentZoneTimes(i);
        bsegCompensateMode = smem.getSegmentCompensateMode(i);
        oStorINI.vSave_XML_INT("         <ACTCOUNT>","</ACTCOUNT>",i,ACTMODEPLANFILE);//arwen xml Add 1001006
        oStorINI.vSave_XML_INT("         <ACTType>","</ACTType>",bsegActType,ACTMODEPLANFILE);//arwen xml Add 1001006
        oStorINI.vSave_XML_INT("         <ACTPlanID>","</ACTPlanID>",bsegPlanId,ACTMODEPLANFILE);//arwen xml Add 1001006
        oStorINI.vSave_XML_INT("         <ACTZoneTime>","</ACTZoneTime>",bsegZonetime,ACTMODEPLANFILE);//arwen xml Add 1001006
        oStorINI.vSave_XML_INT("         <ACTCompensateMode>","</ACTCompensateMode>",bsegCompensateMode,ACTMODEPLANFILE);//arwen xml Add 1001006
        for(int j = 0 ; j < 8 ; j++){
            bsegphaseTriggerSwitch = smem.getSegmentActVDPhaseTriggerSwitch(i,j);
            bsegActExtendSec = smem.getSegmentActExtendSec(i,j);
            sprintf(temp,"             <TriggerSwitch%d var='%d'></TriggerSwitch%d>",j,bsegphaseTriggerSwitch,j);
            oStorINI.vSave_XML_HEAD(temp,1,ACTMODEPLANFILE);
            sprintf(temp,"             <ActExtendSec%d var='%d'></ActExtendSec%d>",j,bsegActExtendSec,j);
            oStorINI.vSave_XML_HEAD(temp,1,ACTMODEPLANFILE);
        }

        oStorINI.vSave_XML_HEAD("    </ACTPLANINFO>",1,ACTMODEPLANFILE);//arwen xml Add 1001006


    }
    //oStorINI.vSave_XML_HEAD("</ActPlanDatas>",1,ACTMODEPLANFILE);//arwen xml Add 1001006
    //------------偵測器各分相編號
    for(int i=0;i<8;i++){
        oStorINI.vSave_XML_HEAD("    <VDPhase>",1,ACTMODEPLANFILE);//arwen xml Add 1001006
        for(int j=0;j<8;j++){
            sprintf(temp,"             <PhaseMapTable%d>%d</PhaseMapTable%d>",j+1,smem.vGetVDPhaseMapTable(i,j),j+1);
            oStorINI.vSave_XML_HEAD(temp,1,ACTMODEPLANFILE);
        }
        oStorINI.vSave_XML_HEAD("    </VDPhase>",1,ACTMODEPLANFILE);//arwen xml Add 1001006
    }
    oStorINI.vSave_XML_HEAD("</ActPlanDatas>",1,ACTMODEPLANFILE);//arwen xml Add 1001006
}
