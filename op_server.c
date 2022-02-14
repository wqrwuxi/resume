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
#define RLT_SIZE 4
#define OPSZ 4

void error_handling(char* message);
int calculate(int opnum,int opnds[],char op);

int main(int argc,char *argv[])
{
    int serv_sock;
    int clnt_sock;
    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size;
    char message[BUF_SIZE];
    int str_len;
    char opinfo[BUF_SIZE];//不知道多大，内存分配到最大

    if(argc!=2)
    {
        printf("Usage:%s <port>\n",argv[0]);
        exit(1);
    }

    serv_sock = socket(PF_INET,SOCK_STREAM,0);//socket会少打两个字母导致错误的,因为变量名serv_sock
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

    if(listen(serv_sock,5)==-1)
    {
        printf("listen Error no.%d: %s\n", errno, strerror(errno));
    }
    
    for(int i=0;i<5;i++)
    {
        clnt_addr_size=sizeof(clnt_addr);//下面取地址，所以被拆开来了
        clnt_sock = accept(serv_sock,(struct sockaddr*)&clnt_addr,&clnt_addr_size);
        if(clnt_sock==-1)
        {
           printf("accept Error no.%d: %s\n", errno, strerror(errno));
        }
        else
        {
            printf("Connected client %d\n",i+1);
        }

        int opnd_cnt;
        //必须强转，不然可能不是那个char，是个很大的数。read函数原型第二个是void*，不一定认的出
        //所以表现为程序下面的用法决定了opnd_cnt的数值大小
        read(clnt_sock,(int*)&opnd_cnt,1);
        printf("opnd_cnt=%d\n",opnd_cnt);
        int opnds[opnd_cnt];

        int recv_len=0;
        while((opnd_cnt*OPSZ+1)>recv_len)
        {
            int recv_cnt = read(clnt_sock,&opinfo[recv_len],BUF_SIZE-1);
            printf("recv_cnt=%d,recv_len=%d\n",recv_cnt,recv_len);
            recv_len += recv_cnt;
        }
        //(int*)opinfo为什么直接就是数组了，可以自动忽略最后一个。疑惑非常疑惑？？？？？？？？？？？？？
        int result = calculate(opnd_cnt,(int*)opinfo,opinfo[recv_len-1]);
        write(clnt_sock,&result,sizeof(result));
        close(clnt_sock);
        
        /*自己的解法，可行
        int opnd_cnt;
        read(clnt_sock,&opnd_cnt,1);
        printf("opnd_cnt=%d\n",opnd_cnt);//这个数多少跟后面怎么用有关
        int opnds[opnd_cnt];
        char op;

        for(int j=0;j<opnd_cnt;j++)
        {
           read(clnt_sock,&opnds[j],OPSZ);
        }

        read(clnt_sock,&op,1);
        int result = calculate(opnd_cnt,opnds,op);
        write(clnt_sock,&result,sizeof(result));
        close(clnt_sock);*/
    }
    //只能一个client Q之后，再进一个clint，并且在第五个clint退出后，服务器才能结束
    
    close(serv_sock);
    return 0;
}

int calculate(int opnum,int opnds[],char op)
{
    int result=opnds[0];
    switch(op)
    {
        case '+':
            for(int i=1;i<opnum;i++)
            {
                result += opnds[i];
            }
            break;
        case '-':
            for(int i=1;i<opnum;i++)
            {
                result -= opnds[i];
            }
            break;
        case '*':
            for(int i=1;i<opnum;i++)
            {
                result *= opnds[i];
            }
            break;
    }
    return result;
}

void error_handling(char* message)
{
    fputs(message,stderr);
    fputs("\n",stderr);
    exit(1);
}
