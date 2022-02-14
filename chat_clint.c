#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>//sockaddr_in
#include <arpa/inet.h>//inet_addr
#include <stdlib.h>
#include <unistd.h>
#include <string.h>//memset
#include <errno.h>
#include <pthread.h>

#define BUF_SIZE 100
#define NAME_SIZE 20

void* send_msg(void* arg);
void* recv_msg(void* arg);

char name[NAME_SIZE]="[DEFAULT]";
char msg[BUF_SIZE];

int main(int argc,char *argv[])
{
    int sock;
    struct sockaddr_in serv_addr;
    char message[BUF_SIZE];
    int str_len,rec_len,rec_cnt;
    pthread_t snd_thread,rec_thread;


    if(argc!=4)
    {
        printf("Usage:%s <IP> <port> <name>\n",argv[0]);
        exit(1);
    }
    
    sprintf(name,"[%s]",argv[3]);
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

    pthread_create(&snd_thread,0,send_msg,(void*)&sock);
    pthread_create(&rec_thread,0,recv_msg,(void*)&sock);
    pthread_join(snd_thread,NULL);
    pthread_join(rec_thread,NULL);
    close(sock);
    return 0;
}

void* send_msg(void* arg)
{
    int sock=*((int*)arg);
    char name_msg[NAME_SIZE+BUF_SIZE];
    
    while(1)
    {
        fputs("Input message(Q to quit):\n",stdout);
        fgets(msg,BUF_SIZE,stdin);

        if(strcmp(msg,"q\n")==0 || strcmp(msg,"Q\n")==0)
        {
           shutdown(sock,SHUT_WR);
           exit(0);
        }
        sprintf(name_msg,"%s %s",name,msg);
        int return_value=write(sock,name_msg,strlen(name_msg));
        printf("return_value:%d \n",return_value);
    }
    
    return NULL;
}

void* recv_msg(void* arg)
{
    int sock=*((int*)arg);
    char name_msg[NAME_SIZE+BUF_SIZE];
    int str_len;
    
    while(1)
    {
          //str_len=read(sock,name_msg,strlen(name_msg));会出错，没传任何消息就直接返回0了，而且传了消息也读不了
          //原因：name_msg是read前的name_msg，不是从网络中read的内容
          str_len=read(sock,name_msg,NAME_SIZE+BUF_SIZE);
          printf("str_len:%d strlen:%d msg:%s",str_len,strlen(name_msg),msg);
          if(str_len==-1)
          {
              return (void*)-1;
          }
          name_msg[str_len]=0;
          printf("message fron serv:%s\n",name_msg);
       
    }
    
    return NULL;  
}