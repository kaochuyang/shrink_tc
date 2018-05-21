#ifndef PTRAFFIC92WEB_H_INCLUDED
#define PTRAFFIC92WEB_H_INCLUDED

#include "var.h"
#include "SMEM.h"

class PTRAFFIC92WEB
{
    public:

        PTRAFFIC92WEB(void);
        ~PTRAFFIC92WEB(void);
        bool DoWorkViaPTraffic92(MESSAGEOK);


    private:

        bool vWriteSegment5F16(MESSAGEOK);
        int vReturnToCenterACK(unsigned char , unsigned char);
        int vReturnToCenterNACK(unsigned char, unsigned char, unsigned char, unsigned char);
};

#endif // PTRAFFIC92WEB_H_INCLUDED
