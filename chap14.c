 #include <stdio.h>
 #include <string.h>
 #include<ctype.h>
 #include<math.h>
// practice 4
/*C语言中的结构体和C++不同
一：两种用法
第一种：
struct all_four
{
...
};
这样的要用 struct all_four
第二种：
typedef struct
{
...
}all_four;
这样就是用 all_four
二：关于嵌套
这样放进
struct name 
{...
};

struct person
{
....
struct name nameStruct；
}

不可以如下：（C++用惯了，但不可以！）
struct person
{
....
struct name{
....
}；
}
*/
struct name{
     char last_name[20];
     char middle[12];
     char surname[12];
   };
struct person{
   unsigned long SIN;
   struct name nameStruct;
};
void print_programe(struct person person_m)
{
  printf("%s,%s",person_m.nameStruct.last_name,person_m.nameStruct.surname);

  if(strlen(person_m.nameStruct.middle)!=0)
  {
      printf(" %c.",toupper(person_m.nameStruct.middle[0]));
  }
  else
  {
     printf("   ");
  }

  printf("——%d\n",person_m.SIN);
}
//————————————————————————————————————————————————————————————————————————————————————————————————————
//——————————————————————————————————————————结构传地址：chap16_3  且用了typedef——————————————————————————
void print_programe_address(struct person *person_m)//打出来是乱码，就是普通数组也是有两头的
{
  for(struct person *p=person_m;p!=NULL;p++)
  {
     printf("%s,%s",p->nameStruct.last_name,p->nameStruct.surname);

     if(strlen(p->nameStruct.middle)!=0)
     {
      printf(" %c.",toupper(p->nameStruct.middle[0]));
     }
    else
    {
     printf("   ");
    }

    printf("——%d\n",p->SIN);
  }
}
void  test_one(int*group)//普通数组只有一头也是乱码
{
    for(int*p=group;p!=NULL;p++)
    {
        printf("%d",*p);
    }
}
void chap14_4_test(void)
{
    int test[5]={1,2,3,4,5};
    test_one(test);
}
//————————————————————————————————————————————————————————————————————————————————————————————
void print_programe_address_two(struct person *person_m1,struct person *person_m2)
{
  for(struct person *p=person_m1;p<=person_m2;p++)
  {
     printf("%s,%s",p->nameStruct.last_name,p->nameStruct.surname);

     if(strlen((*p).nameStruct.middle)!=0)
     {
      printf(" %c.",toupper(p->nameStruct.middle[0]));
     }
    else
    {
     printf("   ");
    }

    printf("——%d\n",p->SIN);
  }
}
void chap14_4(void)
{
    struct person my_four[5]={
        {123123123,{"middle_name1","middle1","surname1"}},
        {234234234,{"middle_name2","\0","surname2"}},
        {345345345,{"middle_name3","biddle3","surname3"}},
        {456456456,{"middle_name4","viddle4","surname4"}},
        {567567567,{"middle_name5","ciddle1","surname5"}},
    };
    for(int i=0;i<5;i++)
    {
        print_programe(my_four[i]);
        
    }
    print_programe_address_two(my_four,&my_four[4]);

}


// practice 11
double add_programe(double num)
{
   return (num+100.0);
};
void transform(double source[],double target[],int n,double(*pf)(double))//数组的形参要注意，不熟练，多想想定义数组
{                                                                        //double(*pf)(double))   C++prime P199 完全不熟练
     for(int i=0;i<n;i++)
     {
         target[i]=pf(source[i]);
     }
}
void chap14_11(void)
{
    double source[5]={-1.1,2.2,-3.3,4.4,-5.5};
    double target[5]={0.0};
    //transform(source, target, 5, add_programe);
    transform(source, target, 5, ceil);
    for(int i=0;i<5;i++)
    {
         printf("%8.1lf",target[i]);
    }
    printf("\n");
}
void main(void)
{
    chap14_11();
}