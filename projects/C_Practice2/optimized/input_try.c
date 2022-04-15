///////////////////////////////////////////////////////////////////////////////
//头文件区域。在这里加上必要的头文件
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h> // 提供 exit()的原型  mallic
#include <errno.h>
///////////////////////////////////////////////////////////////////////////////
//宏定义。在这里加上必要的宏定义
#define LINELEN 128
#define GAME_NUM 3
#define LETTER_ONE_GAME 7
#define LETTER_USED 21
#define COUNT_LETTER 5
#define GROUPS_PER_UNIT 15
#define MAX_WORD_LEN 8
#define MIN_WORD_LEN 4
///////////////////////////////////////////////////////////////////////////////
//结构体定义
typedef struct 
    {
    char date[LINELEN];
    char letter_group[LINELEN];
    int word_num;
    char *str_arr[];//出不来
    }single_day;

typedef struct
{
    int word_count[5];//记录4.5.6.7.8个字母的词分别有几个
    int pangrams;
    int score;
    int total_word;
}everyday;
///////////////////////////////////////////////////////////////////////////////
//变量声明，初始化
//single_day all_day[GAME_NUM];//原先用指针配合malloc报错：initializer element is not constant
single_day* all_day=NULL;
char letter_usedd[LETTER_USED]={'\0'};
char ccore_letter[GAME_NUM]={'\0'};//按游戏组排序的
char core_letter[GAME_NUM]={'\0'};//按字母顺序排序的

single_day* input_process()
{    printf("测试1");
    
    char words[LINELEN];
    char date_now[LINELEN];
    char letter_now[LINELEN];
    FILE* fp;
    int count=-1;
    //char *str[GROUPS_PER_UNIT];

    single_day* all_day=malloc(GAME_NUM*sizeof(single_day));


    for(int i=0;i<GAME_NUM;i++)
    {
        memset(all_day[i].date,'\0',LINELEN);
        memset(all_day[i].letter_group,'\0',LINELEN);
        int word_num=0;

        //char *str[GROUPS_PER_UNIT];
        for(int j=0;j<GROUPS_PER_UNIT;j++)//不初始化没法用
        {
           all_day[i].str_arr[j]=(char*)malloc((MAX_WORD_LEN+1)*sizeof(char));
           all_day[i].str_arr[j]=NULL;
           memset(all_day[i].str_arr[j],'\0',MAX_WORD_LEN+1);
        }

       // all_day[i].str_arr=(char**)malloc(sizeof(char*));
        //all_day[i].str_arr=str;
    }

    fp=fopen("small_test.txt","r");

    if (fp==NULL)
    {
       fprintf(stderr,"Open fail:%s\n",(strerror(errno)));
       exit(EXIT_FAILURE);
    }

    
    while(fgets(words,LINELEN,fp)!=NULL)//取出 output_small_test_games.txt中的date和letters 
    {
        
        if (words[0]=='-')
        {
            count++;
            fgets(date_now,LINELEN,fp);
            fgets(letter_now,LINELEN,fp);
            fgets(words,LINELEN,fp);
            strcpy(all_day[count].date,date_now);//不给等号赋值
            strcpy(all_day[count].letter_group,letter_now);
            memset(date_now,'\0',LINELEN);
            memset(letter_now,'\0',LINELEN);
            memset(words,'\0',LINELEN);

            /*for(int i=0;i<GAME_NUM;i++)
            {
               for(int j=0;j<GROUPS_PER_UNIT;j++)//不初始化没法用
               {
                  str[j]=(char*)malloc((MAX_WORD_LEN+1)*sizeof(char));
                  memset(str[j],'\0',MAX_WORD_LEN+1);
                }

               all_day[i].str_arr=(char**)malloc(sizeof(char*));
               all_day[i].str_arr=str;
            }*/

            continue;
        }
  
        int n=strlen(words);
        words[n-1]='\0';
        char str[MAX_WORD_LEN]={'\0'};
        strcpy(str,words);
        all_day[count].str_arr[all_day[count].word_num]=str;
        memset(words,'\0',LINELEN);
        all_day[count].word_num++;
        
       if (all_day[count].word_num%GROUPS_PER_UNIT==0)
        {
           int n=all_day[count].word_num;
           
           for(int j=0;j<GROUPS_PER_UNIT;j++)//不初始化没法用
            {
                  char str[MAX_WORD_LEN]={'\0'};
                  all_day[count].str_arr[n+j]=(char*)malloc(sizeof(char));
            }
        }

    }

    for(int i=0;i<3;i++)
    {
       for(int j=0;j<all_day[i].word_num;j++)
       {
           if(j%5==0)
           {
               printf("\n");
           }
           printf("%s ",all_day[i].str_arr[j]);
       }
       printf("\n");
    }
   

    if(fclose(fp)!=0)
    {
       fprintf(stderr, "Error in closing files one\n",(strerror(errno)));
    }

   return all_day;

}

void main()
{
   single_day* all_day=input_process();
  for(int i=0;i<3;i++)
   {
     printf("%s\n",all_day[i].date);
     printf("%s\n",all_day[i].letter_group);
     printf("%d\n",all_day[i].word_num);
   }
}