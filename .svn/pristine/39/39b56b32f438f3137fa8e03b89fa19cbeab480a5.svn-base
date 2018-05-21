//---------------------------------------------------------------------------
#include "SCREENOnline.h"
#include "SCREENLogo.h"
#include "SCREENMain.h"
#include "LCD240x128.h"
#include "SMEM.h"

SCREENOnline screenOnline;
//---------------------------------------------------------------------------
SCREENOnline::SCREENOnline(void)
{
    LoadBitmapFromFile();
}
//---------------------------------------------------------------------------
SCREENOnline::~SCREENOnline(void)
{
}
//---------------------------------------------------------------------------
void SCREENOnline::DoKeyWork(BYTE key)                                            //已過濾,進來的是0x80~0x98之間,5X5按鍵
{
try {
    switch (key) {
        case 0x90:
             DoKeyF1Work();
        break;
        case 0x98:
             DoKeyEnterWork();
        break;
        default:
        break;
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENOnline::LoadBitmapFromFile(void)
{
try {
    FILE *bitmap;
    bitmap=fopen("//cct//bitmap//backGround//2_password_online.bit","rb");
   if (bitmap) {
        fread(onlineBitmap,3840,1,bitmap);
        fclose(bitmap);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENOnline::DisplayOnline(void)
{
try {
    smem.SetcFace(cONLINE);
    smem.vWriteMsgToDOM("Enter Online Screen");
    lcd240x128.DISPLAY_GRAPHIC(0,onlineBitmap,128,30);
    setSelectPoint16x16(16,25,16,16/8);       //確定的
    DisplayDBOperStat();
    DisplayOperMode();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENOnline::DisplayDBOperStat(void)
{
try {
    dbOperStat=smem.GetDbOperStat();
    requestResult=INVAILDVALUE;
    if (dbOperStat==0) {                                                        //允許,可直接進入
        clearSelectPoint16x16(16,45,16,16/8);
        setSelectPoint16x16(16,65,16,16/8);
        clearSelectPoint16x16(16,85,16,16/8);
    } else if (dbOperStat==1) {                                                 //不允許,送協定回去中心要求
            smem.vSet0F16LockDBRequestStatus(true);
            setSelectPoint16x16(16,45,16,16/8);
            clearSelectPoint16x16(16,65,16,16/8);
            clearSelectPoint16x16(16,85,16,16/8);
    } else if (dbOperStat==2) {                                                 //允許僅查看
            clearSelectPoint16x16(16,45,16,16/8);
            clearSelectPoint16x16(16,65,16,16/8);
            setSelectPoint16x16(16,85,16,16/8);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENOnline::DisplayOperMode(void)
{
try {
    if (smem.GetcFace()==cONLINE) {
        //遠端操作為1,現場操作為0
        if (smem.GetOperMode()==0) setSelectPoint16x16(16,105,16,16/8);
        else clearSelectPoint16x16(16,105,16,16/8);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENOnline::ClearAll(void)
{
try {
    clearSelectPoint16x16(16,45,16,16/8);
    clearSelectPoint16x16(16,65,16,16/8);
    clearSelectPoint16x16(16,85,16,16/8);
    clearSelectPoint16x16(168,70,16,16/8);
    clearSelectPoint16x16(200,70,16,16/8);
    clearSelectPoint16x16(184,22,16,16/8);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENOnline::RequestAccept(void)
{
    if (smem.GetcFace()==cONLINE) {
        ClearAll();
        dbOperStat=0;
        setSelectPoint16x16(16,65,16,16/8);     //現場操作
        requestResult=0;
        setSelectPoint16x16(200,70,16,16/8);
    }
}
//---------------------------------------------------------------------------
void SCREENOnline::RequestReadOnly(void)
{
    if (smem.GetcFace()==cONLINE) {
        ClearAll();
        dbOperStat=2;
        setSelectPoint16x16(16,85,16,16/8);     //唯讀
        requestResult=1;
        setSelectPoint16x16(200,70,16,16/8);
    }
}
//---------------------------------------------------------------------------
void SCREENOnline::RequestNoAccept(void)
{
    if (smem.GetcFace()==cONLINE) {
        ClearAll();
        dbOperStat=1;
        setSelectPoint16x16(16,45,16,16/8);
        requestResult=2;
        setSelectPoint16x16(184,22,16,16/8);
    }
}
//---------------------------------------------------------------------------
void SCREENOnline::RequestTimeOut(void)
{
    if (smem.GetcFace()==cONLINE) {
        ClearAll();
        dbOperStat=1;
        setSelectPoint16x16(16,45,16,16/8);
        requestResult=3;
        setSelectPoint16x16(168,70,16,16/8);    //TimeOut
    }
}
//---------------------------------------------------------------------------
void SCREENOnline::DoKeyF1Work(void)
{
    screenLogo.DisplayLogo();
}
//---------------------------------------------------------------------------
void SCREENOnline::DoKeyEnterWork(void)
{
try {
//dbOperStat->0:允許現場操作,1:不允許現場操作,2:只允許現場查看
//requestResult->0:accept,1:唯讀,2不允許,3:Timeout

    if (smem.GetOperMode()==0) {                                                //切到現場操作模式
        if (smem.vLoadCenterConnectStatus()==false) {                           //斷線時直接進入
            screenMain.DisplayMain();
        } else {

             if (dbOperStat==0 || dbOperStat==2) {                              //如果是允許操作或只允許查看
                 screenMain.DisplayMain();
             } else if (dbOperStat==1) {                                        //禁止操作,向中心request
                        if (requestResult==0 || requestResult==1)  screenMain.DisplayMain();
             }
        }
    }

  } catch (...) {}
}
//---------------------------------------------------------------------------
