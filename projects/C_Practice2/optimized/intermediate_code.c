///////////////////////////////////////////////////////////////////////////////
//头文件区域。在这里加上必要的头文件
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h> // 提供 exit()的原型  mallic
#include <errno.h>
///////////////////////////////////////////////////////////////////////////////
//宏定义。在这里加上必要的宏定义
#define GAME_NUM 3
#define LETTER_ONE_GAME 7
#define COUNT_LETTER 5
#define GROUPS_PER_UNIT 10
#define LINELEN 128
#define MAX_WORD_LEN 8
#define MIN_WORD_LEN 4
#define ALPHABET 26
#define ASC_a 97
///////////////////////////////////////////////////////////////////////////////
//结构体定义
typedef struct 
{  
   char *str;
   int game_num;
   int str_len;
   int num_core_character;
   char core_character[GAME_NUM];
}single_word;
typedef struct
{
    char *date;
    char *letter_group;
    char letter_used[LETTER_ONE_GAME];
    int word_count[5];
    int total_word;
    int pangrams;
    int score;
}single_game;
typedef struct{
    char letter;
    int if_core;
    int core_avg;
    int not_core_times;
    int not_core_avg;
}letter_included
///////////////////////////////////////////////////////////////////////////////
//变量声明，初始化
//single_day all_day[GAME_NUM];//原先用指针配合malloc报错：initializer element is not constant
char ccore_letter[GAME_NUM+1]={'\0'};//按游戏组排序的
char core_letter[GAME_NUM+1]={'\0'};//按字母顺序排序的
single_game *all_day=NULL;
single_word *word=NULL;//一定要有，表明结构数组是声明了的，后面才能用
letter_included *letter=NULL;
//总结：函数外声明动态数组，函数内赋值 

