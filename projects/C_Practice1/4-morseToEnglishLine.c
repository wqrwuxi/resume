#include <stdio.h>
#include <stdlib.h> // 提供 exit()的原型
#include <string.h>
#include "small_test_4.h"
void morseToEnglishLine(FILE* fp_r,FILE* fp_w)
{
    char line[1785];
    char filename_r[40];
    char filename_w[40];
    int count=0;
    int count_word=0;
    char temp[5]={'\0'};//必须初始化到全空，不然天知道temp一开始里面有什么，所以第一个字母无法打印
    char ch;
    int length;

    while (feof(fp_r) == 0)
    {
        while (fgets(line, 1785, fp_r) !=NULL)
        {
       length=strlen(line);
       line[length]='\n';//人工添加回车，不是每行都有回车
       count=0;//查了好久，从matlab到c？！
       while(line[count]!='\n')
       {

          if(line[count]!='|')
          {
              temp[count_word]=line[count];
              count_word++;
          }
          else
          {
              ch=morseToEnglish(temp);
              //fprintf(fp_w,"%c",ch);输出二进制，为什么？
              //fputs(&ch,fp_w);输出的时候莫斯码一起输出，可能是取地址的问题？
              putc(ch,fp_w);
              count_word=0;
              memset(temp, '\0', 5);
          }

          count++;
       }
       memset(line,'\0', 1785);
       fprintf(fp_w,"\n");

       }
    }      
}
/*void main(void)
{
    morseToEnglishLine();
}*/
