#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

void error_handling(char *message);

// todo 验证TPC套接字传输的数据不存在数据边界

int main(int argc, char const *argv[])
{
    int sock;
    struct sockaddr_in serv_addr;
    char message[30];
    int str_len = 0;
    int idx = 0, read_len = 0;

    if (argc != 3)
    {
        printf("Usage: %s <IP> <port>", argv[0]);
        exit(1);
    }
    // 前两个是PF_INET，SOCK_STREAM时，第三个参数为0，因为这两种方式已经确定了要么TCP要么UDP。除非前两个参数有变动
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (-1 == sock)
        error_handling("socket() error!");

    // memset相当于对结构体进行了初始化
    // 第一个是地址，第二个是设的内容，第三个是设的长度
    //! memset这一步主要是为了将serv_addr中的sin_zero初始化为0
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("connect() error!");

    while (read_len = read(sock, &message[idx++], 1))
    {
        if (read_len == -1)
            error_handling("read() error!");

        str_len += read_len;
    }

    printf("Message from server: %s \n", message);
    printf("Function read call count: %d \n", str_len);

    close(sock);
    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}