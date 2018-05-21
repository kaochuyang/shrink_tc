// vGet0FCommandSet
// 0: Basic (wireless)
// 1: Base
// 2: Basic & Advance
// 3: Basic & Advance & Option

//---------------------------------------------------------------------------
#include "PTRAFFICAMEGIDS.h"

#include "CDataToMessageOK.h"
#include "WRITEJOB.h"
#include "CTIMER.h"
#include "DIGITALIO.h"
#include "CHexToInt.h"
#include "CSTC.h"
#include "CTools.h"

#include "SCREENLast92TCPlanSegmentUpdate.h"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <error.h>

#include "CTIMER.h"

//---------------------------------------------------------------------------
PTRAFFICAMEGIDS::PTRAFFICAMEGIDS(void)
{
}
//---------------------------------------------------------------------------
PTRAFFICAMEGIDS::~PTRAFFICAMEGIDS(void)
{
}
//---------------------------------------------------------------------------
bool PTRAFFICAMEGIDS::DoWorkViaPTraffic(MESSAGEOK message)
{
try {

      switch (message.packet[8]) {
        case 0x10:
          vWriteControlStrategy5F10(message);
          break;

        case 0x40:
          vQurryControlStrategy5F40(message);
          break;

        case 0x44:
          vQueryPlan5F44(message);
          break;

        case 0x18:
          vWriteRunPlan5F18(message);
          break;

        case 0x48:
          vQueryRunPlan5F48(message);
          break;

        case 0x3F:
          vTransmitCycleSetting_5F3F(message);
          break;

        case 0x1C:
          vGoToNextPhaseStepControl_5F1C(message);
          break;

        case 0x4C:
          vQueryPhaseStepControl_5F4C(message);
          break;

//主動回報
        case 0x00:
        case 0x03:
        case 0x0C:
        case 0x08:
        case 0x0A:
        case 0x0B:
        case 0x0F:
          writeJob.WritePhysicalOut(message.packet, message.packetLength, DEVICEAMEGIDS);
        break;

        default:
          vReturnToCenterNACK(message.packet[7], message.packet[8], 0x0, 0x0);
        break;

      }

    return true;

  } catch (...) {}
}
//---------------------------------------------------------------------------
bool PTRAFFICAMEGIDS::vWriteControlStrategy5F10(MESSAGEOK DataMessageIn)
{
try{
  DATA_Bit _ControlStrategy;
  int EffectTime;

  unsigned char data[4];
  MESSAGEOK _MsgOK;

//  _ControlStrategy.DBit = smem.vGetUCData(TC92_ucControlStrategy);              //Read Data from share memory
//  EffectTime = smem.vGetINTData(TC92_iEffectTime);
  int iReturnCommandSet = smem.vGet0FCommandSet();                              //Get Oper Level

  if(iReturnCommandSet <= 1) {
    vReturnToCenterNACK(0x5F, 0x10, 0x80, 0x00);
    return false;
  } // not include level "A""
  if( DataMessageIn.packetLength < 14 ) {
    vReturnToCenterNACK(0x5F, 0x10, 0x08, 0x00);
    return false;
  }
  else if( DataMessageIn.packetLength > 14 ) {
    vReturnToCenterNACK(0x5F, 0x10, 0x08, DataMessageIn.packetLength - 12);
    return false;
  }
  vReturnToCenterACK(0x5F, 0x10);
  vReturnToCenter92ACK(0x5F, 0x10);

  _ControlStrategy.DBit = DataMessageIn.packet[9];
  EffectTime = DataMessageIn.packet[10] * 60;                                   //EffectTime save using Sec, but protocal using Min.

  data[0] = 0x5F;
  data[1] = 0x91;
  data[2] = DataMessageIn.packet[9];
  data[3] = DataMessageIn.packet[10];

  smem.vSetUCData(TC92_ucControlStrategy, _ControlStrategy.DBit);
  smem.vSetINTData(TC92_iEffectTime, EffectTime);
  //then save!?

/* 控制策略不被改變
  if(_ControlStrategy.switchBit.b2 == true) {
    stc.Lock_to_Set_Control_Strategy(STRATEGY_CADC);
    return true;
  }
  if(_ControlStrategy.switchBit.b1 == true) {
    stc.Lock_to_Set_Control_Strategy(STRATEGY_TOD);
    return true;
  }
*/

//For disable Dynamic

  //OT20111128
  //if(MACHINELOCATE == MACHINELOCATEATYULIN)                                     // For CCJ
  //OT20111128
  // if(smem.vGetUCData(TC_CCT_MachineLocation) == MACHINELOCATEATYULIN)
  if(smem.vGetUCData(TC_CCT_MachineLocation) == MACHINELOCATEATYULIN)
  {

    _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, 4, true);
    _MsgOK.InnerOrOutWard = cOutWard;
    writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICECCJDYNCTL);

  } else {

    if(_ControlStrategy.switchBit.b1 == true &&
       _ControlStrategy.switchBit.b5 == false &&
       _ControlStrategy.switchBit.b6 == false    )
    {
      if( stc.Lock_to_LoadControlStrategy() == STRATEGY_ALLDYNAMIC )
        _intervalTimer.vAllDynamicToTODCount(10);
  //      _intervalTimer.vAllDynamicToTODCount(EffectTime);

    }

    else if(_ControlStrategy.DBit == 0x30)
    {
      printf("Get Amegids 0F 10 30\n\n");
      if( stc.Lock_to_LoadControlStrategy() == STRATEGY_TOD ) {
        printf("Get Amegids, now is TOD\n\n");
        stc.Lock_to_Set_Control_Strategy(STRATEGY_ALLDYNAMIC);
        printf("Get Amegids, now changing to ALLDYNAMIC\n\n");

        int usiCurrentSubphaseStep = stc.vGetUSIData(CSTC_exec_phase_current_subphase_step);
        if(usiCurrentSubphaseStep == 0) {
          if(Protocal5F1CStopStep == usiCurrentSubphaseStep) {
            _intervalTimer.vAllDynamicToTODCount(EffectTime);
          }
          else {
            _intervalTimer.vAllDynamicToTODCount(stc.vGetUSIData(CSTC_exec_plan_green_time));
          }
        }
        else if(usiCurrentSubphaseStep == 1)
          _intervalTimer.vAllDynamicToTODCount(stc.vGetUSIData(CSTC_exec_plan_pedgreenflash_time));
        else if(usiCurrentSubphaseStep == 2)
          if(Protocal5F1CStopStep == usiCurrentSubphaseStep) {
            _intervalTimer.vAllDynamicToTODCount(EffectTime);
          }
          else {
            _intervalTimer.vAllDynamicToTODCount(stc.vGetUSIData(CSTC_exec_plan_pedred_time));
          }
        else if(usiCurrentSubphaseStep == 3)
          _intervalTimer.vAllDynamicToTODCount(stc.vGetUSIData(CSTC_exec_plan_yellow_time));
        else if(usiCurrentSubphaseStep == 4)
          _intervalTimer.vAllDynamicToTODCount(stc.vGetUSIData(CSTC_exec_plan_allred_time));

      }
    }
  }

  return true;

} catch(...) {}
}

