//---------------------------------------------------------------------------
#include "screenRunTableList.h"
#include "SCREENTFDStatus.h"
#include "SCREENLogo.h"
#include "SCREENMain.h"
#include "LCD240x128.h"
#include "SMEM.h"

SCREENTFDStatus screenTFDStatus;
//---------------------------------------------------------------------------
SCREENTFDStatus::SCREENTFDStatus(void)
{
    LoadBitmapFromFile();
    bTFDOK = false;
    bTFDMoudleFailure = false;
    bTFDTriggerFailure = false;
    bTFDCCTVFailure = false;
    bTFDDIOOutput = false;
    ClearAll();
}
//---------------------------------------------------------------------------
SCREENTFDStatus::~SCREENTFDStatus(void)
{
}
//---------------------------------------------------------------------------
void SCREENTFDStatus::DoKeyWork(BYTE key)                                            //已過濾,進來的是0x80~0x98之間,5X5按鍵
{
try {
    switch (key) {
        case 0x90:
             DoKeyF1Work();
        break;
        case 0x93:
             DoKeyF4Work();
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
void SCREENTFDStatus::LoadBitmapFromFile(void)
{
try {
    FILE *bitmap;
    bitmap=fopen("//cct//bitmap//backGround//screenTFDStatus.bit","rb");
   if (bitmap) {
        fread(TFDStatusBitmap,3840,1,bitmap);
        fclose(bitmap);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENTFDStatus::DisplayTFDStatus(void)
{
try {
    smem.SetcFace(cTFDSTATUS);
    smem.vWriteMsgToDOM("Enter TFD Status Screen");
    lcd240x128.DISPLAY_GRAPHIC(0,TFDStatusBitmap,128,30);

    ClearAll();
    DisplayTFDStatOK();
    DisplayTFDModuleFailure();
    DisplayTFDTriggerFailure();
    DisplayTFDCCTVFailure();
    DisplayTFDDIOOutput();

  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENTFDStatus::DisplayTFDStatOK(void)
{
try {
    if (bTFDOK) {
        setSelectPoint16x16(16, 32, 16, 16/8);
    } else {
        clearSelectPoint16x16(16, 32, 16, 16/8);
    }
  } catch (...) {}
}

//---------------------------------------------------------------------------
void SCREENTFDStatus::DisplayTFDModuleFailure(void)
{
try {
    if (bTFDMoudleFailure) {
        setSelectPoint16x16(16, 48, 16, 16/8);
    } else {
        clearSelectPoint16x16(16, 48, 16, 16/8);
    }
  } catch (...) {}
}

//---------------------------------------------------------------------------
void SCREENTFDStatus::DisplayTFDTriggerFailure(void)
{
try {
    if (bTFDTriggerFailure) {
        setSelectPoint16x16(16, 64, 16, 16/8);
    } else {
        clearSelectPoint16x16(16, 64, 16, 16/8);
    }
  } catch (...) {}
}

//---------------------------------------------------------------------------
void SCREENTFDStatus::DisplayTFDCCTVFailure(void)
{
try {
    if (bTFDCCTVFailure) {
        setSelectPoint16x16(16, 80, 16, 16/8);
    } else {
        clearSelectPoint16x16(16, 80, 16, 16/8);
    }
  } catch (...) {}
}

//---------------------------------------------------------------------------
void SCREENTFDStatus::DisplayTFDDIOOutput(void)
{
try {
    if (bTFDDIOOutput) {
        clearSelectPoint16x16(192, 32, 16, 16/8);
        setSelectPoint16x16(192, 48, 16, 16/8);
    } else {
        clearSelectPoint16x16(192, 48, 16, 16/8);
        setSelectPoint16x16(192, 32, 16, 16/8);
    }
  } catch (...) {}
}


//---------------------------------------------------------------------------
void SCREENTFDStatus::ClearAll(void)
{
try {
    clearSelectPoint16x16(16, 32, 16, 16/8);
    clearSelectPoint16x16(16, 48, 16, 16/8);
    clearSelectPoint16x16(16, 64, 16, 16/8);
    clearSelectPoint16x16(16, 80, 16, 16/8);
    clearSelectPoint16x16(192, 32, 16, 16/8);
    clearSelectPoint16x16(192, 48, 16, 16/8);
  } catch (...) {}
}

//---------------------------------------------------------------------------
void SCREENTFDStatus::DoKeyF1Work(void)
{
    screenLogo.DisplayLogo();
}

//---------------------------------------------------------------------------
void SCREENTFDStatus::DoKeyF4Work(void)
{
    screenRunTableList.DisplayRunTableList();
}
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
void SCREENTFDStatus::DoKeyEnterWork(void)
{
try {
//dbOperStat->0:允許現場操作,1:不允許現場操作,2:只允許現場查看
//requestResult->0:accept,1:唯讀,2不允許,3:Timeout

  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENTFDStatus::vChangeScreenTFDStatus(bool bDIO, unsigned char ucStatusCode, unsigned char ucVDZoneStatus)
{
try {
  bTFDDIOOutput = bDIO;

  if(ucStatusCode == 0x01) {
    bTFDOK = true;
    bTFDMoudleFailure = false;
    bTFDCCTVFailure = false;
  } else if(ucStatusCode == 0x02) {
    bTFDCCTVFailure = true;
    bTFDMoudleFailure = false;
    bTFDOK = false;
  } else if(ucStatusCode == 0x03) {
    bTFDMoudleFailure = true;
    bTFDOK = false;
    bTFDCCTVFailure = false;
  }

  if( smem.GetcFace() == cTFDSTATUS) {
    ClearAll();
    DisplayTFDStatOK();
    DisplayTFDModuleFailure();
    DisplayTFDTriggerFailure();
    DisplayTFDCCTVFailure();
    DisplayTFDDIOOutput();
  }



} catch(...) {}
}
