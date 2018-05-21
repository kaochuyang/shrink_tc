#include "screenActuateArwenStatus.h"
#include "LCD240x128.h"

#include "SCREENMain.h"
#include "screenRunTableList.h"
#include "CSTC.h"

#include <stdio.h>

#include "WRITEJOB.h"
#include "CDataToMessageOK.h"

#include "SMEM.h"
//---------------------------------------------------------------------------
ScreenActuateArwenStatus screenActuateArwenStatus;
//---------------------------------------------------------------------------
ScreenActuateArwenStatus::ScreenActuateArwenStatus(void)
{
    loadBitmapFromFile();
    initDispWord();
    cSelect=0;
    fixedCurrentGreen=0;
    fixedMinGreen=0;
    fixedMaxGreen=0;
    fixedsta = false;
    checkiLastSubphase=0;
}
//---------------------------------------------------------------------------
ScreenActuateArwenStatus::~ScreenActuateArwenStatus(void)
{
}
//---------------------------------------------------------------------------
void ScreenActuateArwenStatus::loadBitmapFromFile(void)
{
try {
    FILE *bitmap;
    bitmap=fopen("//cct//bitmap//backGround//ActuateArwenManyStatusBitmap.bit","rb");
    if (bitmap) {
        fread(ucActuateArwenBitmap,3840,1,bitmap);
        fclose(bitmap);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenActuateArwenStatus::DisplayActuateArwenStatus(void)
{
try {
    smem.SetcFace(cACTUATEARWENSTATUS);
    lcd240x128.DISPLAY_GRAPHIC(0,ucActuateArwenBitmap,128,30);

    vRefreshActuateArwenStatusData();
    vRefreshActSwitch();
    vShowToPanel();

    vShowActTuningStatus();
//?    vRefreshStepSec();

  } catch (...) {}
}

//---------------------------------------------------------------------------  //not used
void ScreenActuateArwenStatus::vRefreshEverySec(void)
{
try {
  int iTmpFace = smem.GetcFace();
  if( smem.vGetUCData(TC_CCT_Send_ActuateArwen_Protocal) > 0 || iTmpFace == cACTUATEARWENSTATUS) {
    vRefreshActuateArwenStatusData();
    vRefreshActSwitch();
  }

  if(iTmpFace == cACTUATEARWENSTATUS) {
    vShowToPanel();
    vShowActTuningStatus();
  }

  if(smem.vGetUCData(TC_CCT_Send_ActuateArwen_Protocal) > 0) {
    vSendCCTProtocol5F81ActArwen();
  }
}catch(...){}
}

//----------------------------------------------------------
void ScreenActuateArwenStatus::vSetFixedGreen(int tmpGreen,int tmpMinGreen,int tmpMaxGreen)
{
try {
    fixedCurrentGreen = tmpGreen;
    fixedMinGreen = tmpMinGreen;
    fixedMaxGreen = tmpMaxGreen;
    fixedsta = true;

} catch (...) {}
}
//----------------------------------------------------------
void ScreenActuateArwenStatus::vGetFixedGreen(int &tmpGreen,int &tmpMinGreen,int &tmpMaxGreen)
{
try {
    if(fixedsta){
    tmpGreen = fixedCurrentGreen;
    tmpMinGreen = fixedMinGreen;
    tmpMaxGreen = fixedMaxGreen;
    }else{
    tmpGreen = 0;
    tmpMinGreen = 0;
    tmpMaxGreen = 0;
    }

} catch (...) {}
}

//----------------------------------------------------------
void ScreenActuateArwenStatus::vSendCCTProtocol5F81ActArwen(void)
{
try {
  unsigned char data[32];

/*
unsigned short int usiStepSec;
unsigned short int iCurrentPhaseID;
unsigned short int iCurrentPlanID;
int iCurrentSubphase;
int iCurrentSubphaseTotal;
int iCurrentStep;
int iLastStep;
int iCurrentStepTotal;
int iAdjGreenSec;
bool bEnableModAdjGreenSec;
unsigned char ucTCControlStrategy;
bool bActSwitch;
bool bActTuningStatusNow;
bool bActTuningStatusLast;
short int siAdjMaxGreen;
short int siAdjMinGreen;
short int siCurrentPhaseGreen;
*/
  printf("vSendCCTProtocol5F81ActArwen CurrentSubphase:%d CurrentSubphaseTotal:%d iCurrentStep:%d iCurrentStepTotal:%d usiStepSec:%d\n",iCurrentSubphase,iCurrentSubphaseTotal,iCurrentStep,iCurrentStepTotal,usiStepSec);
  printf("vSendCCTProtocol5F81ActArwen iAdjGreenSec:%d siAdjMinGreen:%d siAdjMaxGreen:%d\n",iAdjGreenSec,siAdjMinGreen,siAdjMaxGreen);
  data[0] = 0x5F;
  data[1] = 0x81;
  data[2] = iCurrentSubphase;
  data[3] = iCurrentSubphaseTotal;
  data[4] = iCurrentStep;
  data[5] = iCurrentStepTotal;
  data[6] = usiStepSec;
  data[7] = bActSwitch;
  data[8] = bActTuningStatusNow;
  data[9] = siCurrentPhaseGreen;
  data[10] = iAdjGreenSec;
  data[11] = siAdjMinGreen;
  data[12] = HI(siAdjMaxGreen);
  data[13] = LO(siAdjMaxGreen);
  if(smem.vGetUCData(fixedGreenModeOpen)>0){//固定綠
    if(fixedsta){
        data[14] = 0x01;
    }else{
        data[14] = 0x00;
    }
  }else{//半觸動
    data[14] = 0x01;
  }

  MESSAGEOK _MsgOK;
  _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, 15, true);
  _MsgOK.InnerOrOutWard = cOutWard;
  writeJob.WritePhysicalOutNoSetSeqNoResend(_MsgOK.packet, _MsgOK.packetLength, DEVICECENTER92);
  /**/

  /**/

} catch (...) {}
}



//---------------------------------------------------------------------------
void ScreenActuateArwenStatus::vShowToPanel(void)
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
void ScreenActuateArwenStatus::vRefreshActuateArwenStatusData(void)
{
try {

  iCurrentPhaseID  = stc.vGetUSIData(CSTC_exec_plan_phase_order);
  iCurrentPlanID   = stc.vGetUSIData(CSTC_exec_plan_plan_ID);

//  stc.Lock_to_Load_Segment_for_Panel(iCurrentSegNo);
  stc.Lock_to_Load_Plan_for_Panel(iCurrentPlanID);
  stc.Lock_to_Load_Phase_for_Panel(iCurrentPhaseID);

  iCurrentSubphase = stc.vGetUSIData(CSTC_exec_phase_current_subphase);
  /*arwen 1000926 Add*/
  if(iCurrentSubphase!=checkiLastSubphase){
      fixedsta=false;
  }
  checkiLastSubphase=iCurrentSubphase;
  /*arwen 1000926 End*/

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

//  int iCurrentSegNoTotal = stc._panel_segment._segment_count;
//  int iCurrentSegTypeID = stc._panel_segment._segment_type;


} catch(...){}
}


//---------------------------------------------------------------------------
void ScreenActuateArwenStatus::vRefreshActSwitch(void)
{
try {
//OT20111128
  int iTmpFace = smem.GetcFace();

  if(ucTCControlStrategy == 0x01 &&  smem.vGetBOOLData(TC_CCT_In_LongTanu_ActuateType_FunctionEnable) == true ) {  //0x01 = TOD
    bActSwitch = true;
    if(iTmpFace == cACTUATEARWENSTATUS) {
      lcd240x128.DISPLAY_GRAPHIC_XY(markActSwitch.X,markActSwitch.Y,word8x16[1],markActSwitch.height,markActSwitch.width/8);
    }
  } else {
    bActSwitch = false;
    if(iTmpFace == cACTUATEARWENSTATUS) {
      lcd240x128.DISPLAY_GRAPHIC_XY(markActSwitch.X,markActSwitch.Y,word8x16[0],markActSwitch.height,markActSwitch.width/8);
    }
  }

}catch(...){}
}

//---------------------------------------------------------------------------
void ScreenActuateArwenStatus::vCalActTuningStatus(void)
{
try {
  int iCurrentStep = stc.vGetUSIData(CSTC_exec_phase_current_subphase_step);    //haven show above
  bActTuningStatusLast = bActTuningStatusNow;
  if(bActSwitch == true && smem.vGetThisCycleRunCCJActure5FA2() == true && iCurrentStep <= 2) {
    bActTuningStatusNow = true;
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

    if(smem.vGetUCData(fixedGreenModeOpen)>0){//固定綠
        int tmpGreen,tmpMinGreen,tmpMaxGreen;
        vGetFixedGreen(tmpGreen,tmpMinGreen,tmpMaxGreen);
        printf("fixed arwen iAdjGreenSec:%d\n", iAdjGreenSec);
        siAdjMaxGreen = tmpMaxGreen;
        printf("fixed arwen siAdjMaxGreen:%d\n", siAdjMaxGreen);
        siAdjMinGreen = tmpMinGreen;
        printf("fixed arwen siAdjMinGreen:%d\n", siAdjMinGreen);
        siCurrentPhaseGreen = tmpGreen;
        printf("fixed arwen siCurrentPhaseGreen:%d\n", siCurrentPhaseGreen);

    }else{
        printf("iAdjGreenSec:%d\n", iAdjGreenSec);
        siAdjMaxGreen = stc.vGetAdjCurrentMaxGreen();
        printf("siAdjMaxGreen:%d\n", siAdjMaxGreen);
        siAdjMinGreen = stc.vGetAdjCurrentMinGreen();
        printf("siAdjMinGreen:%d\n", siAdjMinGreen);
        siCurrentPhaseGreen = stc.vGetAdjCurrentPhaseGreenTime();
        printf("siCurrentPhaseGreen:%d\n", siCurrentPhaseGreen);
    }

//  }

}catch(...){}
}

//---------------------------------------------------------------------------
void ScreenActuateArwenStatus::vShowActTuningStatus(void)
{
try {
  int iHiTMP, iLowTMP;

  if(bActTuningStatusNow == true ) {
    //actTuning
    lcd240x128.DISPLAY_GRAPHIC_XY(markActTuning.X,markActTuning.Y,word8x16[1],markActTuning.height,markActTuning.width/8);
  } else {
    lcd240x128.DISPLAY_GRAPHIC_XY(markActTuning.X,markActTuning.Y,word8x16[0],markActTuning.height,markActTuning.width/8);
  }

  //siCurrentPhaseGreen
  iHiTMP = siCurrentPhaseGreen / 10;
  iLowTMP = siCurrentPhaseGreen % 10;
  lcd240x128.DISPLAY_GRAPHIC_XY(markOrgGreenTime[0].X,markOrgGreenTime[0].Y,word8x16[iHiTMP],markOrgGreenTime[0].height,markOrgGreenTime[0].width/8);
  lcd240x128.DISPLAY_GRAPHIC_XY(markOrgGreenTime[1].X,markOrgGreenTime[1].Y,word8x16[iLowTMP],markOrgGreenTime[1].height,markOrgGreenTime[1].width/8);

  //markMinGreenTime
  iHiTMP  = siAdjMinGreen / 10;
  iLowTMP = siAdjMinGreen % 10;
  lcd240x128.DISPLAY_GRAPHIC_XY(markMinGreenTime[0].X,markMinGreenTime[0].Y,word8x16[iHiTMP],markMinGreenTime[0].height,markMinGreenTime[0].width/8);
  lcd240x128.DISPLAY_GRAPHIC_XY(markMinGreenTime[1].X,markMinGreenTime[1].Y,word8x16[iLowTMP],markMinGreenTime[1].height,markMinGreenTime[1].width/8);

  //markMaxGreenTime
  iHiTMP = siAdjMaxGreen / 100;
  lcd240x128.DISPLAY_GRAPHIC_XY(markMaxGreenTime[0].X,markMaxGreenTime[0].Y,word8x16[iHiTMP],markMaxGreenTime[0].height,markMaxGreenTime[0].width/8);
  iHiTMP = (siAdjMaxGreen / 10) % 10;
  lcd240x128.DISPLAY_GRAPHIC_XY(markMaxGreenTime[1].X,markMaxGreenTime[1].Y,word8x16[iHiTMP],markMaxGreenTime[1].height,markMaxGreenTime[1].width/8);
  iLowTMP = siAdjMaxGreen % 10;
  lcd240x128.DISPLAY_GRAPHIC_XY(markMaxGreenTime[2].X,markMaxGreenTime[2].Y,word8x16[iLowTMP],markMaxGreenTime[2].height,markMaxGreenTime[2].width/8);

  //iAdjGreenSec
  iHiTMP  = iAdjGreenSec / 10;
  iLowTMP = iAdjGreenSec % 10;
  lcd240x128.DISPLAY_GRAPHIC_XY(markAdjGreenTime[0].X,markAdjGreenTime[0].Y,word8x16[iHiTMP],markAdjGreenTime[0].height,markAdjGreenTime[0].width/8);
  lcd240x128.DISPLAY_GRAPHIC_XY(markAdjGreenTime[1].X,markAdjGreenTime[1].Y,word8x16[iLowTMP],markAdjGreenTime[1].height,markAdjGreenTime[1].width/8);


  }catch(...){}
}




//---------------------------------------------------------------------------
void ScreenActuateArwenStatus::initDispWord(void)
{
try {

//---------------
    for(int i = 0; i < 4; i++) {
          markSubPhase[i].width = 8;   markSubPhase[i].height = 16;
          markStep[i].width = 8;       markStep[i].height = 16;
    }

    for(int i = 0; i < 2; i++) {
      markStepSecCount[i].width = 8; markStepSecCount[i].height = 16;
      markOrgGreenTime[i].width = 8; markOrgGreenTime[i].height = 16;
      markAdjGreenTime[i].width = 8; markAdjGreenTime[i].height = 16;
      markMinGreenTime[i].width = 8; markMinGreenTime[i].height = 16;
    }
    for(int i = 0; i < 3; i++) {
      markMaxGreenTime[i].width = 8; markMaxGreenTime[i].height = 16;
    }

    markActSwitch.width = 8;    markActSwitch.height = 16;
    markActTuning.width = 8;    markActTuning.height = 16;

//---------------
    markSubPhase[0].X = 64; markSubPhase[0].Y = 8;
    markSubPhase[1].X = 72; markSubPhase[1].Y = 8;
    markSubPhase[2].X = 88; markSubPhase[2].Y = 8;
    markSubPhase[3].X = 96; markSubPhase[3].Y = 8;

    markStep[0].X = 160; markStep[0].Y = 8;
    markStep[1].X = 168; markStep[1].Y = 8;
    markStep[2].X = 184; markStep[2].Y = 8;
    markStep[3].X = 192; markStep[3].Y = 8;

    markActSwitch.X = 64; markActSwitch.Y = 28;
    markStepSecCount[0].X = 184; markStepSecCount[0].Y = 28;
    markStepSecCount[1].X = 192; markStepSecCount[1].Y = 28;
    markActTuning.X = 72; markActTuning.Y = 46;
    markOrgGreenTime[0].X = 72; markOrgGreenTime[0].Y = 68;
    markOrgGreenTime[1].X = 80; markOrgGreenTime[1].Y = 68;
    markAdjGreenTime[0].X = 184; markAdjGreenTime[0].Y = 68;
    markAdjGreenTime[1].X = 192; markAdjGreenTime[1].Y = 68;
    markMinGreenTime[0].X = 72; markMinGreenTime[0].Y = 88;
    markMinGreenTime[1].X = 80; markMinGreenTime[1].Y = 88;
    markMaxGreenTime[0].X = 152; markMaxGreenTime[0].Y = 88;
    markMaxGreenTime[1].X = 160; markMaxGreenTime[1].Y = 88;
    markMaxGreenTime[2].X = 168; markMaxGreenTime[2].Y = 88;



/*
DISP_WORD markSubPhase[4];
DISP_WORD markStep[4];
DISP_WORD markActSwitch;
DISP_WORD markStepSecCount[2];
DISP_WORD markActTuning;
DISP_WORD markOrgGreenTime[2];
DISP_WORD markAdjGreenTime[2];
...
*/


  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenActuateArwenStatus::doKeyWork(BYTE key)
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
void ScreenActuateArwenStatus::doKey0Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenActuateArwenStatus::doKey1Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenActuateArwenStatus::doKey2Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenActuateArwenStatus::doKey3Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenActuateArwenStatus::doKey4Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenActuateArwenStatus::doKey5Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenActuateArwenStatus::doKey6Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenActuateArwenStatus::doKey7Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenActuateArwenStatus::doKey8Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenActuateArwenStatus::doKey9Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenActuateArwenStatus::doKeyAWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenActuateArwenStatus::doKeyBWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenActuateArwenStatus::doKeyCWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenActuateArwenStatus::doKeyDWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenActuateArwenStatus::doKeyEWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenActuateArwenStatus::doKeyFWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenActuateArwenStatus::doKeyF1Work(void)
{
//    screenLogo.DisplayLogo();
    screenMain.DisplayMain();
}
//---------------------------------------------------------------------------
void ScreenActuateArwenStatus::doKeyF2Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenActuateArwenStatus::doKeyF3Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenActuateArwenStatus::doKeyF4Work(void)
{
//    screenMain.DisplayMain();
    screenRunTableList.DisplayRunTableList();
}
//---------------------------------------------------------------------------
void ScreenActuateArwenStatus::doKeyUPWork(void)
{
try {
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenActuateArwenStatus::doKeyDOWNWork(void)
{
try {
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenActuateArwenStatus::doKeyLEFTWork(void)
{
try {
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenActuateArwenStatus::doKeyRIGHTWork(void)
{
try {
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenActuateArwenStatus::doKeyEnterWork(void)
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
