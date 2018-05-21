#include "SCREENCtlNo.h"
#include "SCREENMain.h"
#include "SCREENCtlSetup.h"
#include "LCD240x128.h"
#include "SMEM.h"
#include "SCREENABOErr.h"
#include "SCREENLcnEncode.h"
#include "SCREENHWReset.h"
#include "SCREENV3Related.h"

#include <stdlib.h>

SCREENCtlNo screenCtlNo;
//---------------------------------------------------------------------------
SCREENCtlNo::SCREENCtlNo(void)
{
try{
    for (int i=0;i<5;i++) LCNaddress[i]='0'; //jacky20140312
    cPosition=0;                            //jacky20140312
    LoadBitmapFromFile();
    InitDispWord();
   } catch (...) {}
}
//---------------------------------------------------------------------------
SCREENCtlNo::~SCREENCtlNo(void)
{
}
//---------------------------------------------------------------------------
void SCREENCtlNo::DoKeyWork(BYTE key)                                           //¤w¹LÂo,¶i¨Óªº¬O0x80~0x98¤§¶¡
{
try {
    switch (key) {
        case 0x80:
            if(smem.GetV3orBRT() == 1)
                DoKey0Work();
        break;
        case 0x81:
          if(smem.GetV3orBRT() == 1)
                DoKey1Work();
        break;
        case 0x82:
          if(smem.GetV3orBRT() == 1)
                DoKey2Work();
        break;
        case 0x83:
          if(smem.GetV3orBRT() == 1)
                DoKey3Work();
        break;
        case 0x84:
         if(smem.GetV3orBRT() == 1)
                DoKey4Work();
        break;
        case 0x85:
          if(smem.GetV3orBRT() == 1)
                DoKey5Work();
        break;
        case 0x86:
          if(smem.GetV3orBRT() == 1)
                DoKey6Work();
        break;
        case 0x87:
          if(smem.GetV3orBRT() == 1)
                DoKey7Work();
        break;
        case 0x88:
          if(smem.GetV3orBRT() == 1)
                DoKey8Work();
        break;
        case 0x89:
          if(smem.GetV3orBRT() == 1)
                DoKey9Work();
        break;
        case 0x8A:
        case 0x8B:
        case 0x8C:
        case 0x8D:
        case 0x8E:
        case 0x8F:
        case 0x92://F3
        break;
        case 0x90://F1,ESC
          DoKeyF1Work();
        break;
        case 0x91://F2
          DoKeyF2Work();
        break;
        case 0x93://F4
          DoKeyF4Work();
        break;
        case 0x94://UP
            if(smem.GetV3orBRT() == 1)
                DoKeyUPWork();
        break;
        case 0x95://DOWN
            if(smem.GetV3orBRT() == 1)
                DoKeyDOWNWork();
        break;
        case 0x96://LEFT
            if(smem.GetV3orBRT() == 1)
                DoKeyLEFTWork();
        break;
        case 0x97://RIGHT
            if(smem.GetV3orBRT() == 1)
                DoKeyRIGHTWork();
        break;
        case 0x98://Enter
            if(smem.GetV3orBRT() == 1)
                DoKeyEnterWork();
        break;
        default:
        break;
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENCtlNo::LoadBitmapFromFile(void)
{
try {
    FILE *bitmap;
    bitmap=fopen("//cct//bitmap//backGround//4_1_1.bit","rb");
    if (bitmap) {
        fread(ctlNoBitmap,3840,1,bitmap);
        fclose(bitmap);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENCtlNo::DisplayCtlNo(void)
{
try {
    if (smem.vGet0FCommandSet()>2) {
        smem.SetcFace(cCTLNO);
        smem.vWriteMsgToDOM("Enter Controller Number Screen");
        lcd240x128.DISPLAY_GRAPHIC(0,ctlNoBitmap,128,30);
        DisplayNum();
    } else {
        smem.SetLastFace(cCTLSETUP);
        screenABOErr.DisplayABOErr();
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENCtlNo::InitDispWord(void)
{
    for (int i=0;i<5;i++) {
         lcn[i].X=64+i*24;
         lcn[i].Y=60;
         lcn[i].width=24;
         lcn[i].height=24;
    }
}
//---------------------------------------------------------------------------
void SCREENCtlNo::DisplayNum(void)
{
try {
    address=smem.GetAddress();
    if (smem.GetcFace()==cCTLNO) {
        lcd240x128.DISPLAY_GRAPHIC_XY(lcn[0].X,lcn[0].Y,word24x24[address/10000],lcn[0].height,lcn[0].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(lcn[1].X,lcn[1].Y,word24x24[(address/1000)%10],lcn[1].height,lcn[1].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(lcn[2].X,lcn[2].Y,word24x24[(address/100)%10],lcn[2].height,lcn[2].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(lcn[3].X,lcn[3].Y,word24x24[(address/10)%10],lcn[3].height,lcn[3].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(lcn[4].X,lcn[4].Y,word24x24[address%10],lcn[4].height,lcn[4].width/8);
        //jacky20140312
        if(address != 0)
            sprintf(LCNaddress,"%d", address);
        else {}
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENCtlNo::DoKeyF1Work(void)
{
    screenMain.DisplayMain();
}
//---------------------------------------------------------------------------
void SCREENCtlNo::DoKeyF2Work(void)
{
    screenLcnEncode.DisplayLcnEncode();
}
//---------------------------------------------------------------------------
void SCREENCtlNo::DoKeyF4Work(void)
{
    //screenCtlSetup.DisplayCtlSetup();
    screenV3Related.DisplayV3Related(); //V3相關設定
}
//----------jacky 20140311-----------------------------------------------------------------
void SCREENCtlNo::DoKey0Work(void)
{
try {
    lcd240x128.DISPLAY_GRAPHIC_XY(lcn[cPosition].X,lcn[cPosition].Y,word24x24[0],lcn[cPosition].height,lcn[cPosition].width/8);
    LCNaddress[cPosition]='0';
    DoKeyRIGHTWork();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENCtlNo::DoKey1Work(void)
{
try {
    lcd240x128.DISPLAY_GRAPHIC_XY(lcn[cPosition].X,lcn[cPosition].Y,word24x24[1],lcn[cPosition].height,lcn[cPosition].width/8);
    LCNaddress[cPosition]='1';
    DoKeyRIGHTWork();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENCtlNo::DoKey2Work(void)
{
try {
    lcd240x128.DISPLAY_GRAPHIC_XY(lcn[cPosition].X,lcn[cPosition].Y,word24x24[2],lcn[cPosition].height,lcn[cPosition].width/8);
    LCNaddress[cPosition]='2';
    DoKeyRIGHTWork();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENCtlNo::DoKey3Work(void)
{
try {
    lcd240x128.DISPLAY_GRAPHIC_XY(lcn[cPosition].X,lcn[cPosition].Y,word24x24[3],lcn[cPosition].height,lcn[cPosition].width/8);
    LCNaddress[cPosition]='3';
    DoKeyRIGHTWork();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENCtlNo::DoKey4Work(void)
{
try {
    lcd240x128.DISPLAY_GRAPHIC_XY(lcn[cPosition].X,lcn[cPosition].Y,word24x24[4],lcn[cPosition].height,lcn[cPosition].width/8);
    LCNaddress[cPosition]='4';
    DoKeyRIGHTWork();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENCtlNo::DoKey5Work(void)
{
try {
    lcd240x128.DISPLAY_GRAPHIC_XY(lcn[cPosition].X,lcn[cPosition].Y,word24x24[5],lcn[cPosition].height,lcn[cPosition].width/8);
    LCNaddress[cPosition]='5';
    DoKeyRIGHTWork();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENCtlNo::DoKey6Work(void)
{
try {
    lcd240x128.DISPLAY_GRAPHIC_XY(lcn[cPosition].X,lcn[cPosition].Y,word24x24[6],lcn[cPosition].height,lcn[cPosition].width/8);
    LCNaddress[cPosition]='6';
    DoKeyRIGHTWork();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENCtlNo::DoKey7Work(void)
{
try {
    lcd240x128.DISPLAY_GRAPHIC_XY(lcn[cPosition].X,lcn[cPosition].Y,word24x24[7],lcn[cPosition].height,lcn[cPosition].width/8);
    LCNaddress[cPosition]='7';
    DoKeyRIGHTWork();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENCtlNo::DoKey8Work(void)
{
try {
    lcd240x128.DISPLAY_GRAPHIC_XY(lcn[cPosition].X,lcn[cPosition].Y,word24x24[8],lcn[cPosition].height,lcn[cPosition].width/8);
    LCNaddress[cPosition]='8';
    DoKeyRIGHTWork();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENCtlNo::DoKey9Work(void)
{
try {
    lcd240x128.DISPLAY_GRAPHIC_XY(lcn[cPosition].X,lcn[cPosition].Y,word24x24[9],lcn[cPosition].height,lcn[cPosition].width/8);
    LCNaddress[cPosition]='9';
    DoKeyRIGHTWork();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENCtlNo::DoKeyUPWork(void)
{
    DoKeyLEFTWork();
}
//---------------------------------------------------------------------------
void SCREENCtlNo::DoKeyDOWNWork(void)
{
    DoKeyRIGHTWork();
}
//---------------------------------------------------------------------------
void SCREENCtlNo::DoKeyLEFTWork(void)
{
try {
    clearCursor24x24(lcn[cPosition].X,lcn[cPosition].Y+24);
    if (cPosition==0) cPosition=1;
    cPosition--;
    setCursor24x24(lcn[cPosition].X,lcn[cPosition].Y+24);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENCtlNo::DoKeyRIGHTWork(void)
{
try {
    clearCursor24x24(lcn[cPosition].X,lcn[cPosition].Y+24);
    cPosition++;
    if (cPosition>=5) cPosition=4;
    setCursor24x24(lcn[cPosition].X,lcn[cPosition].Y+24);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENCtlNo::DoKeyEnterWork(void)       //save LCNNO
{
try {
    FILE *lcnNO;
    tmpaddress = atoi(LCNaddress);
    if(tmpaddress != address){
        lcnNO = fopen("/cct/Data/SETTING/LCN.txt","w");
        if(lcnNO == NULL){
            printf("open LCN.txt error\n");
        }
        else fprintf(lcnNO,"enable=1\nLCN=%5s",LCNaddress);
        fclose(lcnNO);
        smem.vReadLCNFromDisk();
    }
    else {}
  } catch (...) {}
}
