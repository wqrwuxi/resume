#include	"apue.h"
#include	<sys/wait.h>

static void	sig_cld(int);

int main()
{
	pid_t	pid;

	if (signal(SIGCLD, sig_cld) == SIG_ERR) //SIGCHLD结果一样
		perror("signal error");

	if ((pid = fork()) < 0) 
    {
		perror("fork error");
	} 
    else if (pid == 0) 
    {		/* child */
		printf("child pid:%d",(getpid()));//为什么不打印
        sleep(2);
		_exit(0);
	}
   /*else//这个else是自己的版本，因为子进度不打印加的
    {
      printf("parent pid:%d\n",(getpid()));
      sleep(4);
    }*/

	pause();	/* parent */ //去除这句直接退出，这是原版程序
	exit(0);
}

static void sig_cld(int signo)	/* interrupts pause() */
{
	pid_t	pid;
	int		status;

	printf("SIGCLD received\n");

	/*
    if (signal(SIGCLD, sig_cld) == SIG_ERR)	//reestablish handler 
		perror("signal error");
    去除了无差别？
    */

	if ((pid = wait(&status)) < 0)		/* fetch child status */
		perror("wait error");

	printf("pid = %d\n", pid);
}

/*
[wqr@localhost chapter10]$ ./child
(2秒空白)
SIGCLD received
pid = 22002
*/