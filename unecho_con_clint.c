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
    struct sockaddr_in serv_addr,from_addr;
    char message[BUF_SIZE];
    int str_len;
    socklen_t addr_size;

    char msg1[] = "HI!";
    char msg2[] = "I am another UDP host!";
    char msg3[] = "Nice to meet you";
 
    if(argc!=3)
    {
        printf("Usage:%s <IP> <port>\n",argv[0]);
        exit(1);
    }

    sock = socket(PF_INET,SOCK_DGRAM,0);
    if (sock == -1)
    {
       printf("socket Error no.%d: %s\n", errno, strerror(errno));
    }

    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    connect(sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr));//比较unecho_clint.c新加

    while(1)
    {
        fputs("Input message(Q to quit):\n",stdout);
        fgets(message,BUF_SIZE,stdin);

        if(strcmp(message,"q\n")==0 || strcmp(message,"Q\n")==0)
        {
            break;
        }

        //sendto(sock,message,strlen(message),0,(struct sockaddr*)&serv_addr,sizeof(serv_addr));
        write(sock,message,strlen(message));
        /*addr_size = sizeof(from_addr);
        str_len = recvfrom(sock,message,BUF_SIZE,0,(struct sockaddr*)&from_addr,&addr_size);*/
        str_len=read(sock,message,sizeof(message));
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

