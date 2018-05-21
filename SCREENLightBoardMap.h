#ifndef SCREENLightBoardMapH
#define SCREENLightBoardMapH

#include "SCREEN.h"
//---------------------------------------------------------------------------
class SCREENLightBoardMap:public SCREEN
{
    public:
        SCREENLightBoardMap(void);
        ~SCREENLightBoardMap(void);

        void DisplayDirMapping(void);                                              //顯示
        void DoKeyWork(BYTE);                                                   //根據KEY的值作事

    private:
        BYTE ucDirMappingBitmap[3840];                                               //底圖

        DISP_WORD title[8];
        DISP_WORD lane[8];
        unsigned int cPosition;                                                 //現在的位置
//        int laneCount;
        unsigned short int usiMappingValue[8];                                                       //要記錄對應的車道

        void LoadBitmapFromFile(void);                                          //將底圖Load進記憶體
        void InitDispWord(void);                                                //初始化此畫面的座標等參數

        void DoKey0Work(void);
        void DoKey1Work(void);
        void DoKey2Work(void);
        void DoKey3Work(void);
        void DoKey4Work(void);
        void DoKey5Work(void);
        void DoKey6Work(void);
        void DoKey7Work(void);

        void DoKeyF1Work(void);
        void DoKeyF2Work(void);
        void DoKeyF4Work(void);

        void DoKeyLEFTWork(void);
        void DoKeyRIGHTWork(void);

        void DoKeyEnterWork(void);
};
//---------------------------------------------------------------------------
extern SCREENLightBoardMap screenLightBoardMap;
#endif
