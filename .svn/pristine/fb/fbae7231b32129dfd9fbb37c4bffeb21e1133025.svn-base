#ifndef SCREENOnlineH
#define SCREENOnlineH

#include "SCREEN.h"
#include "var.h"
//---------------------------------------------------------------------------
class SCREENOnline:public SCREEN
{
    public:
        SCREENOnline(void);
        ~SCREENOnline(void);

        void DisplayOnline(void);
        void DoKeyWork(BYTE);                                                   //根據KEY的值作事


        void RequestAccept(void);
        void RequestReadOnly(void);
        void RequestNoAccept(void);
        void RequestTimeOut(void);


        void DisplayOperMode(void);


    private:

        unsigned short int dbOperStat;                                          //(0:允許現場操作,1:不允許現場操作,2:只允許現場查看)
        unsigned short int requestResult;                                       //0:accept,1:唯讀,2不允許,3:Timeout

        BYTE onlineBitmap[3840];                                                //空白底圖

        void LoadBitmapFromFile(void);
        void DisplayDBOperStat(void);
        void ClearAll(void);


        void DoKeyF1Work(void);
        void DoKeyEnterWork(void);
};
//---------------------------------------------------------------------------
extern SCREENOnline screenOnline;
#endif
