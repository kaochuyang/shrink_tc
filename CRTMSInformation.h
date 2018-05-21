#ifndef CRTMSInformationH
#define CRTMSInformationH

#include "CTarget.h"

#define HEADER 		0XFF

#define VOLLONG 	0X1B
#define VOLUME 		0X10
#define OCCUPAT 	0X11
#define SSPEED 		0X12

#define VOLMID          0X20
#define VOLXLONG        0X36
#define RTCLOCK         0X16
#define TICKCOUNT       0X35  //CRC
#define MSGINFO         0X39

#define TARGET 		0X18
#define RTMS_STAT       0X19
#define RT_SPEED        0X2D
#define BIT 		0X08
#define PBIT 		0X1D
#define POWERMGNT       0XAA

#define DATAREQ 	0X8F
#define FLUSH_BUFREQ    0X1F
#define BIT_REQ 	0X88
#define PBIT_REQ 	0X90
#define POWERMGNT_REQ   0X96
#define SET_BAUD_RATE   0XAC

#define EUSC_SET        0XBA
#define EUSC_REQ        0XB0
#define EUSC_RESET      0XBB
#define EUSC_RETURN1    0XB1
#define EUSC_RETURN2    0XB2
#define EUSC_RETURN3    0XB3
#define EUSC_RETURN4    0XB4
#define EUSC_RETURN5    0XB5
#define EUSC_RETURN6    0XB6
#define EUSC_RETURN7    0XB7
#define EUSC_RETURN8    0XB8
#define EUSC_RETURNC    0xB9
#define EUSC_ACK 	0X1A
#define EUSC_NAK 	0X1C

//---------------------------------------------------------------------------
struct CRTMSInformation
{
  public:
    bool dirin;
    bool health;

    unsigned char info;
    unsigned short int id;
    unsigned short int vollong[8];
    unsigned short int vol[8];
    unsigned short int oc[8];
    unsigned short int sp[8];
    unsigned short int avgspeed;
    unsigned short int voltage;
    unsigned short int uptime;
    CTarget target;

    CRTMSInformation(void):dirin(false),health(false),info(0x00),id(0)
                   ,avgspeed(0),voltage(0),uptime(0)
                   {
                     for(int i=0;i<8;i++){
                       vollong[i]=0;
                       vol[i]=0;
                       oc[i]=0;
                       sp[i]=0;
                     }
                   }

    CRTMSInformation &operator=(const CRTMSInformation &rtms_info);
    void display_status(void);

};
//---------------------------------------------------------------------------
#endif

