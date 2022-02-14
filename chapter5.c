#include<stdio.h>
#define per 60
void chap5_1()
{
    int min_input=0;
    int hour=0;
    int min=0;
    printf("enter the number of minutes");
    while(scanf("%d",&min_input)==1 && min_input>0)
    {
       hour=min_input/per;
       min=min_input%per;
       printf("%d minutes equal to %d hour plus %d minutes\n",min_input,hour,min);
       printf("enter the number of minutes");
    }
}
void chap5_2()
{
   printf("enter an integer:");
   int num;
   scanf("%d",&num);
   for(int i=0;i<10;i++)
      printf("%d ",(num+i+1));
    printf("\n");
    for(int i=0;i<10;i++)
      printf("%d\t",(num+i+1));//制表符\t
    printf("\n");
     
}
void chap5_3()
{
  int day_input=0;
  int week=0;
  int day=0;
  printf("enter the num of days:");
  while(scanf("%d",&day_input)==1 && day_input>0)
  {
     week=day_input/7;
     day=day_input%7;
     printf("%d days are %d weeks, %d days.\n",day_input,week,day);
     printf("enter the num of days:");
  }
}
#define feet_trans 30.48
const float inche_trans=2.54;
void chap5_4()
{
   float cm=0;
   int feet=0;
   float inche=0;
   printf("enter the height(cm):");
   while(scanf("%f",&cm)==1 && cm>0)
   {
      printf("your cm:%.1f. \n",cm);
      feet=cm/feet_trans;
      inche=(cm-feet*feet_trans)/inche_trans;
      printf("%.1f cm = %d feet, %.1f inches\n",cm,feet,inche);
      printf("enter the height(cm):");
   }
}
void chap5_5()
{
  printf("enter the days:");
  int day=0;
  int count=0;
  int sum=0;
  scanf("%d",&day);
  while(day<=0)
  {
     printf("enter wrong days,try again:");
     scanf("%d",&day);
  }
  while(count++<day)
    sum+=count;
  printf("sum=%d\n",sum);
}
//来自参考答案的点：1.do..while 2.define了一个表达式
#define SQUARE(X) ((X) * (X))
void chap5_6()
{
  printf("enter the days:");
  int day=0;
  int sum=0;
  scanf("%d",&day);
  while(day<=0)
  {
     printf("enter wrong days,try again:");
     scanf("%d",&day);
  }
  do{
    sum=sum+SQUARE(day);
  }while(day-->0);//；不可漏
  printf("sum=%d\n",sum);
}
void cube(double num)
{
   double answer=0;
   answer=num*num*num;
   printf("cub=%lf\n",answer);//因为lf不出现全0？如果用double
}
void chap5_7()
{
   double number=0;
   printf("enter the number:");
   scanf("%lf",&number);
   cube(number);
}
void chap5_8()
{
  printf("This program computes moduli.\n");
  printf("Enter an integer to serve as the second operand:");
  int first=0;
  int second=0;
  int answer=0;
  scanf("%d",&second);
  printf("Now enter the first operand:");
  while(scanf("%d",&first)==1 && first>0)
  {
     answer=first%second;
     printf("%d %% %d is %d\n",first,second,answer);//注意如何显示%
     printf("Enter next number for first operand (<= 0 to quit):");
  }
}
void chap5_9()
{
const double C_to_F=273.16;
}
void Temperatures(const double temperature)
{
    const double Fahrenheit_val = 32.0;
    const double Kelvin_val = 273.16;
    const double Fahrenheit_temp = 5.0 / 9.0 * (temperature - Fahrenheit_val);
    const double Kelvin_temp = temperature + Kelvin_val;
 
    printf("Celsius: %.2lf  Fahrenheit:%.2lf  Kelvin:%.2lf\n", temperature, Fahrenheit_temp, Kelvin_temp);
    return;
}
void p5_9(void)
{
    double temperature = 0.0;
 
    printf("please enter the temperature (q to quit):");
    while (scanf("%lf", &temperature) == 1)
    {
        getchar();
        Temperatures(temperature);
        printf("please enter the temperature (q to quit):");
    }
 
    return;
}
void main()
{
    chap5_3();
    //p5_3();
}