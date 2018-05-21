#ifndef ScreenVDPhaseMapTableH
#define ScreenVDPhaseMapTableH
//---------------------------------------------------------------------------
#include "SCREEN.h"
#include "CSegmentInfo.h"
//---------------------------------------------------------------------------
class ScreenVDPhaseMapTable:public SCREEN
{
    public:
      ScreenVDPhaseMapTable(void);
      ~ScreenVDPhaseMapTable(void);

      void DisplayVDPhaseMapTable(void);
      void DoKeyWork(BYTE);

    private:

      DISP_WORD position[40];//48
      unsigned short int value[8][40];//48
      DISP_WORD makrphaseId[2];
      unsigned int cPosition;
      int phase_id;
      int usiLCN[8][8];
      int _tmpVD[8];

      int localIp1_1,localIp1_2,localIp1_3,localIp1_4,localPort1;
      int distIp0_1,distIp0_2,distIp0_3,distIp0_4,dist0Port;
      int distIp1,distIp2,distIp3,distIp4,distPort;

      BYTE networkSetBitmap[3840];

      void LoadBitmapFromFile(void);
      void InitDispWord(void);

      void DisplayVDTable(void);
      void DisplahPhaseId(void);

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
extern ScreenVDPhaseMapTable screenVDPhaseMapTable;
#endif

