#include "apue.h"
#include "error.c"
int
main(void)
{
	pid_t	pid;

	if ((pid = fork()) < 0)
		err_sys("fork error");
	else if (pid != 0) {		/* parent */
		sleep(2);
		exit(2);				/* terminate with exit status 2 */
	}

	if ((pid = fork()) < 0)
		err_sys("fork error");
	else if (pid != 0) {		/* first child */
		sleep(4);
		abort();				/* terminate with core dump */
	}

	if ((pid = fork()) < 0)
		err_sys("fork error");
	else if (pid != 0) {		/* second child */
		execl("/bin/dd", "dd", "if=/etc/passwd", "of=/dev/null", NULL);
		exit(7);				/* shouldn't get here */
	}

	if ((pid = fork()) < 0)
		err_sys("fork error");
	else if (pid != 0) {		/* third child */
		sleep(8);
		exit(0);				/* normal exit */
	}

	sleep(6);					/* fourth child */
	kill(getpid(), SIGKILL);	/* terminate w/signal, no core dump */
	exit(6);					/* shouldn't get here */
}

/*
[root@localhost chapter8]# accton on
Turning on process accounting, file set to the default '/var/account/pacct'.
[root@localhost chapter8]# exit
exit
[wqr@localhost chapter8]$ ./test1
5+1 records in
5+1 records out
2622 bytes (2.6 kB, 2.6 KiB) copied, 6.195e-05 s, 42.3 MB/s
[wqr@localhost chapter8]$ su
Password: 
[root@localhost chapter8]# accton off
Turning off process accounting.
[root@localhost chapter8]# ./pracct /var/account/pacct
                 e =   2552, chars =     536,       S
                 e =   2560, chars =    3120,        
�4                 e =   2568, chars =    3120,       S
*                 e =   2584, chars =    2576,        
�                 e =   2592, chars =    2576,        
�                 e =   2600, chars =    2576,        
�                 e =   2608, chars =    2576,        
�                 e =   2616, chars =    2576,        
*                 e =   2624, chars =    2576,        
�                 e =   2632, chars =    2576,        
7&                 e =   2640, chars =    2576,     F  
�                 e =   2648, chars =    2576,        
7&                 e =   2656, chars =    2576,     F  
�                 e =   2664, chars =    2576,        
7&                 e =   2672, chars =    2576,     F  
7&                 e =   2576, chars =    3120,        
                 e =   2680, chars =    3120,        
�4                 e =   2688, chars =    3120,       S
*                 e =   2704, chars =    2696,        
�                 e =   2712, chars =    2696,        
�                 e =   2720, chars =    2696,        
�                 e =   2728, chars =    2696,        
�                 e =   2736, chars =    2696,        
*                 e =   2744, chars =    2696,        
�                 e =   2752, chars =    2696,        
7&                 e =   2760, chars =    2696,     F  
�                 e =   2768, chars =    2696,        
7&                 e =   2776, chars =    2696,     F  
�                 e =   2784, chars =    2696,        
7&                 e =   2792, chars =    2696,     F  
7&                 e =   2696, chars =    3120,        
                   e =   7757, chars =       2,     F  
�-                 e =    536, chars =     360,       S
�D                 e =    360, chars =    3213,       S
                 e =   2816, chars =    3120,        
�4                 e =   2824, chars =    3120,       S
*                 e =   2840, chars =    2832,        
�                 e =   2848, chars =    2832,        
�                 e =   2856, chars =    2832,        
*                 e =   2864, chars =    2832,        
�                 e =   2872, chars =    2832,        
7&                 e =   2880, chars =    2832,     F  
7&                 e =   2832, chars =    3120,        
                 e =   2888, chars =    3120,        
�4                 e =   2896, chars =    3120,       S
*                 e =   2912, chars =    2904,        
�                 e =   2920, chars =    2904,        
�                 e =   2928, chars =    2904,        
*                 e =   2936, chars =    2904,        
�                 e =   2944, chars =    2904,        
7&                 e =   2952, chars =    2904,     F  
7&                 e =   2904, chars =    3120,        
                 e =   2960, chars =    3120,        
�4                 e =   2968, chars =    3120,       S
*                 e =   2984, chars =    2976,        
�                 e =   2992, chars =    2976,        
�                 e =   3000, chars =    2976,        
*                 e =   3016, chars =    2976,        
�                 e =   3024, chars =    2976,        
7&                 e =   3032, chars =    2976,     F  
7&                 e =   2976, chars =    3120,        
�                 e =   3056, chars =    3048,        
                 e =   3080, chars =    3120,        
�4                 e =   3088, chars =    3120,       S
*                 e =   3104, chars =    3096,        
�                 e =   3112, chars =    3096,        
�                 e =   3120, chars =    3096,        
�                 e =   3128, chars =    3096,        
�                 e =   3136, chars =    3096,        
�                 e =   3040, chars =    3213,        
�                 e =   3144, chars =    3096,        
*                 e =   3152, chars =    3096,        
�                 e =   3160, chars =    3096,        
7&                 e =   3168, chars =    3096,     F  
7&                 e =   3176, chars =    3096,     F  
�                 e =   3184, chars =    3096,        
7&                 e =   3192, chars =    3096,     F  
�                 e =   3200, chars =    3096,        
7&                 e =   3208, chars =    3096,     F  
7&                 e =   3096, chars =    3120,        
                 e =   3216, chars =    3120,        
�4                 e =   3224, chars =    3120,       S
*                 e =   3248, chars =    3232,        
�                 e =   3256, chars =    3232,        
�H                 e =   3240, chars =       2,       S
�                 e =   3048, chars =       1,     F  
�H                 e =   3272, chars =       1,       S
�E                 e =   3280, chars =       2,        
�E                 e =   3312, chars =       2,        
�                 e =   3264, chars =    3232,        
*                 e =   3328, chars =    3232,        
�                 e =   3336, chars =    3232,        
7&                 e =   3344, chars =    3232,     F  
7&                 e =   3232, chars =    3120,        
�                 e =   3072, chars =    3064,     F  
�                 e =   3064, chars =       1,     F  
�                 e =   1912, chars =     955,        
u,                 e =   3360, chars =    3352,        
�,                 e =   3352, chars =     955,     F  
6                 e =   3384, chars =    3376,        
�,                 e =   3376, chars =    3368,     F  
�,                 e =   3392, chars =    3368,     F  
d,                 e =   3400, chars =    3368,        
�,                 e =   3368, chars =     955,     F  
90                 e =   3464, chars =    3416,       S
                 e =   3472, chars =    3120,        
�4                 e =   3480, chars =    3120,       S
*                 e =   3496, chars =    3488,        
�                 e =   3504, chars =    3488,        
�                 e =   3512, chars =    3488,        
�                 e =   3520, chars =    3488,        
*                 e =   3528, chars =    3488,        
�                 e =   3536, chars =    3488,        
7&                 e =   3544, chars =    3488,     F  
�                 e =   3552, chars =    3488,        
7&                 e =   3560, chars =    3488,     F  
7&                 e =   3488, chars =    3120,        
90                 e =   3568, chars =    3416,       S
90                 e =   3576, chars =    3416,       S
�(                 e =   3584, chars =    3416,       S
6                 e =   3600, chars =    3592,       S
�%                 e =   3616, chars =    3608,        
7&                 e =   3608, chars =    3592,       S
�                 e =   3632, chars =    3624,        
�                 e =   3640, chars =    3624,        
x-                 e =   3624, chars =    3592,     F  
�                 e =   3656, chars =    3648,        
x-                 e =   3648, chars =    3592,     F  
�%                 e =   3664, chars =    3592,        
�%                 e =   3680, chars =    3672,        
7&                 e =   3672, chars =    3592,       S
�%                 e =   3696, chars =    3688,        
7&                 e =   3688, chars =    3592,       S
�T                 e =   3728, chars =    3712,       S
x-                 e =   3712, chars =    3704,     F S
x-                 e =   3720, chars =    3704,     F  
x-                 e =   3704, chars =    3592,     F  
*                 e =   3760, chars =    3752,        
x-                 e =   3752, chars =    3592,     F  
�'                 e =   3776, chars =    3768,        
x-                 e =   3768, chars =    3592,     F  
                 e =   3784, chars =    3120,        
�4                 e =   3792, chars =    3120,       S
*                 e =   3808, chars =    3800,        
�                 e =   3816, chars =    3800,        
�                 e =   3824, chars =    3800,        
�                 e =   3832, chars =    3800,        
�                 e =   3840, chars =    3800,        
*                 e =   3848, chars =    3800,        
�                 e =   3856, chars =    3800,        
7&                 e =   3864, chars =    3800,     F  
�                 e =   3872, chars =    3800,        
7&                 e =   3880, chars =    3800,     F  
�                 e =   3888, chars =    3800,        
7&                 e =   3896, chars =    3800,     F  
7&                 e =   3800, chars =    3120,        
                 e =   3904, chars =    3120,        
�4                 e =   3912, chars =    3120,       S
*                 e =   3928, chars =    3920,        
�                 e =   3936, chars =    3920,        
�                 e =   3944, chars =    3920,        
�                 e =   3952, chars =    3920,        
�                 e =   3960, chars =    3920,        
*                 e =   3968, chars =    3920,        
�                 e =   3976, chars =    3920,        
7&                 e =   3984, chars =    3920,     F  
�                 e =   3992, chars =    3920,        
7&                 e =   4000, chars =    3920,     F  
�                 e =   4008, chars =    3920,        
7&                 e =   4016, chars =    3920,     F  
7&                 e =   3920, chars =    3120,        
                   e =   4056, chars =    3592,    
*/
