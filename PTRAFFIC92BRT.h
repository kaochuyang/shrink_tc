//---------------------------------------------------------------------------

#ifndef PTRAFFIC92BRTH
#define PTRAFFIC92BRTH

#include "var.h"
#include "SMEM.h"

//---------------------------------------------------------------------------
class PTRAFFIC92BRT
{
    public:

        PTRAFFIC92BRT(void);
        ~PTRAFFIC92BRT(void);

        bool DoWorkViaPTraffic92(MESSAGEOK);                                    //根據PTraffic92來的訊息作事情

        bool ManualPriority_BF11(MESSAGEOK);
        bool vQueryManual_BF41(MESSAGEOK);
        bool StepControl_BF08(MESSAGEOK);
        bool CarPosition_BF0B(MESSAGEOK);
        bool WritePrioritySegment_BF12(MESSAGEOK);
        bool vQuerySegment_BF42(MESSAGEOK);
        bool WritePriorityParameter_BF13(MESSAGEOK);
        bool vQueryPriorityParameter_BF43(MESSAGEOK);
        bool BRTDelay_BF0D(MESSAGEOK);
        bool DeviceStatus_BF00(MESSAGEOK);


    private:

        int vReturnToCenterACK(unsigned char , unsigned char);
        int vReturnToCenterNACK(unsigned char, unsigned char, unsigned char, unsigned char);



};
//---------------------------------------------------------------------------
#endif
