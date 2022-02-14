#include<stdio.h>
// practice 1
#define MONTHS 12 // 一年的月份数
#define YEARS 5 // 年数
void chap10_1(void)  //PDF P638最上面的例子说明指针的用法 10.7 指针和多维数组
{
  float year_average[YEARS]={0};
  float year_total[YEARS]={0};
  float month_average[MONTHS]={0};
  float month_total[MONTHS]={0};
  const float rain[YEARS][MONTHS] =
  {
  { 4.3, 4.3, 4.3, 3.0, 2.0, 1.2, 0.2, 0.2, 0.4, 2.4, 3.5, 6.6 },
  { 8.5, 8.2, 1.2, 1.6, 2.4, 0.0, 5.2, 0.9, 0.3, 0.9, 1.4, 7.3 },
  { 9.1, 8.5, 6.7, 4.3, 2.1, 0.8, 0.2, 0.2, 1.1, 2.3, 6.1, 8.4 },
  { 7.2, 9.9, 8.4, 3.3, 1.2, 0.8, 0.4, 0.0, 0.6, 1.7, 4.3, 6.2 },
  { 7.6, 5.6, 3.8, 2.8, 3.8, 0.2, 0.0, 0.0, 0.0, 1.3, 2.6, 5.2 },
  };

   for(int i=0;i<YEARS;i++)
   {
       
       for(int j=0;j<MONTHS;j++)
       {
           year_total[i]+=*(*(rain+i)+j);
       }

       year_average[i]=year_total[i]/MONTHS;
       printf("Total precipitation in the %dth year:%.1f\n",(i+1),year_total[i]);
       printf("Average precipitation in the %dth year:%.2f\n",(i+1),year_average[i]);
   }

   printf("\n");

   for(int j=0;j<MONTHS;j++)
   {
      
      for(int i=0;i<YEARS;i++)
      {
         month_total[j]+= *(*(rain+i)+j);
      }

      month_average[j]=month_total[j]/YEARS;
      printf("Total precipitation in the %dth month:%.2f\n",(j+1),month_total[j]);
      printf("Average precipitation in the %dth month:%.2f\n",(j+1),month_average[j]);
   }

} 
// practice 2
void copy_arr(double target[],double origin[],int num)
{
   printf("target1[5]={");

   for(int i=0;i<num;i++)
   {
     target[i]=origin[i];
     printf("%4.1lf,",target[i]);
   }

   printf("}\n");
}
void copy_ptr(double target[],double origin[],int num)
{
    printf("target2[5]={");

    for(int i=0;i<num;i++)
    {
        *(target+i) = *(origin+i);
        printf("%4.1lf,",target[i]);
    }

    printf("}\n");
}
void copy_ptrs(double target[],double origin[],double* point)
{
    printf("target3[5]={");

   for(double* p=origin ; p!=(point) ;p++)
   {
        * target = *p;
        printf("%4.1lf,",*target);
        target++;//想了一会才出来的
   }

   printf("}\n");
}
void copy_ptrs_answer(double *target, double *source_begin, double *source_end)//参考答案
{
    for (double *i = source_begin; i < source_end; i++)
    {
        *target++ = *i;//漂亮
    }
    return;
}
void chap10_2(void)
{
    double source[5] = {1.1, 2.2, 3.3, 4.4, 5.5};
    double target1[5];
    double target2[5];
    double target3[5];
    copy_arr(target1, source, 5);
    copy_ptr(target2, source, 5);
    copy_ptrs(target3, source, source + 5);
}
// practice 3/4/5/6
void find_and_sort(double arr[],int num)
{
   double temp;

   for(int i=0;i<num-1;i++)
   {

     for(int j=0;j<num-i-1;j++)
     {
        
        if(arr[j]>arr[j+1])
        {
            temp=arr[j+1];
            arr[j+1]=arr[j];
            arr[j]=temp;
        }

     }

   }

   printf("The sorted array is:{");

   for(int i=0;i<num;i++)
    {
        printf("%4.2lf;",arr[i]);
    }

    printf("}\n");

}
void chap10_3(void)
{
    int num;

    printf("Input the number of double array:");
    scanf("%d",&num);
    double my_array[num];//这个声明没法写在最前面，只能num出来再定义

    for(int i=0;i<num;i++)
    {
        printf("Enter the %dth element:",(i+1));
        scanf("%lf",&my_array[i]);
    }

    find_and_sort(my_array,num);
}
// practice 7
/*
编写一个程序，初始化一个double类型的二维数组，使用编程练
习2中的一个拷贝函数把该数组中的数据拷贝至另一个二维数组中（因
为二维数组是数组的数组，所以可以使用处理一维数组的拷贝函数来
处理数组中的每个子数组）。
*/
// practice 8
/*
题目提示：
只需要是数组元素的地址（第二个参数）和待处理元素的个数
 */
