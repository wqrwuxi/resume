#include "apue.h"
#include "error.c"
static void	sig_quit(int);

int
main(void)
{
	sigset_t	newmask, oldmask, pendmask;

	if (signal(SIGQUIT, sig_quit) == SIG_ERR)
		err_sys("can't catch SIGQUIT");

	/*
	 * Block SIGQUIT and save current signal mask.
	 */
	sigemptyset(&newmask);
	sigaddset(&newmask, SIGQUIT);
    //sigprocmask执行完，新的屏蔽字为 原屏蔽字+newmask 中屏蔽字
    //sigprocmask的oset若非空，当前信号屏蔽字从oset返回。所以oldmask=老的的屏蔽字，没有SIGQUIT
	if (sigprocmask(SIG_BLOCK, &newmask, &oldmask) < 0) 
		err_sys("SIG_BLOCK error");

    printf("sigpromask one!\n");

	sleep(5);	/* SIGQUIT here will remain pending */
    
    
	if (sigpending(&pendmask) < 0)
		err_sys("sigpending error");
	if (sigismember(&pendmask, SIGQUIT))
		printf("\nSIGQUIT pending\n");
   
	/*
	 * Restore signal mask which unblocks SIGQUIT.
	 */
    //设置屏蔽字为oldmask，解除对SIGQUIT的屏蔽
    /*如果信号被设置成阻塞，且该信号的动作是系统默认动作或捕获该信号
    当该信号产生后，则进程将此信号的状态保持为未决(pending)状态,
    直到对该信号解除了阻塞或将该信号的动作改为忽略。

    解除阻塞（第二个sigpromask）后才处理第一个sleep内产生的^\信号，只是SIGQUIT的动作早于sigprocmask的返回
    */
	if (sigprocmask(SIG_SETMASK, &oldmask, NULL) < 0)
		err_sys("SIG_SETMASK error");
	printf("SIGQUIT unblocked\n");

	sleep(5);	/* SIGQUIT here will terminate with core file */
	exit(0);
}

static void
sig_quit(int signo)
{
	printf("caught SIGQUIT\n");
    //下面的if试过了，删除效果不变  默认处理（SIG_DFL）：信号由该特定信号的默认动作处理。
	if (signal(SIGQUIT, SIG_DFL) == SIG_ERR)
		err_sys("can't reset SIGQUIT");
}
/*
[wqr@localhost chapter10]$ ./critical
sigpromask one!
^\   （第一个sleep时输入，但是sleep会持续满5秒）
SIGQUIT pending
caught SIGQUIT
SIGQUIT unblocked
^\Quit (core dumped)（第二个sleep时输入，输入即退出）
*/