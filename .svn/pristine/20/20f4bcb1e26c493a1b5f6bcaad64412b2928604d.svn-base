//---------------------------------------------------------------------------
#include "PTRAFFICREVSYNC.h"
#include "SMEM.h"
#include "WRITEJOB.h"

/*OTCombo0714*/
#include "CDataToMessageOK.h"
#include "CSTC.h"
#include "unistd.h"

#include <stdio.h>
#include <math.h>
//---------------------------------------------------------------------------
PTRAFFICREVSYNC::PTRAFFICREVSYNC(void)
{

//  pthread_mutex_init(&mutexKeyPad,NULL);

//  firstTime=true;
//  stc.SetDefaultLCNPhaseOrder(1,0);
}
//---------------------------------------------------------------------------
PTRAFFICREVSYNC::~PTRAFFICREVSYNC(void)
{
}
//---------------------------------------------------------------------------
bool PTRAFFICREVSYNC::DoWorkViaPTraffic(MESSAGEOK message)
{
try {
    unsigned short int usiTmp;
    int address=0;

    printf("PPS\n");

    switch (message.packet[8]) {
            case 0xA1:                     //only set.
              vSyncStatus(message);
            break;

            case 0x40:
              vAckTCConn(message, true);   //WDT, will set Rev Status, also
            break;

            default:
            break;
    }

    return true;

  } catch (...) {}
}

//---------------------------------------------------------------------------
bool PTRAFFICREVSYNC::vSyncStatus(MESSAGEOK message)
{
try {
  static unsigned short int usiStepErrorCount = 0;
  bool bRet;

  unsigned short int usiLocalRevStep;
  CReverseTimeInfo _LocalRev;

  time_t currenttime=time(NULL);
  struct tm *now=localtime(&currenttime);
  unsigned int temphh=(now->tm_hour),tempmm=(now->tm_min);

  smem.vSetLastRevSyncTime(currenttime);

  sRevSyncStatus _RevSyncStatus;

  _RevSyncStatus.bYMD_OK = false;
  _RevSyncStatus.bTime_OK = false;
  _RevSyncStatus.bRevStep_OK = false;
  _RevSyncStatus.bRevSegSet_OK = false;
  _RevSyncStatus.bRevTime_OK = false;
  unsigned int uiTimeA, uiTimeB;
  int iDiffTime;

  char cTMP[256];

  stc.vGetRevInfo(&usiLocalRevStep, &_LocalRev);

  printf("Recv Rev-%d:%d:%d, step:%d, SI-%d:%d, EI-%d:%d, SO-%d:%d, EO-%d:%d, %d,%d,%d\n",
  message.packet[12], message.packet[13], message.packet[14], message.packet[15],
  message.packet[16], message.packet[17], message.packet[18], message.packet[19], message.packet[20], message.packet[21], message.packet[22], message.packet[23],
  message.packet[24]*256+message.packet[25], message.packet[26]*256+message.packet[27], message.packet[28]*256+message.packet[29]
  );

  if((now->tm_year) - 11 == message.packet[9] && (now->tm_mon) + 1 == message.packet[10] && (now->tm_mday) == message.packet[11]) {
    _RevSyncStatus.bYMD_OK = true;
  }

  uiTimeA = (now->tm_hour) * 3600 + (now->tm_min) * 60 + (now->tm_sec);
  uiTimeB = (message.packet[12]) * 3600 + (message.packet[13]) * 60 + (message.packet[14]);

  iDiffTime = uiTimeA - uiTimeB;
  iDiffTime = abs(iDiffTime);

  if(iDiffTime <= 10) {  //test
    _RevSyncStatus.bTime_OK = true;
  }

  iDiffTime = (short int)usiLocalRevStep - (short int)message.packet[15];
  iDiffTime = abs(iDiffTime);
  if( usiLocalRevStep == message.packet[15]) {
    _RevSyncStatus.bRevStep_OK = true;
    usiStepErrorCount = 0;

  //ex: step 0 1
  // step 0 7
  // step 0 8
  // step 1 8
  } else if( (iDiffTime == 1 || iDiffTime == 7 || iDiffTime == 8 || iDiffTime == 17 || iDiffTime == 18) && usiStepErrorCount < 3) {
    usiStepErrorCount++;
    _RevSyncStatus.bRevStep_OK = true;
  }

  if(_LocalRev.usiHourStartIn == message.packet[16] && _LocalRev.usiMinStartIn == message.packet[17] &&
     _LocalRev.usiHourEndIn == message.packet[18] && _LocalRev.usiMinEndIn == message.packet[19] &&
     _LocalRev.usiHourStartOut == message.packet[20] && _LocalRev.usiMinStartOut == message.packet[21] &&
     _LocalRev.usiHourEndOut == message.packet[22] && _LocalRev.usiMinEndOut == message.packet[23]        ) {

       _RevSyncStatus.bRevSegSet_OK = true;
  }

  if(_LocalRev.usiClearTime == message.packet[24]*256 + message.packet[25] &&
     _LocalRev.usiFlashGreen == message.packet[26]*256 + message.packet[27] &&
     _LocalRev.usiGreenTime == message.packet[28]*256 + message.packet[29]) {
       _RevSyncStatus.bRevTime_OK = true;
  }

  smem.vSetRevSyncStatus(_RevSyncStatus);

/*Move
  printf("***********************************************************\n");
  printf("bYMD_OK:%d, bTime_OK:%d, bRevStep_OK:%d, bRevSegSet_OK:%d, bRevTime_OK:%d\n", _RevSyncStatus.bYMD_OK, _RevSyncStatus.bTime_OK, _RevSyncStatus.bRevStep_OK, _RevSyncStatus.bRevSegSet_OK, _RevSyncStatus.bRevTime_OK);
  printf("-----------------------------------------------------------\n");

  sprintf(cTMP, "bYMD_OK:%d, bTime_OK:%d, bRevStep_OK:%d, bRevSegSet_OK:%d, bRevTime_OK:%d\n", _RevSyncStatus.bYMD_OK, _RevSyncStatus.bTime_OK, _RevSyncStatus.bRevStep_OK, _RevSyncStatus.bRevSegSet_OK, _RevSyncStatus.bRevTime_OK);
  smem.vWriteMsgToDOM(cTMP);
*/

  MESSAGEOK _MSG;


} catch (...) {}
}

//---------------------------------------------------------------------------
bool PTRAFFICREVSYNC::vAckTCConn(MESSAGEOK message, bool bSet)
{
try {
  MESSAGEOK _MSG;
  unsigned char ucTmp[8];

  smem.SetRevManualPadTime();
  smem.vSetUCData(TC_ReverseLane_Control_Mode, 1);                              //Set to manual

  if(bSet) {
    stc.vSetRevStep(message.packet[9]);
    stc.vLightRevSingal();
  }

  ucTmp[0] = 0x0F;
  ucTmp[1] = 0xC0;
  ucTmp[2] = smem.vGetUCData(TC_ReverseLane_Manual_Enable_Status);
  ucTmp[3] = smem.vGetUCData(TC_ReverseLane_Control_Mode);
  ucTmp[4] = stc.vGetRevStep();                                                 //Rev Status.

  _MSG = oDataToMessageOK.vPackageINFOTo92Protocol(ucTmp, 5, true);
  _MSG.InnerOrOutWard = cOutWard;
  writeJob.WritePhysicalOut(_MSG.packet, _MSG.packetLength, DEVICEREVSYNC);

} catch (...) {}
}

