#include "apue.h"
#include "error.c"
static void	sig_alrm(int);

int main(void)
{
	int		n;
	char	line[MAXLINE];

	if (signal(SIGALRM,sig_alam)==SIG_ERR)
	{
        err_sys("signal(SIGALRM) error");
	}

	alarm(10);
    printf("input some words\n");
	if ((n = read(STDIN_FILENO, line, MAXLINE)) < 0)
		err_sys("read error");
	alarm(0);

	write(STDOUT_FILENO, line, n);
	exit(0);
}

static void sig_alrm(int signo)
{
	/* nothing to do, just return to interrupt the read */
	printf("SIGALRM!\n");

}
/*
[wqr@localhost chapter10]$ ./read1
input some words
（一段时间间隔）
SIGALRM!
read error: Interrupted system call
[wqr@localhost chapter10]$ ./read1
input some words
qwerty
qwerty
*/