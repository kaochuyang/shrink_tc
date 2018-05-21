#ifndef SCREENLockdbH
#define SCREENLockdbH

#include "SCREEN.h"
#include "var.h"
//---------------------------------------------------------------------------
class SCREENLockdb:public SCREEN
{
    public:
        SCREENLockdb(void);
        ~SCREENLockdb(void);

        void DisplayLockdb(void);
        void DoKeyWork(BYTE);

    private:
        BYTE lockdbBitmap[3840];

        void LoadBitmapFromFile(void);
        void DoKeyEnterWork();
};
//---------------------------------------------------------------------------
extern SCREENLockdb screenLockdb;
#endif
