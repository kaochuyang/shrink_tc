#ifndef TCPSERVER_H
#define TCPSERVER_H
#include "shirink_app.h"
#include "json/json.h"
#include <stdio.h>          // for printf() and fprintf()
#include <sys/socket.h>     // for socket(), bind(), and connect()
#include <arpa/inet.h>      // for sockaddr_in and inet_ntoa()
#include <stdlib.h>         // for atoi() and exit()
#include <string.h>         // for memset()
#include <unistd.h>         // for close()
#include <sys/time.h>       // for struct timeval {}
#include <fcntl.h>          // for fcntl()
#include <pthread.h>
#include <sys/epoll.h>
#define MAXPENDING 5        // Maximum outstanding connection requests
#define MAXCLIENT  100      // Maximum client connections
#define RCVBUFSIZE 4096     // Size of receive buffer
class TCPserver
{
public:
    TCPserver();
    virtual ~TCPserver();
    bool parsTCP_JsonV3Content(BYTE ReceiveBuff[4096],int connfd);
    static void tcp_thread_generate();  //argc, argv is the parameter of main
    static void *pthread_func(void *);
    static shirink_app getTcData();
    string StringForSend;
    int     CreateTCPServerSocket( unsigned short );
    int     AcceptTCPConnection( int );
    int     HandleTCPClient( int );
//static bool socketboot;
protected:
private:
//static int tcplivecount;
//static int connfd_record;
    //  static unsigned long int pthread_record;
};
extern TCPserver _tcpserver;
#endif // TCPSERVER_H
