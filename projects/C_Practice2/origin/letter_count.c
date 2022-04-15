#include <stdio.h>
#include <stdlib.h>
#include "h1.h"
#include "input.c"

int mycompare(const void* a,const void* b)
{
    if ( *(char*)a <  *(char*)b ) return -1;
    if ( *(char*)a == *(char*)b ) return 0;
    if ( *(char*)a >  *(char*)b ) return 1;
}

void letter_count(single_day* all_day)
{  
    int n=count*letter_num_day;//后期21全改为n
   
    
    
    char repeat_letter[21]={'\0'};
    
    
    char temp=0;
    
    
    int re_count=0;
    int re_letter=0;

    //++++++++++++++++++++++++++++++++++++++++++++++++++++
    //取出3次所有单词+取出3个核心单词
     //char letter_usedd[21]={'\0'};//后期改
     char ccore_letter[3]={'\0'};//按组排的
     char core_letter[3]={'\0'};
     int valid_character=0;
     int letter_count=0;

    for(int i=0;i<count;i++)
    {
       while(1)
       {
          if(all_day[i].letter_group[letter_count]>='a' && all_day[i].letter_group[letter_count]<='z')//比剔除更好的策略是匡定
          {
              letter_usedd[valid_character]=all_day[i].letter_group[letter_count];
              valid_character++;
              letter_count++;

              if(valid_character%7==1)//取出核心字母
              {
                  ccore_letter[i]=letter_usedd[valid_character-1];
                  core_letter[i]=letter_usedd[valid_character-1];
              }
              
              //用while配合break是因为如果用valid_character%7去判断有个问题，就是一开始valid_character始终是0个，一进循环就出来
              if(valid_character%7==0)
              {
                  valid_character=(i+1)*letter_num_day;
                  letter_count=0;
                  break;
              }
          }
          else
          {   
              letter_count++;
          }
       }
       
    }
 
    for(int i=0;i<3;i++)
    {
        printf("core%d:%c  ",(i+1),core_letter[i]);
    }
    printf("\n");
     
     qsort(core_letter,count,sizeof(char),mycompare);
     for(int i=0;i<3;i++)
     {
         printf("sorted core:%c ",core_letter[i]);
     }
     printf("\n");


    for(int i=0;i<21;i++)
    {
        printf("%d:%c  ",(i+1),letter_usedd[i]);
    }
    printf("\n");

    
    //++++++++++++++++++++++++++++++++++++++++++++++++++++

   //++++++++++++++++++++++++++++++++++++++++++++++++++++
   //qsort函数对单词按ASC码从小到大排序
   //qsort(letter_usedd,count*letter_num_day,sizeof(char),mycompare);//排序后
   //++++++++++++++++++++++++++++++++++++++++++++++++++++
    
    //++++++++++++++++++++++++++++++++++++++++++++++++++++
   //去重，letter_used中为不重复单词
   char letter_used[21]={'\0'};
   int letter_used_count=0;

    for(int i=0;i<count*letter_num_day;i++)
    {
        
        if(letter_usedd[i]!=letter_usedd[i+1])
        {
            letter_used[letter_used_count]=letter_usedd[i];
            letter_used_count++; 
        }
        
    }

    printf("letter_used_count%d\n",letter_used_count);
    for(int i=0;i<letter_used_count;i++)
    {
        printf("%c ",letter_used[i]);
    }
    printf("\n");
    //++++++++++++++++++++++++++++++++++++++++++++++++++++

    //++++++++++++++++++++++++++++++++++++++++++++++++++++
    //计算每个字母在3次比赛中一共出现的次数
    //https://blog.csdn.net/Aiden_yan/article/details/115644288 
    //其实这种做法可以算是桶排序的一个应用。 字典？
    //可以代替 qsort排序+去重
    int m=0;
    int res[26]={0};
     
    for(int i=0;i<21;i++)
    {
        m=(int)letter_usedd[i];
       res[m-97]=res[m-97]+1;
    }
    for(int i=0;i<26;i++)
    {
        printf("%d:%d  ",(i+1),res[i]);
    }
    printf("\n");
   //++++++++++++++++++++++++++++++++++++++++++++++++++++

   
}

/*void main()
{
    single_day* all_day=input_process("small_test.txt");
    letter_count(all_day);
   
}*/