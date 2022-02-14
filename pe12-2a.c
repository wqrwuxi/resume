#include <stdio.h>
static int miles;
static int kilometers;
//extern int mode;方案1,不好
void set_mode(int mode)
{
   if(mode!=0 & mode!=1)
   {
        printf("Invalid mode specified. Mode %s used.\n", (mode ? "1(US)" : "0(metric)"));//抄的参考答案
   }

}

void get_info(int mode)
{

  switch(mode)
  {
    case 0:
       puts("Enter distance traveled in kilometers:");
       scanf("%d",&kilometers);
       break;
    case 1:
       puts("Enter distance traveled in miles:");
       scanf("%d",&miles);
       break;
  }

  return;
  
}

void show_info(int mode)
{
    double liters;
    double gallons;
    double per_km;
    double per_gallon;

   switch(mode)
  {
    case 0:
       puts("Enter fuel consumed in liters:");
       scanf("%lf",&liters);
       per_km=liters/(kilometers/100);
       printf("Fuel consumption is %.2lf liters per 100 km.",per_km);
       break;
    case 1:
       puts("Enter fuel consumed in gallons:");
       scanf("%lf",&gallons);
       per_gallon=miles/gallons;
       printf("FFuel consumption is %.2lf miles per gallon.",per_gallon);
       break;
  }

  return;

}