#include<stdio.h>
void chap3_1()
{
	int a = 65660;
	printf("32768:%d\n", a);
	float b = 64.25;
	//float��ӡĬ��6λ��
	printf("%f\n", b);
}
void chap3_2()
{
	int ASC;
	char ASCC;
	printf("����ASC��\n");
	scanf_s("%d", &ASC);
	printf("�ַ�Ϊ��%c\n", (char)ASC);
}
void chap3_3()
{
	printf("\a");
	printf("Startled by the sudden sound, sally shouted,\n\"By the Great Pumpkin, what was that!\"");
}
void chap3_4()
{
	float notation;
	printf("Enter a floating-point value: \n");
	scanf_s("%e", &notation);
	printf("fixed-point notation:%.6f\n", notation);
	printf("exponential notation: %.6e\n", notation);
	//%xΪʲô���У� ��P201 �嵥4.9����  ��xҪ��������
	printf("p notation: %.2a\n", notation);
	printf("p notation: %#x\n", 64);//float��ʾ0
}
#define SECONDS_PER_YEAR 3.156e7;//3.156*10^7������
void chap3_5()
{
	int year=0;
	printf("Enter your age: \n");
	scanf_s("%d", &year);
	//ע����double������int,��Ϊ%e��Ҫdouble
	double second = year * 3.156e+07;
	double live_seconds = year * SECONDS_PER_YEAR;
	printf("Seconds%e: \n",second);
	printf("Seconds%le: \n", live_seconds);//��P197��l����long
	printf("You have live %f seconds\n", live_seconds);
}
#define SINGLE_WATER_WEIGHT (3e-23);
void chap3_6()
{
	printf("����ˮ�Ŀ�����: \n");
	double kua = 0;
	//double��float��scanf_s����%f,����%lf
	scanf_s("%lf", &kua);
	double water_num = 0;
	water_num = kua *950/SINGLE_WATER_WEIGHT;
	//double��float��printf����%f,����%lf
	//printf������̫�����᲻��
	printf("ˮ��������%lf: \n", water_num);
	printf("ˮ��������%e: \n", water_num);
}
void chap3_7()
{
	printf("������ߣ�/Ӣ�磩: \n");
	float kua = 0;//double kua = 0;�Ͳ���
	scanf_s("%f", &kua);
	printf("��������Ϊ��λ��ʾ���%f: \n", kua*2.54);
}
void chap3_8_1()
{
	printf("���뱭��: \n");
	int cup = 0;
	scanf_s("%d", &cup);
	double pintuo = cup / 2;
	double ansi = pintuo * 8;
	double datangshao = ansi / 2;
	double chashao = datangshao * 3;
	printf("Ʒ�ѣ�%f; ��˾��%f; �����ף�%f; ���ף�%f; \n", pintuo, ansi, datangshao, chashao);

}
void chap3_8_2()
{
	printf("���뱭��: \n");
	int cup = 0;
	scanf_s("%d", &cup);
	int pintuo = cup / 2;
	int ansi = pintuo * 8;
	int datangshao = ansi / 2;
	int chashao = datangshao * 3;
	printf("Ʒ�ѣ�%d; ��˾��%d; �����ף�%d; ���ף�%d; \n", pintuo, ansi, datangshao, chashao);

}
void chap3_8_3()
{
	printf("���뱭��: \n");
	int cup = 0;
	scanf_s("%d", &cup);
	float pintuo = cup / 2;
	float ansi = pintuo * 8;
	float datangshao = ansi / 2;
	float chashao = datangshao * 3;
	printf("Ʒ�ѣ�%f; ��˾��%f; �����ף�%f; ���ף�%f; \n", pintuo, ansi, datangshao, chashao);

}
void main()
{
	chap3_8_3();
}