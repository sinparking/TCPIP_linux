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
todo Ӧ�ò�Э��
    1.�ͻ������ӷ��������� 1�ֽ� ������ʽ���ݵȴ���������ָ���
    2.�ͻ�������������ݵ�ÿ������������ռ4�ֽ�
    3.�������������ݺ󴫵�������������ռ 1�ֽ�
    4.�ַ�����+��-��*
    5.��������4�ֽ���������ͻ��˴���������
    6.�ͻ��˵õ�������֮����ֹ����������
*/
/*
? charռ1�ֽڣ�intռ4�ֽ� char1������Ϊ64λ���������P89
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
    //! ����1 ��������,���õ�fprintf
    opmsg[0] = (char)opnd_cnt;
    for (i = 0; i < opnd_cnt; ++i)
    {
        printf("Operand %d: ", i + 1);
        //! ����2 ֱ�ӽ�intǿ��ת������char
        scanf("%d", (int *)&opmsg[i * OPSZ + 1]);
    }
    //! ���Ǹ�ʲô�� ���ջس���
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
