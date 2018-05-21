#ifndef SCREENCommStatH
#define SCREENCommStatH

#include "SCREEN.h"
//---------------------------------------------------------------------------
class SCREENCommStat:public SCREEN
{
    public:
        SCREENCommStat(void);
        ~SCREENCommStat(void);

        void DisplayCommStat(void);
        void DoKeyWork(BYTE);                                                   //根據KEY的值作事

        void UpdateComm(void);


    private:
        BYTE commStatBitmap[3840];                                              //底圖

        void LoadBitmapFromFile(void);                                          //將底圖Load進記憶體

        void UpdateCenterComm(void);
//        void UpdateSSComm(void);

        void DoKeyF1Work();
        void DoKeyF2Work();
        void DoKeyF3Work();
        void DoKeyF4Work();
};
//---------------------------------------------------------------------------
extern SCREENCommStat screenCommStat;
#endif
