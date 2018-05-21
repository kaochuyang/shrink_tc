#ifndef CPEDPUSHBUTTON_H_INCLUDED
#define CPEDPUSHBUTTON_H_INCLUDED
#include "var.h"

#include <pthread.h>
#include <stack>
#include <queue>
#include <deque>
using namespace std;

typedef struct PEDInit {
        unsigned short int DeviceID;
        int staSec;
        unsigned short int PedSWVar;
        bool Log;
        unsigned short int vdSta;
        unsigned short int vd_stopped;
    }PEDInit;
class CPEDPUSHBUTTON
{
    public:
        CPEDPUSHBUTTON();
        ~CPEDPUSHBUTTON();
        bool QueryPEDState();//每秒詢問行人按鈕是否觸動
        bool SendPedSWLightOpenAndClose();//行人警示燈亮滅

        bool SetDevCount(unsigned short int);
        unsigned short int GetDevCount();

        bool SetDevID(unsigned short int,unsigned short int);
        unsigned short int GetDevID(unsigned short int);

        bool SetNowRs232Status(bool);//true:rs232正在收到協定
        bool GetNowRs232Status();  //由0xEA 0x46回報0xEA 0xC6的按鈕狀況

        bool SetWatchDogClean(unsigned short int);//watchDogClean
        bool SetWatchDogAdd(unsigned short int);//
        bool GetWatchDogStatus(unsigned short int);//

        bool ActivateTakePlace(unsigned short int);//行人按鈕被按
        bool VirtualVDMode(unsigned short int mode);//是否要送假VD
        unsigned short int GetVirtualVDMode();//是否要送假VD
        bool GetVirtualVDStaSttopped(unsigned int no,unsigned int & sta,unsigned int & stopped);
        bool VirtualVDInfo(unsigned short int DevID,unsigned short vdSta);//假VD
        bool sendVirtualVDData();

        bool SetPEDSWVar(unsigned short int,unsigned short int);
        unsigned short int GetPEDSWVar(unsigned short int);

        bool SetPEDNCTUSWVar(unsigned short int*);//交大按鈕

        bool vPushWriteOutMsgToQueue(unsigned char ucPriority, MESSAGEOK _MsgIn);
        bool vCheckQueueMsgForWriteOut(void);

        bool SetOpenSegmentAlter(unsigned short int);//平常日或假日segment 改變別套
        unsigned short int GetOpenSegmentAlter(void);

        bool SetCompensationAct(unsigned short int);//觸動分相不補償
        unsigned short int GetCompensationAct(void);

        bool SetPedisNCTU(bool);
        bool GetPedisNCTU();

        //jacky20160224
        bool SendPedSWConnetState0F08(unsigned char);



    private:
        pthread_mutex_t mutexPed;
        unsigned short int DeviceCount;

        bool isRs232Statas;
        queue<MESSAGEOK> _WaitWriteOutMsg[3];    //[3]:Priority, 0isForceSend, 1isHigh, 21isLow,
        unsigned char ucTimePeriod;

        PEDInit pedDev[20];
        unsigned short int ucPedSWVar;
        //-------------------------------
        unsigned short int ilastCurrentPlanID;
        unsigned short int ilastCurrentPhaseID;

        unsigned short int nowIDCount;
        unsigned short int addVar;

        bool m_openSegment;
        unsigned short int m_CompensationAct;
        unsigned short int m_VirtualVDMode;
        bool m_pedNCTU;//交大板

        //test0225
//        bool t1flag, t2flag, t3flag, t4flag;
//        struct timeval t1, t2, t3, t4, t5;
//        double time_diff(struct timeval, struct timeval);

};
#endif // CPEDPUSHBUTTON_H_INCLUDED
