#include "TCPserver.h"
#include <iostream>

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <climits>
#include <arpa/inet.h>
#define MAXLINE 4096

using namespace std;
TCPserver _tcpserver;
TCPserver::TCPserver()
{
    //ctor
}

TCPserver::~TCPserver()
{
    //dtor
}
int  TCPserver::HandleTCPClient( int clntSocket )
{
    char  echoBuffer[RCVBUFSIZE];        // Buffer for echo string
    int     recvMsgSize;                   // Size of received message
    int readSelectLength;
    bzero( echoBuffer, RCVBUFSIZE );

    // Receive message from client
    if ( ( recvMsgSize = recv( clntSocket, echoBuffer, RCVBUFSIZE, 0 ) ) < 0 )
    {
        perror("recv() failed");
        exit(1);
    }

    // Send received string and receive again until end of transmission
    if ( recvMsgSize > 0 )      // zero indicates end of transmission
    {
        printf( "Recv(%d): %s\n", recvMsgSize, echoBuffer );


        readSelectLength=recvMsgSize;

        /* printf("receive message  =");
         for(int i=0;i<readSelectLength;i++)printf("%x ",echoBuffer[i]);
         printf("\n");*/                                //OT20110526

        parsTCP_JsonV3Content(echoBuffer,clntSocket);


        // Echo message back to client
//                if ( send( clntSocket, echoBuffer, recvMsgSize, 0) != recvMsgSize )
//                {
//                    perror( "send() failed" );
//                    exit(1);
//                }
    }

    return recvMsgSize;
}
void* TCPserver::pthread_func(void *arg)
{
    shirink_app  TcData=getTcData();

    int             tcpServSock;     // Socket descriptors for TCP server
    long            timeout;         // Timeout value given on command-line

//    int cliSock[MAXCLIENT];          // Client Socket Set

    int running = 1;                 // 1 if server should be running; 0 otherwise

    unsigned short portNo;           // Actual port number

    int i, j;                        // For loop use

    int                     epfd;                   // EPOLL File Descriptor.
    struct epoll_event      ev;                     // Used for EPOLL.
    struct epoll_event      events[120];            // Used for EPOLL.
    int                     noEvents;               // EPOLL event number.


    timeout =60;//atol( argv[1] );      // First arg: Timeout
    portNo =smem.GetLocalIP1(5);// atoi( argv[2] );

    // Create epoll file descriptor.
    // MAXCLIENT + 3 = MAXCLIENT + TCP + UDP +STD_IN.
    epfd = epoll_create( MAXCLIENT + 3 );

    // Create port socket
    tcpServSock =_tcpserver.CreateTCPServerSocket( portNo );
//    udpServSock = _tcpserver.CreateUDPServerSocket( portNo );

    // Add to the epoll
    ev.data.fd = tcpServSock;
    ev.events = EPOLLIN | EPOLLOUT | EPOLLET;
    epoll_ctl( epfd, EPOLL_CTL_ADD, tcpServSock, &ev );

    // Add STDIN into the EPOLL set.
    ev.data.fd = STDIN_FILENO;
    ev.events = EPOLLIN | EPOLLET;
    epoll_ctl( epfd, EPOLL_CTL_ADD, STDIN_FILENO, &ev );

    // Initialize the client socket pool
    for( i = 0 ; i < MAXCLIENT ; i++ )
    {
  //     cliSock[i] = -1;
        smem.setCliSock(i,-1);
    }

    printf( "Starting server:  Hit return to shutdown\n" );
    while ( running )
    {
        // Wait for events.
        // int epoll_wait(int epfd, struct epoll_event *events, int maxevents, int timeout);
        // Specifying a timeout of -1 makes epoll_wait() wait indefinitely.
        noEvents = epoll_wait( epfd, events, FD_SETSIZE , ( timeout * 1000 ) );

        if ( noEvents <= 0 )
        {
            printf("No echo requests for %ld secs...Server still alive\n", timeout);
            continue;
        }

        for( i = 0 ; i < noEvents; i++ )
        {
            if( events[i].events & EPOLLIN && STDIN_FILENO == events[i].data.fd )
            {
                printf("Shutting down server\n");
                getchar();
                running = 0;
                continue;
            }
            else if( ( events[i].events & EPOLLIN ) &&
                     ( events[i].data.fd == tcpServSock ) )
            {
                for( i = 0 ; i < MAXCLIENT ; i++ )
                {
                    if( smem.getCliSock(i) < 0 )
                    {
//                        cliSock[i]
                        smem.setCliSock(i ,_tcpserver.AcceptTCPConnection( tcpServSock ));

                        // Add the client socket to the epoll fdset.
                        ev.data.fd = smem.getCliSock(i);
//                        cliSock[i];
                        ev.events = EPOLLIN | EPOLLET;
                        epoll_ctl( epfd, EPOLL_CTL_ADD, smem.getCliSock(i), &ev );

                        i = MAXCLIENT;
                    }
                }
            }
            else if ( events[i].events & EPOLLIN )
            {
                if( _tcpserver.HandleTCPClient( events[i].data.fd ) == 0 )
                {
                    printf( "Connection %d Shudown.\n", events[i].data.fd );

                    // We do not need to del fd from the epfd set.
                    // Close the fd will remove it from epfd automatically.

                    close( events[i].data.fd );

                    for( j = 0 ; j < MAXCLIENT; j++ )
                    {
                        if( //                        cliSock[j]
                            smem.getCliSock(j)== events[i].data.fd )
                        {
//                            cliSock[j] = -1;
                            smem.setCliSock(j,-1);
                            j = MAXCLIENT;
                        }
                    }
                }

            }
        }
    }

    // Close sockets
    close( tcpServSock );


    for ( i = 0; i < MAXCLIENT; i++ )
    {
        if(
//        cliSock[i]
            smem.getCliSock(i)> 0 )
        {
            close( smem.getCliSock(i));
        }
    }

    // Free list of sockets
    close( epfd );

    return 0;
}

