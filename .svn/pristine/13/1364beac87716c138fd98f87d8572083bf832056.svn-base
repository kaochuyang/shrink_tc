//---------------------------------------------------------------------------
#include "PDEVICESS.h"
#include "CHexToInt.h"
#include "CDataToMessageOK.h"
#include "SMEM.h"

#include "WRITEJOB.h"

#include <string.h>
//---------------------------------------------------------------------------
PDEVICESS::PDEVICESS(void)
{
//    bzero(&_SSInfo,sizeof(_SSInfo));
//    smem.vSetSS_S0Status(true);                                                   //預設是狀態OK
//    smem.vSetSS_SKStatus(true);                                                   //預設是狀態OK
}
//---------------------------------------------------------------------------
PDEVICESS::~PDEVICESS(void)
{
}
//---------------------------------------------------------------------------
bool PDEVICESS::DoWorkViaPDevice(MESSAGEOK message)
{
try {

/*
    if(message.packet[0] == 'X' && message.packet[1] == 'D' &&  message.InnerOrOutWard == cInner)
            vDoProtocol_XD(message);
    else if(message.packet[0] == 'X' && message.packet[1] == 'A' &&  message.InnerOrOutWard == cInner)
            vDoProtocol_XA(message);
    else if(message.packet[0] == 'X' && message.packet[1] == '1' &&  message.InnerOrOutWard == cInner)
            vDoProtocol_X1(message);
    else if(message.packet[0] == 'S' && message.packet[1] == '0' &&  message.InnerOrOutWard == cInner)
            vWriteSmartSensor_S0_ToShareMem(message);
    else if(message.packet[0] == 'S' && message.packet[1] == 'K' &&
            message.packet[2] == 'S' && message.packet[3] == 'u' &&
            message.packet[4] == 'c' && message.packet[5] == 'c' &&  message.InnerOrOutWard == cInner)
            vDoReceiveSKSuccess(message);

//OTSS++
    else if(message.packet[0] == 'X' && message.packet[1] == 'G' &&  message.InnerOrOutWard == cInner)
            vXGTransToRTMS0x18(message);
//OTSS--

//Physical Out
    else if(message.packet[0] == 'S' && message.packet[1] == '4' && message.InnerOrOutWard == cOutWard)
            writeJob.WritePhysicalOut(message.packet,message.packetLength,DEVICESS);
    else if(message.packet[0] == 'S' && message.packet[1] == 'K' &&
            message.packet[2] == 'S' && message.packet[7] == '8' &&
            message.packet[8] == 'E' && message.packet[12] == '8' &&
            message.InnerOrOutWard == cOutWard)
            writeJob.WritePhysicalOut(message.packet,message.packetLength,DEVICESS);
    else if(message.packet[0] == 'S' && message.packet[1] == 'K' &&
            message.packet[2] == 'S' && message.packet[7] == '9' &&
            message.packet[8] == '7' && message.packet[12] == '4' &&
            message.InnerOrOutWard == cOutWard)
            writeJob.WritePhysicalOut(message.packet,message.packetLength,DEVICESS);
    else if(message.packet[0] == 'S' && message.packet[1] == 'J' &&               //查車長
            message.packet[2] == 'S' && message.packet[3] == '0' &&
            message.InnerOrOutWard == cOutWard)
            writeJob.WritePhysicalOut(message.packet,message.packetLength,DEVICESS);
    else if(message.packet[0] == 'S' && message.packet[1] == '0' &&               //查BasicInfo
            message.packet[2] == 0x0D && message.InnerOrOutWard == cOutWard)
            writeJob.WritePhysicalOut(message.packet,message.packetLength,DEVICESS);

*/
    return true;

  } catch (...) {}
}
//---------------------------------------------------------------------------
//sim------------------------------------------------------------------------
/*
bool PDEVICESS::vWriteSmartSensor_S0_ToShareMem(MESSAGEOK DataMessageIn)
{
try{
  return true;
  } catch(...) {}
}
//---------------------------------------------------------------------------
bool PDEVICESS::vDoProtocol_XD(MESSAGEOK DataMessageIn)
{
try{
    return true;
  }catch(...) {}
}
//---------------------------------------------------------------------------
bool PDEVICESS::vDoProtocol_XA(MESSAGEOK DataMessageIn)
{
try{
    return true;
  } catch(...) {}
}
//---------------------------------------------------------------------------
bool PDEVICESS::vDoProtocol_X1(MESSAGEOK DataMessageIn)
{
try{
    return true;
  } catch(...) {}
}
//---------------------------------------------------------------------------
bool PDEVICESS::vDoReceiveSKSuccess(MESSAGEOK DataMessageIn)
{
try {
    return true;
  }  catch(...) {}
}
//---------------------------------------------------------------------------

//OTSS++
bool PDEVICESS::vXGTransToRTMS0x18(MESSAGEOK DataMessageIn)
{
try {
  unsigned char ucTmp[16];
  char c2Tmp[2];
  int iTmp;
  if (DataMessageIn.packet[0] == 'X' && DataMessageIn.packet[1] == 'G' && DataMessageIn.packetLength >= 10 )
  {
      bzero(ucTmp, 16);

      ucTmp[0] = 0xFF;
      ucTmp[1] = 0x18;
      ucTmp[2] = 0x04;
      ucTmp[7] = 0x0;

      for(int j = 0; j < 4; j++) {
        c2Tmp[0] = DataMessageIn.packet[2 + 2*j];
        c2Tmp[1] = DataMessageIn.packet[3 + 2*j];
        sscanf(c2Tmp, "%X", &iTmp);
        ucTmp[j+3] = iTmp;
        ucTmp[7] += ucTmp[j+3];                                                 // cal. checkSUM
      }

      //then save to ??
      printf("\n");
      for(int i = 0; i < 8; i++) {
        printf(" %X", ucTmp[i]);
      }
      printf("\n");
  }

}  catch(...) {}
}
*/
//OTSS--
