/*
    LCD模組 使用LPT 尺寸:240x128 pixel
    1.寫一面需30*128=3840個BYTE (一列240pixel為30個BYTE,共128列)
*/
//----------------------------------------------------------------------
#ifndef LCD240x128H
#define LCD240x128H

#include "pthread.h"
//----------------------------------------------------------------------
class LCD240x128
{
    public:
      LCD240x128 (void);
      ~LCD240x128 (void);

      bool GetAuthority(unsigned long);                                         //得到控制權
      bool ReleaseAuthority(void);                                              //釋放控制權
      bool CLEAR_SANYO(void);                                                         //清除營幕
      bool GRAPHIC_MODE(void);                                                  //切換至圖型模式
      bool CHARACTER_MODE_SANYO(void);                                                //切換至文字模式

      bool DISPLAY_GRAPHIC(unsigned int,unsigned char *,int,int);               //顯示GRAPHIC(正常顯示)
      bool DISPLAY_GRAPHIC_XY(int,int,unsigned char *,int,int);                 //顯示GRAPHIC(正常顯示)
      void DISPLAY_GRAPHICFULL_SPECIAL1_SANYO(unsigned char *);                       //顯示GRAPHIC(整面:拼湊效果)
      void DISPLAY_GRAPHICFULL_SPECIAL2_SANYO(unsigned char *);                       //顯示GRAPHIC(整面:由右至左顯示效果)
      void DISPLAY_GRAPHICFULL_SPECIAL3_SANYO(unsigned char *);                       //顯示GRAPHIC(整面:由右至左推進效果)

      //TOSHIBA
      bool CLEAR_TOSHIBA(void);                                                         //清除營幕

    private:

      pthread_mutex_t mutexLcd;                                                 //保護記憶體
      int lcdWidth;                                                             //LCD面板的寬
      int lcdHeight;                                                            //LCD面板的高
      unsigned long DATA_PORT;                                                  //IO位置:DATA PORT
      unsigned long CONTROL_PORT;                                               //IO位置:CONTROL PORT
      bool haveGetPower;                                                        //是否獲得控制LPT的權限
      unsigned char defaultSignal;                                              //原LPT上的訊號
      unsigned char temp;

      void WRITE_PORT_SANYO(unsigned char,unsigned char);                             //寫Port
      void CURSOR_SANYO(unsigned short int);                                                //定位
      unsigned char Invert(unsigned char);                                      //將Bitmap轉換為燈點

      void SPECIAL1_SANYO(int,unsigned char *);                                       //顯示GRAPHIC(整面:拼湊效果)
      void SPECIAL2_SANYO(int,unsigned char *);                                       //顯示GRAPHIC(整面:由右至左顯示效果)
      void SPECIAL3_SANYO(int,unsigned char *);

      //for toshiba
      void CURSOR_TOSHIBA(unsigned short int);
      void WRITE_DATA2_TOSHIBA(unsigned char,unsigned char);
      void WRITE_DATA_TOSHIBA(unsigned char);
      void WRITE_CMD_TOSHIBA(unsigned char);                                       //顯示GRAPHIC(整面:由右至左推進效果)
};
//-------------------------------------------------------------------
extern LCD240x128 lcd240x128;
#endif
