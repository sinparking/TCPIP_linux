#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>

/* 同
    ! struct hostent * gethostbyaddr(const void * addr, socklen_t len, int type);  //IPv4=4, IPv6=16
    ! host->h_addr_list是二级指针，指向char*，此char*保存的是struct in_addr 的地址（实际上为了通用性也可以是IPV6地址）
*/

void error(char *message);

int main(int argc, char const *argv[])
{
    int i;
    struct hostent *host;
    struct sockaddr_in addr;
    if (argc != 2)
    {
        printf("Usage:%s <IP>\n", argv[0]);
        exit(1);
    }
    memset(&addr, 0, sizeof(addr));
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    printf("%d\n", inet_addr(argv[1]));
    host = gethostbyaddr((char *)&addr.sin_addr, 4, AF_INET);
    if (!host)
        error("gethost error");

    printf("Official name:%d:%s \n", i + 1, host->h_name);
    for (i = 0; host->h_aliases[i]; i++)
        printf("Aliases %d:%s \n", i + 1, host->h_aliases[i]);
    printf("Address type is: %s \n", (host->h_addrtype == AF_INET) ? "AF_INET" : "AF_INET6");

    // todo 这里为什么要 inet_ntoa(*(struct in_addr*)host->h_addr_list[i])
    for (i = 0; host->h_addr_list[i]; i++)
        printf("IP addr %d:%s(%d)\n", i + 1, inet_ntoa(*(struct in_addr *)host->h_addr_list[i]), (*(struct in_addr *)host->h_addr_list[i]).s_addr);
    return 0;
}

void error(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}