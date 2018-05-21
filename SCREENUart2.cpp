#include "SCREENUart2.h"
#include "SCREENMain.h"
#include "SCREENOperStat.h"
#include "SCREENLockdb.h"
#include "SCREENWarning.h"

#include "LCD240x128.h"
#include "SMEM.h"

#include "CDataToMessageOK.h"
#include "WRITEJOB.h"

SCREENUart2 screenUart2;
//---------------------------------------------------------------------------
SCREENUart2::SCREENUart2(void)
{
try {
    LoadBitmapFromFile();
    InitDispWord();
    cSelect=0;
  } catch (...) {}
}
//---------------------------------------------------------------------------
SCREENUart2::~SCREENUart2(void)
{
}
//---------------------------------------------------------------------------
void SCREENUart2::DoKeyWork(BYTE key)                                         //已過濾,進來的是0x80~0x98之間
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
        break;
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
        case 0x90://F1 ESC
          DoKeyF1Work();
        break;
        case 0x91:
        case 0x92:
          DoKeyF3Work();
        break;
        case 0x93://F4
          DoKeyF4Work();
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
        case 0x98://ENTER
          DoKeyEnterWork();
        break;
        default:
        break;
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENUart2::LoadBitmapFromFile(void)
{
try {
    FILE *bitmap;
    //bitmap=fopen("//cct//bitmap//backGround//01-08-F3-OTUart2Select.bit","rb");
    bitmap=fopen("//cct//bitmap//backGround//actmenu//ArwenUart2Select.bit","rb");//add Arwen
    if (bitmap) {
        fread(ucBitmap,3840,1,bitmap);
        fclose(bitmap);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENUart2::DisplayUart2Select(void)
{
try {

    int iTmp;

    smem.SetcFace(cUART2SELECT);
    smem.vWriteMsgToDOM("Enter Uart2 Setup Screen");
    lcd240x128.DISPLAY_GRAPHIC(0,ucBitmap,128,30);

    iTmp = smem.vGetINTData(Com2_TYPE);

    if(iTmp == Com2IsGPSPort) {
      cSelect = 1;
    } else if(iTmp == Com2IsRedCount) {
      cSelect = 0;
    } else if(iTmp == Com2IsPedestrianPushButton) {//add Arwen
      cSelect = 2;
    } else if(iTmp == Com2IsRedCountAndCom1IsPedPushButton){//add Arwen
      cSelect = 3;
    } else if(iTmp == Com2IsHOLUXGPSPort){
      cSelect = 5;
    }

    if(cSelect >= 6) {
      cSelect = 0;
    }
    else {
        setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(cycle.X,cycle.Y,word24x24[cSelect],cycle.height,cycle.width/8);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENUart2::InitDispWord(void)
{
try {
    cycle.X=144;
    cycle.Y=5;
    cycle.width=24;
    cycle.height=24;

    for (int i=0;i<6;i++) {
         if (i>=0 && i<=2) {
             mark[i].X=24;
             mark[i].Y=44+i*19;
         } else if (i>=3 && i<=5) {
             mark[i].X=128;
             mark[i].Y=44+(i-3)*19;
         }
         mark[i].width=16;
         mark[i].height=16;
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENUart2::DoKey0Work(void)
{
    if (cSelect<6) clearSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    cSelect=0;
    setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(cycle.X,cycle.Y,word24x24[cSelect],cycle.height,cycle.width/8);
}
//---------------------------------------------------------------------------
void SCREENUart2::DoKey1Work(void)
{
    if (cSelect<6) clearSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    cSelect=1;
    setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(cycle.X,cycle.Y,word24x24[cSelect],cycle.height,cycle.width/8);
}
//---------------------------------------------------------------------------
void SCREENUart2::DoKey2Work(void)
{

    if (cSelect<6) clearSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    cSelect=2;
    setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(cycle.X,cycle.Y,word24x24[cSelect],cycle.height,cycle.width/8);

}
//---------------------------------------------------------------------------
void SCREENUart2::DoKey3Work(void)
{

    if (cSelect<6) clearSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    cSelect=3;
    setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(cycle.X,cycle.Y,word24x24[cSelect],cycle.height,cycle.width/8);

}
//---------------------------------------------------------------------------
void SCREENUart2::DoKey4Work(void)
{
/*
    if (cSelect<6) clearSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    cSelect=4;
    setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(cycle.X,cycle.Y,word24x24[cSelect],cycle.height,cycle.width/8);
*/
}
//---------------------------------------------------------------------------
void SCREENUart2::DoKey5Work(void)
{

    if (cSelect<6) clearSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    cSelect=5;
    setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(cycle.X,cycle.Y,word24x24[cSelect],cycle.height,cycle.width/8);

}
//---------------------------------------------------------------------------
void SCREENUart2::DoKeyF1Work(void)
{
    screenMain.DisplayMain();
}
//---------------------------------------------------------------------------
void SCREENUart2::DoKeyF4Work(void)
{
    screenOperStat.DisplayOperStat();
}
//---------------------------------------------------------------------------
void SCREENUart2::DoKeyF3Work(void)
{
try {

  } catch (...) {}
}

//---------------------------------------------------------------------------
void SCREENUart2::DoKeyEnterWork(void)
{
try {
  smem.SetLastFace(cUART2SELECT);

  if (cSelect==1) {
    smem.vWriteMsgToDOM("Set Uart2 To GPS");
    smem.vSetINTData(Com2_TYPE, Com2IsGPSPort);
  }
  else if (cSelect==0) {
    smem.vWriteMsgToDOM("Set Uart2 To RedCount");
    smem.vSetINTData(Com2_TYPE, Com2IsRedCount);
  }else if (cSelect==2) {//add arwen
    smem.vWriteMsgToDOM("Set Uart2 To Pedestrian");
    smem.vSetINTData(Com2_TYPE,Com2IsPedestrianPushButton);
  }else if (cSelect==3) {//add arwen
    smem.vWriteMsgToDOM("Set Uart2 To RedCount And Uart1 To Pedestrian");
    smem.vSetINTData(Com2_TYPE,Com2IsRedCountAndCom1IsPedPushButton);
  }else if(cSelect==5) {
    smem.vWriteMsgToDOM("Set Uart2 To HOLUX GPS");
    smem.vSetINTData(Com2_TYPE,Com2IsHOLUXGPSPort);
  }

  screenWarning.DisplayWarning();


  } catch (...) {}
}

//---------------------------------------------------------------------------
void SCREENUart2::DoKeyDOWNWork(void)
{
try {
    if (cSelect<6) clearSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    cSelect++;
    if (cSelect>5) cSelect=5;
    setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(cycle.X,cycle.Y,word24x24[cSelect],cycle.height,cycle.width/8);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENUart2::DoKeyUPWork(void)
{
try {
    if (cSelect<6) clearSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    if (cSelect==0 || cSelect>=6) cSelect=1;
    cSelect--;
    setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(cycle.X,cycle.Y,word24x24[cSelect],cycle.height,cycle.width/8);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENUart2::DoKeyLEFTWork(void)
{
try {
/*
    if (cSelect<3) clearSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    if (cSelect>=3 && cSelect<=5) cSelect-=3;
    if (cSelect<3) setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    if (cSelect<3) lcd240x128.DISPLAY_GRAPHIC_XY(cycle.X,cycle.Y,word24x24[cSelect],cycle.height,cycle.width/8);
*/
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENUart2::DoKeyRIGHTWork(void)
{
try {
/*
    if (cSelect<3) clearSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    if (cSelect>=0 && cSelect<=2) cSelect+=3;
    if (cSelect<3) setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    if (cSelect<3) lcd240x128.DISPLAY_GRAPHIC_XY(cycle.X,cycle.Y,word24x24[cSelect],cycle.height,cycle.width/8);
*/
  } catch (...) {}
}
//---------------------------------------------------------------------------
