#ifndef SMEMH
#define SMEMH

#include "var.h"
#include "STORAGE.h"
#include "RS232.h"
#include "UDP.h"
#include "temperatur_humidity_sensor.h"
#include "CPEDPUSHBUTTON.h"
#include "CPlanInfo.h"

#include <pthread.h>
//---------------------------------------------------------------------------
typedef struct MESSAGEWAITREPONSE {
               int sequence;
               BYTE packet[BUFFERSIZE];
               int length;
               int retryCount;
               time_t sendTime;
} MESSAGEWAITREPONSE;
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
class SMEM
{
    public:
        SMEM(void);
        ~SMEM(void);

        STORAGE disk;   //寫檔案用

        int usCutRedAlready;// For BRT odder 8/18/2014

        /*OTCombo0713*/
//        RS232 centerPort,lightPort,ssPort,keypadPort,testerPort,rtmsPort;       //交控中心,Light,SmartSensor,面板,手提測試機,RTMS
        RS232 centerPort,testerPort,keypadPort,lightPort;
        RS232 redCountPort;                                                     //紅燈倒數
        RS232 NSPredCountPort;                                                  //南科用紅燈倒數
        RS232 greenmanPort;                                                     //小綠人
        RS232 NMEAPort;                                                         //GPS Com2
        RS232 passingPort;
//OTSS++
        RS232 ssPort;
        RS232 com2Port;
        RS232 PedPushButtonPort;//add Arwen
        RS232 GPSPort;          //add chou 2013/03/25
        RS232 revLaneManualPort;
        RS232 shrink_port_com3;
        UDP revSyncSocket;

//OTSS++
        UDP centerSocket,testerSocket,tempSocket;                               //交控中心,手提測試機,保留
//        UDP VDBcastForListen5003Socket;                                         //VD廣撥Socket
        UDP DynCalServerInCrossSocket, DynCalServerInCenterSocket;              //全動態計算主機
        UDP SSVD01Socket, SSVD02Socket, SSVD03Socket, SSVD04Socket;

        //OT Pass
        UDP SSVD05Socket, SSVD06Socket, SSVD07Socket, SSVD08Socket, SSVD09Socket, SSVD10Socket,
            SSVD11Socket, SSVD12Socket, SSVD13Socket, SSVD14Socket, SSVD15Socket,
            SSVD16Socket, SSVD17Socket, SSVD18Socket, SSVD19Socket, SSVD20Socket,
            SSVD21Socket, SSVD22Socket, SSVD23Socket, SSVD24Socket, SSVD25Socket,
            SSVD26Socket, SSVD27Socket, SSVD28Socket, SSVD29Socket, SSVD30Socket;
//            SSVD31Socket, SSVD32Socket, SSVD33Socket, SSVD34Socket, SSVD35Socket,
//            SSVD36Socket, SSVD37Socket, SSVD38Socket, SSVD39Socket, SSVD40Socket;

/* OT ADD 950927 */
        UDP CCJDynCtlSocket;
        UDP CCJDynCtl2Socket;
        UDP ArwenVDInfoSocket;//arwen ++
        UDP WebSetSocket;//arwen ++
        //OT20140224
        UDP BRTSetSocket;
        //jacky20140327
        UDP JsonSetSocket;

        UDP amegidsSocket;
         UDP revAPP_socket;
        CPEDPUSHBUTTON cPedPushButton;


        void SetTemperHumi(BYTE tem_1,BYTE tem_2,BYTE humi_1,BYTE humi_2);
        void ReportTemperHumi_0F09();
        void Set_temper_humi_state(bool state);
        void WriteLCNby_shrinkAPP(int LCN);
        int getTemperature();
        int getHumidity();
        bool getT_H_state();
        bool getCom3GPS_state();
            void setCom3GPS_state(bool GPS_state);
        void ShrinkAPP_login(unsigned char *data);

        int GetcFace(void);                                                     //得到目前LCD上是哪個畫面
        int GetLastFace(void);                                                  //得到上次LCD上是哪個畫面
        int GetAddress(void);                                                   //得到目前路口編號
        int GetOperMode(void);                                                  //得到目前操作模式 1:遠端操作 0:現場操作
        int GetSequence(void);                                                  //得到機器上的流水編號
        bool GetKeypadReturn(void);                                             //得到keypad是否有回傳了
        char * GetPassword(void);                                               //得到控制器密碼
        int GetDbOperStat(void);                                                //得到資料庫操作設定(0:允許現場操作,1:不允許現場操作,2:只允許現場查看)
        bool GetSSComm(void);                                                   //得到與SmartSensor的通訊

        bool SetcFace(int);                                                     //設定目前LCD是哪個畫面
        bool SetLastFace(int);                                                  //設定上次LCD是哪個畫面
        bool SetAddress(int);                                                   //設定目前路口編號
        bool SetOperMode(int);                                                  //設定目前操作模式 1:遠端操作 0:現場操作
        bool SetKeypadReturn(bool);                                             //設定keypad是否有回傳了
        bool SetPassword(char,char,char,char,char,char);                        //設定控制器密碼
        bool SetDbOperStat(int);                                                //設定資料庫操作設定(0:允許現場操作,1:不允許現場操作,2:只允許現場查看)
        bool SetSSComm(bool);                                                   //設定與SmartSensor的通訊

