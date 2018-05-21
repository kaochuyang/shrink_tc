//---------------------------------------------------------------------------
#include "SCREENRedCountMap.h"
#include "SCREENMain.h"
#include "screenRedCountMenu.h"
#include "LCD240x128.h"
#include "SMEM.h"

#include "CDataToMessageOK.h"
#include "WRITEJOB.h"

SCREENRedCountMap screenRedCountMap;
//---------------------------------------------------------------------------
SCREENRedCountMap::SCREENRedCountMap(void)
{
    LoadBitmapFromFile();
    InitDispWord();
    cPosition=0;
    for (int i=0;i<8;i++) usiMappingValue[i]=i;
}
//---------------------------------------------------------------------------
SCREENRedCountMap::~SCREENRedCountMap(void)
{
}
//---------------------------------------------------------------------------
void SCREENRedCountMap::DoKeyWork(BYTE key)              //已過濾,進來的是0x80~0x98之間,5X5按鍵
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
void SCREENRedCountMap::LoadBitmapFromFile(void)
{
try {
    FILE *bitmap;
    bitmap=fopen("//cct//bitmap//backGround//_6_RedCountMapping.bit","rb");
    if (bitmap) {
        fread(ucRedCountMappingBitmap,3840,1,bitmap);
        fclose(bitmap);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENRedCountMap::DisplayRedCountMapping(void)
{
try {
    smem.SetcFace(cREDCOUNTMAPPING);
    smem.vWriteMsgToDOM("Enter REDCOUNTMAPPING Screen");
    lcd240x128.DISPLAY_GRAPHIC(0,ucRedCountMappingBitmap,128,30);

//    laneCount=smem.vGetVDInterValData_6F0F_LaneCount();

    for (int x=0;x<8;x++) usiMappingValue[x]=smem.vGetWayMappingRedCount(x);

    for (int i=0;i<8;i++) lcd240x128.DISPLAY_GRAPHIC_XY(lane[i].X,lane[i].Y,word16x16[usiMappingValue[i]],lane[i].height,lane[i].width/8);

    cPosition=0;
    setCursor16x16(lane[cPosition].X,lane[cPosition].Y+16);

  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENRedCountMap::InitDispWord(void)
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
void SCREENRedCountMap::DoKey0Work(void)
{
try {
        usiMappingValue[cPosition]=0;
        lcd240x128.DISPLAY_GRAPHIC_XY(lane[cPosition].X,lane[cPosition].Y,word16x16[0],lane[cPosition].height,lane[cPosition].width/8);
        DoKeyRIGHTWork();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENRedCountMap::DoKey1Work(void)
{
try {
        usiMappingValue[cPosition]=1;
        lcd240x128.DISPLAY_GRAPHIC_XY(lane[cPosition].X,lane[cPosition].Y,word16x16[1],lane[cPosition].height,lane[cPosition].width/8);
        DoKeyRIGHTWork();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENRedCountMap::DoKey2Work(void)
{
try {
        usiMappingValue[cPosition]=2;
        lcd240x128.DISPLAY_GRAPHIC_XY(lane[cPosition].X,lane[cPosition].Y,word16x16[2],lane[cPosition].height,lane[cPosition].width/8);
        DoKeyRIGHTWork();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENRedCountMap::DoKey3Work(void)
{
try {
        usiMappingValue[cPosition]=3;
        lcd240x128.DISPLAY_GRAPHIC_XY(lane[cPosition].X,lane[cPosition].Y,word16x16[3],lane[cPosition].height,lane[cPosition].width/8);
        DoKeyRIGHTWork();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENRedCountMap::DoKey4Work(void)
{
try {
        usiMappingValue[cPosition]=4;
        lcd240x128.DISPLAY_GRAPHIC_XY(lane[cPosition].X,lane[cPosition].Y,word16x16[4],lane[cPosition].height,lane[cPosition].width/8);
        DoKeyRIGHTWork();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENRedCountMap::DoKey5Work(void)
{
try {
        usiMappingValue[cPosition]=5;
        lcd240x128.DISPLAY_GRAPHIC_XY(lane[cPosition].X,lane[cPosition].Y,word16x16[5],lane[cPosition].height,lane[cPosition].width/8);
        DoKeyRIGHTWork();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENRedCountMap::DoKey6Work(void)
{
try {
        usiMappingValue[cPosition]=6;
        lcd240x128.DISPLAY_GRAPHIC_XY(lane[cPosition].X,lane[cPosition].Y,word16x16[6],lane[cPosition].height,lane[cPosition].width/8);
        DoKeyRIGHTWork();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENRedCountMap::DoKey7Work(void)
{
try {
        usiMappingValue[cPosition]=7;
        lcd240x128.DISPLAY_GRAPHIC_XY(lane[cPosition].X,lane[cPosition].Y,word16x16[7],lane[cPosition].height,lane[cPosition].width/8);
        DoKeyRIGHTWork();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENRedCountMap::DoKeyF1Work(void)
{
    screenMain.DisplayMain();
}
//---------------------------------------------------------------------------
void SCREENRedCountMap::DoKeyF2Work(void)
{
try {
  /*
    for (int i=0;i<8;i++) {
         usiMappingValue[i]=INVAILDVALUE;
         clearSelectPoint16x16(lane[i].X,lane[i].Y,lane[i].height,lane[i].width/8);
    }
    */
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENRedCountMap::DoKeyF4Work(void)
{
    screenRedCountMenu.DisplayRedCountMenu();
}
//---------------------------------------------------------------------------
void SCREENRedCountMap::DoKeyEnterWork(void)
{
try {
    //OTMARKPRINTF  printf("%d:%d:%d:%d:%d:%d:%d:%d\n",usiMappingValue[0],usiMappingValue[1],usiMappingValue[2],usiMappingValue[3],usiMappingValue[4],usiMappingValue[5],usiMappingValue[6],usiMappingValue[7]);
      for(int i = 0; i < 8; i++) {
        smem.vSetWayMappingRedCount(i, usiMappingValue[i]);
      }

      char message[200]={0};
      sprintf(message,"Set Lane To %#d-%#d-%#d-%#d-%#d-%#d-%#d-%#d",usiMappingValue[0],usiMappingValue[1],usiMappingValue[2],usiMappingValue[3],usiMappingValue[4],usiMappingValue[5],usiMappingValue[6],usiMappingValue[7]);
      smem.vWriteMsgToDOM(message);
      screenMain.DisplayMain();

  } catch (...) {}
}
//------------------------------------------------------------------------
void SCREENRedCountMap::DoKeyLEFTWork(void)
{
try {
    clearCursor16x16(lane[cPosition].X,lane[cPosition].Y+16);
    if (cPosition==0) cPosition=1;
    cPosition--;
    setCursor16x16(lane[cPosition].X,lane[cPosition].Y+16);
  } catch (...) {}
}
//------------------------------------------------------------------------
void SCREENRedCountMap::DoKeyRIGHTWork(void)
{
try {
        clearCursor16x16(lane[cPosition].X,lane[cPosition].Y+16);
        cPosition++;
        if (cPosition>=8) cPosition=7;
        setCursor16x16(lane[cPosition].X,lane[cPosition].Y+16);
  } catch (...) {}
}
//------------------------------------------------------------------------
