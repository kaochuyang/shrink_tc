//---------------------------------------------------------------------------

#ifndef PDEVICEKEYPADH
#define PDEVICEKEYPADH

#include "var.h"
//---------------------------------------------------------------------------
class PDEVICEKEYPAD
{
    public:

        PDEVICEKEYPAD(void);
        ~PDEVICEKEYPAD(void);
        bool DoWorkViaPDevice(MESSAGEOK);                                       //根據PDevice來的封包作事情
        void SendStartMsg(void);

    private:

        BYTE DATA_P1;                                                           //25個鍵盤加按鈕(按鈕不使用)
        BYTE DATA_P2;                                                           //不使用(TC用來讀AD值)
        DATA_Bit DATA_P3;                                                       //SW27~SW34(僅使用SW27遠端或現場操作)
        BYTE DATA_P4;                                                           //SW35(Address Low Byte)
        BYTE DATA_P5;                                                           //SW36(Address Hi Byte)
        DATA_Bit DATA_P6;                                                           //SW37(不使用)

        DATA_Bit _LastDATA_P6;

        //jacky20131231
        BYTE DATA0;
        DATA_Bit DATA1;

        void DoDisplayLcdWorkByData_P1(BYTE);                                   //控制LCD變化

/*OTCombo0714*/
        int cSwitch;                                                            //現在執行(面板)的控制策略 0:沒開關 1:全紅 2:閃光 3:手動

        bool vSavePlan(unsigned char);

        unsigned char ucLastData[6];

        unsigned char ucData[6];
        MESSAGEOK _MSG;
};
//---------------------------------------------------------------------------
#endif
//---------------------------------------------------------------------------
/*    無按鍵    00H
      0         80H
      1         81H
      2         82H
      3         83H
      4         84H
      5         85H
      6         86H
      7         87H
      8         88H
      9         89H
      A         8AH
      B         8BH
      C         8CH
      D         8DH
      E         8EH
      F         8FH
      ESC/F1    90H
      F2        91H
      F3        92H
      F4        93H
      UP        94H
      DOWN      95H
      LEFT      96H
      RIGHT     97H
      ENTER     98H
      MANUAL    99H
*/
//---------------------------------------------------------------------------
/*
    LCX_403-2 鍵盤開關輸入介面卡(應用於號誌號制器):
    1.SW1~26(25個鍵盤與1個手動步階按鈕),SW27(全紅),SW28(閃光),SW29(手動),SW35~SW37(DipSwitch*3)
    2.Initial時Keypad會送協定進PC,協定內容為現在狀態
    3.按下任一按鍵或開關或DipSwitch都會送協定回PC
    4.PC可送要求狀態詢問其目前狀態
    5.PC可讀取AD值,平時Interrupt回傳時AD值為0,只有當要求時AD才為正確值
*/
/*
    LCX_403-2 鍵盤開關輸入介面卡(應用於車輛偵測控制器):
    1.SW1~26(25個鍵盤與SW26不使用),SW27(遠端與現場操作),SW28(不使用),SW29(不使用),SW35~SW37(DipSwitch*3) SW35,SW36為16位元路口編號,SW37不用

*/
//---------------------------------------------------------------------------
