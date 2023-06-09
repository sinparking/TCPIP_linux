#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>

#define BUF_SIZE 1024
void error(char *message);

// todo 访问服务器之后接收文件到receive.dat,之后发送thank you!

//* shutdown(sock,type);
//* type = SHUT_RD, SHUT_WR, SHUT_RDWR 
//* windows = SD_RECEIVE, SD_SEND, SD_BOTH

// client
int main(int argc, char const *argv[])
{
    int sock;
    struct sockaddr_in addr;
    char message[BUF_SIZE];
    int str_len;

    FILE *fp;
    int read_cnt;

    if (argc != 3)
    {
        printf("Usage: %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (-1 == sock)
        error("sock error");

    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    addr.sin_port = htons(atoi(argv[2]));

    if (-1 == connect(sock, (struct sockaddr *)&addr, sizeof(addr)))
        error("connect error");
    else
        puts("connected.....");

    fp = fopen("receive.dat", "wb");
    if (fp == NULL)
        error("fopen error");

    while ((read_cnt = read(sock, message, BUF_SIZE)) != 0)
        fwrite((void *)message, 1, read_cnt, fp);

    puts("receive file data");
    write(sock, "thank you", 10);
    close(sock);
    return 0;
}

void error(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}