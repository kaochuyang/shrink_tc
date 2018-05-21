//---------------------------------------------------------------------------
#ifndef SCREENH
#define SCREENH

#include "var.h"
//---------------------------------------------------------------------------
    struct DISP_WORD{      //��ܦb�e���W���r
           int X;          //X�b�y��
           int Y;          //Y�b�y��
           int width;      //��ܼe��
           int height;     //��ܰ���
    };
//---------------------------------------------------------------------------
class SCREEN
{
    public:
        SCREEN(void);
        ~SCREEN(void);

        bool DISPLAY8X16(BYTE,unsigned int);                                    //���8X16���r
        bool DISPLAY16X16(BYTE,BYTE,unsigned int);                              //���16X16���r
        bool DISPLAY12X24(BYTE,unsigned int);                                   //���12X24���r
        bool DISPLAY24X24(BYTE,BYTE,unsigned int);                              //���24X24���r

        bool GetWord8X16FromFile(BYTE);                                         //�o��8X16��bitmap
        bool GetWord16X16FromFile(BYTE,BYTE);                                   //�o��16X16��bitmap
        bool GetWord12X24FromFile(BYTE);                                        //�o��12X24��bitmap
        bool GetWord24X24FromFile(BYTE,BYTE);                                   //�o��24X24��bitmap

        void GetDefaultWord8x16FromFile(void);                                  //�q�ɮפ��o��DefaultBitmap 8x16  : 0123456789ABCDEF
        void GetDefaultWord16x16FromFile(void);                                 //�q�ɮפ��o��DefaultBitmap 16x16 : 0123456789ABCDEF
        void GetDefaultWord12x24FromFile(void);                                 //�q�ɮפ��o��DefaultBitmap 12x24 : 0123456789ABCDEF
        void GetDefaultWord24x24FromFile(void);                                 //�q�ɮפ��o��DefaultBitmap 24x24 : 0123456789ABCDEF
        void GetDefaultArrow8x16FromFile(void);                                 //�q�ɮפ��o��DefaultBitmap 8x16  : �W�U���k
        void GetDefaultArrow16x16FromFile(void);                                //�q�ɮפ��o��DefaultBitmap 16x16 : �W�U���k
        void GetDefaultSelectFormFile(void);                                    //�q�ɮפ��o��DefaultBitmap 16x16 : �����(�ť�),���δ�Щ��u
        void GetDefaultUp16x16FromFile(void);

        BYTE word8x16Temp[16];                                                  //���8x16��r���Ȧs��
        BYTE word16x16Temp[32];                                                 //���16x16��r���Ȧs��
        BYTE word12x24Temp[48];                                                 //���12x16��r���Ȧs��
        BYTE word24x24Temp[72];                                                 //���24x24��r���Ȧs��

        BYTE word8x16Light[16];
        BYTE word8x16Die[16];
        BYTE word8x16Flash[16];

        void ClearWord8X16(void) {for (int i=0;i<16;i++) word8x16Temp[i]=0;}    //�M��8x16�Ȧs��
        void ClearWord16X16(void) {for (int i=0;i<32;i++) word16x16Temp[i]=0;}  //�M��16x16�Ȧs��
        void ClearWord12X24(void) {for (int i=0;i<48;i++) word12x24Temp[i]=0;}  //�M��12x16�Ȧs��
        void ClearWord24X24(void) {for (int i=0;i<72;i++) word24x24Temp[i]=0;}  //�M��24x24�Ȧs��

        BYTE word8x16[22][16];                                                  //��Bitmap 8x16 : 0123456789ABCDEFabcdef
        BYTE word16x16[22][32];                                                 //��Bitmap 16X16: 0123456789ABCDEFabcdef
        BYTE word12x24[22][48];                                                 //��Bitmap 12X24: 0123456789ABCDEFabcdef
        BYTE word24x24[22][72];                                                 //��Bitmap 24X24: 0123456789ABCDEFabcdef
        BYTE arrow8x16[4][16];                                                  //��Bitmap 8X16 : �W�U���k
        BYTE arrow16x16[4][32];                                                 //��Bitmap 16X16: �W�U���k
        BYTE select16x16[2][32];                                                //��Bitmap 16X16: �����(�ť�),���

        BYTE up16x16[1][32];

        void setCursor24x24(int,int);                                           //�I���
        void clearCursor24x24(int,int);                                         //�M�����
        void setCursor16x16(int,int);                                           //�I���
        void clearCursor16x16(int,int);                                         //�M�����
        void setCursor8x16(int,int);                                            //�I���
        void clearCursor8x16(int,int);                                          //�M�����
        void setSelectPoint16x16(int,int,int,int);
        void clearSelectPoint16x16(int,int,int,int);
        void clearSelectPoint24x24(int,int,int,int);

        void setUpPoint16x16(int,int,int,int);
        void clearUpPoint16x16(int,int,int,int);

    private:
};
//---------------------------------------------------------------------------
#endif
