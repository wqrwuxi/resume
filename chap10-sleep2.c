#include	<setjmp.h>
#include	<signal.h>
#include	<unistd.h>
#include <stdio.h>

static jmp_buf	env_alrm;

static void
sig_alrm(int signo)
{
	printf("in sig_alrm\n");
	longjmp(env_alrm, 1);
}
int pause_value;
unsigned int
sleep2(unsigned int seconds)
{
	if (signal(SIGALRM, sig_alrm) == SIG_ERR)
		return(seconds);
	if (setjmp(env_alrm) == 0) {
		alarm(seconds);		/* start the timer */
		pause_value=pause();			/* next caught signal wakes us up */

		if(pause_value==-1)
	    {
           printf("pause %d\n",pause_value);
	    }
		printf("have passes pause\n");
	}
	return(alarm(0));		/* turn off timer, return unslept time */
}

void main()
{
	int value = sleep2(3);
	printf("return value:%d",value);
}
/*
[wqr@localhost chapter10]$ ./sleep2
(间隔一段时间)
in sig_alrm
return value:0[

所以pause()没有运行
*/

/*
[wqr@localhost chapter10]$ ./sleep2
in sig_alrm
return value:0

所以pause()没有运行
pause()要执行一个信号处理程序，并从其返回，返回值为-1  书P279
longjmp使信号处理程序提前结束 书P281
*/