//---------------------------------------------------------------------------
bool PTRAFFICAMEGIDS::vQurryControlStrategy5F40(MESSAGEOK DataMessageIn)
{
try{
  DATA_Bit _ControlStrategy;
  int EffectTime;
  int iReturnCommandSet;

  _ControlStrategy.DBit = smem.vGetUCData(TC92_ucControlStrategy);              //Read Data from share memory
  EffectTime = smem.vGetINTData(TC92_iEffectTime);
  iReturnCommandSet = smem.vGet0FCommandSet();                              //Get Oper Level

  if(iReturnCommandSet <= 1) { vReturnToCenterNACK(0x5F, 0x40, 0x80, 0x00); return false; } // not include level "A""
  if( DataMessageIn.packetLength < 12 ) { vReturnToCenterNACK(0x5F, 0x40, 0x08, 0x00); return false; }
  else if( DataMessageIn.packetLength > 12 ) { vReturnToCenterNACK(0x5F, 0x40, 0x08, DataMessageIn.packetLength - 12); return false; }
//Don't respond  vReturnToCenterACK(0x5F, 0x40);

  unsigned char data[6];
  data[0] = 0x5F;
  data[1] = 0xC0;
  data[2] = _ControlStrategy.DBit;
  data[3] = EffectTime / 60;

  MESSAGEOK _MsgOK;
  _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, 4, true);
  _MsgOK.InnerOrOutWard = cOutWard;
//  writeJob.WriteWorkByMESSAGEOUT(_MsgOK);
  writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICEAMEGIDS);
  return true;

} catch(...) {}
}

//---------------------------------------------------------------------------
bool PTRAFFICAMEGIDS::vQueryPlan5F44(MESSAGEOK DataMessageIn)
{
try{

  int iQueryPlan = DataMessageIn.packet[9];

  bool bPlanStatus;
  bPlanStatus = smem.vGetTCPhasePlanSegTypeData(TC_Plan, iQueryPlan);
  if(bPlanStatus == false) {
    vReturnToCenterNACK(0x5F, 0x44, 0x02, 0x0); return false;
  }
  if( DataMessageIn.packetLength < 13) { vReturnToCenterNACK(0x5F, 0x44, 0x08, 0x00); return false; }
  else if( DataMessageIn.packetLength > 13) { vReturnToCenterNACK(0x5F, 0x44, 0x08, DataMessageIn.packetLength - 12); return false; }


  stc.Lock_to_Load_Plan_for_Center(iQueryPlan);

  unsigned char data[256];
  int iDataPtr = 4; //Form 6 Start to send SignalStatus
  data[0] = 0x5F;
  data[1] = 0xC4;
  data[2] = stc._for_center_plan._planid;
  data[3] = stc._for_center_plan._subphase_count;
  for(int i = 0; i < stc._for_center_plan._subphase_count; i++)
  {
    data[iDataPtr] = stc._for_center_plan._ptr_subplaninfo[i]._min_green;      iDataPtr++;
//OT Debug 950410  -2 line
    data[iDataPtr] = stc._for_center_plan._ptr_subplaninfo[i]._max_green / 256;      iDataPtr++;
    data[iDataPtr] = stc._for_center_plan._ptr_subplaninfo[i]._max_green % 256;      iDataPtr++;
    data[iDataPtr] = stc._for_center_plan._ptr_subplaninfo[i]._yellow;         iDataPtr++;
    data[iDataPtr] = stc._for_center_plan._ptr_subplaninfo[i]._allred;         iDataPtr++;
    data[iDataPtr] = stc._for_center_plan._ptr_subplaninfo[i]._pedgreen_flash; iDataPtr++;
    data[iDataPtr] = stc._for_center_plan._ptr_subplaninfo[i]._pedred;         iDataPtr++;
  }

  MESSAGEOK _MsgOK;
  _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, iDataPtr, true);
  _MsgOK.InnerOrOutWard = cOutWard;
//  writeJob.WriteWorkByMESSAGEOUT(_MsgOK);
  writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICEAMEGIDS);

  return true;

} catch (...) {}
}

