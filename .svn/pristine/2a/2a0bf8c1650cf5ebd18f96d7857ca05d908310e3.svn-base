//---------------------------------------------------------------------------
#ifndef WRITEJOBH
#define WRITEJOBH

#include "var.h"
#include "JOB.h"

//---------------------------------------------------------------------------
class WRITEJOB:public JOB
{
    public:

        WRITEJOB(void);
        ~WRITEJOB(void);

        bool WriteWorkByMESSAGEOUT(MESSAGEOK);

        bool WriteSUCCESS92(BYTE,BYTE);                                         //92年版回應成功(14) AA BB SEQ LCN LCN LEN LEN OF 80 CommandID1 CommandID2 AA CC CKS
        bool WriteFAIL92(BYTE,BYTE,BYTE,BYTE);                                  //92年版回應失敗(16) AA BB SEQ LCN LCN LEN LEN OF 81 CommandID1 CommandID2 ErrorCode ParameterNumber AA CC CKS
        bool WriteSUCCESS87(void);                                              //87年版回應成功(  )
        bool WriteFAIL87(void);                                                 //87年版回應失敗

        bool WriteACK92(BYTE);                                                  //92年版正認知碼(8) AA DD SEQ LCN LCN LEN LEN CKS
        bool WriteNAK92(BYTE,BYTE);                                             //92年版負認知碼(9) AA EE SEQ LCN LCN LEN LEN ERR CKS
        bool WriteACK77(void);                                                  //77年版正認知碼(5) AA DD LCN LCN CKS
        bool WriteNAK77(BYTE);                                                  //77年版負認知碼(6) AA EE LCN LCN ERR CKS

        bool WritePhysicalOut(BYTE *,int,int);                                  //實際寫出(輸入協定,長度,欲寫出的DEVICE)
        bool WritePhysicalOutINT(int *,int,int);
        bool WritePhysicalOutNoSetSeqNoResend(BYTE *,int, int);

        //OT Pass
        bool WriteACK92_To_Term(BYTE, BYTE, BYTE, unsigned char);

        bool WriteAskW77E58FWVer();
        bool WriteLetW77E58AutoControl();

    private:
        BCDSW bcdSwitchHi;
        BCDSW bcdSwitchLo;

};
//---------------------------------------------------------------------------
extern WRITEJOB writeJob;
#endif

