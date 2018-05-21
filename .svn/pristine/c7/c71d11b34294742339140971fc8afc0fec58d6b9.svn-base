//---------------------------------------------------------------------------

#ifndef PTRAFFIC92COUNTDOWNH
#define PTRAFFIC92COUNTDOWNH

#include "var.h"
#include "SMEM.h"

//---------------------------------------------------------------------------
class PTRAFFIC92COUNTDOWN
{
    public:

        PTRAFFIC92COUNTDOWN(void);
        ~PTRAFFIC92COUNTDOWN(void);

        bool DoWorkViaPTraffic92(MESSAGEOK);
        bool GreenManCountdownonoff_5014(MESSAGEOK);
        bool vQueryGreenMan_5044(MESSAGEOK);
        bool CarCountdownonoff_5015(MESSAGEOK);
        bool vQureyCar_5045(MESSAGEOK);

    private:

        int vReturnToCenterACK(unsigned char , unsigned char);
        int vReturnToCenterNACK(unsigned char, unsigned char, unsigned char, unsigned char);


};
//---------------------------------------------------------------------------
#endif
