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
	if ((pid = fork()) < 0) {
		err_sys("vfork error");
	} else if (pid == 0) {		/* child */
		globvar++;				/* modify parent's variables */
		var++;
        printf("child:pid = %ld, glob = %d, var = %d\n", (long)getpid(), globvar,var);
		_exit(0);				/* child terminates */  //exit(0)结果一样？
	}
    else{
        sleep(2);
        printf("parent1:pid = %ld, glob = %d, var = %d\n", (long)getpid(), globvar,var);
    }

	/* parent continues here */
	printf("parent2:pid = %ld, glob = %d, var = %d\n", (long)getpid(), globvar,var);
	exit(0);
}
/*
before vfork
child:pid = 4649, glob = 7, var = 89
parent1:pid = 4648, glob = 6, var = 88
parent2:pid = 4648, glob = 6, var = 88
*/