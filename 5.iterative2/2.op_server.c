#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#define BUF_SIZE 1024
#define OPSZ 4
#define RLT_SIZE 4

/*
todo 应用层协议
    1.客户端连接服务器后以 1字节 整数形式传递等待计算的数字个数
    2.客户端想服务器传递的每个整数型数据占4字节
    3.传递整数型数据后传递运算符。运算符占 1字节
    4.字符仅有+，-，*
    5.服务器以4字节整数型向客户端传回运算结果
    6.客户端得到运算结果之后终止服务器连接
*/

void error_handling(char *message);
int computing(const int *a, char operator, int num);

int main(int argc, char const *argv[])
{
    int serv_sock;
    int clnt_sock;

    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size;

    int result, opnd_cnt, i;
    int recv_cnt, recv_len;
    char opinfo[BUF_SIZE];

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
    for (i = 0; i < 5; i++)
    {
        opnd_cnt = 0;
        clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_size);
        if (clnt_sock == -1)
            error_handling("accept() error!");
        //! 1.直接往int的地址读
        read(clnt_sock, &opnd_cnt, 1);

        recv_len = 0;
        //! 2.收到的值小于传输的值就一直读
        while ((opnd_cnt * OPSZ + 1) > recv_len)
        {
            recv_cnt = read(clnt_sock, &opinfo[recv_len], BUF_SIZE - 1);
            recv_len += recv_cnt;
        }
        //! 3.字符数组直接强转为int数组
        result = computing((int *)opinfo, opinfo[recv_len - 1], opnd_cnt);

        //! 4.(char *)&result, result是一个int
        // 这样写也许顺眼一点？
        {
            int i = 10;
            //* 1.服务器以(char *)的方式发送int i的地址
            void *message = (char *)&i;
            //* 2.客户端以 int * 来接收这个值
            int *p = (int *)message;
            int j = *p;
        }
        write(clnt_sock, (char *)&result, sizeof(result));
        printf("result size :%d\n", sizeof(result));
        close(clnt_sock);
    }

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
    int ret = a[0], i;
    switch (operator)
    {
    case '+':
        for (i = 1; i < num; ++i)
            ret += a[i];
        break;
    case '-':
        for (i = 1; i < num; ++i)
            ret -= a[i];
        break;
    case '*':
        for (i = 1; i < num; ++i)
            ret *= a[i];
        break;
    default:
        break;
    }
    return ret;
}