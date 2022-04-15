#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h> // 提供 exit()的原型
/*#define LINELEN 128
#define GAME_NUM 5
#define letter_num_day 7
#define MIN_WORD_LEN 4
#define MAX_WORD_LEN 4
typedef struct 
{
    char date[LINELEN];
    char letter_group[LINELEN];
}single_day;
int count;//这个要全局
char letter_usedd[21]={'\0'};//后期改
single_day* all_day;*/

//___________________________当独立文件用

single_day * input_process(char* filename)
{
    char words[LINELEN];
    char date_now[LINELEN];
    char letter_now[LINELEN];
    FILE* fp;
    int words_len;
    
    
    single_day* all_day=malloc(GAME_NUM*sizeof(single_day));


    if ((fp=fopen(filename,"r"))==NULL)
    {
    
       fprintf(stderr,"Open fail:%s\n",(strerror(errno)));
       //exit(EXIT_FAILURE);
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

     for(int i=0;i<count;i++)
    {
        printf("%dth record:  date:%s  letters:%s",(i+1),all_day[i].date,all_day[i].letter_group);
    }

    return all_day;
}

/*void main()
{
    single_day* all_day=input_process("small_test.txt");
}
*/




