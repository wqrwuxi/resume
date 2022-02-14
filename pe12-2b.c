#include <stdio.h>
#include "pe12-2a.h"
// pe12-2b.c
// 与 pe12-2a.c 一起编译

int main(void)
{
   //extern int mode;方案1,不好
   int mode;
   printf("Enter 0 for metric mode, 1 for US mode: ");
   scanf("%d", &mode);
   while (mode >= 0)
   {
      set_mode(mode);
      get_info(mode);
      show_info(mode);
      printf("Enter 0 for metric mode, 1 for US mode");
      printf(" (-1 to quit): ");
      scanf("%d", &mode);
    }
  printf("Done.\n");
  return 0;
}