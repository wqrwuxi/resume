#include "apue.h"
#include "error.c"
#include <sys/wait.h>
#include<unistd.h>
#include <time.h>
void sig_int(int signo)
{
	printf("interrupt\n%% ");
}
int main(void)
{
    char	buf[MAXLINE];	/* from apue.h */
	pid_t	pid;
	int	status;
      time_t t;

    if (signal(SIGINT, sig_int) == SIG_ERR)
    {
         err_sys("signal error");
    }
		

    printf("%% ");	/* print prompt (printf requires %% to print %) */

    scanf("%s",buf);
    
        
        if (buf[strlen(buf) - 1] == '\n')
        {
            buf[strlen(buf) - 1] = 0; /* replace newline with null */
        }

        if ((pid = fork()) < 0)
        {
           perror("fork() error");
        }
        else if (pid == 0)/* child */
        {
           printf("child begin!\n");
           //sleep(10);会sleep
           execlp(buf, buf, (char *)0);  //这东西什么意思？
           //sleep(10);不运行可能是因为waitpid在子进程结束或者stop处罚
           
           printf("child end!\n");
           
			//err_ret("couldn't execute: %s", buf);
			//exit(127);
        }
       
    
            if ((pid = waitpid(pid, &status, 0)) == -1)
            {
               perror("wait() error");
            }
            else if (pid > 0)
            {
            printf("a value of the process (usually a child) whose status information has been obtained:%d\n",pid);
            }
        
        
 
        //https://www.ibm.com/docs/en/zos/2.1.0?topic=functions-waitpid-wait-specific-child-process-end



        printf("%% ");
   
}

//https://www.ibm.com/docs/en/zos/2.1.0?topic=functions-waitpid-wait-specific-child-process-end
//waitpid():Suspends the calling process until a child process ends or is stopped.





