//---------------------------------------------------------------------------

#ifndef PDEVICERTMSH
#define PDEVICERTMSH

#include "var.h"
//---------------------------------------------------------------------------
class PDEVICERTMS
{
    public:

        PDEVICERTMS(void);
        ~PDEVICERTMS(void);
        bool DoWorkViaPDevice(MESSAGEOK);                                       //根據PDevice來的封包作事情

    private:

};
//---------------------------------------------------------------------------
#endif

