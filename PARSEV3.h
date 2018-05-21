//---------------------------------------------------------------------------
#ifndef PARSEV3H
#define PARSEV3H

#include "var.h"
#include "PARSE.h"

//---------------------------------------------------------------------------
class PARSEV3:public PARSE
{
    public:

        PARSEV3(void);
        ~PARSEV3(void);

        bool ParseBlock(int,BYTE *,MESSAGEOK *,int *,int *);
        bool CheckSum(int *,MESSAGEOK *);
        bool DecideProtocol(int *,MESSAGEOK *,int);
        bool CheckReasonable(int *,MESSAGEOK *);
        bool EchoToGUI(int *,MESSAGEOK *,char *);
        bool MoveLastData(int *, int *, MESSAGEOK *);
    private:
//        BCDSW bcdSwitchHi,bcdSwitchLo;
        bool EchoOriginalPacket(BYTE *,int);
        BYTE GetEquipCode(BYTE);
};
//---------------------------------------------------------------------------
#endif
