// practice 8。和第五题一个问题
#include<stdio.h>
#include <string.h>
char* mystring_in(char* group,char* sshort)//short系统关键词，不能用！
{
    int flag=-1;
    printf("strlen(group)%d:",strlen(group));
    printf("strlen(sshort)%d:",strlen(sshort));
    printf("group:%s,comp:%s\n",group,sshort);
    for(int i=0;i<strlen(group);i++)
    {

        if (group[i]==sshort[0])
        {
          flag=0;

          for(int j=1;j<strlen(sshort);j++)
          {
             
             if(group[i+j]!=sshort[j])
             {
                flag=1;
                break;
             }
          }
          
          if(flag==0)
          {
              return &group[i];
          }

        }
    
    }

    return NULL;
 
    //printf("address of need:%p\n",my_point);
     printf("address of group:%p\n",group);
}
void chap11_8(void)
{
   int num1;
   int num2;

   printf("Enter the length of the origin:");
   scanf("%d",&num1);
   getchar();//不写下面字符串第一个就是回车
   char origin[num1];

   for(int i=0;i<num1;i++)
   {
      origin[i]=getchar();
   }
   
   //strlen计算时，一定要确保字符数组是以空字符结束的，如果没有则可能沿着数组在内存中的位置不断向前寻找，知道遇到空字符才停下来。
   //试过最后一个位置不输入，strlen不是想要的
   //但为什么这种情况下strlen包括强加的\0？？
   origin[num1]='\0';

   printf("Enter the length of the compare:");
   scanf("%d",&num2);
   getchar();//不写下面字符串第一个就是回车
   char compare[num2];

   for(int i=0;i<num2;i++)
   {
      compare[i]=getchar();
   }
   getchar();
   compare[num2]='\0';

printf("strlen(origin)%d:",strlen(origin));
printf("strlen(compare)%d:",strlen(compare));

   char* answer=mystring_in(origin,compare);
   printf("address of origin:%p\n",origin);
   printf("address of origin[2]:%p\n",&origin[2]);
   printf("address of answer:%p\n",answer);
}

void main(void)
{
   chap11_8();
}
/*
[[wqr@localhost chapter11]$ ./task8
Enter the length of the origin:8
zxcvbnm,
Enter the length of the compare:3
vbn
strlen(origin)9:strlen(compare)4:strlen(group)9:strlen(sshort)4:group:zxcvbnm,�,comp:vbn�
address of origin:0x7fffcdee9e90
address of origin[2]:0x7fffcdee9e92
address of answer:0x7fffcdee9e93

Q：strlen为什么不是8和3
Q：group和comp后面是乱码，明明手工加'\0'

bug小结：

乱码来源：比如是8个字符，numi=8，但是我的字符存在0-7的下标中，
我却设置数组为char origin[num1+1]，那个group和comp就是origin[num1]那个空位上的（初始化数组时没有指定数组什么样，什么字符都可能

strlen:查bug查了半天，网上说：strlen计算时，一定要确保字符数组是以空字符结束的，如果没有则可能沿着数组在内存中的位置不断向前寻找，知道遇到空字符才停下来。
所以数组最后一个，即 数组[num]='\0'

scanf后面一定要跟getchar()，不然空格什么时候被别的字符收了都不知道
*/