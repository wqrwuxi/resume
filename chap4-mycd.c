#include "apue.h"
#include "error.c"
int main(void)
{
	if (chdir("/tmp") < 0)
		err_sys("chdir failed");
	printf("chdir to /tmp succeeded\n");
	exit(0);
}
/*是不会变，去看书
[wqr@localhost chapter4]$ pwd
/home/wqr/Documents/APUE_exercise/chapter4
[wqr@localhost chapter4]$ ./t4-7
chdir to /tmp succeeded
[wqr@localhost chapter4]$ pwd
/home/wqr/Documents/APUE_exercise/chapter4

*/