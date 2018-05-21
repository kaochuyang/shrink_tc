//---------------------------------------------------------------------------
#include "LCD240x128.h"

#include <stdio.h>
#include <sys/perm.h>
#include <sys/io.h>

#define E_1   outb(temp &=0x0d,CONTROL_PORT);
#define E_0   outb(temp |=0x02,CONTROL_PORT);
#define RS_1  outb(temp &=0x0e,CONTROL_PORT);
#define RS_0  outb(temp |=0x01,CONTROL_PORT);

LCD240x128 lcd240x128;
//---------------------------------------------------------------------------
LCD240x128::LCD240x128(void)
{
try {

    pthread_mutex_init(&mutexLcd,NULL);

    DATA_PORT=0x378;                      //Default IO位置:DATA PORT
    CONTROL_PORT=0x37A;                   //Default IO位置:CONTROL PORT
// #ICOPBUG
//    DATA_PORT=0x278;                      //Default IO位置:DATA PORT
//    CONTROL_PORT=0x27A;                   //Default IO位置:CONTROL PORT

    lcdWidth=240;                         //Lcd寬度pixel
    lcdHeight=128;                        //Lcd長度pixel
    haveGetPower=false;                   //尚未得到控制LPT的權限
    defaultSignal=0;
    temp=0;

  } catch(...) {}
}
//---------------------------------------------------------------------------
LCD240x128::~LCD240x128 (void)
{
}
//---------------------------------------------------------------------------
bool LCD240x128::GetAuthority(unsigned long ioAddress)
{
try {
    if (ioperm(ioAddress,3,1)==0) {       //得到控制權
        DATA_PORT=ioAddress;              //BASEPORT
        CONTROL_PORT=ioAddress+2;         //CONTROL_PORT=BASEPORT+2
        haveGetPower=true;                //turn on the flag
        defaultSignal=inb(CONTROL_PORT);  //得到CONTROL_PORT Default Status,結束時還原
        outb(3,CONTROL_PORT);             //Set Bit0=1,Bit1=1
        return true;
    } else return false;
  } catch(...) {}
}
//---------------------------------------------------------------------------
bool LCD240x128::ReleaseAuthority(void)
{
try {
    if (haveGetPower) outb(defaultSignal,CONTROL_PORT);                         //還原CONTROL_PORT Default Status
    if (ioperm(DATA_PORT,3,0)==0) {       //釋放控制權
        haveGetPower=false;               //turn off the flag
        return true;
    } else return false;
  } catch(...) {}
}
//---------------------------------------------------------------------------
void LCD240x128::WRITE_PORT(unsigned char C,unsigned char D)
{
try {
    RS_1;                                 //outb(temp &=0x0e,CONTROL_PORT);
    outb(C,DATA_PORT);                    /* output code */
    E_1;                                  //outb(temp &=0x0d,CONTROL_PORT);
    E_0;                                  //outb(temp |=0x02,CONTROL_PORT);
    //while(0);
    RS_0;                                 //outb(temp |=0x01,CONTROL_PORT);
    outb(D,DATA_PORT);                    /* output data */
    E_1;                                  //outb(temp &=0x0d,CONTROL_PORT);
    E_0;                                  //outb(temp |=0x02,CONTROL_PORT);
  } catch(...) {}
}
//---------------------------------------------------------------------------
void LCD240x128::CURSOR(unsigned int ram_address) //定位:此Function不變
{
try {
    unsigned char LSB=ram_address & 0x00ff;
    unsigned char MSB=(ram_address>>8) & 0x00ff;
    WRITE_PORT(10,LSB);
    WRITE_PORT(11,MSB);
  } catch(...) {}
}
//---------------------------------------------------------------------------
bool LCD240x128::CLEAR(void)                //call此function就會清除營幕
{
try {
    if (haveGetPower) {
        CURSOR(0);
        for (int i=0;i<0x2000;i++)
             WRITE_PORT(12,0);
        return true;
    } else {
        GetAuthority(DATA_PORT);
        return false;
    }
  } catch(...) {}
}
//---------------------------------------------------------------------------
bool LCD240x128::GRAPHIC_MODE(void)       //call此function就切換至圖型模式
{
try {
    if (haveGetPower) {
        WRITE_PORT(0,0x12);               //模式控制
        WRITE_PORT(1,0x77);               //字元間距
        WRITE_PORT(2,29);                 //字元數
        WRITE_PORT(3,127);                //時間分割數
        WRITE_PORT(4,0x7);                //游標位置
        WRITE_PORT(0,0x32);               //模式控制
        WRITE_PORT(8,0);                  //低啟始位置
        WRITE_PORT(9,0);                  //高啟始位置
        WRITE_PORT(10,0);                 //低RAM位置
        WRITE_PORT(11,0);                 //高RAM位置
        return true;
    } else {
        GetAuthority(DATA_PORT);
        return false;
    }
  } catch(...) {}
}
//---------------------------------------------------------------------------
bool LCD240x128::CHARACTER_MODE(void)     //call此function就切換至文字模式
{
try {
    if (haveGetPower) {
        WRITE_PORT(0,0x1C);               //模式控制
        WRITE_PORT(1,0x77);               //字元間距
        WRITE_PORT(2,39);                 //字元數
        WRITE_PORT(3,127);                //時間分割數
        WRITE_PORT(4,0x7);                //游標位置
        WRITE_PORT(0,0x3C);               //模式控制
        WRITE_PORT(8,0);                  //低啟始位置
        WRITE_PORT(9,0);                  //高啟始位置
        WRITE_PORT(10,0);                 //低RAM位置
        WRITE_PORT(11,0);                 //高RAM位置
        return true;
    } else {
        GetAuthority(DATA_PORT);
        return false;
    }
  } catch(...) {}
}
//---------------------------------------------------------------------------
unsigned char LCD240x128::Invert(unsigned char data)
{
try {
    return (
             ((data & 0x80) >> 7 )  /* bit7 --> bit0 */
            +((data & 0x40) >> 5 )  /* bit6 --> bit1 */
            +((data & 0x20) >> 3 )  /* bit5 --> bit2 */
            +((data & 0x10) >> 1 )  /* bit4 --> bit3 */
            +((data & 0x08) << 1 )  /* bit3 --> bit4 */
            +((data & 0x04) << 3 )  /* bit2 --> bit5 */
            +((data & 0x02) << 5 )  /* bit1 --> bit6 */
            +((data & 0x01) << 7 )  /* bit0 --> bit7 */
    );
  } catch(...) {}
}
//---------------------------------------------------------------------------
//如果要顯示一個16*16的字就是 heightBit=16,widthByte=2
//如果要顯示一個24*24的字就是 heightBit=24,widthByte=3
//如果要顯示整個240*128的畫面就是 heightBit=128,widthByte=30
//螢幕上那一點開始打起說明
//如果要從第(0,0)點打起ram_address=0
//如果要從第(X,Y)點打起ram_address=X/8+30*Y
//如果要從第(16,0)點打起ram_address=16/8+30*0=2
//如果要從第(24,0)點打起ram_address=24/8+30*0=3
//如果要從第(0,16)點打起ram_address=0/8+30*16=480
//如果要從第(0,24)點打起ram_address=0/8+30*24=720
//---------------------------------------------------------------------------
bool LCD240x128::DISPLAY_GRAPHIC(unsigned int ram_address,unsigned char * font_address,int heightBit,int widthByte)
{                                //從螢幕上那一點開始打起,顯示的bitmap       ,要顯示的圖高幾個bit,要顯示的圖寬幾個byte
try {
    unsigned char *font_addressInput;
    unsigned int ram_addressInput;

    pthread_mutex_lock(&mutexLcd);

    GRAPHIC_MODE();

    CLEAR();                     //先把記憶體清空

    for(int iLoop = 0; iLoop < 2; iLoop++) {  //ot add for ICOP
      font_addressInput = font_address;
      ram_addressInput = ram_address;

      CURSOR(ram_address);
      for (int i=0;i<heightBit;i++) {                         //heightBit,要顯示的圖高幾個bit
           for (int j=0;j<widthByte;j++) {                    //widthByte,要顯示的圖寬幾個byte
                WRITE_PORT(12,Invert(*font_addressInput));
                font_addressInput++;
           }
           CURSOR(ram_addressInput=ram_addressInput+30);
      }
    }

    pthread_mutex_unlock(&mutexLcd);

    return true;
  } catch(...) {}
}
//---------------------------------------------------------------------------
bool LCD240x128::DISPLAY_GRAPHIC_XY(int X,int Y,unsigned char * font_address,int heightBit,int widthByte)
{
try {
    int ram_address;
    unsigned char *font_addressInput;

    GRAPHIC_MODE();  //For ICOP

    pthread_mutex_lock(&mutexLcd);

  for(int iLoop = 0; iLoop < 3; iLoop++) {  //ot add for ICOP
    font_addressInput = font_address;

    ram_address=X/8+30*Y;
    CURSOR(ram_address);
    for (int i=0;i<heightBit;i++) {
         for (int j=0;j<widthByte;j++) {
              WRITE_PORT(12,Invert(*font_addressInput));
              font_addressInput++;
         }
         CURSOR(ram_address=ram_address+30);
    }
  }

    pthread_mutex_unlock(&mutexLcd);

    return true;

  } catch(...) {}
}

