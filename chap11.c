#include<stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
// practice 1
void get_characters(char* group ,int num)
{
   char ch;
   int count=0;

   while(count<num)
   {
       printf("Input a character:\n");
       while(scanf("%c",&ch)!=1)
       {
         printf("Wrong number.Again!");
       }
       
       group[count]=ch;//‘ch'错！查了很久！‘ch'不是成了字符是ch
       count++;
   }
}
void get_characters_two(char* group ,int num);//一开始漏了
void chap11_1(void)
{
   int num;
   printf("Enter the num of characters:");

   while(scanf("%d",&num)!=1 || num<=0)
   {
       printf("Wrong number.Again!");
   }

   getchar();//吸收scanf的回车键，不然会进入get_characters函数，成为第一个输入。缓冲区居然是共用的？
   char team[num];
   //get_characters(team ,num);
   get_characters_two(team ,num);
   //printf("Your input:%s",team);
   printf("Your input:");
   
   for(int i=0;i<num;i++)
   {
     
     if(team[i]=='\n')
     {
         printf("   \\n");
     }
     else if (team[i]==' ')
     {
         printf(" 空格 ");
     }
     
     else
     {
         printf("%4c",team[i]);
     }
     
   }

   printf("\n");

}
// practice 2
//用practice 1主函数，大家差不多
void get_characters_two(char* group ,int num)
{
   char ch;
   int count=0;

   while(count<num)
   {
       printf("Input a character:\n");

       ch=getchar();
       if (ch==' ' || ch=='\n' ||ch=='\t')  // if (isblank(ch))参考答案上的
       {
          printf("NOT accept\n");
          continue;
       }
       else
       {
          group[count]=ch;
          count++;
       }
       
   }
}
// practice 5
//指针可以理解为包含地址的一个存储单元，要显示指针，打印指针名，&指针名 是存储单元的地址
 char* my_strchr(char* group,char ch)
{
    char* my_point;

    for(int i=0;i<strlen(group);i++)
    {
        
        if (group[i]==ch)
        {
          my_point=&group[i];
          printf("address of need:%p\n",my_point);
          return my_point;
        }
        
    }

    return NULL;//记住这个操作，参考答案上的
}
void chap11_5(void)
{
   char team[256]="Former French prime minister Jean-Pierre Raffarin describes himself as a \"keen observer\" of China.";
   char* answer=my_strchr(team,'i');
   printf("address of team:%p\n",team);
   printf("address of answer:%p\n",answer);

}
// practice 7
//strlen:查bug查了半天，网上说：strlen计算时，一定要确保字符数组是以空字符结束的，如果没有则可能沿着数组在内存中的位置不断向前寻找，知道遇到空字符才停下来。
void mystrncpy(char* s1,char* s2,int num)
{
    printf("strlen(s2):%d\n",strlen(s2));
    for(int i=0;i<num;i++)
    {
        if(i>=strlen(s2))
        
        //if(i>=8)
        {
          s1[i]='\0';
        }
        else
        {
          s1[i]=s2[i];
        }
        
    }

    printf("string1:%s\n",s1);
    for(int i=0;i<num;i++)
    {
        if(s1[i]=='\0')
        {
          printf("空");
        }
        else if(s1[i]=='\n')
        {
          printf("回车");
        }
        else
        {
            printf("%c",s1[i]);
        }
    }
    printf("\n");

    printf("string2:%s\n",s2);
    for(int i=0;i<strlen(s2);i++)
    {
        if(s2[i]=='\0')
        {
          printf("空");
        }
        else if(s2[i]=='\n')
        {
          printf("回车");
        }
        else
        {
            printf("%c",s2[i]);
        }
    }
    printf("\n");
}
void chap11_7(void)
{
    int num;
    printf("Enter the length of the string1:");

    while(scanf("%d",&num)!=1 || num<=0)
    {
       printf("Wrong number.Again!\n");
    }
    getchar();
    char my_string1[num]; 

    int len;
    printf("Enter the length of the string2:");

    while(scanf("%d",&len)!=1 || len<=0)
    {
       printf("Wrong number.Again!\n");
    }
    getchar();

    char my_string2[len];
    char ch;
    int count=0;

    for(int i=0;i<len;i++)
    {
        ch=getchar();
        my_string2[i]=ch;
    }
    my_string2[len]='\0';
    printf("my_string2:%d\n",strlen(my_string2));
    printf("my_string2:%s\n",my_string2);
    printf("len:%d\n",len);
    mystrncpy(my_string1,my_string2,num);
}
// practice 8。和第五题一个问题
char* mystring_in(char* group,char* comp)//short系统关键词，不能用！
{
    int flag=-1;
    printf("strlen(group)%d:",strlen(group));
    printf("strlen(comp)%d:",strlen(comp));
    printf("group:%s,comp:%s\n",group,comp);
    int n=0;int m=0;
    for(int i=0;i<strlen(group);i++)
    {
        n = i;
        m = 0;
        
            while (group[n] == comp[m])
            {
               n++;
               m++;
               
               if (m >= strlen(comp))
               {
                return &group[i];
               }
            }
    }
    return NULL;
     printf("address of group:%p\n",group);
}
void chap11_8(void)
{
   int num1;
   int num2;

   printf("Enter the length of the origin:");
   scanf("%d",&num1);
   getchar();//不写下面字符串第一个就是回车
   char origin[num1];

   for(int i=0;i<num1;i++)
   {
      origin[i]=getchar();
   }
   origin[num1]='\0';

   printf("Enter the length of the compare:");
   scanf("%d",&num2);
   getchar();//不写下面字符串第一个就是回车
   char compare[num2];

   for(int i=0;i<num2;i++)
   {
      compare[i]=getchar();
   }
   compare[num2]='\0';

printf("strlen(origin)%d:",strlen(origin));
printf("strlen(compare)%d:",strlen(compare));

   char* answer=mystring_in(origin,compare);
   printf("address of origin:%p\n",origin);
   printf("address of origin[2]:%p\n",&origin[2]);
   printf("address of answer:%p\n",answer);
}
#define LENS 256
void chap11_12(void)
{
   char my[LENS];
   int num_words=0;
   int num_lowercase=0;
   int num_uppercase=0;
   int num_punctuation=0;
   int num_digits=0;
   FILE* fp;

   fp=fopen("text_one","r");

   if (fp==NULL)
    {
      printf("Failed to open file. Bye\n");
      exit(1);
    }

    fgets(my,LENS,fp);//这个函数只读一行
    fputs(my,stdout);

   for(int i=0;i<strlen(my);i++)
   {
      
      if(!isspace(my[i]) && my[i]!='\0')
      {
        num_words++;

        if(isalpha(my[i]))
        {
          
          if(islower(my[i]))
          {
              num_lowercase++;
          }
          else
          {
              num_uppercase++;
          }

        }
        else if(isdigit(my[i]))
        {
          num_digits++;
        }  
        else if(ispunct(my[i]))
        {
          num_punctuation++;
        }

      };

   }

   printf("单词数：%4d；大写字母数：%4d；小写字母数：%4d；标点符号数：%4d；数字字符数：%4d；",num_words,num_uppercase,num_lowercase,num_punctuation,num_digits);
   fclose(fp);
}
void chap11_13(void)
{
    char my[LENS];
    char temp;
    fgets(my,LENS,stdin); //gets不给用
    fputs(my,stdout);
    //fgets留下了还行符号
    //puts会在后面再加个行符号
    puts(my);
    int len=strlen(my);

    for(int i=0;i<(len/2);i++)
    {
        temp=my[len-i-1];
        my[len-i-1]=my[i];
        my[i]=temp;
    }
    putchar('#');
    putchar('\n');
    puts(my);

}
void main(void)
{
   chap11_13();
}