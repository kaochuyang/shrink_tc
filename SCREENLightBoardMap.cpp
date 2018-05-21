//---------------------------------------------------------------------------
#include "SCREENLightBoardMap.h"
#include "screenDirLightBoardMappingMenu.h"
#include "SCREENMain.h"
#include "LCD240x128.h"
#include "SMEM.h"

#include "CDataToMessageOK.h"
#include "WRITEJOB.h"

#include "SCREENCtlSetup.h"

SCREENLightBoardMap screenLightBoardMap;
//---------------------------------------------------------------------------
SCREENLightBoardMap::SCREENLightBoardMap(void)
{
    LoadBitmapFromFile();
    InitDispWord();
    cPosition=0;
}
//---------------------------------------------------------------------------
SCREENLightBoardMap::~SCREENLightBoardMap(void)
{
}
//---------------------------------------------------------------------------
void SCREENLightBoardMap::DoKeyWork(BYTE key)              //已過濾,進來的是0x80~0x98之間,5X5按鍵
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
             DoKey3Work();
        break;
        case 0x84:
             DoKey4Work();
        break;
        case 0x85:
             DoKey5Work();
        break;
        case 0x86:
             DoKey6Work();
        break;
        case 0x87:
             DoKey7Work();
        break;

        case 0x90:
             DoKeyF1Work();
        break;
        case 0x91:
             DoKeyF2Work();
        break;
        case 0x92:
        break;
        case 0x93:
             DoKeyF4Work();
        break;
        case 0x94://UP
        case 0x95://DOWN
        break;
        case 0x96://LEFT
             DoKeyLEFTWork();
        break;
        case 0x97://RIGHT
             DoKeyRIGHTWork();
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
void SCREENLightBoardMap::LoadBitmapFromFile(void)
{
try {
    FILE *bitmap;
    bitmap=fopen("//cct//bitmap//backGround//LightBoardMapping.bit","rb");
    if (bitmap) {
        fread(ucDirMappingBitmap,3840,1,bitmap);
        fclose(bitmap);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENLightBoardMap::DisplayDirMapping(void)
{
try {
    smem.SetcFace(cLIGHTBOARDMAPPING);
    lcd240x128.DISPLAY_GRAPHIC(0,ucDirMappingBitmap,128,30);

    for (int x=0;x<6;x++) usiMappingValue[x]=smem.vGetSignamMapMappingLightBoard(x);

    for (int i=0;i<6;i++) lcd240x128.DISPLAY_GRAPHIC_XY(lane[i].X,lane[i].Y,word16x16[usiMappingValue[i]],lane[i].height,lane[i].width/8);

    cPosition=0;
    setCursor16x16(lane[cPosition].X,lane[cPosition].Y+16);

  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENLightBoardMap::InitDispWord(void)
{
try {
    for (int i=0;i<8;i++) {
         title[i].X=48+i*24;
         title[i].Y=36;
         title[i].width=16;
         title[i].height=16;
    }

    for (int i=0;i<8;i++) {
         lane[i].X=48+i*24;
         lane[i].Y=64;
         lane[i].width=16;
         lane[i].height=16;
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENLightBoardMap::DoKey0Work(void)
{
try {
        usiMappingValue[cPosition]=0;
        lcd240x128.DISPLAY_GRAPHIC_XY(lane[cPosition].X,lane[cPosition].Y,word16x16[0],lane[cPosition].height,lane[cPosition].width/8);
        DoKeyRIGHTWork();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENLightBoardMap::DoKey1Work(void)
{
try {
        usiMappingValue[cPosition]=1;
        lcd240x128.DISPLAY_GRAPHIC_XY(lane[cPosition].X,lane[cPosition].Y,word16x16[1],lane[cPosition].height,lane[cPosition].width/8);
        DoKeyRIGHTWork();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENLightBoardMap::DoKey2Work(void)
{
try {
        usiMappingValue[cPosition]=2;
        lcd240x128.DISPLAY_GRAPHIC_XY(lane[cPosition].X,lane[cPosition].Y,word16x16[2],lane[cPosition].height,lane[cPosition].width/8);
        DoKeyRIGHTWork();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENLightBoardMap::DoKey3Work(void)
{
try {
        usiMappingValue[cPosition]=3;
        lcd240x128.DISPLAY_GRAPHIC_XY(lane[cPosition].X,lane[cPosition].Y,word16x16[3],lane[cPosition].height,lane[cPosition].width/8);
        DoKeyRIGHTWork();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENLightBoardMap::DoKey4Work(void)
{
try {
        usiMappingValue[cPosition]=4;
        lcd240x128.DISPLAY_GRAPHIC_XY(lane[cPosition].X,lane[cPosition].Y,word16x16[4],lane[cPosition].height,lane[cPosition].width/8);
        DoKeyRIGHTWork();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENLightBoardMap::DoKey5Work(void)
{
try {
        usiMappingValue[cPosition]=5;
        lcd240x128.DISPLAY_GRAPHIC_XY(lane[cPosition].X,lane[cPosition].Y,word16x16[5],lane[cPosition].height,lane[cPosition].width/8);
        DoKeyRIGHTWork();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENLightBoardMap::DoKey6Work(void)
{
try {
//        usiMappingValue[cPosition]=6;
//        lcd240x128.DISPLAY_GRAPHIC_XY(lane[cPosition].X,lane[cPosition].Y,word16x16[6],lane[cPosition].height,lane[cPosition].width/8);
//        DoKeyRIGHTWork();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENLightBoardMap::DoKey7Work(void)
{
try {
//        usiMappingValue[cPosition]=7;
//        lcd240x128.DISPLAY_GRAPHIC_XY(lane[cPosition].X,lane[cPosition].Y,word16x16[7],lane[cPosition].height,lane[cPosition].width/8);
//        DoKeyRIGHTWork();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENLightBoardMap::DoKeyF1Work(void)
{
    screenMain.DisplayMain();
}
//---------------------------------------------------------------------------
void SCREENLightBoardMap::DoKeyF2Work(void)
{
try {

  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENLightBoardMap::DoKeyF4Work(void)
{
    screenDirLightBoardMappingMenu.DisplayDirLightBoardMappingMenu();
}
//---------------------------------------------------------------------------
void SCREENLightBoardMap::DoKeyEnterWork(void)
{
try {
    //OTMARKPRINTF  printf("%d:%d:%d:%d:%d:%d:%d:%d\n",usiMappingValue[0],usiMappingValue[1],usiMappingValue[2],usiMappingValue[3],usiMappingValue[4],usiMappingValue[5],usiMappingValue[6],usiMappingValue[7]);
      for(int i = 0; i < 6; i++) {
        smem.vSetSignamMapMappingLightBoard(i, usiMappingValue[i]);
      }

      //screenMain.DisplayMain();
      screenCtlSetup.DisplayCtlSetup();

  } catch (...) {}
}
//------------------------------------------------------------------------
void SCREENLightBoardMap::DoKeyLEFTWork(void)
{
try {
    clearCursor16x16(lane[cPosition].X,lane[cPosition].Y+16);
    if (cPosition==0) cPosition=1;
    cPosition--;
    setCursor16x16(lane[cPosition].X,lane[cPosition].Y+16);
  } catch (...) {}
}
//------------------------------------------------------------------------
void SCREENLightBoardMap::DoKeyRIGHTWork(void)
{
try {
        clearCursor16x16(lane[cPosition].X,lane[cPosition].Y+16);
        cPosition++;
        if (cPosition>=6) cPosition=5;
        setCursor16x16(lane[cPosition].X,lane[cPosition].Y+16);
  } catch (...) {}
}
//------------------------------------------------------------------------