        bool SetWaitMsg(int,BYTE *,int);                                        //記錄要等待回應的msg
        bool ClearWaitMsg(int);                                                 //消掉等候回應的msg

        time_t GetLastKeypadTime(void);                                         //得到最後一次按鍵盤的時間
        bool SetLastKeypadTime(void);                                           //設定最後一次按鍵盤的時間

        time_t GetStartTime(void);                                              //得到此程式啟動時間
        time_t GetCurrentTime(void) {return time(NULL);}


        struct itimerspec _ShareSimCycle;

        bool vSaveShareMemoryDataToDOM(void);

        bool vSetSimIntervalTime(itimerspec);
        bool bSimIntervalTimeChange;
        bool vGetSimIntervalTimeChangeStatus(void);
        bool vSetSimIntervalTimeChangeStatus(bool);

        bool vSet0FHardwareCycle(itimerspec);
        itimerspec _HWCycleReportInterval;
        bool vGet0FHardwareCycleChangeStatus(void);
        bool bHWCycleReportIntervalChange;
        bool vSet0FHardwareCycleChangeStatus(bool);
        itimerspec vGet0FHardwareCycle(void);
        bool vSetHWCycleCodeFor_0F14_0FC4(unsigned char);
        unsigned char vGetHWCycleCodeFor_0F14_0FC4(void);
        unsigned char HWCycleCode;


        unsigned char vGetHardwareStatus(int);                                 //只有vGet沒有vSet，故意的，由vGet自行判斷


        unsigned char ucCommandSet;                                             //for 0F should save
        bool vSet0FCommandSet(unsigned char);
        unsigned char vGet0FCommandSet(void);

        time_t vGetSSCommTime(void);

        DATA_Bit _DIOByte;
        DATA_Bit Get_DIOByte(void);

        SmartSensorINFO _SSInfoInShareMem;
        bool vSyncSSInfo(SmartSensorINFO);                                      //把SmartSensor之S0的Info紀錄到smem
/*new add */
        SmartSensorINFO vGetSSInfo(void);                                       //讀出S0data

        bool vSetLastResetTime(long);                                           //將DOM紀錄之最後時間寫入smem
        YMDHMS vGetLastResetTime(void);                                         //從smem讀出上次關機之時間

        int vGetConnectedSendIntervalTime(void);

        bool vGetCenterConnectOldStatus(void);                                  //上次連線狀態


        bool vCheckConnectStatus(void);

        bool bConnectWithCenter;                                                //紀錄與比較用的 old
        bool bConnectWithCenterStatus;                                          //連線中心的status
        bool vLoadCenterConnectStatus(void);
        bool vSaveCenterConnectStatus(bool);
        bool vSaveGPSStatus(bool);                                              //jacky20140116
        bool GPSStatus;                                                          //jacky20140116 GPS接收Status
        bool vSetTimeInterVal(int, int);
//OT Debug 0706        bool vSyncSSTimeInterVal(void);
        bool GetConnectCenterStatus(void);

        bool vGet0F11CommuncationResetChangeStatus();
        bool vSet0F11CommuncationResetChangeStatus(bool);
        bool b0F11CommuncationResetChangeStatus;

        bool vGet0F16LockDBRequestStatus();
        bool vSet0F16LockDBRequestStatus(bool);
        bool b0F16LockDBRequestStatus;


        bool vSet_DIOByte(DATA_Bit);

        DATA_Bit _OldDIOByte;
        DATA_Bit Get_OldDIOByte(void);

        itimerspec vReadSimIntervalTime(void);


        bool vWriteMsgToDOM(char *);
        bool vWriteMsgToTmp(char *);

        bool vSaveLastAliveStatus();                                             //當SS斷線時，用interval來紀錄時間toDOM
        int vGetFirmwareInfo_0FC3(int);

        int vGetAdjudicateReturnAddressBCDorHEX(void);
        bool vSetAdjudicateReturnAddressBCDorHEX(int);

        //OTCombo
        unsigned char vGetUCData(int);
        int vGetINTData(int);

        bool vSetUCData(const int , const unsigned char);
        bool vSetINTData(const int, const int);

//OT Debug 0523
        bool vSetUSIData(const int, const unsigned short int);
        unsigned short int vGetUSIData(const int);

        bool vSetBOOLData(const int, const bool);
        bool vGetBOOLData(const int);

        bool vGetTCPhasePlanSegTypeData(const int, const int);
        bool vSetTCPhasePlanSegTypeData(const int, const int, const bool);

        tsUDPMappingLCN vGetUDPMappingLCNData(int);

        unsigned short int vGetWayMappingRedCount(unsigned short int);
        unsigned short int vSetWayMappingRedCount(unsigned short int, unsigned short int);
        unsigned short int vGetWayMappingPedGreenCount(unsigned short int);                         //行人綠燈倒數用 chou++ 2013/05/22
        unsigned short int vSetWayMappingPedGreenCount(unsigned short int, unsigned short int);     //行人綠燈倒數用 chou++ 2013/05/22



//OT Pass
        bool vSetACK_W2W(unsigned char, unsigned char);
        unsigned char vGetACK_W2W(unsigned char);
        bool vSetPassMode(unsigned char);
        unsigned char vGetPassMode(void);
        bool vSetPassServerLCN(unsigned short int);
        unsigned short int vGetPassServerLCN(void);


//---------------------------------------------------------------------------
//OT Debug Direct
        unsigned short int vGetSignamMapMappingDir(unsigned short int);
        unsigned short int vSetSignamMapMappingDir(unsigned short int, unsigned short int);
        unsigned short int vGetSignamMapMappingLightBoard(unsigned short int);
        unsigned short int vSetSignamMapMappingLightBoard(unsigned short int, unsigned short int);

