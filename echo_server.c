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
        
        //======================================attention!====================
        //当clint调用close(sock)，会发出EOF，read返回值为0,跳出while
        while((str_len = read(clnt_sock,message,BUF_SIZE))!=0)
        {
            //message[str_len]=0;//必须有！！！！！！！！！！！！！！！！！！！！！！！！1
            //echo_clint.c 用的strlen，这边也得用strlen，不然循环无法继续下去。why？？？
            //原因：read函数实际读到的字节数可以少于要读的字节数，但是write返回值与最后一个参数大小不用就是出错，要返回-1
            printf("str_len:%d\n",str_len);
            //write(clnt_sock,message,strlen(message));
            write(clnt_sock,message,str_len);
        }
        
        close(clnt_sock);
    }
    //只能一个client Q之后，再进一个clint，并且在第五个clint退出后，服务器才能结束
    
    close(serv_sock);
    return 0;
}

void error_handling(char* message)
{
    fputs(message,stderr);
    fputs("\n",stderr);
    exit(1);
}

/*write回去前必须message[str_len]=0的原因
[wqr@localhost book_network_program]$ ./serv3 9190
Connected client 1
str_len:11
str_len:7
^C
[wqr@localhost book_network_program]$ ./clint3 127.0.0.1 9190
connecting...
Input message(Q to quit):
qwertyuiop
test1:11 test2:11
size:11  Message from server:qwertyuiop

Input message(Q to quit):
asdfgh
test1:7 test2:11                 //问题表现这里
size:11  Message from server:asdfgh
iop                              //serv3如果没有message[str_len]=0，在本例子中，上一次11个长的message，覆盖7个字符后，剩下4个字符还在
                                 //之所以会出现上一行的原因，是因为read的第二个参数是void* buf，是指针，不是char数组
Input message(Q to quit):        //fgets/fputs就没这问题，因为指明char*。echo_stdserv.c echo_stdclint.c去看一下
q

message[str_len]=0是解决方法一
!!!!!!!!!!!!解决方法二：不用加message[str_len]=0，write第三个参数变为str_len（read的返回值）
区别于：echo_clint.c  中int test1=write(sock,message,strlen(message));
可以用strlen(message)且message不用处理是因为：message由fgets来
*/
