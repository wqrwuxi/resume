#include<stdio.h>
#include<string.h>
#include <float.h>
void chapter4_1(void)
{
    char first[40];
	char second[40];
	printf("what's your first name?\n");
	scanf("%s" ,first);
	printf("what's your second name?\n");
	scanf("%s",second);
	printf("your name:%s %s", first, second);
}
void chap4_2()
{
    char first_name[40];
    char last_name[40];
	
    printf("What's your first name:");
    scanf("%s", first_name);

    printf("What's your last name:");
    scanf("%s", last_name);
	//strlen()用来计算指定的字符串s 的长度，不包括结束字符"\0"
    int width1=strlen(first_name)+3;
	int width2=strlen(last_name)+3;

    getchar();

    printf("\"%s,%s\"\n",first_name,last_name);
    printf("\"%20s,%20s\"\n", first_name, last_name);
	printf("\"%-20s,%-20s\"\n", first_name, last_name);
	printf("\"%*s,%*s\"\n",width1,first_name,width2,last_name);

}
void chap4_3()
{
	float num;//double不行，那个位数的问题？
	printf("What's your first num:");
	scanf("%f", &num);
	printf("%.2f\n",num);
	printf("%.3e\n",num);

}
void chap4_3_1()
{
	double num;//double不行，那个位数的问题？
	printf("What's your first num:");
	scanf("%lf", &num);
	printf("%.2lf\n",num);
	printf("%.3e\n",num);

}
void chap4_4()
{
	char name[40];
	float inche=0;
	printf("What's your name:\n");
	scanf("%s", name);
	printf("What's your height(inche):\n");
    scanf("%f", &inche);
	float feet=inche*0.8333;//直接放printf里不认
	printf("feet:%.2f\n",feet);
}
void chap4_5()
{
	float megabits=0;
    float megabytes=0;
	printf("What's  megabits per second:\n");
	scanf("%f", &megabits);
    printf("What's  megabytes:\n");
	scanf("%f", &megabytes);
	float seconds=8*megabytes/megabits;
	printf("At %.2f megabits per second, a file of %.2f megabytes downloads in %.2f seconds.\n",megabits,megabytes,seconds);
}
void chap4_6()
{
	char first_name[40];
    char last_name[40];
	
    printf("What's your first name:");
    scanf("%s", first_name);

    printf("What's your last name:");
    scanf("%s", last_name);

	int width1=strlen(first_name);
	int width2=strlen(last_name);

	printf("%s %s\n",first_name,last_name);
	printf("%*d %*d\n",width1,width1,width2,width2);

	printf("%s %s\n",first_name,last_name);
	printf("%*d %*d\n",-width1,width1,-width2,width2);

}
void chap4_7()
{
    double d_value = 1.0/3.0;
    float f_value = 1.0/3.0;
	printf("the value of FLT_DIG:%d, the value of DBL_DIG:%d\n", FLT_DIG, DBL_DIG);
    printf("the value of double:%.6lf, the value of float:%.6lf\n", d_value, f_value);
    printf("the value of double:%.12lf, the value of float:%.12lf\n", d_value, f_value);
    printf("the value of double:%.18lf, the value of float:%.18lf\n", d_value, f_value);

}
#define KM_PER_MILE (1.609)
#define PINT_PER_GALLON (3.785)
void chap4_8()
{
float driven_distance = 0.0;
    float gas_consumption = 0.0;
    float pint_per_hundred_km = 0.0;
    float mile_per_gallon = 0.0;
    printf("How much distance have you traveled in kilometer:");
    scanf("%f", &driven_distance);
    getchar();
 
    printf("How much gas have you used in pint:");
    scanf("%f", &gas_consumption);
    getchar();
 
    pint_per_hundred_km = gas_consumption / driven_distance * 100;
    mile_per_gallon = (driven_distance / KM_PER_MILE) / (gas_consumption / PINT_PER_GALLON);
 
    printf("Fuel consumptions:%f pint/100km or %f mile/gallon\n", pint_per_hundred_km, mile_per_gallon);
}
void main(void)
{
    chap4_3();
}


