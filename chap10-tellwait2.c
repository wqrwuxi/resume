#include "apue.h"
#include "error.c"
#include "tellwait.c"
#include "prmask.c"

static void charatatime(char *);

int main(void)
{
	pid_t	pid;

    TELL_WAIT();
	pr_mask("before all ");

	if ((pid = fork()) < 0) {
		err_sys("fork error");
	} else if (pid == 0) {
		pr_mask("before WAIT_PARENT ");
        WAIT_PARENT();
		pr_mask("after WAIT_PARENT ");
		charatatime("output from child\n");
	} else {
		charatatime("output from parent\n");
		pr_mask("before TELL_CHILD ");
        TELL_CHILD(pid);
		pr_mask("after TELL_CHILD ");
	}
	exit(0);
}

static void charatatime(char *str)
{
	char	*ptr;
	int		c;

	setbuf(stdout, NULL);			/* set unbuffered */
	for (ptr = str; (c = *ptr++) != 0; )
		putc(c, stdout);
}
/*
[wqr@localhost chapter10]$ ./tellwait2
before all  SIGUSR1 SIGUSR2
output from parent
before TELL_CHILD  SIGUSR1 SIGUSR2
after TELL_CHILD  SIGUSR1 SIGUSR2
before WAIT_PARENT  SIGUSR1 SIGUSR2
after WAIT_PARENT 
output from child
*/