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
    int clnt_sock;
    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size;
    char buf[BUF_SIZE];
    FILE* fp;
    int str_len;
    int read_cnt;

    if(argc!=2)
    {
        printf("Usage:%s <port>\n",argv[0]);
        exit(1);
    }
    
    fp=fopen("file_server.c","rb");
    
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

    clnt_addr_size=sizeof(clnt_addr);
    clnt_sock = accept(serv_sock,(struct sockaddr*)&clnt_addr,&clnt_addr_size);
    
    while(1)
    {
        read_cnt = fread((void*)buf,1,BUF_SIZE,fp);//向套节字输出的内容为一个文件时
        if(read_cnt<BUF_SIZE)
        {
            write(clnt_sock,buf,read_cnt);
            printf("read_cnt:%d\n",read_cnt);
            break;
        }
        write(clnt_sock,buf,BUF_SIZE);
        printf("read_cnt:%d\n",BUF_SIZE);
    }
    //read_cnt:1024  read_cnt:1024  read_cnt:159  注意，修改程序会变的
    
    //==========================================注意的点！==========================================================
    //shutdown非常重要，相当于write了一个EOF
    //正常情况：serv9 进行一次write(write内容为fread得到的buf) -> clint9 read一次
    //while break后，到达shutdown  等价于 serv9 write一个EOF -> clint9 read返回0（去看read函数），于是跳出while
    shutdown(clnt_sock,SHUT_WR);
    read(clnt_sock,buf,BUF_SIZE);
    printf("Message from client:%s\n",buf);

    fclose(fp);
    close(clnt_sock);
    close(serv_sock);
    return 0;
}

void error_handling(char* message)
{
    fputs(message,stderr);
    fputs("\n",stderr);
    exit(1);
}
