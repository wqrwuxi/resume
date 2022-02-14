#include <stdio.h>
#include <sys/stat.h>
#include "apue.h"
#include "error.c"
int main(int argc,char* argv[])
{
    struct stat buf;

    for(int i=1;i<argc;i++)
    {
        printf("%s:",argv[i]);

        if(stat(argv[i],&buf)<0)
        {
            err_ret("lstat error");
			continue;
        }

        if((buf.st_mode & S_IFMT)==S_IFREG)
        {
            printf("regular\n");
        }
        else if((buf.st_mode & S_IFMT)==S_IFDIR)
        {
            printf("directory\n");
        }
        else if(S_ISCHR(buf.st_mode))//使用宏的表示
        {
            printf("character special\n");
        }
        else if(S_ISBLK(buf.st_mode))
        {
            printf("block special\n");
        }
        else if(S_ISFIFO(buf.st_mode))
        {
            printf("fifo\n");
        }
        else if(S_ISLNK(buf.st_mode))
        {
            printf("symbolic link\n");
        }
        else if(S_ISSOCK(buf.st_mode))
        {
            printf("socket\n");
        }
        else
        {
            printf("** unknown mode **");
        }
    }
}

/*  https://blog.csdn.net/astrotycoon/article/details/8679676
本文着眼于st_mode成员，该成员描述了文件的类型和权限两个属性。
st_mode是个32位的整型变量，不过现在的linux操作系统只用了低16位（估计是鉴于以后拓展的考虑）。
先看File type属性区域，位于bit12 ~ bit15.

在<sys/stat.h>中有如下定义：
#define S_IFMT  00170000
#define S_IFSOCK 0140000（二进制1100 0000 0000 0000
------------------
#define S_IFLNK  0120000（1010 0000 0000 0000
#define S_IFREG  0100000（1000 0000 0000 0000
#define S_IFBLK  0060000（0110 0000 0000 0000
#define S_IFDIR  0040000（0100 0000 0000 0000
#define S_IFCHR  0020000（0010 0000 0000 0000 
#define S_IFIFO  0010000（0001 0000 0000 0000
------以上为7个-----
#define S_ISUID  0004000（     1000 0000 0000
#define S_ISGID  0002000（     0100 0000 0000
#define S_ISVTX  0001000（     0010 0000 0000
 
#define S_ISLNK(m)      (((m) & S_IFMT) == S_IFLNK)
#define S_ISREG(m)      (((m) & S_IFMT) == S_IFREG)
#define S_ISDIR(m)      (((m) & S_IFMT) == S_IFDIR)
#define S_ISCHR(m)      (((m) & S_IFMT) == S_IFCHR)
#define S_ISBLK(m)      (((m) & S_IFMT) == S_IFBLK)
#define S_ISFIFO(m)     (((m) & S_IFMT) == S_IFIFO)
#define S_ISSOCK(m)     (((m) & S_IFMT) == S_IFSOCK)

首先S_IFMT是一个掩码，它的值是0170000（注意这里用的是八进制）(二进制1111 0000 0000 0000）， 可以用来过滤出前四位表示的文件类型。
其后的连续七个分别对应套接口文件、符号链接文件、普通文件、块设备、目录、字符设备、管道，它们分别对应一个不同的值。
现在假设我们要判断一个文件是不是目录，我们怎么做呢？
很简单，首先通过掩码S_IFMT把其他无关的部分置0，再与表示目录的数值比较，从而判断这是否是一个目录，下面的代码：
if ((info.st_mode & S_IFMT) == S_IFDIR)
    printf("this is a directory");
为了简便操作，<sys/stat.h>中提供了宏来代替上述代码，所以如果需要判断文件是不是目录就可以这样：
if (S_ISDIR(info.st_mode))
    printf("this is a directory");

*/