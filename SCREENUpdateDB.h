#ifndef SCREENUpdateDBH
#define SCREENUpdateDBH

#include "SCREEN.h"
//---------------------------------------------------------------------------
class SCREENUpdateDB:public SCREEN
{
    public:
        SCREENUpdateDB(void);
        ~SCREENUpdateDB(void);

        void DisplayUpdateDB(void);                                             //顯示
        void DoKeyWork(BYTE);                                                   //根據KEY的值作事

    private:
        BYTE updateDBBitmap[3840];                                              //底圖

        void LoadBitmapFromFile(void);                                          //將底圖Load進記憶體
        void DoKeyF1Work();
        void DoKeyF4Work();
        void DoKeyEnterWork();
};
//---------------------------------------------------------------------------
extern SCREENUpdateDB screenUpdateDB;
#endif

