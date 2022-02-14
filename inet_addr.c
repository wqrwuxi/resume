#include <stdio.h>
#include <arpa/inet.h>//inet_addr
#include <errno.h>
void main(int argc,char* argv[])
{
    char *addr1 = "211.214.107.99";
    struct sockaddr_in addr_inet;

    unsigned long conv_addr = inet_addr(addr1);
    if(conv_addr == INADDR_NONE)//两个等号，两天里几次了
    {
       printf("Error ccured!\n");
    }
    else
    {
        printf("Network ordered integer addr:%#lx \n",conv_addr);
    }

    if(!inet_aton(addr1,&addr_inet.sin_addr))
    {
        printf("inet_aton Error no.%d: %s\n", errno, strerror(errno));
    }
    else
    {
        printf("inet_aton result:%#x \n",addr_inet.sin_addr.s_addr);
    }
}

/*
https://www.jianshu.com/p/89cd35010120
d，lx，ld，，lu，这几个都是输出32位的
%#llx 带0x的64位16进制格式输出

printf("%x\n", 47);  //输出结果为:    2f
    printf("%X\n", 47);  //输出结果为:    2F
    printf("%#x\n", 47); //输出结果为:    0x2f
    printf("%#X\n",47); //输出结果为:    0X2F    %#X推荐使用
————————————————
版权声明：本文为CSDN博主「David 's blogs」的原创文章，遵循CC 4.0 BY-SA版权协议，转载请附上原文出处链接及本声明。
原文链接：https://blog.csdn.net/zDavid_2018/article/details/107563701
*/

/*
[wqr@localhost book_network_program]$ ./inet_addr
Network ordered integer addr:0x636bd6d3 
inet_aton result:0x636bd6d3 
转成二进制99.107.214.211
*/

/*https://www.cnblogs.com/52php/p/6114080.html
在网络传输中，TCP/IP协议在保存IP地址这个32位二进制数时，协议规定采用在低位存储地址中包含数据的高位字节的存储顺序，这种顺序格式就被称为网络字节顺序格式。
在实际网络传输时，数据按照每32位二进制数为一组进行传输，由于存储顺序的影响，实际的字节传输顺序是由高位字节到低位字节的传输顺序。
 * /

/*https://blog.csdn.net/Casuall/article/details/98481469
 数字前面是高位，地址后面是高位
 */

/*https://www.cnblogs.com/lesroad/p/9533272.html
1.把ip地址转化为用于网络传输的二进制数值
int inet_aton(const char *cp, struct in_addr *inp);
inet_aton() 转换网络主机地址ip(如192.168.1.10)为二进制数值，并存储在struct in_addr结构中。
即第二个参数*inp,函数返回非0表示cp主机有地有效，返回0表示主机地址无效。
（这个转换完后不能用于网络传输，还需要调用htons或htonl函数才能将主机字节顺序转化为网络字节顺序）

in_addr_t inet_addr(const char *cp);
inet_addr函数转换网络主机地址（如192.168.1.10)为网络字节序二进制值，如果参数char *cp无效，函数返回-1(INADDR_NONE),
这个函数在处理地址为255.255.255.255时也返回－1,255.255.255.255是一个有效的地址，不过inet_addr无法处理;
*/