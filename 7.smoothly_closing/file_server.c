#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/socket.h>
#define BUF_SIZE 1024

void error(char *message);
// todo 被访问后 将 file_server.c发送出去, 发送后即半关闭

int main(int argc, char const *argv[])
{
    struct sockaddr_in c_addr;
    struct sockaddr_in s_addr;
    socklen_t c_addr_len;
    int str_len;
    int clnt_sock;
    int serv_sock;

    FILE *fp;
    int read_cnt;

    char message[BUF_SIZE];

    if (argc != 2)
    {
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
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
    clnt_sock = accept(serv_sock, (struct sockaddr *)&c_addr, &c_addr_len);
    if (clnt_sock == -1)
        error("accpet error");

    fp = fopen("file_client.c", "rb");
    if (fp == NULL)
        error("fopen error");
    while ((read_cnt = fread((void *)message, 1, BUF_SIZE, fp)) != 0)
    {
        if (read_cnt != BUF_SIZE)
        {
            write(clnt_sock, message, read_cnt);
            break;
        }
        write(clnt_sock, message, BUF_SIZE);
    }

    shutdown(clnt_sock, SHUT_WR);
    read(clnt_sock, message, BUF_SIZE);
    printf("msg from clnt:%s\n", message);

    fclose(fp);
    close(clnt_sock);
    close(serv_sock);
    return 0;
}

void error(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}