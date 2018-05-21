#include "CHexToInt.h"
#include <ctype.h>
#include <stdio.h>
#include "var.h"

CHexToInt oHexToInt;
//--------------------------------------------------------------------
long CHexToInt::htoi(char *tmpCheckSumASCII, int po)
{
try {
    int i, j;
    long hi = 0;
    j = po;

    for(i = 0; i < po; i++) {
        j = po - i - 1;
        hi = hchartoi(tmpCheckSumASCII[i], j) + hi;
    }
    return hi;
  } catch (...) {}
}
//---------------------------------------------------------------------
long CHexToInt::hchartoi (char hexdig, int pos)
{
try {
    char hexdigits[] = "0123456789ABCDEF";
    char *p = &hexdigits[0];
    long deci = 0;
    int i;

    while (*p != toupper(hexdig) && deci < 16) {
           ++p;
           ++deci;
    }

    if (*p == toupper(hexdig)) {
        for (i = 0; i < pos; i++)
             deci *= 16;
        return deci;
    }

    return -1;
  } catch (...) {}
}
//----------------------------------------------------------------------
char CHexToInt::vIntToFuckingBCD(int iSW, int iInput)
{
try{

  char cTMP[7] = "000000";
  sprintf(cTMP,"%6d", iInput);

  unsigned char ucNo2;
  unsigned char ucNo1;

  if(iSW==0) {
    ucNo2 = cTMP[4]; ucNo1 = cTMP[5];
  }
  else if(iSW==1) {
    ucNo2 = cTMP[2]; ucNo1 = cTMP[3];
  }
  else if(iSW==2) {
    ucNo2 = cTMP[0]; ucNo1 = cTMP[1];
  }
  else {
    return false;
  }

  DATA_Bit _DATA;
  _DATA.DBit = 0;

  if(ucNo1 == '1') {
    _DATA.switchBit.b1 = 1; _DATA.switchBit.b2 = 0;
    _DATA.switchBit.b3 = 0; _DATA.switchBit.b4 = 0;
  }
  else if(ucNo1 == '2') {
    _DATA.switchBit.b1 = 0; _DATA.switchBit.b2 = 1;
    _DATA.switchBit.b3 = 0; _DATA.switchBit.b4 = 0;
  }
  else if(ucNo1 == '3') {
    _DATA.switchBit.b1 = 1; _DATA.switchBit.b2 = 1;
    _DATA.switchBit.b3 = 0; _DATA.switchBit.b4 = 0;
  }
  else if(ucNo1 == '4') {
    _DATA.switchBit.b1 = 0; _DATA.switchBit.b2 = 0;
    _DATA.switchBit.b3 = 1; _DATA.switchBit.b4 = 0;
  }
  else if(ucNo1 == '5') {
    _DATA.switchBit.b1 = 1; _DATA.switchBit.b2 = 0;
    _DATA.switchBit.b3 = 1; _DATA.switchBit.b4 = 0;
  }
  else if(ucNo1 == '6') {
    _DATA.switchBit.b1 = 0; _DATA.switchBit.b2 = 1;
    _DATA.switchBit.b3 = 1; _DATA.switchBit.b4 = 0;
  }
  else if(ucNo1 == '7') {
    _DATA.switchBit.b1 = 1; _DATA.switchBit.b2 = 1;
    _DATA.switchBit.b3 = 1; _DATA.switchBit.b4 = 0;
  }
  else if(ucNo1 == '8') {
    _DATA.switchBit.b1 = 0; _DATA.switchBit.b2 = 0;
    _DATA.switchBit.b3 = 0; _DATA.switchBit.b4 = 1;
  }
  else if(ucNo1 == '9') {
    _DATA.switchBit.b1 = 1; _DATA.switchBit.b2 = 0;
    _DATA.switchBit.b3 = 0; _DATA.switchBit.b4 = 1;
  }
  else {
    _DATA.switchBit.b1 = 0; _DATA.switchBit.b2 = 0;
    _DATA.switchBit.b3 = 0; _DATA.switchBit.b4 = 0;
  }

  if(ucNo2 == '1') {
    _DATA.switchBit.b5 = 1; _DATA.switchBit.b6 = 0;
    _DATA.switchBit.b7 = 0; _DATA.switchBit.b8 = 0;
  }
  else if(ucNo2 == '2') {
    _DATA.switchBit.b5 = 0; _DATA.switchBit.b6 = 1;
    _DATA.switchBit.b7 = 0; _DATA.switchBit.b8 = 0;
  }
  else if(ucNo2 == '3') {
    _DATA.switchBit.b5 = 1; _DATA.switchBit.b6 = 1;
    _DATA.switchBit.b7 = 0; _DATA.switchBit.b8 = 0;
  }
  else if(ucNo2 == '4') {
    _DATA.switchBit.b5 = 0; _DATA.switchBit.b6 = 0;
    _DATA.switchBit.b7 = 1; _DATA.switchBit.b8 = 0;
  }
  else if(ucNo2 == '5') {
    _DATA.switchBit.b5 = 1; _DATA.switchBit.b6 = 0;
    _DATA.switchBit.b7 = 1; _DATA.switchBit.b8 = 0;
  }
  else if(ucNo2 == '6') {
    _DATA.switchBit.b5 = 0; _DATA.switchBit.b6 = 1;
    _DATA.switchBit.b7 = 1; _DATA.switchBit.b8 = 0;
  }
  else if(ucNo2 == '7') {
    _DATA.switchBit.b5 = 1; _DATA.switchBit.b6 = 1;
    _DATA.switchBit.b7 = 1; _DATA.switchBit.b8 = 0;
  }
  else if(ucNo2 == '8') {
    _DATA.switchBit.b5 = 0; _DATA.switchBit.b6 = 0;
    _DATA.switchBit.b7 = 0; _DATA.switchBit.b8 = 1;
  }
  else if(ucNo2 == '9') {
    _DATA.switchBit.b5 = 1; _DATA.switchBit.b6 = 0;
    _DATA.switchBit.b7 = 0; _DATA.switchBit.b8 = 1;
  }
  else {
    _DATA.switchBit.b5 = 0; _DATA.switchBit.b6 = 0;
    _DATA.switchBit.b7 = 0; _DATA.switchBit.b8 = 0;
  }
  return _DATA.DBit;

}catch(...){}

}
//---------------------------------------------------------------------
