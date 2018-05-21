#include "screenVDPhaseMapTableEdit.h"
#include "screenVDPhaseMapTable.h"
#include "SCREENMain.h"
#include "LCD240x128.h"
#include "SMEM.h"
#include "CSTORINI.h"
#include "SCREENActuateStrategyMenu.h"

ScreenVDPhaseMapTableEdit screenVDPhaseMapTableEdit;
//---------------------------------------------------------------------------
ScreenVDPhaseMapTableEdit::ScreenVDPhaseMapTableEdit(void)
{
    LoadBitmapFromFile();
    InitDispWord();
    phase_id=0;
}
//---------------------------------------------------------------------------
ScreenVDPhaseMapTableEdit::~ScreenVDPhaseMapTableEdit(void)
{
}
//---------------------------------------------------------------------------
void ScreenVDPhaseMapTableEdit::DoKeyWork(BYTE key)                                           //已�\uFFFD�\uFFFD\uFFFD��\uFFFD\uFFFD\uFFFD\uFFFD0x80~0x98�\uFFFD\uFFFD
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
void ScreenVDPhaseMapTableEdit::LoadBitmapFromFile(void)
{
try {
    FILE *bitmap;
    bitmap=fopen("//cct//bitmap//backGround//actmenu//vdPhaseMapTableEdit.bit","rb");
    if (bitmap) {
        fread(networkSetBitmap,3840,1,bitmap);
        fclose(bitmap);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenVDPhaseMapTableEdit::DisplayVDPhaseMapTableEdit(int _tmpid)
{
try {
    smem.SetcFace(cVDPHASEMAPEDIT);
    smem.vWriteMsgToDOM("Enter VDPhaseMapTableEdit Setup Screen");
    lcd240x128.DISPLAY_GRAPHIC(0,networkSetBitmap,128,30);
    cPosition=0;
    setCursor8x16(position[cPosition].X,position[cPosition].Y+16);
    for(int i=0;i<8;i++){
        for(int j=0;j<8;j++){
        	usiLCN[i][j]=smem.vGetVDPhaseMapTable(i,j);
            printf("DisplayVDPhaseMapTableEdit phase%d_%d=%d\n",i,j,usiLCN[i][j]);
        }
    }
    phase_id=_tmpid;
    DisplayVDTableEdit();
    DisplahPhaseId();

  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenVDPhaseMapTableEdit::InitDispWord(void)
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
void ScreenVDPhaseMapTableEdit::DisplahPhaseId()
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
void ScreenVDPhaseMapTableEdit::DisplayVDTableEdit(void)
{
try {
	int _tmpcount=0,j=0;
	for(int i=0;i<8;i++){
		//usiLCN[phase_id][i]=smem.vGetVDPhaseMapTable(phase_id,i);
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
void ScreenVDPhaseMapTableEdit::DisplaySaveValue()
{
try{
	printf("DisplaySaveValue phase_id:%d\n",phase_id);
	for(int i=0;i<40;i++ ){
        printf("[%d]:%d ",i,value[phase_id][i]);
		lcd240x128.DISPLAY_GRAPHIC_XY(position[i].X,position[i].Y,word8x16[value[phase_id][i]],position[i].height,position[i].width/8);
	}
	printf("\n");
} catch (...) {}
}
//---------------------------------------------------------------------------

void ScreenVDPhaseMapTableEdit::DoKey0Work(void)
{
try {
    clearCursor8x16(position[cPosition].X,position[cPosition].Y+16);
    value[phase_id][cPosition]=0;
    lcd240x128.DISPLAY_GRAPHIC_XY(position[cPosition].X,position[cPosition].Y,word8x16[value[phase_id][cPosition]],position[cPosition].height,position[cPosition].width/8);
    if (cPosition<39) cPosition++;
    setCursor8x16(position[cPosition].X,position[cPosition].Y+16);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenVDPhaseMapTableEdit::DoKey1Work(void)
{
try {
    clearCursor8x16(position[cPosition].X,position[cPosition].Y+16);
    value[phase_id][cPosition]=1;
    lcd240x128.DISPLAY_GRAPHIC_XY(position[cPosition].X,position[cPosition].Y,word8x16[value[phase_id][cPosition]],position[cPosition].height,position[cPosition].width/8);
    if (cPosition<39) cPosition++;
    setCursor8x16(position[cPosition].X,position[cPosition].Y+16);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenVDPhaseMapTableEdit::DoKey2Work(void)
{
try {
    clearCursor8x16(position[cPosition].X,position[cPosition].Y+16);
    value[phase_id][cPosition]=2;
    lcd240x128.DISPLAY_GRAPHIC_XY(position[cPosition].X,position[cPosition].Y,word8x16[value[phase_id][cPosition]],position[cPosition].height,position[cPosition].width/8);
    if (cPosition<39) cPosition++;
    setCursor8x16(position[cPosition].X,position[cPosition].Y+16);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenVDPhaseMapTableEdit::DoKey3Work(void)
{
try {
    clearCursor8x16(position[cPosition].X,position[cPosition].Y+16);
    value[phase_id][cPosition]=3;
    lcd240x128.DISPLAY_GRAPHIC_XY(position[cPosition].X,position[cPosition].Y,word8x16[value[phase_id][cPosition]],position[cPosition].height,position[cPosition].width/8);
    if (cPosition<39) cPosition++;
    setCursor8x16(position[cPosition].X,position[cPosition].Y+16);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenVDPhaseMapTableEdit::DoKey4Work(void)
{
try {
    clearCursor8x16(position[cPosition].X,position[cPosition].Y+16);
    value[phase_id][cPosition]=4;
    lcd240x128.DISPLAY_GRAPHIC_XY(position[cPosition].X,position[cPosition].Y,word8x16[value[phase_id][cPosition]],position[cPosition].height,position[cPosition].width/8);
    if (cPosition<39) cPosition++;
    setCursor8x16(position[cPosition].X,position[cPosition].Y+16);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenVDPhaseMapTableEdit::DoKey5Work(void)
{
try {
    clearCursor8x16(position[cPosition].X,position[cPosition].Y+16);
    value[phase_id][cPosition]=5;
    lcd240x128.DISPLAY_GRAPHIC_XY(position[cPosition].X,position[cPosition].Y,word8x16[value[phase_id][cPosition]],position[cPosition].height,position[cPosition].width/8);
    if (cPosition<39) cPosition++;
    setCursor8x16(position[cPosition].X,position[cPosition].Y+16);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenVDPhaseMapTableEdit::DoKey6Work(void)
{
try {
    clearCursor8x16(position[cPosition].X,position[cPosition].Y+16);
    value[phase_id][cPosition]=6;
    lcd240x128.DISPLAY_GRAPHIC_XY(position[cPosition].X,position[cPosition].Y,word8x16[value[phase_id][cPosition]],position[cPosition].height,position[cPosition].width/8);
    if (cPosition<39) cPosition++;
    setCursor8x16(position[cPosition].X,position[cPosition].Y+16);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenVDPhaseMapTableEdit::DoKey7Work(void)
{
try {
    clearCursor8x16(position[cPosition].X,position[cPosition].Y+16);
    value[phase_id][cPosition]=7;
    lcd240x128.DISPLAY_GRAPHIC_XY(position[cPosition].X,position[cPosition].Y,word8x16[value[phase_id][cPosition]],position[cPosition].height,position[cPosition].width/8);
    if (cPosition<47) cPosition++;
    setCursor8x16(position[cPosition].X,position[cPosition].Y+16);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenVDPhaseMapTableEdit::DoKey8Work(void)
{
try {
    clearCursor8x16(position[cPosition].X,position[cPosition].Y+16);
    value[phase_id][cPosition]=8;
    printf("value[%d][%d]=%X \n",phase_id,cPosition,value[phase_id][cPosition]);
    lcd240x128.DISPLAY_GRAPHIC_XY(position[cPosition].X,position[cPosition].Y,word8x16[value[phase_id][cPosition]],position[cPosition].height,position[cPosition].width/8);
    if (cPosition<47) cPosition++;
    setCursor8x16(position[cPosition].X,position[cPosition].Y+16);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenVDPhaseMapTableEdit::DoKey9Work(void)
{
try {
    clearCursor8x16(position[cPosition].X,position[cPosition].Y+16);
    value[phase_id][cPosition]=9;
    lcd240x128.DISPLAY_GRAPHIC_XY(position[cPosition].X,position[cPosition].Y,word8x16[value[phase_id][cPosition]],position[cPosition].height,position[cPosition].width/8);
    if (cPosition<47) cPosition++;
    setCursor8x16(position[cPosition].X,position[cPosition].Y+16);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenVDPhaseMapTableEdit::DoKeyAWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenVDPhaseMapTableEdit::DoKeyBWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenVDPhaseMapTableEdit::DoKeyCWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenVDPhaseMapTableEdit::DoKeyDWork(void)
{
}
//---------------------------------------------------------------------------
void ScreenVDPhaseMapTableEdit::DoKeyEWork(void)
{
	if(phase_id==0)phase_id=1;
	phase_id--;
	DisplaySaveValue();
	DisplahPhaseId();
}
//---------------------------------------------------------------------------
void ScreenVDPhaseMapTableEdit::DoKeyFWork(void)
{
	if(phase_id==7)phase_id=6;
	phase_id++;
	DisplaySaveValue();
	DisplahPhaseId();

}
//---------------------------------------------------------------------------
void ScreenVDPhaseMapTableEdit::DoKeyF1Work(void)
{
try {
    screenMain.DisplayMain();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenVDPhaseMapTableEdit::DoKeyF2Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenVDPhaseMapTableEdit::DoKeyF3Work(void)
{
}
//---------------------------------------------------------------------------
void ScreenVDPhaseMapTableEdit::DoKeyF4Work(void)
{
try {
	//screenVDPhaseMapTable.DisplayVDPhaseMapTable();
	screenActuateStrategyMenu.DisplayActuateStrategyMenu();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenVDPhaseMapTableEdit::DoKeyEnterWork(void)
{
try {

	int _tmpcount=0,j=0;
	bool checkLcn[8][40];
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
		if(usiLCN[i][y] >=0 && usiLCN[i][y] <=65535){ checkLcn[i][y] =true;}
		else { checkLcn[i][j] =false;}
		//printf("Modify VDPhaseMap[%d][%d]=%d Status:%d \n",i,y,usiLCN[i][y],checkLcn[i][y]);
		//printf("\n");
		}
		//_tmpcount=0;
	}
	char tmp[256] = {0};

	for(int a=0;a<8;a++){
		for(int b=0;b<8;b++){
			//printf("S(%d,%d)=%d",a,b,checkLcn[a][b]);
			if(checkLcn[a][b]){
				smem.vSetVDPhaseMapTable(a,b, usiLCN[a][b]);
				printf("SAVE LCN[%d][%d]=%d ",a,b,usiLCN[a][b]);
				sprintf(tmp,"Phase%d_%d",a,b);
				printf(" %s ",tmp);
				oStorINI.vSaveIntToINI("VD", tmp, usiLCN[a][b]);
			}
		}
			//printf("\n");
	}
	screenActuateStrategyMenu.DisplayActuateStrategyMenu();


  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenVDPhaseMapTableEdit::DoKeyUPWork(void)
{
try {
    clearCursor8x16(position[cPosition].X,position[cPosition].Y+16);
    if (cPosition>=10) cPosition-=10;
    setCursor8x16(position[cPosition].X,position[cPosition].Y+16);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenVDPhaseMapTableEdit::DoKeyDOWNWork(void)
{
try {
    clearCursor8x16(position[cPosition].X,position[cPosition].Y+16);
    if (cPosition<=30) cPosition+=10;
    setCursor8x16(position[cPosition].X,position[cPosition].Y+16);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenVDPhaseMapTableEdit::DoKeyLEFTWork(void)
{
try {
    clearCursor8x16(position[cPosition].X,position[cPosition].Y+16);
    if (cPosition==0) cPosition=1;
    cPosition--;
    setCursor8x16(position[cPosition].X,position[cPosition].Y+16);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void ScreenVDPhaseMapTableEdit::DoKeyRIGHTWork(void)
{
try {
    clearCursor8x16(position[cPosition].X,position[cPosition].Y+16);
    cPosition++;
    if (cPosition>=40) cPosition=39;
    setCursor8x16(position[cPosition].X,position[cPosition].Y+16);

  } catch (...) {}
}
//---------------------------------------------------------------------------


