#include "apue.h"
#include <pthread.h>

void printids(const char *s)
{
	pid_t		pid;
	pthread_t	tid;

	pid = getpid();
	tid = pthread_self();
	printf("%s pid %lu tid %lu (0x%lx)\n", s, (unsigned long)pid,
	  (unsigned long)tid, (unsigned long)tid);
}

void *thr_fn(void *arg)
{
	printids("new thread: ");
	//printf("thr_fn\n");
	return((void *)0);
}

int makethread(void *(*fn)(void *), void *arg)
{
	int				err;
	pthread_t		tid;
	pthread_attr_t	attr;

	err = pthread_attr_init(&attr);
	if (err != 0)
		return(err);
	err = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	if (err == 0)
	    err=pthread_create(&tid,&attr,fn,arg);
	printids("main thread:");
	sleep(1);//这句必须加上，不然可能主线程先结束，次线程还没运行
	pthread_attr_destroy(&attr);
	return(err);
}

void main()
{
	makethread(thr_fn, NULL);
}
/*
[wqr@localhost chapter12]$ ./detach_my
main thread: pid 26047 tid 140662216775488 (0x7fee7976d740)
new thread:  pid 26047 tid 140662208378624 (0x7fee78f6b700)
*/

