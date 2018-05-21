//---------------------------------------------------------------------------
#ifndef PARSESSH
#define PARSESSH

#include "var.h"
// #include "PARSE.h"
#include "CHexToInt.h"
// #include "SMEM.h"
//---------------------------------------------------------------------------
class PARSESS
{
    public:

        PARSESS(void);
        ~PARSESS(void);

        bool ParseBlock(int,BYTE *,MESSAGEOK *,int *,int *);
        bool vMultiDropProcess(int *,MESSAGEOK *);
        bool CheckSum(int *,MESSAGEOK *);
        bool EchoToGUI(int *,MESSAGEOK *,char *);
        bool MoveLastData(int *,int *,MESSAGEOK *);

    private:
        bool doCheckSumXD(int, MESSAGEOK *);

};
//---------------------------------------------------------------------------
#endif