///////////////////////////////////////////////////////////////////////////////
/*
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
*/
///////////////////////////////////////////////////////////////////////////////
//主函数入口
/*void main(int argc, char **argv)
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
}*/
///////////////////////////////////////////////////////////////////////////////
//已经实现的函数
void input_process()
{    
    char words[LINELEN];
    char date_now[LINELEN];
    char letter_now[LINELEN];
    FILE* fp;
    int count=-1;
    int word_count=0;

    fp=fopen("small_test.txt","r");

    if (fp==NULL)
    {
       fprintf(stderr,"Open fail:%s\n",(strerror(errno)));
       exit(EXIT_FAILURE);
    }
    
    //在外面malloc就报那个不是常量
    all_day=malloc(GAME_NUM*sizeof(single_game));
    word=malloc(GROUPS_PER_UNIT*sizeof(single_word));

    for(int i=0;i<GAME_NUM;i++)//必须在函数内
    {
      all_day[i].date=(char*)malloc(LINELEN*sizeof(char));
      all_day[i].letter_group=(char*)malloc(LINELEN*sizeof(char));
      memset(all_day[i].date,'\0',LINELEN);
      memset(all_day[i].letter_group,'\0',LINELEN);
      memset(all_day[i].letter_used,'\0',LETTER_ONE_GAME);
      all_day[i].total_word=0;
      all_day[i].pangrams=0;
      all_day[i].score=0;

      for(int j=0;j<COUNT_LETTER;j++)//all_day[i].word_count={0,0,0,0,0}不行
      {
        all_day[i].word_count[j]=0;
      }
    };

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
            continue;
        }

        
        word[word_count].str=malloc((MAX_WORD_LEN+1)*sizeof(char));
        
        word[word_count].game_num=count;
        word[word_count].num_core_character=0;
        //word[word_count].core_character={'\0'};字符串数组可以这么初始化，到结构里就不行了
        memset(word[word_count].core_character,'\0',GAME_NUM);

        int n=strlen(words);
        words[n-1]='\0';
        strcpy(word[word_count].str,words);
        word[word_count].str_len=n-1;
        memset(words,'\0',LINELEN);
        
        all_day[count].total_word++;
        word_count++;

        if (word_count%GROUPS_PER_UNIT==0)
        {
            printf("%d",word_count);
           int n=word_count+GROUPS_PER_UNIT;
           word= realloc(word, n*sizeof(single_word));

           if(word==NULL)
           {
               fprintf(stderr, "Error in realloc\n",(strerror(errno)));
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
              all_day[i].letter_used[valid_character]=all_day[i].letter_group[letter_count];
              valid_character++;
              letter_count++;

              if(valid_character%LETTER_ONE_GAME==1)//取出核心字母
              {
                  ccore_letter[i]=all_day[i].letter_used[valid_character-1];
                  core_letter[i]=all_day[i].letter_used[valid_character-1];
              }
              
              //用while配合break是因为如果用valid_character%7去判断有个问题，就是一开始valid_character始终是0个，一进循环就出来
              if(valid_character%LETTER_ONE_GAME==0)
              {
                  valid_character=0;
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

void calculate_games_single_game(int i,single_word word,int base[])
{
    
    all_day[i].word_count[word.str_len-4]++;
    
    if(word.str_len==4)
    {
        all_day[i].score++;
    }
    else
    {
        all_day[i].score=all_day[i].score+word.str_len;
    }

    int res_word[ALPHABET];

    for(int j=0;i<ALPHABET;j++)
    {
        res_word[j]=-1;
    }

    int m_word=0;

    for(int j=0;j<word.str_len;i++)
    {
        m_word=(int)words[i]-ASC_a;
        res_word[m_word]=1;
    }
     
    int count_vaild=0;

    for(int i=0;i<LETTER_ONE_GAME;i++)
    {
        if(res_word[base[i]]==1)
        {
            count_vaild++;
        }
    }

    if(count_vaild==LETTER_ONE_GAME)
    {
        all_day[i].score=all_day[i].score+LETTER_ONE_GAME;
        all_day[i].pangrams++;
    }

}//calculate_games_single_game结束

//=========范围内主函数=====


void calculate_games()
{  
  int index=0;
  for(int i=0;i<GAME_NUM;i++)
  {
    //7个基准字母转字典
    int res[ALPHABET]={0};
    int m=0;
    int base[LETTER_ONE_GAME]={-1,-1,-1,-1,-1,-1,-1};

    for(int j=0;j<LETTER_ONE_GAME;j++)
    {
        m=(int)all_day[i].letter_usedd[j]-ASC_a;
        res[m]=1;
        base[i]=m;
    }
    
    for (int j=0;j<all_day[i].total_word;j++)
    {
        calculate_games_single_game(i,word[index],base);
        index++;
    }
  }
        
}//word_input结束

//----------------------------------------------------------------
//----------------------------------------------------------------

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
        fprintf(fp,"%6d",all_day[i].score);
        fprintf(fp,"%10d",all_day[i].pangrams);
        fprintf(fp,"%8d",all_day[i].total_word);
        fprintf(fp,"   ");

        for(int j=0;j<COUNT_LETTER;j++)
        {
            fprintf(fp,"%2d ",all_day[i].word_count[j]);
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

void calculate_words_single_word(single_word* word,FILE* fp1)
{
    int res_core[ALPHABET]={0};//基准的3个核心字母 字典 
    int base_core[GAME_NUM]={-1,-1,-1};//存储了3个核心字母的位置 (数组用{-1}初始化只能得到{-1,0,0})
    int m_core=0;

    //因为core_letter是经过排序的，下面所有关于核心字母的信息base_core及choose_core都是按字母表顺序来的
    for(int i=0;i<GAME_NUM;i++)
    {
        m_core=(int)core_letter[i]-ASC_a;
        res_core[m_core]=1;
        base_core[i]=m_core;
    }
//================================================    
    //单个单词转字典
    int res_word[ALPHABET];
    int m_word=0;

    for(int i=0;i<ALPHABET;i++)
    {
        res_word[i]=-1;
    }
    
    for(int i=0;i<LETTER_ONE_GAME;i++)
    {
        m_word=(int)word->str[i]-ASC_a;
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
            word->core_character[core_count]=core_letter[i];
            core_count++;
        }
    }
    
    fprintf(fp1,"%-11s",word->str);//words打完后有空格，下一个打印在空格后开始
    fprintf(fp1,"%-6d",core_count);

    //计算单词所涉及的核心字母在表各中的距离
    int distance[GAME_NUM]={0,0,0};
    int gap=0;
    distance[0]=3*base_core[0];

    for(int i=1;i<GAME_NUM;i++)
    {
        distance[i]=3*base_core[i]-3*base_core[i-1];
    }

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

void print_words_single_word(char* output_filename)
{
    FILE* fp1;
    char words[LINELEN];

    fp1=fopen(output_filename,"a+");

    if (fp1==NULL)//filename="my_small_test_words.txt"
    {
       fprintf(stderr,"Open fail:%s\n",(strerror(errno)));
       exit(EXIT_FAILURE);
    }

    fprintf(fp1,"word   count    ");
    for(int i=0;i<ALPHABET;i++)
    {
        fprintf(fp1,"%c  ",(ASC_a+i));
    }
    fprintf(fp1,"\n");
    fprintf(fp1,"--------------------------------------------------------------------------------------------\n");
    
    int index=0;
    
    for(int i=0;i<GAME_NUM;i++)
    {
        for(int j=0;j<all_day[i].word_num;j++)
        {
           calculate_words_single_word(word[index],fp1);
           index++;
        }
    }
 
    if(fclose(fp1)!=0)
    {
        fprintf(stderr, "Error in closing files one\n",(strerror(errno)));
    }

}//print_words_single_word结束

//----------------------------------------------------------------
//----------------------------------------------------------------
void print_words_single_letters(char* filename)
{
    double letter_count[ALPHABET]={0.0};
    double letter_score_all[ALPHABET]={0.0};
    double letter_avg[ALPHABET]={0.0};
    int m_used=0;
    int m_core=0;
    letter=malloc(ALPHABET*sizeof(letter_included));

    for(int i=0;i<ALPHABET;i++)
    {
       letter[i].letter=(char)(ASC_a+i);
       letter[i].core_times=0;
       letter[i].core_all=0;
       letter[i].core_avg=0;
       letter[i].not_core_times=0;
       letter[i].not_core_all=0;
       letter[i].not_core_avg=0;
    }

    for(int i=0;i<GAME_NUM;i++)//注释：先把核心字母当普通字母，计算出现次数以及总分
    {
        for(int j=0;j<all_day[i].total_word)
        {
           m_used=(int)all_day[i].letter_used[j]-ASC_a;
           letter[m_used].not_core_times++;
           letter[m_used].not_core_all=letter[m_used].not_core_all+all_day[i].score;
        } 
    }
    
    /*for(int i=0;i<LETTER_USED;i++)//先把核心字母当普通字母，计算出现次数以及总分
    {
        int nn=(int)letter_usedd[i]-97;
        letter_count[nn]++;
        int num_record=i/7;
        letter_core[nn]=all_records[num_record].score+letter_core[nn];
    }*/

    for(int i=0;i<GAME_NUM;i++)//注释：去除把核心当普通的
    {
        m_core=(int)ccore_letter[i]-ASC_a;
        letter[m_core].not_core_times--;
        letter[m_core].not_core_all=letter[m_core].not_core_all-all_day[i].score;
        letter[m_core].core_times++;
        letter[m_core].core_all=letter[m_core].not_core_all+all_day[i].score;

    for(int i=0;i<ASC_a;i++)
    {
        if(letter[i].not_core_times!=0)
        {
           letter[i].not_core_avg=letter[i].not_core_all/letter[i].not_core_times;
        }

        if(letter[i].core_times!=0)
        {
           letter[i].core_avg=letter[i].core_all/letter[i].core_times;
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

    for(int i=0;i<ALPHABET;i++)
    {
        fprintf(fp4,"%c",i+ASC_a);

        if(letter[i].core_times!=0)
        {
            fprintf(fp4,"%15d",letter[i].core_times);
            fprintf(fp4,"%12.1lf",letter[i].core_avg);
        
            if(letter[i].not_core_times!=0)//看第4/5栏是否要填写
            {
                fprintf(fp4,"%9.0lf",letter[i].not_core_times);
                fprintf(fp4,"%11.1lf",letter[i].not_core_avg);
            }
            
        }
        else if(letter[i].not_core_times!=0)
        {
            fprintf(fp4,"%36.0lf",letter[i].not_core_times);
            fprintf(fp4,"%11.1lf",letter[i].not_core_avg);
    
        }
        fprintf(fp4,"\n");
    }
}//print_words_single_letters结束

//----------------------------------------------------------------
//----------------------------------------------------------------

void parseArgs(int argc, char *argv[])
{
   if (argc!=4)
   {
    printf("Wrong input.\n");
    printf("The first parameter is the name of program.\n");
    printf("The second parameter is the input file name.\n");
    printf("The thied parameter is the output file name.\n");
    printf("The fourth parameter is --game_stats/--letter_stats/--word_stats\n");
    exit(EXIT_FAILURE);
   }
   
}


