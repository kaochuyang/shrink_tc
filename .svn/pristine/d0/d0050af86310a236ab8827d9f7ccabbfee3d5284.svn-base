//---------------------------------------------------------------------------

#ifndef PDEVICEH
#define PDEVICEH

#include "var.h"
#include "PDEVICESS.h"
#include "PDEVICERTMS.h"
#include "PDEVICEKEYPAD.h"

/*OTCombo0713*/
#include "PDEVICETRAFFICLIGHT.h"
#include "PDEVICEREVMANUALPAD.h"
#include "PDEVICEPEDPUSHBUTTON.h"

//---------------------------------------------------------------------------
class PDEVICE
{
    public:

        PDEVICE(void);
        ~PDEVICE(void);
        bool DoWorkViaProtocol(MESSAGEOK);                                      //根據protocol傳來的Structure作事情


    private:

        PDEVICESS pDeviceSS;
        PDEVICERTMS pDeviceRTMS;
        PDEVICEKEYPAD pDeviceKeypad;
/*OTCombo0713*/
        PDEVICETRAFFICLIGHT pDeviceTrafficLight;
        PDEVICEREVMANUALPAD pDeviceRevManualPad;
        PDEVICEPEDPUSHBUTTON pDevicePedPushButton;

};
//---------------------------------------------------------------------------
#endif


