#include <stdio.h>
#include <stdlib.h> // 提供 exit()的原型
#include <string.h>
#include "small_test_3.h"//正式文件没有这个
void englishToMorseLine(FILE* fp_r,FILE* fp_w)
{
    char line[255];
    char filename_r[40];
    char filename_w[40];
    int count=0;
    char* p;
    int length;

    while (feof(fp_r) == 0)
    {

    while (fgets(line, 255, fp_r) !=NULL)
    {

       length=strlen(line);
       line[length]='\n';//人工添加回车，不是每行都有回车
       count=0;//查了好久，从matlab到c？！
       while(line[count]!='\n')
       {
          p = englishToMorse(line[count]);
          fprintf(fp_w,"%s|",p);
          count++;
       }
        
       fprintf(fp_w,"\n");
       memset(line,'\0', 255);//忘记清空 感觉不用清空，用的fgets函数阿，morse转字符确实也没错？

    }

    }

    

}
/*void main(void)
{
    englishToMorseLine();
}*/