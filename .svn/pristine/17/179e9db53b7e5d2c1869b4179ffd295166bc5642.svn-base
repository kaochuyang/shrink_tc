#ifndef CPacketClusterH
#define CPacketClusterH
#define buffer_block_size 500
#define packet_block_size 100
#define packet_amount 50

//---------------------------------------------------------------------
struct SBuffer
{
    unsigned long length;
    unsigned char block[buffer_block_size];
    SBuffer(void):length(0){
                   for(int i=0;i<buffer_block_size;i++) block[i]=0;
                 }
};
//---------------------------------------------------------------------
//---------------------------------------------------------------------
struct SPacket
{
    bool status;
    unsigned long length;
    unsigned char block[packet_block_size];
    SPacket(void):status(false),length(0){ //BugFix
                   for(int i=0;i<packet_block_size;i++) block[i]=0;
                 }
};
//---------------------------------------------------------------------
//----------------------------------------------------------------------
class CPacketCluster
{
  public:
    SPacket packet[packet_amount];
    int last_packet;
    CPacketCluster(void):last_packet(0){}
    //~CPacketCluster(void);

    void FlushPacket(void);

};
//------------------------------------------------------------------------------
#endif
