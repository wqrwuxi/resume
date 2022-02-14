 //https://blog.csdn.net/m0_46181359/article/details/105739270
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdlib.h> /* 为库函数 srand() 提供原型 */
#include <time.h> /* 为 time() 提供原型 */
#include <stdarg.h>//为 第七题double* new_d_array(int num,double k,...)
#define PI 3.1415926


 # define RECIPROCAL(X) 1/X
 #define MYPRINT(X) printf("answer:%8.2lf",X)
 #define HMEAN(X, Y) (2.0 * (X) * (Y) / ((X) + (Y)))
void chap16_2(void)
{
   double num1=5.5;
   double num2=2;
   double average=(RECIPROCAL(num1)+RECIPROCAL(num2))/2;
   double answer=RECIPROCAL(average);
   MYPRINT(answer);
   printf("\n");
   printf("answer from internet:%8.2lf\n",HMEAN(5.5, 2));
}
//practice 3
    typedef struct
    {
    double length;
    double angle;
    } polar;

    typedef struct
    {
    double x;
    double y;
    } rect;

    rect*  transform_function(polar *p) //注意传地址用法
    {
       printf("length:%lf,angle:%lf\n",p->length,p->angle);
       rect *temp;
       static const double rad = PI / 180.0;
       double ang = rad * p->angle;
       (*temp).x=p->length*cos(ang);
       temp->y=p->length*sin(ang);
       return temp;//漏，没这习惯，以后传参多了会很常见
    }
void chap16_3(void)
{
    rect *my_rect;//指针，声明时带*，用时若是指针，直接就是名字，错几次了？？
    polar my_polar={6.0,60.0};
    my_rect=transform_function(&my_polar);//注意传地址用法
    printf("x:%8.2lf,y:%8.2lf\n",my_rect->x,my_rect->y);
}
//practice 4
void chap16_4(void)
{
   int answer=0;
   clock_t start = clock();
   for(int i=1;i<10000;i++)
   { 
      answer=sqrt(i);
   }
    clock_t end = clock();
    printf("Delay %lf seconds.\n", (double)(end - start) / CLOCKS_PER_SEC);
}
//practice 5
void Select_elements(int group[],int if_choice[],int num)
{
    //生成随机数之前先进行播种,如下
    srand((unsigned)time(NULL));/* 随机种子 */
    int choose=rand()%num;
    printf("You need to select %d numbers\n",choose);
    int choosed=0; 

    for(int i=0;i<choose;i++)
    {
       choosed=rand()%(num-1); 
       while(if_choice[choosed]==1)
       {
         choosed=rand()%(num-1);    
       }
       if_choice[choosed]=1;//先操作，后标记，都是循环前操作，最后标记

       printf("The %dth number chosen:%d\n",(i+1),group[choosed]);
       
    }
}
void chap16_5(void)
{
   int group[10]={0,11,22,33,44,55,66,77,88,99};
   int if_choice[10]={0};
   srand((unsigned)time(NULL));
   Select_elements(group,if_choice,10);
}
//practice 7
double* new_d_array(int num,...)
{
    double *p=malloc(num*sizeof(double));
    va_list ap;
    va_start(ap,num);

    for(int i=0;i<num;i++)
    {
        p[i]=va_arg(ap, double);
    }

    return p;

}
void show_array(double* p,int num)
{
    for(int i=0;i<num;i++)
    {
        printf("%8.1lf",p[i]);
    }

    printf("\n");
}
void chap16_7(void)
{
    double * p1;
    double * p2;
    p1 = new_d_array(5, 1.2, 2.3, 3.4, 4.5, 5.6);
    p2 = new_d_array(4, 100.0, 20.00, 8.08, -1890.0);
    show_array(p1, 5);
    show_array(p2, 4);
    free(p1);
    free(p2);
}
void main(void)
{
    chap16_7();
}