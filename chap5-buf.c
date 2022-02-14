#include <stdio.h>
#include "apue.h"
#include "error.c"
#include <stdlib.h>
void	pr_stdio(const char *, FILE *);
int		is_unbuffered(FILE *);
int		is_linebuffered(FILE *);
int		buffer_size(FILE *);

int
main(void)
{
	FILE	*fp;

	fputs("enter any character\n", stdout);
	if (getchar() == EOF)
		err_sys("getchar error");
	fputs("one line to standard error\n", stderr);

	pr_stdio("stdin",  stdin);
	pr_stdio("stdout", stdout);
	pr_stdio("stderr", stderr);

	if ((fp = fopen("/etc/passwd", "r")) == NULL)
		err_sys("fopen error");
	if (getc(fp) == EOF)
		err_sys("getc error");
	pr_stdio("/etc/passwd", fp);
	exit(0);
}



void pr_stdio(const char *name, FILE *fp)
{
  printf("stream = %s, ", name);
  
  if (is_unbuffered(fp))
		printf("unbuffered");
	else if (is_linebuffered(fp))
		printf("line buffered");
	else /* if neither of above */
		printf("fully buffered");
	printf(", buffer size = %d\n", buffer_size(fp));
}

#if defined(_IONBF)
///////////////////////////
//Added by binary,2021/11/10
#undef _LP64
#define _flag _flags
///////////////////////////

#ifdef _LP64
#define _flag __pad[4]
#define _ptr __pad[1]
#define _base __pad[2]
#endif

int is_unbuffered(FILE *fp)
{
	return(fp->_flag & _IONBF);//书P127   
}

int is_linebuffered(FILE *fp)
{
	return(fp->_flag & _IOLBF);
}

int buffer_size(FILE *fp)
{
#ifdef _LP64
	return(fp->_base - fp->_ptr);
#else
	return(BUFSIZ);	/* just a guess */
#endif
}
#endif
