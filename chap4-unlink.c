#include "apue.h"
#include <fcntl.h>
#include "error.c"
int
main(void)
{
	if (open("tempfile", O_RDWR) < 0)
		err_sys("open error");
	if (unlink("tempfile") < 0)
		err_sys("unlink error");
	printf("file unlinked\n");
	sleep(20);
	printf("done\n");
	exit(0);
}