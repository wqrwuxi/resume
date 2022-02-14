#include	<signal.h>
#include	<unistd.h>
#include <stdio.h>
#include <errno.h>

static void
sig_alrm(int signo)
{
	/* nothing to do, just return to wake up the pause */
    printf("SIGALRM received\n");
}

int pause_value;
unsigned int
sleep1(unsigned int seconds)
{
	if (signal(SIGALRM, sig_alrm) == SIG_ERR)
		return(seconds);
	alarm(seconds);		/* start the timer */
	pause_value=pause();			/* next caught signal wakes us up */
	
	if(pause_value==-1)
	{
       printf("pause %d\n",pause_value);
	}

   // alarm(0);/* turn off timer, return unslept time */试过了pause接受的不是这个信号
	return(alarm(0));	/* turn off timer, return unslept time */
}

void main()
{
    int value=sleep1(3);
    printf("return value:%d\n",value);
}
/*
[wqr@localhost chapter10]$ ./sleep1
（间隔一段时间）
SIGALRM received
return value:0
*/
/*
[wqr@localhost chapter10]$ ./sleep1
（间隔一段时间）
SIGALRM received
pause -1
return value:0
*/