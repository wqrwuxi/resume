#include "apue.h"
#include <errno.h>

int main()
{
     char my[20]="my my my！";

	fprintf(stderr, "EACCES: %s\n", strerror(EACCES));//EACCES: Permission denied


	errno = ENOENT;
	perror(my);//my my my！: No such file or directory

    errno = EMFILE;
    perror(my);//my my my！: Too many open files
	exit(0);
}
//http://www.virtsync.com/c-error-codes-include-errno
