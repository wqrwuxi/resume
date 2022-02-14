// practice 8
#include <stdio.h>
#include <stdlib.h> // 提供 exit()的原型
//argc是命令行总的参数个数  
//argv[]是argc个参数，其中第0个参数是程序的全名，以后的参数是命令行后面跟的用户输入的参数
/*
char *argv[]是一个字符数组,其大小是int argc,主要用于命令行参数argv[]参数，数组里每个元素代表一个参数;
比如你输入  
   test   a.c   b.c   t.c  
   则  
   argc   =   4  
    
   argv[0]   =   "test"  
   argv[1]   =   "a.c"  
   argv[2]   =   "b.c"  
   argv[3]   =   "t.c"
   https://www.cnblogs.com/avril/archive/2010/03/22/1691477.html#:~:text=argv%20%5B%5D%E6%98%AFargc%E4%B8%AA%E5%8F%82%E6%95%B0%EF%BC%8C%E5%85%B6%E4%B8%AD%E7%AC%AC0%E4%B8%AA%E5%8F%82%E6%95%B0%E6%98%AF%E7%A8%8B%E5%BA%8F%E7%9A%84%E5%85%A8%E5%90%8D%EF%BC%8C%E4%BB%A5%E5%90%8E%E7%9A%84%E5%8F%82%E6%95%B0%E6%98%AF%E5%91%BD%E4%BB%A4%E8%A1%8C%E5%90%8E%E9%9D%A2%E8%B7%9F%E7%9A%84%E7%94%A8%E6%88%B7%E8%BE%93%E5%85%A5%E7%9A%84%E5%8F%82%E6%95%B0%EF%BC%8C%E6%AF%94%E5%A6%82%EF%BC%9A%201%20int%20main%20%28int%20argc%2C%20char,6%20cin%3E%3E%20i%3B%207%20return%200%3B%208%20%7D
*/

void main(int argc, char* argv[])
{
    char ch;
    char words[128];
    FILE* fp;
    int count=0;
    int lens=0;

    if(argc==2)
    {
        
        printf("Input some words:\n");

        while((ch=getchar())!='\n')
        {
           putc(ch,stdout);
        }
        
        printf("\n");
    }
    else
    {
        printf("the character itself:%c\n",argv[1]);

        for(int i=0;i<argc-2;i++)
        {
           
           if((fp=fopen(argv[i+2],"r"))==NULL)
           {
               printf("Can't open file %s as read!\n", argv[i+2]);
               exit(EXIT_FAILURE);
           }

           printf("filename:%s\n",argv[i+2]);
           count=0;
       
           while ((ch=getc(fp)) != EOF)
           {
            
             if(ch==*(argv[1]))
             {
                count++;
             }
             
           }

           printf("The times the character appears:%d\n",count);
           if(fclose(fp)!=0)
           {
               fprintf(stderr, "Error in closing files one\n");
           }

        }
    }


}