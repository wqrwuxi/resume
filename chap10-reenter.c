#include "apue.h"
#include "error.c"
#include <pwd.h>

static void my_alarm(int signo)
{
	struct passwd	*rootptr;

	printf("in signal handler\n");
	if ((rootptr = getpwnam("root")) == NULL)
			err_sys("getpwnam(root) error");
	alarm(1);
}

int main(void)
{
	struct passwd	*ptr;

	signal(SIGALRM, my_alarm);
	alarm(1);
	for ( ; ; ) {
		if ((ptr = getpwnam("mail")) == NULL)
			err_sys("getpwnam error");
		if (strcmp(ptr->pw_name, "mal") != 0)  //或者改为maiil
			printf("return value corrupted!, pw_name = %s\n",
					ptr->pw_name);
	}
}
/*  若没更改
[wqr@localhost chapter10]$ ./reenter
in signal handler
^C
*/
/* 若改了
.....
return value corrupted!, pw_name = mail
return value corrupted!, pw_name = mail
return value corrupted!, pw_name = mail
return value corrupted!, pw_name = mail
return value corrupted!, pw_name = mail
return value corrupted!, pw_name = mail
return value corrupted!, pw_name = mail
return value corrupted!, pw_name = mailAlarm clock
*/