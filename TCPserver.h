#ifndef TCPSERVER_H
#define TCPSERVER_H


class TCPserver
{
    public:
        TCPserver();
        virtual ~TCPserver();

        static void tcp_thread_generate();  //argc, argv is the parameter of main
  static void *pthread_func(void *);

    protected:
    private:
};
extern TCPserver _tcpserver;
#endif // TCPSERVER_H
