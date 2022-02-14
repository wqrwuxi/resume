#include "apue.h"
#include "error.c"
#include "sleep2.c"

unsigned int	sleep2(unsigned int);
static void		sig_int(int);

int
main(void)
{
	unsigned int	unslept;

	if (signal(SIGINT, sig_int) == SIG_ERR)
		err_sys("signal(SIGINT) error");
	unslept = sleep2(3);
	printf("sleep2 returned: %u\n", unslept);
	exit(0);
}

static void
sig_int(int signo)
{
	int				i, j;
	volatile int	k;

	/*
	 * Tune these loops to run for more than 5 seconds
	 * on whatever system this test program is run.
	 */
	printf("\nsig_int starting\n");
	for (i = 0; i < 300000; i++)
		for (j = 0; j < 8000; j++)
			k += i * j;
	printf("sig_int finished\n");
}

/*
[wqr@localhost chapter10]$ ./tsleep2
^C
sig_int starting
(间隔一段时间)
sleep2 returned: 0
*/