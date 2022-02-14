#include "apue.h"
#include "error.c"

static void	sig_usr(int);	/* one handler for both signals */

int
main(void)
{
	if (signal(SIGUSR1, sig_usr) == SIG_ERR)
		err_sys("can't catch SIGUSR1");
	if (signal(SIGUSR2, sig_usr) == SIG_ERR)
		err_sys("can't catch SIGUSR2");
	for ( ; ; )
		pause();
}

static void
sig_usr(int signo)		/* argument is signal number */
{
	if (signo == SIGUSR1)
		printf("received SIGUSR1\n");
	else if (signo == SIGUSR2)
		printf("received SIGUSR2\n");
	else
		err_dump("received signal %d\n", signo);
}
/*
[wqr@localhost chapter10]$ ./sigusr &
[1] 14336
[wqr@localhost chapter10]$ kill -USR1 14336
received SIGUSR1
[wqr@localhost chapter10]$ kill -USR2 14336
received SIGUSR2
[wqr@localhost chapter10]$ kill -SIGUSR1 14336  重复接受信号会？
[wqr@localhost chapter10]$ kill 14336
bash: kill: (14336) - No such process
[1]+  User defined signal 1   ./sigusr   ?
*/

/*
[wqr@localhost chapter10]$ ./sigusr &
[1] 14932
[wqr@localhost chapter10]$ kill -SIGUSR1 14932
received SIGUSR1
[wqr@localhost chapter10]$ kill -SIGUSR2 14932
received SIGUSR2
[wqr@localhost chapter10]$ kill 14932
[1]+  Terminated              ./sigusr
*/

/*
[wqr@localhost chapter10]$ ./sigusr &
[1] 16441
[wqr@localhost chapter10]$ kill -USR2 16441
received SIGUSR2
[wqr@localhost chapter10]$ kill -USR1 16441
[wqr@localhost chapter10]$ received SIGUSR1
^C
[wqr@localhost chapter10]$ ./sigusr &
[2] 16723
[wqr@localhost chapter10]$ kill -USR2 16723
[wqr@localhost chapter10]$ received SIGUSR2
[wqr@localhost chapter10]$ kill -USR2 16723
[wqr@localhost chapter10]$ Kill 16723
bash: Kill: command not found...
Similar command is: 'kill'
[2]+  User defined signal 2   ./sigusr
[2]+  User defined signal 2   ./sigusr
[wqr@localhost chapter10]$ kill 16723
bash: kill: (16723) - No such process
[wqr@localhost chapter10]$ kill 16441
[wqr@localhost chapter10]$ ./sigusr &
[2] 17032
[1]   Terminated              ./sigusr
[wqr@localhost chapter10]$ kill 17032
[wqr@localhost chapter10]$ 
^C
*/

/*
wqr@localhost chapter10]$ ./sigusr &
[1] 17233
[wqr@localhost chapter10]$ kill -USR2 17233
received SIGUSR2
[wqr@localhost chapter10]$ kill -USR2 17233
[wqr@localhost chapter10]$ ./sigusr
^C[1]+  User defined signal 2   ./sigusr

[wqr@localhost chapter10]$ ./sigusr
^C
[wqr@localhost chapter10]$ kill 17233
bash: kill: (17233) - No such process
*/

/*
[wqr@localhost chapter10]$ ./sigusr &
[1] 17603
[wqr@localhost chapter10]$ kill -USR2 17603
[wqr@localhost chapter10]$ received SIGUSR2
^C
[wqr@localhost chapter10]$ kill -USR2 17603 
[wqr@localhost chapter10]$ kill 17603
bash: kill: (17603) - No such process
[1]+  User defined signal 2   ./sigusr
*/

/*
[wqr@localhost chapter10]$ ./sigusr &
[1] 17929
[wqr@localhost chapter10]$ ./sigusr &
[2] 18035
[wqr@localhost chapter10]$ kill -USR2 18035
[wqr@localhost chapter10]$ received SIGUSR2
^C
[wqr@localhost chapter10]$ kill -USR1 18035
[wqr@localhost chapter10]$ received SIGUSR1
^C
[wqr@localhost chapter10]$ kill 18035
[wqr@localhost chapter10]$ kill -USR1 17929
[2]+  Terminated              ./sigusr
[wqr@localhost chapter10]$ received SIGUSR1
^C
[wqr@localhost chapter10]$ kill -USR2 17929
[wqr@localhost chapter10]$ received SIGUSR2
^C
[wqr@localhost chapter10]$ kill 17929
[wqr@localhost chapter10]$ ./sigusr &
[2] 18487
[1]   Terminated              ./sigusr
[wqr@localhost chapter10]$ kill 18487
[wqr@localhost chapter10]$ ./sigusr &
[3] 18593
[2]   Terminated              ./sigusr
[wqr@localhost chapter10]$ kill USR2 18593
bash: kill: USR2: arguments must be process or job IDs
[wqr@localhost chapter10]$ kill -USR2 18593
bash: kill: (18593) - No such process
[3]+  Terminated              ./sigusr
[wqr@localhost chapter10]$ ./sigusr &
[1] 18784
[wqr@localhost chapter10]$ kill -USR2 18784
received SIGUSR2
[wqr@localhost chapter10]$ kill -USR1 18784
received SIGUSR1
[wqr@localhost chapter10]$ kill USR1
bash: kill: USR1: arguments must be process or job IDs
[wqr@localhost chapter10]$ kill 18784
[wqr@localhost chapter10]$ ./sigusr &
[2] 19040
[1]   Terminated              ./sigusr
wqr@localhost chapter10]$ kill -USR2 19040
[wqr@localhost chapter10]$ received SIGUSR2
^C
[wqr@localhost chapter10]$ kill 19040
[wqr@localhost chapter10]$ ./sigusr &
[3] 19257
[2]   Terminated              ./sigusr
*/

//还有USR1 USR2也行是为什么？
//&是个什么？