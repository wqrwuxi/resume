#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<string.h>
int main()
{
	pid_t pid,pid_1,pid_2;
	int status;
	int fd[2];
	int ret=pipe(fd);
	if(ret==-1) 
	{perror("pipe error"); exit(1);}
 
	int i;
 
	for( i=0;i<2;i++)
	{
			pid=fork();
			if(pid==0&&i==0) //子进程1
			{
				pid_1=getpid();break;
			}	
			if(pid==0&&i==1) //子进程2
			{
				pid_2=getpid();break;
			}
	}
    
	if(pid==-1)
		{	perror("fork error\n");	exit(1); }
 
	if(i==0)//子进程1
	{
		close(fd[0]);
		//char *p="我是子进程1，我的pid=";
		//write(fd[1],p,strlen(p)+1);
char c[100]="";
		sprintf(c,"我是子进程:%d，我的pid=%d\n",i+1,pid_1);
		write(fd[1],&c,strlen(c)+1);
	}else if(i==1)//子进程2
	{
		waitpid(pid_1,&status,0);//等待子进程1执行完毕
		close(fd[0]);
		char d[100]="";
		sprintf(d,"我是子进程:%d，我的pid=%d\n",i+1,pid_2);
		write(fd[1],&d,strlen(d)+1);
	}
    
    if (pid > 0)//父进程
	{
		waitpid(pid_2,&status,0);
		close(fd[1]);
		char buf[100]={0};
		ret=read(fd[0],buf,sizeof(buf));
		close(fd[0]);
		write(STDOUT_FILENO,buf,ret);
	}
		return 0;
}

/*https://blog.csdn.net/quhongjuan12/article/details/106172448
编写程序，在程序中父进程先后创建子进程1和子进程2。父子三个进程并发执行，并利用同一个匿名管道通信：
两个子进程分别向管道写一则信息（内容自定，但要求包含子进程信息）；
父进程从管道读出信息并显示出来（要求先接收子进程1的信息）
*/
/*
[wqr@localhost chapter15]$ ./pipe1-2
我是子进程:2，我的pid=6475
我是子进程:1，我的pid=6474
*/
/*
waitpid P203

int sprintf(char *str, const char *format, ...)

*/