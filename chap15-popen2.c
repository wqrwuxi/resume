#include "apue.h"
#include <sys/wait.h>

#define	PAGER	"${PAGER:-more}" /* environment variable, or default */

int
main(int argc, char *argv[])
{
	char	line[MAXLINE];
	FILE	*fpin, *fpout;

	if (argc != 2)
		err_quit("usage: a.out <pathname>");
	if ((fpin = fopen(argv[1], "r")) == NULL)//相当于父进程，是写端。pip2.c中父进程核心为20-21行+35-39行。这里为20-21行
		err_sys("can't open %s", argv[1]);

	if ((fpout = popen(PAGER, "w")) == NULL)//如果是popen的type是w，文件指针连接到cmdstring（子进程）的标准输入【子进程是读端，读端读是为了给cmdstring作写入.popen.c54-60行写了机制
		err_sys("popen error");

	/* copy argv[1] to pager */
	while (fgets(line, MAXLINE, fpin) != NULL) //pipe2.c中父进程操作35-39行，但不用再写入管道的fd[1]
    {
		if (fputs(line, fpout) == EOF)//pipe2.c中子进程（cmdstring)操作。父进程/子进程操作拼一起才觉得绕
			err_sys("fputs error to pipe");
	}
	if (ferror(fpin))
		err_sys("fgets error");
	if (pclose(fpout) == -1)
		err_sys("pclose error");

	exit(0);
}