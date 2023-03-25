
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

//!自己的实现

void error_handling(char *message);
int computing(const int *a, char operator, int num);

int main(int argc, char const *argv[])
{
    int serv_sock;
    int clnt_sock;

    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size;

    if (argc != 2)
    {
        printf("Usage: %s <IP> <PORT> \n", argv[0]);
        exit(1);
    }

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (serv_sock == -1)
        error_handling("socket() error!");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    if (-1 == bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)))
        error_handling("bind() error!");

    if (-1 == listen(serv_sock, 5))
        error_handling("listen() error!");

    clnt_addr_size = sizeof(clnt_addr);
    clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_size);
    if (clnt_sock == -1)
        error_handling("accept() error!");

    char message[200] = {0};
    int read_len = 0;
    int buffer[100];
    int operand_num = 0;
    char operator;
    int index = 0;
    while (1)
    {
        read_len = read(clnt_sock, message, sizeof(message));
        printf("%d\n", read_len);
        if (read_len <= 0)
            break;
        else if (read_len == 1 && operand_num == 0)
            operand_num = atoi(message);
        else if (read_len == 4)
            buffer[index++] = atoi(message);
        else
        {
            operator = message[0];
            break;
        }
    }
    printf("nums:%d\n", operand_num);
    for (int i = 0; i < operand_num; i++)
    {
        printf("operand %d = %d\n", i + 1, buffer[i]);
    }
    printf("operator:%c\n", operator);

    int result = computing(buffer, operator, operand_num);
    printf("result=%d\n", result);
    sprintf(message, "%d", result);
    write(clnt_sock, message, 4);

    close(clnt_sock);
    close(serv_sock);

    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}

int computing(const int *a, char operator, int num)
{
    int ret = 0;
    switch (operator)
    {
    case '+':
        for (int i = 0; i < num; ++i)
            ret += a[i];
        break;
    case '-':
        for (int i = 0; i < num; ++i)
            ret -= a[i];
        break;
    case '*':
        for (int i = 0; i < num; ++i)
            ret *= a[i];
        break;
    default:
        break;
    }
    return ret;
}