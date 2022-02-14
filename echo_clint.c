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
    struct sockaddr_in serv_addr;
    char message[BUF_SIZE];
    char message1[BUF_SIZE];
    int str_len;

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

    while(1)
    {
        fputs("Input message(Q to quit):\n",stdout);
        fgets(message,BUF_SIZE,stdin);
        printf("fgets message:%d   %s\n",strlen(message),message);

        if(strcmp(message,"q\n")==0 || strcmp(message,"Q\n")==0)
        {
            break;
        }
        
        //一定要用strlen测量字符串长度，szieof求值为BUF_SIZE
        int test1=write(sock,message,strlen(message));
        int test2=read(sock,message,BUF_SIZE);
        message[test2]=0;
        //int test2=read(sock,message1,strlen(message1));
        printf("test1:%d test2:%d\n",test1,test2);
        printf("size:%d  Message from server:%s\n",strlen(message),message);
    }
    
    close(sock);
    return 0;
}

void error_handling(char* message)
{
    fputs(message,stderr);
    fputs("\n",stderr);
    exit(1);
}

/*read第三个参数有致命误解
当：
加入：char message1[BUF_SIZE];
且经修改后：int test2=read(sock,message1,strlen(message1));
printf("size:%d  Message from server:%s\n",strlen(message1),message1);

[wqr@localhost book_network_program]$ ./serv3 9190
Connected client 1
str_len:8
str_len:7
str_len:-1
[wqr@localhost book_network_program]$ ./clint3 127.0.0.1 9190
connecting...
Input message(Q to quit):
htgtrgr
test1:8 test2:6       //test2长度注意
size:6  Message from server:htgtrgInput message(Q to quit):
asdfgh
test1:7 test2:2      //test2长度注意
size:6  Message from server:r
gtrgInput message(Q to quit):
^C

解析：事实上，test1和test2是个凑巧！！因为用了一个message
当read的message换为没有任何初始化的message1时，strlen(message1)是本来的message1，不是read过来的那个我要的char数组

*/