        unsigned short int vGetScreenCopyPlanID(void);
        bool vSetScreenCopyPlanID(unsigned short int);
        unsigned short int vGetScreenCopySegID(void);
        bool vSetScreenCopySegID(unsigned short int);

        tsUDPMappingLCN vGetCCJDynCtlIPData(void);
        tsUDPMappingLCN vGetCCJDynCtl2IPData(void);
        tsUDPMappingLCN vGetArwenVDInfoIPData(void);//arwen ++
        tsUDPMappingLCN vGetWebSetIPData(void);//arwen ++
        //OT20140224
        tsUDPMappingLCN vGetBRTSetIPData(void);
        //jacky20140327
        tsUDPMappingLCN vGetJsonSetIPData(void);

        bool vReadLCNFromDisk(void);

        //jacky20140411
        bool vReadCompensationFromDisk(void);
        //jacky20150325
        bool vReadCarLearnFromDisk(void);
        bool vReadPedLearnFromDisk(void);
        //jacky20150330
        bool vReadCarCountdownFromDisk(void);
        bool vReadPedCountdownFromDisk(void);

        bool vSaveLast92TC_5F15Time(void);
        bool vLoadLast92TC_5F15Time(void);
        YMDHMS vGetLast92TC_5F15Time(void);

        bool vCmpGreenConflict(void);

        sChildChain vGetChildChainStruct(void);
        bool vSetChildChainStruct(sChildChain);
        bool vClearChildChainStruct(void);

        int vGetChainOffset(const int, const int);
        bool vSetChainOffset(const int, const int, short int);
        bool vWriteDIO(unsigned char);
        unsigned char vGetWriteDIO(void);

        bool vWriteChainDataFromStroage(void);
        bool vReadChainDataFromStroage(void);

        //---------------------------------------------------------------------------
        bool SetLocalIP1(int,int);
        bool SetdistIp0(int,int);
        bool SetDistIP(int,int);
        bool SetNetmask(int, int);
        bool SetGateway(int, int);
        int GetLocalIP1(int);
        int GetdistIp0(int);
        int GetDistIP(int);
        int GetNetmask(int);
        int GetGateway(int);

        void vSendRequestToKeypad(void);

        bool vSetMotherBoardType(unsigned char);
        unsigned char vGetMotherBoardType(void);

        bool vSetActuatePhaseExtend(unsigned char);
        unsigned char vGetActuatePhaseExtend(void);

        bool vSendTimerUpdateToCCJ_5F9E(void);

        //For TC
        bool vSetVDUpdateDB(int, bool);
        bool vGetVDUpdateDB(int);

        bool vSetSignalConflictErrorVar(unsigned char, unsigned char, unsigned char);
        bool vGetSignalConflictErrorVar(unsigned char *);

        bool vTrafficLightSendAndCheck(void);
        bool vTrafficLightAckAdd(void);

        time_t GetRevManualPadTime(void);
        bool SetRevManualPadTime(void);

        bool vSetTimerMutexCSTC(unsigned char);
        unsigned char vGetTimerMutexCSTC(void);
        bool vSetTimerMutexCTIMER(unsigned char);
        unsigned char vGetTimerMutexCTIMER(void);
        bool vSetTimerMutexRESET(unsigned char);
        unsigned char vGetTimerMutexRESET(void);

        unsigned char vGetW77E58FWVer(unsigned char);
        bool vSetW77E58FWVer(unsigned char, unsigned char);

        bool vSetSystemClockTime(unsigned short int,                            //Year,month,day,hour,min,sec
                                 unsigned char,
                                 unsigned char,
                                 unsigned char,
                                 unsigned char,
                                 unsigned char
                                 );

        bool vSetVDPhaseMapTable(unsigned char, unsigned char, unsigned short int);
        unsigned short int vGetVDPhaseMapTable(unsigned char, unsigned char);
        bool vSetVDPhaseTriggerSwitch(unsigned char, unsigned char);
        unsigned char vGetVDPhaseTriggerSwitch(unsigned char);

        bool vSetForceClockRedCountOneTime(bool);
        bool vGetForceClockRedCountOneTime(void);

        bool vSetStopSend0x22(bool);
        bool vGetStopSend0x22(void);

        bool vSetTC5F08Status(unsigned char);
        unsigned char vGetTC5F08Status(void);

        bool vSetTC5F08StatusChange(bool);
        bool vGetTC5F08StatusChange(void);

        bool vSetThisCycleRunCCJPlan5F18(bool);
        bool vGetThisCycleRunCCJPlan5F18(void);
        bool vSetThisCycleRunCCJActure5FA2(bool);
        bool vGetThisCycleRunCCJActure5FA2(void);

        bool vSaveCurrentAsGreenStartTime(void);
        unsigned int vGetActRunningGreenTime(void);

