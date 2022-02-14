#include "apue.h"
#include <setjmp.h>
#include "error.c"
static void		sig_alrm(int);
static jmp_buf	env_alrm;

int
main(void)
{
	int		n;
	char	line[MAXLINE];

	if (signal(SIGALRM, sig_alrm) == SIG_ERR)
		err_sys("signal(SIGALRM) error");
	if (setjmp(env_alrm) != 0)
		err_quit("read timeout");

	alarm(10);
    printf("input some words\n");
	if ((n = read(STDIN_FILENO, line, MAXLINE)) < 0)
		err_sys("read error");
	alarm(0);

	write(STDOUT_FILENO, line, n);
	exit(0);
}

static void
sig_alrm(int signo)
{
	printf("in sig_alrm\n");
    longjmp(env_alrm, 1);
}
/*
[wqr@localhost chapter10]$ ./read2
input some words
（一段时间间隔）
in sig_alrm
read timeout
*/