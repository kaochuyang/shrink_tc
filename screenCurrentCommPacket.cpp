#include "screenCurrentCommPacket.h"
#include "LCD240x128.h"
//#include "screenLogo.h"
#include "SCREENMain.h"
//#include "screenRunTableList.h"
//#include "SCREENCtlSetup.h"
#include "SCREENSystemInfoMenu.h"

#include "CSTC.h"

#include "SCREENNetworkSet.h"

#include <stdio.h>
#include <string.h>

#include "SMEM.h"

int ScreenCurrentCommPacket::iLinePtr = 0;                                      //STATIC
int ScreenCurrentCommPacket::iLineShiftPtr = 0;                                      //STATIC

//---------------------------------------------------------------------------
ScreenCurrentCommPacket screenCurrentCommPacket;
//---------------------------------------------------------------------------
ScreenCurrentCommPacket::ScreenCurrentCommPacket(void)
{
    loadBitmapFromFile();
    initDispWord();
    cSelect=0;
    iLinePtr = 0;
}
//---------------------------------------------------------------------------
ScreenCurrentCommPacket::~ScreenCurrentCommPacket(void)
{
}
//---------------------------------------------------------------------------
void ScreenCurrentCommPacket::loadBitmapFromFile(void)
{
try {
    FILE *bitmap;
    bitmap=fopen("//cct//bitmap//backGround//4_1_000ScreenCommStatus.bit","rb");
    if (bitmap) {
        fread(ucCurrentCommPacketBitmap,3840,1,bitmap);
        fclose(bitmap);
    }
  } catch (...) {}
}


//---------------------------------------------------------------------------
void ScreenCurrentCommPacket::DisplayCurrentCommPacket(void)
{
try {
    smem.SetcFace(cCURRENTCOMMPACKET);
    lcd240x128.DISPLAY_GRAPHIC(0,ucCurrentCommPacketBitmap,128,30);

//    vRefreshCurrentLightStatusData();
//    vRefreshStepSec();

  } catch (...) {}
}


//---------------------------------------------------------------------------
void ScreenCurrentCommPacket::vRefreshCurrentScreenPacket(BYTE *writeMessage,int length,char *cInOut)
{
try {

  if(smem.GetcFace() == cCURRENTCOMMPACKET)
  {
    BYTE test[128]={0};
    char tempBuff[256],buff[2048]="";
    char portName[200]="[Write] ";

//    strcat(portName,cInOut);

    strcat(buff,cInOut);

    for(int i = 0; i < 30; i++) { //Clear Line
      if(iLinePtr < 5)
        lcd240x128.DISPLAY_GRAPHIC_XY(markPacket[iLinePtr][i].X,markPacket[iLinePtr+1][i].Y,test,markPacket[iLinePtr][i].height,markPacket[iLinePtr][i].width/8);
      else
        lcd240x128.DISPLAY_GRAPHIC_XY(markPacket[iLinePtr][i].X,markPacket[0][i].Y,test,markPacket[iLinePtr][i].height,markPacket[iLinePtr][i].width/8);
      lcd240x128.DISPLAY_GRAPHIC_XY(markPacket[iLinePtr][i].X,markPacket[iLinePtr][i].Y,test,markPacket[iLinePtr][i].height,markPacket[iLinePtr][i].width/8);
    }

    for (int i=0;i<length;i++) {
         sprintf(tempBuff,"%3X",writeMessage[i]);
         strcat(buff,tempBuff);
    }

    int iScreenLength = strlen(buff);

    for(int i = 0; i < iScreenLength; i++)
    {
            if(buff[i] == '0') buff[i] = 0x00;
      else if (buff[i] == '1') buff[i] = 0x01;
      else if (buff[i] == '2') buff[i] = 0x02;
      else if (buff[i] == '3') buff[i] = 0x03;
      else if (buff[i] == '4') buff[i] = 0x04;
      else if (buff[i] == '5') buff[i] = 0x05;
      else if (buff[i] == '6') buff[i] = 0x06;
      else if (buff[i] == '7') buff[i] = 0x07;
      else if (buff[i] == '8') buff[i] = 0x08;
      else if (buff[i] == '9') buff[i] = 0x09;
      else if (buff[i] == 'A') buff[i] = 0x0A;
      else if (buff[i] == 'B') buff[i] = 0x0B;
      else if (buff[i] == 'C') buff[i] = 0x0C;
      else if (buff[i] == 'D') buff[i] = 0x0D;
      else if (buff[i] == 'E') buff[i] = 0x0E;
      else if (buff[i] == 'F') buff[i] = 0x0F;
      else if (buff[i] == 'a') buff[i] = 0x10;
      else if (buff[i] == 'b') buff[i] = 0x11;
      else if (buff[i] == 'c') buff[i] = 0x12;
      else if (buff[i] == 'd') buff[i] = 0x13;
      else if (buff[i] == 'e') buff[i] = 0x14;
      else if (buff[i] == 'f') buff[i] = 0x15;
      else if (buff[i] == ' ') buff[i] = 0x99;
    }
//  //OTMARKPRINTF  printf("GoTo vRefreshCurrentScreenPacket, writeMessage= %s.\n", buff);

//    if(iScreenLength < 30) {
      for(int i = 0; i < 30; i++) {
        if( (buff[i] != (char)0x99) ) {
          lcd240x128.DISPLAY_GRAPHIC_XY(markPacket[iLinePtr][i].X,markPacket[iLinePtr][i].Y,word8x16[buff[i+iLineShiftPtr]],markPacket[iLinePtr][i].height,markPacket[iLinePtr][i].width/8);
  //        lcd240x128.DISPLAY_GRAPHIC_XY(markPacket[iLinePtr][i].X,markPacket[iLinePtr][i].Y,word8x16[21],markPacket[iLinePtr][i].height,markPacket[iLinePtr][i].width/8);
        }
        else {
          lcd240x128.DISPLAY_GRAPHIC_XY(markPacket[iLinePtr][i].X,markPacket[iLinePtr][i].Y,test,markPacket[iLinePtr][i].height,markPacket[iLinePtr][i].width/8);
        }
      }
//    }
//    else {
//      for(int i = 0; i < 30; i++) {
//        if(buff[i] != 0x99 ) {
//          lcd240x128.DISPLAY_GRAPHIC_XY(markPacket[iLinePtr][i].X,markPacket[iLinePtr][i].Y,word8x16[buff[i]],markPacket[iLinePtr][i].height,markPacket[iLinePtr][i].width/8);
  //        lcd240x128.DISPLAY_GRAPHIC_XY(markPacket[iLinePtr][i].X,markPacket[iLinePtr][i].Y,test,markPacket[iLinePtr][i].height,markPacket[iLinePtr][i].width/8);
  //        lcd240x128.DISPLAY_GRAPHIC_XY(markPacket[iLinePtr][i].X,markPacket[iLinePtr][i].Y,word8x16[21],markPacket[iLinePtr][i].height,markPacket[iLinePtr][i].width/8);
//        } else {
//          lcd240x128.DISPLAY_GRAPHIC_XY(markPacket[iLinePtr][i].X,markPacket[iLinePtr][i].Y,test,markPacket[iLinePtr][i].height,markPacket[iLinePtr][i].width/8);
//        }
//      }
//    }

    iLinePtr++;
    if(iLinePtr >= 6)
      iLinePtr = 0;
  }

} catch(...){}
}

