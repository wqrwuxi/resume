#include <stdio.h>
#include "apue.h"
#include "error.c"
#include <fcntl.h>//open函数原型
#define FILELEN 40
#define BUFFERSIZE 128
int main(void)
{
	FILE* fp;
    int fd=-1;
    char filename[FILELEN];
    char buf[BUFFERSIZE];
    int n;
    int pianyi;

    printf("input a filename\n");
    scanf("%s",filename);
    getchar();
    /*用fopen打开文件体现不了偏移量
    fp=fopen(filename,"a+");   
    fd=fileno(fp);*/
    fd=open(filename,O_RDWR);

    pianyi=lseek(fd, 50, SEEK_END);
    printf("偏移量：%d\n",pianyi);
    if ( pianyi== -1)
   // if (lseek(STDIN_FILENO, 0, SEEK_CUR) == -1)
   {
       printf("cannot seek\n");
   }
	else
    {
        printf("seek OK\n");
    }
    
    printf("print some word:\n");

    while((n=read(STDIN_FILENO,buf,BUFFERSIZE))>0)
    {
       if(write(fd,buf,n)!=n)   //ctrl+D结束输入
       {
           err_sys("write error");
       }

       if(n<0)
       {
        err_sys("read error");
       }


        printf("read的n：%d\n",n);

    }

    

   
		
	exit(0);
}
//https://www.cnblogs.com/cthon/p/9125575.html
//C语言之fileno()函数--获取已经打开的文件的文件描述符（小技巧）

 /*用fopen打开文件体现不了偏移量*/
/*SEEK_CUR
自己举例的文件原偏移量为0,所以lseek(fd, 0, SEEK_CUR)相当于从文件开始写，覆盖文件原有位置内容
lseek(fd, 50, SEEK_CUR)相当于相当于从开头偏移50开始输入我输入的内容
*/
/*SEEK_END
lseek(fd, 0, SEEK_END)从文档结束位置开始写入我输入的内容
lseek(fd, 10, SEEK_END)会从文档结束位置偏移10再写入内容，会形成空洞，现实出来就是一堆问号
*/
/*SEEK_SET
lseek(fd, 50, SEEK_SET)意味着从开头偏移50再输入我输入的内容
*/
