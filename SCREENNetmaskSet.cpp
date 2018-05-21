#include "SCREENNetworkSet.h"
#include "SCREENNetmaskSet.h"
#include "SCREENNetworkSetConfirm.h"
#include "SCREENMain.h"
#include "SCREENCtlSetup.h"
#include "LCD240x128.h"
#include "SMEM.h"

SCREENNetmaskSet screenNetmaskSet;
//---------------------------------------------------------------------------
SCREENNetmaskSet::SCREENNetmaskSet(void)
{
    LoadBitmapFromFile();
    InitDispWord();
}
//---------------------------------------------------------------------------
SCREENNetmaskSet::~SCREENNetmaskSet(void)
{
}
//---------------------------------------------------------------------------
void SCREENNetmaskSet::DoKeyWork(BYTE key)                                           //å·²é\uFFFDæ¿\uFFFD\uFFFD²ä\uFFFD\uFFFD\uFFFD\uFFFD0x80~0x98ä¹\uFFFD\uFFFD
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
void SCREENNetmaskSet::LoadBitmapFromFile(void)
{
try {
    FILE *bitmap;
    bitmap=fopen("//cct//bitmap//backGround//NetmaskSet.bit","rb");
    if (bitmap) {
        fread(netmaskSetBitmap,3840,1,bitmap);
        fclose(bitmap);
    }
  } catch (...) {}
}


//---------------------------------------------------------------------------
void SCREENNetmaskSet::DisplayNetmaskSet(int a,int b,int c,int d,int e,
                                                       int f,int g,int h,int i,int j,
                                                       int k,int l,int m,int n,int o)
{
try {

    smem.SetcFace(cNETMASKSET);
    smem.vWriteMsgToDOM("Enter Netmask Setup Screen");
    lcd240x128.DISPLAY_GRAPHIC(0, netmaskSetBitmap,128,30);
    localIp1_1=a;
    localIp1_2=b;
    localIp1_3=c;
    localIp1_4=d;
    localPort1=e;

    distIp0_1 = f;
    distIp0_2 = g;
    distIp0_3 = h;
    distIp0_4 = i;
    dist0Port = j;

    distIp1=k;
    distIp2=l;
    distIp3=m;
    distIp4=n;
    distPort=o;

    gateway1 = localIp1_1;
    gateway2 = localIp1_2;
    gateway3 = localIp1_3;

    DisplayNetmask();
    DisplayGateway();

  } catch(...) {}
}



