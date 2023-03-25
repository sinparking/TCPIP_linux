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

    //! һ��ֻ�ܴ���һ���ͻ��ˣ�һ�������кͿͻ��˵������ڶϿ�֮ǰ���Զ�ν���
    while (1)
    {
        fputs("Input message(Q/q to quit):", stdout);
        fgets(message, BUF_SIZE, stdin); // �ȴӱ�׼�����ȡѡ��
        if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
            break;
        // ���������������
        write(sock, message, strlen(message));
        // echo��������д����
        str_len = read(sock, message, BUF_SIZE - 1);
        // ��д�ص����ݷ�һ�±�
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
