#include <stdio.h>
#include <unistd.h>
#define FILELEN 40
#define BUFFERSIZE 128
int main(void)
{
	FILE* fp;
    int fd=-1;
    char filename[FILELEN];
    char buf[BUFFERSIZE];
    int n;
    int pianyi;

    
    /*用fopen打开文件体现不了偏移量
    fp=fopen(filename,"a+");   
    fd=fileno(fp);*/
    fp=fopen("file1.txt","r+");
fprintf(fp,"%d",3);
    pianyi=lseek(fd, 11, SEEK_SET);//三个都不行
    printf("偏移量：%d\n",pianyi);

    fprintf(fp,"%d",3);
}

//https://codetd.com/article/5640528
//https://blog.csdn.net/veghlreywg/article/details/103348856
附加文件访问下用不了fseek