//---------------------------------------------------------------------------

#ifndef PTRAFFICAMEGIDSH
#define PTRAFFICAMEGIDSH

#include "var.h"
#include "SMEM.h"
//---------------------------------------------------------------------------
class PTRAFFICAMEGIDS
{
    public:

        PTRAFFICAMEGIDS(void);
        ~PTRAFFICAMEGIDS(void);
        bool DoWorkViaPTraffic(MESSAGEOK);                                    //根據PTraffic92來的訊息作事情

        bool vWriteControlStrategy5F10(MESSAGEOK);
        bool vQurryControlStrategy5F40(MESSAGEOK);  //with 5FC0

        bool vQueryPlan5F44(MESSAGEOK);  //with 5FC4

        bool vGoToNextPhaseStepControl_5F1C(MESSAGEOK);
        bool vQueryPhaseStepControl_5F4C(MESSAGEOK);

        bool vWriteRunPlan5F18(MESSAGEOK);
        bool vQueryRunPlan5F48(MESSAGEOK);

        bool vTransmitCycleSetting_5F3F(MESSAGEOK);

        bool vWritePlanDBMS5F15(MESSAGEOK);
        bool vQueryPlanDBMS5F45(MESSAGEOK);  //with 5FC5

        bool vTransmitCycleQuery_5F6F(MESSAGEOK);


    private:

        int vReturnToCenterACK(unsigned char, unsigned char);
        int vReturnToCenterNACK(unsigned char, unsigned char, unsigned char, unsigned char);
        int vReturnToCenter92ACK(unsigned char, unsigned char);
        int vReturnToCenter92NACK(unsigned char, unsigned char, unsigned char, unsigned char);

};
//---------------------------------------------------------------------------
#endif

