#include "TCPserver.h"
#include <iostream>
#include "json/json.h"
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
    string H;

    Json::Value root;
    Json::FastWriter fast_writer;


    shirink_app F;



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
    F.send_tc_project_data();
    F.send_LastShutDownTime();
    F.send_ip();
    F.send_hardwareVersion();
    F.send_reportcycle();
    F.send_manual_setting();
    F.send_lastUpdateDBdate();


    // printf("%s\n",A.c_str());

    int listenfd,connfd;
    struct sockaddr_in servaddr;
    char buff[4096];

    char buff2[4096];
    int n;


    if( (listenfd = socket(AF_INET,SOCK_STREAM,0)) == -1)
    {
        printf(" create socket error: %s (errno :%d)\n",strerror(errno),errno);
        return 0;
    }

    //¥ý§â¦a§}²MªÅ¡A??¥ô·NIP
    memset(&servaddr,0,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
//   servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_addr.s_addr = inet_addr("192.168.2.1");
    servaddr.sin_port = htons(5000);



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
        Json::Value r_test;
        Json::Reader reader;
        int chek=0;

        while((n=recv(connfd,buff,MAXLINE,0))>0)
        {
            buff[n] = '\0';
            printf("\n");
            printf("recv msg from client:%s\n",buff);
            printf("string size=%d\n",F.string_to_app.toStyledString().size());
            //   printf("%s\n",F.string_to_app.toStyledString().c_str());

            if(reader.parse(buff,r_test))
            {
                // if(r_test)
                // printf("STEP=%s\n",r_test["weekdaysegment"].toStyledString().c_str());

                printf("r_test=%s\n",r_test.toStyledString().c_str());
                if(r_test.isObject()&&r_test.isMember("setPassword"))
                {
                    printf("hello setPassword\n");
                    printf("%s\n",r_test["setPassword"].toStyledString().c_str());
                    F.set_password(r_test);
                    printf("test\n");
                }

                if(r_test.isObject()&&r_test.isMember("weekdaysegment"))
                {
                    printf("hello weekdaysegment\n");
                    printf("%s\n",r_test["weekdaysegment"].toStyledString().c_str());
                    F.set_weekdaysegment(r_test);
                    printf("test\n");
                    F.refresh_tc_project_data();
                    F.Packed_Tod_info();

                }
                else if(r_test.isMember("specialdaycontext"))
                {
                    printf("%s\n",r_test["specialdaycontext"].toStyledString().c_str());
                    F.set_specialdaycontext(r_test);
                    printf("hello specialdaycontext\n");
                    F.refresh_tc_project_data();
                    F.Packed_Spd_info();

                }
                else if(r_test.isMember("segmentinfo"))
                {

                    printf("%s\n",r_test["segmentinfo"].toStyledString().c_str());
                    F.set_segment_info(r_test);
                    printf("hello segmentinfo\n");
                    F.refresh_tc_project_data();
                    F.Packed_segmentinfo();


                }
                else if(r_test.isMember("plancontext"))
                {
                    printf("%s\n",r_test["plancontext"].toStyledString().c_str());

                    F.set_plancontext_info(r_test);
                    printf("hello plancontext\n");
                    F.refresh_tc_project_data();
                    F.Packed_plancontext_info();

                }
                else if(r_test.isMember("step"))
                {

                    printf("%s\n",r_test["step"].toStyledString().c_str());

                    F.set_step_info(r_test);


                    printf("hello step\n");
                    F.refresh_tc_project_data();
                    F.Packed_step_info();

                }
                else if(r_test.isMember("ReportCycle"))
                {
                    printf("%s\n",r_test["ReportCycle"].toStyledString().c_str());
                    F.setReportCycle(r_test);

                    F.send_reportcycle();

                }
                else if(r_test.isMember("manual_setting"))
                {
                    printf("%s\n",r_test["manual_setting"].toStyledString().c_str());
                    F.set_manual_setting(r_test);

                    F.send_manual_setting();

                }
                else if(r_test.isMember("reboot"))
                {
                    int check=r_test["reboot"].asInt();

                    if(check==1)
                    {
                        smem.vWriteMsgToDOM("shrink reboot tc\n");
                        F.RebootTC();
                    }
                    else printf("reboot command error\n");

                }
                else if(r_test.isMember("IP_Group"))
                {
                    printf("%s\n",r_test["IP_Group"].toStyledString().c_str());

                    F.set_ip(r_test["IP_Group"]);


                    F.send_ip();


                }
                else if(r_test.isMember("UpdateDB"))
                {
                    printf("%s\n",r_test["UpdateDB"].toStyledString().c_str());

                    F.UpdateDB(r_test["UpdateDB"]);



                }
                else if(r_test.isMember("Password"))
                {


                    string check;
                    printf("passcheck %s\n",r_test["Password"].toStyledString().c_str());
                    Json::Value object_c;
                    if(F.checkPassword(r_test))
                    {
                        object_c["Password"]=1;
                        printf("passwd success\n");
                    }
                    else
                    {
                        object_c["Password"]=0;
                        printf("passwd false\n");
                    }
                    check=F.faster_writer.write(object_c);
                    if ( chek=send(connfd,check.c_str(),check.size(),0) <0)
                    {
                        printf("send msg error: %s(errno :%d)\n",strerror(errno),errno);
                        return 0;
                    }

                }
                else if(r_test.isMember("test"))//for pad refresh info about tc
                {
                    printf("read value\n");

                    F.send_lastUpdateDBdate();
                    H=F.faster_writer.write(F.string_to_app);
                    if ( chek=send(connfd,H.c_str(),H.size(),0) <0)
                    {
                        printf("send msg error: %s(errno :%d)\n",strerror(errno),errno);
                        return 0;
                    }
                    //       printf("send=%d string=%s\n",chek,H.c_str());
                    printf("size=%d\n",H.size());
                };


            }

            sleep(1);
            // n=0;

        }
        close(connfd);
        cout<<"TCP THREAD DEAD !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<endl;
        cout<<"TCP THREAD DEAD !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<endl;
        cout<<"TCP THREAD DEAD !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<endl;
    }

    close(listenfd);
    cout<<"TCP THREAD DEAD !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<endl;
    cout<<"TCP THREAD DEAD !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<endl;
    cout<<"TCP THREAD DEAD !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!"<<endl;

    //  cout << fast_writer.write(root)<< endl;
    return 0;
}
