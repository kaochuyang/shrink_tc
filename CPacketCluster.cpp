
#include "CPacketCluster.h"
//------------------------------------------------------------------------------
void CPacketCluster::FlushPacket(void)
{
   if(packet[last_packet].status==false){  // Inevitable to be!!
      for(unsigned int m=0;m<packet[last_packet].length;m++)
         packet[0].block[m]=packet[last_packet].block[m];

      packet[0].length=packet[last_packet].length;
      packet[0].status=false;
      last_packet=0;
   }

   else{
      packet[0].length=0;
      packet[0].status=false;
      last_packet=0;
   }
}
