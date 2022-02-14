#include "apue.h"
#include "error.c"

int		globvar = 6;		/* external variable in initialized data */

int
main(void)
{
	int		var;		/* automatic variable on the stack */
	pid_t	pid;

	var = 88;
	printf("before vfork\n");	/* we don't flush stdio */
	if ((pid = vfork()) < 0) {
		err_sys("vfork error");
	} else if (pid == 0) {		/* child */
		globvar++;				/* modify parent's variables */
		var++;
        printf("pid = %ld, glob = %d, var = %d\n", (long)getpid(), globvar,var);
		_exit(0);				/* child terminates */  //exit(0)结果一样？
	}

	/* parent continues here */
	printf("pid = %ld, glob = %d, var = %d\n", (long)getpid(), globvar,var);
	exit(0);
}
/*
[wqr@localhost chapter8]$ ./vfork
before vfork
pid = 5705, glob = 7, var = 89
pid = 5704, glob = 7, var = 89
*/