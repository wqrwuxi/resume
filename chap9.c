#include<stdio.h>
#include <stdlib.h>//为指针
#include <ctype.h>//为isalpha
#include <math.h>
// practice 1
double min_rewrite(double x, double y)//第一题参考答案
{
    return (x < y ? x : y);
}
// practice 5
void larger_of(double* num1,double* num2)
{
    double temp;
    if(*num1 >= *num2)
    {//交换不会
        temp=*num2;
        *num2=*num1;
        *num1=temp;
    }
    else
    {
        temp=*num1;
        *num1=*num2;
        *num2=temp;
    }
}
void larger_of_another(double* num1,double* num2)
{
    double temp;
    if(*num1 != *num2)
    {//交换不会
        temp=*num2;
        *num2=*num1;
        *num1=temp;
    }
    
}
void chap9_5(void)
{
    double* point1=malloc(sizeof(double));//分配/释放内存，没习惯
    double* point2=malloc(sizeof(double));

    *point1=8.8;//想修改实参，地址传递。没必要主函数数据就用指针，chap9_5_1()用数据的地址就够了
    *point2=7.8;
    printf("the value of point1:%lf\n",*point1);
    printf("the value of point2:%lf\n",*point2);
    larger_of(point1,point2);
    printf("After change\n");
    printf("the value of point1:%lf\n",*point1);
    printf("the value of point2:%lf\n",*point2);
    free(point1);
    free(point2);
}
void larger_of_two(double num1,double num2)
{
    double temp;
    if(num1 >= num2)
    {//交换不会
        temp=num2;
        num2=num1;
        num1=temp;
    }
    else
    {
        temp=num1;
        num1=num2;
        num2=temp;
    }
}
//如果不想改变实参，值传递（larger_of_two）；想修改实参，地址传递（larger_of）
//C++ P213 还有个引用，larger_of_two(double &num1,double &num2)，其他和larger_of_two一样，主函数传数据，不需要传地址
void chap9_5_1(void)
{
    double one=0;
    double two=0;

    printf("input two numbers:");

    while((scanf("%lf %lf",&one,&two))!=2)//注意%lf之间一定要是空格
    {
       getchar();
       printf("wrong input,again");
    }

    printf("the value of point1:%lf\n",one);
    printf("the value of point2:%lf\n",two);

    //larger_of(&one,&two);//想换值，一定要指针
    larger_of_another(&one,&two);


    printf("the value of point1:%lf\n",one);
    printf("the value of point2:%lf\n",two);

}
// practice 7
int character_judgment(char ch)
{
        int position;
        if(isalpha(ch))
        {
            
            if(ch>='a' && ch<='z')
            {
                return position=(int)ch-96;
            }
            else if(ch>='A' && ch<='Z')
            {
                return position=(int)ch-64;
            }

        }
        else
        {
           return -1;
        }

}
void chap9_7(void)
{
    char word;
    char words[41];
    int value;

    //程序清单13.3 书P870 可行
    printf("stdin test.Input some words\n");

    while ((fscanf(stdin, "%40s", words) == 1) && (words[0] != '#'))
    {
      fprintf(stdout, "%s\n", words);
    }
    getchar();
     
    //自己 不可行
    printf("Input some characters\n");

    //while((scanf("%c",&word)==1) && (word != '#'))
    while((fscanf(stdin,"%c",&word)==1) && (word != '#'))
    {
       putchar(word);
       value=character_judgment(word);

       if(value==-1)
       {
           printf("-1;");
       }
       else
       {
           printf("%d;",value);
       }
        
    }
}
// practice 8
void chap9_8(void)
{
    double num1=0.0;
    double num2=0.0;
    double my_power=0.0;

    scanf("%lf %lf",&num1,&num2); //&连续两题漏！！！！！！！！！！！！！
    //printf("num1:");比如输入2,用的2的ASC码：50
    //num1=getchar();
    //getchar();//接受回车的，不然第二个数就是回车的ASC码
    //printf("num2:");
    //num2=getchar();

    if(num2>0)
    {
        my_power=pow(num1,num2);
    }
    else if(num2<0)
    {
        my_power=pow(num1,-num2);
        my_power=1.0/my_power;
    }
    else
    {
        my_power=1;
    }

    printf("%lf ^ %lf = %lf",num1,num2,my_power);
}

// practice 9
double pow_recursive(double base, double index)
{
    if (index > 0)
    {
        index--;
        return (base * pow_recursive(base, index));
    }
    if (0 == index)
    {
        if (base == 0)
        {
            printf("The base number is 0 is not defined!\n");
        }
        return 1;//返回上层函数这个不会
    }
    if (index < 0)
    {
        index++;
        double tmp = 1 / base;
        return (tmp * pow_recursive(base, index));
    }
}
// practice 10
void to_binary(int n , int scale) /* 递归函数 */
{
   int r;
   r = n % scale;

   if (n >= 8)
   {
      to_binary(n / 8,scale);
   }

   printf("%d",r);
   //putchar(r == 0 ? '0' : '1');必须把数字转为字符，因为比如putchar(0),打印的是0的ASC码：30
   return;//单return不一定写的出
}
void chap9_10(void)
{
   int num;
   int scale;
   
   printf("Enter an integer (q to quit):\n");
   scanf("%d",&num);
   printf("Enter the base scale(2-10):\n");

   while(scanf("%d",&scale)!=1 || scale<2 || scale>10)
   {
      printf("Wrong scale.Enter the base scale(2-10):\n");
   }
   
   to_binary(num,scale);

}
// practice 11
void Fibonacci(int n)
{
  //大话数据结构P103  PDF P131
}

void main(void)
{
    chap9_10();
}