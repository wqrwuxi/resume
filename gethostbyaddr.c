#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>//inet_addr
#include <netdb.h>//gethostbyname
#include <errno.h>
#include <string.h>

void main(int argc,char* argv[])
{
    struct hostent *host;
    struct sockaddr_in addr;

    if(argc!=2)
    {
        printf("Usage:%s <IP>\n",argv[0]);
        exit(1);
    }

    memset(&addr,0,sizeof(addr));

    if(!inet_aton(argv[1],&addr.sin_addr))
    {
        printf("inet_aton Error no.%d: %s\n", errno, strerror(errno));
    }
    //addr.sin_addr.s_addr = inet_addr(argv[1]);
    host = gethostbyaddr((char*)&addr.sin_addr,4,AF_INET);
    if(!host)
    {
        herror("error");
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
/*
extern int h_errno;
void herror(const char *s);
const char *hstrerror(int err);
https://linux.die.net/man/3/h_errno
https://stackoverflow.com/questions/396stbyaddr-success
gethostbyaddr2.c中用法
*/

/*error: Unknown host
 *https://bbs.csdn.net/topics/391815602
 最近我也遇到了这个问题，在hosts文件中添加ip地址和域名的映射后解决了：
我觉得应该是这样：
gethostbyname可以通过DNS服务器通过域名获取ip地址等信息。
但是gethostbyaddr输入参数时ip地址，这样无需经过dns转换，无法获取域名等信息，但是可以在hosts文件中增加一个映射，它应该会检查hosts文件。
http://bbs.chinaunix.net/thread-1083939-1-1.html
和自己一个错法
 * /