#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

pthread_mutex_t mt;
int i = 0;

void* fn1(void* agr)
{
  int err;

  pthread_mutex_lock(&mt);
  if((err = pthread_mutex_lock(&mt)) < 0)
  {
    printf("%s\n", strerror(err));
    exit(1);
  }

  ++i;
  printf("%d\n", i);

  //pthread_mutex_unlock(&mt);//-------②
  pthread_mutex_unlock(&mt);
  printf("fn1 end\n");
}

void* fn2(void* arg)
{
  sleep(1);//目的是让线程fn1先执行。
  printf("fn2 start\n");
  pthread_mutex_lock(&mt);//-----------①
  printf("fn2 start2\n");
  ++i;
  printf("second %d\n", i);
  pthread_mutex_unlock(&mt);
}

int main()
{
  pthread_t tid1, tid2;

  pthread_mutexattr_t mat;
  pthread_mutexattr_init(&mat);

  //设置锁的类型为递归锁
  pthread_mutexattr_settype(&mat, PTHREAD_MUTEX_RECURSIVE);
  pthread_mutex_init(&mt, &mat);
    
  pthread_create(&tid1, NULL, fn1, NULL);
  pthread_create(&tid2, NULL, fn2, NULL);

  pthread_join(tid1, NULL);
  pthread_join(tid2, NULL);

  pthread_mutex_destroy(&mt);
}
/*
https://www.cnblogs.com/xiaoshiwang/p/12586549.html
下面的例子验证递归互斥量.
例子很简单，在main函数里创建2个线程，在线程1的函数fn1，加锁互斥量2次，但是只解锁一次。线程fn2就无法给互斥量加锁，导致一直阻塞在①处。
为了能够让线程fn1能够先给互斥量加锁，在fn2里调用了sleep函数，让fn2先睡眠1秒，所以fn1就能够先给互斥量加锁了。
去掉②处的注释，fn2就能锁定mutex了，程序就不会出现死锁状态了。
*/