///////////////////////////////////////////////////////////////////////////////
//头文件区域，所需要的头文件，添加到这里。
#include <stdio.h>
#include <string.h>
#include <stdlib.h>//malloc
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

///////////////////////////////////////////////////////////////////////////////
//宏定义
#define MAX_LINE_LEN 256
#define WORD_MAX_NUM 20
#define WORD_MAX_LEN 25
#define COMMAND_HISTORY_NUM 11

///////////////////////////////////////////////////////////////////////////////
//结构体
typedef struct{
	char *data[COMMAND_HISTORY_NUM];
	int front;
	int rear;
	int size;	//队列当前长度 
}SeqQueue;

///////////////////////////////////////////////////////////////////////////////
//变量声明
SeqQueue command_Q;
char word[WORD_MAX_NUM][WORD_MAX_LEN];
int word_count=0;

///////////////////////////////////////////////////////////////////////////////
//函数声明

void help_handle()
{
   printf("Type program names and arguments, and hit enter.\n");
   printf("The following are built in:\n");
   printf("  help\n");
   printf("  history\n");
   printf("  cd\n");
   printf("  pwd\n");
   printf("  exit\n");
   printf("Use the man command for information on other programs.\n");
}

void history_handle(char* command_now)
{
   //队列的实现 https://blog.csdn.net/qq_44695386/article/details/119494204
   if (command_Q.size==COMMAND_HISTORY_NUM)
   {
       command_Q.front = (command_Q.front + 1) % COMMAND_HISTORY_NUM; 
       command_Q.size--;
   }


   if(command_Q.size<COMMAND_HISTORY_NUM)
   {
       int len=strlen(command_now);
       command_Q.data[command_Q.rear]=(char*)malloc(sizeof(char)*len);
       strcpy(command_Q.data[command_Q.rear],command_now);	
	   command_Q.rear = (command_Q.rear + 1) % COMMAND_HISTORY_NUM; 
	   command_Q.size++;
   }
}

void cd_handle(char* path)
{
   int ret=chdir(path);
   if(ret==-1)
   {
      printf("Error in cd %s\n",(strerror(errno)));
   }
}

void pwd_handle()
{
    char res[MAX_LINE_LEN];   
    getcwd(res, MAX_LINE_LEN);
    printf("%s\n",res);
}

void program_execut()
{
    pid_t pid;

    if((pid=fork())<0)
    {
        perror("Problem creating fork.\n");
        exit(EXIT_FAILURE);
    }
    else if(pid==0)//必须用子进程，不然execvp如何返回，显示binsh>
    {
        char** argv;

        for(int i=0;i<word_count+1;i++)
        {
            int len=strlen(word[i]);
            argv[i]=(char*)malloc(sizeof(char)*len);
            strcpy(argv[i],word[i]);
        }
        
        argv[word_count+1]=NULL;//注意点1,argv最后的SNULL
        execvp(word[0],argv);
        _exit(EXIT_SUCCESS);//注意点3 
    }
    else
    {
        waitpid(pid,NULL,0);//注意点2
    }
}

void output_redirect()
{
    int fd;
    int out;
    pid_t pid;

    for(int i=1;i<word_count+1;i++)
    {
        if(strcmp(word[i],">")==0)
        {
            out=i;
        }
    }

    if((pid=fork())<0)
    {
        perror("Problem creating fork.\n");
        exit(EXIT_FAILURE);
    }
    else if(pid==0)
    {
        char** argv;
        
        fd=open(word[out+1],O_WRONLY | O_CREAT);//注意点1：O_CREAT
        
        if(fd<0)
        {
            printf("Error in output_redirect open :%s\n",(strerror(errno)));
        }

        if(fd!=STDOUT_FILENO)
        {
            if (dup2(fd,STDOUT_FILENO) != STDOUT_FILENO)//将当前进程的标准输出换为fd
            {
                printf("Error in output_redirect dup2 :%s\n",(strerror(errno)));
            }
        }

        for(int i=0;i<out;i++)
        {
            int len=strlen(word[i+1]);
            argv[i]=(char*)malloc(sizeof(char)*len);
            strcpy(argv[i],word[i+1]);
        }

        argv[out]=NULL;
        execvp(word[0],argv); 
        _exit(EXIT_SUCCESS);
        close(fd);
    }
    else
    {
        waitpid(pid,NULL,0);//注意点2
    }
}

