#include <stdio.h>
#include <sys/uio.h>

void main(int argc,char *argv[])
{
    struct iovec vec[2];
    char buf1[]="ABCDEFG";
    char buf2[]="1234567";
    int str_len;

    vec[0].iov_base=buf1;
    vec[0].iov_len=3;
    vec[1].iov_base=buf2;
    vec[1].iov_len=4;
    //writev函数用以将分散保存在多个缓冲中的数据一并发
    //即使仅从C语言角度看，减少函数调用次数也能相应提高性能。但其更大的意义在于减少数据包个数
    //其实writev函数在不采用Nagle 算法时更有价值
    str_len=writev(1,vec,2);
    printf("\n");
    printf("Write bytes:%d\n",str_len);

}
/*
[wqr@localhost book_network_program]$ ./wv
ABC1234
Write bytes:7
*/