#include <fcntl.h>
#include "apue.h"
#include "error.c"
int main(int argc, char *argv[])
{
    if (argc != 2)
    {
       err_quit("usage: ./t4-2 <pathname>");
    }

    if((access(argv[1],R_OK))<0)
    {
        err_ret("read access error for %s", argv[1]);
    }
    else
    {
        printf("read access OK\n");
    }

    if((open(argv[1],O_RDONLY))<0)
    {
        err_ret("open error for %s", argv[1]);
    }
    else
    {
        printf("open for read access OK\n");
    }
		
}

/*
[root@localhost chapter4]# ls -l /etc/shadow
----------. 1 root root 1390 Oct 18 17:32 /etc/shadow
[root@localhost chapter4]# ls -l t4-2
-rwxrwxr-x. 1 wqr wqr 22424 Nov 11 12:27 t4-2
[root@localhost chapter4]# chown root t4-2
[root@localhost chapter4]# ls -l t4-2
-rwxrwxr-x. 1 root wqr 22424 Nov 11 12:27 t4-2
[root@localhost chapter4]# exit
exit
[wqr@localhost chapter4]$ ./t4-2 /etc/shadow
read access error for /etc/shadow: Permission denied
open error for /etc/shadow: Permission denied
[wqr@localhost chapter4]$ su
Password: 
[root@localhost chapter4]# ./t4-2 /etc/shadow
read access OK
open for read access OK
[root@localhost chapter4]# chmod u+s t4-2
[root@localhost chapter4]# exit
exit
[wqr@localhost chapter4]$ ./t4-2 /etc/shadow
read access error for /etc/shadow: Permission denied
open for read access OK
*/

/*关于指令：chmod u+s t4-2
https://blog.csdn.net/modi000/article/details/114574583
s 在文件执行时把进程的属主或组ID置为该文件的文件属主。 
原来只有某用户可以执行httpd这条命令， u+s后其他用户都可享有文件属主的权限。通过chown将文件属主调整为root，这样其他用户即可以root权限操作该文件。
*/