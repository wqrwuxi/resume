#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>//sockaddr_in
#include <stdlib.h>
#include <unistd.h>
#include <string.h>//memset
#include <errno.h>
#include <pthread.h>

#define BUF_SIZE 100
#define MAX_CLNT 256

int clnt_cnt=0;
int clnt_socks[MAX_CLNT];

void *handle_clnt(void *args);
void send_msg(char* msg,int str_len);

pthread_mutex_t mutx;

int main(int argc,char *argv[])
{
    int serv_sock;
    int clnt_sock;
    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size;
    char message[BUF_SIZE];
    int str_len;
    pthread_t t_id;

    if(argc!=2)
    {
        printf("Usage:%s <port>\n",argv[0]);
        exit(1);
    }
    
    pthread_mutex_init(&mutx,NULL);
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
    
    //====================================同一个变量可在循环中代表多个线程号的值=================================
    while(1) //举例：如果有3个clint，在while(1)下，产生三个线程号，t_id在程序里就一个？？
    //for(int i=0;i<3;i++)   一个t_id可以代表3个线程号
    {
        clnt_addr_size=sizeof(clnt_addr);
        clnt_sock = accept(serv_sock,(struct sockaddr*)&clnt_addr,&clnt_addr_size);

        pthread_mutex_lock(&mutx);
        clnt_socks[clnt_cnt++]=clnt_sock;
        pthread_mutex_unlock(&mutx);

        pthread_create(&t_id,0,handle_clnt,(void*)&clnt_sock);
        //https://blog.csdn.net/Set_Mode/article/details/121439463
        //pthread_detach()即主线程与子线程分离，两者相互不干涉，子线程结束同时子线程的资源自动回收。
        //pthread_join()即是子线程合入主线程，主线程会一直阻塞，直到子线程执行结束，然后回收子线程资源，并继续执行。
        pthread_detach(t_id);
        printf("Connected client IP:%s\n",inet_ntoa(clnt_addr.sin_addr));
    }
    close(serv_sock);
    return 0;
}

void *handle_clnt(void *args)
{
    int clnt_sock=*((int*)args);
    int str_len;
    char msg[BUF_SIZE];
    pthread_t tid;
    pid_t pid;

    tid = pthread_self();
    pid = getpid();
    printf("tid:%d  pid:%d\n",tid,pid);
    
    while((str_len=read(clnt_sock,msg,BUF_SIZE))>0)
    {
        printf("str_len:%d msg:%s",str_len,msg);
        send_msg(msg,str_len);
    }

    pthread_mutex_lock(&mutx);
    
    for(int i=0;i<clnt_cnt;i++)
    {
        if(clnt_sock==clnt_socks[i])
        {
            while(i<(clnt_sock-1))
            {
                clnt_socks[i]=clnt_socks[i+1];
                i++;
            }
            break;
        }
    }

    clnt_cnt--;
    
    pthread_mutex_unlock(&mutx);
    close(clnt_sock);
    return NULL;
   
}

void send_msg(char* msg,int str_len)
{
    pthread_mutex_lock(&mutx);
    
    for(int i=0;i<clnt_cnt;i++)
    {
       int return_value= write(clnt_socks[i],msg,str_len);//用循环，向当前所有client写
       printf("return_value:%d \n",return_value);
    }

    pthread_mutex_unlock(&mutx);
}

/*
[wqr@localhost book_network_program]$ ./serv21 9190
Connected client IP:127.0.0.1               //3个不同的线程号
tid:523052800  pid:16017                    //1个进程号
Connected client IP:127.0.0.1
tid:514660096  pid:16017
Connected client IP:127.0.0.1
tid:506267392  pid:16017
str_len:9 msg:[AA] qwe
return_value:9 
return_value:9 
return_value:9 
str_len:9 msg:[BB] asd
return_value:9 
return_value:9 
return_value:9 
str_len:9 msg:[CC] zxc
return_value:9 
return_value:9 
return_value:9 
str_len:9 msg:[BB] fgh
return_value:9 
return_value:9 
str_len:9 msg:[CC] vbn
return_value:9 
return_value:9 
str_len:9 msg:[CC] m,.
return_value:9 
^C
[wqr@localhost book_network_program]$ ./clint21 127.0.0.1 9190 AA
connecting...
Input message(Q to quit):
qwe
return_value:9 
Input message(Q to quit):
str_len:9 strlen:9 msg:qwe
message fron serv:[AA] qwe

str_len:9 strlen:9 msg:qwe
message fron serv:[BB] asd

str_len:9 strlen:9 msg:qwe
message fron serv:[CC] zxc

q
[wqr@localhost book_network_program]$ ./clint21 127.0.0.1 9190 BB
connecting...
Input message(Q to quit):
str_len:9 strlen:9 msg:message fron serv:[AA] qwe

asd
return_value:9 
Input message(Q to quit):
str_len:9 strlen:9 msg:asd
message fron serv:[BB] asd

str_len:9 strlen:9 msg:asd
message fron serv:[CC] zxc

fgh
return_value:9 
Input message(Q to quit):
str_len:9 strlen:9 msg:fgh
message fron serv:[BB] fgh

str_len:9 strlen:9 msg:fgh
message fron serv:[CC] vbn

q
[wqr@localhost book_network_program]$ ./clint21 127.0.0.1 9190 CC
connecting...
Input message(Q to quit):
str_len:9 strlen:9 msg:message fron serv:[AA] qwe

str_len:9 strlen:9 msg:message fron serv:[BB] asd

zxc
return_value:9 
Input message(Q to quit):
str_len:9 strlen:9 msg:zxc
message fron serv:[CC] zxc

str_len:9 strlen:9 msg:zxc
message fron serv:[BB] fgh

vbn
return_value:9 
Input message(Q to quit):
str_len:9 strlen:9 msg:vbn
message fron serv:[CC] vbn

m,.
return_value:9 
Input message(Q to quit):
str_len:9 strlen:9 msg:m,.
message fron serv:[CC] m,.

q
*/

