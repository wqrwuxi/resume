#include "apue.h"
#include "error.c"
#include "prmask.c"
volatile sig_atomic_t	quitflag;	/* set nonzero by signal handler */

static void
sig_int(int signo)	/* one signal handler for SIGINT and SIGQUIT */
{
	if (signo == SIGINT)
		printf("\ninterrupt\n");
	else if (signo == SIGQUIT)
		quitflag = 1;	/* set flag for main loop */
}

int
main(void)
{
	sigset_t	newmask, oldmask, zeromask;

	if (signal(SIGINT, sig_int) == SIG_ERR)
		err_sys("signal(SIGINT) error");
	if (signal(SIGQUIT, sig_int) == SIG_ERR)
		err_sys("signal(SIGQUIT) error");

	sigemptyset(&zeromask);
	sigemptyset(&newmask);
	sigaddset(&newmask, SIGQUIT);

	/*
	 * Block SIGQUIT and save current signal mask.
	 */
	if (sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0)
		err_sys("SIG_BLOCK error");
    
    pr_mask("before sigsuspend ");
	while (quitflag == 0)
		sigsuspend(&zeromask);
    pr_mask("after sigsuspend ");

	/*
	 * SIGQUIT has been caught and is now blocked; do whatever.
	 */
	quitflag = 0;

	/*
	 * Reset signal mask which unblocks SIGQUIT.
	 */
    pr_mask("before last sigpromask ");
	if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0)
		err_sys("SIG_SETMASK error");
    pr_mask("after last sigpromask ");

	exit(0);
}
/* 
[wqr@localhost chapter10]$ ./suspend2
before sigsuspend  SIGQUIT
(没有动静直到按^C)（解释：sigsuspend挂起当前进程）
^C
interrupt（自己理解：停留在sigsuspend）
(没有动静直到按^C)
^C
*/

/*
[wqr@localhost chapter10]$ ./suspend2
before sigsuspend  SIGQUIT
(没有动静直到按^C)（解释：sigsuspend挂起当前进程）
^C
interrupt（自己理解：停留在sigsuspend）
(没有动静直到按^\)
^\after sigsuspend  SIGQUIT(quitflag = 1,不进入含sigsuspend的while，向下运行)
before last sigpromask  SIGQUIT
after last sigpromask 
*/

/*
[wqr@localhost chapter10]$ ./suspend2
before sigsuspend  SIGQUIT
(没有动静直到按^\)（解释：sigsuspend挂起当前进程）
^\after sigsuspend  SIGQUIT(quitflag = 1,不进入含sigsuspend的while，向下运行)
before last sigpromask  SIGQUIT
after last sigpromask 
*/