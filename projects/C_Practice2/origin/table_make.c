#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "letter_count.c"

void table_make(single_day* all_day)
{
    //my_small_test_words.txt
    FILE* fp;

    if ((fp=fopen("my_small_test_words.txt","w"))==NULL)
    {
       fprintf(stderr,"Open fail:%s\n",(strerror(errno)));
       exit(EXIT_FAILURE);
    }
    fprintf(fp,"word   count    ");
    for(int i=0;i<26;i++)
    {
        fprintf(fp,"%c  ",(97+i));
    }
    fprintf(fp,"\n");
    fprintf(fp,"--------------------------------------------------------------------------------------------\n");
    
    if(fclose(fp)!=0)
    {
        fprintf(stderr, "Error in closing files one\n",(strerror(errno)));
    }
//==========================================================================================
    
    //my_small_test_games.txt

    if ((fp=fopen("my_small_test_games.txt","w"))==NULL)
    {
       fprintf(stderr,"Open fail:%s\n",(strerror(errno)));
       exit(EXIT_FAILURE);
    }
    fprintf(fp,"date                letters           score  pangrams  #words    4  5  6  7  8\n");
    fprintf(fp,"------------------------------------------------------------------------------\n");
    
    char date_month[10]={'\0'};
    char date_other[9]={'\0'};
    int letter_month=0;
    int letter_other=0;
    for(int i=0;i<count;i++)
    {   
        //date 格式处理 
        letter_month=0;
        letter_other=0;
        while(all_day[i].date[letter_month]!=' ')
        {
            date_month[letter_month]=all_day[i].date[letter_month];
            letter_month++;
        }
            letter_month++;
        for(int j=0;j<8;j++)
        {
           date_other[j]=all_day[i].date[letter_month];
           letter_month++;
        }
  
        fprintf(fp,"%-10s",date_month);//左对齐是加负号
        fprintf(fp,"%-10s",date_other);//包含后面2个空格

        printf("%-10s",date_month);//左对齐是加负号
        printf("%-10s",date_other);//包含后面2个空格

        memset(date_month,'\0',10);
        memset(date_other,'\0',8);
        
        //7个字母格式处理.letter_group最后有回车
        fprintf(fp,"%s",all_day[i].letter_group);

        //输入计算后数据
        fprintf(fp,"%6d",all_records[i].score);
        fprintf(fp,"%10d",all_records[i].pangrams);
        fprintf(fp,"%8d",all_records[i].total_word);
        fprintf(fp,"  ");

        for(int j=0;j<5;j++)
        {
            fprintf(fp,"%3d ",all_records[i].word_count[j]);
        }
    }
     
     if(fclose(fp)!=0)
    {
        fprintf(stderr, "Error in closing files one\n",(strerror(errno)));
    }

 //======================================================================   
   

    if ((fp=fopen("my_small_test_letters.txt","w"))==NULL)
    {
       fprintf(stderr,"Open fail:%s\n",(strerror(errno)));
       exit(EXIT_FAILURE);
    }

   fprintf(fp,"letter   #center   avg center   #other   avg other\n");
   fprintf(fp,"--------------------------------------------------\n");

   for(int i=0;i<26;i++)
   {
       fprintf(fp,"%c\n",(97+i));
   }

    if(fclose(fp)!=0)
    {
        fprintf(stderr, "Error in closing files one\n",(strerror(errno)));
    }

}

void main()
{
    single_day* all_day=input_process("small_test.txt");
    table_make(all_day);
}
