#include <stdio.h>
#include <string.h>
//结构体定义
typedef struct{
    char ch_m;
    char morse_m[5];//字符串数组，空字符位置要留
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
char morseToEnglish(const char *morse)
{
    char ch;

    if(strcmp(morse,"eeee")==0)
    {
        ch='#';
    }
    else if(strcmp(morse,"/")==0)
    {
        ch=' ';
    }
    else
    {
        for(int i=0;i<26;i++)
        {
           
           if(strcmp(Morse_Code[i].morse_m,morse)==0)
           {
              ch=Morse_Code[i].ch_m;
              break;
           }
        }
    }

    return ch;
}
/*void main(void)
{
    char input[6];
    char output;
   
    for(int i=0;i<5;i++)
    {
        if(Morse_Code[4].morse_m[i]=='\0')
        {
            printf("kong%d:,",i);
        }
        else if (Morse_Code[4].morse_m[i]=='\n')
        {
            printf("hui%d:,",i);
        }
        
    }
    printf("Input a morse:");
    fgets(input,6,stdin);
    //fgets()函数的第2个参数指明了读入字符的最大数量。如果该参数的值是n，那么fgets()将读入n-1个字符，或者读到遇到的第一个【换行符】为止
    //6保证可见字符之后一定是换行符，方便统一处理
    //input因此也是6个字节
    
    int len=strlen(input);
    input[len-1]='\0';//是len-1，第n个字符序号n-1
    output=morseToEnglish(input);
    if(output==' ')
    {
        printf("空格");
    }
    else
    {
        printf("output:%c\n",output);
    }
    

}
//47行，63行，68,69行最重要，数字一定要统一*/
