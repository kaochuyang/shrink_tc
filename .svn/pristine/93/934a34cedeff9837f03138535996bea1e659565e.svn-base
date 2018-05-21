//---------------------------------------------------------------------------

#ifndef PTRAFFIC92COMMH
#define PTRAFFIC92COMMH

#include "var.h"
#include "SMEM.h"

//---------------------------------------------------------------------------
class PTRAFFIC92COMM
{
    public:

        PTRAFFIC92COMM(void);
        ~PTRAFFIC92COMM(void);

        bool DoWorkViaPTraffic92(MESSAGEOK);                                    //�ھ�PTraffic92�Ӫ��T���@�Ʊ�

        bool vRebootIPC_0F10(MESSAGEOK);
        bool vReportRebootMsg_0F90(void);
        bool vSetIPCTime_0F12(MESSAGEOK);                                       //�]�w�ɶ�
        bool vReportSecDif_0F92(void);
        bool vQueryIPCTime_0F42(MESSAGEOK);
        bool vReportIPCTime_0FC2(void);
        bool vReportIPCTime_0F02(void);
        bool vSetCommandSet_0F13(MESSAGEOK);
        unsigned char ucCommandSet;                                             //should save to mem
        bool vQueryVersion_0F43(void);
        bool vReportVersion_0FC3(void);
        bool vSetDBPassword_0F15(MESSAGEOK);
        bool vQueryPassword_0F45(void);
        bool vReportPassword_0FC5(void);

        bool vSetDBOperatingMode_0F16(MESSAGEOK);
        bool vQueryDBOperatingMode_0F46(void);
        bool vReportDBOperatingMode_0FC6(void);

        bool vQueryEquipmentNo_0F40(MESSAGEOK);
        bool vReportEquipmentNo_0FC0(unsigned char);
        bool vReportPowerOff_0F00(void);

        int vQueryProtocolStatus_0F47(MESSAGEOK);
        bool vReportProtocolStatus_0FC7(unsigned char, int);
        bool bProtocol0FStatus[256];
        bool bProtocol5FStatus[256];
//        bool bProtocol6FStatus[256];

        bool vSetCommunicationReset_0F11(MESSAGEOK);                            //�q�T���m�]�w
        bool vReportCommunicationReset_0F91();                                  //�^��q�T���m
        bool vQueryCommunicationReset_0F41(MESSAGEOK);                          //�d�M��^��HW���A
        bool vReportCommunicationStatus_0FC1();                                 //�^��

        bool vSetHardwareCycle_0F14(MESSAGEOK);                                 //�]�wHWStat�^�Ƕg��
        struct itimerspec _itimer_HardwareCycle;                                //HWStat�^�Ƕg ���Ϊ�Interval

        bool vSetHardWareReportCycleTransTo_itimer(unsigned short int);         //��92protocol��Ū��_itimer
        bool vQueryHardwareCycle_0F44(MESSAGEOK);                               //�d�^�Ƕg��
        bool vReportHardwareCycle_0FC4();                                       //�^��^�Ƕg��

        bool vReportHardwareCycle_0F04(MESSAGEOK);

        //OT Pass
        bool vPass_0F8E(MESSAGEOK);

        bool SendTimeisChangetoArwenActuate();//arwen++


    private:

        int SecDif;

        int vReturnToCenterACK(unsigned char , unsigned char);
        int vReturnToCenterNACK(unsigned char, unsigned char, unsigned char, unsigned char);

};
//---------------------------------------------------------------------------
#endif
