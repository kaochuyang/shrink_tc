#ifndef SCREENModifyDTH
#define SCREENModifyDTH

#include "SCREEN.h"
//---------------------------------------------------------------------------
class SCREENModifyDT:public SCREEN
{
    public:
      SCREENModifyDT(void);
      ~SCREENModifyDT(void);

      void DisplayModifyDT(void);
      void DoKeyWork(BYTE);                                                     //�ھ�KEY���ȧ@��
      void DisplayCurrentDateTime(void);                                        //��ܲ{�b�ɶ�
      void SendTimeisChangetoArwenActuate(void);

    private:
      DISP_WORD cDateWord[8];
      DISP_WORD cTimeWord[6];

      DISP_WORD dateWord[8];                                                    //��ܤ������m
      BYTE dateTemp[8];
      DISP_WORD timeWord[6];                                                    //��ܮɶ�����m
      BYTE timeTemp[6];
      int cPosition;                                                            //���k���ʪ���m
      int cSelect;                                                              //�W�U����   0:��� 1:�ɶ�

      void InitDispWord(void);                                                  //��l�Ʀ��e�����y�е��Ѽ�
      void DisplayDate(void);                                                   //��ܤ��
      void DisplayTime(void);                                                   //��ܮɶ�


      BYTE modifyDTBitmap[3840];
      void LoadBitmapFromFile(void);                                            //�N����Load�i�O����

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
      void DoKeyF1Work(void);
      void DoKeyF4Work(void);
      void DoKeyLEFTWork(void);
      void DoKeyRIGHTWork(void);
      void DoKeyEnterWork(void);


};
//---------------------------------------------------------------------------
extern SCREENModifyDT screenModifyDT;
#endif
