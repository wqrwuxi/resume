#include "apue.h"
#include <sys/wait.h>
#include "error.c"

char *env_init[] = { "USER=unknown", "/tmp", NULL };
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
       if(execlp("echoall","echoall","only argv1",(char*)0)<0)
       {
           err_sys("execlp error");
       }
    }
    
    

}