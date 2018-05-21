#ifndef SCREENNoneH
#define SCREENNoneH

#include "SCREEN.h"
#include "var.h"
//---------------------------------------------------------------------------
class SCREENNone:public SCREEN
{
    public:
        SCREENNone(void);
        ~SCREENNone(void);

        void DisplayNone(void);                                                 //顯示空白頁
        void DoKeyWork(BYTE);                                                   //根據KEY的值作事

    private:
        BYTE noneBitmap[3840];                                                  //空白底圖

        void DoKeyDefaultWork();                                                //按其他鍵時作的事
};
//---------------------------------------------------------------------------
extern SCREENNone screenNone;
#endif

