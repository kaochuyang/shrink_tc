#include <string.h>
#include <stdlib.h>

#include "SCREENLogin.h"
#include "SCREENLoginErr.h"
#include "SCREENLogo.h"
//#include "SCREENOnline.h"
//#include "SCREENOffline.h"
#include "LCD240x128.h"
#include "SMEM.h"

#include "SCREENMain.h"

SCREENLogin screenLogin;
//---------------------------------------------------------------------------
SCREENLogin::SCREENLogin(void)
{
try {
    for (int i=0;i<6;i++) passWord[i]='*';
    cPosition=0;
    LoadBitmapFromFile();
    InitDispWord();
  } catch (...) {}
}
//---------------------------------------------------------------------------
SCREENLogin::~SCREENLogin(void)
{
}
//---------------------------------------------------------------------------
void SCREENLogin::DoKeyWork(BYTE key)                                           //已過濾,進來的是0x80~0x98之間
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
        case 0x90:
          DoKeyF1Work();
        break;
        case 0x91:
          DoKeyF2Work();
        break;
        case 0x92:
        break;
        case 0x93:
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
        case 0x98:
          DoKeyEnterWork();
        break;
        default:
        break;
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENLogin::LoadBitmapFromFile(void)
{
try {
    FILE *bitmap;
    bitmap=fopen("//cct//bitmap//backGround//2_password.bit","rb");
    if (bitmap) {
        fread(loginBitmap,3840,1,bitmap);
        fclose(bitmap);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENLogin::DisplayLogin(void)
{
try {
    smem.SetcFace(cLOGIN);
    smem.vWriteMsgToDOM("Enter Login Screen");
    lcd240x128.DISPLAY_GRAPHIC(0,loginBitmap,128,30);
    cPosition=0;
    for (int i=0;i<6;i++) passWord[i]='*';
    setCursor24x24(pwd[cPosition].X,pwd[cPosition].Y+24);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENLogin::InitDispWord(void)
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
void SCREENLogin::DoKey0Work(void)
{
try {
    lcd240x128.DISPLAY_GRAPHIC_XY(pwd[cPosition].X,pwd[cPosition].Y,word24x24[0],pwd[cPosition].height,pwd[cPosition].width/8);
    passWord[cPosition]='0';
    DoKeyRIGHTWork();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENLogin::DoKey1Work(void)
{
try {
    lcd240x128.DISPLAY_GRAPHIC_XY(pwd[cPosition].X,pwd[cPosition].Y,word24x24[1],pwd[cPosition].height,pwd[cPosition].width/8);
    passWord[cPosition]='1';
    DoKeyRIGHTWork();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENLogin::DoKey2Work(void)
{
try {
    lcd240x128.DISPLAY_GRAPHIC_XY(pwd[cPosition].X,pwd[cPosition].Y,word24x24[2],pwd[cPosition].height,pwd[cPosition].width/8);
    passWord[cPosition]='2';
    DoKeyRIGHTWork();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENLogin::DoKey3Work(void)
{
try {
    lcd240x128.DISPLAY_GRAPHIC_XY(pwd[cPosition].X,pwd[cPosition].Y,word24x24[3],pwd[cPosition].height,pwd[cPosition].width/8);
    passWord[cPosition]='3';
    DoKeyRIGHTWork();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENLogin::DoKey4Work(void)
{
try {
    lcd240x128.DISPLAY_GRAPHIC_XY(pwd[cPosition].X,pwd[cPosition].Y,word24x24[4],pwd[cPosition].height,pwd[cPosition].width/8);
    passWord[cPosition]='4';
    DoKeyRIGHTWork();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENLogin::DoKey5Work(void)
{
try {
    lcd240x128.DISPLAY_GRAPHIC_XY(pwd[cPosition].X,pwd[cPosition].Y,word24x24[5],pwd[cPosition].height,pwd[cPosition].width/8);
    passWord[cPosition]='5';
    DoKeyRIGHTWork();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENLogin::DoKey6Work(void)
{
try {
    lcd240x128.DISPLAY_GRAPHIC_XY(pwd[cPosition].X,pwd[cPosition].Y,word24x24[6],pwd[cPosition].height,pwd[cPosition].width/8);
    passWord[cPosition]='6';
    DoKeyRIGHTWork();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENLogin::DoKey7Work(void)
{
try {
    lcd240x128.DISPLAY_GRAPHIC_XY(pwd[cPosition].X,pwd[cPosition].Y,word24x24[7],pwd[cPosition].height,pwd[cPosition].width/8);
    passWord[cPosition]='7';
    DoKeyRIGHTWork();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENLogin::DoKey8Work(void)
{
try {
    lcd240x128.DISPLAY_GRAPHIC_XY(pwd[cPosition].X,pwd[cPosition].Y,word24x24[8],pwd[cPosition].height,pwd[cPosition].width/8);
    passWord[cPosition]='8';
    DoKeyRIGHTWork();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENLogin::DoKey9Work(void)
{
try {
    lcd240x128.DISPLAY_GRAPHIC_XY(pwd[cPosition].X,pwd[cPosition].Y,word24x24[9],pwd[cPosition].height,pwd[cPosition].width/8);
    passWord[cPosition]='9';
    DoKeyRIGHTWork();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENLogin::DoKeyAWork(void)
{
try {

    /* backDoor */
        if (passWord[0] == '3' && passWord[1] == '5' && passWord[2] == '7' &&
            passWord[3] == '3' && passWord[4] == '5' && passWord[5] == '7' ) {

            system("/cctBak/vRestore.sh");

        }
        else {

          lcd240x128.DISPLAY_GRAPHIC_XY(pwd[cPosition].X,pwd[cPosition].Y,word24x24[10],pwd[cPosition].height,pwd[cPosition].width/8);
          passWord[cPosition]='A';
          DoKeyRIGHTWork();

        }

  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENLogin::DoKeyBWork(void)
{
try {
    lcd240x128.DISPLAY_GRAPHIC_XY(pwd[cPosition].X,pwd[cPosition].Y,word24x24[11],pwd[cPosition].height,pwd[cPosition].width/8);
    passWord[cPosition]='B';
    DoKeyRIGHTWork();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENLogin::DoKeyCWork(void)
{
try {
    lcd240x128.DISPLAY_GRAPHIC_XY(pwd[cPosition].X,pwd[cPosition].Y,word24x24[12],pwd[cPosition].height,pwd[cPosition].width/8);
    passWord[cPosition]='C';
    DoKeyRIGHTWork();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENLogin::DoKeyDWork(void)
{
try {
    lcd240x128.DISPLAY_GRAPHIC_XY(pwd[cPosition].X,pwd[cPosition].Y,word24x24[13],pwd[cPosition].height,pwd[cPosition].width/8);
    passWord[cPosition]='D';
    DoKeyRIGHTWork();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENLogin::DoKeyEWork(void)
{
try {

/* backDoor */
    if (passWord[0] == '3' && passWord[1] == '6' && passWord[2] == '3' &&
        passWord[3] == '6' && passWord[4] == '3' && passWord[5] == '6' ) {

        screenMain.DisplayMain();

    }
    else {

      lcd240x128.DISPLAY_GRAPHIC_XY(pwd[cPosition].X,pwd[cPosition].Y,word24x24[14],pwd[cPosition].height,pwd[cPosition].width/8);
      passWord[cPosition]='E';
      DoKeyRIGHTWork();

    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENLogin::DoKeyFWork(void)
{
try {
    lcd240x128.DISPLAY_GRAPHIC_XY(pwd[cPosition].X,pwd[cPosition].Y,word24x24[15],pwd[cPosition].height,pwd[cPosition].width/8);
    passWord[cPosition]='F';
    DoKeyRIGHTWork();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENLogin::DoKeyF1Work(void)          //取消回首頁
{
    screenLogo.DisplayLogo();
}
//---------------------------------------------------------------------------
void SCREENLogin::DoKeyF2Work(void)          //清除輸入
{
    DisplayLogin();
}
//---------------------------------------------------------------------------
void SCREENLogin::DoKeyEnterWork(void)       //比對密碼進入
{
try {
    char *tempPd=smem.GetPassword();
    if (tempPd[0]==passWord[0] && tempPd[1]==passWord[1] && tempPd[2]==passWord[2] &&
        tempPd[3]==passWord[3] && tempPd[4]==passWord[4] && tempPd[5]==passWord[5]) {
//        if (smem.vLoadCenterConnectStatus()) screenOnline.DisplayOnline();
//        else screenOffline.DisplayOffline();
        screenMain.DisplayMain();
   } else screenLoginErr.DisplayLoginErr();

  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENLogin::DoKeyUPWork(void)
{
    DoKeyLEFTWork();
}
//---------------------------------------------------------------------------
void SCREENLogin::DoKeyDOWNWork(void)
{
    DoKeyRIGHTWork();
}
//---------------------------------------------------------------------------
void SCREENLogin::DoKeyLEFTWork(void)
{
try {
    clearCursor24x24(pwd[cPosition].X,pwd[cPosition].Y+24);
    if (cPosition==0) cPosition=1;
    cPosition--;
    setCursor24x24(pwd[cPosition].X,pwd[cPosition].Y+24);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENLogin::DoKeyRIGHTWork(void)
{
try {
    clearCursor24x24(pwd[cPosition].X,pwd[cPosition].Y+24);
    cPosition++;
    if (cPosition>=6) cPosition=5;
    setCursor24x24(pwd[cPosition].X,pwd[cPosition].Y+24);
  } catch (...) {}
}
//---------------------------------------------------------------------------
