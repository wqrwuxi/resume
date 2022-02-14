#include<stdio.h>
#include <stdlib.h>// 提供 exit()的原型
#include <ctype.h>
void chap8_1(void) //命令行要输入：  ./chap8 < text_one   毕竟运行程序
{
    char ch;
    int count=0;

   while((ch=getchar()!=EOF))
   {
       count++;
   }
   printf("There are %d characters in the file!\n", count);
}
void chap8_1_1(void) 
{
    char ch;
    int count=0;

    FILE * fp;
    fp = fopen("text_one", "r");// ls -al出来的文件名是text_one，不是text_one.txt

    if (fp == NULL) // 如果失败
    {
      printf("Failed to open file. Bye\n");
      exit(1); // 退出程序
    }
    else
    {
        printf("File has open.\n");
    }

   while((ch=getc(fp))!=EOF)
   {
       count++;
   }
   printf("There are %d characters in the file!\n", count);
}
//由于无法打出空格前的特殊字符，没有完整测试
void chap8_2(void) //提干第一行的：字符流 怪怪的
{
    char ch;
    FILE* fp;
    int count=0;

    fp=fopen("text_one","r");

    if (fp==NULL)
    {
      printf("Failed to open file. Bye\n");
      exit(1);
    }

    while((ch=getc(fp))!=EOF)
    {
       count++;

       if(ch<' ')
       {

          if(ch=='\n')
          {
           printf("\\n");//如何打印\n
           printf(":%d ,",ch);
          }
          else if(ch=='\t')
          {
           printf("\\t");
           printf(":%d ,",ch);
          }
          else
          {
           printf("^%c",(ch+64));
           printf(":%d ,",ch);
          }

       }
       else
       {
         putchar(ch);
         printf(":%d ,",ch);
       }
     
       if((count%10)==0)
       {
           printf("\n");
       }

    }
    
}
void chap8_3_1(void)
{
    int ch;
    int count_s=0;
    int count_d=0;
    FILE* fp;

    fp=fopen("text_one","r");
    
    if(fp==NULL)
    {
        printf("OPEN FILE FAIL!BYE!\n");
        exit(1);
    }

    while((ch=getc(fp))!=EOF)   //getc对标getchar
    {
       
       if(ch>='a' && ch<='z')
       {
           count_s++;
       }
       else if(ch>='A' && ch<='Z')
       {
           count_d++;
       }

    }

    printf("uppercase letters:%d , lowercase letters%d\n",count_d,count_s);
    fclose(fp);

}
void chap8_3_2(void)
{
    int ch;
    int count_s=0;
    int count_d=0;

    while((ch=getchar())!=EOF)
    {
        if(ch>='a' && ch<='z')
       {
           count_s++;
       }
       else if(ch>='A' && ch<='Z')
       {
           count_d++;
       }

    }

    printf("uppercase letters:%d , lowercase letters%d\n",count_d,count_s);
}
void chap8_3_3(void)//头文件#include <ctype.h>:isupper()  && islower()
{
    int count_up=0;
    int count_low=0;
    char ch;

    while((ch=getchar())!=EOF)
    {
        
        if(isupper(ch))
        {
           count_up++;
        }
        else if(islower(ch))
        {
            count_low++;
        }

    }

    printf("uppercase letters:%d , lowercase letters%d\n",count_up,count_low);
}
void chap8_4(void)
{
    char before={' '};
    char ch;
    int count=0;
    int word_alpha=0;
    int word_sum=0;

    while((ch=getchar())!=EOF)
    {

        if(!(isalpha(ch)))
        {
            
            if(isalpha(before))
            {
               count++;
               word_sum+=word_alpha;
            }

        }
        else
        {
           
           if(!(isalpha(before)))
           {
              word_alpha=1;
           }
           else
           {
               word_alpha++;
           }

        }

        before=ch;
    }

    printf("word's number:%d\n",count);
    printf("The total number of letters in words :%d\n",word_sum);
}
int quiz_program(int num,int upper_bound,int loewr_bound)
{
    int mid=0;
    int answer=0;
    mid=loewr_bound+(upper_bound-loewr_bound)/2;
    printf("mid:%d",mid);

    if(num<mid)
    {
      return quiz_program(num,loewr_bound,mid-1);//自己写的时候少return
    }
    else if(num>mid)                 
    {
      return quiz_program(num,mid+1,upper_bound);//自己写的时候少return
    }
    else
    {
     return mid;
    }
}
void chap8_5(void)
{
    int guess=0;
    int answer=0;

    printf("Pick an integer from 1 to 100. I will tryto guess ");
    printf("Uh...is your number ?\n");
    scanf("%d",&guess);
    answer=quiz_program(guess,100,1);
    printf("my answer %d?\n", answer);
}
void chap8_5_1(void)
{
    int guess=0;
    int answer=0;
    int upper_bound=100;
    int loewr_bound=1;
    int mid=0;

    printf("Pick an integer from 1 to 100. I will tryto guess ");
    printf("Uh...is your number ?\n");
    scanf("%d",&guess);

    while(loewr_bound<=upper_bound)
    {
       mid=loewr_bound+(upper_bound-loewr_bound)/2;
       
       if(guess==mid)
       {
           printf("I got guess!");
           break;
       }
       else if (guess<mid)
       {
           upper_bound=mid-1;
       }
       else
       {
           loewr_bound=mid+1;
       }
       
    }
}
char chap8_6()
{
  char ch;

  ch=getchar();

  while(1)
  {
     if(ch!=' ' && ch!='\n')
     {
         putchar(ch);
         break;
     }
     else
     {
         printf("wrong input!again:");
         ch=getchar();
     }
  }

  return ch;
}
char chap8_6_1()
{
    char ch;

    while((ch=getchar())==' ' || (ch=getchar())=='\n')
    {
     printf("wrong input!again:");
    }

    return ch;

}
char get_first(void)//第六题
{
    int ch = 0;
 
    while (!isalpha(ch = getchar()))//不会！  while continue一直不会
    {
       continue;//没这句一个效果
    }
    printf("%c",ch);
    return ch;
}
void p8_7(void)//部分，注意while里嵌套switch作循环
{
 char chooice = 0;
 
    double work_hours = 0;
    double total_income = 0;
    double tax = 0;
    double net_income = 0;
    double hourly_wage = 0;
 
    while (1)
    {
        printf("Enter the number corresponding to the desired pay rate to action:\n");
        printf("%-20s%-20s\n%-20s%-20s\n%-20s\n", "a) $8.75/hr", "b) $9.33/hr", "c) $10.00/hr", "d) $11.20/hr", "q) quit");
        scanf("%c", &chooice);
        getchar();
 
        switch (chooice)
        {
        case 'a':
            hourly_wage = 8.75;
            break;
 
        case 'b':
            hourly_wage = 9.33;
            break;
 
        case 'c':
            hourly_wage = 10.00;
            break;
 
        case 'd':
            hourly_wage = 11.20;
            break;
 
        case 'q':
            return;
 
        default:
            printf("Please enter the choice between a to d and q\n");
            continue;//构成输入循环
        }
        printf("%c",chooice);
        return;//第一个return退出switch，第一个return退出while(1)
    }
}
void p8_8(void)
{
    char ch;
    float num1=0;
    float num2=0;
    char accept1;
    char accept2;
    
    printf("Enter the operation of your choice:\n");
    printf("a. add s. subtract\n");
    printf("m. multiply d. divide\n");
    printf("q. quit\n");
    ch=getchar();

    while(1)
    {
        if(ch=='a' || ch=='s' ||ch=='m' ||ch=='d')
        {
            printf("your choice:%c\n",ch);
            break;
        }

        getchar();//吸收回车键
        printf("Please enter the right choice:\n");
        ch=getchar();
    }

    printf("Enter first number: ");
    
    while((scanf("%f",&num1))!=1)
    {
        
        while ((ch = getchar()) != '\n')//参考答案抄来的。把缓冲区里全取出来的办法
        {
            putchar(ch);
        }

        printf(" is not an number.\n");
        printf("Please enter a number, such as 2.5, -1.78E8:");
    }

    printf("Enter second number: ");

    while((scanf("%f",&num2))!=1)
    {
        
        while ((ch = getchar()) != '\n')
        {
            putchar(ch);
        }
        
        printf(" is not an number.\n");
        printf("Please enter a number, such as 2.5, -1.78E8:");
    }




}
void main()
{
  //get_first();
  //p8_7();
  p8_8();

  //char ch;
  //ch=chap8_6_1();
  //putchar(ch);
}