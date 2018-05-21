//---------------------------------------------------------------------------
#ifndef PARSETRAFFICH
#define PARSETRAFFICH

#include "var.h"
#include "PARSE.h"

//---------------------------------------------------------------------------
/* Have move to var.h
typedef struct BCD{
        BYTE b1:4;
        BYTE b2:4;
}BCD;
typedef union BCDSW{
        BCD bcdHiLo;
        BYTE bcdCode;
}BCDSW;
*/
//---------------------------------------------------------------------------
class PARSETRAFFIC:public PARSE
{
    public:

        PARSETRAFFIC(void);
        ~PARSETRAFFIC(void);

        bool ParseBlock(int,BYTE *,MESSAGEOK *,int *,int *);                    //解析block
        bool vClearMsg(MESSAGEOK *, unsigned short int);
        bool CheckSum(int *,MESSAGEOK *);                                       //檢查checksum
        bool DecideProtocol(int *,MESSAGEOK *,int);                             //給予此封包協定代碼
        bool vJudgeProtocolAndCheckLength(int *,MESSAGEOK *);
        bool CheckReasonable(int *,MESSAGEOK *);                                //檢查封包合理性
        bool AssignLcn(int *,MESSAGEOK *);                                      //根據封包計算LCN
        bool EchoToGUI(int *,MESSAGEOK *,char *);                               //顯示至畫面
        bool MoveLastData(int *,int *,MESSAGEOK *);                             //搬移剩餘的資料
        BCDSW bcdSwitchHi,bcdSwitchLo;

        bool ParseBlockEasyVersion(int,BYTE *,MESSAGEOK *,int *,int *);

    private:/*
        bool decide77Or87(MESSAGEOK *);                                          //決定此封包是77年或87年
        bool decideProtocol92Normal(MESSAGEOK *);                                //決定此封包是92年共用訊息
        bool decideProtocol92Tc(MESSAGEOK *);                                    //決定此封包是92年號誌控制器訊息
        bool decideProtocol92Vd(MESSAGEOK *);                                    //決定此封包是92年車輛偵測器訊息
        bool decideProtocol92Cms(MESSAGEOK *);                                   //決定此封包是92年資訊可變控制器訊息
*/

};
//---------------------------------------------------------------------------
#endif
