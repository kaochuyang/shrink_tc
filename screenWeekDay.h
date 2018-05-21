#ifndef screenWeekDayH
#define screenWeekDayH
//---------------------------------------------------------------------------
#include "SCREEN.h"
#include "CSegmentInfo.h"
//---------------------------------------------------------------------------
class ScreenWeekDay:public SCREEN
{
    public:
      ScreenWeekDay(void);
      ~ScreenWeekDay(void);

      void DisplayWeekDay(void);
      void doKeyWork(BYTE);                                                     //�ھ�KEY���ȧ@��

    private:
      DISP_WORD dateWord[8];                                                    //��ܤ�j���m
      DISP_WORD segtypeWord[28];                                                //��ܬP�d@~�C���ɶ���s��(1~20)����m

      //CCJ++
      BYTE WeekTemp[28];
      //CCJ--

      void initDispWord(void);                                                  //��l�Ʀ��e�����y�е��Ѽ�
      void DisplayDate(void);                                                   //��ܤ��

      CWeekDaySegType weekDaySegtype[14];
      void LoadWeekDaySegtype(void);                                            //�q�ǫi���̱o��
      void DisplayWeekDaySegtype(void);                                         //��ܦb�e���W

      int cDatePosition;
      int cSegPosition;

      BYTE weekDayBitmap[3840];                                                 //����
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
      void doKeyDefaultWork(void);
};
//---------------------------------------------------------------------------
extern ScreenWeekDay screenWeekDay;
#endif

