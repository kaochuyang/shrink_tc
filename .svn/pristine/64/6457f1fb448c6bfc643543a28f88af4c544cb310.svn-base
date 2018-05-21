#ifndef SCREENABOErrH
#define SCREENABOErrH

#include "SCREEN.h"
#include "var.h"
//---------------------------------------------------------------------------
class SCREENABOErr:public SCREEN
{
    public:
        SCREENABOErr(void);
        ~SCREENABOErr(void);

        void DisplayABOErr(void);
        void DoKeyWork(BYTE);

    private:
        BYTE aboErrBitmap[3840];

        void LoadBitmapFromFile(void);
        void DoKeyEnterWork();
};
//---------------------------------------------------------------------------
extern SCREENABOErr screenABOErr;
#endif





