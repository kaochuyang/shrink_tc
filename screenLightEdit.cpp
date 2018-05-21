#include "screenLightEdit.h"
#include "screenSegtype.h"
#include "screenSegtypeEditF2.h"
#include "screenRunTableList.h"
#include "SCREENMain.h"
#include "screenWeekDay.h"
#include "LCD240x128.h"
#include "CSTC.h"
//#include "screenLightEditEditStep1.h"

#include "screenPlan.h"
#include "screenLightPreview.h"

#include <stdio.h>

#include "SMEM.h"
//---------------------------------------------------------------------------
ScreenLightEdit screenLightEdit;
//---------------------------------------------------------------------------
ScreenLightEdit::ScreenLightEdit(void)
{
    loadBitmapFromFile();
    initDispWord();
    face=1;
    usiCurrentSubphaseNo = 0;
    usiCurrentStepNo = 0;
}
//---------------------------------------------------------------------------
ScreenLightEdit::~ScreenLightEdit(void)
{
}
//---------------------------------------------------------------------------
void ScreenLightEdit::loadBitmapFromFile(void)
{
try {
    FILE *bitmap;
    bitmap=fopen("//cct//bitmap//backGround//LightEdit.bit","rb");
    if (bitmap) {
        fread(lightEditBitmap,3840,1,bitmap);
        fclose(bitmap);
    }

  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenLightEdit::DisplayLightEdit(int phaseID, unsigned short int usiPhaseViewID)
{
try {
    smem.SetcFace(cLIGHTEDIT);

    usiFromWhereInLight = usiPhaseViewID;

    cSelPosition = 0;
    cPosition = 0;

    usiCurrentSubphaseNo = 0;
    usiCurrentStepNo = 0;
    lcd240x128.DISPLAY_GRAPHIC(0,lightEditBitmap,128,30);
    LoadLight(phaseID);
    DisplayLight();

    setCursor8x16(Light[cPosition/8][cPosition%8].X, Light[cPosition/8][cPosition%8].Y+15);

  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenLightEdit::initDispWord(void)
{
    for(int i = 0; i < 6; i++) {
      for(int j = 0; j < 8; j++) {
        Light[i][j].X = 8 + 16*i;
        Light[i][j].Y = j*16;
        Light[i][j].width = 8;
        Light[i][j].height = 16;
      }
    }

    for(int i = 0; i < 2; i++) {
      PhaseID[i].X = 184 + 8*i;
      PhaseID[i].Y = 16;
      PhaseID[i].width = 8;
      PhaseID[i].height = 16;

      SubPhaseID[i].X = 148 + 16*i;
      SubPhaseID[i].Y = 32;
      SubPhaseID[i].width = 8;
      SubPhaseID[i].height = 16;

      StepID[i].X = 148 + 16*i;
      StepID[i].Y = 48;
      StepID[i].width = 8;
      StepID[i].height = 16;

    }

    SignalCount.X = 160;
    SignalCount.Y = 64;
    SignalCount.width = 8;
    SignalCount.height = 16;



}
//---------------------------------------------------------------------------
void ScreenLightEdit::LoadLight(int PhaseID)
{
try {
  stc.Lock_to_Load_Phase_for_Panel(PhaseID);
  for(int i = 0; i < 8; i++) {                                                  //init
    for(int j = 0; j < 5; j++) {
      for(int k = 0; k < 8; k++) {
        usiLight[i][j][k] = 0;
      }
    }
  }
  for(int i = 0; i < 8; i++) {
      ucStepCount[i] = 5;
  }

  ucPhaseID = stc._panel_phase._phase_order;
  ucSignalMap = stc._panel_phase._signal_map;
  ucSignalCount = stc._panel_phase._signal_count;
  ucSubphaseCount = stc._panel_phase._subphase_count;
  for(int i = 0; i < ucSubphaseCount; i++) {
    ucStepCount[i] = stc._panel_phase._ptr_subphase_step_count[i];
  }


  for(int i = 0; i < ucSubphaseCount; i++) {
    for(int j = 0; j < ucStepCount[i]; j++) {
      for(int k = 0; k < ucSignalCount; k++) {
        usiLight[i][j][k] = stc._panel_phase._ptr_subphase_step_signal_status[i][j][k];
      }
    }
  }

  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenLightEdit::DisplayLight(void)
{
try {

  unsigned short int usiTMP;
  int iSignalStatus[16];

//  lcd240x128.DISPLAY_GRAPHIC(0,lightEditBitmap,128,30);

  for(int i = 0; i < ucSignalCount; i++) {
    usiTMP = usiLight[usiCurrentSubphaseNo][usiCurrentStepNo][i];

    for(int j = 0; j < 16; j++) {
      iSignalStatus[j] = ((usiTMP&0x0001)==0) ? 0 : 1;                          // 0x80 => 10000000 (1byte)
      usiTMP = usiTMP >> 1;

      if(j % 2 == 1) {
        if(iSignalStatus[j-1] == 1 && iSignalStatus[j] == 1) {                  //shining
          lcd240x128.DISPLAY_GRAPHIC_XY(Light[i][j/2].X,Light[i][j/2].Y,word8x16Light,Light[i][j/2].height,Light[i][j/2].width/8);
        } else if(iSignalStatus[j-1] == 0 && iSignalStatus[j] == 0) {          //black
          lcd240x128.DISPLAY_GRAPHIC_XY(Light[i][j/2].X,Light[i][j/2].Y,word8x16Die,Light[i][j/2].height,Light[i][j/2].width/8);
        } else {
          lcd240x128.DISPLAY_GRAPHIC_XY(Light[i][j/2].X,Light[i][j/2].Y,word8x16Flash,Light[i][j/2].height,Light[i][j/2].width/8);
        }
      }
    }

  }

  //Phase
  lcd240x128.DISPLAY_GRAPHIC_XY(PhaseID[0].X,PhaseID[0].Y,word8x16[(ucPhaseID>>4)&0xFF],PhaseID[0].height,PhaseID[0].width/8);
  lcd240x128.DISPLAY_GRAPHIC_XY(PhaseID[1].X,PhaseID[1].Y,word8x16[(ucPhaseID)&0x0F],PhaseID[1].height,PhaseID[1].width/8);

  //SubPhase
  lcd240x128.DISPLAY_GRAPHIC_XY(SubPhaseID[0].X,SubPhaseID[0].Y,word8x16[usiCurrentSubphaseNo+1],SubPhaseID[0].height,SubPhaseID[0].width/8);
  lcd240x128.DISPLAY_GRAPHIC_XY(SubPhaseID[1].X,SubPhaseID[1].Y,word8x16[ucSubphaseCount],SubPhaseID[1].height,SubPhaseID[1].width/8);

  //Step
  lcd240x128.DISPLAY_GRAPHIC_XY(StepID[0].X,StepID[0].Y,word8x16[usiCurrentStepNo+1],StepID[0].height,StepID[0].width/8);
  lcd240x128.DISPLAY_GRAPHIC_XY(StepID[1].X,StepID[1].Y,word8x16[ucStepCount[usiCurrentSubphaseNo]],StepID[1].height,StepID[1].width/8);

  //SignalCount
  lcd240x128.DISPLAY_GRAPHIC_XY(SignalCount.X,SignalCount.Y,word8x16[ucSignalCount],SignalCount.height,SignalCount.width/8);

  if(cSelPosition == 0) {
    setCursor8x16(Light[cPosition/8][cPosition%8].X, Light[cPosition/8][cPosition%8].Y+15);
  } else if(cSelPosition == 1) {
    setCursor8x16(PhaseID[cPosition].X, PhaseID[cPosition].Y+16);
  } else if(cSelPosition == 2) {
    setCursor8x16(SubPhaseID[cPosition].X, SubPhaseID[cPosition].Y+16);
  } else if(cSelPosition == 4) {
    setCursor8x16(SignalCount.X, SignalCount.Y+16);
  }

  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenLightEdit::SavePlan(void)
{
}

//---------------------------------------------------------------------------
void ScreenLightEdit::doKeyWork(BYTE key)
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
void ScreenLightEdit::doKey0Work(void)
{
  unsigned short int usiTMP = 0xFFFC;
  unsigned short int usiLIGHTTYPE = 0;
  if(cSelPosition == 0) {
    printf("usiTMP:%X\n", usiTMP);
    usiLIGHTTYPE = cPosition % 8;
    for(int i = 0; i < usiLIGHTTYPE; i++) {
      usiTMP = usiTMP << 2;
      usiTMP += 0x03;
      printf("usiLIGHTTYPE:%d move usiTMP:%X\n", usiLIGHTTYPE, usiTMP);
    }
    printf("start:%X\n",     usiLight[usiCurrentSubphaseNo][usiCurrentStepNo][cPosition/8]);
    usiLight[usiCurrentSubphaseNo][usiCurrentStepNo][cPosition/8] = usiLight[usiCurrentSubphaseNo][usiCurrentStepNo][cPosition/8] & usiTMP;
    printf("end:%X\n",     usiLight[usiCurrentSubphaseNo][usiCurrentStepNo][cPosition/8]);
    doKeyDOWNWork();
  } else if(cSelPosition == 1) {
    if(cPosition==0) {
      ucPhaseID = ucPhaseID & 0x0F;
      lcd240x128.DISPLAY_GRAPHIC_XY(PhaseID[cPosition].X,PhaseID[cPosition].Y,word8x16[0],PhaseID[cPosition].height,PhaseID[cPosition].width/8);
    } else if(cPosition==1) {
      ucPhaseID = ucPhaseID & 0xF0;
      lcd240x128.DISPLAY_GRAPHIC_XY(PhaseID[cPosition].X,PhaseID[cPosition].Y,word8x16[0],PhaseID[cPosition].height,PhaseID[cPosition].width/8);
    }
    doKeyRIGHTWork();
  } else if(cSelPosition == 2) {
  } else if(cSelPosition == 3) {
  } else if(cSelPosition == 4) {
  }
}
//---------------------------------------------------------------------------
void ScreenLightEdit::doKey1Work(void)
{
  unsigned short int usiTMP = 0x03;
  unsigned short int usiLIGHTTYPE = 0;
  if(cSelPosition == 0) {
    printf("usiTMP:%X\n", usiTMP);
    usiLIGHTTYPE = cPosition % 8;
    for(int i = 0; i < usiLIGHTTYPE; i++) {
      usiTMP = usiTMP << 2;
      printf("usiLIGHTTYPE:%d move usiTMP:%X\n", usiLIGHTTYPE, usiTMP);
    }
    printf("start:%X\n",     usiLight[usiCurrentSubphaseNo][usiCurrentStepNo][cPosition/8]);
    usiLight[usiCurrentSubphaseNo][usiCurrentStepNo][cPosition/8] = usiLight[usiCurrentSubphaseNo][usiCurrentStepNo][cPosition/8] | usiTMP;
    printf("end:%X\n",     usiLight[usiCurrentSubphaseNo][usiCurrentStepNo][cPosition/8]);
    doKeyDOWNWork();

  } else if(cSelPosition == 1) {
    if(cPosition==0) {
      ucPhaseID = ucPhaseID & 0x0F;
      ucPhaseID += 0x10;
      lcd240x128.DISPLAY_GRAPHIC_XY(PhaseID[cPosition].X,PhaseID[cPosition].Y,word8x16[1],PhaseID[cPosition].height,PhaseID[cPosition].width/8);
    } else if(cPosition==1) {
      ucPhaseID = ucPhaseID & 0xF0;
      ucPhaseID += 0x01;
      lcd240x128.DISPLAY_GRAPHIC_XY(PhaseID[cPosition].X,PhaseID[cPosition].Y,word8x16[1],PhaseID[cPosition].height,PhaseID[cPosition].width/8);
    }
    doKeyRIGHTWork();
  } else if(cSelPosition == 2) {
    if(cPosition == 1) {
      ucSubphaseCount = 1;
      lcd240x128.DISPLAY_GRAPHIC_XY(SubPhaseID[cPosition].X,SubPhaseID[cPosition].Y,word8x16[1],SubPhaseID[cPosition].height,SubPhaseID[cPosition].width/8);
    }
    doKeyDOWNWork();
  } else if(cSelPosition == 4) {
      ucSignalCount = 1;
      lcd240x128.DISPLAY_GRAPHIC_XY(SignalCount.X,SignalCount.Y,word8x16[1],SignalCount.height,SignalCount.width/8);
      doKeyDOWNWork();
  }

}
//---------------------------------------------------------------------------
void ScreenLightEdit::doKey2Work(void)
{
  unsigned short int usiTMP  = 0xFFFC;
  unsigned short int usiTMP2 = 0x02;
  unsigned short int usiLIGHTTYPE = 0;
  if(cSelPosition == 0) {
    printf("usiTMP:%X\n", usiTMP);
    usiLIGHTTYPE = cPosition % 8;
    for(int i = 0; i < usiLIGHTTYPE; i++) {
      usiTMP = usiTMP << 2;
      usiTMP += 0x03;
      usiTMP2 = usiTMP2 << 2;
      printf("usiLIGHTTYPE:%d move usiTMP:%X usiTMP2:%X\n", usiLIGHTTYPE, usiTMP, usiTMP2);
    }
    printf("start:%X\n",     usiLight[usiCurrentSubphaseNo][usiCurrentStepNo][cPosition/8]);
    usiLight[usiCurrentSubphaseNo][usiCurrentStepNo][cPosition/8] = usiLight[usiCurrentSubphaseNo][usiCurrentStepNo][cPosition/8] & usiTMP;
    usiLight[usiCurrentSubphaseNo][usiCurrentStepNo][cPosition/8] = usiLight[usiCurrentSubphaseNo][usiCurrentStepNo][cPosition/8] | usiTMP2;
    printf("end:%X\n",     usiLight[usiCurrentSubphaseNo][usiCurrentStepNo][cPosition/8]);
    doKeyDOWNWork();
  } else if(cSelPosition == 1) {
    if(cPosition==0) {
      ucPhaseID = ucPhaseID & 0x0F;
      ucPhaseID += 0x20;
      lcd240x128.DISPLAY_GRAPHIC_XY(PhaseID[cPosition].X,PhaseID[cPosition].Y,word8x16[2],PhaseID[cPosition].height,PhaseID[cPosition].width/8);
    } else if(cPosition==1) {
      ucPhaseID = ucPhaseID & 0xF0;
      ucPhaseID += 0x02;
      lcd240x128.DISPLAY_GRAPHIC_XY(PhaseID[cPosition].X,PhaseID[cPosition].Y,word8x16[2],PhaseID[cPosition].height,PhaseID[cPosition].width/8);
    }
    doKeyRIGHTWork();
  } else if(cSelPosition == 2) {
    if(cPosition == 1) {
      ucSubphaseCount = 2;
      lcd240x128.DISPLAY_GRAPHIC_XY(SubPhaseID[cPosition].X,SubPhaseID[cPosition].Y,word8x16[2],SubPhaseID[cPosition].height,SubPhaseID[cPosition].width/8);
    }
    doKeyDOWNWork();
  } else if(cSelPosition == 4) {
      ucSignalCount = 2;
      lcd240x128.DISPLAY_GRAPHIC_XY(SignalCount.X,SignalCount.Y,word8x16[2],SignalCount.height,SignalCount.width/8);
      doKeyDOWNWork();
  }

}
//---------------------------------------------------------------------------
void ScreenLightEdit::doKey3Work(void)
{
  if(cSelPosition == 1) {
    if(cPosition==0) {
      ucPhaseID = ucPhaseID & 0x0F;
      ucPhaseID += 0x30;
      lcd240x128.DISPLAY_GRAPHIC_XY(PhaseID[cPosition].X,PhaseID[cPosition].Y,word8x16[3],PhaseID[cPosition].height,PhaseID[cPosition].width/8);
    } else if(cPosition==1) {
      ucPhaseID = ucPhaseID & 0xF0;
      ucPhaseID += 0x03;
      lcd240x128.DISPLAY_GRAPHIC_XY(PhaseID[cPosition].X,PhaseID[cPosition].Y,word8x16[3],PhaseID[cPosition].height,PhaseID[cPosition].width/8);
    }
    doKeyRIGHTWork();
  } else if(cSelPosition == 2) {
    if(cPosition == 1) {
      ucSubphaseCount = 3;
      lcd240x128.DISPLAY_GRAPHIC_XY(SubPhaseID[cPosition].X,SubPhaseID[cPosition].Y,word8x16[3],SubPhaseID[cPosition].height,SubPhaseID[cPosition].width/8);
    }
    doKeyDOWNWork();
  } else if(cSelPosition == 4) {
      ucSignalCount = 3;
      lcd240x128.DISPLAY_GRAPHIC_XY(SignalCount.X,SignalCount.Y,word8x16[3],SignalCount.height,SignalCount.width/8);
      doKeyDOWNWork();
  }

}
//---------------------------------------------------------------------------
void ScreenLightEdit::doKey4Work(void)
{

  if(cSelPosition == 1) {
    if(cPosition==0) {
      ucPhaseID = ucPhaseID & 0x0F;
      ucPhaseID += 0x40;
      lcd240x128.DISPLAY_GRAPHIC_XY(PhaseID[cPosition].X,PhaseID[cPosition].Y,word8x16[4],PhaseID[cPosition].height,PhaseID[cPosition].width/8);
    } else if(cPosition==1) {
      ucPhaseID = ucPhaseID & 0xF0;
      ucPhaseID += 0x04;
      lcd240x128.DISPLAY_GRAPHIC_XY(PhaseID[cPosition].X,PhaseID[cPosition].Y,word8x16[4],PhaseID[cPosition].height,PhaseID[cPosition].width/8);
    }
    doKeyRIGHTWork();
  } else if(cSelPosition == 2) {
    if(cPosition == 1) {
      ucSubphaseCount = 4;
      lcd240x128.DISPLAY_GRAPHIC_XY(SubPhaseID[cPosition].X,SubPhaseID[cPosition].Y,word8x16[4],SubPhaseID[cPosition].height,SubPhaseID[cPosition].width/8);
    }
    doKeyDOWNWork();
  } else if(cSelPosition == 4) {
      ucSignalCount = 4;
      lcd240x128.DISPLAY_GRAPHIC_XY(SignalCount.X,SignalCount.Y,word8x16[4],SignalCount.height,SignalCount.width/8);
      doKeyDOWNWork();
  }

}

//---------------------------------------------------------------------------
void ScreenLightEdit::doKey5Work(void)
{
  if(cSelPosition == 1) {
    if(cPosition==0) {
      ucPhaseID = ucPhaseID & 0x0F;
      ucPhaseID += 0x50;
      lcd240x128.DISPLAY_GRAPHIC_XY(PhaseID[cPosition].X,PhaseID[cPosition].Y,word8x16[5],PhaseID[cPosition].height,PhaseID[cPosition].width/8);
    } else if(cPosition==1) {
      ucPhaseID = ucPhaseID & 0xF0;
      ucPhaseID += 0x05;
      lcd240x128.DISPLAY_GRAPHIC_XY(PhaseID[cPosition].X,PhaseID[cPosition].Y,word8x16[5],PhaseID[cPosition].height,PhaseID[cPosition].width/8);
    }
    doKeyRIGHTWork();
  } else if(cSelPosition == 2) {
    if(cPosition == 1) {
      ucSubphaseCount = 5;
      lcd240x128.DISPLAY_GRAPHIC_XY(SubPhaseID[cPosition].X,SubPhaseID[cPosition].Y,word8x16[5],SubPhaseID[cPosition].height,SubPhaseID[cPosition].width/8);
    }
    doKeyDOWNWork();
  } else if(cSelPosition == 4) {
      ucSignalCount = 5;
      lcd240x128.DISPLAY_GRAPHIC_XY(SignalCount.X,SignalCount.Y,word8x16[5],SignalCount.height,SignalCount.width/8);
      doKeyDOWNWork();
  }

}
//---------------------------------------------------------------------------
void ScreenLightEdit::doKey6Work(void)
{
  if(cSelPosition == 1) {
    if(cPosition==0) {
      ucPhaseID = ucPhaseID & 0x0F;
      ucPhaseID += 0x60;
      lcd240x128.DISPLAY_GRAPHIC_XY(PhaseID[cPosition].X,PhaseID[cPosition].Y,word8x16[6],PhaseID[cPosition].height,PhaseID[cPosition].width/8);
    } else if(cPosition==1) {
      ucPhaseID = ucPhaseID & 0xF0;
      ucPhaseID += 0x06;
      lcd240x128.DISPLAY_GRAPHIC_XY(PhaseID[cPosition].X,PhaseID[cPosition].Y,word8x16[6],PhaseID[cPosition].height,PhaseID[cPosition].width/8);
    }
    doKeyRIGHTWork();
  } else if(cSelPosition == 2) {
    if(cPosition == 1) {
      ucSubphaseCount = 6;
      lcd240x128.DISPLAY_GRAPHIC_XY(SubPhaseID[cPosition].X,SubPhaseID[cPosition].Y,word8x16[6],SubPhaseID[cPosition].height,SubPhaseID[cPosition].width/8);
    }
    doKeyDOWNWork();
  } else if(cSelPosition == 4) {
      ucSignalCount = 6;
      lcd240x128.DISPLAY_GRAPHIC_XY(SignalCount.X,SignalCount.Y,word8x16[6],SignalCount.height,SignalCount.width/8);
      doKeyDOWNWork();
  }

}
//---------------------------------------------------------------------------
void ScreenLightEdit::doKey7Work(void)
{
  if(cSelPosition == 1) {
    if(cPosition==0) {
      ucPhaseID = ucPhaseID & 0x0F;
      ucPhaseID += 0x70;
      lcd240x128.DISPLAY_GRAPHIC_XY(PhaseID[cPosition].X,PhaseID[cPosition].Y,word8x16[7],PhaseID[cPosition].height,PhaseID[cPosition].width/8);
    } else if(cPosition==1) {
      ucPhaseID = ucPhaseID & 0xF0;
      ucPhaseID += 0x07;
      lcd240x128.DISPLAY_GRAPHIC_XY(PhaseID[cPosition].X,PhaseID[cPosition].Y,word8x16[7],PhaseID[cPosition].height,PhaseID[cPosition].width/8);
    }
    doKeyRIGHTWork();
  } else if(cSelPosition == 2) {
    if(cPosition == 1) {
      ucSubphaseCount = 7;
      lcd240x128.DISPLAY_GRAPHIC_XY(SubPhaseID[cPosition].X,SubPhaseID[cPosition].Y,word8x16[7],SubPhaseID[cPosition].height,SubPhaseID[cPosition].width/8);
    }
    doKeyDOWNWork();
  }

}
//---------------------------------------------------------------------------
void ScreenLightEdit::doKey8Work(void)
{
  if(cSelPosition == 1) {
    if(cPosition==0) {
      ucPhaseID = ucPhaseID & 0x0F;
      ucPhaseID += 0x80;
      lcd240x128.DISPLAY_GRAPHIC_XY(PhaseID[cPosition].X,PhaseID[cPosition].Y,word8x16[8],PhaseID[cPosition].height,PhaseID[cPosition].width/8);
    } else if(cPosition==1) {
      ucPhaseID = ucPhaseID & 0xF0;
      ucPhaseID += 0x08;
      lcd240x128.DISPLAY_GRAPHIC_XY(PhaseID[cPosition].X,PhaseID[cPosition].Y,word8x16[8],PhaseID[cPosition].height,PhaseID[cPosition].width/8);
    }
    doKeyRIGHTWork();
  } else if(cSelPosition == 2) {
    if(cPosition == 1) {
      ucSubphaseCount = 8;
      lcd240x128.DISPLAY_GRAPHIC_XY(SubPhaseID[cPosition].X,SubPhaseID[cPosition].Y,word8x16[8],SubPhaseID[cPosition].height,SubPhaseID[cPosition].width/8);
    }
    doKeyDOWNWork();
  }

}
//---------------------------------------------------------------------------
void ScreenLightEdit::doKey9Work(void)
{
  if(cSelPosition == 1) {
    if(cPosition==0) {
      ucPhaseID = ucPhaseID & 0x0F;
      ucPhaseID += 0x90;
      lcd240x128.DISPLAY_GRAPHIC_XY(PhaseID[cPosition].X,PhaseID[cPosition].Y,word8x16[9],PhaseID[cPosition].height,PhaseID[cPosition].width/8);
    } else if(cPosition==1) {
      ucPhaseID = ucPhaseID & 0xF0;
      ucPhaseID += 0x09;
      lcd240x128.DISPLAY_GRAPHIC_XY(PhaseID[cPosition].X,PhaseID[cPosition].Y,word8x16[9],PhaseID[cPosition].height,PhaseID[cPosition].width/8);
    }
  }
  doKeyRIGHTWork();
}
//---------------------------------------------------------------------------
void ScreenLightEdit::doKeyAWork(void)
{
  if(cSelPosition == 1) {
    if(cPosition==0) {
      ucPhaseID = ucPhaseID & 0x0F;
      ucPhaseID += 0xA0;
      lcd240x128.DISPLAY_GRAPHIC_XY(PhaseID[cPosition].X,PhaseID[cPosition].Y,word8x16[10],PhaseID[cPosition].height,PhaseID[cPosition].width/8);
    } else if(cPosition==1) {
      ucPhaseID = ucPhaseID & 0xF0;
      ucPhaseID += 0x0A;
      lcd240x128.DISPLAY_GRAPHIC_XY(PhaseID[cPosition].X,PhaseID[cPosition].Y,word8x16[10],PhaseID[cPosition].height,PhaseID[cPosition].width/8);
    }
  }
  doKeyRIGHTWork();
}
//---------------------------------------------------------------------------
void ScreenLightEdit::doKeyBWork(void)
{
  if(cSelPosition == 1) {
    if(cPosition==0) {
      ucPhaseID = ucPhaseID & 0x0F;
      ucPhaseID += 0xB0;
      lcd240x128.DISPLAY_GRAPHIC_XY(PhaseID[cPosition].X,PhaseID[cPosition].Y,word8x16[11],PhaseID[cPosition].height,PhaseID[cPosition].width/8);
    } else if(cPosition==1) {
      ucPhaseID = ucPhaseID & 0xF0;
      ucPhaseID += 0x0B;
      lcd240x128.DISPLAY_GRAPHIC_XY(PhaseID[cPosition].X,PhaseID[cPosition].Y,word8x16[11],PhaseID[cPosition].height,PhaseID[cPosition].width/8);
    }
  }
  doKeyRIGHTWork();

}
//---------------------------------------------------------------------------
void ScreenLightEdit::doKeyCWork(void)
{
  bool bPhaseStatus;
  int iQueryPhaseOrder;
  iQueryPhaseOrder = ucPhaseID;

  if(cSelPosition == 1) {
    if(cPosition==0) {
      ucPhaseID = ucPhaseID & 0x0F;
      ucPhaseID += 0xC0;
      lcd240x128.DISPLAY_GRAPHIC_XY(PhaseID[cPosition].X,PhaseID[cPosition].Y,word8x16[12],PhaseID[cPosition].height,PhaseID[cPosition].width/8);
    } else if(cPosition==1) {
      ucPhaseID = ucPhaseID & 0xF0;
      ucPhaseID += 0x0C;
      lcd240x128.DISPLAY_GRAPHIC_XY(PhaseID[cPosition].X,PhaseID[cPosition].Y,word8x16[12],PhaseID[cPosition].height,PhaseID[cPosition].width/8);
    }
  doKeyRIGHTWork();
  } else {
    while(1) {
      if(iQueryPhaseOrder != 0) iQueryPhaseOrder -= 1;
      else iQueryPhaseOrder = 0xFF;

      bPhaseStatus = smem.vGetTCPhasePlanSegTypeData(TC_Phase, iQueryPhaseOrder);
      if(bPhaseStatus) { break; }
    }

    usiCurrentSubphaseNo = 0;
    usiCurrentStepNo = 0;
    lcd240x128.DISPLAY_GRAPHIC(0,lightEditBitmap,128,30);
    LoadLight(iQueryPhaseOrder);
    DisplayLight();
  }
}
//---------------------------------------------------------------------------
void ScreenLightEdit::doKeyDWork(void)
{
  bool bPhaseStatus;
  int iQueryPhaseOrder;
  iQueryPhaseOrder = ucPhaseID;

  if(cSelPosition == 1) {
    if(cPosition==0) {
      ucPhaseID = ucPhaseID & 0x0F;
      ucPhaseID += 0xD0;
      lcd240x128.DISPLAY_GRAPHIC_XY(PhaseID[cPosition].X,PhaseID[cPosition].Y,word8x16[13],PhaseID[cPosition].height,PhaseID[cPosition].width/8);
    } else if(cPosition==1) {
      ucPhaseID = ucPhaseID & 0xF0;
      ucPhaseID += 0x0D;
      lcd240x128.DISPLAY_GRAPHIC_XY(PhaseID[cPosition].X,PhaseID[cPosition].Y,word8x16[13],PhaseID[cPosition].height,PhaseID[cPosition].width/8);
    }
  doKeyRIGHTWork();
  } else {
    while(1) {
      if(iQueryPhaseOrder != 0xFF) iQueryPhaseOrder += 1;
      else iQueryPhaseOrder = 0;

      bPhaseStatus = smem.vGetTCPhasePlanSegTypeData(TC_Phase, iQueryPhaseOrder);
      if(bPhaseStatus) { break; }
    }

    usiCurrentSubphaseNo = 0;
    usiCurrentStepNo = 0;
    lcd240x128.DISPLAY_GRAPHIC(0,lightEditBitmap,128,30);
    LoadLight(iQueryPhaseOrder);
    DisplayLight();
  }

}
//---------------------------------------------------------------------------
void ScreenLightEdit::doKeyEWork(void)
{
  if(cSelPosition == 1) {
    if(cPosition==0) {
      ucPhaseID = ucPhaseID & 0x0F;
      ucPhaseID += 0xE0;
      lcd240x128.DISPLAY_GRAPHIC_XY(PhaseID[cPosition].X,PhaseID[cPosition].Y,word8x16[14],PhaseID[cPosition].height,PhaseID[cPosition].width/8);
    } else if(cPosition==1) {
      ucPhaseID = ucPhaseID & 0xF0;
      ucPhaseID += 0x0E;
      lcd240x128.DISPLAY_GRAPHIC_XY(PhaseID[cPosition].X,PhaseID[cPosition].Y,word8x16[14],PhaseID[cPosition].height,PhaseID[cPosition].width/8);
    }
  doKeyRIGHTWork();
  } else {
    if(usiCurrentStepNo > 0) {
      usiCurrentStepNo--;
    } else if(usiCurrentSubphaseNo > 0 ) {
      usiCurrentSubphaseNo--;
      usiCurrentStepNo = ucStepCount[usiCurrentSubphaseNo]-1;
    }
    DisplayLight();
  }
}
//---------------------------------------------------------------------------
void ScreenLightEdit::doKeyFWork(void)
{
//  unsigned int uiTmp;

  if(cSelPosition == 1) {
    if(cPosition==0) {
      ucPhaseID = ucPhaseID & 0x0F;
      ucPhaseID += 0xF0;
      lcd240x128.DISPLAY_GRAPHIC_XY(PhaseID[cPosition].X,PhaseID[cPosition].Y,word8x16[15],PhaseID[cPosition].height,PhaseID[cPosition].width/8);
    } else if(cPosition==1) {
      ucPhaseID = ucPhaseID & 0xF0;
      ucPhaseID += 0x0F;
      lcd240x128.DISPLAY_GRAPHIC_XY(PhaseID[cPosition].X,PhaseID[cPosition].Y,word8x16[15],PhaseID[cPosition].height,PhaseID[cPosition].width/8);
    }
  doKeyRIGHTWork();
  } else {
    if(usiCurrentStepNo < ucStepCount[usiCurrentSubphaseNo]-1) {
      usiCurrentStepNo++;
    } else if(usiCurrentSubphaseNo < ucSubphaseCount - 1 ) {
      usiCurrentSubphaseNo++;
      usiCurrentStepNo = 0;
    }
    DisplayLight();
  }

}

//---------------------------------------------------------------------------
void ScreenLightEdit::doKeyF1Work(void)
{
    screenMain.DisplayMain();
}
//---------------------------------------------------------------------------
void ScreenLightEdit::doKeyF2Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenLightEdit::doKeyF3Work(void)
{
  bool bMollacOK;
  int iTMP;
  unsigned short int usiTmp;
  bMollacOK = stc.Lock_to_Reset_Phase_for_Panel(ucPhaseID, ucSubphaseCount, ucSignalCount);
  if (bMollacOK) {
    printf("mollacOK ucPhaseID:%X ucSubphaseCount:%d ucSignalCount:%d\n", ucPhaseID, ucSubphaseCount, ucSignalCount);

    stc._panel_phase._phase_order=ucPhaseID;
    if(ucSignalCount == 2) { ucSignalMap = 0x11; }
    if(ucSignalCount == 3) { ucSignalMap = 0x51; }
    if(ucSignalCount == 4) { ucSignalMap = 0x55; }
    if(ucSignalCount == 5) { ucSignalMap = 0x5D; }
    if(ucSignalCount == 6) { ucSignalMap = 0x5F; }
    stc._panel_phase._signal_map= ucSignalMap;
    stc._panel_phase._signal_count=ucSignalCount;
    stc._panel_phase._subphase_count=ucSubphaseCount;
    stc._panel_phase._total_step_count = ucSubphaseCount * 5;  //五個步階




    for(int i = 0; i < ucSubphaseCount; i++) {
      for(int j = 0; j < ucStepCount[i]; j++) {
        for(int k = 0; k < ucSignalCount; k++) {
          printf("i:%d j:%d k:%d\n", i, j, k);

          //OT20110429
          usiTmp = usiLight[i][j][k];
          usiTmp = usiTmp & 0x3000;  //find red? or yellow
          if(usiTmp == 0x2000) {
            //change light!!
            usiLight[i][j][k] = usiLight[i][j][k] & 0xCFFF;  //clear old
            usiLight[i][j][k] = usiLight[i][j][k] | 0x1000;
          }

          usiTmp = usiLight[i][j][k];
          usiTmp = usiTmp & 0x000C;  //find red? or yellow
          if(usiTmp == 0x0008) {
            //change light!!
            usiLight[i][j][k] = usiLight[i][j][k] & 0xFFF3;  //clear old
            usiLight[i][j][k] = usiLight[i][j][k] | 0x0004;
          }


//          if(usiLight[i][j][k] ==  0x1004) { usiLight[i][j][k] = 0x2008; }
//OT20110429          if(usiLight[i][j][k] ==  0x2008) { usiLight[i][j][k] = 0x1004; }
          stc._panel_phase._ptr_subphase_step_signal_status[i][j][k] = usiLight[i][j][k];
        }
      }
    }

    smem.vWriteMsgToDOM("WritePhaseByPanel");

    stc.Lock_to_Save_Phase_from_Panel();             //儲存回去
    smem.vSetINTData(TC92_iUpdatePhaseData, 1);
    smem.vSetTCPhasePlanSegTypeData(TC_Phase, ucPhaseID, true);
    screenLightPreview.DisplayLightPreview(ucPhaseID, INVAILDVALUE);
  }

}
//---------------------------------------------------------------------------
void ScreenLightEdit::doKeyF4Work(void)
{
//    screenSegtypeEditF2.vDisplaySegtypeFromPreview(INVAILDVALUE);
    screenLightPreview.DisplayLightPreview(ucPhaseID, INVAILDVALUE);
}
//---------------------------------------------------------------------------
void ScreenLightEdit::doKeyUPWork(void)
{
try {
  if(cSelPosition == 0) {
    DisplayLight();
    clearCursor8x16(Light[cPosition/8][cPosition%8].X, Light[cPosition/8][cPosition%8].Y+15);
    if(cPosition > 0) {
      cPosition--;
//      setCursor8x16(Light[cPosition/8][cPosition%8].X, Light[cPosition/8][cPosition%8].Y+15);
    }
    setCursor8x16(Light[cPosition/8][cPosition%8].X, Light[cPosition/8][cPosition%8].Y+15);

  } else if(cSelPosition == 1) {
    clearCursor8x16(PhaseID[cPosition].X, PhaseID[cPosition].Y+16);
    cSelPosition = 0;
    cPosition = (ucSignalCount-1)*8;
    setCursor8x16(Light[cPosition/8][cPosition%8].X, Light[cPosition/8][cPosition%8].Y+15);
  } else if(cSelPosition == 2) {
    clearCursor8x16(SubPhaseID[cPosition].X, SubPhaseID[cPosition].Y+16);
    if(cPosition > 0) {
      cSelPosition = 1;
      cPosition = 1;
      setCursor8x16(PhaseID[cPosition].X, PhaseID[cPosition].Y+16);
    }

  } else if(cSelPosition == 3) {
    clearCursor8x16(StepID[cPosition].X, StepID[cPosition].Y+16);
    if(cPosition > 0) {
      cSelPosition = 2;
      cPosition = 1;
      setCursor8x16(SubPhaseID[cPosition].X, SubPhaseID[cPosition].Y+16);
    }

  } else if(cSelPosition == 4) {
    clearCursor8x16(SignalCount.X, SignalCount.Y+16);
    cSelPosition = 2;
    cPosition = 1;
    setCursor8x16(SubPhaseID[cPosition].X, SubPhaseID[cPosition].Y+16);
  }

  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenLightEdit::doKeyDOWNWork(void)
{
try {
  if(cSelPosition == 0) {
    DisplayLight();
    clearCursor8x16(Light[cPosition/8][cPosition%8].X, Light[cPosition/8][cPosition%8].Y+15);
    if(cPosition < ucSignalCount*8 - 1) {
      cPosition++;
      setCursor8x16(Light[cPosition/8][cPosition%8].X, Light[cPosition/8][cPosition%8].Y+15);
    }
    setCursor8x16(Light[cPosition/8][cPosition%8].X, Light[cPosition/8][cPosition%8].Y+15);

  } else if(cSelPosition == 1) {
    clearCursor8x16(PhaseID[cPosition].X, PhaseID[cPosition].Y+16);
    cSelPosition = 2;
    cPosition = 1;
    setCursor8x16(SubPhaseID[cPosition].X, SubPhaseID[cPosition].Y+16);

  } else if(cSelPosition == 2) {
    clearCursor8x16(SubPhaseID[cPosition].X, SubPhaseID[cPosition].Y+16);
    if(cPosition < 1) {
      cPosition ++;
      setCursor8x16(SubPhaseID[cPosition].X, SubPhaseID[cPosition].Y+16);
    } else {
      cSelPosition = 4;
      cPosition = 0;
      setCursor8x16(SignalCount.X, SignalCount.Y+16);
    }
  }

  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenLightEdit::doKeyLEFTWork(void)
{
try {
  if(cSelPosition == 0) {
    DisplayLight();
    clearCursor8x16(Light[cPosition/8][cPosition%8].X, Light[cPosition/8][cPosition%8].Y+15);
    if(cPosition > 7) {
      cPosition -= 8;
      setCursor8x16(Light[cPosition/8][cPosition%8].X, Light[cPosition/8][cPosition%8].Y+15);
    } else {
      setCursor8x16(Light[cPosition/8][cPosition%8].X, Light[cPosition/8][cPosition%8].Y+15);
    }
  } else if(cSelPosition == 1) {
    clearCursor8x16(PhaseID[cPosition].X, PhaseID[cPosition].Y+16);
    if(cPosition > 0) {
      cPosition --;
      setCursor8x16(PhaseID[cPosition].X, PhaseID[cPosition].Y+16);
    } else {
      cSelPosition = 0;
      cPosition = (ucSignalCount-1)*8;
      setCursor8x16(Light[cPosition/8][cPosition%8].X, Light[cPosition/8][cPosition%8].Y+15);
    }
  } else if(cSelPosition == 2) {
    clearCursor8x16(SubPhaseID[cPosition].X, SubPhaseID[cPosition].Y+16);
    if(cPosition > 0) {
      cSelPosition = 1;
      cPosition = 1;
      setCursor8x16(PhaseID[cPosition].X, PhaseID[cPosition].Y+16);
    }

  } else if(cSelPosition == 3) {
    clearCursor8x16(StepID[cPosition].X, StepID[cPosition].Y+16);
    if(cPosition > 0) {
      cSelPosition = 2;
      cPosition = 1;
      setCursor8x16(SubPhaseID[cPosition].X, SubPhaseID[cPosition].Y+16);
    }

  } else if(cSelPosition == 4) {
    clearCursor8x16(SignalCount.X, SignalCount.Y+16);
    cSelPosition = 2;
    cPosition = 1;
    setCursor8x16(SubPhaseID[cPosition].X, SubPhaseID[cPosition].Y+16);
  }




  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenLightEdit::doKeyRIGHTWork(void)
{
try {
  if(cSelPosition == 0) {
    DisplayLight();
    clearCursor8x16(Light[cPosition/8][cPosition%8].X, Light[cPosition/8][cPosition%8].Y+15);
    if(cPosition < (ucSignalCount-1)*8) {
      cPosition += 8;
      setCursor8x16(Light[cPosition/8][cPosition%8].X, Light[cPosition/8][cPosition%8].Y+15);
    } else {
      cSelPosition = 1;
      cPosition = 0;
      setCursor8x16(PhaseID[cPosition].X, PhaseID[cPosition].Y+16);
    }

  } else if(cSelPosition == 1) {
    clearCursor8x16(PhaseID[cPosition].X, PhaseID[cPosition].Y+16);
    if(cPosition < 1) {
      cPosition ++;
      setCursor8x16(PhaseID[cPosition].X, PhaseID[cPosition].Y+16);
    } else {
      cSelPosition = 2;
      cPosition = 1;
      setCursor8x16(SubPhaseID[cPosition].X, SubPhaseID[cPosition].Y+16);
    }

  } else if(cSelPosition == 2) {
    clearCursor8x16(SubPhaseID[cPosition].X, SubPhaseID[cPosition].Y+16);
    if(cPosition < 1) {
      cPosition ++;
      setCursor8x16(SubPhaseID[cPosition].X, SubPhaseID[cPosition].Y+16);
    } else {
      cSelPosition = 4;
      cPosition = 0;
      setCursor8x16(SignalCount.X, SignalCount.Y+16);
    }
  }


  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenLightEdit::doKeyEnterWork(void)
{
//    screenSegtypeEditF2.vDisplaySegtypeFromPreview(iPlanReturn);
}
//---------------------------------------------------------------------------
void ScreenLightEdit::vTmpArrayToData(void)
{
try{
/*
  if(face == 1) {
    for(int i = 0; i < 4; i++) {
      if(i < planInfo._subphase_count) {
        planInfo._ptr_subplaninfo[i]._green = 0;
        planInfo._ptr_subplaninfo[i]._green = planTableTemp[0+i*4] * 1000;
        planInfo._ptr_subplaninfo[i]._green += (planTableTemp[1+i*4] * 100);
        planInfo._ptr_subplaninfo[i]._green += (planTableTemp[2+i*4] * 10);
        planInfo._ptr_subplaninfo[i]._green += planTableTemp[3+i*4];

        planInfo._ptr_subplaninfo[i]._allred = 0;
        planInfo._ptr_subplaninfo[i]._allred = planTableTemp[16+i*4] * 1000;
        planInfo._ptr_subplaninfo[i]._allred += (planTableTemp[17+i*4] * 100);
        planInfo._ptr_subplaninfo[i]._allred += (planTableTemp[18+i*4] * 10);
        planInfo._ptr_subplaninfo[i]._allred += planTableTemp[19+i*4];

        planInfo._ptr_subplaninfo[i]._yellow = 0;
        planInfo._ptr_subplaninfo[i]._yellow = planTableTemp[32+i*4] * 1000;
        planInfo._ptr_subplaninfo[i]._yellow += (planTableTemp[33+i*4] * 100);
        planInfo._ptr_subplaninfo[i]._yellow += (planTableTemp[34+i*4] * 10);
        planInfo._ptr_subplaninfo[i]._yellow += planTableTemp[35+i*4];
      }
    }
  }
  else if(face == 4) {
    for(int i = 0; i < 4; i++) {
      if(i+4 < planInfo._subphase_count) {
        planInfo._ptr_subplaninfo[4+i]._green = 0;
        planInfo._ptr_subplaninfo[4+i]._green = planTableTemp[0+i*4] * 1000;
        planInfo._ptr_subplaninfo[4+i]._green += (planTableTemp[1+i*4] * 100);
        planInfo._ptr_subplaninfo[4+i]._green += (planTableTemp[2+i*4] * 10);
        planInfo._ptr_subplaninfo[4+i]._green += planTableTemp[3+i*4];

        planInfo._ptr_subplaninfo[4+i]._allred = 0;
        planInfo._ptr_subplaninfo[4+i]._allred = planTableTemp[16+i*4] * 1000;
        planInfo._ptr_subplaninfo[4+i]._allred += (planTableTemp[17+i*4] * 100);
        planInfo._ptr_subplaninfo[4+i]._allred += (planTableTemp[18+i*4] * 10);
        planInfo._ptr_subplaninfo[4+i]._allred += planTableTemp[19+i*4];

        planInfo._ptr_subplaninfo[4+i]._yellow = 0;
        planInfo._ptr_subplaninfo[4+i]._yellow = planTableTemp[32+i*4] * 1000;
        planInfo._ptr_subplaninfo[4+i]._yellow += (planTableTemp[33+i*4] * 100);
        planInfo._ptr_subplaninfo[4+i]._yellow += (planTableTemp[34+i*4] * 10);
        planInfo._ptr_subplaninfo[4+i]._yellow += planTableTemp[35+i*4];
      }
    }
  }
  else if(face == 2) {
    for(int i = 0; i < 4; i++) {
      if(i < planInfo._subphase_count) {
        planInfo._ptr_subplaninfo[i]._pedgreen_flash = 0;
        planInfo._ptr_subplaninfo[i]._pedgreen_flash = planTableTemp[0+i*4] * 1000;
        planInfo._ptr_subplaninfo[i]._pedgreen_flash += (planTableTemp[1+i*4] * 100);
        planInfo._ptr_subplaninfo[i]._pedgreen_flash += (planTableTemp[2+i*4] * 10);
        planInfo._ptr_subplaninfo[i]._pedgreen_flash += planTableTemp[3+i*4];

        planInfo._ptr_subplaninfo[i]._pedred = 0;
        planInfo._ptr_subplaninfo[i]._pedred = planTableTemp[16+i*4] * 1000;
        planInfo._ptr_subplaninfo[i]._pedred += (planTableTemp[17+i*4] * 100);
        planInfo._ptr_subplaninfo[i]._pedred += (planTableTemp[18+i*4] * 10);
        planInfo._ptr_subplaninfo[i]._pedred += planTableTemp[19+i*4];
      }
    }
  }
  else if(face == 5) {
    for(int i = 0; i < 4; i++) {
      if(i+4 < planInfo._subphase_count) {
        planInfo._ptr_subplaninfo[4+i]._pedgreen_flash = 0;
        planInfo._ptr_subplaninfo[4+i]._pedgreen_flash = planTableTemp[0+i*4] * 1000;
        planInfo._ptr_subplaninfo[4+i]._pedgreen_flash += (planTableTemp[1+i*4] * 100);
        planInfo._ptr_subplaninfo[4+i]._pedgreen_flash += (planTableTemp[2+i*4] * 10);
        planInfo._ptr_subplaninfo[4+i]._pedgreen_flash += planTableTemp[3+i*4];

        planInfo._ptr_subplaninfo[4+i]._pedred = 0;
        planInfo._ptr_subplaninfo[4+i]._pedred = planTableTemp[16+i*4] * 1000;
        planInfo._ptr_subplaninfo[4+i]._pedred += (planTableTemp[17+i*4] * 100);
        planInfo._ptr_subplaninfo[4+i]._pedred += (planTableTemp[18+i*4] * 10);
        planInfo._ptr_subplaninfo[4+i]._pedred += planTableTemp[19+i*4];
      }
    }
  }
  else if(face == 3) {
    for(int i = 0; i < 4; i++) {
      if(i < planInfo._subphase_count) {
        planInfo._ptr_subplaninfo[i]._min_green = 0;
        planInfo._ptr_subplaninfo[i]._min_green = planTableTemp[0+i*4] * 1000;
        planInfo._ptr_subplaninfo[i]._min_green += (planTableTemp[1+i*4] * 100);
        planInfo._ptr_subplaninfo[i]._min_green += (planTableTemp[2+i*4] * 10);
        planInfo._ptr_subplaninfo[i]._min_green += planTableTemp[3+i*4];

        planInfo._ptr_subplaninfo[i]._max_green = 0;
        planInfo._ptr_subplaninfo[i]._max_green = planTableTemp[16+i*4] * 1000;
        planInfo._ptr_subplaninfo[i]._max_green += (planTableTemp[17+i*4] * 100);
        planInfo._ptr_subplaninfo[i]._max_green += (planTableTemp[18+i*4] * 10);
        planInfo._ptr_subplaninfo[i]._max_green += planTableTemp[19+i*4];
      }
    }
  }
  else if(face == 6) {
    for(int i = 0; i < 4; i++) {
      if(i+4 < planInfo._subphase_count) {
        planInfo._ptr_subplaninfo[4+i]._min_green = 0;
        planInfo._ptr_subplaninfo[4+i]._min_green = planTableTemp[0+i*4] * 1000;
        planInfo._ptr_subplaninfo[4+i]._min_green += (planTableTemp[1+i*4] * 100);
        planInfo._ptr_subplaninfo[4+i]._min_green += (planTableTemp[2+i*4] * 10);
        planInfo._ptr_subplaninfo[4+i]._min_green += planTableTemp[3+i*4];

        planInfo._ptr_subplaninfo[4+i]._max_green = 0;
        planInfo._ptr_subplaninfo[4+i]._max_green = planTableTemp[16+i*4] * 1000;
        planInfo._ptr_subplaninfo[4+i]._max_green += (planTableTemp[17+i*4] * 100);
        planInfo._ptr_subplaninfo[4+i]._max_green += (planTableTemp[18+i*4] * 10);
        planInfo._ptr_subplaninfo[4+i]._max_green += planTableTemp[19+i*4];
      }
    }
  }

  // cal. cycle

  planInfo._cycle_time = 0;
  for(int i = 0; i < planInfo._subphase_count; i++) {
    planInfo._cycle_time += planInfo._ptr_subplaninfo[i]._green;
    planInfo._cycle_time += planInfo._ptr_subplaninfo[i]._allred;
    planInfo._cycle_time += planInfo._ptr_subplaninfo[i]._yellow;
    planInfo._cycle_time += planInfo._ptr_subplaninfo[i]._pedgreen_flash;
    planInfo._cycle_time += planInfo._ptr_subplaninfo[i]._pedred;
  }
*/

}catch(...){}
}
//---------------------------------------------------------------------------
