#include <stdio.h>
#include <arpa/inet.h>//inet_addr
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <errno.h>
#include <string.h>

#define BUF_SIZE 30

int main(int argc,char *argv[])
{
    int recv_sock;
    int str_len;
    char buf[BUF_SIZE];
    struct sockaddr_in adr;

    if(argc!=2)
    {
        printf("Usage:%s <port>\n",argv[0]); 
        exit(1);
    }

    recv_sock=socket(PF_INET,SOCK_DGRAM,0);
    memset(&adr,0,sizeof(adr));
    adr.sin_family = AF_INET;
    adr.sin_addr.s_addr = htonl(INADDR_ANY);
    adr.sin_port = htons(atoi(argv[1]));

    if(bind(recv_sock,(struct sockaddr*)&adr,sizeof(adr))==-1)
    {
        printf("connect Error no.%d: %s\n", errno, strerror(errno));
    }

    while(1)
    {
        str_len=recvfrom(recv_sock,buf,BUF_SIZE-1,0,NULL,0);//recvfrom没有 read/write碰到close发送EOF于是read返回0的存在
        printf("str_len:%d\n",str_len);
        if(str_len<0)
        {
            break;
        }

        buf[str_len]=0;
        fputs(buf,stdout);
    }

    close(recv_sock);
    return 0;
}
/*

[wqr@localhost ~]$ ifconfig
ens160: flags=4163<UP,BROADCAST,RUNNING,MULTICAST>  mtu 1500
        inet 192.168.1.15  netmask 255.255.255.0  broadcast 192.168.1.255
        inet6 240e:3a1:2056:52a0:c4da:190f:dc95:f91a  prefixlen 64  scopeid 0x0<global>
        inet6 fe80::68fa:261:5170:d3e  prefixlen 64  scopeid 0x20<link>
        ether 00:0c:29:ad:61:85  txqueuelen 1000  (Ethernet)
        RX packets 61449  bytes 37925567 (36.1 MiB)
        RX errors 0  dropped 35659  overruns 0  frame 0
        TX packets 3660  bytes 353559 (345.2 KiB)
        TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0

lo: flags=73<UP,LOOPBACK,RUNNING>  mtu 65536
        inet 127.0.0.1  netmask 255.0.0.0
        inet6 ::1  prefixlen 128  scopeid 0x10<host>
        loop  txqueuelen 1000  (Local Loopback)
        RX packets 45  bytes 5080 (4.9 KiB)
        RX errors 0  dropped 0  overruns 0  frame 0
        TX packets 45  bytes 5080 (4.9 KiB)
        TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0

virbr0: flags=4099<UP,BROADCAST,MULTICAST>  mtu 1500
        inet 192.168.122.1  netmask 255.255.255.0  broadcast 192.168.122.255
        ether 52:54:00:a1:ac:7f  txqueuelen 1000  (Ethernet)
        RX packets 0  bytes 0 (0.0 B)
        RX errors 0  dropped 0  overruns 0  frame 0
        TX packets 0  bytes 0 (0.0 B)
        TX errors 0  dropped 0 overruns 0  carrier 0  collisions 0

*/