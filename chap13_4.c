// practice 4
#include <stdio.h>
#include <stdlib.h> // 提供 exit()的原型
//编写一个程序，按顺序在屏幕上显示命令行中列出的所有文件。使用argc控制循环。
// 循环+argc 没想到怎么用循环，抄的参考答案

//[wqr@localhost chapter13]$ gcc -o t4 chap13_4.c
//[wqr@localhost chapter13]$ ./t4 t1 t2 t3  (ti.t2.t3是文档名)
void main (int argc, char **argv)
{
    FILE *fp;
    char ch;
    for (int i = 1; i < argc; i++)
    {
        if ((fp = fopen(argv[i], "r")) == NULL)
        {
            printf("Can't open file %s as read!\n", argv[i]);
            exit(EXIT_FAILURE);
        }
 
        printf("%s:\n", argv[i]);
        while ((ch=getc(fp)) != EOF)
        {
            putc(ch, stdout);
        }
 
        printf("\n");
    }
 
    printf("Done!\n");
 
}