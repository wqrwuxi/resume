#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "apue.h"
#include "error.c"
int main(void)
{
    struct tm* tmp;
    int max=64;
    char buf1[max];
    char buf2[max];
    char buf3[20];
    time_t * pt;

    //time_t t=time(NULL);//书P163 图6.9 time函数(P161)从内核获取 日历时间time_t
    time(pt);
    tmp=gmtime(pt);//书P163 图6.9 日历时间分解时间  gmtime()和localtime()
     printf("ok");
    size_t size=strftime(buf1,max,"time and date: %c",tmp);
    printf("%s\n",buf1);
    printf("%d\n",size);
    strftime(buf2,max,"time and date: %F,%a,%X",tmp);
    printf("%s\n",buf2);
    printf("%d\n",size);
    if(strftime(buf3,20,"time and date: %c",tmp)==0)//返回success，好像这个是没错误检验
    {
        err_sys("buffer");
    }

}