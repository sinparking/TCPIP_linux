#include <stdio.h>
#include <arpa/inet.h>

//! in_addr_t inet_addr(const char * string)

int main(int argc, char const *argv[])
{
    char *addr1 = "133.2.123.25";
    char *addr2 = "1.2.3.256";

    unsigned long conv_addr = inet_addr(addr1);
    if (conv_addr == INADDR_NONE)
        printf("Error!\n");
    else
    {
        printf("Network ordered integer addr: %#lx \n", conv_addr);
        printf("Network ordered integer addr: %d \n", conv_addr);
    }

    conv_addr = inet_addr(addr2);
    if (conv_addr == INADDR_NONE)
        printf("Error!\n");
    else
        printf("Network ordered integer addr: %#lx \n", conv_addr);
    return 0;
}
