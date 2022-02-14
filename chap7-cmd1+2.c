#include "apue.h"
#include "error.c"
#include <setjmp.h>

#define	TOK_ADD	   5
#define CHARLINE 40

void	do_line(char *);
void	cmd_add(void);
int		get_token(void);

jmp_buf jmpbuffer;

int main(void)
{
	char	line[MAXLINE];

    /*if (setjmp(jmpbuffer) != 0)
		printf("error");*/

    int value;
    value=setjmp(jmpbuffer);
    if(value!=0)
    {
        printf("%d error\n",value);
        exit(0);
    }

	while (fgets(line, MAXLINE, stdin) != NULL)
		do_line(line);
    
    //跳转只能往前跳，不可往后
   /* int value;
    value=setjmp(jmpbuffer);
    if(value!=0)
    {
        printf("%d error\n",value);
    }*/ 
    printf("return from loop");//不起作用

	exit(0);
}

char	*tok_ptr;		/* global pointer for get_token() */

void do_line(char *ptr)		/* process one line of input */
{
	int		cmd;

	tok_ptr = ptr;
	while ((cmd = get_token()) > 0) {
		switch (cmd) {	/* one case for each command */
		case TOK_ADD:
				cmd_add();
				break;
                
		}
	}
    printf("exit  from do_line\n");//
    exit(0);//直接退出，不回到main
   
}

void cmd_add(void)
{
	int		token;

	token = get_token();
	/* rest of processing for this command */
    printf("cmd_add!");

    if (token==TOK_ADD)		/* an error has occurred */
		longjmp(jmpbuffer, 1);
	
}

int get_token(void)
{
	/* fetch next token from line pointed to by tok_ptr */
    printf("get_token!");

    int len=strlen(tok_ptr);

    if (len>TOK_ADD)
    {
        return TOK_ADD;
    }
    else if(len>1 && len<=TOK_ADD)
    {
        return 0;
    }
    else
    {
       longjmp(jmpbuffer,2);
    }
    
    

}

/*
[wqr@localhost chapter7]$ ./cmd

get_token!2 error
asdfg
get_token!get_token!cmd_add!1 error
*/


