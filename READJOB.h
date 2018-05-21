//---------------------------------------------------------------------------
#ifndef READJOBH
#define READJOBH

#include "var.h"
#include "JOB.h"
//---------------------------------------------------------------------------
class READJOB:public JOB
{
    public:

        READJOB(void);
        ~READJOB(void);

        bool SetInterfaceAndReadFlag(int *,MESSAGEOK *,int);                    //設定此封包Read的flag
        bool CheckLcn(int *,MESSAGEOK *);                                       //中心來的封包,都要檢查路口編號是否符合
        bool vCheckIfBcastingForwardToUDP(int *,MESSAGEOK *);                   //檢查是否要轉傳，要的話就直接傳了
        bool SetCenterComm(int *,MESSAGEOK *);                                  //設定中心通訊
        bool SetKeypadComm(int *,MESSAGEOK *);                                  //設定鍵盤通訊
        bool DoWorkByMESSAGEIN(int *,MESSAGEOK *);                              //根據封包作事情
        bool DoSendUDP_MESSAGEIN(int *,MESSAGEOK *);

        bool vDoDisplay(int *,MESSAGEOK *);

        //OT Pass
        bool vDoRelayUDP_MESSAGEIN_0F8F(int *, MESSAGEOK *, unsigned char);
        //OT Pass 2
        bool ReadWorkByMESSAGEIN(MESSAGEOK);

    private:
        tsUDPMappingLCN _sUDPMappingLCNinREADJOB[MaxOpenUDPPort];

};
//---------------------------------------------------------------------------
extern READJOB readJob;
#endif

