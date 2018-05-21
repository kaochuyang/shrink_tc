//---------------------------------------------------------------------------

#ifndef PTRAFFIC92CMSH
#define PTRAFFIC92CMSH

#include "var.h"
#include "SMEM.h"

//---------------------------------------------------------------------------
class PTRAFFIC92CMS
{
    public:

        PTRAFFIC92CMS(void);
        ~PTRAFFIC92CMS(void);
        bool DoWorkViaPTraffic92(MESSAGEOK);                                    //根據PTraffic92來的訊息作事情

    private:


        int vReturnToCenterACK(unsigned char , unsigned char);
        int vReturnToCenterNACK(unsigned char, unsigned char, unsigned char, unsigned char);


};
//---------------------------------------------------------------------------
#endif

