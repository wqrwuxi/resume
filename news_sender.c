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
    struct sockaddr_in mul_adr;
    int time_live=TTL;
    FILE *fp;
    char buf[BUF_SIZE];


    if(argc!=3)
    {
        printf("Usage:%s <GroupIP> <port>\n",argv[0]);
        exit(1);
    }
 
    send_sock=socket(PF_INET,SOCK_DGRAM,0);
    memset(&mul_adr,0,sizeof(mul_adr));
    mul_adr.sin_family = AF_INET;
    mul_adr.sin_addr.s_addr = inet_addr(argv[1]);
    mul_adr.sin_port = htons(atoi(argv[2]));

    setsockopt(send_sock,IPPROTO_IP,IP_MULTICAST_TTL,(void*)&time_live,sizeof(time_live));
    if((fp=fopen("news.txt","r"))==NULL)
    {
        printf("fopen Error no.%d: %s\n", errno, strerror(errno));
    }

    while(!feof(fp))
    {
        fgets(buf,BUF_SIZE,fp);
        sendto(send_sock,buf,strlen(buf),0,(struct sockaddr*)&mul_adr,sizeof(mul_adr));
        sleep(2);
    }

    fclose(fp);
    close(send_sock);
    return 0;
}