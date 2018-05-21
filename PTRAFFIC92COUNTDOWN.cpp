#include "PTRAFFIC92COUNTDOWN.h"
#include "CDataToMessageOK.h"
#include "WRITEJOB.h"
#include "CTIMER.h"
#include <stdio.h>
#include <stdlib.h>
#include <error.h>

#include "DIGITALIO.h"
#include "CSTC.h"
#include "CTools.h"
#include "SMEM.h"

//---------------------------------------------------------------------------
PTRAFFIC92COUNTDOWN::PTRAFFIC92COUNTDOWN(void)
{
try {
  } catch (...) {}
}
//---------------------------------------------------------------------------
PTRAFFIC92COUNTDOWN::~PTRAFFIC92COUNTDOWN(void)
{
}
//---------------------------------------------------------------------------
bool PTRAFFIC92COUNTDOWN::DoWorkViaPTraffic92(MESSAGEOK message)
{
try {
    switch (message.packet[8]) {
          case 0x14:
                GreenManCountdownonoff_5014(message);
          break;
          case 0x44:
                vQueryGreenMan_5044(message);
          break;
          case 0x15:
                CarCountdownonoff_5015(message);
          break;
          case 0x45:
                vQureyCar_5045(message);
          break;

          default:
                vReturnToCenterNACK(message.packet[7], message.packet[8], 0x0, 0x0); //protocol error
          break;
    }
  return true;

  } catch (...) {}
}
//--------------------------------------------------------------------------
bool PTRAFFIC92COUNTDOWN::GreenManCountdownonoff_5014(MESSAGEOK DataMessageIn)
{
try{
    printf("\n\nPTRAFFIC92COUNTDOWN::GreenManCountdownonoff_5014\n\n");

    if( DataMessageIn.packetLength < 13 ){ printf("\n\n**********************\n\n"); vReturnToCenterNACK(0x50, 0x14, 0x08, 0x00); return false; }
    else if( DataMessageIn.packetLength > 13 ){ printf("\n\n!!!!!!!!!!!!!!\n\n"); vReturnToCenterNACK(0x50, 0x14, 0x08, DataMessageIn.packetLength - 12); return false; }
    vReturnToCenterACK(0x50, 0x14);

    smem.SetGreenManonoff5014(DataMessageIn.packet[9]);
    smem.SetPedLearnSwitch(DataMessageIn.packet[9]);
 } catch (...) {}
}
//---------------------------------------------------------------------------
bool PTRAFFIC92COUNTDOWN::vQueryGreenMan_5044(MESSAGEOK DataMessageIn)
{
try{
    printf("\n\nPTRAFFIC92COUNTDOWN::vQueryGreenMan_5044\n\n");

    if( DataMessageIn.packetLength < 12 ){ vReturnToCenterNACK(0x50, 0x44, 0x08, 0x00); return false; }
    else if( DataMessageIn.packetLength > 12 ){ vReturnToCenterNACK(0x50, 0x44, 0x08, DataMessageIn.packetLength - 12); return false; }

    unsigned char data[3];
    data[0] = 0x50;
    data[1] = 0xC4;
    data[2] = smem.GetGreenManonoff5014();

    MESSAGEOK _MsgOK;
    _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, 3, true);
    _MsgOK.InnerOrOutWard = cOutWard;
    writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICECENTER92); //中心port
 } catch (...) {}
}
//---------------------------------------------------------------------------
bool PTRAFFIC92COUNTDOWN::CarCountdownonoff_5015(MESSAGEOK DataMessageIn)
{
try{
    printf("\n\nPTRAFFIC92COUNTDOWN::CarCountdownonoff_5015\n\n");

    if( DataMessageIn.packetLength < 13 ){ vReturnToCenterNACK(0x50, 0x15, 0x08, 0x00); return false; }
    else if( DataMessageIn.packetLength > 13 ){ vReturnToCenterNACK(0x50, 0x15, 0x08, DataMessageIn.packetLength - 12); return false; }
    vReturnToCenterACK(0x50, 0x15);

    smem.SetCaronoff5015(DataMessageIn.packet[9]);
    smem.SetCarLearnSwitch(DataMessageIn.packet[9]);
 } catch (...) {}
}
//---------------------------------------------------------------------------
bool PTRAFFIC92COUNTDOWN::vQureyCar_5045(MESSAGEOK DataMessageIn)
{
try{
    printf("\n\nPTRAFFIC92COUNTDOWN::vQureyCar_5045\n\n");

    if( DataMessageIn.packetLength < 12 ){ vReturnToCenterNACK(0x50, 0x45, 0x08, 0x00); return false; }
    else if( DataMessageIn.packetLength > 12 ){ vReturnToCenterNACK(0x50, 0x45, 0x08, DataMessageIn.packetLength - 12); return false; }

    unsigned char data[3];
    data[0] = 0x50;
    data[1] = 0xC5;
    data[2] = smem.GetCaronoff5015();

    MESSAGEOK _MsgOK;
    _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, 3, true);
    _MsgOK.InnerOrOutWard = cOutWard;
    writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICECENTER92); //中心port
 } catch (...) {}
}
//ACK------------------------------------------------------------------------
int PTRAFFIC92COUNTDOWN::vReturnToCenterACK(unsigned char ucDevCode,
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
    writeJob.WriteWorkByMESSAGEOUT(_MsgOK);

    return 0;
  } catch (...) {}
}
//---------------------------------------------------------------------------
int PTRAFFIC92COUNTDOWN::vReturnToCenterNACK(unsigned char ucDevCode,
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
    writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICECENTER92);

    return 0;
  } catch (...) {}
}

