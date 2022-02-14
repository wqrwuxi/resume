#include <sys/wait.h>
#include <errno.h>
#include <stdio.h>
#include <stddef.h> //waitpid(pid, NULL, 0)
#include <stdlib.h>//exit
int main(void)
{
    pid_t pid;
    pid=fork();

    if(pid<0)
    {
        printf("wrong fork:%s\n",(strerror(errno)));
    }
    else if(pid==0)//first child
    {
        printf("first child id:%ld\n",(long)getpid());
        printf("first child return value 0:%ld\n",pid);//书P192 fork子进程的返回值是0
        pid=fork();
        
        if(pid<0)
        {
            printf("wrong fork:%s\n",(strerror(errno)));
        }
        else if(pid==0)
        {
            printf("second child id:%ld\n",(long)getpid());
            printf("second child return value 0:%ld\n",pid);
        }
        else if(pid>0)/* parent from second fork == first child */  // P192 fork 父进程返回的是新建子进程的进程ID
        {
            printf("parent from second fork value ?:%ld\n",pid);
            printf("first child end\n");
            exit(0);
        }
        
        
        sleep(5);//5秒足够 first child（second 的 parent)结束，由于fork的子进程写时复制，因此second child也会执行下面那句
                 //first child的结束，waitpid起作用，parent紧跟着结束
		printf("second child, parent pid = %ld\n", (long)getppid());//getppid父进程ID
		exit(0);
    }
    else if(pid>0)
    {
        printf("parent id:%ld\n",(getpid()));
        printf("parent's child id:%ld\n",pid); // P192 fork 父进程返回的是新建子进程的进程ID
    }

    if (waitpid(pid, NULL, 0) != pid)	/* wait for first child */
    {
        printf("waitpid error%s\n",(strerror(errno)));
    }
    else
    {
        printf("waitpid end child id:%ld\n",pid);
        printf("waitpid id:%ld\n",(long)getpid());
    }

    exit(0);
}
/*
[wqr@localhost chapter8]$ ./fork2
parent? id:17098
first child end
parent id:17098
[wqr@localhost chapter8]$ second child, parent pid = 1
^C
*/

/*
[wqr@localhost chapter8]$ ./fork2
parent id:11169
parent's child id:11170
-----------------------父进程/子进程哪个先调度不确定
first child id:11170
first child return value 0:0
parent from second fork value ?:11171
first child end
second child id:11171
second child return value 0:0
waitpid end child id:11170
waitpid id:11169
[wqr@localhost chapter8]$ second child, parent pid = 1
*/
/*
书P192 fork子进程的返回值是0 fork 父进程返回的是新建子进程的进程ID,
这些返回值对应pid

wait返回终止子进程的进程ID，waitpid函数返回终止子进程的进程ID
这些返回值对应pid
*/