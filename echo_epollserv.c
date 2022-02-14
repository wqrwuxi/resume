#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>//sockaddr_in
#include <stdlib.h>
#include <unistd.h>
#include <string.h>//memset
#include <errno.h>
#include <sys/epoll.h>

#define BUF_SIZE 1024
#define EPOLL_SIZE 50
void error_handling(char* message);

int main(int argc,char *argv[])
{
    int serv_sock;
    int clnt_sock;
    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size;
    char message[BUF_SIZE];
    int str_len;
    struct epoll_event *ep_events;
    struct epoll_event event;
    int epfd,event_cnt;

    if(argc!=2)
    {
        printf("Usage:%s <port>\n",argv[0]);
        exit(1);
    }

    serv_sock = socket(PF_INET,SOCK_STREAM,0);//socket会少打两个字母导致错误的,因为变量名serv_sock
    if (serv_sock == -1)
    {
       printf("socket Error no.%d: %s\n", errno, strerror(errno));

    }

    memset(&serv_addr,0,sizeof(serv_addr));//注：memset可以用于结构体，不过这个结构体里每个元素的每一位不是1就算0
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr))==-1)
    {
        printf("connect Error no.%d: %s\n", errno, strerror(errno));
    }

    if(listen(serv_sock,5)==-1)
    {
        printf("listen Error no.%d: %s\n", errno, strerror(errno));
    }

    epfd=epoll_create(EPOLL_SIZE);
    ep_events=malloc(sizeof(struct epoll_event)*EPOLL_SIZE);

    event.events=EPOLLIN;
    event.data.fd=serv_sock;
    epoll_ctl(epfd,EPOLL_CTL_ADD,serv_sock,&event);

    while(1)
    {
        event_cnt=epoll_wait(epfd,ep_events,EPOLL_SIZE,-1);
        if(event_cnt==-1)
        {
            printf("epoll_wait Error no.%d: %s\n", errno, strerror(errno));
            break;
        }

        for(int i=0;i<event_cnt;i++)
        {
            if(ep_events[i].data.fd==serv_sock)
            {
                clnt_addr_size=sizeof(clnt_addr);
                clnt_sock = accept(serv_sock,(struct sockaddr*)&clnt_addr,&clnt_addr_size);
                event.events=EPOLLIN;
                event.data.fd=clnt_sock;
                epoll_ctl(epfd,EPOLL_CTL_ADD,clnt_sock,&event);
                printf("connected client:%d\n",clnt_sock);
            }
            else
            {
                str_len=read(ep_events[i].data.fd,message,BUF_SIZE);
                if(str_len==0)
                {
                    epoll_ctl(epfd,EPOLL_CTL_DEL,ep_events[i].data.fd,NULL);
                    close(ep_events[i].data.fd);
                }
                else
                {
                    write(ep_events[i].data.fd,message,strlen(message));
                }
            }
        }
    }
    
    close(serv_sock);
    return 0;
}

void error_handling(char* message)
{
    fputs(message,stderr);
    fputs("\n",stderr);
    exit(1);
}
