#include	<sys/wait.h>
#include	<errno.h>
#include	<unistd.h>
#include "apue.h"
#include "error.c"
#include "prexit.c"

int system(const char *cmdstring)	/* version without signal handling */
{
	pid_t	pid;
	int		status;

	if (cmdstring == NULL)
		return(1);		/* always a command processor with UNIX */

	if ((pid = fork()) < 0) 
    {
		status = -1;	/* probably out of processes */
       // printf("fork error %s\n",(strerror(errno)));
	} 
    else if (pid == 0) {				/* child */    //缩小点讲，其实一直运行的这个else if
		execl("/bin/sh", "sh","-c", cmdstring, (char *)0);
		_exit(127);		/* execl error */  //依旧显示exit status = 127
        
	} 
    else 
    {							/* parent */
		while (waitpid(pid, &status, 0) < 0) 
        {
			if (errno != EINTR) 
            {
				status = -1; /* error other than EINTR from waitpid() */
               // printf("waitpid error %s\n",(strerror(errno)));
				break;
			}
		}
	}

	return(status);
}

int main(void)
{
	int		status;

	if ((status = system("date")) < 0)
		err_sys("system() error");

	pr_exit(status);

	if ((status = system("nosuchcommand")) < 0)
		err_sys("system() error");

	pr_exit(status);

	if ((status = system("who; exit 44")) < 0)
		err_sys("system() error");

	pr_exit(status);

	exit(0);
}

/*
[wqr@localhost chapter8]$ ./system
Mon Nov 29 11:18:11 CST 2021
normal termination, exit status = 0
sh: nosuchcommand: command not found
normal termination, exit status = 127
wqr      tty2         2021-11-29 10:40 (tty2)
normal termination, exit status = 44

execl 中的 sh可以去除，一样执行，但是不建议，因为下面终端的演示说明需要sh。
shell程序是/bin/sh ，-c是重要选项，告诉shell程序取下一个命令参数
*/

/*
[wqr@localhost ~]$ sh -c date
Mon Nov 29 11:55:57 CST 2021
[wqr@localhost ~]$ sh -c who
wqr      tty2         2021-11-29 10:40 (tty2)
[wqr@localhost ~]$ sh -c who;exit 44
(然后终端退出)
*/

//exit 的数值似乎是根据情况固定的