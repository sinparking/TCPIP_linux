#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#define BUF_SIZE 1024

void error(char *msg);

/*
    UDP: SOCK_DGRAM
    UDP回声客户端 echo_client.c  socket->sendto/recvfrom
    TCP回声客户端                socket->connect->read/write
    ! 1 ssize_t recvfrom(int sock, void *buff, size_t nbytes, int flags, struct sockaddr * from, socklen_t *addrlen);
    ! 2 ssize_t sendto(int sock, void *buff, size_t nbytes, int flags, struct sockaddr *to, socklen_t addrlen); //第一sendto 的时候会动态绑定地址和端口
*/

/*
 *已连接的UDP套接字
 *用connect连接之后，sendto不会在传输数据完成之后删除UDP套接字中的注册目标信息 P112
 */

int main(int argc, char const *argv[])
{
    if (argc != 3)
    {
        printf("Usage: %s <IP> <port>\n", argv[0]);
        exit(0);
    }

    int sock;
    char message[BUF_SIZE];
    int message_len;
    socklen_t addr_len; //* 多余变量
    //! 对于clnt：serv_addr是目标地址 from_addr是当前包从哪来
    struct sockaddr_in serv_addr, from_addr; //* 不需要from_addr

    sock = socket(PF_INET, SOCK_DGRAM, 0);
    if (sock == -1)
        error("socket error");
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    // todo 简历已连接的UDP套接字
    connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));

    while (1)
    {
        fputs("enter message(q/Q to quit):", stdout);
        fgets(message, BUF_SIZE, stdin);
        if (!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
            break;
        message_len = strlen(message);
        /*
        sendto(sock, message, message_len, 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
        */
        write(sock, message, strlen(message));
        /*
        addr_len = sizeof(from_addr);
        message_len = recvfrom(sock, message, addr_len, 0, (struct sockaddr *)&from_addr, &addr_len);
        */
        message_len = read(sock, message, BUF_SIZE);
        printf("sendto:%s:%d\n", inet_ntoa(serv_addr.sin_addr), serv_addr.sin_port);
        printf("recvfrom:%s:%d\n", inet_ntoa(from_addr.sin_addr), from_addr.sin_port);

        message[message_len] = 0;
        printf("echo:%s", message);
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