#include "SCREENPedSwitchStatus.h"
#include "LCD240x128.h"
#include "SCREENMain.h"
#include "ScreenActuateArwenMenu.h"
#include "SMEM.h"
#include "SCREENHWReset.h"

#include <stdio.h>

SCREENPedSwitchStatus screenPedSwitchStatus;
///////////////////////////////////////////////////
SCREENPedSwitchStatus::SCREENPedSwitchStatus()
{
    loadBitmapFromFile();
    initDispWord();
    cSelect=0;
}
SCREENPedSwitchStatus::~SCREENPedSwitchStatus()
{
}
void SCREENPedSwitchStatus::DisplayPedSwitchStatus()
{
    smem.SetcFace(cPEDSWITCH);
    smem.vWriteMsgToDOM("Display Ped Switch Status");
    lcd240x128.DISPLAY_GRAPHIC(0,padSwitchBitmap,128,30);
    cSelect=0;
    cPosition=0;

    if(smem.cPedPushButton.GetPedisNCTU()==false)
    siDevCount = smem.cPedPushButton.GetDevCount();
    else
    siDevCount =10;

    DisplayDevID();
    setCursor8x16(markDevCount[cPosition].X,markDevCount[cPosition].Y+16);
}
//---------------------------------------------------------------------------
void SCREENPedSwitchStatus::loadBitmapFromFile(void)
{
try {
    FILE *bitmap;
//arwen mobify    bitmap=fopen("//cct//bitmap//backGround//4_1_0ControlSetting.bit","rb");
    bitmap=fopen("//cct//bitmap//backGround//actmenu//PedSwitchStatus.bit","rb");
    if (bitmap) {
        fread(padSwitchBitmap,3840,1,bitmap);
        fclose(bitmap);
    }
  } catch (...) {}
}
void SCREENPedSwitchStatus::vRefreshSwitchStatus()
{
try {
    int iTmpFace = smem.GetcFace();
    if(iTmpFace == cPEDSWITCH){
        DisplayDevID();
    }
} catch (...) {}
}
void SCREENPedSwitchStatus::initDispWord(void)
{
try {

    for(int i=0;i<2;i++){
        markDevCount[i].X=216+(i*8);
        markDevCount[i].Y=6;
        markDevCount[i].width=8;
        markDevCount[i].height=16;
    }
    for (int i=0;i<8;i++) {
        for(int j=0;j<2;j++){
            markDevID[i][j].width=8;
            markDevID[i][j].height=16;
            markDevID[i][j].Y=47;
        }
        markDevID[i][0].X=56+(i*24);
        markDevID[i][1].X=64+(i*24);
        ///
        markSWVar[i].X=56+(i*24);
        markSWVar[i].Y=63;
        markSWVar[i].width=8;
        markSWVar[i].height=16;
        ///
        markStatus[i].X=56+(i*24);
        markStatus[i].Y=78;
        markStatus[i].width=8;
        markStatus[i].height=16;
    }

  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENPedSwitchStatus::DisplayDevID()
{
    try {
    int iHiTMP, iLowTMP;
//    unsigned short int siDevCount=0;
    unsigned short int siDevID=0;
    unsigned short int siDevStatus=0;
    unsigned short int siSWVar=0;

//    if(smem.cPedPushButton.GetPedisNCTU()==false)
//    siDevCount = smem.cPedPushButton.GetDevCount();
//    else
//    siDevCount =10;

    if(siDevCount>=10){
        iHiTMP=siDevCount/10;
        iLowTMP=siDevCount%10;
    }else{
        iHiTMP=0;
        iLowTMP=siDevCount;
    }
    lcd240x128.DISPLAY_GRAPHIC_XY(markDevCount[0].X,markDevCount[0].Y,word8x16[iHiTMP],markDevCount[0].height,markDevCount[0].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(markDevCount[1].X,markDevCount[1].Y,word8x16[iLowTMP],markDevCount[1].height,markDevCount[1].width/8);
    for(int i=0;i<8;i++){
        if(i+(cSelect*8)<siDevCount){
            siDevID=smem.cPedPushButton.GetDevID(i+(cSelect*8));
            iHiTMP = siDevID /10;
            iLowTMP = siDevID %10;
            lcd240x128.DISPLAY_GRAPHIC_XY(markDevID[i][0].X,markDevID[i][0].Y,word8x16[iHiTMP],markDevID[i][0].height,markDevID[i][0].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(markDevID[i][1].X,markDevID[i][1].Y,word8x16[iLowTMP],markDevID[i][1].height,markDevID[i][1].width/8);
            /////SWVar
            siSWVar=smem.cPedPushButton.GetPEDSWVar(i+(cSelect*8));
            lcd240x128.DISPLAY_GRAPHIC_XY(markSWVar[i].X,markSWVar[i].Y,word8x16[siSWVar],markSWVar[i].height,markSWVar[i].width/8);
            /////Status
            siDevStatus=smem.cPedPushButton.GetWatchDogStatus(i+(cSelect*8));
            if(siDevStatus){
                lcd240x128.DISPLAY_GRAPHIC_XY(markStatus[i].X,markStatus[i].Y,word8x16[1],markStatus[i].height,markStatus[i].width/8);
            }else{
                lcd240x128.DISPLAY_GRAPHIC_XY(markStatus[i].X,markStatus[i].Y,word8x16[0],markStatus[i].height,markStatus[i].width/8);
            }
        }else{
            lcd240x128.DISPLAY_GRAPHIC_XY(markDevID[i][0].X,markDevID[i][0].Y,word8x16[15],markDevID[i][0].height,markDevID[i][0].width/8);
            lcd240x128.DISPLAY_GRAPHIC_XY(markDevID[i][1].X,markDevID[i][1].Y,word8x16[15],markDevID[i][1].height,markDevID[i][1].width/8);
            /////SWVar
            lcd240x128.DISPLAY_GRAPHIC_XY(markSWVar[i].X,markSWVar[i].Y,word8x16[0],markSWVar[i].height,markSWVar[i].width/8);
            /////Status
            lcd240x128.DISPLAY_GRAPHIC_XY(markStatus[i].X,markStatus[i].Y,word8x16[0],markStatus[i].height,markStatus[i].width/8);
        }


    }
} catch (...) {}
}
//----------------------------------------------------------------------------
void SCREENPedSwitchStatus::doKeyWork(BYTE key)                                        //¤w¹LÂo,¶i¨Óªº¬O0x80~0x98¤§¶¡
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
        case 0x8B:
        case 0x8C:
        case 0x8D:
        break;
        case 0x8E:
        doKeyEWork();
        break;
        case 0x8F:
        doKeyFWork();
        break;
        case 0x90://F1 ESC
          doKeyF1Work();
        break;
        case 0x91:
          doKeyF2Work();
        break;
        case 0x92:
        //OT Pass
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
        case 0x98://ENTER
          doKeyEnterWork();
        break;
        default:
        break;
    }
  } catch (...) {}
}
//----------------------------------------------------------------
//---------------------------------------------------------------------------
void SCREENPedSwitchStatus::doKey0Work(void)
{
try {
    unsigned char tmp;
    if(smem.cPedPushButton.GetPedisNCTU()==false){
        if(cPosition==0){
            tmp = siDevCount % 10;
            siDevCount = 0*10 + tmp;
        } else if(cPosition==1){
            tmp = siDevCount / 10;
            siDevCount = tmp*10 + 0;
        }
        lcd240x128.DISPLAY_GRAPHIC_XY(markDevCount[cPosition].X,markDevCount[cPosition].Y,word8x16[0],markDevCount[cPosition].height,markDevCount[cPosition].width/8);
    }
} catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENPedSwitchStatus::doKey1Work(void)
{
try {
    unsigned char tmp;
    if(smem.cPedPushButton.GetPedisNCTU()==false){
        if(cPosition==0){
            tmp = siDevCount % 10;
            siDevCount = 1*10 + tmp;
        } else if(cPosition==1){
            tmp = siDevCount / 10;
            siDevCount = tmp*10 + 1;
        }
        lcd240x128.DISPLAY_GRAPHIC_XY(markDevCount[cPosition].X,markDevCount[cPosition].Y,word8x16[1],markDevCount[cPosition].height,markDevCount[cPosition].width/8);
    }
} catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENPedSwitchStatus::doKey2Work(void)
{
try {
    unsigned char tmp;
    if(smem.cPedPushButton.GetPedisNCTU()==false){
        if(cPosition==1){
            tmp = siDevCount / 10;
            siDevCount = tmp*10 + 2;
            lcd240x128.DISPLAY_GRAPHIC_XY(markDevCount[cPosition].X,markDevCount[cPosition].Y,word8x16[2],markDevCount[cPosition].height,markDevCount[cPosition].width/8);
        }
    }
} catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENPedSwitchStatus::doKey3Work(void)
{
try {
    unsigned char tmp;
    if(smem.cPedPushButton.GetPedisNCTU()==false){
        if(cPosition==1){
            tmp = siDevCount / 10;
            siDevCount = tmp*10 + 3;
            lcd240x128.DISPLAY_GRAPHIC_XY(markDevCount[cPosition].X,markDevCount[cPosition].Y,word8x16[3],markDevCount[cPosition].height,markDevCount[cPosition].width/8);
        }
    }
} catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENPedSwitchStatus::doKey4Work(void)
{
try {
    unsigned char tmp;
    if(smem.cPedPushButton.GetPedisNCTU()==false){
        if(cPosition==1){
            tmp = siDevCount / 10;
            siDevCount = tmp*10 + 4;
            lcd240x128.DISPLAY_GRAPHIC_XY(markDevCount[cPosition].X,markDevCount[cPosition].Y,word8x16[4],markDevCount[cPosition].height,markDevCount[cPosition].width/8);
        }
    }
} catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENPedSwitchStatus::doKey5Work(void)
{
try {
    unsigned char tmp;
    if(smem.cPedPushButton.GetPedisNCTU()==false){
        if(cPosition==1){
            tmp = siDevCount / 10;
            siDevCount = tmp*10 + 5;
            lcd240x128.DISPLAY_GRAPHIC_XY(markDevCount[cPosition].X,markDevCount[cPosition].Y,word8x16[5],markDevCount[cPosition].height,markDevCount[cPosition].width/8);
        }
    }
} catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENPedSwitchStatus::doKey6Work(void)
{
try {
    unsigned char tmp;
    if(smem.cPedPushButton.GetPedisNCTU()==false){
        if(cPosition==1){
            tmp = siDevCount / 10;
            siDevCount = tmp*10 + 6;
            lcd240x128.DISPLAY_GRAPHIC_XY(markDevCount[cPosition].X,markDevCount[cPosition].Y,word8x16[6],markDevCount[cPosition].height,markDevCount[cPosition].width/8);
        }
    }
} catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENPedSwitchStatus::doKey7Work(void)
{
try {
    unsigned char tmp;
    if(smem.cPedPushButton.GetPedisNCTU()==false){
        if(cPosition==1){
            tmp = siDevCount / 10;
            siDevCount = tmp*10 + 7;
            lcd240x128.DISPLAY_GRAPHIC_XY(markDevCount[cPosition].X,markDevCount[cPosition].Y,word8x16[7],markDevCount[cPosition].height,markDevCount[cPosition].width/8);
        }
    }
} catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENPedSwitchStatus::doKey8Work(void)
{
try {
    unsigned char tmp;
    if(smem.cPedPushButton.GetPedisNCTU()==false){
        if(cPosition==1){
            tmp = siDevCount / 10;
            siDevCount = tmp*10 + 8;
            lcd240x128.DISPLAY_GRAPHIC_XY(markDevCount[cPosition].X,markDevCount[cPosition].Y,word8x16[8],markDevCount[cPosition].height,markDevCount[cPosition].width/8);
        }
    }
} catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENPedSwitchStatus::doKey9Work(void)
{
}
//---------------------------------------------------------------------------
void SCREENPedSwitchStatus::doKeyEWork(void)
{
    if(cSelect<=0){
        cSelect=0;
    }else{
        cSelect--;
    }
}
//---------------------------------------------------------------------------
void SCREENPedSwitchStatus::doKeyFWork(void)
{
    int siDevCount=0;
    int siShowPage=0;
    if(smem.cPedPushButton.GetPedisNCTU()==false)
    siDevCount = smem.cPedPushButton.GetDevCount();
    else
    siDevCount =10;

    siShowPage = siDevCount/8;
    if(siShowPage >=cSelect){
        cSelect = siShowPage;
    }else{
        cSelect++;
    }
}
//---------------------------------------------------------------------------
void SCREENPedSwitchStatus::doKeyF1Work(void)
{
    screenMain.DisplayMain();
}
//---------------------------------------------------------------------------
void SCREENPedSwitchStatus::doKeyF2Work(void)
{

}
//---------------------------------------------------------------------------
void SCREENPedSwitchStatus::doKeyF3Work(void)
{
try{
    FILE *tmp;
    int iTmp;
    char name[20];

    if(smem.cPedPushButton.GetPedisNCTU()==false && siDevCount <= 16){
        tmp = fopen("/cct/Data/SETTING/PedSwitchDevCount.txt","w");
        if(tmp == NULL){
            printf("open PedSwitchDevCount.txt error\n");
        }
        else fprintf(tmp,"enable=1\nDevCount=%02d",siDevCount);

        fclose(tmp);

        smem.cPedPushButton.SetDevCount(siDevCount);

        for(int i=0;i<smem.cPedPushButton.GetDevCount();i++){
            smem.cPedPushButton.SetDevID(i,i);
        }
//        system("reboot");
//
//        screenHWReset.DisplayHWReset();
    }

} catch(...) {}
}
//---------------------------------------------------------------------------
void SCREENPedSwitchStatus::doKeyF4Work(void)
{
    screenActuateArwenMenu.DisplayActuateArwenMenu();
}
//---------------------------------------------------------------------------
void SCREENPedSwitchStatus::doKeyEnterWork(void)
{
//    switch (cSelect+1) {
//        case 1:
//          doKey1Work();
//        break;
//        case 2:
//          doKey2Work();
//        break;
//        case 3:
//          doKey3Work();
//        break;
//        case 4:
//          doKey4Work();
//        break;
//        case 5:
//          doKey5Work();
//        break;
//        case 6:
//          doKey6Work();
//        break;
//        case 7:
//          doKey7Work();
//        break;
//        case 8:
//          doKey8Work();
//        break;
//        case 9:
//          doKey9Work();
//        break;
//        case 10:
//          doKey0Work();
//        break;
//        default:
//        break;
//    }
}
//---------------------------------------------------------------------------
void SCREENPedSwitchStatus::doKeyUPWork(void)
{
try {

  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENPedSwitchStatus::doKeyDOWNWork(void)
{
try {

  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENPedSwitchStatus::doKeyLEFTWork(void)
{
try {
        clearCursor8x16(markDevCount[cPosition].X,markDevCount[cPosition].Y+16);
        if(cPosition==0){
            cPosition=1;
        }else if(cPosition==1){
            cPosition=0;
        }
        setCursor8x16(markDevCount[cPosition].X,markDevCount[cPosition].Y+16);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENPedSwitchStatus::doKeyRIGHTWork(void)
{
try {
        clearCursor8x16(markDevCount[cPosition].X,markDevCount[cPosition].Y+16);
        if(cPosition==0){
            cPosition=1;
        }else if(cPosition==1){
            cPosition=0;
        }
        setCursor8x16(markDevCount[cPosition].X,markDevCount[cPosition].Y+16);
  } catch (...) {}
}
//---------------------------------------------------------------------------
