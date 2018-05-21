//---------------------------------------------------------------------------
#include "SCREENNetworkSetConfirm.h"
#include "SCREENNetworkSet.h"
#include "SCREENHWReset.h"
#include "LCD240x128.h"
#include "SMEM.h"
#include "DIGITALIO.h"

#include "CSTC.h"
#include "WRITEJOB.h"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <error.h>
#include <strings.h>

SCREENNetworkSetConfirm screenNetworkSetConfirm;
//---------------------------------------------------------------------------
SCREENNetworkSetConfirm::SCREENNetworkSetConfirm(void)
{
    LoadBitmapFromFile();
}
//---------------------------------------------------------------------------
SCREENNetworkSetConfirm::~SCREENNetworkSetConfirm(void)
{
}
//---------------------------------------------------------------------------
void SCREENNetworkSetConfirm::DoKeyWork(BYTE key)
{
try {
    switch (key) {
        case 0x93:
          DoKeyF4Work();
        break;
        case 0x98:
          DoKeyEnterWork();
        break;
        default:
        break;
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENNetworkSetConfirm::LoadBitmapFromFile(void)
{
try {
    FILE *bitmap;
    bitmap=fopen("//cct//bitmap//backGround//1_0_F2_Enter_NetworkSetReady.bit","rb");
    if (bitmap) {
        fread(networkSetConfirmBitmap,3840,1,bitmap);
        fclose(bitmap);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENNetworkSetConfirm::DisplayNetworkSetConfirm(int a,int b,int c,int d,int e,
                                                       int f,int g,int h,int i,int j,
                                                       int k,int l,int m,int n,int o,
                                                       int nm1, int nm2, int nm3, int nm4,
                                                       int gw1, int gw2, int gw3, int gw4 )
{
try {

    smem.SetcFace(cNETWORKSETCONFIRM);
    smem.vWriteMsgToDOM("Enter Network Setup Confirm Screen");
    lcd240x128.DISPLAY_GRAPHIC(0,networkSetConfirmBitmap,128,30);
    localIp1_1=a;
    localIp1_2=b;
    localIp1_3=c;
    localIp1_4=d;
    localPort1=e;

    distIp0_1 = f;
    distIp0_2 = g;
    distIp0_3 = h;
    distIp0_4 = i;
    dist0Port = j;

    distIp1=k;
    distIp2=l;
    distIp3=m;
    distIp4=n;
    distPort=o;

    netmask1 = nm1;
    netmask2 = nm2;
    netmask3 = nm3;
    netmask4 = nm4;
    gateway1 = gw1;
    gateway2 = gw2;
    gateway3 = gw3;
    gateway4 = gw4;

  } catch(...) {}
}
//---------------------------------------------------------------------------
void SCREENNetworkSetConfirm::DoKeyF4Work(void)
{
    screenNetworkSet.DisplayNetworkSet();
}
//---------------------------------------------------------------------------
void SCREENNetworkSetConfirm::DoKeyEnterWork(void)
{
try {

    char cRunString[256];
//    bzero(cRunString, sizeof(cRunString));

    smem.SetLocalIP1(1,localIp1_1);
    smem.SetLocalIP1(2,localIp1_2);
    smem.SetLocalIP1(3,localIp1_3);
    smem.SetLocalIP1(4,localIp1_4);
    smem.SetLocalIP1(5,localPort1);
    smem.SetdistIp0(1,distIp0_1);
    smem.SetdistIp0(2,distIp0_2);
    smem.SetdistIp0(3,distIp0_3);
    smem.SetdistIp0(4,distIp0_4);
    smem.SetdistIp0(5,dist0Port);
    smem.SetDistIP(1,distIp1);
    smem.SetDistIP(2,distIp2);
    smem.SetDistIP(3,distIp3);
    smem.SetDistIP(4,distIp4);
    smem.SetDistIP(5,distPort);

    smem.SetNetmask(1,netmask1);
    smem.SetNetmask(2,netmask2);
    smem.SetNetmask(3,netmask3);
    smem.SetNetmask(4,netmask4);
    smem.SetGateway(1,gateway1);
    smem.SetGateway(2,gateway2);
    smem.SetGateway(3,gateway3);
    smem.SetGateway(4,gateway4);

//autorun    stc.Lock_to_Set_Control_Strategy(STRATEGY_FLASH);                           //OTFix 940804

    system("rm -rf /bin/quickNetwork2");
    system("rm -rf /cct/quickNetwork2");
    bzero(cRunString, sizeof(cRunString));
    sprintf(cRunString, "cp /bin/spaceSH /bin/quickNetwork2");
    sprintf(cRunString, "cp /bin/spaceSH /cct/quickNetwork2");
    system(cRunString);
    bzero(cRunString, sizeof(cRunString));
    sprintf(cRunString, "echo /sbin/ifconfig eth0:3 %d.%d.%d.%d netmask %d.%d.%d.%d >> /bin/quickNetwork2", localIp1_1, localIp1_2, localIp1_3, localIp1_4, netmask1, netmask2, netmask3, netmask4);
    system(cRunString);
    bzero(cRunString, sizeof(cRunString));
    sprintf(cRunString, "echo /sbin/route add default gw %d.%d.%d.%d >> /bin/quickNetwork2", gateway1, gateway2, gateway3, gateway4);
    system(cRunString);
    bzero(cRunString, sizeof(cRunString));
    sprintf(cRunString, "chmod +x /bin/quickNetwork2");
    system(cRunString);

    stc.CalculateAndSendRedCount(10);
    writeJob.WriteLetW77E58AutoControl();                                      //let W77e58 autoControl.
    system("sync");
    system("sync");
    system("reboot");

    screenHWReset.DisplayHWReset();

  } catch (...) {}
}
//---------------------------------------------------------------------------
