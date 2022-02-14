#include "apue.h"
#include "error.c"
#include <errno.h>

int		globvar = 6;		/* external variable in initialized data */
char	buf[] = "a write to stdout\n";

int
main(void)
{
	int		var;		/* automatic variable on the stack */
	pid_t	pid;

	var = 88;
	if (write(STDOUT_FILENO, buf, sizeof(buf)-1) != sizeof(buf)-1)
		err_sys("write error");
	printf("before fork\n");	/* we don't flush stdout */

    pid=fork();
    if(pid<0)
    {
        printf("wrong fork:%s\n",(strerror(errno)));
    }
    else if(pid==0)//child
    {
       globvar++;				/* modify variables */
		var++;
        //sleep(5);
    }
    else
    {
       //printf("parent pid = %ld,, glob = %d, var = %d\n", (long)getpid(), globvar,var);
       sleep(5);
    }

    printf("pid = %ld, glob = %d, var = %d\n", (long)getpid(), globvar,var);
	exit(0);

}
/*
[wqr@localhost chapter8]$ ./fork1
a write to stdout
before fork
parent pid = 15357,, glob = 6, var = 88
pid = 15358, glob = 7, var = 89
pid = 15357, glob = 6, var = 88(5秒后)
*/
/*原因见书，没那么简单  
如果变成子进程sleep(5)，父进程没有这个操作
wqr@localhost chapter8]$ ./fork1
a write to stdout
before fork
parent pid = 15527,, glob = 6, var = 88
pid = 15527, glob = 6, var = 88
[wqr@localhost chapter8]$ pid = 15528, glob = 7, var = 89
^C
*/