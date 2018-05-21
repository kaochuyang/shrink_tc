//---------------------------------------------------------------------------

#ifndef PARSENMEAH
#define PARSENMEAH

#include "var.h"
#include "PARSE.h"
//---------------------------------------------------------------------------
class PARSENMEA:public PARSE
{
    public:

        PARSENMEA(void);
        ~PARSENMEA(void);

        bool ParseNMEABlock(int,BYTE *,MESSAGEOK *,int *,int *);                //解析block
        bool vFindGPRMC(int *,MESSAGEOK *);
        bool vAdjTimeByGPS(int *,MESSAGEOK *);
        bool EchoToGUI(int *,MESSAGEOK *,char *);                               //顯示至畫面
        bool MoveLastData(int *,int *,MESSAGEOK *);                             //搬移剩餘的資料

    private:

};
//---------------------------------------------------------------------------
#endif





