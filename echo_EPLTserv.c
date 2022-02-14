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

#define BUF_SIZE 4
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
                //event.events=EPOLLIN;
                event.events=EPOLLIN|EPOLLET;
                event.data.fd=clnt_sock;
                epoll_ctl(epfd,EPOLL_CTL_ADD,clnt_sock,&event);
                printf("connected client:%d\n",clnt_sock);
            }
            else
            {
                str_len=read(ep_events[i].data.fd,message,BUF_SIZE);
                printf("str_len:%d ,message:%s\n",str_len,message);
                if(str_len==0)
                {
                    epoll_ctl(epfd,EPOLL_CTL_DEL,ep_events[i].data.fd,NULL);
                    close(ep_events[i].data.fd);
                }
                else
                {
                    int return_value=write(ep_events[i].data.fd,message,str_len);
                    printf("return_value:%d,strlen(message)：%d,message:%s\n",return_value,strlen(message),message);
                }
            }
        }
    }
    
    close(serv_sock);
    close(epfd);
    return 0;
}
/*若边缘触发，61/82行均要该成event.events=EPOLLIN|EPOLLET;
解释下面程序，不触发
[wqr@localhost book_network_program]$ ./clint4 127.0.0.1 9190
connecting...
Input message(Q to quit):
qwertyuiop
str_len:11
rec_cnt:5
^C
[wqr@localhost book_network_program]$ ./serv19 9190
return epoll_wait                //if(ep_events[i].data.fd==serv_sock)一次
connected client:5
return epoll_wait                //for(int i=0;i<event_cnt;i++)中else一次
str_len:4 ,message:qwer
return_value:5,strlen(message)：5,message:qwer
(clint4按^C后以下几行才出现)
return epoll_wait
str_len:4 ,message:tyui
return_value:5,strlen(message)：5,message:tyui
return epoll_wait
str_len:3 ,message:op
i                        //解释:op+n\（于是下一行了），i是上次对应位置剩下的
*/

/*若边缘触发，BUF_SIZE 40
[wqr@localhost book_network_program]$ ./serv19 9190
return epoll_wait
connected client:5
return epoll_wait
str_len:11 ,message:qwertyuiop

return_value:11,strlen(message)：11,message:qwertyuiop

return epoll_wait
str_len:0 ,message:qwertyuiop

^C
[wqr@localhost book_network_program]$ ./clint4 127.0.0.1 9190
connecting...
Input message(Q to quit):
qwertyuiop
str_len:11
rec_cnt:11
Message from server:qwertyuiop
Input message(Q to quit):
q
*/

/*若条件触发 BUF_SIZE 4
[wqr@localhost book_network_program]$ ./serv19 9190
return epoll_wait
connected client:5
return epoll_wait
str_len:4 ,message:qwer
return_value:5,strlen(message)：5,message:qwer
return epoll_wait
str_len:4 ,message:tyui
return_value:5,strlen(message)：5,message:tyui
return epoll_wait
str_len:2 ,message:o                           //解释：o+n\（于是下一行了），然后ui是上一次对应位子剩下的，但是系统只认当前2个字符
ui
return_value:5,strlen(message)：5,message:o    //解释：EOF什么都没传过来，所以message还是上次剩下的
ui
return epoll_wait
str_len:-1 ,message:o
ui
[wqr@localhost book_network_program]$ ./clint4 127.0.0.1 9190
connecting...
Input message(Q to quit):
qwertyuio
str_len:10
rec_cnt:10
Message from server:qwertyuiInput message(Q to quit):
q
*/