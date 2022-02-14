#include "apue.h"
#include "error.c"
#include "prexit.c"
int main(int argc, char *argv[])
{
	int		status;

	if (argc < 2)
		err_quit("command-line argument required");

	if ((status = system(argv[1])) < 0)
		err_sys("system() error");

	pr_exit(status);

	exit(0);
}
/*
[wqr@localhost chapter8]$ ./system3 printuids
sh: printuids: command not found
normal termination, exit status = 127
[wqr@localhost chapter8]$ ./system3 who
wqr      tty2         2021-11-29 10:40 (tty2)
normal termination, exit status = 0
*/