//---------------------------------------------------------------------------
bool PTRAFFICAMEGIDS::vWritePlanDBMS5F15(MESSAGEOK DataMessageIn)
{
try{

  int iPhaseOrder = DataMessageIn.packet[11];
  int iSubphaseCount = DataMessageIn.packet[12];

  if(smem.vGetTCPhasePlanSegTypeData(TC_Phase, iPhaseOrder) == false) {
    vReturnToCenterNACK(0x5F, 0x15, 0x04, 0x03); return false;
  }
  stc.Lock_to_Load_Phase_for_Center(iPhaseOrder);

  printf("5F15: stc._for_center_phase._subphase_count:%d\n", stc._for_center_phase._subphase_count);
  printf("5F15: iSubphaseCount:%d\n", iSubphaseCount);

  if( stc._for_center_phase._subphase_count != iSubphaseCount && iSubphaseCount != 0) {
    vReturnToCenterNACK(0x5F, 0x15, 0x04, 0x04); return false;
  }

  if( DataMessageIn.packetLength < 20+(2*DataMessageIn.packet[12])) { vReturnToCenterNACK(0x5F, 0x15, 0x08, 0x00); return false; }
  else if( DataMessageIn.packetLength > 20+(2*DataMessageIn.packet[12])) { vReturnToCenterNACK(0x5F, 0x15, 0x08, DataMessageIn.packetLength - 12); return false; }

//OT Debug 0523
//  vReturnToCenterACK(0x5F, 0x15);

  /*
  PlanID(時制計劃):[9]
  Direct(基準方向):[10]
  PhaseOrder(時相編號):[11]
  SubPhaseCount(分相數):[12]

  Green(綠燈時間):[13][14],[15][16],[17][18]
  CycleTime(週期):[19][20]
  Offset(時差):[21][22]
  */

  int i=0;
  bool bMollacOK=false;

  //OT Debug 0523
  bool bRet;

  if (iSubphaseCount == 0) {    //閃光時相
    bMollacOK = stc.Lock_to_Reset_Plan_for_Center(DataMessageIn.packet[9],1);
  }
  else {
    bMollacOK = stc.Lock_to_Reset_Plan_for_Center(DataMessageIn.packet[9], iSubphaseCount);
  }

  if (bMollacOK) {

      stc._for_center_plan._planid=DataMessageIn.packet[9];
      stc._for_center_plan._dir=DataMessageIn.packet[10];
      printf("Plan:%d,Dir:%d\n",stc._for_center_plan._planid,stc._for_center_plan._dir);

/*
      if (iPhaseOrder == 0x30) iPhaseOrder = 0x20;
      else if (iPhaseOrder==0xB0) iPhaseOrder = 0x80;
*/

      stc._for_center_plan._phase_order = iPhaseOrder;
      stc._for_center_plan._subphase_count= iSubphaseCount;
      printf("Phase_Order:%d,Phase_Count:%d\n",stc._for_center_plan._phase_order,stc._for_center_plan._subphase_count);

      for (i=0;i<stc._for_center_plan._subphase_count;i++) {
           printf("Phase:%d\n",i+1);
           stc._for_center_plan._ptr_subplaninfo[i]._green=DataMessageIn.packet[13+2*i]*256+DataMessageIn.packet[14+2*i];
           printf("Green-A:%d\n",stc._for_center_plan._ptr_subplaninfo[i]._green);
           if(stc._for_center_plan._ptr_subplaninfo[i]._pedgreen_flash > 0 && stc._for_center_plan._ptr_subplaninfo[i]._pedgreen_flash < stc._for_center_plan._ptr_subplaninfo[i]._green) {
             stc._for_center_plan._ptr_subplaninfo[i]._green = stc._for_center_plan._ptr_subplaninfo[i]._green - stc._for_center_plan._ptr_subplaninfo[i]._pedgreen_flash;
             printf("Special A!\n");
           }
           if(stc._for_center_plan._ptr_subplaninfo[i]._pedred > 0 && stc._for_center_plan._ptr_subplaninfo[i]._pedred < stc._for_center_plan._ptr_subplaninfo[i]._green) {
             stc._for_center_plan._ptr_subplaninfo[i]._green = stc._for_center_plan._ptr_subplaninfo[i]._green - stc._for_center_plan._ptr_subplaninfo[i]._pedred;
             printf("Special B!\n");
           }
           printf("Green-B:%d\n",stc._for_center_plan._ptr_subplaninfo[i]._green);
      }

      stc._for_center_plan._cycle_time=DataMessageIn.packet[13+2*i]*256+DataMessageIn.packet[14+2*i];
      printf("cycle_time:%d\n",stc._for_center_plan._cycle_time);
      stc._for_center_plan._offset=DataMessageIn.packet[15+2*i]*256+DataMessageIn.packet[16+2*i];
      printf("offset:%d\n",stc._for_center_plan._offset);

      if (stc._for_center_plan._subphase_count==0) {            //閃光時相
          stc._for_center_plan._subphase_count=1;               //虛擬一個Phase

          //OT Debug 0523
          stc._for_center_plan._ptr_subplaninfo[i]._green=10;
      }

//OT Debug 0523
      bRet = stc.Lock_to_Save_Plan_from_Center();                      //儲存回去
      if(bRet) smem.vSetTCPhasePlanSegTypeData(TC_Plan, DataMessageIn.packet[9], true);
      else {
          vReturnToCenterNACK(0x5F, 0x15, 0x04, 0x05); return false;
      }
      vReturnToCenterACK(0x5F, 0x15);
//      smem.vSaveLast92TC_5F15Time();
      screenLast92TCPlanSegmentUpdate.DisplayPlanUpdate();

      printf("SAVE Plan:%d OK!\n",DataMessageIn.packet[9]);

  }
} catch (...) {}
}

