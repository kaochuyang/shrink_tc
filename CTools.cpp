#include <stdio.h>
#include "CTools.h"

CTools oTools;

//--------------------------------------------------------------------------
unsigned short int CTools::vVer30LightToCCTLight(unsigned char ucIN)
{
try {
  unionByte unionByteIN;
  unionByteIN.ucByte = ucIN;
  unsigned short int usiOut = 0x0000;

  if(unionByteIN.block.b1 == true)  usiOut+=0x0003;                                   //Red
  if(unionByteIN.block.b2 == true)  usiOut+=0x000C;                                   //Yellow
  if(unionByteIN.block.b3 == true)  usiOut+=0x00C0;                                   //Green
  if(unionByteIN.block.b4 == true)  usiOut+=0x0030;                                   //Left Green
  if(unionByteIN.block.b5 == true)  usiOut+=0x0300;                                   //Up Green
  if(unionByteIN.block.b6 == true)  usiOut+=0x0C00;                                   //Right Green

  if     (unionByteIN.block.b7 == true && unionByteIN.block.b8 == true) usiOut+=0x8000;     //People Green flash
  else if(unionByteIN.block.b7 == true && unionByteIN.block.b8 == false) usiOut+=0xC000;    //Poeple Green
  else if(unionByteIN.block.b7 ==false && unionByteIN.block.b8 == true) usiOut+=0x3000;     //People Red

  return usiOut;
}
catch(...){}
}

//--------------------------------------------------------------------------
unsigned char CTools::vCCTLightToVer30Light(const unsigned short int usiIN)
{
try {
  unionByte unionByteOut;
  unionByteOut.ucByte = 0;
  short int usiINShift;
  usiINShift = usiIN;

  int iSignalStatus[16];
  for(int i = 0; i < 16; i++) iSignalStatus[i] = 0;

  for(int i = 0; i < 16; i++) {
    iSignalStatus[i] = ((usiINShift&0x8000)==0) ? 0 : 1;  // 0x80 => 10000000 (1byte)
    usiINShift = usiINShift << 1;
  }

  //<<<CCJ_modify
  if(iSignalStatus[0] == 1 && iSignalStatus[1] == 1)    //Poeple Green
    unionByteOut.block.b7 = true;
  if(iSignalStatus[2] == 1 && iSignalStatus[3] == 1)    //Poeple Red
    unionByteOut.block.b8 = true;
  if(iSignalStatus[4] == 1 && iSignalStatus[5] == 1)    //Right Green
    unionByteOut.block.b6 = true;
  if(iSignalStatus[6] == 1 && iSignalStatus[7] == 1)    //Up Green
    unionByteOut.block.b5 = true;
  if(iSignalStatus[8] == 1 && iSignalStatus[9] == 1)    //Cycle Green
    unionByteOut.block.b3 = true;
  if(iSignalStatus[10] == 1 && iSignalStatus[11] == 1)  //Left Green
    unionByteOut.block.b4 = true;
  if(iSignalStatus[12] == 1 && iSignalStatus[13] == 1)  //Yellow
    unionByteOut.block.b2 = true;
  if(iSignalStatus[14] == 1 && iSignalStatus[15] == 1)  //Red
    unionByteOut.block.b1 = true;

  if(iSignalStatus[0] == 1 && iSignalStatus[1] == 0) //Poeple Green Flash
  {
    unionByteOut.block.b7 = true;
    unionByteOut.block.b8 = true;
  }

  /*
  if(iSignalStatus[0] == 1 && iSignalStatus[1] == 1) //Poeple Green
    unionByteOut.block.b7 = true;
  if(iSignalStatus[2] == 1 && iSignalStatus[3] == 1) //Poeple Red
    unionByteOut.block.b8 = true;
  if(iSignalStatus[4] == 1 && iSignalStatus[5] == 1) //Right Green
    unionByteOut.block.b6 = true;
  if(iSignalStatus[6] == 1 && iSignalStatus[7] == 1) //Up Green
    unionByteOut.block.b5 = true;
  if(iSignalStatus[8] == 1 && iSignalStatus[9] == 1) //Cycle Green
    unionByteOut.block.b3 = true;


  if(iSignalStatus[0] == 1 && iSignalStatus[1] == 0) //Poeple Green Flash
  {
    unionByteOut.block.b7 = true;
    unionByteOut.block.b8 = true;
  }
  */
  //CCJ_modify>>>

  return unionByteOut.ucByte;
  }
  catch(...){}
}