void input_redirect()
{
    int fd;
    int in;
    pid_t pid;

    for(int i=1;i<word_count+1;i++)
    {
        if(strcmp(word[i],"<")==0)
        {
            in=i;
        }
    }

    fd=open(word[in+1],O_RDONLY);

    if(fd<0)
    {
        printf("Error in input_redirect open :%s\n",(strerror(errno)));
    }

    if((pid=fork())<0)
    {
        perror("Problem creating fork.\n");
        exit(EXIT_FAILURE);
    }
    else if(pid==0)
    {
        char** argv;
        
        if(fd!=STDIN_FILENO)
        {
            if (dup2(fd, STDIN_FILENO) != STDIN_FILENO)
            {
                printf("Error in input_redirect dup2 %s\n",(strerror(errno)));
            }
        }

        for(int i=0;i<in;i++)
        {
            int len=strlen(word[i]);
            argv[i]=(char*)malloc(sizeof(char)*len);
            strcpy(argv[i],word[i]);
        }
        
        argv[in]=NULL;
        execvp(word[0],argv);
        _exit(EXIT_SUCCESS);
    }
    else
    {
        waitpid(pid,NULL,0);//注意点2
    }

    close(fd);
}

void pipe_execute()
{
    int fd[2];
    int middle;
    pid_t pid;

    for(int i=1;i<word_count+1;i++)
    {
        if(strcmp(word[i],"|")==0)
        {
            middle=i;
        }
    }

    if(pipe(fd)<0)
    {
        printf("Error in pipe: %s\n",(strerror(errno)));
    }

    if((pid=fork())<0)
    {
        perror("Problem creating fork.\n");
        exit(EXIT_FAILURE);
    }
    else if(pid==0)//固定下来：子进程写，父进程读（需要waitpid)
    {
        char* argv1[middle];

        for(int i=0;i<middle;i++)
        {
            int len=strlen(word[i]);
            argv1[i]=(char*)malloc(sizeof(char)*len);
            strcpy(argv1[i],word[i]);
        }
        argv1[middle]=NULL;

        if(fd[1]!=STDOUT_FILENO)
        {
            if (dup2(fd[1],STDOUT_FILENO) != STDOUT_FILENO)
            {
                perror("Error in input_redirect dup2");
            }
        }

        close(fd[0]);
        execvp(word[0],argv1);
        close(fd[1]);
        _exit(EXIT_SUCCESS);
        
    }
    else if(pid>0)
    {
        waitpid(pid,NULL,0);
        
        char* argv2[word_count-middle+1];

        for(int i=0;i<word_count-middle;i++)
        {
            int len=strlen(word[middle+i+1]);
            argv2[i]=(char*)malloc(sizeof(char)*len);
            strcpy(argv2[i],word[middle+i+1]);
        }
        argv2[word_count-middle]=NULL;

        if(fd[0]!=STDIN_FILENO)
        {
            if (dup2(fd[0],STDIN_FILENO) != STDIN_FILENO)
            {
                printf("Error in pipe STDIN dup2 %s\n",(strerror(errno)));
            }
        }
       
        close(fd[1]);
        execvp(word[middle+1],argv2);
        close(fd[0]);
    }
    
}

int background_run()
{
    pid_t pid;
    int middle=0;

    for(int i=1;i<word_count+1;i++)
    {
        if(strcmp(word[i],"&")==0)
        {
            middle=i;
        }
    }

    pid = fork();
    if (pid < 0) 
    {
        perror("background_run fork()");
        exit(1);
    }
    else if (pid == 0) 
    {
        char* argv1[middle];
        for(int i=0;i<middle;i++)
        {
           int len=strlen(word[i]);
           argv1[i]=(char*)malloc(sizeof(char)*len);
           strcpy(argv1[i],word[i]);
        }
        argv1[middle]=NULL;

        execvp(word[0], argv1);
        // exit函数终止当前进程, 括号内参数为1时, 会像操作系统报告该进程因异常而终止
        _exit(EXIT_SUCCESS);
    }
    else 
    {
        if(middle==word_count)
        {
           // exit(0);
        }
        else
        {
            char* argv2[word_count-middle+1];

            for(int i=0;i<word_count-middle;i++)
            {
               int len=strlen(word[middle+i+1]);
               argv2[i]=(char*)malloc(sizeof(char)*len);
               strcpy(argv2[i],word[middle+i+1]);
            }
            argv2[word_count-middle]=NULL;
            execvp(argv2[0], argv2);
            return 0;

        }
        
    }
}

