#include "PDEVICEPEDPUSHBUTTON.h"
#include <stdio.h>
#include "SMEM.h"
#include "ConfigFile.h"
#include "CDataToMessageOK.h"
PDEVICEPEDPUSHBUTTON::PDEVICEPEDPUSHBUTTON()
{
    for(int i=0 ; i<20 ; i++) //防呆
    {
        ucSWVarSec[i]=0;
    }
}
PDEVICEPEDPUSHBUTTON::~PDEVICEPEDPUSHBUTTON()
{
}
bool PDEVICEPEDPUSHBUTTON::DoWorkViaPDevice(MESSAGEOK message)
{
    if(message.packet[0]==0xAA && message.packet[1]==0xBB)
    {
        switch (message.packet[7])
        {
        case 0xEA:
            switch (message.packet[8])
            {
            case 0xC6:
                ProcessEAC6(message);
                break;
            case 0xD6:
                ProcessEAD6(message);//交大行人按鈕
                break;
            default :
                break;
            }

            break;
        default :
            break;
        }

    }
    return true;
}
bool PDEVICEPEDPUSHBUTTON::ProcessEAC6(MESSAGEOK message)
{
    unsigned short int DIPVar = message.packet[9];
    unsigned short int SWVar = message.packet[10];
    unsigned short int DevId = message.packet[3]*256+message.packet[4];
    bool SWVarSta=false;
    unsigned char dtat[3];
    MESSAGEOK _MsgOK;
    bool PedSWStateChang = false;
    //printf("!!!!!!!!!!!!!!!\n get PED EAC6 ID:%d ",DevId);
    for(int i=0; i<smem.cPedPushButton.GetDevCount(); i++)
    {
        if(smem.cPedPushButton.GetDevID(i)==DevId && SWVar >=1)
        {
            if(ucSWVarSec[i]<254)
            {
                ucSWVarSec[i]++;
            }

            if(DevId >=8 && smem.cPedPushButton.GetVirtualVDMode()>0) //虛擬VD開啟
            {
                if(ucSWVarSec[i]<=1 || ucSWVarSec[i]%3==0 ) SendPEDLightForCHU(DevId,1);

                smem.cPedPushButton.VirtualVDInfo(i,1);

                if(ucSWVarSec[i]>=254)
                {
                    ucSWVarSec[i]=0;
                }
            }
            else if(DevId < 8 && smem.cPedPushButton.GetVirtualVDMode()>0)
            {
                SendPEDLightForCHU(DevId,1);
            }
        }
        else if(smem.cPedPushButton.GetDevID(i)==DevId && SWVar <= 0)
        {
            ucSWVarSec[i]=0;
            if(DevId >=8  && smem.cPedPushButton.GetVirtualVDMode()>0)
            {
                smem.cPedPushButton.VirtualVDInfo(i,0);
            }
        }

        //check
        if(ucSWVarSec[i]>=5 && smem.cPedPushButton.GetDevID(i)< 8)
        {
            SWVarSta=true;
        }
        printf("ucSWVarSec[%d]=:%d SWVarSta:%d ",i,ucSWVarSec[i],SWVarSta);
    }

    printf("!!!!!!!!!!!!!!!\n");

    if(SWVar>0 && DevId<8)
    {
        if(SWVarSta == false)
        {
            printf("------SWVar > 0 ---------\n");
            //smem.cPedPushButton.ActivateTakePlace(DevId);
            char tmp[200];
            sprintf(tmp,"---->DEVID:%d ------------> Ped Button is running",DevId);
            smem.vWriteMsgToDOM(tmp);
            smem.cPedPushButton.ActivateTakePlace(60000);   //devID固定為60000,執行行人觸動
            //----------------------------------------------readact >>>
            ConfigFile config( "/cct/vddata/dev.cfg" );
            // Values can be read from the file by name
            int iTmp = 0;
            // /* mark 1000419 Light
            if(config.readInto( iTmp, "ACTTYPE" ))
            {
                cout << "The ACTTYPE of is " << iTmp << endl;
                if(iTmp == 1 )
                {
                    if(smem.vGetINTData(Com2_TYPE)==Com2IsPedestrianPushButton || smem.vGetINTData(Com2_TYPE)==Com2IsRedCountAndCom1IsPedPushButton)
                    {
                        if(smem.GetPedlightcheck() == false)
                        {
                            smem.SetPedlightcheck(true) ;
                            for(int i=0 ; i<smem.cPedPushButton.GetDevCount(); i++) //送訊號給所有行人觸動警示燈ON
                            {

                                printf("\n\nlight work!\n\n");
                                dtat[0]=0xEA;
                                dtat[1]=0x17;
                                dtat[2]=0x01;

                                _MsgOK= oDataToMessageOK.vPackageINFOTo92PEDProtocol(dtat,3,true,smem.cPedPushButton.GetDevID(i));
                                //_MsgOK= oDataToMessageOK.vPackageINFOTo92PEDProtocol(dtat,3,true,0xFFFF);
                                //writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICEPEDESTRIAN);
                                smem.cPedPushButton.vPushWriteOutMsgToQueue(0,_MsgOK);
                            }
                        }
                        else          //若行人警示燈已亮則不動作
                        {
                            printf("\n\nPed running!!\n\n");
                        }
                    }
                }
            }
            // */

            //----------------------------------------------readact <<<
        }
    }
    if(smem.cPedPushButton.GetPEDSWVar(DevId) != SWVar) PedSWStateChang = true;

    smem.cPedPushButton.SetPEDSWVar(DevId,SWVar);
    smem.cPedPushButton.SetWatchDogClean(DevId);

    if(PedSWStateChang)
    {
        smem.cPedPushButton.SendPedSWConnetState0F08(1);
        PedSWStateChang = false;
    }

    return true;

}
bool PDEVICEPEDPUSHBUTTON::SendPEDLightForCHU(unsigned int device,unsigned int light)
{
    unsigned char data[4]= {0};
    if(smem.vGetINTData(Com2_TYPE)==Com2IsPedestrianPushButton || smem.vGetINTData(Com2_TYPE)==Com2IsRedCountAndCom1IsPedPushButton)
    {
        for(int i=0 ; i<smem.cPedPushButton.GetDevCount(); i++)
        {
            data[0]=0xEA;
            data[1]=0x17;
            data[2]=light;
            //_MsgOK= oDataToMessageOK.vPackageINFOTo92PEDProtocol(dtat,3,true,smem.cPedPushButton.GetDevID(i));
            MESSAGEOK _MsgOK;
            _MsgOK= oDataToMessageOK.vPackageINFOTo92PEDProtocol(data,3,true,device);
            //writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICEPEDESTRIAN);
            smem.cPedPushButton.vPushWriteOutMsgToQueue(1,_MsgOK);
        }
    }
}
bool PDEVICEPEDPUSHBUTTON::ProcessEAD6(MESSAGEOK message)
{
    unsigned short int DIPVar = message.packet[9];
    unsigned short int SWCount = message.packet[10];
    unsigned short int SWVar[10] = {0};
    unsigned short int DevId = message.packet[3]*256+message.packet[4];
    bool SWVarSta=false;
    unsigned char dtat[3];
    MESSAGEOK _MsgOK;
    printf("!!!!!!!!!!!!!!!\n get PED EAC6 ID:%d ",DevId);
    unsigned char tmpId[10];
    if(smem.cPedPushButton.GetPedisNCTU()==false)
    {
        for(int i=0; i<SWCount; i++) //0-1行人按鈕 8-15偵測器
        {
            if(i<2)
            {
                smem.cPedPushButton.SetDevID(i,i);
            }
            else
            {
                smem.cPedPushButton.SetDevID(i,i+6);
            }
        }
        smem.cPedPushButton.SetPedisNCTU(true);
    }
    //for(int i=0;i<smem.cPedPushButton.GetDevCount();i++){
    for(int i=0; i<SWCount; i++)
    {
        if(i<2)
        {
            tmpId[i]=i;
        }
        else
        {
            tmpId[i]=i+6;
        }
        SWVar[i] = message.packet[11+i];
        if(smem.cPedPushButton.GetDevID(i)==tmpId[i] && SWVar[i] >=1)
        {
            if(ucSWVarSec[i]<254)
            {
                ucSWVarSec[i]++;
            }
            if(tmpId[i] >=8 && smem.cPedPushButton.GetVirtualVDMode()>0) //虛擬VD開啟
            {
                //if(ucSWVarSec[i]<=1 || ucSWVarSec[i]%3==0 )SendPEDLightForCHU(i,1);
                smem.cPedPushButton.VirtualVDInfo(i,1);
                if(ucSWVarSec[i]>=254)
                {
                    ucSWVarSec[i]=0;
                }
            }
            else if(tmpId[i] < 8 && smem.cPedPushButton.GetVirtualVDMode()>0)
            {
                //SendPEDLightForCHU(DevId,1);
            }
        }
        else if(smem.cPedPushButton.GetDevID(i)==tmpId[i]&& SWVar[i] <= 0)
        {
            ucSWVarSec[i]=0;
            if(tmpId[i] >=8  && smem.cPedPushButton.GetVirtualVDMode()>0)
            {
                smem.cPedPushButton.VirtualVDInfo(i,0);
            }
        }
        //check
        if(ucSWVarSec[i]>=5 && smem.cPedPushButton.GetDevID(i)< 8)
        {
            SWVarSta=true;
        }
        printf("ucSWVarSec[%d]=:%d SWVarSta:%d ",i,ucSWVarSec[i],SWVarSta);

    }
    printf("\n!!!!!!!!!!!!!!!\n");
    for(int i=0; i<SWCount; i++)
    {
        if(SWVar[i]>0 && tmpId[i]<8)
        {
            if(SWVarSta == false)
            {
                printf("------SWVar > 0 ---------\n");
                //smem.cPedPushButton.ActivateTakePlace(DevId);
                char tmp[200];
                sprintf(tmp,"---->DEVID:%d ------------> Ped Button is running",tmpId[i]);
                smem.vWriteMsgToDOM(tmp);
                smem.cPedPushButton.ActivateTakePlace(60000);
                break;
            }
        }
    }
    smem.cPedPushButton.SetPEDNCTUSWVar(SWVar);
    smem.cPedPushButton.SetWatchDogClean(DevId);
    return true;

}

