#include "shirink_app.h"
#include "SMEM.h"
shirink_app::shirink_app()
{
    //ctor
}

shirink_app::~shirink_app()
{
    //dtor
}
/*shirink_app::
{

    try
    {

    }catch(...)
    {

    }



}*/
void shirink_app::refresh_ip()
{

    try
    {
        for(int i=1; i<5; i++)
        {
            s_ip.localIP1[i-1]=smem.GetLocalIP1(i);
            s_ip.distIP0[i-1]=smem.GetdistIp0(i);
            s_ip.distIP1[i-1]=smem.GetDistIP(i);
            s_ip.netmask[i-1]=smem.GetNetmask(i);
            s_ip.gateway[i-1]=smem.GetGateway(i);
        }
        s_ip.localIP1_port=smem.GetLocalIP1(5);
        s_ip.distIP0_port=smem.GetdistIp0(5);
        s_ip.distIP1_port=smem.GetDistIP(5);

    }
    catch(...)
    {

    }
}

void shirink_app::refresh_railchan_parama()
{

    try
    {
s_railchain_parama.sTC_TrainChainEnable=smem.vGetUCData(TC_TrainChainEnable);
s_railchain_parama.sTrainComingBanSubphase=smem.vGetUCData(TC_TrainComingBanSubphase);
s_railchain_parama.sTrainComingForceJumpSubphase=smem.vGetUCData(TC_TrainComingForceJumpSubphase);


    }catch(...)
    {

    }



}

void shirink_app::refresh_proxy_transfer()
{


    try
    {

//        s_proxy_transfer.PassMode_0F8E8FIN=smem.vGetPassMode;
//        s_proxy_transfer.proxy_LCN=smem.vGetPassServerLCN;
//


    }catch(...){}

}
void shirink_app::refresh_signal_card_direction()
{
    try
    {
//
//     for(int i=0;i<8;i++)
//     s_signal_card_direction.SignamMappingDirIN[i]=smem.vGetSignamMapMappingDir()
//

    }catch(...){}



}











































