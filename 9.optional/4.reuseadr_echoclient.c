#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>

#define BUF_SIZE 1024
void error(char *message);

int main(int argc, char const *argv[])
{
    int sock;
    struct sockaddr_in addr;
    char message[BUF_SIZE];
    int str_len;

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

    //! 一次只能处理一个客户端，一次连接中和客户端的连接在断开之前可以多次交互
    while (1)
    {
        fputs("Input message(Q/q to quit):", stdout);
        fgets(message, BUF_SIZE, stdin); // 先从标准输入读取选项
        if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
            break;
        // 向服务器发送数据
        write(sock, message, strlen(message));
        // echo服务器回写数据
        str_len = read(sock, message, BUF_SIZE - 1);
        // 给写回的数据封一下边
        message[str_len] = 0;
        printf("recall from echo: %s", message);
    }

    close(sock);
    return 0;
}

void error(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
