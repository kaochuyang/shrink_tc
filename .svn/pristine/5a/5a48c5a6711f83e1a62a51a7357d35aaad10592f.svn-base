//---------------------------------------------------------------------------
#ifndef RS232H
#define RS232H

#include "var.h"
#include "IODEVICE.h"

#include <pthread.h>
//---------------------------------------------------------------------------
class RS232:public IODEVICE
{
    public:

        RS232(void);
        ~RS232(void);

        BYTE block[BUFFERSIZE];                                                 //rs232 port buffer

        MESSAGEOK messageIn[MSGINSIZE];                                         //接收解析成功的buffer

        int maxMessageIndex;                                                    //此次處理的messageIn封包個數
        int lastPacketIndex;                                                    //上一次放到最後一個messageIn.packet[Index]

        int Getfd(void);                                                        //獲得file description
        bool GetPortAlreadyOpen(void);                                          //判斷此埠是否已開啟
        int GetConnDevice(void);                                                //獲得此埠所連接的設備
        bool SetConnDevice(int);                                                //設定此埠所連接的設備
        int OpenRs232Port(char *,int,bool);                                     //開通訊埠(device name,baudrate,is even parity)
        bool CloseRs232Port(void);                                              //關通訊埠
        int Rs232Read(void);                                                    //讀通訊埠(read buffer)
        bool Rs232Write(BYTE *,int,char *);                                     //寫通訊埠(write array,write length,device name)


    private:

        pthread_mutex_t mutexRS232;                                             //保護記憶體

        int rs232fd;                                                            //file descript
        bool alreadyOpen;                                                       //此port是否已開啟
        int connDevice;                                                         //此埠連接的設備
        int connSubDevice;                                                      //此埠連接的設備所連接的子設備


};
//---------------------------------------------------------------------------
#endif
