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
    socklen_t addr_size;
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

    for(int i=0;i<3;i++)
    {
        sleep(3);
        addr_size = sizeof(clnt_addr);
        str_len = recvfrom(serv_sock,message,BUF_SIZE,0,(struct sockaddr*)&clnt_addr,&addr_size);
        printf("Message %d:%s\n",i+1,message);
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

// printf("Message %d:%s",i+1,message); 输出结果：9秒之后msg1-msg3一起输出    为什么？？
// printf("Message %d:%s\n",i+1,message);输出结果：每3秒出现一个
/*
printf是一个行缓冲函数，先写到缓冲区，满足条件后，才将缓冲区刷到对应文件中，刷缓冲区的条件如下：

    1 缓冲区填满

    2 写入的字符中有‘\n’ '\r'

    3 调用fflush手动刷新缓冲区

    4 调用scanf要从缓冲区中读取数据时，也会将缓冲区内的数据刷新
————————————————
版权声明：本文为CSDN博主「kerneler_」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/skyflying2012/article/details/10044035
*/
