#include <stdio.h>
#include "apue.h"
#include "error.c"
#define MAX 512

//mks创建的临时文件不会自动删除。如果希望从文件系统命名空间中删除该文件，必须自己对他解除连接
//temp文件夹里全是的


void make_temp(char *template)
{
  int fd;

  if((fd=mkstemp(template))==-1)
  {
    err_sys("can't create temp file");
  }
  printf("temp name = %s\n", template);//如果成功，修改template反映临时文件的名字
  close(fd);
}
void main(void)
{
    char good_template[] = "/tmp/dirXXXXXX";	/* right way */
    make_temp(good_template);
    //修改bad方案使之可以  原：char	*bad_template = "/tmp/dirXXXXXX";	/* wrong way*/
    char *bad_template=malloc(50*sizeof(char));
    strcpy(bad_template, "/tmp/dirXXXXXX");//字符串赋值不能用等号
    make_temp(bad_template);
    free(bad_template);

}