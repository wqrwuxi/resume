#include <stdio.h>
#include <stdlib.h>//mallic
//结构体定义
typedef struct{
    char ch;
    char morse_m[5];
}Morse_info;
///////////////////////////////////////////////////////////////////////////////
//变量声明，初始化
Morse_info Morse_Code[26]={
    {'A',".-"},{'B',"-..."},{'C',"-.-."},{'D',"-.."},{'E',"."},
    {'F',"..-."},{'G',"--."},{'H',"...."},{'I',".."},{'J',".---"},
    {'K',"-.-"},{'L',".-.."},{'M',"--"},{'N',"-."},{'O',"---"},
    {'P',".--."},{'Q',"--.-"},{'R',".-."},{'S',"..."},{'T',"-"},
    {'U',"..-"},{'V',"...-"},{'W',".--"},{'X',"-..-"},{'Y',"-.--"},
    {'Z',"--.."}
};
const char *englishToMorse(char letter)
{
 
    int index=0;
    char *Morse;
    Morse=(char*)malloc(5*sizeof(char));//字符串数组，空字符位置要留
                                        //只有使用malloc，才能传回去。

    if(letter==' ')
    {
        Morse="/";
    }
    else if (letter>='a' && letter<='z')
    {
        index=(int)letter-96;
        Morse=Morse_Code[index-1].morse_m;
    }
    else if (letter>='A' && letter<='Z')
    {
        index=(int)letter-64;
        Morse=Morse_Code[index-1].morse_m;
    }
    else
    {
       Morse="eeee";
       
    } 
    return Morse;//将非const指针赋给const指针是可以的  C++primer P183
    
}
/*
void main(void)
{
    char ch;
    printf("Input a character:");
    //ch=getchar();
    scanf("%c",ch);
    char *p = englishToMorse(ch);
    printf("Morse:%s\n",p);
}
*/