//---------------------------------------------------------------------------
#include "SCREENFWVerDT.h"
#include "SCREENMain.h"
//#include "SCREENCtlSetup.h"
#include "SCREENSystemInfoMenu.h"
#include "SCREENCmdSet.h"
#include "LCD240x128.h"
#include "SMEM.h"

#include "stdlib.h"

SCREENFWVerDT screenFWVerDT;
//---------------------------------------------------------------------------
SCREENFWVerDT::SCREENFWVerDT(void)
{
    LoadBitmapFromFile();
    InitDispWord();
}
//---------------------------------------------------------------------------
SCREENFWVerDT::~SCREENFWVerDT(void)
{
}
//---------------------------------------------------------------------------
void SCREENFWVerDT::DoKeyWork(BYTE key)              //已過濾,進來的是0x80~0x98之間,5X5按鍵
{
try {
    switch (key) {
        case 0x90:     //ESC,F1
          DoKeyF1Work();
        break;
        case 0x91:
          DoKeyF2Work();
        break;
        case 0x92:
          DoKeyF3Work();
        break;
        case 0x93:
          DoKeyF4Work();
        break;
        default:
        break;
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENFWVerDT::LoadBitmapFromFile(void)
{
try {
    FILE *bitmap;
    bitmap=fopen("//cct//bitmap//backGround//4_1_4.bit","rb");
    if (bitmap) {
        fread(fwVerDTBitmap,3840,1,bitmap);
        fclose(bitmap);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENFWVerDT::DisplayFWVerDT(void)
{
try {

    unsigned char ucFW51Year;
    unsigned char ucFW51Month;
    unsigned char ucFW51Day;

    smem.SetcFace(cFWVERDT);
    smem.vWriteMsgToDOM("Enter Firmware and Version Screen");
    lcd240x128.DISPLAY_GRAPHIC(0,fwVerDTBitmap,128,30);


    unsigned short verHi=smem.vGetFirmwareInfo_0FC3(4),verLo=smem.vGetFirmwareInfo_0FC3(5);
    if ( (verHi<=9) && (verLo<=9) ) {
        lcd240x128.DISPLAY_GRAPHIC_XY(version[0].X,version[0].Y,word8x16[verHi],version[0].height,version[0].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(version[1].X,version[1].Y,word8x16[verLo],version[1].height,version[1].width/8);
    }

    unsigned short cYear=smem.vGetFirmwareInfo_0FC3(0),cMonth=smem.vGetFirmwareInfo_0FC3(1),cDay=smem.vGetFirmwareInfo_0FC3(2);
    if (cYear>=1990 && cYear<=2038 && cMonth>=1 && cMonth<=12 && cDay>=1 && cDay<=31) {
        lcd240x128.DISPLAY_GRAPHIC_XY(fwDate[0].X,fwDate[0].Y,word8x16[cYear/1000],fwDate[0].height,fwDate[0].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(fwDate[1].X,fwDate[1].Y,word8x16[(cYear/100)%10],fwDate[1].height,fwDate[1].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(fwDate[2].X,fwDate[2].Y,word8x16[(cYear/10)%10],fwDate[2].height,fwDate[2].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(fwDate[3].X,fwDate[3].Y,word8x16[cYear%10],fwDate[3].height,fwDate[3].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(fwDate[4].X,fwDate[4].Y,word8x16[cMonth/10],fwDate[4].height,fwDate[4].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(fwDate[5].X,fwDate[5].Y,word8x16[cMonth%10],fwDate[5].height,fwDate[5].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(fwDate[6].X,fwDate[6].Y,word8x16[cDay/10],fwDate[6].height,fwDate[6].width/8);
        lcd240x128.DISPLAY_GRAPHIC_XY(fwDate[7].X,fwDate[7].Y,word8x16[cDay%10],fwDate[7].height,fwDate[7].width/8);
    }

    ucFW51Year = smem.vGetW77E58FWVer(0);
    ucFW51Month = smem.vGetW77E58FWVer(1);
    ucFW51Day = smem.vGetW77E58FWVer(2);

    if(ucFW51Year == 0 && ucFW51Month == 0 && ucFW51Day == 0) {
    } else {
      lcd240x128.DISPLAY_GRAPHIC_XY(fwW77E58[0].X,fwW77E58[0].Y,word8x16[2],fwW77E58[0].height,fwW77E58[0].width/8);
      lcd240x128.DISPLAY_GRAPHIC_XY(fwW77E58[1].X,fwW77E58[1].Y,word8x16[0],fwW77E58[1].height,fwW77E58[1].width/8);
      lcd240x128.DISPLAY_GRAPHIC_XY(fwW77E58[2].X,fwW77E58[2].Y,word8x16[ucFW51Year / 10],fwW77E58[2].height,fwW77E58[2].width/8);
      lcd240x128.DISPLAY_GRAPHIC_XY(fwW77E58[3].X,fwW77E58[3].Y,word8x16[ucFW51Year % 10],fwW77E58[3].height,fwW77E58[3].width/8);
      lcd240x128.DISPLAY_GRAPHIC_XY(fwW77E58[4].X,fwW77E58[4].Y,word8x16[ucFW51Month / 10],fwW77E58[4].height,fwW77E58[4].width/8);
      lcd240x128.DISPLAY_GRAPHIC_XY(fwW77E58[5].X,fwW77E58[5].Y,word8x16[ucFW51Month % 10],fwW77E58[5].height,fwW77E58[5].width/8);
      lcd240x128.DISPLAY_GRAPHIC_XY(fwW77E58[6].X,fwW77E58[6].Y,word8x16[ucFW51Day / 10],fwW77E58[6].height,fwW77E58[6].width/8);
      lcd240x128.DISPLAY_GRAPHIC_XY(fwW77E58[7].X,fwW77E58[7].Y,word8x16[ucFW51Day % 10],fwW77E58[7].height,fwW77E58[7].width/8);
    }

  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENFWVerDT::InitDispWord(void)
{
    version[0].X=112;
    version[0].Y=36;
    version[0].width=8;
    version[0].height=16;
    version[1].X=128;
    version[1].Y=36;
    version[1].width=8;
    version[1].height=16;

    for (int i=0;i<8;i++) {
         if (i>=0 && i<=3) fwDate[i].X=104+i*8;
         else if (i>=4 && i<=5) fwDate[i].X=152+(i-4)*8;
         else if (i>=6 && i<=7) fwDate[i].X=184+(i-6)*8;
         fwDate[i].Y=61;
         fwDate[i].width=8;
         fwDate[i].height=16;
    }

    for (int i=0;i<8;i++) {
         if (i>=0 && i<=3) fwW77E58[i].X=104+i*8;
         else if (i>=4 && i<=5) fwW77E58[i].X=152+(i-4)*8;
         else if (i>=6 && i<=7) fwW77E58[i].X=184+(i-6)*8;
         fwW77E58[i].Y = 77;
         fwW77E58[i].width=8;
         fwW77E58[i].height=16;
    }

}
//---------------------------------------------------------------------------
void SCREENFWVerDT::DoKeyF1Work(void)
{
    screenMain.DisplayMain();
}
//---------------------------------------------------------------------------
void SCREENFWVerDT::DoKeyF2Work(void)
{
try {
    system("sync");
    //system("rm -f /cct/Data/SETTING/*.txt");                 //清除change log
    system("sync");
    system("/cct/update.sh");                                //更換程式
//arwen modify    screenCtlSetup.DisplayCtlSetup();
    screenSystemInfoMenu.DisplaySystemInfoMenu();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENFWVerDT::DoKeyF3Work(void)
{
    screenCmdSet.DisplayCmdSet();
}
//---------------------------------------------------------------------------
void SCREENFWVerDT::DoKeyF4Work(void)
{
//arwen modify    screenCtlSetup.DisplayCtlSetup();
    screenSystemInfoMenu.DisplaySystemInfoMenu();
}
//---------------------------------------------------------------------------
