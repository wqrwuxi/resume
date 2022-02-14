#include "apue.h"
#include "error.c"
#include <pthread.h>

struct foo {
	int a, b, c, d;
};

void
printfoo(const char *s, const struct foo *fp)
{
	printf("%s", s);
	printf("  structure at 0x%lx\n", (unsigned long)fp);
	printf("  foo.a = %d\n", fp->a);
	printf("  foo.b = %d\n", fp->b);
	printf("  foo.c = %d\n", fp->c);
	printf("  foo.d = %d\n", fp->d);
}

void *
thr_fn1(void *arg)
{
	struct foo	foo = {1, 2, 3, 4};

	printfoo("thread 1:\n", &foo);
	pthread_exit((void *)&foo);
}

void *
thr_fn2(void *arg)
{
	printf("thread 2: ID is %lu\n", (unsigned long)pthread_self());
	pthread_exit((void *)0);
}

int
main(void)
{
	int			err;
	pthread_t	tid1, tid2;
	struct foo	*fp;

	err = pthread_create(&tid1, NULL, thr_fn1, NULL);
	if (err != 0)
		err_exit(err, "can't create thread 1");
	err = pthread_join(tid1, (void *)&fp);
	if (err != 0)
		err_exit(err, "can't join with thread 1");
	sleep(1);

	printf("parent starting second thread\n");

	err = pthread_create(&tid2, NULL, thr_fn2, NULL);
	if (err != 0)
		err_exit(err, "can't create thread 2");
	sleep(1);
    //thr_fn1中pthread_exit((void *)&foo)，tid1用pthread_join退出时，返回值为(void *)&fp因此用的一个结构？
	printfoo("parent:\n", fp);
	exit(0);
}
/*
[wqr@localhost chapter11]$ ./badexit2
thread 1:
  structure at 0x7fb0f4b52ee0
  foo.a = 1
  foo.b = 2
  foo.c = 3
  foo.d = 4
parent starting second thread
thread 2: ID is 140397996488448
parent:
  structure at 0x7fb0f4b52ee0
  foo.a = 1
  foo.b = 0
  foo.c = 0
  foo.d = 0

structure地址相同，但是内容已经变化了
*/
/*若去除thr_fn1(void *arg)中的pthread_exit((void *)&foo);
wqr@localhost chapter11]$ ./badexit2
thread 1:
  structure at 0x7f877dfa5ee0
  foo.a = 1
  foo.b = 2
  foo.c = 3
  foo.d = 4
parent starting second thread
thread 2: ID is 140219910874880
parent:
  structure at 0xc
Segmentation fault (core dumped)

*/
