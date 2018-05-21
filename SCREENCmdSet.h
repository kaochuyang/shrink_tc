#ifndef SCREENCmdSetH
#define SCREENCmdSetH

#include "SCREEN.h"
#include "var.h"
//---------------------------------------------------------------------------
class SCREENCmdSet:public SCREEN
{
    public:
        SCREENCmdSet(void);
        ~SCREENCmdSet(void);

        void DisplayCmdSet(void);
        void DoKeyWork(BYTE);

    private:
        BYTE cmdSetBitmap[3840];

        DISP_WORD command;
        DISP_WORD position[4];
        int cSelect;

        void LoadBitmapFromFile(void);
        void InitDispWord(void);                                                  //初始化此畫面的座標等參數

        void DoKey0Work(void);
        void DoKey1Work(void);
        void DoKey2Work(void);
        void DoKey3Work(void);

        void DoKeyF1Work(void);
        void DoKeyF2Work(void);
        void DoKeyF4Work(void);

        void DoKeyUPWork(void);
        void DoKeyDOWNWork(void);

        void DoKeyEnterWork(void);
};
//---------------------------------------------------------------------------
extern SCREENCmdSet screenCmdSet;
#endif

