//---------------------------------------------------------------------------
#include "PTRAFFIC92.h"
#include "WRITEJOB.h"
#include "SMEM.h"

//---------------------------------------------------------------------------
PTRAFFIC92::PTRAFFIC92(void)
{
}
//---------------------------------------------------------------------------
PTRAFFIC92::~PTRAFFIC92(void)
{
}
//---------------------------------------------------------------------------
bool PTRAFFIC92::DoWorkViaPTraffic(MESSAGEOK message)
{
try {
  //OT Pass
  unsigned char ucACKToWho = smem.vGetACK_W2W(message.packet[2]);
  char msg[128] = { 0 };

if (message.packet[0]==0xAA) {
    if (message.packet[1]==0xBB) {

      if (message.ReadorWrite==cREAD ) {
        if(smem.vGetPassMode() == cTermPassMode)
        {
          if( message.packet[7] == 0x0F && message.packet[8] == 0x8E ) {
            writeJob.WriteACK92(message.packet[2]);
          }
        } else {                                                              //normal mode
            writeJob.WriteACK92(message.packet[2]);
        }
      }
      sprintf(msg,"GetProtocol%02X%02X", message.packet[7], message.packet[8]);
      smem.vWriteMsgToDOM(msg);

            switch (message.packet[7]) {
                    case 0x0F:                                                          //共用訊息
                         pTraffic92Comm.DoWorkViaPTraffic92(message);
                    break;
                    case 0x5F:                                                          //號誌控制器
                         pTraffic92Tc.DoWorkViaPTraffic92(message);
                    break;
                    case 0x6F:                                                          //車輛偵測器
                         pTraffic92Vd.DoWorkViaPTraffic92(message);
                    break;
                    case 0xAF:                                                          //資訊可變控制器
                        pTraffic92Cms.DoWorkViaPTraffic92(message);
                    break;
                    //OT20140224
                    case 0xBF:
                        pTraffic92BRT.DoWorkViaPTraffic92(message);
                    break;
                    //jacky20140426
                    case 0x50:
                        pTraffic92COUNTDOWN.DoWorkViaPTraffic92(message);
                    break;
                    default:
                        pTraffic92Other.DoWorkViaPTraffic92(message);
                    break;
            }
        } else if (message.packet[1]==0xDD) {
            if (message.ReadorWrite==cREAD) {
              smem.ClearWaitMsg(message.packet[2]);
/*
              //OT Pass
              switch (ucACKToWho) {
                case cCenterToTC:
                  smem.ClearWaitMsg(message.packet[2]);
                break;
                case cCenterToVD01:
                  smem.SSVD01Socket.UdpSend(message.packet, message.packetLength,"SSVD01-ForwardACK");
                  smem.vSetACK_W2W(message.packet[2], cTCToCenter);
                break;
                case cCenterToVD02:
                  smem.SSVD02Socket.UdpSend(message.packet, message.packetLength,"SSVD02-ForwardACK");
                  smem.vSetACK_W2W(message.packet[2], cTCToCenter);

                break;
                case cCenterToVD03:
                  smem.SSVD03Socket.UdpSend(message.packet, message.packetLength,"SSVD03-ForwardACK");
                  smem.vSetACK_W2W(message.packet[2], cTCToCenter);
                break;
                case cCenterToVD04:
                  smem.SSVD04Socket.UdpSend(message.packet, message.packetLength,"SSVD04-ForwardACK");
                  smem.vSetACK_W2W(message.packet[2], cTCToCenter);
                break;

                default:
                break;
              }
*/
            } else if (message.ReadorWrite==cWRITE) {
                     writeJob.WritePhysicalOut(message.packet,message.packetLength,DEVICECENTER92);
            }
        } else if (message.packet[1]==0xEE) {
            smem.ClearWaitMsg(message.packet[2]);
        }
    }

    return true;

  } catch (...) {}
}
//---------------------------------------------------------------------------

