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
#define RLT_SIZE 4
#define OPSZ 4

void error_handling(char* message);

int main(int argc,char *argv[])
{
    int sock;
    struct sockaddr_in serv_addr;
    int result,opnd_cnt,i;
    char opmsg[BUF_SIZE];

    if(argc!=3)
    {
        printf("Usage:%s <IP> <port>\n",argv[0]);
        exit(1);
    }

    sock = socket(PF_INET,SOCK_STREAM,0);
    if (sock == -1)
    {
       printf("socket Error no.%d: %s\n", errno, strerror(errno));
    }

    memset(&serv_addr,0,sizeof(serv_addr));//注：memset可以用于结构体，不过这个结构体里每个元素的每一位不是1就算0
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);//要连接的IP，不是自己的！
    serv_addr.sin_port = htons(atoi(argv[2]));

    if(connect(sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr))==-1)//总觉得serv_addr不对，感觉clnt_addr好点？
    {
        printf("connect Error no.%d: %s\n", errno, strerror(errno));
    }
    else
    {
        printf("connecting...\n");
    }

    fputs("Operand count:",stdout);
    //数字 转 字符串（下面2行注意）
    scanf("%d",&opnd_cnt);
    opmsg[0]=(char)opnd_cnt;//只有一位，所以可以只认一个字符

    for(i=0;i<opnd_cnt;i++)
    {
        printf("Operand %d:",i+1);
        scanf("%d",&opmsg[i*OPSZ+1]);//原答案：scanf("%d",（int*)&opmsg[i*OPSZ+1]);不强转也行
        //确实不用回车也进能入下一行，输入数字回车才会显示下面的阿
    }
    getchar();

    fputs("Operator:",stdout);
    scanf("%c",&opmsg[opnd_cnt*OPSZ+1]);
    write(sock,opmsg,opnd_cnt*OPSZ+2);
    read(sock,&result,RLT_SIZE);//注意那个&result,read一个数的好方法
    printf("Operator result:%d\n",result);
    close(sock);
    return 0;
}

void error_handling(char* message)
{
    fputs(message,stderr);
    fputs("\n",stderr);
    exit(1);
}

