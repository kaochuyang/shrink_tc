//---------------------------------------------------------------------------

//#define SAYNO_LCD
#define TOSHIBA_LCD

#include "LCD240x128.h"

#include <stdio.h>
#include <sys/perm.h>
#include <sys/io.h>

//OT20140314
#include "SMEM.h"

//for sanyo
#define E_1   outb(temp &=0x0d,CONTROL_PORT);
#define E_0   outb(temp |=0x02,CONTROL_PORT);
#define RS_1  outb(temp &=0x0e,CONTROL_PORT);
#define RS_0  outb(temp |=0x01,CONTROL_PORT);

//for toshiba
#define WR_H  outb(temp &=0x0d,CONTROL_PORT);  //old pin is E  //for LC7981
#define WR_L  outb(temp |=0x02,CONTROL_PORT);
#define CD_H  outb(temp &=0x0e,CONTROL_PORT);  //old pin is RS for LC7981
#define CD_L  outb(temp |=0x01,CONTROL_PORT);
#define RD_H  outb(temp &=0x0b,CONTROL_PORT);  //
#define RD_L  outb(temp |=0x04,CONTROL_PORT);
#define CE_H  outb(temp &=0x07,CONTROL_PORT);  //
#define CE_L  outb(temp |=0x08,CONTROL_PORT);

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
    temp = 0;

  } catch(...) {}
}
//---------------------------------------------------------------------------
LCD240x128::~LCD240x128 (void)
{
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

//#ifdef SAYNO_LCD
#ifdef TOSHIBA_LCD
//---------------------------------------------------------------------------
bool LCD240x128::GetAuthority(unsigned long ioAddress)
{
try {
    unsigned char ucLCDSwitch = 1;
//    ucLCDSwitch = smem.vGetLCDVersion();
    if(ucLCDSwitch == 0) {
    if (ioperm(ioAddress,3,1)==0) {       //得到控制權
        DATA_PORT=ioAddress;              //BASEPORT
        CONTROL_PORT=ioAddress+2;         //CONTROL_PORT=BASEPORT+2
        haveGetPower=true;                //turn on the flag
        defaultSignal=inb(CONTROL_PORT);  //得到CONTROL_PORT Default Status,結束時還原
        outb(3,CONTROL_PORT);             //Set Bit0=1,Bit1=1
        return true;
    } else return false;

    } else if (ucLCDSwitch == 1) {
        if (ioperm(ioAddress,3,1)==0) {       //得到控制權
        DATA_PORT=ioAddress;              //BASEPORT
        CONTROL_PORT=ioAddress+2;         //CONTROL_PORT=BASEPORT+2
        haveGetPower=true;                //turn on the flag
        defaultSignal=inb(CONTROL_PORT);  //得到CONTROL_PORT Default Status,結束時還原

        // printf("defaultSignal:%0X\n", defaultSignal);


        // outb(3,CONTROL_PORT);             //Set Bit0=1,Bit1=1

        GRAPHIC_MODE();

        return true;
    } else return false;
    }
  } catch(...) {}
}

//---------------------------------------------------------------------------
void LCD240x128::WRITE_PORT_SANYO(unsigned char C,unsigned char D)
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
void LCD240x128::CURSOR_SANYO(unsigned short int ram_address) //定位:此Function不變
{
try {
    unsigned char LSB=ram_address & 0x00ff;
    unsigned char MSB=(ram_address>>8) & 0x00ff;
    WRITE_PORT_SANYO(10,LSB);
    WRITE_PORT_SANYO(11,MSB);
  } catch(...) {}
}
//---------------------------------------------------------------------------
bool LCD240x128::CLEAR_SANYO(void)                //call此function就會清除營幕
{
try {
    if (haveGetPower) {
        CURSOR_SANYO(0);
        for (int i=0;i<0x2000;i++)
             WRITE_PORT_SANYO(12,0);
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
    unsigned char ucLCDSwitch = 1;
//    ucLCDSwitch = smem.vGetLCDVersion();
    if(ucLCDSwitch == 0) {
    if (haveGetPower) {
        WRITE_PORT_SANYO(0,0x12);               //模式控制
        WRITE_PORT_SANYO(1,0x77);               //字元間距
        WRITE_PORT_SANYO(2,29);                 //字元數
        WRITE_PORT_SANYO(3,127);                //時間分割數
        WRITE_PORT_SANYO(4,0x7);                //游標位置
        WRITE_PORT_SANYO(0,0x32);               //模式控制
        WRITE_PORT_SANYO(8,0);                  //低啟始位置
        WRITE_PORT_SANYO(9,0);                  //高啟始位置
        WRITE_PORT_SANYO(10,0);                 //低RAM位置
        WRITE_PORT_SANYO(11,0);                 //高RAM位置
        return true;
    } else {
        GetAuthority(DATA_PORT);
        return false;
    }

    } else if(ucLCDSwitch == 1) {
        if (haveGetPower) {
        //set text address
        WRITE_DATA2_TOSHIBA(0x00, 0x00);
        WRITE_CMD_TOSHIBA(0x40);

        //set graphic address
        WRITE_DATA2_TOSHIBA(0x00, 0x40);  //0x1000
        WRITE_CMD_TOSHIBA(0x42);

        //text area colume set
        WRITE_DATA2_TOSHIBA(30, 0x00);
        WRITE_CMD_TOSHIBA(0x41);

        //graphics area colume set
        WRITE_DATA2_TOSHIBA(30, 0x00);
        WRITE_CMD_TOSHIBA(0x43);

        //mode set (set display mode, graphic)
        WRITE_CMD_TOSHIBA(0x80);
        WRITE_CMD_TOSHIBA(0x98);

        // FULLON();

        // printf("get authority ok\n");
        return true;
    } else {
        GetAuthority(DATA_PORT);
        return false;
    }
    }
  } catch(...) {}
}
//---------------------------------------------------------------------------
bool LCD240x128::CHARACTER_MODE_SANYO(void)     //call此function就切換至文字模式
{
try {
    if (haveGetPower) {
        WRITE_PORT_SANYO(0,0x1C);               //模式控制
        WRITE_PORT_SANYO(1,0x77);               //字元間距
        WRITE_PORT_SANYO(2,39);                 //字元數
        WRITE_PORT_SANYO(3,127);                //時間分割數
        WRITE_PORT_SANYO(4,0x7);                //游標位置
        WRITE_PORT_SANYO(0,0x3C);               //模式控制
        WRITE_PORT_SANYO(8,0);                  //低啟始位置
        WRITE_PORT_SANYO(9,0);                  //高啟始位置
        WRITE_PORT_SANYO(10,0);                 //低RAM位置
        WRITE_PORT_SANYO(11,0);                 //高RAM位置
        return true;
    } else {
        GetAuthority(DATA_PORT);
        return false;
    }
  } catch(...) {}
}

//---------------------------------------------------------------------------
bool LCD240x128::DISPLAY_GRAPHIC(unsigned int ram_address,unsigned char * font_address,int heightBit,int widthByte)
{                                //從螢幕上那一點開始打起,顯示的bitmap       ,要顯示的圖高幾個bit,要顯示的圖寬幾個byte
try {
    unsigned char *font_addressInput;
    unsigned int ram_addressInput;
    unsigned char ucTmp;

    unsigned char ucLCDSwitch = 1;
//    ucLCDSwitch = smem.vGetLCDVersion();
    if(ucLCDSwitch == 0) {

    pthread_mutex_lock(&mutexLcd);

    GRAPHIC_MODE();

    CLEAR_SANYO();                     //先把記憶體清空

    for(int iLoop = 0; iLoop < 2; iLoop++) {  //ot add for ICOP
      font_addressInput = font_address;
      ram_addressInput = ram_address;

      CURSOR_SANYO(ram_address);
      for (int i=0;i<heightBit;i++) {                         //heightBit,要顯示的圖高幾個bit
           for (int j=0;j<widthByte;j++) {                    //widthByte,要顯示的圖寬幾個byte
                WRITE_PORT_SANYO(12,Invert(*font_addressInput));
                font_addressInput++;
           }
           CURSOR_SANYO(ram_addressInput=ram_addressInput+30);
      }
    }

    pthread_mutex_unlock(&mutexLcd);

    } else if(ucLCDSwitch == 1) {
        if(haveGetPower) {

    pthread_mutex_lock(&mutexLcd);

    GRAPHIC_MODE();

    CLEAR_TOSHIBA();                     //先把記憶體清空

    // ram_address += 0x4000;  //in this system, default graphic address is 0x4000.

    font_addressInput = font_address;
    ram_addressInput = ram_address;

    CURSOR_TOSHIBA(ram_addressInput);

    WRITE_CMD_TOSHIBA(0xB0);  //auto write start

    for (int i=0;i<heightBit;i++) {                         //heightBit,要顯示的圖高幾個bit

//        WRITE_CMD(0xB0);  //auto write start
        for (int j=0;j<widthByte;j++) {                    //widthByte,要顯示的圖寬幾個byte
            //? ucTmp = Invert(*font_addressInput);
            ucTmp = (*font_addressInput);
            WRITE_DATA_TOSHIBA(ucTmp);

            // printf("ucTmp:%0x, font_addressInput:%0x, add:%0000x\n", ucTmp, *font_addressInput, ram_addressInput);
            font_addressInput++;

            ram_addressInput++;
            // getchar();
        }
//        WRITE_CMD(0xB2);  //auto write start
//        CURSOR(ram_addressInput=ram_addressInput+30);
    }
    WRITE_CMD_TOSHIBA(0xB2);  //auto write stop


    pthread_mutex_unlock(&mutexLcd);

    }
    }

    return true;
  } catch(...) {}
}
//---------------------------------------------------------------------------
bool LCD240x128::DISPLAY_GRAPHIC_XY(int X,int Y,unsigned char * font_address,int heightBit,int widthByte)
{
try {
    int ram_address;
    unsigned char *font_addressInput;
    unsigned char ucTmp;

    unsigned char ucLCDSwitch = 1;
//    ucLCDSwitch = smem.vGetLCDVersion();
    if(ucLCDSwitch == 0) {

    GRAPHIC_MODE();  //For ICOP

    pthread_mutex_lock(&mutexLcd);

  for(int iLoop = 0; iLoop < 3; iLoop++) {  //ot add for ICOP
    font_addressInput = font_address;

    ram_address=X/8+30*Y;
    CURSOR_SANYO(ram_address);
    for (int i=0;i<heightBit;i++) {
         for (int j=0;j<widthByte;j++) {
              WRITE_PORT_SANYO(12,Invert(*font_addressInput));
              font_addressInput++;
         }
         CURSOR_SANYO(ram_address=ram_address+30);
    }
  }

    pthread_mutex_unlock(&mutexLcd);

    } else if(ucLCDSwitch == 1) {

if(haveGetPower) {

   // GRAPHIC_MODE();  //For ICOP

    pthread_mutex_lock(&mutexLcd);

    font_addressInput = font_address;
    ram_address = X/8+30*Y;
    // printf("ram_address:%x\n", ram_address);
    // ram_address += 0x4000; //base address is 00x4000

    CURSOR_TOSHIBA(ram_address);

    for (int i=0;i<heightBit;i++) {

         // WRITE_CMD_TOSHIBA(0xB0);
         for (int j=0;j<widthByte;j++) {

              // ucTmp = Invert(*font_addressInput);
              ucTmp = *font_addressInput;
              WRITE_DATA_TOSHIBA(ucTmp);
              WRITE_CMD_TOSHIBA(0xC0);

              font_addressInput++;
         }
        // WRITE_CMD_TOSHIBA(0xB2);
         ram_address=ram_address+30;
         CURSOR_TOSHIBA(ram_address);
    }

    pthread_mutex_unlock(&mutexLcd);

    }

    }

    return true;

  } catch(...) {}
}

//---------------------------------------------------------------------------
void LCD240x128::DISPLAY_GRAPHICFULL_SPECIAL1_SANYO(unsigned char * font_address)   //拼湊效果
{
try {
    pthread_mutex_lock(&mutexLcd);
    CLEAR_SANYO();
    for (int ram_address=30;ram_address>=0;ram_address--)
         SPECIAL1_SANYO(ram_address,font_address);
    pthread_mutex_unlock(&mutexLcd);
  } catch(...) {}
}
//---------------------------------------------------------------------------
void LCD240x128::SPECIAL1_SANYO(int ram_address,unsigned char * font_address)
{
try {
         int temp_ram_address=ram_address;
         CURSOR_SANYO(ram_address);
         for (int i=0;i<128;i++) {
              for (int j=0;j<30-temp_ram_address;j++) {
                   WRITE_PORT_SANYO(12,Invert(*font_address));
                   font_address++;
              }
              CURSOR_SANYO(ram_address=ram_address+30);
         }
  } catch(...) {}
}
//---------------------------------------------------------------------------
void LCD240x128::DISPLAY_GRAPHICFULL_SPECIAL2_SANYO(unsigned char * font_address)   //由右至左顯示效果
{
try {
    pthread_mutex_lock(&mutexLcd);
    CLEAR_SANYO();
    for (int ram_address=30;ram_address>=0;ram_address--)
         SPECIAL2_SANYO(ram_address,font_address);
    pthread_mutex_unlock(&mutexLcd);
  } catch(...) {}
}
//---------------------------------------------------------------------------
void LCD240x128::SPECIAL2_SANYO(int ram_address,unsigned char * font_address)
{
try {
         int temp_ram_address=ram_address;
         CURSOR_SANYO(ram_address);
         for (int i=0;i<128;i++) {
              for (int j=0;j<30;j++) {
                   if(j>=temp_ram_address)
                      WRITE_PORT_SANYO(12,Invert(*font_address));
                   font_address++;
              }
              CURSOR_SANYO(ram_address=ram_address+30);
         }
  } catch(...) {}
}
//---------------------------------------------------------------------------
void LCD240x128::DISPLAY_GRAPHICFULL_SPECIAL3_SANYO(unsigned char * font_address)   //由右至左推進效果
{
try {
    pthread_mutex_lock(&mutexLcd);
    CLEAR_SANYO();
    for (int ram_address=30;ram_address>=0;ram_address--)
         SPECIAL3_SANYO(ram_address,font_address);
    pthread_mutex_unlock(&mutexLcd);
  } catch(...) {}
}
//---------------------------------------------------------------------------
void LCD240x128::SPECIAL3_SANYO(int ram_address,unsigned char * font_address)
{
try {
         int temp_ram_address=30-ram_address;
         int count=0;
         CURSOR_SANYO(ram_address);
         for (int i=0;i<128;i++) {
              for (int j=0;j<30;j++) {
                   count++;
                   if (count%30<=temp_ram_address)
                       WRITE_PORT_SANYO(12,Invert(*font_address));
                   font_address++;
              }
              CURSOR_SANYO(ram_address=ram_address+30);
         }
  } catch(...) {}
}

//---------------------------------------------------------------------------
bool LCD240x128::CLEAR_TOSHIBA(void)                //call此function就會清除營幕
{
try {
    if (haveGetPower) {
        WRITE_DATA2_TOSHIBA(0x00, 0x40);
        WRITE_CMD_TOSHIBA(0x24);
        WRITE_CMD_TOSHIBA(0xB0);
        for(int i = 0; i < 128; i++) {
            for(int j = 0; j < 30; j++) {
                WRITE_DATA_TOSHIBA(0);
            }
        }
        WRITE_CMD_TOSHIBA(0xB2);
    } else {
        GetAuthority(DATA_PORT);
        return false;
    }
  } catch(...) {}
}


//---------------------------------------------------------------------------
void LCD240x128::CURSOR_TOSHIBA(unsigned short int ram_address)  //for toshiba
{
try {

    unsigned char lo, hi;
    unsigned short int usiTmp;

    if(haveGetPower) {

    usiTmp = (ram_address >> 8);
    hi = usiTmp & 0xFF;
//    printf("usiTmp:%x, hi:%x\n", usiTmp, hi);
    usiTmp = (ram_address);
    lo = usiTmp & 0xFF;
//    printf("usiTmp:%x, lo:%x\n", usiTmp, lo);
    WRITE_DATA2_TOSHIBA(lo, hi);
    WRITE_CMD_TOSHIBA(0x24);  //set Address

    }
  } catch(...) {}
}

//---------------------------------------------------------------------------
void LCD240x128::WRITE_DATA2_TOSHIBA(unsigned char L,unsigned char H)
{
try {
    WRITE_DATA_TOSHIBA(L);
    WRITE_DATA_TOSHIBA(H);
  } catch(...) {}
}

//---------------------------------------------------------------------------
void LCD240x128::WRITE_DATA_TOSHIBA(unsigned char C)
{
try {

    CD_L;
    outb(temp,CONTROL_PORT);
    CE_L;
    WR_L;
    //usleep(1);
    // nanosleep(1);
    outb(C,DATA_PORT);                    // output code

    // for(int i = 0; i < delay_cycle; i++);
    CE_H;
    WR_H;

    // for(int i = 0; i < delay_cycle; i++);

  } catch(...) {}
}

//---------------------------------------------------------------------------
void LCD240x128::WRITE_CMD_TOSHIBA(unsigned char C)
{
try {
    CD_H;
    CE_L;
    WR_L;
    outb(C,DATA_PORT);                    // output code
    // for(int i = 0; i < delay_cycle; i++);
    CE_H;
    WR_H;
    // for(int i = 0; i < delay_cycle; i++);
  } catch(...) {}
}


#endif

//---------------------------------------------------------------------------