//--------------------------------------------------------------------------
int CTools::vCCTLightToVer30SignalMapLight_5F0F(const int iSelect, const unsigned short int usiIN)
{
try {

  int iRet;
  int usiINShift = usiIN;

  int iSignalStatus[16];
  for(int i = 0; i < 16; i++) iSignalStatus[i] = 0;

  for(int i = 0; i < 16; i++) {
    iSignalStatus[i] = ((usiINShift&0x8000)==0) ? 0 : 1;  // 0x80 => 10000000 (1byte)
    usiINShift = usiINShift << 1;
  }

//====================================By 92 Protocal 5F0F page5-89, Status 1
  if(iSignalStatus[4]  == 1 && iSignalStatus[5]  == 1 && //Right Green
     iSignalStatus[6]  == 0 && iSignalStatus[7]  == 0 && //Up Green
     iSignalStatus[8]  == 0 && iSignalStatus[9]  == 0 && //Cycle Green
     iSignalStatus[10] == 0 && iSignalStatus[11] == 0 && //Left Green
     iSignalStatus[12] == 0 && iSignalStatus[13] == 0 && //Yellow
     iSignalStatus[14] == 0 && iSignalStatus[15] == 0  ) //Red
   {
     if(iSelect == 0) { //ask GreenSignalMap
       iRet = 1;
     } else if(iSelect == 1) {  //ask YellowSignalMap
       iRet = 1;
     } else if(iSelect == 2) {  //ask RedSignalMap
       iRet = 0;
     }
     return iRet;
   }
//====================================By 92 Protocal 5F0F page5-89, Status 2
   if(iSignalStatus[4]  == 0 && iSignalStatus[5]  == 0 && //Right Green
      iSignalStatus[6]  == 0 && iSignalStatus[7]  == 0 && //Up Green
      iSignalStatus[8]  == 0 && iSignalStatus[9]  == 0 && //Cycle Green
      iSignalStatus[10] == 1 && iSignalStatus[11] == 1 && //Left Green
      iSignalStatus[12] == 0 && iSignalStatus[13] == 0 && //Yellow
      iSignalStatus[14] == 0 && iSignalStatus[15] == 0  ) //Red
    {
      if(iSelect == 0) { //ask GreenSignalMap
        iRet = 1;
      } else if(iSelect == 1) {  //ask YellowSignalMap
        iRet = 0;
      } else if(iSelect == 2) {  //ask RedSignalMap
        iRet = 1;
      }
      return iRet;
    }

//====================================By 92 Protocal 5F0F page5-89, Status 3
  if(iSignalStatus[4]  == 1 && iSignalStatus[5]  == 1 && //Right Green
     iSignalStatus[6]  == 0 && iSignalStatus[7]  == 0 && //Up Green
     iSignalStatus[8]  == 0 && iSignalStatus[9]  == 0 && //Cycle Green
     iSignalStatus[10] == 1 && iSignalStatus[11] == 1 && //Left Green
     iSignalStatus[12] == 0 && iSignalStatus[13] == 0 && //Yellow
     iSignalStatus[14] == 0 && iSignalStatus[15] == 0  ) //Red
   {
     if(iSelect == 0) { //ask GreenSignalMap
       iRet = 1;
     } else if(iSelect == 1) {  //ask YellowSignalMap
       iRet = 1;
     } else if(iSelect == 2) {  //ask RedSignalMap
       iRet = 1;
     }
     return iRet;
   }


//====================================By 92 Protocal 5F0F page5-89, Status 4
if(iSignalStatus[4]  == 0 && iSignalStatus[5]  == 0 && //Right Green
   iSignalStatus[6]  == 1 && iSignalStatus[7]  == 1 && //Up Green
   iSignalStatus[8]  == 0 && iSignalStatus[9]  == 0 && //Cycle Green
   iSignalStatus[10] == 0 && iSignalStatus[11] == 0 && //Left Green
   iSignalStatus[12] == 0 && iSignalStatus[13] == 0 && //Yellow
   iSignalStatus[14] == 0 && iSignalStatus[15] == 0  ) //Red
   {
     if(iSelect == 0) { //ask GreenSignalMap
       iRet = 0;
     } else if(iSelect == 1) {  //ask YellowSignalMap
       iRet = 0;
     } else if(iSelect == 2) {  //ask RedSignalMap
       iRet = 0;
     }
     return iRet;
   }

//====================================By 92 Protocal 5F0F page5-89, Status 5
   if( ((iSignalStatus[4]  == 1 && iSignalStatus[5]  == 1  &&   //Right Green
         iSignalStatus[6]  == 1 && iSignalStatus[7]  == 1) ||   //Up Green
        (iSignalStatus[10]  == 1 && iSignalStatus[11]  == 1 &&  //Left Green
         iSignalStatus[6]  == 1 && iSignalStatus[7]  == 1))     //Up Green
     &&
     iSignalStatus[8]  == 0 && iSignalStatus[9]  == 0 && //Cycle Green
     iSignalStatus[12] == 0 && iSignalStatus[13] == 0 && //Yellow
     iSignalStatus[14] == 0 && iSignalStatus[15] == 0  ) //Red
   {
     if(iSelect == 0) { //ask GreenSignalMap
       iRet = 1;
     } else if(iSelect == 1) {  //ask YellowSignalMap
       iRet = 1;
     } else if(iSelect == 2) {  //ask RedSignalMap
       iRet = 1;
     }
     return iRet;
   }

//====================================
   if(iSignalStatus[8] == 1 && iSignalStatus[9] == 1) //Cycle Green
   {
     if(iSelect == 0) { //ask GreenSignalMap
       iRet = 1;
     } else if(iSelect == 1) {  //ask YellowSignalMap
       iRet = 0;
     } else if(iSelect == 2) {  //ask RedSignalMap
       iRet = 0;
     }
     return iRet;
   }
//====================================
  if(iSignalStatus[12] == 1 && iSignalStatus[13] == 1) //Yellow
  {
    if(iSelect == 0) { //ask GreenSignalMap
      iRet = 0;
    } else if(iSelect == 1) {  //ask YellowSignalMap
      iRet = 1;
    } else if(iSelect == 2) {  //ask RedSignalMap
      iRet = 0;
    }
    return iRet;
  }
//====================================
  if(iSignalStatus[14] == 1 && iSignalStatus[15] == 1) //Red
  {
    if(iSelect == 0) { //ask GreenSignalMap
      iRet = 0;
    } else if(iSelect == 1) {  //ask YellowSignalMap
      iRet = 0;
    } else if(iSelect == 2) {  //ask RedSignalMap
      iRet = 1;
    }
    return iRet;
  }
  iRet = 0;
  return iRet;

  }
catch(...){}
}


