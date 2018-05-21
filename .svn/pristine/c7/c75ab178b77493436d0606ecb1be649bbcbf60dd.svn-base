//---------------------------------------------------------------------------
#include "PROTOCOL.h"

PROTOCOL protocol;
//---------------------------------------------------------------------------
PROTOCOL::PROTOCOL(void)
{
}
//---------------------------------------------------------------------------
PROTOCOL::~PROTOCOL(void)
{
}
//---------------------------------------------------------------------------
bool PROTOCOL::DoWork(MESSAGEOK message)
{
try {
    switch (message.protocol) {
            //第一種分類:交通部通訊協定
            case PROTOCOL77:                                                    //交通部77年版通訊協定
            case PROTOCOL87:                                                    //交通部87年版通訊協定
            case PROTOCOL92:                                                    //交通部92年版通訊協定
            case PROTOCOLNTCIP:                                                 //交通部NTCIP通訊協定
            case PROTOCOLCCJ:
            //OT20140224
            case PROTOCOLBRT:
            //jacky20140426
            case PROTOCOLCOUNTDOWN:
            case PROTOCOLREVSYNC:
            case PROTOCOLAMEGIDS:
            case PROTOCOLARWENVD://arwen ++
            case PROTOCOLWEBSET://arwen ++
                 pTraffic.DoWorkViaProtocol(message);
            break;
            //第二種分類:週邊設備
            case PROTOCOLKEYPAD:                                                //東生面板控制鍵盤通訊協定
            case PROTOCOLRTMS:                                                  //RTMS通訊協定
            case PROTOCOLSS:                                                    //SMART SENSOR通訊協定
            case PROTOCOLOWNWAY:                                                //宏楷紅燈倒數通訊協定
            case PROTOCOLEOI:                                                   //恆嘉紅燈倒數通訊協定
            case PROTOCOLLISCMS:                                                //台北永琦CMS通訊協定
            case PROTOCOLTRAFFICLIGHT:
            case PROTOCOLREVMANUALPAD:
            case PROTOCOLPEDESTRIAN:
                 pDevice.DoWorkViaProtocol(message);
            break;

            case PROTOCOLUNKNOW:                                                //不清楚的協定
            break;

            default:
            break;
    }

    return true;

  } catch (...) {}
}
//---------------------------------------------------------------------------
