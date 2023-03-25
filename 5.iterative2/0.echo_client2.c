#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#define BUF_SIZE 1024

void error(char *message);

int main(int argc, char const *argv[])
{
    if (argc != 3)
        error("argc error");

    int sock;
    struct sockaddr_in addr;
    char message[BUF_SIZE];
    int str_len;
    int recv_len;
    int recv_cnt;

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == -1)
        error("socket error");
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    addr.sin_port = htons(atoi(argv[2]));

    if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) == -1)
        error("bind connect");
    else
        fputs("connecting....", stdout);

    while (1)
    {
        fputs("input message(q/Q to quit):", stdout);
        fgets(message, BUF_SIZE, stdin);
        if (!strcmp("q\n", message) || !strcmp("Q/n", message))
            break;
        // 写入的数据量
        str_len = write(sock, message, strlen(message));
        recv_len = 0;
        while (recv_len < str_len)
        {
            //! 这种读取的方式必须保证message的大小比这次传输的文件要大，不然就写不进去了
            recv_cnt = read(sock, &message[recv_len], BUF_SIZE - 1);
            if (recv_cnt == -1)
                error("read error");
            recv_len += recv_cnt;
        }
        // message[recv_len] = 0;
        printf("echo from server:%s", message);
    }
    fputs("close....\n", stdout);

    close(sock);

    return 0;
}

void error(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(0);
}
