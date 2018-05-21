#ifndef SCREENNetworkSetH
#define SCREENNetworkSetH

#include "SCREEN.h"
//---------------------------------------------------------------------------
class SCREENNetworkSet:public SCREEN
{
    public:

      SCREENNetworkSet(void);
      ~SCREENNetworkSet(void);

      void DisplayNetworkSet(void);
      void DisplayMainNetworkSet(void);
      void DoKeyWork(BYTE);                                                     //\uFFFD��\uFFFDKEY\uFFFD\uFFFD\uFFFD�\uFFFD\uFFFD


    private:

      DISP_WORD position[48];                                                   //此�\uFFFD\uFFFD\uFFFD��\uFFFD��\uFFFD,�\uFFFD��座�\uFFFD
      unsigned short int value[48];
      unsigned int cPosition;                                                   //\uFFFD��\uFFFD\uFFFD\uFFFD\uFFFD�\uFFFD

      int localIp1_1,localIp1_2,localIp1_3,localIp1_4,localPort1;
      int distIp0_1,distIp0_2,distIp0_3,distIp0_4,dist0Port;
      int distIp1,distIp2,distIp3,distIp4,distPort;

      BYTE networkSetBitmap[3840];

      void LoadBitmapFromFile(void);                                            //�\uFFFD\uFFFD\uFFFD\uFFFDoad\uFFFD��\uFFFD\uFFFD��\uFFFD
      void InitDispWord(void);                                                  //\uFFFD\uFFFD\uFFFD\uFFFD\uFFFD��\uFFFD��\uFFFD\uFFFD\uFFFD���\uFFFD\uFFFD\uFFFD\uFFFD\uFFFD

      void DisplayLocalIp1(void);
      void DisplayDistIp0(void);
      void DisplayDistIp(void);

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
extern SCREENNetworkSet screenNetworkSet;
#endif
