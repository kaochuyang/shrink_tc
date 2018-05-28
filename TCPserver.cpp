#include "TCPserver.h"

#include "json/json.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <climits>
#define MAXLINE 4096

TCPserver _tcpserver;
TCPserver::TCPserver()
{
    //ctor
}

TCPserver::~TCPserver()
{
    //dtor
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

        pthread_attr_destroy( & attr );

}

void* TCPserver::pthread_func(void *arg)
{
      Json::Value root;
    Json::FastWriter fast_writer;
    root["REGION_ID"]="600901";

    root["REGION_ID5"]="901";
    root["REGION_ID6"]="5901";
    root["REGION_ID7"]="5501";

//string head="[";
//string tail="]";

//"["{   "REGION_ID" : "600901",   "REGION_ID10" : "6009",   "REGION_ID5" : "700901",   "REGION_ID8" : "6009055"}"]";
    string A="{\"Data\":{\"Name\":\"MichaelChan\",\"Email\":\"XXXX@XXX.com\",\"Phone\":[1234567,0911123456]}}\n";
     string B="{\"Data2\":{\"Name2\":\"MichChan\",\"Email2\":\"XXXX@YYY.com\",\"Phone2\":[1299967,0988823456]}}\n";
     string C="{\"Data3\":{\"Name3\":\"MichAAAn\",\"Email3\":\"XXXX@ZZZ.com\",\"Phone3\":[12998767,0788823456]}}\n";
    //="{[\"{ 123456 }"\";
    // root.toStyledString();
//

    cout<<"\""<<endl;
    cout<<"\\\""<<endl;

    cout<<A.find("\"")<<endl;




    printf("%s\n",A.c_str());

    int listenfd,connfd;
    struct sockaddr_in servaddr;
    char buff[4096];

    char buff2[4096];
    int n;

    //?«Ø¤@?TCPªºsocket
    if( (listenfd = socket(AF_INET,SOCK_STREAM,0)) == -1)
    {
        printf(" create socket error: %s (errno :%d)\n",strerror(errno),errno);
        return 0;
    }

    //¥ý§â¦a§}²MªÅ¡A??¥ô·NIP
    memset(&servaddr,0,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(5000);


    //¦a§}?©w¨ìlistenfd
    if ( bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1)
    {
        printf(" bind socket error: %s (errno :%d)\n",strerror(errno),errno);
        return 0;
    }
    while(1)
    {

        //?§vlistenfdvu,6vu04
        if( listen(listenfd,10) == -1)
        {
            printf(" listen socket error: %s (errno :%d)\n",strerror(errno),errno);
            return 0;
        }

        printf("====waiting for client's request=======\n");
        //accept ©Mrecv,ª`·N±µ¦¬¦r²Å¦ê²K¥[?§ô²Å'\0'

        if( (connfd = accept(listenfd, (struct sockaddr *)NULL, NULL))  == -1)
        {
            printf(" accpt socket error: %s (errno :%d)\n",strerror(errno),errno);
            return 0;
        }
    int chek=0;
        while((n=recv(connfd,buff,MAXLINE,0))>0)
        {

            buff[n] = '\0';
            printf("\n");


            printf("recv msg from client:%s\n",buff);
            printf("\n");

            printf("size=%d\n",A.size());
            if ( chek=send(connfd,A.c_str(),A.size(),0) <0)
            {
                printf("send msg error: %s(errno :%d)\n",strerror(errno),errno);
                return 0;
            }
 printf("send=%d\n",chek);

            sleep(1);
              printf("%s\n",B.c_str());
              if ( chek=send(connfd,B.c_str(),B.size(),0) <0)
            {
                printf("send msg error: %s(errno :%d)\n",strerror(errno),errno);
                return 0;
            }
             printf("%s\n",C.c_str());
             sleep(1);
              if ( chek=send(connfd,C.c_str(),C.size(),0) <0)
            {
                printf("send msg error: %s(errno :%d)\n",strerror(errno),errno);
                return 0;
            }
            printf("send=%d\n",chek);


        }
        close(connfd);
    }

    close(listenfd);


    cout << fast_writer.write(root)<< endl;
    return 0;
}
