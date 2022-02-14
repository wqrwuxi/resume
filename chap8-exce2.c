#include "apue.h"
#include <sys/wait.h>
#include "error.c"

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
       //执行那个解释器文件，所以pathname是那个解释器文件
       //第一个参数一般是文件名，它默认带着信息的！不能少。
       //只显示第2/3个参数
       if(execl("/home/wqr/Documents/awk1","awk1","argv1","argv2",(char*)0)<0)
       {
           err_sys("execle error");
       }
    }
    
    if (waitpid(pid, NULL, 0) < 0)
		err_sys("wait error");//没这两句一直在上一个进程中
}
/*
[wqr@localhost chapter8]$ cat /home/wqr/Documents/awk1
#!/home/wqr/Documents/echoall[wqr@localhost chapter8]$ ./exce2
execle error: Permission denied
awk1要改权限
*/
/*
[wqr@localhost chapter8]$ cat /home/wqr/Documents/awk1
#!/home/wqr/Documents/echoall_awk1[wqr@localhost chapter8]$ ./exce2
argv[0]: /home/wqr/Documents/echoall_awk1
argv[1]: /home/wqr/Documents/awk1
argv[2]: argv1
argv[3]: argv2
*/