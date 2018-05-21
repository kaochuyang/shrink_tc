//---------------------------------------------------------------------------
#include "PDEVICEREVMANUALPAD.h"
#include "SMEM.h"
#include "WRITEJOB.h"

/*OTCombo0714*/
#include "CDataToMessageOK.h"
#include "CSTC.h"
#include "unistd.h"

#include <stdio.h>
//---------------------------------------------------------------------------
PDEVICEREVMANUALPAD::PDEVICEREVMANUALPAD(void)
{

//  pthread_mutex_init(&mutexKeyPad,NULL);

//  firstTime=true;
//  stc.SetDefaultLCNPhaseOrder(1,0);
}
//---------------------------------------------------------------------------
PDEVICEREVMANUALPAD::~PDEVICEREVMANUALPAD(void)
{
}
//---------------------------------------------------------------------------
bool PDEVICEREVMANUALPAD::DoWorkViaPDevice(MESSAGEOK message)
{
try {
    unsigned short int usiTmp;
    int address=0;

    smem.SetRevManualPadTime();

    switch (message.packet[8]) {
            case 0x10:                     //only set.
              vSetRevStatus(message);
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
bool PDEVICEREVMANUALPAD::vSetRevStatus(MESSAGEOK message)
{
try {
  bool bRet;
  MESSAGEOK _MSG;
  bRet = stc.vSetRevStep(message.packet[9]);
  if(bRet == false) {
    vAckTCConn(_MSG, false);
  } else {  //Normal status, every thing is OK, will not send anything.
  }
  stc.vLightRevSingal();

} catch (...) {}
}

//---------------------------------------------------------------------------
bool PDEVICEREVMANUALPAD::vAckTCConn(MESSAGEOK message, bool bSet)
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
  writeJob.WritePhysicalOut(_MSG.packet, _MSG.packetLength, DEVICEREVMANUALPAD);

} catch (...) {}
}

