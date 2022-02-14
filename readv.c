#include <stdio.h>
#include <sys/uio.h>
#define BUF_SIZE 100
void main(int argc,char *argv[])
{
    struct iovec vec[2];
    //初始化为空，学习！
    char buf1[BUF_SIZE]={0,};
    char buf2[BUF_SIZE]={0,};
    int str_len;

    printf("buf1:%s\n",buf1);
    printf("buf2:%s\n",buf2);
    vec[0].iov_base=buf1;
    vec[0].iov_len=5;
    vec[1].iov_base=buf2;
    vec[1].iov_len=BUF_SIZE;
    //通过readv函数口J 以由多个缓冲分别接收
    //需要将输入缓冲中的数据读入不同位置时，可以不必多次调用read函数，而是利用1次readv函数就能大大提髙效率
    str_len=readv(0,vec,2);
    printf("read bytes:%d\n",str_len);
    printf("first message:%s\n",buf1);
    printf("second message:%s\n",buf2);

}
/*
[wqr@localhost book_network_program]$ ./rv
buf1:
buf2:
I like TCP/TP socket programming~
read bytes:34
first message:I lik
second message:e TCP/TP socket programming~

*/