//---------------------------------------------------------------------------

#ifndef PDEVICESSH
#define PDEVICESSH

#include "var.h"
//---------------------------------------------------------------------------
class PDEVICESS
{

    public:

        PDEVICESS(void);
        ~PDEVICESS(void);

        bool DoWorkViaPDevice(MESSAGEOK);                                       //根據PDevice來的封包作事情
/*
        SmartSensorINFO _SSInfo;

        bool vWriteSmartSensor_S0_ToShareMem(MESSAGEOK);
*/
    private:
/*
        bool vDoProtocol_XD(MESSAGEOK);
        bool vDoProtocol_XA(MESSAGEOK);

        bool vDoReceiveSKSuccess(MESSAGEOK);

        short int SS_XDYear, SS_XDMonth, SS_XDDay, SS_XDHour, SS_XDMin, SS_XDSec;
        short int SS_XDLaneCount;
        short int SS_XDLaneID;
        SSXDLaneContent SS_XDLaneContent[8];

        short int SS_XAHour, SS_XAMin, SS_XASec;
        short int SS_XALaneID;
        float SS_XADuration;
        float SS_XASpeed;
        short int SS_XAClassID;
        bool PDEVICESS::vDoProtocol_X1(MESSAGEOK);
        unsigned char SS_X1Presence[4];

        //OTSS++
        bool vXGTransToRTMS0x18(MESSAGEOK);
        //OTSS--
*/
};
//---------------------------------------------------------------------------
#endif
