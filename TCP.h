#ifndef TCP_H
#define TCP_H
#include "var.h"
#include "IODEVICE.h"

#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <pthread.h>

class TCP
{
    public:
        TCP();
        virtual ~TCP();

        BYTE block[BUFFERSIZE];                                                 //udp socket buffer

        MESSAGEOK messageIn[MSGINSIZE];                                         //±µ¦¬¸ÑªR¦¨¥\ªºbuffer

        int maxMessageIndex;                                                    //¦¹¦¸³B²zªºmessageIn«Ê¥]­Ó¼Æ
        int lastPacketIndex;                                                    //¤W¤@¦¸©ñ¨ì³Ì«á¤@­ÓmessageIn.packet[Index]

        int Getfd(void);                                                        //Àò±ofile description
        bool GetPortAlreadyOpen(void);                                          //§PÂ_¦¹°ð¬O§_¤w¶}±Ò
        int GetConnDevice(void);                                                //Àò±o¦¹°ð©Ò³s±µªº³]³Æ
        bool SetConnDevice(int);                                                //³]©w¦¹°ð©Ò³s±µªº³]³Æ
        int OpenTCpSocket(char *,int);                               //¶}³q°T°ð(localhost,listen port,remotehost,remote port)
        bool CloseTCpSocket(void);                                              //Ãö³q°T°ð
        int  TCpRead(void);                                                     //Åª³q°T°ð(read buffer)
        bool TCpSend(BYTE *,int,char *);                                        //¼g³q°T°ð(write array,write length,device name)
        bool TCpSend(BYTE *,int,char *,bool);//CCJ960416¥i±±¨î¬O§_Åã¥Ü°T®§

    private:

        pthread_mutex_t mutexTCp;                                               //«OÅ@°O¾ÐÅé

        int TCpfd;                                                              //file descript
        bool alreadyOpen;                                                       //¦¹port¬O§_¤w¶}±Ò
        int connDevice;                                                         //¦¹°ð³s±µªº³]³Æ
        int connSubDevice;                                                      //¦¹°ð³s±µªº³]³Æ©Ò³s±µªº¤l³]³Æ

        struct sockaddr_in listen_addr;                                         //­nºÊÅ¥ªºport
        struct sockaddr_in send_addr;                                           //­n°e¥Xªºport(¼s¼·µo°e±µ¦¬ªº»P°e¥Xªº°ð¤£¦P¤~¤£·|¤S±µ¦^¨ì¦Û¤v°eªº)

};

#endif // TCP_H
