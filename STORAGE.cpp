//---------------------------------------------------------------------------
#include "STORAGE.h"
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include "WRITEJOB.h"
#include "SMEM.h"//just want to set smem.setlocalip
//---------------------------------------------------------------------------
STORAGE::STORAGE(void)
{
    try
    {
        pthread_mutex_init(&mutexDisk,NULL);

    }
    catch (...) {}
}
//---------------------------------------------------------------------------
STORAGE::~STORAGE(void)
{
    try
    {

    }
    catch (...) {}
}
//---------------------------------------------------------------------------
bool STORAGE::OpenFile(char *fileName,int mode)
{
    try
    {
        char completeFileName[256] = "/cct/Data/VD/";
        int i, fileNameLength;

        pthread_mutex_lock(&mutexDisk);

        fileNameLength = sizeof(fileName);
        for(i = 0; i < fileNameLength; i++)
        {
            completeFileName[i+9] = fileName[i];
            nowOpen[i] = fileName[i];
        }
        nowOpen[i] = 0;

        switch(mode)
        {
        case (0):
            fileD = fopen(completeFileName, "r");
            break;
        case (1):
            fileD = fopen(completeFileName, "w");
            break;
        case (2):
            fileD = fopen(completeFileName, "a+");
            break;
        default:
            break;
        }
        if(fileD)
        {
            pthread_mutex_unlock(&mutexDisk);
            return true;
        }
        else
        {
            pthread_mutex_unlock(&mutexDisk);
            return false;
        }
    }
    catch (...) {}
}
//---------------------------------------------------------------------------
bool STORAGE::CloseFile(void)
{
    try
    {
        pthread_mutex_lock(&mutexDisk);
        fclose(fileD);
        pthread_mutex_unlock(&mutexDisk);
        return true;
    }
    catch (...) {}
}
//---------------------------------------------------------------------------
bool STORAGE::ReadFile(void)
{
    try
    {
        pthread_mutex_lock(&mutexDisk);

        pthread_mutex_unlock(&mutexDisk);
        return true;
    }
    catch (...) {}
}
//---------------------------------------------------------------------------
bool STORAGE::WriteFile(void)
{
    try
    {
        pthread_mutex_lock(&mutexDisk);

        pthread_mutex_unlock(&mutexDisk);
        return true;
    }
    catch (...) {}
}
//---------------------------------------------------------------------------
bool STORAGE::WriteReturn(void)
{
    try
    {
        pthread_mutex_lock(&mutexDisk);

        pthread_mutex_unlock(&mutexDisk);
        return true;
    }
    catch (...) {}
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
bool STORAGE::WriteSSSettingFile(short int TimeInterval, int CarLength1, int CarLength2)
{
    try
    {
        pthread_mutex_lock(&mutexDisk);
        fileD = fopen("/cct/Data/SS/SmartSensorSetting.bin", "w");
        if(fileD)
        {
            fwrite( &TimeInterval, sizeof(short int), 1, fileD);
            fwrite( &CarLength1, sizeof(int), 1, fileD);
            fwrite( &CarLength2, sizeof(int), 1, fileD);
//    CloseFile();
            fclose(fileD);
        }

        system("sync");
        pthread_mutex_unlock(&mutexDisk);
        return true;
    }
    catch (...) {}
}

//---------------------------------------------------------------------------
/* howToUse  storage.ReadSSSettingFile (&T, &C1, &C2); */
bool STORAGE::ReadSSSettingFile(short int *TimeInterval, int *CarLength1, int *CarLength2)
{
    try
    {
        pthread_mutex_lock(&mutexDisk);
//    OpenFile("SmartSensorSetting", 0);
        fileD = fopen("/cct/Data/VD/SS/SmartSensorSetting.bin", "r");
        if(fileD)
        {
            fread( TimeInterval, sizeof(short int), 1, fileD);
            fread( CarLength1, sizeof(int), 1, fileD);
            fread( CarLength2, sizeof(int), 1, fileD);
//      CloseFile();
            fclose(fileD);
        }

        pthread_mutex_unlock(&mutexDisk);
        return true;
    }
    catch (...) {}
}

//---------------------------------------------------------------------------
bool STORAGE::WriteVDRecordFile(int SEQ, YMDHMS Date, unsigned char LC, SeqNoContent SNC, time_t RunSec)
{
    try
    {
        char filename[36];
        sprintf( filename, "/cct/Data/VD/RecordData%d.bin\0", SEQ);

        pthread_mutex_lock(&mutexDisk);
        fileD = fopen(filename, "w");
        if(fileD)
        {
            fwrite( &Date, sizeof(YMDHMS), 1, fileD);
            fwrite( &LC, sizeof(unsigned char), 1, fileD);
            fwrite( &SNC, sizeof(SeqNoContent), 1, fileD);
            fwrite( &RunSec, sizeof(time_t), 1, fileD);                                 //µ¹§ä¥XSeqPtr¥Îªº
            fclose(fileD);
        }

        system("sync");
        pthread_mutex_unlock(&mutexDisk);

        //OTMARKPRINTF  printf("[Message] SEQ: %d time: %d, VD Data save to Device.\n", SEQ, RunSec);

        return true;
    }
    catch (...) {}
}
//---------------------------------------------------------------------------
bool STORAGE::ReadVDRecordFile(int SEQ, YMDHMS *Date, unsigned char *LC, SeqNoContent *SNC, time_t *RunSec)
{
    try
    {
        char filename[36];
        sprintf( filename, "/cct/Data/VD/RecordData%d.bin\0", SEQ);

        pthread_mutex_lock(&mutexDisk);
        fileD = fopen(filename, "r");
        if(fileD)
        {
            fread( Date, sizeof(YMDHMS), 1, fileD);
            fread( LC, sizeof(unsigned char), 1, fileD);
            fread( SNC, sizeof(SeqNoContent), 1, fileD);
            fread( RunSec, sizeof(time_t), 1, fileD);
            fclose(fileD);
//  //OTMARKPRINTF  printf("Read Real Data No.%d Sec@%d OK!\n", SEQ, RunSec);
        }
        pthread_mutex_unlock(&mutexDisk);

        return true;
    }
    catch (...) {}
}

//---------------------------------------------------------------------------
bool STORAGE::vWriteVDHistoryRecordFile(int SEQ, YMDHMS Date, unsigned char LC, SeqNoContent SNC, time_t RunSec)
{
    try
    {
        char filename[48];
        sprintf( filename, "/cct/Data/VD/HistoryRecordData%d.bin\0", SEQ);

        pthread_mutex_lock(&mutexDisk);
        fileD = fopen(filename, "w");
        if(fileD)
        {
            fwrite( &Date, sizeof(YMDHMS), 1, fileD);
            fwrite( &LC, sizeof(unsigned char), 1, fileD);
            fwrite( &SNC, sizeof(SeqNoContent), 1, fileD);
            fwrite( &RunSec, sizeof(time_t), 1, fileD);                                 //µ¹§ä¥XSeqPtr¥Îªº
            fclose(fileD);
        }

        system("sync");
        pthread_mutex_unlock(&mutexDisk);

        //OTMARKPRINTF  printf("[Message] SEQ: %d time: %d, VD History Data save to Device.\n", SEQ, RunSec);

        return true;
    }
    catch (...) {}
}
//---------------------------------------------------------------------------
bool STORAGE::vReadVDHistoryRecordFile(int SEQ, YMDHMS *Date, unsigned char *LC, SeqNoContent *SNC, time_t *RunSec)
{
    try
    {
        char filename[48];
        sprintf( filename, "/cct/Data/VD/HistoryRecordData%d.bin\0", SEQ);

        pthread_mutex_lock(&mutexDisk);
        fileD = fopen(filename, "r");
        if(fileD)
        {
            fread( Date, sizeof(YMDHMS), 1, fileD);
            fread( LC, sizeof(unsigned char), 1, fileD);
            fread( SNC, sizeof(SeqNoContent), 1, fileD);
            fread( RunSec, sizeof(time_t), 1, fileD);
            fclose(fileD);
//  //OTMARKPRINTF  printf("Read Real Data No.%d Sec@%d OK!\n", SEQ, RunSec);
        }
        pthread_mutex_unlock(&mutexDisk);

        return true;
    }
    catch (...) {}
}


//---------------------------------------------------------------------------
bool STORAGE::vWrite92COMMSettingFile(unsigned char ucCommandSetIN,
                                      char *passwordIN,
                                      int dbOperStatIN,
                                      unsigned char HWCycleCodeIN,
                                      int *iSSMapping92VDIN,                      //°»´ú¾¹»P±±¨î¾¹¨®¹D¹ïÀ³
                                      int iAdjudicateReturnAddressBCDorHEXIN,
                                      unsigned short int *usiWayMappingRedCountIN,
                                      // OT Pass
                                      unsigned char ucPassMode_0F8E8FIN,
                                      unsigned short int usiPassServerLCNIN,
                                      //OT Debug Direct
                                      unsigned short int *usiSignamMapMappingDirIN,
                                      unsigned short int *usiSignamMapMappingLightBoardIN,
                                      int localIp1_1,int localIp1_2,int localIp1_3,int localIp1_4,int localPort1,
                                      int distIp0_1,int distIp0_2,int distIp0_3,int distIp0_4,int dist0Port,
                                      int distIp1,int distIp2,int distIp3,int distIp4,int distPort,
                                      unsigned char ucRailChain1, unsigned char ucRailChain2, unsigned char ucRailChain3,
                                      unsigned char ucRevLaneManualEn,
                                      //OT20111020
                                      int netmask_1,int netmask_2,int netmask_3,int netmask_4,
                                      int gateway_1,int gateway_2,int gateway_3,int gateway_4
                                     )
{
    try
    {

        FILE *Lane_wfile=NULL;
        char filename[36];
        sprintf( filename,"/cct/Data/SETTING/92COMM.bin\0" );
        Lane_wfile = fopen(filename , "w"); //fopen return NULL if file not exist
        pthread_mutex_lock(&mutexDisk);
        if(Lane_wfile)
        {


            //OTMARKPRINTF  printf("Writing File 92COMM Setting To Share Memory\n");

            fwrite( &ucCommandSetIN, sizeof( unsigned char ), 1, Lane_wfile );
            fwrite( passwordIN, sizeof(char), 8, Lane_wfile );
            fwrite( &dbOperStatIN, sizeof( int ), 1, Lane_wfile );
            fwrite( &HWCycleCodeIN, sizeof( unsigned char ), 1, Lane_wfile );
            fwrite( iSSMapping92VDIN, sizeof(int), 8, Lane_wfile );
            fwrite( &iAdjudicateReturnAddressBCDorHEXIN, sizeof( int ), 1, Lane_wfile );
            fwrite( usiWayMappingRedCountIN, sizeof( unsigned short int ), 8, Lane_wfile );
            // OT Pass
            fwrite( &ucPassMode_0F8E8FIN, sizeof( unsigned char ), 1, Lane_wfile );
            fwrite( &usiPassServerLCNIN, sizeof( unsigned short int ), 1, Lane_wfile );
            fwrite( usiSignamMapMappingDirIN, sizeof( unsigned short int ), 8, Lane_wfile );
            fwrite( usiSignamMapMappingLightBoardIN, sizeof( unsigned short int ), 6, Lane_wfile );
//OT 961211
            fwrite( &localIp1_1, sizeof( int ), 1, Lane_wfile );
            fwrite( &localIp1_2, sizeof( int ), 1, Lane_wfile );
            fwrite( &localIp1_3, sizeof( int ), 1, Lane_wfile );
            fwrite( &localIp1_4, sizeof( int ), 1, Lane_wfile );
            fwrite( &localPort1, sizeof( int ), 1, Lane_wfile );
            fwrite( &distIp0_1, sizeof( int ), 1, Lane_wfile );
            fwrite( &distIp0_2, sizeof( int ), 1, Lane_wfile );
            fwrite( &distIp0_3, sizeof( int ), 1, Lane_wfile );
            fwrite( &distIp0_4, sizeof( int ), 1, Lane_wfile );
            fwrite( &dist0Port, sizeof( int ), 1, Lane_wfile );
            fwrite( &distIp1, sizeof( int ), 1, Lane_wfile );
            fwrite( &distIp2, sizeof( int ), 1, Lane_wfile );
            fwrite( &distIp3, sizeof( int ), 1, Lane_wfile );
            fwrite( &distIp4, sizeof( int ), 1, Lane_wfile );
            fwrite( &distPort, sizeof( int ), 1, Lane_wfile );

            fwrite( &ucRailChain1, sizeof( unsigned char ), 1, Lane_wfile );
            fwrite( &ucRailChain2, sizeof( unsigned char ), 1, Lane_wfile );
            fwrite( &ucRailChain3, sizeof( unsigned char ), 1, Lane_wfile );
            fwrite( &ucRevLaneManualEn, sizeof( unsigned char ), 1, Lane_wfile );

            fwrite( &netmask_1, sizeof( int ), 1, Lane_wfile );
            fwrite( &netmask_2, sizeof( int ), 1, Lane_wfile );
            fwrite( &netmask_3, sizeof( int ), 1, Lane_wfile );
            fwrite( &netmask_4, sizeof( int ), 1, Lane_wfile );
            fwrite( &gateway_1, sizeof( int ), 1, Lane_wfile );
            fwrite( &gateway_2, sizeof( int ), 1, Lane_wfile );
            fwrite( &gateway_3, sizeof( int ), 1, Lane_wfile );
            fwrite( &gateway_4, sizeof( int ), 1, Lane_wfile );


            fclose( Lane_wfile );
        }

        system("sync");
        pthread_mutex_unlock(&mutexDisk);
    }
    catch(...) {}
}

//---------------------------------------------------------------------------
bool STORAGE::vRead92COMMSettingFile( unsigned char *ucCommandSetIN,
                                      char *passwordIN,
                                      int *dbOperStatIN,
                                      unsigned char *HWCycleCodeIN,
                                      int *iSSMapping92VDIN,                      //°»´ú¾¹»P±±¨î¾¹¨®¹D¹ïÀ³
                                      int *iAdjudicateReturnAddressBCDorHEXIN,
                                      unsigned short int *usiWayMappingRedCountIN,
                                      unsigned short int *usiWayMappingPedGreenCountIN,
                                      // OT Pass
                                      unsigned char *ucPassMode_0F8E8FIN,
                                      unsigned short int *usiPassServerLCNIN,
                                      //OT Debug  Direct
                                      unsigned short int *usiSignamMapMappingDirIN,
                                      unsigned short int *usiSignamMapMappingLightBoardIN,
                                      int *localIp1_1,int *localIp1_2,int *localIp1_3,int *localIp1_4,int *localPort1,
                                      int *distIp0_1,int *distIp0_2,int *distIp0_3,int *distIp0_4,int *dist0Port,
                                      int *distIp1,int *distIp2,int *distIp3,int *distIp4,int *distPort,
                                      unsigned char *ucRailChain1, unsigned char *ucRailChain2, unsigned char *ucRailChain3,
                                      unsigned char *ucRevLaneManualEn,
                                      //OT20111020
                                      int *netmask1,int *netmask2,int *netmask3,int *netmask4,
                                      int *gateway1,int *gateway2,int *gateway3,int *gateway4
                                    )
{
    try
    {
        FILE *Lane_rfile=NULL;
        char filename[36];
        sprintf( filename,"/cct/Data/SETTING/92COMM.bin\0" );
        Lane_rfile = fopen(filename, "r"); //fopen return NULL if file not exist
        pthread_mutex_lock(&mutexDisk);
        if(Lane_rfile)
        {


            //OTMARKPRINTF  printf("Reading File 92COMM Setting To Share Memory\n");

            fread( ucCommandSetIN, sizeof( unsigned char ), 1, Lane_rfile );
            fread( passwordIN, sizeof(char), 8, Lane_rfile );
            fread( dbOperStatIN, sizeof( int ), 1, Lane_rfile );
            fread( HWCycleCodeIN, sizeof( unsigned char ), 1, Lane_rfile );
            fread( iSSMapping92VDIN, sizeof(int), 8, Lane_rfile );
            fread( iAdjudicateReturnAddressBCDorHEXIN, sizeof( int ), 1, Lane_rfile );
            fread( usiWayMappingRedCountIN, sizeof( unsigned short int ), 8, Lane_rfile );
            // OT Pass
            fread( ucPassMode_0F8E8FIN, sizeof( unsigned char ), 1, Lane_rfile );

            //OT Debug 950628
            fread( usiPassServerLCNIN, sizeof( unsigned short int ), 1, Lane_rfile );
            //OT Debug Direct
            fread( usiSignamMapMappingDirIN, sizeof( unsigned short int ), 8, Lane_rfile );
            fread( usiSignamMapMappingLightBoardIN, sizeof( unsigned short int ), 6, Lane_rfile );

//OT 961211
            fread( localIp1_1, sizeof( int ), 1, Lane_rfile );
            fread( localIp1_2, sizeof( int ), 1, Lane_rfile );
            fread( localIp1_3, sizeof( int ), 1, Lane_rfile );
            fread( localIp1_4, sizeof( int ), 1, Lane_rfile );
            fread( localPort1, sizeof( int ), 1, Lane_rfile );
            fread( distIp0_1, sizeof( int ), 1, Lane_rfile );
            fread( distIp0_2, sizeof( int ), 1, Lane_rfile );
            fread( distIp0_3, sizeof( int ), 1, Lane_rfile );
            fread( distIp0_4, sizeof( int ), 1, Lane_rfile );
            fread( dist0Port, sizeof( int ), 1, Lane_rfile );
            fread( distIp1, sizeof( int ), 1, Lane_rfile );
            fread( distIp2, sizeof( int ), 1, Lane_rfile );
            fread( distIp3, sizeof( int ), 1, Lane_rfile );
            fread( distIp4, sizeof( int ), 1, Lane_rfile );
            fread( distPort, sizeof( int ), 1, Lane_rfile );

            fread( ucRailChain1, sizeof( unsigned char ), 1, Lane_rfile );
            fread( ucRailChain2, sizeof( unsigned char ), 1, Lane_rfile );
            fread( ucRailChain3, sizeof( unsigned char ), 1, Lane_rfile );

            fread( ucRevLaneManualEn, sizeof( unsigned char ), 1, Lane_rfile );

            //OT20111020
            fread( netmask1, sizeof( int ), 1, Lane_rfile );
            fread( netmask2, sizeof( int ), 1, Lane_rfile );
            fread( netmask3, sizeof( int ), 1, Lane_rfile );
            fread( netmask4, sizeof( int ), 1, Lane_rfile );
            fread( gateway1, sizeof( int ), 1, Lane_rfile );
            fread( gateway2, sizeof( int ), 1, Lane_rfile );
            fread( gateway3, sizeof( int ), 1, Lane_rfile );
            fread( gateway4, sizeof( int ), 1, Lane_rfile );

            fclose( Lane_rfile );
        }
        pthread_mutex_unlock(&mutexDisk);
    }
    catch(...) {}
}
//---------------------------------------------------------------------------
bool STORAGE::vWrite92VDSettingFile( unsigned char ucActuateTypeIN,
                                     unsigned char ucOccupyTimeIN,
                                     unsigned char ucSettingLaneCountIN,
                                     unsigned char ucDetectorMapIN,
                                     short int siTimeGapIN,
                                     unsigned short int usiBigCarLengthIN,
                                     unsigned short int usiCarLengthIN,
                                     short int *siTransmitType,
                                     unsigned char uc92VDLaneMapIN
                                   )
{
    try
    {

        FILE *Lane_wfile=NULL;
        char filename[36];
        sprintf( filename,"/cct/Data/SETTING/92VD.bin\0" );
        Lane_wfile = fopen(filename , "w"); //fopen return NULL if file not exist
        pthread_mutex_lock(&mutexDisk);
        if(Lane_wfile)
        {

            //OTMARKPRINTF  printf("Writing File 92VD Setting To Share Memory\n");

            fwrite( &ucActuateTypeIN, sizeof( unsigned char ), 1, Lane_wfile );
            fwrite( &ucOccupyTimeIN, sizeof( unsigned char ), 1, Lane_wfile );
            fwrite( &ucSettingLaneCountIN, sizeof( unsigned char ), 1, Lane_wfile );
            fwrite( &ucDetectorMapIN, sizeof( unsigned char ), 1, Lane_wfile );
            fwrite( &siTimeGapIN, sizeof( short int ), 1, Lane_wfile );
            fwrite( &usiBigCarLengthIN, sizeof( unsigned short int ), 1, Lane_wfile );
            fwrite( &usiCarLengthIN, sizeof( unsigned short int ), 1, Lane_wfile );
            fwrite( siTransmitType, sizeof( short int ), 5, Lane_wfile );
            fwrite( &uc92VDLaneMapIN, sizeof( unsigned char ), 1, Lane_wfile );
            fclose( Lane_wfile );
        }

        system("sync");
        pthread_mutex_unlock(&mutexDisk);
    }
    catch(...) {}
}

//---------------------------------------------------------------------------
bool STORAGE::vRead92VDSettingFile(  unsigned char *ucActuateTypeIN,
                                     unsigned char *ucOccupyTimeIN,
                                     unsigned char *ucSettingLaneCountIN,
                                     unsigned char *ucDetectorMapIN,
                                     short int *siTimeGapIN,
                                     unsigned short int *usiBigCarLengthIN,
                                     unsigned short int *usiCarLengthIN,
                                     short int *siTransmitType,
                                     unsigned char *uc92VDLaneMapIN
                                  )
{
    try
    {

        FILE *Lane_rfile=NULL;
        char filename[36];
        sprintf( filename,"/cct/Data/SETTING/92VD.bin\0" );
        Lane_rfile = fopen(filename , "r"); //fopen return NULL if file not exist
        pthread_mutex_lock(&mutexDisk);
        if(Lane_rfile)
        {


            //OTMARKPRINTF  printf("Reading File 92VD Setting To Share Memory\n");

            fread( ucActuateTypeIN, sizeof( unsigned char ), 1, Lane_rfile );
            fread( ucOccupyTimeIN, sizeof( unsigned char ), 1, Lane_rfile );
            fread( ucSettingLaneCountIN, sizeof( unsigned char ), 1, Lane_rfile );
            fread( ucDetectorMapIN, sizeof( unsigned char ), 1, Lane_rfile );
            fread( siTimeGapIN, sizeof( short int ), 1, Lane_rfile );
            fread( usiBigCarLengthIN, sizeof( unsigned short int ), 1, Lane_rfile );
            fread( usiCarLengthIN, sizeof( unsigned short int ), 1, Lane_rfile );
            fread( siTransmitType, sizeof( short int ), 5, Lane_rfile );
            fread( uc92VDLaneMapIN, sizeof( unsigned char ), 1, Lane_rfile );
            fclose( Lane_rfile );
        }
        pthread_mutex_unlock(&mutexDisk);
    }
    catch(...) {}
}

//OTCombo

//---------------------------------------------------------------------------
bool STORAGE::vWrite92TCSettingFile( unsigned char ucSmem92TC_ControlStrategyIN,
                                     int i92TC_SignalLightStatus_5F0F_IntervalTimeIN,
                                     int i92TC_SignalStepStatus_5F03_IntervalTimeIN,
                                     int iRedCountVerSelectIN,
//OT Debug 0523
                                     bool bTC_ActuateTypeFunctionEnableIN,
                                     unsigned short int usiTC_CCT_In_LongTanu_ActuateType_PlanIDIN,
                                     unsigned char ucActuatePhaseExtendIN,
                                     unsigned short int usiActuateVDIDIN,
                                     unsigned char ucBootingDisplayRedcountIN,
                                     unsigned char ucTC_ActuateTypeByTODIN,
                                     bool bActuateautoswitchIN
                                   )
{
    try
    {

        FILE *_wfile=NULL;
        char filename[36];
        sprintf( filename,"/cct/Data/SETTING/92TC.bin\0" );
        _wfile = fopen(filename , "w"); //fopen return NULL if file not exist
        pthread_mutex_lock(&mutexDisk);
        if(_wfile)
        {

            //OTMARKPRINTF  printf("Writing File 92TC Setting To Share Memory\n");

            fwrite( &ucSmem92TC_ControlStrategyIN, sizeof( unsigned char ), 1, _wfile );
            fwrite( &i92TC_SignalLightStatus_5F0F_IntervalTimeIN, sizeof( int ), 1, _wfile );
            fwrite( &i92TC_SignalStepStatus_5F03_IntervalTimeIN, sizeof( int ), 1, _wfile );
            fwrite( &iRedCountVerSelectIN, sizeof( int ), 1, _wfile );
            fwrite( &bTC_ActuateTypeFunctionEnableIN, sizeof( bool ), 1, _wfile );
            fwrite( &usiTC_CCT_In_LongTanu_ActuateType_PlanIDIN, sizeof( unsigned short int ), 1, _wfile );
            fwrite( &ucActuatePhaseExtendIN, sizeof( unsigned char ), 1, _wfile );
            fwrite( &usiActuateVDIDIN, sizeof( unsigned short int ), 1, _wfile );
            fwrite( &ucBootingDisplayRedcountIN, sizeof( unsigned char ), 1, _wfile );
            fwrite( &ucTC_ActuateTypeByTODIN, sizeof( unsigned char ), 1, _wfile );
            fwrite( &bActuateautoswitchIN, sizeof( bool ), 1, _wfile );
            fclose( _wfile );
        }

        system("sync");
        pthread_mutex_unlock(&mutexDisk);
    }
    catch(...) {}
}

//---------------------------------------------------------------------------
bool STORAGE::vRead92TCSettingFile(  unsigned char *ucSmem92TC_ControlStrategyIN,
                                     int *i92TC_SignalLightStatus_5F0F_IntervalTimeIN,
                                     int *i92TC_SignalStepStatus_5F03_IntervalTimeIN,
                                     int *iRedCountVerSelectIN,
//OT Debug 0523
                                     bool *bTC_ActuateTypeFunctionEnableIN,
                                     unsigned short int *usiTC_CCT_In_LongTanu_ActuateType_PlanIDIN,
                                     unsigned char *ucActuatePhaseExtendIN,
                                     unsigned short int *usiActuateVDIDIN,
                                     unsigned char *ucBootingDisplayRedcountIN,
                                     unsigned char *ucTC_ActuateTypeByTODIN,
                                     bool *bActuateautoswitchIN
                                  )
{
    try
    {

        unsigned char ucTmp;

        FILE *_rfile=NULL;
        char filename[36];
        sprintf( filename,"/cct/Data/SETTING/92TC.bin\0" );
        _rfile = fopen(filename , "r"); //fopen return NULL if file not exist
        pthread_mutex_lock(&mutexDisk);
        if(_rfile)
        {

            printf("Reading File 92TC Setting To Share Memory\n");
//        printf("0:%d\n", feof(_rfile));

            fread( ucSmem92TC_ControlStrategyIN, sizeof( unsigned char ), 1, _rfile );
            /*
                    fread( &ucTmp, sizeof( unsigned char ), 1, _rfile );
                    if(!feof(_rfile)) {  //Not end line.
                      *ucSmem92TC_ControlStrategyIN = ucTmp;
                    }
            */

            fread( i92TC_SignalLightStatus_5F0F_IntervalTimeIN, sizeof( int ), 1, _rfile );
            fread( i92TC_SignalStepStatus_5F03_IntervalTimeIN, sizeof( int ), 1, _rfile );
            fread( iRedCountVerSelectIN, sizeof( int ), 1, _rfile );
            fread( bTC_ActuateTypeFunctionEnableIN, sizeof( bool ), 1, _rfile );
            fread( usiTC_CCT_In_LongTanu_ActuateType_PlanIDIN, sizeof( unsigned short int ), 1, _rfile );
            fread( ucActuatePhaseExtendIN, sizeof( unsigned char ), 1, _rfile );
            fread( usiActuateVDIDIN, sizeof( unsigned short int ), 1, _rfile );
            fread( bActuateautoswitchIN, sizeof( bool ), 1, _rfile );

//        fread( ucBootingDisplayRedcountIN, sizeof( unsigned char ), 1, _rfile );
            ucTmp = 2;
            fread( &ucTmp, sizeof( unsigned char ), 1, _rfile );
            if(!feof(_rfile))    //Not end line.
            {
                *ucBootingDisplayRedcountIN = ucTmp;
                printf("ucBootingDisplayRedcountIN in storage:%d\n", ucTmp);
            }
            else
            {
                printf("end suddenedly!\n");
            }

            fread( ucTC_ActuateTypeByTODIN, sizeof( unsigned char ), 1, _rfile );

            fclose( _rfile );
        }
        pthread_mutex_unlock(&mutexDisk);
    }
    catch(...) {}
}

//---------------------------------------------------------------------------
bool STORAGE::vWriteShareMEMALLFile(char *cTmp)
{
    try
    {

        struct tm* currenttime;
        time_t now = time(NULL);
        currenttime = localtime(&now);

        FILE *Lane_wfile=NULL;
        char cFileTmp[128];
        bzero(cFileTmp, 128);
        sprintf(cFileTmp, "/tmp/_RT%#02d%#02d-%#02d%#02d%#02d_SINGALRECORD.txt\0", currenttime->tm_mon+1, currenttime->tm_mday, currenttime->tm_hour, currenttime->tm_min, currenttime->tm_sec);


        Lane_wfile = fopen(cFileTmp , "w"); //fopen return NULL if file not exist
        pthread_mutex_lock(&mutexDisk);
        if(Lane_wfile)
        {


            //OTMARKPRINTF  printf("[MESSAGE] Saving Share mem data now.\n");

            fwrite( cTmp, sizeof( char ), 65535, Lane_wfile );
            fclose( Lane_wfile );
        }

        system("sync");
        pthread_mutex_unlock(&mutexDisk);
    }
    catch(...) {}
}

//---------------------------------------------------------------------------
bool STORAGE::vWriteMsgToFile(char *cTmp)
{
    try
    {

        struct tm* currenttime;
        time_t now = time(NULL);
        currenttime = localtime(&now);

        FILE *Lane_wfile=NULL;

        char cFileTmp[64]= {0};
        sprintf(cFileTmp, "/cct/Data/SETTING/");

        char dateTemp[32]= {0};
        sprintf(dateTemp, "%#04d%#02d%#02d", currenttime->tm_year+1900, currenttime->tm_mon+1, currenttime->tm_mday);

        strcat(cFileTmp,dateTemp);
        strcat(cFileTmp,"_changeLog.txt");

        char cTimeHeader[64]= {0};
        sprintf(cTimeHeader, "%#04d/%#02d/%#02d %#02d:%#02d:%#02d  \0", currenttime->tm_year+1900, currenttime->tm_mon+1, currenttime->tm_mday, currenttime->tm_hour, currenttime->tm_min, currenttime->tm_sec);

        unsigned short int length=0;
        char cTmpTmp[300]= {0};

        strcat(cTmpTmp,cTimeHeader);
        strcat(cTmpTmp,cTmp);

        for (int i=0; i<300; i++)
            if (cTmpTmp[i]=='\0')
            {
                length=i;
                break;
            }

        char enterCode[2];
        enterCode[0]=0x0D;
        enterCode[1]=0x0A;

        Lane_wfile = fopen(cFileTmp , "a+"); //fopen return NULL if file not exist

        pthread_mutex_lock(&mutexDisk);
        if(Lane_wfile)
        {

            //OTMARKPRINTF  printf("[MESSAGE] Write Change Log To DOM.\n");

            fwrite( cTmpTmp, length, 1, Lane_wfile );
            fwrite( enterCode, 2, 1, Lane_wfile );              //¼g¤Jenter
            fclose( Lane_wfile );
        }

        system("sync");
        pthread_mutex_unlock(&mutexDisk);
    }
    catch(...) {}
}

//OT20140304
//---------------------------------------------------------------------------
bool STORAGE::vWriteMsgToTmp(char *cTmp)
{
    try
    {

        struct tm* currenttime;
        time_t now = time(NULL);
        currenttime = localtime(&now);

        FILE *Lane_wfile=NULL;

        char cFileTmp[64]= {0};
        sprintf(cFileTmp, "/tmp/");

        char dateTemp[32]= {0};
        sprintf(dateTemp, "%#04d%#02d%#02d", currenttime->tm_year+1900, currenttime->tm_mon+1, currenttime->tm_mday);

        strcat(cFileTmp,dateTemp);
        strcat(cFileTmp,"_log.txt");

        char cTimeHeader[64]= {0};
        sprintf(cTimeHeader, "%#02d:%#02d:%#02d \0", currenttime->tm_hour, currenttime->tm_min, currenttime->tm_sec);

        unsigned short int length=0;
        char cTmpTmp[300]= {0};

        strcat(cTmpTmp,cTimeHeader);
        strcat(cTmpTmp,cTmp);

        for (int i=0; i<300; i++)
            if (cTmpTmp[i]=='\0')
            {
                length=i;
                break;
            }

        char enterCode[2];
        enterCode[0]=0x0D;
        enterCode[1]=0x0A;

        Lane_wfile = fopen(cFileTmp , "a+"); //fopen return NULL if file not exist

        pthread_mutex_lock(&mutexDisk);
        if(Lane_wfile)
        {

            //OTMARKPRINTF  printf("[MESSAGE] Write Change Log To DOM.\n");

            fwrite( cTmpTmp, length, 1, Lane_wfile );
            fwrite( enterCode, 2, 1, Lane_wfile );              //¼g¤Jenter
            fclose( Lane_wfile );
        }

        system("sync");
        pthread_mutex_unlock(&mutexDisk);
    }
    catch(...) {}
}

//---------------------------------------------------------------------------
bool STORAGE::vWriteLastAliveTimeStampFile(time_t RunSec)
{
    try
    {
        char filename[256];
        sprintf( filename, "/cct/Data/NoSSLastAliveTimeStamp.long\0");

        pthread_mutex_lock(&mutexDisk);
        fileD = fopen(filename, "w");
        if(fileD)
        {
            fwrite( &RunSec, sizeof(time_t), 1, fileD);                                 //µ¹§ä¥XSeqPtr¥Îªº
            fclose(fileD);
        }

        system("sync");
        pthread_mutex_unlock(&mutexDisk);

        //OTMARKPRINTF  printf("[Message] Save Time Stamp.\n");

        return true;
    }
    catch (...) {}
}
//---------------------------------------------------------------------------
bool STORAGE::vReadLastAliveTimeStampFile(time_t *RunSec)
{
    try
    {
        char filename[256];
        sprintf( filename, "/cct/Data/NoSSLastAliveTimeStamp.long\0");

        pthread_mutex_lock(&mutexDisk);
        fileD = fopen(filename, "r");
        if(fileD)
        {
            fread( RunSec, sizeof(time_t), 1, fileD);
            fclose(fileD);
        }
        pthread_mutex_unlock(&mutexDisk);

        return true;
    }
    catch (...) {}
}

//OT Debug 951121
//---------------------------------------------------------------------------
bool STORAGE::vReadLCNFromFile(  bool *bReadEnableLCN, unsigned int *uiReadLCN)
{
    try
    {

        bool bRetEnable =false;
        unsigned int uiRetLCN = 0;

        char cReadString[255];
        char cTMP[255];
        int iStrLen = 0;
        int iLine = 0;
        int iLcnLength = 0;

        FILE *_rfile=NULL;
        char filename[36];

        bzero(cReadString, sizeof(cReadString));
        bzero(cTMP, sizeof(cTMP));

        sprintf( filename,"/cct/Data/SETTING/LCN.txt\0" );
        _rfile = fopen(filename , "r"); //fopen return NULL if file not exist
        pthread_mutex_lock(&mutexDisk);

        if(_rfile)
        {
            // while(


            fread( &uiRetLCN,      sizeof( unsigned short int ), 1, _rfile );
//for RevAPP so cancel
            /* while(fscanf(_rfile, "%s", cReadString) != EOF) {
               iStrLen = strlen(cReadString);

               if(cReadString[0] == 'e' &&
                  cReadString[1] == 'n' &&
                  cReadString[2] == 'a' &&
                  cReadString[3] == 'b' &&
                  cReadString[4] == 'l' &&
                  cReadString[5] == 'e' &&
                  cReadString[6] == '='    )
                  {
                    cTMP[0] = cReadString[7];
                    uiRetLCN = atoi(cTMP);                                                //here, uiRetLCN is used for tmp.
                    if(uiRetLCN > 0) bRetEnable = true;
                    else bRetEnable = false;
               }

               if(cReadString[7] == 'L' &&
                  cReadString[8] == 'C' &&
                  cReadString[9] == 'N' &&
                  cReadString[10] == '='   )
                  {
                    if(iStrLen - 10 > 5) iLcnLength = 5;                                //when read string too much
                    else iLcnLength = iStrLen - 4;

                    for(int i = 0; i < iLcnLength; i++) {
                      cTMP[i] = cReadString[i+10];
                    }

                    uiRetLCN = atoi(cTMP);
               }

             }*/

            printf("[OTMsg] From file, bRetEnable is %d, LCN is %d\n", bRetEnable, uiRetLCN);
            fclose( _rfile );
        }
        else
        {
            bRetEnable = false;
            uiRetLCN = 0;
        }

        //*bReadEnableLCN = bRetEnable;

        *bReadEnableLCN = true;//20170618

        *uiReadLCN = uiRetLCN;

        pthread_mutex_unlock(&mutexDisk);
    }
    catch(...) {}
}
//------------jacky20140411-------------------------------------
bool STORAGE::vReadCompensationFromFile(bool *bReadEnablecycle, unsigned short int *cyclenum )
{
    try
    {
        bool bRetEnable =false;
        unsigned short int tmpcycle = 0;

        char cReadString[255];
        char cTMP[2];
//    int iStrLen = 0;

        FILE *_rfile=NULL;
        char filename[40];

        bzero(cReadString, sizeof(cReadString));
        bzero(cTMP, sizeof(cTMP));

        sprintf( filename,"/cct/Data/SETTING/compensationset.txt\0" );
        _rfile = fopen(filename , "r"); //fopen return NULL if file not exist
        pthread_mutex_lock(&mutexDisk);

        if(_rfile)
        {
            while(fscanf(_rfile, "%s", cReadString) != EOF)
            {
//      iStrLen = strlen(cReadString);

                if(cReadString[0] == 'e' &&
                        cReadString[1] == 'n' &&
                        cReadString[2] == 'a' &&
                        cReadString[3] == 'b' &&
                        cReadString[4] == 'l' &&
                        cReadString[5] == 'e' &&
                        cReadString[6] == '='    )
                {
                    cTMP[0] = cReadString[7];
                    tmpcycle = atoi(cTMP);                                                //here, uiRetLCN is used for tmp.
                    if(tmpcycle > 0) bRetEnable = true;
                    else bRetEnable = false;
                }

                if(cReadString[0] == 'c' &&
                        cReadString[1] == 'y' &&
                        cReadString[2] == 'c' &&
                        cReadString[3] == 'l' &&
                        cReadString[4] == 'e' &&
                        cReadString[5] == '=' )
                    cTMP[0] = cReadString[6];

                tmpcycle = atoi(cTMP);
            }

            printf("[OTMsg] From file, bRetEnable is %d, Compensation cycle is %d\n", bRetEnable, tmpcycle);
            fclose( _rfile );
        }
        else
        {
            bRetEnable = false;
            tmpcycle = 1;
        }

        *bReadEnablecycle = bRetEnable;
        *cyclenum = tmpcycle;

        pthread_mutex_unlock(&mutexDisk);
    }
    catch(...) {}
}
//------------jacky20150325-------------------------------------
bool STORAGE::vReadCarLearnFromFile(bool *bReadEnableCountdown, unsigned short int *Countdownnum )
{
    try
    {
        bool bRetEnable =false;
        unsigned short int CountSwitch = 0;

        char cReadString[255];
        char cTMP[2];
//    int iStrLen = 0;

        FILE *_rfile=NULL;
        char filename[40];

        bzero(cReadString, sizeof(cReadString));
        bzero(cTMP, sizeof(cTMP));

        sprintf( filename,"/cct/Data/SETTING/CarLearn.txt\0" );
        _rfile = fopen(filename , "r"); //fopen return NULL if file not exist
        pthread_mutex_lock(&mutexDisk);

        if(_rfile)
        {
            while(fscanf(_rfile, "%s", cReadString) != EOF)
            {
//      iStrLen = strlen(cReadString);

                if(cReadString[0] == 'e' &&
                        cReadString[1] == 'n' &&
                        cReadString[2] == 'a' &&
                        cReadString[3] == 'b' &&
                        cReadString[4] == 'l' &&
                        cReadString[5] == 'e' &&
                        cReadString[6] == '='    )
                {
                    cTMP[0] = cReadString[7];
                    CountSwitch = atoi(cTMP);                                                //here, uiRetLCN is used for tmp.
                    if(CountSwitch > 0) bRetEnable = true;
                    else bRetEnable = false;
                }

                if(cReadString[0] == 's' &&
                        cReadString[1] == 'w' &&
                        cReadString[2] == 'i' &&
                        cReadString[3] == 't' &&
                        cReadString[4] == 'c' &&
                        cReadString[5] == 'h' &&
                        cReadString[6] == '=' )
                    cTMP[0] = cReadString[7];

                CountSwitch = atoi(cTMP);
            }

            printf("[OTMsg] From file, bRetEnable is %d, CarLearn is %d\n", bRetEnable, CountSwitch);
            fclose( _rfile );
        }
        else
        {
            bRetEnable = false;
            CountSwitch = 0;
        }

        *bReadEnableCountdown = bRetEnable;
        *Countdownnum = CountSwitch;

        pthread_mutex_unlock(&mutexDisk);
    }
    catch(...) {}
}
//------------jacky20150325-------------------------------------
bool STORAGE::vReadPedLearnFromFile(bool *bReadEnableCountdown, unsigned short int *Countdownnum )
{
    try
    {
        bool bRetEnable =false;
        unsigned short int CountSwitch = 0;

        char cReadString[255];
        char cTMP[2];
//    int iStrLen = 0;

        FILE *_rfile=NULL;
        char filename[40];

        bzero(cReadString, sizeof(cReadString));
        bzero(cTMP, sizeof(cTMP));

        sprintf( filename,"/cct/Data/SETTING/PedLearn.txt\0" );
        _rfile = fopen(filename , "r"); //fopen return NULL if file not exist
        pthread_mutex_lock(&mutexDisk);

        if(_rfile)
        {
            while(fscanf(_rfile, "%s", cReadString) != EOF)
            {
//      iStrLen = strlen(cReadString);

                if(cReadString[0] == 'e' &&
                        cReadString[1] == 'n' &&
                        cReadString[2] == 'a' &&
                        cReadString[3] == 'b' &&
                        cReadString[4] == 'l' &&
                        cReadString[5] == 'e' &&
                        cReadString[6] == '='    )
                {
                    cTMP[0] = cReadString[7];
                    CountSwitch = atoi(cTMP);                                                //here, uiRetLCN is used for tmp.
                    if(CountSwitch > 0) bRetEnable = true;
                    else bRetEnable = false;
                }

                if(cReadString[0] == 's' &&
                        cReadString[1] == 'w' &&
                        cReadString[2] == 'i' &&
                        cReadString[3] == 't' &&
                        cReadString[4] == 'c' &&
                        cReadString[5] == 'h' &&
                        cReadString[6] == '=' )
                    cTMP[0] = cReadString[7];

                CountSwitch = atoi(cTMP);
            }

            printf("[OTMsg] From file, bRetEnable is %d, PedLearn is %d\n", bRetEnable, CountSwitch);
            fclose( _rfile );
        }
        else
        {
            bRetEnable = false;
            CountSwitch = 0;
        }

        *bReadEnableCountdown = bRetEnable;
        *Countdownnum = CountSwitch;

        pthread_mutex_unlock(&mutexDisk);
    }
    catch(...) {}
}
//------------jacky20150330-------------------------------------
bool STORAGE::vReadCarCountdownFromFile(bool *bReadEnableCarCountdown, unsigned short int *CarCountdownnum )
{
    try
    {
        bool bRetEnable =false;
        unsigned short int CarCountProperty = 0;

        char cReadString[255];
        char cTMP[2];

        FILE *_rfile=NULL;
        char filename[40];

        bzero(cReadString, sizeof(cReadString));
        bzero(cTMP, sizeof(cTMP));

        sprintf( filename,"/cct/Data/SETTING/CarCountdownProperty.txt\0" );
        _rfile = fopen(filename , "r"); //fopen return NULL if file not exist
        pthread_mutex_lock(&mutexDisk);

        if(_rfile)
        {
            while(fscanf(_rfile, "%s", cReadString) != EOF)
            {
//      iStrLen = strlen(cReadString);

                if(cReadString[0] == 'e' &&
                        cReadString[1] == 'n' &&
                        cReadString[2] == 'a' &&
                        cReadString[3] == 'b' &&
                        cReadString[4] == 'l' &&
                        cReadString[5] == 'e' &&
                        cReadString[6] == '='    )
                {
                    cTMP[0] = cReadString[7];
                    CarCountProperty = atoi(cTMP);                                                //here, uiRetLCN is used for tmp.
                    if(CarCountProperty > 0) bRetEnable = true;
                    else bRetEnable = false;
                }

                if(cReadString[0] == 'P' &&
                        cReadString[1] == 'r' &&
                        cReadString[2] == 'o' &&
                        cReadString[3] == 'p' &&
                        cReadString[4] == 'e' &&
                        cReadString[5] == 'r' &&
                        cReadString[6] == 't' &&
                        cReadString[7] == 'y' &&
                        cReadString[8] == '=' )
                    cTMP[0] = cReadString[9];

                CarCountProperty = atoi(cTMP);
            }

            printf("[OTMsg] From file, bRetEnable is %d, CarCountdown is %d\n", bRetEnable, CarCountProperty);
            fclose( _rfile );
        }
        else
        {
            bRetEnable = false;
            CarCountProperty = 0;
        }

        *bReadEnableCarCountdown = bRetEnable;
        *CarCountdownnum = CarCountProperty;

        pthread_mutex_unlock(&mutexDisk);
    }
    catch(...) {}
}
//------------jacky20150330-------------------------------------
bool STORAGE::vReadPedCountdownFromFile(bool *bReadEnablePedCountdown, unsigned short int *PedCountdownnum )
{
    try
    {
        bool bRetEnable =false;
        unsigned short int PedCountProperty = 0;

        char cReadString[255];
        char cTMP[2];

        FILE *_rfile=NULL;
        char filename[40];

        bzero(cReadString, sizeof(cReadString));
        bzero(cTMP, sizeof(cTMP));

        sprintf( filename,"/cct/Data/SETTING/PedCountdownProperty.txt\0" );
        _rfile = fopen(filename , "r"); //fopen return NULL if file not exist
        pthread_mutex_lock(&mutexDisk);

        if(_rfile)
        {
            while(fscanf(_rfile, "%s", cReadString) != EOF)
            {
//      iStrLen = strlen(cReadString);

                if(cReadString[0] == 'e' &&
                        cReadString[1] == 'n' &&
                        cReadString[2] == 'a' &&
                        cReadString[3] == 'b' &&
                        cReadString[4] == 'l' &&
                        cReadString[5] == 'e' &&
                        cReadString[6] == '='    )
                {
                    cTMP[0] = cReadString[7];
                    PedCountProperty = atoi(cTMP);                                                //here, uiRetLCN is used for tmp.
                    if(PedCountProperty > 0) bRetEnable = true;
                    else bRetEnable = false;
                }

                if(cReadString[0] == 'P' &&
                        cReadString[1] == 'r' &&
                        cReadString[2] == 'o' &&
                        cReadString[3] == 'p' &&
                        cReadString[4] == 'e' &&
                        cReadString[5] == 'r' &&
                        cReadString[6] == 't' &&
                        cReadString[7] == 'y' &&
                        cReadString[8] == '=' )
                    cTMP[0] = cReadString[9];

                PedCountProperty = atoi(cTMP);
            }

            printf("[OTMsg] From file, bRetEnable is %d, PedCountdown is %d\n", bRetEnable, PedCountProperty);
            fclose( _rfile );
        }
        else
        {
            bRetEnable = false;
            PedCountProperty = 0;
        }

        *bReadEnablePedCountdown = bRetEnable;
        *PedCountdownnum = PedCountProperty;

        pthread_mutex_unlock(&mutexDisk);
    }
    catch(...) {}
}
//------------jacky20160223-------------------------------------
bool STORAGE::vReadPedSWDevCountFromFile(bool *bReadEnableSWCount, unsigned short int *SWCount )
{
    try
    {
        bool bRetEnable =false;
        unsigned short int SWDevCount = 0;

        char cReadString[255];
        char cTMP[2];
//    int iStrLen = 0;

        FILE *_rfile=NULL;
        char filename[40];

        bzero(cReadString, sizeof(cReadString));
        bzero(cTMP, sizeof(cTMP));

        sprintf( filename,"/cct/Data/SETTING/PedSwitchDevCount.txt\0" );
        _rfile = fopen(filename , "r"); //fopen return NULL if file not exist
        pthread_mutex_lock(&mutexDisk);

        if(_rfile)
        {
            while(fscanf(_rfile, "%s", cReadString) != EOF)
            {
//      iStrLen = strlen(cReadString);

                if(cReadString[0] == 'e' &&
                        cReadString[1] == 'n' &&
                        cReadString[2] == 'a' &&
                        cReadString[3] == 'b' &&
                        cReadString[4] == 'l' &&
                        cReadString[5] == 'e' &&
                        cReadString[6] == '='    )
                {
                    cTMP[0] = cReadString[7];
                    SWDevCount = atoi(cTMP);                                                //here, uiRetLCN is used for tmp.
                    if(SWDevCount > 0) bRetEnable = true;
                    else bRetEnable = false;
                }

                if(cReadString[0] == 'D' &&
                        cReadString[1] == 'e' &&
                        cReadString[2] == 'v' &&
                        cReadString[3] == 'C' &&
                        cReadString[4] == 'o' &&
                        cReadString[5] == 'u' &&
                        cReadString[6] == 'n' &&
                        cReadString[7] == 't' &&
                        cReadString[8] == '=')
                {
                    cTMP[0] = cReadString[9];
                    cTMP[1] = cReadString[10];
                }
                SWDevCount = atoi(cTMP);
            }

            printf("[OTMsg] From file, bRetEnable is %d, Ped SW DevCount is %d\n", bRetEnable, SWDevCount);
            fclose( _rfile );
        }
        else
        {
            bRetEnable = false;
            SWDevCount = 0;
        }

        *bReadEnableSWCount = bRetEnable;
        *SWCount = SWDevCount;

        pthread_mutex_unlock(&mutexDisk);
    }
    catch(...) {}
}
//---------------------------------------------------------------------------
bool STORAGE::vWriteLast92TC_5F15Time(time_t RunSec)
{
    try
    {
        char filename[256];
        sprintf( filename, "/cct/Data/Last92TC_5F15TimeStamp.long\0");

        pthread_mutex_lock(&mutexDisk);
        fileD = fopen(filename, "w");
        if(fileD)
        {
            fwrite( &RunSec, sizeof(time_t), 1, fileD);
            fclose(fileD);
        }

        system("sync");
        pthread_mutex_unlock(&mutexDisk);

        //OTMARKPRINTF  printf("[Message] Save Time Stamp.\n");

        return true;
    }
    catch (...) {}
}
//---------------------------------------------------------------------------
bool STORAGE::vReadLast92TC_5F15Time(time_t *RunSec)
{
    try
    {
        char filename[256];
        sprintf( filename, "/cct/Data/Last92TC_5F15TimeStamp.long\0");

        pthread_mutex_lock(&mutexDisk);
        fileD = fopen(filename, "r");
        if(fileD)
        {
            fread( RunSec, sizeof(time_t), 1, fileD);
            fclose(fileD);
        }
        pthread_mutex_unlock(&mutexDisk);

        return true;
    }
    catch (...) {}
}

//---------------------------------------------------------------------------
bool STORAGE::vWriteChainSettFile(unsigned char ucTC92_5F31ManualIN,
                                  unsigned char ucTC92_5F31TODIN,
                                  unsigned char ucTC92_5F31StartSubPhaseIdIN,
                                  unsigned char ucTC92_5F31EndSubPhaseIdIN,
                                  unsigned char ucTC92_5F32StartSubPhaseIdIN,
                                  unsigned char ucTC92_5F32EndSubPhaseIdIN,
                                  unsigned char ucTC_MotherChainStartStepIdIN,
                                  unsigned char ucTC_MotherChainEndStepIdIN,
                                  short int *siTC92_5F33StartOffsetIN,
                                  short int *siTC92_5F33EndOffsetIN
                                 )
{
    try
    {

        FILE *_wfile=NULL;
        char filename[36];
        sprintf( filename,"/cct/Data/SETTING/CHAINSET.bin\0" );
        _wfile = fopen(filename , "w"); //fopen return NULL if file not exist
        pthread_mutex_lock(&mutexDisk);
        if(_wfile)
        {

            fwrite( &ucTC92_5F31ManualIN, sizeof( unsigned char ), 1, _wfile );
            fwrite( &ucTC92_5F31TODIN, sizeof( unsigned char ), 1, _wfile );
            fwrite( &ucTC92_5F31StartSubPhaseIdIN, sizeof( unsigned char ), 1, _wfile );
            fwrite( &ucTC92_5F31EndSubPhaseIdIN, sizeof( unsigned char ), 1, _wfile );
            fwrite( &ucTC92_5F32StartSubPhaseIdIN, sizeof( unsigned char ), 1, _wfile );
            fwrite( &ucTC92_5F32EndSubPhaseIdIN, sizeof( unsigned char ), 1, _wfile );
            fwrite( &ucTC_MotherChainStartStepIdIN, sizeof( unsigned char ), 1, _wfile );
            fwrite( &ucTC_MotherChainEndStepIdIN, sizeof( unsigned char ), 1, _wfile );
            fwrite( siTC92_5F33StartOffsetIN, sizeof(short int), 64, _wfile );
            fwrite( siTC92_5F33EndOffsetIN, sizeof(short int), 64, _wfile );
            fclose( _wfile );
        }

        system("sync");
        pthread_mutex_unlock(&mutexDisk);
    }
    catch(...) {}
}

//---------------------------------------------------------------------------
bool STORAGE::vReadChainSettFile( unsigned char *ucTC92_5F31ManualIN,
                                  unsigned char *ucTC92_5F31TODIN,
                                  unsigned char *ucTC92_5F31StartSubPhaseIdIN,
                                  unsigned char *ucTC92_5F31EndSubPhaseIdIN,
                                  unsigned char *ucTC92_5F32StartSubPhaseIdIN,
                                  unsigned char *ucTC92_5F32EndSubPhaseIdIN,
                                  unsigned char *ucTC_MotherChainStartStepIdIN,
                                  unsigned char *ucTC_MotherChainEndStepIdIN,
                                  short int *siTC92_5F33StartOffsetIN,
                                  short int *siTC92_5F33EndOffsetIN
                                )
{
    try
    {
        FILE *_rfile=NULL;
        char filename[36];
        sprintf( filename,"/cct/Data/SETTING/CHAINSET.bin\0" );
        _rfile = fopen(filename, "r"); //fopen return NULL if file not exist
        pthread_mutex_lock(&mutexDisk);
        if(_rfile)
        {

            fread( ucTC92_5F31ManualIN, sizeof( unsigned char ), 1, _rfile );
            fread( ucTC92_5F31TODIN, sizeof( unsigned char ), 1, _rfile );
            fread( ucTC92_5F31StartSubPhaseIdIN, sizeof( unsigned char ), 1, _rfile );
            fread( ucTC92_5F31EndSubPhaseIdIN, sizeof( unsigned char ), 1, _rfile );
            fread( ucTC92_5F32StartSubPhaseIdIN, sizeof( unsigned char ), 1, _rfile );
            fread( ucTC92_5F32EndSubPhaseIdIN, sizeof( unsigned char ), 1, _rfile );
            fread( ucTC_MotherChainStartStepIdIN, sizeof( unsigned char ), 1, _rfile );
            fread( ucTC_MotherChainEndStepIdIN, sizeof( unsigned char ), 1, _rfile );
            fread( siTC92_5F33StartOffsetIN, sizeof(short int), 64, _rfile );
            fread( siTC92_5F33EndOffsetIN, sizeof(short int), 64, _rfile );
            fclose( _rfile );
        }
        pthread_mutex_unlock(&mutexDisk);
        return true;
    }
    catch(...) {}
}

//---------------------------------------------------------------------------
bool STORAGE::vWriteCom2TypeFromFile(int iCom2TypeIn)
{
    try
    {
        FILE *_wfile=NULL;

        char cFileTmp[64]= {0};
        char cCom2TypeTmp;
        sprintf(cFileTmp, "/cct/Data/SETTING/ttyS1Type.conf\0");

        unsigned short int length=0;
        char cTmpTmp[300]= {0};

        char enterCode[2];
//  enterCode[0]=0x0D;
//  enterCode[1]=0x0A;

        if(iCom2TypeIn == 0)
        {
            cCom2TypeTmp = '0';
        }
        if(iCom2TypeIn == 1)
        {
            cCom2TypeTmp = '1';
        }
        if(iCom2TypeIn == 3)
        {
            cCom2TypeTmp = '3';
        }
        if(iCom2TypeIn == 4)
        {
            cCom2TypeTmp = '4';
        }
        if(iCom2TypeIn == 5)
        {
            cCom2TypeTmp = '5';
        }
        if(iCom2TypeIn == 6)
        {
            cCom2TypeTmp = '6';    //add Arwen 行人按鈕
        }
        if(iCom2TypeIn == 7)
        {
            cCom2TypeTmp = '7';    //add Arwen 紅燈行人
        }
        if(iCom2TypeIn == 8)
        {
            cCom2TypeTmp = '8';
        }

        sprintf(cTmpTmp, "ttyS1=%c\n", cCom2TypeTmp);

        length = strlen(cTmpTmp);


        _wfile = fopen(cFileTmp , "w"); //fopen return NULL if file not exist

        pthread_mutex_lock(&mutexDisk);
        if(_wfile)
        {

            printf("[MESSAGE] Write Com2 Type To DOM.\n");

            fwrite( cTmpTmp, length, 1, _wfile );
//        fwrite( enterCode, 2, 1, _wfile );              //¼g¤Jenter
            fclose( _wfile );
        }

        system("sync");
        pthread_mutex_unlock(&mutexDisk);

    }
    catch(...) {}
}

//---------------------------------------------------------------------------
bool STORAGE::vReadCom2TypeFromFile(int *iCom2TypeIn)
{
    try
    {

        /*
          Com2IsGPSPort 0
          Com2IsTesterPort 1
          Com2IsPassingPort 3
          Com2IsRedCount 4
          Com2IsTainanPeopleLight 5
        */
        int iCom2Type = 0;

        char cReadString[16];
        char cTMP[16];
        int iStrLen = 0;
        int iLine = 0;
        int iLcnLength = 0;

        FILE *_rfile=NULL;
        char filename[36];

        bzero(cReadString, sizeof(cReadString));
        bzero(cTMP, sizeof(cTMP));

        sprintf( filename,"/cct/Data/SETTING/ttyS1Type.conf\0" );
        _rfile = fopen(filename , "r"); //fopen return NULL if file not exist
        pthread_mutex_lock(&mutexDisk);

        if(_rfile)
        {
            while(fscanf(_rfile, "%s", cReadString) != EOF)
            {
                iStrLen = strlen(cReadString);

                if(cReadString[0] == 't' &&
                        cReadString[1] == 't' &&
                        cReadString[2] == 'y' &&
                        cReadString[3] == 'S' &&
                        cReadString[4] == '1' &&
                        cReadString[5] == '='  )
                {
                    cTMP[0] = cReadString[6];
                    iCom2Type = atoi(cTMP);
                }

            }

            printf("[OTMsg] From file, iCom2Type is %d\n", iCom2Type);
            fclose( _rfile );
        }
        else
        {
            printf("[OTMsg] no file ttyS1Type\n");
        }

        *iCom2TypeIn = iCom2Type;
        pthread_mutex_unlock(&mutexDisk);

    }
    catch(...) {}
}


//---------------------------------------------------------------------------
bool STORAGE::vWriteMotherBoardTypeFromFile(unsigned char ucMBTypeIn)
{
    try
    {
        FILE *_wfile=NULL;

        char cFileTmp[64]= {0};
        unsigned char ucMBType;
        sprintf(cFileTmp, "/cct/Data/SETTING/MBType.conf\0");

        unsigned short int length=0;
        char cTmpTmp[300]= {0};

        char enterCode[2];
//  enterCode[0]=0x0D;
//  enterCode[1]=0x0A;

        if(ucMBTypeIn == 0)
        {
            ucMBType = '0';
        }
        if(ucMBTypeIn == 1)
        {
            ucMBType = '1';
        }
        if(ucMBTypeIn == 2)
        {
            ucMBType = '2';
        }
        if(ucMBTypeIn == 3)
        {
            ucMBType = '3';
        }

        sprintf(cTmpTmp, "MBType=%c\n", ucMBType);

        length = strlen(cTmpTmp);


        _wfile = fopen(cFileTmp , "w"); //fopen return NULL if file not exist

        pthread_mutex_lock(&mutexDisk);
        if(_wfile)
        {

            printf("[MESSAGE] Write MotherBoard Type To DOM.\n");

            fwrite( cTmpTmp, length, 1, _wfile );
            fclose( _wfile );
        }
        system("sync");
        pthread_mutex_unlock(&mutexDisk);


    }
    catch(...) {}
}

//---------------------------------------------------------------------------
bool STORAGE::vReadMotherBoardTypeFromFile(unsigned char *ucMBTypeIn)
{
    try
    {

        /*
          Com2IsGPSPort 0
          Com2IsTesterPort 1
          Com2IsPassingPort 3
          Com2IsRedCount 4
          Com2IsTainanPeopleLight 5
        */
        unsigned char ucMBType = 0;

        char cReadString[16];
        char cTMP[16];
        int iStrLen = 0;
        int iLine = 0;
        int iLcnLength = 0;

        FILE *_rfile=NULL;
        char filename[36];

        bzero(filename, sizeof(filename));
        bzero(cReadString, sizeof(cReadString));
        bzero(cTMP, sizeof(cTMP));

        sprintf( filename,"/cct/Data/SETTING/MBType.conf\0" );
        _rfile = fopen(filename , "r"); //fopen return NULL if file not exist
        pthread_mutex_lock(&mutexDisk);

        if(_rfile)
        {
            printf("open MBType.conf OK\n");
            while(fscanf(_rfile, "%s", cReadString) != EOF)
            {
                iStrLen = strlen(cReadString);

                if(cReadString[0] == 'M' &&
                        cReadString[1] == 'B' &&
                        cReadString[2] == 'T' &&
                        cReadString[3] == 'y' &&
                        cReadString[4] == 'p' &&
                        cReadString[5] == 'e' &&
                        cReadString[6] == '='  )
                {
                    cTMP[0] = cReadString[7];
                    ucMBType = atoi(cTMP);
                    printf("MB cTMP=%c, ucMBType=%d\n", cTMP, ucMBType);
                }

            }

            printf("[OTMsg] From file, ucMBType is %d\n", ucMBType);
            fclose( _rfile );
        }
        else
        {
            printf("[OTMsg] no file MBType.conf\n");
        }

        *ucMBTypeIn = ucMBType;
        pthread_mutex_unlock(&mutexDisk);

    }
    catch(...) {}
}


bool STORAGE::WriteLCNby_RevAPP(int LCN)
{
    try
    {

        unsigned int uiRetLCN = 0;

        FILE *_rfile=NULL;
        char filename[36];

        sprintf( filename,"/cct/Data/SETTING/LCN.txt\0" );
        _rfile = fopen(filename , "w"); //fopen return NULL if file not exist
        pthread_mutex_lock(&mutexDisk);
        if(_rfile)
        {
            fwrite( &LCN, sizeof(int), 1, _rfile);
            printf("%d\n",LCN);
        }
        else printf("write LCN error\n");
        fclose( _rfile );
        pthread_mutex_unlock(&mutexDisk);

    }



    catch(...) {}


}
bool STORAGE::WriteNetworkSetConfirm_RevAPP(MESSAGEOK messgein)
{
    try
    {

        char cRunString[256];
        int localIp1_[6];//localIp[5]=localport
        int distIp0_[6];//distIp0_[5]=distIp0_port
        int distIp1_[6];//distIp1_[5]=distIp1_port
        int netmask[5];
        int gateway[5];
        int LCN;
        for(int i=1; i<5; i++)
        {
            localIp1_[i]=messgein.packet[1+i];
            distIp0_[i]=messgein.packet[7+i];
            distIp1_[i]=messgein.packet[13+i];

        }
        localIp1_[5]=messgein.packet[6]*256+messgein.packet[7];
        distIp0_[5]=messgein.packet[12]*256+messgein.packet[13];
        distIp1_[5]=messgein.packet[18]*256+messgein.packet[19];


        for(int i=1; i<5; i++)
        {
            netmask[i]=messgein.packet[19+i];
            gateway[i]=messgein.packet[23+i];
        }
        for(int i=1; i<6; i++)
        {
            smem.SetLocalIP1(i,localIp1_[i]);
            smem.SetdistIp0(i,distIp0_[i]);
            smem.SetDistIP(i,distIp1_[i]);
        }
        for(int i=1; i<5; i++)
        {
            smem.SetNetmask(i,netmask[i]);
            smem.SetGateway(i,gateway[i]);
        }

        LCN=messgein.packet[28]*256+messgein.packet[29];
        WriteLCNby_RevAPP(LCN);

          printf("****receive network setting is =***\n");
        for(int i=1; i<6; i++)
            printf("%d ",localIp1_[i]);
        printf("\n");
        for(int i=1; i<6; i++)
            printf("%d ",distIp0_[i]);

        printf("\n");
        for(int i=1; i<6; i++)
            printf("%d ",distIp0_[i]);
        printf("\n");
        for(int i=1; i<5; i++)
            printf("%d ",netmask[i]);
        printf("\n");
        for(int i=1; i<5; i++)
            printf("%d ",gateway[i]);
        printf("\n");
        printf("LCN=%d\n",LCN);




//autorun    stc.Lock_to_Set_Control_Strategy(STRATEGY_FLASH);                           //OTFix 940804

        system("rm -rf /bin/quickNetwork2");
        system("rm -rf /cct/quickNetwork2");
        bzero(cRunString, sizeof(cRunString));
        sprintf(cRunString, "cp /bin/spaceSH /bin/quickNetwork2");
        sprintf(cRunString, "cp /bin/spaceSH /cct/quickNetwork2");
        system(cRunString);
        bzero(cRunString, sizeof(cRunString));
        sprintf(cRunString, "echo /sbin/ifconfig eth0:3 %d.%d.%d.%d netmask %d.%d.%d.%d >> /bin/quickNetwork2", localIp1_[1], localIp1_[2], localIp1_[3], localIp1_[4], netmask[1], netmask[2], netmask[3], netmask[4]);
        system(cRunString);
        bzero(cRunString, sizeof(cRunString));
        sprintf(cRunString, "echo /sbin/route add default gw %d.%d.%d.%d >> /bin/quickNetwork2", gateway[1], gateway[2], gateway[3], gateway[4]);
        system(cRunString);
        bzero(cRunString, sizeof(cRunString));
        sprintf(cRunString, "chmod +x /bin/quickNetwork2");
        system(cRunString);


        system("sync");
        system("sync");
        //  system("reboot");

    }
    catch (...) {}
}

/*
        //---------------------------------------------------------------------------
        bool SetLocalIP1(int,int);
        bool SetdistIp0(int,int);
        bool SetDistIP(int,int);
        bool SetNetmask(int, int);
        bool SetGateway(int, int);
        int GetLocalIP1(int);
        int GetdistIp0(int);
        int GetDistIP(int);
        int GetNetmask(int);
        int GetGateway(int);
*/
bool STORAGE::dispalyLCN_NetworkIP_to_RevAPP()
{
    try
    {
        BYTE data[25+4];
        data[0]=0x18;
        for(int i=1; i<5; i++)
        {
            data[i]=smem.GetLocalIP1(i);
            data[i+6]=smem.GetdistIp0(i);
            data[i+12]=smem.GetDistIP(i);
        }

        data[5]=(smem.GetLocalIP1(5)>>8) & 0x00ff;
        data[6]=smem.GetLocalIP1(5)& 0x00ff;
        data[11]=(smem.GetdistIp0(5)>>8) & 0x00ff;
        data[12]=smem.GetdistIp0(5)& 0x00ff;
        data[17]=(smem.GetDistIP(5)>>8) & 0x00ff;
        data[18]=smem.GetDistIP(5)& 0x00ff;





        for(int i=1; i<5; i++)
        {
            data[i+18]=smem.GetNetmask(i);
            data[i+22]=smem.GetGateway(i);
        }
        data[27]=(smem.GetAddress()>>8)& 0x00ff;//LCN
        data[28]=smem.GetAddress()& 0x00ff;
        writeJob.WritePhysicalOut(data,29,revAPP);




        /*for print information*/
        printf("network setting is =\n");
        for(int i=1; i<6; i++)
            printf("%d ",smem.GetLocalIP1(i));
        printf("\n");
        for(int i=1; i<6; i++)
            printf("%d ",smem.GetdistIp0(i));

        printf("\n");
        for(int i=1; i<6; i++)
            printf("%d ",smem.GetDistIP(i));
        printf("\n");
        for(int i=1; i<5; i++)
            printf("%d ",smem.GetNetmask(i));
        printf("\n");
        for(int i=1; i<5; i++)
            printf("%d ",smem.GetGateway(i));
        printf("\n");
        printf("LCN=%d\n",smem.GetAddress());
        return true;



    }
    catch(...) {}
}



