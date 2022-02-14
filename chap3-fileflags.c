#include "apue.h"
#include "error.c"
#include <fcntl.h>

int main(int argc, char *argv[])
{
	int		val;

	if (argc != 2)
    {
        err_quit("usage: a.out <descriptor#>");
    }  

    if ((val=fcntl(atoi(argv[1]),F_GETFL, 0))<0)
    {
        err_sys("fcntl error for fd %d", atoi(argv[1]));
    }

    switch (val & O_ACCMODE)
    {
        case O_RDONLY:
		printf("read only");
		break;

	    case O_WRONLY:
		printf("write only");
		break;

	    case O_RDWR:
		printf("read write");
		break;

        //case O_EXEC:
        //case O_SEARCH:  
        //库里没有
    
    }

    if (val & O_APPEND)
		printf(", append");
	if (val & O_NONBLOCK)
		printf(", nonblocking");
	if (val & O_SYNC)
		printf(", synchronous writes");
    if (val & O_DSYNC)
		printf(", synchronous writes(only Data)");
    if (val & O_RSYNC)
		printf("Synchronize reading and writing");

        putchar('\n');
	exit(0);

}
		