#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>//inet_addr
#include <netdb.h>//gethostbyname
#include <errno.h>

void main(int argc,char* argv[])
{
    struct hostent *host;

    if(argc!=2)
    {
        printf("Usage:%s <addr>\n",argv[0]);
        exit(1);
    }

    host = gethostbyname(argv[1]);
    if(!host)
    {
        printf("gethostbyname() error\n%s\nh_errno=%d\n", hstrerror(h_errno), h_errno);
    }

    printf("Offical name:%s \n",host->h_name);

    for(int i=0;host->h_aliases[i];i++)//第二个参数的用法：内容存在，不为NULL，判断为true，则继续
    {
       printf("Aliases %d:%s\n",i+1,host->h_aliases[i]);
    }

    printf("Address type:%s\n",(host->h_addrtype == AF_INET)?"AF_INET":"AF_INET6");

    for(int i=0;host->h_addr_list[i];i++)//第二个参数的用法：内容存在，不为NULL，判断为true，则继续
    {
       printf("IP address %d:%s\n",i+1,inet_ntoa(*(struct in_addr*)host->h_addr_list[i]));
    }
}

/*http://c.biancheng.net/cpp/html/363.html
定义函数：char * inet_ntoa(struct in_addr in);
函数说明：inet_ntoa()用来将参数in 所指的网络二进制的数字转换成网络地址, 然后将指向此网络地址字符串的指针返回.
*/