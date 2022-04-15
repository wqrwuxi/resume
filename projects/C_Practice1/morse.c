/**
 * @file morse.c
 * @author binary (binary@techbootcamp)
 * @brief 
 * @version 0.1
 * @date 2021-10-28
 * 
 * @copyright Copyright (c) 2021
 * 
 */

///////////////////////////////////////////////////////////////////////////////
//头文件区域。在这里加上必要的头文件
#include <stdio.h>
#include <stdlib.h> // 提供 exit()的原型  mallic
#include <stdio.h>
#include <string.h>
///////////////////////////////////////////////////////////////////////////////
//宏定义。在这里加上必要的宏定义
#define lineE 255
#define lineMc 1785
#define filenameLen 40
///////////////////////////////////////////////////////////////////////////////
//结构体定义
typedef struct{
    char chM;
    char morseM[5];//字符串数组，空字符位置要留
}MorseInfo;
///////////////////////////////////////////////////////////////////////////////
//变量声明，初始化
MorseInfo MorseCode[26]={
    {'A',".-"},{'B',"-..."},{'C',"-.-."},{'D',"-.."},{'E',"."},
    {'F',"..-."},{'G',"--."},{'H',"...."},{'I',".."},{'J',".---"},
    {'K',"-.-"},{'L',".-.."},{'M',"--"},{'N',"-."},{'O',"---"},
    {'P',".--."},{'Q',"--.-"},{'R',".-."},{'S',"..."},{'T',"-"},
    {'U',"..-"},{'V',"...-"},{'W',".--"},{'X',"-..-"},{'Y',"-.--"},
    {'Z',"--.."}
};

//是否摩尔斯点码
int mc = 0;
int useLower = 0;

///////////////////////////////////////////////////////////////////////////////
//函数声明区域

/**
 * @brief 英文转换到摩尔斯电码
 * 
 * 根据字母映射到对应的摩尔斯电码
 * 
 * @param letter 英文字母
 * @return const char* 摩尔斯电码        1
 */
const char *englishToMorse(char letter);

/**
 * @brief 摩尔斯电码转换回英文
 * 
 * 根据摩尔斯编码映射到字母，需要字符串比较
 * 
 * @param morse 摩尔斯电码
 * @return char 英文字母                2
 */
char morseToEnglish(const char *morse);

/**
 * @brief 英文转换到摩尔斯电码，这是一行数据
 * 
 * 处理一行字母，转换到摩尔斯电码。
 * 注意：需要一个字母一个字母处理
 * 
 * @see const char *englishToMorse(char letter)    3
 * 
 */
void englishToMorseLine(FILE* fp_r,FILE* fp_w);

/**
 * @brief 摩尔斯电码转换回英文，这是一行数据
 * 
 * 处理一行摩尔斯电码格式的数据，转换回英文字母
 * 注意：需要先分离出单个摩尔斯电码，再进行转换
 * 
 * @see char morseToEnglish(const char *morse)    4
 * 
 */
void morseToEnglishLine(FILE* fp_r,FILE* fp_w);

/**
 * @brief 处理英文转换到摩尔斯电码
 * 
 * 整个业务处理流程都在这里做：
 * 1.打开输入、输出文件，需要考虑哪个文件需要读或写的权限，如果没有文件该怎么办。
 * 2.逐行读取数据，并进行英文转换到摩尔斯电码，并输出数据到文件。
 * 3.关闭输入、输出文件。
 * 
 * @see void englishToMorseLine()                5
 * 
 * @param inFile 输入文件名
 * @param outFile 输出文件名
 */
void handlerEnglishToMorse(const char *inFile, const char *outFile);

/**
 * @brief 处理摩尔斯电码转换回英文
 * 
 * 整个业务处理流程都在这里做：
 * 1.打开输入、输出文件，需要考虑哪个文件需要读或写的权限，如果没有文件该怎么办。
 * 2.逐行读取数据，并进行摩尔斯电码转换回英文，并输出数据到文件。
 * 3.关闭输入、输出文件。
 * 
 * @see void morseToEnglishLine()              6
 * 
 * @param inFile 输入文件名
 * @param outFile 输出文件名
 */
void handlerMorseToEnglish(const char *inFile, const char *outFile);

/**
 * @brief 解析命令行参数
 * 
 * 检查参数个数，及其的合法性，如果有错误，则需要输出用法，告诉用户如何使用，并退出程序。
 * 需要分析当前是那种转换方式，并赋值给变量mc
 * 
 * @param argc 参数个数
 * @param argv 参数值数组                    7
 */
void parseArgs(int argc, char **argv);

///////////////////////////////////////////////////////////////////////////////
//已经实现的函数

/**
 * @brief 开启小写字母转换(默认是转回为大写字母)
 * 
 */
void enableLetterLower()
{
    useLower = 1;
}

/**
 * @brief 处理业务
 * 
 * @see void handlerEnglishToMorse(const char *inFile, const char *outFile)
 * @see void handlerMorseToEnglish(const char *inFile, const char *outFile)
 * 
 * @param inFile 输入文件名
 * @param outFile 输出文件名
 */
void handler(const char *inFile, const char *outFile)
{
    //extern int mc;
    if (mc)
    {
        handlerEnglishToMorse(inFile, outFile);
    }
    else
    {
        handlerMorseToEnglish(inFile, outFile);
    }
}

//主函数入口
int main(int argc, char **argv)
{
    //解析参数
    parseArgs(argc, argv);
    //默认为大写字母，如果开启则转换为小写字母
    //enableLetterLower();
    //处理业务
    handler(argv[1], argv[3]);

    return 0;
}

