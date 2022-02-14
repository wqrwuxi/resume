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
#include <fcntl.h>

#define BUF_SIZE 4
#define EPOLL_SIZE 50

void setnonblockingmode(int fd);

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
    //setnonblockingmode(serv_sock);
    event.events=EPOLLIN;
    //event.events=EPOLLIN|EPOLLET;
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
        
        puts("return epoll_wait");//条件触发时，若传回EOF也会显示一次这个
        for(int i=0;i<event_cnt;i++)
        {
            if(ep_events[i].data.fd==serv_sock)
            {
                clnt_addr_size=sizeof(clnt_addr);
                clnt_sock = accept(serv_sock,(struct sockaddr*)&clnt_addr,&clnt_addr_size);
                //setnonblockingmode(clnt_sock);
                event.events=EPOLLIN|EPOLLET;
                event.data.fd=clnt_sock;
                epoll_ctl(epfd,EPOLL_CTL_ADD,clnt_sock,&event);
                printf("connected client:%d\n",clnt_sock);
            }
            else
            {
              while(1)//边缘触发方式中，发生事件时需要读取输入缓冲中的所有数据，因此需要循环调用read函数
              {
                str_len=read(ep_events[i].data.fd,message,BUF_SIZE);
                printf("str_len:%d ,message:%s\n",str_len,message);
                if(str_len==0)
                {
                    epoll_ctl(epfd,EPOLL_CTL_DEL,ep_events[i].data.fd,NULL);
                    close(ep_events[i].data.fd);
                }
                else if(str_len<0)
                {
                    //read函数返回-1 变量ermo中的值为EAGAIN时，说明没有数据可读
                    //这东西在阻塞模式下，如果读完一行，且没输入q，则没有反映
                    //https://www.cnblogs.com/big-xuyue/p/3905597.html
                    //这个错误经常出现在当应用程序进行一些非阻塞(non-blocking)操作(对文件或socket)的时候。
                    //例如，以 O_NONBLOCK的标志打开文件/socket/FIFO，如果你连续做read操作而没有数据可读，此时程序不会阻塞起来等待数据准备就绪返回，
                    //read函数会返回一个错误EAGAIN，提示你的应用程序现在没有数据可读请稍后再试。
                    if(errno==EAGAIN)
                    {
                        printf("read Error no.%d: %s\n", errno, strerror(errno));
                        break;
                    }
                    else
                    {
                        printf("read Error no.%d: %s\n", errno, strerror(errno));
                        break;
                    }
                }
                else
                {
                    int return_value=write(ep_events[i].data.fd,message,str_len);
                    printf("return_value:%d,strlen(message)：%d,message:%s\n",return_value,strlen(message),message);
                }
              }
            }
        }
    }
    
    close(serv_sock);
    close(epfd);
    return 0;
}

void setnonblockingmode(int fd)
{
    int flag=fcntl(fd,F_GETFL,0);
    fcntl(fd,F_SETFL,flag|O_NONBLOCK);//边缘触发方式下，以阻塞方式工作的read& write函数有可能引起服务器端的长时间停顿
}
/*
[wqr@localhost book_network_program]$ ./serv20 9190
return epoll_wait                     //1个
connected client:5
return epoll_wait                    //2个
str_len:4 ,message:qwer
return_value:5,strlen(message)：5,message:qwer
str_len:4 ,message:tyui
return_value:5,strlen(message)：5,message:tyui
str_len:4 ,message:opp

return_value:5,strlen(message)：5,message:opp

str_len:-1 ,message:opp

read Error no.11: Resource temporarily unavailable
(以下为clint4 按q后输出)
return epoll_wait                   //3个
str_len:0 ,message:opp

str_len:-1 ,message:opp

read Error no.9: Bad file descriptor  //str_len:0时关闭了fd
[wqr@localhost book_network_program]$ ./clint4 127.0.0.1 9190
connecting...
Input message(Q to quit):
qwertyuiopp
str_len:12
rec_cnt:15
Message from server:qwertyuiopp
Input message(Q to quit):
q
*/

/*
[wqr@localhost book_network_program]$  ./serv20 9191
return epoll_wait
connected client:5
return epoll_wait
str_len:4 ,message:qwer
return_value:5,strlen(message)：5,message:qwer
str_len:4 ,message:tyui
return_value:5,strlen(message)：5,message:tyui
str_len:4 ,message:opp

return_value:5,strlen(message)：5,message:opp

(以下为clint4 按q后输出)
str_len:0 ,message:opp

str_len:-1 ,message:opp

read Error no.9: Bad file descriptor
[wqr@localhost book_network_program]$ ./clint4 127.0.0.1 9191
connecting...
Input message(Q to quit):
qwertyuiopp
str_len:12
rec_cnt:5
rec_cnt:5
rec_cnt:5
Message from server:qwertyuiopp
Input message(Q to quit):
q
*/