        bool vSetLastRevSyncTime(time_t);

        bool vCheckRevSyncStatus(void);
        bool vSetRevSyncStatus(sRevSyncStatus);
        sRevSyncStatus vGetRevSyncStatus(void);

        bool vSetRevSyncVar(int);
        int vGetRevSyncVar(void);

        bool vGetCommEnable(void);
        bool vSetCommEnable(bool);

//OT1000218
        bool vSet5F18EffectTime(unsigned int);
        bool vSet5F18EffectTimeCountDown(void);
        unsigned int vGet5F18EffectTime(void);

        bool vSetLastGetProtocolTime(void);
        time_t vGetLastGetProtocolTime(void);
        bool vSetSystemTime(time_t _tmp);
        time_t vGetSystemTime(void);

        bool vSetShowRedCountVar(unsigned char, unsigned char, unsigned char, unsigned char, unsigned char, unsigned char, unsigned char, unsigned char);

        bool vLogTrafficLightTime(void);
        time_t vGetLastTrafficeLightTime(void);
        bool vSetTrafficLightBoardConnectStatus(bool);

        int vSystemClockTimetoAct(void);

        int ReadPEDInfo(void);//add Arwen
        bool readSegmentActInfo(void);//arwen ++ 1000929
        int getSegmentActVDPhaseTriggerSwitch(int planNo,int count);//arwen ++ 1000929
        int getSegmentActExtendSec(int planNo,int count);//arwen ++ 1000929
        int getSegmentActType(int planNo);//arwen ++ 1000929
        int getSegmentActPlan(int planNo);//arwen ++ 1000929
        int getSegmentZoneTimes(int planNo);//arwen ++ 1000929
        int getSegmentCompensateMode(int planNo);//arwen ++ 1000929
        int getSegmentActVDPhaseMap(int planNo,unsigned char ucPhase, unsigned char ucVD);//arwen ++ 1000929
        int saveSegmentActData(int planNo,int tmpactType,int *actSwitch,int *ExtendSec,int tmpactPlan,int tmpactionSec,int tmpCompensateMode);//arwen ++ 1000929
        bool setRunSegmentActMode(unsigned short int);
        unsigned short int getRunSegmentActMode();

        //---
        bool vCleanActCountEnd();
        bool vSetCurrentActCountAddVal();
        bool vSetCurrentActEndVal(unsigned short int);
        unsigned short int vGetCurrentActCountAddVal();
        unsigned short int vGetCurrentActEndVal();
        //-------------------------------------------------
        bool Restartcheck5F1C ; //Chou ++ 2012/10/02
           //---------------------------chou side
        bool SetPedlightcheck(bool) ;//確認行人閃燈狀態常時為0,觸發後為1   chou++ 2013/01/17
        bool GetPedlightcheck() ;
        bool Pedlightcheck;
        //---------------------------------------------------
        bool vSetExtendMode(bool);           // chou test 2013/05/28
        bool vGetExtendMode();               // chou test 2013/05/28
        //----------------------------------------------------
        bool SetPedopen(bool);
        bool GetPedopen();
        bool vsetGreenmancount(int pedtype,int pedphase ,unsigned short int peddata);
       unsigned short int vgetGreenmancount(int);
       unsigned short int vgetredmancount(int);
       bool vsetredcount(int lighttype,int lightphase ,unsigned short int lightdata);
       unsigned short int vgetgreencount(int);
       unsigned short int vgetredcount(int);
       bool vSetAdjcount(bool);
       bool vGetAdjcount();

       //OT20131219
       bool vSetRedCountRemainder(unsigned short int *);
       bool vSetPgCountRemainder(unsigned short int *);
       bool vSendPedAndRedCountEverySec(void);
       //OT20131221
       bool vSetPgRemainder(unsigned short int *);
       //jacky20141118
       bool vSetPrRemainder(unsigned short int *);
       //jacky 20131223
       bool GreenmanManual(bool (*bCountIF)[5][8]);
       //jacky 20140108 BRT
       unsigned char BRTOnBoardLED(unsigned char[]);


       //OT20140214
       bool vGet5F1CEffectTimeStatus(void);
       unsigned short int vGet5F1CEffectTime(unsigned short int);
       bool vSet5F1CEffectTime(unsigned short int in, unsigned short int);

       bool vSetStepRunCounter(unsigned short int);
       bool vPlusStepRunCounter(void);
       unsigned short int vGetStepRunCounter(void);
       bool vSetCurrentSubphaseRunning5F1C(unsigned short int, bool);
       bool vGetCurrentSubphaseRunning5F1C(unsigned short int);

       //jacky20140804
       bool vSetBRTCompensationSec(unsigned short int);
       bool vPlusBRTCompensationSec(unsigned short int);
       unsigned short int vGetBRTCompensationSec(void);

       //OT20140224
       bool vReadBRTSetFromStorage();

       //jacky20140327
       bool vReadJsonSetFromStorage();