///////////////////////////////////////////////////////////////////////////////
//以下为需要自行实现的函数

const char *englishToMorse(char letter)//自作主张去掉const
{
    int index=0;
    char *morseP;
    morseP=(char*)malloc(5*sizeof(char));//字符串数组，空字符位置要留
                                        //只有使用malloc，才能传回去。

    if(letter==' ')
    {
        morseP="/";
    }
    else if (letter>='a' && letter<='z')
    {
        index=(int)letter-96;
        morseP=MorseCode[index-1].morseM;
    }
    else if (letter>='A' && letter<='Z')
    {
        index=(int)letter-64;
        morseP=MorseCode[index-1].morseM;
    }
    else
    {
       morseP="eeee";
       
    } 
    return morseP;//将非const指针赋给const指针是可以的  C++primer P183
}

char morseToEnglish(const char *morse)
{
    char ch='\0';//打印出来是乱码

    if(strcmp(morse,"eeee")==0)//字符串比较用函数！==判断的是数字
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
           
           if(strcmp(MorseCode[i].morseM,morse)==0)
           {
              ch=MorseCode[i].chM;
              break;
           }
        }
    }

    return ch;
}

void englishToMorseLine(FILE* fp_r,FILE* fp_w)
{
    char line[lineE];
    char filename_r[filenameLen];
    char filename_w[filenameLen];
    int count=0;
    char* p;
    int length;

    while (feof(fp_r) == 0)
    {

    while (fgets(line, lineE, fp_r) !=NULL)
    {

       length=strlen(line);
       line[length]='\n';//人工添加回车，不是每行都有回车
       count=0;//查了好久，从matlab到c？！
       while(line[count]!='\n')
       {
          p = englishToMorse(line[count]);
          fprintf(fp_w,"%s|",p);
          count++;
       }
        
       fprintf(fp_w,"\n");
       memset(line,'\0', lineE);//如果字符串赋值时有结束符号（\0），可以不清零，没有的话就得清
                                //morseToEnglishLine不用应该是因为用的测试例子是程序生成的，自动在最后加了

    }

    }
}

void morseToEnglishLine(FILE* fp_r,FILE* fp_w)
{
    char line[lineMc];
    char filename_r[filenameLen];
    char filename_w[filenameLen];
    int count=0;
    int count_word=0;
    //char temp[5]={'\0'};//必须初始化到全空，不然天知道temp一开始里面有什么，所以第一个字母无法打印
    char temp[5]={0};//也可以，\0对应的ASC值为0
    char ch;
    int length;
    char *pch;

    while (feof(fp_r) == 0)
    {
        while (fgets(line, lineMc, fp_r) !=NULL)
        {
       length=strlen(line);
       line[length]='\n';//人工添加回车，不是每行都有回车,不加这行读完再读出的东西不保证
       count=0;//查了好久，从matlab到c？！
       pch=strtok(line,"|");
       while(pch!=NULL)
       {
           ch=morseToEnglish(pch);
           if(ch=='\0')
           {
               break;
           }
           putc(ch,fp_w);
           pch=strtok(NULL,"|");
       }
      /* while(line[count]!='\n')
       {
    
          if(line[count]!='|')
          {
              temp[count_word]=line[count];
              count_word++;
          }
          else
          {
              ch=morseToEnglish(temp);
              //fprintf(fp_w,"%c",ch);//再测试又可以了？
             // fputs(&ch,fp_w);//再测试又可以了？
              putc(ch,fp_w);
              count_word=0;
              memset(temp, '\0', 5);
          }

          count++;
       }*/
       memset(line,'\0', lineMc);
       fprintf(fp_w,"\n");

       }
    }      
}

void handlerEnglishToMorse(const char *inFile, const char *outFile)
{
    FILE* fp_r;
    FILE* fp_w;

    if((fp_r=fopen(inFile,"r"))==NULL)
    {
        printf( "Can't open the read file.\n");
        exit(EXIT_FAILURE);
    }
 
    if((fp_w=fopen(outFile,"a+"))==NULL)
    {
        printf( "Can't open the write file.\n");
        exit(EXIT_FAILURE);
    }

    englishToMorseLine(fp_r,fp_w);

    if (fclose(fp_r) != 0 || fclose(fp_w) != 0)  
    {
     fprintf(stderr, "Error in closing files\n");
    }
}

void handlerMorseToEnglish(const char *inFile, const char *outFile)
{
    FILE* fp_r;
    FILE* fp_w;

    if((fp_r=fopen(inFile,"r"))==NULL)
    {
        printf( "Can't open the read file.\n");
        exit(EXIT_FAILURE);
    }
 
    if((fp_w=fopen(outFile,"a+"))==NULL)
    {
        printf( "Can't open the write file.\n");
        exit(EXIT_FAILURE);
    }

    morseToEnglishLine(fp_r,fp_w);

    if (fclose(fp_r) != 0 || fclose(fp_w) != 0)  
    {
     fprintf(stderr, "Error in closing files\n");
    }
}

void parseArgs(int argc, char **argv)
{
  // extern int mc;

  if (argc!=4)
  {
    printf("Wrong input.\n");
    printf("The first parameter is the name of program.\n");
    printf("The second parameter is the input file name.\n");
    printf("The thied parameter is \"-mc\" or \"-e\".\"-mc\"means switching to Morse code.\"-e\"means converting to English.\n");
    printf("The fourth parameter is the output file name.\n");
    exit(EXIT_FAILURE);
  }
  else if(argc==4)
  {

      if(strcmp(argv[2],"-mc")==0)
      {
          mc=1;
      }
      else if(strcmp(argv[2],"-e")==0)
      {
          mc=0;
      }
      
  }
  
}
