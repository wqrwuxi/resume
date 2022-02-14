#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>//memset
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>//sockaddr_in
#include <signal.h>
#include <sys/wait.h>

#define BUF_SIZE 30

void read_childproc(int sig);


void main(int argc,char* argv[])
{
    int serv_sock;
    int clnt_sock;
    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size;
    struct sigaction act;
    sigset_t newmask, oldmask;
    int state , str_len;
    pid_t pid;
    char buf[BUF_SIZE];
    
    
    if(argc!=2)
    {
        printf("Usage:%s <port>\n",argv[0]);
        exit(1);
    }

    /*等价于下面一块代码，都是表示接受某信号并且有所动作
    if (signal(SIGCHLD, read_childproc) == SIG_ERR)
    {
        printf("signal Error no.%d: %s\n", errno, strerror(errno));
    }*/
    
    act.sa_handler = read_childproc;//sa_handler 要自己输入，不会挑出来的
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    state = sigaction(SIGCHLD,&act,0);
    if(state<0)
    {
        printf("sigaction Error no.%d: %s\n", errno, strerror(errno));
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
    
    //====================================理解困难处=================================
    //这个while理解困难，其实serv会因为这个一直循环下去，根本不会运行出while的范围内，所以测试以 ^C 终止
    while(1)
    {
        clnt_addr_size=sizeof(clnt_addr);//下面取地址，所以被拆开来了
        clnt_sock = accept(serv_sock,(struct sockaddr*)&clnt_addr,&clnt_addr_size);
        if(clnt_sock==-1)
        {
            continue;
        }
        else
        {
            printf("new client connected...\n");
        }

        pid = fork();
        if(pid==-1)
        {
            close(clnt_sock);
            continue;
        }
        else if(pid==0)
        {
            close(serv_sock);

            /*有时候会有无限循环  原因不详
            str_len = read(clnt_sock,buf,BUF_SIZE);
            printf("str_len:%d\n",str_len);
            while(str_len>0)
            {
                write(clnt_sock,buf,str_len);
                str_len = read(clnt_sock,buf,BUF_SIZE);
                printf("str_len:%d\n",str_len);
            }*/

            //========================注意read第三个参数=================
            while((str_len = read(clnt_sock,buf,BUF_SIZE))>0)
            {
                printf("str_len:%d\n",str_len);
                write(clnt_sock,buf,str_len);
            }

            close(clnt_sock);
            printf("client disconnected...\n");
            exit(1);//少了这个所以接受不了信号，没退出。不然不退出，子进程/父进程谁先结束还不一定呢
        }
        else
        {
            close(clnt_sock);
        }
    }
    close(serv_sock);

}

void read_childproc(int sig)
{
    printf("removed SIGCHLD\n");
    pid_t pid;
    int status;
    pid =wait(&status);
    printf("removed proc id:%d\n",pid);
}