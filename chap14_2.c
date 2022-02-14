// practice 2
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct myy{
   int MONTH_int;
   char full_name[12];
   char short_name[4];
   int days;
};
void main(void)
{
    int year;
    char month[12];
    int day=0;
    int day_total=0;
    char *ptr;

    struct myy my_months[5]={//查了半天少个等号
  {1,"January","JAN",31},
  {2,"February","FEB",28},
  {3,"March","MAR",31},
  {4,"April","APR",30},
  (5,"May","MAY",31)
  };

    printf("Input the year:");
    scanf("%d",&year);
    getchar();
    printf("Input the month:");
    scanf("%s",month);
    //fgets(month,12,stdin);用这个会有一个换行符，比对肯定不对阿
    getchar();
    printf("Input the day:");
    scanf("%d",&day);
    getchar();


    for(int i=0;i<5;i++)
    {
        
            //11.9 把字符串转换为数字 atoi() strtol()
            if(strtol(month,&ptr,10)==my_months[i].MONTH_int || (strcmp(month,my_months[i].full_name)==0) || (strcmp(month,my_months[i].short_name)==0))
            {
                break;
            }
      
        day_total+=my_months[i].days;
    }

    day_total+=day;
    printf("total days:%d",day_total);

}
