/*
 * ScreenActuateArwenFlashManyStatus.cpp
 *
 *  Created on: 2010/10/8
 *      Author: arwen
 */

#include "ScreenActuateArwenFlashManyStatus.h"
#include "LCD240x128.h"

#include "SCREENMain.h"
#include "ScreenActuateArwenMenu.h"
#include "CSTC.h"

#include <stdio.h>

#include "WRITEJOB.h"
#include "CDataToMessageOK.h"

#include "SMEM.h"
#include "CPlanInfo.h"
//-----------------------------------------------------------------------
ScreenActuateArwenFlashManyStatus screenActuateArwenFlashManyStatus ;
ScreenActuateArwenFlashManyStatus::ScreenActuateArwenFlashManyStatus() {
	// TODO Auto-generated constructor stub
	loadBitmapFromFile();
	initDispWord();
	cSelect=0;
	switch_times=-1;
	for(int i=0;i<4;i++){
	  	ilastSubphase[i]=-1;
	   	ilastStep[i]=-1;
	   	ilastPhaseGreen[i]=-1;
	   	ilastGreenSec[i]=-1;
	   	ilastbActTuningStatus[i]=-1;
	   	//ilastAdjMinGree[i]=-1;
	   	//ilastAdjMaxGree[i]=-1;
	}

}

