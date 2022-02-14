#include "apue.h"
#include "error.c"
#include <stdio.h>
#include <sys/sysmacros.h>//st_dev major minor原型
int main(int argc, char *argv[])
{
	int			i;
	struct stat	buf;

	for (i = 1; i < argc; i++) {
		printf("%s: ", argv[i]);
		if (stat(argv[i], &buf) < 0) {
			err_ret("stat error");
			continue;
		}

		printf("dev = %d/%d", major(buf.st_dev),  minor(buf.st_dev));
		//书P121  major为主设备号   minor为次设备号


		if (S_ISCHR(buf.st_mode) || S_ISBLK(buf.st_mode)) //P86 文件类型信息
		{
			printf(" (%s) rdev = %d/%d",
					(S_ISCHR(buf.st_mode)) ? "character" : "block",
					major(buf.st_rdev), minor(buf.st_rdev));
		}
		printf("\n");
	}

	exit(0);
}