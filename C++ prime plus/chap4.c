#include<stdio.h>
void chap4_1()
{
	char first[40];
	char second[40];
	printf("what's your first name?\n");
	scanf_s("%s" ,first);
	printf("what's your second name?\n");
	scanf_s("%s",second);
	printf("your name:%s %s", first, second);
}
void chap4_1_1()
{
    char first_name[40];
    char last_name[40];
    printf("What's your first name:");
    scanf_s("%s", first_name);

    printf("What's your last name:");
    scanf_s("%s", last_name);

    getchar();

    printf("%*s,%*s\n", 10, first_name, 10, last_name);

    return;
}
void chap4_2()
{
    char first_name[40];
    char last_name[40];
    printf("What's your first name:");
    scanf_s("%s", first_name);

    printf("What's your last name:");
    scanf_s("%s", last_name);

    getchar();

    printf("\"%s,%s\"\n",first_name,last_name);
    printf("\"%20s,%20s\"\n", first_name, last_name);

}
void main()
{
	chap4_1();
}