#include "apue.h"
#include "error.c"
int main(void)
{
    struct stat	statbuf;
    if(stat("foo",&statbuf)<0);
    {
        err_sys("stat error for foo");
    }
    if (chmod("foo", (statbuf.st_mode & ~S_IXGRP) | S_ISGID) < 0)
    {
        err_sys("chmod error for foo");
    }

    if (chmod("bar", S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) < 0)
		err_sys("chmod error for bar");

	exit(0);
		
}
/*
[wqr@localhost chapter4]$ ls -l foo
-rw-rw-rw-. 1 wqr wqr 0 Nov 11 16:38 foo
[wqr@localhost chapter4]$ ls -l bar
-rw-------. 1 wqr wqr 0 Nov 11 16:38 bar
*/