//---------------------------------------------------------------------------
void SCREENNetmaskSet::InitDispWord(void)
{
try {

    for (int i=0; i<24; i++) {
         value[i] = 0;

         position[i].width=8;
         position[i].height=16;

         if (i%12==0)  position[i].X=64;
         else if (i%12==1) position[i].X=72;
         else if (i%12==2) position[i].X=80;

         else if (i%12==3) position[i].X=96;
         else if (i%12==4) position[i].X=104;
         else if (i%12==5) position[i].X=112;

         else if (i%12==6) position[i].X=128;
         else if (i%12==7) position[i].X=136;
         else if (i%12==8) position[i].X=144;

         else if (i%12==9) position[i].X=160;
         else if (i%12==10) position[i].X=168;
         else if (i%12==11) position[i].X=176;

         if (i>=0 && i<=11) position[i].Y=52;
         else if (i>=12 && i<=23) position[i].Y=72;

    }

  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENNetmaskSet::DisplayNetmask(void)
{
try {

    netmask1 = smem.GetNetmask(1);
    netmask2 = smem.GetNetmask(2);
    netmask3 = smem.GetNetmask(3);
    netmask4 = smem.GetNetmask(4);

    value[0]=netmask1/100;    value[1]=(netmask1%100)/10;    value[2]=netmask1%10;
    value[3]=netmask2/100;    value[4]=(netmask2%100)/10;    value[5]=netmask2%10;
    value[6]=netmask3/100;    value[7]=(netmask3%100)/10;    value[8]=netmask3%10;
    value[9]=netmask4/100;    value[10]=(netmask4%100)/10;   value[11]=netmask4%10;

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


  } catch (...) {}
}

//---------------------------------------------------------------------------
void SCREENNetmaskSet::DisplayGateway(void)
{
try {

    gateway1 = smem.GetGateway(1);
    gateway2 = smem.GetGateway(2);
    gateway3 = smem.GetGateway(3);
    gateway4 = smem.GetGateway(4);

    value[0+12]=gateway1/100;    value[1+12]=(gateway1%100)/10;    value[2+12]=gateway1%10;
    value[3+12]=gateway2/100;    value[4+12]=(gateway2%100)/10;    value[5+12]=gateway2%10;
    value[6+12]=gateway3/100;    value[7+12]=(gateway3%100)/10;    value[8+12]=gateway3%10;
    value[9+12]=gateway4/100;    value[10+12]=(gateway4%100)/10;   value[11+12]=gateway4%10;

    lcd240x128.DISPLAY_GRAPHIC_XY(position[0+12].X,position[0+12].Y,word8x16[value[0+12]],position[0+12].height,position[0+12].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(position[1+12].X,position[1+12].Y,word8x16[value[1+12]],position[1+12].height,position[1+12].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(position[2+12].X,position[2+12].Y,word8x16[value[2+12]],position[2+12].height,position[2+12].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(position[3+12].X,position[3+12].Y,word8x16[value[3+12]],position[3+12].height,position[3+12].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(position[4+12].X,position[4+12].Y,word8x16[value[4+12]],position[4+12].height,position[4+12].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(position[5+12].X,position[5+12].Y,word8x16[value[5+12]],position[5+12].height,position[5+12].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(position[6+12].X,position[6+12].Y,word8x16[value[6+12]],position[6+12].height,position[6+12].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(position[7+12].X,position[7+12].Y,word8x16[value[7+12]],position[7+12].height,position[7+12].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(position[8+12].X,position[8+12].Y,word8x16[value[8+12]],position[8+12].height,position[8+12].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(position[9+12].X,position[9+12].Y,word8x16[value[9+12]],position[9+12].height,position[9+12].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(position[10+12].X,position[10+12].Y,word8x16[value[10+12]],position[10+12].height,position[10+12].width/8);
    lcd240x128.DISPLAY_GRAPHIC_XY(position[11+12].X,position[11+12].Y,word8x16[value[11+12]],position[11+12].height,position[11+12].width/8);


  } catch (...) {}
}

//---------------------------------------------------------------------------
void SCREENNetmaskSet::DoKey0Work(void)
{
try {
    clearCursor8x16(position[cPosition].X,position[cPosition].Y+16);
    value[cPosition]=0;
    lcd240x128.DISPLAY_GRAPHIC_XY(position[cPosition].X,position[cPosition].Y,word8x16[value[cPosition]],position[cPosition].height,position[cPosition].width/8);
    DoKeyRIGHTWork();
    setCursor8x16(position[cPosition].X,position[cPosition].Y+16);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENNetmaskSet::DoKey1Work(void)
{
try {
    clearCursor8x16(position[cPosition].X,position[cPosition].Y+16);
    value[cPosition]=1;
    lcd240x128.DISPLAY_GRAPHIC_XY(position[cPosition].X,position[cPosition].Y,word8x16[value[cPosition]],position[cPosition].height,position[cPosition].width/8);
    DoKeyRIGHTWork();
    setCursor8x16(position[cPosition].X,position[cPosition].Y+16);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENNetmaskSet::DoKey2Work(void)
{
try {
    clearCursor8x16(position[cPosition].X,position[cPosition].Y+16);
    value[cPosition]=2;
    lcd240x128.DISPLAY_GRAPHIC_XY(position[cPosition].X,position[cPosition].Y,word8x16[value[cPosition]],position[cPosition].height,position[cPosition].width/8);
    DoKeyRIGHTWork();
    setCursor8x16(position[cPosition].X,position[cPosition].Y+16);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENNetmaskSet::DoKey3Work(void)
{
try {
    clearCursor8x16(position[cPosition].X,position[cPosition].Y+16);
    value[cPosition]=3;
    lcd240x128.DISPLAY_GRAPHIC_XY(position[cPosition].X,position[cPosition].Y,word8x16[value[cPosition]],position[cPosition].height,position[cPosition].width/8);
    DoKeyRIGHTWork();
    setCursor8x16(position[cPosition].X,position[cPosition].Y+16);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENNetmaskSet::DoKey4Work(void)
{
try {
    clearCursor8x16(position[cPosition].X,position[cPosition].Y+16);
    value[cPosition]=4;
    lcd240x128.DISPLAY_GRAPHIC_XY(position[cPosition].X,position[cPosition].Y,word8x16[value[cPosition]],position[cPosition].height,position[cPosition].width/8);
    DoKeyRIGHTWork();
    setCursor8x16(position[cPosition].X,position[cPosition].Y+16);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENNetmaskSet::DoKey5Work(void)
{
try {
    clearCursor8x16(position[cPosition].X,position[cPosition].Y+16);
    value[cPosition]=5;
    lcd240x128.DISPLAY_GRAPHIC_XY(position[cPosition].X,position[cPosition].Y,word8x16[value[cPosition]],position[cPosition].height,position[cPosition].width/8);
    DoKeyRIGHTWork();
    setCursor8x16(position[cPosition].X,position[cPosition].Y+16);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENNetmaskSet::DoKey6Work(void)
{
try {
    clearCursor8x16(position[cPosition].X,position[cPosition].Y+16);
    value[cPosition]=6;
    lcd240x128.DISPLAY_GRAPHIC_XY(position[cPosition].X,position[cPosition].Y,word8x16[value[cPosition]],position[cPosition].height,position[cPosition].width/8);
    DoKeyRIGHTWork();
    setCursor8x16(position[cPosition].X,position[cPosition].Y+16);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENNetmaskSet::DoKey7Work(void)
{
try {
    clearCursor8x16(position[cPosition].X,position[cPosition].Y+16);
    value[cPosition]=7;
    lcd240x128.DISPLAY_GRAPHIC_XY(position[cPosition].X,position[cPosition].Y,word8x16[value[cPosition]],position[cPosition].height,position[cPosition].width/8);
    DoKeyRIGHTWork();
    setCursor8x16(position[cPosition].X,position[cPosition].Y+16);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENNetmaskSet::DoKey8Work(void)
{
try {
    clearCursor8x16(position[cPosition].X,position[cPosition].Y+16);
    value[cPosition]=8;
    lcd240x128.DISPLAY_GRAPHIC_XY(position[cPosition].X,position[cPosition].Y,word8x16[value[cPosition]],position[cPosition].height,position[cPosition].width/8);
    DoKeyRIGHTWork();
    setCursor8x16(position[cPosition].X,position[cPosition].Y+16);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENNetmaskSet::DoKey9Work(void)
{
try {
    clearCursor8x16(position[cPosition].X,position[cPosition].Y+16);
    value[cPosition]=9;
    lcd240x128.DISPLAY_GRAPHIC_XY(position[cPosition].X,position[cPosition].Y,word8x16[value[cPosition]],position[cPosition].height,position[cPosition].width/8);
    DoKeyRIGHTWork();
    setCursor8x16(position[cPosition].X,position[cPosition].Y+16);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENNetmaskSet::DoKeyAWork(void)
{
}
//---------------------------------------------------------------------------
void SCREENNetmaskSet::DoKeyBWork(void)
{
}
//---------------------------------------------------------------------------
void SCREENNetmaskSet::DoKeyCWork(void)
{
}
//---------------------------------------------------------------------------
void SCREENNetmaskSet::DoKeyDWork(void)
{
}
//---------------------------------------------------------------------------
void SCREENNetmaskSet::DoKeyEWork(void)
{
}
//---------------------------------------------------------------------------
void SCREENNetmaskSet::DoKeyFWork(void)
{
}
//---------------------------------------------------------------------------
void SCREENNetmaskSet::DoKeyF1Work(void)
{
try {
    screenMain.DisplayMain();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENNetmaskSet::DoKeyF2Work(void)
{
}
//---------------------------------------------------------------------------
void SCREENNetmaskSet::DoKeyF3Work(void)
{
}
//---------------------------------------------------------------------------
void SCREENNetmaskSet::DoKeyF4Work(void)
{
try {
    screenCtlSetup.DisplayCtlSetup();
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENNetmaskSet::DoKeyEnterWork(void)
{
try {

    netmask1 =value[0]*100+value[1]*10+value[2];
    netmask2 =value[3]*100+value[4]*10+value[5];
    netmask3 =value[6]*100+value[7]*10+value[8];
    netmask4 =value[9]*100+value[10]*10+value[11];

    gateway1 =value[12]*100+value[13]*10+value[14];
    gateway2 =value[15]*100+value[16]*10+value[17];
    gateway3 =value[18]*100+value[19]*10+value[20];
    gateway4 =value[21]*100+value[22]*10+value[23];

    printf("netmask:%d.%d.%d.%d\n",netmask1, netmask2, netmask3, netmask4);
    printf("gatewak:%d.%d.%d.%d\n",gateway1, gateway2, gateway3, gateway4);


    if  ((netmask1>=0 && netmask1<=255) && (netmask2>=0 && netmask2<=255) &&
         (netmask3 == 0 || netmask3 == 128 || netmask3 == 192 || netmask3 == 224 || netmask3 == 240 || netmask3 == 248 || netmask3 == 252 || netmask3 == 255) &&
         (netmask4 == 0 || netmask4 == 128 || netmask4 == 192 || netmask4 == 224 || netmask4 == 240 || netmask4 == 248 || netmask4 == 252) &&
         (gateway1>=0 && gateway1<=255) && (gateway2>=0 && gateway2<=255) && (gateway3>=0 && gateway3<=255) && (gateway4>=0 && gateway4<=255)) {

          if ((netmask1==smem.GetNetmask(1)) && (netmask2==smem.GetNetmask(2)) && (netmask3==smem.GetNetmask(3)) && (netmask4==smem.GetNetmask(4)) &&
              (gateway1==smem.GetGateway(1)) && (gateway2==smem.GetGateway(2)) && (gateway3==smem.GetGateway(3)) && (gateway4==smem.GetGateway(4)) &&
              (localIp1_1==smem.GetLocalIP1(1)) && (localIp1_2==smem.GetLocalIP1(2)) && (localIp1_3==smem.GetLocalIP1(3)) && (localIp1_4==smem.GetLocalIP1(4)) && (localPort1==smem.GetLocalIP1(5)) &&
              (distIp0_1==smem.GetdistIp0(1)) && (distIp0_2==smem.GetdistIp0(2)) && (distIp0_3==smem.GetdistIp0(3)) && (distIp0_4==smem.GetdistIp0(4)) && (dist0Port==smem.GetdistIp0(5)) &&
              (distIp1==smem.GetDistIP(1)) && (distIp2==smem.GetDistIP(2)) && (distIp3==smem.GetDistIP(3)) && (distIp4==smem.GetDistIP(4)) && (distPort==smem.GetDistIP(5))
          ) {
               printf("It is the same!!\n");

          } else {
            screenNetworkSetConfirm.DisplayNetworkSetConfirm(localIp1_1,localIp1_2,localIp1_3,localIp1_4,localPort1,
                                                                   distIp0_1,distIp0_2,distIp0_3,distIp0_4,dist0Port,
                                                                   distIp1,distIp2,distIp3,distIp4,distPort,
                                                                   netmask1, netmask2, netmask3, netmask4,
                                                                   gateway1, gateway2, gateway3, gateway4
                                                                   );
          }

    } else {

          netmask1=smem.GetNetmask(1);
          netmask2=smem.GetNetmask(2);
          netmask3=smem.GetNetmask(3);
          netmask4=smem.GetNetmask(4);

          gateway1=smem.GetGateway(1);
          gateway2=smem.GetGateway(2);
          gateway3=smem.GetGateway(3);
          gateway4=smem.GetGateway(4);

          printf("netmask:%d.%d.%d.%d\n",netmask1, netmask2, netmask3, netmask4);
          printf("gatewak:%d.%d.%d.%d\n",gateway1, gateway2, gateway3, gateway4);

          DisplayNetmask();
          DisplayGateway();
    }


  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENNetmaskSet::DoKeyUPWork(void)
{
try {
    clearCursor8x16(position[cPosition].X,position[cPosition].Y+16);
    if (cPosition>=12) cPosition-=12;
    setCursor8x16(position[cPosition].X,position[cPosition].Y+16);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENNetmaskSet::DoKeyDOWNWork(void)
{
try {
    clearCursor8x16(position[cPosition].X,position[cPosition].Y+16);
    if (cPosition<=11) cPosition+=12;
    setCursor8x16(position[cPosition].X,position[cPosition].Y+16);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENNetmaskSet::DoKeyLEFTWork(void)
{
try {
    clearCursor8x16(position[cPosition].X,position[cPosition].Y+16);
    if (cPosition==0) cPosition=1;
    cPosition--;
    setCursor8x16(position[cPosition].X,position[cPosition].Y+16);
  } catch (...) {}
}
//---------------------------------------------------------------------------
void SCREENNetmaskSet::DoKeyRIGHTWork(void)
{
try {
    clearCursor8x16(position[cPosition].X,position[cPosition].Y+16);
    cPosition++;
    if (cPosition>=24) cPosition=23;
    setCursor8x16(position[cPosition].X,position[cPosition].Y+16);

  } catch (...) {}
}
//---------------------------------------------------------------------------
