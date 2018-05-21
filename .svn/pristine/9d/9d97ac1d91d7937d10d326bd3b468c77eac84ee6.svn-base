#ifndef SCREENNetworkSetConfirmH
#define SCREENNetworkSetConfirmH

#include "SCREEN.h"
#include "var.h"
//---------------------------------------------------------------------------
class SCREENNetworkSetConfirm:public SCREEN
{
    public:
        SCREENNetworkSetConfirm(void);
        ~SCREENNetworkSetConfirm(void);

        void DisplayNetworkSetConfirm(int,int,int,int,int,
                                      int,int,int,int,int,
                                      int,int,int,int,int,
                                      int,int,int,int,
                                      int,int,int,int  );
        void DoKeyWork(BYTE);

    private:
        BYTE networkSetConfirmBitmap[3840];

        int localIp1_1,localIp1_2,localIp1_3,localIp1_4,localPort1;
        int distIp0_1,distIp0_2,distIp0_3,distIp0_4,dist0Port;
        int distIp1,distIp2,distIp3,distIp4,distPort;
        int netmask1,netmask2,netmask3,netmask4;
        int gateway1,gateway2,gateway3,gateway4;

        void LoadBitmapFromFile(void);
        void DoKeyF4Work();
        void DoKeyEnterWork();
};
//---------------------------------------------------------------------------
extern SCREENNetworkSetConfirm screenNetworkSetConfirm;
#endif

