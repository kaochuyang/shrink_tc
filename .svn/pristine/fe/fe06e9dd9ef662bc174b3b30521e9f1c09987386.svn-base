#include "screenChainStatus.h"
#include "LCD240x128.h"
//#include "screenLogo.h"
#include "SCREENMain.h"
#include "screenRunTableList.h"
#include "CSTC.h"

#include <stdio.h>

#include "SMEM.h"
//---------------------------------------------------------------------------
ScreenChainStatus screenChainStatus;
//---------------------------------------------------------------------------
ScreenChainStatus::ScreenChainStatus(void)
{
    loadBitmapFromFile();
    initDispWord();
    cSelect=0;
}
//---------------------------------------------------------------------------
ScreenChainStatus::~ScreenChainStatus(void)
{
}
//---------------------------------------------------------------------------
void ScreenChainStatus::loadBitmapFromFile(void)
{
try {
    FILE *bitmap;
    bitmap=fopen("//cct//bitmap//backGround//TC5F//ChainStatus.bit","rb");
    if (bitmap) {
        fread(ucChainStatusBitmap,3840,1,bitmap);
        fclose(bitmap);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenChainStatus::DisplayChainStatus(void)
{
try {
    smem.SetcFace(cCHAINSTATUS);
    lcd240x128.DISPLAY_GRAPHIC(0,ucChainStatusBitmap,128,30);

    vRefreshChainStatusData();

  } catch (...) {}
}

//---------------------------------------------------------------------------
void ScreenChainStatus::vRefreshChainStatusData(void)
{
try {
  bool bTmp;
  unsigned char ucMBType;
  unsigned char ucMotherOutputEnable = 0;
  unsigned char ucMotherOutputSignal = 0;
  unsigned char ucChildInputEnable = 0;
  unsigned char ucChildInputSignal = 0;
  unsigned char ucChildInputReasonable = 0;
  unsigned char ucChildCycle[3];
  sChildChain sCCTMP;
  ucChildCycle[0] = 0;   ucChildCycle[1] = 0;   ucChildCycle[2] = 0;

  unsigned char ucTmp1, ucTmp2;
  switch  (stc.Lock_to_LoadControlStrategy()) {
    case STRATEGY_TOD:
         ucMotherOutputEnable = smem.vGetUCData(TC92_5F31TOD);
    break;
    case STRATEGY_MANUAL:
         ucMotherOutputEnable = smem.vGetUCData(TC92_5F31Manual);
    break;
    default:
    break;
  }
  lcd240x128.DISPLAY_GRAPHIC_XY(_MotherOutputEnable.X,_MotherOutputEnable.Y,word8x16[ucMotherOutputEnable],_MotherOutputEnable.height,_MotherOutputEnable.width/8);

//MotherOutput
  ucMotherOutputSignal = smem.vGetWriteDIO();
  //reverse for view
  if (ucMotherOutputSignal) ucMotherOutputSignal = 0;
  else { ucMotherOutputSignal = 1; }
  lcd240x128.DISPLAY_GRAPHIC_XY(_MotherOutputSignal.X,_MotherOutputSignal.Y,word8x16[ucMotherOutputSignal],_MotherOutputSignal.height,_MotherOutputSignal.width/8);

  ucTmp1 = smem.vGetUCData(TC92_5F32StartSubPhaseId);
  ucTmp2 = smem.vGetUCData(TC92_5F32EndSubPhaseId);
  if(ucTmp1 != 0 && ucTmp2 != 0) {
    ucChildInputEnable = 1;
  }
  lcd240x128.DISPLAY_GRAPHIC_XY(_ChildInputEnable.X,_ChildInputEnable.Y,word8x16[ucChildInputEnable],_ChildInputEnable.height,_ChildInputEnable.width/8);

  DATA_Bit tempDIOByte=smem.Get_DIOByte();

  ucMBType = smem.vGetMotherBoardType();
  if(ucMBType == MB_ICOP6115) {
    bTmp = tempDIOByte.switchBit.b2;
  } else {
    bTmp = tempDIOByte.switchBit.b1;
  }


//  if(tempDIOByte.switchBit.b1 == TC_CHAIN_SIGNAL_IN_START_SUB_PAHSE) {          //get StartLock Signal
  if(bTmp == TC_CHAIN_SIGNAL_IN_START_SUB_PAHSE) {          //get StartLock Signal
    ucChildInputSignal = 1;
  }
  lcd240x128.DISPLAY_GRAPHIC_XY(_ChildInputSignal.X,_ChildInputSignal.Y,word8x16[ucChildInputSignal],_ChildInputSignal.height,_ChildInputSignal.width/8);

  sCCTMP = smem.vGetChildChainStruct();
  if(sCCTMP.bHaveReasonableChildChainSignal == true) { ucChildInputReasonable = 1; }
  lcd240x128.DISPLAY_GRAPHIC_XY(_ChildInputReasonable.X,_ChildInputReasonable.Y,word8x16[ucChildInputReasonable],_ChildInputReasonable.height,_ChildInputReasonable.width/8);

  ucChildCycle[0] = sCCTMP.iChainCycle/100;
  ucChildCycle[1] = (sCCTMP.iChainCycle/10)%10;
  ucChildCycle[2] = sCCTMP.iChainCycle%10;;
  for(int i = 0; i < 3; i++) {
    lcd240x128.DISPLAY_GRAPHIC_XY(_ChildCycle[i].X,_ChildCycle[i].Y,word8x16[ucChildCycle[i]],_ChildCycle[i].height,_ChildCycle[i].width/8);
  }

} catch(...){}
}

//---------------------------------------------------------------------------
void ScreenChainStatus::initDispWord(void)
{
try {
  _MotherOutputEnable.X =80;
  _MotherOutputEnable.Y =32;
  _MotherOutputEnable.width = 8;
  _MotherOutputEnable.height = 16;

  _MotherOutputSignal.X =168;
  _MotherOutputSignal.Y =32;
  _MotherOutputSignal.width = 8;
  _MotherOutputSignal.height = 16;

  _ChildInputEnable.X =80;
  _ChildInputEnable.Y =56;
  _ChildInputEnable.width = 8;
  _ChildInputEnable.height = 16;

  _ChildInputSignal.X =168;
  _ChildInputSignal.Y =56;
  _ChildInputSignal.width = 8;
  _ChildInputSignal.height = 16;

  _ChildInputReasonable.X =80;
  _ChildInputReasonable.Y =72;
  _ChildInputReasonable.width = 8;
  _ChildInputReasonable.height = 16;

  for(int i = 0; i < 3; i++) {
    _ChildCycle[i].X = 144 + i*8;
    _ChildCycle[i].Y = 72;
    _ChildCycle[i].width = 8;
    _ChildCycle[i].height = 16;
  }


  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenChainStatus::doKeyWork(BYTE key)
{
try {
    switch (key) {
        case 0x80:
          doKey0Work();
        break;
        case 0x81:
          doKey1Work();
        break;
        case 0x82:
          doKey2Work();
        break;
        case 0x83:
          doKey3Work();
        break;
        case 0x84:
          doKey4Work();
        break;
        case 0x85:
          doKey5Work();
        break;
        case 0x86:
          doKey6Work();
        break;
        case 0x87:
          doKey7Work();
        break;
        case 0x88:
          doKey8Work();
        break;
        case 0x89:
          doKey9Work();
        break;
        case 0x8A:
          doKeyAWork();
        break;
        case 0x8B:
          doKeyBWork();
        break;
        case 0x8C:
          doKeyCWork();
        break;
        case 0x8D:
          doKeyDWork();
        break;
        case 0x8E:
          doKeyEWork();
        break;
        case 0x8F:
          doKeyFWork();
        break;
        case 0x90:
          doKeyF1Work();
        break;
        case 0x91:
          doKeyF2Work();
        break;
        case 0x92:
          doKeyF3Work();
        break;
        case 0x93:
          doKeyF4Work();
        break;
        case 0x94:
          doKeyUPWork();
        break;
        case 0x95:
          doKeyDOWNWork();
        break;
        case 0x96:
          doKeyLEFTWork();
        break;
        case 0x97:
          doKeyRIGHTWork();
        break;
        case 0x98:
          doKeyEnterWork();
        break;
        default:
        break;
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenChainStatus::doKey0Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenChainStatus::doKey1Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenChainStatus::doKey2Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenChainStatus::doKey3Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenChainStatus::doKey4Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenChainStatus::doKey5Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenChainStatus::doKey6Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenChainStatus::doKey7Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenChainStatus::doKey8Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenChainStatus::doKey9Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenChainStatus::doKeyAWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenChainStatus::doKeyBWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenChainStatus::doKeyCWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenChainStatus::doKeyDWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenChainStatus::doKeyEWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenChainStatus::doKeyFWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenChainStatus::doKeyF1Work(void)
{
//    screenLogo.DisplayLogo();
    screenMain.DisplayMain();
}
//---------------------------------------------------------------------------
void ScreenChainStatus::doKeyF2Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenChainStatus::doKeyF3Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenChainStatus::doKeyF4Work(void)
{
//    screenMain.DisplayMain();
    screenRunTableList.DisplayRunTableList();
}
//---------------------------------------------------------------------------
void ScreenChainStatus::doKeyUPWork(void)
{
try {
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenChainStatus::doKeyDOWNWork(void)
{
try {
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenChainStatus::doKeyLEFTWork(void)
{
try {
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenChainStatus::doKeyRIGHTWork(void)
{
try {
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenChainStatus::doKeyEnterWork(void)
{
    switch (cSelect+1) {
        case 1:
          doKey1Work();
        break;
        case 2:
          doKey2Work();
        break;
        case 3:
          doKey3Work();
        break;
        case 4:
          doKey4Work();
        break;
        case 5:
          doKey5Work();
        break;
        case 6:
          doKey6Work();
        break;
        case 7:
          doKey7Work();
        break;
        case 8:
          doKey8Work();
        break;
        case 9:
          doKey9Work();
        break;
        case 10:
          doKey0Work();
        break;
        default:
        break;
    }
}
//---------------------------------------------------------------------------

