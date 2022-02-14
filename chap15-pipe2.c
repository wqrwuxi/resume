#include "apue.h"
#include "error.c"
#include <sys/wait.h>

#define	DEF_PAGER	"/bin/more"		/* default pager program */

int
main(int argc, char *argv[])
{
	int		n;
	int		fd[2];
	pid_t	pid;
	char	*pager, *argv0;
	char	line[MAXLINE];
	FILE	*fp;

	if (argc != 2)
		err_quit("usage: a.out <pathname>");

	if ((fp = fopen(argv[1], "r")) == NULL)
		err_sys("can't open %s", argv[1]);

	if (pipe(fd) < 0)
		err_sys("pipe error");

	if ((pid = fork()) < 0) 
    {
		err_sys("fork error");
	} 
    else if (pid > 0) 
    {								/* parent */
		close(fd[0]);		/* close read end */

		/* parent copies argv[1] to pipe */
		while (fgets(line, MAXLINE, fp) != NULL) {
			n = strlen(line);
			if (write(fd[1], line, n) != n)
				err_sys("write error to pipe");
		}
		if (ferror(fp))
			err_sys("fgets error");

		close(fd[1]);	/* close write end of pipe for reader */

		if (waitpid(pid, NULL, 0) < 0)
			err_sys("waitpid error");
		exit(0);
	} 
    else 
    {										/* child */
		close(fd[1]);	/* close write end */
		if (fd[0] != STDIN_FILENO) {
			if (dup2(fd[0], STDIN_FILENO) != STDIN_FILENO)
				err_sys("dup2 error to stdin");
			close(fd[0]);	/* don't need this after dup2 */
        /*一开始是这样的
        close(fd[1]);
		n = read(fd[0], line, MAXLINE);
		write(STDOUT_FILENO, line, n);

        原先read为了打印（write+STDOUT_FILENO),现在read为了给pager程序做输入
        */
		}

		/* get arguments for execl() */
		if ((pager = getenv("PAGER")) == NULL)
			pager = DEF_PAGER;
		if ((argv0 = strrchr(pager, '/')) != NULL)
			argv0++;		/* step past rightmost slash */
		else
			argv0 = pager;	/* no slash in pager */
        
        printf("pager:%s\n",pager);
        printf("argv0:%s\n",argv0);

		if (execl(pager, argv0, (char *)0) < 0)
			err_sys("execl error for %s", pager);
	}
	exit(0);
}
/*
pager:/bin/more
argv0:more
*/
/*
[wqr@localhost chapter15]$ ./pipe2 test
1234567890
1234567890
1234567890
1234567890
123456qwertgb
4567890
1234567890
1234567890
1234567890
----------

[wqr@localhost chapter15]$ ./pipe2 test
1234567890
1234567890
1234567890
1234567890
123456qwertgb
4567890
1234567890
1234567890
1234567890
----------
--More--
*/

/*如果没有waitpid
[wqr@localhost chapter15]$ 1234567890
1234567890
1234567890
1234567890
123456qwertgb
4567890
1234567890
1234567890
1234567890
----------
^C
*/
