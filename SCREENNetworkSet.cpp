#include "SCREENNetworkSet.h"
#include "SCREENNetmaskSet.h"
#include "SCREENNetworkSetConfirm.h"
#include "SCREENMain.h"
//#include "SCREENCtlSetup.h"
#include "SCREENSystemInfoMenu.h"
#include "LCD240x128.h"
#include "SMEM.h"

SCREENNetworkSet screenNetworkSet;
//---------------------------------------------------------------------------
SCREENNetworkSet::SCREENNetworkSet(void)
{
    LoadBitmapFromFile();
    InitDispWord();
}
//---------------------------------------------------------------------------
SCREENNetworkSet::~SCREENNetworkSet(void)
{
}
//---------------------------------------------------------------------------
void SCREENNetworkSet::DoKeyWork(BYTE key)                                           //已�\uFFFD�\uFFFD\uFFFD��\uFFFD\uFFFD\uFFFD\uFFFD0x80~0x98�\uFFFD\uFFFD
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
void SCREENNetworkSet::LoadBitmapFromFile(void)
{
try {
    FILE *bitmap;
    bitmap=fopen("//cct//bitmap//backGround//1_0_F2_NetworkSet.bit","rb");
    if (bitmap) {
        fread(networkSetBitmap,3840,1,bitmap);
        fclose(bitmap);
    }
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENNetworkSet::DisplayNetworkSet(void)
{
try {
    smem.SetcFace(cNETWORKSET);
    smem.vWriteMsgToDOM("Enter Network Setup Screen");
    lcd240x128.DISPLAY_GRAPHIC(0,networkSetBitmap,128,30);
    cPosition=0;
    setCursor8x16(position[cPosition].X,position[cPosition].Y+16);

    DisplayLocalIp1();
    DisplayDistIp0();
    DisplayDistIp();

  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENNetworkSet::DisplayMainNetworkSet(void)
{
try {
    smem.SetcFace(cNETWORKSET_MAIN);
    smem.vWriteMsgToDOM("Enter Network Setup Screen");
    lcd240x128.DISPLAY_GRAPHIC(0,networkSetBitmap,128,30);
    cPosition=0;
    setCursor8x16(position[cPosition].X,position[cPosition].Y+16);

    DisplayLocalIp1();
    DisplayDistIp0();
    DisplayDistIp();

  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENNetworkSet::InitDispWord(void)
{
try {

    for (int i=0;i<48;i++) {
         value[i]=0;

         position[i].width=8;
         position[i].height=16;

         if (i%16==0)  position[i].X=64;
         else if (i%16==1) position[i].X=72;
         else if (i%16==2) position[i].X=80;
         else if (i%16==3) position[i].X=96;
         else if (i%16==4) position[i].X=104;
         else if (i%16==5) position[i].X=112;
         else if (i%16==6) position[i].X=128;
         else if (i%16==7) position[i].X=136;
         else if (i%16==8) position[i].X=144;
         else if (i%16==9) position[i].X=160;
         else if (i%16==10) position[i].X=168;
         else if (i%16==11) position[i].X=176;
         else if (i%16==12) position[i].X=192;
         else if (i%16==13) position[i].X=200;
         else if (i%16==14) position[i].X=208;
         else if (i%16==15) position[i].X=216;

         if (i>=0 && i<=15) position[i].Y=32;
         else if (i>=16 && i<=31) position[i].Y=52;
         else if (i>=32 && i<=47) position[i].Y=72;

    }

  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENNetworkSet::DisplayLocalIp1(void)
{
try {

    localIp1_1=smem.GetLocalIP1(1);
    localIp1_2=smem.GetLocalIP1(2);
    localIp1_3=smem.GetLocalIP1(3);
    localIp1_4=smem.GetLocalIP1(4);
    localPort1=smem.GetLocalIP1(5);

    value[0]=localIp1_1/100;    value[1]=(localIp1_1%100)/10;    value[2]=localIp1_1%10;
    value[3]=localIp1_2/100;    value[4]=(localIp1_2%100)/10;    value[5]=localIp1_2%10;
    value[6]=localIp1_3/100;    value[7]=(localIp1_3%100)/10;    value[8]=localIp1_3%10;
    value[9]=localIp1_4/100;    value[10]=(localIp1_4%100)/10;   value[11]=localIp1_4%10;
    value[12]=localPort1/1000;    value[13]=(localPort1%1000)/100;    value[14]=(localPort1%100)/10;    value[15]=localPort1%10;

    lcd240x128.DISPLAY_GRAPHIC_XY(position[0].X,position[0].Y,word8x16[value[0]],position[0].height,position[0].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(position[1].X,position[1].Y,word8x16[value[1]],position[1].height,position[1].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(position[2].X,position[2].Y,word8x16[value[2]],position[2].height,position[2].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(position[3].X,position[3].Y,word8x16[value[3]],position[3].height,position[3].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(position[4].X,position[4].Y,word8x16[value[4]],position[4].height,position[4].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(position[5].X,position[5].Y,word8x16[value[5]],position[5].height,position[5].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(position[6].X,position[6].Y,word8x16[value[6]],position[6].height,position[6].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(position[7].X,position[7].Y,word8x16[value[7]],position[7].height,position[7].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(position[8].X,position[8].Y,word8x16[value[8]],position[8].height,position[8].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(position[9].X,position[9].Y,word8x16[value[9]],position[9].height,position[9].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(position[10].X,position[10].Y,word8x16[value[10]],position[10].height,position[10].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(position[11].X,position[11].Y,word8x16[value[11]],position[11].height,position[11].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(position[12].X,position[12].Y,word8x16[value[12]],position[12].height,position[12].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(position[13].X,position[13].Y,word8x16[value[13]],position[13].height,position[13].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(position[14].X,position[14].Y,word8x16[value[14]],position[14].height,position[14].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(position[15].X,position[15].Y,word8x16[value[15]],position[15].height,position[15].width/8);

  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENNetworkSet::DisplayDistIp0(void)
{
try {

    distIp0_1=smem.GetdistIp0(1);
    distIp0_2=smem.GetdistIp0(2);
    distIp0_3=smem.GetdistIp0(3);
    distIp0_4=smem.GetdistIp0(4);
    dist0Port=smem.GetdistIp0(5);

    value[16]=distIp0_1/100;    value[17]=(distIp0_1%100)/10;    value[18]=distIp0_1%10;
    value[19]=distIp0_2/100;    value[20]=(distIp0_2%100)/10;    value[21]=distIp0_2%10;
    value[22]=distIp0_3/100;    value[23]=(distIp0_3%100)/10;    value[24]=distIp0_3%10;
    value[25]=distIp0_4/100;    value[26]=(distIp0_4%100)/10;    value[27]=distIp0_4%10;
    value[28]=dist0Port/1000;    value[29]=(dist0Port%1000)/100;    value[30]=(dist0Port%100)/10;    value[31]=dist0Port%10;

    lcd240x128.DISPLAY_GRAPHIC_XY(position[16].X,position[16].Y,word8x16[value[16]],position[16].height,position[16].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(position[17].X,position[17].Y,word8x16[value[17]],position[17].height,position[17].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(position[18].X,position[18].Y,word8x16[value[18]],position[18].height,position[18].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(position[19].X,position[19].Y,word8x16[value[19]],position[19].height,position[19].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(position[20].X,position[20].Y,word8x16[value[20]],position[20].height,position[20].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(position[21].X,position[21].Y,word8x16[value[21]],position[21].height,position[21].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(position[22].X,position[22].Y,word8x16[value[22]],position[22].height,position[22].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(position[23].X,position[23].Y,word8x16[value[23]],position[23].height,position[23].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(position[24].X,position[24].Y,word8x16[value[24]],position[24].height,position[24].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(position[25].X,position[25].Y,word8x16[value[25]],position[25].height,position[25].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(position[26].X,position[26].Y,word8x16[value[26]],position[26].height,position[26].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(position[27].X,position[27].Y,word8x16[value[27]],position[27].height,position[27].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(position[28].X,position[28].Y,word8x16[value[28]],position[28].height,position[28].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(position[29].X,position[29].Y,word8x16[value[29]],position[29].height,position[29].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(position[30].X,position[30].Y,word8x16[value[30]],position[30].height,position[30].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(position[31].X,position[31].Y,word8x16[value[31]],position[31].height,position[31].width/8);

  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENNetworkSet::DisplayDistIp(void)
{
try {

    distIp1=smem.GetDistIP(1);
    distIp2=smem.GetDistIP(2);
    distIp3=smem.GetDistIP(3);
    distIp4=smem.GetDistIP(4);
    distPort=smem.GetDistIP(5);

    value[32]=distIp1/100;    value[33]=(distIp1%100)/10;    value[34]=distIp1%10;
    value[35]=distIp2/100;    value[36]=(distIp2%100)/10;    value[37]=distIp2%10;
    value[38]=distIp3/100;    value[39]=(distIp3%100)/10;    value[40]=distIp3%10;
    value[41]=distIp4/100;    value[42]=(distIp4%100)/10;    value[43]=distIp4%10;
    value[44]=distPort/1000;    value[45]=(distPort%1000)/100;    value[46]=(distPort%100)/10;    value[47]=distPort%10;

    lcd240x128.DISPLAY_GRAPHIC_XY(position[32].X,position[32].Y,word8x16[value[32]],position[32].height,position[32].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(position[33].X,position[33].Y,word8x16[value[33]],position[33].height,position[33].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(position[34].X,position[34].Y,word8x16[value[34]],position[34].height,position[34].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(position[35].X,position[35].Y,word8x16[value[35]],position[35].height,position[35].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(position[36].X,position[36].Y,word8x16[value[36]],position[36].height,position[36].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(position[37].X,position[37].Y,word8x16[value[37]],position[37].height,position[37].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(position[38].X,position[38].Y,word8x16[value[38]],position[38].height,position[38].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(position[39].X,position[39].Y,word8x16[value[39]],position[39].height,position[39].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(position[40].X,position[40].Y,word8x16[value[40]],position[40].height,position[40].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(position[41].X,position[41].Y,word8x16[value[41]],position[41].height,position[41].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(position[42].X,position[42].Y,word8x16[value[42]],position[42].height,position[42].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(position[43].X,position[43].Y,word8x16[value[43]],position[43].height,position[43].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(position[44].X,position[44].Y,word8x16[value[44]],position[44].height,position[44].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(position[45].X,position[45].Y,word8x16[value[45]],position[45].height,position[45].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(position[46].X,position[46].Y,word8x16[value[46]],position[46].height,position[46].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(position[47].X,position[47].Y,word8x16[value[47]],position[47].height,position[47].width/8);

  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENNetworkSet::DoKey0Work(void)
{
try {
    clearCursor8x16(position[cPosition].X,position[cPosition].Y+16);
    value[cPosition]=0;
    lcd240x128.DISPLAY_GRAPHIC_XY(position[cPosition].X,position[cPosition].Y,word8x16[value[cPosition]],position[cPosition].height,position[cPosition].width/8);
    if (cPosition<47) cPosition++;
    setCursor8x16(position[cPosition].X,position[cPosition].Y+16);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENNetworkSet::DoKey1Work(void)
{
try {
    clearCursor8x16(position[cPosition].X,position[cPosition].Y+16);
    value[cPosition]=1;
    lcd240x128.DISPLAY_GRAPHIC_XY(position[cPosition].X,position[cPosition].Y,word8x16[value[cPosition]],position[cPosition].height,position[cPosition].width/8);
    if (cPosition<47) cPosition++;
    setCursor8x16(position[cPosition].X,position[cPosition].Y+16);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENNetworkSet::DoKey2Work(void)
{
try {
    clearCursor8x16(position[cPosition].X,position[cPosition].Y+16);
    value[cPosition]=2;
    lcd240x128.DISPLAY_GRAPHIC_XY(position[cPosition].X,position[cPosition].Y,word8x16[value[cPosition]],position[cPosition].height,position[cPosition].width/8);
    if (cPosition<47) cPosition++;
    setCursor8x16(position[cPosition].X,position[cPosition].Y+16);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENNetworkSet::DoKey3Work(void)
{
try {
    clearCursor8x16(position[cPosition].X,position[cPosition].Y+16);
    value[cPosition]=3;
    lcd240x128.DISPLAY_GRAPHIC_XY(position[cPosition].X,position[cPosition].Y,word8x16[value[cPosition]],position[cPosition].height,position[cPosition].width/8);
    if (cPosition<47) cPosition++;
    setCursor8x16(position[cPosition].X,position[cPosition].Y+16);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENNetworkSet::DoKey4Work(void)
{
try {
    clearCursor8x16(position[cPosition].X,position[cPosition].Y+16);
    value[cPosition]=4;
    lcd240x128.DISPLAY_GRAPHIC_XY(position[cPosition].X,position[cPosition].Y,word8x16[value[cPosition]],position[cPosition].height,position[cPosition].width/8);
    if (cPosition<47) cPosition++;
    setCursor8x16(position[cPosition].X,position[cPosition].Y+16);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENNetworkSet::DoKey5Work(void)
{
try {
    clearCursor8x16(position[cPosition].X,position[cPosition].Y+16);
    value[cPosition]=5;
    lcd240x128.DISPLAY_GRAPHIC_XY(position[cPosition].X,position[cPosition].Y,word8x16[value[cPosition]],position[cPosition].height,position[cPosition].width/8);
    if (cPosition<47) cPosition++;
    setCursor8x16(position[cPosition].X,position[cPosition].Y+16);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENNetworkSet::DoKey6Work(void)
{
try {
    clearCursor8x16(position[cPosition].X,position[cPosition].Y+16);
    value[cPosition]=6;
    lcd240x128.DISPLAY_GRAPHIC_XY(position[cPosition].X,position[cPosition].Y,word8x16[value[cPosition]],position[cPosition].height,position[cPosition].width/8);
    if (cPosition<47) cPosition++;
    setCursor8x16(position[cPosition].X,position[cPosition].Y+16);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENNetworkSet::DoKey7Work(void)
{
try {
    clearCursor8x16(position[cPosition].X,position[cPosition].Y+16);
    value[cPosition]=7;
    lcd240x128.DISPLAY_GRAPHIC_XY(position[cPosition].X,position[cPosition].Y,word8x16[value[cPosition]],position[cPosition].height,position[cPosition].width/8);
    if (cPosition<47) cPosition++;
    setCursor8x16(position[cPosition].X,position[cPosition].Y+16);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENNetworkSet::DoKey8Work(void)
{
try {
    clearCursor8x16(position[cPosition].X,position[cPosition].Y+16);
    value[cPosition]=8;
    lcd240x128.DISPLAY_GRAPHIC_XY(position[cPosition].X,position[cPosition].Y,word8x16[value[cPosition]],position[cPosition].height,position[cPosition].width/8);
    if (cPosition<47) cPosition++;
    setCursor8x16(position[cPosition].X,position[cPosition].Y+16);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENNetworkSet::DoKey9Work(void)
{
try {
    clearCursor8x16(position[cPosition].X,position[cPosition].Y+16);
    value[cPosition]=9;
    lcd240x128.DISPLAY_GRAPHIC_XY(position[cPosition].X,position[cPosition].Y,word8x16[value[cPosition]],position[cPosition].height,position[cPosition].width/8);
    if (cPosition<47) cPosition++;
    setCursor8x16(position[cPosition].X,position[cPosition].Y+16);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENNetworkSet::DoKeyAWork(void)
{
}
//---------------------------------------------------------------------------
void SCREENNetworkSet::DoKeyBWork(void)
{
}
//---------------------------------------------------------------------------
void SCREENNetworkSet::DoKeyCWork(void)
{
}
//---------------------------------------------------------------------------
void SCREENNetworkSet::DoKeyDWork(void)
{
}
//---------------------------------------------------------------------------
void SCREENNetworkSet::DoKeyEWork(void)
{
}
//---------------------------------------------------------------------------
void SCREENNetworkSet::DoKeyFWork(void)
{
}
//---------------------------------------------------------------------------
void SCREENNetworkSet::DoKeyF1Work(void)
{
try {
    screenMain.DisplayMain();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENNetworkSet::DoKeyF2Work(void)
{
}
//---------------------------------------------------------------------------
void SCREENNetworkSet::DoKeyF3Work(void)
{
}
//---------------------------------------------------------------------------
void SCREENNetworkSet::DoKeyF4Work(void)
{
try {
	if(smem.GetcFace()==cNETWORKSET){
		//screenCtlSetup.DisplayCtlSetup();
		screenSystemInfoMenu.DisplaySystemInfoMenu();
	}else if(smem.GetcFace()==cNETWORKSET_MAIN){
		screenMain.DisplayMain();
	}
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENNetworkSet::DoKeyEnterWork(void)
{
try {

    localIp1_1=value[0]*100+value[1]*10+value[2];
    localIp1_2=value[3]*100+value[4]*10+value[5];
    localIp1_3=value[6]*100+value[7]*10+value[8];
    localIp1_4=value[9]*100+value[10]*10+value[11];
    localPort1=value[12]*1000+value[13]*100+value[14]*10+value[15];

    distIp0_1=value[16]*100+value[17]*10+value[18];
    distIp0_2=value[19]*100+value[20]*10+value[21];
    distIp0_3=value[22]*100+value[23]*10+value[24];
    distIp0_4=value[25]*100+value[26]*10+value[27];
    dist0Port=value[28]*1000+value[29]*100+value[30]*10+value[31];

    distIp1=value[32]*100+value[33]*10+value[34];
    distIp2=value[35]*100+value[36]*10+value[37];
    distIp3=value[38]*100+value[39]*10+value[40];
    distIp4=value[41]*100+value[42]*10+value[43];
    distPort=value[44]*1000+value[45]*100+value[46]*10+value[47];

    printf("%d.%d.%d.%d:%d\n",localIp1_1,localIp1_2,localIp1_3,localIp1_4,localPort1);
    printf("%d.%d.%d.%d:%d\n",distIp0_1,distIp0_2,distIp0_3,distIp0_4,dist0Port);
    printf("%d.%d.%d.%d:%d\n",distIp1,distIp2,distIp3,distIp4,distPort);


    //æª¢æ\uFFFD\uFFFD\uFFFD\uFFFD\uFFFD\uFFFD
    if  ((localIp1_1>=0 && localIp1_1<=255) && (localIp1_2>=0 && localIp1_2<=255) && (localIp1_3>=0 && localIp1_3<=255) && (localIp1_4>=0 && localIp1_4<=255) &&
         (distIp0_1>=0 && distIp0_1<=255) && (distIp0_2>=0 && distIp0_2<=255) && (distIp0_3>=0 && distIp0_3<=255) && (distIp0_4>=0 && distIp0_4<=255) &&
         (distIp1>=0 && distIp1<=255) && (distIp2>=0 && distIp2<=255) && (distIp3>=0 && distIp3<=255) && (distIp4>=0 && distIp4<=255) &&
         (localPort1>=0 && localPort1<=9999) && (dist0Port>=0 && dist0Port<=9999) && (distPort>=0 && distPort<=9999)) {
          //æª¢æ\uFFFD\uFFFD\uFFFD\uFFFD\uFFFD\uFFFDå¿\uFFFD\uFFFD\uFFFD\uFFFD¿®\uFFFD¹é\uFFFD\uFFFD\uFFFD\uFFFDä»\uFFFD

          /* OT20111020
          screenNetworkSetConfirm.DisplayNetworkSetConfirm(localIp1_1,localIp1_2,localIp1_3,localIp1_4,localPort1,
                                                                   distIp0_1,distIp0_2,distIp0_3,distIp0_4,dist0Port,
                                                                   distIp1,distIp2,distIp3,distIp4,distPort);
          */
          screenNetmaskSet.DisplayNetmaskSet(localIp1_1,localIp1_2,localIp1_3,localIp1_4,localPort1,
                                                                   distIp0_1,distIp0_2,distIp0_3,distIp0_4,dist0Port,
                                                                   distIp1,distIp2,distIp3,distIp4,distPort);
    } else {    //ä¸\uFFFD\uFFFD\uFFFD\uFFFD

          localIp1_1=smem.GetLocalIP1(1);
          localIp1_2=smem.GetLocalIP1(2);
          localIp1_3=smem.GetLocalIP1(3);
          localIp1_4=smem.GetLocalIP1(4);
          localPort1=smem.GetLocalIP1(5);
          distIp0_1=smem.GetdistIp0(1);
          distIp0_2=smem.GetdistIp0(2);
          distIp0_3=smem.GetdistIp0(3);
          distIp0_4=smem.GetdistIp0(4);
          dist0Port=smem.GetdistIp0(5);
          distIp1=smem.GetDistIP(1);
          distIp2=smem.GetDistIP(2);
          distIp3=smem.GetDistIP(3);
          distIp4=smem.GetDistIP(4);
          distPort=smem.GetDistIP(5);

          printf("%d.%d.%d.%d:%d\n",localIp1_1,localIp1_2,localIp1_3,localIp1_4,localPort1);
          printf("%d.%d.%d.%d:%d\n",distIp0_1,distIp0_2,distIp0_3,distIp0_4,dist0Port);
          printf("%d.%d.%d.%d:%d\n",distIp1,distIp2,distIp3,distIp4,distPort);
    }

  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENNetworkSet::DoKeyUPWork(void)
{
try {
    clearCursor8x16(position[cPosition].X,position[cPosition].Y+16);
    if (cPosition>=16) cPosition-=16;
    setCursor8x16(position[cPosition].X,position[cPosition].Y+16);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENNetworkSet::DoKeyDOWNWork(void)
{
try {
    clearCursor8x16(position[cPosition].X,position[cPosition].Y+16);
    if (cPosition<=31) cPosition+=16;
    setCursor8x16(position[cPosition].X,position[cPosition].Y+16);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENNetworkSet::DoKeyLEFTWork(void)
{
try {
    clearCursor8x16(position[cPosition].X,position[cPosition].Y+16);
    if (cPosition==0) cPosition=1;
    cPosition--;
    setCursor8x16(position[cPosition].X,position[cPosition].Y+16);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENNetworkSet::DoKeyRIGHTWork(void)
{
try {
    clearCursor8x16(position[cPosition].X,position[cPosition].Y+16);
    cPosition++;
    if (cPosition>=48) cPosition=47;
    setCursor8x16(position[cPosition].X,position[cPosition].Y+16);

  } catch (...) {}
}
//---------------------------------------------------------------------------
