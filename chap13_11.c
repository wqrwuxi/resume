#include <stdio.h>
#include <stdlib.h> // 提供 exit()的原型
#include <string.h>//strstr()
#define MAX 255
void main (int argc, char *argv[])
{
    char words[MAX];
    int count=0;
    FILE* fp;

    if ((fp = fopen(argv[2], "r")) == NULL)
    {
        printf("Can't open file %s as read!\n", argv[2]);
        exit(EXIT_FAILURE);
    }

    while (fgets(words, MAX, fp) !=NULL)//因为fgets读到第一个换行符，能把文档切成一行行的
    {
        count++;

        if((strstr(words,argv[1]))!=NULL)
        {
           printf("The %dth line has the target string",count);
        } 

    }

} 