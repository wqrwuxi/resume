#include <stdio.h>

void code2_1(void)
{
	printf("% s \n" , "Cus Mah");
	printf("% s \n", "Cus");
	printf("% s \n", "Mah");
	printf("% s", "Cus ");
	printf("% s", "Mah");

	//�ο������£����Բ�����ô����
	printf("Jimmy Chen\n");
	printf("Jimmy\nChen\n");
	printf("Jimmy");
	printf(" Chen\n");
}
void code2_2(void)
{
	char name[20]="Cus Mah";
	char adress[20] ="Road 2";
	//����ڶ��У�Road 2ǰ��һ���ո�ע������
	printf("% s \n%s \n", name,adress);
	printf("% s \n %s \n", name, adress);
}
void code2_3(void)
{
	int year=0;
	int day = 0;
	printf("How old are you?\n");
	//scanf��% d��printf����% d������Ϊ�ᵼ��%d������ ǰ �и��ո�
	//yearǰ&��©
	scanf_s("%d",&year); 

	getchar(); // ɾ�����뻺�����еĻس���(�ο��𰸣�

	day = 365 * year;
	printf("year:%d,day:%d\n" , year, day);
}
void jolly(void)
{
	printf("For he's a jolly good fellow!\n");
}
void deny(void)
{
	printf("Which nobody can deny!\n");
}
void code2_4(void)
{
	for (int i = 0;  i < 3;i++)
		jolly();
		
	deny();
}
void br(void)
{
	printf("Brazil, Russia");
}
void ic(void)
{
	printf("India, China");
}
void code2_5(void)
{
	br();
	printf(" ,");
	ic();
	printf("\n");
	br();
	printf(",\n");
	ic();
}
void code2_6(void)
{
	int toes = 10;
	printf("toes:%d, 2*toes:%d, toes^2:%d\n", toes, 2 * toes, toes^2);
}
void code2_7(void)
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 3-i; j > 0; j--)
			printf(" Smile!");
		printf("\n");
	}
}
void one_three(void)
{
	printf("One\n");
}
void two(void)
{
	printf("Two\n");
}
void code2_8(void)
{
	printf("starting now:\n");
	one_three();
	two();
	printf("Three\nDone!");

}

void main(void)
{
	//code2_1();
	//code2_2();
	code2_8();
}