//---------------------------------------------------------------------------
bool PTRAFFICAMEGIDS::vQueryPlanDBMS5F45(MESSAGEOK DataMessageIn)
{
try{
  int iQueryPlan = DataMessageIn.packet[9];
  unsigned short int usiGreenTMP;

  bool bPlanStatus;
  bPlanStatus = smem.vGetTCPhasePlanSegTypeData(TC_Plan, iQueryPlan);
  if(bPlanStatus == false) {
    vReturnToCenterNACK(0x5F, 0x45, 0x02, 0x0); return false;
  }
  if( DataMessageIn.packetLength < 13) { vReturnToCenterNACK(0x5F, 0x45, 0x08, 0x00); return false; }
  else if( DataMessageIn.packetLength > 13) { vReturnToCenterNACK(0x5F, 0x45, 0x08, DataMessageIn.packetLength - 12); return false; }


  stc.Lock_to_Load_Plan_for_Center(iQueryPlan);

  unsigned char data[256];
  int iDataPtr = 6;
  data[0] = 0x5F;
  data[1] = 0xC5;
  data[2] = stc._for_center_plan._planid;
  data[3] = stc._for_center_plan._dir;
  data[4] = stc._for_center_plan._phase_order;
  data[5] = stc._for_center_plan._subphase_count;

  for(int i = 0; i < data[5]; i++)
  {
    usiGreenTMP = 0;

    //OT Debug 0523 -3line
    if(stc._for_center_plan._phase_order != 0xB0) {                             //Flash PhaseOrder In HsinChu
      usiGreenTMP += stc._for_center_plan._ptr_subplaninfo[i]._green;
    } else { }
    usiGreenTMP += stc._for_center_plan._ptr_subplaninfo[i]._pedgreen_flash;
    usiGreenTMP += stc._for_center_plan._ptr_subplaninfo[i]._pedred;

    data[iDataPtr] = HI(usiGreenTMP); iDataPtr++;
    data[iDataPtr] = LO(usiGreenTMP); iDataPtr++;
  }

  //OT Debug 0523 -3line
  if(stc._for_center_plan._phase_order != 0xB0) {                             //Flash PhaseOrder In HsinChu
    data[iDataPtr] = stc._for_center_plan._cycle_time / 256; iDataPtr++;
    data[iDataPtr] = stc._for_center_plan._cycle_time % 256; iDataPtr++;
  } else { data[iDataPtr] = 0; data[iDataPtr+1] = 0; iDataPtr+=2; }

  data[iDataPtr] = stc._for_center_plan._offset / 256; iDataPtr++;
  data[iDataPtr] = stc._for_center_plan._offset % 256; iDataPtr++;

  MESSAGEOK _MsgOK;
  _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, iDataPtr, true);
  _MsgOK.InnerOrOutWard = cOutWard;
//  writeJob.WriteWorkByMESSAGEOUT(_MsgOK);
  writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICEAMEGIDS);

  return true;

  } catch (...) {}
}

//---------------------------------------------------------------------------
bool PTRAFFICAMEGIDS::vWriteRunPlan5F18(MESSAGEOK DataMessageIn)
{
try{

  int iSetPlanID = DataMessageIn.packet[9];

  if( DataMessageIn.packetLength < 13) { vReturnToCenterNACK(0x5F, 0x18, 0x08, 0x00); return false; }
  else if( DataMessageIn.packetLength > 13) { vReturnToCenterNACK(0x5F, 0x18, 0x08, DataMessageIn.packetLength - 12); return false; }
  vReturnToCenterACK(0x5F, 0x18);

  smem.vSetINTData(TC92_PlanOneTime5F18_PlanID, iSetPlanID);

} catch (...) {}
}

