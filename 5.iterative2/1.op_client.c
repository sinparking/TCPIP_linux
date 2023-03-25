#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
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
/*
? char占1字节，int占4字节 char1，以下为64位。具体见书P89
    short int2
    int4
    long int8
    float4
    double8
    long long8
    long double16
*/
/*
! 1.scanf("%d",(int *)&k);
! 2.char c = char(int);
*/

void error(char *message);

int main(int argc, char const *argv[])
{
    if (argc != 3)
        error("argc error");

    int sock;
    struct sockaddr_in addr;
    char opmsg[BUF_SIZE];
    int result, opnd_cnt, i;

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

    fputs("Operand count: ", stdout);
    scanf("%d", &opnd_cnt);
    //! 这里1 还能这样,我用的fprintf
    opmsg[0] = (char)opnd_cnt;
    for (i = 0; i < opnd_cnt; ++i)
    {
        printf("Operand %d: ", i + 1);
        //! 这里2 直接将int强制转换读到char
        scanf("%d", (int *)&opmsg[i * OPSZ + 1]);
    }
    //! 这是干什么的 吸收回车吗
    fgetc(stdin);
    fputs("Operator: ", stdout);
    scanf("%c", &opmsg[opnd_cnt * OPSZ + 1]);
    write(sock, opmsg, opnd_cnt * OPSZ + 2);
    read(sock, &result, RLT_SIZE);

    printf("Operation result:%d\n", result);
    close(sock);

    return 0;
}

void error(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(0);
}
