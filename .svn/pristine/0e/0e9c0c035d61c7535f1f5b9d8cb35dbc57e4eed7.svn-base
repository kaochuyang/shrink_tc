#ifndef SCREENLogoH
#define SCREENLogoH

#include "SCREEN.h"
#include "var.h"
//---------------------------------------------------------------------------
class SCREENLogo:public SCREEN
{
    public:
        SCREENLogo(void);
        ~SCREENLogo(void);

        void DisplayLogo(void);                                                 //顯示首頁
        void DoKeyWork(BYTE);                                                   //根據KEY的值作事

    private:
        BYTE logoBitmap[3840];                                                  //底圖

        void LoadBitmapFromFile(void);                                          //將底圖Load進記憶體
        void DoKeyEscWork();                                                    //按ESC鍵時作的事
        void DoKeyDefaultWork();                                                //按其他鍵時作的事
};
//---------------------------------------------------------------------------
extern SCREENLogo screenLogo;
#endif
