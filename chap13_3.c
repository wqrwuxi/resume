#include <stdio.h>
#include <stdlib.h> // 提供 exit()的原型
#include <string.h> // 提供 strcpy()、strcat()的原型
//gcc -o test2 chap13_3.c
//./test2 text_one text_two
void main(int argc, char *argv [])
{
   FILE* fp1;
   FILE* fp2;
   char ch;

   if (argc != 3)
   {
     printf("Parameter is not enough\n");
     exit(EXIT_FAILURE);
   }

   if((fp1=fopen(argv[1],"r"))==NULL)
   {
       printf("OPEN %s FAIL!BYE!\n",argv[1]);
       exit(EXIT_FAILURE);
   }

   if((fp2=fopen(argv[2],"a+"))==NULL)
   {
       printf("OPEN %s FAIL!BYE!\n",argv[2]);
       exit(EXIT_FAILURE);
   }
  
   while((ch=getc(fp1))!=EOF)
   {
       putc(ch,fp2);
   }
   
   fclose(fp1);
   fclose(fp2);
}