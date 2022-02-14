#include "apue.h"
#include <sys/wait.h>
#include "error.c"

char *env_init[] = { "USER=unknown", "/tmp:/home/wqr/Documents", NULL };
int main(void)
{
    pid_t pid;
    pid=fork();

    if(pid<0)
    {
       err_sys("fork error");
    }
    else if(pid==0)
    {
       if(execle("/home/wqr/Documents/echoall","echoall","argv1","argv2",(char*)0,env_init)<0)
       {
           err_sys("execle error");
       }
    }
    
    if (waitpid(pid, NULL, 0) < 0)
		err_sys("wait error");//没这两句一直在上一个进程中

    pid=fork();

    if(pid<0)
    {
       err_sys("fork error");
    }
    else if(pid==0)
    {
       if(execlp("echoall","echoall","only argv1",(char*)0)<0)
       {
           err_sys("execlp error");
       }
    }
    
    

}