void chap10_8(void)
{
    double source[7]={1.0,2.0,3.0,4.0,5.0,6.0,7.0};//里面是逗号！！
    double target1[3];
    double target2[3];
    double target3[3];
    copy_arr(target1, source+2, 3);
    copy_ptr(target2, source+2, 3);
    copy_ptrs(target3, source+2,source+5);//注意最后一个参数，在第二题里，该指针指向数组最后一个元素的后一个位置
}
// practice 9
//https://blog.csdn.net/YunLaowang/article/details/82054680
//根据这个链接的解释，要两个参数全未知还能传参，只能用指针：传参用指针，内部操作用指针（只针对那个传的参，比如arr）。
//copy_arr_nine内的copy二元数组跟传参没关系，所以依旧可以使用数组形式
//调用方式：display3((int*)arr,irows,icols); 即将二维数组强转成一维使用（形参部分强转）
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
         *(copyy+i*num2+j)=*(arr+i*num2+j);//这个表达式和第2题不一样,因为一元指针
         printf("%8.2lf",*(copyy+i*num2+j));
       }

       printf("\n");
       
    }

}
void copy_arr_nine_trytwo(float **arr,float **copyy,int num1,int num2)
{
    float temp=0.0;
    float temp_test=0.0;
    
    /*printf("In copy_arr_nine_trytwo,arr=\n");
    temp_test=arr[0][0];
    printf("%8.2lf",temp_test);*/
    /*for(int i=0;i<num1;i++)
    {
       for(int j=0;j<num2;j++)
       {
         temp_test=**(arr+2);
         printf("%8.2lf",temp_test);
       }
       printf("\n");
    }
    */


    printf("In copy_arr_nine_trytwo,copy=\n");
    
    for(int i=0;i<num1;i++)
    {
       //printf("%8.2lf",*(arr+i));试图知道arr是什么东西
       //printf("\n");
       for(int j=0;j<num2;j++)
       {
         *((float*)copyy+i*num2+j)=*((float*)arr+i*num2+j);
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

   //copy_arr_nine((float*)my,(float*)copy,3,5);
  // copy_arr_nine_trytwo((float**)my,(float**)copy,3,5);
  copy_arr_nine_trytwo((float**)my,(float**)copy,3,5);
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
// practice 10
void my_add_arr_ten(int rarr1[],int rarr2[],int rarr3[],int numm)
{

    for(int i=0;i<numm;i++)
    {
        rarr3[i]=rarr1[i]+rarr2[i];
    }

    printf("In add_arr=\n");

    for(int i=0;i<numm;i++)
    {
        printf("%4d",rarr3[i]);
    }

}
void chap10_10()
{
    int larr1[4] = {1,2,3,4};
    int larr2[4] = {5,6,7,8};
    int larr3[4] = {0};

    my_add_arr_ten(larr1,larr2,larr3,4);
    printf("In achap10_10=\n");
    
    for(int i=0;i<4;i++)
    {
        printf("%4d",larr3[i]);
    }

}
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
void double_eleven(int (*arr)[5],int num)//一重指针，传数组指针,第二维必须标明
{
    int temp;
    int temp2;
    printf("In double_eleven=\n");
    for(int i=0;i<num;i++)
    {
       
       for(int j=0;j<5;j++)
       {
         temp=arr[i][j];
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
  double_eleven(my,3);

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
void main(void)
{
    chap10_9();
    //p10_6();
}
