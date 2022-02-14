#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>//sockaddr_in
#include <stdlib.h>
#include <unistd.h>
#include <string.h>//memset
#include <errno.h>

#define BUF_SIZE 1024
void error_handling(char* message);

int main(int argc,char *argv[])
{
    int serv_sock;
    int clnt_sock;
    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size;
    char message[BUF_SIZE];
    int str_len;

    if(argc!=2)
    {
        printf("Usage:%s <port>\n",argv[0]);
        exit(1);
    }

    serv_sock = socket(PF_INET,SOCK_STREAM,0);//socket会少打两个字母导致错误的,因为变量名serv_sock
    if (serv_sock == -1)
    {
       printf("socket Error no.%d: %s\n", errno, strerror(errno));

    }

    memset(&serv_addr,0,sizeof(serv_addr));//注：memset可以用于结构体，不过这个结构体里每个元素的每一位不是1就算0
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock,(struct sockaddr*)&serv_addr,sizeof(serv_addr))==-1)
    {
        printf("connect Error no.%d: %s\n", errno, strerror(errno));
    }

    if(listen(serv_sock,5)==-1)
    {
        printf("listen Error no.%d: %s\n", errno, strerror(errno));
    }
    
    for(int i=0;i<5;i++)
    {
        clnt_addr_size=sizeof(clnt_addr);//下面取地址，所以被拆开来了
        clnt_sock = accept(serv_sock,(struct sockaddr*)&clnt_addr,&clnt_addr_size);
        if(clnt_sock==-1)
        {
           printf("accept Error no.%d: %s\n", errno, strerror(errno));
        }
        else
        {
            printf("Connected client %d\n",i+1);
        }
        
        FILE* readfp = fdopen(clnt_sock,"r");
        FILE* writefp = fdopen(clnt_sock,"w");
        while(!feof(readfp))
        {
            fgets(message,BUF_SIZE,readfp);
            fputs(message,writefp);
            //int return_value=fread(message,1,strlen(message),readfp);
            //printf("return_value:%d  strlen(message):%d   string:%s\n",return_value,strlen(message),message);
            //fwrite(message,1,strlen(message),writefp);
            //标准I/O函数为了提高性能，内部提供额外的缓冲。因此，若不凋用 fHush函数则无法保证立即将数据传输到客户端
            fflush(writefp);
        }
        
        fclose(readfp);
        fclose(writefp);
        //close(clnt_sock);
    }
    
    close(serv_sock);
    return 0;
}

/*https://stackoverflow.com/questions/8589425/how-does-fread-really-work  
There may or may not be any difference in performance. There is a difference in semantics.
fread(a, 1, 1000, stdin);attempts to read 1000 data elements, each of which is 1 byte long.
fread(a, 1000, 1, stdin);attempts to read 1 data element which is 1000 bytes long.
[返回值具体含义]They're different because fread() returns the number of data elements it was able to read, not the number of bytes.
[有读的大小限制]If it reaches end-of-file (or an error condition) before reading the full 1000 bytes, 
（接着上行）the first version has to indicate exactly how many bytes it read; the second just fails and returns 0.

为什么单传回车键会出问题
*/


