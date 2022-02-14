#include "apue.h"
#include <fcntl.h>
#include "error.c"
char	buf1[] = "abcdefghij";
char	buf2[] = "ABCDEFGHIJ";

int
main(void)
{

    int fd;
    int pianyi;

    //似乎用open保留偏移量，但是fopen不保留
	/*fp=fopen("file.hole","a+");
    fd=fileno(fp);*/
	//open函数第三个设置文件权限
    fd=open("file.hole2",O_RDWR|O_CREAT,0776);
   
	if (write(fd, buf1, 10) != 10)
		err_sys("buf1 write error");
	/* offset now = 10 */
    
    pianyi=lseek(fd, 16384, SEEK_SET);
    printf("偏移量：%d\n",pianyi);
	if (pianyi== -1)
		err_sys("lseek error");
	/* offset now = 16384 */

	if (write(fd, buf2, 10) != 10)
		err_sys("buf2 write error");
	/* offset now = 16394 */

	exit(0);
}
/*
wqr@localhost chapter3]$ gcc -o hole2 hole2.c
[wqr@localhost chapter3]$ ./hole2
偏移量：16384
[wqr@localhost chapter3]$ ls -l file.hole2
-rwxrwxrw-. 1 wqr wqr 16394 Nov 12 12:04 file.hole2
*/