void TCPserver::tcp_thread_generate()
{
    pthread_t tid;
    printf("TCP thread Starting!\n");
    printf("\nMAIN:                 pid=%d\n",getpid());
    pthread_attr_t attr;
    pthread_attr_init( & attr );
    pthread_attr_setdetachstate( & attr, PTHREAD_CREATE_DETACHED );
//  pthread_create( & ( CSTC::_stc_thread_detector_info ), & attr, & ( CSTC::_stc_thread_detector_info_func ), NULL );
    pthread_create( & ( tid ), & attr, & ( TCPserver::pthread_func), NULL );
//pthread_record=tid;
    pthread_attr_destroy( & attr );

}
int TCPserver::AcceptTCPConnection( int servSock )
{
    int                 clntSock;     // Socket descriptor for client
    struct sockaddr_in  echoClntAddr; // Client address
    unsigned int        clntLen;      // Length of client address data structure

    // Set the size of the in-out parameter
    clntLen = sizeof( echoClntAddr );

    // Wait for a client to connect
    if ( ( clntSock = accept( servSock, (struct sockaddr *) &echoClntAddr, &clntLen ) ) < 0 )
    {
        perror("accept() failed");
        exit(1);
    }

    // clntSock is connected to a client!

    printf( "Handling client %s:%d (%d)\n", inet_ntoa( echoClntAddr.sin_addr ),
            ntohs( echoClntAddr.sin_port ), clntSock );

    return clntSock;
}
int TCPserver::CreateTCPServerSocket(unsigned short port)
{
    int sock;                        // socket to create
    struct sockaddr_in echoServAddr; // Local address

    // Create socket for incoming connections
    if ( ( sock = socket( PF_INET, SOCK_STREAM, IPPROTO_TCP ) ) < 0 )
    {
        perror( "socket() failed" );
        exit(1);
    }

    // Construct local address structure
    memset( &echoServAddr, 0, sizeof( echoServAddr ) );     // Zero out structure
    echoServAddr.sin_family = AF_INET;                      // Internet address family
    echoServAddr.sin_addr.s_addr = htonl( INADDR_ANY );     // Any incoming interface
    echoServAddr.sin_port = htons( 5000 );                  // Local port

    // Bind to the local address
    if ( bind(sock, (struct sockaddr *) &echoServAddr, sizeof( echoServAddr ) ) < 0 )
    {
        perror( "bind() failed" );
        exit(1);
    }

    // Mark the socket so it will listen for incoming connections
    if ( listen( sock, MAXPENDING ) < 0 )
    {
        perror( "listen() failed" );
        exit(1);
    }

    return sock;
}
bool TCPserver::parsTCP_JsonV3Content(char buff[4096],int connfd)
{
    Json::Value JsonObjForAp;
    Json::Reader reader;
    shirink_app ShrinkAppDriver;
    string check;
    int chek=0;
    Json::Value object_c;

    try
    {


        if(reader.parse(buff,JsonObjForAp))
        {
            // if(r_test)
            // printf("STEP=%s\n",r_test["weekdaysegment"].toStyledString().c_str());

            printf("r_test=%s\n",JsonObjForAp.toStyledString().c_str());
            if(JsonObjForAp.isObject()&&JsonObjForAp.isMember("setPassword"))
            {
                printf("hello setPassword\n");
                printf("%s\n",JsonObjForAp["setPassword"].toStyledString().c_str());
                ShrinkAppDriver.set_password(JsonObjForAp);
                printf("test\n");
            }

            if(JsonObjForAp.isObject()&&JsonObjForAp.isMember("weekdaysegment"))
            {
                printf("hello weekdaysegment\n");
                printf("%s\n",JsonObjForAp["weekdaysegment"].toStyledString().c_str());
                ShrinkAppDriver.set_weekdaysegment(JsonObjForAp);
                printf("test\n");
                ShrinkAppDriver.refresh_tc_project_data();
                ShrinkAppDriver.Packed_Tod_info();

                object_c["Report"]="todspdinfo_ok";
                check=ShrinkAppDriver.faster_writer.write(object_c);
                if ( chek=send(connfd,check.c_str(),check.size(),0) <0)
                {
                    printf("send msg error: %s(errno :%d)\n",strerror(errno),errno);
                    return 0;
                }

            }
            else if(JsonObjForAp.isMember("specialdaycontext"))
            {
                printf("%s\n",JsonObjForAp["specialdaycontext"].toStyledString().c_str());
                ShrinkAppDriver.set_specialdaycontext(JsonObjForAp);
                printf("hello specialdaycontext\n");
                ShrinkAppDriver.refresh_tc_project_data();
                ShrinkAppDriver.Packed_Spd_info();

                object_c["Report"]="todspdinfo_ok";
                check=ShrinkAppDriver.faster_writer.write(object_c);
                if ( chek=send(connfd,check.c_str(),check.size(),0) <0)
                {
                    printf("send msg error: %s(errno :%d)\n",strerror(errno),errno);
                    return 0;
                }
            }
            else if(JsonObjForAp.isMember("segmentinfo"))
            {

                printf("%s\n",JsonObjForAp["segmentinfo"].toStyledString().c_str());
                ShrinkAppDriver.set_segment_info(JsonObjForAp);
                printf("hello segmentinfo\n");
                ShrinkAppDriver.refresh_tc_project_data();
                ShrinkAppDriver.Packed_segmentinfo();
                object_c["Report"]="segmentinfo_ok";
                check=ShrinkAppDriver.faster_writer.write(object_c);
                if ( chek=send(connfd,check.c_str(),check.size(),0) <0)
                {
                    printf("send msg error: %s(errno :%d)\n",strerror(errno),errno);
                    return 0;
                }

            }
            else if(JsonObjForAp.isMember("plancontext"))
            {
                printf("%s\n",JsonObjForAp["plancontext"].toStyledString().c_str());

                ShrinkAppDriver.set_plancontext_info(JsonObjForAp);
                printf("hello plancontext\n");
                ShrinkAppDriver.refresh_tc_project_data();
                ShrinkAppDriver.Packed_plancontext_info();
                object_c["Report"]="planinfo_ok";
                check=ShrinkAppDriver.faster_writer.write(object_c);
                if ( chek=send(connfd,check.c_str(),check.size(),0) <0)
                {
                    printf("send msg error: %s(errno :%d)\n",strerror(errno),errno);
                    return 0;
                }
            }
            else if(JsonObjForAp.isMember("step"))
            {

                printf("%s\n",JsonObjForAp["step"].toStyledString().c_str());

                ShrinkAppDriver.set_step_info(JsonObjForAp);


                printf("hello step\n");
                ShrinkAppDriver.refresh_tc_project_data();
                ShrinkAppDriver.Packed_step_info();
                object_c["Report"]="stepinfo_ok";
                check=ShrinkAppDriver.faster_writer.write(object_c);
                if ( chek=send(connfd,check.c_str(),check.size(),0) <0)
                {
                    printf("send msg error: %s(errno :%d)\n",strerror(errno),errno);
                    return 0;
                }
            }
            else if(JsonObjForAp.isMember("ReportCycle"))
            {
                printf("%s\n",JsonObjForAp["ReportCycle"].toStyledString().c_str());
                ShrinkAppDriver.setReportCycle(JsonObjForAp);

                ShrinkAppDriver.send_reportcycle();
                object_c["Report"]="cycleinfo_ok";
                check=ShrinkAppDriver.faster_writer.write(object_c);
                if ( chek=send(connfd,check.c_str(),check.size(),0) <0)
                {
                    printf("send msg error: %s(errno :%d)\n",strerror(errno),errno);
                    return 0;
                }
                ShrinkAppDriver.send_reportcycle();
            }
            else if(JsonObjForAp.isMember("manual_setting"))
            {
                printf("%s\n",JsonObjForAp["manual_setting"].toStyledString().c_str());
                ShrinkAppDriver.set_manual_setting(JsonObjForAp);

                ShrinkAppDriver.send_manual_setting();

            }
            else if(JsonObjForAp.isMember("date"))
            {
                printf("%s\n",JsonObjForAp["date"].toStyledString().c_str());
                ShrinkAppDriver.ModifyDate(JsonObjForAp);
                object_c["Report"]="dateinfo_ok";
                check=ShrinkAppDriver.faster_writer.write(object_c);
                if ( chek=send(connfd,check.c_str(),check.size(),0) <0)
                {
                    printf("send msg error: %s(errno :%d)\n",strerror(errno),errno);
                    return 0;
                }


            }
            else if(JsonObjForAp.isMember("reboot"))
            {
                int check=JsonObjForAp["reboot"].asInt();

                if(check==1)
                {
                    smem.vWriteMsgToDOM("shrink reboot tc\n");
                    ShrinkAppDriver.RebootTC();
                }
                else printf("reboot command error\n");

            }
            else if(JsonObjForAp.isMember("IP_Group"))
            {
                printf("%s\n",JsonObjForAp["IP_Group"].toStyledString().c_str());

                ShrinkAppDriver.set_ip(JsonObjForAp["IP_Group"]);


                ShrinkAppDriver.send_ip();


            }
            else if(JsonObjForAp.isMember("UpdateDB"))
            {
                printf("%s\n",JsonObjForAp["UpdateDB"].toStyledString().c_str());

                ShrinkAppDriver.UpdateDB(JsonObjForAp["UpdateDB"]);



            }
            else if(JsonObjForAp.isMember("Password"))
            {


                string check;
                printf("passcheck %s\n",JsonObjForAp["Password"].toStyledString().c_str());
                Json::Value object_c;
                if(ShrinkAppDriver.checkPassword(JsonObjForAp))
                {
                    object_c["Password"]=1;
                    printf("passwd success\n");
                }
                else
                {
                    object_c["Password"]=0;
                    printf("passwd false\n");
                }
                check=ShrinkAppDriver.faster_writer.write(object_c);
                if ( chek=send(connfd,check.c_str(),check.size(),0) <0)
                {
                    printf("send msg error: %s(errno :%d)\n",strerror(errno),errno);
                    return 0;
                }

            }
            else if(JsonObjForAp.isMember("handshaking"))
            {
                printf("handshaking\n");
//                    tcplivecount=0;
            }
            else if(JsonObjForAp.isMember("test"))//for pad refresh info about tc
            {
                printf("read value\n");

                ShrinkAppDriver.send_lastUpdateDBdate();
                StringForSend=ShrinkAppDriver.faster_writer.write(ShrinkAppDriver.string_to_app);
                if ( chek=send(connfd,StringForSend.c_str(),StringForSend.size(),0) <0)
                {
                    printf("send msg error: %s(errno :%d)\n",strerror(errno),errno);
                    return 0;
                }
                //       printf("send=%d string=%s\n",chek,H.c_str());
                printf("size=%d\n",StringForSend.size());
            };


        }
    }
    catch(exception e)
    {
     printf("%s\n",e);
    }

}

shirink_app TCPserver::getTcData()
{
    try
    {
        shirink_app TcData;


//F.send_execute_data();
        /*
               F.send_manual_setting();//v
               F.send_railchain_parama();//V
               F.send_proxy_transfer();//V
               F.send_signal_card_direction();//V
               F.send_tc_stratage_send();//v
               F.send_compensation();//v
               F.send_learn_mode_group();//V
               F.send_ped_control_send();//V*/
//    F.send_chain_send_group();//V
        TcData.send_tc_project_data();
        TcData.send_LastShutDownTime();
        TcData.send_ip();
        TcData.send_hardwareVersion();
        TcData.send_reportcycle();
        TcData.send_manual_setting();
        TcData.send_lastUpdateDBdate();

        return TcData;
    }
    catch(...) {}
}

