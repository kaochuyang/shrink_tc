//---------------------------------------------------------------------------

#ifndef PROTOCOLH
#define PROTOCOLH

#include "var.h"
#include "PTRAFFIC.h"
#include "PDEVICE.h"
//---------------------------------------------------------------------------
class PROTOCOL
{
    public:

        PROTOCOL(void);
        ~PROTOCOL(void);
        bool DoWork(MESSAGEOK);                                                 //根據readJob或writeJob傳來的Structure作事情

    private:

        PTRAFFIC pTraffic;
        PDEVICE pDevice;

};
//---------------------------------------------------------------------------
extern PROTOCOL protocol;
#endif
