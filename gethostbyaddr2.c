#include <stdio.h>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <arpa/inet.h>

int main(int argc, char *argv[])
{
        struct hostent *phost;
        struct sockaddr_in testaddr;
        inet_pton(AF_INET, "180.101.49.12", &(testaddr.sin_addr.s_addr));
//        htonl(testaddr.sin_addr.s_addr);
        if ((phost = gethostbyaddr((char *)&(testaddr.sin_addr.s_addr), 4, AF_INET)) == NULL)
                printf("gethostbyaddr() error\n%s\nh_errno=%d\n", hstrerror(h_errno), h_errno);
        else
                printf("official name = %s\n", phost->h_name);
        exit(0);
}

/*http://bbs.chinaunix.net/thread-1083939-1-1.html
[wqr@localhost book_network_program]$ ./hostaddr2
gethostbyaddr() error
Unknown host
h_errno=1
*/