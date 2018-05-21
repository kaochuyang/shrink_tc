#ifndef CTIMERH
#define CTIMERH

#include <pthread.h>
//--------------------------------------------------------------------------
class intervalTimer
{
    static pthread_mutex_t _ctimer_mutex;

    static void TimersCreating(void);
    static void TimersSetting(void);
    static pthread_t thread_forVDTimer;

    static timer_t _t1, _t2, _t3, _t4, _t5, _t6, _t7, _t8, _t9;
//OTSS++
    static timer_t _t10;                                                        // For SS XG
    static timer_t _tTrafficeLight;
    static struct itimerspec _it1, _it2, _it3, _it4, _it5, _it6, _it7, _it8, _it9;
    static struct itimerspec _it10;                                             // For SS XG
    static struct itimerspec _itTrafficeLight;
//OTSS--

//OTSS--
    static timer_t _t11;//arwen add;
    static struct itimerspec _it11;//arwen add;
    static void * PTime(void *);

 //   static timer_t _tBRTLightctrl;
 //   static struct itimerspec _itBRTLightctrl;

    public:
        intervalTimer();
        void ThreadsGenerate(void);
//        bool vSSInit(int);

        bool GetDoorStatus(void);
        //DATA_Bit _DByte;
        bool vCheckSMEM(void);

        bool TimersRead_BeforeResetCMOSTime();
        bool TimersReset_AfterResetCMOSTime();
//        bool vEveryOneSecCheckSS_S0SK();
//        bool vCheckAndReSendSS_S0_Status();
//        bool vCheckAndReSendSS_SK_Status();
//        int iS0Count;
//        int iSKCount;


        bool vCheckScreenAndDoSomething(void);
        bool vCheckVDUpdateDBAndDoSomething(void);

        bool vCommuncationReset(int);
        bool vDBLockRequest(int);
//        bool vSend92VD_6F02(void);
        bool vAllDynamicToTODCount(unsigned short int);

        unsigned short int vGetEffectTime(void);

        unsigned short int usiGreenConflictCount;

        bool vChildChain(void);

        bool vRefreshWatchDog(void);
        unsigned short int usiWatchDogCount;
        unsigned short int usiLCX405WatchDogCount;

        bool vSendHeartBeatToLCX405(void);
        void vReportCurrentOperationMode_5F08_inCtimer(void);

        //OT20140515
        bool vReportDynSegStatus_5F0D_inCtimer(void);
};
//--------------------------------------------------------------------------
extern intervalTimer _intervalTimer;
#endif
