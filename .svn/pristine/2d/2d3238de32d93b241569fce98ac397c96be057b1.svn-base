/*
    LCX_403-5 燈號驅動卡:
    1.可接收設定時間指令,指定設定參數時間
    2.時間參數會自動向上加,至255即停止並將燈號點為預設(By DipSwitch)
    3.可接受設定輸出埠參數(@20,@21,@22,@23,@24,@25)
    4.一個Address具8個燈點,分別代表  紅,黃,左綠,圓綠,直綠,右綠,行人紅,行人綠
    5.燈點設定每次必須全部設一輪(不論是否需要用到),也就是每次都要設定@20~@25
    6.燈號驅動卡的時間參數會不斷的往上數,當 a.設定時間參數 b.收到@20~@25的輸出埠控制 會改變其值
*/

#ifndef lightH
#define lightH

#define BYTE unsigned char
//----------------------------------------------------------------------
class Light
{
    public:
      Light (void);
      ~Light (void);

      //Base
      bool GetAuthority(unsigned long);      //得到控制權
      bool ReleaseAuthority(void);           //釋放控制權
      bool SetTimeOfCard(BYTE);              //設定燈號驅動卡時間參數
      bool SetLight(BYTE,BYTE,BYTE);         //設定燈號輸出埠

      //Advance
      bool SetLight(BYTE *);                 //點燈
      bool SetAllLight(void);                //點燈全亮
      bool SetAllDark(void);                 //點燈全暗
      bool SetCardDefault(void);             //將燈號驅動卡設default:使用設定時間(254秒)方式
      bool SayHelloToCard(void);             //將燈號驅動卡時間參數歸0,不會跳default

    private:
      unsigned long LPTBASEPORT;             //IO位置
      bool haveGetPower;                     //是否獲得控制LPT的權限

};
//----------------------------------------------------------------------
extern Light light;
#endif
