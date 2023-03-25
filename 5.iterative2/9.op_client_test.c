#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#define BUF_SIZE 1024
#define OPSZ 4
#define RSTSZ 4

/* //!自己的实现
    1.客户端连接服务器后以1字节整数形式传递等待计算的数字个数
    2.客户端想服务器传递的每个整数型数据占4字节
    3.传递整数型数据后传递运算符。运算符占1字节
    4.字符仅有+，-，*
    5.服务器以4字节整数型向客户端传回运算结果
    6.客户端得到运算结果之后终止服务器连接
*/

void error(char *message);

int main(int argc, char const *argv[])
{
    if (argc != 3)
        error("argc error");

    int sock;
    int oprand_cnt;
    int index;
    int operand;
    struct sockaddr_in addr;
    char message[BUF_SIZE];
    char operater;

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
        fputs("connecting....\n", stdout);

    fputs("enter operand count:", stdout);
    scanf("%d", &oprand_cnt);
    if (oprand_cnt < 2)
        error("operand nums error");
    sprintf(message, "%d", oprand_cnt);
    write(sock, message, 1);

    index = 0;
    for (int i = 0; i < oprand_cnt; i++)
    {
        printf("operand %d:", i + 1);
        scanf("%d", &operand);
        sprintf(&message[index], "%d", operand);
        write(sock, &message[index], 4);
        index += 4;
    }

    getchar(); //! 这里需要吸收一个回车
    printf("operator :");
    scanf("%c", &operater);
    message[index] = operater;
    write(sock, &message[index], 1);

    read(sock, message, sizeof(message));
    printf("result is %s\n",message);

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
