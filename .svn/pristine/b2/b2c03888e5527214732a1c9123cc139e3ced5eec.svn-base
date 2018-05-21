//---------------------------------------------------------------------------
#include "PTRAFFIC.h"

//---------------------------------------------------------------------------
PTRAFFIC::PTRAFFIC(void)
{
}
//---------------------------------------------------------------------------
PTRAFFIC::~PTRAFFIC(void)
{
}
//---------------------------------------------------------------------------
bool PTRAFFIC::DoWorkViaProtocol(MESSAGEOK message)
{
try {
    switch (message.protocol) {
            //第一種分類:交通部77年版通訊協定
            case PROTOCOL77:                                                    //交通部77年版通訊協定
                 pTraffic77.DoWorkViaPTraffic(message);
            break;
            //第二種分類:交通部87年版通訊協定
            case PROTOCOL87:                                                    //交通部87年版通訊協定
                 pTraffic87.DoWorkViaPTraffic(message);
            break;
            //第三種分類:交通部92年版通訊協定
            case PROTOCOL92:                                                    //交通部92年版通訊協定
                 pTraffic92.DoWorkViaPTraffic(message);
            break;
            //第四種分類:交通部Ntcip年版通訊協定
            case PROTOCOLNTCIP:                                                 //交通部NTCIP通訊協定
                 pTrafficNtcip.DoWorkViaPTraffic(message);
            break;

            case PROTOCOLCCJ:                                                 //交通部NTCIP通訊協定
                 pTrafficCCJ.DoWorkViaPTraffic(message);
            break;

            //OT20140224
            case PROTOCOLBRT:
                 pTrafficBRT.DoWorkViaPTraffic92(message);
            break;

            //jacky20140226
            case PROTOCOLCOUNTDOWN:
                pTrafficCOUNTDOWN.DoWorkViaPTraffic92(message);
            break;

            case PROTOCOLREVSYNC:
                 pTrafficRevSync.DoWorkViaPTraffic(message);
            break;

            case PROTOCOLAMEGIDS:
                 pTrafficAmegids.DoWorkViaPTraffic(message);
            break;
            case PROTOCOLARWENVD:
                 pTraffic92Vd.DoWorkViaPTraffic92(message);
            break;
            case PROTOCOLWEBSET:
                 pTraffic92Web.DoWorkViaPTraffic92(message);
            break;

            case PROTOCOLKEYPAD:                                                //東生面板控制鍵盤通訊協定
            case PROTOCOLRTMS:                                                  //RTMS通訊協定
            case PROTOCOLSS:                                                    //SMART SENSOR通訊協定
            case PROTOCOLOWNWAY:                                                //宏楷紅燈倒數通訊協定
            case PROTOCOLEOI:                                                   //恆嘉紅燈倒數通訊協定
            case PROTOCOLLISCMS:                                                //台北永琦CMS通訊協定
            case PROTOCOLUNKNOW:                                                //不清楚的協定
            break;

            default:
            break;
    }

    return true;

  } catch (...) {}
}
//---------------------------------------------------------------------------
