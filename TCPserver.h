#ifndef TCPSERVER_H
#define TCPSERVER_H
#include "shirink_app.h"
#include "json/json.h"
class TCPserver
{
public:
    TCPserver();
    virtual ~TCPserver();
    bool parsTCP_JsonV3Content(char ReceiveBuff[4096],int connfd);
    static void tcp_thread_generate();  //argc, argv is the parameter of main
    static void *pthread_func(void *);
    static shirink_app getTcData();
        string StringForSend;

//static bool socketboot;
protected:
private:
//static int tcplivecount;
//static int connfd_record;
    //  static unsigned long int pthread_record;
};
extern TCPserver _tcpserver;
#endif // TCPSERVER_H
