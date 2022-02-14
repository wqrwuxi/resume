#include "apue.h"
#include <fcntl.h>
#include "error.c"
#include<time.h>
 #include<sys/time.h>
int main(int argc, char *argv[])
{
    struct stat statbuf;
    struct timespec time[2];
    int fd;
    struct timespec ts;

    for (int i = 1; i < argc; i++)
    {
       
       if( stat(argv[i],&statbuf)<0 )
       {
          err_ret("%s: stat error", argv[i]);
			continue;
       }

       if( (fd=open(argv[i],O_RDWR | O_TRUNC))<0)
       {
           err_ret("%s:open error", argv[i]);
			continue;
       }
       
       //https://www.cnblogs.com/book-gary/p/3716790.html
       clock_gettime(CLOCK_REALTIME, &ts);
       time[0]=ts;
       //time[0]=statbuf.st_atim;
       time[1]=statbuf.st_mtim;

       if(futimens(fd,time)<0)
       {
           err_ret("%s: futimens error", argv[i]);
       }

       close(fd);
    }
}
/*  书P109 图4-19 与每个文件相关的时间值
什么都不做：
[wqr@localhost chapter4]$ ls -l foo
-rw-rw-rw-. 1 wqr wqr 0 Nov 11 19:09 foo
[wqr@localhost chapter4]$ ls -ul foo
-rw-rw-rw-. 1 wqr wqr 0 Nov 11 19:09 foo
[wqr@localhost chapter4]$ ls -lc foo
-rw-rw-rw-. 1 wqr wqr 0 Nov 11 19:09 foo

去看题目要求，time[0]故意那么处理的
time[0]=statbuf.st_atim
[wqr@localhost chapter4]$ ls -l foo
-rw-rw-rw-. 1 wqr wqr 0 Nov 11 19:09 foo
[wqr@localhost chapter4]$ ls -ul foo
-rw-rw-rw-. 1 wqr wqr 0 Nov 11 19:09 foo
[wqr@localhost chapter4]$ ls -cl foo
-rw-rw-rw-. 1 wqr wqr 0 Nov 11 20:12 foo

time[0]=ts;
[wqr@localhost chapter4]$ ls -cl foo
-rw-rw-rw-. 1 wqr wqr 0 Nov 11 20:14 foo
[wqr@localhost chapter4]$ ls -ul foo
-rw-rw-rw-. 1 wqr wqr 0 Nov 11 20:14 foo
[wqr@localhost chapter4]$ ls -l foo
-rw-rw-rw-. 1 wqr wqr 0 Nov 11 19:09 foo
*/