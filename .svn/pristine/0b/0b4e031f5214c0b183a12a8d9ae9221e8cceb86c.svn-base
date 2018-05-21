#include "screenCommunication.h"
#include "LCD240x128.h"
#include "SCREENMain.h"
#include "screenPrioritymenu.h"
#include "CSTC.h"
#include "SMEM.h"

#include <stdio.h>
//---------------------------------------------------------------------------
ScreenCommunication screenCommunication;
//---------------------------------------------------------------------------
ScreenCommunication::ScreenCommunication(void)
{
try{
    loadBitmapFromFile();
    initDispWord();
 } catch (...) {}
}
//---------------------------------------------------------------------------
ScreenCommunication::~ScreenCommunication(void)
{
}
//---------------------------------------------------------------------------
void ScreenCommunication::loadBitmapFromFile(void)
{
try {
    FILE *bitmap;
    bitmap=fopen("//cct//bitmap//backGround//TC5F//communication.bit","rb");
    if (bitmap) {
        fread(BRTpriorityBitmap,3840,1,bitmap);
        fclose(bitmap);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenCommunication::DisplayCommunication(void)
{
try {
    smem.SetcFace(cCommunication);
    lcd240x128.DISPLAY_GRAPHIC(0,BRTpriorityBitmap,128,30);
    status();

  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenCommunication::doKeyWork(BYTE key)
{
try {
    switch (key) {
        case 0x80:
        case 0x81:
        case 0x82:
        case 0x83:
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
        case 0x90:
          doKeyF1Work();
        break;
        case 0x91:
        case 0x92:
        break;
        case 0x93:
          doKeyF4Work();
        break;
        case 0x94:
        case 0x95:
        case 0x96:
        case 0x97:
        case 0x98:
        break;
        default:
        break;
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenCommunication::initDispWord(void)
{
try {
    for(int i=0; i<2; i++){
        IPC[i].width=8; IPC[i].height=16; IPC[i].Y=48;
        SRC1[i].width=8; SRC1[i].height=16; SRC1[i].Y=80;
        SRC2[i].width=8; SRC2[i].height=16; SRC2[i].Y=112;
    }
    IPC[0].X=24; IPC[1].X=64;
    SRC1[0].X=24; SRC1[1].X=64;
    SRC2[0].X=24; SRC2[1].X=64;
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenCommunication::status(void)
{
try{
    lcd240x128.DISPLAY_GRAPHIC_XY(IPC[0].X,IPC[0].Y,word8x16Flash,IPC[0].height,IPC[0].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(IPC[1].X,IPC[1].Y,word8x16Flash,IPC[1].height,IPC[1].width/8);

    if(smem.GetSRC1BF00() == 0){
        lcd240x128.DISPLAY_GRAPHIC_XY(SRC1[0].X,SRC1[0].Y,word8x16Light,SRC1[0].height,SRC1[0].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(SRC1[1].X,SRC1[1].Y,word8x16Flash,SRC1[1].height,SRC1[1].width/8);
    } else{
        lcd240x128.DISPLAY_GRAPHIC_XY(SRC1[0].X,SRC1[0].Y,word8x16Flash,SRC1[0].height,SRC1[0].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(SRC1[1].X,SRC1[1].Y,word8x16Light,SRC1[1].height,SRC1[1].width/8);
    }

    if(smem.GetSRC2BF00() == 0){
        lcd240x128.DISPLAY_GRAPHIC_XY(SRC2[0].X,SRC2[0].Y,word8x16Light,SRC2[0].height,SRC2[0].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(SRC2[1].X,SRC2[1].Y,word8x16Flash,SRC2[1].height,SRC2[1].width/8);
    } else{
        lcd240x128.DISPLAY_GRAPHIC_XY(SRC2[0].X,SRC2[0].Y,word8x16Flash,SRC2[0].height,SRC2[0].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(SRC2[1].X,SRC2[1].Y,word8x16Light,SRC2[1].height,SRC2[1].width/8);
    }
 } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenCommunication::doKeyF1Work(void)
{
    screenMain.DisplayMain();
}
//---------------------------------------------------------------------------
void ScreenCommunication::doKeyF4Work(void)
{
    //screenRunTableList.DisplayRunTableList();
    screenPrioritymenu.DisplayPrioritymenu();
}
