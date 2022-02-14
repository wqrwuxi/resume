#include <stdio.h>
#include "apue.h"
#include "error.c"
#define MAX 512


void main(void)
{
  char name[L_tmpnam];
  FILE* fp;
  char line[MAX];

//tmpnam不给用，原因书P146
  
  ///////////////////////////////////////////////

  if((fp=tmpfile())==NULL)
  {
     err_sys("temp file created fail");
  }
  fputs("write temp file-1\n",fp);
  fputs("write temp file-2\n",fp);
  fputs("write temp file-3\n",fp);
  rewind(fp);
  while(fgets(line,MAX,fp)!=NULL)
  {
     printf("%s",line);
  }
  exit(0);
}





