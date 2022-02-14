#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>//sockaddr_in
#include <arpa/inet.h>//inet_addr
#include <stdlib.h>
#include <unistd.h>
#include <string.h>//memset
#include <errno.h>
#include <sys/wait.h>

#define BUF_SIZE 30
void error_handling(char* message);

int main(int argc,char *argv[])
{
    int sock;
    struct sockaddr_in serv_addr;
    char message[BUF_SIZE];
    int str_len;
    pid_t pid;

    if(argc!=3)
    {
        printf("Usage:%s <IP> <port>\n",argv[0]);
        exit(1);
    }

    sock = socket(PF_INET,SOCK_STREAM,0);
    if (sock == -1)
    {
       printf("socket Error no.%d: %s\n", errno, strerror(errno));

    }

    memset(&serv_addr,0,sizeof(serv_addr));//注：memset可以用于结构体，不过这个结构体里每个元素的每一位不是1就算0
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);//要连接的IP，不是自己的！
    serv_addr.sin_port = htons(atoi(argv[2]));

    if(connect(sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr))==-1)//总觉得serv_addr不对，感觉clnt_addr好点？
    {
        printf("connect Error no.%d: %s\n", errno, strerror(errno));
    }
    else
    {
        printf("connecting...\n");
    }

    pid =fork();
    if(pid==0)
    {
        while(1)
        {
            fputs("Input message(Q to quit):\n",stdout);
            fgets(message,BUF_SIZE,stdin);
            
            if(strcmp(message,"q\n")==0 || strcmp(message,"Q\n")==0)
            {
               shutdown(sock,SHUT_WR);//必须要有，有了才能传EOF出去,相当于写一个EOF
               break;
            }

            int str_len=write(sock,message,strlen(message));
            if(str_len<0)
            {
                printf("write Error no.%d: %s\n", errno, strerror(errno));
            }
        }
    }
    else if(pid>0)
    {
        //======================================attention!====================
        //具体过程
        //正常情况下，clint10 从 子进程 传一次数据，进行一次write -> serv10 从 子进程 read一次+write一次 -> clint10 从 父进程 read一次
        //输入Q/q：clint10子进程shutdown 等价于 write EOF-> serv10 从 子进程 read 且返回0 跳出while循环 -> serv10 close(clnt_sock) 等价于 write EOF
        //（接上行）clint10 父进程 read 返回0 跳出while循环
        while(1)
        {
           //========================注意read第三个参数=================
           //字符串strlen取的值（sizeof出来大小为BUF_SIZE)  或者  字符串的BUF_SIZE  或者  结构体/数组sizeof取的值 
           //原因：见read函数
           //int str_len = read(sock,message,strlen(message));
           int str_len = read(sock,message,BUF_SIZE);
           printf("str_len:%d\n",str_len);
           if(str_len<=0)
           {
               break;
           }
           printf("Message from server:%s",message);
        }
        //wait(NULL);
    }
    close(sock);


}