#ifndef SCREENPEDSWITCHSTATUS_H_INCLUDED
#define SCREENPEDSWITCHSTATUS_H_INCLUDED

#include "SCREEN.h"

class SCREENPedSwitchStatus: public SCREEN {
    public:
        SCREENPedSwitchStatus();
        virtual ~SCREENPedSwitchStatus();
        void DisplayPedSwitchStatus(void);
	    void doKeyWork(BYTE);                                                     //�ھ�KEY���ȧ@��

	    void vRefreshSwitchStatus();

	    private:
	      DISP_WORD markDevCount[2];
	      DISP_WORD markDevID[20][2];
	      DISP_WORD markStatus[20];
	      DISP_WORD markSWVar[20];

	      BYTE padSwitchBitmap[3840];
	      void initDispWord(void);                                                  //��l�Ʀ��e�����y�е��Ѽ�
	      int cSelect;                                                              //�{�b��ܪ��ﶵ
          unsigned char cPosition;
          unsigned char siDevCount;

	      void loadBitmapFromFile(void);                                            //�N����Load�i�O����

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

	      void DisplayDevID();


};
extern SCREENPedSwitchStatus screenPedSwitchStatus;

#endif // SCREENPEDSWITCHSTATUS_H_INCLUDED
