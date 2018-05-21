#ifndef DIGITALIOH
#define DIGITALIOH

#include "var.h"
#include "pthread.h"

//--------------------------------------------------------------------------
class DIGITALIO
{
    public:
        DIGITALIO(void);
        ~DIGITALIO(void);

        bool GetAuthority(unsigned long, unsigned long);                        //å¾\uFFFD\uFFFD\uFFFD§å\uFFFDæ¬\uFFFD
        bool ReleaseAuthority(void);                                            //\uFFFD\uFFFD\uFFFD\uFFFD§å\uFFFDæ¬\uFFFD
        DATA_Bit vReadDigitalIO(void);                                          //å¾\uFFFDigital IOè®\uFFFD\uFFFD\uFFFD\uFFFD
        bool WriteDigitalIO(unsigned char);

        bool vRefresfWatchDog0x444(void);


    private:
        pthread_mutex_t mutexDIO;                                               //ä¿\uFFFD­·è¨\uFFFD\uFFFDé«\uFFFD
        unsigned long DATAIN_PORT;                                              //IOä½\uFFFD½®
        unsigned long DATAOUT_PORT;                                             //IOä½\uFFFD½®
        bool haveGetINPower;                                                    //\uFFFD¯å\uFFFD\uFFFD²å\uFFFD\uFFFD§å\uFFFDLPT\uFFFD\uFFFD\uFFFD\uFFFD\uFFFD
        bool haveGetOUTPower;                                                   //\uFFFD¯å\uFFFD\uFFFD²å\uFFFD\uFFFD§å\uFFFDLPT\uFFFD\uFFFD\uFFFD\uFFFD\uFFFD
        unsigned char defaultSignal;                                            //\uFFFD\uFFFDPTä¸\uFFFD\uFFFDè¨\uFFFD\uFFFD
        unsigned char temp;
        DATA_Bit IOByte;

        int iIOPLResult;
};
//--------------------------------------------------------------------------

//========================================================================
//========================================================================
//DIGITALIO_ICOP6047 Winbond Super I/O
//---------------------------------------------------------------------------
//--------------------------------------------------------------------------
class DIGITALIO_ICOP6047
{
    public:
        DIGITALIO_ICOP6047(void);
        ~DIGITALIO_ICOP6047(void);

        bool GetAuthority(unsigned long, unsigned long);
        bool ReleaseAuthority(void);
        DATA_Bit vReadDigitalIO(void);
        bool WriteDigitalIO(unsigned char);

        bool vRefresfWatchDog0x444(void);


    private:
        pthread_mutex_t mutexDIO;
        unsigned long DATAIN_PORT;
        unsigned long DATAOUT_PORT;
        bool haveGetINPower;
        bool haveGetOUTPower;
        unsigned char defaultSignal;
        unsigned char temp;
        DATA_Bit IOByte;

        int iIOPLResult;
};
//--------------------------------------------------------------------------

//========================================================================
//========================================================================
//DIGITALIO_ICOP_Vortex86
//---------------------------------------------------------------------------
//--------------------------------------------------------------------------
class DIGITALIO_Vortex86SX
{
    public:
        DIGITALIO_Vortex86SX(void);
        ~DIGITALIO_Vortex86SX(void);

        bool GetAuthority(unsigned long, unsigned long);
        bool ReleaseAuthority(void);
        DATA_Bit vReadDigitalIO(void);
        bool WriteDigitalIO(unsigned char);

        bool vRefresfWatchDog0x444(void);


    private:
        pthread_mutex_t mutexDIO;
        unsigned long DATAIN_PORT;
        unsigned long DATAOUT_PORT;
        bool haveGetINPower;
        bool haveGetOUTPower;
        unsigned char defaultSignal;
        unsigned char temp;
        DATA_Bit IOByte;

        int iIOPLResult;
};


//here select ICOP & ARBOR
extern DIGITALIO digitalIO;
// extern DIGITALIO_ICOP6047 digitalIO;
// extern DIGITALIO_Vortex86SX digitalIO;
#endif
