#ifndef screenRevSpecialDayH
#define screenRevSpecialDayH
//---------------------------------------------------------------------------
#include "SCREEN.h"
#include "CReverseTime.h"
//---------------------------------------------------------------------------
class ScreenRevSpecialDay:public SCREEN
{
    public:
      ScreenRevSpecialDay(void);
      ~ScreenRevSpecialDay(void);

      void DisplaySpecialDay(void);
      void doKeyWork(BYTE);                                                     //�ھ�KEY���ȧ@��

    private:
      DISP_WORD dateWord[8];                                                    //��ܤ�j���m
      DISP_WORD itemWord[6];                                                    //��ܽs������m
      DISP_WORD specialDayWord[30];                                             //��ܸ�ƪ���m

      int cPosition;                                                            //
      int cSelect;                                                              //0:���\uFFFD1:�S�O�餺�e
      int cFace;

      void initDispWord(void);                                                  //��l�Ʀ��e�����y�е��Ѽ�
      void DisplayDate(void);                                                   //��ܤ��

      CHoliDayRevType specialDayRev[13];
      //CCJ++
      CHoliDayRevType Tmp_specialDayRev[13];
      void ModifyRev(int tmp_value);
      //CCJ++
      void LoadSpecialDayRev(void);                                         //�q�ǫi���̱o��
      void DisplaySpecialDayRev(int);                                       //��ܦb�e���W
      void SaveSpecialDayRev(void);                                         //�g�^�ǫi����
      void DisplayItem(int);                                                    //��ܽs��
      void DisplayUpAndPage(int);                                               //��ܤW���ΤU��

      BYTE specialDayBitmap[3840];                                              //����
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
extern ScreenRevSpecialDay screenRevSpecialDay;
#endif
