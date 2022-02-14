#include <stdio.h>
#include "apue.h"
#include "error.c"
int main(void)
{
    int		c; //这个c怪怪的
    char ch;
    ch=getc(stdin);
    printf("ch=getc(stdput):%c",ch);
    printf("\n");
    putc('A',stdout);

    while((c=getc(stdin))!=EOF)
    {
       if(putc(c,stdout)==EOF)
       {
           err_sys("putc");
       }

    }
    if (ferror(stdin))
		err_sys("input error");
}