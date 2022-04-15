#include<stdio.h>
void chap3_1()
{
	int a = 65660;
	printf("32768:%d\n", a);
	float b = 64.25;
	//float打印默认6位？
	printf("%f\n", b);
}
void chap3_2()
{
	int ASC;
	char ASCC;
	printf("输入ASC码\n");
	scanf_s("%d", &ASC);
	printf("字符为：%c\n", (char)ASC);
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
	//%x为什么不行？ 书P201 清单4.9可以  答：x要求整数！
	printf("p notation: %.2a\n", notation);
	printf("p notation: %#x\n", 64);//float显示0
}
#define SECONDS_PER_YEAR 3.156e7;//3.156*10^7不可以
void chap3_5()
{
	int year=0;
	printf("Enter your age: \n");
	scanf_s("%d", &year);
	//注意是double，不是int,因为%e需要double
	double second = year * 3.156e+07;
	double live_seconds = year * SECONDS_PER_YEAR;
	printf("Seconds%e: \n",second);
	printf("Seconds%le: \n", live_seconds);//书P197，l代表long
	printf("You have live %f seconds\n", live_seconds);
}
#define SINGLE_WATER_WEIGHT (3e-23);
void chap3_6()
{
	printf("输入水的夸脱数: \n");
	double kua = 0;
	//double和float在scanf_s都用%f,甚至%lf
	scanf_s("%lf", &kua);
	double water_num = 0;
	water_num = kua *950/SINGLE_WATER_WEIGHT;
	//double和float在printf都用%f,甚至%lf
	//printf参数别太长，会不认
	printf("水分子数量%lf: \n", water_num);
	printf("水分子数量%e: \n", water_num);
}
void chap3_7()
{
	printf("输入身高（/英寸）: \n");
	float kua = 0;//double kua = 0;就不行
	scanf_s("%f", &kua);
	printf("后以厘米为单位显示身高%f: \n", kua*2.54);
}
void chap3_8_1()
{
	printf("输入杯数: \n");
	int cup = 0;
	scanf_s("%d", &cup);
	double pintuo = cup / 2;
	double ansi = pintuo * 8;
	double datangshao = ansi / 2;
	double chashao = datangshao * 3;
	printf("品脱：%f; 盎司：%f; 大汤勺：%f; 茶勺：%f; \n", pintuo, ansi, datangshao, chashao);

}
void chap3_8_2()
{
	printf("输入杯数: \n");
	int cup = 0;
	scanf_s("%d", &cup);
	int pintuo = cup / 2;
	int ansi = pintuo * 8;
	int datangshao = ansi / 2;
	int chashao = datangshao * 3;
	printf("品脱：%d; 盎司：%d; 大汤勺：%d; 茶勺：%d; \n", pintuo, ansi, datangshao, chashao);

}
void chap3_8_3()
{
	printf("输入杯数: \n");
	int cup = 0;
	scanf_s("%d", &cup);
	float pintuo = cup / 2;
	float ansi = pintuo * 8;
	float datangshao = ansi / 2;
	float chashao = datangshao * 3;
	printf("品脱：%f; 盎司：%f; 大汤勺：%f; 茶勺：%f; \n", pintuo, ansi, datangshao, chashao);

}
void main()
{
	chap3_8_3();
}