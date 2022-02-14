#include <stdio.h>
#include <arpa/inet.h>//inet_addr
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <errno.h>
#include <string.h>

#define TTL 64
#define BUF_SIZE 30

int main(int argc,char *argv[])
{
    int send_sock;
    struct sockaddr_in broad_adr;
    int time_live=TTL;
    FILE *fp;
    char buf[BUF_SIZE];
    int so_brd=1;

    if(argc!=3)
    {
        printf("Usage:%s <Boradcast IP> <port>\n",argv[0]);
        exit(1);
    }
 
    send_sock=socket(PF_INET,SOCK_DGRAM,0);
    memset(&broad_adr,0,sizeof(broad_adr));
    broad_adr.sin_family = AF_INET;
    broad_adr.sin_addr.s_addr = inet_addr(argv[1]);
    broad_adr.sin_port = htons(atoi(argv[2]));

    setsockopt(send_sock,SOL_SOCKET,SO_BROADCAST,(void*)&so_brd,sizeof(so_brd));
    if((fp=fopen("news.txt","r"))==NULL)
    {
        printf("fopen Error no.%d: %s\n", errno, strerror(errno));
    }

    while(!feof(fp))
    {
        printf("sender\n");
        fgets(buf,BUF_SIZE,fp);
        sendto(send_sock,buf,strlen(buf),0,(struct sockaddr*)&broad_adr,sizeof(broad_adr));
        sleep(2);
    }

    close(send_sock);
    return 0;
}
//./sender2 192.168.1.15 9190 可行
//https://bbs.csdn.net/topics/392086315  好像受到了但是没显示，因为防火墙
//关了防火墙就有了，是防火墙的问题