#include "screenVDPhaseMapTable.h"
#include "screenVDPhaseMapTableEdit.h"
#include "SCREENMain.h"
#include "SCREENControlStrategyMenu.h"
#include "LCD240x128.h"
#include "SMEM.h"

ScreenVDPhaseMapTable screenVDPhaseMapTable;
//---------------------------------------------------------------------------
ScreenVDPhaseMapTable::ScreenVDPhaseMapTable(void)
{
    LoadBitmapFromFile();
    InitDispWord();
    phase_id=0;
}
//---------------------------------------------------------------------------
ScreenVDPhaseMapTable::~ScreenVDPhaseMapTable(void)
{
}
//---------------------------------------------------------------------------
void ScreenVDPhaseMapTable::DoKeyWork(BYTE key)                                           //已�\uFFFD�\uFFFD\uFFFD��\uFFFD\uFFFD\uFFFD\uFFFD0x80~0x98�\uFFFD\uFFFD
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
          DoKeyF3Work();
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
void ScreenVDPhaseMapTable::LoadBitmapFromFile(void)
{
try {
    FILE *bitmap;
    bitmap=fopen("//cct//bitmap//backGround//actmenu//vdPhaseMapTable.bit","rb");
    if (bitmap) {
        fread(networkSetBitmap,3840,1,bitmap);
        fclose(bitmap);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenVDPhaseMapTable::DisplayVDPhaseMapTable(void)
{
try {
    smem.SetcFace(cVDPHASEMAP);
    lcd240x128.DISPLAY_GRAPHIC(0,networkSetBitmap,128,30);
    //cPosition=0;
    //setCursor8x16(position[cPosition].X,position[cPosition].Y+16);
    phase_id=0;

    DisplayVDTable();
    DisplahPhaseId();


  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenVDPhaseMapTable::InitDispWord(void)
{
try {
	for(int i=0;i<2;i++){
		makrphaseId[i].width=8;
		makrphaseId[i].height=16;
		makrphaseId[i].X=160+(i*8);
		makrphaseId[i].Y=1;
	}

	for (int i=0;i<40;i++) {
		 value[0][i]=0;
		 value[1][i]=0;
		 value[2][i]=0;
		 value[3][i]=0;
		 value[4][i]=0;
		 value[5][i]=0;
		 value[6][i]=0;
		 value[7][i]=0;

         position[i].width=8;
         position[i].height=16;

         if (i%10==0)  position[i].X=64;
         else if (i%10==1) position[i].X=72;
         else if (i%10==2) position[i].X=80;
         else if (i%10==3) position[i].X=88;
         else if (i%10==4) position[i].X=96;

         else if (i%10==5) position[i].X=112+56;
         else if (i%10==6) position[i].X=120+56;
         else if (i%10==7) position[i].X=128+56;
         else if (i%10==8) position[i].X=136+56;
         else if (i%10==9) position[i].X=144+56;

         if (i>=0 && i<=9) position[i].Y=30;
         else if (i>=10 && i<=19) position[i].Y=50;
         else if (i>=20 && i<=29) position[i].Y=70;
         else if (i>=30 && i<=39) position[i].Y=90;

    }

  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenVDPhaseMapTable::DisplahPhaseId()
{
try{
	int tmp_ip[2];
	tmp_ip[0]=((phase_id+1)%100)/10;
	tmp_ip[1]=(phase_id)+1%10;
	for(int i=0;i<2;i++){
		lcd240x128.DISPLAY_GRAPHIC_XY(makrphaseId[i].X,makrphaseId[i].Y,word8x16[tmp_ip[i]],makrphaseId[i].height,makrphaseId[i].width/8);
	}
}catch(...){}
}
void ScreenVDPhaseMapTable::DisplayVDTable(void)
{
try {
	int _tmpcount=0,j=0;
	for(int i=0;i<8;i++){
		usiLCN[phase_id][i]=smem.vGetVDPhaseMapTable(phase_id,i);
		printf("[%d][%d]=%d ",phase_id,i,usiLCN[phase_id][i]);
		for(j=_tmpcount;j<_tmpcount+5;j++){
			if(j%5==0){
				value[phase_id][j]=(usiLCN[phase_id][i]/10000);
				printf("--value[%d][%d]=%d ",phase_id,j,value[phase_id][j]);
			}else if(j%5 ==1){
				value[phase_id][j]=(usiLCN[phase_id][i]/1000)%10;
				printf("--value[%d][%d]=%d ",phase_id,j,value[phase_id][j]);
			}else if(j%5==2){
				value[phase_id][j]=(usiLCN[phase_id][i]%1000)/100;
				printf("--value[%d][%d]=%d ",phase_id,j,value[phase_id][j]);
			}else if(j%5==3){
				value[phase_id][j]=(usiLCN[phase_id][i]%100)/10;
				printf("--value[%d][%d]=%d ",phase_id,j,value[phase_id][j]);
			}else if(j%5==4){
				value[phase_id][j]=usiLCN[phase_id][i]%10;
				printf("--value[%d][%d]=%d ",phase_id,j,value[phase_id][j]);
			}
		}
		_tmpcount=j;
		printf(" >>%d\n",_tmpcount);

	}
	printf("\n");
	for(int i=0;i<40;i++){
		lcd240x128.DISPLAY_GRAPHIC_XY(position[i].X,position[i].Y,word8x16[value[phase_id][i]],position[i].height,position[i].width/8);

	}




  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenVDPhaseMapTable::DoKey0Work(void)
{
try {
    /*
	clearCursor8x16(position[cPosition].X,position[cPosition].Y+16);
    value[phase_id][cPosition]=0;
    lcd240x128.DISPLAY_GRAPHIC_XY(position[cPosition].X,position[cPosition].Y,word8x16[value[phase_id][cPosition]],position[cPosition].height,position[cPosition].width/8);
    if (cPosition<39) cPosition++;
    setCursor8x16(position[cPosition].X,position[cPosition].Y+16);
    */
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenVDPhaseMapTable::DoKey1Work(void)
{
try {
    /*
	clearCursor8x16(position[cPosition].X,position[cPosition].Y+16);
    value[phase_id][cPosition]=1;
    lcd240x128.DISPLAY_GRAPHIC_XY(position[cPosition].X,position[cPosition].Y,word8x16[value[phase_id][cPosition]],position[cPosition].height,position[cPosition].width/8);
    if (cPosition<39) cPosition++;
    setCursor8x16(position[cPosition].X,position[cPosition].Y+16);
    */
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenVDPhaseMapTable::DoKey2Work(void)
{
try {
    /*
	clearCursor8x16(position[cPosition].X,position[cPosition].Y+16);
    value[phase_id][cPosition]=2;
    lcd240x128.DISPLAY_GRAPHIC_XY(position[cPosition].X,position[cPosition].Y,word8x16[value[phase_id][cPosition]],position[cPosition].height,position[cPosition].width/8);
    if (cPosition<39) cPosition++;
    setCursor8x16(position[cPosition].X,position[cPosition].Y+16);
    */
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenVDPhaseMapTable::DoKey3Work(void)
{
try {
    /*
	clearCursor8x16(position[cPosition].X,position[cPosition].Y+16);
    value[phase_id][cPosition]=3;
    lcd240x128.DISPLAY_GRAPHIC_XY(position[cPosition].X,position[cPosition].Y,word8x16[value[phase_id][cPosition]],position[cPosition].height,position[cPosition].width/8);
    if (cPosition<39) cPosition++;
    setCursor8x16(position[cPosition].X,position[cPosition].Y+16);
    */
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenVDPhaseMapTable::DoKey4Work(void)
{
try {
    /*
	clearCursor8x16(position[cPosition].X,position[cPosition].Y+16);
    value[phase_id][cPosition]=4;
    lcd240x128.DISPLAY_GRAPHIC_XY(position[cPosition].X,position[cPosition].Y,word8x16[value[phase_id][cPosition]],position[cPosition].height,position[cPosition].width/8);
    if (cPosition<39) cPosition++;
    setCursor8x16(position[cPosition].X,position[cPosition].Y+16);
    */
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenVDPhaseMapTable::DoKey5Work(void)
{
try {
	/*
    clearCursor8x16(position[cPosition].X,position[cPosition].Y+16);
    value[phase_id][cPosition]=5;
    lcd240x128.DISPLAY_GRAPHIC_XY(position[cPosition].X,position[cPosition].Y,word8x16[value[phase_id][cPosition]],position[cPosition].height,position[cPosition].width/8);
    if (cPosition<39) cPosition++;
    setCursor8x16(position[cPosition].X,position[cPosition].Y+16);
    */
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenVDPhaseMapTable::DoKey6Work(void)
{
try {
	/*
    clearCursor8x16(position[cPosition].X,position[cPosition].Y+16);
    value[phase_id][cPosition]=6;
    lcd240x128.DISPLAY_GRAPHIC_XY(position[cPosition].X,position[cPosition].Y,word8x16[value[phase_id][cPosition]],position[cPosition].height,position[cPosition].width/8);
    if (cPosition<39) cPosition++;
    setCursor8x16(position[cPosition].X,position[cPosition].Y+16);
    */
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenVDPhaseMapTable::DoKey7Work(void)
{
try {
	/*
    clearCursor8x16(position[cPosition].X,position[cPosition].Y+16);
    value[phase_id][cPosition]=7;
    lcd240x128.DISPLAY_GRAPHIC_XY(position[cPosition].X,position[cPosition].Y,word8x16[value[phase_id][cPosition]],position[cPosition].height,position[cPosition].width/8);
    if (cPosition<47) cPosition++;
    setCursor8x16(position[cPosition].X,position[cPosition].Y+16);
    */
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenVDPhaseMapTable::DoKey8Work(void)
{
try {
	/*
    clearCursor8x16(position[cPosition].X,position[cPosition].Y+16);
    value[phase_id][cPosition]=8;
    lcd240x128.DISPLAY_GRAPHIC_XY(position[cPosition].X,position[cPosition].Y,word8x16[value[phase_id][cPosition]],position[cPosition].height,position[cPosition].width/8);
    if (cPosition<47) cPosition++;
    setCursor8x16(position[cPosition].X,position[cPosition].Y+16);
    */
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenVDPhaseMapTable::DoKey9Work(void)
{
try {
    /*
	clearCursor8x16(position[cPosition].X,position[cPosition].Y+16);
    value[phase_id][cPosition]=9;
    lcd240x128.DISPLAY_GRAPHIC_XY(position[cPosition].X,position[cPosition].Y,word8x16[value[phase_id][cPosition]],position[cPosition].height,position[cPosition].width/8);
    if (cPosition<47) cPosition++;
    setCursor8x16(position[cPosition].X,position[cPosition].Y+16);
    */
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenVDPhaseMapTable::DoKeyAWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenVDPhaseMapTable::DoKeyBWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenVDPhaseMapTable::DoKeyCWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenVDPhaseMapTable::DoKeyDWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenVDPhaseMapTable::DoKeyEWork(void)
{
	if(phase_id==0)phase_id=1;
	phase_id--;
	DisplayVDTable();
	DisplahPhaseId();
}
//---------------------------------------------------------------------------
void ScreenVDPhaseMapTable::DoKeyFWork(void)
{
	if(phase_id==7)phase_id=6;
	phase_id++;
	DisplayVDTable();
	DisplahPhaseId();

}
//---------------------------------------------------------------------------
void ScreenVDPhaseMapTable::DoKeyF1Work(void)
{
try {
    screenMain.DisplayMain();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenVDPhaseMapTable::DoKeyF2Work(void)
{
	screenVDPhaseMapTableEdit.DisplayVDPhaseMapTableEdit(phase_id);
}
//---------------------------------------------------------------------------
void ScreenVDPhaseMapTable::DoKeyF3Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenVDPhaseMapTable::DoKeyF4Work(void)
{
try {
    screenControlStrategyMenu.DisplayControlStrategyMenu();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenVDPhaseMapTable::DoKeyEnterWork(void)
{
try {
	/*
	int _tmpcount=0,j=0;
	bool checkLcn=false;
	for(int i=0;i<8;i++){
		for(int y=0;y<8;y++){
			for(j=_tmpcount;j<_tmpcount+5;j++){
				if(j%5 == 0){
					usiLCN[i][y]=value[i][j]*10000;
				}else if(j%5==1){
					usiLCN[i][y]+=value[i][j]*1000;
				}else if(j%5==2){
					usiLCN[i][y]+=value[i][j]*100;
				}else if(j%5==3){
					usiLCN[i][y]+=value[i][j]*10;
				}else if(j%5==4){
					usiLCN[i][y]+=value[i][j];
				}
				//printf(">>[%d][%d]=%d value[%d][%d]=%d  ",i,y,usiLCN[i][y],i,j,value[i][j]);
			}
			_tmpcount=j;
			if(_tmpcount>=40) _tmpcount=0;
		if(usiLCN[i][j] >=0 && usiLCN[i][j] <=65535){ checkLcn[i][j] =true;}
		else { checkLcn[i][j] =false;}
		//printf("Modify VDPhaseMap[%d][%d]=%d Status:%d \n",i,y,usiLCN[i][y],checkLcn);
		printf("\n");
		}
		//_tmpcount=0;
	}
	for(int a=0;a<8;a++){
		for(int b=0;b<8;b++){
			if(checkLcn[a][b]){
				smem.vSetVDPhaseMapTable(a,b, usiLCN[a][b]);
				printf("SAVE LCN[%d][%d]=%d ",a,b,usiLCN[a][b]);
			}else{
				smem.vSetVDPhaseMapTable(a,b, 0);
			}
		}
			printf("\n");
	}


	*/
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenVDPhaseMapTable::DoKeyUPWork(void)
{
try {
    /*
	clearCursor8x16(position[cPosition].X,position[cPosition].Y+16);
    if (cPosition>=10) cPosition-=10;
    setCursor8x16(position[cPosition].X,position[cPosition].Y+16);
    */
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenVDPhaseMapTable::DoKeyDOWNWork(void)
{
try {
    /*
	clearCursor8x16(position[cPosition].X,position[cPosition].Y+16);
    if (cPosition<=30) cPosition+=10;
    setCursor8x16(position[cPosition].X,position[cPosition].Y+16);
    */
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenVDPhaseMapTable::DoKeyLEFTWork(void)
{
try {
    /*
	clearCursor8x16(position[cPosition].X,position[cPosition].Y+16);
    if (cPosition==0) cPosition=1;
    cPosition--;
    setCursor8x16(position[cPosition].X,position[cPosition].Y+16);
    */
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenVDPhaseMapTable::DoKeyRIGHTWork(void)
{
try {
	/*
    clearCursor8x16(position[cPosition].X,position[cPosition].Y+16);
    cPosition++;
    if (cPosition>=40) cPosition=39;
    setCursor8x16(position[cPosition].X,position[cPosition].Y+16);
	*/
  } catch (...) {}
}
//---------------------------------------------------------------------------


