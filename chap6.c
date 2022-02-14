#include<stdio.h>
#include<string.h>
#include <math.h>
void chap6_1()
{
  printf("begin");
  int num=26;
  char mytest[num];
  char ww='a';
  for(int i=0;i<num;i++)
     mytest[i]=(char)(97+i);
  for(int i=0;i<num;i++)
     printf("%c ",mytest[i]);
}
void chap6_1_1()
{
  printf("begin");
  int num=26;
  char mytest[num];
  char ww='a';
  for(int i=0;i<num;i++,ww++)
     mytest[i]=ww;//ch[i] = 'a' + i;更简洁
  for(int i=0;i<num;i++)
     printf("%c ",mytest[i]);
   printf("\n");
}
void chap6_2()
{
    for(int i=0;i<5;i++)
    {
        for(int j=0;j<=i;j++)
          printf("$");
        printf("\n");
    }
}
void chap6_3()
{
    for(int i=0;i<6;i++)
    {
        for(int j=0;j<=i;j++)
          printf("%c",('F'-j));
        printf("\n");
    }
}
void chap6_4()//没想到设一个外部char就可以++
{
    char ch='A';
    for(int i=0;i<6;i++)
    {
        for(int j=0;j<=i;j++)
          printf("%c",(ch++));
        printf("\n");
    }
}
void chap6_5()
{
   char ch=0;//字符初始化类似整形初始化。
   printf("输入一个大写字母\n");
   scanf("%c",&ch);//字符要写&。字符在scanf中第一次出现
   int num=(int)ch-64;
   int space=0;
   int letter=0;
   for(int i=0;i<num;i++)
   {
      char char_now='A';
      for(space=0;space<num-i-1;space++)
         printf(" ");
      for(letter=0;letter<=i;letter++)
      {
          printf("%c",char_now);
          char_now++;
      }
      char_now--;
      for(letter=0;letter<i;letter++)
      {
          char_now--;
          printf("%c",char_now);
      }
     for(space=0;space<num-i-1;space++)
         printf(" ");
     printf("\n");
   }
}
void p6_5(void)//参考答案
{
    char ch = 0;
    printf("Please input an uppercase letter:");
    scanf("%c", &ch);
    getchar();
 
    int length = ch - 'A' + 1;
    for (size_t i = 0; i < length; i++)//size_t 类型表示C中任何对象所能达到的最大长度，无符号整数
    {
        char temp = 'A' - 1;
        for (size_t j = 0; j < (length - i - 1); j++)
        {
            printf(" ");
        }
        for (size_t j = 0; j <= i; j++)
        {
            printf("%c", ++temp);//注意这个。先打印后加
        }
        for (size_t j = 0; j < i; j++)
        {
            printf("%c", --temp);//注意这个。确实是是先减再打印
        }
 
        printf("\n");
    }
    return;
}
void chap6_7()
{
    char group[40];
    printf("Please input a word:");
    scanf("%s",group);
    for(int i=strlen(group);i>0;i--)
      printf("%c",group[i-1]);
}
void chap6_8()
{
    double num_one=0;
    double num_two=0;
    double answer=0;
    printf("Please input first word:");
    while( scanf("%lf",&num_one)==1)
    {
        getchar();
        printf("Please input second word:");
        if(scanf("%lf",&num_two)!=1)
          break;
        printf("number one=:%lf\n",num_one);
        printf("number two=:%lf\n",num_two);
        answer=num_one/num_two;
        printf("one/two=:%lf\n",answer);
        printf("Please input first word:");
    }
}
void chap6_8_1()
{
    double num_one=0;
    double num_two=0;
    double answer=0;
    printf("Please input two numbers:");
    while( scanf("%lf",&num_one)==1 && scanf("%lf",&num_two)==1)
    {
        printf("number one=:%lf\n",num_one);
        printf("number two=:%lf\n",num_two);
        answer=num_one/num_two;
        printf("one/two=:%lf\n",answer);
        printf("Please input two numbers:");
    }
}
void p6_8(void)//参考答案
{
    double divsor1 = 0;
    double divsor2 = 0;
    printf("Please input two float number:");
    while (scanf("%lf %lf", &divsor1, &divsor2) == 2)//这个不会
    {
        getchar();
        double sub = divsor1 - divsor2;
        double pro = divsor1 * divsor2;
        double result = sub / pro;
        printf("two number input were: %lf %lf; result = %lf\n", divsor1, divsor2, result);
        printf("Please input two float number:");
    }
 
    return;
}
double calculation(double num1,double num2)
{
  double answer=0;
  answer=num1/num2;
}
void chap6_9()
{
    double num_one=0;
    double num_two=0;
    printf("Please input two numbers:");
    while( scanf("%lf",&num_one)==1 && scanf("%lf",&num_two)==1)
    {
        printf("number one=:%lf\n",num_one);
        printf("number two=:%lf\n",num_two);
        printf("one/two=:%lf\n",calculation(num_one,num_two));
        printf("Please input two numbers:");
    }
}
//幂次方，c语言不认^ . 头文件加#include<math.m>，并且编译指令改为gcc -lm -o chap6 chap6.c
//-lm：指定库  -l是指定库  m 是数学库的名字  在-o后面输入自定义的文件名
void chap6_13()
{
    double group[8]={0.0};
    for(int i=0;i<8;i++)
       group[i]=pow(2.0,i);
    int count=0;
    do{
       printf("%lf ",group[count]);
       count++;
    }while(count<8);
    printf("\n");
}
void chap6_14()
{
    double group1[8]={0.0};
    double group2[8]={0.0};
    for(int i=0;i<8;i++)
    {
        printf("Please input the %dth number:",(i+1));
        scanf("%lf",&group1[i]);//数组元素也需要加&
        if(i==0)
          group2[i]=group1[i];
        else
          group2[i]=group1[i]+group2[i-1];
    }
    for(int i=0;i<8;i++)
       printf("%10.2lf",group1[i]);
    printf("\n");
    for(int i=0;i<8;i++)
       printf("%10.2lf",group2[i]);
    printf("\n");

}
/*

格式参考标准！！！！！！！！！！

*/
//本解法为遇循环标准格式
//循环的前后都要空一行
//循环的大括号别少
/*  int count=0;下一行的空行：
    函数前面一般是声明变量等
    //空行
    函数调用
*/
void chap6_15()
{
    char group[255];
    int count=0;
    
    printf("Please input  a line of input\n");

    while( scanf("%c",&group[count])==1 && count<255)
    {
        if (group[count]=='\n')
        {
          break;
        }
        //特意空一行的
        count++;
    }
      
    printf("what's your input:\n");
    printf("%s\n",group);//不是group[255]

    for(int i=count;i>0;i--)
    {
      printf("%c",group[i]);
    }
}
void chap6_15_1()
{
    char group[255];
    int count=0;
    printf("Please input  a line of input\n");
    scanf("%c",&group[count]);
    while(group[count]!='\n')
    {
        count++;
        scanf("%c",&group[count]);
        
    }
      
    printf("what's your input:\n");
    printf("%s\n",group);
    for(int i=count;i>0;i--)
      printf("%c",group[i-1]);
    
}
void p6_15(void)
{
    int i = -1;
    char input[256];
 
    printf("Please input the string:");
 
    do
    {
        i++;
        scanf("%c", &input[i]);
    } while (input[i] != '\n');//当出现：输入 循环入口判断 循环内输入 时，可以用do..while。使用时，循环只用于输入，没有关于输入字符的判断 第七章第一题
 
    for (size_t j = i; j > 0; j--)
    {
        printf("%c", input[j - 1]);
    }
    printf("\n");
 
    return;
}
void chap6_16()
{
   double Daphne=100.0;
   double Deirdre=100.0;
   int year=0;
   
   while(Deirdre<=Daphne)
   {
       year++;
       Daphne=0.1*100.0+Daphne;
       Deirdre=(1.0+0.05)*Deirdre;
   }

   printf("After %d years,Deirdre’s investment to exceed the value of Daphne’s investment\n",year);
   printf("Deirdre=%.4lf , Daphne=%.4lf",Deirdre,Daphne);
}
void main()
{
    chap6_16();
    //p6_15();
}