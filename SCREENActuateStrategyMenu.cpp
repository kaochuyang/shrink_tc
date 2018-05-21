#include "SCREENActuateStrategyMenu.h"
#include "LCD240x128.h"
//#include "screenLogo.h"
#include "SCREENMain.h"
#include "screenRunTableList.h"

#include "screenSegtypeMenu.h"
#include "screenVDPhaseMapTable.h"
#include "screenVDActPhaseEdit.h"
#include "screenFlashMode.h"
#include "screenPlan.h"
#include "SCREENControlStrategyMenu.h"
#include "CSTC.h"
#include <stdio.h>
#include "SMEM.h"
#include <time.h>
#include "ConfigFile.h"
#include <stdlib.h>
//NO USE chou 2013/01/30
//---------------------------------------------------
SCREENActuateStrategyMenu screenActuateStrategyMenu;
//---------------------------------------------------------------------------
SCREENActuateStrategyMenu::SCREENActuateStrategyMenu(void)
{
try {
    loadBitmapFromFile();
    initDispWord();
    cSelect=0;
  } catch (...) {}
}
//---------------------------------------------------------------------------
SCREENActuateStrategyMenu::~SCREENActuateStrategyMenu(void)
{

}
//---------------------------------------------------------------------------
void SCREENActuateStrategyMenu::doKeyWork(BYTE key)                                        //¤w¹LÂo,¶i¨Óªº¬O0x80~0x98¤§¶¡
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
        case 0x8E:
        case 0x8F:
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
//---------------------------------------------------------------------------
void SCREENActuateStrategyMenu::loadBitmapFromFile(void)
{
try {
    FILE *bitmap;
    bitmap=fopen("//cct//bitmap//backGround//actmenu//ActuateStrategyMenu.bit","rb");
    if (bitmap) {
        fread(ActuateStrategyBitmap,3840,1,bitmap);
        fclose(bitmap);
    }
    bitmap=fopen("//cct//bitmap//backGround//actmenu//ActuateCloseMode.bit","rb");
       if (bitmap) {
           fread(ActuateCloseModeBitmap,3840,1,bitmap);
           fclose(bitmap);
       }
  } catch (...) {}
}

//---------------------------------------------------------------------------
void SCREENActuateStrategyMenu::DisplayActuateCloseMode(void)
{
try{
	smem.SetcFace(cACTSTRATEGY_CLOSE);
	lcd240x128.DISPLAY_GRAPHIC(0,ActuateCloseModeBitmap,128,30);

}catch(...){}
}
//---------------------------------------------------------------------------