       //jacky20140324
       unsigned char pCondition;
       bool SetBF08Condition(unsigned char);
       unsigned char GetBF08Condition(void);
       unsigned char pStrategy;
       bool SetBF08Strategy(unsigned char);
       unsigned char GetBF08Strategy(void);
       //jacky20140514
       unsigned char pParameter;
       bool SetBF08Parameter(unsigned char);
       unsigned char GetBF08Parameter(void);
       //jacky20140325
       unsigned char pCarposition;
       bool SetBF0BCarposition(unsigned char);
       unsigned char GetBF0BCarposition(void);
       unsigned char pRCarposition;
       bool SetBF0BRCarposition(unsigned char);
       unsigned char GetBF0BRCarposition(void);
       //jacky20140515
       unsigned char pDir;
       bool SetBF0BDIR(unsigned char);
       unsigned char GetBF0BDIR(void);
       //jacky20140327
       MESSAGEOK setmessageEast;
       bool SetBF0BmessageEast(MESSAGEOK);
       MESSAGEOK GetBF0BmessageEast(void);
       MESSAGEOK setmessageWest;
       bool SetBF0BmessageWest(MESSAGEOK);
       MESSAGEOK GetBF0BmessageWest(void);

       //jacky20140110
       unsigned short int setcyclenum;
       bool SetCompensationcycle(unsigned short int);
       unsigned short int GetCompensationcycle(void);
       //jacky20150325
       unsigned short int setCarnum;
       bool SetCarLearnSwitch(unsigned short int);
       unsigned short int GetCarLearnSwitch(void);
       unsigned short int setPednum;
       bool SetPedLearnSwitch(unsigned short int);
       unsigned short int GetPedLearnSwitch(void);
       //jacky20150330
       unsigned short int setCarProperty;
       bool SetCarCountdownProperty(unsigned short int);
       unsigned short int GetCarCountdownProperty(void);
       unsigned short int setPedProperty;
       bool SetPedCountdownProperty(unsigned short int);
       unsigned short int GetPedCountdownProperty(void);
       //jacky20150505
       unsigned char setExecute;
       bool SetPriorityexecute(unsigned char);
       unsigned char GetPriorityexecute(void);
       //jacky20151209
       unsigned char setVer;
       bool SetV3orBRT(unsigned char);
       unsigned char GetV3orBRT(void);
       //jacky20151210
       unsigned char setReq;
       bool SetRequestKeypad(unsigned char);
       unsigned char GetRequestKeypad(void);

       //jacky20140416
       int setMingreen;
       bool SetCompensationMingreen(int);
       int GetCompensationMingreen(void);

       //jacky20140424
       unsigned char setonff;
       bool SetBF11onoff(unsigned char);
       unsigned char GetBF11onoff(void);
       unsigned short int settime;
       bool SetBF11Time(unsigned short int);
       unsigned short int GetBF11Time(void);
       unsigned short int DefiniteTimeonoff;
       bool SetDefiniteTimeonoff(unsigned short int);
       unsigned short int GetDefiniteTimeonoff(void);
       bool Priorityswitch ;
       bool SetPriorityswitch(bool);
       bool GetPriorityswitch(void);
       //jacky20140426
       bool SetGreenManonoff5014(unsigned char);
       unsigned char greenmanonoff;
       unsigned char GetGreenManonoff5014(void);
       bool SetCaronoff5015(unsigned char);
       unsigned char caronoff;
       unsigned char GetCaronoff5015(void);
       //jacky20140430
       bool SetOperateConditionBF0D(unsigned char);
       unsigned char Operate;
       unsigned char GetOperateConditionBF0D(void);
       bool SetScheduleTHBF0D(unsigned int);
       unsigned int schedule;
       unsigned int GetScheduleTHBF0D(void);
       bool SetStatusBF0D(unsigned char);
       unsigned char status;
       unsigned char GetStatusBF0D(void);
       bool SetDelayTimeBF0D(unsigned int);
       unsigned int DelayTime;
       unsigned int GetDelayTimeBF0D(void);
       //jacky20140521
       bool Set_exeBF13(CPlanInfo);
       CPlanInfo _exeplan;
       CPlanInfo Get_exeBF13(void);
       //jacky20140522
       bool SetSRC1BF00(unsigned char);
       unsigned char SRC1;
       unsigned char GetSRC1BF00(void);
       bool SetSRC2BF00(unsigned char);
       unsigned char SRC2;
       unsigned char GetSRC2BF00(void);
       //jacky20140730
       bool vSetSystemClockTimeSelect(unsigned char);
       unsigned char vComeform;
       unsigned char vGetSystemClockTimeSelect(void);
       bool sGPSSetTimeSwitch(unsigned char);
       unsigned char gComeform;
       unsigned char sGPSGetTimeSwitch(void);
       //jacky20140731
       bool SetExtendedswitch(unsigned char);
       unsigned char sonoff;
       unsigned char GetExtendedswitch(void);
       //jacky20140812
       bool SetSegmentChange(bool);
       bool segchange;
       bool GetSegmentChange(void);
       //jacky20140813
       bool SetBRTRemainCompensation(unsigned short int);
       unsigned short int RemainSec;
       unsigned short int GetBRTRemainCompensation(void);
       //jacky20140821
       bool SetPriorityStrategiesIN(bool);
       bool sIN;
       bool GetPriorityStrategiesIN(void);
       //jacky20141202
       bool SetLearningPGPRAcrossCycle(bool);
       bool PGPRAcrossCycle;
       bool GetLearningPGPRAcrossCycle(void);
       bool SetLearningPGPRThreeSecondoff(bool);
       bool PGPRThreeSecond;
       bool GetLearningPGPRThreeSecondoff(void);
       bool SetLearningPGPROneSecondon(bool);
       bool PGPRTwoSecond;
       bool GetLearningPGPROneSecondon(void);
       bool SetLearningPGPRsubphase(bool);
       bool PGPRsubphase;
       bool GetLearningPGPRsubphase(void);
       //jacky20141217
       bool SetPlantransition(bool);
       bool Plantransition;
       //jacky20141227
       bool SetClearBF0BEast(bool);
       bool ClearBF0BEast;
       bool GetClearBF0BEast(void);
       bool SetClearBF0BWest(bool);
       bool ClearBF0BWest;
       bool GetClearBF0BWest(void);

