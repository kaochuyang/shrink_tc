#ifndef SCREENACTUATEARWENVDINFO_H_INCLUDED
#define SCREENACTUATEARWENVDINFO_H_INCLUDED

#include "SCREEN.h"
//---------------------------------------------------------------------------
class ScreenActuateArwenVDINFO:public SCREEN
{
    public:
     ScreenActuateArwenVDINFO(void);
      ~ScreenActuateArwenVDINFO(void);

      void DisplayArwenDetectorInfo(void);
      void doKeyWork(BYTE);

      void SetDetectorZoneStatus(int,int,int);
      void SetDetectorZoneStopped(int,int,int);

      void  detectorWatchDog();

      void  detectorClear(int);
      void vRefreshshowDetectorData();
      void vRefreshshowPEDData();
      void  SetactTOTCDetectorCount(unsigned short int);
      void  SetDetectorLCN(unsigned short int,unsigned short int);

    private:
      DISP_WORD _vdZoneNo[5][4];
      DISP_WORD _vdZoneStatus[5][4];
      DISP_WORD _vdZoneWatchDog[5];
      DISP_WORD _vdZoneStopped[5][4][12];

      void initDispWord(void);                                                  //ªì©l¤Æ¦¹µe­±ªº®y¼Ðµ¥°Ñ¼Æ
      int cSelect;                                                              //²{¦b¿ï¾Üªº¿ï¶µ

      BYTE ucChainStatusBitmap[3840];
      void loadBitmapFromFile(void);                                            //±N©³¹ÏLoad¶i°O¾ÐÅé

      void doKey0Work(void);
      void doKey1Work(void);
      void doKey2Work(void);
      void doKey3Work(void);
      void doKey4Work(void);
      void doKey5Work(void);
      void doKey6Work(void);
      void doKey7Work(void);
      void doKey8Work(void);
      void doKey9Work(void);
      void doKeyAWork(void);
      void doKeyBWork(void);
      void doKeyCWork(void);
      void doKeyDWork(void);
      void doKeyEWork(void);
      void doKeyFWork(void);
      void doKeyF1Work(void);
      void doKeyF2Work(void);
      void doKeyF3Work(void);
      void doKeyF4Work(void);
      void doKeyUPWork(void);
      void doKeyDOWNWork(void);
      void doKeyLEFTWork(void);
      void doKeyRIGHTWork(void);
      void doKeyEnterWork(void);
      int m_VDseconds[10];
      int m_VDlcn[10];
      int m_VDZoneStatus[10][4];
      int m_VDZoneStopped[10][4];
      int m_vdNowCount;
};
//---------------------------------------------------------------------------
extern ScreenActuateArwenVDINFO screenActuateArwenVDINFO;
#endif // SCREENACTUATEARWENVDINFO_H_INCLUDED
