#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>//sockaddr_in
#include <stdlib.h>
#include <unistd.h>
#include <string.h>//memset
#include <errno.h>
#include <signal.h>
#include <fcntl.h>

#define BUF_SIZE 30
void error_handling(char* message);
void urg_handler(int signo);
int serv_sock;
int clnt_sock;

int main(int argc,char *argv[])
{
    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size;
    char message[BUF_SIZE];
    int str_len,state;
    struct sigaction act;

    if(argc!=2)
    {
        printf("Usage:%s <port>\n",argv[0]);
        exit(1);
    }

    act.sa_handler = urg_handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags=0;
    state = sigaction(SIGURG,&act,0);

    serv_sock = socket(PF_INET,SOCK_STREAM,0);
    if (serv_sock == -1)
    {
       printf("socket Error no.%d: %s\n", errno, strerror(errno));

    }

    memset(&serv_addr,0,sizeof(serv_addr));
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
    
    clnt_addr_size=sizeof(clnt_addr);
    clnt_sock = accept(serv_sock,(struct sockaddr*)&clnt_addr,&clnt_addr_size);
    
    //将文件描述符recv sock指向的套接字拥有者(F_SETOWN )（即操作系统） 改为把getpid函数返回值用作ID的进程
    //上一行等价于 文件描述符recv_sock指向的套接字引发的SIGURG信号处理进程变为将getpid函 数返回值用作ID的进程
    //上一行在本程序中等价于  当前进程该进程处理SIGURG
    fcntl(clnt_sock,F_SETOWN,getpid());
    
    //非紧急传输数据只能通过未设置MSG_OOB可选项的普通输入函数读取
    while((str_len = recv(clnt_sock,message,BUF_SIZE,0))!=0)
    {
        if(str_len==-1)
        {
            continue;
        }
        message[str_len]=0;//recv第二个参数是个指针，不是个数组，不加后面会有乱码
        printf("recv:%s\n",message);
    }
        
    close(clnt_sock);
    close(serv_sock);
    return 0;
}

void urg_handler(int signo)
{
    int str_len;
    char buf[BUF_SIZE];
    //通过MSG_OOB可选项传递数据时不会加快数据传输速度，而且通过信号处理函数urg_handler取数据时也只能读1个字节。
    //剩余数据只能通过未设置MSG_OOB可选项的普通输入函数读取：main函数中while((str_len = recv(clnt_sock,message,BUF_SIZE,0))!=0)读取
    str_len = recv(clnt_sock,buf,BUF_SIZE,MSG_OOB);//recv第二个参数是个指针，不是个数组，不加后面会有乱码
    buf[str_len]=0;
    printf("Urgent message:%s\n",buf);
}

void error_handling(char* message)
{
    fputs(message,stderr);
    fputs("\n",stderr);
    exit(1);
}