       //OT20131225
       bool vAddCCJHeartBeatCount(unsigned short int);

       bool vGetPlanForceTOD(unsigned short int);
       bool vSetPlanForceTOD(unsigned short int, bool);


    private:
        pthread_mutex_t mutexSmem;                                              //保護記憶體

        void InitialSMem(void);                                                 //初始化記憶體或通訊重置

        int address;                                                            //此設備路口編號
        int cFace;                                                              //目前LCD畫面上的頁面
        int lastFace;                                                           //上一次停留的畫面
        int operMode;                                                           //機器操作模式 1:遠端操作 0:現場操作
        int sequence;                                                           //機器上要送出的流水編號
        char password[6];                                                       //控制器密碼
        int dbOperStat;                                                         //資料庫操作設定(0:允許現場操作,1:不允許現場操作,2:只允許現場查看)

        bool ssComm;                                                            //與SmartSensor的通訊
        time_t ssCommTime;

        time_t startTime;

        bool keypadReturn;                                                      //我們必須借由keypad的回傳值來設定路口編號(address)及操作模式(operMode)
        time_t lastKeypadTime;                                                  //最後一次按鍵盤的時間


        MESSAGEWAITREPONSE msgWaitResponse[MSGWAITREPONSECOUNT];                //重傳機制


        YMDHMS ResetTime;

        unsigned char ucActuateType;
        unsigned char ucOccupyTime;
        unsigned char ucSettingLaneCount;
        unsigned char ucDetectorMap;
        short int siTimeGap;
        unsigned short int usiBigCarLength;
        unsigned short int usiCarLength;
        short int siTransmitType[5];
        long lLastRebootSec;

        bool vSave92COMMToDisk();
        bool vRead92COMMFromDisk();

        int iFirmwareYear;
        int iFirmwareMonth;
        int iFirmwareDay;
        int iFirmwareCompanyID;
        int iFirmwareFirstVersion;
        int iFirmwareSecondVersion;

        int iAdjudicateReturnAddressBCDorHEX;

        //OTCombo

        bool vSave92TCSettingToDisk(void);
        bool vRead92TCSettingFromDisk(void);

        unsigned char ucSmem92TC_ControlStrategy;
        int iSmem92TC_EffectTime;
        int iSmem92TC_PhaseUpdateFlag;
        bool bSmem92TC_TransmitCycle5F03_ChangeFlag;
        bool bSmem92TC_TransmitCycle5F0F_ChangeFlag;
        int iSmem92TC_SignalLightStatus_5F0F_IntervalTime;
        int iSmem92TC_SignalStepStatus_5F03_IntervalTime;

        bool bSmem92TC_ChangePlanOneTime5F18_ChangeFlag;
        int iSmem92TC_ChangePlanOneTime5F18_PlanID;

        int iSmemTC_RedCountVer;                                                //紅燈倒數版本

        bool bSmemTC_PhaseAlive[255];                                           //快速判斷Phase是否存在
        bool bSmemTC_PlanAlive[255];
        bool bSmemTC_SegTypeAlive[255];

        bool bTCDoorStatus;

        bool bHaveTCDoorSwitch;                                                 //have tc hardward door check switch

        bool bTCSignalConflictError;

        bool bSmem92TC_SubPhaseOfPhasePlanIncorrent;                            //當Phase與Plan定義的subphase不同時，此flag為true

//OT Debug 0523
        bool bTC_ActuateTypeFunctionEnable;
        bool bSmemTC_CCT_In_LongTanu_ActuateType_Switch;                        //龍潭特別觸動, 1 cycle change execplan
        unsigned short int usiSmemTC_CCT_In_LongTanu_ActuateType_PlanID;                         //when actuating, change to this plan
        bool bActuateRunningFlag;
        unsigned char ucActuatePhaseExtend;                                     //trash

        bool vReadUDPMappingLCNDataFromStorage();
        tsUDPMappingLCN _sUDPMappingLCN[MaxOpenUDPPort];

        unsigned short int usiWayMappingRedCount[8];
        unsigned short int usiWayMappingPedGreenCount[8];

        int iSmem_Com2Type;

        unsigned char ucSignalLightStatus[14];                                  // 7 x 2 Lane Signal

        //OT Debug 0410
        int iSmemTC_SPID;
        int iSmemTC_SID;
        int iSmemTC_ET;

        //OT Pass
        unsigned char ucACKW2W[256];                                            //Seq only have 1 byte
        unsigned char ucPassMode_0F8E8F;
        unsigned short int usiPassServerLCN;

