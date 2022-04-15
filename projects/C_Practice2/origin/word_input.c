#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>//open函数原型
#include "letter_count.c"


//https://www.cnblogs.com/programnote/p/4719078.html


void word_input(single_day* all_day)
{
    typedef struct{
     int word_count[5];
     int pangrams;
     int score;
     int total_word;
    }everyday;
    everyday* all_records=malloc(3*sizeof(everyday));//3=count


    char words[LINELEN];
    FILE* fp1;
    FILE* fp;

    if ((fp=fopen("small_test.txt","r"))==NULL)
    {
       fprintf(stderr,"Open fail:%s\n",(strerror(errno)));
       exit(EXIT_FAILURE);
    }
     
     if ((fp1=fopen("my_small_test_words.txt","a+"))==NULL)
    {
        fprintf(stderr,"Open fail:%s\n",(strerror(errno)));
        exit(EXIT_FAILURE);
    }
     int day=-1;
     while(fgets(words,LINELEN,fp)!=NULL)
     {
         //处理前四行，分割各日期单词
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
        
        all_records[day].total_word++;

        //数各字数的单词数
        
        
        int single_word_len=strlen(words)-1;
        words[single_word_len]='\0';
        printf("%s ",words);
        printf("%d ",single_word_len);
        all_records[day].word_count[single_word_len-4]++;
           //===================================================        
         //7个基准字母转字典
         int res[26]={0};
         int m=0;
         int base7[7]={-1,-1,-1,-1,-1,-1,-1};

         for(int i=0;i<letter_num_day;i++)
         {
            m=(int)letter_usedd[i+day*letter_num_day]-97;
            res[m]=1;
            base7[i]=m;
         }
        /*for(int i=0;i<7;i++)
        {
            printf("base7:%d ",base7[i]);
        }*/
        
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
        //printf("\n");
        /*for(int i=0;i<26;i++)
        {
            printf("%d:%d ",(i+1),res_word[i]);
        }*/
        /*
        for(int i=0;i<26;i++)
        {
            printf("w%d:%d ",(i+1),res_word[i]);
        }*/
        
        //计算当前单词含了几个目标字母，并且计算分数
        
         if(single_word_len==4)
         {
             all_records[day].score++;
         }
         else
         {
             all_records[day].score=all_records[day].score+single_word_len;
         }

        int count_vaild=0;

         for(int i=0;i<letter_num_day;i++)
         {
            if(res_word[base7[i]]==1)
            {
                count_vaild++;
            }
         }
        
        printf(" avild:%d ",count_vaild);
        printf("\n");

         if(count_vaild==7)
         {
           all_records[day].score=all_records[day].score+7;
           all_records[day].pangrams++;
         }
         
         //计算3个核心字母共有几个
         int res_core[26]={0};//基准的3个核心字母 字典  
         int base_core[3]={-1,-1,-1};//存储了3个核心字母的位置 (数组用{-1}初始化只能得到{-1,0,0})
         int choose_core[3]={-1,-1,-1};//若该核心字母选中，该位值改为 核心字母的位置，即base_core[i]
         int m_core=0;
         int core_count=0;
         char temp_core_letter[3]={'i','l','u'};//后期改！！！！！！是core_letter
         
         //因为core_letter是经过排序的，下面所有关于核心字母的信息base_core及choose_core都是按字母表顺序来的
         for(int i=0;i<count;i++)
         {
             m_core=(int)temp_core_letter[i]-97;//后期改！！！！！！是core_letter
             res_core[m_core]=1;
             base_core[i]=m_core;
         }

         for(int i=0;i<count;i++)
         {
             if(res_word[base_core[i]]==1)
             {
                 choose_core[i]=base_core[i];
                 core_count++;
             }
         }

         /*
         for(int i=0;i<count;i++)
         {
             printf("base %d ",base_core[i]);
         }
         for(int i=0;i<count;i++)
         {
             printf("choose %d ",choose_core[i]);
         }*/
        
        //
       // open原来位置
        //不用偏移，即使open配合write打印出来的也有乱码
        //fseek在追加情况下又无效
        
        fprintf(fp1,"%-11s",words);//words打完后有空格，下一个打印在空格后开始
        fprintf(fp1,"%-6d",core_count);
         
        int distance[3]={0,0,0};
        distance[0]=3*base_core[0];
        for(int i=1;i<count;i++)
        {
            distance[i]=3*base_core[i]-3*base_core[i-1];
        }
        /*
        for(int i=0;i<count;i++)
         {
             printf("distance %d ",distance[i]);
         }*/
        
        int gap=0;
        for(int i=0;i<count;i++)
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

        //printf("day:%s\n",day);
        
        
        }//while结束！！！！！！！！！！！！！！！！
           

         //==========================================================================================  
        for(int i=0;i<21;i++)
        {
            printf("%d:%c ",(i+1),letter_usedd[i]);
        }

        printf("\n");
        for(int i=0;i<count;i++)
          {
              for(int j=0;j<5;j++)
              {
                  printf("%d ",all_records[i].word_count[j]);
              }
          printf("eight:%d\n",all_records[i].pangrams);
          printf("score:%d\n",all_records[i].score);  
          printf("total_word:%d\n",all_records[i].total_word);      


     }
     if(fclose(fp1)!=0)
        {
        fprintf(stderr, "Error in closing files one\n",(strerror(errno)));
         }
         if(fclose(fp)!=0)
        {
        fprintf(stderr, "Error in closing files one\n",(strerror(errno)));
         }

         //==========================================================================================
    
    //my_small_test_games.txt
    FILE* fp3;
    if ((fp3=fopen("my_small_test_games.txt","w"))==NULL)
    {
       fprintf(stderr,"Open fail:%s\n",(strerror(errno)));
       exit(EXIT_FAILURE);
    }
    fprintf(fp3,"date                letters           score  pangrams  #words    4  5  6  7  8\n");
    fprintf(fp3,"------------------------------------------------------------------------------\n");
    
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
        printf("kaishi\n");
        fprintf(fp3,"%-10s",date_month);//左对齐是加负号
        fprintf(fp3,"%-10s",date_other);//包含后面2个空格

        printf("%-10s",date_month);//左对齐是加负号
        printf("%-10s",date_other);//包含后面2个空格

        memset(date_month,'\0',10);
        memset(date_other,'\0',8);
        
        //7个字母格式处理.letter_group最后有回车
        int len=strlen(all_day[i].letter_group);
        all_day[i].letter_group[len-1]='\0';
        fprintf(fp3,"%s",all_day[i].letter_group);

        //输入计算后数据
        fprintf(fp3,"%6d",all_records[i].score);
        fprintf(fp3,"%10d",all_records[i].pangrams);
        fprintf(fp3,"%8d",all_records[i].total_word);
        fprintf(fp3,"   ");

        for(int j=0;j<5;j++)
        {
            fprintf(fp3,"%2d ",all_records[i].word_count[j]);
        }
  
        fprintf(fp3,"\n");
    }
     
     if(fclose(fp)!=0)
    {
        fprintf(stderr, "Error in closing files one\n",(strerror(errno)));
    }

 //======================================================================   
    char temp_core_letter[3]={'l','i','u'};
    double letter_count[26]={0.0};
    double letter_core[26]={0.0};
    double letter_avg[26]={0.0};
   /* printf("\n");
    for(int i=0;i<26;i++)
    {
        printf("%lf ",letter_core[i]);
    }
    printf("\n");*/
    for(int i=0;i<21;i++)
    {
         int nn=(int)letter_usedd[i]-97;
        letter_count[nn]++;
        int num_record=i/7;
        letter_core[nn]=all_records[num_record].score+letter_core[nn];
    }
    /*printf("\n");
    for(int i=0;i<26;i++)
    {
        printf("%lf ",letter_core[i]);
    }
    printf("\n");*/
    for(int i=0;i<3;i++)
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
    if ((fp4=fopen("my_small_test_letters.txt","w"))==NULL)
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
            for(int j=0;j<3;j++)
            {    
                int cmp=(int)temp_core_letter[j]-97;
                if(i==cmp)
                {
                   fprintf(fp4,"%12.1lf",(1.0*all_records[j].score));
                }
                
            }

            if(letter_count[i]!=0)
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
          





}
void main()
{
    single_day* all_day=input_process("small_test.txt");
    letter_count(all_day);
    word_input(all_day);

}