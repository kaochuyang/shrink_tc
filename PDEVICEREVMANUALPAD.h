//---------------------------------------------------------------------------

#ifndef PDEVICEREVMANUALPADH
#define PDEVICEREVMANUALPADH

#include "var.h"
//---------------------------------------------------------------------------
class PDEVICEREVMANUALPAD
{
    public:

        PDEVICEREVMANUALPAD(void);
        ~PDEVICEREVMANUALPAD(void);
        bool DoWorkViaPDevice(MESSAGEOK);                                       //根據PDevice來的封包作事情

        bool vSetRevStatus(MESSAGEOK);
        bool vAckTCConn(MESSAGEOK, bool);

    private:

};
//---------------------------------------------------------------------------
#endif
