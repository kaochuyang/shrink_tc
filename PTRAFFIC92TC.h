//---------------------------------------------------------------------------

#ifndef PTRAFFIC92TCH
#define PTRAFFIC92TCH

#include "var.h"
#include "SMEM.h"
//---------------------------------------------------------------------------
class PTRAFFIC92TC
{
    public:

        PTRAFFIC92TC(void);
        ~PTRAFFIC92TC(void);
        bool DoWorkViaPTraffic92(MESSAGEOK);                                    //根據PTraffic92來的訊息作事情

        bool vWriteControlStrategy5F10(MESSAGEOK);
        bool vQurryControlStrategy5F40(MESSAGEOK);  //with 5FC0

        bool vWriteReverseTimeData5F11(MESSAGEOK);
        bool vWriteReverseTimeData5F12(MESSAGEOK);
        bool vQueryReverseTimeData5F41(MESSAGEOK);
        bool vQueryReverseTimeData5F42(MESSAGEOK);


        bool vWritePlan5F14(MESSAGEOK);
        bool vQueryPlan5F44(MESSAGEOK);  //with 5FC4

        bool vWritePhase5F13(MESSAGEOK);
        bool vQueryPhase5F43(MESSAGEOK);  //with 5FC3

        bool vWritePlanDBMS5F15(MESSAGEOK);
        bool vQueryPlanDBMS5F45(MESSAGEOK);  //with 5FC5

        bool vWriteSegment5F16(MESSAGEOK);
        bool vQuerySegment5F46(MESSAGEOK);

        bool vWriteSpecialSegment5F17(MESSAGEOK);
        bool vQueryHolidaySegment5F47(MESSAGEOK);

//jantai 20140216
        bool vSetActuatedType_5F19(MESSAGEOK);
        bool vQueryActuatedType_5F49(MESSAGEOK);
        bool vSetActuatedSegment_5F1A(MESSAGEOK);
        bool vQueryActuatedSegment_5F4A(MESSAGEOK);
        bool vSetActuatedHolidaySegment_5F1B(MESSAGEOK);
        bool vQueryActuatedHolidaySegment_5F4B(MESSAGEOK);

        bool vWritePhaseByStep5F2F(MESSAGEOK);
//OT Debug 4010
        bool vQueryPhase5F5F(MESSAGEOK);

        bool vTransmitCycleSetting_5F3F(MESSAGEOK);
        bool vTransmitCycleQuery_5F6F(MESSAGEOK);

        bool vGoToNextPhaseStepControl_5F1C(MESSAGEOK);
        bool vQueryPhaseStepControl_5F4C(MESSAGEOK);

        bool vWriteRunPlan5F18(MESSAGEOK);
        bool vQueryRunPlan5F48(MESSAGEOK);

        //jacky20150324
        bool vTouchControlConfiguration_5F19(MESSAGEOK);
        bool vTouchControlConfiguration_5F49(MESSAGEOK);
        //ccj20150722
        bool vSetDynSegParameter(MESSAGEOK);
        bool vQueryDynSegParameter(MESSAGEOK);
        bool vSetDynSegSwitch(MESSAGEOK);
    private:

        int vReturnToCenterACK(unsigned char, unsigned char);
        int vReturnToCenterNACK(unsigned char, unsigned char, unsigned char, unsigned char);

};
//---------------------------------------------------------------------------
#endif

