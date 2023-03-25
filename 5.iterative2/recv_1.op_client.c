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
    memset(&addr, 0,