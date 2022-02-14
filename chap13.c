#include <stdio.h>
#include <stdlib.h> // 提供 exit()的原型
#include <string.h> // 提供 strcpy()、strcat()的原型
#include <ctype.h>
// practice 1
//命令行参数 PDF P860 13.2.1 检查命令行参数
//PDF P860：首先，程序清单13.1中的程序检查argc的值，查看是否有命令行参数
//int main(int argc, char *argv [])中的int argc, char *argv []
//字符串 argv[0]是该程序的名称   if ((fp = fopen(argv[1], "r")) == NULL)
void chap13_1(void)
{
    FILE* fp;
    char filename[40];
    char ch;

    printf("Enter the file name(one word): ");
    scanf("%s",filename);

    while((fp=fopen(filename,"r"))==NULL)
    {
        printf("OPEN FILE FAIL!BYE!\n");
        exit(1);
    }

    while ((ch = getc(fp)) != EOF)
    {
        putc(ch, stdout);
    }

    fclose(fp);
}
// practice 3
void chap13_3(void)
{
    FILE* fp1;
    FILE* fp2;
    char ch;
    char filename[40];

    printf("Enter the file name(one word): ");
    scanf("%s",filename);

    if((fp1=fopen(filename,"r"))==NULL)
    {
       printf("OPEN FOR READ FAIL!BYE!\n");
       exit(EXIT_FAILURE);
    }

   if((fp2=fopen(filename,"a+"))==NULL)
   {
       printf("OPEN FOR WRITE FAIL!BYE!\n");
       exit(EXIT_FAILURE);
   }

   while((ch=getc(fp1))!=EOF)
   {
     putc(toupper(ch),fp2);
   }


   //while(feof(fp1)==0)//和上一个while同用法 参考答案看到了feof，但是结尾有奇怪字符？
   //{
    //   ch=getc(fp1);
     //  putc(toupper(ch),fp2);
   //}
   /*
   feof()函数用来检测当前文件流上的文件结束标识，判断是否读到了文件结尾，其原型为：int feof(FILE * stream);
   检测到文件结束标识返回1，否则返回0。
   对feof()来说,它的工作原理是，站在光标所在位置，向后看看还有没有字符。如果有，返回0；如果没有，返回非0。它并不会读取相关信息，只是查看光标后是否还有内容。
   对于一个空文件来说，当程序打开它的时候，它的光标会停在文件的开头，但是由于文件里什么内容都没有存（但是EOF是存在的），即整个文件就存贮了一个EOF。当程序打开文件，并直接调用feof()时，这个函数就会站在光标的位置向后张望，结果就看见了EOF，然后就当然返回0了。
   !!!先使用getc(),从文件中读取一个字符，让光标向后移动一个字符。这时空文件的光标就已经移动到EOF的后面，这时使用feof()就会返回1了。这才是feof()的正确用法。
   https://blog.csdn.net/konghouy/article/details/80530937
   */

   if (fclose(fp1) != 0 || fclose(fp2) != 0)   //书P868 +  13.2.5 fclose()函数
   {
     fprintf(stderr, "Error in closing files\n");
   }
   
}
#define LENS 512
void chap13_7(void)
{
    FILE* fp1;
    FILE* fp2;
    char filename1[40];
    char filename2[40];
    char words[LENS];
    int length=0;

    printf("Enter the file name(file one): ");
    scanf("%s",filename1);
    getchar();
    printf("Enter the file name(file two): ");
    scanf("%s",filename2);
    getchar();

    if ((fp1=fopen(filename1,"r"))==NULL)
    {
       printf("OPEN FOR READ FAIL!BYE!\n");
       exit(EXIT_FAILURE);
    }

    if ((fp2=fopen(filename2,"r"))==NULL)
    {
       printf("OPEN FOR READ FAIL!BYE!\n");
       exit(EXIT_FAILURE);
    }

     //下面这个while有难度
     //该程序以这样的顺序打印：打印第1个文件的第1行，第2个文件的第1行，第1个文件的第2行，第2个文件的第2行，以此类推，打印到行数较多文件的最后一行。
     //^^^^^^^^^^^^^^^^^^^^^^^^^^注意^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
     while ((feof(fp1) == 0) || (feof(fp2) == 0))//用while和if实现，没想到
     {
         
         if(fgets(words,LENS,fp1)!=NULL)//https://www.runoob.com/cprogramming/c-function-fgets.html
         {                              //NULL是抄的，fgets(words,LENS,fp1)!=NULL不会    fgets只读一行
           // 要实现B情况，添加如下代码
            /*
            length=strlen(words);
            printf("length:%d:",length);
            if (words[length-1] == '\n')//换行符算长度的，所以length-1
            {
                words[length-1] = '\0';
            }
            */
           fputs(words, stdout);
         }

        if(fgets(words,LENS,fp2)!=NULL)
         {
           fputs(words, stdout);
         }

     }
    

    if(fclose(fp1)!=0)
    {
        fprintf(stderr, "Error in closing files one\n");
    }

    if(fclose(fp2)!=0)
    {
        fprintf(stderr, "Error in closing files two\n");
    }

}
// practice 9
#define MAX 41
void chap13_9(void)
{
    FILE *fp;
    char words[MAX];
    int count=0;
    int num_contain=0;
    if ((fp = fopen("wordy", "a+")) == NULL)
    {
        fprintf(stdout, "Can't open \"wordy\" file.\n");
        exit(EXIT_FAILURE);
    }
    puts("Enter words to add to the file; press the #");
    puts("key at the beginning of a line to terminate.");
    rewind(fp);

    while (fgets(words, MAX, fp) !=NULL)
    {
        count++;
    }

    count=count/2;

    while ((fscanf(stdin, "%40s", words) == 1) && (words[0] != '#'))
    {
        count++;
        fprintf(fp, "%d\n", count);
        fprintf(fp, "%s\n", words);
    }

    puts("File contents:");
    rewind(fp); /* 返回到文件开始处 */

    while (fscanf(fp, "%s", words) == 1)
    {
        puts(words);
    }

    puts("Done!");

    if (fclose(fp) != 0)
    {
       fprintf(stderr, "Error closing file\n");
    }

}
// practice 10
void p13_10(void)
{
    char file_name[256];
    char content[256];
    int file_seek = 0;
    FILE *fp;
 
    printf("Enter the file name:");
    fscanf(stdin, "%s", file_name);
//  scanf("%s", file_name);
 
    if ((fp = fopen(file_name, "r")) == NULL)
    {
        printf("Can't open %s file.\n", file_name);
    }
 
    while (1)
    {
        printf("Enter the location of file to print ( <0 to terminate) :");
        if (fscanf(stdin, "%d", &file_seek) != 1 || file_seek < 0)
        {
            break;
        }

        fseek(fp, file_seek, SEEK_SET);//这个函数！！！


        //然后该程序打印从该位置开始到下一个换行符之前的内容。fgets只读一行，不就是 换行符为界么
        fgets(content, 256, fp);
//      fscanf(fp, "%s", content);
//      奇怪，下面这两个打印方法都没有办法打印 (好吧，我知道了，我这是输出啊，应该用的是stdout，汗....)
//      fprintf(stdin, "%s", content);
//      fputs(content, stdin);
        printf("%s", content);
    }
 
    puts("Done");
    fclose(fp);
 
    return;
}

void main(void)
{
   chap13_9();
}
