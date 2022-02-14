#include "apue.h"
#include "error.c"
#include <pthread.h>
#include <time.h>
#include <sys/time.h>
#include <errno.h>	

struct to_info {
	void	      (*to_fn)(void *);	/* function */
	void           *to_arg;			/* argument */
	struct timespec to_wait;		/* time to wait */
};

pthread_mutexattr_t attr;
pthread_mutex_t mutex;

#define SECTONSEC  1000000000	/* seconds to nanoseconds */

#if !defined(CLOCK_REALTIME) || defined(BSD)
#define clock_nanosleep(ID, FL, REQ, REM)	nanosleep((REQ), (REM))
#endif

#ifndef CLOCK_REALTIME
#define CLOCK_REALTIME 0
#define USECTONSEC 1000		/* microseconds to nanoseconds */

void
clock_gettime(int id, struct timespec *tsp)
{
	struct timeval tv;

	gettimeofday(&tv, NULL);
	tsp->tv_sec = tv.tv_sec;
	tsp->tv_nsec = tv.tv_usec * USECTONSEC;
}
#endif

void retry(void *arg);
//void retry();
int makethread(void *(*fn)(void *), void *arg);
void * timeout_helper(void *arg);
void timeout(const struct timespec *when, void (*func)(void *), void *arg);
void printids(const char *s);

void printids(const char *s)
{
	pid_t		pid;
	pthread_t	tid;

	pid = getpid();
	tid = pthread_self();
	printf("%s pid %lu tid %lu (0x%lx)\n", s, (unsigned long)pid,
	  (unsigned long)tid, (unsigned long)tid);
}

int makethread(void *(*fn)(void *), void *arg)
{
	int				err;
	pthread_t		tid;
	pthread_attr_t	attr;
	struct to_info	*tip;
	struct timespec  wait;

	tip = (struct to_info *)arg;
	wait=tip->to_wait;
	wait.tv_sec=tip->to_wait.tv_sec+2;

	err = pthread_attr_init(&attr);
	if (err != 0)
		return(err);
	err = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	if (err == 0)
		err = pthread_create(&tid, &attr, fn, arg);
	printids("main");
	sleep(wait.tv_sec);
	printf("out");
	pthread_attr_destroy(&attr);
	return(err);
}

void * timeout_helper(void *arg)
{
	struct to_info	*tip;

	tip = (struct to_info *)arg;
    printf("sec:%d,nsec:%ld\n",tip->to_wait.tv_sec,tip->to_wait.tv_nsec);
	int reply=clock_nanosleep(CLOCK_REALTIME, 0, &tip->to_wait, NULL);//主线程休眠太短导致来不及返回就结束了，所以测不出返回值

	if (reply!=0)
	{
		printf("clock_nanosleep %s",(strerror(errno)));
	}

	printids("new");
	(*tip->to_fn)(tip->to_arg);

	int kind=PTHREAD_MUTEX_RECURSIVE;
	int err = pthread_mutexattr_gettype(&attr,&kind);
	if(err==0)
	{
		printf("recursive type\n");
	}

	free(arg);
	return(0);
}

void
timeout(const struct timespec *when, void (*func)(void *), void *arg)
{
	struct timespec	now;
	struct to_info	*tip;
	int				err;

	clock_gettime(CLOCK_REALTIME, &now);
	if ((when->tv_sec > now.tv_sec) ||
	  (when->tv_sec == now.tv_sec && when->tv_nsec > now.tv_nsec)) {
		tip = malloc(sizeof(struct to_info));
		if (tip != NULL) {
			tip->to_fn = func;
			tip->to_arg = arg;
			tip->to_wait.tv_sec = when->tv_sec - now.tv_sec;
			if (when->tv_nsec >= now.tv_nsec) {
				tip->to_wait.tv_nsec = when->tv_nsec - now.tv_nsec;
			} else {
				tip->to_wait.tv_sec--;
				tip->to_wait.tv_nsec = SECTONSEC - now.tv_nsec +
				  when->tv_nsec;
			}
			printf("in timeout\n");
			err = makethread(timeout_helper, (void *)tip);
			if (err == 0)
				return;
			else
				free(tip);
		}
	}

	/*
	 * We get here if (a) when <= now, or (b) malloc fails, or
	 * (c) we can't make a thread, so we just call the function now.
	 */
	(*func)(arg);
}


//void retry()
void retry(void *arg)
{
	
    pthread_mutex_init(&mutex, &attr);//强加的，想着另一个线程了，但是为什么？？
	
	pthread_mutex_lock(&mutex);

	/* perform retry steps ... */
	for(int i=0;i<10;i++)
	{
        printf("output : %d\n",i);
    }

	pthread_mutex_unlock(&mutex);
}

int
main(void)
{
	int				err, condition, arg;
	struct timespec	when;

	if ((err = pthread_mutexattr_init(&attr)) != 0)
		err_exit(err, "pthread_mutexattr_init failed");
	if ((err = pthread_mutexattr_settype(&attr,PTHREAD_MUTEX_RECURSIVE)) != 0)
		err_exit(err, "can't set recursive type");
	if ((err = pthread_mutex_init(&mutex, &attr)) != 0)
		err_exit(err, "can't create recursive mutex");

	
	int kind=PTHREAD_MUTEX_RECURSIVE;
	err = pthread_mutexattr_gettype(&attr,&kind);
	if(err==0)
	{
		printf("recursive type\n");
	}
    
	/* continue processing ... */
	pthread_mutex_lock(&mutex);
    
	/*
	 * Check the condition under the protection of a lock to
	 * make the check and the call to timeout atomic.
	 */
	condition=1;
	if (condition) {
		/*
		 * Calculate the absolute time when we want to retry.
		 */
		printf("in\n");
		clock_gettime(CLOCK_REALTIME, &when);
		when.tv_sec += 3;	/* 10 seconds from now */
		timeout(&when, retry, (void *)((unsigned long)arg));
	}
	pthread_mutex_unlock(&mutex);

	/* continue processing ... */

	exit(0);
}
