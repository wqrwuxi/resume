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
    char buf[1024];
    FILE* fp;
    int str_len;
    int read_cnt;

    if(argc!=3)
    {
        printf("Usage:%s <IP> <port>\n",argv[0]);
        exit(1);
    }
    
    fp=fopen("file_server.dat","wb");
    sock = socket(PF_INET,SOCK_STREAM,0);
    if (sock == -1)
    {
       printf("socket Error no.%d: %s\n", errno, strerror(errno));

    }

    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    if(connect(sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr))==-1)
    {
        printf("connect Error no.%d: %s\n", errno, strerror(errno));
    }
  

    //一开始忘改char buf[30]，结果就一直Segmentation fault (core dumped)，为什么
    //多种实验结果见result文档
    //Segmentation fault (core dumped) 来源  似乎是fwrite的buf不够用？？  但是1000/1024那种result是为什么？？
    //！！！！！！！！！！！！！！！！！！注意下面的，最终猜测
    /*https://linux.die.net/man/2/read
    一。read() attempts to read up to count bytes from file descriptor fd into the buffer starting at buf.
    the buffer starting at buf，说的是buf地址处开始的buffer,没说是多大的buffer，read(sock,buf,1024)里的buf大小根本没人关心！
    二。It is not an error if this number（the number of bytes read） is smaller than the number of bytes requested; (APUE书上也有说明该情况)
    this may happen for example because fewer bytes are actually available right now 
    (maybe because we were close to end-of-file, or because we are reading from a pipe, or from a terminal), or because read() was interrupted by a signal. 
    所以read第三个参数BUF_SIZE 或者 strlen(buf)都可以*/
    /*https://linux.die.net/man/3/fwrite   size_t fwrite(const void *ptr, size_t size, size_t nmemb,FILE *stream);
    一。The function fwrite() writes nmemb elements of data, each size bytes long, to the stream pointed to by stream, obtaining them from the location given by ptr.
    又是：the location given by ptr，是指针，没说某个多大的buf
    二。猜测的fwrite的Segmentation fault (core dumped) 原因
    read有一种错误方式 EFAULT：buf is outside your accessible address space.
    可能fwrite也有（没找到fwrite的error情况）,所以1000VS1024可以，但是500VS1024不行，因为差太多。
    其实这个动作很危险，根本没分配内存给buf
    三。因为read的内容很多没放入buf，剩余的还在缓冲里，write Thank you 时缓冲就先发出去了
    */
    while((read_cnt = read(sock,buf,1024))!=0)
    {
        printf("read_cnt:%d\n",read_cnt);
        int return_value=fwrite((void*)buf,1,read_cnt,fp);
       //fprintf(fp,"%s\n",buf);
        printf("return_value:%d\n",return_value);
    }

    printf("Received file data\n");
    write(sock,"Thank you",10);
    fclose(fp);
    close(sock);
    return 0;
}

void error_handling(char* message)
{
    fputs(message,stderr);
    fputs("\n",stderr);
    exit(1);
}

/*
 (1) 调用格式：fwrite(buf, sizeof(buf), 1, fp);
    成功写入返回值为1(即count)
    (2)调用格式：fwrite(buf, 1, sizeof(buf), fp);
    成功写入则返回实际写入的数据个数(单位为Byte)
*/

