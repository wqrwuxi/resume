#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>//sockaddr_in
#include <stdlib.h>
#include <unistd.h>
#include <string.h>//memset
#include <errno.h>

#define BUF_SIZE 100
void error_handling(char* message);

int main(int argc,char *argv[])
{
    int serv_sock;
    int clnt_sock;
    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size;
    char buf[BUF_SIZE];
    int str_len,fd_max,fd_num;
    struct timeval timeout;
    fd_set reads,cpy_reads;

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
    
    FD_ZERO(&reads);
    FD_SET(serv_sock,&reads);
    fd_max=serv_sock;
    
    while(1)
    {
        //调用select 函数后，除发生变化的文件描述符对应位外，剩下的所有位将初始化为0
        cpy_reads=reads;
        //调用select函数后，结构体tiiTievol的成员tv_sec和tv_usec的值将被替换为超时前剩余时间,每次调用select前都要初始化timeout
        timeout.tv_sec=5;
        timeout.tv_usec=5000;

        if((fd_num=select(fd_max+1,&cpy_reads,0,0,&timeout))==-1)//select函数的第一个参数:监视（检查）范围
        {
            break;
        }
        else if(fd_num==0)
        {
            continue;
        }

        for(int i=0;i<fd_max+1;i++)
        {
            if(FD_ISSET(i,&cpy_reads))//只有这个是cpy_reads   FD_ISSET用于验证select函数的调用结果，观察是否发生状态变化，即那些需要监视的文件描述符在cpy_reads相应位置为1
            {
                if(i==serv_sock)//发生状态变化时，首先验证服务器端套接字中是否有变化
                {
                    clnt_addr_size=sizeof(clnt_addr);
                    clnt_sock = accept(serv_sock,(struct sockaddr*)&clnt_addr,&clnt_addr_size);
                    FD_SET(clnt_sock,&reads);
                    if(fd_max<clnt_sock)
                    {
                        fd_max = clnt_sock;//更新监视（检查）范围
                    }
                    printf("connected client:%d\n",clnt_sock);
                }
                else//发生变化的套接字并非服务器端套接字时
                {
                    str_len = read(i,buf,BUF_SIZE);
                    if(str_len==0)
                    {
                        FD_CLR(i,&reads);//由1转为0，不再需要被监视
                        close(i);
                    }
                    else
                    {
                        write(i,buf,str_len);
                    }
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

/*
[wqr@localhost book_network_program]$ ./serv12 9190
connected client:4     注意这个数
^C
*/
