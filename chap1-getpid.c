#include "apue.h"
#include <stdio.h>
int main(void)
{
	printf("hello world from process ID %ld\n", (long)getpid());
	exit(0);
}
