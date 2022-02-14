#include "apue.h"
#include "error.c"
#define	BUFFSIZE	4096

int main(void)
{
	int		n;
	char	buf[BUFFSIZE];

	while ((n = read(STDIN_FILENO, buf, BUFFSIZE)) > 0) //P59文件描述符 STDIN_FILENO
	{
        printf("n:%d\n",n);
        if (write(STDOUT_FILENO, buf, n) != n)
        {
           err_sys("write error");
        }

        if (n < 0)
        {
          err_sys("read error");
        }
		
			
    }	
        
        

	

	exit(0);
}
