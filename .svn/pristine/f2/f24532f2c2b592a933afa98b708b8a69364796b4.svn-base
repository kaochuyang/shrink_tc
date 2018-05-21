#include "CRTMSDecoder.h"
#include <string.h>
#include <stdio.h>

//---------------------------------------------------
CRTMSDecoder::CRTMSDecoder(void)
{
}
//---------------------------------------------------
CRTMSDecoder::~CRTMSDecoder()
{
}
//------------------------------------------------------------------------
void CRTMSDecoder::ParseAndCheck(const SBuffer &buffer_COM, CPacketCluster &packet_c_COM)
{
try
{
  if( packet_c_COM.packet[packet_c_COM.last_packet].status==true ) // should be redundant, but for error avoidance
  {  packet_c_COM.last_packet++;
     packet_c_COM.packet[packet_c_COM.last_packet].length = 0;
     packet_c_COM.packet[packet_c_COM.last_packet].status = false;
  }

  int &j=packet_c_COM.last_packet;  // j should be '0' at the beginning
  unsigned char qualin;
  int lenin;
  int len;

  // Parse buffer to packet and check the packet
  for(unsigned int i=0; i<buffer_COM.length;i++)
  {
      switch(packet_c_COM.packet[j].length){
        case(0):
          packet_c_COM.packet[j].status=false; //Never forget setting it to false first!!
          if(buffer_COM.block[i]==HEADER){
            packet_c_COM.packet[j].block[packet_c_COM.packet[j].length] = buffer_COM.block[i];
            packet_c_COM.packet[j].length++;
      	  }
      	  break;

      	default:
          packet_c_COM.packet[j].block[packet_c_COM.packet[j].length] = buffer_COM.block[i];
          packet_c_COM.packet[j].length++;

          if(packet_c_COM.packet[j].length<4) break;

          qualin = packet_c_COM.packet[j].block[1];
          lenin = packet_c_COM.packet[j].block[2];
          len = packet_c_COM.packet[j].length;
          switch(qualin){
           case TICKCOUNT:
           case EUSC_SET:
           case EUSC_RESET:
           case EUSC_RETURN1:
           case EUSC_RETURN2:
           case EUSC_RETURN3:
           case EUSC_RETURN4:
           case EUSC_RETURN5:
           case EUSC_RETURN6:
           case EUSC_RETURN7:
           case EUSC_RETURN8:
           case EUSC_RETURNC:
             if(len == lenin+6) {
               if(checkCRC(packet_c_COM.packet[j])) j++;
               packet_c_COM.packet[j].length=0;
               packet_c_COM.packet[j].status=false;
             }
             break;

           default:
             if(len == lenin+4) {
               if(checkCKS(packet_c_COM.packet[j])) j++;
               packet_c_COM.packet[j].length=0;
               packet_c_COM.packet[j].status=false;
             }
             break;

         } // end switch(qualin)
      } // end switch(packet.position)
  } // end for(buffer.length)
}
catch(...){}
}
//--------------------------------------------------------------------------
void CRTMSDecoder::Decode(const SPacket &packet, CRTMSInformation &rtms_info)
{
  rtms_info.info = packet.block[1];
  switch(rtms_info.info){
      case TARGET:{
          unsigned char temp_packet[4];
          for(short int i=0;i<4;i++) temp_packet[i]=packet.block[3+i];
          rtms_info.target.SetTarget(temp_packet);
        }
        break;

      case VOLLONG:
        for(int i=0;i<8;i++)
          rtms_info.vollong[i] = packet.block[3+i];
        rtms_info.voltage = packet.block[11];
        break;

      case VOLUME:
        for(int i=0;i<8;i++)
          rtms_info.vol[i] = packet.block[3+i];
        rtms_info.uptime = packet.block[11];
        break;

      case OCCUPAT:
        if(packet.block[2]==9){
          for(int i=0;i<8;i++)
            rtms_info.oc[i] = packet.block[3+i]*10;
          rtms_info.id = packet.block[11];
        }
        else
        if(packet.block[2]==17){
          for(int i=0;i<8;i++)
            rtms_info.oc[i] = packet.block[3+i*2]*256+packet.block[4+i*2];
          rtms_info.id = packet.block[19];
        }
        break;

      case SSPEED:
        for(int i=0;i<8;i++)
          rtms_info.sp[i] = packet.block[3+i];
        rtms_info.dirin     = packet.block[11]==0x80?false:true;
        rtms_info.avgspeed  = packet.block[12];
        rtms_info.health    = (packet.block[13]!=0x10
                             &&packet.block[13]!=0x20
                             &&packet.block[13]!=0x30
                             &&packet.block[13]!=0x40
                             &&packet.block[13]!=0x50
                             &&packet.block[13]!=0x60
                             &&packet.block[13]!=0x70)?false:true;
        break;
  }//switch(rtms_info.info)
}
//--------------------------------------------------------------------------

