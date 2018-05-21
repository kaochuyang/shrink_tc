#include "CPEDPUSHBUTTON.h"
#include "SMEM.h"
#include "var.h"
#include "CDataToMessageOK.h"
#include "WRITEJOB.h"

#include "CSTC.h"
#include "CPlanInfo.h"
#include "CPhaseInfo.h"
#include "CSegmentInfo.h"
#include <sys/time.h>
CPEDPUSHBUTTON::CPEDPUSHBUTTON()
{
    nowIDCount=0;
    addVar=0;
//    t1flag = false;
//    t2flag = false;
//    t3flag = false;
//    t4flag = false;
    for(int i=0; i<20; i++)
    {
        pedDev[i].staSec =6;
        pedDev[i].vd_stopped =0;
        pedDev[i].vdSta=0;
    }
    m_openSegment=0;
    m_CompensationAct=0;
    m_pedNCTU=false;
}
CPEDPUSHBUTTON::~CPEDPUSHBUTTON()
{
}

bool CPEDPUSHBUTTON::QueryPEDState()
{
    unsigned char dtat[2];
    unsigned char tmpSWState = 0;
    MESSAGEOK _MsgOK;
    if((smem.vGetINTData(Com2_TYPE)==Com2IsPedestrianPushButton|| smem.vGetINTData(Com2_TYPE)==Com2IsRedCountAndCom1IsPedPushButton)
       && smem.cPedPushButton.GetNowRs232Status()==false)
    {
        /*for(int i=0 ; i<smem.cPedPushButton.GetDevCount();i++){
            dtat[0]=0xEA;
            dtat[1]=0x46;
            _MsgOK= oDataToMessageOK.vPackageINFOTo92PEDProtocol(dtat,2,true,smem.cPedPushButton.GetDevID(i));
            //writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICEPEDESTRIAN);
            vPushWriteOutMsgToQueue(2,_MsgOK);
        } */

        if(addVar<1)
        {
            addVar++;
        }
        else
        {
            addVar=0;
            //addVar=1;
            nowIDCount++;
        }

        if(nowIDCount<=smem.cPedPushButton.GetDevCount())
        {
            if(nowIDCount>=smem.cPedPushButton.GetDevCount())     //20130116 chou fix
            {
                nowIDCount=0;                                     //fix in /cct/Data/Setting/SET.ini
            }
            smem.cPedPushButton.SetWatchDogAdd(nowIDCount);//連線
            dtat[0]=0xEA;
            dtat[1]=0x46;
            _MsgOK= oDataToMessageOK.vPackageINFOTo92PEDProtocol(dtat,2,true,smem.cPedPushButton.GetDevID(nowIDCount));
            //writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICEPEDESTRIAN);
            vPushWriteOutMsgToQueue(2,_MsgOK);
            //nowIDCount++;
//            if(t1flag && !t2flag){ gettimeofday(&t2 , NULL); t2flag = true;}
//            if(!t1flag && !t2flag){ gettimeofday(&t1 , NULL); t1flag = true;}
//            if(t1flag && t2flag){
//                printf("@@@@@@@@ QueryPEDState %.02f us \n\n" ,time_diff(t1 , t2));
//                t1flag=false;
//                t2flag=false;
//            }
        }

    }
    return true;
}
/////////////////////////////////////////////////////////////////
bool CPEDPUSHBUTTON::SendPedSWLightOpenAndClose()
{

    unsigned short int iCurrentPhaseID  = stc.vGetUSIData(CSTC_exec_plan_phase_order);
    unsigned short int iCurrentPlanID   = stc.vGetUSIData(CSTC_exec_plan_plan_ID);
    unsigned char dtat[3];
    MESSAGEOK _MsgOK;

    /*mark 1000419 Light
    printf("PhaseID:%d lastPhaseID:%d PlanID:%d lastPlanID:%d\n",iCurrentPhaseID,ilastCurrentPlanID,iCurrentPhaseID,ilastCurrentPlanID);
    if(iCurrentPlanID == 0x28 && ilastCurrentPlanID != iCurrentPlanID && (iCurrentPhaseID!=0xB0 || iCurrentPhaseID!=0xB1)  ){ //plan 40 open
        if(smem.vGetINTData(Com2_TYPE)==Com2IsPedestrianPushButton || smem.vGetINTData(Com2_TYPE)==Com2IsRedCountAndCom1IsPedPushButton){
            for(int i=0 ; i<smem.cPedPushButton.GetDevCount();i++){
                dtat[0]=0xEA;
                dtat[1]=0x17;
                dtat[2]=0x01;
                //_MsgOK= oDataToMessageOK.vPackageINFOTo92PEDProtocol(dtat,3,true,smem.cPedPushButton.GetDevID(i));
                _MsgOK= oDataToMessageOK.vPackageINFOTo92PEDProtocol(dtat,3,true,0xFFFF);
                //writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICEPEDESTRIAN);
                vPushWriteOutMsgToQueue(0,_MsgOK);
            }
        }
    }else if(ilastCurrentPlanID != iCurrentPlanID && (iCurrentPlanID != 0x28 || (iCurrentPhaseID==0xB0 || iCurrentPhaseID==0xB1)) ){//close
        if(smem.vGetINTData(Com2_TYPE)==Com2IsPedestrianPushButton || smem.vGetINTData(Com2_TYPE)==Com2IsRedCountAndCom1IsPedPushButton){
            for(int i=0 ; i<smem.cPedPushButton.GetDevCount();i++){
                dtat[0]=0xEA;
                dtat[1]=0x17;
                dtat[2]=0x00;
                //_MsgOK= oDataToMessageOK.vPackageINFOTo92PEDProtocol(dtat,3,true,smem.cPedPushButton.GetDevID(i));
                _MsgOK= oDataToMessageOK.vPackageINFOTo92PEDProtocol(dtat,3,true,0xFFFF);
                //writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICEPEDESTRIAN);
                vPushWriteOutMsgToQueue(0,_MsgOK);
            }
        }
    }
    */
    ilastCurrentPhaseID = iCurrentPhaseID;
    ilastCurrentPlanID = iCurrentPlanID;

    return true;
}
/////////////////////////////////////////////////////////////////
bool CPEDPUSHBUTTON::SetDevCount(unsigned short int uCount)
{
    DeviceCount = uCount;
    return true;
}
unsigned short int CPEDPUSHBUTTON::GetDevCount()
{
    return DeviceCount;
}
bool CPEDPUSHBUTTON::SetDevID(unsigned short int uCount,unsigned short int uDevNum)
{
    pedDev[uCount].DeviceID=uDevNum;
    return true;
}
unsigned short int CPEDPUSHBUTTON::GetDevID(unsigned short int uCount)
{
    return pedDev[uCount].DeviceID;
}
bool CPEDPUSHBUTTON::SetNowRs232Status(bool status)
{
    isRs232Statas=status;
    return true;
}
bool CPEDPUSHBUTTON::GetNowRs232Status()
{
    return isRs232Statas;
}
/////////////////////////////////////////////////////////////////////////////////////////
bool CPEDPUSHBUTTON::SetWatchDogClean(unsigned short int DevID)
{
    if(m_pedNCTU==false)
    {
        for(int i=0; i<smem.cPedPushButton.GetDevCount(); i++)
        {
            if(pedDev[i].DeviceID == DevID)
            {
                pedDev[i].staSec=6;
                //printf("!!!!!!!SetWatchDogClean DevID:%d==pedDev[%d].DeviceID!!!!!!!\n",DevID,pedDev[i].DeviceID);
            }
        }
    }
    else  //交大版
    {
        for(int i=0; i<10; i++)
        {
            //if(pedDev[i].DeviceID == i){
            pedDev[i].staSec=6;
            //printf("!!!!!!!SetWatchDogClean DevID:%d==pedDev[%d].DeviceID!!!!!!!\n",DevID,pedDev[i].DeviceID);
            //}
        }
    }

    return true;
}
/////////////////////////////////////////////////////////////////////////////////////////
bool CPEDPUSHBUTTON::SetWatchDogAdd(unsigned short int lineID)
{
    /*
    for(int i=0;i<smem.cPedPushButton.GetDevCount();i++){
        if(pedDev[i].staSec <= 255){
            pedDev[i].staSec++;
        }
    }
    */
    if(m_pedNCTU==false)
    {
        if(smem.cPedPushButton.GetDevCount() > lineID)
        {
            if(pedDev[lineID].staSec <= 6 && pedDev[lineID].staSec >= -255 )
            {
                pedDev[lineID].staSec--;
            }
        }
    }
    else  //交大版
    {
        for(int i= 0 ; i<10; i++)
        {
            if(pedDev[i].staSec <= 6 && pedDev[i].staSec >= -255 )
            {
                pedDev[i].staSec--;
            }
        }
    }

    return true;
}
/////////////////////////////////////////////////////////////////////////////////////////
bool CPEDPUSHBUTTON::GetWatchDogStatus(unsigned short int Count)
{
    if(m_pedNCTU==false)
    {
        if(smem.cPedPushButton.GetDevCount() < Count)
        {
            return false;
        }
    }
    else  //交大版
    {
        if(10< Count)
        {
            return false;
        }
    }

    if( pedDev[Count].staSec >= 0)
    {
BenchmarkStepSec:
        return true;
    }
    else
    {
        return false;
    }
}
/////////////////////////////////////////////////////////////////////////////////////////
bool CPEDPUSHBUTTON::ActivateTakePlace(unsigned short int DevID)
{
    unsigned short int iCurrentPhaseID  = stc.vGetUSIData(CSTC_exec_plan_phase_order);
    unsigned short int iCurrentPlanID   = stc.vGetUSIData(CSTC_exec_plan_plan_ID);
    //unsigned short int iCurrentPlanID   = 0x28;
    unsigned char ucData[4];
    ucData[0] = 0x5F;
    ucData[1] = 0xA6;
    ucData[2] = HI(DevID);
    ucData[3] = LO(DevID);
    MESSAGEOK _MSG;
    _MSG = oDataToMessageOK.vPackageINFOTo92Protocol(ucData, 4, true);
    _MSG.InnerOrOutWard = cOutWard;

    if(smem.cPedPushButton.GetVirtualVDMode()<=0)
    {
        writeJob.WritePhysicalOut(_MSG.packet, _MSG.packetLength, DEVICECCJDYNCTL);
    }
    else  //虛擬才丟
    {
        writeJob.WritePhysicalOut(_MSG.packet, _MSG.packetLength, DEVICEARWENVDINFO);
    }
    //smem.vSetBOOLData(TC_CCT_In_LongTanu_ActuateType_Switch, true);
    //stc.vChangeTODCurrentStepSec(2, 0);

    unsigned short int usiTmp;
    smem.vSetUCData(TC92_ucControlStrategy, 0x01); //chou ++ test 2013/01/25
    printf("ilastCurrentPlanID = %d ,iCurrentPlanID = %d\n",ilastCurrentPlanID ,iCurrentPlanID);
    if(1)
    {
        //if(ilastCurrentPlanID != iCurrentPlanID  (iCurrentPhaseID==0xB0 || iCurrentPhaseID==0xB1)  ){
        if(smem.vGetINTData(Com2_TYPE)==Com2IsPedestrianPushButton || smem.vGetINTData(Com2_TYPE)==Com2IsRedCountAndCom1IsPedPushButton)
        {
            printf("\n\n\nPed run!!\nPed run!!\n");
            printf("smem.vGetUCData(TC92_ucControlStrategy) == %d \n\n",smem.vGetUCData(TC92_ucControlStrategy));
            if(smem.vGetUCData(TC92_ucControlStrategy) == 0x01)          //0x04 == TOD
            {
                //if(1) {                                                        //chou ++ test 2013/01/24
                printf("000\n");
                printf("smem.vGetUCData(TC_CCT_Send_ActuateArwen_Protocal)= %d\n",smem.vGetUCData(TC_CCT_Send_ActuateArwen_Protocal));
                //   if( smem.vGetUCData(TC_CCT_Send_ActuateArwen_Protocal) > 0 ) {
                printf("111\n");
                ucData[0] = 0x5F;
                ucData[1] = 0xA6;
                ucData[2] = HI(60000);
                ucData[3] = LO(60000);
                _MSG = oDataToMessageOK.vPackageINFOTo92Protocol(ucData, 4, true);
                _MSG.InnerOrOutWard = cOutWard;
                writeJob.WritePhysicalOut(_MSG.packet, _MSG.packetLength, DEVICECCJDYNCTL);
                /*  } else {
                     printf("222\n");
                    if(smem.vGetBOOLData(TC_CCTActuate_TOD_Running) == false) {
                      printf("333\n");
                      smem.vSetBOOLData(TC_CCT_In_LongTanu_ActuateType_Switch, true);
                      stc.vChangeTODCurrentStepSec(2, 0);
                    } else {
                      printf("444\n");
                      usiTmp = stc.vGetUSIData(CSTC_exec_phase_current_subphase);
                      stc.vChangeTODCurrentStepSec(2, 0);
                      if(usiTmp != 0) {
                        printf("555\n");                                          //Not First Subphase, can re-trigger
                        smem.vSetBOOLData(TC_CCT_In_LongTanu_ActuateType_Switch, true);
                      }
                    }

                  }*/
            }
            else
            {
                printf("error\n");
            }
        }
    }
    else
    {
        printf("\n\nNO use! \n\n");     //no use
    }

    return true;
}
/////////////////////////////////////////////////////////////////
bool CPEDPUSHBUTTON::VirtualVDMode(unsigned short int mode) //SetVirtualVDMode chou 2013/01/28 check
{
    try
    {
        m_VirtualVDMode = mode;
        return true;
    }
    catch(...) {}
}
/////////////////////////////////////////////////////////////////
unsigned short int CPEDPUSHBUTTON::GetVirtualVDMode()
{
    try
    {
        unsigned short int mode=0;
        mode = m_VirtualVDMode;
        return mode;
    }
    catch(...) {}
}
/////////////////////////////////////////////////////////////////
bool CPEDPUSHBUTTON::GetVirtualVDStaSttopped(unsigned int no,unsigned int & sta,unsigned int & stopped)
{
    sta = pedDev[no].vdSta ;
    stopped = pedDev[no].vd_stopped;
    return true;
}
/////////////////////////////////////////////////////////////////
bool CPEDPUSHBUTTON::VirtualVDInfo(unsigned short int DevID,unsigned short tmpSta)
{
    try
    {
        if(pedDev[DevID].vdSta!=tmpSta)
        {
            pedDev[DevID].vd_stopped = 0;
        }
        pedDev[DevID].vdSta= tmpSta;
        return true;
    }
    catch(...) {}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CPEDPUSHBUTTON::sendVirtualVDData()
{
    try
    {
        int iMaxVDNum = 0;
        unsigned short int usiLCN[64] = {0};
        unsigned short int usiTmp;
        unsigned char data[30]= {0};
        bool reVDNum=false;
        //---------------------------------------------------chou check 2013/01/28 實機時MARK
        printf("m_VirtualVDMode = %d \n",m_VirtualVDMode);
        if(m_VirtualVDMode > 0) //虛擬VD開啟
        {
            printf("\nVirtualVD ON!\n");
            for(int i = 0; i < 8; i++)
            {
                for(int j = 0; j < 8; j++)
                {
                    usiTmp = smem.vGetVDPhaseMapTable(i, j);
                    if(usiTmp > 0)
                    {
                        reVDNum=false;
                        for(int k=0; k<iMaxVDNum; k++)
                        {
                            if(usiLCN[i]==usiTmp) //VD編號有沒有重覆
                            {
                                reVDNum=true;
                                break;
                            }
                        }
                        if(reVDNum==false)
                        {
                            usiLCN[iMaxVDNum] = usiTmp;
                            iMaxVDNum++;
                        }
                    }
                }
            }
            // *////////////////////////////////chou check 2013/01/28 實機時MARK
            int ucDevNum=0;
            if(m_pedNCTU==false)    ucDevNum=GetDevCount();
            else ucDevNum = 10;//交大版

            for(int i =0 ; i<ucDevNum; i++)
            {
                //printf("VDDev:%d pedDev[%d].vdSta:%d pedDev[%d].vd_stopped:%d\n",GetDevID(i),i,pedDev[i].vdSta,i,pedDev[i].vd_stopped);
                //for(int j=0;j<iMaxVDNum;j++){
                //if(usiLCN[j] == GetDevID(i)&&  GetDevID(i)>=8 && GetWatchDogStatus(i)==true){
                if(GetDevID(i)>=8 && GetWatchDogStatus(i)==true)
                {
                    data[0]=0x6F;
                    data[1]=0x05;
                    data[2]=pedDev[i].vdSta;
                    data[3]=HI(pedDev[i].vd_stopped);
                    data[4]=LO(pedDev[i].vd_stopped);
                    data[5]=pedDev[i].vdSta;
                    data[6]=HI(pedDev[i].vd_stopped);
                    data[7]=LO(pedDev[i].vd_stopped);
                    data[8]=pedDev[i].vdSta;
                    data[9]=HI(pedDev[i].vd_stopped);
                    data[10]=LO(pedDev[i].vd_stopped);
                    data[11]=pedDev[i].vdSta;
                    data[12]=HI(pedDev[i].vd_stopped);
                    data[13]=LO(pedDev[i].vd_stopped);
                    if(pedDev[i].vd_stopped<254)
                    {
                        pedDev[i].vd_stopped++;
                    }
                    MESSAGEOK _MsgOK;

                    _MsgOK= oDataToMessageOK.vPackageINFOTo92PEDProtocol(data,14,true,smem.cPedPushButton.GetDevID(i));
                    _MsgOK.InnerOrOutWard = cOutWard;
                    writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICEARWENVDINFO);
                }

            }
        }
        return true;

    }
    catch(...) {}
}
/////////////////////////////////////////////////////////////////
bool CPEDPUSHBUTTON::SetPEDSWVar(unsigned short int DevID,unsigned short int tmpVar)
{
    try
    {
        for(int i=0; i<smem.cPedPushButton.GetDevCount(); i++)
        {
            if(pedDev[i].DeviceID == DevID)
            {
                pedDev[i].PedSWVar=tmpVar;
                //printf("!!!!!!!SetPEDSWVar  DevID:%d pedDev[%d].PedSWVar:%d!!!!!!!\n",DevID,i,pedDev[i].PedSWVar);
            }
        }
        return true;
    }
    catch(...) {}
}
///////////////////////////////////////////////////////////////////
unsigned short int CPEDPUSHBUTTON::GetPEDSWVar(unsigned short int Count)
{
    try
    {
        if(m_pedNCTU==false)
        {
            if(smem.cPedPushButton.GetDevCount() < Count)
            {
                return 0;
            }
            return pedDev[Count].PedSWVar;
        }
        else  //交大版
        {
            if(10 < Count)
            {
                return 0;
            }
            return pedDev[Count].PedSWVar;
        }

    }
    catch(...) {}
}
/////////////////////////////////////////////////////////////////
bool CPEDPUSHBUTTON::SetPEDNCTUSWVar(unsigned short int *tmpVar)
{
    try
    {
        for(int i=0; i<10; i++)
        {
            //if(pedDev[i].DeviceID == i){
            pedDev[i].PedSWVar=tmpVar[i];
            //printf("!!!!!!!SetPEDSWVar  DevID:%d pedDev[%d].PedSWVar:%d!!!!!!!\n",DevID,i,pedDev[i].PedSWVar);
            //}
        }
        return true;
    }
    catch(...) {}
}
//----------------------------------------------------------------------
bool CPEDPUSHBUTTON::vPushWriteOutMsgToQueue(unsigned char ucPriority, MESSAGEOK _MsgIn)
{
    try
    {

        pthread_mutex_lock(&mutexPed);

        _WaitWriteOutMsg[ucPriority].push(_MsgIn);
        //writeJob.vPrintf("\nvPushWriteOutMsgToQueue[BUFFERSIZE]=%d\n",_WaitWriteOutMsg[ucPriority].size());
        pthread_mutex_unlock(&mutexPed);
        return true;
    }
    catch(...) {}
}
//----------------------------------------------------------------
bool CPEDPUSHBUTTON::vCheckQueueMsgForWriteOut(void)
{
    try
    {

        MESSAGEOK _Msg;
        bool bSent = false;
        pthread_mutex_lock(&mutexPed);
        //writeJob.vMainPrintf("[P:%3X]",ucTimePeriod);
        //0  1  2  3  4  5  6  7  8  9
        //RMS            RMS

        if(_WaitWriteOutMsg[0].empty())
        {
        }
        while(_WaitWriteOutMsg[0].empty() == false)
        {
            _Msg = _WaitWriteOutMsg[0].front();
            //writeJob.WritePhysicalOut(_Msg.packet, _Msg.packetLength, _Msg.device);  //BestHi Prority
            if(smem.PedPushButtonPort.GetPortAlreadyOpen())
            {
                smem.PedPushButtonPort.Rs232Write(_Msg.packet, _Msg.packetLength,"PedPushButtonPort-case0");
            }
            bSent = true;
            _WaitWriteOutMsg[0].pop();
            //writeJob.vMainPrintf("[P:%3X] BestHi",ucTimePeriod);
        }
        if(bSent == false)
        {
            if(_WaitWriteOutMsg[1].empty() == false)    //Hi Prority
            {
                _Msg = _WaitWriteOutMsg[1].front();
                //writeJob.WritePhysicalOut(_Msg.packet, _Msg.packetLength, _Msg.device);  //Hi Prority
                if(smem.PedPushButtonPort.GetPortAlreadyOpen())
                {
                    smem.PedPushButtonPort.Rs232Write(_Msg.packet, _Msg.packetLength,"PedPushButtonPort-case1");
                }
                _WaitWriteOutMsg[1].pop();
                //writeJob.vMainPrintf("[P:%3X] Hi",ucTimePeriod);
            }
            else
            {
                if(_WaitWriteOutMsg[2].empty() == false)    //Lo Prority
                {
                    _Msg = _WaitWriteOutMsg[2].front();
                    //writeJob.WritePhysicalOut(_Msg.packet, _Msg.packetLength, _Msg.device);  //Lo Prority
                    if(smem.PedPushButtonPort.GetPortAlreadyOpen())
                    {
                        smem.PedPushButtonPort.Rs232Write(_Msg.packet, _Msg.packetLength,"PedPushButtonPort-case2");
//                    gettimeofday(&t5 , NULL);
//                    printf("vCheckQueueMsgForWriteOut @@@@@@@ %.02lf us\n", ((double)t5.tv_sec*1000000 + (double)t5.tv_usec));
//                    if(t3flag && !t4flag){ gettimeofday(&t4 , NULL); t4flag = true;}
//                    if(!t3flag && !t4flag){ gettimeofday(&t3 , NULL); t3flag = true;}
//                    if(t3flag && t4flag){
//                        printf("$$$$$$$$ vCheckQueueMsgForWriteOut %.02lf us \n\n" ,time_diff(t3 , t4));
//                        t3flag=false;
//                        t4flag=false;
//                    }
                    }
                    _WaitWriteOutMsg[2].pop();
                    printf("^^^^^^^^ _WaitWriteOutMsg[2] %d\n",_WaitWriteOutMsg[2].size());
                    //writeJob.vMainPrintf("[P:%3X] Lo %X %X %X",ucTimePeriod,_Msg.packet[0],_Msg.packet[1],_Msg.packet[2]);
                }
            }
        }

        ucTimePeriod++;
        if(ucTimePeriod > 0x13)
        {
            ucTimePeriod = 0x00;
        }
        pthread_mutex_unlock(&mutexPed);

        return true;
    }
    catch(...)
    {
        return false;
    }
}
//----------------------------------------------------------------
//Arwen ++ 1000411
bool CPEDPUSHBUTTON::SetOpenSegmentAlter(unsigned short int tmp_openSegment)
{
    m_openSegment = tmp_openSegment;
    return true;
}//----------------------------------------------------------------
unsigned short int CPEDPUSHBUTTON::GetOpenSegmentAlter()
{
    unsigned short int tmp_openSegment=0;
    tmp_openSegment = m_openSegment;
    return tmp_openSegment;
}
//----------------------------------------------------------------
bool CPEDPUSHBUTTON::SetCompensationAct(unsigned short int tmp_CompensationAct)
{
    m_CompensationAct=tmp_CompensationAct;
    return true;
}//----------------------------------------------------------------
unsigned short int CPEDPUSHBUTTON::GetCompensationAct()
{
    unsigned short int tmp_CompensationAct=0;
    tmp_CompensationAct=m_CompensationAct;
    return tmp_CompensationAct;
}
bool CPEDPUSHBUTTON::SetPedisNCTU(bool tmp)
{
    m_pedNCTU = tmp;
    return true;
}
bool CPEDPUSHBUTTON::GetPedisNCTU()
{
    bool tmp;
    tmp = m_pedNCTU;
    return tmp;
}
//------------jcaky20160224----------------------------------
bool CPEDPUSHBUTTON::SendPedSWConnetState0F08(unsigned char whoreport) // whoreport 0:定期回報 1：SW狀態改變回報
{
    try
    {
        unsigned char data[8]= {0};
        MESSAGEOK _MsgOK;
        unsigned short int YJPedSWVar=0;
        bool YJState = false;

        data[0] = 0x0F;
        data[1] = 0x08;
        data[2] = whoreport;
        data[3] = GetDevCount();

//    data[3] = (PedSWConnectState >> 8) & 0xFF;
//    data[4] = PedSWConnectState & 0xFF;
        for(int i=GetDevCount(); i>0; i--)
        {
            YJState = GetWatchDogStatus(i-1);
            YJPedSWVar = GetPEDSWVar(i-1);
            if(GetDevCount()>8)
            {
                if(i>8)
                {
                    data[4] |= (YJState << i-1);
                    data[6] |= (YJPedSWVar << i-1);
                }
                else
                {
                    data[5] |= (YJState << i-1);
                    data[7] |= (YJPedSWVar << i-1);
                }
            }
            else
            {
                data[4] = 0x0;
                data[5] |= (YJState << i-1);
                data[6] = 0x0;
                data[7] |= (YJPedSWVar << i-1);
            }
        }
        _MsgOK = oDataToMessageOK.vPackageINFOTo92Protocol(data, 8, true);
        _MsgOK.InnerOrOutWard = cOutWard;
        writeJob.WritePhysicalOut(_MsgOK.packet, _MsgOK.packetLength, DEVICECENTER92); //中心port

    }
    catch (...) {}
}
//test 2025
//double CPEDPUSHBUTTON::time_diff(struct timeval x , struct timeval y)
//{
//    double x_ms , y_ms , diff;
//
//    x_ms = (double)x.tv_sec*1000000 + (double)x.tv_usec;
//    y_ms = (double)y.tv_sec*1000000 + (double)y.tv_usec;
//
//    diff = (double)y_ms - (double)x_ms;
//
//    return diff;
//}
