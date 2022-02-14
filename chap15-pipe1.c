#include "apue.h"
#include "error.c"

int main(void)
{
	int		n;
	int		fd[2];
	pid_t	pid;
	char	line[MAXLINE];

	if (pipe(fd) < 0)
		err_sys("pipe error");
	if ((pid = fork()) < 0) {
		err_sys("fork error");
	} else if (pid > 0) {		/* parent */
		close(fd[0]);
		write(fd[1], "hello world\n", 12);
        close(fd[1]);
	} else {					/* child */
		close(fd[1]);
		n = read(fd[0], line, MAXLINE);
		write(STDOUT_FILENO, line, n);
        close(fd[0]);
	}
	exit(0);
}
/*
[wqr@localhost chapter15]$ ./pipe1
hello world


*/