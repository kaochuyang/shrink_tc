#include "CRTMSInformation.h"

//CRTMSInformation rtms_info;
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
CRTMSInformation &CRTMSInformation::operator=(const CRTMSInformation &rtms_info) // Remember copy assignment operator should never be inlined
{
  if(this!=&rtms_info){
    info = rtms_info.info;
    switch(info){
      case TARGET:
        target = rtms_info.target;
        break;

      case VOLLONG:
        for(int i=0;i<8;i++){
          vollong[i] =   rtms_info.vollong[i];
        }
        voltage =   rtms_info.voltage;
        break;

      case VOLUME:
        for(int i=0;i<8;i++){
          vol[i] =   rtms_info.vol[i];
        }
        uptime = rtms_info.uptime;
        break;

      case OCCUPAT:
        for(int i=0;i<8;i++){
          oc[i] =   rtms_info.oc[i];
        }
        id        = rtms_info.id;
        break;

      case SSPEED:
        for(int i=0;i<8;i++){
          sp[i] =   rtms_info.sp[i];
        }
        avgspeed  =     rtms_info.avgspeed;
        dirin     =     rtms_info.dirin;
        health    =     rtms_info.health;
        break;
    }
  }//if(this!=rtms_info)
  return *this;
}
//---------------------------------------------------------------------------
#include <stdio.h>
void CRTMSInformation::display_status(void)
{
//OTMARKPRINTF  printf("\n\n============= The traffic status of last period =============");
//OTMARKPRINTF  printf("\n  Detector ID: %d   Detector Voltage: %d   Uptime: %d", id, voltage, uptime);
//OTMARKPRINTF  printf("\n  Average Speed: %d",avgspeed);
//OTMARKPRINTF  printf("\n  Speed:");
    for(int i=0;i<8;i++) printf(" %d",sp[i]);
//OTMARKPRINTF  printf("\n  Volume of Long Vehicle:  ");
    for(int i=0;i<8;i++) printf(" %d",vollong[i]);
//OTMARKPRINTF  printf("\n  Volume of Normal Vehilce:");
    for(int i=0;i<8;i++) printf(" %d",vol[i]);
//OTMARKPRINTF  printf("\n  Occupancy:");
    for(int i=0;i<8;i++) printf(" %d",oc[i]);
//OTMARKPRINTF  printf("\n=============================================================\n\n");
}
//---------------------------------------------------------------------------

