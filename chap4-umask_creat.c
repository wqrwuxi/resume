#include <fcntl.h>
#include "apue.h"
#include "error.c"
#define RWRWRW (S_IRUSR|S_IWUSR|S_IRGRP|S_IWGRP|S_IROTH|S_IWOTH)
int main(void)
{
   //printf("befoer1:%x\n",umask);打印的是umask函数的地址
   //除了打印mode，没办法在main中现实umask

   //默认是002 即为S_IWOTH, --- --- -w- 
   //此时mode_t mode1=umask（0）返回之前屏蔽字，即--- --- -w-
   //小注释：0,即为不屏蔽任何权限位
   mode_t mode1=umask(0);
    if (creat("umask-t1",mode1)<0) 
    {                                                    
       err_sys("creat error for umask-t1");
    }
   
   //mask(0)前的屏蔽字 是 上一次屏蔽字：umask(0)
    umask(0);//可有可无
    //mode2返回之前屏蔽字：unmask(0) mode2取值位0	--- --- ---
    //因此umask-t2 --- --- ---（直接带入）(这个表达把屏蔽字当权限用)
    mode_t mode2=umask(S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);//去除括号内四个权限，可本来就只剩other的w了
    if (creat("umask-t2",mode2)<0)                            // --- --- ---  
    {
       err_sys("creat error for umask-t2");
    }

    //当前屏蔽字=上一次屏蔽字：--- RW- RW-  【mode_t mode2=umask(S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH)】中umask里面那部分
    //if行创造：rwx r-- ---
    //屏蔽字+创造=RWX --- ---
    if (creat("umask-t3",S_IRUSR|S_IWUSR|S_IXUSR|S_IRGRP)<0) //USER的RWX，group的R
    {
       err_sys("creat error for umask-t3");
    }
   
    //初始屏蔽字：--- RW- RW-（上一次没用umask函数，所以不变动
    mode_t mode3=umask(S_IXOTH);
    //mode3值 为 上一次屏蔽字结果：	--- rw- rw-
    if (creat("umask-t4",mode3)<0)//所以umask-t4为	--- rw- rw- (直接带入)
    {
       err_sys("creat error for umask-t4");
    }
       printf("after4:%x\n",mode3);
    
	if (creat("foo", RWRWRW) < 0)
		err_sys("creat error for foo");
   //其实下行umask函数没实施前，返回上一次屏蔽字结果，所的屏蔽字结果为--- --- --x
	umask(S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
   //bar:创造RWRWRW+上一次umask --- rw- rw- ：rw- --- ---
	if (creat("bar", RWRWRW) < 0)
		err_sys("creat error for bar");
	exit(0);
}
/*
[wqr@localhost chapter4]$ ls -l /home/wqr/Documents/umask-t1
--------w-. 1 wqr wqr 0 Nov 11 16:02 /home/wqr/Documents/umask-t1
[wqr@localhost chapter4]$ ls -l umask-t2
----------. 1 wqr wqr 0 Nov 11 16:02 umask-t2
[wqr@localhost chapter4]$ ls -l foo
-rw-rw-rw-. 1 wqr wqr 0 Nov 11 16:02 foo
[wqr@localhost chapter4]$ ls -l bar
-rw-------. 1 wqr wqr 0 Nov 11 16:02 bar


*/