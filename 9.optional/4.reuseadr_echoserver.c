#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>

#define BUF_SIZE 1024
void error(char *message);

//* 先断开连接的一方会经历time_wait（2msl）的状态，此时端口属于使用状态。如果此时服务器断开连接，在短时间内无法重用该同一个端口
//* SOL_SOCKET -> SO_REUSEADDR 改为true 则可以从用time_wait状态下的port

int main(int argc, char const *argv[])
{
    struct sockaddr_in c_addr;
    struct sockaddr_in s_addr;
    socklen_t c_addr_len;
    int str_len;

    int clnt_sock;
    int serv_sock;

    char message[BUF_SIZE];

    if (argc != 2)
    {
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);

    // todo 修改socket的选项
    int status;
    int optlen, option;
    option = true;
    optlen = sizeof(option);
    status = setsockopt(serv_sock, SOL_SOCKET, SO_REUSEADDR, (void *)&option, optlen);

    if (-1 == serv_sock)
        error("sock error");

    memset(&s_addr, 0, sizeof(s_addr));
    s_addr.sin_family = AF_INET;
    s_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    s_addr.sin_port = htons(atoi(argv[1]));
    if (-1 == bind(serv_sock, (struct sockaddr *)&s_addr, sizeof(s_addr)))
        error("bind error");
    if (-1 == listen(serv_sock, 5))
        error("listen error");

    /*
        这里的addrlen所指向的值，是必须初始化的，而且要初始化为一个大于等于实际获取socket的数据长度的值，而accept函数在执行后，会将实际值赋给addrlen所指向的值
    */
    c_addr_len = sizeof(c_addr);

    //! 服务器最多服务五次客户端，之后将关闭
    for (size_t i = 0; i < 5; i++)
    {
        clnt_sock = accept(serv_sock, (struct sockaddr *)&c_addr, &c_addr_len);
        if (-1 == clnt_sock)
            error("accept error");
        else
            printf("connected client %ld \n", i + 1);
        // 读多少写回多少
        while (0 != (str_len = read(clnt_sock, message, BUF_SIZE)))
            write(clnt_sock, message, str_len);

        close(clnt_sock);
    }

    close(serv_sock);
    return 0;
}

void error(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
