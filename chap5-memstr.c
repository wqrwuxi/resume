#include <stdio.h>
#include "apue.h"
#include "error.c"
#define BSZ 48
int main(void)
{
   FILE* fp;
   char buffer[BSZ];

  // memset(buffer,'A',BSZ);
   memset(buffer,'A',20);
   buffer[20]=0;
   fp=fmemopen(buffer,BSZ,"a+");
   printf("1:%p\n",fp);

   if(fp==NULL)
   {
       err_sys("fmemopen:");
   }
   
   printf("fmemopen,memset1:%s\n",buffer);//空
   memset(buffer,'a',BSZ-2);
   buffer[BSZ-2] = '\0';
   buffer[BSZ-1] = 'X';
   printf("fmemopen,memset2:%s\n",buffer);//46个a
   
   printf("2:%p\n",fp);
   fprintf(fp, "hello, world");
   printf("fmemopen,memset1,fprintf1:%s\n",buffer);//46个a
   printf("3:%p\n",fp);
   fflush(fp);//调用fflush、fclose、、fseek、fseeko、fsetpos会在当前位置添加null
   printf("4:%p\n",fp);
   printf("after fflush: %s\n", buffer);//hello, world
   printf("len of string in buf = %ld\n", (long)strlen(buffer));

    memset(buffer, 'b', BSZ-2);
	buffer[BSZ-2] = '\0';
	buffer[BSZ-1] = 'X';
    printf("memset2.1:%s\n",buffer);//46个b
    fprintf(fp, "hello, world");
    printf("memset2.1,fprintf1:%s\n",buffer);//46个b
	fseek(fp, 0, SEEK_CUR);//保持偏移值冲洗之后的位置
	printf("after  fseek: %s\n", buffer);//bbbbbbbbbbbbhello, world
	printf("len of string in buf = %ld\n", (long)strlen(buffer));

     memset(buffer, 'c', BSZ-2);
	buffer[BSZ-2] = '\0';
	buffer[BSZ-1] = 'X';
    printf("memset2.1:%s\n",buffer);
    fprintf(fp, "hello, world");
    printf("memset2.1,fprintf1:%s\n",buffer);
	fseek(fp, 0, SEEK_CUR);//保持偏移值冲洗之后的位置
	printf("after  fseek: %s\n", buffer);
	printf("len of string in buf = %ld\n", (long)strlen(buffer));
	
    memset(buffer, 'd', BSZ-2);
    buffer[BSZ-2] = '\0';
    buffer[BSZ-1] = 'X';
    fprintf(fp, "hello, world"); //继续写进去
    fclose(fp); //然后fclose在当前位置也就是数据尾端添加一个null
    printf("after  fclose: %s\n", buffer);




}