//---------------------------------------------------------------------------
bool PTRAFFICAMEGIDS::vQueryRunPlan5F48(MESSAGEOK DataMessageIn)                   //then return 5FC8
{
try{
  unsigned char data[256];
  int iDataPtr = 6; //Form 6 Start to send SignalStatus

  unsigned short int usiGreen;

  unsigned short int usiRunningPlanID = stc.vGetUSIData(CSTC_exec_plan_plan_ID);

  printf("usiRunningPlanID:%d\n", usiRunningPlanID);

  stc.Lock_to_Load_Plan_for_Center(usiRunningPlanID);
//  unsigned short int iCurrentPhaseID  = stc.vGetUSIData(CSTC_exec_plan_phase_order);
//  stc.Lock_to_Load_Phase_for_Center(iCurrentPhaseID);

/*
  DATA_Bit _SignalMap, _GSignalMap;
  _SignalMap.DBit = stc._for_center_phase._signal_map;
  int iSignalMapWayCount;
  unsigned short int usiSignalStatus;
  int iGSignalMap[8];

  for(int i = 0; i < stc._for_center_phase._subphase_count; i++) {
    _GSignalMap.DBit = 0;
    iSignalMapWayCount = 0;
    for(int j = 0; j < 8; j++)                                                  //Init
      iGSignalMap[j] = 0;

    if(_SignalMap.switchBit.b1 == true) {                                         //從SignalMap對應CCT的燈點
      usiSignalStatus = stc._for_center_phase._ptr_subphase_step_signal_status[i][0][iSignalMapWayCount];
      iGSignalMap[0] = oTools.vCCTLightToVer30SignalMapLight_5F0F(0, usiSignalStatus);
      iSignalMapWayCount++;
    }
    if(_SignalMap.switchBit.b2 == true) {                                         //從SignalMap對應CCT的燈點
      usiSignalStatus = stc._for_center_phase._ptr_subphase_step_signal_status[i][0][iSignalMapWayCount];
      iGSignalMap[1] = oTools.vCCTLightToVer30SignalMapLight_5F0F(0, usiSignalStatus);
      iSignalMapWayCount++;
    }
    if(_SignalMap.switchBit.b3 == true) {                                         //從SignalMap對應CCT的燈點
      usiSignalStatus = stc._for_center_phase._ptr_subphase_step_signal_status[i][0][iSignalMapWayCount];
      iGSignalMap[2] = oTools.vCCTLightToVer30SignalMapLight_5F0F(0, usiSignalStatus);
      iSignalMapWayCount++;
    }
    if(_SignalMap.switchBit.b4 == true) {                                         //從SignalMap對應CCT的燈點
      usiSignalStatus = stc._for_center_phase._ptr_subphase_step_signal_status[i][0][iSignalMapWayCount];
      iGSignalMap[3] = oTools.vCCTLightToVer30SignalMapLight_5F0F(0, usiSignalStatus);
      iSignalMapWayCount++;
    }
    if(_SignalMap.switchBit.b5 == true) {                                         //從SignalMap對應CCT的燈點
      usiSignalStatus = stc._for_center_phase._ptr_subphase_step_signal_status[i][0][iSignalMapWayCount];
      iGSignalMap[4] = oTools.vCCTLightToVer30SignalMapLight_5F0F(0, usiSignalStatus);
      iSignalMapWayCount++;
    }
    if(_SignalMap.switchBit.b6 == true) {                                         //從SignalMap對應CCT的燈點
      usiSignalStatus = stc._for_center_phase._ptr_subphase_step_signal_status[i][0][iSignalMapWayCount];
      iGSignalMap[5] = oTools.vCCTLightToVer30SignalMapLight_5F0F(0, usiSignalStatus);
      iSignalMapWayCount++;
    }
    if(_SignalMap.switchBit.b7 == true) {                                         //從SignalMap對應CCT的燈點
      usiSignalStatus = stc._for_center_phase._ptr_subphase_step_signal_status[i][0][iSignalMapWayCount];
      iGSignalMap[6] = oTools.vCCTLightToVer30SignalMapLight_5F0F(0, usiSignalStatus);
      iSignalMapWayCount++;
    }
    if(_SignalMap.switchBit.b8 == true) {                                         //從SignalMap對應CCT的燈點
      usiSignalStatus = stc._for_center_phase._ptr_subphase_step_signal_status[i][0][iSignalMapWayCount];
      iGSignalMap[7] = oTools.vCCTLightToVer30SignalMapLight_5F0F(0, usiSignalStatus);
      iSignalMapWayCount++;
    }

    for(int j = 0; j < 8; j++) {
      if(iGSignalMap[j] == 1) {
        if(j == 0) _GSignalMap.switchBit.b1 = true;
        if(j == 1) _GSignalMap.switchBit.b2 = true;
        if(j == 2) _GSignalMap.switchBit.b3 = true;
        if(j == 3) _GSignalMap.switchBit.b4 = true;
        if(j == 4) _GSignalMap.switchBit.b5 = true;
        if(j == 5) _GSignalMap.switchBit.b6 = true;
        if(j == 6) _GSignalMap.switchBit.b7 = true;
        if(j == 7) _GSignalMap.switchBit.b8 = true;
      }
    }
    data[iDataPtr] = _GSignalMap.DBit;
    iDataPtr++;
  }
*/

//  int iDataPtr = 6; //Form 6 Start to send SignalStatus
  data[0] = 0x5F;
  data[1] = 0xC8;
  data[2] = usiRunningPlanID;
  data[3] = stc._for_center_plan._dir;
  data[4] = stc._for_center_plan._phase_order;
  data[5] = stc._for_center_plan._subphase_count;

/*
data[6] = have beed place before
*/


//OT Debug CECI
  for(int i = 0; i < data[5]; i++)                                              //data[5] = subphasecount
  {
    usiGreen = 0;
    if(stc._for_center_plan._phase_order != 0xB0) {                             //Flash PhaseOrder In HsinChu
    usiGreen += stc._for_center_plan._ptr_subplaninfo[i]._green;
    } else { }
    usiGreen += stc._for_center_plan._ptr_subplaninfo[i]._pedgreen_flash;
    usiGreen += stc._for_center_plan._ptr_subplaninfo[i]._pedred;

    data[iDataPtr] = HI(usiGreen);
    iDataPtr++;
    data[iDataPtr] = LO(usiGreen);
    iDataPtr++;
  }

  if(stc._for_center_plan._phase_order != 0xB0) {                               //Flash PhaseOrder In HsinChu
    data[iDataPtr] = HI(stc._for_center_plan._cycle_time);
    iDataPtr++;
    data[iDataPtr] = LO(stc._for_center_plan._cycle_time);
    iDataPtr++;
  } else {
    data[iDataPtr] = 0; data[iDataPtr+1] = 0; iDataPtr+=2;
  }

  data[iDataPtr] = HI(stc._for_center_plan._offset);
  iDataPtr++;
  data[iDataPtr] = LO(stc._for_center_plan._offset);
  iDataPtr++;

  MESSAGEOK _MsgOK;
  _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, iDataPtr, true);
  _MsgOK.InnerOrOutWard = cOutWard;
//  writeJob.WriteWorkByMESSAGEOUT(_MsgOK);
  writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICEAMEGIDS);

} catch (...) {}
}