        //OT Debug Direct
        unsigned short int usiSignamMapMappingDir[8];
        unsigned short int usiSignamMapMappingLightBoard[6];

        unsigned short int usiScreenCopyPlanID;
        unsigned short int usiScreenCopySegID;

        tsUDPMappingLCN _sCCJDynCtl;
        tsUDPMappingLCN _sCCJDynCtl2;
        tsUDPMappingLCN _sArwenVDInfo;//arwen ++
        tsUDPMappingLCN _sWebSet;//arwen ++
        //OT20140224
        tsUDPMappingLCN _sBRTSet;
        //jacky20140327
        tsUDPMappingLCN _sJsonSet;
        bool vReadCCJDynCtlFromStorage(void);
        bool vReadCCJDynCtl2FromStorage(void);
        bool vReadArwenVDInfoFromStorage(void);//arwen ++
        bool vReadWebSetFromStorage(void);//arwen ++

//OT Debug Signal 951116
        bool bTC_SIGNAL_NEXT_STEP_OK;

        bool bEnableLCNUsingStorage;
        unsigned int uiLCNUsingStorage;
        //jacky20140411
        bool bEnalbecompensation;
        unsigned short int cyclenum;
        //jacky20150325
        bool bEnalbeCarLearn;
        unsigned short int CarLearn;
        bool bEnalbePedLearn;
        unsigned short int PedLearn;
        //jacky20150330
        bool bEnalbeCarCountdown;
        unsigned short int CarCountdownnum;
        bool bEnalbePedCountdown;
        unsigned short int PedCountdownnum;
        //jacky20160223
        bool bEnablePedSWDevCount;
        unsigned short int PedSWDevCount;

        long lLast92TC_5F15Time;

        //Debug 951128 for dongSignal driver unit
        bool bSignalDriverStatus;

        unsigned char ucGreenConflictDetFromCSTC;
        unsigned char ucGreenConflictDetFromDIO;

        sChildChain sSMEMChildChain;
        unsigned char ucTC92_5F31Manual;
        unsigned char ucTC92_5F31TOD;
        unsigned char ucTC92_5F31StartSubPhaseId;
        unsigned char ucTC92_5F31EndSubPhaseId;
        unsigned char ucTC92_5F32StartSubPhaseId;
        unsigned char ucTC92_5F32EndSubPhaseId;
        unsigned char ucTC_MotherChainStartStepId;
        unsigned char ucTC_MotherChainEndStepId;
        short int siTC92_5F33StartOffset[64];
        short int siTC92_5F33EndOffset[64];

        unsigned char ucSendDigitalIO;

        int localIp1_1,localIp1_2,localIp1_3,localIp1_4,localPort1;
        int distIp0_1,distIp0_2,distIp0_3,distIp0_4,dist0Port;
        int distIp1,distIp2,distIp3,distIp4,distPort;
        //OT20111020
        int netmask1, netmask2, netmask3, netmask4;
        int gateway1, gateway2, gateway3, gateway4;

        unsigned char ucMBType;                                                 // 0:Arbor 1:ICOP6047 2:ICOP6115

        bool bGPSSyncStatus;
        bool bActuateByTFD;

        unsigned char ucTC_KeyPadP6Value;

        bool b92VDUpdateDB_b0;
        bool b92VDUpdateDB_b1;
        bool b92VDUpdateDB_b2;
        bool b92VDUpdateDB_b3;
        bool b92VDUpdateDB_Status;                                              //自已加的，讓Interval判斷比較快

        bool bEnableSendStepToCCJ;

        bool bNextStepForceEnable;
        unsigned short int usiActuateVDID;
        //--------------------------------------------------------------------------chou+ for BUSEDIT 2012/10/01
        bool bActuateautoswitch;

        //---------------------------------------------------------------------------------------
        unsigned char ucTC_TrainChainEnable;
        unsigned char ucTC_TrainChainStatusNOW;
        unsigned char ucTC_TrainChainStatusLast;
        unsigned char ucTrainComingBanSubphase;
        unsigned char ucTrainComingForceJumpSubphase;
//        unsigned char ucTrainComingBanSubphaseLast;

        unsigned char ucSignalConflictErrorVar[3];

        unsigned int uiTrafficLightSend;
        unsigned int uiTrafficLightACK;

        unsigned char ucReverseLaneManualEnable;
        unsigned char ucReverseLaneControlMode;                                 // 0: auto, 1:Manual, 2:ManualToAuto
        time_t lastRevManualPadTime;

        //OT980420
        unsigned char ucBootingDisplayRedcount;
        unsigned char ucDisplayRedcount;

        //jacky20140115 BRT
        unsigned char ucBRTDisplayLight;

        //CCJ20140526 觸動型態
        unsigned char ucActType;
        unsigned short int usiActPlan;

        bool bEnableUpdateRTC;
        bool bGetResetTCSignal;
        unsigned char ucCSTCControlStrategy;
        unsigned char ucCSTCFieldOperate;
        unsigned char ucTimerCSTC;
        unsigned char ucTimerCTIMER;
        unsigned char ucTimerRESET;

        unsigned char ucW77E58FWVer[3];
        unsigned char ucTC_ActuateTypeByTOD;

