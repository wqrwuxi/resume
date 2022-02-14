#include <pthread.h>
#include <time.h>
#include <sys/time.h>
#include <errno.h>	
#include <stdio.h>
struct timespec wait;
struct timespec when;
struct timespec to_wait;
void main()
{
  clock_gettime(CLOCK_REALTIME, &when);
  wait=when;
  when.tv_sec += 5;	/* 5 seconds from now */
  to_wait.tv_sec=when.tv_sec-wait.tv_sec;
  to_wait.tv_nsec=when.tv_nsec-wait.tv_nsec;
  printf("before\n");
  int s=clock_nanosleep(CLOCK_REALTIME, 0, &to_wait, NULL);
  if (s != 0) {
    if (s == EINTR)
        printf("Interrupted by signal handler\n");
    else
        printf("clock_nanosleep %s",(strerror(errno)));
   }
   printf("after\n");

}