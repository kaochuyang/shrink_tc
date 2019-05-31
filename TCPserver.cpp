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
//pthread_record=tid;
    pthread_attr_destroy( & attr );

}
bool TCPserver::parsTCP_JsonV3Content(char buff[4096],int connfd)
{
    Json::Value JsonObjForAp;
    Json::Reader reader;
    shirink_app ShrinkAppDriver;
    string check;
    int chek=0;
    Json::Value object_c;
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

void* TCPserver::pthread_func(void *arg)
{
    shirink_app  TcData=getTcData();

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
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    //servaddr.sin_addr.s_addr = inet_addr("192.168.2.1");
    servaddr.sin_port = htons(5000);



    if ( bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) == -1)
    {
        printf(" bind socket error: %s (errno :%d)\n",strerror(errno),errno);
        return 0;
    }

    sleep(1);

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
//        else
//         {socketboot=true;}





        while((n=recv(connfd,buff,MAXLINE,0))>0)
        {
            buff[n] = '\0';
            printf("\n");
            printf("recv msg from client:%s\n",buff);
            printf("string size=%d\n",TcData.string_to_app.toStyledString().size());
            //   printf("%s\n",F.string_to_app.toStyledString().c_str());


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

