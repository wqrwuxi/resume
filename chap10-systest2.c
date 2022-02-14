#include "apue.h"
#include "error.c"
#include "system.c"
//#include "system_chap8.c"
static void
sig_int(int signo)
{
	printf("caught SIGINT\n");
}

static void
sig_chld(int signo)
{
	printf("caught SIGCHLD\n");
}

int
main(void)
{
	if (signal(SIGINT, sig_int) == SIG_ERR)
		err_sys("signal(SIGINT) error");
	if (signal(SIGCHLD, sig_chld) == SIG_ERR)
		err_sys("signal(SIGCHLD) error");
	if (system("/bin/ed") < 0)
		err_sys("system() error");
	exit(0);
}
/*"system_chap8.c"
[wqr@localhost chapter10]$ ./systest2
a
hello word
.
1,$p
hello word
w temp.foo
11
^C
?
caught SIGINT
q           
caught SIGCHLD
*/
/*正确system
a
hello word
.
1,$p
hello word
w temp.foo
11
^C
?
q           
caught SIGCHLD

*/