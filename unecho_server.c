#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>//sockaddr_in
#include <stdlib.h>
#include <unistd.h>
#include <string.h>//memset
#include <errno.h>

#define BUF_SIZE 1024

void error_handling(char* message);

int main(int argc,char *argv[])
{
    int serv_sock;
    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size;
    char message[BUF_SIZE];
    int str_len;

    if(argc!=2)
    {
        printf("Usage:%s <port>\n",argv[0]);
        exit(1);
    }

    serv_sock = socket(PF_INET,SOCK_DGRAM,0);//socket会少打两个字母导致错误的,因为变量名serv_sock
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

    while(1)
    {
        clnt_addr_size = sizeof(clnt_addr);
        str_len = recvfrom(serv_sock,message,BUF_SIZE,0,(struct sockaddr*)&clnt_addr,&clnt_addr_size);
        sendto(serv_sock,message,str_len,0,(struct sockaddr*)&clnt_addr,clnt_addr_size);
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
