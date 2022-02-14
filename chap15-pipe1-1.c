#include "apue.h"
#include "error.c"
#include<sys/wait.h>
int main(void)
{
	int		n;
	int		fd[2];
	pid_t	pid;
	char	line[MAXLINE];

    if (pipe(fd) < 0)
    {
           err_sys("pipe error");
    }

	if ((pid = fork()) < 0) 
    {
		err_sys("fork error");
	} 
    else if (pid > 0) 
    {		/* parent */
        
        waitpid(pid,NULL,0);//少这个就不行
		 close(fd[1]);
		n = read(fd[0], line, MAXLINE);
		write(STDOUT_FILENO, line, n);
	} 
    else 
    {					/* child */
        close(fd[0]);
		write(fd[1], "hello world\n", 12);
	}
	exit(0);
}