//--------------------------------------------------------------------------
unsigned char CTools::vGuessLightStep(const int iStep, unsigned char ucIn)
{
try {
  unionByte ucOutLight;
  ucOutLight.ucByte = ucIn;

  switch(iStep) {
    case(2):
      if(ucOutLight.block.b7 == true && ucOutLight.block.b8 == false)             //Poeple Green to People Green Flash
        { ucOutLight.block.b7 = true; ucOutLight.block.b8 = true; }
      break;
    case(3):
      if(ucOutLight.block.b7 == true && ucOutLight.block.b8 == true)              //Poeple Green to People Red
        { ucOutLight.block.b7 = false; ucOutLight.block.b8 = true; }
      break;
    case(4):
      if(ucOutLight.block.b3 == true)                                            //Green to yellow
        { ucOutLight.block.b3 = false; ucOutLight.block.b2 = true; }
      if(ucOutLight.block.b4 == true)                                            //LGreen to yellow
        { ucOutLight.block.b4 = false; ucOutLight.block.b2 = true; }
      if(ucOutLight.block.b5 == true)                                            //UGreen to yellow
        { ucOutLight.block.b5 = false; ucOutLight.block.b2 = true; }
      if(ucOutLight.block.b6 == true)                                            //RGreen to yellow
        { ucOutLight.block.b6 = false; ucOutLight.block.b2 = true; }
      break;
    case(5):
      if(ucOutLight.block.b2 == true)                                            //Yellow to Red
        { ucOutLight.block.b1 = true; ucOutLight.block.b2 = false; }
      break;

    default:
      printf("Error.\n");
      break;
  }
  return ucOutLight.ucByte;
}
catch(...){}
}

