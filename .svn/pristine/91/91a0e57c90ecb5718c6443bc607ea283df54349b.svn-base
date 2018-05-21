#ifndef SCREENNetmaskSetH
#define SCREENNetmaskSetH

#include "SCREEN.h"
//---------------------------------------------------------------------------
class SCREENNetmaskSet:public SCREEN
{
    public:

      SCREENNetmaskSet(void);
      ~SCREENNetmaskSet(void);

      void DisplayNetmaskSet(int,int,int,int,int,
                             int,int,int,int,int,
                             int,int,int,int,int );

      void DoKeyWork(BYTE);


    private:

      DISP_WORD position[48];
      unsigned short int value[48];
      unsigned int cPosition;

      int localIp1_1,localIp1_2,localIp1_3,localIp1_4,localPort1;
      int distIp0_1,distIp0_2,distIp0_3,distIp0_4,dist0Port;
      int distIp1,distIp2,distIp3,distIp4,distPort;
      int netmask1, netmask2, netmask3, netmask4;
      int gateway1, gateway2, gateway3, gateway4;

      BYTE netmaskSetBitmap[3840];

      void LoadBitmapFromFile(void);
      void InitDispWord(void);

      void DisplayNetmask(void);
      void DisplayGateway(void);

      void DoKey0Work(void);
      void DoKey1Work(void);
      void DoKey2Work(void);
      void DoKey3Work(void);
      void DoKey4Work(void);
      void DoKey5Work(void);
      void DoKey6Work(void);
      void DoKey7Work(void);
      void DoKey8Work(void);
      void DoKey9Work(void);
      void DoKeyAWork(void);
      void DoKeyBWork(void);
      void DoKeyCWork(void);
      void DoKeyDWork(void);
      void DoKeyEWork(void);
      void DoKeyFWork(void);
      void DoKeyF1Work(void);
      void DoKeyF2Work(void);
      void DoKeyF3Work(void);
      void DoKeyF4Work(void);
      void DoKeyUPWork(void);
      void DoKeyDOWNWork(void);
      void DoKeyLEFTWork(void);
      void DoKeyRIGHTWork(void);
      void DoKeyEnterWork(void);
};
//---------------------------------------------------------------------------
extern SCREENNetmaskSet screenNetmaskSet;
#endif