//-------------------------------------------------------------------
bool PTRAFFICAMEGIDS::vTransmitCycleSetting_5F3F(MESSAGEOK DataMessageIn)
{
try {

    printf("printfMsg go to 5F3F.\n");

    short int TT, TC;
    TT = DataMessageIn.packet[9];
    TC = DataMessageIn.packet[10];

    int iReturnCommandSet = smem.vGet0FCommandSet();
    if (iReturnCommandSet == 0 )   //wireless
    if ( TT==2 ||  (TT == 1 && ((TC>=1 && TC<=4) || (TC >= 6 && TC < 0xFF))) )
        { vReturnToCenterNACK(0x5F, 0x3F, 0x80, 0x00); return false; } // in wireless

    if ( DataMessageIn.packetLength < 14 ) { vReturnToCenterNACK(0x5F, 0x3F, 0x08, 0x00); return false; }
    else if ( DataMessageIn.packetLength > 14 ) { vReturnToCenterNACK(0x5F, 0x3F, 0x08, DataMessageIn.packetLength - 14); return false; }

    if(TT >= 3 || TT == 0) {
       vReturnToCenterNACK(0x5F, 0x3F, 0x04, 0x01);
       return false;
    }

  //OTMARKPRINTF  printf("Go to Function\n\n");

    if(TC == 3)
       TC = 5;  //Sec
    else if(TC == 4)
       TC = 60;
    else if(TC == 5)
       TC = 300;
    else if(TC == 0)        // send change
       TC = 0;
    else if(TC == 1)
       TC = 1;
    else if(TC == 2)
       TC = 2;
    else if(TC == 0xFF)     // stop send
       TC = 0xFF;

    else {
       vReturnToCenterNACK(0x5F, 0x3F, 0x04, 0x02);
       return false;
    }

    if(DataMessageIn.InnerOrOutWard != cComingFromScreen) {
      printf("printfMsg go to ACK.\n");
      vReturnToCenterACK(0x5F, 0x3F);
    }
    printf("printfMsg after ACK.\n");

    if(TT == 1) {
       smem.vSetINTData(TC92SignalLightStatus_5F0F_IntervalTime, TC);                            //將紀錄間隔設到smem裡
    }
    if(TT == 2) {
       smem.vSetINTData(TC92SignalStepStatus_5F03_IntervalTime, TC);                            //將紀錄間隔設到smem裡
    }

    return true;
  } catch (...) {}
}

//-------------------------------------------------------------------
bool PTRAFFICAMEGIDS::vTransmitCycleQuery_5F6F(MESSAGEOK DataMessageIn)
{
try {

    short int TT, TC;
    TT = DataMessageIn.packet[9];

    if(TT == 0 || TT > 2) { vReturnToCenterNACK(0x5F, 0x6F, 0x04, 0x01); return false; }
    if( DataMessageIn.packetLength < 13 ) { vReturnToCenterNACK(0x5F, 0x6F, 0x08, 0x00); return false; }
    else if( DataMessageIn.packetLength > 13 ) { vReturnToCenterNACK(0x5F, 0x6F, 0x08, DataMessageIn.packetLength - 13); return false; }

    if(TT == 1)
      TC = smem.vGetINTData(TC92SignalLightStatus_5F0F_IntervalTime);
    if(TT == 2)
      TC = smem.vGetINTData(TC92SignalStepStatus_5F03_IntervalTime);

    if(TC == 0)         //系統為傳送變化
       TC = 0;
    else if(TC == 1)
       TC = 1;
    else if(TC == 2)
       TC = 2;
    else if(TC == 5)
       TC = 3;
    else if(TC == 60)
       TC = 4;
    else if(TC == 300)
       TC = 5;
    else if(TC == 0xFF)    //系統的傳送週期為 0 (不傳送)
       TC = 0xFF;

    unsigned char data[6];
    data[0] = 0x5F;
    data[1] = 0xEF;
    data[2] = TT;
    data[3] = TC;

    MESSAGEOK _MsgOK;
    _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, 4, true);
    _MsgOK.InnerOrOutWard = cOutWard;
//    writeJob.WriteWorkByMESSAGEOUT(_MsgOK);
    writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICEAMEGIDS);
    return true;

  } catch(...) {}
}

