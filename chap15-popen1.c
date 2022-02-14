#include "apue.h"
#include "error.c"
//#include "myuclc.c"
#include <sys/wait.h>

int main(void)
{
	char	line[MAXLINE];
	FILE	*fpin;

	if ((fpin = popen("/home/wqr/Documents/myuclc", "r")) == NULL) //和popen1.c不一样的1。程序路径只能是绝对路径
		err_sys("popen error");
	for ( ; ; ) {
		fputs("prompt> ", stdout);
		fflush(stdout);
		if (fgets(line, MAXLINE, fpin) == NULL)	/* read from pipe */
			break;
		if (fputs(line, stdout) == EOF)//和popen1.c不一样的2
			err_sys("fputs error to pipe");
	}
	if (pclose(fpin) == -1)
		err_sys("pclose error");
	putchar('\n');
	exit(0);
}
/*
[wqr@localhost chapter15]$ ./popen1
prompt> a
a
prompt> A
a
prompt> c
c
prompt> Q
q
prompt> ^C
*/