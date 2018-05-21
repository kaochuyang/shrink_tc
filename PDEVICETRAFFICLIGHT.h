//---------------------------------------------------------------------------

#include "var.h"
//---------------------------------------------------------------------------
class PDEVICETRAFFICLIGHT
{
    public:

        PDEVICETRAFFICLIGHT(void);
        ~PDEVICETRAFFICLIGHT(void);
        bool DoWorkViaPDevice(MESSAGEOK);                                       //®Ú¾ÚPDevice¨Óªº«Ê¥]§@¨Æ±¡

    private:
        int iAskW77E58CounterReturn;

    //jacky20140116 BRT GreenConflict
    private:
        DATA_Bit GC_DATA0;
        DATA_Bit GC_DATA1;
        DATA_Bit GC_DATA2;
       // BYTE    GC0;
       // BYTE    GC1;
       // BYTE    GC2;
        void BRTGreenConflict(BYTE, BYTE, BYTE, BYTE);
    //jacky20140116 BRT 觸動連鎖
    private:
        DATA_Bit TC_DATA0;
        DATA_Bit TC_DATA1;
        DATA_Bit TC_DATA2;
        BYTE    TC0;
        BYTE    TC1;
        BYTE    TC2;
    //jacky20140116 BRT ADC
    private:
        DATA_Bit ADC_DATA0;
        DATA_Bit ADC_DATA1;
        DATA_Bit ADC_DATA2;
        BYTE    ADC0;
        BYTE    ADC1;
        BYTE    ADC2;
    //jacky20140120 BRT 燈卡回報
    private:
        BYTE statusCOUNT;
        BYTE GC_status0;
        BYTE GC_status1;
        BYTE GC_status2;
        BYTE TC_status0;
        BYTE TC_status1;
        BYTE TC_status2;
        BYTE ADC_status0;
        BYTE ADC_status1;
        BYTE ADC_status2;
        bool GG;

};
//---------------------------------------------------------------------------
