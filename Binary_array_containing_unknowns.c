#include<stdio.h>
//对二元数组的总结
//float * value; float value[N]; 这种可以传换的。
//2维数组的情况：float value[N][M];float ** value;
//指针用的时候有几种：*(value+i*N+j) 或者是 (value+i)[j]
//************************************************************************
//^^^^^^^^^^^^^^^^^^^^^^^^行和列都为未知数时^^^^^^^^^^^^^^^^^^^^^^^^
// practice 10_9
//————————————————————————方法1——————————————————————————————————————————
void copy_arr_nine(float *arr,float *copyy,int num1,int num2)
//重要理解：(float*)my=arr。my通过(float*)my强转为一元数组，可记为(float*)my=my_new（一元数组），才能使my_new的类型和arr对上
//float *copyy就是为了和传入的(float*)copy作区别，两个东西！
//arr的类型是float*，整个copy_arr_nine是对arr这个float*一元数组操作，不是那个二元数组了！！不是了！！arr+i*num2+j这就是一元指针，只是表达成这样可以和二元数组的元素对上
//所以出了copy_arr_nine，返回chap10_9，my和copy仍是二元数组，按二元数组操作
{
    printf("In copy_arr_nine,copy=\n");
    
    for(int i=0;i<num1;i++)
    {
       
       for(int j=0;j<num2;j++)
       {
         *(copyy+i*num2+j)=*(arr+i*num2+j);//一元指针
         printf("%8.2lf",*(copyy+i*num2+j));
       }

       printf("\n");
       
    }

}
//————————————————————————方法2——————————————————————————————————————————
void copy_arr_nine_trytwo(float **arr,float **copyy,int num1,int num2)
//Q:这个float **arr到底是个什么东西，和数组什么关系？？
{
    float temp=0.0;

    printf("In copy_arr_nine_trytwo,copy=\n");
    
    for(int i=0;i<num1;i++)
    { 

       for(int j=0;j<num2;j++)
       {
         *((float*)copyy+i*num2+j)=*((float*)arr+i*num2+j);//强制转一元指针，有没有别的表达方式？
         temp=*((float*)copyy+i*num2+j);
         printf("%8.2lf",temp);
       }

       printf("\n");
       
    }
}
void chap10_9(void)//想两个参数都是未知，结果难搞
{
  float copy[3][5]={0};
  float my[3][5] =
  {
  { 4.1, 4.2, 4.3, 3.0, 2.0},
  { 8.5, 8.2, 1.2, 1.6, 2.4},
  { 9.1, 8.5, 6.7, 4.4, 2.1},
  };
  float temp;

   //copy_arr_nine((float*)my,(float*)copy,3,5);//强转
   copy_arr_nine_trytwo((float**)my,(float**)copy,3,5);//强转
   printf("In chap10_9,copy=\n");

   for(int i=0;i<3;i++)
    {
       
       for(int j=0;j<5;j++)
       {
         temp=*(*(copy+i)+j) ;//出了copy_arr_nine，返回chap10_9，my和copy仍是二元数组，按二元数组操作
         //temp=copy[i][j];也是对的，因为出了copy_arr_nine，返回chap10_9，my和copy仍是二元数组，按二元数组操作
         printf("%8.2lf",temp);
       }

       printf("\n");
       
    }

}

//^^^^^^^^^^^^^^^^^^^^^^^^第二维必须标明时^^^^^^^^^^^^^^^^^^^^^^^^
// practice 11
void print_eleven(int arr[][5],int num)
{
    printf("In print_eleven=\n");
    for(int i=0;i<num;i++)
    {
       
       for(int j=0;j<5;j++)
       {
         printf("%4d",arr[i][j]);
       }

       printf("\n");
       
    }
}
void double_eleven(int (*arr)[5],int num)
{
    int temp;
    int temp2;
    printf("In double_eleven=\n");
    for(int i=0;i<num;i++)
    {
       
       for(int j=0;j<5;j++)
       {
         temp=arr[i][j];//PDF p645+p658
         temp2=2*temp;
         printf("%4d",temp2);
       }

       printf("\n");
       
    }
}
void chap10_11()
{
   int my[3][5] =
  {
  { 4, 4, 3, 3, 2},
  { 8, 2, 1, 6, 4},
  { 9, 5, 7, 3, 1},
  };

  print_eleven(my,3);
  double_eleven(my,3);//这个传参只要一个my没想到，为什么？？？

  printf("In chap10_11=\n");
    for(int i=0;i<3;i++)
    {
       
       for(int j=0;j<5;j++)
       {
         printf("%4d",my[i][j]);
       }

       printf("\n");
       
    }

}
