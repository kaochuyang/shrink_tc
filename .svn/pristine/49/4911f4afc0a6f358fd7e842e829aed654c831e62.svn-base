#include "SCREENDBSetup.h"
#include "SCREENMain.h"
//#include "SCREENCtlSetup.h"
#include "SCREENLockdb.h"
#include "SCREENABOErr.h"
#include "SCREENV3Related.h"

#include "LCD240x128.h"
#include "SMEM.h"

SCREENDBSetup screenDBSetup;
//---------------------------------------------------------------------------
SCREENDBSetup::SCREENDBSetup(void)
{
try {
    LoadBitmapFromFile();
    InitDispWord();
    cSelect=0;
  } catch (...) {}
}
//---------------------------------------------------------------------------
SCREENDBSetup::~SCREENDBSetup(void)
{
}
//---------------------------------------------------------------------------
void SCREENDBSetup::DoKeyWork(BYTE key)                                         //已過濾,進來的是0x80~0x98之間
{
try {
    switch (key) {
        case 0x80:
          DoKey0Work();
        break;
        case 0x81:
          DoKey1Work();
        break;
        case 0x82:
          DoKey2Work();
        break;
        case 0x83:
        break;
        case 0x84:
        case 0x85:
        case 0x86:
        case 0x87:
        case 0x88:
        case 0x89:
        case 0x8A:
        case 0x8B:
        case 0x8C:
        case 0x8D:
        case 0x8E:
        case 0x8F:
        break;
        case 0x90://F1 ESC
          DoKeyF1Work();
        break;
        case 0x91:
        case 0x92:
        break;
        case 0x93://F4
          DoKeyF4Work();
        break;
        case 0x94:
          DoKeyUPWork();
        break;
        case 0x95:
          DoKeyDOWNWork();
        break;
        case 0x96:
          DoKeyLEFTWork();
        break;
        case 0x97:
          DoKeyRIGHTWork();
        break;
        case 0x98://ENTER
          DoKeyEnterWork();
        break;
        default:
        break;
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENDBSetup::LoadBitmapFromFile(void)
{
try {
    FILE *bitmap;
    bitmap=fopen("//cct//bitmap//backGround//4_1_7.bit","rb");
    if (bitmap) {
        fread(dbSetupBitmap,3840,1,bitmap);
        fclose(bitmap);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENDBSetup::DisplayDBSetup(void)
{
try {
    smem.SetcFace(cDBSETUP);
    smem.vWriteMsgToDOM("Enter Database Setup Screen");
    for (int i=0;i<90;i++) dbSetupBitmap[i]=0;                        //這裡總是有黑點,手動清除
    lcd240x128.DISPLAY_GRAPHIC(0,dbSetupBitmap,128,30);

    int temp=smem.GetDbOperStat();  //(0:允許現場操作,1:不允許現場操作,2:只允許現場查看)
    if (temp==0)  cSelect=0;
    else if (temp==1)  cSelect=1;
    else if (temp==2)  cSelect=2;
    else cSelect=99;

    if (cSelect!=99) {
        setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(db.X,db.Y,word24x24[cSelect],db.height,db.width/8);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENDBSetup::InitDispWord(void)
{
try {
    db.X=136;
    db.Y=5;
    db.width=24;
    db.height=24;

    for (int i=0;i<5;i++) {
         mark[i].X=24;
         mark[i].Y=36+i*20;
         mark[i].width=16;
         mark[i].height=16;
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENDBSetup::DoKey0Work(void)
{
    if (cSelect<3) clearSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    cSelect=0;
    setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(db.X,db.Y,word24x24[cSelect],db.height,db.width/8);
}
//---------------------------------------------------------------------------
void SCREENDBSetup::DoKey1Work(void)
{
    if (cSelect<3) clearSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    cSelect=1;
    setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(db.X,db.Y,word24x24[cSelect],db.height,db.width/8);
}
//---------------------------------------------------------------------------
void SCREENDBSetup::DoKey2Work(void)
{
    if (cSelect<3) clearSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    cSelect=2;
    setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(db.X,db.Y,word24x24[cSelect],db.height,db.width/8);
}
//---------------------------------------------------------------------------
void SCREENDBSetup::DoKeyF1Work(void)
{
    screenMain.DisplayMain();
}
//---------------------------------------------------------------------------
void SCREENDBSetup::DoKeyF4Work(void)
{
//arwem modify    screenCtlSetup.DisplayCtlSetup();
    screenV3Related.DisplayV3Related();
}
//---------------------------------------------------------------------------
void SCREENDBSetup::DoKeyEnterWork(void)
{
try {
    if (smem.GetDbOperStat()==0 || smem.vLoadCenterConnectStatus()==false) {
        if (smem.vGet0FCommandSet()>2) {
            if (cSelect<3) {

                if (cSelect==0)  smem.vWriteMsgToDOM("Accept To Modify Data");
                else if (cSelect==1)  smem.vWriteMsgToDOM("Reject To Modify Data");
                else if (cSelect==2)  smem.vWriteMsgToDOM("Accpet To Read Data Only");

                smem.SetDbOperStat(cSelect);
//arwen modify                screenCtlSetup.DisplayCtlSetup();
                screenV3Related.DisplayV3Related();
            }
        } else {
            smem.SetLastFace(cDBSETUP);
            screenABOErr.DisplayABOErr();
        }
    } else {
        smem.SetLastFace(cDBSETUP);
        screenLockdb.DisplayLockdb();
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENDBSetup::DoKeyUPWork(void)
{
try {
    if (cSelect<3) clearSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    if (cSelect==0 || cSelect>=3) cSelect=1;
    cSelect--;
    setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(db.X,db.Y,word24x24[cSelect],db.height,db.width/8);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENDBSetup::DoKeyDOWNWork(void)
{
try {
    if (cSelect<3) clearSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    cSelect++;
    if (cSelect>=2) cSelect=2;
    setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(db.X,db.Y,word24x24[cSelect],db.height,db.width/8);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENDBSetup::DoKeyLEFTWork(void)
{
}
//---------------------------------------------------------------------------
void SCREENDBSetup::DoKeyRIGHTWork(void)
{
}
//---------------------------------------------------------------------------
