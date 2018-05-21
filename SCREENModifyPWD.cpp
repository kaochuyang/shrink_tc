#include "SCREENModifyPWD.h"
#include "SCREENMain.h"
#include "SCREENCtlSetup.h"
#include "SCREENLockdb.h"
#include "SCREENV3Related.h"
#include "LCD240x128.h"
#include "SMEM.h"

SCREENModifyPWD screenModifyPWD;
//---------------------------------------------------------------------------
SCREENModifyPWD::SCREENModifyPWD(void)
{
try {
    for (int i=0;i<6;i++) passWord[i]='*';
    cPosition=0;
    LoadBitmapFromFile();
    InitDispWord();
  } catch (...) {}
}
//---------------------------------------------------------------------------
SCREENModifyPWD::~SCREENModifyPWD(void)
{
}
//---------------------------------------------------------------------------
void SCREENModifyPWD::DoKeyWork(BYTE key)                                           //已過濾,進來的是0x80~0x98之間
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
        case 0x88:
          DoKey8Work();
        break;
        case 0x89:
          DoKey9Work();
        break;
        case 0x8A:
          DoKeyAWork();
        break;
        case 0x8B:
          DoKeyBWork();
        break;
        case 0x8C:
          DoKeyCWork();
        break;
        case 0x8D:
          DoKeyDWork();
        break;
        case 0x8E:
          DoKeyEWork();
        break;
        case 0x8F:
          DoKeyFWork();
        break;
        case 0x90://F1,ESC
          DoKeyF1Work();
        break;
        case 0x91://F2
          DoKeyF2Work();
        break;
        case 0x92://F3
        break;
        case 0x93://F4
          DoKeyF4Work();
        break;
        case 0x94://UP
          DoKeyUPWork();
        break;
        case 0x95://DOWN
          DoKeyDOWNWork();
        break;
        case 0x96://LEFT
          DoKeyLEFTWork();
        break;
        case 0x97://RIGHT
          DoKeyRIGHTWork();
        break;
        case 0x98://Enter
          DoKeyEnterWork();
        break;
        default:
        break;
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENModifyPWD::LoadBitmapFromFile(void)
{
try {
    FILE *bitmap;
    bitmap=fopen("//cct//bitmap//backGround//4_1_6.bit","rb");
    if (bitmap) {
        fread(modifyPWDBitmap,3840,1,bitmap);
        fclose(bitmap);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENModifyPWD::DisplayModifyPWD(void)
{
try {
    smem.SetcFace(cMODIFYPWD);
    smem.vWriteMsgToDOM("Enter Modify Password Screen");
    lcd240x128.DISPLAY_GRAPHIC(0,modifyPWDBitmap,128,30);
    GetTCPassword();
    cPosition=0;
    setCursor24x24(pwd[cPosition].X,pwd[cPosition].Y+24);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENModifyPWD::GetTCPassword(void)
{
try {
    char *tempPassword=smem.GetPassword();
    for (int a=0;a<6;a++) passWord[a]=tempPassword[a];
    for (int i=0;i<6;i++) {
         switch (passWord[i]) {
                 case '0':
                 lcd240x128.DISPLAY_GRAPHIC_XY(pwd[i].X,pwd[i].Y,word24x24[0],pwd[i].height,pwd[i].width/8);
                 break;
                 case '1':
                 lcd240x128.DISPLAY_GRAPHIC_XY(pwd[i].X,pwd[i].Y,word24x24[1],pwd[i].height,pwd[i].width/8);
                 break;
                 case '2':
                 lcd240x128.DISPLAY_GRAPHIC_XY(pwd[i].X,pwd[i].Y,word24x24[2],pwd[i].height,pwd[i].width/8);
                 break;
                 case '3':
                 lcd240x128.DISPLAY_GRAPHIC_XY(pwd[i].X,pwd[i].Y,word24x24[3],pwd[i].height,pwd[i].width/8);
                 break;
                 case '4':
                 lcd240x128.DISPLAY_GRAPHIC_XY(pwd[i].X,pwd[i].Y,word24x24[4],pwd[i].height,pwd[i].width/8);
                 break;
                 case '5':
                 lcd240x128.DISPLAY_GRAPHIC_XY(pwd[i].X,pwd[i].Y,word24x24[5],pwd[i].height,pwd[i].width/8);
                 break;
                 case '6':
                 lcd240x128.DISPLAY_GRAPHIC_XY(pwd[i].X,pwd[i].Y,word24x24[6],pwd[i].height,pwd[i].width/8);
                 break;
                 case '7':
                 lcd240x128.DISPLAY_GRAPHIC_XY(pwd[i].X,pwd[i].Y,word24x24[7],pwd[i].height,pwd[i].width/8);
                 break;
                 case '8':
                 lcd240x128.DISPLAY_GRAPHIC_XY(pwd[i].X,pwd[i].Y,word24x24[8],pwd[i].height,pwd[i].width/8);
                 break;
                 case '9':
                 lcd240x128.DISPLAY_GRAPHIC_XY(pwd[i].X,pwd[i].Y,word24x24[9],pwd[i].height,pwd[i].width/8);
                 break;
                 case 'A':
                 case 'a':
                 lcd240x128.DISPLAY_GRAPHIC_XY(pwd[i].X,pwd[i].Y,word24x24[10],pwd[i].height,pwd[i].width/8);
                 break;
                 case 'B':
                 case 'b':
                 lcd240x128.DISPLAY_GRAPHIC_XY(pwd[i].X,pwd[i].Y,word24x24[11],pwd[i].height,pwd[i].width/8);
                 break;
                 case 'C':
                 case 'c':
                 lcd240x128.DISPLAY_GRAPHIC_XY(pwd[i].X,pwd[i].Y,word24x24[12],pwd[i].height,pwd[i].width/8);
                 break;
                 case 'D':
                 case 'd':
                 lcd240x128.DISPLAY_GRAPHIC_XY(pwd[i].X,pwd[i].Y,word24x24[13],pwd[i].height,pwd[i].width/8);
                 break;
                 case 'E':
                 case 'e':
                 lcd240x128.DISPLAY_GRAPHIC_XY(pwd[i].X,pwd[i].Y,word24x24[14],pwd[i].height,pwd[i].width/8);
                 break;
                 case 'F':
                 case 'f':
                 lcd240x128.DISPLAY_GRAPHIC_XY(pwd[i].X,pwd[i].Y,word24x24[15],pwd[i].height,pwd[i].width/8);
                 break;
                 default:
                 break;
         }
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENModifyPWD::InitDispWord(void)
{
try {
    for (int i=0;i<6;i++) {
         pwd[i].X=48+i*24;
         pwd[i].Y=55;
         pwd[i].width=24;
         pwd[i].height=24;
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENModifyPWD::DoKey0Work(void)
{
try {
    lcd240x128.DISPLAY_GRAPHIC_XY(pwd[cPosition].X,pwd[cPosition].Y,word24x24[0],pwd[cPosition].height,pwd[cPosition].width/8);
    passWord[cPosition]='0';
    DoKeyRIGHTWork();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENModifyPWD::DoKey1Work(void)
{
try {
    lcd240x128.DISPLAY_GRAPHIC_XY(pwd[cPosition].X,pwd[cPosition].Y,word24x24[1],pwd[cPosition].height,pwd[cPosition].width/8);
    passWord[cPosition]='1';
    DoKeyRIGHTWork();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENModifyPWD::DoKey2Work(void)
{
try {
    lcd240x128.DISPLAY_GRAPHIC_XY(pwd[cPosition].X,pwd[cPosition].Y,word24x24[2],pwd[cPosition].height,pwd[cPosition].width/8);
    passWord[cPosition]='2';
    DoKeyRIGHTWork();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENModifyPWD::DoKey3Work(void)
{
try {
    lcd240x128.DISPLAY_GRAPHIC_XY(pwd[cPosition].X,pwd[cPosition].Y,word24x24[3],pwd[cPosition].height,pwd[cPosition].width/8);
    passWord[cPosition]='3';
    DoKeyRIGHTWork();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENModifyPWD::DoKey4Work(void)
{
try {
    lcd240x128.DISPLAY_GRAPHIC_XY(pwd[cPosition].X,pwd[cPosition].Y,word24x24[4],pwd[cPosition].height,pwd[cPosition].width/8);
    passWord[cPosition]='4';
    DoKeyRIGHTWork();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENModifyPWD::DoKey5Work(void)
{
try {
    lcd240x128.DISPLAY_GRAPHIC_XY(pwd[cPosition].X,pwd[cPosition].Y,word24x24[5],pwd[cPosition].height,pwd[cPosition].width/8);
    passWord[cPosition]='5';
    DoKeyRIGHTWork();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENModifyPWD::DoKey6Work(void)
{
try {
    lcd240x128.DISPLAY_GRAPHIC_XY(pwd[cPosition].X,pwd[cPosition].Y,word24x24[6],pwd[cPosition].height,pwd[cPosition].width/8);
    passWord[cPosition]='6';
    DoKeyRIGHTWork();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENModifyPWD::DoKey7Work(void)
{
try {
    lcd240x128.DISPLAY_GRAPHIC_XY(pwd[cPosition].X,pwd[cPosition].Y,word24x24[7],pwd[cPosition].height,pwd[cPosition].width/8);
    passWord[cPosition]='7';
    DoKeyRIGHTWork();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENModifyPWD::DoKey8Work(void)
{
try {
    lcd240x128.DISPLAY_GRAPHIC_XY(pwd[cPosition].X,pwd[cPosition].Y,word24x24[8],pwd[cPosition].height,pwd[cPosition].width/8);
    passWord[cPosition]='8';
    DoKeyRIGHTWork();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENModifyPWD::DoKey9Work(void)
{
try {
    lcd240x128.DISPLAY_GRAPHIC_XY(pwd[cPosition].X,pwd[cPosition].Y,word24x24[9],pwd[cPosition].height,pwd[cPosition].width/8);
    passWord[cPosition]='9';
    DoKeyRIGHTWork();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENModifyPWD::DoKeyAWork(void)
{
try {
    lcd240x128.DISPLAY_GRAPHIC_XY(pwd[cPosition].X,pwd[cPosition].Y,word24x24[10],pwd[cPosition].height,pwd[cPosition].width/8);
    passWord[cPosition]='A';
    DoKeyRIGHTWork();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENModifyPWD::DoKeyBWork(void)
{
try {
    lcd240x128.DISPLAY_GRAPHIC_XY(pwd[cPosition].X,pwd[cPosition].Y,word24x24[11],pwd[cPosition].height,pwd[cPosition].width/8);
    passWord[cPosition]='B';
    DoKeyRIGHTWork();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENModifyPWD::DoKeyCWork(void)
{
try {
    lcd240x128.DISPLAY_GRAPHIC_XY(pwd[cPosition].X,pwd[cPosition].Y,word24x24[12],pwd[cPosition].height,pwd[cPosition].width/8);
    passWord[cPosition]='C';
    DoKeyRIGHTWork();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENModifyPWD::DoKeyDWork(void)
{
try {
    lcd240x128.DISPLAY_GRAPHIC_XY(pwd[cPosition].X,pwd[cPosition].Y,word24x24[13],pwd[cPosition].height,pwd[cPosition].width/8);
    passWord[cPosition]='D';
    DoKeyRIGHTWork();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENModifyPWD::DoKeyEWork(void)
{
try {
    lcd240x128.DISPLAY_GRAPHIC_XY(pwd[cPosition].X,pwd[cPosition].Y,word24x24[14],pwd[cPosition].height,pwd[cPosition].width/8);
    passWord[cPosition]='E';
    DoKeyRIGHTWork();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENModifyPWD::DoKeyFWork(void)
{
try {
    lcd240x128.DISPLAY_GRAPHIC_XY(pwd[cPosition].X,pwd[cPosition].Y,word24x24[15],pwd[cPosition].height,pwd[cPosition].width/8);
    passWord[cPosition]='F';
    DoKeyRIGHTWork();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENModifyPWD::DoKeyF1Work(void)
{
    screenMain.DisplayMain();
}
//---------------------------------------------------------------------------
void SCREENModifyPWD::DoKeyF2Work(void)
{
    for (int i=0;i<6;i++) {
         passWord[i]='*';
         clearSelectPoint24x24(pwd[i].X,pwd[i].Y,pwd[i].height,pwd[i].width/8);
    }
}
//---------------------------------------------------------------------------
void SCREENModifyPWD::DoKeyF4Work(void)
{
//arwen modify     screenCtlSetup.DisplayCtlSetup();
     screenV3Related.DisplayV3Related();
}
//---------------------------------------------------------------------------
void SCREENModifyPWD::DoKeyEnterWork(void)
{
    if (smem.GetDbOperStat()==0 || smem.vLoadCenterConnectStatus()==false) {
        if (smem.SetPassword(passWord[0],passWord[1],passWord[2],passWord[3],passWord[4],passWord[5])) {

            char message[200]={0};
            sprintf(message,"Change Password for %#c%#c%#c%#c%#c%#c",passWord[0],passWord[1],passWord[2],passWord[3],passWord[4],passWord[5]);
            smem.vWriteMsgToDOM(message);

            //screenCtlSetup.DisplayCtlSetup();
            screenV3Related.DisplayV3Related();
        }
    } else {
        smem.SetLastFace(cMODIFYPWD);
        screenLockdb.DisplayLockdb();
    }
}
//---------------------------------------------------------------------------
void SCREENModifyPWD::DoKeyUPWork(void)
{
    DoKeyLEFTWork();
}
//---------------------------------------------------------------------------
void SCREENModifyPWD::DoKeyDOWNWork(void)
{
    DoKeyRIGHTWork();
}
//---------------------------------------------------------------------------
void SCREENModifyPWD::DoKeyLEFTWork(void)
{
try {
    clearCursor24x24(pwd[cPosition].X,pwd[cPosition].Y+24);
    if (cPosition==0) cPosition=1;
    cPosition--;
    setCursor24x24(pwd[cPosition].X,pwd[cPosition].Y+24);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENModifyPWD::DoKeyRIGHTWork(void)
{
try {
    clearCursor24x24(pwd[cPosition].X,pwd[cPosition].Y+24);
    cPosition++;
    if (cPosition>=6) cPosition=5;
    setCursor24x24(pwd[cPosition].X,pwd[cPosition].Y+24);
  } catch (...) {}
}
//---------------------------------------------------------------------------
