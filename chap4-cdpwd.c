#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include "apue.h"
#include "error.c"
char* path_alloc()
{
   char *p = NULL;
   int size;

   p = malloc(256);
   if(p)
   {
     size = 256;
   }
   else
   {
     size = 0;
   }

    return p;
}
int main(void)  //函数说明：getcwd()会将当前的工作目录绝对路径复制到参数buf 所指的内存空间，参数size 为buf 的空间大小。
{
	char	*ptr;
	size_t		size;

	//if (chdir("/home/wqr/Documents") < 0)
		//err_sys("chdir failed");

	ptr = path_alloc();	/* our own function */ //https://blog.csdn.net/hbtj_1216/article/details/50832638
	if (getcwd(ptr, size) == NULL)
		err_sys("getcwd failed");

	printf("cwd = %s\n", ptr);
	exit(0);
}
/*
[wqr@localhost chapter4]$ ./cdpwd
cwd = /home/wqr/Documents/APUE_exercise/chapter4
*/