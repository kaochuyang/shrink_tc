//---------------------------------------------------------------------------

#ifndef PTRAFFICCCJH
#define PTRAFFICCCJH

#include "var.h"
#include "SMEM.h"
//---------------------------------------------------------------------------
class PTRAFFICCCJ
{
    public:

        PTRAFFICCCJ(void);
        ~PTRAFFICCCJ(void);
        bool DoWorkViaPTraffic(MESSAGEOK);               //根據pTraffic傳來的Structure作事情

        bool v5F90(MESSAGEOK);
        bool vQurryControlStrategy5F40(MESSAGEOK);  //with 5FC0

        bool vGoToNextPhaseStepControl_5F1C(MESSAGEOK);
        bool vQueryPhaseStepControl_5F4C(MESSAGEOK);
        bool vQueryCCJSec_5F70(MESSAGEOK);
        bool vCCJSetIPCTime_5F12(MESSAGEOK);

        bool vCCJQueryIPCTime_5F42(MESSAGEOK);

        bool vCCJTFDActuateExtend5F9F(MESSAGEOK);
        bool vCCJTFDActuateStatusScreen5F9A(MESSAGEOK);

        bool vWriteRunPlan5F18(MESSAGEOK);

        bool vWritePlan5F14(MESSAGEOK);
        bool vQueryPlan5F44(MESSAGEOK);  //with 5FC4
        bool vWritePlanDBMS5F15(MESSAGEOK);
        bool vQueryPlanDBMS5F45(MESSAGEOK);  //with 5FC5

        bool vSetSendStep5F91(MESSAGEOK);

        bool vCCJTOMActuateExtendShort5FA2(MESSAGEOK);

        bool vQueryCCTPlan5F48(MESSAGEOK);

        bool vCloseRedCount5FA3(MESSAGEOK);
        bool vUsingNormalTODPlan5FA4(MESSAGEOK);

        bool vActTOD5FA5(MESSAGEOK);

        bool vWriteControlStrategy5F10(MESSAGEOK);

        bool vActPedPushLight5FA9(MESSAGEOK);
        bool vActGetConfigForProgramRestart5FA8(MESSAGEOK);//20140815 程式重啟需取得觸動參數
        bool vActFixedMaxGreen5FA7(MESSAGEOK);//fixedGreen
        bool vActChangePhaseRedCount5FA6(MESSAGEOK);//改變紅燈倒數秒數 1001206


    private:

        int vReturnToCenterACK(unsigned char, unsigned char);
        int vReturnToCenterNACK(unsigned char, unsigned char, unsigned char, unsigned char);

        bool vReturnWriteACK92(BYTE sequence);
        bool vReturnWriteNACK92(BYTE sequence,BYTE error);

        unsigned char uc5F90LastContralCode;

        bool vCCJPhaseStep_5F7C(void);


};
//---------------------------------------------------------------------------
#endif