void SCREENActuateStrategyMenu::DisplayActuateStrategyMenu(void)
{
try {
    smem.SetcFace(cACTSTRATEGY);
    smem.vWriteMsgToDOM("Display Actuateler Common Setup Screen");
    lcd240x128.DISPLAY_GRAPHIC(0,ActuateStrategyBitmap,128,30);
    setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);




    ConfigFile config( "/cct/vddata/dev.cfg" );
    int iTmp = 0;
    if(config.readInto( iTmp, "ACTTYPE" )){
    	printf("Read The ACTTYPE of is:%d\n",iTmp);
    	usiActType=iTmp;
    }
    if(usiActType==0){iTmp=0x0;}
    else if(usiActType==1){iTmp=0x1;}
    else if(usiActType==3){iTmp=0x0;}
    else if(usiActType==6){iTmp=0x2;}

    setUpPoint16x16(markActType[iTmp].X,markActType[iTmp].Y,markActType[iTmp].height,markActType[iTmp].width/8);




  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENActuateStrategyMenu::initDispWord(void)
{
try {

    for (int i=0;i<10;i++) {
         if (i>=0 && i<=4) {
             mark[i].X=16;
             mark[i].Y=34+i*16;
         } else if (i>=5 && i<=9) {
             mark[i].X=128;
             mark[i].Y=34+(i-5)*16;
         }
         mark[i].width=16;
         mark[i].height=16;

         if(i>=0 && i<=3){
			 markActType[i].X=32;
			 markActType[i].Y=34+i*16;
			 markActType[i].width=16;
			 markActType[i].height=16;
         }

    }


  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENActuateStrategyMenu::doKey0Work(void)
{

}
//---------------------------------------------------------------------------
void SCREENActuateStrategyMenu::doKey1Work(void)
{
	if(smem.GetcFace()==cACTSTRATEGY){
	screenVDActPhaseEdit.DisplayVDActPhaseEdit();//半（全）觸動
	}
}
//---------------------------------------------------------------------------
void SCREENActuateStrategyMenu::doKey2Work(void)
{
	if(smem.GetcFace()==cACTSTRATEGY){
	//screenVDPhaseMapTable.DisplayVDPhaseMapTable();//閃光觸動設定
	screenFlashMode.DisplayFlashMode();
	}
}
//---------------------------------------------------------------------------
void SCREENActuateStrategyMenu::doKey3Work(void)
{
	if(smem.GetcFace()==cACTSTRATEGY){
		DisplayActuateCloseMode();
	}
}
//---------------------------------------------------------------------------
void SCREENActuateStrategyMenu::doKey4Work(void)
{

}
//---------------------------------------------------------------------------
void SCREENActuateStrategyMenu::doKey5Work(void)
{

}
//---------------------------------------------------------------------------
void SCREENActuateStrategyMenu::doKey6Work(void)
{

}
//---------------------------------------------------------------------------
void SCREENActuateStrategyMenu::doKey7Work(void)
{

}
//---------------------------------------------------------------------------
void SCREENActuateStrategyMenu::doKey8Work(void)
{
}
//---------------------------------------------------------------------------
void SCREENActuateStrategyMenu::doKey9Work(void)
{
}
//---------------------------------------------------------------------------
void SCREENActuateStrategyMenu::doKeyF1Work(void)
{
	if(smem.GetcFace()==cACTSTRATEGY){
	screenMain.DisplayMain();
	}
}
//---------------------------------------------------------------------------
void SCREENActuateStrategyMenu::doKeyF2Work(void)
{

}
//OT Pass
//---------------------------------------------------------------------------
void SCREENActuateStrategyMenu::doKeyF3Work(void)
{

}
//---------------------------------------------------------------------------
void SCREENActuateStrategyMenu::doKeyF4Work(void)
{

	//screenActuateStrategyMenu.DisplayActuateStrategyMenu();
	screenControlStrategyMenu.DisplayControlStrategyMenu();
}
//---------------------------------------------------------------------------
void SCREENActuateStrategyMenu::doKeyEnterWork(void)
{
	if(smem.GetcFace()==cACTSTRATEGY){
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
    if(smem.GetcFace()==cACTSTRATEGY_CLOSE){
    	ConfigFile config( "/cct/vddata/dev.cfg" );
    	// Values can be read from the file by name
    	config.add( "ACTTYPE", 6 );//關閉觸動
    	printf("Set ACTTYPE is:%d",6);
    	config.writeCf();
    	smem.vSetBOOLData(TC_CCT_In_LongTanu_ActuateType_FunctionEnable, false);
    	smem.vSetActuatePhaseExtend(0);
    	smem.vSetUSIData(TC_ActuateVDID, 00000);
    	smem.vSetUCData(TC_CCT_ActuateType_By_TOD, false);
    	smem.vSetBOOLData(TCSegmentTypeUpdate, true);
    	system("sync");
    	system("sync");
    	system("killall -9 act_mode");
    	//if(smem.vGetINTData(Com2_TYPE) != Com2IsPedestrianPushButton){
            system("/cct/act_mode >/dev/tty5 &");
    	//}
    	DisplayActuateStrategyMenu();
    }
}
//---------------------------------------------------------------------------
void SCREENActuateStrategyMenu::doKeyUPWork(void)
{
try {
	if(smem.GetcFace()==cACTSTRATEGY){
	clearSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    if (cSelect==0) cSelect=1;
    cSelect--;
    setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
	}
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENActuateStrategyMenu::doKeyDOWNWork(void)
{
try {
	if(smem.GetcFace()==cACTSTRATEGY){
	clearSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    cSelect++;
    if (cSelect>=2) cSelect=2;
//    if (cSelect>=8) cSelect=8;
    setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
	}
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENActuateStrategyMenu::doKeyLEFTWork(void)
{
try {
	if(smem.GetcFace()==cACTSTRATEGY){
	clearSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    //if (cSelect>=5 && cSelect<=9)
        //cSelect-=5;
    setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
	}
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENActuateStrategyMenu::doKeyRIGHTWork(void)
{
try {
	if(smem.GetcFace()==cACTSTRATEGY){
	clearSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
    //if (cSelect>=0 && cSelect<=4)
        //cSelect+=5;
    setSelectPoint16x16(mark[cSelect].X,mark[cSelect].Y,mark[cSelect].height,mark[cSelect].width/8);
	}
  } catch (...) {}
}
//---------------------------------------------------------------------------
