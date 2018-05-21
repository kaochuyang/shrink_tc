#ifndef screenSegtypeEditF2H
#define screenSegtypeEditF2H
//---------------------------------------------------------------------------
//OTADD
#include "SCREEN.h"
//---------------------------------------------------------------------------

    struct ExecTimeEditF2 {
      unsigned short int _hour;
      unsigned short int _minute;
      unsigned short int _planid;
      unsigned short int _actMode;
    };
    struct SegtypeInfoEditF2 {
      unsigned short int _segment_type;
      unsigned short int _segment_count;
      ExecTimeEditF2 _ptr_seg_exec_time[32];
    }; //重新定義,ExecTime定最大數

//---------------------------------------------------------------------------
class ScreenSegtypeEditF2:public SCREEN
{
    public:
      ScreenSegtypeEditF2(void);
      ~ScreenSegtypeEditF2(void);

      void DisplaySegtype(int);
      void vDisplaySegtypeFromPreview(int);
      void doKeyWork(BYTE);                                                     //根據KEY的值作事

    private:
      DISP_WORD dateWord[8];                                                    //顯示日期的位置
      DISP_WORD timeWord[4];                                                    //顯示時間的位置
      DISP_WORD itemWord[12];                                                   //顯示編號的位置
      DISP_WORD segmentWord[48];                                                //顯示時間表內容
      DISP_WORD segmnetNoWord[2];                                               //顯示時間表編號
      DISP_WORD segmnetCountWord[2];                                            //顯示時間表時段數
      int cPosition;
      int cSelect;                                                              //0:時間 1:日期 2:時間表內容 3:時間表時段數
      int cFace;                                                                //頁面1~6(每頁6段)

//OT Debug 0523
      BYTE ucSegmentWord[42];
      int iSegtypeEditing;
      bool vAppendSeg(void);

      void initDispWord(void);                                                  //初始化此畫面的座標等參數
      void DisplayDate(void);                                                   //顯示日期
      void DisplayTime(void);                                                   //顯示時間
      void DisplayItem(int);                                                    //顯示編號
      void DisplayUpAndDownPage(int);
      void DisplaySegmnetNo(void);                                              //
      void DisplaySegmnetCount(void);                                           //
      void DisplaySegmentActMode(void);//arwen ++ 1000930

      void LoadSegtype(int, bool);                                                    //從學勇那裡得到
      int GetNowPlanOfSegtypeCount(void);                                       //得到現在是一天中的第幾段
      void DisplaySegtypeTable(int);                                            //顯示在畫面上
      void SaveSegtype(void);                                                   //寫回學勇那裡

      SegtypeInfoEditF2 segment;

      BYTE segtypeBitmap[3840];                                                 //底圖
      void loadBitmapFromFile(void);                                            //將底圖Load進記憶體

      void vTmpArrayToData(void);
      void vDataToTmpArray(void);
      bool vCheckRationality(void);
      bool vCalSegCount(void);
      void vSaveSeg(bool);

      void doKey0Work(void);
      void doKey1Work(void);
      void doKey2Work(void);
      void doKey3Work(void);
      void doKey4Work(void);
      void doKey5Work(void);
      void doKey6Work(void);
      void doKey7Work(void);
      void doKey8Work(void);
      void doKey9Work(void);
      void doKeyAWork(void);
      void doKeyBWork(void);
      void doKeyCWork(void);
      void doKeyDWork(void);
      void doKeyEWork(void);
      void doKeyFWork(void);
      void doKeyF1Work(void);
      void doKeyF2Work(void);
      void doKeyF3Work(void);
      void doKeyF4Work(void);
      void doKeyUPWork(void);
      void doKeyDOWNWork(void);
      void doKeyLEFTWork(void);
      void doKeyRIGHTWork(void);
      void doKeyEnterWork(void);
      void doKeyDefaultWork(void);
};
//---------------------------------------------------------------------------
extern ScreenSegtypeEditF2 screenSegtypeEditF2;
#endif