ScreenActuateArwenFlashManyStatus::~ScreenActuateArwenFlashManyStatus() {
	// TODO Auto-generated destructor stub
}
//---------------------------------------------------------------------------
void ScreenActuateArwenFlashManyStatus::loadBitmapFromFile(void)
{
try {
    FILE *bitmap;
    bitmap=fopen("//cct//bitmap//backGround//actmenu//ActuateArwenManyStatusFlash_open.bit","rb");
    if (bitmap) {
        fread(ucActuateArwenBitmap_open,3840,1,bitmap);
        fclose(bitmap);
    }
    bitmap=fopen("//cct//bitmap//backGround//actmenu//ActuateArwenManyStatusFlash_close.bit","rb");
       if (bitmap) {
           fread(ucActuateArwenBitmap_close,3840,1,bitmap);
           fclose(bitmap);
       }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenActuateArwenFlashManyStatus::DisplayActuateArwenFlashManyStatus(void)
{
try {
    smem.SetcFace(cACTUATEARWENFLASHMANYSTATUS);
    if(bActTuningStatusNow == true){
    	lcd240x128.DISPLAY_GRAPHIC(0,ucActuateArwenBitmap_open,128,30);
    	showopen=true;
    	showclose=false;
    }else{
    	lcd240x128.DISPLAY_GRAPHIC(0,ucActuateArwenBitmap_close,128,30);
    	showclose=true;
    	showopen=false;
    }

    vRefreshActuateArwenManyStatusData();
    vRefreshActSwitch();
    //vShowToPanel();
    vShowActTuningStatus();
//?    vRefreshStepSec();

  } catch (...) {}
}

//---------------------------------------------------------------------------  //not used
void ScreenActuateArwenFlashManyStatus::vRefreshEverySec(void)
{
try {
  int iTmpFace = smem.GetcFace();

  if( smem.vGetUCData(TC_CCT_Send_ActuateArwen_Protocal) > 0 || iTmpFace == cACTUATEARWENFLASHMANYSTATUS) {
    vRefreshActuateArwenManyStatusData();

    //vRefreshActSwitch();
  }


  if(iTmpFace == cACTUATEARWENFLASHMANYSTATUS) {

	vShowToPanel();
    vShowActTuningStatus();
  }

}catch(...){}
}
//---------------------------------------------------------------------------
void ScreenActuateArwenFlashManyStatus::vShowToPanel(void)
{
try {

  int iHiTMP, iLowTMP;
  unsigned short int usi1, usi2, usi3;
  int iTmp, i1, i2, i3, i4;



  iTmp = iCurrentSubphase+1;                                                           //From 0 Start, For Human, plus 1
  if(iTmp < 100) {
    iHiTMP  = iTmp / 10;
    iLowTMP = iTmp % 10;
    lcd240x128.DISPLAY_GRAPHIC_XY(markSubPhase[0].X,markSubPhase[0].Y,word8x16[iHiTMP],markSubPhase[0].height,markSubPhase[0].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(markSubPhase[1].X,markSubPhase[1].Y,word8x16[iLowTMP],markSubPhase[1].height,markSubPhase[1].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(mark2SubPhase.X,mark2SubPhase.Y,word8x16[iLowTMP],mark2SubPhase.height,mark2SubPhase.width/8);
    //lcd240x128.DISPLAY_GRAPHIC_XY(OnlySubPhase[0].X,OnlySubPhase[0].Y,word8x16[iLowTMP],OnlySubPhase[0].height,OnlySubPhase[0].width/8);
  }

  for(int i=0;i<3;i++){
	  i1 =ilastSubphase[i]+1;
      i2 =ilastStep[i]+1;
	  iHiTMP = i1 % 10;
      iLowTMP = i2 % 10;
      if(iHiTMP>0 && iLowTMP>0){
      lcd240x128.DISPLAY_GRAPHIC_XY(OnlySubPhase[i].X,OnlySubPhase[i].Y,word8x16[iHiTMP],OnlySubPhase[i].height,OnlySubPhase[i].width/8);
      lcd240x128.DISPLAY_GRAPHIC_XY(OnlyStep[i].X,OnlyStep[i].Y,word8x16[iLowTMP],OnlyStep[i].height,OnlyStep[i].width/8);
      }
  }

  if(iCurrentSubphaseTotal < 100) {
    iHiTMP  = iCurrentSubphaseTotal / 10;
    iLowTMP = iCurrentSubphaseTotal % 10;
    lcd240x128.DISPLAY_GRAPHIC_XY(markSubPhase[2].X,markSubPhase[2].Y,word8x16[iHiTMP],markSubPhase[2].height,markSubPhase[2].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(markSubPhase[3].X,markSubPhase[3].Y,word8x16[iLowTMP],markSubPhase[3].height,markSubPhase[3].width/8);
  }

//--
  iTmp = iCurrentStep+1;                                                               //From 0 Start, For Human, plus 1
  if(iTmp < 100) {
    iHiTMP  = iTmp / 10;
    iLowTMP = iTmp % 10;
    lcd240x128.DISPLAY_GRAPHIC_XY(markStep[0].X,markStep[0].Y,word8x16[iHiTMP],markStep[0].height,markStep[0].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(markStep[1].X,markStep[1].Y,word8x16[iLowTMP],markStep[1].height,markStep[1].width/8);
    if(iCurrentStep+1==1){
    	lcd240x128.DISPLAY_GRAPHIC_XY(mark2Step.X,mark2Step.Y,word8x16[iLowTMP],mark2Step.height,mark2Step.width/8);
    }else{lcd240x128.DISPLAY_GRAPHIC_XY(mark2Step.X,mark2Step.Y,word8x16[1],mark2Step.height,mark2Step.width/8);}
    	//lcd240x128.DISPLAY_GRAPHIC_XY(OnlyStep[0].X,OnlyStep[0].Y,word8x16[iLowTMP],OnlyStep[0].height,OnlyStep[0].width/8);
  }

  iCurrentStepTotal = iCurrentStepTotal / iCurrentSubphaseTotal;
  if(iCurrentStepTotal < 100) {
    iHiTMP  = iCurrentStepTotal / 10;
    iLowTMP = iCurrentStepTotal % 10;
    lcd240x128.DISPLAY_GRAPHIC_XY(markStep[2].X,markStep[2].Y,word8x16[iHiTMP],markStep[2].height,markStep[2].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(markStep[3].X,markStep[3].Y,word8x16[iLowTMP],markStep[3].height,markStep[3].width/8);
  }

  if(usiStepSec < 10000) {
    i1  = usiStepSec / 1000;
    i2 = (usiStepSec - (i1*1000)) / 100;
//    lcd240x128.DISPLAY_GRAPHIC_XY(markStepSecCount[0].X,markStepSecCount[0].Y,word8x16[i1],markStepSecCount[0].height,markStepSecCount[0].width/8);
//    lcd240x128.DISPLAY_GRAPHIC_XY(markStepSecCount[1].X,markStepSecCount[1].Y,word8x16[i2],markStepSecCount[1].height,markStepSecCount[1].width/8);
  }
  if(usiStepSec < 10000) {
    i3  = (usiStepSec - (i2*100+i1*1000)) / 10;
    i4 = (usiStepSec - (i3*10+i2*100+i1*1000));
//    lcd240x128.DISPLAY_GRAPHIC_XY(markStepSecCount[2].X,markStepSecCount[2].Y,word8x16[i3],markStepSecCount[2].height,markStepSecCount[2].width/8);
//    lcd240x128.DISPLAY_GRAPHIC_XY(markStepSecCount[3].X,markStepSecCount[3].Y,word8x16[i4],markStepSecCount[3].height,markStepSecCount[3].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(markStepSecCount[0].X,markStepSecCount[0].Y,word8x16[i3],markStepSecCount[0].height,markStepSecCount[0].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(markStepSecCount[1].X,markStepSecCount[1].Y,word8x16[i4],markStepSecCount[1].height,markStepSecCount[1].width/8);
  }



} catch(...){}
}


//---------------------------------------------------------------------------
void ScreenActuateArwenFlashManyStatus::vRefreshActuateArwenManyStatusData(void)
{
try {

  iCurrentPhaseID  = stc.vGetUSIData(CSTC_exec_plan_phase_order);
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



//  int iCurrentSegNoTotal = stc._panel_segment._segment_count;
//  int iCurrentSegTypeID = stc._panel_segment._segment_type;


} catch(...){}
}


//---------------------------------------------------------------------------
void ScreenActuateArwenFlashManyStatus::vRefreshActSwitch(void)
{
try {
  if(ucTCControlStrategy == 0x09 &&  smem.vGetBOOLData(TC_CCT_In_LongTanu_ActuateType_FunctionEnable) == true ) {  //0x01 = TOD 0x09 = flash
    bActSwitch = true;
    lcd240x128.DISPLAY_GRAPHIC_XY(markActSwitch.X,markActSwitch.Y,word8x16[1],markActSwitch.height,markActSwitch.width/8);
  } else {
    bActSwitch = false;
    lcd240x128.DISPLAY_GRAPHIC_XY(markActSwitch.X,markActSwitch.Y,word8x16[0],markActSwitch.height,markActSwitch.width/8);
  }

}catch(...){}
}

//---------------------------------------------------------------------------
void ScreenActuateArwenFlashManyStatus::vCalActTuningStatus(void)
{
try {
	/*****************************/
	  time_t stc_time;
	  CPhaseInfo stc_phase;
	  CPlanInfo stc_plan;
	  CPlanInfo stc_runningPlan;
	  CSegmentInfo stc_segment;
	  stc.Lock_to_Determine_SegmentPlanPhase_For_Act(&stc_time, &stc_phase, &stc_plan, &stc_segment, &stc_runningPlan);
	  stc_runningPlan._planid;
	  stc_runningPlan._phase_order;
	  stc_runningPlan._subphase_count;
	  for(int i = 0; i < stc_runningPlan._subphase_count; i++) {
		  siAdjALLGreen[i]=stc_runningPlan._ptr_subplaninfo[i]._green+stc_runningPlan._ptr_subplaninfo[i]._pedgreen_flash;
	  }

	/****************************/

  int iCurrentStep = stc.vGetUSIData(CSTC_exec_phase_current_subphase_step);    //haven show above
  printf(">>>stc_runningPlan._planid;:%d stc_runningPlan._phase_order:%d stc_runningPlan._subphase_count:%d\n",stc_runningPlan._planid,stc_runningPlan._phase_order,stc_runningPlan._subphase_count);
  bActTuningStatusLast = bActTuningStatusNow;
  //if(bActSwitch == true && stc_runningPlan._planid==0x28 && stc_runningPlan._phase_order==0x00 && iCurrentStep <= 2 ) {
  if(bActSwitch == true && stc_runningPlan._planid==0x28 && stc_runningPlan._phase_order!=0xB0 && stc_runningPlan._phase_order!=0xB1 && iCurrentStep <= 2 ) {
    bActTuningStatusNow = true;
    RecordActTuningStatus=bActTuningStatusLast;
  } else {
    bActTuningStatusNow = false;
  }

/*
  if(bActTuningStatusNow == true ) {
    lcd240x128.DISPLAY_GRAPHIC_XY(markActTuning.X,markActTuning.Y,word8x16[1],markActTuning.height,markActTuning.width/8);
  } else {
    lcd240x128.DISPLAY_GRAPHIC_XY(markActTuning.X,markActTuning.Y,word8x16[0],markActTuning.height,markActTuning.width/8);
  }
*/

/*not use
  if(bActTuningStatusLast == false && bActTuningStatusNow == true) {  //first change
    smem.vSaveCurrentAsGreenStartTime();
  }
*/

//testMark  if(bActTuningStatusNow == true) {
    if(iCurrentStep <= 2) {
      iAdjGreenSec = smem.vGetActRunningGreenTime();
            //    } else if (iLastStep <= 2 && iCurrentStep >=3) {
            //      iAdjGreenSec = smem.vGetActRunningGreenTime();
    }
    //mod.
    if(iCurrentStep == 0 && iLastStep >=3) {
      if(iAdjGreenSec == 0) {
        bEnableModAdjGreenSec = true;
      } else {
        bEnableModAdjGreenSec = false;
      }
    }
    if(bEnableModAdjGreenSec && iCurrentStep <= 2) { iAdjGreenSec+=1; }

    printf("iAdjGreenSec:%d\n", iAdjGreenSec);
    siCurrentPhaseGreen = stc.vGetAdjCurrentPhaseGreenTime();
    printf("siCurrentPhaseGreen:%d\n", siCurrentPhaseGreen);
//  }

}catch(...){}
}

//---------------------------------------------------------------------------
void ScreenActuateArwenFlashManyStatus::vShowActTuningStatus(void)
{
try {
  int iHiTMP, iLowTMP,iMedianTMP;


  if(bActTuningStatusNow == true || RecordActTuningStatus==true ) {
    //actTuning
    lcd240x128.DISPLAY_GRAPHIC_XY(markActTuning[0].X,markActTuning[0].Y,word8x16[1],markActTuning[0].height,markActTuning[0].width/8);
  } else {
    lcd240x128.DISPLAY_GRAPHIC_XY(markActTuning[0].X,markActTuning[0].Y,word8x16[0],markActTuning[0].height,markActTuning[0].width/8);
  }
  for(int i=1;i<4;i++){
	  if((i-1) <=switch_times){
	  if(ilastbActTuningStatus[i-1]==true){
		  lcd240x128.DISPLAY_GRAPHIC_XY(markActTuning[i].X,markActTuning[i].Y,word8x16[1],markActTuning[i].height,markActTuning[i].width/8);
	  }else{
		  lcd240x128.DISPLAY_GRAPHIC_XY(markActTuning[i].X,markActTuning[i].Y,word8x16[0],markActTuning[i].height,markActTuning[i].width/8);
	  }
	  }
  }

  //siCurrentPhaseGreen

  if(siCurrentPhaseGreen>99){
      iHiTMP = siCurrentPhaseGreen / 100;
      iMedianTMP =(siCurrentPhaseGreen / 10) %10;
      iLowTMP = siCurrentPhaseGreen % 10;
  }else{
      iHiTMP = siCurrentPhaseGreen / 100;
      iMedianTMP =siCurrentPhaseGreen / 10;
      iLowTMP = siCurrentPhaseGreen % 10;
  }
  lcd240x128.DISPLAY_GRAPHIC_XY(markOrgGreenTime[0][0].X,markOrgGreenTime[0][0].Y,word8x16[iHiTMP],markOrgGreenTime[0][0].height,markOrgGreenTime[0][0].width/8);
  lcd240x128.DISPLAY_GRAPHIC_XY(markOrgGreenTime[0][1].X,markOrgGreenTime[0][1].Y,word8x16[iMedianTMP],markOrgGreenTime[0][1].height,markOrgGreenTime[0][1].width/8);
  lcd240x128.DISPLAY_GRAPHIC_XY(markOrgGreenTime[0][2].X,markOrgGreenTime[0][2].Y,word8x16[iLowTMP],markOrgGreenTime[0][2].height,markOrgGreenTime[0][2].width/8);
  //printf("show siCurrentPhaseGreen=%d iAdjGreenSec=%d\n",siCurrentPhaseGreen,iAdjGreenSec);
  for(int i=1;i<4;i++){
	  if(ilastPhaseGreen[i-1]>99){
          iHiTMP = ilastPhaseGreen[i-1] / 100;
          iMedianTMP =(ilastPhaseGreen[i-1] / 10) %10;
          iLowTMP = ilastPhaseGreen[i-1] % 10;
	  }else{
	      iHiTMP = ilastPhaseGreen[i-1] / 100;
          iMedianTMP =ilastPhaseGreen[i-1] / 10;
          iLowTMP = ilastPhaseGreen[i-1] % 10;
	  }

	  if(ilastPhaseGreen[i-1]>=0){
		  lcd240x128.DISPLAY_GRAPHIC_XY(markOrgGreenTime[i][0].X,markOrgGreenTime[i][0].Y,word8x16[iHiTMP],markOrgGreenTime[i][0].height,markOrgGreenTime[i][0].width/8);
		  lcd240x128.DISPLAY_GRAPHIC_XY(markOrgGreenTime[i][1].X,markOrgGreenTime[i][1].Y,word8x16[iMedianTMP],markOrgGreenTime[i][1].height,markOrgGreenTime[i][1].width/8);
		  lcd240x128.DISPLAY_GRAPHIC_XY(markOrgGreenTime[i][2].X,markOrgGreenTime[i][2].Y,word8x16[iLowTMP],markOrgGreenTime[i][2].height,markOrgGreenTime[i][2].width/8);
	  }
	  if(ilastGreenSec[i-1]>99){
          iHiTMP  = ilastGreenSec[i-1] / 100;
          iMedianTMP = (ilastGreenSec[i-1] /10) %10;
          iLowTMP = ilastGreenSec[i-1] % 10;
	  }else{
	      iHiTMP  = ilastGreenSec[i-1] / 100;
	      iMedianTMP = ilastGreenSec[i-1] /10;
	      iLowTMP = ilastGreenSec[i-1] % 10;
	  }
	  if(ilastGreenSec[i-1]>=0){
	  lcd240x128.DISPLAY_GRAPHIC_XY(markAdjGreenTime[i][0].X,markAdjGreenTime[i][0].Y,word8x16[iHiTMP],markAdjGreenTime[i][0].height,markAdjGreenTime[i][0].width/8);
	  lcd240x128.DISPLAY_GRAPHIC_XY(markAdjGreenTime[i][1].X,markAdjGreenTime[i][1].Y,word8x16[iMedianTMP],markAdjGreenTime[i][1].height,markAdjGreenTime[i][1].width/8);
	  lcd240x128.DISPLAY_GRAPHIC_XY(markAdjGreenTime[i][2].X,markAdjGreenTime[i][2].Y,word8x16[iLowTMP],markAdjGreenTime[i][2].height,markAdjGreenTime[i][2].width/8);
	  }
	  //printf("show ilastPhaseGreen[%d]=%d ilastGreenSec[%d]=%d\n",i-1,ilastPhaseGreen[i-1],i-1,ilastGreenSec[i-1]);
  }

  //if(RecordActTuningStatus ==true && bActTuningStatusNow==true){
  if(bActTuningStatusNow==true && bActTuningStatusLast==true && RecordActTuningStatus==true){
  //markALLGreenTime
	  for(int i=0;i<iCurrentSubphaseTotal;i++){
		  if(i<4){
		  iHiTMP  = siAdjALLGreen[i] / 100;
		  iMedianTMP =(siAdjALLGreen[i] / 10) % 10;
		  iLowTMP = siAdjALLGreen[i] % 10;
		  lcd240x128.DISPLAY_GRAPHIC_XY(markALLGreenTime[i][0].X,markALLGreenTime[i][0].Y,word8x16[iHiTMP],markALLGreenTime[i][0].height,markALLGreenTime[i][0].width/8);
		  lcd240x128.DISPLAY_GRAPHIC_XY(markALLGreenTime[i][1].X,markALLGreenTime[i][1].Y,word8x16[iMedianTMP],markALLGreenTime[i][1].height,markALLGreenTime[i][1].width/8);
		  lcd240x128.DISPLAY_GRAPHIC_XY(markALLGreenTime[i][2].X,markALLGreenTime[i][2].Y,word8x16[iLowTMP],markALLGreenTime[i][2].height,markALLGreenTime[i][2].width/8);
		  }
	  }
  }
  /*
  for(int i=1;i<4;i++){
	  if(ilastbActTuningStatus[i-1]==true){
		  iHiTMP  = ilastAdjMinGree[i-1] / 100;
		  iMedianTMP =(ilastAdjMinGree[i-1] / 10) % 10;
		  iLowTMP = ilastAdjMinGree[i-1] % 10;
		  if(ilastAdjMinGree[i-1]!=-1 && ilastAdjMaxGree[i-1]!=1 ){
		  lcd240x128.DISPLAY_GRAPHIC_XY(markALLGreenTime[i][0].X,markALLGreenTime[i][0].Y,word8x16[iHiTMP],markALLGreenTime[i][0].height,markALLGreenTime[i][0].width/8);
		  lcd240x128.DISPLAY_GRAPHIC_XY(markALLGreenTime[i][1].X,markALLGreenTime[i][1].Y,word8x16[iMedianTMP],markALLGreenTime[i][1].height,markALLGreenTime[i][1].width/8);
		  lcd240x128.DISPLAY_GRAPHIC_XY(markALLGreenTime[i][2].X,markALLGreenTime[i][2].Y,word8x16[iLowTMP],markALLGreenTime[i][2].height,markALLGreenTime[i][2].width/8);

		  iHiTMP  = ilastAdjMaxGree[i-1] / 100;
		  lcd240x128.DISPLAY_GRAPHIC_XY(markMaxGreenTime[i][0].X,markMaxGreenTime[i][i].Y,word8x16[iHiTMP],markMaxGreenTime[i][0].height,markMaxGreenTime[i][0].width/8);
		  iHiTMP = (ilastAdjMaxGree[i-1] /10) % 10;
		  lcd240x128.DISPLAY_GRAPHIC_XY(markMaxGreenTime[i][1].X,markMaxGreenTime[i][1].Y,word8x16[iHiTMP],markMaxGreenTime[i][1].height,markMaxGreenTime[i][1].width/8);
		  iLowTMP = ilastAdjMaxGree[i-1] % 10;
		  lcd240x128.DISPLAY_GRAPHIC_XY(markMaxGreenTime[i][2].X,markMaxGreenTime[i][2].Y,word8x16[iLowTMP],markMaxGreenTime[i][2].height,markMaxGreenTime[i][2].width/8);
		  }
	  }
  }
   */

  //iAdjGreenSec
if(iAdjGreenSec>99){
      iHiTMP = iAdjGreenSec / 100;
      iMedianTMP =(iAdjGreenSec / 10) %10;
      iLowTMP = iAdjGreenSec % 10;
  }else{
      iHiTMP = iAdjGreenSec / 100;
      iMedianTMP =iAdjGreenSec / 10;
      iLowTMP = iAdjGreenSec % 10;
  }
  lcd240x128.DISPLAY_GRAPHIC_XY(markAdjGreenTime[0][0].X,markAdjGreenTime[0][0].Y,word8x16[iHiTMP],markAdjGreenTime[0][0].height,markAdjGreenTime[0][0].width/8);
  lcd240x128.DISPLAY_GRAPHIC_XY(markAdjGreenTime[0][1].X,markAdjGreenTime[0][1].Y,word8x16[iMedianTMP],markAdjGreenTime[0][1].height,markAdjGreenTime[0][1].width/8);
  lcd240x128.DISPLAY_GRAPHIC_XY(markAdjGreenTime[0][2].X,markAdjGreenTime[0][2].Y,word8x16[iLowTMP],markAdjGreenTime[0][2].height,markAdjGreenTime[0][2].width/8);


  }catch(...){}
}




//---------------------------------------------------------------------------
void ScreenActuateArwenFlashManyStatus::initDispWord(void)
{
try {

//---------------
    for(int i = 0; i < 4; i++) {
          markSubPhase[i].width = 8;   markSubPhase[i].height = 16;
          markStep[i].width = 8;       markStep[i].height = 16;
    }

    for(int i = 0; i < 4; i++) {
      for(int j=0;j<3;j++){
      markOrgGreenTime[i][j].width = 8; markOrgGreenTime[i][j].height = 16;
      markAdjGreenTime[i][j].width = 8; markAdjGreenTime[i][j].height = 16;
      markALLGreenTime[i][j].width = 8; markALLGreenTime[i][j].height = 16;
      markMaxGreenTime[i][j].width = 8; markMaxGreenTime[i][j].height = 16;
      }
    }
    for(int i=0;i<3;i++){
    	markStepSecCount[i].width = 8; markStepSecCount[i].height = 16;
    }
    markActSwitch.width = 8;    markActSwitch.height = 16;
//---------------
    for(int i=0;i<3;i++){
    	OnlySubPhase[i].X= 130+(i*40);OnlySubPhase[i].Y=1;
    	OnlySubPhase[i].width=8;  OnlySubPhase[i].height =16;
    	OnlyStep[i].X= 146+(i*40) ;OnlyStep[i].Y=1;
    	OnlyStep[i].width=8;  OnlyStep[i].height =16;
    }
    mark2SubPhase.X=90;mark2SubPhase.Y=1;
    mark2Step.X=106;mark2Step.Y=1;
    mark2SubPhase.width=8;  mark2SubPhase.height =16;
    mark2Step.width=8;  mark2Step.height =16;



    markSubPhase[0].X = 56; markSubPhase[0].Y = 66;
    markSubPhase[1].X = 64; markSubPhase[1].Y = 66;
    markSubPhase[2].X = 80; markSubPhase[2].Y = 66;
    markSubPhase[3].X = 88; markSubPhase[3].Y = 66;


    markStep[0].X = 56; markStep[0].Y = 81;
    markStep[1].X = 64; markStep[1].Y = 81;
    markStep[2].X = 80; markStep[2].Y = 81;
    markStep[3].X = 88; markStep[3].Y = 81;

    markActSwitch.X = 56; markActSwitch.Y = 112;//觸動開關

    markStepSecCount[0].X = 56; markStepSecCount[0].Y = 96;
    markStepSecCount[1].X = 64; markStepSecCount[1].Y = 96;

    for(int i=0;i<4;i++){
    	markActTuning[i].X = 96+(i*40); markActTuning[i].Y = 48;//動態調整
    	markActTuning[i].width = 8;    markActTuning[i].height = 16;
    }

    for(int i=0;i<4;i++){
    markOrgGreenTime[i][0].X = 90+(i*40); markOrgGreenTime[i][0].Y = 32;//原綠
    markOrgGreenTime[i][1].X = 98+(i*40); markOrgGreenTime[i][1].Y = 32;
    markOrgGreenTime[i][2].X = 106+(i*40); markOrgGreenTime[i][2].Y = 32;

    markAdjGreenTime[i][0].X = 90+(i*40); markAdjGreenTime[i][0].Y = 16;//調綠
    markAdjGreenTime[i][1].X = 98+(i*40); markAdjGreenTime[i][1].Y = 16;
    markAdjGreenTime[i][2].X = 106+(i*40); markAdjGreenTime[i][2].Y = 16;

    markALLGreenTime[i][0].X = 112+(i*32); markALLGreenTime[i][0].Y = 96;
    markALLGreenTime[i][1].X = 120+(i*32); markALLGreenTime[i][1].Y = 96;
    markALLGreenTime[i][2].X = 128+(i*32); markALLGreenTime[i][2].Y = 96;
    /*
    markMaxGreenTime[i][0].X = 110+(i*32); markMaxGreenTime[i][0].Y = 64;
    markMaxGreenTime[i][1].X = 118+(i*32); markMaxGreenTime[i][1].Y = 64;
    markMaxGreenTime[i][2].X = 126+(i*32); markMaxGreenTime[i][2].Y = 64;
    */
    }






  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenActuateArwenFlashManyStatus::doKeyWork(BYTE key)
{
try {
    switch (key) {
        case 0x80:
          doKey0Work();
        break;
        case 0x81:
          doKey1Work();
        break;
        case 0x82:
          doKey2Work();
        break;
        case 0x83:
          doKey3Work();
        break;
        case 0x84:
          doKey4Work();
        break;
        case 0x85:
          doKey5Work();
        break;
        case 0x86:
          doKey6Work();
        break;
        case 0x87:
          doKey7Work();
        break;
        case 0x88:
          doKey8Work();
        break;
        case 0x89:
          doKey9Work();
        break;
        case 0x8A:
          doKeyAWork();
        break;
        case 0x8B:
          doKeyBWork();
        break;
        case 0x8C:
          doKeyCWork();
        break;
        case 0x8D:
          doKeyDWork();
        break;
        case 0x8E:
          doKeyEWork();
        break;
        case 0x8F:
          doKeyFWork();
        break;
        case 0x90:
          doKeyF1Work();
        break;
        case 0x91:
          doKeyF2Work();
        break;
        case 0x92:
          doKeyF3Work();
        break;
        case 0x93:
          doKeyF4Work();
        break;
        case 0x94:
          doKeyUPWork();
        break;
        case 0x95:
          doKeyDOWNWork();
        break;
        case 0x96:
          doKeyLEFTWork();
        break;
        case 0x97:
          doKeyRIGHTWork();
        break;
        case 0x98:
          doKeyEnterWork();
        break;
        default:
        break;
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenActuateArwenFlashManyStatus::doKey0Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenActuateArwenFlashManyStatus::doKey1Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenActuateArwenFlashManyStatus::doKey2Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenActuateArwenFlashManyStatus::doKey3Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenActuateArwenFlashManyStatus::doKey4Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenActuateArwenFlashManyStatus::doKey5Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenActuateArwenFlashManyStatus::doKey6Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenActuateArwenFlashManyStatus::doKey7Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenActuateArwenFlashManyStatus::doKey8Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenActuateArwenFlashManyStatus::doKey9Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenActuateArwenFlashManyStatus::doKeyAWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenActuateArwenFlashManyStatus::doKeyBWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenActuateArwenFlashManyStatus::doKeyCWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenActuateArwenFlashManyStatus::doKeyDWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenActuateArwenFlashManyStatus::doKeyEWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenActuateArwenFlashManyStatus::doKeyFWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenActuateArwenFlashManyStatus::doKeyF1Work(void)
{
//    screenLogo.DisplayLogo();
    screenMain.DisplayMain();
}
//---------------------------------------------------------------------------
void ScreenActuateArwenFlashManyStatus::doKeyF2Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenActuateArwenFlashManyStatus::doKeyF3Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenActuateArwenFlashManyStatus::doKeyF4Work(void)
{
//    screenMain.DisplayMain();
	screenActuateArwenMenu.DisplayActuateArwenMenu();
}
//---------------------------------------------------------------------------
void ScreenActuateArwenFlashManyStatus::doKeyUPWork(void)
{
try {
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenActuateArwenFlashManyStatus::doKeyDOWNWork(void)
{
try {
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenActuateArwenFlashManyStatus::doKeyLEFTWork(void)
{
try {
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenActuateArwenFlashManyStatus::doKeyRIGHTWork(void)
{
try {
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenActuateArwenFlashManyStatus::doKeyEnterWork(void)
{
    switch (cSelect+1) {
        case 1:
          doKey1Work();
        break;
        case 2:
          doKey2Work();
        break;
        case 3:
          doKey3Work();
        break;
        case 4:
          doKey4Work();
        break;
        case 5:
          doKey5Work();
        break;
        case 6:
          doKey6Work();
        break;
        case 7:
          doKey7Work();
        break;
        case 8:
          doKey8Work();
        break;
        case 9:
          doKey9Work();
        break;
        case 10:
          doKey0Work();
        break;
        default:
        break;
    }
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
void ScreenActuateArwenFlashManyStatus::vRefreshActuateTimes()
{
try {

	if(smem.GetcFace()==cACTUATEARWENFLASHMANYSTATUS){
		if((iCurrentSubphaseTotal > 1  && iCurrentStep == 0) ||iCurrentPlanID==0x28  ){
		if((RecordPhaseGreen !=siCurrentPhaseGreen) && (siCurrentPhaseGreen!=9999) && (RecordPhaseGreen!=9999)/*(iAdjGreenSec >=siCurrentPhaseGreen)*/ ) {
			printf("=====!!Change bActTuningStatusLast :%d PhaseGreen:%d RecordPhaseGreen:%d iAdjGreenSec:%d  RecordGreenSec:%d\n",bActTuningStatusLast,siCurrentPhaseGreen,RecordPhaseGreen,iAdjGreenSec,RecordGreenSec);
			//printf("=====AdjMinGree:%d RecordAdjMinGreen:%d AdjMaxGree:%d RecordAdjMinGreen:%d\n",siAdjMinGreen,RecordAdjMinGreen,siAdjMaxGreen,RecordAdjMaxGreen);
			if(switch_times <3){
				switch_times++;
			}else{
				switch_times=3;
			}

			int i=switch_times;
			if(switch_times==0){
				ilastSubphase[i]=RecordSubphase;
				ilastStep[i]=iCurrentStep;
				ilastPhaseGreen[i]=RecordPhaseGreen;//原綠
				ilastGreenSec[i]=RecordGreenSec;//目綠
				if(RecordActTuningStatus==true || bActTuningStatusNow==true){
					ilastbActTuningStatus[i]=true;
				}else{
					ilastbActTuningStatus[i]=false;
				}
				//ilastbActTuningStatus[i]=RecordActTuningStatus;
				//ilastAdjMinGree[i]=RecordAdjMinGreen;
				//ilastAdjMaxGree[i]=RecordAdjMaxGreen;
			}else if(switch_times==1){
				ilastSubphase[i]=ilastSubphase[i-1];
				ilastStep[i]=ilastStep[i-1];
				ilastPhaseGreen[i]=ilastPhaseGreen[i-1];//原綠
				ilastGreenSec[i]=ilastGreenSec[i-1];//目綠
				ilastbActTuningStatus[i]=ilastbActTuningStatus[i-1];
				//ilastAdjMinGree[i]=ilastAdjMinGree[i-1];
				//ilastAdjMaxGree[i]=ilastAdjMinGree[i-1];

				ilastSubphase[i-1]=RecordSubphase;
				ilastStep[i-1]=iCurrentStep;
				ilastPhaseGreen[i-1]=RecordPhaseGreen;//原綠
				ilastGreenSec[i-1]=RecordGreenSec;//目綠
				if(RecordActTuningStatus==true || bActTuningStatusNow==true){
					ilastbActTuningStatus[i-1]=true;
				}else{
					ilastbActTuningStatus[i-1]=false;
				}
				//ilastbActTuningStatus[i-1]=RecordActTuningStatus;
				//ilastAdjMinGree[i-1]=RecordAdjMinGreen;
				//ilastAdjMaxGree[i-1]=RecordAdjMaxGreen;

			}else if(switch_times==2){

				ilastSubphase[i]=ilastSubphase[i-1];
				ilastStep[i]=ilastStep[i-1];
				ilastPhaseGreen[i]=ilastPhaseGreen[i-1];//原綠
				ilastGreenSec[i]=ilastGreenSec[i-1];//目綠
				ilastbActTuningStatus[i]=ilastbActTuningStatus[i-1];
				//ilastAdjMinGree[i]=ilastAdjMinGree[i-1];
				//ilastAdjMaxGree[i]=ilastAdjMinGree[i-1];

				ilastSubphase[i-1]=ilastSubphase[i-2];
				ilastStep[i-1]=ilastStep[i-2];
				ilastPhaseGreen[i-1]=ilastPhaseGreen[i-2];//原綠
				ilastGreenSec[i-1]=ilastGreenSec[i-2];//目綠
				ilastbActTuningStatus[i-1]=ilastbActTuningStatus[i-2];
				//ilastAdjMinGree[i-1]=ilastAdjMinGree[i-2];
				//ilastAdjMaxGree[i-1]=ilastAdjMinGree[i-2];

				ilastSubphase[i-2]=RecordSubphase;
				ilastStep[i-2]=iCurrentStep;
				ilastPhaseGreen[i-2]=RecordPhaseGreen;//原綠
			    ilastGreenSec[i-2]=RecordGreenSec;//目綠
			    if(RecordActTuningStatus==true || bActTuningStatusNow==true){
			    	ilastbActTuningStatus[i-2]=true;
			    }else{
			    	ilastbActTuningStatus[i-2]=false;
			    }
			    //ilastbActTuningStatus[i-2]=RecordActTuningStatus;
			    //ilastAdjMinGree[i-2]=RecordAdjMinGreen;
			    //ilastAdjMaxGree[i-2]=RecordAdjMaxGreen;
			}else if(switch_times==3){

				ilastSubphase[i]=ilastSubphase[i-1];
				ilastStep[i]=ilastStep[i-1];
				ilastPhaseGreen[i]=ilastPhaseGreen[i-1];//原綠
				ilastGreenSec[i]=ilastGreenSec[i-1];//目綠
				ilastbActTuningStatus[i]=ilastbActTuningStatus[i-1];
				//ilastAdjMinGree[i]=ilastAdjMinGree[i-1];
				//ilastAdjMaxGree[i]=ilastAdjMinGree[i-1];

				ilastSubphase[i-1]=ilastSubphase[i-2];
				ilastStep[i-1]=ilastStep[i-2];
				ilastPhaseGreen[i-1]=ilastPhaseGreen[i-2];//原綠
				ilastGreenSec[i-1]=ilastGreenSec[i-2];//目綠
				ilastbActTuningStatus[i-1]=ilastbActTuningStatus[i-2];
				//ilastAdjMinGree[i-1]=ilastAdjMinGree[i-2];
				//ilastAdjMaxGree[i-1]=ilastAdjMinGree[i-2];

				ilastSubphase[i-2]=ilastSubphase[i-3];
				ilastStep[i-2]=ilastStep[i-3];
				ilastPhaseGreen[i-2]=ilastPhaseGreen[i-3];//原綠
			    ilastGreenSec[i-2]=ilastGreenSec[i-3];//目綠
			    ilastbActTuningStatus[i-2]=ilastbActTuningStatus[i-3];
			    //ilastAdjMinGree[i-2]=ilastAdjMinGree[i-3];
			    //ilastAdjMaxGree[i-2]=ilastAdjMinGree[i-3];

			    ilastSubphase[i-3]=RecordSubphase;
			    ilastStep[i-3]=iCurrentStep;
			    ilastPhaseGreen[i-3]=RecordPhaseGreen;//原綠
			    ilastGreenSec[i-3]=RecordGreenSec;//目綠
			    if(RecordActTuningStatus==true || bActTuningStatusNow==true){
			    	ilastbActTuningStatus[i-3]=true;
			   	}else{
			   		ilastbActTuningStatus[i-3]=false;
			   	}
			    //ilastbActTuningStatus[i-3]=RecordActTuningStatus;
			    //ilastAdjMinGree[i-3]=RecordAdjMinGreen;
			    //ilastAdjMaxGree[i-3]=RecordAdjMaxGreen;


			}
			RecordActTuningStatus=false;
		}
		RecordSubphase=iCurrentSubphase;
	}
	RecordPhaseGreen=siCurrentPhaseGreen;
	RecordGreenSec=iAdjGreenSec;
	//RecordAdjMinGreen=siAdjMinGreen;
	//RecordAdjMaxGreen=siAdjMaxGreen;

	}

} catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenActuateArwenFlashManyStatus::vDisplayActionChange(void)
{
	printf("--->>bActTuningStatusNow :%d  bActTuningStatusLast:%d RecordActTuningStatus:%d\n",bActTuningStatusNow,bActTuningStatusLast,RecordActTuningStatus);
	printf("showopen:%d showclose:%d \n",showopen,showclose);
	//if(RecordActTuningStatus==true && bActTuningStatusLast==true){
	if(bActTuningStatusNow==true && bActTuningStatusLast==true && RecordActTuningStatus==true){
			if(showopen==false){
			lcd240x128.DISPLAY_GRAPHIC(0,ucActuateArwenBitmap_open,128,30);
			 showopen=true;
			 showclose=false;
			}
	//}else if(RecordActTuningStatus==false && bActTuningStatusLast==true){
	}else if(bActTuningStatusNow==false && bActTuningStatusLast==false && RecordActTuningStatus==true){

		if(showclose==false){
			lcd240x128.DISPLAY_GRAPHIC(0,ucActuateArwenBitmap_close,128,30);
			showopen=false;
			showclose=true;
		}
	}
}

