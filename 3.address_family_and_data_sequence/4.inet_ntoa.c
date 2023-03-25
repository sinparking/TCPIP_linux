#include <stdio.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>

//! char * inet_ntoa(struct in_addr adr);

int main(int argc, char const *argv[])
{\
\
    struct sockaddr_in addr1, addr2;
    char *str_ptr;
    char str_arr[20];

    addr1.sin_addr.s_addr = htonl(0x1020304);
    addr2.sin_addr.s_addr = htonl(0x1010101);

    str_ptr = inet_ntoa(addr1.sin_addr);
    strcpy(str_arr, str_ptr);
    printf("1.:%s \n", str_ptr);
    inet_ntoa(addr2.sin_addr); //* 此时返回的是同一块地址，因此后序str_ptr的值变了
    printf("2.:%s \n", str_ptr);
    printf("3.:%s \n", str_arr);

    return 0;
}
