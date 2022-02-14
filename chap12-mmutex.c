#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include "apue.h"
#include "error.c"

pthread_mutex_t mutex;
pthread_mutexattr_t attr;
int i = 0;


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

	err = pthread_attr_init(&attr);
	if (err != 0)
		return(err);
	//err = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	//if (err == 0)
		//err = pthread_create(&tid, &attr, fn, arg);
    pthread_create(&tid, &attr, fn, arg);
	printids("main");
	sleep(2);
	pthread_attr_destroy(&attr);
	return(err);
}

void *fn2(void *arg)
  {
    
    pthread_mutex_init(&mutex, &attr);
     	printf("in \n");
     pthread_mutex_lock(&mutex);
    
    for(int j=0;j<10;j++)
	{
        printf("output : %d\n",j);
    }

    pthread_mutex_unlock(&mutex);

  }

void fn1()
{
  makethread(fn2, NULL);
}

void main()
{
    int err;
    
    if ((err = pthread_mutexattr_init(&attr)) != 0)
		err_exit(err, "pthread_mutexattr_init failed");
	if ((err = pthread_mutexattr_settype(&attr,PTHREAD_MUTEX_RECURSIVE)) != 0)
		err_exit(err, "can't set recursive type");
	if ((err = pthread_mutex_init(&mutex, &attr)) != 0)
		err_exit(err, "can't create recursive mutex");

    pthread_mutex_lock(&mutex);
    
    fn1();

    pthread_mutex_unlock(&mutex);

}