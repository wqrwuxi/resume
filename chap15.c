#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <memory.h>
void p15_1(void)
{
    char *pbin = "10010111";//pbin[0]在最左边，想想：this，序号最大的是s
    int num = 0;
    int p_length = strlen(pbin);
 
    for (int i = 0; i < p_length; i++)
    {
        num = num * 2 + (pbin[i] - '0');//(pbin[i] - '0') :ACS码转数字,0的ACS码48
        printf("(pbin[%d] - '0'):%d\n",i,(pbin[i] - '0'));
        printf("num:%d\n",num);
        printf("pbin[i]%c\n",pbin[i]);
    }
 
    printf("the number is %d.\n", num);
}

// practice 3
int check_open_bit(int num)
{
    int open = 0;
    while (num != 0)
    {
        if ((num & 0x01) == 1)//0x01表明只和最右比
        {
            open++;
        }
        num = num >> 1;//右移
    }
 
    return open;
}
void p15_3(void)
{
    int num = check_open_bit(0x55);//0x55=0101 0101
    printf("0x55 has %d open bits.\n", num);
}
// practice 4
int check_bit(int number, int bit)
{
    return (number & (0x01 << (bit - 1)));
}
void p15_4(void)
{
    int num = check_bit(0x55, 1);
    int num2 = check_bit(0x55, 2);
 
    printf("0x55 bit 1 is %d, bit 2 is %d.\n", num, num2);
}

unsigned int rotate_l(unsigned int number, unsigned int ro)
{
    unsigned int tmp = 0;
    unsigned int int_bit = sizeof(unsigned int) * 8;
    for (size_t i = 0; i < ro; i++)
    {
        if (((number >> (int_bit -1)) & 0x01) == 1)
        {
            number = (number << 1) + 0x01;
        }
        else
        {
            number = number << 1;
        }
    }
 
    return number;
}
void p15_5(void)
{
    unsigned int number = rotate_l(0x80000001, 1);//只算8位 P1031
 
    printf("Number:%d", (unsigned int)number);
}
 
 void main(void)
 {
     p15_5();
 }