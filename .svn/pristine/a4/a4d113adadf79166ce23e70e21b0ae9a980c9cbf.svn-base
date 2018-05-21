//---------------------------------------------------------------------------
#ifndef UDPH
#define UDPH

#include "var.h"
#include "IODEVICE.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <pthread.h>

//---------------------------------------------------------------------------
class UDP:public IODEVICE
{
    public:

        UDP(void);
        ~UDP(void);

        BYTE block[BUFFERSIZE];                                                 //udp socket buffer

        MESSAGEOK messageIn[MSGINSIZE];                                         //接收解析成功的buffer

        int maxMessageIndex;                                                    //此次處理的messageIn封包個數
        int lastPacketIndex;                                                    //上一次放到最後一個messageIn.packet[Index]

        int Getfd(void);                                                        //獲得file description
        bool GetPortAlreadyOpen(void);                                          //判斷此埠是否已開啟
        int GetConnDevice(void);                                                //獲得此埠所連接的設備
        bool SetConnDevice(int);                                                //設定此埠所連接的設備
        int OpenUdpSocket(char *,int,char *,int);                               //開通訊埠(localhost,listen port,remotehost,remote port)
        bool CloseUdpSocket(void);                                              //關通訊埠
        int  UdpRead(void);                                                     //讀通訊埠(read buffer)
        bool UdpSend(BYTE *,int,char *);                                        //寫通訊埠(write array,write length,device name)
        bool UdpSend(BYTE *,int,char *,bool);//CCJ960416可控制是否顯示訊息

    private:

        pthread_mutex_t mutexUdp;                                               //保護記憶體

        int udpfd;                                                              //file descript
        bool alreadyOpen;                                                       //此port是否已開啟
        int connDevice;                                                         //此埠連接的設備
        int connSubDevice;                                                      //此埠連接的設備所連接的子設備

        struct sockaddr_in listen_addr;                                         //要監聽的port
        struct sockaddr_in send_addr;                                           //要送出的port(廣撥發送接收的與送出的埠不同才不會又接回到自己送的)

};
//---------------------------------------------------------------------------
#endif
