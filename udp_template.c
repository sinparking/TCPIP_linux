#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#define BUF_SIZE 1024

void error(char *msg);

int main(int argc, char const *argv[])
{
    if (argc != 3)
    {
        printf("Usage: %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    int sock;
    char message[BUF_SIZE];
    int message_len;
    socklen_t addr_len;
    //! 对于clnt：serv_addr是目标地址 from_addr是当前发送位置
    struct sockaddr_in serv_addr, from_addr;

    sock = socket(PF_INET, SOCK_DGRAM, 0);
    if (sock == -1)
        error("socket error");
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));


    close(sock);
    return 0;
}

void error(char *msg)
{
    fputs(msg, stderr);
    fputc('\n', stderr);
    exit(1);
}