        int usiVDPhaseMap[8][8];
        int ucVDPhaseTriggerSwitch[8];
        bool bForceClockRedCountOneTime;

        bool bBugStopSend0x22ForceCtl51Light;  //in version 2009/7/2, when send 0x22, 51 light borad will be controlled immd.
        unsigned char ucTC5F08Status;
        bool bTC5F08StatusChange;
        bool bThisCycleRunCCJPlan5F18;
        bool bThisCycleRunCCJActure5FA2;

        unsigned char ucSmemTC_DynShowRedCountForAct;

        timespec actGreenStartTimeForScreenActuateArwenStatus;
        timespec actGreenCurrentTimeForScreenActuateArwenStatus;

        unsigned char ucSendCCTActArwenProtocal;

        bool bTCSegmentTypeUpdate;
        time_t _RecRevSyncTime;

        unsigned char ucRevSyncEnable;
        unsigned char ucRevSyncInterval;
        unsigned char ucRevSyncTimeDiffTolarence;
        unsigned char ucRevSyncCountineStatusAdj;
        unsigned char ucRevDefaultVehWay;
        unsigned char ucRevLogToFile;
        unsigned char ucRevSendProtocol;
        unsigned char ucRevSyncByteStatus;

        sRevSyncStatus _NewestRevSyncStatus;
        int iRevSyncStatus;  // -1: error, 0:unknow, 1:sync ok
        int iLastRevSyncStatus;  // -1: error, 0:unknow, 1:sync ok

        unsigned char ucAmegidsDynEnable;
        unsigned short int usiAmegidsDynDevLCN;

        unsigned short int usiTODPlanID;
        bool bCommEnableSwitch;

        unsigned int ui5F18EffectTime;

        //OT20110526
        time_t lastGetProtocolTime;
        time_t _SystemTime;
        time_t _SystemTimeTmp;

        unsigned char ucLastShowRedCountVar[8];
        time_t ucLastShowRedCountTime;

        //OT20110607
        unsigned char ucSendCCTPhaseCycleProtocalForCenter;
        unsigned char ucSegmentNoRunning;

 		//OT20110624
        unsigned char ucRedcountRepeatCount;

        //OT20110825
        time_t _lastGetTrafficLightTime;
        bool bTrafficeLightBoardConnectStatus;

        //OT20111128
        unsigned char ucMachineLocation;
        //OT20131225
        unsigned short int usiCCJHeartBeatCount;

        //OT20140211
        bool bPlanForceTOD[50];

        //ARWEN++1001207
        unsigned char ucPedestrianModeOpen;
        unsigned char ucfixedGreenModeOpen;
        unsigned char ucActModeChange;
        unsigned char ucArwenActRedCountSec;

        int actVDPhaseTriggerSwitch[10][8];
        int actTimeExtend[10][8];
        int actVDPhaseMap[10][8][8];
        int actType[10];
        int actPlanId[10];
        int zonetimes[10];
        int actCompensateMode[10];

        int m_RunningSegActMode;

        unsigned short int m_CurrentActCount;
        unsigned short int m_CurrentEnd;
        //-----------------------------------------------
        bool ExtendModecheck;  //檢查是否為com port擴充模式 chou++ 2013/05/24
        bool pedlock;
        //-----------------------------------------------
        unsigned short int m_greenman[8];   //儲存行人綠燈各phase該倒數的秒數 chou++ 2013/05/28
        unsigned short int m_greenremainder[8]; //儲存剩餘的倒數秒數 chou++ 2013/05/28
        unsigned short int m_redman[8];   //儲存行人綠燈各phase該倒數的秒數 chou++ 2013/05/28
        unsigned short int m_redremainder[8]; //儲存剩餘的紅燈倒數秒數 chou++ 2013/05/28
        unsigned short int m_redcount[8];     //儲存紅燈倒數各phase該倒數的秒數 chou++ 2013/06/05
        unsigned short int m_greencount[8];   ////儲存綠燈倒數各phase該倒數的秒數 chou++ 2013/06/05
        bool m_adjcount;        //flag 紀錄是否有收到GPS訊號

        //OT20131219
        unsigned short int redcount_remainder[8];
        //OT20131221
        unsigned short int Pg_remainder[8];
        unsigned short int Pr_remainder[8];
        //jacky 20131223
        bool CheckbCountIF[8][5][8];
        //jacky 20140108
        unsigned char BoardLed[12];  //BRT

        bool FlashClear;

        //OT20140214
        unsigned short int effectTime5F1C[8];
        unsigned short int usiStepRunCount;
        bool bCurrentSubphaseRunning5F1C[8];

        //jacky20140804
        unsigned short int usiBRTCompensationSec;

        //OT20140415
        bool bDynSegSwitch;
//        unsigned char ucDynSegAdjustType;
        unsigned char ucDynSegAdjustHour;
        unsigned char ucDynSegAdjustMin;
        unsigned short int usiDynSegAdjustKeepTime;
        bool bDynSegStatus;
        bool bDynSegNextPlanIdSendToCCJ;//CCJ20150910 是否要傳下週期的計劃給CCJ動態程式
        unsigned short int usiDynSegRemainSec;



        bool temper_humi_state;
        int temperature;
        int humidity_record;
        bool com3_GPS_state;


};
//---------------------------------------------------------------------------
extern SMEM smem;
#endif