//---------------------------------------------------------------------------
/*
void ScreenCurrentCommPacket::vRefreshStepSec(void)
{
try {
  int i1, i2, i3, i4;
  unsigned short int usiStepSec = stc.vGetStepTime();

  if(usiStepSec < 10000) {
    i1  = usiStepSec / 1000;
    i2 = (usiStepSec - (i1*1000)) / 100;
    lcd240x128.DISPLAY_GRAPHIC_XY(markStepSecCount[0].X,markStepSecCount[0].Y,word8x16[i1],markStepSecCount[0].height,markStepSecCount[0].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(markStepSecCount[1].X,markStepSecCount[1].Y,word8x16[i2],markStepSecCount[1].height,markStepSecCount[1].width/8);

  }
  if(usiStepSec < 10000) {
    i3  = (usiStepSec - (i2*100+i1*1000)) / 10;
    i4 = (usiStepSec - (i3*10+i2*100+i1*1000));
    lcd240x128.DISPLAY_GRAPHIC_XY(markStepSecCount[2].X,markStepSecCount[2].Y,word8x16[i3],markStepSecCount[2].height,markStepSecCount[2].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(markStepSecCount[3].X,markStepSecCount[3].Y,word8x16[i4],markStepSecCount[3].height,markStepSecCount[3].width/8);
  }
}catch(...){}
}
*/
//---------------------------------------------------------------------------
void ScreenCurrentCommPacket::initDispWord(void)
{
try {

  for(int i = 0; i < 6; i++) {
    for(int j = 0; j < 30; j++) {
      markPacket[i][j].width = 8;
      markPacket[i][j].height = 16;
      markPacket[i][j].X = j*8;
      markPacket[i][j].Y = i*16 + 26;
    }
  }

  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenCurrentCommPacket::doKeyWork(BYTE key)
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
void ScreenCurrentCommPacket::doKey0Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenCurrentCommPacket::doKey1Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenCurrentCommPacket::doKey2Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenCurrentCommPacket::doKey3Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenCurrentCommPacket::doKey4Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenCurrentCommPacket::doKey5Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenCurrentCommPacket::doKey6Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenCurrentCommPacket::doKey7Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenCurrentCommPacket::doKey8Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenCurrentCommPacket::doKey9Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenCurrentCommPacket::doKeyAWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenCurrentCommPacket::doKeyBWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenCurrentCommPacket::doKeyCWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenCurrentCommPacket::doKeyDWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenCurrentCommPacket::doKeyEWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenCurrentCommPacket::doKeyFWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenCurrentCommPacket::doKeyF1Work(void)
{
//    screenLogo.DisplayLogo();
    screenMain.DisplayMain();
}
//---------------------------------------------------------------------------
void ScreenCurrentCommPacket::doKeyF2Work(void)
{
    screenNetworkSet.DisplayNetworkSet();
}
//---------------------------------------------------------------------------
void ScreenCurrentCommPacket::doKeyF3Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenCurrentCommPacket::doKeyF4Work(void)
{
//    screenMain.DisplayMain();
//    screenRunTableList.DisplayRunTableList();
//arwen modify screenCtlSetup.DisplayCtlSetup();
    screenSystemInfoMenu.DisplaySystemInfoMenu();
}
//---------------------------------------------------------------------------
void ScreenCurrentCommPacket::doKeyUPWork(void)
{
try {
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenCurrentCommPacket::doKeyDOWNWork(void)
{
try {
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenCurrentCommPacket::doKeyLEFTWork(void)
{
try {
  if(iLineShiftPtr > 3)
    iLineShiftPtr-=4;
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenCurrentCommPacket::doKeyRIGHTWork(void)
{
try {
  iLineShiftPtr+=4;
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenCurrentCommPacket::doKeyEnterWork(void)
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

