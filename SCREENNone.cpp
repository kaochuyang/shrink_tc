//---------------------------------------------------------------------------
#include "SCREENNone.h"
#include "LCD240x128.h"
#include "SMEM.h"
#include "SCREENLogo.h"

SCREENNone screenNone;
//---------------------------------------------------------------------------
SCREENNone::SCREENNone(void)
{
    for (int i=0;i<3840;i++)  noneBitmap[i]=0;
}
//---------------------------------------------------------------------------
SCREENNone::~SCREENNone(void)
{
}
//---------------------------------------------------------------------------
void SCREENNone::DoKeyWork(BYTE key)                                            //已過濾,進來的是0x80~0x98之間,5X5按鍵
{
try {
    switch (key) {
        default:
          DoKeyDefaultWork();
        break;
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENNone::DisplayNone(void)
{
try {
    if (smem.GetcFace()!=cNONE) {   //上面是為了避免作傻事,因為timer當沒有人來按時會一直來refresh一次
        smem.SetcFace(cNONE);
        lcd240x128.DISPLAY_GRAPHIC(0,noneBitmap,128,30);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENNone::DoKeyDefaultWork(void)
{
    screenLogo.DisplayLogo();
}
//---------------------------------------------------------------------------