void multiple_handle()
{
   pid_t pid;
   int middle=0;
   
   for(int i=1;i<word_count+1;i++)
   {
       if(strcmp(word[i],";")==0)
       {
           middle=i;
       }
   }
 
   pid = fork();
    if (pid < 0) 
    {
        perror("multiple_handle fork()");
        exit(1);
    }
    else if (pid == 0) 
    {
        char* argv1[middle];
        for(int i=0;i<middle;i++)
        {
           int len=strlen(word[i]);
           argv1[i]=(char*)malloc(sizeof(char)*len);
           strcpy(argv1[i],word[i]);
        }
        argv1[middle]=NULL;

        execvp(argv1[0], argv1);
        // exit函数终止当前进程, 括号内参数为1时, 会像操作系统报告该进程因异常而终止
        _exit(EXIT_SUCCESS);
    }
    else 
    {
        char* argv2[word_count-middle+1];

        for(int i=0;i<word_count-middle;i++)
        {
            int len=strlen(word[middle+i+1]);
            argv2[i]=(char*)malloc(sizeof(char)*len);
            strcpy(argv2[i],word[middle+i+1]);
        }
        argv2[word_count-middle]=NULL;

        execvp(argv2[0], argv2);
        waitpid(pid,NULL,0);
    }



}

void input()
{
    char line[MAX_LINE_LEN];
    int len=0;
    word_count=0;
    int char_count=0;
    
    //不初始化word[i]就是乱码
    for(int i=0;i<WORD_MAX_NUM;i++)
    {
        for(int j=0;j<WORD_MAX_LEN;j++)
        {
            word[i][j]='\0';
        }
    }

    //不适合用scanf("%s",..),最后那个回车scanf自动跳过。文件内内容适合scanf
    fgets(line,MAX_LINE_LEN,stdin);
    len=strlen(line);
 
    for(int i=0;i<len-1;i++)
    {
       if(line[i]!=' ')
       {
           word[word_count][char_count++]=line[i];
       }
       else
       {
            char_count=0;
            word_count++;
       }
    }

    history_handle(line);
}

void handle()
{
    int flag=0;

    for(int i=1;i<word_count+1;i++)
    {
        if(strcmp(word[i],">")==0)
        {
            flag=1;
        }
        else if(strcmp(word[i],"<")==0)
        {
            flag=2;
        }
        else if(strcmp(word[i],"|")==0)
        {
            flag=3;
        }
        else if(strcmp(word[i],"&")==0)
        {
            flag=4;
        }
        else if(strcmp(word[i],";")==0)
        {
            flag=5;
        }
    }
    
    if(flag==1)
    {
        output_redirect();
    }
    else if(flag==2)
    {
        input_redirect();
    }
    else if(flag==3)
    {
        pipe_execute();
    }
    else if(flag==4)
    {
        background_run();
    }
    else if(flag==5)
    {
        multiple_handle();
    }
    else if(strcmp(word[0],"help")==0 && word_count==0)
    {
        help_handle();
    }
    else if(strcmp(word[0],"history")==0 && word_count==0)
    {
        int m=command_Q.front;
        for(int j=0;j<command_Q.size-1;j++)
        {
           printf("%d %s",j,command_Q.data[m]);
           m = (m+1) % COMMAND_HISTORY_NUM;
        }
    }
    else if(strcmp(word[0],"!!")==0 && word_count==0)
    {
        printf("%s",command_Q.data[command_Q.rear-2]);
    }
    else if(strcmp(word[0],"cd")==0 && word_count==1)
    {
        cd_handle(word[1]);
    }
    else if(strcmp(word[0],"pwd")==0 && word_count==0)
    {
        pwd_handle();
    }
    else if(strcmp(word[0],"exit")==0 && word_count==0)
    {
        exit(0);
    }
    else
    {
        program_execut();
    }
}

int main() 
{
    
    command_Q.front=0;
    command_Q.rear=0;
    command_Q.size=0;
    
    while(1)
    {
        printf("binsh>");
        input();
        handle();
    }
}
