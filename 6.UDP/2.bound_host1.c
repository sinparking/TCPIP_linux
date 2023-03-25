#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#define BUF_SIZE 30

void error(char *msg);

int main(int argc, char const *argv[])
{
    int sock;
    char message[BUF_SIZE];
    struct sockaddr_in my_adr, you_adr;
    socklen_t adr_sz;
    int str_len, i;

    if (argc != 2)
    {
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    sock = socket(PF_INET, SOCK_DGRAM, 0);
    if (sock == -1)
        error("socket error");

    memset(&my_adr, 0, sizeof(my_adr));
    my_adr.sin_family = AF_INET;
    my_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    my_adr.sin_port = htons(atoi(argv[1]));

    if (bind(sock, (struct sockaddr *)&my_adr, sizeof(my_adr)) == -1)
        error("bind error");

    for (i = 0; i < 3; i++)
    {
        //? why sleep here?
        sleep(5);
        adr_sz = sizeof(you_adr);
        str_len = recvfrom(sock, message, BUF_SIZE, 0, (struct sockaddr *)&you_adr, &adr_sz);

        printf("Message %d:%s \n", i + 1, message);
    }
    close(sock);
    return 0;
}

void error(char *msg)
{
    fputs(msg, stderr);
    fputc('\n', stderr);
    exit(0);
}