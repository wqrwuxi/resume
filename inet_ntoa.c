#include <stdio.h>
#include <arpa/inet.h>//inet_addr
#include <errno.h>

void main()
{
    struct sockaddr_in addr1,addr2;
    char* str_ptr;

    addr1.sin_addr.s_addr = 0x636bd6d3;//inet_addr.c中转换出的网络字序
    addr2.sin_addr.s_addr = htonl(0x04030201);
    str_ptr = inet_ntoa(addr1.sin_addr);
    printf("1：%s \n",str_ptr);
    inet_ntoa(addr2.sin_addr);
    printf("2：%s \n",str_ptr);
}
/*
[wqr@localhost book_network_program]$ ./inet_ntoa
1：211.214.107.99 
2：4.3.2.1 
*/