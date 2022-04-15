///////////////////////////////////////////////////////////////////////////////
//头文件区域。在这里加上必要的头文件
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h> // 提供 exit()的原型  mallic
#include <errno.h>
#include "spelling_bee.c"


void calculate_words_single_word(char* words)
{
//=========================================
//计算3个核心字母间在图上的距离
    int res_core[26]={0};//基准的3个核心字母 字典 
    int base_core[GAME_NUM]={-1,-1,-1};//存储了3个核心字母的位置 (数组用{-1}初始化只能得到{-1,0,0})
    int distance[3]={0,0,0};
    int m_core=0;

    //因为core_letter是经过排序的，下面所有关于核心字母的信息base_core及choose_core都是按字母表顺序来的
    for(int i=0;i<GAME_NUM;i++)
    {
        m_core=(int)core_letter[i]-97;//后期改！！！！！！是core_letter
        res_core[m_core]=1;
        base_core[i]=m_core;
    }

    distance[0]=3*base_core[0];
    for(int i=1;i<GAME_NUM;i++)
    {
        distance[i]=3*base_core[i]-3*base_core[i-1];
    }
//================================================    
    //单个单词转字典
    int res_word[26];
    for(int i=0;i<26;i++)
    {
        res_word[i]=-1;
    }
    int m_word=0;
    for(int i=0;i<single_word_len;i++)
    {
        m_word=(int)words[i]-97;
        res_word[m_word]=1;
    }
    
    //计算各字母出现次数
    int choose_core[GAME_NUM]={-1,-1,-1};//若该核心字母选中，该位值改为 核心字母的位置，即base_core[i]
    int core_count=0;
    for(int i=0;i<GAME_NUM;i++)
    {
        if(res_word[base_core[i]]==1)
        {
            choose_core[i]=base_core[i];
            core_count++;
        }
    }
    
    //计算单词所涉及的核心字母在表各中的距离
    int gap=0;
    for(int i=0;i<GAME_NUM;i++)
    {
        if(choose_core[i]==-1)
        {
            gap=gap+distance[i];
        }
        else
        {
            gap=gap+distance[i];
            fprintf(fp1,"%*d",gap,1);
            gap=0;
        }
    }

}//calculate_words_single_word结束

void print_words_single_word(char* filename)
{
    FILE* fp1;

    if ((fp1=fopen(filename,"a+"))==NULL)//filename="my_small_test_words.txt"
    {
       fprintf(stderr,"Open fail:%s\n",(strerror(errno)));
       exit(EXIT_FAILURE);
    }
    
     char words[LINELEN];
     //处理前四行，分割各日期单词,并且对当天比赛数据初始化
        if (words[0]=='-')
        {
            for(int i=0;i<3;i++)
            {
                fgets(words,LINELEN,fp);
            }

            continue;
        }
        
        calculate_words_single_word(words);

    if(fclose(fp1)!=0)
    {
        fprintf(stderr, "Error in closing files one\n",(strerror(errno)));
    }
}//print_words_single_word结束

void table_make()
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

}