//-------------------------------------------------------------------
bool PTRAFFICAMEGIDS::vGoToNextPhaseStepControl_5F1C(MESSAGEOK DataMessageIn)
{
try {

    short int siSubPhaseID, siStepID, siEffectTime;
    siSubPhaseID = DataMessageIn.packet[9];
    siStepID = DataMessageIn.packet[10];
    siEffectTime = DataMessageIn.packet[11];

    char msg[256];

    int iReturnCommandSet = smem.vGet0FCommandSet();
    if (iReturnCommandSet <= 2 ) { vReturnToCenterNACK(0x5F, 0x1C, 0x80, 0x00); return false; }

    if ( DataMessageIn.packetLength < 15 ) { vReturnToCenterNACK(0x5F, 0x1C, 0x08, 0x00); return false; }
    else if ( DataMessageIn.packetLength > 15 ) { vReturnToCenterNACK(0x5F, 0x1C, 0x08, DataMessageIn.packetLength - 15); return false; }

    if(siSubPhaseID > 8) { vReturnToCenterNACK(0x5F, 0x1C, 0x04, 0x01); return false; }
    if(siStepID > 8) { vReturnToCenterNACK(0x5F, 0x1C, 0x04, 0x02); return false; }

    DATA_Bit uc5F10_ControlStrategy;
    int i5F10_EffectTime;

    uc5F10_ControlStrategy.DBit = smem.vGetUCData(TC92_ucControlStrategy);
    i5F10_EffectTime = smem.vGetINTData(TC92_iEffectTime);

    memset(msg,0,sizeof(msg));
    sprintf(msg,"[Debug] 5F1C: siSubPhaseID:%d, siStepID:%d, siEffectTime:%d, uc5F10_ControlStrategy:%x, ", siSubPhaseID, siStepID, siEffectTime, uc5F10_ControlStrategy.DBit);
    smem.vWriteMsgToDOM(msg);


    if(uc5F10_ControlStrategy.switchBit.b5 == false && uc5F10_ControlStrategy.switchBit.b6 == false) {
      memset(msg,0,sizeof(msg));
      sprintf(msg,"[Debug] 5F1C: bit 5/6 all false");
      smem.vWriteMsgToDOM(msg);

      vReturnToCenterNACK(0x5F, 0x1C, 0x80, 0x00); return false;
    }

    if(siSubPhaseID == 0 && siStepID == 0 && siEffectTime == 0) {  //Go To Next Step
//      stc.Lock_to_Set_Control_Strategy(STRATEGY_MANUAL);
//      stc.Lock_to_Set_Control_Strategy(STRATEGY_TOD);

      if (!stc.Lock_to_Set_Control_Strategy(STRATEGY_ALLDYNAMIC)) {
        stc.Lock_to_Set_Next_Step();
      }

    usleep(50);
    int usiCurrentSubphaseStep = stc.vGetUSIData(CSTC_exec_phase_current_subphase_step);

    memset(msg,0,sizeof(msg));
    sprintf(msg,"[Debug] 5F1C: new usiCurrentSubphaseStep:%d", usiCurrentSubphaseStep);
    smem.vWriteMsgToDOM(msg);

    if(usiCurrentSubphaseStep == 0) {
      if(Protocal5F1CStopStep == usiCurrentSubphaseStep) {
        _intervalTimer.vAllDynamicToTODCount(i5F10_EffectTime);
      }
      else {
        _intervalTimer.vAllDynamicToTODCount(stc.vGetUSIData(CSTC_exec_plan_green_time));
      }
    }
    else if(usiCurrentSubphaseStep == 1)
      _intervalTimer.vAllDynamicToTODCount(stc.vGetUSIData(CSTC_exec_plan_pedgreenflash_time));
    else if(usiCurrentSubphaseStep == 2) {
      if(Protocal5F1CStopStep == usiCurrentSubphaseStep) {
        _intervalTimer.vAllDynamicToTODCount(i5F10_EffectTime);
      }
      else {
        _intervalTimer.vAllDynamicToTODCount(stc.vGetUSIData(CSTC_exec_plan_pedred_time));
      }
    }
    else if(usiCurrentSubphaseStep == 3)
      _intervalTimer.vAllDynamicToTODCount(stc.vGetUSIData(CSTC_exec_plan_yellow_time));
    else if(usiCurrentSubphaseStep == 4)
      _intervalTimer.vAllDynamicToTODCount(stc.vGetUSIData(CSTC_exec_plan_allred_time));

/*
      stc.vLockToSetControlStrategyToManualNotBySignal();
      usleep(50);
      stc.vLockToSetControlStrategyToTODNotBySignal();
*/
      return true;
    }

    if(siSubPhaseID == 0) { vReturnToCenterNACK(0x5F, 0x1C, 0x04, 0x01); return false; }  //錯誤判斷
    if(siStepID == 0) { vReturnToCenterNACK(0x5F, 0x1C, 0x04, 0x02); return false; }

    unsigned short int usiCSTC_RunningPhaseCurrentPhase = stc.vGetUSIData(CSTC_exec_plan_phase_order);  //Should +1
    unsigned short int usiCSTC_RunningPhaseCurrentSubphase = stc.vGetUSIData(CSTC_exec_phase_current_subphase);  //Should +1
    unsigned short int usiCSTC_RunningPhaseCurrentSubphaseStep = stc.vGetUSIData(CSTC_exec_phase_current_subphase_step);
    stc.Lock_to_Load_Phase_for_Center(usiCSTC_RunningPhaseCurrentPhase);

    return false;

} catch(...) {}
}


