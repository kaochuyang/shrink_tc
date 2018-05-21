//---------------------------------------------------------------------------
#include "PDEVICETRAFFICLIGHT.h"
#include "SMEM.h"
#include "WRITEJOB.h"

#include "CDataToMessageOK.h"

#include "CSTC.h"

#include "CSTORINI.h"

#include <stdio.h>
//---------------------------------------------------------------------------
PDEVICETRAFFICLIGHT::PDEVICETRAFFICLIGHT(void)
{
  //OT20110930
  iAskW77E58CounterReturn = 0;
}
//---------------------------------------------------------------------------
PDEVICETRAFFICLIGHT::~PDEVICETRAFFICLIGHT(void)
{
}
//---------------------------------------------------------------------------
bool PDEVICETRAFFICLIGHT::DoWorkViaPDevice(MESSAGEOK message)
{
try {
    char cTMP[128];
    unsigned char ucRet;
    unsigned int count = 0;           // BRT 燈卡回報數計數

    switch (message.packet[2]) {
            case 0x46:                                           //down board(LCX-405) autoruning, last step
                smem.vSetUCData(TC_Redcount_Display_Enable, 1);
                smem.vSetUCData(BRT_Light_Display_Enable, 1);     //jacky20140115 BRT Light Switch
                ucRet = smem.vGetUCData(TC_Redcount_Booting_Display_Enable);           // this machine have autorun 8051 IC
                if(ucRet == 1) {
                    smem.vSetUCData(TC_Redcount_Booting_Display_Enable, 0);
                }

                smem.vWriteMsgToDOM("From AutoRun To TC");
                stc.vGetLCX405ControlPower();
                smem.vSetBOOLData(TC_SignalConflictError, false);

                smem.vSetStopSend0x22(false);  //means have autorun 51

                oStorINI.vSaveIntToINI("LightCtlBoard", "AutoRun51", 1);

                smem.SetRequestKeypad(1);

              //OT980420, let redcount display open.
//move to stc.vGetLCX405ControlPower()              smem.vSetUCData(TC_Redcount_Display_Enable, 1);
            break;

            case 0x45:             //GreenConflict 綠衝突
                sprintf(cTMP, "Get GreenColfiect:%02X,%02X,%02X", message.packet[3], message.packet[4], message.packet[5]);
                smem.vWriteMsgToDOM(cTMP);
                printf("%s\n", cTMP);
                smem.vSetBOOLData(TC_SignalConflictError, true);
                smem.vSetSignalConflictErrorVar(message.packet[3], message.packet[4], message.packet[5]);

              //OT980420, let redcount display close.

                printf("Lock_to_Set_Control_Strategy by TRAFFICELIGHT!!!\n");
                printf("Lock_to_Set_Control_Strategy by TRAFFICELIGHT!!!\n");
                printf("Lock_to_Set_Control_Strategy by TRAFFICELIGHT!!!\n");

                stc.Lock_to_Set_Control_Strategy(STRATEGY_FLASH);
                smem.vSetUCData(TC_Redcount_Display_Enable, 0);
            break;

            case 0x48:
                smem.vTrafficLightAckAdd();
            break;

            case 0x44:                                                          //Error Happen, not send WDT.
                smem.vWriteMsgToDOM("From AutoRun To TC, No WDT, do nothing");
            break;

            case 0x61:                                                          //Get W77E58 Ver
                smem.vSetW77E58FWVer(0, message.packet[3]);
                smem.vSetW77E58FWVer(1, message.packet[4]);
                smem.vSetW77E58FWVer(2, message.packet[5]);

                if(iAskW77E58CounterReturn == 2) {
                    oStorINI.vSaveIntToINI("LightCtlBoard", "FWVerYear", message.packet[3]);
                    oStorINI.vSaveIntToINI("LightCtlBoard", "FWVerMonth", message.packet[4]);
                    oStorINI.vSaveIntToINI("LightCtlBoard", "FWVerDay", message.packet[5]);

                    bzero(cTMP, 128);
                    sprintf(cTMP, "Ask, W77E58 FW Ver:%02d-%02d-%02d", message.packet[3], message.packet[4], message.packet[5]);
                    smem.vWriteMsgToDOM(cTMP);
                }
                iAskW77E58CounterReturn++;

            break;

            case 0x62:                                                          //When booting, Get W77E58 Ver
                smem.vSetW77E58FWVer(0, message.packet[3]);
                smem.vSetW77E58FWVer(1, message.packet[4]);
                smem.vSetW77E58FWVer(2, message.packet[5]);
                bzero(cTMP, 128);
                sprintf(cTMP, "TrafficBoard W77E58 Booting, FW Ver:%02d-%02d-%02d", message.packet[3], message.packet[4], message.packet[5]);
                smem.vWriteMsgToDOM(cTMP);
            break;

            case 0x63:                                                          //booting WDT, Get W77E58 Ver
                smem.vSetW77E58FWVer(0, message.packet[3]);
                smem.vSetW77E58FWVer(1, message.packet[4]);
                smem.vSetW77E58FWVer(2, message.packet[5]);
                bzero(cTMP, 128);
                sprintf(cTMP, "TrafficBoard W77E58 WDT Start, FW Ver:%02d-%02d-%02d", message.packet[3], message.packet[4], message.packet[5]);
                smem.vWriteMsgToDOM(cTMP);
            break;

            case 0x15:        //BRT GreenConflict 綠衝突
                BRTGreenConflict(message.packet[3],message.packet[4],message.packet[5],message.packet[2]);
            break;

            case 0x16:      //BRT 觸動連鎖狀態 (TouchChain)
                TC0=message.packet[3];  TC1=message.packet[4];  TC2=message.packet[5];
                TC_DATA0.DBit=TC0;      TC_DATA1.DBit=TC1;      TC_DATA2.DBit=TC2;
                if(TC_DATA0.switchBit.b1 == 1) {    //Door Open
                    if(smem.vGetBOOLData(TC_DoorStatus_Change) == false) {
                     //OT FIX Door
                     smem.vSetBOOLData(TC_DoorStatus_Change, true);
                     unsigned char uc0F04[4];
                     uc0F04[0] = 0x0F;
                     uc0F04[1] = 0x04;
                     MESSAGEOK _MSG;
                     //send HW Status
                     uc0F04[2] = smem.vGetHardwareStatus(3);        //取得HWstat
                     uc0F04[3] = smem.vGetHardwareStatus(4);        //取得HWstat
                     _MSG = oDataToMessageOK.vPackageINFOTo92Protocol(uc0F04, 4, true);
                     _MSG.InnerOrOutWard = cOutWard;
                     writeJob.WritePhysicalOut(_MSG.packet, _MSG.packetLength, DEVICECENTER92);
                   }
                } else {
                   if(smem.vGetBOOLData(TC_DoorStatus_Change) == true) {
                     //OT Fix Door
                     smem.vSetBOOLData(TC_DoorStatus_Change, false);
                     unsigned char uc0F04[4];
                     uc0F04[0] = 0x0F;
                     uc0F04[1] = 0x04;
                     MESSAGEOK _MSG;
                     //send HW Status
                     uc0F04[2] = smem.vGetHardwareStatus(3);        //取得HWstat
                     uc0F04[3] = smem.vGetHardwareStatus(4);        //取得HWstat
                     _MSG = oDataToMessageOK.vPackageINFOTo92Protocol(uc0F04, 4, true);
                     _MSG.InnerOrOutWard = cOutWard;
                     writeJob.WritePhysicalOut(_MSG.packet, _MSG.packetLength, DEVICECENTER92);
                   }
                }
            break;

            case 0x17:      //BRT ADC命令
                ADC0=message.packet[3]; ADC1=message.packet[4]; ADC2=message.packet[5];
                ADC_DATA0.DBit=ADC0;    ADC_DATA1.DBit=ADC1;    ADC_DATA2.DBit=ADC2;
//                printf("\n\n &&&&&&&&&&&&& ADC0: %X  ADC1: %X  ADC2: %X",ADC0,ADC1,ADC2);
//                printf("\n &&&&&&&&&&&&& ADC0: %X  ADC1: %X  ADC2: %X",ADC0,ADC1,ADC2);
//                printf("\n &&&&&&&&&&&&& ADC0: %X  ADC1: %X  ADC2: %X\n\n",ADC0,ADC1,ADC2);
            break;

//            case 0x19:      //BRT 燈卡 回報8051Status
//                statusCOUNT=message.packet[3];
//                GC_status0=message.packet[4];   GC_status1=message.packet[5];   GC_status2=message.packet[6];
//                TC_status0=message.packet[7];   TC_status1=message.packet[8];   TC_status2=message.packet[9];
//                ADC_status0=message.packet[10]; ADC_status1=message.packet[11]; ADC_status2=message.packet[12];
//
//                if(statusCOUNT==0x28) count = 0;
//                else count+=1;
//
//                if(GC_status0==0xFF && GC_status1==0xFF && GC_status2==0xC0){ }
//                else BRTGreenConflict(GC_status0,GC_status1,GC_status2,message.packet[2]);
//
//               /* if(TC_status0==TC0){ } else { TC_DATA0.DBit=TC_status0; }
//                if(TC_status1==TC1){ } else { TC_DATA1.DBit=TC_status1; }
//                if(TC_status2==TC2){ } else { TC_DATA2.DBit=TC_status2; }
//
//                if(ADC_status0==GC0){ } else { ADC_DATA0.DBit=GC_status0; }
//                if(ADC_status1==GC1){ } else { ADC_DATA1.DBit=GC_status1; }
//                if(ADC_status2==GC2){ } else { ADC_DATA2.DBit=GC_status2; }
//                */
//            break;

            default:
//too much              smem.vWriteMsgToDOM("W77E58 Get unknow msg");
            break;
    }
    smem.vLogTrafficLightTime();
    return true;

  } catch (...) {}
}
//---------------------------------------------------------------------------
void PDEVICETRAFFICLIGHT::BRTGreenConflict(BYTE GC0,BYTE GC1, BYTE GC2, BYTE CMD)
{
try{
    char cTMP[128];
    unsigned char cardnumber = 0x00;  // BRT 燈卡編號
    unsigned char LEDnumber = 0x00;   // NO.1 左綠 NO.2 綠  NO.3 直綠

    GC_DATA0.DBit=GC0;      GC_DATA1.DBit=GC1;      GC_DATA2.DBit=GC2;
    sprintf(cTMP, "Get GreenConflict:%02X,%02X,%02X,CMD:%02X", GC0, GC1, GC2, CMD);
    smem.vWriteMsgToDOM(cTMP);
    printf("%s\n", cTMP);
    smem.vSetBOOLData(TC_SignalConflictError, true);

    if(GC_DATA0.switchBit.b1==0 || GC_DATA0.switchBit.b2==0 || GC_DATA0.switchBit.b3==0){
        cardnumber = 0x01;
    }else if(GC_DATA0.switchBit.b4==0 || GC_DATA0.switchBit.b5==0 || GC_DATA0.switchBit.b6==0){
        cardnumber = 0x02;
    }else if(GC_DATA0.switchBit.b7==0 || GC_DATA0.switchBit.b8==0 || GC_DATA1.switchBit.b1==0){
        cardnumber = 0x03;
    }else if(GC_DATA1.switchBit.b2==0 || GC_DATA1.switchBit.b3==0 || GC_DATA1.switchBit.b4==0){
        cardnumber = 0x04;
    }else if(GC_DATA1.switchBit.b5==0 || GC_DATA1.switchBit.b6==0 || GC_DATA1.switchBit.b7==0){
        cardnumber = 0X05;
    }else if(GC_DATA1.switchBit.b8==0 || GC_DATA2.switchBit.b7==0 || GC_DATA2.switchBit.b8==0){
        cardnumber = 0x06;
    }

    if(GC_DATA0.switchBit.b1==0 || GC_DATA0.switchBit.b4==0 || GC_DATA0.switchBit.b7==0 || GC_DATA1.switchBit.b2==0 || GC_DATA1.switchBit.b5==0 || GC_DATA1.switchBit.b8==0){
        LEDnumber = 0x01;
    }else if(GC_DATA0.switchBit.b2==0 || GC_DATA0.switchBit.b5==0 || GC_DATA0.switchBit.b8==0 || GC_DATA1.switchBit.b3==0 || GC_DATA1.switchBit.b6==0 || GC_DATA2.switchBit.b7==0){
        LEDnumber = 0x02;
    }else if(GC_DATA0.switchBit.b3==0 || GC_DATA0.switchBit.b6==0 || GC_DATA1.switchBit.b1==0 || GC_DATA1.switchBit.b4==0 || GC_DATA1.switchBit.b7==0 || GC_DATA2.switchBit.b8==0){
        LEDnumber = 0x03;
    }

    smem.vSetSignalConflictErrorVar(cardnumber, LEDnumber, 0x00);

    stc.Lock_to_Set_Control_Strategy(STRATEGY_FLASH);
    //printf("------------ BRT GreenConflict ------------\n");
    //printf("------------ BRT GreenConflict ------------\n");
    //printf("------------ BRT GreenConflict ------------\n");
    smem.vSetUCData(TC_Redcount_Display_Enable, 0);
} catch (...) {}
}
