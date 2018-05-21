#include "PTRAFFIC92WEB.h"
#include "CDataToMessageOK.h"
#include "WRITEJOB.h"
#include "CSTC.h"
#include "SCREENLast92TCPlanSegmentUpdate.h"
//--------------------------------------------------------------
PTRAFFIC92WEB::PTRAFFIC92WEB(void)
{
}
//--------------------------------------------------------------
PTRAFFIC92WEB::~PTRAFFIC92WEB(void)
{
}
//--------------------------------------------------------------
bool PTRAFFIC92WEB::DoWorkViaPTraffic92(MESSAGEOK message)
{
try {
    printf("PTRAFFIC92WEB::DoWorkViaPTraffic92 %d %d\n",message.packet[7],message.packet[8]);
    switch (message.packet[7]){
    case 0x5F:
        switch (message.packet[8]) {
            case 0x16:  vWriteSegment5F16(message); break;

            default:
                vReturnToCenterNACK(message.packet[7], message.packet[8], 0x0, 0x0);
            break;
        }
    break;

    default:
        vReturnToCenterNACK(message.packet[7], message.packet[8], 0x0, 0x0);
    break;
    }
    return true;

  } catch (...) {}
}
//---------------------------------------------------------------------------
bool PTRAFFIC92WEB::vWriteSegment5F16(MESSAGEOK DataMessageIn)
{
  printf("PTRAFFIC92WEB::vWriteSegment5F16\n");
  int i=0;

  stc.Lock_to_Reset_Segment_for_Web(DataMessageIn.packet[9],DataMessageIn.packet[10]);
  stc.Lock_to_Load_WeekDaySegment_for_Web();

  stc._for_web_segment._segment_type=DataMessageIn.packet[9];

/*OTMARKPRINTF*/  printf("Web segment_type%d\n",stc._for_web_segment._segment_type);

  stc._for_web_segment._segment_count=DataMessageIn.packet[10];

/*OTMARKPRINTF*/  printf("Web segment_Count%d\n",stc._for_web_segment._segment_count);


  for (i=0;i<stc._for_web_segment._segment_count;i++) {
     stc._for_web_segment._ptr_seg_exec_time[i]._hour=DataMessageIn.packet[11+3*i];
     stc._for_web_segment._ptr_seg_exec_time[i]._minute=DataMessageIn.packet[12+3*i];
     stc._for_web_segment._ptr_seg_exec_time[i]._planid=DataMessageIn.packet[13+3*i];

     if(smem.vGetTCPhasePlanSegTypeData(TC_Plan, DataMessageIn.packet[13+3*i]) == false) {
       vReturnToCenterNACK(0x5F, 0x16, 0x04, 3*i + 5); return false;
     }


   /*OTMARKPRINTF*/  printf("%d:%d--%d\n",stc._for_web_segment._ptr_seg_exec_time[i]._hour,stc._for_web_segment._ptr_seg_exec_time[i]._minute,stc._for_web_segment._ptr_seg_exec_time[i]._planid);

  }

  int numWeekDay=DataMessageIn.packet[11+3*i];

/*OTMARKPRINTF*/  printf("Web numWeekDay:%d\n",numWeekDay);

  if( DataMessageIn.packetLength < 15+(3*DataMessageIn.packet[10])+numWeekDay) { vReturnToCenterNACK(0x5F, 0x16, 0x08, 0x00); return false; }
  else if( DataMessageIn.packetLength > 15+(3*DataMessageIn.packet[10])+numWeekDay) { vReturnToCenterNACK(0x5F, 0x16, 0x08, DataMessageIn.packetLength - 12); return false; }
  vReturnToCenterACK(0x5F, 0x16);


  for (int j=0;j<numWeekDay;j++) {

   /*OTMARKPRINTF*/  printf("%3x--%d\n",DataMessageIn.packet[12+3*i+j],stc._for_web_segment._segment_type);

     switch (DataMessageIn.packet[12+3*i+j]) {
             case 0x01:
             stc._for_web_weekdayseg[0]._segment_type=stc._for_web_segment._segment_type;
             break;
             case 0x02:
             stc._for_web_weekdayseg[1]._segment_type=stc._for_web_segment._segment_type;
             break;
             case 0x03:
             stc._for_web_weekdayseg[2]._segment_type=stc._for_web_segment._segment_type;
             break;
             case 0x04:
             stc._for_web_weekdayseg[3]._segment_type=stc._for_web_segment._segment_type;
             break;
             case 0x05:
             stc._for_web_weekdayseg[4]._segment_type=stc._for_web_segment._segment_type;
             break;
             case 0x06:
             stc._for_web_weekdayseg[5]._segment_type=stc._for_web_segment._segment_type;
             break;
             case 0x07:
             stc._for_web_weekdayseg[6]._segment_type=stc._for_web_segment._segment_type;
             break;
             case 0x11:
             stc._for_web_weekdayseg[7]._segment_type=stc._for_web_segment._segment_type;
             break;
             case 0x12:
             stc._for_web_weekdayseg[8]._segment_type=stc._for_web_segment._segment_type;
             break;
             case 0x13:
             stc._for_web_weekdayseg[9]._segment_type=stc._for_web_segment._segment_type;
             break;
             case 0x14:
             stc._for_web_weekdayseg[10]._segment_type=stc._for_web_segment._segment_type;
             break;
             case 0x15:
             stc._for_web_weekdayseg[11]._segment_type=stc._for_web_segment._segment_type;
             break;
             case 0x16:
             stc._for_web_weekdayseg[12]._segment_type=stc._for_web_segment._segment_type;
             break;
             case 0x17:
             stc._for_web_weekdayseg[13]._segment_type=stc._for_web_segment._segment_type;
             break;
             default:
             break;
     }
  }

  stc.Lock_to_Save_Segment_from_Web();
  //stc.Lock_to_Save_WeekDaySegment_from_Web();
  smem.vSetTCPhasePlanSegTypeData(TC_SegType, DataMessageIn.packet[9], true);
  screenLast92TCPlanSegmentUpdate.DisplaySegmentUpdate();


  return true;
}
//ACK------------------------------------------------------------------------
int PTRAFFIC92WEB::vReturnToCenterACK(unsigned char ucDevCode,
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
    writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICEWEBSET);

    return 0;
  } catch (...) {}
}
//---------------------------------------------------------------------------
int PTRAFFIC92WEB::vReturnToCenterNACK(unsigned char ucDevCode,
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
    writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICEWEBSET);

    return 0;
  } catch (...) {}
}
