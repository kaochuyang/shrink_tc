#include "screenActuateBUSEdit.h"
#include "LCD240x128.h"
#include "SCREENMain.h"
#include "SCREENCtlSetup.h"
//#include "SCREENPedSignalMenu.h"
#include <stdio.h>
#include "SMEM.h"
#include <string.h>

//---------------------------------------------------------------------------
ScreenActuateBUSEdit screenActuateBUSEdit;
//---------------------------------------------------------------------------
ScreenActuateBUSEdit::ScreenActuateBUSEdit(void)
{
    loadBitmapFromFile();
    initDispWord();
    cPosition=0;
}
//---------------------------------------------------------------------------
ScreenActuateBUSEdit::~ScreenActuateBUSEdit(void)
{
}
//---------------------------------------------------------------------------
void ScreenActuateBUSEdit::loadBitmapFromFile(void)
{
try {
    FILE *bitmap;
    bitmap=fopen("//cct//bitmap//backGround//actuateBUSEdit.bit","rb");
    if (bitmap) {
        fread(actuateBUSEditBitmap,3840,1,bitmap);
        fclose(bitmap);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenActuateBUSEdit::DisplayActuateBUSEdit(void)
{
try {
    smem.SetcFace(cActuateBUSEdit);
    lcd240x128.DISPLAY_GRAPHIC(0,actuateBUSEditBitmap,128,30);

    cPosition=0;
    cSelect = 0;

    vInitUCTMPData();

    vDataToTmpArray();

    vDisplayActuateSwitch();
    vDisplayActuatesubphase();
    vDisplayActuateExtendPhaseID();
    vDisplayActuateautoswitch();
    //vDisplayActuateByTOD();

    setCursor8x16(cActuateSwitch.X,cActuateSwitch.Y+16);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenActuateBUSEdit::initDispWord(void)
{
try {
    cActuateSwitch.X = 88;
    cActuateSwitch.Y = 32;
    cActuateSwitch.width = 8;
    cActuateSwitch.height = 16;


  for (int i = 0; i < 2; i++) {
    cActuatePlan[i].X = 132 + i*8;
    cActuatePlan[i].Y = 48;
    cActuatePlan[i].width = 8;
    cActuatePlan[i].height = 16;
  }

  cActuatePhaseID.X = 115;
  cActuatePhaseID.Y = 69;
  cActuatePhaseID.width = 8;
  cActuatePhaseID.height = 16;


    cActuateautoswitch.X = 100;
    cActuateautoswitch.Y = 87;
    cActuateautoswitch.width = 8;
    cActuateautoswitch.height = 16;

/*
  cActuateType_By_TOD.X = 80;
  cActuateType_By_TOD.Y = 106;
  cActuateType_By_TOD.width = 8;
  cActuateType_By_TOD.height = 16;
*/
} catch (...) {}
}


//---------------------------------------------------------------------------
void ScreenActuateBUSEdit::doKeyWork(BYTE key)
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
void ScreenActuateBUSEdit::doKey0Work(void)
{
try {

    if (cSelect == 0) {
      lcd240x128.DISPLAY_GRAPHIC_XY(cActuateSwitch.X,cActuateSwitch.Y,word8x16[0],cActuateSwitch.height,cActuateSwitch.width/8);
      bTMP_ActuateSwitch = false;
    }
    else if (cSelect == 1) {
      lcd240x128.DISPLAY_GRAPHIC_XY(cActuatePlan[cPosition].X,cActuatePlan[cPosition].Y,word8x16[0],cActuatePlan[cPosition].height,cActuatePlan[cPosition].width/8);
      ucTMP_ActuatePlan[cPosition] = 0x0;
    }
    else if (cSelect == 2) {
      lcd240x128.DISPLAY_GRAPHIC_XY(cActuatePhaseID.X, cActuatePhaseID.Y, word8x16[0],cActuatePhaseID.height, cActuatePhaseID.width/8);
      ucActuatePhaseID = 0x0;
    }
    else if (cSelect == 3) {
      lcd240x128.DISPLAY_GRAPHIC_XY(cActuateautoswitch.X,cActuateautoswitch.Y,word8x16[0],cActuateautoswitch.height,cActuateautoswitch.width/8);
      bTMP_Actuateautoswitch = false;
    }
    else if (cSelect == 4) {
   //   lcd240x128.DISPLAY_GRAPHIC_XY(cActuateType_By_TOD.X, cActuateType_By_TOD.Y, word8x16[0],cActuateType_By_TOD.height, cActuateType_By_TOD.width/8);
   //   ucActuateType_By_TOD = 0x0;
    }


    doKeyRIGHTWork();

  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenActuateBUSEdit::doKey1Work(void)
{
try {

  if (cSelect == 0) {
    lcd240x128.DISPLAY_GRAPHIC_XY(cActuateSwitch.X,cActuateSwitch.Y,word8x16[1],cActuateSwitch.height,cActuateSwitch.width/8);
    bTMP_ActuateSwitch = true;
  }
  else if (cSelect == 1) {
    lcd240x128.DISPLAY_GRAPHIC_XY(cActuatePlan[cPosition].X,cActuatePlan[cPosition].Y,word8x16[1],cActuatePlan[cPosition].height,cActuatePlan[cPosition].width/8);
    ucTMP_ActuatePlan[cPosition] = 0x01;
  }
  else if (cSelect == 2) {
    lcd240x128.DISPLAY_GRAPHIC_XY(cActuatePhaseID.X, cActuatePhaseID.Y, word8x16[1],cActuatePhaseID.height, cActuatePhaseID.width/8);
    ucActuatePhaseID = 0x01;
  }
  else if (cSelect == 3) {
      lcd240x128.DISPLAY_GRAPHIC_XY(cActuateautoswitch.X,cActuateautoswitch.Y,word8x16[1],cActuateautoswitch.height,cActuateautoswitch.width/8);
      bTMP_Actuateautoswitch = true;
  }
  else if (cSelect == 4) {
  //  lcd240x128.DISPLAY_GRAPHIC_XY(cActuateType_By_TOD.X, cActuateType_By_TOD.Y, word8x16[1],cActuateType_By_TOD.height, cActuateType_By_TOD.width/8);
  //  ucActuateType_By_TOD = 0x01;
  }


  doKeyRIGHTWork();

  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenActuateBUSEdit::doKey2Work(void)
{
try {
  if (cSelect == 0) {
  }
  else if (cSelect == 1) {
    lcd240x128.DISPLAY_GRAPHIC_XY(cActuatePlan[cPosition].X,cActuatePlan[cPosition].Y,word8x16[2],cActuatePlan[cPosition].height,cActuatePlan[cPosition].width/8);
    ucTMP_ActuatePlan[cPosition] = 0x2;
    doKeyRIGHTWork();
  }
  else if (cSelect == 2) {
    lcd240x128.DISPLAY_GRAPHIC_XY(cActuatePhaseID.X, cActuatePhaseID.Y, word8x16[2],cActuatePhaseID.height, cActuatePhaseID.width/8);
    ucActuatePhaseID = 0x02;
  }
  else if (cSelect == 3) {
  //   lcd240x128.DISPLAY_GRAPHIC_XY(cActuateautoswitch.X,cActuateautoswitch.Y,word8x16[2],cActuateautoswitch.height,cActuateautoswitch.width/8);
  //    bTMP_Actuateautoswitch = 0x02;
  }
  else if (cSelect == 4) {
  //  lcd240x128.DISPLAY_GRAPHIC_XY(cActuateType_By_TOD.X, cActuateType_By_TOD.Y, word8x16[2],cActuateType_By_TOD.height, cActuateType_By_TOD.width/8);
  //  ucActuateType_By_TOD = 0x02;
  }

  doKeyRIGHTWork();
} catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenActuateBUSEdit::doKey3Work(void)
{
try {
  if (cSelect == 0) {
  }
  else if (cSelect == 1) {
    lcd240x128.DISPLAY_GRAPHIC_XY(cActuatePlan[cPosition].X,cActuatePlan[cPosition].Y,word8x16[3],cActuatePlan[cPosition].height,cActuatePlan[cPosition].width/8);
    ucTMP_ActuatePlan[cPosition] = 0x3;
    doKeyRIGHTWork();
  }
  else if (cSelect == 2) {
  //  lcd240x128.DISPLAY_GRAPHIC_XY(cActuatePhaseID.X, cActuatePhaseID.Y, word8x16[3],cActuatePhaseID.height, cActuatePhaseID.width/8);
  // ucActuatePhaseID = 0x03;
  }
  else if (cSelect == 3) {
  //    lcd240x128.DISPLAY_GRAPHIC_XY(cActuateautoswitch.X,cActuateautoswitch.Y,word8x16[3],cActuateautoswitch.height,cActuateautoswitch.width/8);
  //    bTMP_Actuateautoswitch = 0x03;
  }
  else if (cSelect == 4) {
  //  lcd240x128.DISPLAY_GRAPHIC_XY(cActuateType_By_TOD.X, cActuateType_By_TOD.Y, word8x16[3],cActuateType_By_TOD.height, cActuateType_By_TOD.width/8);
  //  ucActuateType_By_TOD = 0x03;
  }

  doKeyRIGHTWork();

} catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenActuateBUSEdit::doKey4Work(void)
{
try {
  if (cSelect == 0) {
  }
  else if (cSelect == 1) {
    lcd240x128.DISPLAY_GRAPHIC_XY(cActuatePlan[cPosition].X,cActuatePlan[cPosition].Y,word8x16[4],cActuatePlan[cPosition].height,cActuatePlan[cPosition].width/8);
    ucTMP_ActuatePlan[cPosition] = 0x4;
    doKeyRIGHTWork();
  }
  else if (cSelect == 2) {
  //  lcd240x128.DISPLAY_GRAPHIC_XY(cActuatePhaseID.X, cActuatePhaseID.Y, word8x16[4],cActuatePhaseID.height, cActuatePhaseID.width/8);
  //  ucActuatePhaseID = 0x04;
  }
  else if (cSelect == 3) {
   //   lcd240x128.DISPLAY_GRAPHIC_XY(cActuateautoswitch.X,cActuateautoswitch.Y,word8x16[4],cActuateautoswitch.height,cActuateautoswitch.width/8);
   //   bTMP_Actuateautoswitch = 0x04;
  }
  else if (cSelect == 4) {
   // lcd240x128.DISPLAY_GRAPHIC_XY(cActuateType_By_TOD.X, cActuateType_By_TOD.Y, word8x16[4],cActuateType_By_TOD.height, cActuateType_By_TOD.width/8);
   // ucActuateType_By_TOD = 0x04;
  }

  doKeyRIGHTWork();

} catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenActuateBUSEdit::doKey5Work(void)
{
try {
  if (cSelect == 0) {
  }
  else if (cSelect == 1) {
    lcd240x128.DISPLAY_GRAPHIC_XY(cActuatePlan[cPosition].X,cActuatePlan[cPosition].Y,word8x16[5],cActuatePlan[cPosition].height,cActuatePlan[cPosition].width/8);
    ucTMP_ActuatePlan[cPosition] = 0x5;
    doKeyRIGHTWork();
  }
  else if (cSelect == 2) {
  //  lcd240x128.DISPLAY_GRAPHIC_XY(cActuatePhaseID.X, cActuatePhaseID.Y, word8x16[5],cActuatePhaseID.height, cActuatePhaseID.width/8);
  //  ucActuatePhaseID = 0x05;
  }
  else if (cSelect == 3) {
  //   lcd240x128.DISPLAY_GRAPHIC_XY(cActuateautoswitch.X,cActuateautoswitch.Y,word8x16[5],cActuateautoswitch.height,cActuateautoswitch.width/8);
  //    bTMP_Actuateautoswitch = 0x05;
  }
  else if (cSelect == 4) {
 //   lcd240x128.DISPLAY_GRAPHIC_XY(cActuateType_By_TOD.X, cActuateType_By_TOD.Y, word8x16[5],cActuateType_By_TOD.height, cActuateType_By_TOD.width/8);
 //   ucActuateType_By_TOD = 0x05;
  }


  doKeyRIGHTWork();
} catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenActuateBUSEdit::doKey6Work(void)
{
try {
 /* if (cSelect == 0) {
  }
  else if (cSelect == 1) {
    lcd240x128.DISPLAY_GRAPHIC_XY(cActuatePlan[cPosition].X,cActuatePlan[cPosition].Y,word8x16[6],cActuatePlan[cPosition].height,cActuatePlan[cPosition].width/8);
    ucTMP_ActuatePlan[cPosition] = 0x6;
    doKeyRIGHTWork();
  }
  else if (cSelect == 2) {
    lcd240x128.DISPLAY_GRAPHIC_XY(cActuatePhaseID.X, cActuatePhaseID.Y, word8x16[6],cActuatePhaseID.height, cActuatePhaseID.width/8);
    ucActuatePhaseID = 0x06;
  }
  else if (cSelect == 3) {
      lcd240x128.DISPLAY_GRAPHIC_XY(cActuateautoswitch.X,cActuateautoswitch.Y,word8x16[6],cActuateautoswitch.height,cActuateautoswitch.width/8);
      bTMP_Actuateautoswitch = 0x06;
  }
  else if (cSelect == 4) {
    lcd240x128.DISPLAY_GRAPHIC_XY(cActuateType_By_TOD.X, cActuateType_By_TOD.Y, word8x16[6],cActuateType_By_TOD.height, cActuateType_By_TOD.width/8);
    ucActuateType_By_TOD = 0x06;
  }
*/
  doKeyRIGHTWork();

} catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenActuateBUSEdit::doKey7Work(void)
{
try {
 /* if (cSelect == 0) {
  }
  else if (cSelect == 1) {
    lcd240x128.DISPLAY_GRAPHIC_XY(cActuatePlan[cPosition].X,cActuatePlan[cPosition].Y,word8x16[7],cActuatePlan[cPosition].height,cActuatePlan[cPosition].width/8);
    ucTMP_ActuatePlan[cPosition] = 0x7;
    doKeyRIGHTWork();
  }
  else if (cSelect == 2) {
    lcd240x128.DISPLAY_GRAPHIC_XY(cActuatePhaseID.X, cActuatePhaseID.Y, word8x16[7],cActuatePhaseID.height, cActuatePhaseID.width/8);
    ucActuatePhaseID = 0x07;
  }
  else if (cSelect == 3) {
     lcd240x128.DISPLAY_GRAPHIC_XY(cActuateautoswitch.X,cActuateautoswitch.Y,word8x16[7],cActuateautoswitch.height,cActuateautoswitch.width/8);
      bTMP_Actuateautoswitch = 0x07;
  }
  else if (cSelect == 4) {
    lcd240x128.DISPLAY_GRAPHIC_XY(cActuateType_By_TOD.X, cActuateType_By_TOD.Y, word8x16[7],cActuateType_By_TOD.height, cActuateType_By_TOD.width/8);
    ucActuateType_By_TOD = 0x07;
  }
*/
  doKeyRIGHTWork();

} catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenActuateBUSEdit::doKey8Work(void)
{
try {
  /*if (cSelect == 0) {
  }
  else if (cSelect == 1) {
    lcd240x128.DISPLAY_GRAPHIC_XY(cActuatePlan[cPosition].X,cActuatePlan[cPosition].Y,word8x16[8],cActuatePlan[cPosition].height,cActuatePlan[cPosition].width/8);
    ucTMP_ActuatePlan[cPosition] = 0x8;
    doKeyRIGHTWork();
  }
  else if (cSelect == 2) {
    lcd240x128.DISPLAY_GRAPHIC_XY(cActuatePhaseID.X, cActuatePhaseID.Y, word8x16[8],cActuatePhaseID.height, cActuatePhaseID.width/8);
    ucActuatePhaseID = 0x08;
  }
  else if (cSelect == 3) {
      lcd240x128.DISPLAY_GRAPHIC_XY(cActuateautoswitch.X,cActuateautoswitch.Y,word8x16[8],cActuateautoswitch.height,cActuateautoswitch.width/8);
      bTMP_Actuateautoswitch = 0x08;
  }
  else if (cSelect == 4) {
    lcd240x128.DISPLAY_GRAPHIC_XY(cActuateType_By_TOD.X, cActuateType_By_TOD.Y, word8x16[8],cActuateType_By_TOD.height, cActuateType_By_TOD.width/8);
    ucActuateType_By_TOD = 0x08;
  }
*/
  doKeyRIGHTWork();

} catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenActuateBUSEdit::doKey9Work(void)
{
try {
 /* if (cSelect == 0) {
  }
  else if (cSelect == 1) {
    lcd240x128.DISPLAY_GRAPHIC_XY(cActuatePlan[cPosition].X,cActuatePlan[cPosition].Y,word8x16[9],cActuatePlan[cPosition].height,cActuatePlan[cPosition].width/8);
    ucTMP_ActuatePlan[cPosition] = 0x9;
    doKeyRIGHTWork();
  }
  else if (cSelect == 3) {
    //  lcd240x128.DISPLAY_GRAPHIC_XY(cActuateautoswitch.X,cActuateautoswitch.Y,word8x16[9],cActuateautoswitch.height,cActuateautoswitch.width/8);
    //  bTMP_Actuateautoswitch = 0x09;
  }
 // else if (cSelect == 4) {
 //   lcd240x128.DISPLAY_GRAPHIC_XY(cActuateType_By_TOD.X, cActuateType_By_TOD.Y, word8x16[9],cActuateType_By_TOD.height, cActuateType_By_TOD.width/8);
 //   ucActuateType_By_TOD = 0x09;
 // }
*/
  doKeyRIGHTWork();
} catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenActuateBUSEdit::doKeyAWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenActuateBUSEdit::doKeyBWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenActuateBUSEdit::doKeyCWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenActuateBUSEdit::doKeyDWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenActuateBUSEdit::doKeyEWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenActuateBUSEdit::doKeyFWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenActuateBUSEdit::doKeyF1Work(void)
{
    screenMain.DisplayMain();
}
//---------------------------------------------------------------------------
void ScreenActuateBUSEdit::doKeyF2Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenActuateBUSEdit::doKeyF3Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenActuateBUSEdit::doKeyF4Work(void)
{
//arwen mobify    screenMain.DisplayMain();
    screenCtlSetup.DisplayCtlSetup();
    //screenPedSignalMenu.DisplayPedSignalMenu();
}
//---------------------------------------------------------------------------
void ScreenActuateBUSEdit::doKeyUPWork(void)
{
  if (cSelect == 0) {
  }
  else if (cSelect == 1) {
    clearCursor8x16(cActuatePlan[cPosition].X,cActuatePlan[cPosition].Y+16);
    cSelect = 0;
    cPosition = 0;
    setCursor8x16(cActuateSwitch.X,cActuateSwitch.Y+16);
  }
  else if (cSelect == 2) {
    clearCursor8x16(cActuatePhaseID.X,cActuatePhaseID.Y+16);
    cSelect = 1;
    cPosition = 0;
    setCursor8x16(cActuatePlan[cPosition].X,cActuatePlan[cPosition].Y+16);
  }
  else if (cSelect == 3) {
    clearCursor8x16(cActuateautoswitch.X,cActuateautoswitch.Y+16);
    cSelect = 2;
    cPosition = 0;
    setCursor8x16(cActuatePhaseID.X,cActuatePhaseID.Y+16);
  }
  /*
  else if (cSelect == 4) {
    clearCursor8x16(cActuateType_By_TOD.X,cActuateType_By_TOD.Y+16);
    cSelect = 3;
    cPosition = 0;
    setCursor8x16(cActuateautoswitch.X,cActuateautoswitch.Y+16);
  }
  */


}
//---------------------------------------------------------------------------
void ScreenActuateBUSEdit::doKeyDOWNWork(void)
{

  if (cSelect == 0) {
    clearCursor8x16(cActuateSwitch.X,cActuateSwitch.Y+16);
    cSelect = 1;  // = cSelect++;
    cPosition = 0;
    setCursor8x16(cActuatePlan[cPosition].X,cActuatePlan[cPosition].Y+16);
  } else if(cSelect == 1) {
    clearCursor8x16(cActuatePlan[cPosition].X,cActuatePlan[cPosition].Y+16);
    cSelect = 2;  // = cSelect++;
    cPosition = 0;
    setCursor8x16(cActuatePhaseID.X,cActuatePhaseID.Y+16);
  } else if(cSelect == 2) {
    clearCursor8x16(cActuatePhaseID.X,cActuatePhaseID.Y+16);
    cSelect = 3;  // = cSelect++;
    cPosition = 0;
    setCursor8x16(cActuateautoswitch.X,cActuateautoswitch.Y+16);
  }
  /*
  else if(cSelect == 3) {
    clearCursor8x16(cActuateautoswitch.X,cActuateautoswitch.Y+16);
    cSelect = 4;  // = cSelect++;
    cPosition = 0;
    setCursor8x16(cActuateType_By_TOD.X,cActuateType_By_TOD.Y+16);
  }
  */

}
//---------------------------------------------------------------------------
void ScreenActuateBUSEdit::doKeyLEFTWork(void)
{
try {
  if (cSelect == 0) {
    clearCursor8x16(cActuateSwitch.X,cActuateSwitch.Y+16);
    setCursor8x16(cActuateSwitch.X,cActuateSwitch.Y+16);
  }

  else if (cSelect == 1) {
    clearCursor8x16(cActuatePlan[cPosition].X,cActuatePlan[cPosition].Y+16);
    if(cPosition > 0 ) {
      cPosition--;
      setCursor8x16(cActuatePlan[cPosition].X,cActuatePlan[cPosition].Y+16);
    }
    else {
      cSelect = 0;
      cPosition = 0;
      setCursor8x16(cActuateSwitch.X,cActuateSwitch.Y+16);
    }
  } else if (cSelect == 2) {
    clearCursor8x16(cActuatePhaseID.X,cActuatePhaseID.Y+16);
    cSelect = 1;
    cPosition = 1;
    setCursor8x16(cActuatePlan[cPosition].X,cActuatePlan[cPosition].Y+16);
  } else if (cSelect == 3) {
    clearCursor8x16(cActuateautoswitch.X,cActuateautoswitch.Y+16);
    if(cPosition > 0 ) {
      cPosition--;
      setCursor8x16(cActuateautoswitch.X,cActuateautoswitch.Y+16);
    } else {
      cSelect = 2;
      cPosition = 0;
      setCursor8x16(cActuatePhaseID.X,cActuatePhaseID.Y+16);
    }
  }
  /*
  else if (cSelect == 4) {
    clearCursor8x16(cActuateType_By_TOD.X,cActuateType_By_TOD.Y+16);
    cSelect = 3;
    cPosition = 0;
    setCursor8x16(cActuateautoswitch.X,cActuateautoswitch.Y+16);
  }
  */

  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenActuateBUSEdit::doKeyRIGHTWork(void)
{
try {

  if (cSelect == 0) {
    clearCursor8x16(cActuateSwitch.X,cActuateSwitch.Y+16);
    cPosition = 0;
    cSelect = 1;
    setCursor8x16(cActuatePlan[cPosition].X,cActuatePlan[cPosition].Y+16);
  }

  else if (cSelect == 1) {
    clearCursor8x16(cActuatePlan[cPosition].X,cActuatePlan[cPosition].Y+16);
    if(cPosition < 1 ) {
      cPosition++;
      setCursor8x16(cActuatePlan[cPosition].X,cActuatePlan[cPosition].Y+16);
    }
    else {
      cPosition = 0;
      cSelect = 2;
      setCursor8x16(cActuatePhaseID.X, cActuatePhaseID.Y+16);
    }
  } else if (cSelect == 2) {
    clearCursor8x16(cActuatePhaseID.X,cActuatePhaseID.Y+16);
    cPosition = 0;
    cSelect = 3;
    setCursor8x16(cActuateautoswitch.X,cActuateautoswitch.Y+16);
  } else if (cSelect == 3) {
    clearCursor8x16(cActuateautoswitch.X,cActuateautoswitch.Y+16);
    if(cPosition < 4 ) {
      cPosition++;
      setCursor8x16(cActuateautoswitch.X,cActuateautoswitch.Y+16);
    } else {
      clearCursor8x16(cActuateautoswitch.X,cActuateautoswitch.Y+16);
      cSelect = 4;  // = cSelect++;
      cPosition = 0;
      setCursor8x16(cActuateType_By_TOD.X,cActuateType_By_TOD.Y+16);
    }
  } /*else if(cSelect == 4) {
    clearCursor8x16(cActuateType_By_TOD.X,cActuateType_By_TOD.Y+16);
    cSelect = 4;  // = cSelect++;
    cPosition = 0;
    setCursor8x16(cActuateType_By_TOD.X,cActuateType_By_TOD.Y+16);
  } */


  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenActuateBUSEdit::doKeyEnterWork(void)
{

  vTmpArrayToData();

  if( vCheckRationality() ) { //Check
    smem.vSetBOOLData(TC_CCT_In_LongTanu_ActuateType_FunctionEnable, bTMP_ActuateSwitch);
    smem.vSetUSIData(TC_CCT_In_LongTanu_ActuateType_Switch, usiActuatePlan);
    smem.vSetActuatePhaseExtend(ucActuatePhaseID);
    smem.vSetBOOLData(TC_Actuateautoswitch, bTMP_Actuateautoswitch);
    //smem.vSetUCData(TC_CCT_ActuateType_By_TOD, ucActuateType_By_TOD);

    smem.vSetBOOLData(TCSegmentTypeUpdate, true);

    //screenMain.DisplayMain();
    screenCtlSetup.DisplayCtlSetup();
//
  }

}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void ScreenActuateBUSEdit::vDisplayActuateSwitch(void)
{
try{
  int iTMP;
  if(bTMP_ActuateSwitch == false)
    iTMP = 0;
  else
    iTMP = 1;

  lcd240x128.DISPLAY_GRAPHIC_XY(cActuateSwitch.X,cActuateSwitch.Y,word8x16[iTMP],cActuateSwitch.height,cActuateSwitch.width/8);
}catch(...){}
}

//---------------------------------------------------------------------------
void ScreenActuateBUSEdit::vDisplayActuatesubphase(void)          //執行觸動之分相步階
{
try{
  //HEX
  lcd240x128.DISPLAY_GRAPHIC_XY(cActuatePlan[0].X,cActuatePlan[0].Y,word8x16[ucTMP_ActuatePlan[0]],cActuatePlan[0].height,cActuatePlan[0].width/8);
  lcd240x128.DISPLAY_GRAPHIC_XY(cActuatePlan[1].X,cActuatePlan[1].Y,word8x16[ucTMP_ActuatePlan[1]],cActuatePlan[1].height,cActuatePlan[1].width/8);

}catch(...){}
}

//---------------------------------------------------------------------------
void ScreenActuateBUSEdit::vDisplayActuateExtendPhaseID(void)
{
try{
 if(ucActuatePhaseID != 0){
  lcd240x128.DISPLAY_GRAPHIC_XY(cActuatePhaseID.X, cActuatePhaseID.Y, word8x16[ucActuatePhaseID], cActuatePhaseID.height, cActuatePhaseID.width/8);
}else{
    lcd240x128.DISPLAY_GRAPHIC_XY(cActuatePhaseID.X, cActuatePhaseID.Y, word8x16[ucActuatePhaseID+1], cActuatePhaseID.height, cActuatePhaseID.width/8);
}

}catch(...){}
}


//---------------------------------------------------------------------------
void ScreenActuateBUSEdit::vDisplayActuateautoswitch(void)
{
try{
    int iTMP;
    if(bTMP_Actuateautoswitch == false)
        iTMP = 0;
    else
        iTMP = 1;

    lcd240x128.DISPLAY_GRAPHIC_XY(cActuateautoswitch.X,cActuateautoswitch.Y,word8x16[iTMP],cActuateautoswitch.height,cActuateautoswitch.width/8);
  //HEX
  //lcd240x128.DISPLAY_GRAPHIC_XY(cActuateVDID[0].X,cActuateVDID[0].Y,word8x16[ucTMP_ActuateVDID[0]],cActuateVDID[0].height,cActuateVDID[0].width/8);
  //lcd240x128.DISPLAY_GRAPHIC_XY(cActuateVDID[1].X,cActuateVDID[1].Y,word8x16[ucTMP_ActuateVDID[1]],cActuateVDID[1].height,cActuateVDID[1].width/8);
  //lcd240x128.DISPLAY_GRAPHIC_XY(cActuateVDID[2].X,cActuateVDID[2].Y,word8x16[ucTMP_ActuateVDID[2]],cActuateVDID[2].height,cActuateVDID[2].width/8);
  //lcd240x128.DISPLAY_GRAPHIC_XY(cActuateVDID[3].X,cActuateVDID[3].Y,word8x16[ucTMP_ActuateVDID[3]],cActuateVDID[3].height,cActuateVDID[3].width/8);
  //lcd240x128.DISPLAY_GRAPHIC_XY(cActuateVDID[4].X,cActuateVDID[4].Y,word8x16[ucTMP_ActuateVDID[4]],cActuateVDID[4].height,cActuateVDID[4].width/8);

}catch(...){}
}


//---------------------------------------------------------------------------
void ScreenActuateBUSEdit::vDisplayActuateByTOD(void)
{
try{

  lcd240x128.DISPLAY_GRAPHIC_XY(cActuateType_By_TOD.X, cActuateType_By_TOD.Y, word8x16[ucActuateType_By_TOD], cActuateType_By_TOD.height, cActuateType_By_TOD.width/8);

}catch(...){}
}


//---------------------------------------------------------------------------
void ScreenActuateBUSEdit::vDataToTmpArray(void)
{
try{
  usiActuatePlan = smem.vGetUSIData(TC_CCT_In_LongTanu_ActuateType_Switch);
  ucActuatePhaseID = smem.vGetActuatePhaseExtend();             //可延長之分相
  bTMP_ActuateSwitch = smem.vGetBOOLData(TC_CCT_In_LongTanu_ActuateType_FunctionEnable);
  bTMP_Actuateautoswitch = smem.vGetBOOLData(TC_Actuateautoswitch);

  //ucActuateType_By_TOD = smem.vGetUCData(TC_CCT_ActuateType_By_TOD);

  usiActuatePlan = 0x00;

 // ucTMP_ActuatePlan[0] = _exec_phase_current_subphase;
 // ucTMP_ActuatePlan[1] = _exec_phase_current_subphase_step ;




  //printf("usiActuateautoswitch:%d\n", usiActuateautoswitch);


}catch(...){}
}

//---------------------------------------------------------------------------
void ScreenActuateBUSEdit::vTmpArrayToData(void)
{
try{

  usiActuatePlan = 0;
  usiActuatePlan += ucTMP_ActuatePlan[0]*10;
  usiActuatePlan += ucTMP_ActuatePlan[1];



}catch(...){}
}

//---------------------------------------------------------------------------
bool ScreenActuateBUSEdit::vCheckRationality(void)
{
try{
  bool bRET;
  bRET = true;

  if(usiActuatePlan > 48)
    bRET = false;

  if(ucActuateType_By_TOD > 1)
    bRET = false;

  return bRET;

}catch(...){}
}

//---------------------------------------------------------------------------
bool ScreenActuateBUSEdit::vInitUCTMPData(void)
{
try{
  bzero(ucTMP_ActuatePlan, 2);

}catch(...){}
}


