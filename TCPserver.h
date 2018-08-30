#ifndef TCPSERVER_H
#define TCPSERVER_H
#include "shirink_app.h"
#include "json/json.h"
class TCPserver
{
    public:
        TCPserver();
        virtual ~TCPserver();
void endthread();
void setTcpLivecount(int sec);
int getTcpLivecount();
void ReportInfoOK(char* ok_info);
        static void tcp_thread_generate();  //argc, argv is the parameter of main
  static void *pthread_func(void *);
//static bool socketboot;
    protected:
    private:
//static int tcplivecount;
//static int connfd_record;
  //  static unsigned long int pthread_record;
};
extern TCPserver _tcpserver;
#endif // TCPSERVER_H
