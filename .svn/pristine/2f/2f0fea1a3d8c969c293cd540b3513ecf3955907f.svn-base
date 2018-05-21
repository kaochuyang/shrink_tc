#ifndef SCREENLcnEncodeH
#define SCREENLcnEncodeH

#include "SCREEN.h"
//---------------------------------------------------------------------------
class SCREENLcnEncode:public SCREEN
{
    public:
        SCREENLcnEncode(void);
        ~SCREENLcnEncode(void);

        void DisplayLcnEncode(void);
        void DoKeyWork(BYTE);                                                   //根據KEY的值作事

    private:
        BYTE lcnEncodeBitmap[3840];                                              //底圖

        DISP_WORD select[2];
        int cSelect;
        DISP_WORD lcnCode;

        void LoadBitmapFromFile(void);                                          //將底圖Load進記憶體
        void InitDispWord(void);                                                //初始化此畫面的座標等參數

        void DoKey1Work(void);
        void DoKey2Work(void);
        void DoKeyF1Work(void);
        void DoKeyF2Work(void);
        void DoKeyF4Work(void);
        void DoKeyUpWork(void);
        void DoKeyDownWork(void);
        void DoKeyEnterWork(void);
};
//---------------------------------------------------------------------------
extern SCREENLcnEncode screenLcnEncode;
#endif

