#include	<stdio.h>
#include <stdlib.h>

char* t;
char* p;
FILE *open_data(void)
{
	FILE	*fp;
	char	databuf[BUFSIZ];  /* setvbuf makes this the stdio buffer */ //声明自动变量的函数已经返回后，不能再引用这些自动变量
    char test[10]={"qwe"};
    //char* databuf=malloc(BUFSIZ*sizeof(char*));
	if ((fp = fopen("datafile", "r")) == NULL)
		return(NULL);
	if (setvbuf(fp, databuf, _IOLBF, BUFSIZ) != 0)
		return(NULL);
    t=databuf;
    p=test;
    printf("%s END\n",databuf);
    printf("databuf:%u\n",&databuf);
    printf("t:%u\n",&t);//t和databuf不是一个地址
    //free(databuf);
	return(fp);		/* error */ 
}

void main()
{
    FILE* fp;
    fp=open_data();//当open_data返回时，它栈上所使用的空间将由下一个被调用的函数的栈帧使用
    //标准I/O库函数仍将使用这部分存储空间作为该流的缓冲区
    fp = fopen("datafile2", "r");
    printf("|%s|",t);
    printf("|%s|",p);
    //printf("|%s|",test);

}
/*
不用malloc:
[wqr@localhost chapter7]$ ./opendata
 END

 用malooc:
 [wqr@localhost chapter7]$ ./opendata
 END
�$���[wqr@localhost chapter7]$ 
*/

//声明自动变量的函数已经返回后，不能再引用这些自动变量
//当open_data返回时，它栈上所使用的空间将由下一个被调用的函数的栈帧使用
//标准I/O库函数仍将使用这部分存储空间作为该流的缓冲区————————————麻烦所在
//书P128 如果一个函数分配一个自动变量类的标准I/O缓冲区，则该函数返回前，必须关闭该流
//自动变量 C语言书P783-784 自动变量在离开函数时销毁