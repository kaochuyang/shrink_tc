//---------------------------------------------------------------------------
#include "PTRAFFIC92OTHER.h"
#include "CDataToMessageOK.h"
#include "WRITEJOB.h"
#include "CTIMER.h"
#include <stdio.h>
#include <stdlib.h>
#include <error.h>
//---------------------------------------------------------------------------
PTRAFFIC92OTHER::PTRAFFIC92OTHER(void)
{
try {
  } catch (...) {}
}
//---------------------------------------------------------------------------
PTRAFFIC92OTHER::~PTRAFFIC92OTHER(void)
{
}
//---------------------------------------------------------------------------
bool PTRAFFIC92OTHER::DoWorkViaPTraffic92(MESSAGEOK message)
{
try {
  switch (message.packet[8]) {

          default:
            vReturnToCenterNACK(message.packet[7], message.packet[8], 0x0, 0x0);
          break;
  }

  return true;

  } catch (...) {}
}
//---------------------------------------------------------------------------

//ACK------------------------------------------------------------------------
int PTRAFFIC92OTHER::vReturnToCenterACK(unsigned char ucDevCode,
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
    writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICECENTER92);

    return 0;
  } catch (...) {}
}
//---------------------------------------------------------------------------
int PTRAFFIC92OTHER::vReturnToCenterNACK(unsigned char ucDevCode,
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


