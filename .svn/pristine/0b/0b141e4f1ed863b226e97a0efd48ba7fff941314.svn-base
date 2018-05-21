//---------------------------------------------------------------------------
#include "SCREENDirMap.h"
#include "SCREENMain.h"
#include "LCD240x128.h"
#include "SMEM.h"

#include "CDataToMessageOK.h"
#include "WRITEJOB.h"

#include "screenDirLightBoardMappingMenu.h"

#include "SCREENCtlSetup.h"
SCREENDirMap screenDirMap;
//---------------------------------------------------------------------------
SCREENDirMap::SCREENDirMap(void)
{
    LoadBitmapFromFile();
    InitDispWord();
    cPosition=0;
}
//---------------------------------------------------------------------------
SCREENDirMap::~SCREENDirMap(void)
{
}
//---------------------------------------------------------------------------
void SCREENDirMap::DoKeyWork(BYTE key)              //已過濾,進來的是0x80~0x98之間,5X5按鍵
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
void SCREENDirMap::LoadBitmapFromFile(void)
{
try {
    FILE *bitmap;
    bitmap=fopen("//cct//bitmap//backGround//DirMapping.bit","rb");
    if (bitmap) {
        fread(ucDirMappingBitmap,3840,1,bitmap);
        fclose(bitmap);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENDirMap::DisplayDirMapping(void)
{
try {
    smem.SetcFace(cDIRMAPPING);
//    smem.vWriteMsgToDOM("Enter DIR Screen");
    lcd240x128.DISPLAY_GRAPHIC(0,ucDirMappingBitmap,128,30);

    for (int x=0;x<8;x++) usiMappingValue[x]=smem.vGetSignamMapMappingDir(x);

    for (int i=0;i<8;i++) lcd240x128.DISPLAY_GRAPHIC_XY(lane[i].X,lane[i].Y,word16x16[usiMappingValue[i]],lane[i].height,lane[i].width/8);

    cPosition=0;
    setCursor16x16(lane[cPosition].X,lane[cPosition].Y+16);

  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENDirMap::InitDispWord(void)
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
void SCREENDirMap::DoKey0Work(void)
{
try {
        usiMappingValue[cPosition]=0;
        lcd240x128.DISPLAY_GRAPHIC_XY(lane[cPosition].X,lane[cPosition].Y,word16x16[0],lane[cPosition].height,lane[cPosition].width/8);
        DoKeyRIGHTWork();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENDirMap::DoKey1Work(void)
{
try {
        usiMappingValue[cPosition]=1;
        lcd240x128.DISPLAY_GRAPHIC_XY(lane[cPosition].X,lane[cPosition].Y,word16x16[1],lane[cPosition].height,lane[cPosition].width/8);
        DoKeyRIGHTWork();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENDirMap::DoKey2Work(void)
{
try {
        usiMappingValue[cPosition]=2;
        lcd240x128.DISPLAY_GRAPHIC_XY(lane[cPosition].X,lane[cPosition].Y,word16x16[2],lane[cPosition].height,lane[cPosition].width/8);
        DoKeyRIGHTWork();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENDirMap::DoKey3Work(void)
{
try {
        usiMappingValue[cPosition]=3;
        lcd240x128.DISPLAY_GRAPHIC_XY(lane[cPosition].X,lane[cPosition].Y,word16x16[3],lane[cPosition].height,lane[cPosition].width/8);
        DoKeyRIGHTWork();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENDirMap::DoKey4Work(void)
{
try {
        usiMappingValue[cPosition]=4;
        lcd240x128.DISPLAY_GRAPHIC_XY(lane[cPosition].X,lane[cPosition].Y,word16x16[4],lane[cPosition].height,lane[cPosition].width/8);
        DoKeyRIGHTWork();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENDirMap::DoKey5Work(void)
{
try {
        usiMappingValue[cPosition]=5;
        lcd240x128.DISPLAY_GRAPHIC_XY(lane[cPosition].X,lane[cPosition].Y,word16x16[5],lane[cPosition].height,lane[cPosition].width/8);
        DoKeyRIGHTWork();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENDirMap::DoKey6Work(void)
{
try {
        usiMappingValue[cPosition]=6;
        lcd240x128.DISPLAY_GRAPHIC_XY(lane[cPosition].X,lane[cPosition].Y,word16x16[6],lane[cPosition].height,lane[cPosition].width/8);
        DoKeyRIGHTWork();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENDirMap::DoKey7Work(void)
{
try {
        usiMappingValue[cPosition]=7;
        lcd240x128.DISPLAY_GRAPHIC_XY(lane[cPosition].X,lane[cPosition].Y,word16x16[7],lane[cPosition].height,lane[cPosition].width/8);
        DoKeyRIGHTWork();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENDirMap::DoKeyF1Work(void)
{
    screenMain.DisplayMain();
}
//---------------------------------------------------------------------------
void SCREENDirMap::DoKeyF2Work(void)
{
try {

  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENDirMap::DoKeyF4Work(void)
{
    screenDirLightBoardMappingMenu.DisplayDirLightBoardMappingMenu();
}
//---------------------------------------------------------------------------
void SCREENDirMap::DoKeyEnterWork(void)
{
try {
    //OTMARKPRINTF  printf("%d:%d:%d:%d:%d:%d:%d:%d\n",usiMappingValue[0],usiMappingValue[1],usiMappingValue[2],usiMappingValue[3],usiMappingValue[4],usiMappingValue[5],usiMappingValue[6],usiMappingValue[7]);
      for(int i = 0; i < 8; i++) {
        smem.vSetSignamMapMappingDir(i, usiMappingValue[i]);
      }

      //screenMain.DisplayMain();
      screenCtlSetup.DisplayCtlSetup();

  } catch (...) {}
}
//------------------------------------------------------------------------
void SCREENDirMap::DoKeyLEFTWork(void)
{
try {
    clearCursor16x16(lane[cPosition].X,lane[cPosition].Y+16);
    if (cPosition==0) cPosition=1;
    cPosition--;
    setCursor16x16(lane[cPosition].X,lane[cPosition].Y+16);
  } catch (...) {}
}
//------------------------------------------------------------------------
void SCREENDirMap::DoKeyRIGHTWork(void)
{
try {
        clearCursor16x16(lane[cPosition].X,lane[cPosition].Y+16);
        cPosition++;
        if (cPosition>=8) cPosition=7;
        setCursor16x16(lane[cPosition].X,lane[cPosition].Y+16);
  } catch (...) {}
}
//------------------------------------------------------------------------
