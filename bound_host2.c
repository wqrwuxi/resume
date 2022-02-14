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

    memset(&serv_addr,0,sizeof(serv_addr));//注：memset可以用于结构体，不过这个结构体里每个元素的每一位不是1就算0
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);//要连接的IP，不是自己的！
    serv_addr.sin_port = htons(atoi(argv[2]));

    sendto(sock,msg1,sizeof(msg1),0,(struct sockaddr*)&serv_addr,sizeof(serv_addr));
    sendto(sock,msg2,sizeof(msg2),0,(struct sockaddr*)&serv_addr,sizeof(serv_addr));
    sendto(sock,msg3,sizeof(msg3),0,(struct sockaddr*)&serv_addr,sizeof(serv_addr));

    close(sock);
    return 0;
}

void error_handling(char* message)
{
    fputs(message,stderr);
    fputs("\n",stderr);
    exit(1);
}

