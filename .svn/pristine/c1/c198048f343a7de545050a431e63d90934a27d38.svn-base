//---------------------------------------------------------------------------
#include "SCREEN.h"
#include "LCD240x128.h"

#include <stdio.h>
#include <string.h>
//---------------------------------------------------------------------------
SCREEN::SCREEN(void)
{
try {

    ClearWord8X16();
    ClearWord16X16();
    ClearWord12X24();
    ClearWord24X24();

    GetDefaultWord8x16FromFile();
    GetDefaultWord16x16FromFile();
    GetDefaultWord12x24FromFile();
    GetDefaultWord24x24FromFile();
    GetDefaultArrow8x16FromFile();
    GetDefaultArrow16x16FromFile();
    GetDefaultSelectFormFile();
    GetDefaultUp16x16FromFile();

    word8x16Light[0] = 0x00;  //b00000000;
    word8x16Light[1] = 0x00;  //b00000000;
    word8x16Light[2] = 0x00;  //b00000000;
    word8x16Light[3] = 0x00;  //b00000000;
    word8x16Light[4] = 0x00;  //b00000000;
    word8x16Light[5] = 0x3C;  //b00111100;
    word8x16Light[6] = 0x7E;  //b01111110;
    word8x16Light[7] = 0xFF;  //b11111111;
    word8x16Light[8] = 0xFF;  //b11111111;
    word8x16Light[9] = 0x7E;  //b01111110;
    word8x16Light[10] = 0x3C; //b00111100;
    word8x16Light[11] = 0x00; //b00000000;
    word8x16Light[12] = 0x00; //b00000000;
    word8x16Light[13] = 0x00; //b00000000;
    word8x16Light[14] = 0x00; //b00000000;
    word8x16Light[15] = 0x00; //b00000000;

    for(int i = 0; i < 16; i++)
      word8x16Die[i] = 0;

    word8x16Flash[0] = 0x00;  //b00000000;
    word8x16Flash[1] = 0x00;  //b00000000;
    word8x16Flash[2] = 0x00;  //b00000000;
    word8x16Flash[3] = 0x00;  //b00000000;
    word8x16Flash[4] = 0x00;  //b00000000;
    word8x16Flash[5] = 0x3C;  //b00111100;
    word8x16Flash[6] = 0x42;  //b01000010;
    word8x16Flash[7] = 0x81;  //b10000001;
    word8x16Flash[8] = 0x81;  //b10000001;
    word8x16Flash[9] = 0x42;  //b01000010;
    word8x16Flash[10] = 0x3C;  //b00111100;
    word8x16Flash[11] = 0x00;  //b00000000;
    word8x16Flash[12] = 0x00;  //b00000000;
    word8x16Flash[13] = 0x00;  //b00000000;
    word8x16Flash[14] = 0x00;  //b00000000;
    word8x16Flash[15] = 0x00;  //b00000000;



  } catch (...) {}
}
//---------------------------------------------------------------------------
SCREEN::~SCREEN(void)
{
}
//---------------------------------------------------------------------------
bool SCREEN::DISPLAY8X16(BYTE ascii,unsigned int ram_address)
{
try {
    if (GetWord8X16FromFile(ascii)) {
        lcd240x128.DISPLAY_GRAPHIC(ram_address,word8x16Temp,16,1);
        return true;
    } return false;
  } catch(...) {}
}
//---------------------------------------------------------------------------
bool SCREEN::DISPLAY16X16(BYTE big5Hi,BYTE big5Lo,unsigned int ram_address)
{
try {
    if (GetWord16X16FromFile(big5Hi,big5Lo)) {
        lcd240x128.DISPLAY_GRAPHIC(ram_address,word16x16Temp,16,2);
        return true;
    } return false;
  } catch(...) {}
}
//---------------------------------------------------------------------------
bool SCREEN::DISPLAY12X24(BYTE ascii,unsigned int ram_address)
{
try {
    if (GetWord12X24FromFile(ascii)) {
        lcd240x128.DISPLAY_GRAPHIC(ram_address,word12x24Temp,24,2);
        return true;
    } return false;
  } catch(...) {}
}
//---------------------------------------------------------------------------
bool SCREEN::DISPLAY24X24(BYTE big5Hi,BYTE big5Lo,unsigned int ram_address)
{
try {
    if (GetWord24X24FromFile(big5Hi,big5Lo)) {
        lcd240x128.DISPLAY_GRAPHIC(ram_address,word24x24Temp,24,3);
        return true;
    } return false;
  } catch(...) {}
}
//---------------------------------------------------------------------------
bool SCREEN::GetWord8X16FromFile(BYTE ascii)     //ascii=1F~9F
{
try {
    if (ascii>=0x1F && ascii<=0x9F) {
        FILE *asciiStream;                                                      //�b���r�ɮ�8X16
        if ((asciiStream=fopen("//cct//bitmap//8X16kai.bit","rb"))!=NULL) {

            int seekPoint=16*(ascii-0x1F);                                      //1F�ONULL,20�Ospace
            fseek(asciiStream,seekPoint,SEEK_SET);
            fread(&word8x16Temp,16,1,asciiStream);

            fclose(asciiStream);
            return true;
        } else return false;
    } else return false;
  } catch(...) {}
}
//---------------------------------------------------------------------------
bool SCREEN::GetWord16X16FromFile(BYTE big5Hi,BYTE big5Lo)
{
try {
    if (big5Hi>=0xA1 && big5Hi<=0xF9) {     //BIG5�з�13973�r�H�� hi byte A1~F9
        if ((big5Lo>=0x40 && big5Lo<=0x7E) || (big5Lo>=0xA1 && big5Lo<=0xFE)) { //lo byte 40~7E,A1~FE

             FILE *big5;                                                        //�����r�ɮ�16X16
             int seekPoint=0;
             if ((big5=fopen("//cct//bitmap//16X16kai.bit","rb"))!=NULL) {

                 if (big5Lo>=0x40 && big5Lo<=0x7E) {
                     seekPoint=32*(((big5Hi-0xA1)*157)+(big5Lo-0x40));
                     fseek(big5,seekPoint,SEEK_SET);
                     fread(&word16x16Temp,32,1,big5);

                 } else if(big5Lo>=0xA1 && big5Lo<=0xFE) {
                           seekPoint=32*(((big5Hi-0xA1)*157)+(big5Lo-0xA1)+63);
                           fseek(big5,seekPoint,SEEK_SET);
                           fread(&word16x16Temp,32,1,big5);

                 }

                 fclose(big5);
                 return true;
             } else return false;
        } else return false;
    } else return false;
  } catch(...) {}
}
//---------------------------------------------------------------------------
bool SCREEN::GetWord12X24FromFile(BYTE ascii)
{
try {
    if (ascii>=0x1F && ascii<=0x9F) {
        FILE *asciiStream;                                                      //�b���r�ɮ�12X24
        if ((asciiStream=fopen("//cct//bitmap//12X24kai.bit","rb"))!=NULL) {

            int seekPoint=48*(ascii-0x1F);                                      //1F�ONULL,20�Ospace
            fseek(asciiStream,seekPoint,SEEK_SET);
            fread(&word12x24Temp,48,1,asciiStream);

            fclose(asciiStream);
            return true;
        } else return false;
    } else return false;
  } catch(...) {}
}
//---------------------------------------------------------------------------
bool SCREEN::GetWord24X24FromFile(BYTE big5Hi,BYTE big5Lo)
{
try {
    if (big5Hi>=0xA1 && big5Hi<=0xF9) {                                         //BIG5�з�13973�r�H�� hi byte A1~F9
        if ((big5Lo>=0x40 && big5Lo<=0x7E) || (big5Lo>=0xA1 && big5Lo<=0xFE)) { //lo byte 40~7E,A1~FE

             FILE *big5;                                                        //�����r�ɮ�24X24
             int seekPoint=0;
             if ((big5=fopen("//cct//bitmap//24X24kai.bit","rb"))!=NULL) {


                 if (big5Lo>=0x40 && big5Lo<=0x7E) {
                     seekPoint=72*(((big5Hi-0xA1)*157)+(big5Lo-0x40));
                     fseek(big5,seekPoint,SEEK_SET);
                     fread(&word24x24Temp,72,1,big5);

                 } else if(big5Lo>=0xA1 && big5Lo<=0xFE) {
                           seekPoint=72*(((big5Hi-0xA1)*157)+(big5Lo-0xA1)+63);
                           fseek(big5,seekPoint,SEEK_SET);
                           fread(&word24x24Temp,72,1,big5);

                 }

                 fclose(big5);
                 return true;
             } else return false;
        } else return false;
    } else return false;
  } catch(...) {}
}
//---------------------------------------------------------------------------
void SCREEN::GetDefaultWord8x16FromFile(void)
{    //�ɮת�bit�� ,�ɦW0~21���O�N��0123456789ABCDEFabcdef���I�}����
try {
    FILE *stream;
    char origin[100]="//cct//bitmap//8x16//";
    char name[100]="";
    char temp[4]="";
    char bitString[10]=".bit";

    for (int i=0;i<22;i++) {
         strcpy(name,origin);
         sprintf(temp,"%d",i);
         strcat(name,temp);
         strcat(name,bitString);

         stream=fopen(name,"rb");
         if (stream) {
             fread(&word8x16[i],16,1,stream);
             fclose(stream);
         }
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREEN::GetDefaultWord16x16FromFile(void)
{
try {
    FILE *stream;

    char origin[100]="//cct//bitmap//16x16//";
    char name[100]="";
    char temp[4]="";
    char bitString[10]=".bit";


    for (int i=0;i<22;i++) {
         strcpy(name,origin);
         sprintf(temp,"%d",i);
         strcat(name,temp);
         strcat(name,bitString);

         stream=fopen(name,"rb");
         if (stream) {
             fread(&word16x16[i],32,1,stream);
             fclose(stream);
         }
    }

  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREEN::GetDefaultWord12x24FromFile(void)
{
try {
    FILE *stream;

    char origin[100]="//cct//bitmap//12x24//";
    char name[100]="";
    char temp[4]="";
    char bitString[10]=".bit";


    for (int i=0;i<22;i++) {
         strcpy(name,origin);
         sprintf(temp,"%d",i);
         strcat(name,temp);
         strcat(name,bitString);

         stream=fopen(name,"rb");
         if (stream) {
             fread(&word12x24[i],48,1,stream);
             fclose(stream);
         }
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREEN::GetDefaultWord24x24FromFile(void)
{
try {
    FILE *stream;

    char origin[100]="//cct//bitmap//24x24//";
    char name[100]="";
    char temp[4]="";
    char bitString[10]=".bit";

    for (int i=0;i<22;i++) {
         strcpy(name,origin);
         sprintf(temp,"%d",i);
         strcat(name,temp);
         strcat(name,bitString);

         stream=fopen(name,"rb");
         if (stream) {
             fread(&word24x24[i],72,1,stream);
             fclose(stream);
         }
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREEN::GetDefaultArrow8x16FromFile(void)
{    //�ɮת�bit�� ,�ɦW1~4���O�N��W�U���k���I�}����
try {
    FILE *stream;

    char origin[100]="//cct//bitmap//arrow//8x16//";
    char name[100]="";
    char temp[4]="";
    char bitString[10]=".bit";

    for (int i=1;i<=4;i++) {
         strcpy(name,origin);
         sprintf(temp,"%d",i);
         strcat(name,temp);
         strcat(name,bitString);

         stream=fopen(name,"rb");
         if (stream) {
             fread(&arrow8x16[i-1],16,1,stream);
             fclose(stream);
         }
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREEN::GetDefaultArrow16x16FromFile(void)
{
try {
    FILE *stream;

    char origin[100]="//cct//bitmap//arrow//16x16//";
    char name[100]="";
    char temp[4]="";
    char bitString[10]=".bit";

    for (int i=1;i<=4;i++) {
         strcpy(name,origin);
         sprintf(temp,"%d",i);
         strcat(name,temp);
         strcat(name,bitString);

         stream=fopen(name,"rb");
         if (stream) {
             fread(&arrow16x16[i-1],32,1,stream);
             fclose(stream);
         }
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREEN::GetDefaultSelectFormFile(void)
{
try {
    FILE *stream;

    stream=fopen("//cct//bitmap//other//16x16UnSelect.bit","rb");
    if (stream) {
        fread(&select16x16[0],32,1,stream);
        fclose(stream);
    }

    stream=fopen("//cct//bitmap//other//16x16Select.bit","rb");
    if (stream) {
        fread(&select16x16[1],32,1,stream);
        fclose(stream);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREEN::GetDefaultUp16x16FromFile(void)
{
	FILE *stream;
	stream=fopen("//cct//bitmap//other//16x16Up.bit","rb");
	if (stream) {
		fread(&up16x16[0],32,1,stream);
	    fclose(stream);
	}
}
//---------------------------------------------------------------------------
void SCREEN::setCursor24x24(int x,int y)
{
try {
    BYTE cursor[3]={0xFF,0xFF,0xFF};
    lcd240x128.DISPLAY_GRAPHIC_XY(x,y,cursor,1,3);
    lcd240x128.DISPLAY_GRAPHIC_XY(x,y,cursor,1,3);
  } catch (...){}
}
//---------------------------------------------------------------------------
void SCREEN::clearCursor24x24(int x,int y)
{
try {
    BYTE cursor[3]={0};
    lcd240x128.DISPLAY_GRAPHIC_XY(x,y,cursor,1,3);
    lcd240x128.DISPLAY_GRAPHIC_XY(x,y,cursor,1,3);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREEN::setCursor16x16(int x,int y)
{
try {
    BYTE cursor[2]={0xFF,0xFF};
    lcd240x128.DISPLAY_GRAPHIC_XY(x,y,cursor,1,2);
    lcd240x128.DISPLAY_GRAPHIC_XY(x,y,cursor,1,2);
  } catch (...){}
}
//---------------------------------------------------------------------------
void SCREEN::clearCursor16x16(int x,int y)
{
try {
    BYTE cursor[2]={0};
    lcd240x128.DISPLAY_GRAPHIC_XY(x,y,cursor,1,2);
    lcd240x128.DISPLAY_GRAPHIC_XY(x,y,cursor,1,2);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREEN::setCursor8x16(int x,int y)
{
try {
    BYTE cursor[1]={0xFF};
    lcd240x128.DISPLAY_GRAPHIC_XY(x,y,cursor,1,1);
    lcd240x128.DISPLAY_GRAPHIC_XY(x,y,cursor,1,1);
  } catch (...){}
}
//---------------------------------------------------------------------------
void SCREEN::clearCursor8x16(int x,int y)
{
try {
    BYTE cursor[1]={0};
    lcd240x128.DISPLAY_GRAPHIC_XY(x,y,cursor,1,1);
    lcd240x128.DISPLAY_GRAPHIC_XY(x,y,cursor,1,1);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREEN::setSelectPoint16x16(int x,int y,int heightBit,int widthByte)
{
try {
    lcd240x128.DISPLAY_GRAPHIC_XY(x,y,select16x16[1],heightBit,widthByte);
    lcd240x128.DISPLAY_GRAPHIC_XY(x,y,select16x16[1],heightBit,widthByte);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREEN::clearSelectPoint16x16(int x,int y,int heightBit,int widthByte)
{
try {
    lcd240x128.DISPLAY_GRAPHIC_XY(x,y,select16x16[0],heightBit,widthByte);
    lcd240x128.DISPLAY_GRAPHIC_XY(x,y,select16x16[0],heightBit,widthByte);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREEN::clearSelectPoint24x24(int x,int y,int heightBit,int widthByte)
{
try {
    BYTE unSelect24x24[72]={0};

    lcd240x128.DISPLAY_GRAPHIC_XY(x,y,unSelect24x24,heightBit,widthByte);
    lcd240x128.DISPLAY_GRAPHIC_XY(x,y,unSelect24x24,heightBit,widthByte);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREEN::setUpPoint16x16(int x,int y,int heightBit,int widthByte)
{
try {
    lcd240x128.DISPLAY_GRAPHIC_XY(x,y,up16x16[0],heightBit,widthByte);
    lcd240x128.DISPLAY_GRAPHIC_XY(x,y,up16x16[0],heightBit,widthByte);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREEN::clearUpPoint16x16(int x,int y,int heightBit,int widthByte)
{
try {
	lcd240x128.DISPLAY_GRAPHIC_XY(x,y,select16x16[0],heightBit,widthByte);
	lcd240x128.DISPLAY_GRAPHIC_XY(x,y,select16x16[0],heightBit,widthByte);
	}catch(...){}
}
//---------------------------------------------------------------------------