//---------------------------------------------------------------------------
void LCD240x128::DISPLAY_GRAPHICFULL_SPECIAL1(unsigned char * font_address)   //拼湊效果
{
try {
    pthread_mutex_lock(&mutexLcd);
    CLEAR();
    for (int ram_address=30;ram_address>=0;ram_address--)
         SPECIAL1(ram_address,font_address);
    pthread_mutex_unlock(&mutexLcd);
  } catch(...) {}
}
//---------------------------------------------------------------------------
void LCD240x128::SPECIAL1(int ram_address,unsigned char * font_address)
{
try {
         int temp_ram_address=ram_address;
         CURSOR(ram_address);
         for (int i=0;i<128;i++) {
              for (int j=0;j<30-temp_ram_address;j++) {
                   WRITE_PORT(12,Invert(*font_address));
                   font_address++;
              }
              CURSOR(ram_address=ram_address+30);
         }
  } catch(...) {}
}
//---------------------------------------------------------------------------
void LCD240x128::DISPLAY_GRAPHICFULL_SPECIAL2(unsigned char * font_address)   //由右至左顯示效果
{
try {
    pthread_mutex_lock(&mutexLcd);
    CLEAR();
    for (int ram_address=30;ram_address>=0;ram_address--)
         SPECIAL2(ram_address,font_address);
    pthread_mutex_unlock(&mutexLcd);
  } catch(...) {}
}
//---------------------------------------------------------------------------
void LCD240x128::SPECIAL2(int ram_address,unsigned char * font_address)
{
try {
         int temp_ram_address=ram_address;
         CURSOR(ram_address);
         for (int i=0;i<128;i++) {
              for (int j=0;j<30;j++) {
                   if(j>=temp_ram_address)
                      WRITE_PORT(12,Invert(*font_address));
                   font_address++;
              }
              CURSOR(ram_address=ram_address+30);
         }
  } catch(...) {}
}
//---------------------------------------------------------------------------
void LCD240x128::DISPLAY_GRAPHICFULL_SPECIAL3(unsigned char * font_address)   //由右至左推進效果
{
try {
    pthread_mutex_lock(&mutexLcd);
    CLEAR();
    for (int ram_address=30;ram_address>=0;ram_address--)
         SPECIAL3(ram_address,font_address);
    pthread_mutex_unlock(&mutexLcd);
  } catch(...) {}
}
//---------------------------------------------------------------------------
void LCD240x128::SPECIAL3(int ram_address,unsigned char * font_address)
{
try {
         int temp_ram_address=30-ram_address;
         int count=0;
         CURSOR(ram_address);
         for (int i=0;i<128;i++) {
              for (int j=0;j<30;j++) {
                   count++;
                   if (count%30<=temp_ram_address)
                       WRITE_PORT(12,Invert(*font_address));
                   font_address++;
              }
              CURSOR(ram_address=ram_address+30);
         }
  } catch(...) {}
}
//---------------------------------------------------------------------------
