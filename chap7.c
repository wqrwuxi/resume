#include<stdio.h>
void chap7_1(void)
{
    char ch='0';
    int space_count=0;
    int line_break_count=0;
    int other=0;
   
    printf("enter some chars:\n");

    do
    {
        //ch=getchar();
        scanf("%c",&ch);
        
        if(ch==' ')//空字符 \0 NULL ；空格 ‘ ’
        {
            space_count++;
        }
        else if(ch=='\n')
        {
            line_break_count++;
        }
        else if(ch=='#')
        {
            break;
        }
        else
        {
            other++;
        }

    } while (ch!='#');
    
    printf("space_count=%d ,line_break_count=%d ,other=%d\n",space_count,line_break_count,other);

}
void p7_1(void)
{
    char ch = 0;
    int n_space = 0;
    int n_break = 0;
    int n_other = 0;
 
    printf("Please enter text to be analyzed(# to terminate):");
    while ((ch = getchar()) != '#')
    {
        if (' ' == ch)
        {
            n_space++;
        }
        else if ('\n' == ch)//else if中间的空格要记得
        {
            n_break++;
        }
        else
        {
            n_other++;
        }
    }
 
 
    printf("break = %d, space = %d, other = %d\n", n_break, n_space, n_other);
    return;
}
void chap7_2(void)
{
    char ch;
    int count=0;
    char group[8]={0};

    printf("enter some characters:");

    while((ch=getchar()) !='#')
    {
       group[count]=ch;
       count++;

       if ((count%8)==0)
       {
          count=0;
          
          for(int i=0;i<8;i++)
          {

              if(group[i]=='\n')
              {
                 printf("%10s","[Enter]");
              }
              else
              {
                  printf("%10c",group[i]);
              }
              
          }

          printf("\n");

          for(int i=0;i<8;i++)
          {
              printf("%10d",(int)group[i]);
          }
      
          printf("\n");
       }

    }

    if(count<8)
    {
        for(int i=0;i<count;i++)
        {

          if(group[i]=='\n')
          {
             printf("%10s","[Enter]");
          }
          else
          {
            printf("%10c",group[i]);
          }
          
        }

        printf("\n");

        for(int i=0;i<count;i++)
        {
          printf("%10d",(int)group[i]);// printf("%c:%d ", ch, ch);不写int也性S
        }

        printf("\n");
    }

}
void chap7_5(void)
{
   char ch;
   char group[255];
   int count=0;
   int count_full_stop=0;
   int count_exclamation=0;

   printf("enter some characters:");

   while( (ch=getchar()) !='#')
   {
       group[count]=ch;

       switch(ch)
       {
         case('.')://最后；要记得
         {
           group[count]='!';
           count_full_stop++;
           break;//break要漏
         }
         case('!'):
         {
           group[count+1]='!';
           count_exclamation++;
           break;
         }//可以不写default:情况
       }

       count++;
   }
   printf("characters:%s,count_full_stop=%d,count_exclamation=%d\n",group,count_full_stop,count_exclamation);
}
void p7_5(void)//putchar()可以边读取字符边打印字符，记得可以用，相比自己的没有char数字更保险
{
    char ch = 0;
 
    printf("Please enter text to be analyzed (# to terminate):");
    while ((ch = getchar()) != '#')
    {
        switch (ch)
        {
        case '.':
            putchar('!');
            break;
 
        case '!':
            putchar('!');
            putchar('!');
            break;
 
        default:
            putchar(ch);
            break;
        }
    }
 
    return;
}
void chap7_6(void)//学习如何识别2个字符
{
  char ch;
  char before=' ';
  int count=0;

  printf("enter some characters:");

  while ((ch = getchar()) != '#')
  {
     
     if (ch=='i')
     {

       if (before=='e')
       {
          count++;
       }

     }

     before=ch;

  }

  printf("%d",count);
}
#define basic_salary 100
void chap7_8(void)
{
    double working_hour=0.0;
    double conversion=0.0;
    double gross_pay=0.0;
    double taxes=0.0;
    double net_worth=0.0;
    int rate=0;
    int s_return=0;
    
    printf("*****************************************************************\n");
    printf("Enter the number corresponding to the desired pay rate or action:\n");
    printf(" 1) $8.75/hr 2) $9.33/hr\n");
    printf("3) $10.00/hr 4) $11.20/hr\n");
    printf(" 5) quit\n");
    printf("*****************************************************************\n");

    printf("the hours worked in a week:");
    scanf("%lf",&working_hour);

    if(working_hour<=40)
    {
      conversion=working_hour;
    }
    else
    {
      conversion=40+1.5*(working_hour-40);
    }

    printf("the desired pay rate or action:");
   
    while((scanf("%d",&rate))!=1 || rate<1 || rate>4 )
    {
        getchar();//这一行没有的话，非整数测试会成为死循环，因为其实输入最后有个回车键
        printf("Enter 1 to 4:");
    }

    switch(rate)
    {
      case 1:
        gross_pay=8.75*conversion;
        break;
      case 2:
        gross_pay=9.33*conversion;
        break;
      case 3:
        gross_pay=10.00*conversion;
        break;
      case 4:
        gross_pay=11.2*conversion;
        break;
    }

    if(gross_pay<=300)
    {
        taxes=0.15*gross_pay;
    }
    else if(gross_pay>300 && gross_pay<=450)
    {
        taxes=0.15*300+(gross_pay-300)*0.2;
    }
    else
    {
        taxes=0.15*300+(450-300)*0.2+(gross_pay-450)*0.25;
    }

    net_worth=gross_pay-taxes;
    printf("gross_pay=%lf,net_worth=%lf,taxes=%lf\n",gross_pay,net_worth,taxes);
}

void main(void)
{
   //chap7_8(); 
   p7_8();
}