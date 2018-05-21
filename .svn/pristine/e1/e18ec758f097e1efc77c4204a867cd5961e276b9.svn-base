//---------------------------------------------------------------------------
#include "SCREENUpdateDB.h"
#include "SCREENMain.h"
#include "LCD240x128.h"
#include "SMEM.h"

#include "CDataToMessageOK.h"
#include "WRITEJOB.h"

#include "SCREENLast92TCPlanSegmentUpdate.h"

SCREENUpdateDB screenUpdateDB;
//---------------------------------------------------------------------------
SCREENUpdateDB::SCREENUpdateDB(void)
{
    LoadBitmapFromFile();
}
//---------------------------------------------------------------------------
SCREENUpdateDB::~SCREENUpdateDB(void)
{
}
//---------------------------------------------------------------------------
void SCREENUpdateDB::DoKeyWork(BYTE key)
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
void SCREENUpdateDB::LoadBitmapFromFile(void)
{
try {
    FILE *bitmap;
    bitmap=fopen("//cct//bitmap//backGround//4_5_updatedb.bit","rb");
    if (bitmap) {
        fread(updateDBBitmap,3840,1,bitmap);
        fclose(bitmap);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENUpdateDB::DisplayUpdateDB(void)
{
try {
    smem.SetcFace(cUPDATEDB);
    smem.vWriteMsgToDOM("Enter Update Database Screen");
    lcd240x128.DISPLAY_GRAPHIC(0,updateDBBitmap,128,30);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENUpdateDB::DoKeyF1Work(void)
{
    screenMain.DisplayMain();
}
//---------------------------------------------------------------------------
void SCREENUpdateDB::DoKeyF4Work(void)
{
    screenMain.DisplayMain();
}
//---------------------------------------------------------------------------
void SCREENUpdateDB::DoKeyEnterWork(void)
{
try {
    unsigned char data[5];

    data[0]  = 0x5F;
    data[1]  = 0x0B;
    data[2]  = 0x0F;   //要求全部

    MESSAGEOK _MsgOK;

    _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, 3,true);
    _MsgOK.InnerOrOutWard = cComingFromScreen;
//    writeJob.WriteWorkByMESSAGEOUT(_MsgOK);
    writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICECENTER92);

    smem.vWriteMsgToDOM("Request Traffic Center To Update My Database");

//    screenMain.DisplayMain();
    screenLast92TCPlanSegmentUpdate.DisplayLastUpdate();

  } catch (...) {}
}
//---------------------------------------------------------------------------
