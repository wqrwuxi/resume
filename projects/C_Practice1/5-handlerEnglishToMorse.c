#include <stdio.h>
#include <stdlib.h> // 提供 exit()的原型
#include "small_test_5.h"
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
/*void main(void)
{
    char filename_r[40];
    char filename_w[40];
    printf("Enter the file to be read:");
    scanf("%s",filename_r);
    printf("Enter the file to be write:");
    scanf("%s",filename_w);
    handlerEnglishToMorse(filename_r, filename_w);
}*/