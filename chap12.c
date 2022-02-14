#include <stdio.h>
#include <stdlib.h>//provide a declaration of ‘malloc’
#include <string.h>
// practice 1
int critic(void)
{
    int units;
    printf("No luck, my friend, Try again.\n");
    scanf("%d", &units);
    return units;
}
void p12_1(void)
{
    int units;
 
    printf("How many pounds to firkin of butter?\n");
    scanf("%d", &units);
    while (units != 56)
    {
        units = critic();
    }
    printf("You must have looked it up!\n");
 
    return;
}
// practice 4
void my_count(void)
{
   static int count=0;//这东西进来第二次就不用初始化了
   count++;
   printf("%d",count);
}
void chap12_4(void)
{
    
    for(int i=0;i<10;i++)
    {
        my_count();
    }
}
// practice 8
int * make_array(int elem, int val)
{
  int * pa;
  pa=(int*)malloc(elem*sizeof(int));

  //int * pa=(int*)malloc(elem);

  for (int i=0;i<elem;i++)
  {
      pa[i]=val;
  }

  return pa;//指针是pa，*pa是值
}
void show_array(const int ar [], int n)
{
    int rows=n/8;
    int left=n%8;

    if(rows==0)
    {
        
        for(int i=0;i<n;i++)
        {
          printf("%d",ar[i]);
        }

        printf("\n");
    }
    else
    {
        for(int i=0;i<rows;i++)
        {
           
           for(int j=0;j<8;j++)
           {
             printf("%d",ar[8*i+j]);
           }

           printf("\n");

        }

        for(int i=0;i<left;i++)
        {
          printf("%d",ar[8*rows+i]);
        }

        printf("\n");

    }

}
 int chap12_8(void)
 {
   int * pa;
   int size;
   int value;
   printf("Enter the number of elements: ");

   while (scanf("%d", &size) == 1 && size > 0)
   {
     printf("Enter the initialization value: ");
     scanf("%d", &value);
     pa = make_array(size, value);
     if (pa)
     {
       show_array(pa, size);
       free(pa);
     }
     printf("Enter the number of elements (<1 to quit): ");
   }
     
   printf("Done.\n");
   return 0;
 }
// practice 8
void chap12_9(void)
{
    int num;
    char ch;
    int count=0;
    char* pb;
    char temp[40];
    
    printf("How many words do you wish to enter?");
    scanf("%d",&num);
    getchar();
    printf("Enter %d words now:",num);
    char** pa=(char**)malloc(num*sizeof(char**));//双重指针，是有指针类型的

    for(int i=0;i<num;i++)
    {
       count=0; 

       //while((ch=getchar())!=' ')//最后一个单词最后加个空格才能结束输入
       //{
         //temp[count]=ch;
         //count++;
      // }

       scanf("%s",&temp);//好处 1.自动分辨单词 2.不用清楚temp，因为是赋值temp
       
       //char* pb=(char*)malloc(count*sizeof(char*));错的，不会，看下一行
       pa[i]=(char*)malloc((strlen(temp)+1)*sizeof(char*));//pa[i]没想到
       strcpy(pa[i],temp);
       printf("%s\n",pa[i]);

       //memset(temp, '\0', 40);//只有函数可以清空.配套while循环用
       //strcpy(temp,"");不行，清空不料
       //for(int j=0;j<(strlen(temp)+1);j++)//明明清楚了，打印出来也是没东西，结果下一个单词就是还会跳出之前单词多余的一段
       //{
           //temp[j]='\0';
       //}
       //printf(temp);
    
    }
    printf("Here are your words:\n");
    for (int i = 0; i < num; i++)
    {
        printf("%s\n", pa[i]);
    }
 
    // 别忘记释放内存
    for (int i = 0; i <num; i++)
    {
        free(pa[i]);
    }
    free(pa);
}

void main(void) 
{
  chap12_9();
}