//---------------------------------------------------------------------------
bool PTRAFFICAMEGIDS::vQueryPhaseStepControl_5F4C(MESSAGEOK DataMessageIn)
{
try{

//For Amegids of NCKU, not Ver3 Protocal
//  unsigned short int usiEffectTime = _intervalTimer.vGetEffectTime();
  unsigned short int usiStepSec = stc.vGetStepTime();

  unsigned char data[6];
  data[0] = 0x5F;
  data[1] = 0xCC;
  data[2] = smem.vGetUCData(TC92_ucControlStrategy);
  data[3] = stc.vGetUSIData(CSTC_exec_phase_current_subphase) + 1;
  data[4] = stc.vGetUSIData(CSTC_exec_phase_current_subphase_step)  + 1;

//  stc.vGetStepTime();

//  if( stc.Lock_to_LoadControlStrategy == STRATEGY_ALLDYNAMIC )

//For Amegids of NCKU, not Ver3 Protocal
//    data[5] = (usiEffectTime>>8) & 0x00ff;
//    data[6] = usiEffectTime & 0x00ff;
    data[5] = (usiStepSec>>8) & 0x00ff;
    data[6] = usiStepSec & 0x00ff;

//  else
//    data[4] = 0;

  MESSAGEOK _MsgOK;
  _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, 7, true);
  _MsgOK.InnerOrOutWard = cOutWard;
//  writeJob.WriteWorkByMESSAGEOUT(_MsgOK);
  writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICEAMEGIDS);
  return true;

} catch(...) {}
}

//ACK------------------------------------------------------------------------
int PTRAFFICAMEGIDS::vReturnToCenterACK(unsigned char ucDevCode,
                                     unsigned char ucControlCode)
{
try{
    unsigned char data[4];

    data[0] = 0x0F;
    data[1] = 0x80;
    data[2] = ucDevCode;
    data[3] = ucControlCode;

    MESSAGEOK _MsgOK;

    _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, 4,true);
    _MsgOK.InnerOrOutWard = cOutWard;
//    writeJob.WriteWorkByMESSAGEOUT(_MsgOK);
    writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICEAMEGIDS);

    return 0;
  } catch (...) {}
}

//ACK------------------------------------------------------------------------
int PTRAFFICAMEGIDS::vReturnToCenter92ACK(unsigned char ucDevCode,
                                     unsigned char ucControlCode)
{
try{
    unsigned char data[4];

    data[0] = 0x0F;
    data[1] = 0x80;
    data[2] = ucDevCode;
    data[3] = ucControlCode;

    MESSAGEOK _MsgOK;

    _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, 4,true);
    _MsgOK.InnerOrOutWard = cOutWard;
//    writeJob.WriteWorkByMESSAGEOUT(_MsgOK);
    writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICECENTER92);

    return 0;
  } catch (...) {}
}



//---------------------------------------------------------------------------
int PTRAFFICAMEGIDS::vReturnToCenterNACK(unsigned char ucDevCode,
                                     unsigned char ucControlCode,
                                     unsigned char ErrorCode,
                                     unsigned char ParameterNumber)
{
try{
    unsigned char data[6];

    data[0] = 0x0F;
    data[1] = 0x81;
    data[2] = ucDevCode;
    data[3] = ucControlCode;
    data[4] = ErrorCode;
    data[5] = ParameterNumber;

    MESSAGEOK _MsgOK;

    _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, 6,true);
    _MsgOK.InnerOrOutWard = cOutWard;
//    writeJob.WriteWorkByMESSAGEOUT(_MsgOK);
    writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICEAMEGIDS);

    return 0;
  } catch (...) {}
}

//---------------------------------------------------------------------------
int PTRAFFICAMEGIDS::vReturnToCenter92NACK(unsigned char ucDevCode,
                                     unsigned char ucControlCode,
                                     unsigned char ErrorCode,
                                     unsigned char ParameterNumber)
{
try{
    unsigned char data[6];

    data[0] = 0x0F;
    data[1] = 0x81;
    data[2] = ucDevCode;
    data[3] = ucControlCode;
    data[4] = ErrorCode;
    data[5] = ParameterNumber;

    MESSAGEOK _MsgOK;

    _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, 6,true);
    _MsgOK.InnerOrOutWard = cOutWard;
//    writeJob.WriteWorkByMESSAGEOUT(_MsgOK);
    writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICECENTER92);

    return 0;
  } catch (...) {}
}

