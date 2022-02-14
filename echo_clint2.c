#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>//sockaddr_in
#include <arpa/inet.h>//inet_addr
#include <stdlib.h>
#include <unistd.h>
#include <string.h>//memset
#include <errno.h>

#define BUF_SIZE 1024
void error_handling(char* message);

int main(int argc,char *argv[])
{
    int sock;
    struct sockaddr_in serv_addr;
    char message[BUF_SIZE];
    int str_len,rec_len,rec_cnt;

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

    while(1)
    {
        fputs("Input message(Q to quit):\n",stdout);
        fgets(message,BUF_SIZE,stdin);

        if(strcmp(message,"q\n")==0 || strcmp(message,"Q\n")==0)
        {
            break;
        }
        
        //读取总数据长度str_len
        //本段代码的重点在循环！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！！
        //用read循环保证读取服务器的所有信息
        str_len=write(sock,message,strlen(message));
        printf("str_len:%d\n",str_len);
        rec_len=0;
        while(rec_len<str_len)//注意while跳出循环条件，只能这样，原因看下面注释了的while的解析
        {
            rec_cnt=read(sock,&message[rec_len],BUF_SIZE-1);
            printf("rec_cnt:%d\n",rec_cnt);
            if(rec_cnt==-1)
            {
                printf("read Error no.%d: %s\n", errno, strerror(errno));
            }
            rec_len += rec_cnt;
        }

        //==========================================注意的点！==========================================================
        //下面写法来自：Linux系统编程- 进程管理  V3 ,试图代替上面while，但是不行
        //原因：至少在这个例子里，第一轮read可以读完，当第二轮read时，什么也没读到，应该？就一直等着服务器发信息过来，所以程序没有继续下去
        //上述原因会导致serv不写，clint却等着读，会死锁
        //也是为什么echo_mpclient.c中有shutdown的重要原因。shundown相当于serv write EOF,此时clint read返回值为0,跳出循环
        /*while((rec_cnt=read(sock,&message[rec_len],BUF_SIZE-1))>0)
        {
            printf("rec_cnt=%d,rec_len=%d\n",rec_cnt,rec_len);
            rec_len += rec_cnt;
        }*/
        printf("Message from server:%s",message);
    }
    
    close(sock);
    return 0;
}

void error_handling(char* message)
{
    fputs(message,stderr);
    fputs("\n",stderr);
    exit(1);
}

