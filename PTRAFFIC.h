//---------------------------------------------------------------------------

#ifndef PTRAFFICH
#define PTRAFFICH

#include "var.h"
#include "PTRAFFIC77.h"
#include "PTRAFFIC87.h"
#include "PTRAFFIC92.h"
#include "PTRAFFICNTCIP.h"

#include "PTRAFFICCCJ.h"
//OT20140224
#include "PTRAFFIC92BRT.h"
//jacky20140426
#include "PTRAFFIC92COUNTDOWN.h"

#include "PTRAFFICREVSYNC.h"
#include "PTRAFFICAMEGIDS.h"
#include "PTRAFFIC92VD.h"
#include "PTRAFFIC92WEB.h"
//---------------------------------------------------------------------------
class PTRAFFIC
{
    public:

        PTRAFFIC(void);
        ~PTRAFFIC(void);
        bool DoWorkViaProtocol(MESSAGEOK);                                      //根據protocol傳來的Structure作事情


    private:

        PTRAFFIC77 pTraffic77;
        PTRAFFIC87 pTraffic87;
        PTRAFFIC92 pTraffic92;
        PTRAFFICNTCIP pTrafficNtcip;

        PTRAFFICCCJ pTrafficCCJ;
        //OT20140224
        PTRAFFIC92BRT pTrafficBRT;
        //jacky20140426
        PTRAFFIC92COUNTDOWN pTrafficCOUNTDOWN;

        PTRAFFICREVSYNC pTrafficRevSync;
        PTRAFFICAMEGIDS pTrafficAmegids;

        PTRAFFIC92VD pTraffic92Vd;//arwen ++
        PTRAFFIC92WEB pTraffic92Web;//arwen ++

};
//---------------------------------------------------------------------------
#endif


