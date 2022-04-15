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
///////////////////////////////////////////////////////////////////////////////
//结构体定义
typedef struct 
{
    char date[LINELEN];
    char letter_group[LINELEN];
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
single_day all_day[GAME_NUM];//原先用指针配合malloc报错：initializer element is not constant
everyday all_records[GAME_NUM];
char letter_usedd[LETTER_USED]={'\0'};
char ccore_letter[GAME_NUM]={'\0'};//按游戏组排序的
char core_letter[GAME_NUM]={'\0'};//按字母顺序排序的

///////////////////////////////////////////////////////////////////////////////
//函数声明区域
//取出small_test.txt中3次比赛的日期和每次涉及的7个字母。产生single_day all_day[GAME_NUM]
void input_process(char* filename);
//--------------------------
//将3*7个21个放入一个数组。按比赛顺序取出3个核心字母形成数组，再排序形成按字母排序的数组（因为打印时按字母前后）
//char letter_usedd[LETTER_USED]   ccore_letter[GAME_NUM]  core_letter[GAME_NUM]来自此处
int my_compare(const void* a,const void* b);
void processe_input();
//-------------------------
//everyday all_records[GAME_NUM]来自此处，该结构数组的数据为核心数据，是下文很多处计算的基础
//word_input函数将调用calculate_games_single_game。print_words_single_games使用的是全局数据做的输出
void calculate_games_single_game(int day,char* words,int base7[]);
void word_input(char* filename_input);
void print_words_single_games(char* filename);
//-------------------------
//统计每个单词涉及了3个核心字母中的几个
void calculate_words_single_word(char* words,FILE* fp1);
void print_words_single_word(char* output_filename,char* input_filename);
//--------------------------
void print_words_single_letters(char* filename);
//--------------------------
void parseArgs(int argc, char *argv[]);
///////////////////////////////////////////////////////////////////////////////
//主函数入口
void main(int argc, char **argv)
{
     parseArgs(argc, argv);

     input_process(argv[1]);
     processe_input();
     word_input(argv[1]);
      
      //字符串比较是否一直要用函数，几次了？？
      //strcmp用了要几个括号搞不清 。strcmp(a,b) == 0

        if (strcmp(argv[3],"--game_stats")==0)
        {
           print_words_single_games(argv[2]);
        }
        else if (strcmp(argv[3],"--word_stats")==0)
        {
           print_words_single_word(argv[2],argv[1]);
        }
        else if (strcmp(argv[3],"--letter_stats")==0)
        {
           print_words_single_letters(argv[2]);
        }
}
///////////////////////////////////////////////////////////////////////////////
//已经实现的函数
void input_process(char* filename)
{
    char words[LINELEN];
    char date_now[LINELEN];
    char letter_now[LINELEN];
    FILE* fp;
    int count=0;

    //single_day* all_day=malloc(GAME_NUM*sizeof(single_day));

    if ((fp=fopen(filename,"r"))==NULL)
    {
       fprintf(stderr,"Open fail:%s\n",(strerror(errno)));
       exit(EXIT_FAILURE);
    }

    while(fgets(words,LINELEN,fp)!=NULL)//取出 output_small_test_games.txt中的date和letters 
    {
        if (words[0]=='-')
        {
            fgets(date_now,LINELEN,fp);
            fgets(letter_now,LINELEN,fp);
            fgets(words,LINELEN,fp);
            strcpy(all_day[count].date,date_now);//不给等号赋值
            strcpy(all_day[count].letter_group,letter_now);
            memset(date_now,'\0',LINELEN);
            memset(letter_now,'\0',LINELEN);
            count++;
        }
    }

    if(fclose(fp)!=0)
    {
       fprintf(stderr, "Error in closing files one\n",(strerror(errno)));
    }

}
//------------------------------------------------------------------------

int my_compare(const void* a,const void* b)
{
    if ( *(char*)a <  *(char*)b ) return -1;
    if ( *(char*)a == *(char*)b ) return 0;
    if ( *(char*)a >  *(char*)b ) return 1;
}

void processe_input()
{
  int valid_character=0;
  int letter_count=0;

    for(int i=0;i<GAME_NUM;i++)
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
              if(valid_character%LETTER_ONE_GAME==0)
              {
                  valid_character=(i+1)*LETTER_ONE_GAME;
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

    qsort(core_letter,GAME_NUM,sizeof(char),my_compare);
    
}
//----------------------------------------------------------------
//----------------------------------------------------------------

void calculate_games_single_game(int day,char* words,int base7[])
{
    //更新当天总单词数
    all_records[day].total_word++;

    //记录单词字母个数，并且对all_records[day].word_count进行统计
    int single_word_len=strlen(words)-1;
    words[single_word_len]='\0';
    
    //计算当天各字母个数的单词个数
    all_records[day].word_count[single_word_len-4]++;
    
    //计算当天分数
    if(single_word_len==4)
    {
        all_records[day].score++;
    }
    else
    {
        all_records[day].score=all_records[day].score+single_word_len;
    }

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
     
    //计算该单词有7个目标字母中的几个，从而计算 加分以及pangrams
    int count_vaild=0;

    for(int i=0;i<LETTER_ONE_GAME;i++)
    {
        if(res_word[base7[i]]==1)
        {
            count_vaild++;
        }
    }

    if(count_vaild==LETTER_ONE_GAME)
    {
        all_records[day].score=all_records[day].score+LETTER_ONE_GAME;
        all_records[day].pangrams++;
    }

}//calculate_games_single_word结束

//=========范围内主函数=====
void word_input(char* filename_input)
{
    
    FILE* fp1;
    FILE* fp;

    //everyday* all_records=malloc(GAME_NUM*sizeof(everyday));

    if ((fp=fopen(filename_input,"r"))==NULL)//filename_input="small_test.txt"
    {
       fprintf(stderr,"Open fail:%s\n",(strerror(errno)));
       exit(EXIT_FAILURE);
    }

    char words[LINELEN];
    int day=-1;

    while(fgets(words,LINELEN,fp)!=NULL)
    {
        //处理前四行，分割各日期单词,并且对当天比赛数据初始化
        if (words[0]=='-')
        {
            day++;
            //all_records[day].word_count= {0,0,0,0,0}; 不行？？？
            for(int i=0;i<5;i++)
            {
                all_records[day].word_count[i]=0;
            }

            all_records[day].score=0;
            all_records[day].pangrams=0;
            all_records[day].total_word=0;
            

            for(int i=0;i<3;i++)
            {
                fgets(words,LINELEN,fp);
            }

            continue;
        }

        //7个基准字母转字典
        int res[26]={0};
        int m=0;
        int base7[LETTER_ONE_GAME]={-1,-1,-1,-1,-1,-1,-1};

        for(int i=0;i<LETTER_ONE_GAME;i++)
        {
           m=(int)letter_usedd[i+day*LETTER_ONE_GAME]-97;
           res[m]=1;
           base7[i]=m;
        }
        
        calculate_games_single_game(day,words,base7);

    }//while结束
}//word_input结束

void print_words_single_games(char* filename)
{
    FILE* fp;

    if ((fp=fopen(filename,"w"))==NULL)//filename=my_small_test_games.txt
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

    for(int i=0;i<GAME_NUM;i++)
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

        memset(date_month,'\0',10);
        memset(date_other,'\0',8);
        
        //7个字母格式处理.letter_group最后有回车
        int len=strlen(all_day[i].letter_group);
        all_day[i].letter_group[len-1]='\0';
        fprintf(fp,"%s",all_day[i].letter_group);

        //输入计算后数据
        fprintf(fp,"%6d",all_records[i].score);
        fprintf(fp,"%10d",all_records[i].pangrams);
        fprintf(fp,"%8d",all_records[i].total_word);
        fprintf(fp,"   ");

        for(int j=0;j<5;j++)
        {
            fprintf(fp,"%2d ",all_records[i].word_count[j]);
        }
  
        fprintf(fp,"\n");
    }
     
     if(fclose(fp)!=0)
    {
        fprintf(stderr, "Error in closing files one\n",(strerror(errno)));
    }
}//print_words_single_games结束
//----------------------------------------------------------------
//----------------------------------------------------------------

void calculate_words_single_word(char* words,FILE* fp1)
{
    int single_word_len=strlen(words)-1;
    words[single_word_len]='\0';
//=========================================
//计算3个核心字母间在图上的距离
    int res_core[26]={0};//基准的3个核心字母 字典 
    int base_core[GAME_NUM]={-1,-1,-1};//存储了3个核心字母的位置 (数组用{-1}初始化只能得到{-1,0,0})
    int distance[3]={0,0,0};
    int m_core=0;

    //因为core_letter是经过排序的，下面所有关于核心字母的信息base_core及choose_core都是按字母表顺序来的
    for(int i=0;i<GAME_NUM;i++)
    {
        m_core=(int)core_letter[i]-97;
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

    for(int i=0;i<LETTER_ONE_GAME;i++)
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
    
    fprintf(fp1,"%-11s",words);//words打完后有空格，下一个打印在空格后开始
    fprintf(fp1,"%-6d",core_count);

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

    fprintf(fp1,"\n");

    

}//calculate_words_single_word结束

void print_words_single_word(char* output_filename,char* input_filename)
{
    FILE* fp1;
    FILE* fp2;
    char words[LINELEN];


    if ((fp1=fopen(output_filename,"a+"))==NULL)//filename="my_small_test_words.txt"
    {
       fprintf(stderr,"Open fail:%s\n",(strerror(errno)));
       exit(EXIT_FAILURE);
    }

    if ((fp2=fopen(input_filename,"r"))==NULL)//filename_input="small_test.txt"
    {
       fprintf(stderr,"Open fail:%s\n",(strerror(errno)));
       exit(EXIT_FAILURE);
    }

    fprintf(fp1,"word   count    ");
    for(int i=0;i<26;i++)
    {
        fprintf(fp1,"%c  ",(97+i));
    }
    fprintf(fp1,"\n");
    fprintf(fp1,"--------------------------------------------------------------------------------------------\n");
    
     while(fgets(words,LINELEN,fp2)!=NULL)
     {  
        //处理前四行，分割各日期单词,并且对当天比赛数据初始化 
        if (words[0]=='-')
        {
            for(int i=0;i<3;i++)
            {
                fgets(words,LINELEN,fp2);
            }

            continue;
        }
        
        calculate_words_single_word(words,fp1);

     }

    if(fclose(fp1)!=0)
    {
        fprintf(stderr, "Error in closing files one\n",(strerror(errno)));
    }

    if(fclose(fp2)!=0)
    {
        fprintf(stderr, "Error in closing files one\n",(strerror(errno)));
    }
}//print_words_single_word结束

//----------------------------------------------------------------
//----------------------------------------------------------------
void print_words_single_letters(char* filename)
{
    double letter_count[26]={0.0};
    double letter_core[26]={0.0};
    double letter_avg[26]={0.0};
    

    for(int i=0;i<LETTER_USED;i++)//先把核心字母当普通字母，计算出现次数以及总分
    {
        int nn=(int)letter_usedd[i]-97;
        letter_count[nn]++;
        int num_record=i/7;
        letter_core[nn]=all_records[num_record].score+letter_core[nn];
    }

    for(int i=0;i<GAME_NUM;i++)//去除把核心当普通的
    {
        int nn=(int)letter_usedd[i*7]-97;
        letter_count[nn]--;
        letter_core[nn]=letter_core[nn]-all_records[i].score;
    }

    for(int i=0;i<26;i++)
    {
        if(letter_count[i]!=0)
        {
           letter_avg[i]=letter_core[i]/letter_count[i];
        }
    }

    FILE* fp4;
    
    if ((fp4=fopen(filename,"w"))==NULL)//filename="my_small_test_letters.txt"
    {
       fprintf(stderr,"Open fail:%s\n",(strerror(errno)));
       exit(EXIT_FAILURE);
    }

    fprintf(fp4,"letter   #center   avg center   #other   avg other\n");
    fprintf(fp4,"--------------------------------------------------\n");

    for(int i=0;i<26;i++)
    {
        fprintf(fp4,"%c",i+97);

        if(i==8 ||i==11 ||i==20)
        {
            fprintf(fp4,"%15d",1);

            for(int j=0;j<GAME_NUM;j++)//core_letter是按出现顺序排的，不是按字母表顺序排的
            {    
                int cmp=(int)core_letter[j]-97;

                if(i==cmp)
                {
                   fprintf(fp4,"%12.1lf",(1.0*all_records[j].score));
                }
                
            }

            if(letter_count[i]!=0)//看第4/5栏是否要填写
            {
                letter_avg[i]=letter_core[i]/letter_count[i];
                fprintf(fp4,"%9.0lf",letter_count[i]);
                fprintf(fp4,"%11.1lf",letter_avg[i]);
            }
            
        }
        else
        {
            if(letter_count[i]!=0)
            {
               letter_avg[i]=letter_core[i]/letter_count[i];
               fprintf(fp4,"%36.0lf",letter_count[i]);
               fprintf(fp4,"%11.1lf",letter_avg[i]);
            }
            
        }
        fprintf(fp4,"\n");
    }
}//print_words_single_letters结束

//----------------------------------------------------------------
//----------------------------------------------------------------




